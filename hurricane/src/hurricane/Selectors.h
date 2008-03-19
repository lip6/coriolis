// ****************************************************************************************************
// File: Selectors.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_SELECTORS
#define HURRICANE_SELECTORS

#include "Collection.h"

namespace Hurricane {

class Selector;



// ****************************************************************************************************
// Selectors declaration
// ****************************************************************************************************

typedef GenericCollection<Selector*> Selectors;



// ****************************************************************************************************
// SelectorLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Selector*> SelectorLocator;



// ****************************************************************************************************
// SelectorFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Selector*> SelectorFilter;



// ****************************************************************************************************
// for_each_selector declaration
// ****************************************************************************************************

#define for_each_selector(selector, selectors)\
/*********************************************/\
{\
	SelectorLocator _locator = selectors.getLocator();\
	while (_locator.isValid()) {\
		Selector* selector = _locator.getElement();\
		_locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_SELECTORS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
