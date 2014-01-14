/*
 * targetposvectorset.h
 *
 *  Created on: Nov 15, 2013
 *      Author: quannt
 */

#ifndef TARGETPOSVECTORSET_H_
#define TARGETPOSVECTORSET_H_

#include <coutvector.h>

class TargetPosVectorSet
{
    private:
        int tarId;
        cOutVector *vecPosX;
        cOutVector *vecPosY;

    public:
        TargetPosVectorSet();
        TargetPosVectorSet(int tarId);
        ~TargetPosVectorSet();
        int getTarId() const;
        void setTarId(int tarId);
        void record(double posX, double posY);
};

#endif /* TARGETPOSVECTORSET_H_ */
