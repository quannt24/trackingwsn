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

#include "frame802154_m.h"
#include "packet802154_m.h"
#include <omnetpp.h>

// Radio mode
#define RADIO_OFF 0
#define RADIO_ON 1

// Max numbers of connected nodes
#define MAX_CONNECTIONS 25
// Broadcast address
#define BROADCAST_ADDR -1

// CSMA time symbol (s)
#define CSMA_SYMBOL 0.000016
// A unit back-off period (s)
#define aUnitBP (20 * CSMA_SYMBOL)

/**
 * Phy/Link layer using IEEE 802.15.4, CSMA-CA
 */
class Link802154 : public cSimpleModule
{
    private:
        int radioMode;
        int macAddress; // Address of link layer
        int numAdjNode; // Number of connected nodes (adjacent nodes), which is <= MAX_CONNECTIONS
        int adjNode[MAX_CONNECTIONS]; // Adjacent nodes' addresses

        cPacketQueue outQueue; // Frame sending queue

        // Unslotted CSMA/CA variables
        int BE; // Back-off exponent
        int NB; // Number of back-off
        cMessage *csmaMsg; // Self message for starting CSMA process
        cMessage *releaseChannelMsg; // Self message for releasing channel timer
        Frame802154 *txFrame; // Frame going to be transmitted

        // Timer for calculating power consumed by transceiver listening
        cMessage *rxConsumeTimer;

        // Timers for duty cycling
        cMessage *dcListenTimer; // Start listening for strobes
        cMessage *dcSleepTimer; // Stop listening for strobes

        Frame802154* createFrame(Packet802154 *packet);
        void queueFrame(Frame802154 *frame);
        void recvFrame(Frame802154 *frame);
        void sendFrame(Frame802154 *frame, simtime_t propagationDelay, simtime_t duration, Link802154 *desNode,
                const char *inputGateName, int gateIndex = -1);

        void csmaTransmit();
        bool performCCA();
        void releaseChannel();
        void transmit();
        void backoff();

        /* Calculate and draw energy from energy module for transmitting */
        void useEnergyTx(int nbits);
        /* Calculate and draw energy for listening
         * @param onTime Time interval that transceiver is turned on for listening signal */
        void useEnergyRx(double onTime);

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
