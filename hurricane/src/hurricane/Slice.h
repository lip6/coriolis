// ****************************************************************************************************
// File: Slice.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_SLICE
#define HURRICANE_SLICE

#include "QuadTree.h"
#include "Components.h"
#include "Markers.h"
#include "Transformation.h"

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
	private: Layer* _layer;
	private: QuadTree _quadTree;
	private: Slice* _nextOfCellSliceMap;

// Constructors
// ************

	protected: Slice(Cell* cell, Layer* layer);

	private: Slice(const Slice& slice); // not implemented to forbid copy construction

// Destructor
// **********

	protected: ~Slice();

// Operators
// *********

	private: Slice& operator=(const Slice& slice); // not implemented to forbid assignment

// Accessors
// *********

	public: Cell* GetCell() const {return _cell;};
	public: Layer* GetLayer() const {return _layer;};
	public: const Box& GetBoundingBox() const {return _quadTree.GetBoundingBox();};
	public: Gos GetGos() const {return _quadTree.GetGos();};
	public: Gos GetGosUnder(const Box& area) const {return _quadTree.GetGosUnder(area);};
	public: Components GetComponents() const;
	public: Components GetComponentsUnder(const Box& area) const;
	public: Markers GetMarkers() const;
	public: Markers GetMarkersUnder(const Box& area) const;

// Predicates
// **********

	public: bool IsEmpty() const {return _quadTree.IsEmpty();};

// Others
// ******

	public: static Slice* _Create(Cell* cell, Layer* layer);

	public: void _Delete();

    public: string _GetTypeName() const { return _TName("Slice"); };
	public: string _GetString() const;
	public: Record* _GetRecord() const;
	public: QuadTree* _GetQuadTree() {return &_quadTree;};
	public: Slice* _GetNextOfCellSliceMap() const {return _nextOfCellSliceMap;};

	public: void _SetNextOfCellSliceMap(Slice* slice) {_nextOfCellSliceMap = slice;};

	//public: void _Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation);

};


} // End of Hurricane namespace.

#endif // HURRICANE_SLICE

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
