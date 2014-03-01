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
#include "packetcr_m.h"
#include "wsnexception.h"
#include "channelutil.h"
#include "energy.h"
#include "app.h"
#include "statcollector.h"

Define_Module(Link802154);

/* =========================================================================
 * Protected functions
 * ========================================================================= */
void Link802154::initialize()
{
    // Address is also module id of link module. This address is only available when initializing
    // stage of this module finishes.
    macAddress = this->getId();

    // Turn radio on
    setRadioMode(RADIO_ON);
    WATCH(radioMode);
}

void Link802154::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == backoffTimer) {
            // End of back-off period, perform CCA
            scheduleAt(simTime() + par("ccaDur").doubleValue(), ccaTimer); // Simulate CCA
        } else if (msg == ccaTimer) {
            // Perform CCA, if channel is idle, acquire the channel
            performCCA();
        } else if (msg == releaseChannelTimer) {
            releaseChannel();
            // TODO LIFS
            startSending(); // Send next frame in queue
        } else if (msg == rxConsumeTimer) {
            // Calculate consumed energy for completed period
            double period = par("rxConsumingPeriod");
            useEnergyRx(period);
            // Schedule next period
            rxConsumeTimer->setTimestamp();
            scheduleAt(simTime() + period, rxConsumeTimer);
        }
    } else {
        if (msg->getArrivalGate() == gate("netGate$i")) {
            recvPacket(check_and_cast<Packet802154*>(msg));
        } else if (msg->getArrivalGate() == gate("radioIn")) {
            Frame802154 *frame = check_and_cast<Frame802154*>(msg);
            recvFrame(frame);
        }
    }
}

/* Set radio mode. Support: RADIO_FULL_OFF, RADIO_ON */
void Link802154::setRadioMode(int mode)
{
    if (mode == RADIO_FULL_OFF || mode == RADIO_ON) {
        radioMode = mode;

        if (radioMode == RADIO_FULL_OFF) {
            if (rxConsumeTimer->isScheduled()) {
                // Turn off transceiver and calculate consumed energy of last incomplete timer's period
                double onTime = SIMTIME_DBL(simTime() - rxConsumeTimer->getTimestamp());
                if (onTime > 0) {
                    useEnergyRx(onTime);
                }
                cancelEvent(rxConsumeTimer);
            }
            poweroff();
        } else if (radioMode == RADIO_ON) {
            //EV << "Radio on\n";
            if (!rxConsumeTimer->isScheduled()) {
                // Turn on transceiver and set power consuming timer for simulation
                rxConsumeTimer->setTimestamp();
                scheduleAt(simTime() + par("rxConsumingPeriod").doubleValue(), rxConsumeTimer);
            }
        }
    }
    updateDisplay();
}

/*
 * Stop all timer and clean up memory when run out of energy.
 * Should only be called by setRadioMode().
 */
void Link802154::poweroff()
{
    cPacket *pkt;
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

    radioMode = RADIO_FULL_OFF;

    cancelEvent(backoffTimer);
    cancelEvent(ccaTimer);
    cancelEvent(releaseChannelTimer);

    cancelEvent(rxConsumeTimer);
}

void Link802154::updateDisplay()
{
    cDisplayString &ds = getParentModule()->getDisplayString();

    // Set color according to radio mode
    switch (radioMode) {
        case RADIO_FULL_OFF:
            ds.setTagArg("i", 1, "black");
            break;
        case RADIO_ON:
            ds.setTagArg("i", 1, "green");
            break;
    }
}

/* Receive packet from network layer */
void Link802154::recvPacket(Packet802154 *pkt)
{
    if (radioMode != RADIO_FULL_OFF) {
        // RADIO_ON
        // Packet from upper layer, assemble frame and add to sending queue
        queueFrame(createFrame(pkt));
        startSending();
    } else {
        StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("sc"));

        // Count lost payload frame
        sc->incLostFrame();
        sc->incLostPacket();

        // Count number of lost payload to BS by link layer
        PacketCR *p = check_and_cast<PacketCR*>(pkt);
        if (p->getPkType() == PK_PAYLOAD_TO_BS) {
            sc->incLostMTRbyLink();
        }
        delete pkt;
    }
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
    startSending();
}

