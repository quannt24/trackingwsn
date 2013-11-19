/*
 * measurement.cc
 *
 *  Created on: Oct 4, 2013
 *      Author: quannt
 */

#include "measurement.h"

/* Initial function for constructors */
void Measurement::init(int tarId, double md, double nodePosX, double nodePosY, double nodeEnergy)
{
    this->tarId = tarId;
    this->measuredDistance = md;
    this->nodePosX = nodePosX;
    this->nodePosY = nodePosY;
    this->nodeEnergy = nodeEnergy;
}

Measurement::Measurement()
{
    init (-1, 0, 0, 0, 0);
}

Measurement::Measurement(int tarId, double md)
{
    init (tarId, md, 0, 0, 0);
}

Measurement::Measurement(int tarId, double md, double nodePosX, double nodePosY, double nodeEnergy)
{
    init(tarId, md, nodePosX, nodePosY, nodeEnergy);
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

double Measurement::getNodePosX() const
{
    return nodePosX;
}

void Measurement::setNodePosX(double nodePosX)
{
    this->nodePosX = nodePosX;
}

double Measurement::getNodePosY() const
{
    return nodePosY;
}

void Measurement::setNodePosY(double nodePosY)
{
    this->nodePosY = nodePosY;
}

double Measurement::getNodeEnergy()
{
    return nodeEnergy;
}

void Measurement::setNodeEnergy(double nodeEnergy)
{
    this->nodeEnergy = nodeEnergy;
}
