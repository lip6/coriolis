// ****************************************************************************************************
// File: ./hurricane/Cells.h
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

#ifndef HURRICANE_CELLS
#define HURRICANE_CELLS

#include "hurricane/Collection.h"

namespace Hurricane {

class Cell;



// ****************************************************************************************************
// Cells declaration
// ****************************************************************************************************

typedef GenericCollection<Cell*> Cells;



// ****************************************************************************************************
// CellLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Cell*> CellLocator;



// ****************************************************************************************************
// CellFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Cell*> CellFilter;



// ****************************************************************************************************
// for_each_cell declaration
// ****************************************************************************************************

#define for_each_cell(cell, cells)\
/*********************************/\
{\
    CellLocator _locator = cells.getLocator();\
    while (_locator.isValid()) {\
        Cell* cell = _locator.getElement();\
        _locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_CELLS


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
