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

package trackingwsn.linklayer.link802154;

import trackingwsn.base.layers.ILink;

//
// Phy/Link layer using IEEE 802.15.4
//
simple Link802154 like ILink
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
        
        // Max size of a link layer frame in byte, not including physical layer header
        int maxFrameSize = default(127);
        // Physical layer header size in byte
        // Physical (transmited) frame size = phyHeaderSize + link frame size
        int phyHeaderSize = default(6);
        
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
		double symPeriod = default(0.000016); // Symbol period
		double ccaDur = 8 * symPeriod;
		int aMaxBE = default(5);
		double aUnitBP = 20 * symPeriod; // A unit backoff-period
		int macMaxNB = default(4); // 0 ~ 5
		int macMinBE = default(3); // 0 ~ 3
		
		// X-MAC duty cycling
		// Enable/Disable X-MAC. If disabled, even strobe flag of packet will be ignored.
		bool enableXmac = default(true);
		// Receiver
		// Listen period, must long enough to receive one of two consecutive strobes
		double lR @unit(s) = default(0.003s);
		// Sleep period (radio totally off), causes delay
		double sR @unit(s) = default(0.1s);
		// Sender
		// Strobe period: time between sending 2 strobes. Must be long enough for receiving strobe
		// ACK and short enough so that current strobe and next strobe fit in one listen period (lR)
		// Transmit 27 bytes over 250kbps channel takes 0.000864s ~> 0.001s
		double strobePeriod @unit(s) = default(0.002s);

        @display("i=block/wrxtx");

    gates:
        inout netGate; // Connect with network layer
        input radioIn @directIn; // Receive wireless packet
}
