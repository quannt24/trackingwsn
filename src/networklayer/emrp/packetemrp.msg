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
#include "packetcr_m.h"
}}

packet PacketCR;

// EMRP network packet
packet PacketEMRP extends PacketCR {
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
    double consumedEnergy; // Predicted energy to send relayed packet
    
    pkSize = 22; // 18 + 4
}