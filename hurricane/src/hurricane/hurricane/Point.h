// ****************************************************************************************************
// File: Point.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_POINT
#define HURRICANE_POINT

#include "hurricane/DbU.h"

namespace Hurricane {



// ****************************************************************************************************
// Point declaration
// ****************************************************************************************************

class Point {
// ********

// Attributes
// **********

    private: DbU::Unit _x;
    private: DbU::Unit _y;

// Constructors
// ************

    public: Point();

    public: Point(const DbU::Unit& x, const DbU::Unit& y);

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

    public: const DbU::Unit& getX() const {return _x;};
    public: const DbU::Unit& getY() const {return _y;};
    public: DbU::Unit manhattanDistance(const Point pt) const 
            { return abs(_x - pt.getX()) + abs(_y - pt.getY()); }

// Updators
// ********

    public: void setX(const DbU::Unit& x) {_x = x;};
    public: void setY(const DbU::Unit& y) {_y = y;};
    public: Point& translate(const DbU::Unit& dx, const DbU::Unit& dy);
    public: Point getTranslated(const DbU::Unit& dx, const DbU::Unit& dy) const;

// Others
// ******

    public: string _getTypeName() const { return _TName("Point"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

};



} // End of Hurricane namespace.


ValueIOStreamSupport(Hurricane::Point)


#endif // HURRICANE_POINT

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
