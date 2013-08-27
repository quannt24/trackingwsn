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

#ifndef __TRACKINGWSN_MOBILITY_H_
#define __TRACKINGWSN_MOBILITY_H_

#include <omnetpp.h>

#define MAX_PATH_LEN 1000

/**
 * Provide logic about position and mobility of an Entity
 */
class Mobility : public cSimpleModule
{
    private:
        int x, y; // Position
        cMessage *moveMsg; // Self message to move target to new position
        int *xArr; // Path array storing x coordinates
        int *yArr; // Path array storing y coordinates
        int pathLen; // Number of point in path
        int posId; // Current index in path array
        int loadPath(); // Load path from file to coordinate arrays

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        Mobility();
        ~Mobility();
        int getX();
        void setX(int x);
        int getY();
        void setY(int y);
        void updateDisplay(); // Update display of parent module in simulation
};

#endif
