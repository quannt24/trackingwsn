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

package trackingwsn;

//
// Network layer using EMRP protocol
//
simple NetEMRP
{
    parameters:
        bool isBaseStation = default(false);
        // Period for broadcast first relay request. If this period is too short, channel may be
        // too busy and some nodes may fail to complete initializing procedure. The warm-up period
        // will be dependent to this period.
        double initPeriod = default(20);
        // Period for responsing for a relay request
        double resRelayPeriod = default(2);
        // Waiting for relay info timeout. This timeout is used when a message is needed to be sent
        // to BS, but relay info is not ready (may be due to incomplete initializing stage). A request
        // for relay info will be sent and retry to send the message after this timeout. If retry is
        // unsuccessful (relay info is still not ready), this transmision is fail.
        double waitRelayTimeout = default(0.1);
        // Differential amount of relay/backup nodes' energy that triggers switch nodes
        double switchingEnergy = default(0.1);
        // Critical energy level that a node cannot guarantees reliable transmission
        double criticalEnergy = default(0.2);
        
        @display("i=block/layer");
    gates:
        inout appGate;
        inout linkGate;
}
