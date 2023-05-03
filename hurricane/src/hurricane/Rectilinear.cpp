// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2018-2018, All Rights Reserved
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
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Rectilinear.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Rectilinear.h"
#include "hurricane/Net.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Layer.h"
#include "hurricane/Cell.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"


namespace {

  using namespace std;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Box;
  using Hurricane::Interval;
  using Hurricane::Rectilinear;


  class SweepInterval : public Interval {
    public:
      inline                SweepInterval ( DbU::Unit vmin , DbU::Unit vmax, DbU::Unit xmin  );
      inline                SweepInterval ( Interval&, DbU::Unit xmin );
      inline SweepInterval& inflate       ( DbU::Unit dvMin, DbU::Unit dvMax );
      inline SweepInterval& merge         ( DbU::Unit v );
      inline DbU::Unit      getXMin       () const;
      inline void           setXMin       ( DbU::Unit );
      inline string         _getString    () const;
    private:
      DbU::Unit  _xMin;
  };

  
  inline  SweepInterval::SweepInterval ( DbU::Unit vmin , DbU::Unit vmax, DbU::Unit xmin )
    : Interval(vmin,vmax)
    , _xMin   (xmin)
  { }
  
  inline  SweepInterval::SweepInterval ( Interval& base, DbU::Unit xmin )
    : Interval(base)
    , _xMin   (xmin)
  { }
  
  inline SweepInterval& SweepInterval::inflate       ( DbU::Unit dvMin, DbU::Unit dvMax ) { Interval::inflate(dvMin,dvMax); return *this; }
  inline SweepInterval& SweepInterval::merge         ( DbU::Unit v )                      { Interval::merge(v); return *this; }
  inline DbU::Unit      SweepInterval::getXMin       () const                             { return _xMin; }
  inline void           SweepInterval::setXMin       ( DbU::Unit xmin )                   { _xMin=xmin; }

  inline string  SweepInterval::_getString () const
  {
    string s;
    s += "@"  + DbU::getValueString(_xMin);
    s += " [" + DbU::getValueString(getVMin());
    s += " "  + DbU::getValueString(getVMax()) + "]";
    return s;
  }


} // Anonymous namespace.


GETSTRING_VALUE_SUPPORT(::SweepInterval);


namespace {

  
  class SweepLine {
    public:
            SweepLine    ( const Rectilinear*, vector<Box>& );
           ~SweepLine    ();
      void  addVEdge     ( DbU::Unit ymin, DbU::Unit ymax, DbU::Unit x );
      void  loadVEdges   ();
      void  process      ( Interval );
      void  process      ( const pair< DbU::Unit, list<Interval> >& );
      void  toBox        ( SweepInterval& );
      void  asRectangles ();
    private:
      const Rectilinear*                         _rectilinear;
      vector<Box>&                               _boxes;
      list< pair< DbU::Unit, list<Interval> > >  _vedges;
      list< SweepInterval >                      _sweepLine;
      DbU::Unit                                  _prevX;
      DbU::Unit                                  _currX;
  };


  SweepLine::SweepLine ( const Rectilinear* r, vector<Box>& boxes )
    : _rectilinear(r)
    , _boxes      (boxes)
    , _vedges     ()
    , _sweepLine  ()
    , _prevX      (0)
    , _currX      (0)
  {
    cdebug_log(17,1) << "SweepLine::SweepLine()" << endl;
  }

  
  SweepLine::~SweepLine ()
  {
    cdebug_tabw(17,-1);
  }


