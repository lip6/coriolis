// ****************************************************************************************************
// File: Intervals.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_INTERVALS
#define HURRICANE_INTERVALS

#include "Collection.h"
#include "Interval.h"

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
	IntervalLocator _locator = intervals.GetLocator();\
	while (_locator.IsValid()) {\
		Interval interval = _locator.GetElement();\
		_locator.Progress();



} // End of Hurricane namespace.

#endif // HURRICANE_INTERVALS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
