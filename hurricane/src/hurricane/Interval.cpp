// ****************************************************************************************************
// File: Interval.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Interval.h"

namespace Hurricane {



// ****************************************************************************************************
// Interval implementation
// ****************************************************************************************************

Interval::Interval(bool makeEmpty)
// *******************************
:    _vMin(1),
    _vMax(-1)
{
  if (!makeEmpty) {
    _vMin = UNIT_MIN;
    _vMax = UNIT_MAX;
  }
}

Interval::Interval(const Unit& value)
// **********************************
:    _vMin(value),
    _vMax(value)
{
}

Interval::Interval(const Unit& v1, const Unit& v2)
// ***********************************************
:    _vMin(min(v1, v2)),
    _vMax(max(v1, v2))
{
}

Interval::Interval(const Interval& interval)
// *****************************************
:    _vMin(interval._vMin),
    _vMax(interval._vMax)
{
}

Interval& Interval::operator=(const Interval& interval)
// ****************************************************
{
    _vMin = interval._vMin;
    _vMax = interval._vMax;
    return *this;
}

bool Interval::operator==(const Interval& interval) const
// ******************************************************
{
    return !IsEmpty() && !interval.IsEmpty() && (_vMin == interval._vMin) && (_vMax == interval._vMax);
}

bool Interval::operator!=(const Interval& interval) const
// ******************************************************
{
    return IsEmpty() || interval.IsEmpty() || (_vMin != interval._vMin) || (_vMax != interval._vMax);
}

Interval Interval::getUnion(const Interval& interval) const
// ********************************************************
{
    if (IsEmpty() && interval.IsEmpty()) return Interval();
    return Interval(min(_vMin, interval._vMin), max(_vMax, interval._vMax));
}

Interval Interval::getIntersection(const Interval& interval) const
// ***************************************************************
{
    if (!Intersect(interval)) return Interval();
    return Interval(max(_vMin, interval._vMin), min(_vMax, interval._vMax));
}

bool Interval::IsEmpty() const
// ***************************
{
    return (_vMax < _vMin);
}

bool Interval::IsPonctual() const
// ******************************
{
    return (_vMax == _vMin);
}

bool Interval::Contains(const Unit& v) const
// *****************************************
{
    return !IsEmpty() && (_vMin <= v) && (v <= _vMax);
}

bool Interval::Contains(const Interval& interval) const
// ****************************************************
{
    return !IsEmpty() && !interval.IsEmpty() && (_vMin <= interval._vMin) && (interval._vMax <= _vMax);
}

bool Interval::Intersect(const Interval& interval) const
// *****************************************************
{
    return !IsEmpty() && !interval.IsEmpty() && !((_vMax < interval._vMin) || (interval._vMax < _vMin));
}

bool Interval::Inferior(const Interval& interval, bool strict) const
// *****************************************************************
{
  if (_vMax < interval._vMin) return true;
  return !strict && (_vMax == interval._vMin);
}

bool Interval::Superior(const Interval& interval, bool strict) const
// *****************************************************************
{
  if (_vMin > interval._vMax) return true;
  return !strict && (_vMin == interval._vMax);
}

bool Interval::IsConstrainedBy(const Interval& interval) const
// ***********************************************************
{
    return (!IsEmpty() &&
              !interval.IsEmpty() &&
              ((_vMin == interval.getVMin()) || (_vMax == interval.getVMax())));
}

Interval& Interval::MakeEmpty()
// ****************************
{
    _vMin = 1;
    _vMax = -1;
    return *this;
}

Interval& Interval::Inflate(const Unit& dv)
// ****************************************
{
    return Inflate(dv, dv);
}

Interval& Interval::Inflate(const Unit& dvMin, const Unit& dvMax)
// **************************************************************
{
    if (!IsEmpty()) {
        _vMin -= dvMin;
        _vMax += dvMax;
    }
    return *this;
}

Interval& Interval::Merge(const Unit& v)
// *************************************
{
    if (IsEmpty()) {
        _vMin = v;
        _vMax = v;
    }
    else {
        _vMin = min(_vMin, v);
        _vMax = max(_vMax, v);
    }
    return *this;
}

Interval& Interval::Merge(const Interval& interval)
// ************************************************
{
    if (!interval.IsEmpty()) {
        _vMin = min(_vMin, interval._vMin);
        _vMax = max(_vMax, interval._vMax);
    }
    return *this;
}

Interval& Interval::Intersection(const Unit& vMin, const Unit& vMax)
// *****************************************************************
{
  return Intersection(Interval(vMin,vMax));
}

Interval& Interval::Intersection(const Interval& interval)
// *******************************************************
{
  if (!Intersect(interval)) MakeEmpty();
  else {
    _vMin = max(_vMin, interval._vMin);
    _vMax = min(_vMax, interval._vMax);
  }

  return *this;
}

Interval& Interval::Translate(const Unit& dv)
// ******************************************
{
    if (!IsEmpty()) {
        _vMin += dv;
        _vMax += dv;
    }
    return *this;
}

string Interval::_getString() const
// ********************************
{
    return "<" + _TName("Interval") + " " + getValueString(_vMin) + " " + getValueString(_vMax) + ">";
}

Record* Interval::_getRecord() const
// ***************************
{
    Record* record = new Record(getString(this));
    record->Add(getSlot("VMin", &_vMin));
    record->Add(getSlot("VMin", &_vMax));
    return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
