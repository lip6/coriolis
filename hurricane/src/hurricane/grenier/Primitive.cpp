// ****************************************************************************************************
// File: Primitive.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************

#include "Primitive.h"
#include "Symbol.h"
#include "View.h"
#include "Error.h"

#include <math.h>

namespace Hurricane {



// ****************************************************************************************************
// Variables
// ****************************************************************************************************

static vector<Point> POINTS;



// ****************************************************************************************************
// Primitive implementation
// ****************************************************************************************************

Primitive::Primitive(Symbol* symbol)
// *********************************
:	Inherit(),
	_symbol(symbol)
{
	if (!_symbol)
		throw Error("Can't create " + _TName("Primitive") + " : null symbol");
}

void Primitive::_PostCreate()
// **************************
{
	_symbol->_GetPrimitiveList().push_back(this);
	_symbol->_Fit(GetBoundingBox());

	Inherit::_PostCreate();
}

void Primitive::_PreDelete()
// *************************
{
	Inherit::_PreDelete();

	_symbol->_Unfit(GetBoundingBox());
	_symbol->_GetPrimitiveList().remove(this);
}

string Primitive::_GetString() const
// *********************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(_symbol->GetName()));
	return s;
}

Record* Primitive::_GetRecord() const
// ****************************
{
 	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Symbol", _symbol));
	}
	return record;
}

void Primitive::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
// ************************************************************************************************
{
	_Draw(view, updateArea, transformation);
}



// ****************************************************************************************************
// Port implementation
// ****************************************************************************************************

Port::Port(Symbol* symbol, const Name& name, const Point& position)
// ****************************************************************
:	Inherit(symbol),
	_name(name),
	_position(position)
{
	if (name.IsEmpty())
		throw Error("Can't create " + _TName("Port") + " : empty name");

	if (GetSymbol()->GetPort(_name))
		throw Error("Can't create " + _TName("Port") + " : already exists");
}

Port* Port::Create(Symbol* symbol, const Name& name, const Point& position)
// ************************************************************************
{
	Port* port = new Port(symbol, name, position);

	port->_PostCreate();

	return port;
}

Port* Port::Create(Symbol* symbol, const Name& name, const Unit& x, const Unit& y)
// *******************************************************************************
{
	Port* port = new Port(symbol, name, Point(x, y));

	port->_PostCreate();

	return port;
}

string Port::_GetString() const
// ****************************
{
	string s = Inherit::_GetString();
    s.insert(s.length() - 1, " " + GetString(_name));
	s.insert(s.length() - 1, " [" + GetValueString(_position.GetX()));
	s.insert(s.length() - 1, " " + GetValueString(_position.GetY()) + "]");
	return s;
}

Record* Port::_GetRecord() const
// ***********************
{
 	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Name", &_name));
		record->Add(GetSlot("Position", &_position));
	}
	return record;
}

bool Port::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
// ************************************************************************************
{
	return GetBoundingBox().Intersect(Box(point).Inflate(aperture));
}

void Port::_Draw(View* view, const Box& updateArea, const Transformation& transformation)
// **************************************************************************************
{
	if (0 < view->GetScale()) {
		if (view->GetScale() <= 10)
			view->DrawPoint(transformation.GetPoint(_position), 1);
		else if (view->GetScale() <= 20)
			view->DrawPoint(transformation.GetPoint(_position), 2);
		else
			view->DrawPoint(transformation.GetPoint(_position), 3);
	}
}



// ****************************************************************************************************
// Line implementation
// ****************************************************************************************************

Line::Line(Symbol* symbol, const Point& source, const Point& target)
// *****************************************************************
:	Inherit(symbol),
	_source(source),
	_target(target)
{
}

Line* Line::Create(Symbol* symbol, const Point& source, const Point& target)
// *************************************************************************
{
	Line* line = new Line(symbol, source, target);

	line->_PostCreate();

	return line;
}

Line* Line::Create(Symbol* symbol, const Unit& xSource, const Unit& ySource, const Unit& xTarget, const Unit& yTarget)
// ****************************************************************************************************
{
	Line* line = new Line(symbol, Point(xSource, ySource), Point(xTarget, yTarget));

	line->_PostCreate();

	return line;
}

string Line::_GetString() const
// ****************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " [" + GetValueString(_source.GetX()));
	s.insert(s.length() - 1, " " + GetValueString(_source.GetY()) + "]");
	s.insert(s.length() - 1, " [" + GetValueString(_target.GetX()));
	s.insert(s.length() - 1, " " + GetValueString(_target.GetY()) + "]");
	return s;
}

Record* Line::_GetRecord() const
// ***********************
{
 	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Source", &_source));
		record->Add(GetSlot("Target", &_target));
	}
	return record;
}

