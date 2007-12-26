// ****************************************************************************************************
// File: Library.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Library.h"
#include "DataBase.h"
#include "Cell.h"
#include "Technology.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Library implementation
// ****************************************************************************************************

Library::Library(DataBase* dataBase, Library* library, const Name& name)
// *********************************************************************
:	Inherit(),
	_dataBase(dataBase),
	_library(library),
	_name(name),
	_libraryMap(),
	_cellMap(),
	_nextOfLibraryLibraryMap(NULL)
{
	if (!_dataBase)
		throw Error("Can't create " + _TName("Library") + " : null data base");

	if (name.IsEmpty())
		throw Error("Can't create " + _TName("Library") + " : empty name");

	if (!_library) {
		if (_dataBase->GetRootLibrary())
			throw Error("Can't create " + _TName("Library") + " : root library already exists");
	}
	else {
		if (_library->GetLibrary(_name))
			throw Error("Can't create " + _TName("Library") + " : already exists");
	}
}

Library* Library::Create(DataBase* dataBase, const Name& name)
// ***********************************************************
{
	Library* library = new Library(dataBase, NULL, name);

	library->_PostCreate();

	return library;
}

Library* Library::Create(Library* library, const Name& name)
// *********************************************************
{
	if (!library)
		throw Error("Can't create " + _TName("Library") + " : null library");

	library = new Library(library->GetDataBase(), library, name);

	library->_PostCreate();

	return library;
}

void Library::SetName(const Name& name)
// ************************************
{
	if (name != _name) {
		if (name.IsEmpty())
			throw Error("Can't change library name : empty name");

		if (_library && _library->GetLibrary(name))
			throw Error("Can't change library name : already exists");

		if (_library) _library->_GetLibraryMap()._Remove(this);
		_name = name;
		if (_library) _library->_GetLibraryMap()._Insert(this);
	}
}

void Library::_PostCreate()
// ************************
{
	if (!_library)
		_dataBase->_SetRootLibrary(this);
	else
		_library->_GetLibraryMap()._Insert(this);

	Inherit::_PostCreate();
}

void Library::_PreDelete()
// ***********************
{
	Inherit::_PreDelete();

	for_each_cell(cell, GetCells()) cell->Delete(); end_for;
	for_each_library(library, GetLibraries()) library->Delete(); end_for;

	if (!_library)
		_dataBase->_SetRootLibrary(NULL);
	else
		_library->_GetLibraryMap()._Remove(this);
}

string Library::_GetString() const
// *******************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " ");
	s.insert(s.length() - 1, GetString(_name));
	return s;
}

Record* Library::_GetRecord() const
// **************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("DataBase", _dataBase));
		record->Add(GetSlot("Library", _library));
		record->Add(GetSlot("Name", &_name));
		record->Add(GetSlot("Libraries", &_libraryMap));
		record->Add(GetSlot("Cells", &_cellMap));
	}
	return record;
}

// ****************************************************************************************************
// Library::LibraryMap implementation
// ****************************************************************************************************

Library::LibraryMap::LibraryMap()
// ******************************
:	Inherit()
{
}

Name Library::LibraryMap::_GetKey(Library* library) const
// ******************************************************
{
	return library->GetName();
}

unsigned Library::LibraryMap::_GetHashValue(Name name) const
// *********************************************************
{
	return ( (unsigned int)( (unsigned long)name._GetSharedName() ) ) / 8;
}

Library* Library::LibraryMap::_GetNextElement(Library* library) const
// ******************************************************************
{
	return library->_GetNextOfLibraryLibraryMap();
}

void Library::LibraryMap::_SetNextElement(Library* library, Library* nextLibrary) const
// ************************************************************************************
{
	library->_SetNextOfLibraryLibraryMap(nextLibrary);
};



// ****************************************************************************************************
// Library::CellMap implementation
// ****************************************************************************************************

Library::CellMap::CellMap()
// ************************
:	Inherit()
{
}

Name Library::CellMap::_GetKey(Cell* cell) const
// *********************************************
{
	return cell->GetName();
}

unsigned Library::CellMap::_GetHashValue(Name name) const
// ******************************************************
{
	return ( (unsigned int)( (unsigned long)name._GetSharedName() ) ) / 8;
}

Cell* Library::CellMap::_GetNextElement(Cell* cell) const
// ******************************************************
{
	return cell->_GetNextOfLibraryCellMap();
}

void Library::CellMap::_SetNextElement(Cell* cell, Cell* nextCell) const
// *********************************************************************
{
	cell->_SetNextOfLibraryCellMap(nextCell);
};


} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
