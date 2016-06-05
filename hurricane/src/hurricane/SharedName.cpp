// ****************************************************************************************************
// File: ./SharedName.cpp
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

#include <limits>
#include "hurricane/Error.h"
#include "hurricane/SharedName.h"

namespace Hurricane {



// ****************************************************************************************************
// SharedName implementation
// ****************************************************************************************************

  SharedName::SharedNameMap* SharedName::_SHARED_NAME_MAP = NULL;
  unsigned int               SharedName::_idCounter       = 0;


  SharedName::SharedName ( const string& name )
    : _id    (_idCounter++)
    , _count (0)
    , _string(name)
{
    if (!_SHARED_NAME_MAP) _SHARED_NAME_MAP = new SharedNameMap();
    (*_SHARED_NAME_MAP)[&_string] = this;

    if (_idCounter == std::numeric_limits<unsigned int>::max()) {
      throw Error( "SharedName::SharedName(): Identifier counter has reached it's limit (%d bits)."
                 , std::numeric_limits<unsigned int>::digits );
    }
}


SharedName::~SharedName()
// **********************
{
    _SHARED_NAME_MAP->erase(&_string);
}

void SharedName::capture()
// ***********************
{
    _count++;
}

void SharedName::release()
// ***********************
{
    if (!--_count) delete this;
}

string SharedName::_getString() const
// **********************************
{
    return "<" + _TName("SharedName") + " " + getString(_count) + " " + _string + ">";
}

Record* SharedName::_getRecord() const
// *****************************
{
    Record* record = new Record(getString(this));
    record->add(getSlot("_count", &_count));
    record->add(getSlot("_string", &_string));
    return record;
}



// ****************************************************************************************************
// SharedName::SharedNameMapComparator
// ****************************************************************************************************

bool SharedName::SharedNameMapComparator::operator()(string* s1, string* s2) const
// *******************************************************************************
{
    return (*s1) < (*s2);
}



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