bool Line::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
// ************************************************************************************
{
	if (GetBoundingBox().Intersect(Box(point).Inflate(aperture))) {

		double a = GetValue(aperture);

		double x = GetValue(point.GetX());
		double y = GetValue(point.GetY());
		double xo = GetValue(_source.GetX());
		double yo = GetValue(_source.GetY());
		double xe = GetValue(_target.GetX());
		double ye = GetValue(_target.GetY());

		double xp = xo;
		double yp = yo;

		if (xo != xe) xp = (((xe - xo) / (ye - yo)) * (y - yo)) + xo;
		if (yo != ye) yp = (((ye - yo) / (xe - xo)) * (x - xo)) + yo;
		if ((abs(xp - x) <= a) || (abs(yp - y) <= a)) return true;

	}

	return false;
}

void Line::_Draw(View* view, const Box& updateArea, const Transformation& transformation)
// **************************************************************************************
{
	view->DrawLine(transformation.GetPoint(_source), transformation.GetPoint(_target));
}



// ****************************************************************************************************
// Rectangle implementation
// ****************************************************************************************************

Rectangle::Rectangle(Symbol* symbol, const Box& box, bool isFilled)
// ****************************************************************
:	Inherit(symbol),
	_box(box),
	_isFilled(isFilled)
{
}

Rectangle* Rectangle::Create(Symbol* symbol, const Box& box, bool isFilled)
// ************************************************************************
{
	Rectangle* rectangle = new Rectangle(symbol, box, isFilled);

	rectangle->_PostCreate();

	return rectangle;
}

Rectangle* Rectangle::Create(Symbol* symbol, const Point& point1, const Point& point2, bool isFilled)
// **************************************************************************************************
{
	Rectangle* rectangle = new Rectangle(symbol, Box(point1, point2), isFilled);

	rectangle->_PostCreate();

	return rectangle;
}

Rectangle* Rectangle::Create(Symbol* symbol, const Unit& x1, const Unit& y1, const Unit& x2, const Unit& y2, bool isFilled)
// **************************************************************************************************
{
	Rectangle* rectangle = new Rectangle(symbol, Box(x1, y1, x2, y2), isFilled);

	rectangle->_PostCreate();

	return rectangle;
}

string Rectangle::_GetString() const
// *********************************
{
	string s = Inherit::_GetString();
	if (_box.IsEmpty())
		s.insert(s.length() - 1, " [empty]");
	else {
		s.insert(s.length() - 1, " [" + GetValueString(_box.GetXMin()));
		s.insert(s.length() - 1, " " + GetValueString(_box.GetYMin()));
		s.insert(s.length() - 1, " " + GetValueString(_box.GetXMax()));
		s.insert(s.length() - 1, " " + GetValueString(_box.GetYMax()) + "]");
	}
	if (_isFilled) s.insert(s.length() - 1, " FILLED");
	return s;
}

Record* Rectangle::_GetRecord() const
// ****************************
{
 	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Box", &_box));
		record->Add(GetSlot("IsFilled", &_isFilled));
	}
	return record;
}

bool Rectangle::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
// *****************************************************************************************
{
	Box area = Box(point).Inflate(aperture);

	return (_isFilled) ?
			 _box.Intersect(area) : 
			 (_box.Intersect(area) && !Box(_box).Inflate(-aperture * 2).Intersect(area));

}

void Rectangle::_Draw(View* view, const Box& updateArea, const Transformation& transformation)
// *******************************************************************************************
{
	if (!_isFilled)
		view->DrawRectangle(transformation.GetBox(_box));
	else
		view->FillRectangle(transformation.GetBox(_box));
}



// ****************************************************************************************************
// Circle implementation
// ****************************************************************************************************

Circle::Circle(Symbol* symbol, const Point& center, const Unit& radius)
// ********************************************************************
:	Inherit(symbol),
	_center(center),
	_radius(radius)
{
	if (_radius < 0)
		throw Error("Can't create " + _TName("Circle") + " : negative radius");
}

Circle* Circle::Create(Symbol* symbol, const Unit& x, const Unit& y, const Unit& radius)
// *************************************************************************************
{
	Circle* circle = new Circle(symbol, Point(x, y), radius);

	circle->_PostCreate();

	return circle;
}

Circle* Circle::Create(Symbol* symbol, const Point& center, const Unit& radius)
// ****************************************************************************
{
	Circle* circle = new Circle(symbol, center, radius);

	circle->_PostCreate();

	return circle;
}

string Circle::_GetString() const
// ******************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " [" + GetValueString(_center.GetX()));
	s.insert(s.length() - 1, " " + GetValueString(_center.GetY()) + "]");
	s.insert(s.length() - 1, " " + GetValueString(_radius));
	return s;
}

Record* Circle::_GetRecord() const
// *************************
{
 	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Center", &_center));
		record->Add(GetSlot("Radius", &_radius));
	}
	return record;
}

bool Circle::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
// **************************************************************************************
{
	if (GetBoundingBox().Intersect(Box(point).Inflate(aperture))) {

		double xp = GetValue(point.GetX());
		double yp = GetValue(point.GetY());
		double xc = GetValue(_center.GetX());
		double yc = GetValue(_center.GetY());

		double d = sqrt(pow(abs(xp - xc), 2) + pow(abs(yp - yc), 2)) - GetValue(aperture);

		return (d <= GetValue(_radius));

	}

	return false;
}

