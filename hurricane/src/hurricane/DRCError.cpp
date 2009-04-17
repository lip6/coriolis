// ****************************************************************************************************
// File: ./DRCError.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
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

#include "hurricane/DRCError.h"
#include "hurricane/Cell.h"
#include "hurricane/Slice.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// DRCError implementation
// ****************************************************************************************************

DRCError::DRCError(Cell* cell, const Name& name, const Box& boundingBox)
// *********************************************************************
:  Inherit(cell),
    _name(name),
    _boundingBox(boundingBox)
{
    if (_name.isEmpty())
        throw Error("Can't create " + _TName("DRCError") + " : empty name");

    if (_boundingBox.isEmpty())
        throw Error("Can't create " + _TName("DRCError") + " : empty bounding box");
}

DRCError* DRCError::create(Cell* cell, const Name& name, const Box& boundingBox)
// *****************************************************************************
{
    DRCError* drcError = new DRCError(cell, name, boundingBox);

    drcError->_postCreate();

    return drcError;
}

void DRCError::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// *****************************************************
{
    if ((dx != 0) || (dy != 0)) {
        invalidate(false);
        _boundingBox.translate(dx, dy);
    }
}

string DRCError::_getString() const
// ********************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_name));
    s.insert(s.length() - 1, " " + getString(_boundingBox));
    return s;
}

Record* DRCError::_getRecord() const
// ***************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Name", &_name));
        record->add(getSlot("BoundingBox", &_boundingBox));
    }
    return record;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************
