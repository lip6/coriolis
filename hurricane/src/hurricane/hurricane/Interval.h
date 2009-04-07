// ****************************************************************************************************
// File: Interval.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
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
    public: DbU::Unit getSize() const {return (_vMax - _vMin);};
    public: DbU::Unit getHalfSize() const {return (getSize() / 2);};
    public: Interval getUnion(const Interval& interval) const;
    public: Interval getIntersection(const Interval& interval) const;

// Predicates
// **********

    public: bool isEmpty() const;
    public: bool isPonctual() const;
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


INSPECTOR_PV_SUPPORT(Hurricane::Interval);


#endif // HURRICANE_INTERVAL

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
