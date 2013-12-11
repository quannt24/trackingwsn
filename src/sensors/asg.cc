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

#include "asg.h"
#include "sensedsignal_m.h"
#include "msgkind.h"
#include "sensereq_m.h"
#include "mobility.h"
#include "worldutil.h"

Define_Module(Asg);

void Asg::initialize()
{
}

void Asg::handleMessage(cMessage *msg)
{
    SenseReq *req = check_and_cast<SenseReq*>(msg);
    Mobility *mob = check_and_cast<Mobility*>(getParentModule()->getSubmodule("mobility"));

    // Create signal message and send back to sensor
    SensedSignal *sig = new SensedSignal("SensedSignal", SS_SIGNAL);
    sig->setTarId(getParentModule()->getId());
    sig->setDistance(distance(mob, req->getSrcMob()));
    sig->setX(mob->getX());
    sig->setY(mob->getY());
    // TODO Add signal string for analyzing

    sendDirect(sig, req->getSrcAss(), "sigIn");

    // Delete requesting message
    delete msg;
}
