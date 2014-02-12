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

package trackingwsn.networklayer.emrp;

import trackingwsn.networklayer.net.Net;

//
// Network layer using EMRP protocol
//
simple NetEMRP extends Net
{
    parameters:
        // Period for responsing for a relay request in initial period
        double resRelayPeriod = default(0.1);
        // Timeout for Waiting for packet of energy update of relay node.
        // If updating info is not received before timeout, relay node will be changed (considered
        // as incident happended).
        // This timeout must long enough for max CSMA/CA backoff time (of two nodes) plus sending
        // time of relayed packet and energy info packet as:
        // t = maxBackoff * 2 + maxXMACstrobeTime + sendingTime(relay packet + energy info packet)
        double waitEnergyInfoTimeout = default(0.1);
        // Differential amount of relay/backup nodes' energy that triggers switch nodes
        double switchingEnergy @unit(J)= default(0.1J);
        // Critical energy level that a node cannot guarantees reliable transmission
        double criticalEnergy @unit(J) = default(0.2J);
        
        @class("NetEMRP");
        @display("i=block/layer");
}
