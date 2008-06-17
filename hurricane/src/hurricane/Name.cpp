// ****************************************************************************************************
// File: Name.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "hurricane/Name.h"
#include "hurricane/SharedName.h"

namespace Hurricane {



// ****************************************************************************************************
// Name implementation
// ****************************************************************************************************

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
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
