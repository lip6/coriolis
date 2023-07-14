// ****************************************************************************************************
// File: ./Pin.cpp
// Authors: Christophe Alexandre.
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

#include "hurricane/Warning.h"
#include "hurricane/Cell.h"
#include "hurricane/Pin.h"

namespace Hurricane {



// ****************************************************************************************************
// Pin implementation
// ****************************************************************************************************

Pin::Pin(Net* net, const Name& name, const AccessDirection& accessDirection, const PlacementStatus& placementStatus, const Layer* layer, DbU::Unit x, DbU::Unit y, DbU::Unit width, DbU::Unit height)
// **************************************************************************************************************************************************************************************************
:  Inherit(net, layer, x, y, width, height),
    _name(name),
    _accessDirection(accessDirection),
    _placementStatus(placementStatus),
    _nextOfCellPinMap(NULL)
{
    if (getCell()->getPin(name))
      throw Error("Can't create " + _TName("Pin") + " <" + getString(name) + "> : already exists");

}

Pin* Pin::create(Net* net, const Name& name, const AccessDirection& accessDirection, const PlacementStatus& placementStatus, const Layer* layer, DbU::Unit x, DbU::Unit y, DbU::Unit width, DbU::Unit height)
// **********************************************************************************************************************************************************************************************************
{
    if (!net)
        throw Error("Can't create " + _TName("Pin") + " : NULL net");
    if (!layer)
        throw Error("Can't create " + _TName("Pin") + " : NULL layer");

    Pin* pin = new Pin(net, name, accessDirection, placementStatus, layer, x, y, width, height);
    pin->_postCreate();
    pin->_postCheck();
    return pin;
}

  bool  Pin::_postCheck ()
  // *********************
  {
    bool rvalue = true;
    if ((not getLayer()->isSymbolic() or (getWidth()))
       and (  (_accessDirection == AccessDirection::NORTH)
           or (_accessDirection == AccessDirection::SOUTH) )
       and (getWidth() < getLayer()->getMinimalSize())) {
      cerr << Warning( "Pin::_postCheck(): Width %s is inferior to layer minimal size %s, bumping.\n"
                       "          (on %s)"
                     , DbU::getValueString(getWidth()).c_str()
                     , DbU::getValueString(getLayer()->getMinimalSize()).c_str()
                     , getString(this).c_str() )
           << endl;
      setWidth( getLayer()->getMinimalSize() );
      rvalue = false;
    }
    if ((not getLayer()->isSymbolic() or (getHeight()))
       and (  (_accessDirection == AccessDirection::WEST)
           or (_accessDirection == AccessDirection::EAST) )
       and getHeight() < getLayer()->getMinimalSize()) {
      cerr << Warning( "Pin::_postCheck(): Height %s is inferior to layer minimal size %s, bumping.\n"
                       "          (on %s)"
                     , DbU::getValueString(getHeight()).c_str()
                     , DbU::getValueString(getLayer()->getMinimalSize()).c_str()
                     , getString(this).c_str() )
           << endl;
      setHeight( getLayer()->getMinimalSize() );
      rvalue = false;
    }
    return rvalue;
  }

void Pin::setPlacementStatus(const PlacementStatus& placementstatus)
// *****************************************************************
{
    if (placementstatus != _placementStatus) {
        invalidate(true);
        _placementStatus = placementstatus;
    }
}

void Pin::_postCreate()
// ********************
{
    getCell()->_getPinMap()._insert(this);

    Inherit::_postCreate();
}

void Pin::_preDestroy()
// ********************
{
  Inherit::_preDestroy();
  getCell()->_getPinMap()._remove(this);
}

string Pin::_getString() const
// ***************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_name));
    s.insert(s.length() - 1, " " + getString(_accessDirection));
    return s;
}

Record* Pin::_getRecord() const
// ****************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Name", &_name));
        record->add(getSlot("AccessDirection", &_accessDirection));
        record->add(getSlot("PlacementStatus", &_placementStatus));
    }
    return record;
}

// ****************************************************************************************************
// Pin::AccessDirection implementation
// ****************************************************************************************************

Pin::AccessDirection::AccessDirection(Code code)
// *********************************************
:    _code(code)
{
}

Pin::AccessDirection::AccessDirection(const AccessDirection& accessDirection)
// **************************************************************************
:    _code(accessDirection._code)
{
}

Pin::AccessDirection& Pin::AccessDirection::operator=(const AccessDirection& accessDirection)
// ******************************************************************************************
{
    _code = accessDirection._code;
    return *this;
}

string Pin::AccessDirection::_getString() const
// ********************************************
{
    switch (_code) {
        case UNDEFINED : return "UNDEFINED";
        case NORTH : return "NORTH";
        case SOUTH : return "SOUTH";
        case WEST : return "WEST";
        case EAST : return "EAST";
    }
    return "ABNORMAL";
}

Record* Pin::AccessDirection::_getRecord() const
// *********************************************
{
    Record* record = new Record(getString(this));
    record->add(getSlot("Code", (int)_code));
    return record;
}



// ****************************************************************************************************
// Pin::PlacementStatus implementation
// ****************************************************************************************************

Pin::PlacementStatus::PlacementStatus(Code code)
// *********************************************
:    _code(code)
{
}

Pin::PlacementStatus::PlacementStatus(const PlacementStatus& placementstatus)
// **************************************************************************
:    _code(placementstatus._code)
{
}

Pin::PlacementStatus& Pin::PlacementStatus::operator=(const PlacementStatus& placementstatus)
// ******************************************************************************************
{
    _code = placementstatus._code;
    return *this;
}

string Pin::PlacementStatus::_getString() const
// ********************************************
{
    switch (_code) {
        case UNPLACED : return "UNPLACED";
        case PLACED : return "PLACED";
        case FIXED : return "FIXED";
    }
    return "ABNORMAL";
}

Record* Pin::PlacementStatus::_getRecord() const
// *********************************************
{
    Record* record = new Record(getString(this));
    record->add(getSlot("Code", (int)_code));
    return record;
}



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
