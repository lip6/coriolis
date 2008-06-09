// ****************************************************************************************************
// File: Reference.h
// Authors: J.-P. Chaput
// Copyright (c) LIP6/UPMC 2006-2006, All Rights Reserved
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

// Attributes
// **********

    public: Name _name;
    public: Point _point;
    public: static DbU::Unit _extend;

// Constructors
// ************

    protected: Reference(Cell* cell, const Name& name, DbU::Unit x, DbU::Unit y);

    public: static Reference* create(Cell* cell, const Name& name, DbU::Unit x, DbU::Unit y );
    public: static Reference* create(Cell* cell, const Name& name, const Point& point );

// Accessors
// *********

    public: virtual Box getBoundingBox() const;
    public: const Name& getName() const {return _name;};
    public: const Point& getPoint() const {return _point;};

// Updators
// ********

    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);

// Others
// ******

    public: virtual string _getTypeName() const {return _TName("Reference");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};



} // End of Hurricane namespace.

#endif // HURRICANE_REFERENCE

// ****************************************************************************************************
// Copyright (c) LIP6/UPMC 2006-2006, All Rights Reserved
// ****************************************************************************************************
