// ****************************************************************************************************
// File: Horizontal.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Horizontal.h"
#include "Net.h"
#include "BasicLayer.h"
#include "CompositeLayer.h"
#include "Plug.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Horizontal implementation
// ****************************************************************************************************

Horizontal::Horizontal(Net* net, Component* source, Component* target, Layer* layer, const Unit& y, const Unit& width, const Unit& dxSource, const Unit& dxTarget)
// ****************************************************************************************************
:  Inherit(net, source, target, layer, width),
	_y(y),
	_dxSource(dxSource),
	_dxTarget(dxTarget)
{
}

Horizontal* Horizontal::Create(Net* net, Layer* layer, const Unit& y, const Unit& width, const Unit& dxSource, const Unit& dxTarget)
// ****************************************************************************************************
{
	if (!net)
		throw Error("Can't create " + _TName("Horizontal") + " : null net");

	Horizontal* horizontal = new Horizontal(net, NULL, NULL, layer, y, width, dxSource, dxTarget);

	horizontal->_PostCreate();

	return horizontal;
}

Horizontal* Horizontal::Create(Component* source, Component* target, Layer* layer, const Unit& y, const Unit& width, const Unit& dxSource, const Unit& dxTarget)
// ****************************************************************************************************
{
	if (!source)
		throw Error("Can't create " + _TName("Horizontal") + " : null source");

	if (!target)
		throw Error("Can't create " + _TName("Horizontal") + " : null target");

	Horizontal* horizontal =
		new Horizontal(source->GetNet(), source, target, layer, y, width, dxSource, dxTarget);

	horizontal->_PostCreate();

	return horizontal;
}

Box Horizontal::GetBoundingBox() const
// ***********************************
{
	Unit size = GetHalfWidth() + _GetSize();
	Unit extention = _GetExtention();

	return Box(GetSourceX(), _y, GetTargetX(), _y).inflate(extention, size);
}

Box Horizontal::GetBoundingBox(const BasicLayer* basicLayer) const
// *********************************************************
{
	if (!GetLayer()->Contains(basicLayer)) return Box();

	Unit size = GetHalfWidth() + _GetSize(basicLayer);
	Unit extention = _GetExtention(basicLayer);

	return Box(GetSourceX(), _y, GetTargetX(), _y).inflate(extention, size);
}

Unit Horizontal::GetSourceX() const
// ********************************
{
	Component* source = GetSource();
	return (source) ? source->GetX() + _dxSource : _dxSource;
}

Unit Horizontal::GetTargetX() const
// ********************************
{
	Component* target = GetTarget();
	return (target) ? target->GetX() + _dxTarget : _dxTarget;
}

Unit Horizontal::GetLength() const
// *******************************
{
	return abs(GetSourceX() - GetTargetX());
}

Point Horizontal::GetCenter() const
// *******************************
{
	return Point ( (GetSourceX()+GetTargetX())/2, GetY() );
}

void Horizontal::Translate(const Unit& dx, const Unit& dy)
// *******************************************************
{
	if (dy != 0) {
		Invalidate(true);
		_y += dy;
	}
}

void Horizontal::SetY(const Unit& y)
// *********************************
{
	if (y != _y) {
		Invalidate(true);
		_y = y;
	}
}

void Horizontal::SetDxSource(const Unit& dxSource)
// ***********************************************
{
	if (dxSource != _dxSource) {
		Invalidate(false);
		_dxSource = dxSource;
	}
}

void Horizontal::SetDxTarget(const Unit& dxTarget)
// ***********************************************
{
	if (dxTarget != _dxTarget) {
		Invalidate(false);
		_dxTarget = dxTarget;
	}
}

void Horizontal::Translate(const Unit& dy)
// ***************************************
{
	if (dy != 0) {
		Invalidate(true);
		_y += dy;
	}
}

string Horizontal::_GetString() const
// **********************************
{
	return Inherit::_GetString();
}

Record* Horizontal::_GetRecord() const
// *****************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Y", &_y));
		record->Add(GetSlot("DxSource", &_dxSource));
		record->Add(GetSlot("DxTarget", &_dxTarget));
	}
	return record;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
