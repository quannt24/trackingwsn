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
#include "messagecr_m.h"
#include "messagetracking_m.h"
#include "link802154.h"
#include "energy.h"
#include "mobility.h"
#include "worldutil.h"
#include "msgkind.h"

Define_Module(NetEMRP);

void NetEMRP::initialize()
{
    // Set timer for initialize EMRP first time
    scheduleAt(uniform(0, par("initInterval")), initMsg);
}

void NetEMRP::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == initMsg) {
            requestRelay(true);
        } else if (msg->getKind() == RES_RELAY) {
            // This message has a packet passed in context pointer
            sendRelayInfo((PacketEMRP*) msg->getContextPointer());
            delete msg;
        } else if (msg->getKind() == EMRP_WAIT_RELAY) {
            // An amount of time waited for relay info. If relay info is ready, send the delayed
            // message; if not, report a failure.
            if (bsAddr > 0 || rnAddr > 0) {
                sendMsgDown((MessageCR*) msg->getContextPointer());
            } else {
                // Delete message that failed to be sent
                delete (MessageCR*) msg->getContextPointer();
                // TODO Report failure
                EV << "Error: Cannot send data to BS now\n";
            }
            delete msg;
        }
    } else {
        if (msg->getArrivalGate() == gate("appGate$i")) {
            // Packet from upper layer
            recvMessage((MessageCR*) msg);
        } else if (msg->getArrivalGate() == gate("linkGate$i")) {
            // Packet from link layer
            recvPacket((PacketEMRP*) msg);
        }
    }
}

NetEMRP::NetEMRP()
{
    bsAddr = 0;
    rnAddr = 0;
    bnAddr = 0;

    enerRn = 0;
    dRnBs = 0;
    enerBn = 0;
    dBnBs = 0;

    initMsg = new cMessage("InitEmrpMsg");
}

NetEMRP::~NetEMRP()
{
    cancelAndDelete(initMsg);
}

/* Process received message from upper layer */
void NetEMRP::recvMessage(MessageCR *msg)
{
    sendMsgDown(msg);
}

/*
 * Process received packet
 */
void NetEMRP::recvPacket(PacketEMRP *pkt)
{
    if (pkt->getPkType() == PK_REQ_RELAY) {
        // Receive a request for relay information
        cMessage *resRelayTimer = new cMessage("ResRelayTimer");
        resRelayTimer->setKind(RES_RELAY);
        resRelayTimer->setContextPointer(pkt->dup()); // Pack sender information with timer
        scheduleAt(simTime() + uniform(0, par("resRelayPeriod").doubleValue()), resRelayTimer);
        delete pkt;

        // Notify application that event occurs
        notifyApp();
    } else if (pkt->getPkType() == PK_RELAY_INFO) {
        // Receive relay information
        PacketEMRP_RelayInfo *ri = check_and_cast<PacketEMRP_RelayInfo*>(pkt);
        if (ri->getBsFlag() == true) {
            bsAddr = ri->getSrcMacAddr();
        } else {
            if (bsAddr <= 0) {
                bool ret = considerRelay(ri);
                if (!ret) considerBackup(ri);
            }
        }
        delete pkt;

        // Notify application that event occurs
        notifyApp();
    } else if (pkt->getPkType() == PK_ENERGY_INFO) {
        // Receive energy information
        updateRelayEnergy(check_and_cast<PacketEMRP_EnergyInfo*>(pkt));

        // Notify application that event occurs
        notifyApp();
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
            // Forward to base station
            int sender = pkt->getSrcMacAddr();
            pkt->setSrcMacAddr(getMacAddr());
            if (bsAddr > 0) {
                pkt->setDesMacAddr(bsAddr);
            } else {
                pkt->setDesMacAddr(rnAddr);
            }
            send(pkt, "linkGate$o");

            // Send back a report about energy
            sendEnergyInfo(sender, pkt->getBitLength());

            // Notify application that event occurs
            notifyApp();
        }
    }
}

