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
#include "mobility.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cconfiguration.h>

Define_Module(StatCollector);

void StatCollector::initialize()
{
    totalSensorEnergySignal = registerSignal("total_sensor_energy");
    sigRecvPacket = registerSignal("recv_packet");
    sigLostPacket = registerSignal("lost_packet");
    // Signal for emitting estimation error
    estErrSignal = registerSignal("est_err");

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
    numRecvPacket = 0;
    numLostPacket = 0;
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

void StatCollector::incRecvPacket()
{
    Enter_Method_Silent("incRecvPacket");
    numRecvPacket++;
    emit(sigRecvPacket, numRecvPacket);
}

void StatCollector::incLostPacket()
{
    Enter_Method_Silent("incLostPacket");
    numLostPacket++;
    emit(sigLostPacket, numLostPacket);
}

void StatCollector::recEstError(double err)
{
    Enter_Method_Silent("recEstError");
    emit(estErrSignal, err);
}

void StatCollector::finish()
{
    recRemainingEnergy();
}

void StatCollector::recRemainingEnergy()
{
    using namespace std;

    cConfigurationEx *configEx = ev.getConfigEx();
    string buffer;
    cModule *wsn = getModuleByPath("Wsn");
    Energy *ener;
    Mobility *mob;
    int nss = wsn->par("numSensors").longValue();
    int i;

    buffer.append("results/remain_ener_");
    buffer.append(configEx->getActiveConfigName());
    buffer.append(".data\0");
    ofstream out(buffer.c_str(), ios::out | ios::trunc);

    if (!out) {
        cerr << "Cannot output data\n";
        return;
    }

    out << "# Config: " << configEx->getActiveConfigName() << '\n';
    out << "# Remaining energy" << endl;
    out << '#' << right << setw(3) << "row" << ' ' << setw(4) << "col" << ' '
            << setw(7) << "x" << ' ' << setw(7) << "y" << ' '
            << setw(10) << "energy" << endl;

    for (i = 0; i < nss; i++) {
        ener = check_and_cast<Energy*>(wsn->getSubmodule("sensor", i)->getSubmodule("energy"));
        mob = check_and_cast<Mobility*>(wsn->getSubmodule("sensor", i)->getSubmodule("mobility"));

        out << right << setw(4) << mob->getRow() << ' ' << setw(4) << mob->getCol() << ' '
                << setw(7) << mob->getX() << ' ' << setw(7) << mob->getY() << ' '
                << setw(10) << ener->getCapacity() << endl;
    }

    out.close();
}
