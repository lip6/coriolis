// ****************************************************************************************************
// File: ./Library.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
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

#include "hurricane/SharedName.h"
#include "hurricane/Library.h"
#include "hurricane/DataBase.h"
#include "hurricane/Cell.h"
#include "hurricane/Technology.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Library implementation
// ****************************************************************************************************

Library::Library(DataBase* dataBase, Library* library, const Name& name)
// *********************************************************************
:    Inherit(),
    _dataBase(dataBase),
    _library(library),
    _name(name),
    _libraryMap(),
    _cellMap(),
    _nextOfLibraryLibraryMap(NULL)
{
    if (!_dataBase)
        throw Error("Can't create " + _TName("Library") + " : null data base");

    if (name.isEmpty())
        throw Error("Can't create " + _TName("Library") + " : empty name");

    if (!_library) {
        if (_dataBase->getRootLibrary())
            throw Error("Can't create " + _TName("Library") + " : root library already exists");
    }
    else {
        if (_library->getLibrary(_name))
          throw Error("Can't create Library " + getString(name) + " : already exists");
    }
}

Library* Library::create(DataBase* dataBase, const Name& name)
// ***********************************************************
{
    Library* library = new Library(dataBase, NULL, name);

    library->_postCreate();

    return library;
}

Library* Library::create(Library* library, const Name& name)
// *********************************************************
{
    if (!library)
        throw Error("Can't create " + _TName("Library") + " : null library");

    library = new Library(library->getDataBase(), library, name);

    library->_postCreate();

    return library;
}

void Library::setName(const Name& name)
// ************************************
{
    if (name != _name) {
        if (name.isEmpty())
            throw Error("Can't change library name : empty name");

        if (_library && _library->getLibrary(name))
            throw Error("Can't change library name : already exists");

        if (_library) _library->_getLibraryMap()._remove(this);
        _name = name;
        if (_library) _library->_getLibraryMap()._insert(this);
    }
}

string Library::getHierarchicalName () const
// *****************************************
{
  string   rpath;
  Library* library = getLibrary();
  do {
    rpath.insert( 0, getString(library->getName())+"." );

    library = library->getLibrary();
  } while ( library );

  return rpath + getString(getName());
}
void Library::_postCreate()
// ************************
{
    if (!_library)
        _dataBase->_setRootLibrary(this);
    else
        _library->_getLibraryMap()._insert(this);

    Inherit::_postCreate();
}

void Library::_preDestroy()
// ***********************
{
    Inherit::_preDestroy();

    Cells     cells     = getCells    (); while (cells    .getFirst()) cells    .getFirst()->destroy();
    Libraries libraries = getLibraries(); while (libraries.getFirst()) libraries.getFirst()->destroy();

    if (not _library) _dataBase->_setRootLibrary( NULL );
    else _library->_getLibraryMap()._remove( this );
}

string Library::_getString() const
// *******************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " ");
    s.insert(s.length() - 1, getString(_name));
    return s;
}

Record* Library::_getRecord() const
// **************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("DataBase", _dataBase));
        record->add(getSlot("Library", _library));
        record->add(getSlot("Name", &_name));
        record->add(getSlot("Libraries", &_libraryMap));
        record->add(getSlot("Cells", &_cellMap));
    }
    return record;
}

// ****************************************************************************************************
// Library::LibraryMap implementation
// ****************************************************************************************************

Library::LibraryMap::LibraryMap()
// ******************************
:    Inherit()
{
}

Name Library::LibraryMap::_getKey(Library* library) const
// ******************************************************
{
    return library->getName();
}

unsigned Library::LibraryMap::_getHashValue(Name name) const
// *********************************************************
{
  return name._getSharedName()->getId() / 8;
}

Library* Library::LibraryMap::_getNextElement(Library* library) const
// ******************************************************************
{
    return library->_getNextOfLibraryLibraryMap();
}

void Library::LibraryMap::_setNextElement(Library* library, Library* nextLibrary) const
// ************************************************************************************
{
    library->_setNextOfLibraryLibraryMap(nextLibrary);
};



// ****************************************************************************************************
// Library::CellMap implementation
// ****************************************************************************************************

Library::CellMap::CellMap()
// ************************
:    Inherit()
{
}

Name Library::CellMap::_getKey(Cell* cell) const
// *********************************************
{
    return cell->getName();
}

unsigned Library::CellMap::_getHashValue(Name name) const
// ******************************************************
{
  return name._getSharedName()->getId() / 8;
}

Cell* Library::CellMap::_getNextElement(Cell* cell) const
// ******************************************************
{
    return cell->_getNextOfLibraryCellMap();
}

void Library::CellMap::_setNextElement(Cell* cell, Cell* nextCell) const
// *********************************************************************
{
    cell->_setNextOfLibraryCellMap(nextCell);
};

void Library::_toJson(JsonWriter* w) const
// ***************************************
{
  Inherit::_toJson( w );

  jsonWrite( w, "_name"      , getName()      );
//jsonWrite( w, "+cellMap"   , getCells()     );
  jsonWrite( w, "+libraryMap", getLibraries() );
}


// ****************************************************************************************************
// JsonLibrary implementation
// ****************************************************************************************************

Initializer<JsonLibrary>  jsonLibraryInit ( 0 );

void  JsonLibrary::initialize()
// **************************
{ JsonTypes::registerType( new JsonLibrary (JsonWriter::RegisterMode) ); }

JsonLibrary::JsonLibrary(unsigned long flags)
// ************************************
  : JsonDBo(flags)
{
//add( ".Library"   , typeid(Library*)  );
  add( "_name"      , typeid(string)    );
//add( "+cellMap"   , typeid(JsonArray) );
  add( "+libraryMap", typeid(JsonArray) );
}

string JsonLibrary::getTypeName() const
// *********************************
{ return "Library"; }

JsonLibrary* JsonLibrary::clone(unsigned long flags) const
// *************************************************
{ return new JsonLibrary ( flags ); }

void JsonLibrary::toData(JsonStack& stack)
// ***************************************
{
  check( stack, "JsonLibrary::toData" );

  Name     libName ( get<string>  ( stack, "_name"    ) );
  Library* library = NULL;
  Library* parent  = NULL;

  if (stack.rhas(".Library"))
    parent = get<Library*>( stack, ".Library" );
  else if (stack.rhas("_rootLibrary"))
    parent = get<Library*>( stack, "_rootLibrary" );

  if (parent) {
    library = parent->getLibrary( libName );
    if (not library)
      library = Library::create( parent, libName );
  } else {
    library = DataBase::getDB()->getRootLibrary();
    if (not library)
      library = Library::create( DataBase::getDB(), libName );
    else {
      if (library->getName() != libName) {
        throw Error( "JsonLibrary::toData(): Root library name discrepency, \"%s\" vs. \"%s\"."
                   , getString(library->getName()).c_str()
                   , getString(libName).c_str()
                   );
      }
    }
  }

  update( stack, library );
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
