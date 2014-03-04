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

#include "linkxtmac.h"
#include "packetcr_m.h"
#include "app.h"
#include "channelutil.h"
#include "statcollector.h"

Define_Module(LinkXTMAC);

/* =========================================================================
 * Private functions
 * ========================================================================= */

/* Stay active for a short time */
void LinkXTMAC::setActive()
{
    if (radioMode == RADIO_OFF) {
        // Turn radio on
        setRadioMode(RADIO_ON);
    }

    isActive = true;
    if (!forcedOn) {
        // If not in forced on interval then set dcSleepTimer as normal. If in forced on interval,
        // dcSleepTimer is already set and we will not change it.
        cancelEvent(dcSleepTimer);
        scheduleAt(simTime() + par("activeTime"), dcSleepTimer);
    }

    updateDisplay();
}

/* =========================================================================
 * Protected functions
 * ========================================================================= */

void LinkXTMAC::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == strobeTimer) {
            prepareStrobe();
            startSending();
        } else if (msg == backoffTimer) {
            // End of back-off period, perform CCa
            scheduleAt(simTime() + par("ccaDur").doubleValue(), ccaTimer); // Simulation
        } else if (msg == ccaTimer) {
            // Perform CCA, if channel is idle, acquire the channel
            performCCA();
        } else if (msg == releaseChannelTimer) {
            releaseChannel();
            finishSending();
        } else if (msg == rxConsumeTimer) {
            // Calculate consumed energy for completed period
            double period = par("rxConsumingPeriod");
            useEnergyRx(period);
            // Schedule next period
            rxConsumeTimer->setTimestamp();
            scheduleAt(simTime() + period, rxConsumeTimer);
        } else if (msg == dcListenTimer) {
            // In radio duty cycling, turn on radio for short period
            setRadioMode(RADIO_ON);
            //getParentModule()->bubble("Radio ON");
        } else if (msg == dcSleepTimer) {
            // In radio duty cycling
            if (isActive) {
                isActive = false;
                forcedOn = false;
                updateDisplay();
            }
            setRadioMode(RADIO_OFF);
            //getParentModule()->bubble("Radio OFF");
        }
    } else {
        if (msg->getArrivalGate() == gate("netGate$i")) {
            if (radioMode == RADIO_FULL_OFF) {
                delete msg;
                EV << "Error: Out of energy\n";
            } else {
                setActive();
                // Packet from upper layer, assemble frame and add to sending queue
                queueFrame(createFrame((Packet802154*) msg));
            }
        } else if (msg->getArrivalGate() == gate("radioIn")) {
            Frame802154 *frame = check_and_cast<Frame802154*>(msg);
            recvFrame(frame);
        }
    }
}

