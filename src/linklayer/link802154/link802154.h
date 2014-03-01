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

#ifndef __TRACKINGWSN_LINK802154_H_
#define __TRACKINGWSN_LINK802154_H_

#include "link.h"
#include "frame802154_m.h"
#include "packet802154_m.h"
#include <omnetpp.h>


/**
 * Phy/Link layer using IEEE 802.15.4, CSMA/CA.
 * Supported radio modes: RADIO_FULL_OFF, RADIO_ON.
 */
class Link802154 : public cSimpleModule
{
    protected:
        int radioMode;

        int macAddress; // Address of link layer
        int numAdjNode; // Number of connected nodes (adjacent nodes), which is <= MAX_CONNECTIONS
        int adjNode[MAX_CONNECTIONS]; // Adjacent nodes' addresses

        cPacketQueue outQueue; // Payload frame sending queue

        // Timer for calculating power consumed by transceiver listening
        cMessage *rxConsumeTimer;

        // Unslotted CSMA/CA variables
        int BE; // Back-off exponent
        int NB; // Number of back-off
        cMessage *backoffTimer; // Timer for finishing back-off
        cMessage *ccaTimer; // Timer for performing CCa
        cMessage *releaseChannelTimer; // Self message for releasing channel timer
        Frame802154 *outFrame; // Frame going to be transmitted
        Frame802154 *txFrame; // Frame being sent by CSMA

        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

        /* Set radio mode. Support: RADIO_FULL_OFF, RADIO_ON */
        virtual void setRadioMode(int mode);
        /* Force transceiver to stay in specific mode for a time duration.
         * Pass 'duration' = -1 for permanent.
         * Derived link classes should override this function for their specific use.
         * Default behavior is empty. */
        virtual void forceRadioMode(int mode, double duration) {};
        /* Stop all timer and clean up memory when run out of energy */
        virtual void poweroff();
        /* Update display */
        virtual void updateDisplay();

        /* Receive packet from network layer */
        virtual void recvPacket(Packet802154 *pkt);
        virtual Frame802154* createFrame(Packet802154 *packet);
        virtual void queueFrame(Frame802154 *frame);
        virtual void sendFrame(Frame802154 *frame, simtime_t propagationDelay, simtime_t duration,
                Link802154 *desNode, const char *inputGateName, int gateIndex = -1);
        virtual void recvFrame(Frame802154 *frame);
        /* Simulate incoming frame loss.
         * If the frame is lost, its pointer should not be used anymore.
         * Return true when the frame is lost, false otherwise. */
        virtual bool simInFrameLoss(Frame802154 *frame);

        /* Calculate and draw energy from energy module for transmitting.
         * Return true when have enough energy, false when short of energy. */
        bool useEnergyTx(int nbits);
        /* Calculate and draw energy for listening
         * @param onTime Time interval that transceiver is turned on for listening signal */
        void useEnergyRx(double onTime);

        // Unslotted CSMA/CA sending procedures
        virtual void startSending();
        void startCsma();
        void backoff();
        void performCCA();
        void transmit();
        void releaseChannel();

    public:
        Link802154();
        virtual ~Link802154();
        int getAddr();
        bool isFullConn();
        int addAdjNode(int addr);
        int getRadioMode() { return radioMode; };
};

#endif
