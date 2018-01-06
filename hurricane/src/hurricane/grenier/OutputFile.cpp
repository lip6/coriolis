// ****************************************************************************************************
// File: OutputFile.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************

#include "OutputFile.h"
#include "Layer.h"
#include "Library.h"
#include "Cell.h"
#include "Instance.h"
#include "Component.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// OutputFile implementation
// ****************************************************************************************************

OutputFile::OutputFile(const string& filePathName)
// ***********************************************
:	_filePathName(filePathName),
	_file(filePathName.c_str()),
	_tab("\t"),
	_dboIdMap(),
	_componentIdMap(),
	_cellSet(),
	_cellVector()
{
	if (_filePathName.empty())
		throw Error("Can't create OutputFile : empty file pathname");

	if (!_file.good())
		throw Error("Can't create OutputFile : bad file");
}

unsigned OutputFile::GetId(DBo* dbo)
// *********************************
{
	DBoIdMap::iterator it = _dboIdMap.find(dbo);
	return (it != _dboIdMap.end()) ? (*it).second : 0;
}

Cells OutputFile::GetCells()
// *************************
 	{return GetCollection(_cellVector);}

unsigned OutputFile::GetId(Component* component)
// *********************************************
{
	ComponentIdMap::iterator it = _componentIdMap.find(component);
	return (it != _componentIdMap.end()) ? (*it).second : 0;
}

void OutputFile::Push(Cell* cell)
// ******************************
{
	if (_cellSet.find(cell) == _cellSet.end()) {
		for_each_instance(instance, cell->GetInstances()) {
			Push(instance->GetMasterCell());
			end_for;
		}
		_cellSet.insert(cell);
		_cellVector.push_back(cell);
	}
}

void OutputFile::Register(DBo* dbo)
// ********************************
{
	if (dbo && !GetId(dbo)) {
		unsigned id = _dboIdMap.size() + 1;
		_dboIdMap[dbo] = id;
	}
}

void OutputFile::Register(Component* component)
// ********************************************
{
	if (component && !GetId(component)) {
		unsigned id = _componentIdMap.size() + 1;
		_componentIdMap[component] = id;
	}
}

void OutputFile::OpenHeader()
// **************************
{
	_file << _tab;
}

void OutputFile::CloseHeader()
// ***************************
{
	_file << endl;
}

void OutputFile::OpenContent()
// ***************************
{
	++_tab;
}

void OutputFile::CloseContent()
// ****************************
{
	--_tab;
}

void OutputFile::ClearComponentIdMap()
// ***********************************
{
	_componentIdMap.clear();
}

string OutputFile::_GetString() const
// **********************************
{
	return "<" + _TName("OutputFile") + " " + _filePathName + ">";
}

Record* OutputFile::_GetRecord() const
// *****************************
{
	Record* record = new Record(GetString(this));
	if (record) {
		record->Add(GetSlot("FilePathName", &_filePathName));
		record->Add(GetSlot("DBoIds", &_dboIdMap));
		record->Add(GetSlot("ComponentIds", &_componentIdMap));
		record->Add(GetSlot("Cells", &_cellVector));
	}
	return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
