// ****************************************************************************************************
// File: ./hurricane/Point.h
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

    public: void setX(DbU::Unit x) {_x = x;};
    public: void setY(DbU::Unit y) {_y = y;};
    public: Point& translate(const DbU::Unit& dx, const DbU::Unit& dy);
    public: Point getTranslated(const DbU::Unit& dx, const DbU::Unit& dy) const;

// Others
// ******

    public: string _getTypeName() const { return _TName("Point"); };
    public: string _getString() const;
    public: Record* _getRecord() const;
    public: void toJson(JsonWriter*) const;

};


class JsonPoint : public JsonObject {
// **********************************

  public: static void initialize();
  public: JsonPoint(unsigned long);
  public: virtual string getTypeName() const;
  public: virtual JsonPoint* clone(unsigned long) const;
  public: virtual void toData(JsonStack&); 
};


} // End of Hurricane namespace.

INSPECTOR_PR_SUPPORT(Hurricane::Point);


#endif // HURRICANE_POINT


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
