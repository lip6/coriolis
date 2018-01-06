// ****************************************************************************************************
// File: ./hurricane/Intervals.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
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
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
