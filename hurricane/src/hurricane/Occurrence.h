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

    public: Entity* getEntity() const {return _entity;};
    public: Path getPath() const {return Path(_sharedPath);};
    public: Cell* getOwnerCell() const;
    public: Cell* getMasterCell() const;
    public: Property* getProperty(const Name& name) const;
    public: Properties getProperties() const;
    public: Box getBoundingBox() const;

// Predicates
// **********

    public: bool isValid() const {return (_entity != NULL);};
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

    public: string getName() const;
    public: string _getTypeName() const { return _TName("Occurrence"); };
    public: string _getString() const;
    public: Record* _getRecord() const;
    public: SharedPath* _getSharedPath() const {return _sharedPath;};
    public: Quark* _getQuark() const;

};



} // End of Hurricane namespace.


ValueIOStreamSupport(Hurricane::Occurrence)


#endif // HURRICANE_OCCURENCE

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
