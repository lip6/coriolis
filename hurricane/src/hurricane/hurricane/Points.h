// ****************************************************************************************************
// File: ./hurricane/Points.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
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

#ifndef HURRICANE_POINTS
#define HURRICANE_POINTS

#include "hurricane/Collection.h"
#include "hurricane/Point.h"

namespace Hurricane {



// ****************************************************************************************************
// Points declaration
// ****************************************************************************************************

typedef GenericCollection<Point> Points;



// ****************************************************************************************************
// PointLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Point> PointLocator;



// ****************************************************************************************************
// PointFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Point> PointFilter;



// ****************************************************************************************************
// for_each_point declaration
// ****************************************************************************************************

#define for_each_point(point, points)\
/************************************/\
{\
	PointLocator _locator = points.getLocator();\
	while (_locator.isValid()) {\
		Point point = _locator.getElement();\
		_locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_POINTS


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