  void  SweepLine::addVEdge ( DbU::Unit ymin, DbU::Unit ymax, DbU::Unit x )
  {
    if (ymin > ymax) std::swap( ymin, ymax );

    cdebug_log(17,1) << "SweepLine::addVEdge() @"<< DbU::getValueString(x)
                     << " [" << DbU::getValueString(ymin)
                     << " "  << DbU::getValueString(ymax) << "]"  << endl;

    bool inserted = false;
    for ( auto ix = _vedges.begin() ; ix != _vedges.end() ; ++ix ) {
      cdebug_log(17,0) << "| Looking @" << DbU::getValueString(ix->first)
                       << " size=" << ix->second.size() << endl;

      if (ix->first > x) {
        _vedges.insert( ix, make_pair( x, list<Interval>() ));
        cdebug_log(17,0) << "+ add new @" << DbU::getValueString(x) << endl;
        --ix;
      }
      if (ix->first == x) {
        for ( auto iintv = ix->second.begin() ; iintv != ix->second.end() ; ++iintv ) {
          if (iintv->getVMin() >= ymax) {
            ix->second.insert( iintv, Interval(ymin,ymax) );
            inserted = true;
            break;
          }
        }
        if (not inserted) {
          ix->second.push_back( Interval(ymin,ymax) );
          inserted = true;
        }
        break;
      }
    }
    if (not inserted) {
      cdebug_log(17,0) << "+ add new (back) @" << DbU::getValueString(x) << endl;
      _vedges.push_back( make_pair( x, list<Interval>() ));
      _vedges.back().second.push_back( Interval(ymin,ymax) );
    }

    cdebug_tabw(17,-1);
  }


  void  SweepLine::loadVEdges ()
  {
    const vector<Point>& points = _rectilinear->getPoints();
    for ( size_t i=0 ; i<points.size() ; ++i ) {
      const Point& source = points[  i ];
      const Point& target = points[ (i+1) % points.size() ];
      DbU::Unit dx = target.getX() - source.getX();
    //DbU::Unit dy = target.getY() - source.getY();
      if (dx == 0) {
        addVEdge( source.getY(), target.getY(), source.getX() );
      }
    }
  }


  void  SweepLine::toBox ( SweepInterval& intv )
  {
    if (intv.getXMin() == _currX) return;
    _boxes.push_back( Box( intv.getXMin(), intv.getVMin()
                         , _currX        , intv.getVMax() ));
    intv.setXMin( _currX );
  }


  void  SweepLine::process ( Interval v )
  {
    cdebug_log(17,1) << "SweepLine::process(Interval&) "
                     << " [" << DbU::getValueString(v.getVMin())
                     << " "  << DbU::getValueString(v.getVMax()) << "]"  << endl;
    bool done = false;
    for ( auto iintv = _sweepLine.begin() ; iintv != _sweepLine.end() ; ++iintv ) {
    // Extractor p. 9 (a).
      if (v.getVMax() < iintv->getVMin()) {
        _sweepLine.insert( iintv, SweepInterval(v,_currX) );
        done = true;
        break;
      }
    // Extractor p. 9 (f).
      if (   (v.getVMin() == iintv->getVMin()) 
         and (v.getVMax() == iintv->getVMax()) ) {
        toBox( *iintv );
        _sweepLine.erase( iintv );
        done = true;
        break;
      }
    // Extractor p. 9 (b).
      if (v.getVMax() == iintv->getVMin()) {
        toBox( *iintv );
        iintv->merge( v.getVMin() );
        done = true;
        break;
      }
    // Extractor p. 9 (g).
      if (v.getVMax() == iintv->getVMax()) {
        toBox( *iintv );
        cdebug_log(17,0) << "case (g): carve" << endl;
        iintv->inflate( 0, v.getVMin() - iintv->getVMax() );
        cdebug_log(17,0) << "| " << (*iintv) << endl; 
        done = true;
        break;
      }
    // Extractor p. 9 (h).
      if (v.getVMin() == iintv->getVMin()) {
        toBox( *iintv );
        iintv->inflate(iintv->getVMin() - v.getVMax(), 0 );
        done = true;
        break;
      }
    // Extractor p. 9 (c).
      if (   (v.getVMin() > iintv->getVMin()) 
         and (v.getVMax() < iintv->getVMax()) ) {
        toBox( *iintv );
        cdebug_log(17,0) << "case (c): carve" << endl;
        DbU::Unit wholeVMin = iintv->getVMin();
        iintv->inflate( iintv->getVMin() - v.getVMax(), 0 );
        cdebug_log(17,0) << "| " << (*iintv) << endl; 
        _sweepLine.insert( iintv, SweepInterval( wholeVMin, v.getVMin(), _currX ) );
        cdebug_log(17,0) << "| " << (*(--iintv)) << endl; 
        done = true;
        break;
      }
    // Extractor p. 9 (d,e).
      if (v.getVMin() == iintv->getVMax()) {
        auto iintvNext = iintv;
        ++iintvNext;
      // Extractor p. 9 (d).
        if (iintvNext == _sweepLine.end()) {
          toBox( *iintv );
          iintv->merge( v.getVMax() );
        } else {
        // Extractor p. 9 (d).
          if (v.getVMax() < iintvNext->getVMin()) {
            toBox( *iintv );
            iintv->merge( v.getVMax() );
          } else {
          // Extractor p. 9 (e).
            toBox( *iintv );
            toBox( *iintvNext );
            iintv->merge( iintvNext->getVMax() );
            _sweepLine.erase( iintvNext );
          }
        }
        done = true;
        break;
      }
    }
    if (not done) {
      _sweepLine.push_back( SweepInterval(v,_currX) );
    }

    cdebug_tabw(17,-1);
  }


