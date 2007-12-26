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
	if (GetCell()->GetPin(name))
		throw Error("Can't create " + _TName("Pin") + " : already exists");

}

Pin* Pin::Create(Net* net, const Name& name, const AccessDirection& accessDirection, const PlacementStatus& placementStatus, Layer* layer, const Unit& x, const Unit& y, const Unit& width, const Unit& height)
// ****************************************************************************************************
{
	if (!net)
		throw Error("Can't create " + _TName("Pin") + " : NULL net");
	if (!layer)
		throw Error("Can't create " + _TName("Pin") + " : NULL layer");

	Pin* pin = new Pin(net, name, accessDirection, placementStatus, layer, x, y, width, height);

	pin->_PostCreate();

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

void Pin::_PostCreate()
// **********************
{
	GetCell()->_GetPinMap()._Insert(this);

	Inherit::_PostCreate();
}

void Pin::_PreDelete()
// *********************
{
	Inherit::_PreDelete();

	GetCell()->_GetPinMap()._Remove(this);
}

string Pin::_GetString() const
// *****************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(_name));
	s.insert(s.length() - 1, " " + GetString(_accessDirection));
	return s;
}

Record* Pin::_GetRecord() const
// ************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Name", &_name));
		record->Add(GetSlot("AccessDirection", &_accessDirection));
		record->Add(GetSlot("PlacementStatus", &_placementStatus));
	}
	return record;
}

//void Pin::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
//// *************************************************************************************************************
//{
//	view->FillRectangle(transformation.GetBox(GetBoundingBox(basicLayer)), true);
//}
//
//void Pin::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
//// ******************************************************************************************
//{
//	if (_width && _height) {
//		if (1 < view->GetScreenSize(max(_width, _height))) {
//			for_each_basic_layer(basicLayer, GetLayer()->GetBasicLayers()) {
//				basicLayer->_Fill(view, transformation.GetBox(GetBoundingBox(basicLayer)));
//				end_for;
//			}
//		}
//	}
//	if (view->GetScale() <= 1)
//		view->DrawPoint(transformation.GetPoint(GetPosition()), 1);
//	else if (view->GetScale() <= 3)
//    {
//		view->DrawPoint(transformation.GetPoint(GetPosition()), 2);
//
//        if ( view->IsTextVisible() )
//        {
//            string text = "("
//                        + GetString ( GetValue ( GetX() ) ) + ","
//                        + GetString ( GetValue ( GetY() ) ) + ")";
//            view->DrawString ( text,
//                               transformation.GetBox ( GetBoundingBox() ).GetXMin(),
//                               transformation.GetBox ( GetBoundingBox() ).GetYMax() );
//        }
//    }
//	else {
//		Point position = GetPosition();
//		view->DrawPoint(transformation.GetPoint(position), 3);
//		if (_width) {
//			Box box = transformation.GetBox(Box(position).Inflate(GetHalfWidth(), 0));
//			view->DrawLine(box.GetXMin(), box.GetYMin(), box.GetXMax(), box.GetYMax());
//		}
//		if (_height) {
//			Box box = transformation.GetBox(Box(position).Inflate(0, GetHalfHeight()));
//			view->DrawLine(box.GetXMin(), box.GetYMin(), box.GetXMax(), box.GetYMax());
//		}
//
//        if ( view->IsTextVisible() )
//        {
//            string text = GetString ( _name ) + "("
//                        + GetString ( GetValue ( GetX() ) ) + ","
//                        + GetString ( GetValue ( GetY() ) ) + ")";
//            view->DrawString ( text,
//                               transformation.GetBox ( GetBoundingBox() ).GetXMin(),
//                               transformation.GetBox ( GetBoundingBox() ).GetYMax() );
//        }
//	}
//}
//

// ****************************************************************************************************
// Pin::AccessDirection implementation
// ****************************************************************************************************

Pin::AccessDirection::AccessDirection(const Code& code)
// ******************************************************
:	_code(code)
{
}

Pin::AccessDirection::AccessDirection(const AccessDirection& accessDirection)
// ****************************************************************************
:	_code(accessDirection._code)
{
}

Pin::AccessDirection& Pin::AccessDirection::operator=(const AccessDirection& accessDirection)
// **********************************************************************************************
{
	_code = accessDirection._code;
	return *this;
}

string Pin::AccessDirection::_GetString() const
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

Record* Pin::AccessDirection::_GetRecord() const
// *****************************************
{
	Record* record = new Record(GetString(this));
	record->Add(GetSlot("Code", (int)_code));
	return record;
}



// ****************************************************************************************************
// Pin::PlacementStatus implementation
// ****************************************************************************************************

Pin::PlacementStatus::PlacementStatus(const Code& code)
// ****************************************************
:	_code(code)
{
}

Pin::PlacementStatus::PlacementStatus(const PlacementStatus& placementstatus)
// **************************************************************************
:	_code(placementstatus._code)
{
}

Pin::PlacementStatus& Pin::PlacementStatus::operator=(const PlacementStatus& placementstatus)
// ******************************************************************************************
{
    _code = placementstatus._code;
	return *this;
}

string Pin::PlacementStatus::_GetString() const
// ********************************************
{
	switch (_code) {
		case UNPLACED : return "UNPLACED";
		case PLACED : return "PLACED";
		case FIXED : return "FIXED";
	}
	return "ABNORMAL";
}

Record* Pin::PlacementStatus::_GetRecord() const
// ***************************************
{
	Record* record = new Record(GetString(this));
	record->Add(GetSlot("Code", (int)_code));
	return record;
}



} // End of Hurricane namespace.

