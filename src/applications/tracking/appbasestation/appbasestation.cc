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
#include <iostream>
#include <fstream>

Define_Module(AppBaseStation);

void AppBaseStation::initialize()
{
    cModule *wsn = simulation.getModuleByPath("Wsn");
    cModule *tar;
    int numTargets = wsn->par("numTargets").longValue();
    int i;
    cConfigurationEx *configEx = ev.getConfigEx();
    std::ostringstream oss;
    std::ofstream out;

    for (i = 0; i < numTargets; i++) {
        tar = wsn->getSubmodule("target", i);

        // Prepare output vectors
        TargetPosVectorSet *tpvs = new TargetPosVectorSet(tar->getId());
        tpvsList.push_back(tpvs);

        // Prepare output files
        oss.seekp(0);
        oss << "results/";
        oss << configEx->getActiveConfigName() << "_TrackedPath_ID" << tar->getId() << ".data\0";
        out.open(oss.str().c_str(), std::ios::out | std::ios::trunc);
        if (!out) {
            std::cerr << "Cannot open file " << oss.str() << endl;
        } else {
            out << "# Config: " << configEx->getActiveConfigName() << endl;
            out << "# Tracked path ID" << tar->getId() << endl;
            out << "# x y" << endl;
            out.close();
        }
    }

    // Turn on tranceiver
    Link802154 *link = check_and_cast<Link802154*>(getParentModule()->getSubmodule("link"));
    link->setRadioMode(RADIO_ON, false);

    // Prepare signal
    e2edelaySignal = registerSignal("e2e_delay");
}

void AppBaseStation::handleMessage(cMessage *msg)
{
    if (msg->getArrivalGate() == gate("netGate$i")) {
        MsgTracking *m = check_and_cast<MsgTracking*>(msg);
        recvMessage(m);
    }
}

AppBaseStation::~AppBaseStation()
{
    for (std::list<TargetPosVectorSet*>::iterator tpvsIt = tpvsList.begin(); tpvsIt != tpvsList.end(); tpvsIt++) {
        delete *tpvsIt;
    }
}

void AppBaseStation::recvMessage(MsgTracking *msg)
{
    if (msg->getMsgType() == MSG_TRACK_RESULT) {
        EV << "Tracking result\n";
        std::list<TargetPos*> tpList = ((MsgTrackResult*) msg)->getTpList();
        TargetPos *tp;
        TargetPosVectorSet *tpvs;

        for (std::list<TargetPos*>::iterator it = tpList.begin(); it != tpList.end(); it++) {
            tp = *it;

            // Record tracked target position
            EV << tp->getTarId() << ' ' << tp->getX() << ' ' << tp->getY() << '\n';
            for (std::list<TargetPosVectorSet*>::iterator tpvsIt = tpvsList.begin(); tpvsIt != tpvsList.end(); tpvsIt++) {
                tpvs = *tpvsIt;
                if (tpvs->getTarId() == tp->getTarId()) {
                    tpvs->record(tp->getX(), tp->getY());
                    outputTarPos(tp->getTarId(), tp->getX(), tp->getY());
                    break;
                }
            }
        }

        emit(e2edelaySignal, simTime() - ((MsgTrackResult*) msg)->getTsSense());
    }
    delete msg;
}

/* Output target position to file */
void AppBaseStation::outputTarPos(int tarId, double x, double y)
{
    cConfigurationEx *configEx = ev.getConfigEx();
    std::ostringstream oss;
    std::ofstream out;

    // Note: File name must be matched with the file prepared in initialize()
    oss << "results/";
    oss << configEx->getActiveConfigName() << "_TrackedPath_ID" << tarId << ".data\0";
    out.open(oss.str().c_str(), std::ios::app);

    if (!out) {
        std::cerr << "Cannot open file " << oss.str() << endl;
        return;
    } else {
        out << x << ' ' << y << endl;
        out.close();
    }
}
