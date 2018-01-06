// ****************************************************************************************************
// File: ./hurricane/Quarks.h
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

#ifndef HURRICANE_QUARKS
#define HURRICANE_QUARKS

#include "hurricane/Collection.h"

namespace Hurricane {

class Quark;



// ****************************************************************************************************
// Quarks declaration
// ****************************************************************************************************

typedef GenericCollection<Quark*> Quarks;



// ****************************************************************************************************
// QuarkLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Quark*> QuarkLocator;



// ****************************************************************************************************
// QuarkFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Quark*> QuarkFilter;



// ****************************************************************************************************
// for_each_quark declaration
// ****************************************************************************************************

#define for_each_quark(quark, quarks)\
/************************************/\
{\
    QuarkLocator _locator = quarks.getLocator();\
    while (_locator.isValid()) {\
        Quark* quark = _locator.getElement();\
        _locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_QUARKS


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
