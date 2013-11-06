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
#include <sstream>
#include <iostream>
#include <fstream>

Define_Module(AppBaseStation);

void AppBaseStation::initialize()
{
    trackResultSignal = registerSignal("TrackResult");
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
        for (std::list<TargetPos>::iterator it = tpList.begin(); it != tpList.end(); it++) {
            tp = (*it);
            EV << tp.getTarId() << ' ' << tp.getX() << ' ' << tp.getY() << '\n';
            // Output tracking result to file
            output(tp.getTarId(), tp.getX(), tp.getY());
        }
    }
    delete msg;
}

void AppBaseStation::output(int tarId, double x, double y)
{
    using namespace std;

    stringstream ssOut;
    ssOut << "output/bs-output-" << tarId << ".txt";

    ofstream out(ssOut.str().c_str(), ios::app);
    if (!out) {
        cerr << "BaseStation: Cannot output to file\n";
        return;
    }
    out << x << ' ' << y << '\n';
    out.close();
}
