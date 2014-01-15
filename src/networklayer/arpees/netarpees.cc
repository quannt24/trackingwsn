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

#include "netarpees.h"

Define_Module(NetArpees);

void NetArpees::initialize()
{
    // TODO - Generated method body
}

void NetArpees::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
    } else {
        if (msg->getArrivalGate() == gate("appGate$i")) {
            // Packet from upper layer
            recvMessage((MessageCR*) msg);
        } else if (msg->getArrivalGate() == gate("linkGate$i")) {
            // Packet from link layer
            recvPacket((PacketARPEES*) msg);
        }
    }
}

NetArpees::NetArpees()
{
    bsAddr = 0;
    rnAddr = 0;
}

NetArpees::~NetArpees()
{
}

void NetArpees::recvMessage(MessageCR* msg)
{
}

void NetArpees::recvPacket(PacketARPEES* pkt)
{
}

void NetArpees::requestRelay()
{
    // Send request for relay node
    PacketARPEES_RelayReq *pkt = new PacketARPEES_RelayReq();
    pkt->setSrcMacAddr(getMacAddr());
    // No need to set desMacAddr here

    pkt->setByteLength(pkt->getPkSize());

    send(pkt, "linkGate$o");
}

void NetArpees::sendRelayInfo(PacketARPEES* reqPkt)
{
}
