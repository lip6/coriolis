// ****************************************************************************************************
// File: ./SharedPath.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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
#include "hurricane/SharedPath.h"
#include "hurricane/Instance.h"
#include "hurricane/Cell.h"
#include "hurricane/Quark.h"
#include "hurricane/Error.h"

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

        public: virtual Instance* getElement() const;
        public: virtual Hurricane::Locator<Instance*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;
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

    public: virtual Collection<Instance*>* getClone() const;
    public: virtual Hurricane::Locator<Instance*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// SharedPath implementation
// ****************************************************************************************************

static char NAME_SEPARATOR = '.';
unsigned int  SharedPath::_idCounter = 0;


SharedPath::SharedPath(Instance* headInstance, SharedPath* tailSharedPath)
// ***********************************************************************
  : _id(_idCounter++),
    _headInstance(headInstance),
    _tailSharedPath(tailSharedPath),
    _quarkMap(),
    _nextOfInstanceSharedPathMap(NULL)
{
    if (_idCounter == std::numeric_limits<unsigned int>::max()) {
      throw Error( "SharedName::SharedName(): Identifier counter has reached it's limit (%d bits)."
                 , std::numeric_limits<unsigned int>::digits );
    }

    if (!_headInstance)
        throw Error("Can't create " + _TName("SharedPath") + " : null head instance");

    if (_headInstance->_getSharedPath(_tailSharedPath))
        throw Error("Can't create " + _TName("SharedPath") + " : already exists");

    if (_tailSharedPath && (_tailSharedPath->getOwnerCell() != _headInstance->getMasterCell()))
        throw Error( "Can't create %s, incompatible tail path between:\n"
                     "        - head instance %s\n"
                     "        - tail path %s\n"
                     "        - head owner %s\n"
                     "        - tail owner %s\n"
                   , _TName("SharedPath").c_str()
                   , getString(_headInstance  ).c_str()
                   , getString(_tailSharedPath).c_str()
                   , getString(_headInstance  ->getMasterCell()).c_str()
                   , getString(_tailSharedPath->getOwnerCell ()).c_str()
                   );

    _headInstance->_getSharedPathMap()._insert(this);
}

SharedPath::~SharedPath()
// **********************
{
    for_each_quark(quark, _getQuarks()) quark->destroy(); end_for;

    Cell* cell = _headInstance->getCell();
    for_each_instance(instance, cell->getSlaveInstances()) {
        SharedPath* sharedPath = instance->_getSharedPath(this);
        if (sharedPath) delete sharedPath;
        end_for;
    }
    _headInstance->_getSharedPathMap()._remove(this);
}

SharedPath* SharedPath::getHeadSharedPath() const
// **********************************************
{
    if (!_tailSharedPath) return NULL;

    SharedPath* tailSharedPath = _tailSharedPath->getHeadSharedPath();

    SharedPath* headSharedPath = _headInstance->_getSharedPath(tailSharedPath);

    if (!headSharedPath) headSharedPath = new SharedPath(_headInstance, tailSharedPath);

    return headSharedPath;
}

Instance* SharedPath::getTailInstance() const
// ******************************************
{
    return (_tailSharedPath) ? _tailSharedPath->getTailInstance() : _headInstance;
}

char SharedPath::getNameSeparator()
// ********************************
{
    return NAME_SEPARATOR;
}

string SharedPath::getName() const
// *******************************
{
    string name = "";
    string nameSeparator = "";
    SharedPath* sharedPath = (SharedPath*)this;
    while (sharedPath) {
        name += nameSeparator + getString(sharedPath->getHeadInstance()->getName());
        nameSeparator = getString(getNameSeparator());
        sharedPath = sharedPath->getTailSharedPath();
    }
    return name;
}

string SharedPath::getJsonString(unsigned long flags) const
// ********************************************************
{
    string      name       = "";
    SharedPath* sharedPath = (SharedPath*)this;
    while (sharedPath) {
      if (flags & JsonWriter::DesignBlobMode) 
        name += getString(sharedPath->getHeadInstance()->getId());
      else
        name += getString(sharedPath->getHeadInstance()->getName());

      sharedPath =           sharedPath->getTailSharedPath();
      if (sharedPath) name += getNameSeparator();
    }
    return name;
}

Cell* SharedPath::getOwnerCell() const
// ***********************************
{
    return _headInstance->getCell();
}

Cell* SharedPath::getMasterCell() const
// ************************************
{
    Cell* masterCell = NULL;
    SharedPath* sharedPath = (SharedPath*)this;
    while (sharedPath) {
        masterCell = sharedPath->getHeadInstance()->getMasterCell();
        sharedPath = sharedPath->getTailSharedPath();
    }
    return masterCell;
}

