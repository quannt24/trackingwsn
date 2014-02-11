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

// Network packet
packet PacketARPEES extends PacketCR {
}

// Relay request packet
packet PacketARPEES_RelayReq extends PacketARPEES {
    txType = TX_BROADCAST;
    pkType = PK_REQ_RELAY;
}

// Relay info packet
packet PacketARPEES_RelayInfo extends PacketARPEES {
    pkType = PK_RELAY_INFO;
    pkSize = 37; // plus 17 bytes of new fields
    
    bool bsFlag; // Flag on if sender is base station, 1 byte
    double energy; // Current energy, 4 bytes
    double posX; // Coordination x in meter, 4 bytes
    double posY; // Coordination y in meter, 4 bytes
    double dBS; // Distance to base station, 4 bytes
}
