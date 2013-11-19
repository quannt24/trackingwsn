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

#include "statcollector.h"
#include "energy.h"

Define_Module(StatCollector);

void StatCollector::initialize()
{
    totalSensorEnergySignal = registerSignal("total_sensor_energy");

    // Record total sensor energy for first time
    pollTotalSensorEnergy();
    scheduleAt(simTime() + par("pollTSEInterval").doubleValue(), pollTSE);
}

void StatCollector::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == pollTSE) {
            pollTotalSensorEnergy();
            scheduleAt(simTime() + par("pollTSEInterval").doubleValue(), pollTSE);
        }
    }
}

StatCollector::StatCollector()
{
    pollTSE = new cMessage();
}

StatCollector::~StatCollector()
{
    cancelAndDelete(pollTSE);
}

void StatCollector::pollTotalSensorEnergy()
{
    /* Calculate total sensor energy */
    cModule *wsn = getModuleByPath("Wsn");
    Energy *ener;
    int nss = wsn->par("numSensors").longValue();
    int i;
    double totalEnergy = 0;

    for (i = 0; i < nss; i++) {
        ener = check_and_cast<Energy*>(wsn->getSubmodule("sensor", i)->getSubmodule("energy"));
        totalEnergy += ener->getCapacity();
    }

    emit(totalSensorEnergySignal, totalEnergy);
}
