// ****************************************************************************************************
// File: Intervals.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_INTERVALS
#define HURRICANE_INTERVALS

#include "hurricane/Collection.h"
#include "hurricane/Interval.h"

namespace Hurricane {



// ****************************************************************************************************
// Intervals declaration
// ****************************************************************************************************

typedef GenericCollection<Interval> Intervals;



// ****************************************************************************************************
// IntervalLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Interval> IntervalLocator;



// ****************************************************************************************************
// IntervalFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Interval> IntervalFilter;



// ****************************************************************************************************
// for_each_interval declaration
// ****************************************************************************************************

#define for_each_interval(interval, intervals)\
/*********************************************/\
{\
	IntervalLocator _locator = intervals.getLocator();\
	while (_locator.isValid()) {\
		Interval interval = _locator.getElement();\
		_locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_INTERVALS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