void Circle::_Draw(View* view, const Box& updateArea, const Transformation& transformation)
// ****************************************************************************************
{
	view->DrawCircle(transformation.GetPoint(_center), _radius);
}



// ****************************************************************************************************
// Lines implementation
// ****************************************************************************************************

Lines::Lines(Symbol* symbol, const Points& points)
// ***********************************************
:	Inherit(symbol),
	_points(points)
{
}

Lines* Lines::Create(Symbol* symbol, const Points& points)
// *******************************************************
{
	Lines* lines = new Lines(symbol, points);

	lines->_PostCreate();

	return lines;
}

Box Lines::GetBoundingBox() const
// ******************************
{
	Box boundingBox;
	for (unsigned i = 0; i < _points.size(); i++)
		boundingBox.Merge(_points[i]);
	return boundingBox;
}

string Lines::_GetString() const
// *****************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetValueString(_points.size()));
	return s;
}

Record* Lines::_GetRecord() const
// ************************
{
 	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Points", &_points));
	}
	return record;
}

bool Lines::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
// *************************************************************************************
{
	if (1 < _points.size()) {
		double a = GetValue(aperture);
		double x = GetValue(point.GetX());
		double y = GetValue(point.GetY());

		Point origin = _points[0];
		for (unsigned i = 1; i < _points.size(); i++) {
			Point extremity = _points[i];
			if (Box(origin, extremity).Intersect(Box(point).Inflate(aperture))) {
				double xo = GetValue(origin.GetX());
				double yo = GetValue(origin.GetY());
				double xe = GetValue(extremity.GetX());
				double ye = GetValue(extremity.GetY());
				double xp = xo;
				double yp = yo;
				if (xo != xe) xp = (((xe - xo) / (ye - yo)) * (y - yo)) + xo;
				if (yo != ye) yp = (((ye - yo) / (xe - xo)) * (x - xo)) + yo;
				if ((abs(xp - x) <= a) || (abs(yp - y) <= a)) return true;
			}
			origin = extremity;
		}
	}

	return false;
}

void Lines::_Draw(View* view, const Box& updateArea, const Transformation& transformation)
// ***************************************************************************************
{
	for (unsigned i = POINTS.size(); i < _points.size(); i++)
		POINTS.push_back(Point(0, 0));

	for (unsigned i = 0; i < _points.size(); i++)
		POINTS[i] = transformation.GetPoint(_points[i]);

	view->DrawLines(POINTS, _points.size());
}



// ****************************************************************************************************
// Polygon implementation
// ****************************************************************************************************

Polygon::Polygon(Symbol* symbol, const Points& points)
// ***************************************************
:	Inherit(symbol),
	_points(points)
{
}

Polygon* Polygon::Create(Symbol* symbol, const Points& points)
// ***********************************************************
{
	Polygon* polygon = new Polygon(symbol, points);

	polygon->_PostCreate();

	return polygon;
}

Box Polygon::GetBoundingBox() const
// ********************************
{
	Box boundingBox;
	for (unsigned i = 0; i < _points.size(); i++)
		boundingBox.Merge(_points[i]);
	return boundingBox;
}

string Polygon::_GetString() const
// *******************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetValueString(_points.size()));
	return s;
}

Record* Polygon::_GetRecord() const
// **************************
{
 	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Points", &_points));
	}
	return record;
}

bool Polygon::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
// ***************************************************************************************
{
	if (1 < _points.size()) {
		double a = GetValue(aperture);
		double x = GetValue(point.GetX());
		double y = GetValue(point.GetY());

		Point origin = _points[0];
		for (unsigned i = 1; i <= _points.size(); i++) {
			Point extremity = (i == _points.size()) ? _points[0] : _points[i];
			if (Box(origin, extremity).Intersect(Box(point).Inflate(aperture))) {
				double xo = GetValue(origin.GetX());
				double yo = GetValue(origin.GetY());
				double xe = GetValue(extremity.GetX());
				double ye = GetValue(extremity.GetY());
				double xp = xo;
				double yp = yo;
				if (xo != xe) xp = (((xe - xo) / (ye - yo)) * (y - yo)) + xo;
				if (yo != ye) yp = (((ye - yo) / (xe - xo)) * (x - xo)) + yo;
				if ((abs(xp - x) <= a) || (abs(yp - y) <= a)) return true;
			}
			origin = extremity;
		}
	}

	return false;
}

void Polygon::_Draw(View* view, const Box& updateArea, const Transformation& transformation)
// *****************************************************************************************
{
	for (unsigned i = POINTS.size(); i < _points.size(); i++)
		POINTS.push_back(Point(0, 0));

	for (unsigned i = 0; i < _points.size(); i++)
		POINTS[i] = transformation.GetPoint(_points[i]);

	view->DrawPolygon(POINTS, _points.size());
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
