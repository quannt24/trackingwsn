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
#include "messagetracking_m.h"
#include "msgkind.h"

Define_Module(AppSensor);

void AppSensor::initialize()
{
    // TODO Test Start sensing
    scheduleAt(22 + uniform(0, 5), senseTimer);
}

void AppSensor::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == senseTimer) {
            // Sensing timer:
            cMessage *ssStartMsg = new cMessage("ssStartMsg", SS_START);
            send(ssStartMsg, "ssGate$o");
            // Schedule next sensing
            scheduleAt(simTime() + par("senseInterval"), senseTimer);
        } else if (msg == reportTimer) {
            // Report measurements (broadcast)
            MsgSenseResult *msgResult = new MsgSenseResult();
            msgResult->setMeaList(meaList);
            send(msgResult, "netGate$o");
        } else if (msg == collTimer) {
            // TODO Finish measurement collection. Evaluate result.
        }
    } else {
        if (msg->getArrivalGate() == gate("ssGate$i")) {
            if (msg->getKind() == SS_RESULT) {
                SensedResult *result = check_and_cast<SensedResult*>(msg);
                recvSensedResult(result);
            }
        } else if (msg->getArrivalGate() == gate("netGate$i")) {
            MsgTracking *m = check_and_cast<MsgTracking*>(msg);
            recvMessage(m);
        }
    }
}

AppSensor::AppSensor()
{
    syncSense = false;
    // Create self messages for timers
    senseTimer = new cMessage("SenseTimer");
    reportTimer = new cMessage("ReportTimer");
    collTimer = new cMessage("CollTimer");
}

AppSensor::~AppSensor()
{
    cancelAndDelete(senseTimer);
    cancelAndDelete(reportTimer);
    cancelAndDelete(collTimer);
    mc.clear(); // Clear collection
}

void AppSensor::recvSensedResult(SensedResult *result)
{
    meaList = result->getMeaList();

    if (meaList.size() == 0) {
        EV << "Nothing sensed\n";
        syncSense = false; // Come back to unsynchronized state
    } else {
        if (!syncSense) {
            EV << "Synchronizing sensing\n";
            MsgSyncRequest *notify = new MsgSyncRequest("SyncRequest");
            send(notify, "netGate$o");
            syncSense = true;
            // Do nothing except change to synchronized state
        } else {
            // Add sensed measurement to collection
            mc.clear();
            for (std::list<Measurement>::iterator it=meaList.begin(); it != meaList.end(); ++it) {
                mc.addMeasurement(*it);
            }

            // Sensing is synchronized locally, collect measurements from other nodes
            EV << "Sensing is synchronized, collecting measurements\n";
            // Set collect measurement timer
            scheduleAt(simTime() + par("collMeaPeriod").doubleValue(), collTimer);
            // Set report timer
            scheduleAt(simTime() + uniform(0, par("repMeaPeriod").doubleValue()), reportTimer);
        }
    }

    delete result;
}

/*
 * Receive message from other node
 */
void AppSensor::recvMessage(MsgTracking *msg)
{
    if (msg->getMsgType() == MSG_SYNC_REQUEST) {
        EV<< "Synchronized sensing by notify\n";
        syncSense = true;

        // If in sense delay period, cancel the sensing
        cMessage *cancelSense = new cMessage();
        cancelSense->setKind(SS_CANCEL);
        send(cancelSense, "ssGate$o");

        // Reset sensing timer with synchronized value
        cancelEvent(senseTimer);
        scheduleAt(simTime() + par("senseInterval").doubleValue()
                - this->getParentModule()->getSubmodule("ass")->par("responseDelay").doubleValue() - 0.005,
                senseTimer);
    } else if (msg->getMsgType() == MSG_SENSE_RESULT) {
        // Add measurements to collection
        std::list<Measurement> ml = ((MsgSenseResult*) msg)->getMeaList();
        for (std::list<Measurement>::iterator it=ml.begin(); it != ml.end(); ++it) {
            mc.addMeasurement(*it);
        }
    }

    delete msg;
}
