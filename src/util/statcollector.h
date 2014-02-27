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

#ifndef __TRACKINGWSN_STATCOLLECTOR_H_
#define __TRACKINGWSN_STATCOLLECTOR_H_

#include <omnetpp.h>

/**
 * Statistics Collector
 */
class StatCollector : public cSimpleModule
{
    private:
        cMessage *pollTSE; // Timer for polling total sensor energy

        int numRecvFrame; // Number of successfully received frames
        int numLostFrame; // Number of lost frames
        int numRecvPacket; // Number of successfully received packets
        int numLostPacket; // Number of lost packets
        int numCreatedMTR; // Number of MsgTrackResult messages created
        int numRecvMTR; // Number of MsgTrackResult received at BS
        int numLostMTRbyLink; // Number of MsgTrackResult lost by link layer
        int numLostMTRbyNet; // Number of MsgTrackResult lost by net layer

        simsignal_t sigTotalSensorEnergy;
        simsignal_t sigEstErr;
        simsignal_t sigRecvFrame;
        simsignal_t sigLostFrame;
        simsignal_t sigRecvPacket;
        simsignal_t sigLostPacket;
        simsignal_t sigCreatedMTR;
        simsignal_t sigRecvMTR;
        simsignal_t sigLostMTRbyLink;
        simsignal_t sigLostMTRbyNet;

        /**
         * Record remaining energy of sensor nodes
         */
        void recRemainingEnergy();

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        StatCollector();
        ~StatCollector();
        /* Calculate total energy and emit it */
        void pollTotalSensorEnergy();
        /* Record estimation error */
        void recEstError(double err);
        /* Increase number of successfully received payload frames */
        void incRecvFrame();
        /* Increase number of lost payload frames */
        void incLostFrame();
        /* Increase number of successfully received packets (payload frames) */
        void incRecvPacket();
        /* Increase number of lost packets (payload frames) */
        void incLostPacket();
        /* Increase number of created message MsgTrackResult (aka DATA_TO_BS) */
        void incCreatedMTR();
        /* Increase number of successfully received MsgTrackResult */
        void incRecvMTR();
        /* Increase number of lost MsgTrackResult by link layer */
        void incLostMTRbyLink();
        /* Increase number of lost MsgTrackResult by network layer */
        void incLostMTRbyNet();

        void finish();
};

#endif
