// ****************************************************************************************************
// File: ./Interval.cpp
// Authors: R. Escassut
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
    _vMin = DbU::Min;
    _vMax = DbU::Max;
  }
}

Interval::Interval(const DbU::Unit& value)
// **********************************
:    _vMin(value),
    _vMax(value)
{
}

Interval::Interval(const DbU::Unit& v1, const DbU::Unit& v2)
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

bool Interval::contains(const DbU::Unit& v) const
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

Interval& Interval::shrinkVMin(const DbU::Unit& vMin)
// **************************************************
{
  if ( vMin > _vMin )
    _vMin = vMin;
  return *this;
}

Interval& Interval::shrinkVMax(const DbU::Unit& vMax)
// **************************************************
{
  if ( vMax < _vMax )
    _vMax = vMax;
  return *this;
}

Interval& Interval::inflate(const DbU::Unit& dv)
// ****************************************
{
    return inflate(dv, dv);
}

Interval& Interval::inflate(const DbU::Unit& dvMin, const DbU::Unit& dvMax)
// **************************************************************
{
    if (!isEmpty()) {
        _vMin -= dvMin;
        _vMax += dvMax;
    }
    return *this;
}

Interval& Interval::merge(const DbU::Unit& v)
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
    if ( isEmpty() ) {
        _vMin = interval._vMin;
        _vMax = interval._vMax;
    } else {
        if (!interval.isEmpty()) {
            _vMin = min(_vMin, interval._vMin);
            _vMax = max(_vMax, interval._vMax);
        }
    }
    return *this;
}

Interval& Interval::intersection(const DbU::Unit& vMin, const DbU::Unit& vMax)
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

Interval& Interval::translate(const DbU::Unit& dv)
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
  if ( isEmpty() )
    return "<" + _TName("Interval") + " (empty) " + DbU::getValueString(_vMin) + " " + DbU::getValueString(_vMax) + ">";
  return "<" + _TName("Interval") + " " + DbU::getValueString(_vMin) + " " + DbU::getValueString(_vMax) + ">";
}

Record* Interval::_getRecord() const
// ***************************
{
    Record* record = new Record(getString(this));
    record->add(DbU::getValueSlot("VMin", &_vMin));
    record->add(DbU::getValueSlot("VMin", &_vMax));
    return record;
}



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
