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

package trackingwsn.base.layers;

// Base interface for every physical/link layer
moduleinterface ILink
{
    parameters:
        @display("i=block/layer");
        
        // Transmission range
        double txRange @unit(m);
        // Bit rate
        int bitRate @unit(bps);
        // Random frame lost probability (0 ~ 1)
        double ranFrameLossProb;
        
        // Frame configuration
        // =====================================
        // Max size of a link layer frame in byte, not including physical layer header
        int maxFrameSize;
        // Physical layer header size in byte (if this module simulates physical layer too)
        // Physical (transmited) frame size = phyHeaderSize + link frame size
        int phyHeaderSize;
        
        // Energy consumption
        // =====================================
        // Transmission range threshold. This parameter depends on environment and affects energy
        // to transmit data.
        double txRTh @unit(m);
        // Energy for encode/decode signal for 1 bit
        double e_elec; // Unit J/bit
        // Energy for amplifying signal for 1 bit when txRange < txRTh
        double e_fs; // Unit J/bit/m2
        // Time period for calculating energy consumed by transceiver listening
        // (for simulating continuously power consumtion)
        double rxConsumingPeriod;
        
    gates:
        inout netGate; // Connect with network layer
        input radioIn @directIn; // Receive wireless packet
}