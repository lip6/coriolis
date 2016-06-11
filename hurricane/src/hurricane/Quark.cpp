// ****************************************************************************************************
// File: ./Quark.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
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

#include "hurricane/Quark.h"
#include "hurricane/SharedPath.h"
#include "hurricane/Entity.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Variables
// ****************************************************************************************************

typedef SharedPath::QuarkMap Quark_QuarkMap;

static Quark_QuarkMap* NULL_SHARED_PATH_QUARK_MAP = NULL;



// ****************************************************************************************************
// Quark implementation
// ****************************************************************************************************

Quark::Quark(const Occurrence& occurrence)
// *************************************
:    Inherit(),
    _occurrence(occurrence),
    _nextOfSharedPathQuarkMap(NULL)
{
    if (!_occurrence.isValid())
        throw Error("Can't create " + _TName("Quark") + " : invalid occurrence");

    if (_occurrence._getQuark())
        throw Error("Can't create " + _TName("Quark") + " : already exists");
}

Quark* Quark::_create(const Occurrence& occurrence)
// **********************************************
{
    Quark* quark = new Quark(occurrence);

    quark->_postCreate();

    return quark;
}

void Quark::_postCreate()
// **********************
{
    SharedPath* sharedPath = _occurrence._getSharedPath();

    if (sharedPath)
        sharedPath->_getQuarkMap()._insert(this);
    else {
        if (!NULL_SHARED_PATH_QUARK_MAP) NULL_SHARED_PATH_QUARK_MAP = new Quark_QuarkMap();
        NULL_SHARED_PATH_QUARK_MAP->_insert(this);
    }

    Inherit::_postCreate();
}

void Quark::_preDestroy()
// *********************
{
  cdebug_log(18,1) << "entering Quark::_preDestroy: " << this << endl;

  Inherit::_preDestroy();

  SharedPath* sharedPath = _occurrence._getSharedPath();

  if (sharedPath)
    sharedPath->_getQuarkMap()._remove(this);
  else
    if (NULL_SHARED_PATH_QUARK_MAP) NULL_SHARED_PATH_QUARK_MAP->_remove(this);

  cdebug_log(18,0) << "exiting Quark::_preDestroy:" << endl;
  cdebug_tabw(18,-1);
}

string Quark::_getString() const
// *****************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_occurrence));
    return s;
}

Record* Quark::_getRecord() const
// ************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Occurrence", &_occurrence));
    }
    return record;
}



// ****************************************************************************************************
// Entity implementation (located here to access the local variables)
// ****************************************************************************************************

Quark* Entity::_getQuark(SharedPath* sharedPath) const
// ***************************************************
{
    if (sharedPath)
        return sharedPath->_getQuark(this);
    else {
        if (!NULL_SHARED_PATH_QUARK_MAP) return NULL;
        return NULL_SHARED_PATH_QUARK_MAP->getElement(this);
    }
}



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
