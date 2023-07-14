// ****************************************************************************************************
// File: ./hurricane/Slice.h
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

#pragma  once
#include "hurricane/QuadTree.h"
#include "hurricane/Components.h"
#include "hurricane/Markers.h"
#include "hurricane/Transformation.h"

namespace Hurricane {

class Cell;
class Layer;
class BasicLayer;



// ****************************************************************************************************
// Slice declaration
// ****************************************************************************************************

class Slice {
// ********

// Attributes
// **********

    private: Cell* _cell;
    private: const Layer* _layer;
    private: QuadTree _quadTree;
    private: Slice* _nextOfCellSliceMap;

// Constructors
// ************

    protected: Slice(Cell* cell, const Layer* layer);

    private: Slice(const Slice& slice); // not implemented to forbid copy construction

// Destructor
// **********

    protected: ~Slice();

// Operators
// *********

    private: Slice& operator=(const Slice& slice); // not implemented to forbid assignment

// Accessors
// *********

    public: Cell* getCell() const {return _cell;};
    public: const Layer* getLayer() const {return _layer;};
    public: const Box& getBoundingBox() const {return _quadTree.getBoundingBox();};
    public: Gos getGos() const {return _quadTree.getGos();};
    public: Gos getGosUnder(const Box& area, DbU::Unit threshold=0) const {return _quadTree.getGosUnder(area,threshold);};
    public: Components getComponents() const;
    public: Components getComponentsUnder(const Box& area, DbU::Unit threshold=0) const;
    public: Markers getMarkers() const;
    public: Markers getMarkersUnder(const Box& area) const;

// Predicates
// **********

    public: bool isEmpty() const {return _quadTree.isEmpty();};

// Others
// ******

    public: static Slice* _create(Cell* cell, const Layer* layer);

    public: void _destroy();

    public: string _getTypeName() const { return _TName("Slice"); };
    public: string _getString() const;
    public: Record* _getRecord() const;
    public: QuadTree* _getQuadTree() {return &_quadTree;};
    public: Slice* _getNextOfCellSliceMap() const {return _nextOfCellSliceMap;};

    public: void _setNextOfCellSliceMap(Slice* slice) {_nextOfCellSliceMap = slice;};

};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Slice);


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
