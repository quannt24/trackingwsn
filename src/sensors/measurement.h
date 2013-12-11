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
        double tarPosX, tarPosY; // True position of target (for reference)
        double nodePosX, nodePosY; // Position of sensor node
        double nodeEnergy; // Remaining energy of sensor

        /* Initial function for constructors */
        void init(int tarId, double md, double tarPosX, double tarPosY, double nodePosX, double nodePosY, double nodeEnergy);

    public:
        Measurement();
        Measurement(int tarId, double md, double tarPosX, double tarPosY);
        Measurement(int tarId, double md, double tarPosX, double tarPosY, double nodePosX, double nodePosY, double nodeEnergy);
        virtual ~Measurement();

        int getTarId() const;
        void setTarId(int tarId);
        double getMeasuredDistance() const;
        void setMeasuredDistance(double measuredDistance);
        double getTarPosX() const;
        void setTarPosX(double tarPosX);
        double getTarPosY() const;
        void setTarPosY(double tarPosY);
        double getNodePosX() const;
        void setNodePosX(double nodePosX);
        double getNodePosY() const;
        void setNodePosY(double nodePosY);
        double getNodeEnergy();
        void setNodeEnergy(double nodeEnergy);
};

#endif /* MEASUREMENT_H_ */
