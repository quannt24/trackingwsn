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

#include "linkbmac.h"
#include "statcollector.h"
#include "channelutil.h"
#include "packetcr_m.h"

Define_Module(LinkBMAC);

/* =========================================================================
 * Private functions
 * ========================================================================= */

/* Stay active for a short time */
void LinkBMAC::setActive()
{
    isActive = true;
    if (!forcedOn) {
        if (radioMode == RADIO_OFF) {
            // Turn radio on
            setRadioMode(RADIO_ON);
        }
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

void LinkBMAC::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == backoffTimer) {
            // End of back-off period, perform CCa
            scheduleAt(simTime() + par("ccaDur").doubleValue(), ccaTimer); // Simulation
        } else if (msg == ccaTimer) {
            // Perform CCA, if channel is idle, acquire the channel
            performCCA();
        } else if (msg == releaseChannelTimer) {
            releaseChannel();
            // TODO LIFS
            startSending();
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
            // Perform a CCA to receive preamble (without acquiring channel)
            ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("cu");
            if (cu->checkChannel(this) == false) {
                setActive(); // Change to active state
                getParentModule()->bubble("Waken up");
            }
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
            recvPacket(check_and_cast<Packet802154*>(msg));
        } else if (msg->getArrivalGate() == gate("radioIn")) {
            Frame802154 *frame = check_and_cast<Frame802154*>(msg);
            recvFrame(frame);
        }
    }
}

void LinkBMAC::setRadioMode(int mode)
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
            cancelEvent(dcSleepTimer);
            cancelEvent(dcListenTimer);

            // Clear pending transmissions
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

void LinkBMAC::poweroff()
{
    cancelEvent(dcSleepTimer);
    cancelEvent(dcListenTimer);

    Link802154::poweroff();

    updateDisplay();
}

void LinkBMAC::updateDisplay()
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

/* Receive packet from network layer */
void LinkBMAC::recvPacket(Packet802154 *pkt)
{
    if (radioMode != RADIO_FULL_OFF) {
        // Packet from upper layer, assemble frame and add to sending queue
        setActive();
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
 * Add Frame to sending queue and start a transmit timer.
 */
void LinkBMAC::queueFrame(Frame802154 *frame)
{
    // Prepare preamble
    Packet802154 *pkt = (Packet802154*) frame->getEncapsulatedPacket();
    if (frame->getType() == FR_PAYLOAD && pkt != NULL && pkt->getPreambleFlag()) {
        // Add preamble to head of the queue
        Frame802154 *pre = new Frame802154();
        pre->setType(FR_PREAMBLE);
        pre->setDesAddr(BROADCAST_ADDR); // There is no actual address field in frame, just broadcast
        pre->setByteLength(par("preambleByteLen").longValue());
        outQueue.insert(pre);
    }

    // Insert payload frame to queue
    outQueue.insert(frame);

    // Check if something being sent
    if (backoffTimer->isScheduled() || ccaTimer->isScheduled() || releaseChannelTimer->isScheduled()) {
        return;
    } else {
        startSending();
    }
}

/*
 * Receive frame from other node, forward to upper layer
 */
void LinkBMAC::recvFrame(Frame802154* frame)
{
    if (frame->getType() == FR_PREAMBLE) {
        delete frame;
        return; // Nothing to do
    }
    if (simInFrameLoss(frame)) return; // The frame is lost

    if (frame->getType() == FR_PAYLOAD) {
        setActive(); // Change to or lengthen active state
        // Forward to upper layer
        send(frame->decapsulate(), "netGate$o");
        // Count received frame
        StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("sc"));
        sc->incRecvFrame();
        delete frame;
    }
}

/* =========================================================================
 * Public functions
 * ========================================================================= */

LinkBMAC::LinkBMAC()
{
    isActive = true;
    forcedOn = false;
    dcListenTimer = new cMessage("DcListenTimer");
    dcSleepTimer = new cMessage("DcSleepTimer");
}

LinkBMAC::~LinkBMAC()
{
    cancelAndDelete(dcListenTimer);
    cancelAndDelete(dcSleepTimer);
}

void LinkBMAC::forceRadioOn(double duration)
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
