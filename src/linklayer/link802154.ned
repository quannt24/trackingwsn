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

package trackingwsn.linklayer;

//
// Phy/Link layer using IEEE 802.15.4
//
simple Link802154
{
    parameters:
        // Transmission range threshold. This parameter depends on environment and affects energy to transmit data.
        double txRTh @unit(m) = default(50m);
        // Energy for encode/decode signal for 1 bit
        double e_elec = default(50e-9); // Unit J/bit
        // Energy for amplifying signal for 1 bit when txRange < txRTh
        double e_fs = default(10e-12); // Unit J/bit/m2
        // Time period for calculating energy consumed by transceiver listening
        // (for simulating continuously power consumtion)
        double rxConsumingPeriod = default(0.5);
        
        // Transmission range
        double txRange @unit(m) = default(40m);
        // Bit rate
        int bitRate @unit(bps) = default(250000bps);
        // Random frame lost probability (0 ~ 1)
        double ranFrameLossProb = default(0.0);
        
        int maxFrameSize = default(127); // Max size of a link layer frame in byte, not including physical layer header
        int phyHeaderSize = default(6); // Physical layer header size in byte
        // Physical (transmited) frame size = phyHeaderSize + link frame size
        
        // Size of MAC layer fields in a frame in byte
        int fldFrameControl = default(2);
        int fldSequenceId = default(1);
        int fldSrcAddr = default(8); // Sender address. Short address ~ 2 bytes; Long address ~ 8 bytes
        int fldDesAddr = default(8); // Receiver address
        // Security segment is ignored
        int fldFooter = default(2);
        // Total: 21 bytes => physical frame size = 27 bytes
        
        // Max packet size for upper layer
        int maxPacketSize = maxFrameSize - fldFrameControl - fldSequenceId - fldDesAddr - fldSrcAddr - fldFooter;

		// Unslotted CSMA/CA parameters
		int macMinBE = default(3);
		int aMaxBE = default(5);
		int aMaxNB = default(5);
		
		// X-MAC duty cycling (RADIO_OFF mode)
		// Receiver
		double lR = default(0.005); // Listen period, must long enough to receive one of two consecutive strobes
		double sR = default(0.1); // Sleep period (radio totally off), causes delay
		// Sender
		// Strobe period: time between sending 2 strobes. Must be long enough for receiving strobe
		// ACK and short enough so that current strobe and next strobe fit in one listen period (lR)
		// Transmit 27 bytes over 250kbps channel takes 0.000864s ~> 0.001s
		double strobePeriod = default(0.003);

        @display("i=block/wrxtx");

    gates:
        inout netGate; // Connect with network layer
        input radioIn @directIn; // Receive wireless packet
}
