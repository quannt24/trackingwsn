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

#include "lateration.h"
#include "matrix.h"

Define_Module(Lateration);

void Lateration::initialize()
{
}

void Lateration::handleMessage(cMessage *msg)
{
}

TargetPos Lateration::estimate(std::list<Measurement> meaList)
{
    if ((unsigned) meaList.size() < par("minNumMeasurement").longValue()) throw NOT_ENOUGH_MEASUREMENT;

    Matrix tarPos = Matrix(2, 1);
    Matrix A = Matrix(meaList.size() - 1, 2);
    Matrix B = Matrix(meaList.size() - 1, 1);
    Measurement &m0 = meaList.front();
    double x0 = m0.getNodePosX();
    double y0 = m0.getNodePosY();
    double r0 = m0.getMeasuredDistance();
    double xi, yi, ri;

    int i = 0;
    std::list<Measurement>::iterator it = meaList.begin();
    it++; // Skip first element which is m0
    for (; it != meaList.end(); ++it) {
        xi = (*it).getNodePosX();
        yi = (*it).getNodePosY();
        ri = (*it).getMeasuredDistance();

        //set value of Matrix A
        A.setCell(i, 0, 2 * (xi - x0));
        A.setCell(i, 1, 2 * (yi - y0));

        //set value of Matrix B
        B.setCell(i, 0, (r0 * r0 - ri * ri) - (x0 * x0 - xi * xi) - (y0 * y0 - yi * yi));

        i++; // Next row
    }

    tarPos = (A.transpose() * A).inverse() * A.transpose() * B;

    return TargetPos(tarPos.getCell(0, 0), tarPos.getCell(1, 0));
}
