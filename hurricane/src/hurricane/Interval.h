// ****************************************************************************************************
// File: Interval.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_INTERVAL
#define HURRICANE_INTERVAL

#include "Unit.h"

namespace Hurricane {



// ****************************************************************************************************
// Interval declaration
// ****************************************************************************************************

class Interval {
// ***********

// Attributes
// **********

    private: Unit _vMin;
    private: Unit _vMax;

// Constructors
// ************

    public: Interval(bool makeEmpty=true);
    public: Interval(const Unit& v);
    public: Interval(const Unit& v1, const Unit& v2);
    public: Interval(const Interval& interval);

// Operators
// *********

    public: Interval& operator=(const Interval& interval);

    public: bool operator==(const Interval& interval) const;
    public: bool operator!=(const Interval& interval) const;

// Accessors
// *********

    public: const Unit& getVMin() const {return _vMin;};
    public: const Unit& getVMax() const {return _vMax;};
    public: Unit& getVMin() {return _vMin;};
    public: Unit& getVMax() {return _vMax;};
    public: Unit getCenter() const {return ((_vMin + _vMax) / 2);};
    public: Unit getSize() const {return (_vMax - _vMin);};
    public: Unit getHalfSize() const {return (getSize() / 2);};
    public: Interval getUnion(const Interval& interval) const;
    public: Interval getIntersection(const Interval& interval) const;

// Predicates
// **********

    public: bool isEmpty() const;
    public: bool isPonctual() const;
    public: bool contains(const Unit& v) const;
    public: bool contains(const Interval& interval) const;
    public: bool intersect(const Interval& interval) const;
    public: bool inferior(const Interval& interval, bool strict=true) const;
    public: bool superior(const Interval& interval, bool strict=true) const;
    public: bool isConstrainedBy(const Interval& interval) const;

// Updators
// ********

    public: Interval& makeEmpty();
    public: Interval& inflate(const Unit& dv);
    public: Interval& inflate(const Unit& dvMin, const Unit& dvMax);
    public: Interval& merge(const Unit& v);
    public: Interval& merge(const Interval& interval);
    public: Interval& intersection(const Unit& vMin, const Unit& vMax);
    public: Interval& intersection(const Interval& interval);
    public: Interval& translate(const Unit& dv);

// Others
// ******

    public: string _getTypeName() const { return _TName("Interval"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

};



} // End of Hurricane namespace.


ValueIOStreamSupport(Hurricane::Interval)


#endif // HURRICANE_INTERVAL

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
