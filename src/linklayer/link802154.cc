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
#include "app.h"
#include "statcollector.h"

Define_Module(Link802154);

void Link802154::initialize()
{
    // Address is also module id of link module. This address is only available when initializing
    // stage of this module finishes.
    macAddress = this->getId();
    WATCH(radioMode);
}

void Link802154::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == strobeTimer) {
            startSending();
        } else if (msg == backoffTimer) {
            // End of back-off period, perform CCa
            scheduleAt(simTime() + par("ccaDur").doubleValue(), ccaTimer); // Simulation
        } else if (msg == ccaTimer) {
            // Perform CCA, if channel is idle, acquire the channel
            performCCA();
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
            //getParentModule()->bubble("Radio ON");
        } else if (msg == dcSleepTimer) {
            // In radio duty cycling
            setRadioMode(RADIO_OFF, true);
            //getParentModule()->bubble("Radio OFF");
        }
    } else {
        if (msg->getArrivalGate() == gate("netGate$i")) {
            if (radioMode == RADIO_ON) {
                // Packet from upper layer, assemble frame and add to sending queue
                queueFrame(createFrame((Packet802154*) msg));
            } else {
                delete msg;
                EV << "Error: Cannot send packet when radio is off\n";
            }
        } else if (msg->getArrivalGate() == gate("radioIn")) {
            Frame802154 *frame = check_and_cast<Frame802154*>(msg);
            if (radioMode == RADIO_ON) {
                // Frame from other node
                recvFrame(frame);
            } else {
                // Just drop frame if radio is off
                //getParentModule()->bubble("Radio OFF");
                if (frame->getType() == FR_PAYLOAD) {
                    // Count lost payload frame
                    StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("Wsn.sc"));
                    sc->incLostPacket();
                }
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
    backoffTimer = new cMessage("BackoffTimer");
    ccaTimer = new cMessage("CCATimer");
    releaseChannelTimer = new cMessage("ReleaseChannelTimer");

    rxConsumeTimer = new cMessage("RxConsumeTimer");
}

Link802154::~Link802154()
{
    cPacket *pkt;
    while (!outQueue.isEmpty()) {
        pkt = outQueue.pop();
        delete pkt;
    }

    cancelAndDelete(strobeTimer);
    cancelAndDelete(dcSleepTimer);
    cancelAndDelete(dcListenTimer);

    cancelAndDelete(backoffTimer);
    cancelAndDelete(ccaTimer);
    cancelAndDelete(releaseChannelTimer);
    if (txFrame != NULL) delete txFrame;

    cancelAndDelete(rxConsumeTimer);
}

void Link802154::setRadioMode(int mode, bool dutyCycling)
{
    Enter_Method("setRadioMode");

    if (mode == RADIO_OFF || mode == RADIO_ON || mode == RADIO_FULL_OFF) {
        radioMode = mode;

        if (mode == RADIO_ON) {
            //EV << "Radio on\n";
            if (!rxConsumeTimer->isScheduled()) {
                // Turn on transceiver and set power consuming timer for simulation
                rxConsumeTimer->setTimestamp();
                scheduleAt(simTime() + par("rxConsumingPeriod").doubleValue(), rxConsumeTimer);
            }

            // If called by duty cycling, plan a sleep timer
            cancelEvent(dcSleepTimer);
            if (dutyCycling) scheduleAt(simTime() + par("lR").doubleValue(), dcSleepTimer);
        } else if (mode == RADIO_OFF) {
            // Clear pending transmissions
            nStrobe = 0;
            cPacket *pkt;
            while (!outQueue.isEmpty()) {
                pkt = outQueue.pop();
                delete pkt;
                getParentModule()->bubble("Frame discarded");
            }

            if (outFrame != NULL) {
                delete outFrame;
                outFrame = NULL;
                getParentModule()->bubble("Frame discarded");
            }
            if (txFrame != NULL) {
                delete txFrame;
                txFrame = NULL;
                getParentModule()->bubble("Frame discarded");
            }

            // Calculate consumed energy
            if (rxConsumeTimer->isScheduled()) {
                // Turn off transceiver and calculate consumed energy of last incomplete timer's period
                double onTime = SIMTIME_DBL(simTime() - rxConsumeTimer->getTimestamp());
                if (onTime > 0) {
                    useEnergyRx(onTime);
                }
                cancelEvent(rxConsumeTimer);
            }

            // Plan a listen timer
            if (par("enableXmac").boolValue()) {
                cancelEvent(dcListenTimer);
                scheduleAt(simTime() + par("sR").doubleValue(), dcListenTimer);
            }
        } else if (mode == RADIO_FULL_OFF) {
            EV << "Radio full off\n";
            if (rxConsumeTimer->isScheduled()) {
                // Turn off transceiver and calculate consumed energy of last incomplete timer's period
                double onTime = SIMTIME_DBL(simTime() - rxConsumeTimer->getTimestamp());
                if (onTime > 0) {
                    useEnergyRx(onTime);
                }
                cancelEvent(rxConsumeTimer);
            }
            poweroff();
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
    if (packet->getByteLength() > par("maxPacketSize").longValue()) {
        EV << "Link802154::createFrame: packet length " << packet->getByteLength() << '\n';
        throw PACKET_TOO_LONG;
    }

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
    StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("Wsn.sc"));
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("Wsn.cu");

    if (cu->hasCollision(desNode)) {
        /* At this time, this node has acquired channel and collision by hidden node problem may occur.
         * In simulation, collision may be gone before this frame is sent completely;
         * therefore frame loss in this situation is simulated here. */
        desNode->getParentModule()->bubble("collision");
        EV << "Link802154: Collision at destination";

        if (frame->getType() == FR_PAYLOAD) {
            // Count lost payload frame
            sc->incLostPacket();
        }
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
        getParentModule()->bubble("collision");
        EV << "Link802154: Lost frame by collision\n";

        if (frame->getType() == FR_PAYLOAD) {
            // Count lost payload frame
            StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("Wsn.sc"));
            sc->incLostPacket();
        }
        delete frame;
        return;
    }

    /* Random frame loss by environment */
    double rand = uniform(0, 1);
    if (rand < par("ranFrameLossProb").doubleValue()) {
        getParentModule()->bubble("Lost frame");
        EV << "Link802154: Lost frame\n";

        if (frame->getType() == FR_PAYLOAD) {
            // Count lost payload frame
            StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("Wsn.sc"));
            sc->incLostPacket();
        }
        delete frame;
        return;
    }

    if (frame->getType() == FR_PAYLOAD) {
        // Forward to upper layer
        send(frame->decapsulate(), "netGate$o");
        delete frame;
    } else if (frame->getType() == FR_STROBE) {
        getParentModule()->bubble("Get strobe");
        if (frame->getDesAddr() == macAddress) {
            App *app = check_and_cast<App*>(getParentModule()->getSubmodule("app"));
            app->notifyEvent();
            sendStrobeAck(frame);
        } else if (frame->getDesAddr() == BROADCAST_ADDR) {
            App *app = check_and_cast<App*>(getParentModule()->getSubmodule("app"));
            app->notifyEvent();
        }
        delete frame;
    } else if (frame->getType() == FR_STROBE_ACK) {
        //EV << "Link802154: Receive strobe ACK\n";
        getParentModule()->bubble("Receive strobe ACK");
        nStrobe = 0;
        // Cancel current strobe sending (if any)
        cancelEvent(strobeTimer);
        if (outFrame != NULL && outFrame->getType() == FR_STROBE) {
            delete outFrame;
            outFrame = NULL;
        }
        if (txFrame != NULL && txFrame->getType() == FR_STROBE) {
            cancelEvent(backoffTimer);
            cancelEvent(ccaTimer);
            delete txFrame;
            txFrame = NULL;
        }
        startSending();
        delete frame;
    }
}

