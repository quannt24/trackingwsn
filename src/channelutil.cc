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

#include "channelutil.h"
#include "link802154.h"
#include "mobility.h"
#include "worldutil.h"

Define_Module(ChannelUtil);

void ChannelUtil::initialize()
{
    // TODO - Generated method body
}

void ChannelUtil::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

ChannelUtil::ChannelUtil()
{
}

ChannelUtil::~ChannelUtil()
{
    hostList.clear();
}

/*
 * Check if channel around a host is busy or not.
 * This function will check if the issuing host is in Tx range of any other host which has already
 * acquired the channel; if it is the case, this function will return false. List of acquiring
 * channel hosts is maintained by the ChannelUtil object.
 * Return 'true' if channel is available and 'false' if channel is busy.
 */
bool ChannelUtil::checkChannel(Link802154 *host)
{
    bool available = true; // Channel is available or not
    Link802154 *acqHost = NULL; // Pointer to a acquiring channel host
    Mobility *mob1 = (Mobility*) host->getParentModule()->getSubmodule("mobility");
    Mobility *mob2;

    for (std::list<Link802154*>::iterator it = hostList.begin(); it != hostList.end(); it++) {
        acqHost = *it;
        mob2 = (Mobility*) acqHost->getParentModule()->getSubmodule("mobility");
        if (host != acqHost && distance(mob1, mob2) <= acqHost->par("txRange").doubleValue()) {
            available = false;
            break;
        }
    }

    return available;
}

/*
 * Acquire channel to transmit data. This function will perform a checkChannel() before acquiring
 * the channel to issuing host. If the check is passed, all hosts which is in range of the issuing
 * host will be removed from the acquiring list of the ChannelUtil object; this is for programming
 * purpose only, because in reality, the channel will be busy the next time these hosts sense.
 * If there are some hosts kicked out (usually when issuing host has larger txRange than them),
 * they may still transmitting data and collisions may occurs. TODO Collisions simulation is not
 * implemented.
 */
int ChannelUtil::acquireChannel(Link802154 *host)
{
    if (host == NULL) return -1;
    if (!checkChannel(host)) return 1;

    Link802154 *acqHost = NULL; // Pointer to a acquiring channel host
    bool flagIn = false; // flagIn = true when issuing host has been already in list
    Mobility *mob1 = (Mobility*) host->getParentModule()->getSubmodule("mobility");
    Mobility *mob2;

    // Check if issuing host has been already in list, and kick out all other hosts in its range
    for (std::list<Link802154*>::iterator it = hostList.begin(); it != hostList.end(); it++) {
        acqHost = *it;
        mob2 = (Mobility*) acqHost->getParentModule()->getSubmodule("mobility");
        if (acqHost == host) flagIn = true;
        if (host != acqHost && distance(mob1, mob2) <= host->par("txRange").doubleValue()) {
            hostList.remove(acqHost);
        }
    }

    if (!flagIn) {
        hostList.insert(hostList.end(), host);
    }
    return 0;
}

/*
 * Remove host from the list of acquiring channel hosts.
 */
void ChannelUtil::releaseChannel(Link802154 *host)
{
    if (host == NULL) return;
    Link802154 *acqHost = NULL; // Pointer to a acquiring channel host

    for (std::list<Link802154*>::iterator it = hostList.begin(); it != hostList.end(); it++) {
        acqHost = *it;
        if (acqHost == host) {
            hostList.remove(acqHost);
        }
    }
}