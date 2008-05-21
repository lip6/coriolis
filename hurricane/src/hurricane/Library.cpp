// ****************************************************************************************************
// File: Library.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

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
            throw Error("Can't create " + _TName("Library") + " : already exists");
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

    for_each_cell(cell, getCells()) cell->destroy(); end_for;
    for_each_library(library, getLibraries()) library->destroy(); end_for;

    if (!_library)
        _dataBase->_setRootLibrary(NULL);
    else
        _library->_getLibraryMap()._remove(this);
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
    return ( (unsigned int)( (unsigned long)name._getSharedName() ) ) / 8;
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
    return ( (unsigned int)( (unsigned long)name._getSharedName() ) ) / 8;
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


} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
