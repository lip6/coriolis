// ****************************************************************************************************
// File: Library.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_LIBRARY
#define HURRICANE_LIBRARY

#include "DBo.h"
#include "Libraries.h"
#include "Cell.h"
#include "IntrusiveMap.h"

namespace Hurricane {

class DataBase;



// ****************************************************************************************************
// Library declaration
// ****************************************************************************************************

class Library : public DBo {
// ***********************

// Types
// *****

	public: typedef DBo Inherit;

	public: class LibraryMap : public IntrusiveMap<Name, Library> {
	// **********************************************************

		public: typedef IntrusiveMap<Name, Library> Inherit;
	
		public: LibraryMap();

		public: virtual Name _GetKey(Library* library) const;
		public: virtual unsigned _GetHashValue(Name name) const;
		public: virtual Library* _GetNextElement(Library* library) const;
		public: virtual void _SetNextElement(Library* library, Library* nextLibrary) const;

	};

	public: class CellMap : public IntrusiveMap<Name, Cell> {
	// ****************************************************

		public: typedef IntrusiveMap<Name, Cell> Inherit;
	
		public: CellMap();

		public: virtual Name _GetKey(Cell* cell) const;
		public: virtual unsigned _GetHashValue(Name name) const;
		public: virtual Cell* _GetNextElement(Cell* cell) const;
		public: virtual void _SetNextElement(Cell* cell, Cell* nextCell) const;

	};

// Attributes
// **********

	private: DataBase* _dataBase;
	private: Library* _library;
	private: Name _name;
	private: LibraryMap _libraryMap;
	private: CellMap _cellMap;
	private: Library* _nextOfLibraryLibraryMap;

// Constructors
// ************

	protected: Library(DataBase* dataBase, Library* library, const Name& name);

	public: static Library* Create(DataBase* dataBase, const Name& name);
	public: static Library* Create(Library* library, const Name& name);

// Accessors
// *********

	public: DataBase* GetDataBase() const {return _dataBase;};
	public: Library* GetLibrary() const {return _library;};
	public: const Name& GetName() const {return _name;};
	public: Library* GetLibrary(const Name& name) const {return _libraryMap.GetElement(name);};
	public: Libraries GetLibraries() const {return _libraryMap.GetElements();};
	public: Cell* GetCell(const Name& name) const {return _cellMap.GetElement(name);};
	public: Cells GetCells() const {return _cellMap.GetElements();};

// Updators
// ********

	public: void SetName(const Name& name);

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("Library");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;
	public: LibraryMap& _GetLibraryMap() {return _libraryMap;};
	public: CellMap& _GetCellMap() {return _cellMap;};
	public: Library* _GetNextOfLibraryLibraryMap() const {return _nextOfLibraryLibraryMap;};

	public: void _SetNextOfLibraryLibraryMap(Library* library) {_nextOfLibraryLibraryMap = library;};

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Library)

#endif // HURRICANE_LIBRARY

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
