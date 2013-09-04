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

#include "netemrp.h"
#include "wsnpacket_m.h"
#include "msgkind.h"

Define_Module(NetEMRP);

void NetEMRP::initialize()
{
    // TODO test
    cMessage *self = new cMessage("Self");
    scheduleAt(uniform(0, 1), self);
}

void NetEMRP::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        // TODO Test
        WsnPacket *packet = new WsnPacket();
        packet->setKind(WL_PACKET);
        packet->setDesAddr(adjNode[intuniform(0, numAdjNode - 1)]);
        send(packet, "linkGate$o");
    } else if (msg->getArrivalGate() == gate("linkGate$i")) {
        // TODO Process message
        EV << "Packet received\n";
        WsnPacket *packet = (WsnPacket*) msg;
        packet->setDesAddr(adjNode[intuniform(0, numAdjNode - 1)]);
        send(packet, "linkGate$o");
    }
}

NetEMRP::NetEMRP()
{
    numAdjNode = 0;
}

/*
 * Check if connection list is full or not.
 */
bool NetEMRP::isFullConn()
{
    return numAdjNode >= MAX_CONNECTIONS;
}

/*
 * Add a node to connection list.
 * Return: 0 on success, -1 when error
 */
int NetEMRP::addAdjNode(int addr)
{
    if (numAdjNode >= MAX_CONNECTIONS) return -1;

    adjNode[numAdjNode] = addr;
    numAdjNode++;
    return 0;
}
