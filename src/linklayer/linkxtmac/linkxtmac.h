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

#ifndef __TRACKINGWSN_LINKXTMAC_H_
#define __TRACKINGWSN_LINKXTMAC_H_

#include "link802154.h"
#include <omnetpp.h>

/**
 * Phy/Link layer IEEE 802.15.4, CSMA-CA, X-MAC. Support radio mode switching.
 */
class LinkXTMAC : public Link802154
{
    protected:
        virtual void handleMessage(cMessage *msg);

        virtual void queueFrame(Frame802154 *frame);
        virtual void recvFrame(Frame802154 *frame);

        // X-MAC
        virtual void prepareSending();
        virtual void prepareStrobe();
        virtual void sendStrobeAck(Frame802154 *strobe);
        virtual void finishSending();

    public:
        int getRadioMode() { return radioMode; };
        /* Set radio mode with a duty cycling flag. If the flag is true, it's considered this
         * function is called by duty cycling and a sleep timer is set if mode is on. When mode is
         * off, a listen timer is always set. Default value for the flag is false for normal use. */
        void setRadioMode(int mode, bool dutyCycling = false);
};

#endif
