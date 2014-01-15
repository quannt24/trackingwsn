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

#ifndef __TRACKINGWSN_NET_H_
#define __TRACKINGWSN_NET_H_

#include <omnetpp.h>

/**
 * Abstract module for network layer
 */
class Net : public cSimpleModule
{
    private:
        // This node MAC address. This field should not used directly, please use getMacAddr().
        int macAddr;

    protected:
        // Stored information about base station, these info must be set so that node can work properly.
        double bsPosX, bsPosY; // Base station position
        double dBS; // Distance from this node to base station

        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        /* Get MAC address from link layer. At initial stage, this address may not available. */
        int getMacAddr();
        /* Create a message to notify application layer that some events occur but the content is
         * not forwarded to application */
        void notifyApp();

    public:
        Net();
        ~Net();
        /* Store position of base station */
        void setBSPos(double x, double y);
};

#endif
