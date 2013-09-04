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
#include "msgkind.h"

Define_Module(Link802154);

void Link802154::initialize()
{
    addr = this->getId(); // Address is also module id of link module
}

void Link802154::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == txMsg) {
            // Transmit timer
            transmitPackets();
        }
    } else {
        if (msg->getArrivalGate() == gate("netGate$i")) {
            // Packet from upper layer, send to next hop
            queuePacket((WsnPacket*) msg);
        } else if (msg->getArrivalGate() == gate("radioIn")) {
            if (msg->getKind() == WL_PACKET) {
                // Packet from other node
                recvPacket((WsnPacket*) msg);
            }
        }
    }
}

Link802154::Link802154()
{
    txMsg = new cMessage("TxMsg");
}

Link802154::~Link802154()
{
    cancelAndDelete(txMsg);
}

/*
 * Get MAC address
 */
int Link802154::getAddr()
{
    return addr;
}

/*
 * Add packet to sending queue and start a transmit timer.
 */
void Link802154::queuePacket(WsnPacket *packet)
{
    // TODO set frame size
    packet->setSrcAddr(addr);
    outQueue.insert(packet);

    // If transmit timer is not set, set it immediately
    if (!txMsg->isScheduled()) scheduleAt(simTime(), txMsg);
}

/*
 * Check if channel is idle, then transmit queued packets to the air.
 * If channel is busy, back-off.
 */
void Link802154::transmitPackets()
{
    WsnPacket *pkt;
    Link802154 *des;

    while (!outQueue.isEmpty()) {
        // TODO Sense channel, back-off if channel is busy
        pkt = (WsnPacket*) outQueue.pop();
        des = (Link802154*) simulation.getModule(pkt->getDesAddr());
        if (des == NULL) {
            EV << "Link802154::transmitPackets : destination error\n";
            delete pkt;
        } else {
            // TODO Calculate delay
            sendDirect(pkt, 0, 0, des, "radioIn");
        }
    }
}

/*
 * Receive packet from other node, forward to upper layer
 */
void Link802154::recvPacket(WsnPacket* packet)
{
    // Forward to upper layer
    send(packet, "netGate$o");
}
