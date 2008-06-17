// ****************************************************************************************************
// File: Library.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_LIBRARY
#define HURRICANE_LIBRARY

#include "hurricane/DBo.h"
#include "hurricane/Libraries.h"
#include "hurricane/Cell.h"
#include "hurricane/IntrusiveMap.h"

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

        public: virtual Name _getKey(Library* library) const;
        public: virtual unsigned _getHashValue(Name name) const;
        public: virtual Library* _getNextElement(Library* library) const;
        public: virtual void _setNextElement(Library* library, Library* nextLibrary) const;

    };

    public: class CellMap : public IntrusiveMap<Name, Cell> {
    // ****************************************************

        public: typedef IntrusiveMap<Name, Cell> Inherit;
    
        public: CellMap();

        public: virtual Name _getKey(Cell* cell) const;
        public: virtual unsigned _getHashValue(Name name) const;
        public: virtual Cell* _getNextElement(Cell* cell) const;
        public: virtual void _setNextElement(Cell* cell, Cell* nextCell) const;

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

    public: static Library* create(DataBase* dataBase, const Name& name);
    public: static Library* create(Library* library, const Name& name);

// Accessors
// *********

    public: DataBase* getDataBase() const {return _dataBase;};
    public: Library* getLibrary() const {return _library;};
    public: const Name& getName() const {return _name;};
    public: Library* getLibrary(const Name& name) const {return _libraryMap.getElement(name);};
    public: Libraries getLibraries() const {return _libraryMap.getElements();};
    public: Cell* getCell(const Name& name) const {return _cellMap.getElement(name);};
    public: Cells getCells() const {return _cellMap.getElements();};

// Updators
// ********

    public: void setName(const Name& name);

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("Library");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: LibraryMap& _getLibraryMap() {return _libraryMap;};
    public: CellMap& _getCellMap() {return _cellMap;};
    public: Library* _getNextOfLibraryLibraryMap() const {return _nextOfLibraryLibraryMap;};

    public: void _setNextOfLibraryLibraryMap(Library* library) {_nextOfLibraryLibraryMap = library;};

};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Library);
INSPECTOR_P_SUPPORT(Hurricane::Library::LibraryMap);
INSPECTOR_P_SUPPORT(Hurricane::Library::CellMap);

#endif // HURRICANE_LIBRARY

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
