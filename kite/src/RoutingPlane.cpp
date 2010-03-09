
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingPlane.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/Error.h"
#include  "hurricane/Box.h"
#include  "hurricane/Cell.h"

#include  "crlcore/RoutingLayerGauge.h"

#include  "kite/HorizontalTrack.h"
#include  "kite/VerticalTrack.h"
#include  "kite/RoutingPlane.h"
#include  "kite/KiteEngine.h"


namespace {


  const char* badLayerGauge =
    "RoutingPlane::create() :\n\n"
    "    No plane at depth %u in %s.";


} // End of local namespace.


namespace Kite {


  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::Error;
  using Hurricane::Box;
  using Hurricane::Cell;


// -------------------------------------------------------------------
// Class  :  "RoutingPlane".


  RoutingPlane::RoutingPlane ( KiteEngine* kite, size_t depth )
    : _kite      (kite)
    , _layerGauge(kite->getLayerGauge(depth))
    , _depth     (depth)
    , _tracks    ()
  { }


  RoutingPlane::~RoutingPlane ()
  { }


  void  RoutingPlane::destroy ()
  {
    ltrace(90) << "RoutingPlane::destroy() - "
               << (void*)this << " " << this << endl;
    ltracein(90);

    for ( size_t index = 0 ; index < _tracks.size() ; index++ )
      _tracks[index]->destroy ();

    delete this;

    ltraceout(90);
  }


  RoutingPlane* RoutingPlane::create ( KiteEngine* kite, size_t depth )
  {
    RoutingPlane* plane = new RoutingPlane ( kite, depth );

    if ( !plane->_layerGauge )
      throw Error ( badLayerGauge, depth, getString(kite->getRoutingGauge()).c_str() );

    size_t  trackNumber;
    Box     abutmentBox = kite->getCell()->getAbutmentBox();
    if ( plane->getLayerGauge()->getDirection() & Constant::Horizontal ) {
      plane->_trackMin = abutmentBox.getXMin () - DbU::lambda (2.0);
      plane->_trackMax = abutmentBox.getXMax () + DbU::lambda (2.0);
      plane->_axisMin  = abutmentBox.getYMin ();
      plane->_axisMax  = abutmentBox.getYMax ();
      trackNumber      = plane->computeTracksSize ();
    } else {
      plane->_trackMin = abutmentBox.getYMin () - DbU::lambda (2.0);
      plane->_trackMax = abutmentBox.getYMax () + DbU::lambda (2.0);
      plane->_axisMin  = abutmentBox.getXMin ();
      plane->_axisMax  = abutmentBox.getXMax ();
      trackNumber      = plane->computeTracksSize ();
    }

    plane->_tracks.reserve ( trackNumber );
    for ( size_t index = 0 ; index < trackNumber ; index++ ) {
      if ( plane->getLayerGauge()->getDirection() & Constant::Horizontal ) {
        plane->_tracks.push_back ( HorizontalTrack::create ( plane, index ) );
      } else {
        plane->_tracks.push_back ( VerticalTrack::create ( plane, index ) );
      }
    }

    return plane;
  }


  RoutingPlane* RoutingPlane::getTop () const
  { return getKiteEngine()->getRoutingPlaneByIndex ( getDepth()+1 ); }


  RoutingPlane* RoutingPlane::getBottom () const
  {
    if ( !getDepth() ) return NULL;
    return getKiteEngine()->getRoutingPlaneByIndex ( getDepth()-1 );
  }


  Track* RoutingPlane::getTrackByIndex ( size_t index ) const
  {
    if ( index >= getTracksSize() ) return NULL;
    return _tracks[index];
  }


  Track* RoutingPlane::getTrackByPosition ( DbU::Unit axis, unsigned int mode ) const
  {
    return getTrackByIndex ( getLayerGauge()->getTrackIndex ( getAxisMin()
                                                            , getAxisMax()
                                                            , axis
                                                            , mode
                                                            ) );
  }


  bool  RoutingPlane::_check ( unsigned int& overlaps ) const
  {
    bool coherency = true;

    for ( size_t i=0 ; i<_tracks.size() ; i++ ) {
      coherency = _tracks[i]->_check(overlaps) && coherency;
    }

    return coherency;
  }


  string  RoutingPlane::_getString () const
  {
    return "<" + _getTypeName() + " @"
               + getString(_depth) + " ["
               + getString(_tracks.size()) + "/"
               + getString(_tracks.capacity())
               + "]>";
  }


  Record* RoutingPlane::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot ( "_kite"         ,  _kite          ) );
    record->add ( getSlot ( "_layerGauge"   ,  _layerGauge    ) );
    record->add ( getSlot ( "_depth"        , &_depth         ) );
    record->add ( DbU::getValueSlot ( "_axisMin"      , &_axisMin       ) );
    record->add ( DbU::getValueSlot ( "_axisMax"      , &_axisMax       ) );
    record->add ( DbU::getValueSlot ( "_trackMin"     , &_trackMin      ) );
    record->add ( DbU::getValueSlot ( "_trackMax"     , &_trackMax      ) );
    record->add ( getSlot ( "_tracks"       , &_tracks        ) );
                                     
    return record;
  }


} // End of Kite namespace.
