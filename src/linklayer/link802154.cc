//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "link802154.h"
#include "frame802154_m.h"
#include "packet802154_m.h"
#include "wsnexception.h"
#include "channelutil.h"
#include "energy.h"

Define_Module(Link802154);

void Link802154::initialize()
{
    // Address is also module id of link module. This address is only available when initializing
    // stage of this module finishes.
    macAddress = this->getId();
}

void Link802154::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == strobeTimer) {
            startSending();
        } else if (msg == csmaTimer) {
            // Start CSMA transmission
            csmaTransmit();
        } else if (msg == releaseChannelTimer) {
            releaseChannel();
        } else if (msg == rxConsumeTimer) {
            // Calculate consumed energy for completed period
            double period = par("rxConsumingPeriod");
            useEnergyRx(period);
            // Schedule next period
            rxConsumeTimer->setTimestamp();
            scheduleAt(simTime() + period, rxConsumeTimer);
        } else if (msg == dcListenTimer) {
            // In radio duty cycling, turn on radio for short period
            setRadioMode(RADIO_ON, true);
            getParentModule()->bubble("In duty cycling: Radio ON");
        } else if (msg == dcSleepTimer) {
            // In radio duty cycling
            setRadioMode(RADIO_OFF, true);
            getParentModule()->bubble("In duty cycling: Radio OFF");
        }
    } else {
        if (msg->getArrivalGate() == gate("netGate$i")) {
            if (radioMode == RADIO_ON) {
                // Packet from upper layer, assemble frame and add to sending queue
                queueFrame(createFrame((Packet802154*) msg));
            } else {
                delete msg;
                EV<< "Error: Cannot send packet when radio is off\n";
            }
        } else if (msg->getArrivalGate() == gate("radioIn")) {
            if (radioMode == RADIO_ON) {
                // Frame from other node
                recvFrame((Frame802154*) msg);
            } else {
                // Just drop frame if radio is off
                delete msg;
            }
        }
    }
}

Link802154::Link802154()
{
    radioMode = RADIO_OFF;
    numAdjNode = 0;

    nStrobe = 0;
    strobeTimer = new cMessage("StrobeTimer");
    dcListenTimer = new cMessage("DcListenTimer");
    dcSleepTimer = new cMessage("DcSleepTimer");

    outFrame = NULL;
    txFrame = NULL;
    csmaTimer = new cMessage("CSMATimer");
    releaseChannelTimer = new cMessage("ReleaseChannelTimer");
    rxConsumeTimer = new cMessage("RxConsumeTimer");
}

Link802154::~Link802154()
{
    cancelAndDelete(rxConsumeTimer);
    cPacket *pkt;
    while (!outQueue.isEmpty()) {
        pkt = outQueue.pop();
        delete pkt;
    }

    cancelAndDelete(strobeTimer);
    cancelAndDelete(dcSleepTimer);
    cancelAndDelete(dcListenTimer);

    cancelAndDelete(csmaTimer);
    cancelAndDelete(releaseChannelTimer);
    if (txFrame != NULL) delete txFrame;
}

void Link802154::setRadioMode(int mode, bool dutyCycling)
{
    Enter_Method_Silent("setRadioMode");

    if (mode == RADIO_OFF || mode == RADIO_ON) {
        radioMode = mode;

        if (mode == RADIO_ON && !rxConsumeTimer->isScheduled()) {
            // Turn on transceiver and set power consuming timer for simulation
            rxConsumeTimer->setTimestamp();
            scheduleAt(simTime() + par("rxConsumingPeriod").doubleValue(), rxConsumeTimer);

            // If called by duty cycling, plan a sleep timer
            if (dutyCycling) scheduleAt(simTime() + par("lR").doubleValue(), dcSleepTimer);
        } else if (mode == RADIO_OFF && rxConsumeTimer->isScheduled()) {
            // Turn off transceiver and calculate consumed energy of last incomplete timer's period
            double onTime = SIMTIME_DBL(simTime() - rxConsumeTimer->getTimestamp());
            if (onTime > 0) {
                useEnergyRx(onTime);
            }
            cancelEvent(rxConsumeTimer);

            // Always start duty cycling when radio mode is off
            scheduleAt(simTime() + par("sR").doubleValue(), dcListenTimer);
        }
    }
}

/*
 * Get MAC address
 */
int Link802154::getAddr()
{
    return macAddress;
}

/*
 * Check if connection list is full or not.
 */
bool Link802154::isFullConn()
{
    return numAdjNode >= MAX_CONNECTIONS;
}

/*
 * Add a node to connection list.
 * Return: 0 on success, -1 when error
 */
int Link802154::addAdjNode(int addr)
{
    if (numAdjNode >= MAX_CONNECTIONS) return -1;

    adjNode[numAdjNode] = addr;
    numAdjNode++;
    return 0;
}

