// ****************************************************************************************************
// File: ./hurricane/Pins.h
// Authors: Christophe Alexandre.
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

#ifndef HURRICANE_PINS
#define HURRICANE_PINS

#include "hurricane/Collection.h"

namespace Hurricane {

class Pin;



// ****************************************************************************************************
// Pins declaration
// ****************************************************************************************************

typedef GenericCollection<Pin*> Pins;



// ****************************************************************************************************
// PinLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Pin*> PinLocator;



// ****************************************************************************************************
// PinFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Pin*> PinFilter;



// ****************************************************************************************************
// for_each_pin declaration
// ****************************************************************************************************

#define for_each_pin(pin, pins)\
/******************************/\
{\
	PinLocator _locator = pins.getLocator();\
	while (_locator.isValid()) {\
		Pin* pin = _locator.getElement();\
		_locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_PINS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
