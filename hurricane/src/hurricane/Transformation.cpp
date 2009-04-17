// ****************************************************************************************************
// File: ./Transformation.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
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

#include "hurricane/Transformation.h"

namespace Hurricane {

#define TO Transformation::Orientation



// ****************************************************************************************************
// Variables
// ****************************************************************************************************

static const int A[8] = {1, 0, -1, 0, -1, 0, 1, 0};
static const int B[8] = {0, -1, 0, 1, 0, -1, 0, 1};
static const int C[8] = {0, 1, 0, -1, 0, -1, 0, 1};
static const int D[8] = {1, 0, -1, 0, 1, 0, -1, 0};

static const Transformation::Orientation INVERT[8] =
    {TO::ID, TO::R3, TO::R2, TO::R1, TO::MX, TO::XR, TO::MY, TO::YR};

static const int DISCREMINENT[8] = {1, 1, 1, 1, -1, -1, -1, -1};

static const Transformation::Orientation COMPOSE[] = {
    TO::ID, TO::R1, TO::R2, TO::R3, TO::MX, TO::XR, TO::MY, TO::YR,
    TO::R1, TO::R2, TO::R3, TO::ID, TO::XR, TO::MY, TO::YR, TO::MX,
    TO::R2, TO::R3, TO::ID, TO::R1, TO::MY, TO::YR, TO::MX, TO::XR,
    TO::R3, TO::ID, TO::R1, TO::R2, TO::YR, TO::MX, TO::XR, TO::MY,
    TO::MX, TO::YR, TO::MY, TO::XR, TO::ID, TO::R3, TO::R2, TO::R1,
    TO::XR, TO::MX, TO::YR, TO::MY, TO::R1, TO::ID, TO::R3, TO::R2,
    TO::MY, TO::XR, TO::MX, TO::YR, TO::R2, TO::R1, TO::ID, TO::R3,
    TO::YR, TO::MY, TO::XR, TO::MX, TO::R3, TO::R2, TO::R1, TO::ID
};



// ****************************************************************************************************
// Transformation implementation
// ****************************************************************************************************

Transformation::Transformation()
// *****************************
:    _tx(0),
    _ty(0),
    _orientation()
{
}

Transformation::Transformation(const DbU::Unit& tx, const DbU::Unit& ty, const Orientation& orientation)
// *******************************************************************************************
:    _tx(tx),
    _ty(ty),
    _orientation(orientation)
{
}

Transformation::Transformation(const Point& translation, const Orientation& orientation)
// *************************************************************************************
:    _tx(translation.getX()),
    _ty(translation.getY()),
    _orientation(orientation)
{
}

Transformation::Transformation(const Transformation& transformation)
// *****************************************************************
:    _tx(transformation._tx),
    _ty(transformation._ty),
    _orientation(transformation._orientation)
{
}

Transformation& Transformation::operator=(const Transformation& transformation)
// ****************************************************************************
{
    _tx = transformation._tx;
    _ty = transformation._ty;
    _orientation = transformation._orientation;
    return *this;
}

bool Transformation::operator==(const Transformation& transformation) const
// ************************************************************************
{
    return ((_tx == transformation._tx) &&
              (_ty == transformation._ty) &&
              (_orientation == transformation._orientation));
}

bool Transformation::operator!=(const Transformation& transformation) const
// ************************************************************************
{
    return ((_tx != transformation._tx) ||
              (_ty != transformation._ty) ||
              (_orientation != transformation._orientation));
}

DbU::Unit Transformation::getX(const DbU::Unit& x, const DbU::Unit& y) const
// **********************************************************
{
    return (x * A[_orientation]) + (y * B[_orientation]) + _tx;
}

DbU::Unit Transformation::getY(const DbU::Unit& x, const DbU::Unit& y) const
// **********************************************************
{
    return (x * C[_orientation]) + (y * D[_orientation]) + _ty;
}

DbU::Unit Transformation::getX(const Point& point) const
// ************************************************
{
    return getX(point.getX(), point.getY());
}

DbU::Unit Transformation::getY(const Point& point) const
// ************************************************
{
    return getY(point.getX(), point.getY());
}

DbU::Unit Transformation::getDx(const DbU::Unit& dx, const DbU::Unit& dy) const
// *************************************************************
{
    return (dx * A[_orientation]) + (dy * B[_orientation]);
}

DbU::Unit Transformation::getDy(const DbU::Unit& dx, const DbU::Unit& dy) const
// *************************************************************
{
    return (dx * C[_orientation]) + (dy * D[_orientation]);
}

Point Transformation::getPoint(const DbU::Unit& x, const DbU::Unit& y) const
// ***************************************************************
{
    return Point(getX(x, y), getY(x, y));
}

Point Transformation::getPoint(const Point& point) const
// *****************************************************
{
    return getPoint(point.getX(), point.getY());
}

Box Transformation::getBox(const DbU::Unit& x1, const DbU::Unit& y1, const DbU::Unit& x2, const DbU::Unit& y2) const
// *********************************************************************************************
{
    return Box(getX(x1, y1), getY(x1, y1), getX(x2, y2), getY(x2, y2));
}

Box Transformation::getBox(const Point& point1, const Point& point2) const
// ***********************************************************************
{
    return getBox(point1.getX(), point1.getY(), point2.getX(), point2.getY());
}

Box Transformation::getBox(const Box& box) const
// *********************************************
{
    if (box.isEmpty()) return box;
    return getBox(box.getXMin(), box.getYMin(), box.getXMax(), box.getYMax());
}

Transformation Transformation::getTransformation(const Transformation& transformation) const
// *****************************************************************************************
{
    DbU::Unit x = transformation._tx;
    DbU::Unit y = transformation._ty;

    return Transformation(
                (x * A[_orientation]) + (y * B[_orientation]) + _tx,
                (x * C[_orientation]) + (y * D[_orientation]) + _ty,
                COMPOSE[(_orientation * 8) + transformation._orientation]);
}

Transformation Transformation::getInvert() const
// *********************************************
{
    Transformation transformation = *this;
    return transformation.invert();
}

Transformation& Transformation::invert()
// *************************************
{
    return operator=(
        Transformation(
            ((_ty * B[_orientation]) - (_tx * D[_orientation])) * DISCREMINENT[_orientation],
            ((_tx * C[_orientation]) - (_ty * A[_orientation])) * DISCREMINENT[_orientation],
            INVERT[_orientation]));
}

void Transformation::applyOn(DbU::Unit& x, DbU::Unit& y) const
// *************************************************
{
    DbU::Unit xi = x, yi = y;
    x = getX(xi, yi);
    y = getY(xi, yi);
}

void Transformation::applyOn(Point& point) const
// *********************************************
{
    point = getPoint(point);
}

void Transformation::applyOn(Box& box) const
// *****************************************
{
    box = getBox(box);
}

void Transformation::applyOn(Transformation& transformation) const
// ***************************************************************
{
    transformation = getTransformation(transformation);
}

string Transformation::_getString() const
// **************************************
{
    return "<" + _TName("Transformation") + " " +
      DbU::getValueString(_tx) + " " +
      DbU::getValueString(_ty) + " " +
      getString(_orientation) +
      ">";
}

Record* Transformation::_getRecord() const
// *********************************
{
    Record* record = new Record(getString(this));
    record->add(DbU::getValueSlot("_tx",&_tx));
    record->add(DbU::getValueSlot("_ty",&_ty));
    record->add(getSlot("_orientation", &_orientation));
    return record;
}



// ****************************************************************************************************
// Transformation::Orientation implementation
// ****************************************************************************************************

Transformation::Orientation::Orientation(const Code& code)
// *******************************************************
:    _code(code)
{
}

Transformation::Orientation::Orientation(const Orientation& orientation)
// *********************************************************************
:    _code(orientation._code)
{
}

Transformation::Orientation& Transformation::Orientation::operator=(const Orientation& orientation)
// ************************************************************************************************
{
    _code = orientation._code;
    return *this;
}

string Transformation::Orientation::_getString() const
// ***************************************************
{
  return getString(&_code);
}

Record* Transformation::Orientation::_getRecord() const
// **********************************************
{
  Record* record = new Record(getString(this));
    record->add(getSlot("Code", &_code));
    return record;
}



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************
