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

cplusplus {{
#include "packet802154_m.h"
}}

packet Packet802154;

enum PacketType {
    PK_PAYLOAD_TO_AN = 0; // Packet to be transmited to adjacent node (cluster head or a specific node)
    PK_PAYLOAD_TO_BS = 1; // Packet to be transmited to base station
    PK_REQ_RELAY = 2; // Request for relay info
    PK_RELAY_INFO = 3; // Relay info to response the request
};

// EMRP network packet
packet PacketEMRP extends Packet802154 {
    int pkType; // Packet type
}

// EMRP relay info packet
packet PacketEMRP_RelayInfo extends PacketEMRP {
    bool bsFlag; // Flag on if sender is base station
    double energy; // Current energy
    double posX; // Coordination x in meter
    double posY; // Coordination y in meter
    double dBS; // Distance to base station
}