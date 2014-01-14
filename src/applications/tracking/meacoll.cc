/*
 * meacoll.cc
 *
 *  Created on: Oct 21, 2013
 *      Author: quannt
 */

#include "meacoll.h"

MeaColl::MeaColl()
{
}

MeaColl::~MeaColl()
{
    teList.clear();
}

/*
 * Add a measurement to collection
 */
void MeaColl::addMeasurement(Measurement m)
{
    for (std::list<TargetEntry>::iterator it=teList.begin(); it != teList.end(); ++it) {
        if ((*it).tarId == m.getTarId()) {
            // Add measure into existing entry
            (*it).meaList.push_back(m);
            return;
        }
    }

    // Add measure into new entry
    TargetEntry te;
    te.tarId = m.getTarId();
    te.meaList.push_back(m);
    teList.push_back(te);
}

/*
 * Get pointer to entry of a target. Node that this entry will not available when the
 * collection is out of scope (deleted) or cleared.
 */
std::list<TargetEntry>* MeaColl::getEntryList()
{
    return &teList;
}

/*
 * Get pointer to measurement list of a target. Note that this list will not be available
 * when the collection is out of scope (deleted) or cleared.
 */
std::list<Measurement>* MeaColl::getMeaList(int tarId)
{
    for (std::list<TargetEntry>::iterator it = teList.begin(); it != teList.end(); ++it) {
        if ((*it).tarId == tarId) {
            return &((*it).meaList);
        }
    }

    return 0; // NULL
}

/*
 * Clear all collected measurements
 */
void MeaColl::clear()
{
    teList.clear();
}