/* =========================================================================
 * Duty cycling sending procedures
 * ========================================================================= */

/*
 * Prepare for sending frame at head of the queue if nothing being sent.
 */
void Link802154::prepareSending()
{
    // Check if something being sent
    if (strobeTimer->isScheduled() || backoffTimer->isScheduled() || ccaTimer->isScheduled()
            || releaseChannelTimer->isScheduled()) {
        return;
    }

    if (!outQueue.isEmpty()) {
        Frame802154 *frame = check_and_cast<Frame802154*>(outQueue.front());
        Packet802154 *pkt = (Packet802154*) frame->getEncapsulatedPacket();
        if (par("enableXmac").boolValue()
                && frame->getType() == FR_PAYLOAD
                && pkt != NULL && pkt->getStrobeFlag()) {
            // Prepare strobes
            nStrobe = (int) ceil(par("sR").doubleValue() / par("strobePeriod").doubleValue());
            EV<< "Link802154: Sending " << nStrobe << " strobes\n";
        } else {
            nStrobe = 0;
        }
        startSending();
    }
}

/*
 * Start sending frame in queue (payload) or strobe.
 */
void Link802154::startSending()
{
    // Check if something being sent
    if (strobeTimer->isScheduled() || backoffTimer->isScheduled() || ccaTimer->isScheduled()
            || releaseChannelTimer->isScheduled()) {
        return;
    }

    if (!outQueue.isEmpty()) {
        if (nStrobe > 0) {
            sendStrobe();
        } else {
            sendPayload();
        }
    }
}

void Link802154::sendStrobe()
{
    if (!outQueue.isEmpty()) {
        EV<< "Link802154: Sending strobe " << nStrobe << " \n";

        Frame802154 *payloadFrame = check_and_cast<Frame802154*>(outQueue.front());
        Frame802154 *strobe = new Frame802154();
        strobe->setType(FR_STROBE);
        strobe->setSrcAddr(macAddress);
        strobe->setDesAddr(payloadFrame->getDesAddr());
        strobe->setByteLength(
                par("fldFrameControl").longValue() + par("fldSequenceId").longValue() + par("fldDesAddr").longValue()
                + par("fldSrcAddr").longValue() + par("fldFooter").longValue() + par("phyHeaderSize").longValue());

        outFrame = strobe;
        startCsma();
    }
}

