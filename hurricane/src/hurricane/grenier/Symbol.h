// ****************************************************************************************************
// File: Symbol.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_SYMBOL
#define HURRICANE_SYMBOL

#include "DBo.h"
#include "Symbols.h"
#include "Cell.h"
#include "Transformation.h"
#include "IntrusiveSet.h"
#include "IntrusiveMap.h"

namespace Hurricane {

class Library;
class Port;
class View;



// ****************************************************************************************************
// Symbol declaration
// ****************************************************************************************************

class Symbol : public DBo {
// **********************

// Types
// *****

	public: typedef DBo Inherit;

	public: class CellSet : public IntrusiveSet<Cell> {
	// **********************************************

		public: typedef IntrusiveSet<Cell> Inherit;
	
		public: CellSet();

		public: virtual unsigned _GetHashValue(Cell* cell) const;
		public: virtual Cell* _GetNextElement(Cell* cell) const;
		public: virtual void _SetNextElement(Cell* cell, Cell* nextCell) const;

	};

	public: typedef list<Primitive*> PrimitiveList;

// Attributes
// **********

	private: Library* _library;
	private: Name _name;
	private: CellSet _cellSet;
	private: PrimitiveList _primitiveList;
	private: Box _boundingBox;
	private: Symbol* _nextOfLibrarySymbolMap;

// Constructors
// ************

	protected: Symbol(Library* library, const Name& name);

	public: static Symbol* Create(Library* library, const Name& name);
	public: static Symbol* CreateFor(Cell* cell);

// Accessors
// *********

	public: Library* GetLibrary() const {return _library;};
	public: const Name& GetName() const {return _name;};
	public: Cells GetCells() const {return _cellSet.GetElements();};
	public: Primitives GetPrimitives() const {return GetCollection(_primitiveList);};
	public: Box GetBoundingBox() const;
	public: Port* GetPort(const Name& name) const;

// Updators
// ********

	public: void SetName(const Name& name);

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("Symbol");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;
	public: CellSet& _GetCellSet() {return _cellSet;};
	public: PrimitiveList& _GetPrimitiveList() {return _primitiveList;};
	public: Symbol* _GetNextOfLibrarySymbolMap() const {return _nextOfLibrarySymbolMap;};

	public: void _SetNextOfLibrarySymbolMap(Symbol* symbol) {_nextOfLibrarySymbolMap = symbol;};

	public: void _Fit(const Box& box);
	public: void _Unfit(const Box& box);

	public: bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
	public: void _Draw(View* view, const Box& updateArea, const Transformation& transformation);
	public: void _Highlight(View* view, const Box& updateArea, const Transformation& transformation);

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Symbol)

#endif // HURRICANE_SYMBOL

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
