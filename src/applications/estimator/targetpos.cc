/*
 * targetpos.cc
 *
 *  Created on: Nov 1, 2013
 *      Author: quannt
 */

#include "targetpos.h"

TargetPos::TargetPos()
{
    x = 0;
    y = 0;
}

TargetPos::TargetPos(double x, double y)
{
    this->x = x;
    this->y = y;
}

double TargetPos::getX() const
{
    return x;
}

void TargetPos::setX(double x)
{
    this->x = x;
}

double TargetPos::getY() const
{
    return y;
}

void TargetPos::setY(double y)
{
    this->y = y;
}
