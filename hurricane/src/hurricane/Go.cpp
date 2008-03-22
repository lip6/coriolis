// ****************************************************************************************************
// File: Go.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Go.h"
#include "QuadTree.h"

namespace Hurricane {



// ****************************************************************************************************
// Go implementation
// ****************************************************************************************************

static bool AUTO_MATERIALIZATION_IS_ENABLED = true;

Go::Go()
// *****
:    Inherit(),
    _quadTree(NULL),
    _nextOfQuadTreeGoSet(NULL)
{
}

bool Go::AutoMaterializationIsDisabled()
// *************************************
{
    return !AUTO_MATERIALIZATION_IS_ENABLED;
}

void Go::EnableAutoMaterialization()
// *********************************
{
    AUTO_MATERIALIZATION_IS_ENABLED = true;
}

void Go::DisableAutoMaterialization()
// **********************************
{
    AUTO_MATERIALIZATION_IS_ENABLED = false;
}

void Go::_postCreate()
// *******************
{
    Inherit::_postCreate();

    if (!AutoMaterializationIsDisabled()) materialize(); // materialized after entire post creation
}

void Go::_preDestroy()
// ******************
{
// trace << "entering Go::_preDestroy: " << this << endl;
// trace_in();

    unmaterialize(); // unmaterialized before starting pre destruction

    Inherit::_preDestroy();

// trace << "exiting Go::_preDestroy:" << endl;
// trace_out();
}

string Go::_getString() const
// **************************
{
    return Inherit::_getString();
}

Record* Go::_getRecord() const
// *********************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->Add(getSlot("QuadTree", _quadTree));
    }
    return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
