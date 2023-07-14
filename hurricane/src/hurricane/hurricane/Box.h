// ****************************************************************************************************
// File: ./hurricane/Box.h
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

#ifndef HURRICANE_BOX
#define HURRICANE_BOX

#include "hurricane/Point.h"

namespace Hurricane {

// ****************************************************************************************************
// Box declaration
// ****************************************************************************************************

class Box {
// ******

// Attributes
// **********

    private: DbU::Unit _xMin;
    private: DbU::Unit _yMin;
    private: DbU::Unit _xMax;
    private: DbU::Unit _yMax;

// constructors
// ************

    public: Box();

    public: Box(const DbU::Unit& x, const DbU::Unit& y);
    public: Box(const Point& point);
    public: Box(const DbU::Unit& x1, const DbU::Unit& y1, const DbU::Unit& x2, const DbU::Unit& y2);
    public: Box(const Point& point1, const Point& point2);

    public: Box(const Box& box);

// Operators
// *********

    public: Box& operator=(const Box& box);

    public: bool operator==(const Box& box) const;
    public: bool operator!=(const Box& box) const;

// Accessors
// *********

    public: const DbU::Unit& getXMin() const {return _xMin;};
    public: const DbU::Unit& getYMin() const {return _yMin;};
    public: const DbU::Unit& getXMax() const {return _xMax;};
    public: const DbU::Unit& getYMax() const {return _yMax;};

    public: DbU::Unit getXCenter() const {return ((_xMin + _xMax) / 2);};
    public: DbU::Unit getYCenter() const {return ((_yMin + _yMax) / 2);};
    public: Point getCenter() const {return Point(getXCenter(), getYCenter());};
    public: Point getCornerBL() const { return Point(_xMin,_yMin); }
    public: Point getCornerTL() const { return Point(_xMin,_yMax); }
    public: Point getCornerTR() const { return Point(_xMax,_yMax); }
    public: Point getCornerBR() const { return Point(_xMax,_yMin); }

    public: DbU::Unit getWidth() const {return (_xMax - _xMin);};
    public: DbU::Unit getHalfWidth() const {return (getWidth() / 2);};
    public: DbU::Unit getHeight() const {return (_yMax - _yMin);};
    public: DbU::Unit getHalfHeight() const {return (getHeight() / 2);};

    public: Box getUnion(const Box& box) const;

    public: Box getIntersection(const Box& box) const;
    public: DbU::Unit manhattanDistance(const Point& pt) const;
    public: DbU::Unit manhattanDistance(const Box& box) const;

// Predicates
// **********

    public: bool isEmpty() const;
    public: bool isFlat() const;
    public: bool isPonctual() const;

    public: bool contains(const DbU::Unit& x, const DbU::Unit& y) const;
    public: bool contains(const Point& point) const;
    public: bool contains(const Box& box) const;

    public: bool intersect(const Box& box) const;

    public: bool isConstrainedBy(const Box& box) const;

// Updators
// ********

    public: Box& makeEmpty();

    public: Box& inflate(const DbU::Unit& d);
    public: Box& inflate(const DbU::Unit& dx, const DbU::Unit& dy);
    public: Box& inflate(const DbU::Unit& dxMin, const DbU::Unit& dyMin, const DbU::Unit& dxMax, const DbU::Unit& dyMax);
    public: Box  getInflated(const DbU::Unit& d) const;
    public: Box& shrinkByFactor(double factor);   // 0 <= factor <= 1

    public: Box& merge(const DbU::Unit& x, const DbU::Unit& y);
    public: Box& merge(const Point& point);
    public: Box& merge(const DbU::Unit& x1, const DbU::Unit& y1, const DbU::Unit& x2, const DbU::Unit& y2);
    public: Box& merge(const Box& box);

    public: Box& translate(const DbU::Unit& dx, const DbU::Unit& dy);

// Others
// ******


    public: string _getTypeName() const { return _TName("Box"); };
    public: string _getString() const;
    public: Record* _getRecord() const;
    public: void toJson(JsonWriter*) const;

};


class JsonBox : public JsonObject {
// ********************************

  public: static void initialize();
  public: JsonBox(unsigned long);
  public: virtual string getTypeName() const;
  public: virtual JsonBox* clone(unsigned long) const;
  public: virtual void toData(JsonStack&); 
};


} // End of Hurricane namespace.


INSPECTOR_PR_SUPPORT(Hurricane::Box);


#endif // HURRICANE_BOX


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