  void  SweepLine::process ( const pair< DbU::Unit, list<Interval> >& intervals )
  {
    cdebug_log(17,1) << "SweepLine::process() @"<< DbU::getValueString(intervals.first)
                     << " size=" << intervals.second.size() << endl;
    _currX = intervals.first;
    for ( const Interval& v : intervals.second ) process( v );
    cdebug_tabw(17,-1);
  }


  void  SweepLine::asRectangles ()
  {
    loadVEdges();
    for ( auto intervals : _vedges ) {
      process( intervals );
    }
    cdebug_log(17,0) << "SweepLine::asRectangles() size=" << _boxes.size() << endl;
    for ( const Box& b : _boxes )
      cdebug_log(17,0) << "| " << b << endl;
  }


} // Anonymous namespace.


namespace Hurricane {

  
// -------------------------------------------------------------------
// Class  :  "Rectilinear".

  Rectilinear::Rectilinear ( Net* net, const Layer* layer, const vector<Point>& points )
    :  Super (net)
    , _layer (layer)
    , _points(points)
    , _flags (IsRectilinear)
  { }


  Rectilinear* Rectilinear::create ( Net* net, const Layer* layer, const vector<Point>& points )
  {
    if (not layer)
      throw Error( "Rectilinear::create(): Can't create, NULL layer" );

    if (points.size() > 1000)
      throw Error( "Rectilinear::create(): Rectlinear polygons must not exceed 1000 vertexes." );

    bool       isRect  = true;
    DbU::Unit  oneGrid = DbU::fromGrid( 1.0 );
    for ( size_t i=0 ; i<points.size() ; ++i ) {
      size_t j = (i+1) % points.size();

      DbU::Unit dx = std::abs( points[i].getX() - points[j].getX() );
      DbU::Unit dy = std::abs( points[i].getY() - points[j].getY() );

      if ((dx != 0) and (dy != 0)) {
        isRect = false;
        if (dx != dy)
          throw Error( "Rectilinear::create(): Can't create, non H/V edge (points %d:%s - %d:%s)."
                     , i, getString(points[i]).c_str(), j, getString(points[j]).c_str() );
      }

      if (points[i].getX() % oneGrid)
        cerr << Warning( "Rectilinear::create(): In Cell \"%s\", Net \"%s\",\n"
                         "          Point [%d] = %s, X is off foundry grid (%s)"
                       , getString(net->getCell()->getName()).c_str()
                       , getString(net->getName()).c_str()
                       , i
                       , getString(points[i]).c_str()
                       , DbU::getValueString(oneGrid).c_str() ) << endl;
      if (points[i].getY() % oneGrid)
        cerr << Warning( "Rectilinear::create(): In Cell \"%s\", Net \"%s\",\n"
                         "          Point [%d] = %s, Y is off foundry grid (%s)"
                       , getString(net->getCell()->getName()).c_str()
                       , getString(net->getName()).c_str()
                       , i
                       , getString(points[i]).c_str()
                       , DbU::getValueString(oneGrid).c_str() ) << endl;
    }

    Rectilinear* rectilinear = new Rectilinear ( net, layer, points );
    if (not isRect) rectilinear->_flags &= ~IsRectilinear;
    rectilinear->_postCreate();

    return rectilinear;
  }




  bool         Rectilinear::isNonRectangle () const { return true; }
  const Layer* Rectilinear::getLayer       () const { return _layer; }
  DbU::Unit    Rectilinear::getX           () const { return getBoundingBox().getCenter().getX(); }
  DbU::Unit    Rectilinear::getY           () const { return getBoundingBox().getCenter().getY(); }


