// ****************************************************************************************************
// File: ./hurricane/Selectors.h
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

#ifndef HURRICANE_SELECTORS
#define HURRICANE_SELECTORS

#include "hurricane/Collection.h"

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
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
