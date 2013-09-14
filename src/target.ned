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
// Target to be tracked
//
module Target extends Entity
{
    parameters:
        double range @unit(m) = default(8m);
        mobility.moving = true;
        mobility.startMovingTime = 22;
        @display("i=block/broadcast;is=s;r=$range");
        
    submodules:
        asg: Asg {
            ssRange = range;
        }
}
