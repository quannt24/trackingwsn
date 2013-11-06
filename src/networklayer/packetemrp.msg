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
    PK_ENERGY_INFO = 4; // Enegry info report after relay a packet
};

// EMRP network packet
packet PacketEMRP extends Packet802154 {
    int pkType; // Packet type
    
    int pkSize = 18; // 17 bytes from 3 fields of Packet802154 + 1 byte pkType
}

// EMRP relay info packet
packet PacketEMRP_RelayInfo extends PacketEMRP {
    pkType = PK_RELAY_INFO;
    bool bsFlag; // Flag on if sender is base station, 1 byte
    double energy; // Current energy, 4 bytes
    double posX; // Coordination x in meter
    double posY; // Coordination y in meter
    double dBS; // Distance to base station
    
    pkSize = 35; // 18 + 17
}

// EMRP energy report packet
packet PacketEMRP_EnergyInfo extends PacketEMRP {
    pkType = PK_ENERGY_INFO;
    double energy; // Current energy
    double consumedEnergy; // Predicted energy to send relayed packet
    
    pkSize = 26; // 18 + 8
}