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
#include <omnetpp.h>

// Max numbers of connected nodes
#define MAX_CONNECTIONS 25

/**
 * Phy/Link layer using IEEE 802.15.4, CSMA-CA
 */
class Link802154 : public cSimpleModule
{
    private:
        int macAddress; // Address of link layer
        int numAdjNode; // Number of connected nodes (adjacent nodes), which is <= MAX_CONNECTIONS
        int adjNode[MAX_CONNECTIONS]; // Adjacent nodes' addresses

        cPacketQueue outQueue; // Frame sending queue
        cMessage *txMsg; // Self message for transmit timer

        Frame802154* createFrame(cPacket *packet);
        void queueFrame(Frame802154 *frame);
        void transmitFrames(); // Transmit queued frames to the air
        void recvFrame(Frame802154 *frame); // Receive frame from other node, forward to upper layer

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        Link802154();
        ~Link802154();
        int getAddr();
        bool isFullConn(); // Check if connection list is full or not.
        int addAdjNode(int addr); // Add a node to connection list

};

#endif
