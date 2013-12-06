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

#include "energy.h"
#include "app.h"

Define_Module(Energy);

void Energy::initialize()
{
    energyDrawSignal = registerSignal("energy_draw");
    setCapacity(par("initCap").doubleValue());
}

void Energy::handleMessage(cMessage *msg)
{
}

/*
 * Get current capacity
 */
double Energy::getCapacity()
{
    return capacity;
}

/*
 * Set a non-negative capacity. If a negative capacity is passed, this function will only store zero.
 */
void Energy::setCapacity(double cap)
{
    if (cap >= 0)
        capacity = cap;
    else
        capacity = 0;

    emit(energyDrawSignal, capacity);
}

/*
 * Draw energy from this source. If drawn amount is negative, the capacity of energy source has no
 * change.
 * Return: Energy amount which is really drawn, which is non-negative and less than or equal current
 * capacity. If 'amount' is negative, 0 is returned and capacity of the source has no change.
 * If 'amount' is greater than current capacity, the current capacity is returned and capacity of
 * the source is set to zero.
 */
double Energy::draw(double amount)
{
    Enter_Method("draw");
    if (par("hasLinePower").boolValue()) {
        return amount;
    } else {
        // Draw energy from battery
        double cap = getCapacity();

        if (amount > 0 && amount <= cap) {
            setCapacity(cap - amount);
            return amount;
        } else if (amount > cap) {
            setCapacity(0);
            App *app = check_and_cast<App*>(getParentModule()->getSubmodule("app"));
            if (app->isWorking()) app->poweroff(); // Check working state to prevent loop calls.
            return cap;
        } else {
            return 0;
        }
    }
}
