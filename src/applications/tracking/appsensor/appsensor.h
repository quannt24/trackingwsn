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

#ifndef __TRACKINGWSN_APPSENSOR_H_
#define __TRACKINGWSN_APPSENSOR_H_

#include "app.h"
#include "sensedresult_m.h"
#include "messagetracking_m.h"
#include "meacoll.h"
#include <omnetpp.h>

/**
 * Sensor's Application Layer
 */
class AppSensor : public App
{
    private:
        int workMode;

        bool syncSense;
        cMessage *senseTimer; // Self message for start sensing
        cMessage *reportTimer; // Timer for reporting measurement
        cMessage *collTimer; // Timer for collecting measurements
        std::list<Measurement> meaList; // Measurement list of recent sensing
        MeaColl mc; // Measurement collection
        simtime_t tsSense; // Time stamp for receiving sense result from sensor

        /* Send sense result to CH (broadcast, CH will collect these result) */
        void sendSenseResult();
        /* Receive measurements from sensor module. Prepare to broadcast the result and collect other's */
        void recvSenseResult(SensedResult *result);
        /* Receive message from other node */
        void recvMessage(MsgTracking *msg);
        /* Promote this node to CH (if appropriate). Then estimate targets' positions (if is CH). */
        void trackTargets();

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        AppSensor();
        ~AppSensor();
};

#endif
