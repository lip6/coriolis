// ****************************************************************************************************
// File: Reference.h
// Authors: J.-P. Chaput
// Copyright (c) LIP6/UPMC 2006-2006, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_REFERENCE
#define HURRICANE_REFERENCE

#include "Marker.h"
#include "References.h"

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
    public: static Unit _extend;

// Constructors
// ************

	protected: Reference(Cell* cell, const Name& name, Unit x, Unit y);

	public: static Reference* Create(Cell* cell, const Name& name, Unit x, Unit y );
	public: static Reference* Create(Cell* cell, const Name& name, const Point& point );

// Accessors
// *********

    public: virtual Box GetBoundingBox() const;
	public: const Name& GetName() const {return _name;};
	public: const Point& GetPoint() const {return _point;};

// Updators
// ********

	public: virtual void Translate(const Unit& dx, const Unit& dy);

// Others
// ******

	public: virtual string _GetTypeName() const {return _TName("Reference");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	//public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
	//public: virtual void _Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation);
	//public: virtual void _Highlight(View* view, const Box& updateArea, const Transformation& transformation);

};



} // End of Hurricane namespace.

#endif // HURRICANE_REFERENCE

// ****************************************************************************************************
// Copyright (c) LIP6/UPMC 2006-2006, All Rights Reserved
// ****************************************************************************************************
