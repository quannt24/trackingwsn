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
#include "msgkind.h"
#include "packetarpees_m.h"
#include "energy.h"
#include "mobility.h"
#include "worldutil.h"

Define_Module(NetARPEES);

void NetARPEES::initialize()
{
}

void NetARPEES::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == recvRelayInfoTimer) {
            sendPackets();
        } else if (msg->getKind() == RES_RELAY) {
            // This message has a packet passed in context pointer
            sendRelayInfo((PacketARPEES*) msg->getContextPointer());
            delete msg;
        }
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

NetARPEES::NetARPEES()
{
    bsAddr = 0;
    rnAddr = 0;

    recvRelayInfoTimer = new cMessage("RecvRelayInfoTimer");
}

NetARPEES::~NetARPEES()
{
    cancelAndDelete(recvRelayInfoTimer);
}

void NetARPEES::recvMessage(MessageCR* msg)
{
    if (msg == NULL) return;

    // Encapsulate the message then enqueue it
    outQueue.insert(createPacket(msg));

    if (!recvRelayInfoTimer->isScheduled()) {
        if (msg->getRoutingType() == RT_TO_BS) {
            // Clear relay node address
            rnAddr = 0;
            // Send relay request then send message
            requestRelay();
            // Plan timer for sending message
            scheduleAt(simTime() + par("resRelayPeriod").doubleValue(), recvRelayInfoTimer);
        } else {
            // Send the new queued packets immediately (there should no other packet in the queue)
            sendPackets();
        }
    }
}

void NetARPEES::recvPacket(PacketARPEES* pkt)
{
    if (pkt->getPkType() == PK_REQ_RELAY) {
        // Receive a request for relay information

        // Setting a timer for responsing
        cMessage *resRelayTimer = new cMessage("ResRelayTimer");
        resRelayTimer->setKind(RES_RELAY);
        // Pack sender information with the timer. Duplicate requesting packet and delete original
        // packet to make sure it is not changed else where.
        resRelayTimer->setContextPointer(pkt->dup());
        scheduleAt(simTime() + uniform(0, par("resRelayPeriod").doubleValue()), resRelayTimer);
        delete pkt;

    } else if (pkt->getPkType() == PK_RELAY_INFO) {
        if (recvRelayInfoTimer->isScheduled()) {
            // Receive relay information (only when recvRelayInfoTimer is set)
            PacketARPEES_RelayInfo *ri = check_and_cast<PacketARPEES_RelayInfo*>(pkt);
            if (ri->getBsFlag() == true) {
                bsAddr = ri->getSrcMacAddr();
            } else {
                if (bsAddr <= 0) {
                    considerRelay(ri);
                }
            }
        }
        delete pkt;

    } else if (pkt->getPkType() == PK_PAYLOAD_TO_AN) {
        // Send message to upper layer
        MessageCR *msg = (MessageCR*) pkt->decapsulate();
        send(msg, "appGate$o");
        delete pkt;

    } else if (pkt->getPkType() == PK_PAYLOAD_TO_BS) {
        if (par("isBaseStation").boolValue() == true) {
            // Here is the destination, send message to upper layer
            MessageCR *msg = (MessageCR*) pkt->decapsulate();
            send(msg, "appGate$o");
            delete pkt;
        } else {
            pkt->setSrcMacAddr(getMacAddr());
            // Note: Destination address will be set just before sending in sendPackets()

            // Queue the packet
            outQueue.insert(pkt);
            if (!recvRelayInfoTimer->isScheduled()) {
                // Clear relay node address
                rnAddr = 0;
                // Send relay request then send packet
                requestRelay();
                // Plan timer for sending packet
                scheduleAt(simTime() + par("resRelayPeriod").doubleValue(), recvRelayInfoTimer);
            }
        }
    }

    // Notify application that event occurs
    notifyApp();
}

/*
 * Create a packet encapsulating a message (from application layer).
 */
PacketARPEES* NetARPEES::createPacket(MessageCR *msg)
{
    if (msg == NULL) return NULL;

    PacketARPEES *pkt;

    pkt = new PacketARPEES();
    if (msg->getRoutingType() == RT_TO_AN) {
        pkt->setTxType(TX_PPP);
        pkt->setPkType(PK_PAYLOAD_TO_AN);
        pkt->setDesMacAddr(msg->getDesMacAddr());
    } else if (msg->getRoutingType() == RT_TO_BS) {
        pkt->setTxType(TX_PPP);
        pkt->setPkType(PK_PAYLOAD_TO_BS);
    } else if (msg->getRoutingType() == RT_BROADCAST) {
        pkt->setTxType(TX_BROADCAST);
        pkt->setPkType(PK_PAYLOAD_TO_AN);
        // No need to set desMacAddr here
    }
    pkt->setSrcMacAddr(getMacAddr());
    pkt->setStrobeFlag(msg->getStrobeFlag());

    pkt->setByteLength(pkt->getPkSize());
    pkt->encapsulate(msg); // Packet length will be increased by message length

    return pkt;
}

