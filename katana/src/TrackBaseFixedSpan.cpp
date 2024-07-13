// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2024.
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TrackBaseFixedSpan.cpp"                 |
// +-----------------------------------------------------------------+


#include <sstream>
#include "hurricane/Bug.h"
#include "hurricane/Warning.h"
#include "hurricane/Net.h"
#include "hurricane/Name.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "anabatic/AutoContact.h"
#include "crlcore/RoutingGauge.h"
#include "katana/DataNegociate.h"
#include "katana/TrackBaseFixedSpan.h"
#include "katana/TrackCost.h"
#include "katana/Track.h"
#include "katana/RoutingPlane.h"
#include "katana/Session.h"
#include "katana/RoutingEvent.h"
#include "katana/NegociateWindow.h"
#include "katana/KatanaEngine.h"


namespace Katana {

  using namespace std;
  using Hurricane::Warning;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Net;
  using Hurricane::Name;
  using Hurricane::RegularLayer;
  using Hurricane::Technology;
  using Hurricane::DataBase;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Anabatic::GCellsUnder;


// -------------------------------------------------------------------
// Class  :  "TrackBaseFixedSpan".


  TrackBaseFixedSpan::TrackBaseFixedSpan ( Net* net, const Box& bb )
    : TrackElement(nullptr)
    , _boundingBox(bb)
  { }


  TrackBaseFixedSpan::~TrackBaseFixedSpan ()
  { }


  void  TrackBaseFixedSpan::_postCreate ( Track* track )
  {
    setFlags( TElemFixed|TElemForwardSetTrack );
    setTrack( track );

    uint32_t      depth      = track->getDepth();
  //Technology*   technology = DataBase::getDB()->getTechnology();
  //const Layer*  layer1     = track->getLayer()->getTerminalLayer();
  //RegularLayer* layer2     = dynamic_cast<RegularLayer*>(technology->getLayer(layer1->getMask()));
  //if (layer2) {
  //cerr << track->getLayer() << " minSpace:" << DbU::getValueString(track->getLayer()->getMinimalSpacing()) << endl;

    Point      source;
    Point      target;
    Interval   segside;
    Interval   uside   = track->getKatanaEngine()->getUSide( track->getDirection() );
    DbU::Unit  cap     = track->getLayer()->getMinimalSpacing()/2 /*+ track->getLayer()->getExtentionCap()*/;
    cdebug_log(159,0) << "uside:" << uside << " cap:" << DbU::getValueString(cap) << endl;
    cdebug_log(159,0) << "bb:" << _boundingBox << endl;
    if (track->getDirection() == Flags::Horizontal) {
      segside  = Interval( _boundingBox.getXMin(), _boundingBox.getXMax() );
      _sourceU = max( _boundingBox.getXMin() - cap, uside.getVMin());
      _targetU = min( _boundingBox.getXMax() + cap, uside.getVMax());
      source = Point( _boundingBox.getXMin(), track->getAxis() );
      target = Point( _boundingBox.getXMax(), track->getAxis() );
    } else {
      segside  = Interval( _boundingBox.getYMin(), _boundingBox.getYMax() );
      _sourceU = max( _boundingBox.getYMin() - cap, uside.getVMin());
      _targetU = min( _boundingBox.getYMax() + cap, uside.getVMax());
      source = Point( track->getAxis(), _boundingBox.getYMin() );
      target = Point( track->getAxis(), _boundingBox.getYMax() );
    }

    if (not getNet() or (getNet() == Session::getBlockageNet())) {
      setNet( Session::getBlockageNet() );
      setFlags( TElemBlockage );
      if (track->getRoutingPlane()->getLayerGauge()->isUsable()) {
        GCellsUnder gcells = track->getKatanaEngine()->getGCellsUnder( source, target );
        for ( size_t i=0 ; i<gcells->size() ; ++i ) {
          GCell* gcell = gcells->gcellAt( i );
          gcell->addBlockage
            ( depth, gcell->getSide( track->getDirection() ).getIntersection( segside ).getSize() );
        }
      }
    }
  }


