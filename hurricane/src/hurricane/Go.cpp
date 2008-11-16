// ****************************************************************************************************
// File: Go.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "hurricane/Go.h"
#include "hurricane/QuadTree.h"

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

bool Go::autoMaterializationIsDisabled()
// *************************************
{
    return !AUTO_MATERIALIZATION_IS_ENABLED;
}

void Go::enableAutoMaterialization()
// *********************************
{
    AUTO_MATERIALIZATION_IS_ENABLED = true;
}

void Go::disableAutoMaterialization()
// **********************************
{
    AUTO_MATERIALIZATION_IS_ENABLED = false;
}

void Go::_postCreate()
// *******************
{
    Inherit::_postCreate();

    if (!autoMaterializationIsDisabled()) materialize(); // materialized after entire post creation
}


void Go::_preDestroy()
{
//ltrace(10) << "Go::_preDestroy() - " << (void*)this << endl;
//ltracein(10);

  unmaterialize(); // unmaterialized before starting pre destruction
//ltrace(10) << "Unmaterialize successful"  << endl;

  Inherit::_preDestroy();

//ltrace(10) << "Go::_preDestroy() - exit" << endl;
//ltraceout(10);
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
        record->add(getSlot("QuadTree", _quadTree));
    }
    return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
