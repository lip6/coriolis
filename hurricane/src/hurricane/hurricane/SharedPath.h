// ****************************************************************************************************
// File: ./hurricane/SharedPath.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
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

#ifndef HURRICANE_SHARED_PATH
#define HURRICANE_SHARED_PATH

#include "hurricane/Instances.h"
#include "hurricane/SharedPathes.h"
#include "hurricane/Quark.h"
#include "hurricane/Transformation.h"
#include "hurricane/IntrusiveMap.h"

namespace Hurricane {

class Cell;
class Entity;



// ****************************************************************************************************
// SharedPath declaration
// ****************************************************************************************************

class SharedPath {
// *************

// Types
// *****

    public: class QuarkMap : public IntrusiveMap<const Entity*, Quark> {
    // ***************************************************************

        public: typedef IntrusiveMap<const Entity*, Quark> Inherit;
    
        public: QuarkMap();

        public: virtual const Entity* _getKey(Quark* quark) const;
        public: virtual unsigned _getHashValue(const Entity* entity) const;
        public: virtual Quark* _getNextElement(Quark* quark) const;
        public: virtual void _setNextElement(Quark* quark, Quark* nextQuark) const;

    };

// Attributes
// **********

    private: static unsigned int _idCounter;
    private: unsigned int _id;
    private: Instance* _headInstance;
    private: SharedPath* _tailSharedPath;
    private: QuarkMap _quarkMap;
    private: SharedPath* _nextOfInstanceSharedPathMap;

// Constructors
// ************

    public: SharedPath(Instance* headInstance, SharedPath* tailSharedPath = NULL);

    private: SharedPath(const SharedPath& sharedPath);
                // not implemented to forbid copy construction

// Destructor
// **********

    public: ~SharedPath();

// Operators
// *********

    private: SharedPath& operator=(const SharedPath& sharedPath);
                // not implemented to forbid assignment

// Accessors
// *********

    public: static char getNameSeparator();

    public: unsigned int  getId() const { return _id; }
    public: Instance* getHeadInstance() const {return _headInstance;};
    public: SharedPath* getTailSharedPath() const {return _tailSharedPath;};
    public: SharedPath* getHeadSharedPath() const;
    public: Instance* getTailInstance() const;
    public: string getName() const;
    public: string getJsonString(unsigned long flags) const;
    public: Cell* getOwnerCell() const;
    public: Cell* getMasterCell() const;
    public: Instances getInstances() const;
    public: Transformation getTransformation(const Transformation& transformation = Transformation()) const;

// Updators
// ********

    public: static void setNameSeparator(char nameSeparator);

// Accessors
// *********

    public: string _getTypeName() const { return _TName("SharedPath"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

    public: Quark* _getQuark(const Entity* entity) const {return _quarkMap.getElement(entity);};
    public: Quarks _getQuarks() const {return _quarkMap.getElements();};
    public: QuarkMap& _getQuarkMap() {return _quarkMap;};
    public: SharedPath* _getNextOfInstanceSharedPathMap() const {return _nextOfInstanceSharedPathMap;};

    public: void _setNextOfInstanceSharedPathMap(SharedPath* sharedPath) {_nextOfInstanceSharedPathMap = sharedPath;};

};



} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::SharedPath);



#endif // HURRICANE_SHARED_PATH


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
