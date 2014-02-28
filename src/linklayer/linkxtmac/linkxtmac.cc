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
                    StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("sc"));
                    sc->incLostFrame();
                    sc->incLostPacket();

                    // TODO For counting number of lost payload to BS by link layer
                    PacketCR *pkt = check_and_cast<PacketCR*>(frame->decapsulate());
                    if (pkt->getPkType() == PK_PAYLOAD_TO_BS) {
                        sc->incLostMTRbyLink();
                    }
                    delete pkt;
                }
                delete msg;
            }
        }
    }
}

/* Set radio mode with a duty cycling flag. If the flag is true, it's considered this
 * function is called by duty cycling and a sleep timer is set if mode is on. When mode is
 * off, a listen timer is always set. Default value for the flag is false for normal use. */
void LinkXTMAC::setRadioMode(int mode, bool dutyCycling)
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
            if (par("enableXmac").boolValue() && dutyCycling) {
                scheduleAt(simTime() + par("lR").doubleValue(), dcSleepTimer);
            }
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
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("cu");
    StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("sc"));

    /* Frame loss when collision still occurs at time when the frame is received completely. */
    if (cu->hasCollision(this)) {
        getParentModule()->bubble("collision");
        EV << "Link802154: Lost frame by collision\n";

        if (frame->getType() == FR_PAYLOAD) {
            // Count lost payload frame
            sc->incLostFrame();
            sc->incLostPacket();

            // TODO For counting number of lost payload to BS by link layer
            PacketCR *pkt = check_and_cast<PacketCR*>(frame->decapsulate());
            if (pkt->getPkType() == PK_PAYLOAD_TO_BS) {
                sc->incLostMTRbyLink();
            }
            delete pkt;
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
            sc->incLostFrame();
            sc->incLostPacket();

            // TODO For counting number of lost payload to BS by link layer
            PacketCR *pkt = check_and_cast<PacketCR*>(frame->decapsulate());
            if (pkt->getPkType() == PK_PAYLOAD_TO_BS) {
                sc->incLostMTRbyLink();
            }
            delete pkt;
        }
        delete frame;
        return;
    }

    if (frame->getType() == FR_PAYLOAD) {
        // Forward to upper layer
        send(frame->decapsulate(), "netGate$o");

        // Count received frame
        sc->incRecvFrame();
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
        if (par("enableXmac").boolValue()
                && frame->getType() == FR_PAYLOAD
                && pkt != NULL && pkt->getStrobeFlag()) {
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
    strobe->setType(FR_STROBE);
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
