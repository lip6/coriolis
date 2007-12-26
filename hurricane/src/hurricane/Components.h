// ****************************************************************************************************
// File: Components.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_COMPONENTS
#define HURRICANE_COMPONENTS

#include "Collection.h"

namespace Hurricane {

class Component;



// ****************************************************************************************************
// Components declaration
// ****************************************************************************************************

typedef GenericCollection<Component*> Components;



// ****************************************************************************************************
// ComponentLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Component*> ComponentLocator;



// ****************************************************************************************************
// ComponentFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Component*> ComponentFilter;



// ****************************************************************************************************
// for_each_component declaration
// ****************************************************************************************************

#define for_each_component(component, components)\
/************************************************/\
{\
	ComponentLocator _locator = components.GetLocator();\
	while (_locator.IsValid()) {\
		Component* component = _locator.GetElement();\
		_locator.Progress();



} // End of Hurricane namespace.

#endif // HURRICANE_COMPONENTS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
