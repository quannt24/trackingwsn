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

#include "appsensor.h"
#include "msgkind.h"

Define_Module(AppSensor);

void AppSensor::initialize()
{
    // TODO Test Start sensing immediately
    //scheduleAt(0, senseMsg);
}

void AppSensor::handleMessage(cMessage *msg)
{
    if (msg == senseMsg) {
        // Sensing timer

        cMessage *ssStartMsg = new cMessage("ssStartMsg", SS_START);
        send(ssStartMsg, "ssGate$o");
        // Schedule next sensing
        scheduleAt(simTime() + par("senseInterval"), senseMsg);
    } else if (msg->getKind() == SS_SIGNAL) {
        EV << "Target is sensed\n";
        delete msg;
    }
}

AppSensor::AppSensor()
{
    // Create self messages for timers
    senseMsg = new cMessage("SenseMsg");
}

AppSensor::~AppSensor()
{
    cancelAndDelete(senseMsg);
}
