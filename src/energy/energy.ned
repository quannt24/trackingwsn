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

package trackingwsn.energy;

//
// Module for simulating and managing energy.
//
simple Energy
{
    parameters:
        bool hasLinePower = default(false); // If module having line power supply (infinite)
        double initCap @unit(J) = default(10J); // Initial capacity
        
        @signal[energy_draw](type="double");
        //@statistic[energy](title="energy"; source="energy_draw"; record=vector);
        
        @display("i=block/plug");
}
