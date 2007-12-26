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

	public: const Unit& GetVMin() const {return _vMin;};
	public: const Unit& GetVMax() const {return _vMax;};
	public: Unit& GetVMin() {return _vMin;};
	public: Unit& GetVMax() {return _vMax;};
	public: Unit GetCenter() const {return ((_vMin + _vMax) / 2);};
	public: Unit GetSize() const {return (_vMax - _vMin);};
	public: Unit GetHalfSize() const {return (GetSize() / 2);};
	public: Interval GetUnion(const Interval& interval) const;
	public: Interval GetIntersection(const Interval& interval) const;

// Predicates
// **********

	public: bool IsEmpty() const;
	public: bool IsPonctual() const;
	public: bool Contains(const Unit& v) const;
	public: bool Contains(const Interval& interval) const;
	public: bool Intersect(const Interval& interval) const;
	public: bool Inferior(const Interval& interval, bool strict=true) const;
	public: bool Superior(const Interval& interval, bool strict=true) const;
	public: bool IsConstrainedBy(const Interval& interval) const;

// Updators
// ********

	public: Interval& MakeEmpty();
	public: Interval& Inflate(const Unit& dv);
	public: Interval& Inflate(const Unit& dvMin, const Unit& dvMax);
	public: Interval& Merge(const Unit& v);
	public: Interval& Merge(const Interval& interval);
	public: Interval& Intersection(const Unit& vMin, const Unit& vMax);
	public: Interval& Intersection(const Interval& interval);
	public: Interval& Translate(const Unit& dv);

// Others
// ******

    public: string _GetTypeName() const { return _TName("Interval"); };
	public: string _GetString() const;
	public: Record* _GetRecord() const;

};



} // End of Hurricane namespace.


ValueIOStreamSupport(Hurricane::Interval)


#endif // HURRICANE_INTERVAL

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
