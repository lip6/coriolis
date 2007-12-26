// ****************************************************************************************************
// File: Horizontals.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_HORIZONTALS
#define HURRICANE_HORIZONTALS

#include "Collection.h"

namespace Hurricane {

class Horizontal;



// ****************************************************************************************************
// Horizontals declaration
// ****************************************************************************************************

typedef GenericCollection<Horizontal*> Horizontals;



// ****************************************************************************************************
// HorizontalLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Horizontal*> HorizontalLocator;



// ****************************************************************************************************
// HorizontalFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Horizontal*> HorizontalFilter;



// ****************************************************************************************************
// for_each_horizontal declaration
// ****************************************************************************************************

#define for_each_horizontal(horizontal, horizontals)\
/***************************************************/\
{\
	HorizontalLocator _locator = horizontals.GetLocator();\
	while (_locator.IsValid()) {\
		Horizontal* horizontal = _locator.GetElement();\
		_locator.Progress();



} // End of Hurricane namespace.

#endif // HURRICANE_HORIZONTALS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
