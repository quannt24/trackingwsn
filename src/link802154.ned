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
// Phy/Link layer using IEEE 802.15.4
//
simple Link802154
{
    parameters:
        double txRange @unit(m) = default(10m); // Transmission range
        int maxFrameSize = default(127); // Max size of a link layer frame in byte, not including physical layer header
        int phyHeaderSize = default(6); // Physical layer header size in byte
        // Physical (transmited) frame size = phyHeaderSize + link frame size
        
        // Size of fixed segments in a frame in byte
        int segFrameControl = default(2);
        int segSequenceId = default(1);
        int segAddr = default(8); // Short address ~ 2 bytes; Long address ~ 8 bytes
        // Security segment is ignored
        int segFooter = default(2);
        
        // Max packet size for upper layer
        int maxPacketSize = maxFrameSize - segFrameControl - segSequenceId - segAddr - segFooter;
        
        @display("i=block/wrxtx");

    gates:
        inout netGate; // Connect with network layer
        input radioIn; // Receive wireless packet
}
