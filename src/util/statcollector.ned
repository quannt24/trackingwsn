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

package trackingwsn.util;

//
// Statistics Collector
//
simple StatCollector
{
    parameters:
        double pollTSEInterval = default(0.5);

        @display("i=block/table2");

        @signal[total_sensor_energy](type="double");
        @statistic[totalEnergy](title="Total energy"; source="total_sensor_energy"; record=vector);

        @signal[est_err](type="double");
        @statistic[estimationError](title="Estimation error"; source="est_err"; record=vector);

        @signal[recv_frame](type="int");
        @statistic[numRecvFrame](title="Number of received frames"; source="recv_frame"; record=last);

        @signal[lost_frame](type="int");
        @statistic[numLostFrame](title="Number of lost frames"; source="lost_frame"; record=last);

        @signal[recv_packet](type="int");
        @statistic[numRecvPacket](title="Number of received packets"; source="recv_packet"; record=last);

        @signal[lost_packet](type="int");
        @statistic[numLostPacket](title="Number of lost packets"; source="lost_packet"; record=last);
        
        @signal[created_mtr](type="int");
        @statistic[numCreatedMTR](title="Number of created MsgTrackResult"; source="created_mtr"; record=last);
        
        @signal[recv_mtr](type="int");
        @statistic[numRecvMTR](title="Number of received MsgTrackResult"; source="recv_mtr"; record=last);
        
        @signal[lost_mtr_link](type="int");
        @statistic[numLostMTRbyLink](title="Number of lost MsgTrackResult by link layer"; source="lost_mtr_link"; record=last);
        
        @signal[lost_mtr_net](type="int");
        @statistic[numLostMTRbyNet](title="Number of lost MsgTrackResult by network layer"; source="lost_mtr_net"; record=last);
        
        @signal[num_meas_ch](type="int");
        @statistic[numMeasCH](title="Number of measurement used by CH"; source="num_meas_ch"; record=vector,histogram);
}
