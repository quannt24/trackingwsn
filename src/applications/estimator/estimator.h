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

#ifndef __TRACKINGWSN_ESTIMATOR_H_
#define __TRACKINGWSN_ESTIMATOR_H_

#include "targetpos.h"
#include "measurement.h"
#include <omnetpp.h>

enum EstimatorException {NOT_ENOUGH_MEASUREMENT};

/**
 * Estimator for tracking position. This module work like a plug-in for application layer.
 */
class Estimator : public cSimpleModule
{
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        /* Minimum number of measurements so that the estimator can work properly */
        virtual unsigned int minNumMeasurement() { return par("minNumMeasurement").longValue(); }
        virtual TargetPos estimate(std::list<Measurement> meaList) { return TargetPos(); }
};

#endif
