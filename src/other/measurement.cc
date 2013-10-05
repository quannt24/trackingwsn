/*
 * measurement.cc
 *
 *  Created on: Oct 4, 2013
 *      Author: quannt
 */

#include "measurement.h"

Measurement::Measurement()
{
    tarId = -1;
    measuredDistance = 0;
}

/* Constructor
 * @para tarId Target's ID
 * @para md Measured distance */
Measurement::Measurement(int tarId, double md)
{
    this->tarId = tarId;
    this->measuredDistance = md;
}

Measurement::~Measurement()
{
}

int Measurement::getTarId() const
{
    return tarId;
}

void Measurement::setTarId(int tarId)
{
    this->tarId = tarId;
}

double Measurement::getMeasuredDistance() const
{
    return measuredDistance;
}

void Measurement::setMeasuredDistance(double measuredDistance)
{
    this->measuredDistance = measuredDistance;
}
