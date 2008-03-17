// ****************************************************************************************************
// File: Transformation.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Transformation.h"

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

Transformation::Transformation(const Unit& tx, const Unit& ty, const Orientation& orientation)
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

Unit Transformation::getX(const Unit& x, const Unit& y) const
// **********************************************************
{
    return (x * A[_orientation]) + (y * B[_orientation]) + _tx;
}

Unit Transformation::getY(const Unit& x, const Unit& y) const
// **********************************************************
{
    return (x * C[_orientation]) + (y * D[_orientation]) + _ty;
}

Unit Transformation::getX(const Point& point) const
// ************************************************
{
    return getX(point.getX(), point.getY());
}

Unit Transformation::getY(const Point& point) const
// ************************************************
{
    return getY(point.getX(), point.getY());
}

Unit Transformation::getDx(const Unit& dx, const Unit& dy) const
// *************************************************************
{
    return (dx * A[_orientation]) + (dy * B[_orientation]);
}

Unit Transformation::getDy(const Unit& dx, const Unit& dy) const
// *************************************************************
{
    return (dx * C[_orientation]) + (dy * D[_orientation]);
}

Point Transformation::getPoint(const Unit& x, const Unit& y) const
// ***************************************************************
{
    return Point(getX(x, y), getY(x, y));
}

Point Transformation::getPoint(const Point& point) const
// *****************************************************
{
    return getPoint(point.getX(), point.getY());
}

Box Transformation::getBox(const Unit& x1, const Unit& y1, const Unit& x2, const Unit& y2) const
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
    Unit x = transformation._tx;
    Unit y = transformation._ty;

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

void Transformation::applyOn(Unit& x, Unit& y) const
// *************************************************
{
    Unit xi = x, yi = y;
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
             getValueString(_tx) + " " +
             getValueString(_ty) + " " +
             getString(_orientation) +
             ">";
}

Record* Transformation::_getRecord() const
// *********************************
{
    Record* record = new Record(getString(this));
    record->Add(getSlot("X", &_tx));
    record->Add(getSlot("Y", &_ty));
    record->Add(getSlot("Orientation", _orientation));
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
    record->Add(getSlot("Code", &_code));
    return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
