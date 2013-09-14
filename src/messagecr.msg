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

enum MsgType {
    MSG_TO_AN = 0; // Message to adjacent node
    MSG_TO_BS = 1; // Message to base station
};

//
// Application level message (actually a packet) working with network layer which is
// implementing a cluster routing protocol
//
packet MessageCR {
    int msgType; // Message type
    int desMacAddr; // Destination MAC address, use when msgType is MSG_TO_AN
}