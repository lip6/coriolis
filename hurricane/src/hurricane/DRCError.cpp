// ****************************************************************************************************
// File: DRCError.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "DRCError.h"
#include "Cell.h"
#include "Slice.h"
#include "Error.h"

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
    if (_name.IsEmpty())
        throw Error("Can't create " + _TName("DRCError") + " : empty name");

    if (_boundingBox.isEmpty())
        throw Error("Can't create " + _TName("DRCError") + " : empty bounding box");
}

DRCError* DRCError::Create(Cell* cell, const Name& name, const Box& boundingBox)
// *****************************************************************************
{
    DRCError* drcError = new DRCError(cell, name, boundingBox);

    drcError->_postCreate();

    return drcError;
}

void DRCError::Translate(const Unit& dx, const Unit& dy)
// *****************************************************
{
    if ((dx != 0) || (dy != 0)) {
        Invalidate(false);
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
        record->Add(getSlot("Name", &_name));
        record->Add(getSlot("BoundingBox", &_boundingBox));
    }
    return record;
}

} // End of Hurricane namespace.
