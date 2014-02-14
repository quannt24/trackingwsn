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
#include "link802154.h"
#include "mobility.h"
#include "energy.h"
#include "estimator.h"
#include "measurement.h"
#include "worldutil.h"
#include "statcollector.h"

Define_Module(AppSensor);

void AppSensor::initialize()
{
    // At start, all nodes are active
    setWorkMode(WORK_MODE_ACTIVE);
    /* Modify sleep time so that sleep time of sensor nodes will be uniformly distributed over
     * next 'idleTime' period to prevent "all sleepy nodes" phenomena. */
    cancelEvent(sleepTimer);
    if (par("enableSleep").boolValue()) {
        scheduleAt(getParentModule()->getSubmodule("net")->par("initInterval").doubleValue()
                + uniform(0, par("idleTime").doubleValue()),
                sleepTimer);
    }
}

void AppSensor::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == activeTimer) {
            setWorkMode(WORK_MODE_ACTIVE);
        } else if (msg == sleepTimer) {
            setWorkMode(WORK_MODE_SLEEP);
        } else if (msg == senseTimer) {
            cMessage *ssStartMsg = new cMessage("ssStartMsg", SS_START);
            send(ssStartMsg, "ssGate$o");
            // Schedule next sensing
            scheduleAt(simTime() + par("senseInterval"), senseTimer);
        } else if (msg == reportTimer) {
            // Report measurements (broadcast)
            sendSenseResult();
        } else if (msg == collTimer) {
            // Finish measurement collection. Evaluate result.
            trackTargets();
        }
    } else {
        if (msg->getArrivalGate() == gate("ssGate$i")) {
            if (msg->getKind() == SS_RESULT) {
                SensedResult *result = check_and_cast<SensedResult*>(msg);
                recvSenseResult(result);
            } else {
                std::cerr << "AppSensor: Unexpected message from ssGate\n";
                delete msg;
            }
        } else if (msg->getArrivalGate() == gate("netGate$i")) {
            MsgTracking *m = check_and_cast<MsgTracking*>(msg);
            recvMessage(m);
            notifyEvent();
        }
    }
}

AppSensor::AppSensor()
{
    syncSense = false;
    // Create self messages for timers
    activeTimer = new cMessage("ActiveTimer");
    sleepTimer = new cMessage("SleepTimer");
    senseTimer = new cMessage("SenseTimer");
    reportTimer = new cMessage("ReportTimer");
    collTimer = new cMessage("CollTimer");

    // Sense time stamp
    tsSense = 0;
}

AppSensor::~AppSensor()
{
    cancelAndDelete(activeTimer);
    cancelAndDelete(sleepTimer);
    cancelAndDelete(senseTimer);
    cancelAndDelete(reportTimer);
    cancelAndDelete(collTimer);
    mc.clear(); // Clear collection
}

void AppSensor::notifyEvent()
{
    Enter_Method("notifyEvent");
    EV << "AppSensor: Event notify\n";
    /* Check if it is currently network initial period; if not, extend active time.
     * In network initial interval, all nodes should be active then sleep time should be
     * uniformly distributed over next 'idleTime' period so that "all sleepy nodes" does
     * not occur. */
    if (simTime() > getParentModule()->getSubmodule("net")->par("initInterval").doubleValue()) {
        // Extend active time when having event
        setWorkMode(WORK_MODE_ACTIVE);
    }
}

void AppSensor::poweroff()
{
    Enter_Method("poweroff");
    setWorkMode(WORK_MODE_OFF);
}

bool AppSensor::isWorking()
{
    return workMode != WORK_MODE_OFF;
}

void AppSensor::sendSenseResult()
{
    Mobility *mob = (Mobility*) getParentModule()->getSubmodule("mobility");
    Energy *ener = (Energy*) getParentModule()->getSubmodule("energy");

    MsgSenseResult *msgResult = new MsgSenseResult();

    // Add node info
    msgResult->setNodePosX(mob->getX());
    msgResult->setNodePosY(mob->getY());
    msgResult->setNodeEnergy(ener->getCapacity());

    // Add measurement list
    msgResult->setMeaList(meaList);

    /* Set message size
     * Each measurement contains target ID + measuredDistance will have size of 1 + 4 bytes
     * Node info will have size of 4 + 4 + 4 bytes for x, y, energy. */
    msgResult->setMsgSize(msgResult->getMsgSize() + 12 + 5 * meaList.size());
    msgResult->setByteLength(msgResult->getMsgSize());
    send(msgResult, "netGate$o");
}

