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
// Utility module for checking status of channel, used to simulate channel sensing. Each instance
// of this module can be used to track the using of a channel (just like a register table, no
// mechanism to control the behaviors of other modules is provided).
// Other modules should register with this module if they are going to use/release channel; and
// they can check the status of the channel.
//
simple ChannelUtil
{
    @display("i=block/network");
}
