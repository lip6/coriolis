// ****************************************************************************************************
// File: ./Go.cpp
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
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************
