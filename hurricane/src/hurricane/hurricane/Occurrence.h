// ****************************************************************************************************
// File: ./hurricane/Occurrence.h
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

#ifndef HURRICANE_OCCURENCE
#define HURRICANE_OCCURENCE

#include "hurricane/Path.h"
#include "hurricane/Name.h"
#include "hurricane/Properties.h"

namespace Hurricane {

class Entity;
class SharedPath;
class Quark;
class BasicLayer;


// ****************************************************************************************************
// Occurrence declaration
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
    public: Occurrence getNetOccurrence() const;
    public: Box getBoundingBox() const;
    public: Box getBoundingBox(const BasicLayer*) const;

// Predicates
// **********

    public: bool isValid() const {return (_entity != NULL);};
    public: bool isBelowTerminalNetlist() const;
    public: bool hasProperty() const;

// Updators
// ********

    public: void makeInvalid();
    public: void put(Property* property);
    public: void remove(Property* property);
    public: void removeProperty(const Name& name);
    public: void clearProperties();

// Others
// ******

    public: string getName() const;
    public: string _getTypeName() const { return _TName("Occurrence"); };
    public: string _getString() const;
    public: string getCompactString() const;
    public: void toJson(JsonWriter*) const;
    public: Record* _getRecord() const;
    public: SharedPath* _getSharedPath() const {return _sharedPath;};
    public: Quark* _getQuark() const;

};


// ****************************************************************************************************
// JsonOccurrence declaration
// ****************************************************************************************************


class JsonOccurrence : public JsonObject {
// **********************************

  public: static void initialize();
  public: JsonOccurrence(unsigned long);
  public: virtual string getTypeName() const;
  public: virtual JsonOccurrence* clone(unsigned long) const;
  public: virtual void toData(JsonStack&); 
};


  typedef  std::set<Occurrence>  OccurrenceSet;


} // End of Hurricane namespace.


INSPECTOR_PV_SUPPORT(Hurricane::Occurrence);


#endif // HURRICANE_OCCURENCE


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
