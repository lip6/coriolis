// ****************************************************************************************************
// File: ./hurricane/Marker.h
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
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
