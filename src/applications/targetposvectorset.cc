/*
 * targetposvectorset.cc
 *
 *  Created on: Nov 15, 2013
 *      Author: quannt
 */

#include "targetposvectorset.h"

TargetPosVectorSet::TargetPosVectorSet()
{
    vecPosX = new cOutVector();
    vecPosY = new cOutVector();
    tarId = -1;
}

TargetPosVectorSet::TargetPosVectorSet(int tarId)
{
    vecPosX = new cOutVector();
    vecPosY = new cOutVector();
    setTarId(tarId);
}

TargetPosVectorSet::~TargetPosVectorSet()
{
    delete vecPosX;
    delete vecPosY;
}

int TargetPosVectorSet::getTarId() const
{
    return tarId;
}

void TargetPosVectorSet::setTarId(int tarId)
{
    this->tarId = tarId;
    char vecName[100];

    // Set vector names
    sprintf(vecName, "target_%d_tracked_x", tarId);
    vecPosX->setName(vecName);
    sprintf(vecName, "target_%d_tracked_y", tarId);
    vecPosY->setName(vecName);
}

void TargetPosVectorSet::record(double posX, double posY)
{
    if (tarId < 0) return; // Target ID is not set
    vecPosX->record(posX);
    vecPosY->record(posY);
}