/* Wrapper for sendDirect() */
void Link802154::sendFrame(Frame802154 *frame, simtime_t propagationDelay, simtime_t duration, Link802154 *desNode,
        const char *inputGateName, int gateIndex)
{
    StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("sc"));
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("cu");

    if (cu->hasCollision(desNode)) {
        /* At this time, this node has acquired channel and collision by hidden node problem may occur.
         * In simulation, collision may be gone before this frame is sent completely;
         * therefore frame loss in this situation is simulated here. */
        desNode->getParentModule()->bubble("collision");
        EV << "Link802154: Collision at destination";

        if (frame->getType() == FR_PAYLOAD) {
            // Count lost payload frame
            sc->incLostFrame();
            sc->incLostPacket();

            // Count number of lost payload to BS by link layer
            PacketCR *pkt = check_and_cast<PacketCR*>(frame->decapsulate());
            if (pkt->getPkType() == PK_PAYLOAD_TO_BS) {
                sc->incLostMTRbyLink();
            }
            delete pkt;
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
    if (simInFrameLoss(frame)) return; // The frame is lost

    if (frame->getType() == FR_PAYLOAD) {
        // Forward to upper layer
        send(frame->decapsulate(), "netGate$o");

        // Count received frame
        StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("sc"));
        sc->incRecvFrame();
        delete frame;
    } else {
        EV << "Unknown frame\n";
        delete frame;
    }
}

/* Simulate incoming frame loss.
 * If the frame is lost, its pointer should not be used anymore.
 * Return true when the frame is lost, false otherwise. */
bool Link802154::simInFrameLoss(Frame802154* frame)
{
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("cu");
    StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("sc"));

    /* Frame loss because radio is off */
    if (radioMode != RADIO_ON) {
        getParentModule()->bubble("radio off");
        // Just drop frame if radio is off
        if (frame->getType() == FR_PAYLOAD) {
            // Count lost payload frame
            sc->incLostFrame();
            sc->incLostPacket();

            // Count number of lost payload to BS by link layer
            PacketCR *pkt = check_and_cast<PacketCR*>(frame->decapsulate());
            if (pkt->getPkType() == PK_PAYLOAD_TO_BS) {
                sc->incLostMTRbyLink();
            }
            delete pkt;
        }
        delete frame;
        return true;
    }

    /* Frame loss when collision still occurs at time when the frame is received completely. */
    if (cu->hasCollision(this)) {
        getParentModule()->bubble("collision");
        //EV << "Link802154: Lost frame by collision\n";

        if (frame->getType() == FR_PAYLOAD) {
            // Count lost payload frame
            sc->incLostFrame();
            sc->incLostPacket();

            // Count number of lost payload to BS by link layer
            PacketCR *pkt = check_and_cast<PacketCR*>(frame->decapsulate());
            if (pkt->getPkType() == PK_PAYLOAD_TO_BS) {
                sc->incLostMTRbyLink();
            }
            delete pkt;
        }
        delete frame;
        return true;
    }

    /* Random frame loss by environment */
    double rand = uniform(0, 1);
    if (rand < par("ranFrameLossProb").doubleValue()) {
        getParentModule()->bubble("Lost frame");
        EV << "Link802154: Lost frame\n";

        if (frame->getType() == FR_PAYLOAD) {
            // Count lost payload frame
            sc->incLostFrame();
            sc->incLostPacket();

            // Count number of lost payload to BS by link layer
            PacketCR *pkt = check_and_cast<PacketCR*>(frame->decapsulate());
            if (pkt->getPkType() == PK_PAYLOAD_TO_BS) {
                sc->incLostMTRbyLink();
            }
            delete pkt;
        }
        delete frame;
        return true;
    }

    // The frame is not lost
    return false;
}

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

/*
 * Start sending frame in head of outQueue
 */
void Link802154::startSending()
{
    // Check if something being sent
    if (backoffTimer->isScheduled()
            || ccaTimer->isScheduled()
            || releaseChannelTimer->isScheduled()) {
        return;
    }

    if (!outQueue.isEmpty()) {
        outFrame = check_and_cast<Frame802154*>(outQueue.pop());
        startCsma();
    }
}

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
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("cu");

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
            StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("sc"));

            // Count lost payload frame
            sc->incLostFrame();
            sc->incLostPacket();

            // Count number of lost payload to BS by link layer
            cPacket *pkt = txFrame->decapsulate();
            if (pkt != NULL) {
                if (((PacketCR*) pkt)->getPkType() == PK_PAYLOAD_TO_BS) {
                    sc->incLostMTRbyLink();
                }
                delete pkt;
            }

            delete txFrame;
            txFrame = NULL;
            // EV << "Link802154::csmaTransmit : Transmission failure\n";
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
            setRadioMode(RADIO_FULL_OFF);
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
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("cu");
    cu->releaseChannel(this);
    EV << "Link802154::releaseChannel\n";
}

/* =========================================================================
 * Public functions
 * ========================================================================= */
Link802154::Link802154()
{
    radioMode = RADIO_ON;
    numAdjNode = 0;

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

    cancelAndDelete(backoffTimer);
    cancelAndDelete(ccaTimer);
    cancelAndDelete(releaseChannelTimer);
    if (txFrame != NULL) delete txFrame;

    cancelAndDelete(rxConsumeTimer);
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
