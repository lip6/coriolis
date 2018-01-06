// ****************************************************************************************************
// File: ./hurricane/Relation.h
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

#ifndef HURRICANE_RELATION_H
#define HURRICANE_RELATION_H

#include "hurricane/Property.h"

namespace Hurricane {



// ****************************************************************************************************
// Relation declaration
// ****************************************************************************************************

class Relation : public SharedProperty {
// ***********************************

// Types
// *****

    public: typedef SharedProperty Inherit;

// Attributes
// **********

    private: DBo* _masterOwner;

// Constructors
// ************

    protected: Relation(DBo* masterOwner);

// Accessors
// *********

    public: DBo* getMasterOwner() const {return _masterOwner;};
    public: DBos getSlaveOwners() const;

// Managers
// ********

    public: virtual void onReleasedBy(DBo* owner);

// Others
// ******

    public: void _setMasterOwner(DBo* owner) {_masterOwner=owner; }
    protected: virtual void _postCreate();

    public: virtual string _getTypeName() const {return _TName("Relation");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};



// ****************************************************************************************************
// StandardRelation declaration
// ****************************************************************************************************

class StandardRelation : public Relation {
// *************************************

// Types
// *****

    public: typedef Relation Inherit;

// Attributes
// **********

    private: Name _name;

// Constructors
// ************

    protected: StandardRelation(DBo* masterOwner, const Name& name);

    public: static StandardRelation* create(DBo* masterOwner, const Name& name);

// Accessors
// *********

    public: virtual Name getName() const {return _name;};

// Others
// ******

    public: virtual string _getTypeName() const {return _TName("StandardRelation");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


} // End of Hurricane namespace.


#endif // HURRICANE_RELATION


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
