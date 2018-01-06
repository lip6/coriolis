// ****************************************************************************************************
// File: ./hurricane/DRCError.h
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

#ifndef HURRICANE_DRCERROR
#define HURRICANE_DRCERROR

#include "hurricane/Marker.h"

namespace Hurricane {



// ****************************************************************************************************
// DRCError declaration
// ****************************************************************************************************

class DRCError : public Marker {
// *****************************

// Types
// *****

    public: typedef Marker Inherit;

// Attributes
// **********

    public: Name _name;
    public: Box _boundingBox;

// Constructors
// ************

    protected: DRCError(Cell* cell, const Name& name, const Box& boundingBox);

    public: static DRCError* create(Cell* cell, const Name& name, const Box& boundingBox);

// Accessors
// *********

    public: virtual Box getBoundingBox() const {return _boundingBox;};
    public: const Name& getName() const {return _name;};

// Updators
// ********

    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);

// Others
// ******

    public: virtual string _getTypeName() const {return _TName("DRCError");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


} // End of Hurricane namespace.

#endif // HURRICANE_DRCERROR


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
