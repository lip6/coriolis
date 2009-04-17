// ****************************************************************************************************
// File: ./hurricane/Entity.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
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

#ifndef HURRICANE_ENTITY
#define HURRICANE_ENTITY

#include "hurricane/DBo.h"
#include "hurricane/Entities.h"
#include "hurricane/Box.h"

namespace Hurricane {

class Cell;
class Quark;
class SharedPath;



// ****************************************************************************************************
// Entity declaration
// ****************************************************************************************************

class Entity : public DBo {
// **********************

#   if !defined(__DOXYGEN_PROCESSOR__)

// Types
// *****

    public: typedef DBo Inherit;

// Constructors
// ************

    protected: Entity();

// Others
// ******

    protected: virtual void _preDestroy();

    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: Quark* _getQuark(SharedPath* sharedPath = NULL) const;

#   endif

// Accessors
// *********

    public: virtual Cell* getCell() const = 0;
    public: virtual Box getBoundingBox() const = 0;

};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Entity);


#endif // HURRICANE_ENTITY


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************