  AutoSegment*   TrackBaseFixedSpan::base             () const { return nullptr; }
  Segment*       TrackBaseFixedSpan::getSegment       () const { return nullptr; }
  DbU::Unit      TrackBaseFixedSpan::getAxis          () const { return getTrack()->getAxis(); }
  bool           TrackBaseFixedSpan::isHorizontal     () const { return getTrack()->isHorizontal(); }
  bool           TrackBaseFixedSpan::isVertical       () const { return getTrack()->isVertical(); }
  bool           TrackBaseFixedSpan::isFixed          () const { return true; }
  bool           TrackBaseFixedSpan::isPriorityLocked () const { return false; }
  bool           TrackBaseFixedSpan::isFixedSpan      () const { return true; }
  bool           TrackBaseFixedSpan::isFixedSpanRp    () const { return false; }
  Flags          TrackBaseFixedSpan::getDirection     () const { return getTrack()->getDirection(); }
  DbU::Unit      TrackBaseFixedSpan::getWidth         () const { return _boundingBox.getWidth(); }
  const Layer*   TrackBaseFixedSpan::getLayer         () const { return getTrack()->getLayer(); }
  RoutingPad*    TrackBaseFixedSpan::getRoutingPad    () const { return nullptr; }
  unsigned int   TrackBaseFixedSpan::getDepth         () const { return Session::getLayerDepth(getLayer()); }
  Interval       TrackBaseFixedSpan::getFreeInterval  () const { return Interval(); }
  size_t         TrackBaseFixedSpan::getTrackSpan     () const { return 1; }


  DbU::Unit  TrackBaseFixedSpan::getSourceAxis () const
  { return (isHorizontal()) ? _boundingBox.getXMin() : _boundingBox.getYMin(); }


  DbU::Unit  TrackBaseFixedSpan::getTargetAxis () const
  { return (isHorizontal()) ? _boundingBox.getXMax() : _boundingBox.getYMax(); }


  unsigned long  TrackBaseFixedSpan::getId () const
  {
    cerr << Error("TrackBaseFixedSpan::getId() called on %s.",_getString().c_str()) << endl;
    return 0;
  }


  TrackElement* TrackBaseFixedSpan::getNext () const
  {
    size_t dummy = _track->find( this );
    return _track->getNext( dummy, getNet() );
  }


  TrackElement* TrackBaseFixedSpan::getPrevious () const
  {
    size_t dummy = _track->find( this );
    return _track->getPrevious( dummy, getNet() );
  }


  void  TrackBaseFixedSpan::addOverlapCost ( TrackCost& cost ) const
  { cost.getTrack()->addOverlapCost( cost ); }


  float  TrackBaseFixedSpan::getPriority () const
  { return 0.0; }


  void  TrackBaseFixedSpan::setPriorityLock ( bool )
  { }


  void  TrackBaseFixedSpan::forcePriority ( float )
  { }


  void  TrackBaseFixedSpan::computePriority ()
  { }


  void  TrackBaseFixedSpan::computeAlignedPriority ()
  { }


  void  TrackBaseFixedSpan::detach ( TrackSet& removeds )
  {
    // cerr << Error( "TrackBaseFixedSpan::detach(): Must never be called on %s."
    //              , getString(this).c_str()) << endl;
  }


  string  TrackBaseFixedSpan::_getTypeName () const
  { return "TrackBaseFixedSpan"; }


  string  TrackBaseFixedSpan::_getString () const
  {
    string s1 = "<" + _getTypeName();
    s1 += " \"" + (getNet() ? getString(getNet()->getName()) : "net") + "\" ";
    if (getTrack()) {
      s1 += "[" + getString(getTrack()->getIndex()) + "] "
         + getString(getLayer()->getName()) + " @"
         + DbU::getValueString(getTrack()->getAxis());
    } else {
      s1 += "[i] layer @axis";
    }
    s1 += " [" + DbU::getValueString(_sourceU)
       +  ":"  + DbU::getValueString(_targetU) + "]"
       +  " "  + DbU::getValueString(_targetU-_sourceU)
       +  " F";
    s1 += (isBlockage() ? "B" : "-");
    s1 += "N>";
    return s1;
  }


  Record* TrackBaseFixedSpan::_getRecord () const
  {
    Record* record = TrackElement::_getRecord();
    record->add( getSlot( "_boundingBox", &_boundingBox ) );
    return record;
  }


} // Katana namespace.
