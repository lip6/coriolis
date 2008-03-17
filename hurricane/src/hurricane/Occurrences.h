// ****************************************************************************************************
// File: Occurrences.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_OCCURENCES
#define HURRICANE_OCCURENCES

#include "Collection.h"
#include "Occurrence.h"

namespace Hurricane {



// ****************************************************************************************************
// Occurrences declaration
// ****************************************************************************************************

typedef GenericCollection<Occurrence> Occurrences;



// ****************************************************************************************************
// OccurrenceLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Occurrence> OccurrenceLocator;



// ****************************************************************************************************
// OccurrenceFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Occurrence> OccurrenceFilter;



// ****************************************************************************************************
// for_each_occurrence declaration
// ****************************************************************************************************

#define for_each_occurrence(occurrence, occurrences)\
/************************************************/\
{\
    OccurrenceLocator _locator = occurrences.getLocator();\
    while (_locator.IsValid()) {\
        Occurrence occurrence = _locator.getElement();\
        _locator.Progress();



} // End of Hurricane namespace.


#endif // HURRICANE_OCCURENCES

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
