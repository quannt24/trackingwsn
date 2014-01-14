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

#ifndef __TRACKINGWSN_APPBASESTATION_H_
#define __TRACKINGWSN_APPBASESTATION_H_

#include "app.h"
#include "messagetracking_m.h"
#include "targetposvectorset.h"
#include <omnetpp.h>

/**
 * Base station's Application Layer
 */
class AppBaseStation : public App
{
    private:
        std::list<TargetPosVectorSet*> tpvsList;
        simsignal_t e2edelaySignal;

        void recvMessage(MsgTracking *msg);

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        ~AppBaseStation();
};

#endif
