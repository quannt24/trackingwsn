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
    #include "measurement.h"
    #include "targetpos.h"
    typedef std::list<Measurement> MeasurementList;
    typedef std::list<TargetPos*> TargetPosList;
}}
class noncobject Measurement; // announcement for the message compiler
class noncobject MeasurementList;
class noncobject TargetPosList;

packet MessageCR;

enum MessageType {
    MSG_SYNC_REQUEST = 0; // Nofity other nodes to synchronize sensing
    MSG_SENSE_RESULT = 1; // Raw measurements
    MSG_TRACK_RESULT = 2; // Tracked postions of targets
    MSG_CH_BEACON = 3; // CH's beacon for synchronizing sensing cycle
};

// Application level message for tracking. Extend MessageCR defined by network layer.
packet MsgTracking extends MessageCR {
    int msgType;
    int msgSize = 11; // Message size in byte. Default is 10 for routingType, desMacAddr, strobeFlag and msgType
}

// Nofity other nodes to synchronize sensing
packet MsgSyncRequest extends MsgTracking {
    int routingType = RT_BROADCAST;
    int msgType = MSG_SYNC_REQUEST;
    // No new field, use default size
}

// Message carries raw measurements 
packet MsgSenseResult extends MsgTracking {
    int routingType = RT_BROADCAST;
    int msgType = MSG_SENSE_RESULT;
    
    // List of measurements (size of this list does not include size of node info packed along with each object
    MeasurementList meaList;
    double nodePosX;
    double nodePosY;
    double nodeEnergy;
    
    // Message size should be calculated in application
}

// Message carries tracked positions of targets to send to base station
packet MsgTrackResult extends MsgTracking {
    int routingType = RT_TO_BS;
    int msgType = MSG_TRACK_RESULT;
    
    // List of tracked result (positions of targets)
    TargetPosList tpList;
    
    // Note: Message size should be calculated in application
    
    // Sense time stamp
    simtime_t tsSense;
}

// CH's beacon for synchronizing sensing cycle
packet MsgCHBeacon extends MsgTracking {
    int routingType = RT_BROADCAST;
    int msgType = MSG_CH_BEACON;
    // Use default size
}