  Box  Rectilinear::getBoundingBox() const
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


  Box  Rectilinear::getBoundingBox ( const BasicLayer* basicLayer ) const
  {
    if (not _layer->contains(basicLayer)) return Box();
    return getBoundingBox();
  }


  void  Rectilinear::translate ( const DbU::Unit& dx, const DbU::Unit& dy )
  {
    if ( (dx != 0) or (dy != 0) ) {
      invalidate( true );
      for ( Point& p : _points ) p.translate( dx, dy );
    }
  }


  void  Rectilinear::setLayer ( const Layer* layer )
  {
    if (not layer) throw Error( "Rectilinear::setLayer(): Can't set layer, NULL layer" );

    if (layer != _layer) {
      invalidate( false );
      _layer = layer;
    }
  }


  void  Rectilinear::setPoints ( const vector<Point>& points )
  {
    for ( size_t i=0 ; i<points.size() ; ++i ) {
      size_t j = (i+1) % points.size();

      if (   (points[i].getX() != points[j].getX())
         and (points[i].getY() != points[j].getY()) )
         throw Error( "Rectilinear::create(): Can't create, non H/V edge." );
    }

    _points = points;
    invalidate(true);
  }


  size_t  Rectilinear::getPointsSize () const
  { return _points.size(); }


  Point  Rectilinear::getPoint ( size_t i ) const
  {
    return _points[ i % getPointsSize() ];
  }


  bool  Rectilinear::getAsRectangles ( std::vector<Box>& rectangles ) const
  {
    rectangles.clear();
    if (not isRectilinear()) return false;
    SweepLine( this, rectangles ).asRectangles();
    return true;
  }


  void Rectilinear::_toJson ( JsonWriter* writer ) const
  {
    Inherit::_toJson( writer );

    jsonWrite( writer, "_layer" ,  _layer->getName() );
  }


  string  Rectilinear::_getTypeName () const
  { return _TName( "Rectilinear" ); }


  string  Rectilinear::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.length() - 1, " " + getString(_layer->getName()) );
    s.insert( s.length() - 1, " points:" + getString(_points.size()) );
    return s;
  }


  Record* Rectilinear::_getRecord () const
  {
    Record* record = Super::_getRecord();
    if (record) {
      record->add( getSlot("_layer" ,  _layer ) );
      record->add( getSlot("_points", &_points) );
    }
    return record;
  }

  
// -------------------------------------------------------------------
// Class  :  "JsonRectilinear".


  Initializer<JsonRectilinear>  jsonRectilinearInit ( 0 );


  void  JsonRectilinear::initialize ()
  { JsonTypes::registerType( new JsonRectilinear (JsonWriter::RegisterMode) ); }


  JsonRectilinear::JsonRectilinear ( unsigned long flags )
    : JsonComponent(flags)
  {
    add( "_layer" , typeid(string)   );
  }


  string JsonRectilinear::getTypeName () const
  { return "Rectilinear"; }


  JsonRectilinear* JsonRectilinear::clone ( unsigned long flags ) const
  { return new JsonRectilinear ( flags ); }


  void JsonRectilinear::toData ( JsonStack& stack )
  {
    check( stack, "JsonRectilinear::toData" );
    unsigned int jsonId = presetId( stack );

    vector<Point> points;

  // Have to loop over all the points.
  //points.push_back( get<Point>(stack,".Point") );

    Rectilinear* rectilinear = Rectilinear::create
      ( get<Net*>(stack,".Net")
      , DataBase::getDB()->getTechnology()->getLayer( get<const char*>(stack,"_layer") )
      , points
      );
  
    JsonNet* jnet = jget<JsonNet>( stack );
    if (jnet) {
      jnet->addHookLink( rectilinear->getBodyHook  (), jsonId, get<string>(stack,"_bodyHook"  ) );
    } else {
      cerr << Error( "Jsonrectilinear::toData(): Missing (Json)Net in stack context." ) << endl;
    }

  // Hook/Ring rebuild are done as a post-process.
    update( stack, rectilinear );
}


} // Hurricane namespace.
