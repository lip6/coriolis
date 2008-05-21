// ****************************************************************************************************
// File: Properties.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_PROPERTIES
#define HURRICANE_PROPERTIES

#include "hurricane/Collection.h"

namespace Hurricane {

class Property;



// ****************************************************************************************************
// Properties declaration
// ****************************************************************************************************

typedef GenericCollection<Property*> Properties;



// ****************************************************************************************************
// PropertyLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Property*> PropertyLocator;



// ****************************************************************************************************
// PropertyFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Property*> PropertyFilter;



// ****************************************************************************************************
// for_each_property declaration
// ****************************************************************************************************

#define for_each_property(property, properties)\
/**********************************************/\
{\
	PropertyLocator _locator = properties.getLocator();\
	while (_locator.isValid()) {\
		Property* property = _locator.getElement();\
		_locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_PROPERTIES

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
