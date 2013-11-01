/*
 * targetpos.h
 *
 *  Created on: Nov 1, 2013
 *      Author: quannt
 */

#ifndef TARGETPOS_H_
#define TARGETPOS_H_

/*
 * Tracked target position
 */
class TargetPos
{
    private:
        double x;
        double y;

    public:
        TargetPos();
        TargetPos(double x, double y);
        double getX() const;
        void setX(double x);
        double getY() const;
        void setY(double y);
};

#endif /* TARGETPOS_H_ */
