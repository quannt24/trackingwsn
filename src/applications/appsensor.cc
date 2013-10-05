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
            // TODO Report measurements (broadcast)
            MsgSenseResult *msgResult = new MsgSenseResult("MsgSenseResult");
            send(msgResult, "netGate$o");
        } else if (msg == collTimer) {
            // TODO Collect measurements
            EV << "Number of result " << nResult << '\n';
            if (nResult >= 4) {
                // TODO
                getParentModule()->bubble("Enough measurements");
                EV << "Evaluate CH\n";
            } else {
                // Not enough measurements try to re-synchronize
                getParentModule()->bubble("Nah");
                syncSense = false;
            }
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
}

void AppSensor::recvSensedResult(SensedResult *result)
{
    std::list<Measurement> meaList = result->getMeaList();

    if (meaList.size() == 0) {
        EV << "Nothing sensed\n";
        syncSense = false; // Come back to unsynchronized state
    } else {
        if (!syncSense) {
            EV << "Synchronizing sensing\n";
            MsgSenseNotify *notify = new MsgSenseNotify("SenseNotify");
            send(notify, "netGate$o");
            // Plant timer to sense again immediately after broadcast notify finished
            syncSense = true;
            cancelEvent(senseTimer);
            scheduleAt(simTime() + 0.005, senseTimer); // TODO 127 byte / 250kbps
        } else {
            // Sensing is synchronized locally, collect measurements
            EV << "Sensing is synchronized, collecting measurements\n";
            nResult = 0;
            cancelEvent(collTimer);
            scheduleAt(simTime() + 0.1, collTimer); // TODO Number of expected results * tx time
            cancelEvent(reportTimer);
            scheduleAt(simTime() + uniform(0, 0.08), reportTimer); // TODO Based on collTimer and tx time
        }
    }

    delete result;
}

/*
 * Receive message from other node
 */
void AppSensor::recvMessage(MsgTracking *msg)
{
    if (msg->getMsgType() == MSG_SENSE_NOTIFY) {
        if (!syncSense) {
            EV<< "Synchronized sensing by notify\n";
            syncSense = true;
            cancelEvent (senseTimer);
            scheduleAt(simTime() + 0.04, senseTimer); // TODO calculate period
        }
        // If synchronized, do nothing
    } else if (msg->getMsgType() == MSG_SENSE_RESULT) {
        nResult++;
    }

    delete msg;
}
