// ****************************************************************************************************
// File: Point.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6 : added operators+ += ... (questionnable!)

#ifndef HURRICANE_POINT
#define HURRICANE_POINT

#include "Unit.h"

namespace Hurricane {



// ****************************************************************************************************
// Point declaration
// ****************************************************************************************************

class Point {
// ********

// Attributes
// **********

    private: Unit _x;
    private: Unit _y;

// Constructors
// ************

    public: Point();

    public: Point(const Unit& x, const Unit& y);

    public: Point(const Point& point);

// Operators
// *********

    public: Point& operator=(const Point& point);

    public: bool operator==(const Point& point) const;
    public: bool operator!=(const Point& point) const;

     public: Point operator+(const Point& point) const;
     public: Point operator-(const Point& point) const;
     public: Point& operator+=(const Point& point);
     public: Point& operator-=(const Point& point);

// Accessors
// *********

    public: const Unit& getX() const {return _x;};
    public: const Unit& getY() const {return _y;};
    public: Unit manhattanDistance(const Point pt) const 
            { return abs(_x - pt.getX()) + abs(_y - pt.getY()); }

// Updators
// ********

    public: void setX(const Unit& x) {_x = x;};
    public: void setY(const Unit& y) {_y = y;};
    public: Point& translate(const Unit& dx, const Unit& dy);

// Others
// ******

    public: string _GetTypeName() const { return _TName("Point"); };
    public: string _GetString() const;
    public: Record* _GetRecord() const;

};



} // End of Hurricane namespace.


ValueIOStreamSupport(Hurricane::Point)


#endif // HURRICANE_POINT

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
