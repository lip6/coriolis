// ****************************************************************************************************
// File: DRCError.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "DRCError.h"
#include "Cell.h"
#include "Slice.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// DRCError implementation
// ****************************************************************************************************

DRCError::DRCError(Cell* cell, const Name& name, const Box& boundingBox)
// *********************************************************************
:  Inherit(cell),
	_name(name),
	_boundingBox(boundingBox)
{
	if (_name.IsEmpty())
		throw Error("Can't create " + _TName("DRCError") + " : empty name");

	if (_boundingBox.IsEmpty())
		throw Error("Can't create " + _TName("DRCError") + " : empty bounding box");
}

DRCError* DRCError::Create(Cell* cell, const Name& name, const Box& boundingBox)
// *****************************************************************************
{
	DRCError* drcError = new DRCError(cell, name, boundingBox);

	drcError->_PostCreate();

	return drcError;
}

void DRCError::Translate(const Unit& dx, const Unit& dy)
// *****************************************************
{
	if ((dx != 0) || (dy != 0)) {
		Invalidate(false);
		_boundingBox.Translate(dx, dy);
	}
}

string DRCError::_GetString() const
// ********************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(_name));
	s.insert(s.length() - 1, " " + GetString(_boundingBox));
	return s;
}

Record* DRCError::_GetRecord() const
// ***************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Name", &_name));
		record->Add(GetSlot("BoundingBox", &_boundingBox));
	}
	return record;
}

//bool DRCError::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
//// ****************************************************************************************
//{
//	return GetBoundingBox().Intersect(Box(point).Inflate(aperture));
//}
//
//void DRCError::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
//// ****************************************************************************************************
//{
//	assert(!basicLayer);
//
//	view->DrawRectangle(transformation.GetBox(GetBoundingBox()));
//}
//
//void DRCError::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
//// **********************************************************************************************
//{
//	view->FillRectangle(transformation.GetBox(GetBoundingBox()));
//}
//


} // End of Hurricane namespace.
