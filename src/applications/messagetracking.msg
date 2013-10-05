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
	#include "messagecr_m.h"
}}

packet MessageCR;

enum MessageType {
    MSG_SENSE_NOTIFY = 0; // Nofity other node about target existence
    MSG_SENSE_RESULT = 1; // Raw measurements
};

// Application level message for tracking
packet MsgTracking extends MessageCR {
    int msgType;
    int msgSize = 10; // Message size in byte. Default is 10 for routingType, desMacAddr and msgType
}

//
// Nofity other node about target existence
//
packet MsgSenseNotify extends MsgTracking {
    int routingType = RT_BROADCAST;
    int msgType = MSG_SENSE_NOTIFY;
    // No new field, use default size
}

//
// Message carries raw measurements
//
packet MsgSenseResult extends MsgTracking {
    int routingType = RT_BROADCAST;
    int msgType = MSG_SENSE_RESULT;
}