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
#include "wsnpacket_m.h"

Define_Module(Link802154);

void Link802154::initialize()
{
    addr = this->getId(); // Address is also module id of link module
}

void Link802154::handleMessage(cMessage *msg)
{
    if (msg->getArrivalGate() == gate("netGate$i")) {
        // From upper layer
        sendPacket((WsnPacket*) msg);
        EV << "sending packet";
    } else if (msg->getArrivalGate() == gate("radioIn")) {
        EV << "receive packet";
        delete msg; // TODO process message
    }
}

/*
 * Get MAC address
 */
int Link802154::getAddr()
{
    return addr;
}

/*
 * Send packet out
 */
void Link802154::sendPacket(WsnPacket* packet)
{
    Link802154 *desLink = (Link802154*) simulation.getModule(packet->getDesAddr());
    if (desLink == NULL) {
        EV << "destination error\n";
        delete packet;
        return;
    }

    packet->setSrcAddr(addr);
    // TODO set frame size

    // TODO Sense channel
    sendDirect(packet, 0, 0, desLink, "radioIn"); // TODO add delay
}

/*
 * Receive packet from other node, forward to upper layer
 */
void Link802154::recvPacket(WsnPacket* packet)
{
}
