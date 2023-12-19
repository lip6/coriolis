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
#include "hurricane/DebugSession.h"


namespace {

  using namespace std;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Box;
  using Hurricane::Interval;
  using Hurricane::Rectilinear;
  using Hurricane::DebugSession;

  
// -------------------------------------------------------------------
// Class  :  "SweepInterval".


  class SweepInterval {
    public:
      inline                 SweepInterval    ();
      inline                 SweepInterval    ( const SweepInterval& );
      inline                 SweepInterval    ( DbU::Unit vmin , DbU::Unit vmax, DbU::Unit xmin  );
      inline                 SweepInterval    ( Interval&, DbU::Unit xmin );
      inline bool            isEmpty          () const;
      inline bool            isKeep           () const;
      inline bool            isNewLeft        () const;
      inline bool            isExactMatch     () const;
      inline bool            isSplinter       () const;
      inline bool            isPonctual       () const;
      inline uint32_t        getId            () const;
      inline DbU::Unit       getVMin          () const;
      inline DbU::Unit       getVMax          () const;
      inline DbU::Unit       getSize          () const;
      inline const Interval& getSpan          () const;
      inline void            inflate          ( DbU::Unit dvMin, DbU::Unit dvMax );
      inline void            updateSpan       ( const Interval& );
      inline void            merge            ( DbU::Unit v );
      inline DbU::Unit       getXMin          () const;
      inline void            setXMin          ( DbU::Unit );
      inline void            setKeep          ();
      inline void            setNewLeft       ();
      inline void            setExactMatch    ();
      inline void            setSplinter      ();
      inline void            unsetNewLeft     ();
      inline void            unsetSplinter    ();
      inline void            clearUpdateFlags ();
      inline string          _getString       () const;
      inline bool            operator<        ( const SweepInterval& ) const;
    private:
      const  uint32_t  NOFLAGS     = 0;
      const  uint32_t  UPDATED     = (1<<0);
      const  uint32_t  CARVED      = (1<<1);
      const  uint32_t  ENLARGED    = (1<<2);
      const  uint32_t  EMPTY       = (1<<3);
      const  uint32_t  NEW_LEFT    = (1<<4);
      const  uint32_t  KEEP        = (1<<5);
      const  uint32_t  EXACT_MATCH = (1<<6);
      const  uint32_t  SPLINTER    = (1<<7);
      static uint32_t  _idCounter;
    private:
      uint32_t   _id;
      Interval   _span;
      Interval   _nextSpan;
      DbU::Unit  _xMin;
      uint32_t   _flags;
  };


  uint32_t  SweepInterval::_idCounter = 0;

  
  inline  SweepInterval::SweepInterval ()
    : _id   (_idCounter++)
    , _span ()
    , _xMin (0)
    , _flags(EMPTY|NEW_LEFT)
  { }

  
  inline  SweepInterval::SweepInterval ( DbU::Unit vmin , DbU::Unit vmax, DbU::Unit xmin )
    : _id   (_idCounter++)
    , _span (vmin,vmax)
    , _xMin (xmin)
    , _flags(NOFLAGS|NEW_LEFT)
  { }


  inline  SweepInterval::SweepInterval ( Interval& base, DbU::Unit xmin )
    : _id   (_idCounter++)
    , _span (base)
    , _xMin (xmin)
    , _flags(NEW_LEFT)
  { }
  

  inline  SweepInterval::SweepInterval ( const SweepInterval& other )
    : _id   (_idCounter++)
    , _span (other._span)
    , _xMin (other._xMin)
    , _flags(other._flags)
  { }


  inline bool  SweepInterval::operator< ( const SweepInterval& o ) const
  {
    if (_span.getVMin() != o._span.getVMin()) return _span.getVMin() < o._span.getVMin();
    if (_span.getVMax() != o._span.getVMax()) return _span.getVMax() < o._span.getVMax();
    if (_xMin != o._xMin) return _xMin < o._xMin;
    return _id < o._id;
  }


