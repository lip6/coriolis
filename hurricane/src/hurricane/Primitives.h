// ****************************************************************************************************
// File: Primitives.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_PRIMITIVES
#define HURRICANE_PRIMITIVES

#include "Collection.h"

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
	PrimitiveLocator _locator = primitives.GetLocator();\
	while (_locator.IsValid()) {\
		Primitive* primitive = _locator.GetElement();\
		_locator.Progress();



} // End of Hurricane namespace.

#endif // HURRICANE_PRIMITIVES

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
