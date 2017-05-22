// ****************************************************************************************************
// File: ./hurricane/Interval.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
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

#ifndef HURRICANE_INTERVAL
#define HURRICANE_INTERVAL

#include "hurricane/DbU.h"

namespace Hurricane {



// ****************************************************************************************************
// Interval declaration
// ****************************************************************************************************

class Interval {
// ***********

// Attributes
// **********

    private: DbU::Unit _vMin;
    private: DbU::Unit _vMax;

// Constructors
// ************

    public: Interval(bool makeEmpty=true);
    public: Interval(const DbU::Unit& v);
    public: Interval(const DbU::Unit& v1, const DbU::Unit& v2);
    public: Interval(const Interval& interval);

// Operators
// *********

    public: Interval& operator=(const Interval& interval);

    public: bool operator==(const Interval& interval) const;
    public: bool operator!=(const Interval& interval) const;

// Accessors
// *********

    public: const DbU::Unit& getVMin() const {return _vMin;};
    public: const DbU::Unit& getVMax() const {return _vMax;};
    public: DbU::Unit& getVMin() {return _vMin;};
    public: DbU::Unit& getVMax() {return _vMax;};
    public: DbU::Unit getCenter() const {return ((_vMin + _vMax) / 2);};
    public: DbU::Unit getSize() const {return (isEmpty() ? 0 : (_vMax - _vMin));};
    public: DbU::Unit getHalfSize() const {return (getSize() / 2);};
    public: Interval getUnion(const Interval& interval) const;
    public: Interval getIntersection(const Interval& interval) const;

// Predicates
// **********

    public: bool isEmpty() const { return (_vMax < _vMin);};
    public: bool isPonctual() const { return (_vMax == _vMin);};
    public: bool contains(const DbU::Unit& v) const;
    public: bool contains(const Interval& interval) const;
    public: bool intersect(const Interval& interval) const;
    public: bool inferior(const Interval& interval, bool strict=true) const;
    public: bool superior(const Interval& interval, bool strict=true) const;
    public: bool isConstrainedBy(const Interval& interval) const;

// Updators
// ********

    public: Interval& makeEmpty();
    public: Interval& shrinkVMin(const DbU::Unit& vMin);
    public: Interval& shrinkVMax(const DbU::Unit& vMax);
    public: Interval& inflate(const DbU::Unit& dv);
    public: Interval& inflate(const DbU::Unit& dvMin, const DbU::Unit& dvMax);
    public: Interval& merge(const DbU::Unit& v);
    public: Interval& merge(const Interval& interval);
    public: Interval& intersection(const DbU::Unit& vMin, const DbU::Unit& vMax);
    public: Interval& intersection(const Interval& interval);
    public: Interval& translate(const DbU::Unit& dv);

// Others
// ******

    public: string _getTypeName() const { return _TName("Interval"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

};



} // End of Hurricane namespace.


inline void  jsonWrite ( JsonWriter* w, const std::string& key, const Hurricane::Interval* interval )
{
  w->key( key );
  w->startArray();
  w->write( &interval->getVMin() );
  w->write( &interval->getVMax() );
  w->endArray();
}

INSPECTOR_PV_SUPPORT(Hurricane::Interval);


#endif // HURRICANE_INTERVAL


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