/*
 * Assemble a frame to encapsulate a packet.
 */
Frame802154* Link802154::createFrame(Packet802154* packet)
{
    // TODO Process long packet
    if (packet->getByteLength() > par("maxPacketSize").longValue()) throw PACKET_TOO_LONG;

    Frame802154 *frm = new Frame802154();
    frm->setSrcAddr(macAddress);
    if (packet->getTxType() == TX_BROADCAST) {
        // This packet is intended to be broadcasted
        frm->setDesAddr(BROADCAST_ADDR);
    } else {
        // Send to specific address
        frm->setDesAddr(packet->getDesMacAddr());
    }
    frm->setByteLength(
            par("fldFrameControl").longValue() + par("fldSequenceId").longValue() + par("fldDesAddr").longValue()
                    + par("fldSrcAddr").longValue() + par("fldFooter").longValue() + par("phyHeaderSize").longValue());
    frm->encapsulate(packet); // Frame length will be increased by length of packet

    return frm;
}

/*
 * Add Frame to sending queue and start a transmit timer.
 */
void Link802154::queueFrame(Frame802154 *frame)
{
    outQueue.insert(frame);
    prepareSending();
}

/* Wrapper for sendDirect() */
void Link802154::sendFrame(Frame802154 *frame, simtime_t propagationDelay, simtime_t duration, Link802154 *desNode,
        const char *inputGateName, int gateIndex)
{
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("Wsn.cu");
    if (cu->hasCollision(desNode)) {
        /* At this time, this node has acquired channel and collision by hidden node problem may occur.
         * In simulation, collision may be gone before this frame is sent completely;
         * therefore frame loss in this situation is simulated here. */
        desNode->getParentModule()->bubble("Lost frame by collision");
        EV << "Lost frame by collision";
        delete frame;
    } else {
        sendDirect(frame, propagationDelay, duration, desNode, inputGateName, gateIndex);
    }
}

/*
 * Receive frame from other node, forward to upper layer
 */
void Link802154::recvFrame(Frame802154* frame)
{
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("Wsn.cu");

    /* Frame loss when collision still occurs at time when the frame is received completely. */
    if (cu->hasCollision(this)) {
        getParentModule()->bubble("Lost frame by collision");
        EV << "Lost frame by collision";
        delete frame;
        return;
    }

    /* Random frame loss by environment */
    double rand = uniform(0, 1);
    if (rand < par("ranFrameLossProb").doubleValue()) {
        getParentModule()->bubble("Lost frame");
        EV << "Lost frame";
        delete frame;
        return;
    }

    if (frame->getType() == FR_PAYLOAD) {
        // Forward to upper layer
        send(frame->decapsulate(), "netGate$o");
    } else if (frame->getType() == FR_STROBE) {

    } else if (frame->getType() == FR_STROBE_ACK) {

    }
    delete frame;
}

/* =========================================================================
 * Duty cycling sending procedures
 * ========================================================================= */

void Link802154::prepareSending() {
    // Check if something being sent
    if (/*strobeTimer->isScheduled() ||*/ csmaTimer->isScheduled() || releaseChannelTimer->isScheduled()) {
        return;
    }

    if (!outQueue.isEmpty()) {
        // Prepare strobes
        nStrobe = (int) ceil(par("sR").doubleValue() / par("strobePeriod").doubleValue());
        EV << "Sending " << nStrobe << " strobes\n";
        startSending();
    }
}

void Link802154::startSending() {
    if (!outQueue.isEmpty()) {
        if (nStrobe > 0) {
            sendStrobe();
        } else {
            sendPayload();
        }
    }
}

void Link802154::sendStrobe() {
    if (!outQueue.isEmpty()) {
        EV<< "Sending strobe " << nStrobe << " \n";

        Frame802154 *payloadFrame = check_and_cast<Frame802154*>(outQueue.front());
        Frame802154 *strobe = new Frame802154();
        strobe->setType(FR_STROBE);
        strobe->setSrcAddr(macAddress);
        strobe->setDesAddr(payloadFrame->getDesAddr());
        strobe->setByteLength(
                par("fldFrameControl").longValue() + par("fldSequenceId").longValue() + par("fldDesAddr").longValue()
                + par("fldSrcAddr").longValue() + par("fldFooter").longValue() + par("phyHeaderSize").longValue());

        outFrame = strobe;
        csmaTransmit();
    }
}

void Link802154::sendPayload() {
    if (!outQueue.isEmpty()) {
        EV << "Sending payload\n";
        outFrame = check_and_cast<Frame802154*>(outQueue.pop());
        csmaTransmit();
    }
}

void Link802154::finishSending() {
    if (--nStrobe > 0) {
        // Set timer for sending next strobe
        scheduleAt(simTime() + par("strobePeriod").doubleValue(), strobeTimer);
    } else {
        // Send next payload
        prepareSending();
    }
}