/*
 * Broadcast request for info of base station/relay/backup node.
 */
void NetEMRP::requestRelay(bool init)
{
    // Send request for relay node
    PacketEMRP *pkt = new PacketEMRP();
    pkt->setTxType(TX_BROADCAST);
    pkt->setPkType(PK_REQ_RELAY);
    pkt->setSrcMacAddr(getMacAddr());
    // No need to set desMacAddr here

    pkt->setByteLength(pkt->getPkSize());

    send(pkt, "linkGate$o");
}

/*
 * Response to a request for relay node, given the requesting packet
 */
void NetEMRP::sendRelayInfo(PacketEMRP *reqPkt)
{
    if (reqPkt == NULL) return;

    PacketEMRP_RelayInfo *pkt = new PacketEMRP_RelayInfo();
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
 * Return true when new the node having new relay info is selected as new relay node.
 */
bool NetEMRP::considerRelay(PacketEMRP_RelayInfo *ri)
{
    Mobility *mob = (Mobility*) getParentModule()->getSubmodule("mobility");
    // Distance from this node to candidate
    double dRc = distance(mob->getX(), mob->getY(), ri->getPosX(), ri->getPosY());

    if (rnAddr <= 0) {
        // No relay node has been selected, select this candidate
        rnAddr = ri->getSrcMacAddr();

        enerRn = ri->getEnergy();
        dRnBs = ri->getDBS();
        dRn = dRc;

        return true;
    } else {
        // Compare relay info of candidate to current relay node
        double a1 = assessRelay(enerRn, dRn, dBS, dRnBs);
        double a2 = assessRelay(ri->getEnergy(), dRc, dBS, ri->getDBS());

        if (a2 > a1) {
            rnAddr = ri->getSrcMacAddr();

            enerRn = ri->getEnergy();
            dRnBs = ri->getDBS();
            dRn = dRc;

            return true;
        } else {
            return false;
        }
    }
}

/*
 * Compare new relay info with current backup node; if it's better, select it as new backup node.
 * Return true when new the node having new relay info is selected as new backup node.
 */
bool NetEMRP::considerBackup(PacketEMRP_RelayInfo *ri)
{
    Mobility *mob = (Mobility*) getParentModule()->getSubmodule("mobility");
    // Distance from this node to candidate
    double dRc = distance(mob->getX(), mob->getY(), ri->getPosX(), ri->getPosY());

    if (bnAddr <= 0) {
        // No relay node has been selected, select this candidate
        bnAddr = ri->getSrcMacAddr();

        enerBn = ri->getEnergy();
        dBnBs = ri->getDBS();
        dBn = dRc;

        return true;
    } else {
        // Compare relay info of candidate to current relay node
        double a1 = assessRelay(enerBn, dBn, dBS, dBnBs);
        double a2 = assessRelay(ri->getEnergy(), dRc, dBS, ri->getDBS());

        if (a2 > a1) {
            bnAddr = ri->getSrcMacAddr();

            enerBn = ri->getEnergy();
            dBnBs = ri->getDBS();
            dBn = dRc;

            return true;
        } else {
            return false;
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
double NetEMRP::assessRelay(double ener, double dRc, double dBs, double dRcBs)
{
    double cosa = (dRc*dRc + dBs*dBs - dRcBs*dRcBs) / (2 * dRcBs * dBs);
    return ener / dRcBs * cosa;
}

/*
 * Send an energy report to a node
 * Param:
 *  addr Address of node receiving info
 *  bitLen Length in bits of relayed packet
 */
void NetEMRP::sendEnergyInfo(int addr, int bitLen)
{
    PacketEMRP_EnergyInfo *ei = new PacketEMRP_EnergyInfo("EnergyInfo");
    ei->setTxType(TX_PPP);

    ei->setPkType(PK_ENERGY_INFO);
    ei->setSrcMacAddr(getMacAddr());
    ei->setDesMacAddr(addr);

    // Estimate consumed energy (original protocol, not accurate)
    Link802154* link = (Link802154*) getParentModule()->getSubmodule("link");
    double e_elec = link->par("e_elec").doubleValue();
    double e_fs = link->par("e_fs").doubleValue();
    double d = link->par("txRange").doubleValue();
    double eTotal;
    if (d <= link->par("txRTh").doubleValue()) {
        eTotal = bitLen * e_elec + (bitLen * e_elec + bitLen * e_fs * d * d);
    } else {
        eTotal = bitLen * e_elec + (bitLen * e_elec + bitLen * e_fs * d * d * d * d);
    }
    ei->setConsumedEnergy(eTotal);

    // Note: It's better to use current energy

    ei->setByteLength(ei->getPkSize());

    send(ei, "linkGate$o");
}

/*
 * Update energy of relay node when receive an energy reporting packet.
 * Perform switch relay node or find new relay node if neccessary.
 */
void NetEMRP::updateRelayEnergy(PacketEMRP_EnergyInfo *ei)
{
    enerRn -= ei->getConsumedEnergy();
    delete ei;

    // Check critical energy value
    if (enerRn < par("criticalEnergy").doubleValue()) {
        if (enerBn < par("criticalEnergy").doubleValue()) {
            // Find new relay/backup nodes
            requestRelay(false);
        } else {
            // Switch to backup node
            switchRN();
        }
        return;
    }

    if (enerBn - enerRn >= par("switchingEnergy").doubleValue()) {
        switchRN();
    }
}

/*
 * Switch between relay/backup nodes
 */
void NetEMRP::switchRN()
{
    int tempi;
    double tempd;

    tempi = rnAddr;
    rnAddr = bnAddr;
    bnAddr = tempi;

    tempd = enerRn;
    enerRn = enerBn;
    enerBn = tempd;

    tempd = dRnBs;
    dRnBs = dBnBs;
    dBnBs = tempd;

    tempd = dRn;
    dRn = dBn;
    dBn = tempd;
}

/*
 * Package and send message from upper layer down to lower layer
 */
void NetEMRP::sendMsgDown(MessageCR *msg)
{
    if (msg->getRoutingType() == RT_TO_BS && bsAddr <= 0 && rnAddr <= 0) {
        // If at the time having a message need to be sent to BS, relay info is not ready
        // (may be due to incomplete initializing stage), send a request for relay info,
        // and delay sending this message for a short time.
        cMessage *waitRelayMsg = new cMessage("WaitRelayMsg");
        waitRelayMsg->setKind(EMRP_WAIT_RELAY);
        waitRelayMsg->setContextPointer(msg);
        // Set timeout for waiting relay info
        scheduleAt(simTime() + par("resRelayPeriod").doubleValue(), waitRelayMsg);
        return;
    }

    PacketEMRP *pkt = new PacketEMRP();
    if (msg->getRoutingType() == RT_TO_AN) {
        pkt->setTxType(TX_PPP);
        pkt->setPkType(PK_PAYLOAD_TO_AN);
        pkt->setDesMacAddr(msg->getDesMacAddr());
    } else if (msg->getRoutingType() == RT_TO_BS) {
        pkt->setTxType(TX_PPP);
        pkt->setPkType(PK_PAYLOAD_TO_BS);
        pkt->setDesMacAddr(rnAddr);
    } else if (msg->getRoutingType() == RT_BROADCAST) {
        pkt->setTxType(TX_BROADCAST);
        pkt->setPkType(PK_PAYLOAD_TO_AN);
        // No need to set desMacAddr here
    }
    pkt->setSrcMacAddr(getMacAddr());
    pkt->setStrobeFlag(msg->getStrobeFlag());

    pkt->setByteLength(pkt->getPkSize());
    pkt->encapsulate(msg); // Packet length will be increased by message length

    send(pkt, "linkGate$o");
}
