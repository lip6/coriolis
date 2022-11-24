
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Tracks.cpp"                             |
// +-----------------------------------------------------------------+


#include <iostream>
#include "kite/Track.h"
#include "kite/Tracks.h"
#include "kite/RoutingPlane.h"


namespace Kite {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;


// -------------------------------------------------------------------
// Class  :  "Tracks_Range::Locator".


  Tracks_Range::Locator::Locator ( const RoutingPlane* routingPlane
                                 , const Interval& constraints
                                 )
    : Hurricane::Locator<Track*>()
    , _constraints (constraints)
  {
    cdebug_log(159,0) << "Tracks_Range::Locator()" << endl;
    cdebug_log(159,0) << "* Constraints: " << _constraints << endl;

    _track = routingPlane->getTrackByPosition ( _constraints.getVMin() );

    if ( _track and (_track->getAxis() < _constraints.getVMin()) ) _track = _track->getNextTrack();
    if ( _track and (_track->getAxis() > _constraints.getVMax()) ) _track = NULL;

    cdebug_log(159,0) << "_track: " << _track << endl;;
  }


  Tracks_Range::Locator::Locator ( const Locator& locator )
    : Hurricane::Locator<Track*>()
    , _constraints(locator._constraints)
    , _track      (locator._track)
  { }


  Hurricane::Locator<Track*>* Tracks_Range::Locator::getClone () const
  { return new Locator(*this); }


  bool  Tracks_Range::Locator::isValid () const
  { return (_track != NULL); }


  Track* Tracks_Range::Locator::getElement () const
  { return _track; }



  void  Tracks_Range::Locator::progress ()
  {
    if ( !_track ) return;

    _track = _track->getNextTrack();
    if ( _track and (_track->getAxis() > _constraints.getVMax()) ) _track = NULL;
  }


