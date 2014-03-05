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
    sigTotalSensorEnergy = registerSignal("total_sensor_energy");
    // Signal for emitting estimation error
    sigEstErr = registerSignal("est_err");
    // Signals for received/lost packets (count at both network and link layer)
    sigRecvPacket = registerSignal("recv_packet");
    sigLostPacket = registerSignal("lost_packet");
    // Signal for received/lost frames (count at link layer)
    sigRecvFrame = registerSignal("recv_frame");
    sigLostFrame = registerSignal("lost_frame");
    // For record number of created MsgTrackResult
    sigCreatedMTR = registerSignal("created_mtr");
    // For record number of MsgTrackResult received at BS
    sigRecvMTR = registerSignal("recv_mtr");
    // For record number of MsgTrackResult lost by link layer
    sigLostMTRbyLink = registerSignal("lost_mtr_link");
    // For record number of MsgTrackResult lost by network layer
    sigLostMTRbyNet = registerSignal("lost_mtr_net");
    // Record number of measurement used by CH
    sigNumMeasCH = registerSignal("num_meas_ch");

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
    numRecvFrame = 0;
    numLostFrame = 0;
    numRecvPacket = 0;
    numLostPacket = 0;
    numCreatedMTR = 0;
    numRecvMTR = 0;
    numLostMTRbyLink = 0;
    numLostMTRbyNet = 0;
}

StatCollector::~StatCollector()
{
    cancelAndDelete(pollTSE);
}

void StatCollector::pollTotalSensorEnergy()
{
    /* Calculate total sensor energy */
    cModule *wsn = getModuleByPath("^");
    Energy *ener;
    int nss = wsn->par("numSensors").longValue();
    int i;
    double totalEnergy = 0;

    for (i = 0; i < nss; i++) {
        ener = check_and_cast<Energy*>(wsn->getSubmodule("sensor", i)->getSubmodule("energy"));
        totalEnergy += ener->getCapacity();
    }

    emit(sigTotalSensorEnergy, totalEnergy);
}

void StatCollector::recEstError(double err)
{
    Enter_Method_Silent("recEstError");
    emit(sigEstErr, err);
}

void StatCollector::incRecvFrame()
{
    Enter_Method_Silent("incRecvFrame");
    numRecvFrame++;
    emit(sigRecvFrame, numRecvFrame);
}

void StatCollector::incLostFrame()
{
    Enter_Method_Silent("incLostFrame");
    numLostFrame++;
    emit(sigLostFrame, numLostFrame);
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

void StatCollector::incCreatedMTR()
{
    Enter_Method("incCreatedMTR");
    numCreatedMTR++;
    emit(sigCreatedMTR, numCreatedMTR);
}

void StatCollector::incRecvMTR()
{
    Enter_Method("incRecvMTR");
    numRecvMTR++;
    emit(sigRecvMTR, numRecvMTR);
}

/* Increase number of lost MsgTrackResult by link layer */
void StatCollector::incLostMTRbyLink()
{
    Enter_Method("incLostMTRbyLink");
    numLostMTRbyLink++;
    emit(sigLostMTRbyLink, numLostMTRbyLink);
}

/* Increase number of lost MsgTrackResult by network layer */
void StatCollector::incLostMTRbyNet()
{
    Enter_Method("incLostMTRbyNet");
    numLostMTRbyNet++;
    emit(sigLostMTRbyNet, numLostMTRbyNet);
}

void StatCollector::recNumMeasCH(int numMeas)
{
    Enter_Method("recNumMeasCH");
    emit(sigNumMeasCH, numMeas);
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
    cModule *wsn = getModuleByPath("^");
    Energy *ener;
    Mobility *mob;
    int nss = wsn->par("numSensors").longValue();
    int i;

    buffer.append("results/");
    buffer.append(configEx->getActiveConfigName());
    buffer.append("_RemainEnergy.data\0");
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
