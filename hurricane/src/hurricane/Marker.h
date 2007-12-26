// ****************************************************************************************************
// File: Marker.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_MARKER
#define HURRICANE_MARKER

#include "Go.h"

namespace Hurricane {



// ****************************************************************************************************
// Marker declaration
// ****************************************************************************************************

class Marker : public Go {
// *********************

// Types
// *****

	public: typedef Go Inherit;

// Attributes
// **********

	private: Cell* _cell;
	private: Marker* _nextOfCellMarkerSet;

// Constructors
// ************

	protected: Marker(Cell* cell);

// Accessors
// *********

	public: virtual Cell* GetCell() const {return _cell;};
	public: virtual Box GetBoundingBox() const = 0;

// Updators
// ********

	public: virtual void Materialize();
	public: virtual void Unmaterialize();

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: Marker* _GetNextOfCellMarkerSet() const {return _nextOfCellMarkerSet;};

	public: void _SetNextOfCellMarkerSet(Marker* marker) {_nextOfCellMarkerSet = marker;};

};



} // End of Hurricane namespace.

#endif // HURRICANE_MARKER

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