void LinkXTMAC::setRadioMode(int mode)
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

            cancelEvent(dcSleepTimer);
            cancelEvent(dcListenTimer);
            // If called by duty cycling, plan a sleep timer
            scheduleAt(simTime() + par("lR").doubleValue(), dcSleepTimer);
        } else if (mode == RADIO_OFF) {
            cancelEvent(strobeTimer);
            cancelEvent(dcSleepTimer);
            cancelEvent(dcListenTimer);

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
            cancelEvent(dcListenTimer);
            scheduleAt(simTime() + par("sR").doubleValue(), dcListenTimer);
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

void LinkXTMAC::poweroff()
{
    nStrobe = 0;
    cancelEvent(strobeTimer);
    cancelEvent(dcSleepTimer);
    cancelEvent(dcListenTimer);

    Link802154::poweroff();

    updateDisplay();
}

void LinkXTMAC::updateDisplay()
{
    cDisplayString &ds = getParentModule()->getDisplayString();

    // Set color according to radio mode
    switch (radioMode) {
        case RADIO_FULL_OFF:
            ds.setTagArg("i", 1, "black");
            break;
        case RADIO_ON:
            if (isActive) {
                ds.setTagArg("i", 1, "green");
            } else {
                ds.setTagArg("i", 1, "yellow");
            }
            break;
    }
}

/*
 * Add Frame to sending queue and start a transmit timer.
 */
void LinkXTMAC::queueFrame(Frame802154 *frame)
{
    outQueue.insert(frame);
    prepareSending();
}

/*
 * Receive frame from other node, forward to upper layer
 */
void LinkXTMAC::recvFrame(Frame802154* frame)
{
    if (simInFrameLoss(frame)) return; // The frame is lost
    StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("sc"));

    if (frame->getType() == FR_PAYLOAD) {
        setActive(); // Change to active state
        // Forward to upper layer
        send(frame->decapsulate(), "netGate$o");
        // Count received frame
        sc->incRecvFrame();
        delete frame;
    } else if (frame->getType() == FR_PREAMBLE) {
        getParentModule()->bubble("Get strobe");
        if (frame->getDesAddr() == macAddress) {
            setActive(); // Change to active state
            sendStrobeAck(frame);
        } else if (frame->getDesAddr() == BROADCAST_ADDR) {
            setActive(); // Change to active state
        }
        delete frame;
    } else if (frame->getType() == FR_PREAMBLE_ACK) {
        setActive(); // Change to active state

        //EV << "Link802154: Receive strobe ACK\n";
        getParentModule()->bubble("Receive strobe ACK");
        nStrobe = 0;
        // Cancel current strobe sending (if any)
        cancelEvent(strobeTimer);
        if (outFrame != NULL && outFrame->getType() == FR_PREAMBLE) {
            delete outFrame;
            outFrame = NULL;
        }
        if (txFrame != NULL && txFrame->getType() == FR_PREAMBLE) {
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
void LinkXTMAC::prepareSending()
{
    // Check if something being sent
    if (strobeTimer->isScheduled() || backoffTimer->isScheduled() || ccaTimer->isScheduled()
            || releaseChannelTimer->isScheduled()) {
        return;
    }

    if (!outQueue.isEmpty()) {
        Frame802154 *frame = check_and_cast<Frame802154*>(outQueue.front());
        Packet802154 *pkt = (Packet802154*) frame->getEncapsulatedPacket();
        if (frame->getType() == FR_PAYLOAD
                && pkt != NULL && pkt->getPreambleFlag()) {
            // Prepare strobes
            nStrobe = (int) ceil(par("sR").doubleValue() / par("strobePeriod").doubleValue());
            EV << "Link802154: Sending " << nStrobe << " strobes\n";
            prepareStrobe();
        } else {
            nStrobe = 0;
        }
        startSending();
    }
}

/*
 * Prepare strobe frame (if necessary) and push it to sending queue.
 */
void LinkXTMAC::prepareStrobe()
{
    if (nStrobe < 0) return;
    if (outQueue.isEmpty()) return;

    Frame802154 *payload = check_and_cast<Frame802154*>(outQueue.front());
    if (payload->getType() != FR_PAYLOAD) return;

    Frame802154 *strobe = new Frame802154();
    strobe->setType(FR_PREAMBLE);
    strobe->setSrcAddr(macAddress);
    strobe->setDesAddr(payload->getDesAddr());
    strobe->setByteLength(
            par("fldFrameControl").longValue() + par("fldSequenceId").longValue() + par("fldDesAddr").longValue()
                    + par("fldSrcAddr").longValue() + par("fldFooter").longValue() + par("phyHeaderSize").longValue());

    outQueue.insertBefore(payload, strobe);
}

/*
 * Queue a strobe ACK to send back to the node that sent strobes.
 */
void LinkXTMAC::sendStrobeAck(Frame802154 *strobe)
{
    if (strobe->getDesAddr() == macAddress || strobe->getDesAddr() == BROADCAST_ADDR) {
        EV<< "Link802154: Sending strobe ack\n";
        Frame802154 *ack = new Frame802154();
        ack->setType(FR_PREAMBLE_ACK);
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
void LinkXTMAC::finishSending()
{
    if (nStrobe > 0) {
        nStrobe--;
        if (nStrobe > 0) {
            // Set timer for sending next strobe
            scheduleAt(simTime() + par("strobePeriod").doubleValue(), strobeTimer);
        } else {
            // Last strobe is sent, send payload whatever, immediately
            startSending();
        }
    } else {
        // Send next payload
        prepareSending();
    }
}

/* =========================================================================
 * Public functions
 * ========================================================================= */

LinkXTMAC::LinkXTMAC()
{
    isActive = true;
    forcedOn = false;
    nStrobe = 0;
    strobeTimer = new cMessage("StrobeTimer");
    dcListenTimer = new cMessage("DcListenTimer");
    dcSleepTimer = new cMessage("DcSleepTimer");
}

LinkXTMAC::~LinkXTMAC()
{
    cancelAndDelete(strobeTimer);
    cancelAndDelete(dcListenTimer);
    cancelAndDelete(dcSleepTimer);
}

void LinkXTMAC::forceRadioOn(double duration)
{
    Enter_Method("forceRadioOn");

    isActive = true;
    forcedOn = true;
    setRadioMode(RADIO_ON);
    cancelEvent(dcSleepTimer);
    if (duration >= 0) {
        scheduleAt(simTime() + duration, dcSleepTimer);
    }

    updateDisplay();
}
