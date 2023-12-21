// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
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
#include "katana/HorizontalTrack.h"
#include "katana/VerticalTrack.h"
#include "katana/RoutingPlane.h"
#include "katana/KatanaEngine.h"


namespace {


  const char* badLayerGauge =
    "RoutingPlane::create() :\n\n"
    "    No plane at depth %u in %s.";


} // End of local namespace.


namespace Katana {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::Error;
  using Hurricane::Box;
  using Hurricane::Cell;


// -------------------------------------------------------------------
// Class  :  "RoutingPlane".


  RoutingPlane::RoutingPlane ( KatanaEngine* katana, size_t depth )
    : _katana    (katana)
    , _layerGauge(katana->getConfiguration()->getLayerGauge(depth))
    , _depth     (depth)
    , _flags     (0)
    , _axisMin   (0)
    , _axisMax   (0)
    , _trackMin  (0)
    , _trackMax  (0)
    , _tracks    ()
  {
    switch ( _layerGauge->getDirection() ) {
      case Constant::Horizontal: _flags |= Flags::Horizontal; break;
      case Constant::Vertical:   _flags |= Flags::Vertical; break;
      default:
        cerr << Error( "RoutingPlane::RoutingPlane() - Unknown plane direction from LayerGauge: %u"
                     , _layerGauge->getDirection() ) << endl;
    }
  }


  RoutingPlane::~RoutingPlane ()
  { }


  void  RoutingPlane::destroy ()
  {
    cdebug_log(155,1) << "RoutingPlane::destroy() - "
                      << (void*)this << " " << this << endl;

    for ( size_t index=0 ; index<_tracks.size() ; ++index )
      _tracks[index]->destroy();

    delete this;

    cdebug_tabw(155,-1);
  }


  RoutingPlane* RoutingPlane::create ( KatanaEngine* katana, size_t depth )
  {
    RoutingPlane* plane = new RoutingPlane ( katana, depth );

    if (not plane->_layerGauge)
      throw Error( badLayerGauge, depth, getString(katana->getConfiguration()->getRoutingGauge()).c_str() );

    DbU::Unit extensionCap = ( std::max( Session::getWireWidth(depth), Session::getViaWidth(depth) )
                             + Session::getPitch(depth) ) / 2;

    size_t  trackNumber;
    Box     abutmentBox = katana->getCell()->getAbutmentBox();
  // HARD CODED.
    if (plane->getDirection() == Flags::Horizontal) {
      plane->_trackMin = abutmentBox.getXMin() - extensionCap;
      plane->_trackMax = abutmentBox.getXMax() + extensionCap;
      plane->_axisMin  = abutmentBox.getYMin();
      plane->_axisMax  = abutmentBox.getYMax();
      trackNumber      = plane->computeTracksSize();
    } else {
      plane->_trackMin = abutmentBox.getYMin() - extensionCap;
      plane->_trackMax = abutmentBox.getYMax() + extensionCap;
      plane->_axisMin  = abutmentBox.getXMin();
      plane->_axisMax  = abutmentBox.getXMax();
      trackNumber      = plane->computeTracksSize();
    }

    plane->_tracks.reserve( trackNumber );
    for ( size_t index=0 ; index<trackNumber ; ++index ) {
      if (plane->getDirection() == Flags::Horizontal) {
        plane->_tracks.push_back( HorizontalTrack::create( plane, index ) );
      // Ugly: Direct uses of CellGauge (middle tracks 4 & 5 for local use).
        if ( (depth == 1) and not Session::getRoutingGauge()->isVH() ) {
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
  { return getKatanaEngine()->getRoutingPlaneByIndex( getDepth()+1 ); }


  RoutingPlane* RoutingPlane::getBottom () const
  {
    if (not getDepth()) return NULL;
    return getKatanaEngine()->getRoutingPlaneByIndex( getDepth()-1 );
  }


  Track* RoutingPlane::getTrackByIndex ( size_t index ) const
  {
    if (index >= getTracksSize()) return NULL;
    return _tracks[index];
  }


  Track* RoutingPlane::getTrackByPosition ( DbU::Unit axis, uint32_t mode ) const
  {
    if (_tracks.empty())     return nullptr;
    if (axis < getAxisMin()) return _tracks.front();
    if (axis > getAxisMax()) return _tracks.back();
    
    size_t index = getLayerGauge()->getTrackIndex( getAxisMin(), getAxisMax(), axis, mode );
    if (index == getTracksSize()) {
      if (not index) return nullptr;
      if ((mode == Constant::Superior) or (mode == Constant::Exact)) return nullptr;
      --index;
    }

    return getTrackByIndex( index );
  }


  bool  RoutingPlane::_check ( uint32_t& overlaps ) const
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
               + ((getDirection() == Flags::Horizontal) ? " horizontal [" : " vertical [")
               + getString(_tracks.size()) + "/"
               + getString(_tracks.capacity())
               + "]>";
  }


  Record* RoutingPlane::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add( getSlot          ( "_katana"         ,  _katana       ) );
    record->add( getSlot          ( "_layerGauge"   ,  _layerGauge ) );
    record->add( getSlot          ( "_depth"        , &_depth      ) );
    record->add( DbU::getValueSlot( "_axisMin"      , &_axisMin    ) );
    record->add( DbU::getValueSlot( "_axisMax"      , &_axisMax    ) );
    record->add( DbU::getValueSlot( "_trackMin"     , &_trackMin   ) );
    record->add( DbU::getValueSlot( "_trackMax"     , &_trackMax   ) );
    record->add( getSlot          ( "_tracks"       , &_tracks     ) );
                                     
    return record;
  }


} // Katana namespace.