void Link802154::sendPayload()
{
    if (!outQueue.isEmpty()) {
        //EV << "Link802154: Sending payload\n";
        //getParentModule()->bubble("Sending payload");
        outFrame = check_and_cast<Frame802154*>(outQueue.pop());
        startCsma();
    }
}

/*
 * Queue a strobe ACK to send back to the node that sent strobes.
 */
void Link802154::sendStrobeAck(Frame802154 *strobe)
{
    if (strobe->getDesAddr() == macAddress || strobe->getDesAddr() == BROADCAST_ADDR) {
        EV<< "Link802154: Sending strobe ack\n";
        Frame802154 *ack = new Frame802154();
        ack->setType(FR_STROBE_ACK);
        ack->setSrcAddr(macAddress);
        ack->setDesAddr(strobe->getSrcAddr());
        ack->setByteLength(
                par("fldFrameControl").longValue() + par("fldSequenceId").longValue() + par("fldDesAddr").longValue()
                + par("fldSrcAddr").longValue() + par("fldFooter").longValue() + par("phyHeaderSize").longValue());

        // Add ACK to sending queue
        queueFrame(ack);
    }
}

/*
 * Called when finish sending a frame. Determine to send next strobe (or payload) or start over the
 * sending process (so that next frame in queue can be sent).
 */
void Link802154::finishSending()
{
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
 * Prepare transmitted frame and initialize CSMA/CA.
 * This function is entry point of unslotted CSMA/CA algorithm.
 */
void Link802154::startCsma()
{
    if (txFrame == NULL) {
        // Initialize when a frame is transmitted for first time
        if (outFrame == NULL) return; // Nothing to transmit
        txFrame = outFrame;
        outFrame = NULL;
        // Initialize first CSMA attempt
        NB = 0;
        BE = par("macMinBE").longValue();
    }

    backoff();
}

/*
 * Back-off
 */
void Link802154::backoff()
{
    double backoffDur = intuniform(0, (int)(pow(2, BE) - 1)) * par("aUnitBP").doubleValue();
    scheduleAt(simTime() + backoffDur, backoffTimer);
    //EV << "Link802154::backoff : duration " << backoffDur << '\n';
}

/*
 * Perform a Clear Channel Assessment (CCA).
 * In simulation, this function will also acquire the channel if possible.
 * Return true if channel is idle, false if channel is busy or error.
 */
void Link802154::performCCA()
{
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("Wsn.cu");

    if (cu->acquireChannel(this) == 0) {
        // Acquire channel successfully (channel is idle)
        transmit();
        txFrame = NULL;
    } else {
        // Channel is busy
        int maxBE = par("aMaxBE").longValue();

        if (++BE > maxBE) BE = maxBE;
        NB++;
        if (NB <= par("macMaxNB").longValue()) {
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
        bool enoughEnergy = useEnergyTx(txFrame->getBitLength());
        if (!enoughEnergy) {
            getParentModule()->bubble("Cannot send frame. Out of energy!");
            delete txFrame;
            txFrame = NULL;
            releaseChannel();
            return;
        }

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
                    //delete copy;
                }
            }
            delete txFrame; // Original frame is redundant
        } else {
            // Transmit to specific destination
            des = (Link802154*) simulation.getModule(desAddr);
            if (des != NULL) {
                sendFrame(txFrame, 0, txDuration, des, "radioIn");
            } else {
                std::cerr << "Link802154::transmit : destination error, ID " << desAddr << '\n';
                std::cerr << "Link802154::transmit: " << txFrame->getType() << '\n';
                delete txFrame;
            }
        }
    }

    // Set a timer to release channel
    scheduleAt(simTime() + txDuration, releaseChannelTimer);
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

/* =========================================================================
 * Consume energy
 * ========================================================================= */

/*
 * Calculate and draw energy from energy module for transmitting.
 * Return true when have enough energy, false when short of energy
 */
bool Link802154::useEnergyTx(int nbits)
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
    double e = energy->draw(ce);
    //EV << "Link802154::useEnergyTx : use " << e << " J\n";

    if (e < ce) return false;
    else return true;
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

/* Stop all timer and clean up memory when run out of energy */
void Link802154::poweroff()
{
    cPacket *pkt;

    nStrobe = 0;
    while (!outQueue.isEmpty()) {
        pkt = outQueue.pop();
        delete pkt;
    }

    if (outFrame != NULL) {
        delete outFrame;
        outFrame = NULL;
    }
    if (txFrame != NULL) {
        delete txFrame;
        txFrame = NULL;
    }

    cancelEvent(strobeTimer);
    cancelEvent(dcSleepTimer);
    cancelEvent(dcListenTimer);

    cancelEvent(backoffTimer);
    cancelEvent(ccaTimer);
    cancelEvent(releaseChannelTimer);

    cancelEvent(rxConsumeTimer);
}
