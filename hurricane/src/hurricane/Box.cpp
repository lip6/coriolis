// ****************************************************************************************************
// File: Box.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 added ManhattanDistance & ShrinkByFactor

#include "Error.h"
#include "Box.h"

namespace Hurricane {



// ****************************************************************************************************
// Box implementation
// ****************************************************************************************************

Box::Box()
// *******
:	_xMin(1),
	_yMin(1),
	_xMax(-1),
	_yMax(-1)
{
}

Box::Box(const Unit& x, const Unit& y)
// ***********************************
:	_xMin(x),
	_yMin(y),
	_xMax(x),
	_yMax(y)
{
}

Box::Box(const Point& point)
// *************************
:	_xMin(point.GetX()),
	_yMin(point.GetY()),
	_xMax(point.GetX()),
	_yMax(point.GetY())
{
}

Box::Box(const Unit& x1, const Unit& y1, const Unit& x2, const Unit& y2)
// *********************************************************************
:	_xMin(min(x1, x2)),
	_yMin(min(y1, y2)),
	_xMax(max(x1, x2)),
	_yMax(max(y1, y2))
{
}

Box::Box(const Point& point1, const Point& point2)
// ***********************************************
:	_xMin(min(point1.GetX(), point2.GetX())),
	_yMin(min(point1.GetY(), point2.GetY())),
	_xMax(max(point1.GetX(), point2.GetX())),
	_yMax(max(point1.GetY(), point2.GetY()))
{
}

Box::Box(const Box& box)
// *********************
:	_xMin(box._xMin),
	_yMin(box._yMin),
	_xMax(box._xMax),
	_yMax(box._yMax)
{
}

Box& Box::operator=(const Box& box)
// ********************************
{
	_xMin = box._xMin;
	_yMin = box._yMin;
	_xMax = box._xMax;
	_yMax = box._yMax;
	return *this;
}

bool Box::operator==(const Box& box) const
// ***************************************
{
	return (!IsEmpty() &&
			  !box.IsEmpty() &&
			  (_xMin == box._xMin) &&
			  (_yMin == box._yMin) &&
			  (_xMax == box._xMax) &&
			  (_yMax == box._yMax));
}

bool Box::operator!=(const Box& box) const
// ***************************************
{
	return (IsEmpty() ||
			  box.IsEmpty() ||
			  (_xMin != box._xMin) ||
			  (_yMin != box._yMin) ||
			  (_xMax != box._xMax) ||
			  (_yMax != box._yMax));
}

Box Box::GetUnion(const Box& box) const
// ************************************
{
	if (IsEmpty() && box.IsEmpty()) return Box();
	return Box(min(_xMin, box._xMin),
				  min(_yMin, box._yMin),
				  max(_xMax, box._xMax),
				  max(_yMax, box._yMax));
}

Box Box::GetIntersection(const Box& box) const
// *******************************************
{
	if (!Intersect(box)) return Box();
	return Box(max(_xMin, box._xMin),
				  max(_yMin, box._yMin),
				  min(_xMax, box._xMax),
				  min(_yMax, box._yMax));
}

Unit Box::ManhattanDistance(const Point& pt) const
// ***********************************************
{
	Unit dist = 0;
	if (IsEmpty())
		throw Error("Can't compute distance to an empty Box");
	if (pt.GetX() < _xMin) dist = _xMin - pt.GetX();
	else if (pt.GetX() > _xMax) dist = pt.GetX() - _xMax;
	// else                     dist = 0;
	if (pt.GetY() < _yMin) dist += _yMin - pt.GetY();
	else if (pt.GetY() > _yMax) dist += pt.GetY() - _yMax;
	// else                     dist += 0;
	return dist;
}

Unit Box::ManhattanDistance(const Box& box) const
// **********************************************
{
	if (IsEmpty() || box.IsEmpty())
		throw Error("Can't compute distance to an empty Box");
	Unit dx, dy;
	if ((dx=box.GetXMin() - _xMax) < 0)
		if ((dx=_xMin-box.GetXMax()) < 0) dx=0;
	if ((dy=box.GetYMin() - _yMax) < 0)
		if ((dy=_yMin-box.GetYMax()) < 0) dy=0;
	return dx+dy;
}

bool Box::IsEmpty() const
// **********************
{
	return ((_xMax < _xMin) || (_yMax < _yMin));
}

bool Box::IsFlat() const
// *********************
{
	return (!IsEmpty() &&
			  (((_xMin == _xMax) && (_yMin < _yMax)) ||
			   ((_xMin < _xMax) && (_yMin == _yMax))));
}

bool Box::IsPonctual() const
// *************************
{
	return (!IsEmpty() && (_xMax == _xMin) && (_yMax == _yMin));
}

bool Box::Contains(const Unit& x, const Unit& y) const
// ***************************************************
{
	return (!IsEmpty() &&
			  (_xMin <= x) &&
			  (_yMin <= y) &&
			  (x <= _xMax) &&
			  (y <= _yMax));
}

bool Box::Contains(const Point& point) const
// *****************************************
{
	return Contains(point.GetX(), point.GetY());
}

bool Box::Contains(const Box& box) const
// *************************************
{
	return (!IsEmpty() &&
			  !box.IsEmpty() &&
			  (_xMin <= box._xMin) &&
			  (box._xMax <= _xMax) &&
			  (_yMin <= box._yMin) &&
			  (box._yMax <= _yMax));
}

bool Box::Intersect(const Box& box) const
// **************************************
{
	return (!IsEmpty() &&
			  !box.IsEmpty() &&
			  !((_xMax < box._xMin) ||
				 (box._xMax < _xMin) ||
				 (_yMax < box._yMin) ||
				 (box._yMax < _yMin)));
}

bool Box::IsConstrainedBy(const Box& box) const
// ********************************************
{
	return (!IsEmpty() &&
			  !box.IsEmpty() &&
			  ((_xMin == box.GetXMin()) ||
				(_yMin == box.GetYMin()) ||
				(_xMax == box.GetXMax()) ||
				(_yMax == box.GetYMax())));
}

Box& Box::MakeEmpty()
// ******************
{
	_xMin = 1;
	_yMin = 1;
	_xMax = -1;
	_yMax = -1;
	return *this;
}

Box& Box::Inflate(const Unit& d)
// *****************************
{
	return Inflate(d, d, d, d);
}

Box& Box::Inflate(const Unit& dx, const Unit& dy)
// **********************************************
{
	return Inflate(dx, dy, dx, dy);
}

Box& Box::Inflate(const Unit& dxMin, const Unit& dyMin, const Unit& dxMax, const Unit& dyMax)
// ******************************************************************************************
{
	if (!IsEmpty()) {
		_xMin -= dxMin;
		_yMin -= dyMin;
		_xMax += dxMax;
		_yMax += dyMax;
	}
	return *this;
}

Box& Box::ShrinkByFactor(double factor)
// **************************************
{
	assert((0 <= factor) && (factor <= 1));
	Unit dx=GetUnit(0.5*(1- factor) * (GetValue(_xMax) - GetValue(_xMin)));
	Unit dy=GetUnit(0.5*(1- factor) * (GetValue(_yMax) - GetValue(_yMin)));
	return Inflate(-dx, -dy);
}

Box& Box::Merge(const Unit& x, const Unit& y)
// ******************************************
{
	if (IsEmpty()) {
		_xMin = x;
		_yMin = y;
		_xMax = x;
		_yMax = y;
	}
	else {
		_xMin = min(_xMin, x);
		_yMin = min(_yMin, y);
		_xMax = max(_xMax, x);
		_yMax = max(_yMax, y);
	}
	return *this;
}

Box& Box::Merge(const Point& point)
// ********************************
{
	return Merge(point.GetX(), point.GetY());
}

Box& Box::Merge(const Unit& x1, const Unit& y1, const Unit& x2, const Unit& y2)
// ****************************************************************************
{
	Merge(x1, y1);
	Merge(x2, y2);
	return *this;
}

Box& Box::Merge(const Box& box)
// ****************************
{
	if (!box.IsEmpty()) {
		Merge(box.GetXMin(), box.GetYMin());
		Merge(box.GetXMax(), box.GetYMax());
	}
	return *this;
}

Box& Box::Translate(const Unit& dx, const Unit& dy)
// ************************************************
{
	if (!IsEmpty()) {
		_xMin += dx;
		_yMin += dy;
		_xMax += dx;
		_yMax += dy;
	}
	return *this;
}

string Box::_GetString() const
// ***************************
{
	if (IsEmpty())
		return "<" + _TName("Box") + " empty>";
	else
		return "<" + _TName("Box") + " " +
				 GetValueString(_xMin) + " " + GetValueString(_yMin) + " " +
				 GetValueString(_xMax) + " " + GetValueString(_yMax) +
				 ">";
}

Record* Box::_GetRecord() const
// **********************
{
	if (IsEmpty()) return NULL;

	Record* record = new Record(GetString(this));
	record->Add(GetSlot("XMin", &_xMin));
	record->Add(GetSlot("YMin", &_yMin));
	record->Add(GetSlot("XMax", &_xMax));
	record->Add(GetSlot("YMax", &_yMax));
	return record;
}


} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
