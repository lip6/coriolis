// ****************************************************************************************************
// File: ./Point.cpp
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

#include "hurricane/Point.h"

namespace Hurricane {



// ****************************************************************************************************
// Point implementation
// ****************************************************************************************************

Point::Point()
// ***********
:    _x(0),
    _y(0)
{
}

Point::Point(const DbU::Unit& x, const DbU::Unit& y)
// ***************************************
:    _x(x),
    _y(y)
{
}

Point::Point(const Point& point)
// *****************************
:    _x(point._x),
    _y(point._y)
{
}

Point& Point::operator=(const Point& point)
// ****************************************
{
    _x = point._x;
    _y = point._y;
    return *this;
}

bool Point::operator==(const Point& point) const
// *********************************************
{
    return ((_x == point._x) && (_y == point._y));
}

bool Point::operator!=(const Point& point) const
// *********************************************
{
    return ((_x != point._x) || (_y != point._y));
}

Point Point::operator+(const Point& point) const
// *********************************************
{
    return Point(_x+point._x,_y+point._y);
}

Point Point::operator-(const Point& point) const
// *********************************************
{
    return Point(_x-point._x,_y-point._y);
}

Point& Point::operator+=(const Point &point)
// *****************************************
{
    _x += point._x;
    _y += point._y;
    return *this;
}

Point& Point::operator-=(const Point &point)
// *****************************************
{
    _x -= point._x;
    _y -= point._y;
    return *this;
}

Point& Point::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// ****************************************************
{
    _x += dx;
    _y += dy;
    return *this;
}

Point& Point::translate(const Point& p)
// ************************************
{
    _x += p.getX();
    _y += p.getY();
    return *this;
}

Point Point::getTranslated(const DbU::Unit& dx, const DbU::Unit& dy) const {
    return Point(*this).translate(dx, dy);
}

string Point::_getString() const
// *****************************
{
  return "<" + _TName("Point") + " " + DbU::getValueString(_x) + " " + DbU::getValueString(_y) + ">";
}

Record* Point::_getRecord() const
// ******************************
{
    Record* record = new Record(getString(this));
    record->add(DbU::getValueSlot("X", &_x));
    record->add(DbU::getValueSlot("Y", &_y));
    return record;
}

void  Point::toJson ( JsonWriter* w ) const
// ****************************************
{

  w->startObject();
  jsonWrite( w, "@typename", "Point" );
  jsonWrite( w, "_x", getX() );
  jsonWrite( w, "_y", getY() );
  w->endObject();
}

Initializer<JsonPoint>  jsonPointInit ( 0 );

void  JsonPoint::initialize()
// **************************
{ JsonTypes::registerType( new JsonPoint (JsonWriter::RegisterMode) ); }

JsonPoint::JsonPoint(unsigned long flags)
// **************************************
  : JsonObject(flags)
{
  add( "_x", typeid(int64_t) );
  add( "_y", typeid(int64_t) );
}

JsonPoint* JsonPoint::clone(unsigned long flags) const
// ***************************************************
{ return new JsonPoint ( flags ); }

string JsonPoint::getTypeName() const
// **********************************
{ return "Point"; }

void JsonPoint::toData(JsonStack& stack)
// *************************************
{
  check( stack, "JsonPoint::toData" );
  Point point ( DbU::fromDb(get<int64_t>(stack,"_x"))
              , DbU::fromDb(get<int64_t>(stack,"_y")) );
  update( stack, point );
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