Instances SharedPath::getInstances() const
// ***************************************
{
    return SharedPath_Instances(this);
}

Transformation SharedPath::getTransformation(const Transformation& transformation) const
// *************************************************************************************
{
    Transformation headTransformation = _headInstance->getTransformation();
    Transformation tailTransformation =
        (!_tailSharedPath) ? transformation : _tailSharedPath->getTransformation(transformation);
    return headTransformation.getTransformation(tailTransformation);
}

void SharedPath::setNameSeparator(char nameSeparator)
// **************************************************
{
    NAME_SEPARATOR = nameSeparator;
}

string SharedPath::_getString() const
// **********************************
{
    string s = "<" + _TName("SharedPath");
    string name = getName();
    if (!name.empty()) s += " " + name;
    s += ">";
    return s;
}

Record* SharedPath::_getRecord() const
// *****************************
{
     Record* record = new Record(getString(this));
    if (record) {
        record->add(getSlot("HeadInstance", _headInstance));
        record->add(getSlot("TailSharedPath", _tailSharedPath));
        record->add(getSlot("Quarks", &_quarkMap));
    }
    return record;
}



// ****************************************************************************************************
// SharedPath::QuarkMap implementation
// ****************************************************************************************************

SharedPath::QuarkMap::QuarkMap()
// *****************************
:    Inherit()
{
}

const Entity* SharedPath::QuarkMap::_getKey(Quark* quark) const
// ************************************************************
{
    return quark->getOccurrence().getEntity();
}

unsigned SharedPath::QuarkMap::_getHashValue(const Entity* entity) const
// *********************************************************************
{
  return entity->getId() / 8;
}

Quark* SharedPath::QuarkMap::_getNextElement(Quark* quark) const
// *************************************************************
{
    return quark->_getNextOfSharedPathQuarkMap();
}

void SharedPath::QuarkMap::_setNextElement(Quark* quark, Quark* nextQuark) const
// *****************************************************************************
{
    quark->_setNextOfSharedPathQuarkMap(nextQuark);
}



// ****************************************************************************************************
// SharedPath_Instances implementation
// ****************************************************************************************************

SharedPath_Instances::SharedPath_Instances(const SharedPath* sharedPath)
// *********************************************************************
:     Inherit(),
    _sharedPath(sharedPath)
{
}

SharedPath_Instances::SharedPath_Instances(const SharedPath_Instances& instances)
// ******************************************************************************
:     Inherit(),
    _sharedPath(instances._sharedPath)
{
}

SharedPath_Instances& SharedPath_Instances::operator=(const SharedPath_Instances& instances)
// *****************************************************************************************
{
    _sharedPath = instances._sharedPath;
    return *this;
}

Collection<Instance*>* SharedPath_Instances::getClone() const
// **********************************************************
{
    return new SharedPath_Instances(*this);
}

Locator<Instance*>* SharedPath_Instances::getLocator() const
// *********************************************************
{
    return new Locator(_sharedPath);
}

string SharedPath_Instances::_getString() const
// ********************************************
{
    string s = "<" + _TName("SharedPath::Instances");
    if (_sharedPath) s += " " + getString(_sharedPath);
    s += ">";
    return s;
}



// ****************************************************************************************************
// SharedPath_Instances::Locator implementation
// ****************************************************************************************************

SharedPath_Instances::Locator::Locator(const SharedPath* sharedPath)
// *****************************************************************
:    Inherit(),
    _sharedPath(sharedPath)
{
}

SharedPath_Instances::Locator::Locator(const Locator& locator)
// ***********************************************************
:    Inherit(),
    _sharedPath(locator._sharedPath)
{
}

SharedPath_Instances::Locator& SharedPath_Instances::Locator::operator=(const Locator& locator)
// ********************************************************************************************
{
    _sharedPath = locator._sharedPath;
    return *this;
}

Instance* SharedPath_Instances::Locator::getElement() const
// ********************************************************
{
    return (_sharedPath) ? _sharedPath->getHeadInstance() : NULL;
}

Locator<Instance*>* SharedPath_Instances::Locator::getClone() const
// ****************************************************************
{
    return new Locator(*this);
}

bool SharedPath_Instances::Locator::isValid() const
// ************************************************
{
    return (_sharedPath != NULL);
}

void SharedPath_Instances::Locator::progress()
// *******************************************
{
    if (_sharedPath) _sharedPath = _sharedPath->getTailSharedPath();
}

string SharedPath_Instances::Locator::_getString() const
// *****************************************************
{
    string s = "<" + _TName("SharedPath::Instances::Locator");
    if (_sharedPath) s += " " + getString(_sharedPath);
    s += ">";
    return s;
}



} // End of Hurricane namespace.



// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
