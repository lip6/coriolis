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
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TrackBlockage.cpp"                      |
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
#include "katana/TrackBlockage.h"
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
// Class  :  "TrackBlockage".


  TrackBlockage::TrackBlockage ( Track* track, Box boundingBox )
    : TrackElement(NULL)
    , _boundingBox(boundingBox)
  {
    cdebug_log(159,0) << "TrackBlockage::TrackBlockage() track=" << track << endl;

    setFlags( TElemFixed|TElemBlockage|TElemForwardSetTrack );
    setTrack( track );

    uint32_t      depth      = track->getDepth();
  //Technology*   technology = DataBase::getDB()->getTechnology();
  //const Layer*  layer1     = track->getLayer()->getBlockageLayer();
  //RegularLayer* layer2     = dynamic_cast<RegularLayer*>(technology->getLayer(layer1->getMask()));
  //if (layer2) {
  //cerr << track->getLayer() << " minSpace:" << DbU::getValueString(track->getLayer()->getMinimalSpacing()) << endl;

    Point      source;
    Point      target;
    Interval   segside;
    Interval   uside   = track->getKatanaEngine()->getUSide( track->getDirection() );
    DbU::Unit  cap     = track->getLayer()->getMinimalSpacing()/2 /*+ track->getLayer()->getExtentionCap()*/;
    cdebug_log(159,0) << "uside:" << uside << " cap:" << DbU::getValueString(cap) << endl;
    cdebug_log(159,0) << "bb:" << boundingBox << endl;
    if (track->getDirection() == Flags::Horizontal) {
      segside  = Interval( boundingBox.getXMin(), boundingBox.getXMax() );
      _sourceU = max( boundingBox.getXMin() - cap, uside.getVMin());
      _targetU = min( boundingBox.getXMax() + cap, uside.getVMax());
      source = Point( boundingBox.getXMin(), track->getAxis() );
      target = Point( boundingBox.getXMax(), track->getAxis() );
    } else {
      segside  = Interval( boundingBox.getYMin(), boundingBox.getYMax() );
      _sourceU = max( boundingBox.getYMin() - cap, uside.getVMin());
      _targetU = min( boundingBox.getYMax() + cap, uside.getVMax());
      source = Point( track->getAxis(), boundingBox.getYMin() );
      target = Point( track->getAxis(), boundingBox.getYMax() );
    }

    if (track->getRoutingPlane()->getLayerGauge()->isUsable()) {
      Flags gcellFlags = Flags::NoFlags;
      GCellsUnder gcells = track->getKatanaEngine()->getGCellsUnder( source, target );
      for ( size_t i=0 ; i<gcells->size() ; ++i ) {
        GCell* gcell = gcells->gcellAt(i);
        gcell->addBlockage
          ( depth, gcell->getSide( track->getDirection() ).getIntersection( segside ).getSize() );
        gcell->flags() |= gcellFlags;
      }
    }
  }


  void  TrackBlockage::_postCreate ()
  { TrackElement::_postCreate(); }


  TrackBlockage::~TrackBlockage ()
  { }


  void  TrackBlockage::_preDestroy ()
  {
    cdebug_log(155,0) << "TrackBlockage::_preDestroy() - " << (void*)this << endl;
    TrackElement::_preDestroy();
  }


  TrackElement* TrackBlockage::create ( Track* track, Box boundingBox )
  {
    TrackBlockage* trackBlockage = NULL;
    if (not track) 
      throw Error( "TrackBlockage::create(): NULL track argument." );
    
    cdebug_log(159,0) << "TrackBlockage::create() track:" << track << endl;
    trackBlockage = new TrackBlockage ( track, boundingBox );
    trackBlockage->_postCreate();
    cdebug_log(159,0) << "TrackBlockage::create(): " << trackBlockage << endl;
    Session::addInsertEvent( trackBlockage, track, track->getAxis() );
    return trackBlockage;
  }


  AutoSegment*   TrackBlockage::base             () const { return NULL; }
  Segment*       TrackBlockage::getSegment       () const { return NULL; }
  DbU::Unit      TrackBlockage::getAxis          () const { return getTrack()->getAxis(); }
  bool           TrackBlockage::isHorizontal     () const { return getTrack()->isHorizontal(); }
  bool           TrackBlockage::isVertical       () const { return getTrack()->isVertical(); }
  bool           TrackBlockage::isFixed          () const { return true; }
  bool           TrackBlockage::isPriorityLocked () const { return false; }
  Flags          TrackBlockage::getDirection     () const { return getTrack()->getDirection(); }
  DbU::Unit      TrackBlockage::getWidth         () const { return _boundingBox.getWidth(); }
  const Layer*   TrackBlockage::getLayer         () const { return getTrack()->getLayer(); }
  unsigned int   TrackBlockage::getDepth         () const { return Session::getLayerDepth(getLayer()); }
  Interval       TrackBlockage::getFreeInterval  () const { return Interval(); }
  size_t         TrackBlockage::getTrackSpan     () const { return 1; }


  DbU::Unit  TrackBlockage::getSourceAxis () const
  { return (isHorizontal()) ? _boundingBox.getXMin() : _boundingBox.getYMin(); }


  DbU::Unit  TrackBlockage::getTargetAxis () const
  { return (isHorizontal()) ? _boundingBox.getXMax() : _boundingBox.getYMax(); }
  

  unsigned long  TrackBlockage::getId () const
  {
    cerr << Error("::getId() called on %s.",_getString().c_str()) << endl;
    return 0;
  }


  Net* TrackBlockage::getNet () const
  { return Session::getBlockageNet(); }


  TrackElement* TrackBlockage::getNext () const
  {
    size_t dummy = _track->find( this );
    return _track->getNext( dummy, getNet() );
  }


  TrackElement* TrackBlockage::getPrevious () const
  {
    size_t dummy = _track->find( this );
    return _track->getPrevious( dummy, getNet() );
  }


  void  TrackBlockage::addOverlapCost ( TrackCost& cost ) const
  { cost.getTrack()->addOverlapCost( cost ); }


  float  TrackBlockage::getPriority () const
  { return 0.0; }


  void  TrackBlockage::setPriorityLock ( bool )
  { }


  void  TrackBlockage::forcePriority ( float )
  { }


  void  TrackBlockage::computePriority ()
  { }


  void  TrackBlockage::computeAlignedPriority ()
  { }


  void  TrackBlockage::detach ( TrackSet& removeds )
  {
    // cerr << Error( "TrackBlockage::detach(): Must never be called on %s."
    //              , getString(this).c_str()) << endl;
  }


  string  TrackBlockage::_getTypeName () const
  { return "TrackBlockage"; }


  string  TrackBlockage::_getString () const
  {
    string s1 = string("<TrackBlockage ")
              + "[" + getString(getTrack()->getIndex()) + "] "
              + getString(getLayer()->getName()) + " @"
              + DbU::getValueString(getTrack()->getAxis())
              + " ["   + DbU::getValueString(_sourceU)
              +  ":"   + DbU::getValueString(_targetU) + "]"
              +  " "   + DbU::getValueString(_targetU-_sourceU)
              + " FBN>";
    return s1;
  }


  Record* TrackBlockage::_getRecord () const
  {
    Record* record = TrackElement::_getRecord();
    record->add( getSlot( "_boundingBox", &_boundingBox ) );
    return record;
  }


} // Katana namespace.
