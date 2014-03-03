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

package trackingwsn.linklayer.linkbmac;

import trackingwsn.linklayer.link802154.Link802154;

//
// Phy/Link layer based on IEEE 802.15.4 and B-MAC
//
simple LinkBMAC extends Link802154
{
    parameters:
        @class("LinkBMAC");
        
        // Size of MAC layer fields in a frame in byte (only for payload frames)
        int fldPreamble = 8;
        int fldSync = 2;
        int fldHeader = 5;
        fldFooter = 2;
        fldSrcAddr = 8; // Sender address. Short address ~ 2 bytes; Long address ~ 8 bytes
        fldDesAddr = 8; // Receiver address

		// Listen period
        double lR @unit(s) = default(0.003s);
        // Sleep period aka check interval (radio totally off), causes delay
        double sR @unit(s) = default(0.1s);
        // Preamble length in bytes should equal 'byteRoundup(bitRate * (sR + lR))' 
        int preambleByteLen = 3219;
        // Active time length
        double activeTime @unit(s) = default(2s);
}
