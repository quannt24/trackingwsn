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

package trackingwsn.entities;

import trackingwsn.applications.IApp;
import trackingwsn.networklayer.INet;
import trackingwsn.linklayer.ILink;
import trackingwsn.energy.Energy;

module Node extends Entity
{
    parameters:
        // Application layer module name
        string appModuleName = default("AppSensor");
        // Network layer module name
        string netModuleName = default("NetEMRP");
        // Link layer module name
        string linkModuleName = default("Link802154");

    submodules:
        app: <appModuleName> like IApp;
        net: <netModuleName> like INet;
        link: <linkModuleName> like ILink;
        energy: Energy;
        
    connections:
        app.netGate <--> net.appGate;
        net.linkGate <--> link.netGate;
}
