// ****************************************************************************************************
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: Pin.cpp
// Authors: Christophe Alexandre.
// ****************************************************************************************************

#include "Cell.h"
#include "Pin.h"

namespace Hurricane {



// ****************************************************************************************************
// Pin implementation
// ****************************************************************************************************

Pin::Pin(Net* net, const Name& name, const AccessDirection& accessDirection, const PlacementStatus& placementStatus, Layer* layer, const Unit& x, const Unit& y, const Unit& width, const Unit& height)
// ****************************************************************************************************
:  Inherit(net, layer, x, y, width, height),
    _name(name),
    _accessDirection(accessDirection),
    _placementStatus(placementStatus),
    _nextOfCellPinMap(NULL)
{
    if (getCell()->getPin(name))
        throw Error("Can't create " + _TName("Pin") + " : already exists");

}

Pin* Pin::create(Net* net, const Name& name, const AccessDirection& accessDirection, const PlacementStatus& placementStatus, Layer* layer, const Unit& x, const Unit& y, const Unit& width, const Unit& height)
// ****************************************************************************************************
{
    if (!net)
        throw Error("Can't create " + _TName("Pin") + " : NULL net");
    if (!layer)
        throw Error("Can't create " + _TName("Pin") + " : NULL layer");

    Pin* pin = new Pin(net, name, accessDirection, placementStatus, layer, x, y, width, height);

    pin->_postCreate();

    return pin;
}

void Pin::SetPlacementStatus(const PlacementStatus& placementstatus)
// **********************************************************************
{
    if (placementstatus != _placementStatus) {
        Invalidate(true);
        _placementStatus = placementstatus;
    }
}

void Pin::_postCreate()
// **********************
{
    getCell()->_getPinMap()._Insert(this);

    Inherit::_postCreate();
}

void Pin::_preDestroy()
// *********************
{
    Inherit::_preDestroy();

    getCell()->_getPinMap()._Remove(this);
}

string Pin::_getString() const
// *****************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_name));
    s.insert(s.length() - 1, " " + getString(_accessDirection));
    return s;
}

Record* Pin::_getRecord() const
// ************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->Add(getSlot("Name", &_name));
        record->Add(getSlot("AccessDirection", &_accessDirection));
        record->Add(getSlot("PlacementStatus", &_placementStatus));
    }
    return record;
}

//void Pin::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
//// *************************************************************************************************************
//{
//    view->FillRectangle(transformation.getBox(getBoundingBox(basicLayer)), true);
//}
//
//void Pin::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
//// ******************************************************************************************
//{
//    if (_width && _height) {
//        if (1 < view->getScreenSize(max(_width, _height))) {
//            for_each_basic_layer(basicLayer, getLayer()->getBasicLayers()) {
//                basicLayer->_Fill(view, transformation.getBox(getBoundingBox(basicLayer)));
//                end_for;
//            }
//        }
//    }
//    if (view->getScale() <= 1)
//        view->DrawPoint(transformation.getPoint(getPosition()), 1);
//    else if (view->getScale() <= 3)
//    {
//        view->DrawPoint(transformation.getPoint(getPosition()), 2);
//
//        if ( view->IsTextVisible() )
//        {
//            string text = "("
//                        + getString ( getValue ( getX() ) ) + ","
//                        + getString ( getValue ( getY() ) ) + ")";
//            view->DrawString ( text,
//                               transformation.getBox ( getBoundingBox() ).getXMin(),
//                               transformation.getBox ( getBoundingBox() ).getYMax() );
//        }
//    }
//    else {
//        Point position = getPosition();
//        view->DrawPoint(transformation.getPoint(position), 3);
//        if (_width) {
//            Box box = transformation.getBox(Box(position).Inflate(getHalfWidth(), 0));
//            view->DrawLine(box.getXMin(), box.getYMin(), box.getXMax(), box.getYMax());
//        }
//        if (_height) {
//            Box box = transformation.getBox(Box(position).Inflate(0, getHalfHeight()));
//            view->DrawLine(box.getXMin(), box.getYMin(), box.getXMax(), box.getYMax());
//        }
//
//        if ( view->IsTextVisible() )
//        {
//            string text = getString ( _name ) + "("
//                        + getString ( getValue ( getX() ) ) + ","
//                        + getString ( getValue ( getY() ) ) + ")";
//            view->DrawString ( text,
//                               transformation.getBox ( getBoundingBox() ).getXMin(),
//                               transformation.getBox ( getBoundingBox() ).getYMax() );
//        }
//    }
//}
//

// ****************************************************************************************************
// Pin::AccessDirection implementation
// ****************************************************************************************************

Pin::AccessDirection::AccessDirection(const Code& code)
// ******************************************************
:    _code(code)
{
}

Pin::AccessDirection::AccessDirection(const AccessDirection& accessDirection)
// ****************************************************************************
:    _code(accessDirection._code)
{
}

Pin::AccessDirection& Pin::AccessDirection::operator=(const AccessDirection& accessDirection)
// **********************************************************************************************
{
    _code = accessDirection._code;
    return *this;
}

string Pin::AccessDirection::_getString() const
// **********************************************
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
// *****************************************
{
    Record* record = new Record(getString(this));
    record->Add(getSlot("Code", (int)_code));
    return record;
}



// ****************************************************************************************************
// Pin::PlacementStatus implementation
// ****************************************************************************************************

Pin::PlacementStatus::PlacementStatus(const Code& code)
// ****************************************************
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
// ***************************************
{
    Record* record = new Record(getString(this));
    record->Add(getSlot("Code", (int)_code));
    return record;
}



} // End of Hurricane namespace.

