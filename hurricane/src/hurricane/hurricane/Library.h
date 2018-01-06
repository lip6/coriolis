// ****************************************************************************************************
// File: ./hurricane/Library.h
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
    public: string getHierarchicalName () const;
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
    public: virtual void _toJson( JsonWriter* ) const;
    public: LibraryMap& _getLibraryMap() {return _libraryMap;};
    public: CellMap& _getCellMap() {return _cellMap;};
    public: Library* _getNextOfLibraryLibraryMap() const {return _nextOfLibraryLibraryMap;};

    public: void _setNextOfLibraryLibraryMap(Library* library) {_nextOfLibraryLibraryMap = library;};

};


class JsonLibrary : public JsonDBo {
// *******************************

  public: static void initialize();
  public: JsonLibrary(unsigned long flags);
  public: virtual string getTypeName() const;
  public: virtual JsonLibrary* clone(unsigned long) const;
  public: virtual void toData(JsonStack&); 
};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Library);
INSPECTOR_P_SUPPORT(Hurricane::Library::LibraryMap);
INSPECTOR_P_SUPPORT(Hurricane::Library::CellMap);

#endif // HURRICANE_LIBRARY


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
