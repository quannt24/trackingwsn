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
#include "link802154.h"
#include "netemrp.h"

Define_Module(WorldUtil);

void WorldUtil::initialize()
{
    // Arrange nodes into positions
    arrangeNodes();
    // Create connections
    connectNodes();
}

void WorldUtil::handleMessage(cMessage *msg)
{
}

/*
 * Arrange position of nodes in network.
 */
void WorldUtil::arrangeNodes()
{
    // Arrange sensors
    cModule *wsn = getModuleByPath("^");
    double wsnWidth = wsn->par("width");
    double wsnHeight = wsn->par("height");
    int ssRows = wsn->par("ssRows");
    int ssCols = wsn->par("ssCols");
    int numSensors = wsn->par("numSensors");
    cModule *ss;
    Mobility *mob;

    int i = 0;
    int n = (numSensors <= ssRows * ssCols) ? numSensors : ssRows * ssCols; // Number of sensors which will be arranged
    for (i = 0; i < n; i++) {
        ss = wsn->getSubmodule("sensor", i);
        mob = check_and_cast<Mobility*>(ss->getSubmodule("mobility"));
        mob->setX((i % ssCols) * rint(wsnWidth / ssCols) + intuniform(0, wsnWidth / ssCols / 2));
        mob->setY((i / ssCols) * rint(wsnHeight / ssRows) + intuniform(0, wsnHeight / ssRows / 2));
        mob->setRow(i / ssCols);
        mob->setCol(i % ssCols);
        mob->updateDisplay(); // Update display of sensor
    }
}

/*
 * Connect adjacent nodes. The setup phase of the network is not simulated,
 * therefore, this function helps each node have a connection list.
 */
void WorldUtil::connectNodes()
{
    cModule *wsn = getModuleByPath("^");
    cModule *ss1, *ss2, *bs;
    Mobility *mob1, *mob2;
    Link802154 *link1, *link2;
    NetEMRP *net1;
    int n = wsn->par("numSensors").longValue();
    int i, j;
    double d;

    // Add adjacent nodes of base station first
    bs = wsn->getSubmodule("bs");
    mob2 = (Mobility*) bs->getSubmodule("mobility");
    link2 = (Link802154*) bs->getSubmodule("link");
    for (i = 0; i < n; i++) {
        if (link2->isFullConn()) break;

        ss1 = wsn->getSubmodule("sensor", i);

        // Store information about base station to sensors
        net1 = (NetEMRP*) ss1->getSubmodule("net");
        net1->setBSPos(mob2->getX(), mob2->getY());

        // Connect sensors with base station
        mob1 = (Mobility*) ss1->getSubmodule("mobility");
        link1 = (Link802154*) ss1->getSubmodule("link");
        if (link1->isFullConn()) continue;

        d = distance(mob1, mob2);
        if (d <= link1->par("txRange").longValue() && d <= link2->par("txRange").longValue()) {
            link1->addAdjNode(link2->getAddr());
            link2->addAdjNode(link1->getAddr());
        }
    }

    // Connect sensor nodes
    for (i = 0; i < n - 1; i++) {
        ss1 = wsn->getSubmodule("sensor", i);

        mob1 = (Mobility*) ss1->getSubmodule("mobility");
        link1 = (Link802154*) ss1->getSubmodule("link");
        if (link1->isFullConn()) continue;

        for (j = i + 1; j < n; j++) {
            ss2 = wsn->getSubmodule("sensor", j);

            mob2 = (Mobility*) ss2->getSubmodule("mobility");
            link2 = (Link802154*) ss2->getSubmodule("link");
            if (link2->isFullConn()) continue;

            d = distance(mob1, mob2);
            if (d <= link1->par("txRange").longValue() && d <= link2->par("txRange").longValue()) {
                link1->addAdjNode(link2->getAddr());
                link2->addAdjNode(link1->getAddr());
            }
        }
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

/*
 * Calculate distance between two coordinates
 */
double distance(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
