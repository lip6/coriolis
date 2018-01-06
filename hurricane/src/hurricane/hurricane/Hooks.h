// ****************************************************************************************************
// File: ./hurricane/Hooks.h
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

#ifndef HURRICANE_HOOKS
#define HURRICANE_HOOKS

#include "hurricane/Collection.h"

namespace Hurricane {

class Hook;



// ****************************************************************************************************
// Hooks declaration
// ****************************************************************************************************

typedef GenericCollection<Hook*> Hooks;



// ****************************************************************************************************
// HookLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Hook*> HookLocator;



// ****************************************************************************************************
// HookFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Hook*> HookFilter;



// ****************************************************************************************************
// for_each_hook declaration
// ****************************************************************************************************

#define for_each_hook(hook, hooks)\
/*********************************/\
{\
    HookLocator _locator = hooks.getLocator();\
    while (_locator.isValid()) {\
        Hook* hook = _locator.getElement();\
        _locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_HOOKS


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
