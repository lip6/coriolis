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
#include "hurricane/UpdateSession.h"
#include "hurricane/Breakpoint.h"


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
          if (not isXIncrease()) y += DbU::getPolygonStep() - m;
          else                   y += -m;
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
          if (isYIncrease()) x += DbU::getPolygonStep() - m;
          else               x += -m;
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
    , _flags (Convex)
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

    Polygon* polygon = new Polygon ( net, layer, points );
    normalize( polygon->_points, polygon->_flags );
    polygon->_postCreate();
    polygon->manhattanize();
    return polygon;
  }


  void  Polygon::normalize ( vector<Point>& points, uint32_t& flags )
  {
    vector<Point> rawPoints ( points );
    flags = Convex|Polygon45;

    float sign = 0.0;
    for ( size_t i=0 ; i<rawPoints.size() ; ++i ) {
      float nextSign = getSign( rawPoints, i );
      if ( (sign != 0.0) and ( (sign < 0.0) xor (nextSign < 0.0) ) ) {
      //cerr << Error("Can't create " + _TName("Polygon") + " : non-convex polygon.") << endl;
        flags &= ~Convex; 
        break;
      }
      sign = nextSign;
    }

    for ( size_t i=0 ; i<rawPoints.size() ; ++i ) {
      if (not isEdge45(rawPoints,i)) {
        flags &= ~Polygon45;
        break;
      }
    }

    size_t istart = 0;
    for ( size_t i=0 ; i<rawPoints.size() ; ++i ) {
      if (      (rawPoints[i].getX() >  rawPoints[istart].getX())
         or (   (rawPoints[i].getX() == rawPoints[istart].getX())
            and (rawPoints[i].getY() >  rawPoints[istart].getY()) ))
        istart = i;
    }

    if ( (istart != 0) or (sign > 0.0) ) {
      if (sign < 0.0) {
        for ( size_t i=0 ; i<rawPoints.size() ; ++i )
          points[ (rawPoints.size()-i) % rawPoints.size() ] = rawPoints[ (istart+i) % rawPoints.size() ];
      } else {
        for ( size_t i=0 ; i<rawPoints.size() ; ++i )
          points[ i ] = rawPoints[ (istart+i) % points.size() ];
      }
    }
  }


  Polygon::~Polygon ()
  {
    for ( Edge* edge : _edges ) delete edge;
  }


  bool  Polygon::isConvex () const
  { return (_flags & Convex); }


  bool  Polygon::isPolygon45 () const
  { return (_flags & Polygon45); }


  bool  Polygon::isNonRectangle () const
  { return true; }


  bool  Polygon::isManhattanized () const
  { return not _edges.empty(); }


  const Layer* Polygon::getLayer () const
  { return _layer; };


  size_t  Polygon::getPointsSize () const
  { return _points.size(); }


  Point  Polygon::getPoint ( size_t i ) const
  { return _points[ i % _points.size() ]; }


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

  
  Points  Polygon::getMContour () const
  { return Points_Manhattan(this); }


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
    normalize( _points, _flags );

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


  bool  Polygon::isEdge45 ( const vector<Point>& points, size_t i )
  {
    size_t i1 = (i+1) % points.size();

    DbU::Unit dx = std::abs( points[i1].getX() - points[i ].getX() );
    DbU::Unit dy = std::abs( points[i1].getY() - points[i ].getY() );
    // cerr << "| dx=" << dx << " dy=" << dy << "("
    //      << DbU::getValueString(dx) << "," << DbU::getValueString(dy) << ")" << endl;
    return (dx == 0) or (dy == 0) or (dx == dy);
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

    if (isPolygon45()) return;

    for ( size_t i=0 ; i<_points.size() ; ++i ) {
      const Point&    origin     = _points[  i    % _points.size()];
      const Point&    extremity  = _points[ (i+1) % _points.size()];
            bool      dxPositive = (extremity.getX() > origin.getX());
            uint32_t  flags      = (dxPositive) ? 0 : Above;

      _edges.push_back( new Edge ( origin, extremity, flags ) );
    }
  }

  
  void  Polygon::getSubPolygons ( vector< vector<Point> >& subpolygons ) const
  {
    static const size_t subPolygonSize = 1000;
    
    // cerr << "Polygon::getSubPolygons(): " << this << endl;

    if (isPolygon45()) return;

    vector<Point> upSide;
    vector<Point> downSide;
    Point         first;

    bool firstToDown = false;
    bool inUpSide    = true;
    for ( Point p : getMContour() ) {
      // cerr << "| " << p << endl;

      if (upSide.size() == 1) {
        if (upSide[0].getX() == p.getX()) {
          if (not firstToDown) { firstToDown = true; first = upSide[0]; }
          upSide.pop_back();
        }
      }
          
      if (inUpSide and not upSide.empty()) {
        if (upSide.back().getX() < p.getX()) {
          inUpSide = false;
          // cerr << "Switch to down side." << endl;
        }
      }

      if (inUpSide) {
        size_t length = upSide.size();
        if (        (length > 1)
           and (  ( (upSide[length-2].getX() == upSide[length-1].getX()) and (upSide.back().getX() == p.getX()) )
               or ( (upSide[length-2].getY() == upSide[length-1].getY()) and (upSide.back().getY() == p.getY()) ) ) ) {
          upSide.pop_back();
        } 
        upSide.push_back( p );
      } else {
        size_t length = downSide.size();
        if (        (length > 1)
           and (  ( (downSide[length-2].getX() == downSide[length-1].getX()) and (downSide.back().getX() == p.getX()) )
               or ( (downSide[length-2].getY() == downSide[length-1].getY()) and (downSide.back().getY() == p.getY()) ) ) ) {
          downSide.pop_back();
        }

        if (downSide.empty()) {
          downSide.push_back( upSide.back() );
          upSide.pop_back();
        }
        downSide.push_back( p );
      }
    }

    std::reverse( upSide.begin(), upSide.end() );
    
    // cerr << "firstToDown:" << firstToDown << endl;
    if (firstToDown and (first.getX() > downSide.back().getX())) downSide.push_back( first );
    while ( downSide.size() > 1 ) {
      size_t length = downSide.size()-1;
      if (downSide[length-1].getX() != downSide[length].getX()) break;
      downSide.pop_back();
    }

    // cerr << "Up side" << endl;
    // for ( size_t i=0 ; i<upSide.size() ; ++i )
    //   cerr << "[ " << setw(3) << i << "] " << upSide[i] << endl;

    // cerr << "Down side" << endl;
    // for ( size_t i=0 ; i<downSide.size() ; ++i )
    //   cerr << "[ " << setw(3) << i << "] " << downSide[i] << endl;

    vector<Point>* polygon   = NULL;
    size_t         startUp   = 0;
    size_t         startDown = 0;
    size_t         stopUp    = 1;
    size_t         stopDown  = 1;

    while ( true ) {
      size_t leftCutUp    = 0;
      size_t leftCutDown  = 0;
      size_t rightCutUp   = 0;
      size_t rightCutDown = 0;
      size_t downIncrease = 0;
      size_t upDecrease   = 0;

      if (stopUp - startUp + stopDown - startDown > subPolygonSize) {
      // carve out a new sub-polygon->
        // cerr << "New sub polygon:" << endl;
        // cerr << "    stopUp:" << setw(3) << stopUp   << "   startUp:" << setw(3) << startUp   << endl;
        // cerr << "  stopDown:" << setw(3) << stopDown << " startDown:" << setw(3) << startDown << endl;
        
        subpolygons.push_back( vector<Point>() );
        polygon = &subpolygons.back();

        if (upSide[startUp].getX() > downSide[startDown].getX()) {
          polygon->push_back( Point( downSide[startDown].getX(), upSide[startUp].getY() ) );
          // cerr << "  + " << polygon->back() << endl;
          leftCutUp = 1;
        } else {
          if (upSide[startUp].getX() < downSide[startDown].getX()) {
            polygon->push_back( Point( upSide[startUp].getX(), downSide[startDown].getY() ) );
            // cerr << "  + " << polygon->back() << endl;
            leftCutDown = 1;
          }
        }

        if      (upSide[stopUp].getX() > downSide[stopDown].getX()) rightCutUp   = 1;
        else if (upSide[stopUp].getX() < downSide[stopDown].getX()) rightCutDown = 1;

        if (not leftCutDown and (downSide[startDown+1].getY() > downSide[startDown].getY())) downIncrease = 1;
        
        for ( size_t i=startDown+downIncrease ; i<=stopDown-rightCutDown ; ++i ) {
          polygon->push_back( downSide[i] );
          // cerr << "  > " << polygon->back() << endl;
        }

        if (rightCutUp) {
          polygon->push_back( Point( downSide[stopDown].getX(), upSide[stopUp].getY() ) );
          // cerr << "  + " << polygon->back() << endl;
        } else {
          if (rightCutDown) {
            polygon->push_back( Point( upSide[stopUp].getX(), downSide[stopDown].getY() ) );
            // cerr << "  + " << polygon->back() << endl;
            // cerr << "     Adjust down (cut)" << endl;
          }
        }
        
        if (not leftCutUp and (upSide[startUp+1].getY() < upSide[startUp].getY())) upDecrease = 1;

        for ( size_t i=rightCutUp ; i<=stopUp-startUp-upDecrease ; ++i ) {
          polygon->push_back( upSide[stopUp-i] );
          // cerr << "  < " << polygon->back() << endl;
        }

        startUp   = stopUp;
        startDown = stopDown;

        // cerr << "Done sub polygon." << endl;
      }

      if (upSide[stopUp].getX() == downSide[stopDown].getX()) {
        stopUp   += 2;
        stopDown += 2;
      } else {
        if (upSide[stopUp].getX() < downSide[stopDown].getX()) stopUp   += 2;
        else                                                   stopDown += 2;
      }
      stopUp   = std::min( stopUp  ,   upSide.size()-1 );
      stopDown = std::min( stopDown, downSide.size()-1 );

      // cerr << "  stopUp:" << stopUp << " stopDown:" << stopDown << endl;

      if ( (stopUp+5 > upSide.size()) and (stopDown+5 > downSide.size()) ) {
        stopUp   =   upSide.size() - 1;
        stopDown = downSide.size() - 1;
        break;
      }
    }

    subpolygons.push_back( vector<Point>() );
    polygon = &subpolygons.back();

    // cerr << "Last (right) sub polygon:" << endl;
    // cerr << "    stopUp:" << setw(3) << stopUp   << "   startUp:" << setw(3) << startUp   << endl;
    // cerr << "  stopDown:" << setw(3) << stopDown << " startDown:" << setw(3) << startDown << endl;

    size_t downIncrease = 0;
    size_t upDecrease   = 0;
    size_t leftCutUp    = 0;
    size_t leftCutDown  = 0;

    if (upSide[startUp].getX() > downSide[startDown].getX()) {
      polygon->push_back( Point( downSide[startDown].getX(), upSide[startUp].getY() ) );
      // cerr << "  + " << polygon->back() << endl;
      leftCutUp = 1;
    } else {
      if (upSide[startUp].getX() < downSide[startDown].getX()) {
        polygon->push_back( Point( upSide[startUp].getX(), downSide[startDown].getY() ) );
        // cerr << "  + " << polygon->back() << endl;
        leftCutDown = 1;
      }
    }

    if (not leftCutDown and (downSide[startDown+1].getY() > downSide[startDown].getY())) downIncrease = 1;

    for ( size_t i=startDown+downIncrease ; i<=stopDown ; ++i ) {
      polygon->push_back( downSide[i] );
      // cerr << "  > " << polygon->back() << endl;
    }
        
    if (not leftCutUp and (upSide[startUp+1].getY() < upSide[startUp].getY())) upDecrease = 1;
        
    for ( size_t i=0 ; i<=stopUp-startUp-upDecrease ; ++i ) {
      polygon->push_back( upSide[stopUp-i] );
      // cerr << "  < " << polygon->back() << endl;
    }
    
    // cerr << endl;
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
