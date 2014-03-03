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

package trackingwsn.linklayer.linkxtmac;

import trackingwsn.linklayer.link802154.Link802154;

//
// Phy/Link layer based on IEEE 802.15.4 and XT-MAC
//
simple LinkXTMAC extends Link802154
{
    parameters:
        @class("LinkXTMAC");
        
		// X-MAC duty cycling
		// Receiver
		// Listen period (seconds), must long enough to receive one of two consecutive strobes
		// TODO This period must contain delay for turning on radio (about 2.5ms, in this time,
		// transceiver has not been able to receive)
		double lR @unit(s) = default(0.0055s);
		// Sleep period (radio totally off), causes delay (seconds)
		double sR @unit(s) = default(0.1s);
		// Sender
		// Strobe period: time between sending 2 strobes. Must be long enough for receiving strobe
		// ACK and short enough so that current strobe and next strobe fit in one listen period (lR)
		// Transmit 27 bytes over 250kbps channel takes 0.000864s ~> 0.001s
		double strobePeriod @unit(s) = default(0.002s);
        // Active time length
        double activeTime @unit(s) = default(2s);
}
