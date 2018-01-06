// ****************************************************************************************************
// File: ./hurricane/Primitives.h
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

#ifndef HURRICANE_PRIMITIVES
#define HURRICANE_PRIMITIVES

#include "hurricane/Collection.h"

namespace Hurricane {

class Primitive;



// ****************************************************************************************************
// Primitives declaration
// ****************************************************************************************************

typedef GenericCollection<Primitive*> Primitives;



// ****************************************************************************************************
// PrimitiveLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Primitive*> PrimitiveLocator;



// ****************************************************************************************************
// PrimitiveFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Primitive*> PrimitiveFilter;



// ****************************************************************************************************
// for_each_primitive declaration
// ****************************************************************************************************

#define for_each_primitive(primitive, primitives)\
/************************************************/\
{\
	PrimitiveLocator _locator = primitives.getLocator();\
	while (_locator.isValid()) {\
		Primitive* primitive = _locator.getElement();\
		_locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_PRIMITIVES


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
