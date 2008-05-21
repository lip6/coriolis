// ****************************************************************************************************
// File: Verticals.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_VERTICALS
#define HURRICANE_VERTICALS

#include "hurricane/Collection.h"

namespace Hurricane {

class Vertical;



// ****************************************************************************************************
// Verticals declaration
// ****************************************************************************************************

typedef GenericCollection<Vertical*> Verticals;



// ****************************************************************************************************
// VerticalLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Vertical*> VerticalLocator;



// ****************************************************************************************************
// VerticalFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Vertical*> VerticalFilter;



// ****************************************************************************************************
// for_each_vertical declaration
// ****************************************************************************************************

#define for_each_vertical(vertical, verticals)\
/*********************************************/\
{\
	VerticalLocator _locator = verticals.getLocator();\
	while (_locator.isValid()) {\
		Vertical* vertical = _locator.getElement();\
		_locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_VERTICALS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
