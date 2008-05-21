// ****************************************************************************************************
// File: DRCError.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
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

    public: virtual void translate(const Unit& dx, const Unit& dy);

// Others
// ******

    public: virtual string _getTypeName() const {return _TName("DRCError");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


} // End of Hurricane namespace.

#endif // HURRICANE_DRCERROR

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
