// ****************************************************************************************************
// File: Interval.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "hurricane/Interval.h"

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
    return !isEmpty() && !interval.isEmpty() && (_vMin == interval._vMin) && (_vMax == interval._vMax);
}

bool Interval::operator!=(const Interval& interval) const
// ******************************************************
{
    return isEmpty() || interval.isEmpty() || (_vMin != interval._vMin) || (_vMax != interval._vMax);
}

Interval Interval::getUnion(const Interval& interval) const
// ********************************************************
{
    if (isEmpty() && interval.isEmpty()) return Interval();
    return Interval(min(_vMin, interval._vMin), max(_vMax, interval._vMax));
}

Interval Interval::getIntersection(const Interval& interval) const
// ***************************************************************
{
    if (!intersect(interval)) return Interval();
    return Interval(max(_vMin, interval._vMin), min(_vMax, interval._vMax));
}

bool Interval::isEmpty() const
// ***************************
{
    return (_vMax < _vMin);
}

bool Interval::isPonctual() const
// ******************************
{
    return (_vMax == _vMin);
}

bool Interval::contains(const Unit& v) const
// *****************************************
{
    return !isEmpty() && (_vMin <= v) && (v <= _vMax);
}

bool Interval::contains(const Interval& interval) const
// ****************************************************
{
    return !isEmpty() && !interval.isEmpty() && (_vMin <= interval._vMin) && (interval._vMax <= _vMax);
}

bool Interval::intersect(const Interval& interval) const
// *****************************************************
{
    return !isEmpty() && !interval.isEmpty() && !((_vMax < interval._vMin) || (interval._vMax < _vMin));
}

bool Interval::inferior(const Interval& interval, bool strict) const
// *****************************************************************
{
  if (_vMax < interval._vMin) return true;
  return !strict && (_vMax == interval._vMin);
}

bool Interval::superior(const Interval& interval, bool strict) const
// *****************************************************************
{
  if (_vMin > interval._vMax) return true;
  return !strict && (_vMin == interval._vMax);
}

bool Interval::isConstrainedBy(const Interval& interval) const
// ***********************************************************
{
    return (!isEmpty() &&
              !interval.isEmpty() &&
              ((_vMin == interval.getVMin()) || (_vMax == interval.getVMax())));
}

Interval& Interval::makeEmpty()
// ****************************
{
    _vMin = 1;
    _vMax = -1;
    return *this;
}

Interval& Interval::inflate(const Unit& dv)
// ****************************************
{
    return inflate(dv, dv);
}

Interval& Interval::inflate(const Unit& dvMin, const Unit& dvMax)
// **************************************************************
{
    if (!isEmpty()) {
        _vMin -= dvMin;
        _vMax += dvMax;
    }
    return *this;
}

Interval& Interval::merge(const Unit& v)
// *************************************
{
    if (isEmpty()) {
        _vMin = v;
        _vMax = v;
    }
    else {
        _vMin = min(_vMin, v);
        _vMax = max(_vMax, v);
    }
    return *this;
}

Interval& Interval::merge(const Interval& interval)
// ************************************************
{
    if (!interval.isEmpty()) {
        _vMin = min(_vMin, interval._vMin);
        _vMax = max(_vMax, interval._vMax);
    }
    return *this;
}

Interval& Interval::intersection(const Unit& vMin, const Unit& vMax)
// *****************************************************************
{
  return intersection(Interval(vMin,vMax));
}

Interval& Interval::intersection(const Interval& interval)
// *******************************************************
{
  if (!intersect(interval)) makeEmpty();
  else {
    _vMin = max(_vMin, interval._vMin);
    _vMax = min(_vMax, interval._vMax);
  }

  return *this;
}

Interval& Interval::translate(const Unit& dv)
// ******************************************
{
    if (!isEmpty()) {
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
    record->add(getSlot("VMin", &_vMin));
    record->add(getSlot("VMin", &_vMax));
    return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
