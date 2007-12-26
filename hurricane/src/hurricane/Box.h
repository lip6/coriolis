// ****************************************************************************************************
// File: Box.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 added ManhattanDistance & ShrinkByFactor

#ifndef HURRICANE_BOX
#define HURRICANE_BOX

#include "Point.h"

namespace Hurricane {



// ****************************************************************************************************
// Box declaration
// ****************************************************************************************************

class Box {
// ******

// Attributes
// **********

	private: Unit _xMin;
	private: Unit _yMin;
	private: Unit _xMax;
	private: Unit _yMax;

// constructors
// ************

	public: Box();

	public: Box(const Unit& x, const Unit& y);
	public: Box(const Point& point);
	public: Box(const Unit& x1, const Unit& y1, const Unit& x2, const Unit& y2);
	public: Box(const Point& point1, const Point& point2);

	public: Box(const Box& box);

// Operators
// *********

	public: Box& operator=(const Box& box);

	public: bool operator==(const Box& box) const;
	public: bool operator!=(const Box& box) const;

// Accessors
// *********

	public: const Unit& GetXMin() const {return _xMin;};
	public: const Unit& GetYMin() const {return _yMin;};
	public: const Unit& GetXMax() const {return _xMax;};
	public: const Unit& GetYMax() const {return _yMax;};

	public: Unit GetXCenter() const {return ((_xMin + _xMax) / 2);};
	public: Unit GetYCenter() const {return ((_yMin + _yMax) / 2);};
	public: Point GetCenter() const {return Point(GetXCenter(), GetYCenter());};

	public: Unit GetWidth() const {return (_xMax - _xMin);};
	public: Unit GetHalfWidth() const {return (GetWidth() / 2);};
	public: Unit GetHeight() const {return (_yMax - _yMin);};
	public: Unit GetHalfHeight() const {return (GetHeight() / 2);};

	public: Box GetUnion(const Box& box) const;

	public: Box GetIntersection(const Box& box) const;
	public: Unit ManhattanDistance(const Point& pt) const;
	public: Unit ManhattanDistance(const Box& box) const;

// Predicates
// **********

	public: bool IsEmpty() const;
	public: bool IsFlat() const;
	public: bool IsPonctual() const;

	public: bool Contains(const Unit& x, const Unit& y) const;
	public: bool Contains(const Point& point) const;
	public: bool Contains(const Box& box) const;

	public: bool Intersect(const Box& box) const;

	public: bool IsConstrainedBy(const Box& box) const;

// Updators
// ********

	public: Box& MakeEmpty();

	public: Box& Inflate(const Unit& d);
	public: Box& Inflate(const Unit& dx, const Unit& dy);
	public: Box& Inflate(const Unit& dxMin, const Unit& dyMin, const Unit& dxMax, const Unit& dyMax);
	public: Box& ShrinkByFactor(double factor);   // 0 <= factor <= 1

	public: Box& Merge(const Unit& x, const Unit& y);
	public: Box& Merge(const Point& point);
	public: Box& Merge(const Unit& x1, const Unit& y1, const Unit& x2, const Unit& y2);
	public: Box& Merge(const Box& box);

	public: Box& Translate(const Unit& dx, const Unit& dy);

// Others
// ******

    public: string _GetTypeName() const { return _TName("Box"); };
	public: string _GetString() const;
	public: Record* _GetRecord() const;

};



} // End of Hurricane namespace.


ValueIOStreamSupport(Hurricane::Box)


#endif // HURRICANE_BOX

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
