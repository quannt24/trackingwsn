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

struct TargetEntry
{
    public:
        int tarId;
        std::list<Measurement> meaList;
        bool flagCH; // On when this node is Cluster Head for tracking this target

        TargetEntry() { flagCH = false; }
};

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
        /* Get pointer to list entries. Note that this list will not be available when the
         * collection is out of scope (deleted) or cleared. */
        std::list<TargetEntry>* getEntryList();
        /* Get pointer to measurement list of a target. Note that this list will not be available
         * when the collection is out of scope (deleted) or cleared. */
        std::list<Measurement>* getMeaList(int tarId);
        /* Clear all collected measurements */
        void clear();
};

#endif /* MEACOLL_H_ */
