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
        double nodeEnergy; // Remaining energy of sensor

    public:
        Measurement();
        /* Constructor
         * @para tarId Target's ID
         * @para md Measured distance */
        Measurement(int tarId, double md, double nodeEnergy);
        virtual ~Measurement();

        int getTarId() const;
        void setTarId(int tarId);
        double getMeasuredDistance() const;
        void setMeasuredDistance(double measuredDistance);
        double getNodeEnergy();
        void setNodeEnergy(double nodeEnergy);
};

#endif /* MEASUREMENT_H_ */
