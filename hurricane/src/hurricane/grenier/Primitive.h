// ****************************************************************************************************
// File: Primitive.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_PRIMITIVE
#define HURRICANE_PRIMITIVE

#include "DBo.h"
#include "Primitives.h"
#include "Transformation.h"

namespace Hurricane {

class Symbol;
class View;



// ****************************************************************************************************
// Primitive declaration
// ****************************************************************************************************

class Primitive : public DBo {
// *************************

// Types
// *****

	public: typedef DBo Inherit;

// Attributes
// **********

	private: Symbol* _symbol;

// Constructors
// ************

	protected: Primitive(Symbol* symbol);

// Accessors
// *********

	public: Symbol* GetSymbol() const {return _symbol;};
	public: virtual Box GetBoundingBox() const = 0;

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const = 0;
	public: virtual void _Draw(View* view, const Box& updateArea, const Transformation& transformation) = 0;
	public: virtual void _Highlight(View* view, const Box& updateArea, const Transformation& transformation);

};



// ****************************************************************************************************
// Port declaration
// ****************************************************************************************************

class Port : public Primitive {
// **************************

// Types
// *****

	public: typedef Primitive Inherit;

// Attributes
// **********

	private: Name _name;
	private: Point _position;

// Constructors
// ************

	protected: Port(Symbol* symbol, const Name& name, const Point& position);

	public: static Port* Create(Symbol* symbol, const Name& name, const Point& position);
	public: static Port* Create(Symbol* symbol, const Name& name, const Unit& x, const Unit& y);

// Accessors
// *********

	public: virtual Box GetBoundingBox() const {return Box(_position);};
	public: const Name& GetName() const {return _name;};
	public: const Unit& GetX() const {return _position.GetX();};
	public: const Unit& GetY() const {return _position.GetY();};
	public: const Point& GetPosition() const {return _position;};

// Others
// ******

	public: virtual string _GetTypeName() const {return _TName("Port");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
	public: virtual void _Draw(View* view, const Box& updateArea, const Transformation& transformation);

};



// ****************************************************************************************************
// Line declaration
// ****************************************************************************************************

class Line : public Primitive {
// **************************

// Types
// *****

	public: typedef Primitive Inherit;

// Attributes
// **********

	private: Point _source;
	private: Point _target;

// Constructors
// ************

	protected: Line(Symbol* symbol, const Point& source, const Point& target);

	public: static Line* Create(Symbol* symbol, const Point& source, const Point& target);
	public: static Line* Create(Symbol* symbol, const Unit& xSource, const Unit& ySource, const Unit& xTarget, const Unit& yTarget);

// Accessors
// *********

	public: virtual Box GetBoundingBox() const {return Box(_source, _target);};
	public: const Point& GetSource() const {return _source;};
	public: const Point& GetTarget() const {return _target;};

// Others
// ******

	public: virtual string _GetTypeName() const {return _TName("Line");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
	public: virtual void _Draw(View* view, const Box& updateArea, const Transformation& transformation);

};



// ****************************************************************************************************
// Rectangle declaration
// ****************************************************************************************************

class Rectangle : public Primitive {
// *******************************

// Types
// *****

	public: typedef Primitive Inherit;

// Attributes
// **********

	private: Box _box;
	private: bool _isFilled;

// Constructors
// ************

	protected: Rectangle(Symbol* symbol, const Box& box, bool isFilled = false);

	public: static Rectangle* Create(Symbol* symbol, const Box& box, bool isFilled = false);
	public: static Rectangle* Create(Symbol* symbol, const Point& point1, const Point& point2, bool isFilled = false);
	public: static Rectangle* Create(Symbol* symbol, const Unit& x1, const Unit& y1, const Unit& x2, const Unit& y2, bool isFilled = false);

// Accessors
// *********

	public: virtual Box GetBoundingBox() const {return _box;};
	public: const Box& GetBox() const {return _box;};

// Others
// ******

	public: virtual string _GetTypeName() const {return _TName("Rectangle");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
	public: virtual void _Draw(View* view, const Box& updateArea, const Transformation& transformation);

};



// ****************************************************************************************************
// Circle declaration
// ****************************************************************************************************

class Circle : public Primitive {
// ****************************

// Types
// *****

	public: typedef Primitive Inherit;

// Attributes
// **********

	private: Point _center;
	private: Unit _radius;

// Constructors
// ************

	protected: Circle(Symbol* symbol, const Point& center, const Unit& radius);

	public: static Circle* Create(Symbol* symbol, const Unit& x, const Unit& y, const Unit& radius);
	public: static Circle* Create(Symbol* symbol, const Point& center, const Unit& radius);

// Accessors
// *********

	public: virtual Box GetBoundingBox() const {return Box(_center).Inflate(_radius);};
	public: const Point& GetCenter() const {return _center;};
	public: const Unit& GetRadius() const {return _radius;};

// Others
// ******

	public: virtual string _GetTypeName() const {return _TName("Circle");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
	public: virtual void _Draw(View* view, const Box& updateArea, const Transformation& transformation);

};



// ****************************************************************************************************
// Lines declaration
// ****************************************************************************************************

class Lines : public Primitive {
// ***************************

// Types
// *****

	public: typedef Primitive Inherit;

	public: typedef vector<Point> Points;

// Attributes
// **********

	private: Points _points;

// Constructors
// ************

	protected: Lines(Symbol* symbol, const Points& points);

	public: static Lines* Create(Symbol* symbol, const Points& points);

// Accessors
// *********

	public: virtual Box GetBoundingBox() const;
	public: const Points& GetPoints() const {return _points;};
	public: unsigned GetSize() const {return _points.size();};

// Others
// ******

	public: virtual string _GetTypeName() const {return _TName("Lines");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
	public: virtual void _Draw(View* view, const Box& updateArea, const Transformation& transformation);

};



// ****************************************************************************************************
// Polygon declaration
// ****************************************************************************************************

class Polygon : public Primitive {
// *****************************

// Types
// *****

	public: typedef Primitive Inherit;

	public: typedef vector<Point> Points;

// Attributes
// **********

	private: Points _points;

// Constructors
// ************

	protected: Polygon(Symbol* symbol, const Points& points);

	public: static Polygon* Create(Symbol* symbol, const Points& points);

// Accessors
// *********

	public: virtual Box GetBoundingBox() const;
	public: const Points& GetPoints() const {return _points;};
	public: unsigned GetSize() const {return _points.size();};

// Others
// ******

	public: virtual string _GetTypeName() const {return _TName("Polygon");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
	public: virtual void _Draw(View* view, const Box& updateArea, const Transformation& transformation);

};



} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Primitive)
SetNestedSlotAdapter(Hurricane::Port)
SetNestedSlotAdapter(Hurricane::Line)
SetNestedSlotAdapter(Hurricane::Rectangle)
SetNestedSlotAdapter(Hurricane::Lines)
SetNestedSlotAdapter(Hurricane::Polygon)

#endif // HURRICANE_PRIMITIVE

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
