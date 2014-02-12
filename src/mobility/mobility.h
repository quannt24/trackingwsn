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

#define MAX_PATH_LEN 10000

/**
 * Provide logic about position and mobility of an Entity
 */
class Mobility : public cSimpleModule
{
    private:
        double x, y; // Position (coordinate in meter), used to place module on inspector
        int row, col; // Position in row/column (just for reference only, not for actual drawing)
        cMessage *moveMsg; // Self message to move target to new position
        double *xArr; // Path array storing x coordinates
        double *yArr; // Path array storing y coordinates
        int pathLen; // Number of point in path
        int posId; // Current index in path array
        int loadPath(); // Load path from file to coordinate arrays
        cOutVector *vecPosX = NULL;
        cOutVector *vecPosY = NULL;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        Mobility();
        ~Mobility();
        double getX();
        void setX(double x);
        double getY();
        void setY(double y);
        int getRow();
        void setRow(int r);
        int getCol();
        void setCol(int c);
        void updateDisplay(); // Update display of parent module in simulation
};

#endif
