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

    public: const Unit& getXMin() const {return _xMin;};
    public: const Unit& getYMin() const {return _yMin;};
    public: const Unit& getXMax() const {return _xMax;};
    public: const Unit& getYMax() const {return _yMax;};

    public: Unit getXCenter() const {return ((_xMin + _xMax) / 2);};
    public: Unit getYCenter() const {return ((_yMin + _yMax) / 2);};
    public: Point getCenter() const {return Point(getXCenter(), getYCenter());};

    public: Unit getWidth() const {return (_xMax - _xMin);};
    public: Unit getHalfWidth() const {return (getWidth() / 2);};
    public: Unit getHeight() const {return (_yMax - _yMin);};
    public: Unit getHalfHeight() const {return (getHeight() / 2);};

    public: Box getUnion(const Box& box) const;

    public: Box getIntersection(const Box& box) const;
    public: Unit manhattanDistance(const Point& pt) const;
    public: Unit manhattanDistance(const Box& box) const;

// Predicates
// **********

    public: bool isEmpty() const;
    public: bool isFlat() const;
    public: bool isPonctual() const;

    public: bool contains(const Unit& x, const Unit& y) const;
    public: bool contains(const Point& point) const;
    public: bool contains(const Box& box) const;

    public: bool intersect(const Box& box) const;

    public: bool isConstrainedBy(const Box& box) const;

// Updators
// ********

    public: Box& makeEmpty();

    public: Box& inflate(const Unit& d);
    public: Box& inflate(const Unit& dx, const Unit& dy);
    public: Box& inflate(const Unit& dxMin, const Unit& dyMin, const Unit& dxMax, const Unit& dyMax);
    public: Box& shrinkByFactor(double factor);   // 0 <= factor <= 1

    public: Box& merge(const Unit& x, const Unit& y);
    public: Box& merge(const Point& point);
    public: Box& merge(const Unit& x1, const Unit& y1, const Unit& x2, const Unit& y2);
    public: Box& merge(const Box& box);

    public: Box& translate(const Unit& dx, const Unit& dy);

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
