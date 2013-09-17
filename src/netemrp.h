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

#ifndef __TRACKINGWSN_NETEMRP_H_
#define __TRACKINGWSN_NETEMRP_H_

#include "packetemrp_m.h"
#include "messagecr_m.h"
#include <omnetpp.h>

/**
 * Network layer using EMRP protocol
 */
class NetEMRP : public cSimpleModule
{
    private:
        // This node MAC address. This field should not used directly, please use getMacAddr().
        int macAddr;
        // MAC addresses for routing
        // Value 0 means connection info is not initialized.
        int bsAddr;
        int rlAddr;
        int bkAddr;

        // Stored information about base station, these info must be set so that node can work properly.
        double bsPosX, bsPosY; // Base station position
        double dBS; // Distance from this node to base station

        // Stored information about relay/backup nodes
        double enerRl; // Energy
        double dRlBs; // Distance from relay node to base station in meter
        double dRl; // Distance from relay node to this node
        double enerBk;
        double dBkBs;
        double dBk;

        cMessage *initMsg; // Self message for initializing EMRP procedure

        /* Get MAC address from link layer. At initial stage, this address may not available. */
        int getMacAddr();
        /* Process received message from upper layer */
        void recvMessage(MessageCR *msg);
        /* Process received packet from lower layer */
        void recvPacket(PacketEMRP *pkt);
        /* Broadcast request for info of base station/relay/backup node */
        void requestRelay();
        /* Response to a request for relay node, given the requesting packet */
        void sendRelayInfo(PacketEMRP *reqPkt);
        /* Compare new relay info with current relay node; if it's better, select it as new relay node.
         * Return true when new the node having new relay info is selected as new relay node. */
        bool considerRelay(PacketEMRP_RelayInfo *ri);
        /* Compare new relay info with current backup node; if it's better, select it as new backup node.
         * Return true when new the node having new relay info is selected as new backup node. */
        bool considerBackup(PacketEMRP_RelayInfo *ri);
        /* Function for assessing a candidate for relaying.
         * Parameters:
         *  ener: energy of relay candidate
         *  dRc: distance from this node to relay candidate
         *  dBs: distance from this node to base station
         *  dRcBs: distance from relay candidate to base station */
        double assessRelay(double ener, double dRc, double dBs, double dRcBs);
        /* Send an energy report to a node */
        void sendEnergyInfo(int addr);
        /* Update energy of relay node when receive an energy reporting packet.
         * Perform switch relay node or find new relay node if neccessary. */
        void updateRelayEnergy(PacketEMRP_EnergyInfo *ei);
        /* Switch between relay/backup nodes */
        void switchRN();

        /* Package and send message from upper layer down to lower layer */
        void sendMsgDown(MessageCR *msg);

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        NetEMRP();
        ~NetEMRP();
        /* Store position of base station */
        void setBSPos(double x, double y);
};

#endif
