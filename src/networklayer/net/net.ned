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

package trackingwsn.networklayer.net;

import trackingwsn.networklayer.INet;

//
// Abstract module for network layer
//
simple Net like INet
{
    parameters:
        bool isBaseStation = default(false);
        // This parameter is used for protocol which needs an initialization stage at start of
        // simulation. This value is set to 0 by default.
        // Period for broadcast first relay request. If this period is too short, channel may be
        // too busy and some nodes may fail to complete initializing procedure. The warm-up period
        // will be dependent to this period.
        double initInterval = default(0);

		@class("Net");        
        @display("i=block/layer");
    gates:
        inout appGate;
        inout linkGate;
}
