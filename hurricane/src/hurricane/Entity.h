// ****************************************************************************************************
// File: Entity.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_ENTITY
#define HURRICANE_ENTITY

#include "DBo.h"
#include "Entities.h"
#include "Box.h"

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


SetNestedSlotAdapter(Hurricane::Entity)

#endif // HURRICANE_ENTITY

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