  inline bool            SweepInterval::isEmpty          () const                             { return (_flags & EMPTY); }
  inline bool            SweepInterval::isKeep           () const                             { return (_flags & KEEP); }
  inline bool            SweepInterval::isNewLeft        () const                             { return (_flags & NEW_LEFT); }
  inline bool            SweepInterval::isExactMatch     () const                             { return (_flags & EXACT_MATCH); }
  inline bool            SweepInterval::isSplinter       () const                             { return (_flags & SPLINTER); }
  inline bool            SweepInterval::isPonctual       () const                             { return _span.isPonctual(); }
  inline uint32_t        SweepInterval::getId            () const                             { return _id; }
  inline const Interval& SweepInterval::getSpan          () const                             { return _span; }
  inline DbU::Unit       SweepInterval::getVMin          () const                             { return _span.getVMin(); }
  inline DbU::Unit       SweepInterval::getVMax          () const                             { return _span.getVMax(); }
  inline DbU::Unit       SweepInterval::getSize          () const                             { return _span.getSize(); }
  inline void            SweepInterval::inflate          ( DbU::Unit dvMin, DbU::Unit dvMax ) { _span.inflate(dvMin,dvMax); }
  inline void            SweepInterval::merge            ( DbU::Unit v )                      { _span.merge(v); }
  inline DbU::Unit       SweepInterval::getXMin          () const                             { return _xMin; }
  inline void            SweepInterval::setXMin          ( DbU::Unit xmin )                   { _xMin=xmin; }
  inline void            SweepInterval::setKeep          ()                                   { _flags |= KEEP; }
  inline void            SweepInterval::setNewLeft       ()                                   { _flags |= NEW_LEFT; }
  inline void            SweepInterval::setExactMatch    ()                                   { _flags |= EXACT_MATCH; }
  inline void            SweepInterval::setSplinter      ()                                   { _flags |= SPLINTER; }
  inline void            SweepInterval::unsetNewLeft     ()                                   { _flags &= ~NEW_LEFT; }
  inline void            SweepInterval::unsetSplinter    ()                                   { _flags &= ~SPLINTER; }
  inline void            SweepInterval::clearUpdateFlags ()                                   { _flags &= ~(KEEP | NEW_LEFT | EXACT_MATCH); }
  inline void            SweepInterval::updateSpan       ( const Interval& v )                { _span.intersection(v); }
  

  inline string  SweepInterval::_getString () const
  {
    string s;
    s += "<SI @"  + DbU::getValueString(_xMin);
    s += " [" + DbU::getValueString(_span.getVMin());
    s += " "  + DbU::getValueString(_span.getVMax()) + "]>";
    return s;
  }


}


GETSTRING_VALUE_SUPPORT(::SweepInterval);


