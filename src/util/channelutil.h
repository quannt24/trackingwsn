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

#ifndef __TRACKINGWSN_CHANNELUTIL_H_
#define __TRACKINGWSN_CHANNELUTIL_H_

#include "link802154.h"
#include <omnetpp.h>

/**
 * Utility module for checking status of wireless channel, used to simulate channel sensing. Each
 * instance of this module can be used to track the using of a channel (just like a register table,
 * no mechanism to control the behaviors of other modules is provided).
 * Other modules should register with this module if they are going to use/release channel; and
 * they can check the status of the channel.
 */
class ChannelUtil : public cSimpleModule
{
    private:
        std::list<Link802154*> hostList;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        ChannelUtil();
        ~ChannelUtil();
        /* Check if channel around a host is busy or not: true = available, false = busy */
        bool checkChannel(Link802154 *host);
        /* Check if channel around a host has collisions. Collisions occur when two or more other
         * hosts' signals are transmitted in this area. This function can be used for issued host
         * when it is receiving packet to simulate packet lost. */
        bool hasCollision(Link802154 *host);
        /* Acquire channel to transmit data. Return 0 on success. */
        int acquireChannel(Link802154 *host);
        /* Release channel acquired by a host */
        void releaseChannel(Link802154 *host);
};

#endif
