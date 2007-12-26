// ****************************************************************************************************
// File: Relation.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_RELATION
#define HURRICANE_RELATION

#include "Property.h"

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

	public: DBo* GetMasterOwner() const {return _masterOwner;};
	public: DBos GetSlaveOwners() const;

// Managers
// ********

	public: virtual void OnReleasedBy(DBo* owner);

// Others
// ******

	protected: virtual void _PostCreate();

	public: virtual string _GetTypeName() const {return _TName("Relation");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

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

	public: static StandardRelation* Create(DBo* masterOwner, const Name& name);

// Accessors
// *********

	public: virtual Name GetName() const {return _name;};

// Others
// ******

	public: virtual string _GetTypeName() const {return _TName("StandardRelation");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Relation)
SetNestedSlotAdapter(Hurricane::StandardRelation)

#endif // HURRICANE_RELATION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
