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
        simsignal_t totalSensorEnergySignal;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        StatCollector();
        ~StatCollector();
        /* Calculate total energy and emit it */
        void pollTotalSensorEnergy();
};

#endif
