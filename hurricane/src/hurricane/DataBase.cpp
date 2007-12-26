// ****************************************************************************************************
// File: DataBase.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "DataBase.h"
#include "Technology.h"
#include "Library.h"
#include "Cell.h"
#include "Timer.h"
#include "Error.h"
#include "UpdateSession.h"

namespace Hurricane {



// ****************************************************************************************************
// DataBase implementation
// ****************************************************************************************************

static DataBase* DATA_BASE = NULL;

DataBase::DataBase()
// *****************
:	Inherit(),
	_technology(NULL),
	_rootLibrary(NULL)
{
	if (DATA_BASE)
		throw Error("Can't create " + _TName("DataBase") + " : already exists");
}

DataBase* DataBase::Create()
// *************************
{
	DataBase* dataBase = new DataBase();

	dataBase->_PostCreate();

	return dataBase;
}

void DataBase::_PostCreate()
// *************************
{
	Inherit::_PostCreate();

	DATA_BASE = this;
}

void DataBase::_PreDelete()
// ************************
{
	OpenUpdateSession();
	Inherit::_PreDelete();

	if (_rootLibrary) _rootLibrary->Delete();
	if (_technology) _technology->Delete();
	CloseUpdateSession ();

	DATA_BASE = NULL;
}

string DataBase::_GetString() const
// ********************************
{
	return Inherit::_GetString();
}

Record* DataBase::_GetRecord() const
// ***************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Technology", _technology));
		record->Add(GetSlot("RootLibrary", _rootLibrary));
		record->Add(GetSlot("Precision", GetPrecision()));
		record->Add(GetSlot("Resolution", GetValueString(1)));
		record->Add(GetSlot("GridStep", GetValueString(GetGridStep())));
	}
	return record;
}

// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

DataBase* GetDataBase()
// ********************
{
	return DATA_BASE;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