/*
 * Send all queued packets.
 * When finish reset relay node address (to find new relay node next time).
 */
void NetARPEES::sendPackets()
{
    PacketARPEES *pkt;

    while (!outQueue.isEmpty()) {
        pkt = check_and_cast<PacketARPEES*>(outQueue.pop());

        // Set destination address for 'payload to bs'
        if (pkt->getPkType() == PK_PAYLOAD_TO_BS) {
            if (bsAddr > 0) {
                pkt->setDesMacAddr(bsAddr);
            } else if (rnAddr > 0) {
                pkt->setDesMacAddr(rnAddr);
            } else {
                EV << "NetARPEES: Cannot send packet. There is no relay node.";
                delete pkt;
                continue;
            }
        }

        send(pkt, "linkGate$o");
    }

    // Reset relay node address
    rnAddr = 0;
}

/* Broadcast packet for requesting relay information */
void NetARPEES::requestRelay()
{
    // Send request for relay node
    PacketARPEES_RelayReq *pkt = new PacketARPEES_RelayReq();
    pkt->setSrcMacAddr(getMacAddr());
    // No need to set desMacAddr here

    pkt->setByteLength(pkt->getPkSize());

    send(pkt, "linkGate$o");
}

/* Response to a request for relay node, given the requesting packet */
void NetARPEES::sendRelayInfo(PacketARPEES* reqPkt)
{
    if (reqPkt == NULL) return;

    PacketARPEES_RelayInfo *pkt = new PacketARPEES_RelayInfo();
    pkt->setTxType(TX_PPP);
    pkt->setPkType(PK_RELAY_INFO); // WARNING: do not confuse with PK_REQ_RELAY
    pkt->setSrcMacAddr(getMacAddr());
    pkt->setDesMacAddr(reqPkt->getSrcMacAddr());

    // Add relay info
    if (par("isBaseStation").boolValue() == true) {
        pkt->setBsFlag(true);
    } else {
        pkt->setBsFlag(false);
    }
    pkt->setEnergy(((Energy*) getParentModule()->getSubmodule("energy"))->getCapacity());
    Mobility *mob = (Mobility*) getParentModule()->getSubmodule("mobility");
    pkt->setPosX(mob->getX());
    pkt->setPosY(mob->getX());
    pkt->setDBS(dBS);

    pkt->setByteLength(pkt->getPkSize());

    send(pkt, "linkGate$o");
    delete reqPkt;
}

/*
 * Compare new relay info with current relay node; if it's better, select it as new relay node.
 */
void NetARPEES::considerRelay(PacketARPEES_RelayInfo *ri)
{
    Mobility *mob = (Mobility*) getParentModule()->getSubmodule("mobility");
    // Distance from this node to relay candidate
    double dRc = distance(mob->getX(), mob->getY(), ri->getPosX(), ri->getPosY());

    if (rnAddr <= 0) {
        // No relay node has been selected, select this candidate
        rnAddr = ri->getSrcMacAddr();

        enerRn = ri->getEnergy();
        dRnBs = ri->getDBS();
        dRn = dRc;
    } else {
        // Compare relay info of candidate to current relay node
        double a1 = assessRelay(enerRn, dRn, dBS, dRnBs);
        double a2 = assessRelay(ri->getEnergy(), dRc, dBS, ri->getDBS());

        if (a2 > a1) {
            rnAddr = ri->getSrcMacAddr();

            enerRn = ri->getEnergy();
            dRnBs = ri->getDBS();
            dRn = dRc;
        }
    }
}

/*
 * Function for assessing a candidate for relaying.
 * Parameters:
 *  ener: energy of relay candidate
 *  dRc: distance from this node to relay candidate
 *  dBs: distance from this node to base station
 *  dRcBs: distance from relay candidate to base station
 */
double NetARPEES::assessRelay(double ener, double dRc, double dBs, double dRcBs)
{
    // TODO use arpees assess function
    double cosa = (dRc * dRc + dBs * dBs - dRcBs * dRcBs) / (2 * dRcBs * dBs);
    return ener / dRcBs * cosa;
}

