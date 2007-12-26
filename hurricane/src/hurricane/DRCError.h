// ****************************************************************************************************
// File: DRCError.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_DRCERROR
#define HURRICANE_DRCERROR

#include "Marker.h"

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

	public: static DRCError* Create(Cell* cell, const Name& name, const Box& boundingBox);

// Accessors
// *********

	public: virtual Box GetBoundingBox() const {return _boundingBox;};
	public: const Name& GetName() const {return _name;};

// Updators
// ********

	public: virtual void Translate(const Unit& dx, const Unit& dy);

// Others
// ******

	public: virtual string _GetTypeName() const {return _TName("DRCError");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

//	public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
//	public: virtual void _Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation);
//	public: virtual void _Highlight(View* view, const Box& updateArea, const Transformation& transformation);
//
};


} // End of Hurricane namespace.

#endif // HURRICANE_DRCERROR

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
