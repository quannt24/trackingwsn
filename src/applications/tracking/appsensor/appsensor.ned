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

package trackingwsn.applications.tracking.appsensor;

import trackingwsn.applications.app.App;

//
// Sensor's Application Layer
//
simple AppSensor extends App
{
    parameters:
        double senseInterval = default(0.5);
        // Collect measurement period
        double collMeaPeriod = default(0.05);
        // Report measurement period (should shorter than collMeaPeriod
        double repMeaPeriod = default(0.04);
        // Time point for CH broadcasts a synchronization beacon, relative to senseInterval.
        // This point should be near the end of senseInterval and long enough for beacon arrives
        // before next sensing cycle.
        double chBeaconTime = default(0.4);

        @class("AppSensor");
        @display("i=block/app");
}
