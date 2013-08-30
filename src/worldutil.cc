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

#include "worldutil.h"
#include "mobility.h"

Define_Module(WorldUtil);

void WorldUtil::initialize()
{
    arrangeNodes();
}

void WorldUtil::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

/*
 * Arrange position of nodes in network.
 */
void WorldUtil::arrangeNodes()
{
    // Arrange sensors
    cModule *wsn = simulation.getModuleByPath("Wsn");
    double wsnWidth = wsn->par("width");
    double wsnHeight = wsn->par("height");
    int ssRows = wsn->par("ssRows");
    int ssCols = wsn->par("ssCols");
    int numSensors = wsn->par("numSensors");
    cCompoundModule *ss;
    Mobility *mob;

    int i = 0;
    int n = (numSensors <= ssRows * ssCols) ? numSensors : ssRows * ssCols; // Number of sensors which will be arranged
    for (i = 0; i < n; i++) {
        ss = check_and_cast<cCompoundModule*>(wsn->getSubmodule("sensor", i));
        mob = check_and_cast<Mobility*>(ss->getSubmodule("mobility"));
        mob->setX((i % ssCols) * rint(wsnWidth / ssCols) + intuniform(0, wsnWidth / ssCols / 2));
        mob->setY((i / ssCols) * rint(wsnHeight / ssRows) + intuniform(0, wsnHeight / ssRows / 2));
        mob->updateDisplay(); // Update display of sensor
    }
}

/*
 * Calculate distance between two (objects containing) mobility modules.
 */
double distance(Mobility *mob1, Mobility *mob2)
{
    return sqrt((mob1->getX() - mob2->getX()) * (mob1->getX() - mob2->getX())
            + (mob1->getY() - mob2->getY()) * (mob1->getY() - mob2->getY()));
}
