// ****************************************************************************************************
// File: ./hurricane/Boxes.h
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

#ifndef HURRICANE_BOXES
#define HURRICANE_BOXES

#include "hurricane/Collection.h"
#include "hurricane/Box.h"

namespace Hurricane {



// ****************************************************************************************************
// Boxes declaration
// ****************************************************************************************************

typedef GenericCollection<Box> Boxes;



// ****************************************************************************************************
// BoxLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Box> BoxLocator;



// ****************************************************************************************************
// BoxFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Box> BoxFilter;



// ****************************************************************************************************
// for_each_box declaration
// ****************************************************************************************************

#define for_each_box(box, boxes)\
/*******************************/\
{\
    BoxLocator _locator = boxes.getLocator();\
    while (_locator.isValid()) {\
        Box box = _locator.getElement();\
        _locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_BOXES


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
