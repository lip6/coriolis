// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Polygon.cpp"                                 |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Polygon.h"
#include "hurricane/Net.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Layer.h"
#include "hurricane/Error.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Polygon::Edge".

  Polygon::Edge::Edge ( Point origin, Point extremity, uint32_t flags )
    : _flags   (flags)
    , _xyOrigin(0)
    , _steps   ()
  {
    if (origin.getX() == extremity.getX()) {
      _flags |= Polygon::Vertical;
      _xyOrigin = origin.getX();
      _steps.push_back( origin.getY() );
      return;
    }
    if (origin.getY() == extremity.getY()) {
      _flags |= Polygon::Horizontal;
      _xyOrigin = origin.getX();
      _steps.push_back( origin.getY() );
      return;
    }

    float slope = Polygon::getSlope( origin, extremity );
    if (extremity.getX() > origin.getX()) _flags |= Polygon::XIncrease;
    if (extremity.getY() > origin.getY()) _flags |= Polygon::YIncrease;

    if ( (slope > 1.0) or (slope < -1.0) ) {
      _flags   |= Polygon::XSteps;
      _xyOrigin = origin.getX();

      DbU::Unit b     = origin.getY() +- slope*origin.getX();
      DbU::Unit delta = DbU::getPolygonStep();
      if (not (_flags & Polygon::XIncrease)) delta = -delta;

      _steps.push_back( origin.getY() );

      DbU::Unit x = origin.getX() + delta;
      while ( true ) {
        if ( (delta > 0) and (x > extremity.getX()) ) break;
        if ( (delta < 0) and (x < extremity.getX()) ) break;
        
        DbU::Unit y = slope*x + b;
        DbU::Unit m = y % DbU::getPolygonStep();

        if (m) {
          if (isClockwise() xor not isXIncrease()) y += DbU::getPolygonStep() - m;
          else                                     y += -m;
        }
        _steps.push_back( y );

        x += delta;
      }
    } else {
      _flags   |= Polygon::YSteps;
      _xyOrigin = origin.getY();

      float     islope = 1/slope;
      DbU::Unit delta  = DbU::getPolygonStep();
      if (not (_flags & Polygon::YIncrease)) delta = -delta;

      _steps.push_back( origin.getX() );

      DbU::Unit y = origin.getY() + delta;
      while ( true ) {
        if ( (delta > 0) and (y > extremity.getY()) ) break;
        if ( (delta < 0) and (y < extremity.getY()) ) break;

        DbU::Unit x = islope*(y - origin.getY()) + origin.getX();
        DbU::Unit m = x % DbU::getPolygonStep();

        if (m) {
          if (isClockwise() xor isYIncrease()) x += DbU::getPolygonStep() - m;
          else                                 x += -m;
        }
        _steps.push_back( x );

        y += delta;
      }
    }
  }


  Point  Polygon::Edge::getPoint ( size_t i ) const
  {
    if (i >= getSize()) return Point( 0, 0 );
    if (isHV()) return Point( _xyOrigin, _steps[0] );

    size_t offset = (_flags & Polygon::Above) ? 0 : 1;
    if (not (_flags & Polygon::YIncrease)) offset = 1 - offset;
      
    Point  point;


    if (_flags & YSteps) {
      DbU::Unit delta = DbU::getPolygonStep();
      if (not (_flags & Polygon::YIncrease)) delta  = -delta;

      point.setX( _steps[ (i+offset)/2 ] );
      point.setY( _xyOrigin + delta*((i+1-offset)/2) );
    } else {
      DbU::Unit delta = DbU::getPolygonStep();
      if (not (_flags & Polygon::XIncrease)) delta  = -delta;

      point.setX( _xyOrigin + delta*((i+offset)/2) );
      point.setY( _steps[ (i+1-offset)/2 ] );
    }

    return point;
  }


  void  Polygon::Edge::translate ( DbU::Unit dx, DbU::Unit dy )
  {
    if (isXSteps()) {
      _xyOrigin += dx;
      for ( DbU::Unit& y : _steps ) y += dy;
    } else if (isYSteps()) {
      _xyOrigin += dy;
      for ( DbU::Unit& x : _steps ) x += dx;
    } else if (isHV()) {
      _xyOrigin += dx;
      _steps[0] += dy;
    }
  }


  string  Polygon::Edge::_getTypeName () const
  { return _TName( "Polygon::Edge" ); };


  string Polygon::Edge::_getString() const
  {
    string s = "<Polygon::Edge ";
    if (isYSteps()) s += "YSteps:" + getString(_steps.size());
    if (isXSteps()) s += "XSteps:" + getString(_steps.size());
    if (isHV    ()) s += "HV";
    s += ">";
    return s;
  }


  Record* Polygon::Edge::_getRecord() const
  {
    Record* record = new Record ( getString(this) );
    record->add(           getSlot("_flags"   ,  _flags   ) );
    record->add( DbU::getValueSlot("_xyOrigin", &_xyOrigin) );
    record->add(           getSlot("_steps"   , &_steps   ) );
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Polygon".

  Polygon::Polygon ( Net* net, const Layer* layer, const vector<Point>& points )
    : Super  (net)
    , _layer (layer)
    , _points(points)
    , _edges ()
  { }


  Polygon* Polygon::create ( Net* net, const Layer* layer, const vector<Point>& points )
  {
    if (not layer)
      throw Error("Can't create " + _TName("Polygon") + " : null layer");

    if (points.size() < 3)
      throw Error("Can't create " + _TName("Polygon") + " : less than three points");

    float sign = 0.0;
    for ( size_t i=0 ; i<points.size() ; ++i ) {
      float nextSign = getSign( points, i );
      if ( (sign != 0.0) and ( (sign < 0.0) xor (nextSign < 0.0) ) ) 
        throw Error("Can't create " + _TName("Polygon") + " : non-convex polygon");

      sign = nextSign;
    }

    Polygon* triangle = new Polygon ( net, layer, points );
    triangle->_postCreate();
    triangle->manhattanize();
    return triangle;
  }


  Polygon::~Polygon ()
  {
    for ( Edge* edge : _edges ) delete edge;
  }


  const Layer* Polygon::getLayer () const
  { return _layer; };


  DbU::Unit  Polygon::getX () const
  {
    DbU::Unit center = 0;
    for ( Point p : _points ) center += p.getX();
    return center/_points.size();
  }


  DbU::Unit  Polygon::getY () const
  {
    DbU::Unit center = 0;
    for ( Point p : _points ) center += p.getY();
    return center/_points.size();
  }


  Box  Polygon::getBoundingBox() const
  {
    DbU::Unit xmin = DbU::Max;
    DbU::Unit ymin = DbU::Max;
    DbU::Unit xmax = DbU::Min;
    DbU::Unit ymax = DbU::Min;

    for ( Point p : _points ) {
      xmin = std::min( xmin, p.getX() );
      ymin = std::min( ymin, p.getY() );
      xmax = std::max( xmax, p.getX() );
      ymax = std::max( ymax, p.getY() );
    }

    return Box( xmin, ymin, xmax, ymax );
  }


  Box  Polygon::getBoundingBox ( const BasicLayer* basicLayer ) const
  {
    if (not _layer->contains(basicLayer)) return Box();
    return getBoundingBox();
  }


  void  Polygon::translate ( const DbU::Unit& dx, const DbU::Unit& dy )
  {
    if ((dx != 0) or (dy != 0)) {
      invalidate( true );
      for ( Point& p : _points ) p .translate( dx, dy );
      for ( Edge*  e : _edges  ) e->translate( dx, dy );
    }
  }


  void  Polygon::setLayer ( const Layer* layer )
  {
    if (not layer) throw Error("Can't set layer : null layer");

    if (layer != _layer) {
      invalidate(false);
      _layer = layer;
    }
  }


  void  Polygon::setPoints ( const vector<Point>& points )
  {
    invalidate( true );

    vector<Point> emptyVector;
    _points.swap( emptyVector );
    _points.reserve( points.size() );
    _points = points;

    if (isManhattanized()) {
      for ( Edge* edge : _edges ) delete edge;
      _edges.clear();

      manhattanize();
    }
  }


  float  Polygon::getSign ( const vector<Point>& points, size_t i )
  {
    size_t i1 = (i+1) % points.size();
    size_t i2 = (i+2) % points.size();

    float dx1 = (float)(points[i1].getX() - points[i ].getX());
    float dy1 = (float)(points[i1].getY() - points[i ].getY());
    float dx2 = (float)(points[i2].getX() - points[i1].getX());
    float dy2 = (float)(points[i2].getY() - points[i1].getY());
    return dx1*dy2 - dx2*dy1;
  }


  float  Polygon::getSlope ( const Point& origin, const Point& extremity )
  {
    float dx1 = (float)(extremity.getX() - origin.getX());
    float dy1 = (float)(extremity.getY() - origin.getY());
    return dy1 / dx1;
  }


  float  Polygon::getSlope ( size_t i ) const
  {
    return getSlope( _points[ i % _points.size() ], _points[ (i+1) % _points.size() ] );
  }


  void  Polygon::manhattanize ()
  {
    for ( Edge* edge : _edges ) delete edge;
    _edges.clear();

    bool clockwise = (getSign(_points,0) <= 0);

    for ( size_t i=0 ; i<_points.size() ; ++i ) {
      const Point&    origin     = _points[  i    % _points.size()];
      const Point&    extremity  = _points[ (i+1) % _points.size()];
            bool      dxPositive = (extremity.getX() > origin.getX());
            uint32_t  flags      = (clockwise xor dxPositive) ? 0 : Above;
                      flags     |= (clockwise) ? Clockwise : 0;

      _edges.push_back( new Edge ( origin, extremity, flags ) );
    }
  }


  void Polygon::_toJson ( JsonWriter* writer ) const
  {
    Super::_toJson( writer );

    jsonWrite( writer, "_layer",  _layer->getName() );
    for ( size_t i=0 ; i<_points.size() ; ++i ) {
      string label = "_points[" + getString(i) + "]";
      jsonWrite( writer, label, &_points[i] );
    }
  }


  string  Polygon::_getTypeName () const
  { return _TName( "Polygon" ); };


  string Polygon::_getString() const
  {
    string s = Super::_getString();
    s.insert( s.length() - 1, " " + getString(_layer->getName()));
    s.insert( s.length() - 1, " points:" + getString(_points.size()));
    return s;
  }


  Record* Polygon::_getRecord() const
  {
    Record* record = Inherit::_getRecord();
    if (record) {
      record->add( getSlot("_layer" ,  _layer ) );
      record->add( getSlot("_points", &_points) );
      record->add( getSlot("_edges" , &_edges ) );
    }
    return record;
  }


  Initializer<JsonPolygon>  jsonPolygonInit ( 0 );


  void  JsonPolygon::initialize ()
  { JsonTypes::registerType( new JsonPolygon (JsonWriter::RegisterMode) ); }


  JsonPolygon::JsonPolygon ( unsigned long flags )
    : JsonComponent(flags)
  {
    add( "_layer"   , typeid(string) );
  // THIS IS BUGGY !
    add( "_point[0]", typeid(Point)  );
    add( "_point[1]", typeid(Point)  );
    add( "_point[2]", typeid(Point)  );
  }


  string JsonPolygon::getTypeName () const
  { return "Polygon"; }


  JsonPolygon* JsonPolygon::clone ( unsigned long flags ) const
  { return new JsonPolygon ( flags ); }


  void JsonPolygon::toData ( JsonStack& stack )
  {
    check( stack, "JsonPolygon::toData" );
    unsigned int jsonId = presetId( stack );

    vector<Point> points;
    points.push_back( get<Point>(stack,".Point") );
    points.push_back( get<Point>(stack,".Point") );
    points.push_back( get<Point>(stack,".Point") );

    Polygon* triangle = Polygon::create
      ( get<Net*>(stack,".Net")
      , DataBase::getDB()->getTechnology()->getLayer( get<const char*>(stack,"_layer") )
      , points
      );
  
    JsonNet* jnet = jget<JsonNet>( stack );
    if (jnet) {
      jnet->addHookLink( triangle->getBodyHook(), jsonId, get<string>(stack,"_bodyHook" ) );
    } else {
      cerr << Error( "JsonPolygon::toData(): Missing (Json)Net in stack context." ) << endl;
    }

  // Hook/Ring rebuild are done as a post-process.
    update( stack, triangle );
}


// -------------------------------------------------------------------
// Class  :  "Polygon::Points_Manhattan".

  Polygon::Points_Manhattan::Locator::Locator ( const Polygon* polygon )
    : PointHL ()
    , _polygon(polygon)
    , _iEdge  (0)
    , _iPoint (0)
  { }


  PointHL* Polygon::Points_Manhattan::Locator::getClone () const
  { return new Locator(*this); }


  Point  Polygon::Points_Manhattan::Locator::getElement () const
  { return _polygon->getEdges()[_iEdge]->getPoint(_iPoint); }


  bool  Polygon::Points_Manhattan::Locator::isValid () const
  { return (_iEdge < _polygon->getEdges().size()); }


  void  Polygon::Points_Manhattan::Locator::progress ()
  {
    if (isValid()) {
      if (_iPoint + 1 >= _polygon->getEdges()[_iEdge]->getSize()) {
        ++_iEdge;
        _iPoint = 0;
      } else {
        ++_iPoint;
      }
    }
  }


  string  Polygon::Points_Manhattan::Locator::_getString () const
  {
    string s = "<" + _TName("Points_Manhattan::Locator")
                   + getString(getElement())
                   + ">";
    return s;
  }


  PointHC* Polygon::Points_Manhattan::getClone () const
  { return new Points_Manhattan(*this); }


  PointHL* Polygon::Points_Manhattan::getLocator () const
  { return new Locator(_polygon); }


  string  Polygon::Points_Manhattan::_getString () const
  {
    string s = "<" + _TName("Points_Manhattan") + " "
                   + getString(_polygon)
                   + ">";
    return s;
  }


} // Hurricane namespace.
