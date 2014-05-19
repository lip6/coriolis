// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingPlane.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Box.h"
#include "hurricane/Cell.h"
#include "crlcore/RoutingLayerGauge.h"
#include "kite/HorizontalTrack.h"
#include "kite/VerticalTrack.h"
#include "kite/RoutingPlane.h"
#include "kite/KiteEngine.h"


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
    , _flags     (0)
    , _axisMin   (0)
    , _axisMax   (0)
    , _trackMin  (0)
    , _trackMax  (0)
    , _tracks    ()
  {
    switch ( _layerGauge->getDirection() ) {
      case Constant::Horizontal: _flags |= KbHorizontal; break;
      case Constant::Vertical:   _flags |= KbVertical; break;
      default:
        cerr << Error( "RoutingPlane::RoutingPlane() - Unknown plane direction from LayerGauge: %u"
                     , _layerGauge->getDirection() ) << endl;
    }
  }


  RoutingPlane::~RoutingPlane ()
  { }


  void  RoutingPlane::destroy ()
  {
    ltrace(90) << "RoutingPlane::destroy() - "
               << (void*)this << " " << this << endl;
    ltracein(90);

    for ( size_t index=0 ; index<_tracks.size() ; ++index )
      _tracks[index]->destroy();

    delete this;

    ltraceout(90);
  }


  RoutingPlane* RoutingPlane::create ( KiteEngine* kite, size_t depth )
  {
    RoutingPlane* plane = new RoutingPlane ( kite, depth );

    if (not plane->_layerGauge)
      throw Error( badLayerGauge, depth, getString(kite->getRoutingGauge()).c_str() );

    DbU::Unit    hExtension = 0;
    DbU::Unit    vExtension = 0;
    unsigned int gaugeDepth = 0;
    if (Session::getLayerGauge(gaugeDepth)->getType() == Constant::PinOnly) ++gaugeDepth;

    bool HV = (Session::getLayerGauge(gaugeDepth)->getDirection() == Constant::Horizontal);
    hExtension = Session::getLayerGauge( gaugeDepth + (HV?1:0) )->getPitch() / 2;
    vExtension = Session::getLayerGauge( gaugeDepth + (HV?0:1) )->getPitch() / 2;

    size_t  trackNumber;
    Box     abutmentBox = kite->getCell()->getAbutmentBox();
  // HARD CODED.
    if (plane->getDirection() == KbHorizontal) {
      plane->_trackMin = abutmentBox.getXMin() - hExtension;
      plane->_trackMax = abutmentBox.getXMax() + hExtension;
      plane->_axisMin  = abutmentBox.getYMin();
      plane->_axisMax  = abutmentBox.getYMax();
      trackNumber      = plane->computeTracksSize();
    } else {
      plane->_trackMin = abutmentBox.getYMin() - vExtension;
      plane->_trackMax = abutmentBox.getYMax() + vExtension;
      plane->_axisMin  = abutmentBox.getXMin();
      plane->_axisMax  = abutmentBox.getXMax();
      trackNumber      = plane->computeTracksSize();
    }

    plane->_tracks.reserve( trackNumber );
    for ( size_t index=0 ; index<trackNumber ; ++index ) {
      if (plane->getDirection() == KbHorizontal) {
        plane->_tracks.push_back( HorizontalTrack::create( plane, index ) );
      // Ugly: Direct uses of CellGauge (middle tracks 4 & 5 for local use).
        if (depth == 1) {
          switch ( index%10 ) {
            case 4:
            case 5:
              plane->_tracks.back()->setLocalAssigned( true );
              break;
          }
        }
      } else {
        plane->_tracks.push_back( VerticalTrack::create( plane, index ) );
      }
    }

    return plane;
  }


  RoutingPlane* RoutingPlane::getTop () const
  { return getKiteEngine()->getRoutingPlaneByIndex( getDepth()+1 ); }


  RoutingPlane* RoutingPlane::getBottom () const
  {
    if (not getDepth()) return NULL;
    return getKiteEngine()->getRoutingPlaneByIndex( getDepth()-1 );
  }


  Track* RoutingPlane::getTrackByIndex ( size_t index ) const
  {
    if (index >= getTracksSize()) return NULL;
    return _tracks[index];
  }


  Track* RoutingPlane::getTrackByPosition ( DbU::Unit axis, unsigned int mode ) const
  {
    return getTrackByIndex( getLayerGauge()->getTrackIndex( getAxisMin()
                                                          , getAxisMax()
                                                          , axis
                                                          , mode
                                                          ) );
  }


  bool  RoutingPlane::_check ( unsigned int& overlaps ) const
  {
    bool coherency = true;

    for ( size_t i=0 ; i<_tracks.size() ; ++i ) {
      coherency = _tracks[i]->check(overlaps) and coherency;
    }

    return coherency;
  }


  string  RoutingPlane::_getString () const
  {
    return "<" + _getTypeName() + " @"
               + getString(_depth) + " "
               + getString(getLayer()) + " [ "
               + ((getDirection() == KbHorizontal) ? " horizontal [" : " vertical [")
               + getString(_tracks.size()) + "/"
               + getString(_tracks.capacity())
               + "]>";
  }


  Record* RoutingPlane::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add( getSlot          ( "_kite"         ,  _kite       ) );
    record->add( getSlot          ( "_layerGauge"   ,  _layerGauge ) );
    record->add( getSlot          ( "_depth"        , &_depth      ) );
    record->add( DbU::getValueSlot( "_axisMin"      , &_axisMin    ) );
    record->add( DbU::getValueSlot( "_axisMax"      , &_axisMax    ) );
    record->add( DbU::getValueSlot( "_trackMin"     , &_trackMin   ) );
    record->add( DbU::getValueSlot( "_trackMax"     , &_trackMax   ) );
    record->add( getSlot          ( "_tracks"       , &_tracks     ) );
                                     
    return record;
  }


} // Kite namespace.
