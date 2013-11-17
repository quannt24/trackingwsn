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

#include "ass.h"
#include "msgkind.h"
#include "worldutil.h"
#include "mobility.h"
#include "energy.h"
#include "sensereq_m.h"
#include "sensedsignal_m.h"
#include "sensedresult_m.h"

Define_Module(Ass);

void Ass::initialize()
{
    // TODO Configure noise
    noise = new GaussianNoise(0, simulation.getModuleByPath("Wsn.target[0]")->par("range").doubleValue() / 25);
    nSensedTarget = 0;
    nMeasurement = 0;
}

void Ass::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == responseTimer) {
            // Send sensing result to application
            SensedResult *result = new SensedResult();
            result->setKind(SS_RESULT);
            result->setMeaList(meaList); // Send a copy of current meaList
            send(result, "appGate$o");
        }
    } else {
        if (msg->getKind() == SS_START) {
            // Start sensing order from app
            reqTargets();
            delete msg;
        } else if (msg->getKind() == SS_SIGNAL) {
            // Signal from target, add to measurement list
            SensedSignal *sig = check_and_cast<SensedSignal*>(msg);
            recvSenseData(sig);
        } else if (msg->getKind() == SS_CANCEL) {
            cancelEvent(responseTimer);
            delete msg;
        }
    }
}

Ass::Ass()
{
    responseTimer = new cMessage("ResponseTimer");
}

Ass::~Ass()
{
    cancelAndDelete(responseTimer);
    delete noise;
}

/*
 * Request targets for sensed data. Call of this function is start of new sensing action.
 */
void Ass::reqTargets()
{
    cModule *wsn = simulation.getModuleByPath("Wsn");
    int numTargets = wsn->par("numTargets");
    int i;
    cCompoundModule *tar;
    Mobility *mob1, *mob2;
    double d; // Distance from the sensor to target

    // If the sensor is in range of one target, send request
    mob1 = check_and_cast<Mobility*>(getParentModule()->getSubmodule("mobility"));
    nSensedTarget = 0;
    nMeasurement = 0;
    meaList.clear();
    for (i = 0; i < numTargets; i++) {
        tar = check_and_cast<cCompoundModule*>(wsn->getSubmodule("target", i));
        mob2 = check_and_cast<Mobility*>(tar->getSubmodule("mobility"));
        d = distance(mob1, mob2);
        if (d < tar->par("range").doubleValue()) {
            nSensedTarget++; // Calculate number of sensed targets
            // Send request
            SenseReq *senseReq = new SenseReq("SenseReq", SS_REQ);
            senseReq->setSrcAss(this);
            senseReq->setSrcMob(mob1);
            sendDirect(senseReq, tar->getSubmodule("asg"), "reqIn");
        }
    }
}

void Ass::recvSenseData(SensedSignal* sig)
{
    Measurement m(sig->getTarId(), sig->getDistance() + noise->nextNoise());
    meaList.push_back(m);
    nMeasurement++;
    if (nMeasurement == nSensedTarget) {
        // All signals are received, set delay timer to send result out
        cancelEvent(responseTimer);
        scheduleAt(simTime() + par("responseDelay").doubleValue(), responseTimer);
    }
    delete sig;
}