/* =========================================================================
 * CSMA/CA sending procedures
 * ========================================================================= */

/*
 * Transmit a prepared frame using unslotted CSMA/CA.
 * This function is entry point of unslotted CSMA/CA algorithm.
 */
void Link802154::csmaTransmit()
{
    if (txFrame == NULL) {
        // Initialize when a frame is transmitted for first time
        if (outFrame == NULL) return; // Nothing to transmit
        txFrame = outFrame;
        outFrame = NULL;
        // Initialize first CSMA attempt
        NB = 0;
        BE = par("macMinBE").longValue();
    } else {
        // Re-attempt to transmit prepared frame
        NB++;
        if (BE < par("aMaxBE").longValue()) {
            BE++;
        } else {
            BE = par("aMaxBE").longValue();
        }
    }

    if (performCCA()) {
        transmit();
        txFrame = NULL;
    } else {
        if (NB < par("aMaxNB").longValue()) {
            backoff();
        } else {
            // TODO Transmission failure
            delete txFrame;
            txFrame = NULL;
            EV << "Link802154::csmaTransmit : Transmission failure\n";
        }
    }
}

/*
 * Perform a Clear Channel Assessment (CCA).
 * In simulation, this function will also acquire the channel if possible.
 * Return true if channel is idle, false if channel is busy or error.
 */
bool Link802154::performCCA()
{
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("Wsn.cu");
    int ret = cu->acquireChannel(this);
    EV << "Link802154::performCCA : result " << ret << '\n';
    return ret == 0;
}

/*
 * Release channel
 */
void Link802154::releaseChannel()
{
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("Wsn.cu");
    cu->releaseChannel(this);
    EV << "Link802154::releaseChannel\n";
    finishSending();
}

/*
 * Transmit prepared frame, set a timer to release channel when transmission completes.
 * If outgoing frame queue is not empty, set a timer for next CSMA transmission.
 */
void Link802154::transmit()
{
    double txDuration = 0;

    if (txFrame != NULL) {
        txDuration = ((double) txFrame->getBitLength()) / par("bitRate").doubleValue();
        int desAddr = txFrame->getDesAddr();
        Link802154 *des;

        // Draw energy for transmission
        useEnergyTx(txFrame->getBitLength());

        if (desAddr == BROADCAST_ADDR) {
            // Broadcast frame. In simulation, we send frame to all connected nodes
            Frame802154 *copy;
            for (int i = 0; i < numAdjNode; i++) {
                des = (Link802154*) simulation.getModule(adjNode[i]);
                if (des != NULL) {
                    copy = txFrame->dup();
                    sendFrame(copy, 0, txDuration, des, "radioIn");
                } else {
                    EV << "Link802154::transmitFrames : destination error, ID " << adjNode[i] << '\n';
                    delete copy;
                }
            }
            delete txFrame; // Original frame is redundant
        } else {
            // Transmit to specific destination
            des = (Link802154*) simulation.getModule(desAddr);
            if (des != NULL) {
                sendFrame(txFrame, 0, txDuration, des, "radioIn");
            } else {
                EV << "Link802154::transmitFrames : destination error, ID " << desAddr << '\n';
                delete txFrame;
            }
        }
    }

    // Set a timer to release channel
    scheduleAt(simTime() + txDuration, releaseChannelTimer);
}

/*
 * Back-off when channel is not idle.
 */
void Link802154::backoff()
{
    double backoffDur = intuniform(0, (int)(pow(2, BE) - 1)) * aUnitBP;
    scheduleAt(simTime() + backoffDur, csmaTimer);
    EV << "Link802154::backoff : duration " << backoffDur << '\n';
}

/*
 * Calculate and draw energy from energy module for transmitting.
 */
void Link802154::useEnergyTx(int nbits)
{
    double ce; // Consumed energy
    double d = par("txRange").doubleValue();
    double e_elect = par("e_elec").doubleValue();
    double e_fs = par("e_fs").doubleValue();

    if (d <= par("txRTh").doubleValue()) {
        ce = (e_elect + e_fs * d * d) * nbits;
    } else {
        ce = (e_elect + e_fs * d * d * d * d) * nbits;
    }

    Energy *energy = (Energy*) getParentModule()->getSubmodule("energy");
    energy->draw(ce);
    //EV << "Link802154::useEnergyTx : use " << ce << " J\n";
}

/*
 * Calculate and draw energy for listening
 * @param onTime Time interval that transceiver is turned on for listening signal
 */
void Link802154::useEnergyRx(double onTime)
{
    int bitRate = par("bitRate").longValue();
    double e_elect = par("e_elec").doubleValue();
    double ce = (double) bitRate * e_elect * onTime; // Consumed energy
    Energy *energy = (Energy*) getParentModule()->getSubmodule("energy");
    energy->draw(ce);
}