void AppSensor::recvSenseResult(SensedResult *result)
{
    Mobility *mob;
    Energy *ener;
    meaList = result->getMeaList();

    if (meaList.size() == 0) {
        EV << "Nothing sensed\n";
        syncSense = false; // Come back to unsynchronized state
    } else {
        // Self notify for event so that the node can wake up if target was sensed
        notifyEvent();

        if (!syncSense) {
            EV << "Synchronizing sensing\n";
            MsgSyncRequest *notify = new MsgSyncRequest("SyncRequest");
            notify->setByteLength(notify->getMsgSize());
            send(notify, "netGate$o");
            syncSense = true;
            getParentModule()->bubble("Notifying");
            // Do nothing except change to synchronized state
        } else {
            // Add sensed measurement to collection
            mc.clear();
            for (std::list<Measurement>::iterator it=meaList.begin(); it != meaList.end(); ++it) {
                /* Add node's information to measurement object for simulation convenience.
                 * Note: Measurement object holds these information just for simulation programming convenience.
                 * In theory, these information is not packed with every object in a MsgSenseResult message,
                 * but is stored separately in the message so that the size of the message is not
                 * increased by the redundancy. However, each node adds these information to its
                 * own Measurement objects in case it may become CH, then the objects is carried by
                 * MsgSenseResult message (for programming convenience); so that the portion of
                 * code to add these information to objects in recvMessage() seem to be redundant,
                 * BUT IT'S NOT. The code is for demonstrating the full working mechanism. */
                mob = (Mobility*) getParentModule()->getSubmodule("mobility");
                ener = (Energy*) getParentModule()->getSubmodule("energy");
                (*it).setNodePosX(mob->getX());
                (*it).setNodePosY(mob->getY());
                (*it).setNodeEnergy(ener->getCapacity());

                // Add measurement object to collection
                mc.addMeasurement(*it);
            }
            tsSense = simTime(); // Punch time stamp for sense action

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
        Link802154 *link = check_and_cast<Link802154*>(getParentModule()->getSubmodule("link"));
        double addTime = par("senseInterval").doubleValue()
                        - getParentModule()->getSubmodule("ass")->par("responseDelay").doubleValue()
                        - (link->par("maxFrameSize").doubleValue() + link->par("phyHeaderSize").doubleValue()) / link->par("bitRate").doubleValue();
        cancelEvent(senseTimer);
        scheduleAt(simTime() + addTime, senseTimer);
    } else if (msg->getMsgType() == MSG_SENSE_RESULT) {
        MsgSenseResult *msr = check_and_cast<MsgSenseResult*>(msg);
        std::list<Measurement> ml = msr->getMeaList();

        // Add sender information to Measurement objects then add them to collection
        for (std::list<Measurement>::iterator it=ml.begin(); it != ml.end(); ++it) {
            // NOTE: THIS PORTION OF CODE IS NOT REDUNDANT. It demonstrates the working mechanism.
            (*it).setNodePosX(msr->getNodePosX());
            (*it).setNodePosY(msr->getNodePosY());
            (*it).setNodeEnergy(msr->getNodeEnergy());

            // Add Measurement object to collection
            mc.addMeasurement(*it);
        }
    }

    delete msg;
}

/*
 * Promote this node to CH (if appropriate). Then estimate targets' positions (if is CH).
 */
void AppSensor::trackTargets()
{
    std::list<TargetEntry>* el = mc.getEntryList(); // Entry list of collection
    std::list<TargetEntry>::iterator ite; // Iterator for entry list
    std::list<Measurement> ml; // Measurement list of a target
    std::list<Measurement>::iterator itm; // Iterator for measurement list
    std::list<Measurement>::iterator itmo; // Iterator for own measurement list of this node

    bool hasMea;
    Estimator *est = (Estimator*) getParentModule()->getSubmodule("est");
    double myCHValue; // Evaluated CH value of this node for a target
    bool isCH = false; // True if this node is CH of at least one target (global CH flag)
    std::list<TargetPos*> tpList;
    TargetPos *tp = NULL;

    double err = 0; // Estimation error
    StatCollector *sc = check_and_cast<StatCollector*>(getModuleByPath("Wsn.sc"));

    for (ite = el->begin(); ite != el->end(); ++ite) {
        // Check if this node has its own measurement of a target (in range of that target)
        hasMea = false;
        for (itmo = meaList.begin(); itmo != meaList.end(); ++itmo) {
            if ((*itmo).getTarId() == (*ite).tarId) hasMea = true;
        }
        if (!hasMea) continue;
        // If this node does not have any measurements of this target, skip it

        // Check if having enough measurements for a target
        ml = (*ite).meaList;
        if (ml.size() >= est->minNumMeasurement()) {
            // Promote this node to CH if appropriate
            (*ite).flagCH = true;
            // First measurement belongs to this node
            myCHValue = ml.front().getNodeEnergy() / ml.front().getMeasuredDistance();
            EV << "My measure " << ml.front().getMeasuredDistance() << '\n';
            EV << "my CH Value " << myCHValue << "\n";

            for (itm = ++ml.begin(); itm != ml.end(); ++itm) {
                EV << "other CH value " << (*itm).getNodeEnergy() / (*itm).getMeasuredDistance() << "\n";
                if (myCHValue < (*itm).getNodeEnergy() / (*itm).getMeasuredDistance()) {
                    (*ite).flagCH = false;
                    break;
                }
            }
        }
    }

    for (ite = el->begin(); ite != el->end(); ++ite) {
        if ((*ite).flagCH) {
            isCH = true; // This node is CH of at least one target
            // Estimate targets' positions
            tp = est->estimate((*ite).meaList);

            // If estimate complete successfully
            if (tp != NULL) {
                tp->setTarId((*ite).tarId); // TODO Label the position with target's ID
                //EV << "Estimated pos: " << tp.getX() << " ; " << tp.getY();
                tpList.push_back(tp);

                // Calculate error
                err = distance(tp->getX(), tp->getY(), (*ite).meaList.front().getTarPosX(),
                        (*ite).meaList.front().getTarPosY());
                sc->recEstError(err);
            }
        }
    }

    if (isCH) {
        getParentModule()->bubble("CH");
        // Create a tracking result message
        MsgTrackResult *msgTrackResult = new MsgTrackResult();
        msgTrackResult->setTpList(tpList);
        /* Set message size
         * Each TargetPos contains target ID + x + y will have size of 1 + 4 + 4 bytes */
        msgTrackResult->setMsgSize(msgTrackResult->getMsgSize() + 9 * tpList.size());
        msgTrackResult->setByteLength(msgTrackResult->getMsgSize());
        // Turn on strobe flag for this message
        msgTrackResult->setStrobeFlag(true);
        // Set time stamp
        msgTrackResult->setTsSense(tsSense);
        // Send result to base station
        send(msgTrackResult, "netGate$o");
    } else {
        getParentModule()->bubble("Non-CH");
    }
}

void AppSensor::setWorkMode(int mode)
{
    if (mode == WORK_MODE_OFF || mode == WORK_MODE_SLEEP || mode == WORK_MODE_ACTIVE) {
        workMode = mode;
        Link802154 *link = check_and_cast<Link802154*>(getParentModule()->getSubmodule("link"));

        switch (workMode) {
            case WORK_MODE_OFF:
                // Turn off transceiver
                link->setRadioMode(RADIO_FULL_OFF);
                // Cancel all timers
                cancelEvent(activeTimer);
                cancelEvent(sleepTimer);
                cancelEvent(senseTimer);
                cancelEvent(reportTimer);
                cancelEvent(collTimer);
                break;

            case WORK_MODE_SLEEP:
                // Turn off transceiver
                link->setRadioMode(RADIO_OFF);

                // Plan for waking up after sleepTime
                // If a working event occurs, wake up immediately and cancel this timer
                cancelEvent(activeTimer);
                scheduleAt(simTime() + par("sleepTime"), activeTimer);

                // Cancel sensing timer
                //cancelEvent(senseTimer);
                break;

            case WORK_MODE_ACTIVE:
                // Turn on transceiver
                link->setRadioMode(RADIO_ON);

                // Plan for sleeping after idleTime
                // If a working event occurs, cancel this timer and plan new one
                cancelEvent(sleepTimer);
                if (par("enableSleep").boolValue()) scheduleAt(simTime() + par("idleTime"), sleepTimer);

                // Start sensing immediately
                if (!senseTimer->isScheduled()) scheduleAt(simTime(), senseTimer);
                break;
        }
        updateDisplay();
    }
}

/*
 * Update display of sensor in simulation
 */
void AppSensor::updateDisplay()
{
    cDisplayString &ds = getParentModule()->getDisplayString();

    // Set color according to working mode
    switch (workMode) {
        case WORK_MODE_OFF:
            ds.setTagArg("i", 1, "black");
            break;
        case WORK_MODE_SLEEP:
            ds.setTagArg("i", 1, "yellow");
            break;
        case WORK_MODE_ACTIVE:
            ds.setTagArg("i", 1, "green");
            break;
    }
}
