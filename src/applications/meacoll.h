/*
 * meacoll.h
 *
 *  Created on: Oct 21, 2013
 *      Author: quannt
 */

#ifndef MEACOLL_H_
#define MEACOLL_H_

#include "measurement.h"
#include <list>

typedef struct TargetEntry
{
    public:
        int tarId;
        std::list<Measurement> meaList;
} TargetEntry;

/*
 * Measurement Collection. Collect measurements and organize into targets' measurement list
 */
class MeaColl
{
    private:
        std::list<TargetEntry> teList;

    public:
        MeaColl();
        virtual ~MeaColl();

        /* Add a measurement to collection */
        void addMeasurement(Measurement m);
        /* Get measurement list of a target */
        std::list<Measurement>* getMeaList(int tarId);
        /* Clear all collected measurements */
        void clear();
};

#endif /* MEACOLL_H_ */
