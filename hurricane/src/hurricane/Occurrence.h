// ****************************************************************************************************
// File: Occurrence.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_OCCURENCE
#define HURRICANE_OCCURENCE

#include "Path.h"
#include "Name.h"
#include "Properties.h"

namespace Hurricane {

class Entity;
class SharedPath;
class Quark;



// ****************************************************************************************************
// Occurente declaration
// ****************************************************************************************************

class Occurrence {
// *************

// Attributes
// **********

	private: Entity* _entity;
	private: SharedPath* _sharedPath;

// Constructors
// ************

	public: Occurrence(const Entity* entity = NULL);
	public: Occurrence(const Entity* entity, const Path& path);
	public: Occurrence(const Occurrence& occurrence);

// Operators
// *********

	public: Occurrence& operator=(const Occurrence& occurrence);

	public: bool operator==(const Occurrence& occurrence) const;
	public: bool operator!=(const Occurrence& occurrence) const;

	public: bool operator<(const Occurrence& occurrence) const; // for stl set -> less predicate

// Accessors
// *********

	public: Entity* GetEntity() const {return _entity;};
	public: Path GetPath() const {return Path(_sharedPath);};
	public: Cell* GetOwnerCell() const;
	public: Cell* GetMasterCell() const;
	public: Property* GetProperty(const Name& name) const;
	public: Properties GetProperties() const;
	public: Box GetBoundingBox() const;

// Predicates
// **********

	public: bool IsValid() const {return (_entity != NULL);};
	public: bool HasProperty() const;

// Updators
// ********

	public: void MakeInvalid();
	public: void Put(Property* property);
	public: void Remove(Property* property);
	public: void RemoveProperty(const Name& name);
	public: void ClearProperties();

// Others
// ******

    public: string GetName() const;
    public: string _GetTypeName() const { return _TName("Occurrence"); };
	public: string _GetString() const;
	public: Record* _GetRecord() const;
	public: SharedPath* _GetSharedPath() const {return _sharedPath;};
	public: Quark* _GetQuark() const;

};



} // End of Hurricane namespace.


ValueIOStreamSupport(Hurricane::Occurrence)


#endif // HURRICANE_OCCURENCE

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
