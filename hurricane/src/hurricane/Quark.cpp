// ****************************************************************************************************
// File: Quark.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
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
// trace << "entering Quark::_preDestroy: " << this << endl;
// trace_in();

    Inherit::_preDestroy();

    SharedPath* sharedPath = _occurrence._getSharedPath();

    if (sharedPath)
        sharedPath->_getQuarkMap()._remove(this);
    else
        if (NULL_SHARED_PATH_QUARK_MAP) NULL_SHARED_PATH_QUARK_MAP->_remove(this);

// trace << "exiting Quark::_preDestroy:" << endl;
// trace_out();
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
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
