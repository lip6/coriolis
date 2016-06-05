// ****************************************************************************************************
// File: ./Path.cpp
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

#include "hurricane/Path.h"
#include "hurricane/SharedPath.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Path implementation
// ****************************************************************************************************

Path::Path(SharedPath* sharedPath)
// *******************************
:  _sharedPath(sharedPath)
{
}

Path::Path(Instance* instance)
// ***************************
:  _sharedPath(NULL)
{
    if (instance) {
        _sharedPath = instance->_getSharedPath(NULL);
        if (!_sharedPath) _sharedPath = new SharedPath(instance);
    }
}

Path::Path(Instance* headInstance, const Path& tailPath)
// *****************************************************
:  _sharedPath(NULL)
{
    if (!headInstance)
        throw Error("Cant't create " + _TName("Path") + " : null head instance");

    if (!tailPath._getSharedPath()) {
        _sharedPath = headInstance->_getSharedPath(NULL);
        if (!_sharedPath) _sharedPath = new SharedPath(headInstance);
    }
    else {
        SharedPath* tailSharedPath = tailPath._getSharedPath();
        if (tailSharedPath->getOwnerCell() != headInstance->getMasterCell())
            throw Error("Cant't create " + _TName("Path") + " : incompatible tail path");

        _sharedPath = headInstance->_getSharedPath(tailSharedPath);
        if (!_sharedPath) _sharedPath = new SharedPath(headInstance, tailSharedPath);
    }
}

Path::Path(const Path& headPath, Instance* tailInstance)
// *****************************************************
:  _sharedPath(NULL)
{
    if (!tailInstance)
        throw Error("Cant't create " + _TName("Path") + " : null tail instance");

    if (!headPath._getSharedPath()) {
        _sharedPath = tailInstance->_getSharedPath(NULL);
        if (!_sharedPath) _sharedPath = new SharedPath(tailInstance);
    }
    else {
        Instance* headInstance = headPath.getHeadInstance();
        SharedPath* tailSharedPath = Path(headPath.getTailPath(), tailInstance)._getSharedPath();
        _sharedPath = headInstance->_getSharedPath(tailSharedPath);
        if (!_sharedPath) _sharedPath = new SharedPath(headInstance, tailSharedPath);
    }
}

Path::Path(const Path& headPath, const Path& tailPath)
// *****************************************************
:  _sharedPath(tailPath._getSharedPath())
{
    vector<Instance*> instances;
    headPath.getInstances().fill(instances);
    
    for (vector<Instance*>::reverse_iterator rit=instances.rbegin() ; rit != instances.rend() ; rit++)
    { Instance* instance=*rit;
        SharedPath* sharedPath = _sharedPath;
        _sharedPath = instance->_getSharedPath(sharedPath);
        if (!_sharedPath) _sharedPath = new SharedPath(instance,sharedPath);
    }
}

Path::Path(Cell* cell, const string& pathName)
// *******************************************
:  _sharedPath(NULL)
{
    if (cell) {
        list<Instance*> instanceList;
        string restOfPathName = pathName;
        char nameSeparator = getNameSeparator();
        while (!restOfPathName.empty()) {
            size_t pos = restOfPathName.find(nameSeparator);
            Instance* instance = cell->getInstance(restOfPathName.substr(0, pos));
            if (!instance) throw Error("Cant't create " + _TName("Path") + " : invalid path name");
            cell = instance->getMasterCell();
            restOfPathName = (pos == string::npos) ? string("") : restOfPathName.substr(pos + 1);
            instanceList.push_back(instance);
        }
        if (!instanceList.empty()) {
            list<Instance*>::reverse_iterator instanceIterator = instanceList.rbegin();
            while (instanceIterator != instanceList.rend()) {
                Instance* headInstance = *instanceIterator;
                SharedPath* tailSharedPath = _sharedPath;
                _sharedPath = headInstance->_getSharedPath(tailSharedPath);
                if (!_sharedPath) _sharedPath = new SharedPath(headInstance, tailSharedPath);
                ++instanceIterator;
            }
        }
    }
}