namespace {

  
// -------------------------------------------------------------------
// Class  :  "SweepLine".

  
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
    for ( size_t i=0 ; i<points.size()-1 ; ++i ) {
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

  
// -------------------------------------------------------------------
// Class  :  "SweepLineBig".

  
  class SweepLineBig {
    public:
            SweepLineBig   ( const Rectilinear*
                           , vector<Box>&
                           , DbU::Unit xThreshold
                           , DbU::Unit yThreshold );
           ~SweepLineBig   ();
      void  addVEdge       ( DbU::Unit ymin, DbU::Unit ymax, DbU::Unit x );
      void  loadVEdges     ();
      void  updateLeftSide ();
      void  process        ( Interval );
      void  process        ( const pair< DbU::Unit, list<Interval> >& );
      void  flushBoxes     ( const Interval& );
      void  toBox          ( SweepInterval& );
      void  asRectangles   ();
    private:
      const Rectilinear*                         _rectilinear;
      vector<Box>&                               _boxes;
      list< pair< DbU::Unit, list<Interval> > >  _vedges;
      list< SweepInterval >                      _sweepLine;
      list< SweepInterval >                      _leftSides;
      DbU::Unit                                  _currX;
      DbU::Unit                                  _xThreshold;
      DbU::Unit                                  _yThreshold;
  };


  SweepLineBig::SweepLineBig ( const Rectilinear* r
                             , vector<Box>&       boxes
                             , DbU::Unit          xThreshold
                             , DbU::Unit          yThreshold )
    : _rectilinear(r)
    , _boxes      (boxes)
    , _vedges     ()
    , _sweepLine  ()
    , _leftSides  ()
    , _currX      (0)
    , _xThreshold (xThreshold)
    , _yThreshold (yThreshold)
  {
    cdebug_log(17,1) << "SweepLineBig::SweepLineBig()" << endl;
    cdebug_log(17,0) << "_xThreshold=" << DbU::getValueString(_xThreshold) << endl;
    cdebug_log(17,0) << "_yThreshold=" << DbU::getValueString(_yThreshold) << endl;
  }

  
  SweepLineBig::~SweepLineBig ()
  {
    cdebug_tabw(17,-1);
  }


  void  SweepLineBig::addVEdge ( DbU::Unit ymin, DbU::Unit ymax, DbU::Unit x )
  {
    if (ymin > ymax) std::swap( ymin, ymax );

    cdebug_log(17,1) << "SweepLineBig::addVEdge() @"<< DbU::getValueString(x)
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


  void  SweepLineBig::loadVEdges ()
  {
    const vector<Point>& points = _rectilinear->getPoints();
    for ( size_t i=0 ; i<points.size()-1 ; ++i ) {
      const Point& source = points[  i ];
      const Point& target = points[ (i+1) % points.size() ];
      DbU::Unit dx = target.getX() - source.getX();
    //DbU::Unit dy = target.getY() - source.getY();
      if (dx == 0) {
        addVEdge( source.getY(), target.getY(), source.getX() );
      }
    }
  }


  void  SweepLineBig::toBox ( SweepInterval& intv )
  {
    if (intv.getXMin() == _currX) return;
    if ((_xThreshold > 0) and (_currX - intv.getXMin() < _xThreshold)) return;
    if ((_yThreshold > 0) and (intv.getSize()          < _yThreshold)) return;

    _boxes.push_back( Box( intv.getXMin(), intv.getVMin()
                         , _currX        , intv.getVMax() ));
    cdebug_log(17,0) << "  toBox() " << _boxes.back() << endl;
  }

  
  void  SweepLineBig::updateLeftSide ()
  {
    cdebug_log(17,1) << "SweepLineBig::updateLeftSide() @" << DbU::getValueString(_currX) << endl;
    for ( auto isweep = _sweepLine.begin() ; isweep != _sweepLine.end() ; ++isweep ) {
      cdebug_log(17,1) << "Sweep " << *isweep << endl;
      DbU::Unit leftX = _currX;
      for ( auto ileft = _leftSides.begin() ; (ileft != _leftSides.end()) ; ++ileft ) {
        cdebug_log(17,0) << "| Left side " << *ileft << endl;
        if (ileft->isNewLeft()) {
          ileft->unsetNewLeft();
          continue;
        }
        if (isweep->getSpan() == ileft->getSpan()) {
          cdebug_log(17,0) << "> Exact match" << endl;
          isweep->setExactMatch();
          ileft ->setKeep();
          continue;
        }
        if (isweep->getSpan().contains( ileft->getSpan() )) {
          cdebug_log(17,0) << "> Contained" << endl;
          ileft->setKeep();
          continue;
        }
        if (ileft->getSpan().contains( isweep->getSpan() )) {
          cdebug_log(17,0) << "> Full shrink" << endl;
          if (not ileft->isSplinter()) toBox( *ileft );
          ileft->setKeep();
          _leftSides.push_back( SweepInterval( isweep->getVMax(), ileft->getVMax(), ileft->getXMin() ) );
          _leftSides.back().setSplinter();
          cdebug_log(17,0) << "+ Add remainder " << _leftSides.back() << endl;
          ileft->updateSpan( isweep->getSpan() );
          ileft->unsetSplinter();
          isweep->setExactMatch();
          continue;
        }
        if (isweep->getSpan().intersect( ileft->getSpan() )) {
          cdebug_log(17,0) << "> Intersect" << endl;
          toBox( *ileft );
          ileft->updateSpan( isweep->getSpan() );
          ileft->unsetSplinter();
          if (not ileft->isPonctual()) ileft->setKeep();
          cdebug_log(17,0) << "> Shrinked span " << *ileft << endl;
          continue;
        }
      }

      if (not isweep->isExactMatch()) {
        _leftSides.push_back( SweepInterval( *isweep ));
        _leftSides.back().setXMin( leftX );
        _leftSides.back().setKeep();
        cdebug_log(17,0) << "+ Added span " << _leftSides.back() << endl;
      }
      cdebug_tabw(17,-1);
    }

    cdebug_log(17,0) << "Clean sweepline flags" << endl;
    for ( auto isweep = _sweepLine.begin() ; isweep != _sweepLine.end() ; ++isweep ) {
      isweep->clearUpdateFlags();
    }
    _leftSides.sort();
    cdebug_log(17,0) << "Delete closed left sides & clean flags" << endl;
    for ( auto ileft = _leftSides.begin() ; ileft != _leftSides.end() ; ) {
      cdebug_log(17,0) << "| " << *ileft << endl;
      auto ileftNext = ileft;
      ileftNext++;
      if (ileftNext != _leftSides.end()) {
        if (   (ileft->getSpan().getVMin() == ileftNext->getSpan().getVMin())
           and (ileft->getSpan().getVMax() == ileftNext->getSpan().getVMax())) {
          cdebug_log(17,0) << "> Remove duplicate " << *ileftNext << endl;
          _leftSides.erase( ileftNext );
          continue;
        }
      }
      if (not ileft->isKeep()) {
        cdebug_log(17,0) << "> Remove/close " << *ileft << endl;
        auto ileftNext = ileft;
        ++ileftNext;
        if (not ileft->isSplinter()) {
          toBox( *ileft );
        }
        _leftSides.erase( ileft );
        ileft = ileftNext;
        continue;
      }
      ileft->clearUpdateFlags();
      ileft++;
    }
    cdebug_tabw(17,-1);
  }
  

  void  SweepLineBig::process ( Interval v )
  {
    cdebug_log(17,1) << "SweepLineBig::process(Interval&) "
                     << " [" << DbU::getValueString(v.getVMin())
                     << " "  << DbU::getValueString(v.getVMax()) << "]"  << endl;
    bool done = false;
    for ( auto iintv = _sweepLine.begin() ; iintv != _sweepLine.end() ; ++iintv ) {
      cdebug_log(17,0) << "> Sweepline element " << *iintv << endl;
    // Extractor p. 9 (a).
      if (v.getVMax() < iintv->getVMin()) {
        cdebug_log(17,0) << "case (a): new left side" << endl;
        _sweepLine.insert( iintv, SweepInterval(v,_currX) );
        done = true;
        break;
      }
    // Extractor p. 9 (f).
      if (   (v.getVMin() == iintv->getVMin()) 
         and (v.getVMax() == iintv->getVMax()) ) {
        cdebug_log(17,0) << "case (f): closing right side" << endl;
        _sweepLine.erase( iintv );
        done = true;
        break;
      }
    // Extractor p. 9 (b).
      if (v.getVMax() == iintv->getVMin()) {
        cdebug_log(17,0) << "case (b): extend min" << endl;
        iintv->merge( v.getVMin() );
        done = true;
        break;
      }
    // Extractor p. 9 (g).
      if (v.getVMax() == iintv->getVMax()) {
        cdebug_log(17,0) << "case (g): carve" << endl;
        iintv->inflate( 0, v.getVMin() - iintv->getVMax() );
        cdebug_log(17,0) << "| " << (*iintv) << endl; 
        done = true;
        break;
      }
    // Extractor p. 9 (h).
      if (v.getVMin() == iintv->getVMin()) {
        cdebug_log(17,0) << "case (h): carve" << endl;
        iintv->inflate(iintv->getVMin() - v.getVMax(), 0 );
        done = true;
        break;
      }
    // Extractor p. 9 (c).
      if (   (v.getVMin() > iintv->getVMin()) 
         and (v.getVMax() < iintv->getVMax()) ) {
        cdebug_log(17,0) << "case (c): carve" << endl;
        DbU::Unit wholeVMin = iintv->getVMin();
        iintv->inflate( iintv->getVMin() - v.getVMax(), 0 );
        cdebug_log(17,0) << "| " << (*iintv) << endl; 
        _sweepLine.insert( iintv, SweepInterval( wholeVMin, v.getVMin(), _currX ) );
        cdebug_log(17,0) << "| " << (*iintv) << endl; 
        done = true;
        break;
      }
    // Extractor p. 9 (d,e).
      if (v.getVMin() == iintv->getVMax()) {
        cdebug_log(17,0) << "case (d,e): merge" << endl;
        auto iintvNext = iintv;
        ++iintvNext;
      // Extractor p. 9 (d).
        if (iintvNext == _sweepLine.end()) {
          cdebug_log(17,0) << "case (d): end of vertical line" << endl;
          iintv->merge( v.getVMax() );
          iintv->setXMin( _currX );
        } else {
        // Extractor p. 9 (d).
          if (v.getVMax() < iintvNext->getVMin()) {
            cdebug_log(17,0) << "case (d): extend max" << endl;
            iintv->merge( v.getVMax() );
            iintv->setXMin( _currX );
          } else {
          // Extractor p. 9 (e).
            cdebug_log(17,0) << "case (e): interval joining" << endl;
            iintv->merge( iintvNext->getVMax() );
            iintv->setXMin( _currX );
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


  void  SweepLineBig::process ( const pair< DbU::Unit, list<Interval> >& intervals )
  {
    cdebug_log(17,1) << "SweepLineBig::process() @"<< DbU::getValueString(intervals.first)
                     << " size=" << intervals.second.size() << endl;
    _currX = intervals.first;
    for ( const Interval& v : intervals.second ) process( v );
    updateLeftSide();
    cdebug_tabw(17,-1);
  }


  void  SweepLineBig::asRectangles ()
  {
    loadVEdges();
    for ( auto intervals : _vedges ) {
      process( intervals );
    }
    cdebug_log(17,0) << "SweepLineBig::asRectangles() size=" << _boxes.size() << endl;
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

    if (points.size() < 4)
      throw Error( "Rectilinear::create(): Rectilinear polygons must at least contains 3 vertexes." );

    if (points.size() > 1000)
      throw Error( "Rectilinear::create(): Rectilinear polygons must not exceed 1000 vertexes." );

    if (points[0] != points[points.size()-1])
      throw Error( "Rectilinear::create(): First and last point must be the same.\n"
                   "0:%s %d:%s"
                 , getString(points[0]).c_str()
                 , points.size()-1
                 , getString(points[points.size()-1]).c_str()
                 );

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


  bool  Rectilinear::getAsBiggestRectangles ( std::vector<Box>& rectangles
                                            , DbU::Unit         xThreshold
                                            , DbU::Unit         yThreshold
                                            ) const
  {
  //DebugSession::open( 17, 18 );
    rectangles.clear();
    if (not isRectilinear()) return false;
    SweepLineBig( this, rectangles, xThreshold, yThreshold ).asRectangles();
  //DebugSession::close();
    return true;
  }


  Box  Rectilinear::getNearestHSide ( DbU::Unit y ) const
  {
    Box side;
    for ( size_t i=1 ; i<_points.size() ; ++i ) {
      if (_points[i-1].getY() != _points[i].getY()) continue;
      if (   not side.isEmpty()
         and (std::abs(side.getYCenter() - y) >= std::abs(_points[i].getY() - y))) continue;
      side = Box( _points[i-1], _points[i] );
    }
    return side;
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
