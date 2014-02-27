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
 * Phy/Link layer using IEEE 802.15.4, CSMA-CA, X-MAC
 */
class Link802154 : public cSimpleModule
{
    private:
        int radioMode;
        int macAddress; // Address of link layer
        int numAdjNode; // Number of connected nodes (adjacent nodes), which is <= MAX_CONNECTIONS
        int adjNode[MAX_CONNECTIONS]; // Adjacent nodes' addresses

        cPacketQueue outQueue; // Payload frame sending queue

        // Timer for calculating power consumed by transceiver listening
        cMessage *rxConsumeTimer;

        // Duty cycling
        int nStrobe;
        cMessage *strobeTimer;
        cMessage *dcListenTimer; // Start listening for strobes
        cMessage *dcSleepTimer; // Stop listening for strobes

        // Unslotted CSMA/CA variables
        int BE; // Back-off exponent
        int NB; // Number of back-off
        cMessage *backoffTimer; // Timer for finishing back-off
        cMessage *ccaTimer; // Timer for performing CCa
        cMessage *releaseChannelTimer; // Self message for releasing channel timer
        Frame802154 *outFrame; // Frame going to be transmitted
        Frame802154 *txFrame; // Frame being sent by CSMA

        Frame802154* createFrame(Packet802154 *packet);
        void queueFrame(Frame802154 *frame);
        void sendFrame(Frame802154 *frame, simtime_t propagationDelay, simtime_t duration, Link802154 *desNode,
                const char *inputGateName, int gateIndex = -1);
        void recvFrame(Frame802154 *frame);

        // X-MAC
        void prepareSending();
        void startSending();
        void sendStrobe();
        void sendPayload();
        void sendStrobeAck(Frame802154 *strobe);
        void finishSending();

        // Unslotted CSMA/CA
        void startCsma();
        void backoff();
        void performCCA();
        void transmit();
        void releaseChannel();

        /*
         * Calculate and draw energy from energy module for transmitting.
         * Return true when have enough energy, false when short of energy
         */
        bool useEnergyTx(int nbits);
        /* Calculate and draw energy for listening
         * @param onTime Time interval that transceiver is turned on for listening signal */
        void useEnergyRx(double onTime);
        /* Stop all timer and clean up memory when run out of energy */
        void poweroff();

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        Link802154();
        ~Link802154();
        int getRadioMode() { return radioMode; };
        /* Set radio mode with a duty cycling flag. If the flag is true, it's considered this
         * function is called by duty cycling and a sleep timer is set if mode is on. When mode is
         * off, a listen timer is always set. Default value for the flag is false for normal use. */
        void setRadioMode(int mode, bool dutyCycling = false);
        int getAddr();
        bool isFullConn();
        int addAdjNode(int addr);
};

#endif