Path::Path(const Path& path)
// *************************
:  _sharedPath(path._sharedPath)
{
}

Path::~Path()
// **********
{
}

Path& Path::operator=(const Path& path)
// ************************************
{
    _sharedPath = path._sharedPath;
    return *this;
}

bool Path::operator==(const Path& path) const
// ******************************************
{
    return (_sharedPath == path._sharedPath);
}

bool Path::operator!=(const Path& path) const
// ******************************************
{
    return (_sharedPath != path._sharedPath);
}

bool Path::operator<(const Path& path) const
// *****************************************
{
    return (_sharedPath < path._sharedPath);
}

Instance* Path::getHeadInstance() const
// ************************************
{
    return (_sharedPath) ? _sharedPath->getHeadInstance() : NULL;
}

Path Path::getTailPath() const
// ***************************
{
    return Path((_sharedPath) ? _sharedPath->getTailSharedPath() : NULL);
}

Path Path::getHeadPath() const
// ***************************
{
    return Path((_sharedPath) ? _sharedPath->getHeadSharedPath() : NULL);
}

Instance* Path::getTailInstance() const
// ************************************
{
    return (_sharedPath) ? _sharedPath->getTailInstance() : NULL;
}

char Path::getNameSeparator()
// **************************
{
    return SharedPath::getNameSeparator();
}

string Path::getName() const
// *************************
{
    return (_sharedPath) ? _sharedPath->getName() : string("");
}

Cell* Path::getOwnerCell() const
// *****************************
{
    return (_sharedPath) ? _sharedPath->getOwnerCell() : NULL;
}

Cell* Path::getMasterCell() const
// ******************************
{
    return (_sharedPath) ? _sharedPath->getMasterCell() : NULL;
}

Instances Path::getInstances() const
// *********************************
{
    return (_sharedPath) ? _sharedPath->getInstances() : Instances();
}

Transformation Path::getTransformation(const Transformation& transformation) const
// *******************************************************************************
{
    return (_sharedPath) ? _sharedPath->getTransformation(transformation) : transformation;
}

bool Path::isEmpty() const
// ***********************
{
    return (_sharedPath == NULL);
}

void Path::makeEmpty()
// *******************
{
    _sharedPath = NULL;
}

void Path::setNameSeparator(char nameSeparator)
// ********************************************
{
    SharedPath::setNameSeparator(nameSeparator);
}

string Path::getCompactString() const
// **********************************
{
  if (isEmpty()) return "<empty>";

  string s = "<";
  s += getString(getOwnerCell()->getName());
  s += ":";
  s += getString(_sharedPath->getName()) + ":";
  s += getString(getMasterCell()->getName()) + ">";
  return s;
}

string Path::getJsonString(unsigned long flags) const
// **************************************************
{
  if (isEmpty()) return "";

  string s;
  if (flags & JsonWriter::DesignBlobMode) {
  //s += getString(getOwnerCell()->getId()) + "::";
    s += getString(_sharedPath->getJsonString(flags));
  //s += "::" + getString(getMasterCell()->getId());
  } else {
  //s += getString(getOwnerCell()->getName()) + "::";
    s += getString(_sharedPath->getJsonString(flags));
  //s += "::" + getString(getMasterCell()->getName());
  }

  return s;
}

string Path::_getString() const
// ****************************
{
    string s = "<" + _TName("Path");
    if (!_sharedPath)
        s += " empty";
    else
        s += " " + getString(getName());
    s += ">";
    return s;
}

Record* Path::_getRecord() const
// ***********************
{
     Record* record = NULL;
    if (_sharedPath) {
          record = new Record(getString(this));
        record->add(getSlot("SharedPath", _sharedPath));
    }
    return record;
}


} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
