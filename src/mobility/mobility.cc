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

#include "mobility.h"
#include <fstream>

Define_Module(Mobility);

void Mobility::initialize()
{
    if (par("moving").boolValue()) {
        // Load path from file
        pathLen = loadPath();
        if (pathLen < 1) {
            EV<< "Target: path error";
            return;
        }

        posId = 0;
        setX(xArr[posId]);
        setY(yArr[posId]);

        // Schedule first move
        scheduleAt(par("startMovingTime").doubleValue(), moveMsg);
    } else {
        // Static entity
        setX(par("posX"));
        setY(par("posY"));
    }

    // Update display
    updateDisplay();
}

void Mobility::handleMessage(cMessage *msg)
{
    // Self message: move
    // Move target to next position in path array
    posId++;
    setX(xArr[posId]);
    setY(yArr[posId]);
    updateDisplay();
    if (posId < pathLen - 1) scheduleAt(simTime() + par("movingTimeStep").doubleValue(), msg);
}

Mobility::Mobility()
{
    moveMsg = new cMessage("MoveMsg");
    xArr = new double[MAX_PATH_LEN];
    yArr = new double[MAX_PATH_LEN];
}

Mobility::~Mobility()
{
    cancelAndDelete(moveMsg);
    delete xArr;
    delete yArr;
}

double Mobility::getX()
{
    return x;
}

void Mobility::setX(double x)
{
    this->x = x;
}

double Mobility::getY()
{
    return y;
}

void Mobility::setY(double y)
{
    this->y = y;
}

/*
 * Load moving path from file.
 * Return number of coordinates read, -1 on error.
 */
int Mobility::loadPath()
{
    std::ifstream in(par("pathFile"));
    if (!in) {
        EV << "Cannot open path file " << par("pathFile").stringValue() << "\n";
        return -1;
    }

    int n = 0;
    while (!in.eof() && n < MAX_PATH_LEN) {
        in >> xArr[n];
        in >> yArr[n];
        n++;
    }

    return n;
}

/*
 * Update display of parent module in simulation
 */
void Mobility::updateDisplay()
{
    cDisplayString &ds = getParentModule()->getDisplayString();

    // Update displayed position according to object's coordination (x,y)
    ds.setTagArg("p", 0, x);
    ds.setTagArg("p", 1, y);
}
