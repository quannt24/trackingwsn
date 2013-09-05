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
#include "packetemrp_m.h"
#include "msgkind.h"

Define_Module(NetEMRP);

void NetEMRP::initialize()
{
    // TODO test
    cMessage *self = new cMessage("Self");
    scheduleAt(uniform(0, 5), self);
}

void NetEMRP::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        // TODO Test, dispose self
        PacketEMRP *packet = new PacketEMRP("PacketEMRP");
        packet->setByteLength(86); // TODO Test
        send(packet, "linkGate$o");
    } else if (msg->getArrivalGate() == gate("linkGate$i")) {
        // TODO Process message
        PacketEMRP *packet = (PacketEMRP*) msg;
        EV << "Packet received, size " << packet->getByteLength() << "\n";
        send(packet, "linkGate$o");
    }
}
