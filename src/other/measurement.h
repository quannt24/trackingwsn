/*
 * measurement.h
 *
 *  Created on: Oct 4, 2013
 *      Author: quannt
 */

#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

/*
 * A measurement of distance to a target
 */
class Measurement
{
    private:
        int tarId; // Target ID
        double measuredDistance; // Measured distance (not true distance)
        double nodePosX, nodePosY; // Position of sensor node
        double nodeEnergy; // Remaining energy of sensor

        /* Initial function for constructors */
        void init(int tarId, double md, double nodePosX, double nodePosY, double nodeEnergy);

    public:
        Measurement();
        Measurement(int tarId, double md);
        Measurement(int tarId, double md, double nodePosX, double nodePosY, double nodeEnergy);
        virtual ~Measurement();

        int getTarId() const;
        void setTarId(int tarId);
        double getMeasuredDistance() const;
        void setMeasuredDistance(double measuredDistance);
        double getNodePosX() const;
        void setNodePosX(double nodePosX);
        double getNodePosY() const;
        void setNodePosY(double nodePosY);
        double getNodeEnergy();
        void setNodeEnergy(double nodeEnergy);
};

#endif /* MEASUREMENT_H_ */
