// ****************************************************************************************************
// File: DataBase.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Library.h"
#include "hurricane/Cell.h"
#include "hurricane/Timer.h"
#include "hurricane/Error.h"
#include "hurricane/UpdateSession.h"

namespace Hurricane {



// ****************************************************************************************************
// DataBase implementation
// ****************************************************************************************************

DataBase* DataBase::_db = NULL;

DataBase::DataBase()
// *****************
:    Inherit(),
    _technology(NULL),
    _rootLibrary(NULL)
{
    if (_db)
        throw Error("Can't create " + _TName("DataBase") + " : already exists");
}

DataBase* DataBase::create()
// *************************
{
    DataBase* dataBase = new DataBase();

    dataBase->_postCreate();

    return dataBase;
}

void DataBase::_postCreate()
// *************************
{
    Inherit::_postCreate();

    _db = this;
}

void DataBase::_preDestroy()
// ************************
{
    UpdateSession::open();
    Inherit::_preDestroy();

    if (_rootLibrary) _rootLibrary->destroy();
    if (_technology) _technology->destroy();
    UpdateSession::close();

    _db = NULL;
}

string DataBase::_getString() const
// ********************************
{
    return Inherit::_getString();
}

Record* DataBase::_getRecord() const
// ***************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Technology", _technology));
        record->add(getSlot("RootLibrary", _rootLibrary));
        record->add(getSlot("Precision", DbU::getPrecision()));
        record->add(getSlot("Resolution", DbU::db(1)));
      //record->add(getSlot("GridStep", getValueString(getGridStep())));
    }
    return record;
}

DataBase* DataBase::getDB()
// ************************
{
    return _db;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
