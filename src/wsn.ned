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

network Wsn
{
    parameters:
        double width @unit(m) = default(100m);
        double height @unit(m) = default(100m);
        int numTargets;
        int numSensors;
        int ssRows; // Number of sensor rows
        int ssCols; // Number of sensor columns
        // Note: First ssRows * ssCols sensors will be arranged, the rest is randomly distributed

        @display("bgs=10,m;bgb=$width,$height;"); // 1m ~ 10px

    submodules:
        target[numTargets]: Target;
        sensor[numSensors]: Sensor;
        bs: BaseStation {
            parameters:
                mobility.posX = width / 2;
                mobility.posY = height;
        }
        // These following modules are intended to initialize afterward
        wu: WorldUtil;
        cu: ChannelUtil;
}
