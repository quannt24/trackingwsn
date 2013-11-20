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

#include "appbasestation.h"
#include "link802154.h"
#include <sstream>
#include <iostream>
#include <fstream>

Define_Module(AppBaseStation);

AppBaseStation::~AppBaseStation()
{
    for (std::list<TargetPosVectorSet*>::iterator tpvsIt = tpvsList.begin(); tpvsIt != tpvsList.end(); tpvsIt++) {
        delete *tpvsIt;
    }
}

void AppBaseStation::initialize()
{
    cModule *wsn = simulation.getModuleByPath("Wsn");
    cModule *tar;
    int i;

    // Prepare output vectors
    for (i = 0; i < wsn->par("numTargets").longValue(); i++) {
        tar = wsn->getSubmodule("target", i);
        TargetPosVectorSet *tpvs = new TargetPosVectorSet(tar->getId());
        tpvsList.push_back(tpvs);
    }

    // Turn on tranceiver
    Link802154 *link = check_and_cast<Link802154*>(getParentModule()->getSubmodule("link"));
    link->setRadioMode(RADIO_ON, false);
}

void AppBaseStation::handleMessage(cMessage *msg)
{
    if (msg->getArrivalGate() == gate("netGate$i")) {
        MsgTracking *m = check_and_cast<MsgTracking*>(msg);
        recvMessage(m);
    }
}

void AppBaseStation::recvMessage(MsgTracking *msg)
{
    if (msg->getMsgType() == MSG_TRACK_RESULT) {
        EV << "Tracking result\n";
        std::list<TargetPos> tpList = ((MsgTrackResult*) msg)->getTpList();
        TargetPos tp;
        TargetPosVectorSet *tpvs;

        for (std::list<TargetPos>::iterator it = tpList.begin(); it != tpList.end(); it++) {
            tp = *it;

            // Record tracked target position
            EV << tp.getTarId() << ' ' << tp.getX() << ' ' << tp.getY() << '\n';
            for (std::list<TargetPosVectorSet*>::iterator tpvsIt = tpvsList.begin(); tpvsIt != tpvsList.end(); tpvsIt++) {
                tpvs = *tpvsIt;
                if (tpvs->getTarId() == tp.getTarId()) {
                    tpvs->record(tp.getX(), tp.getY());
                    break;
                }
            }
        }
    }
    delete msg;
}
