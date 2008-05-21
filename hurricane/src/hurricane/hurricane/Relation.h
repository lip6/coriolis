// ****************************************************************************************************
// File: Relation.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_RELATION
#define HURRICANE_RELATION

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


SetNestedSlotAdapter(Hurricane::Relation)
SetNestedSlotAdapter(Hurricane::StandardRelation)

#endif // HURRICANE_RELATION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
