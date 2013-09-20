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

#ifndef __TRACKINGWSN_WORLDUTIL_H_
#define __TRACKINGWSN_WORLDUTIL_H_

#include <omnetpp.h>
#include "mobility.h"

/**
 * Utilities about simulation world
 */
class WorldUtil : public cSimpleModule
{
    private:
        void arrangeNodes(); // Arrange nodes in positions
        void connectNodes(); // Connect adjacent nodes

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

double distance(Mobility *mob1, Mobility *mob2);
double distance(double x1, double y1, double x2, double y2);

#endif
