// ****************************************************************************************************
// File: Marker.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_MARKER
#define HURRICANE_MARKER

#include "hurricane/Go.h"

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

    public: virtual Cell* getCell() const {return _cell;};
    public: virtual Box getBoundingBox() const = 0;

// Updators
// ********

    public: virtual void materialize();
    public: virtual void unmaterialize();

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: Marker* _getNextOfCellMarkerSet() const {return _nextOfCellMarkerSet;};

    public: void _setNextOfCellMarkerSet(Marker* marker) {_nextOfCellMarkerSet = marker;};

};



} // End of Hurricane namespace.

#endif // HURRICANE_MARKER

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