  string  Tracks_Range::Locator::_getString () const
  {
    string s = "<Tracks_Range::Locator ["
             + DbU::getValueString(_constraints.getVMin()) + ":"
             + DbU::getValueString(_constraints.getVMax()) + "] "
             + getString(_track) + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Tracks_Range".


  Tracks  Tracks_Range::get ( RoutingPlane* routingPlane
                            , Interval&     constraints )
  { return new Tracks_Range ( routingPlane, constraints ); }


  Tracks_Range::Tracks_Range ( const RoutingPlane* routingPlane
                             , const Interval&     constraints )
    : Collection<Track*>()
    , _routingPlane(routingPlane)
    , _constraints (constraints)
  { }


  Tracks_Range::Tracks_Range ( const Tracks_Range& tracks )
    : Collection<Track*>()
    , _routingPlane(tracks._routingPlane)
    , _constraints (tracks._constraints)
  { }


  Collection<Track*>* Tracks_Range::getClone () const
  { return new Tracks_Range(*this); }


  Hurricane::Locator<Track*>* Tracks_Range::getLocator () const
  { return new Locator(_routingPlane,_constraints); }


  string  Tracks_Range::_getString () const
  {
    string s = "<" + _TName("Tracks_Range") + " "
                   + getString(_routingPlane->getLayer()) + " ["
                   + getString(_constraints.getVMin()) + ":"
                   + getString(_constraints.getVMax()) + "]"
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Tracks_Spiral::Locator".


  Tracks_Spiral::Locator::Locator ( const RoutingPlane* routingPlane
                                  , const Interval& optimal
                                  , const Interval& constraints
                                  )
    : Hurricane::Locator<Track*>()
    , _optimal     (optimal)
    , _constraints (constraints)
    , _onMin       (false)
    , _inMinOptimal(true)
    , _inMaxOptimal(true)
  {
    cdebug_log(159,0) << "Tracks_Spiral::Locator()" << endl;
    cdebug_log(159,0) << "* Optimal:     " << _optimal     << endl;
    cdebug_log(159,0) << "* Constraints: " << _constraints << endl;

    _minTrack = _maxTrack = routingPlane->getTrackByPosition ( _optimal.getCenter() );

    if ( _minTrack->getAxis() < _constraints.getVMin() ) _minTrack = NULL;
    if ( _maxTrack->getAxis() > _constraints.getVMax() ) _maxTrack = NULL;

    if ( _minTrack && !_maxTrack ) {
      _minTrack = _minTrack->getPreviousTrack();
      if (_minTrack->getAxis() < _constraints.getVMin()) _minTrack = NULL;
    }

    if ( _maxTrack && !_minTrack ) {
      _maxTrack = _maxTrack->getNextTrack();
      if ( _maxTrack->getAxis() > _constraints.getVMax() ) _maxTrack = NULL;
    }

    if ( _minTrack && (_minTrack->getAxis() < _optimal.getVMin()) ) _inMinOptimal = false;
    if ( _maxTrack && (_maxTrack->getAxis() > _optimal.getVMax()) ) _inMaxOptimal = false;

    cdebug_log(159,0) << "_minTrack: " << _minTrack << endl;;
    cdebug_log(159,0) << "_maxTrack: " << _maxTrack << endl;;
  }


  Tracks_Spiral::Locator::Locator ( const Locator& locator )
    : Hurricane::Locator<Track*>()
    , _optimal     (locator._optimal)
    , _constraints (locator._constraints)
    , _minTrack    (locator._minTrack)
    , _maxTrack    (locator._maxTrack)
    , _onMin       (locator._onMin)
    , _inMinOptimal(locator._inMinOptimal)
    , _inMaxOptimal(locator._inMaxOptimal)
  { }


  Hurricane::Locator<Track*>* Tracks_Spiral::Locator::getClone () const
  { return new Locator(*this); }


  bool  Tracks_Spiral::Locator::isValid () const
  { return _minTrack||_maxTrack; }


  Track* Tracks_Spiral::Locator::getElement () const
  {
    if ( !_minTrack ) return _maxTrack;
    if ( !_maxTrack ) return _minTrack;

    return (_onMin) ? _maxTrack : _minTrack;
  }


  bool  Tracks_Spiral::Locator::InOptimal () const
  { return _inMinOptimal&&_inMaxOptimal; }



  void  Tracks_Spiral::Locator::progress ()
  {
    cdebug_log(159,1) << "Track_Spiral::progress() - State:" << endl;
    cdebug_log(159,0)   << _onMin
                      << " " << _minTrack
                      << " " << _maxTrack << endl;

    if ( !isValid() ) {
      cdebug_tabw(159,-1);
      return;
    }

    if ( _onMin ) {
      _onMin = (_maxTrack == NULL);
      if ( _minTrack ) {
        _minTrack = _minTrack->getPreviousTrack();

        if ( _minTrack ) {
          if ( _minTrack->getAxis() <     _optimal.getVMin() ) _inMinOptimal = false;
          if ( _minTrack->getAxis() < _constraints.getVMin() ) _minTrack     = NULL;
        }
        if ( !_minTrack && _maxTrack ) progress();
      }
    } else {
      _onMin = (_minTrack != NULL);
      if ( _maxTrack ) {
        _maxTrack = _maxTrack->getNextTrack();

        if ( _maxTrack ) {
          if ( _maxTrack->getAxis() >     _optimal.getVMax() ) _inMaxOptimal = false;
          if ( _maxTrack->getAxis() > _constraints.getVMax() ) _maxTrack     = NULL;
        }

        if ( !_maxTrack && _minTrack ) progress();
      }
    }

    cdebug_log(159,0) << _onMin
                << " " << _minTrack
                << " " << _maxTrack << endl;
    cdebug_tabw(159,-1);
  }


  string  Tracks_Spiral::Locator::_getString () const
  {
    string s = "<Tracks_Spiral::Locator ["
             + getString(_minTrack) + ":"
             + getString(_maxTrack) + "]"
             + getString(_onMin)
             + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Tracks_Spiral".


  Tracks  Tracks_Spiral::get ( RoutingPlane* routingPlane
                             , Interval&     optimal
                             , Interval&     constraints )
  { return new Tracks_Spiral ( routingPlane, optimal, constraints ); }


  Tracks_Spiral::Tracks_Spiral ( const RoutingPlane* routingPlane
                               , const Interval&     optimal
                               , const Interval&     constraints )
    : Collection<Track*>()
    , _routingPlane(routingPlane)
    , _optimal     (optimal)
    , _constraints (constraints)
  { }


  Tracks_Spiral::Tracks_Spiral ( const Tracks_Spiral& tracks )
    : Collection<Track*>()
    , _routingPlane(tracks._routingPlane)
    , _optimal     (tracks._optimal)
    , _constraints (tracks._constraints)
  { }


  Collection<Track*>* Tracks_Spiral::getClone () const
  { return new Tracks_Spiral(*this); }


  Hurricane::Locator<Track*>* Tracks_Spiral::getLocator () const
  { return new Locator(_routingPlane,_optimal,_constraints); }


  string  Tracks_Spiral::_getString () const
  {
    string s = "<" + _TName("Tracks_Spiral") + " "
                   + getString(_routingPlane->getLayer()) + " ["
                   + getString(_constraints.getVMin()) + " ("
                   + getString(_optimal.getVMin()) + ":"
                   + getString(_optimal.getVMax()) + ") "
                   + getString(_constraints.getVMax()) + "]"
                   + ">";
    return s;
  }


} // Kite namespace.
