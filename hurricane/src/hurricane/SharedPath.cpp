// ****************************************************************************************************
// File: SharedPath.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "SharedPath.h"
#include "Instance.h"
#include "Cell.h"
#include "Quark.h"
#include "Error.h"

namespace Hurricane {


// ****************************************************************************************************
// SharedPath_Instances declaration
// ****************************************************************************************************

class SharedPath_Instances : public Collection<Instance*> {
// ******************************************************

// Types
// *****

	public: typedef Collection<Instance*> Inherit;

	public: class Locator : public Hurricane::Locator<Instance*> {
	// *********************************************************

		public: typedef Hurricane::Locator<Instance*> Inherit;

		private: const SharedPath* _sharedPath;

		public: Locator(const SharedPath* sharedPath = NULL);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Instance* GetElement() const;
		public: virtual Hurricane::Locator<Instance*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;
	};

// Attributes
// **********

	private: const SharedPath* _sharedPath;

// Constructors
// ************

	public: SharedPath_Instances(const SharedPath* sharedPath = NULL);
	public: SharedPath_Instances(const SharedPath_Instances& instances);

// Operators
// *********

	public: SharedPath_Instances& operator=(const SharedPath_Instances& instances);

// Accessors
// *********

	public: virtual Collection<Instance*>* GetClone() const;
	public: virtual Hurricane::Locator<Instance*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};



// ****************************************************************************************************
// SharedPath implementation
// ****************************************************************************************************

static char NAME_SEPARATOR = '.';

SharedPath::SharedPath(Instance* headInstance, SharedPath* tailSharedPath)
// ***********************************************************************
:  _headInstance(headInstance),
	_tailSharedPath(tailSharedPath),
	_quarkMap(),
	_nextOfInstanceSharedPathMap(NULL)
{
	if (!_headInstance)
		throw Error("Can't create " + _TName("SharedPath") + " : null head instance");

	if (_headInstance->_GetSharedPath(_tailSharedPath))
		throw Error("Can't create " + _TName("SharedPath") + " : already exists");

	if (_tailSharedPath && (_tailSharedPath->GetOwnerCell() != _headInstance->GetMasterCell()))
		throw Error("Can't create " + _TName("SharedPath") + " : incompatible tail path");

	_headInstance->_GetSharedPathMap()._Insert(this);
}

SharedPath::~SharedPath()
// **********************
{
	for_each_quark(quark, _GetQuarks()) quark->Delete(); end_for;

	Cell* cell = _headInstance->GetCell();
	for_each_instance(instance, cell->GetSlaveInstances()) {
		SharedPath* sharedPath = instance->_GetSharedPath(this);
		if (sharedPath) delete sharedPath;
		end_for;
	}
	_headInstance->_GetSharedPathMap()._Remove(this);
}

SharedPath* SharedPath::GetHeadSharedPath() const
// **********************************************
{
	if (!_tailSharedPath) return NULL;

	SharedPath* tailSharedPath = _tailSharedPath->GetHeadSharedPath();

	SharedPath* headSharedPath = _headInstance->_GetSharedPath(tailSharedPath);

	if (!headSharedPath) headSharedPath = new SharedPath(_headInstance, tailSharedPath);

	return headSharedPath;
}

Instance* SharedPath::GetTailInstance() const
// ******************************************
{
	return (_tailSharedPath) ? _tailSharedPath->GetTailInstance() : _headInstance;
}

char SharedPath::GetNameSeparator()
// ********************************
{
	return NAME_SEPARATOR;
}

string SharedPath::GetName() const
// *******************************
{
	string name = "";
	string nameSeparator = "";
	SharedPath* sharedPath = (SharedPath*)this;
	while (sharedPath) {
		name += nameSeparator + GetString(sharedPath->GetHeadInstance()->GetName());
		nameSeparator = GetString(GetNameSeparator());
		sharedPath = sharedPath->GetTailSharedPath();
	}
	return name;
}

Cell* SharedPath::GetOwnerCell() const
// ***********************************
{
	return _headInstance->GetCell();
}

Cell* SharedPath::GetMasterCell() const
// ************************************
{
	Cell* masterCell = NULL;
	SharedPath* sharedPath = (SharedPath*)this;
	while (sharedPath) {
		masterCell = sharedPath->GetHeadInstance()->GetMasterCell();
		sharedPath = sharedPath->GetTailSharedPath();
	}
	return masterCell;
}

Instances SharedPath::GetInstances() const
// ***************************************
{
	return SharedPath_Instances(this);
}

Transformation SharedPath::GetTransformation(const Transformation& transformation) const
// *************************************************************************************
{
	Transformation headTransformation = _headInstance->GetTransformation();
	Transformation tailTransformation =
		(!_tailSharedPath) ? transformation : _tailSharedPath->GetTransformation(transformation);
	return headTransformation.GetTransformation(tailTransformation);
}

void SharedPath::SetNameSeparator(char nameSeparator)
// **************************************************
{
	NAME_SEPARATOR = nameSeparator;
}

string SharedPath::_GetString() const
// **********************************
{
	string s = "<" + _TName("SharedPath");
	string name = GetName();
	if (!name.empty()) s += " " + name;
	s += ">";
	return s;
}

Record* SharedPath::_GetRecord() const
// *****************************
{
 	Record* record = new Record(GetString(this));
	if (record) {
		record->Add(GetSlot("HeadInstance", _headInstance));
		record->Add(GetSlot("TailSharedPath", _tailSharedPath));
		record->Add(GetSlot("Quarks", &_quarkMap));
	}
	return record;
}



// ****************************************************************************************************
// SharedPath::QuarkMap implementation
// ****************************************************************************************************

SharedPath::QuarkMap::QuarkMap()
// *****************************
:	Inherit()
{
}

const Entity* SharedPath::QuarkMap::_GetKey(Quark* quark) const
// ************************************************************
{
	return quark->GetOccurrence().GetEntity();
}

unsigned SharedPath::QuarkMap::_GetHashValue(const Entity* entity) const
// *********************************************************************
{
	return ( (unsigned int)( (unsigned long)entity ) ) / 8;
}

Quark* SharedPath::QuarkMap::_GetNextElement(Quark* quark) const
// *************************************************************
{
	return quark->_GetNextOfSharedPathQuarkMap();
}

void SharedPath::QuarkMap::_SetNextElement(Quark* quark, Quark* nextQuark) const
// *****************************************************************************
{
	quark->_SetNextOfSharedPathQuarkMap(nextQuark);
}



// ****************************************************************************************************
// SharedPath_Instances implementation
// ****************************************************************************************************

SharedPath_Instances::SharedPath_Instances(const SharedPath* sharedPath)
// *********************************************************************
: 	Inherit(),
	_sharedPath(sharedPath)
{
}

SharedPath_Instances::SharedPath_Instances(const SharedPath_Instances& instances)
// ******************************************************************************
: 	Inherit(),
	_sharedPath(instances._sharedPath)
{
}

SharedPath_Instances& SharedPath_Instances::operator=(const SharedPath_Instances& instances)
// *****************************************************************************************
{
	_sharedPath = instances._sharedPath;
	return *this;
}

Collection<Instance*>* SharedPath_Instances::GetClone() const
// **********************************************************
{
	return new SharedPath_Instances(*this);
}

Locator<Instance*>* SharedPath_Instances::GetLocator() const
// *********************************************************
{
	return new Locator(_sharedPath);
}

string SharedPath_Instances::_GetString() const
// ********************************************
{
	string s = "<" + _TName("SharedPath::Instances");
	if (_sharedPath) s += " " + GetString(_sharedPath);
	s += ">";
	return s;
}



// ****************************************************************************************************
// SharedPath_Instances::Locator implementation
// ****************************************************************************************************

SharedPath_Instances::Locator::Locator(const SharedPath* sharedPath)
// *****************************************************************
:	Inherit(),
	_sharedPath(sharedPath)
{
}

SharedPath_Instances::Locator::Locator(const Locator& locator)
// ***********************************************************
:	Inherit(),
	_sharedPath(locator._sharedPath)
{
}

SharedPath_Instances::Locator& SharedPath_Instances::Locator::operator=(const Locator& locator)
// ********************************************************************************************
{
	_sharedPath = locator._sharedPath;
	return *this;
}

Instance* SharedPath_Instances::Locator::GetElement() const
// ********************************************************
{
	return (_sharedPath) ? _sharedPath->GetHeadInstance() : NULL;
}

Locator<Instance*>* SharedPath_Instances::Locator::GetClone() const
// ****************************************************************
{
	return new Locator(*this);
}

bool SharedPath_Instances::Locator::IsValid() const
// ************************************************
{
	return (_sharedPath != NULL);
}

void SharedPath_Instances::Locator::Progress()
// *******************************************
{
	if (_sharedPath) _sharedPath = _sharedPath->GetTailSharedPath();
}

string SharedPath_Instances::Locator::_GetString() const
// *****************************************************
{
	string s = "<" + _TName("SharedPath::Instances::Locator");
	if (_sharedPath) s += " " + GetString(_sharedPath);
	s += ">";
	return s;
}



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
