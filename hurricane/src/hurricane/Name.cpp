// ****************************************************************************************************
// File: ./Name.cpp
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

#include "hurricane/Name.h"
#include "hurricane/SharedName.h"

namespace Hurricane {



// ****************************************************************************************************
// Name implementation
// ****************************************************************************************************


  const Name  Name::_emptyName;


Name::Name()
// *********
:  _sharedName(NULL)
{
    string s = "";
    if (!SharedName::_SHARED_NAME_MAP)
        _sharedName = new SharedName(s);
    else {
        SharedName::SharedNameMap::iterator iterator =
            SharedName::_SHARED_NAME_MAP->find(&(string&)s);
        if (iterator != SharedName::_SHARED_NAME_MAP->end())
            _sharedName = (*iterator).second;
        else
            _sharedName = new SharedName(s);
    }

    assert(_sharedName);

    _sharedName->capture();
}

Name::Name(const char* c)
// **********************
:  _sharedName(NULL)
{
    string s = c;
    if (!SharedName::_SHARED_NAME_MAP)
        _sharedName = new SharedName(s);
    else {
        SharedName::SharedNameMap::iterator iterator =
            SharedName::_SHARED_NAME_MAP->find(&(string&)s);
        if (iterator != SharedName::_SHARED_NAME_MAP->end())
            _sharedName = (*iterator).second;
        else
            _sharedName = new SharedName(s);
    }

    assert(_sharedName);

    _sharedName->capture();
}

Name::Name(const string& s)
// ************************
:  _sharedName(NULL)
{
    if (!SharedName::_SHARED_NAME_MAP)
        _sharedName = new SharedName(s);
    else {
        SharedName::SharedNameMap::iterator iterator =
            SharedName::_SHARED_NAME_MAP->find(&(string&)s);
        if (iterator != SharedName::_SHARED_NAME_MAP->end())
            _sharedName = (*iterator).second;
        else
            _sharedName = new SharedName(s);
    }

    assert(_sharedName);

    _sharedName->capture();
}

Name::Name(const Name& name)
// *************************
:  _sharedName(name._sharedName)
{
    _sharedName->capture();
}

Name::~Name()
// **********
{
    _sharedName->release();
}

Name& Name::operator=(const Name& name)
// ************************************
{
    SharedName* sharedName = name._sharedName;
    if (sharedName != _sharedName) {
        _sharedName->release();
        _sharedName = sharedName;
        _sharedName->capture();
    }
    return *this;
}

bool Name::operator==(const Name& name) const
// ******************************************
{
    return (_sharedName == name._sharedName);
}

bool Name::operator!=(const Name& name) const
// ******************************************
{
    return (_sharedName != name._sharedName);
}

bool Name::operator<(const Name& name) const
// *****************************************
{
    return ((_sharedName != name._sharedName) &&
              (_sharedName->_string < name._sharedName->_string));
}

bool Name::operator<=(const Name& name) const
// ******************************************
{
    return ((_sharedName == name._sharedName) ||
              (_sharedName->_string < name._sharedName->_string));
}

bool Name::operator>(const Name& name) const
// *****************************************
{
    return ((_sharedName != name._sharedName) &&
              (_sharedName->_string > name._sharedName->_string));
}

bool Name::operator>=(const Name& name) const
// ******************************************
{
    return ((_sharedName == name._sharedName) ||
              (_sharedName->_string >= name._sharedName->_string));
}

char Name::operator[](unsigned index) const
// ****************************************
{
    return _sharedName->_string[index];
}

size_t Name::size() const
// **********************
{
    return _sharedName->_string.size();
}

bool Name::isEmpty() const
// ***********************
{
    return _sharedName->_string.empty();
}

string Name::_getString() const
// ****************************
{
    return _sharedName->_string;
}

Record* Name::_getRecord() const
// ***********************
{
    Record* record = new Record(getString(this));
    record->add(getSlot("SharedName", _sharedName));
    return record;
}



} // End of Hurricane namespace.



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************



// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************
