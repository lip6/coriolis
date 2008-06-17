// ****************************************************************************************************
// File: Slice.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_SLICE
#define HURRICANE_SLICE

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
    public: Gos getGosUnder(const Box& area) const {return _quadTree.getGosUnder(area);};
    public: Components getComponents() const;
    public: Components getComponentsUnder(const Box& area) const;
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


#endif // HURRICANE_SLICE

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
