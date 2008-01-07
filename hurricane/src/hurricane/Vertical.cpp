// ****************************************************************************************************
// File: Vertical.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Vertical.h"
#include "Net.h"
#include "BasicLayer.h"
#include "CompositeLayer.h"
#include "Plug.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Vertical implementation
// ****************************************************************************************************

Vertical::Vertical(Net* net, Component* source, Component* target, Layer* layer, const Unit& x, const Unit& width, const Unit& dySource, const Unit& dyTarget)
// ****************************************************************************************************
:  Inherit(net, source, target, layer, width),
	_x(x),
	_dySource(dySource),
	_dyTarget(dyTarget)
{
}

Vertical* Vertical::Create(Net* net, Layer* layer, const Unit& x, const Unit& width, const Unit& dySource, const Unit& dyTarget)
// ****************************************************************************************************
{
	if (!net)
		throw Error("Can't create " + _TName("Vertical") + " : null net");

	Vertical* vertical = new Vertical(net, NULL, NULL, layer, x, width, dySource, dyTarget);

	vertical->_PostCreate();

	return vertical;
}

Vertical* Vertical::Create(Component* source, Component* target, Layer* layer, const Unit& x, const Unit& width, const Unit& dySource, const Unit& dyTarget)
// ****************************************************************************************************
{
	if (!source)
		throw Error("Can't create " + _TName("Vertical") + " : null source");

	if (!target)
		throw Error("Can't create " + _TName("Vertical") + " : null target");

	Vertical* vertical =
		new Vertical(source->GetNet(), source, target, layer, x, width, dySource, dyTarget);

	vertical->_PostCreate();

	return vertical;
}

Box Vertical::GetBoundingBox() const
// *********************************
{
	Unit size = GetHalfWidth() + _GetSize();
	Unit extention = _GetExtention();

	return Box(_x, GetSourceY(), _x, GetTargetY()).inflate(size, extention);
}

Box Vertical::GetBoundingBox(const BasicLayer* basicLayer) const
// *******************************************************
{
	if (!GetLayer()->Contains(basicLayer)) return Box();

	Unit size = GetHalfWidth() + _GetSize(basicLayer);
	Unit extention = _GetExtention(basicLayer);

	return Box(_x, GetSourceY(), _x, GetTargetY()).inflate(size, extention);
}

Unit Vertical::GetSourceY() const
// ******************************
{
	Component* source = GetSource();
	return (source) ? source->GetY() + _dySource : _dySource;
}

Unit Vertical::GetTargetY() const
// ******************************
{
	Component* target = GetTarget();
	return (target) ? target->GetY() + _dyTarget : _dyTarget;
}

Unit Vertical::GetLength() const
// *****************************
{
	return abs(GetSourceY() - GetTargetY());
}

Point Vertical::GetCenter() const
// *******************************
{
        return Point ( GetX(), (GetSourceY()+GetTargetY())/2 );
}

void Vertical::Translate(const Unit& dx, const Unit& dy)
// *****************************************************
{
	if (dx != 0) {
		Invalidate(true);
		_x += dx;
	}
}

void Vertical::SetX(const Unit& x)
// *******************************
{
	if (x != _x) {
		Invalidate(true);
		_x = x;
	}
}

void Vertical::SetDySource(const Unit& dySource)
// *********************************************
{
	if (dySource != _dySource) {
		Invalidate(false);
		_dySource = dySource;
	}
}

void Vertical::SetDyTarget(const Unit& dyTarget)
// *********************************************
{
	if (dyTarget != _dyTarget) {
		Invalidate(false);
		_dyTarget = dyTarget;
	}
}

void Vertical::Translate(const Unit& dx)
// *************************************
{
	if (dx != 0) {
		Invalidate(true);
		_x += dx;
	}
}

string Vertical::_GetString() const
// ********************************
{
	return Inherit::_GetString();
}

Record* Vertical::_GetRecord() const
// ***************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("X", &_x));
		record->Add(GetSlot("DySource", &_dySource));
		record->Add(GetSlot("DyTarget", &_dyTarget));
	}
	return record;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
