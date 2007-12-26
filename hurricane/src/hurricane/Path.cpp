// ****************************************************************************************************
// File: Path.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Path.h"
#include "SharedPath.h"
#include "Cell.h"
#include "Instance.h"
#include "Error.h"

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
		_sharedPath = instance->_GetSharedPath(NULL);
		if (!_sharedPath) _sharedPath = new SharedPath(instance);
	}
}

Path::Path(Instance* headInstance, const Path& tailPath)
// *****************************************************
:  _sharedPath(NULL)
{
	if (!headInstance)
		throw Error("Cant't create " + _TName("Path") + " : null head instance");

	if (!tailPath._GetSharedPath()) {
		_sharedPath = headInstance->_GetSharedPath(NULL);
		if (!_sharedPath) _sharedPath = new SharedPath(headInstance);
	}
	else {
		SharedPath* tailSharedPath = tailPath._GetSharedPath();
		if (tailSharedPath->GetOwnerCell() != headInstance->GetMasterCell())
			throw Error("Cant't create " + _TName("Path") + " : incompatible tail path");

		_sharedPath = headInstance->_GetSharedPath(tailSharedPath);
		if (!_sharedPath) _sharedPath = new SharedPath(headInstance, tailSharedPath);
	}
}

Path::Path(const Path& headPath, Instance* tailInstance)
// *****************************************************
:  _sharedPath(NULL)
{
	if (!tailInstance)
		throw Error("Cant't create " + _TName("Path") + " : null tail instance");

	if (!headPath._GetSharedPath()) {
		_sharedPath = tailInstance->_GetSharedPath(NULL);
		if (!_sharedPath) _sharedPath = new SharedPath(tailInstance);
	}
	else {
		Instance* headInstance = headPath.GetHeadInstance();
		SharedPath* tailSharedPath = Path(headPath.GetTailPath(), tailInstance)._GetSharedPath();
		_sharedPath = headInstance->_GetSharedPath(tailSharedPath);
		if (!_sharedPath) _sharedPath = new SharedPath(headInstance, tailSharedPath);
	}
}

Path::Path(const Path& headPath, const Path& tailPath)
// *****************************************************
:  _sharedPath(tailPath._GetSharedPath())
{
    vector<Instance*> instances;
    headPath.GetInstances().Fill(instances);
    
    for (vector<Instance*>::reverse_iterator rit=instances.rbegin() ; rit != instances.rend() ; rit++)
    { Instance* instance=*rit;
        SharedPath* sharedPath = _sharedPath;
        _sharedPath = instance->_GetSharedPath(sharedPath);
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
		char nameSeparator = GetNameSeparator();
		while (!restOfPathName.empty()) {
			size_t pos = restOfPathName.find(nameSeparator);
			Instance* instance = cell->GetInstance(restOfPathName.substr(0, pos));
			if (!instance) throw Error("Cant't create " + _TName("Path") + " : invalid path name");
			cell = instance->GetMasterCell();
			restOfPathName = (pos == string::npos) ? string("") : restOfPathName.substr(pos + 1);
			instanceList.push_back(instance);
		}
		if (!instanceList.empty()) {
			list<Instance*>::reverse_iterator instanceIterator = instanceList.rbegin();
			while (instanceIterator != instanceList.rend()) {
				Instance* headInstance = *instanceIterator;
				SharedPath* tailSharedPath = _sharedPath;
				_sharedPath = headInstance->_GetSharedPath(tailSharedPath);
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

Instance* Path::GetHeadInstance() const
// ************************************
{
	return (_sharedPath) ? _sharedPath->GetHeadInstance() : NULL;
}

Path Path::GetTailPath() const
// ***************************
{
	return Path((_sharedPath) ? _sharedPath->GetTailSharedPath() : NULL);
}

Path Path::GetHeadPath() const
// ***************************
{
	return Path((_sharedPath) ? _sharedPath->GetHeadSharedPath() : NULL);
}

Instance* Path::GetTailInstance() const
// ************************************
{
	return (_sharedPath) ? _sharedPath->GetTailInstance() : NULL;
}

char Path::GetNameSeparator()
// **************************
{
	return SharedPath::GetNameSeparator();
}

string Path::GetName() const
// *************************
{
	return (_sharedPath) ? _sharedPath->GetName() : string("");
}

Cell* Path::GetOwnerCell() const
// *****************************
{
	return (_sharedPath) ? _sharedPath->GetOwnerCell() : NULL;
}

Cell* Path::GetMasterCell() const
// ******************************
{
	return (_sharedPath) ? _sharedPath->GetMasterCell() : NULL;
}

Instances Path::GetInstances() const
// *********************************
{
	return (_sharedPath) ? _sharedPath->GetInstances() : Instances();
}

Transformation Path::GetTransformation(const Transformation& transformation) const
// *******************************************************************************
{
	return (_sharedPath) ? _sharedPath->GetTransformation(transformation) : transformation;
}

bool Path::IsEmpty() const
// ***********************
{
	return (_sharedPath == NULL);
}

void Path::MakeEmpty()
// *******************
{
	_sharedPath = NULL;
}

void Path::SetNameSeparator(char nameSeparator)
// ********************************************
{
	SharedPath::SetNameSeparator(nameSeparator);
}

string Path::_GetString() const
// ****************************
{
	string s = "<" + _TName("Path");
	if (!_sharedPath)
		s += " empty";
	else
		s += " " + GetString(GetName());
	s += ">";
	return s;
}

Record* Path::_GetRecord() const
// ***********************
{
 	Record* record = NULL;
	if (_sharedPath) {
  		record = new Record(GetString(this));
		record->Add(GetSlot("SharedPath", _sharedPath));
	}
	return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
