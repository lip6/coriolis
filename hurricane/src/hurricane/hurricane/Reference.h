// ****************************************************************************************************
// File: ./hurricane/Reference.h
// Authors: J.-P. Chaput
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

#ifndef HURRICANE_REFERENCE
#define HURRICANE_REFERENCE

#include "hurricane/Marker.h"
#include "hurricane/References.h"

namespace Hurricane {



// ****************************************************************************************************
// Reference declaration
// ****************************************************************************************************

class Reference : public Marker {
// *********************************

// Types
// *****

    public: typedef Marker Inherit;
    public: enum Type { Label=1, Position=2 };

// Attributes
// **********

    private: Name _name;
    private: Point _point;
    private: Type  _type;
    private: static DbU::Unit _extend;

// Constructors
// ************

    protected: Reference(Cell* cell, const Name& name, DbU::Unit x, DbU::Unit y, Type type);

    public: static Reference* create(Cell* cell, const Name& name, DbU::Unit x, DbU::Unit y, Type type=Position );
    public: static Reference* create(Cell* cell, const Name& name, const Point& point, Type type=Position );

// Accessors
// *********

    public: virtual Box getBoundingBox() const;
    public: const Name& getName() const {return _name;};
    public: const Point& getPoint() const {return _point;};
    public: Type getType() const { return _type; }

// Updators
// ********

    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);
    public: void setType ( Type type ) { _type=type; }

// Others
// ******

    public: virtual string _getTypeName() const {return _TName("Reference");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};



} // End of Hurricane namespace.

#endif // HURRICANE_REFERENCE


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
