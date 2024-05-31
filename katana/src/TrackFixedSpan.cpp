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
// |  C++ Module  :       "./TrackFixedSpan.cpp"                     |
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
#include "katana/TrackFixedSpan.h"
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
// Class  :  "TrackFixedSpan".


  TrackFixedSpan::TrackFixedSpan ( Net* net, const Box& bb, Track* track )
    : TrackElement(NULL)
    , _net        (net)
    , _boundingBox(bb)
  {
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

    setFlags( TElemFixed|TElemForwardSetTrack );
    setTrack( track );

    if (not net or (net == Session::getBlockageNet())) {
      _net = Session::getBlockageNet();
      setFlags( TElemBlockage );
      if (track->getRoutingPlane()->getLayerGauge()->isUsable()) {
        GCellsUnder gcells = track->getKatanaEngine()->getGCellsUnder( source, target );
        for ( size_t i=0 ; i<gcells->size() ; ++i ) {
          GCell* gcell = gcells->gcellAt(i);
          gcell->addBlockage
            ( depth, gcell->getSide( track->getDirection() ).getIntersection( segside ).getSize() );
        }
      }
    }
  }


  void  TrackFixedSpan::_postCreate ()
  { TrackElement::_postCreate(); }


  TrackFixedSpan::~TrackFixedSpan ()
  { }


  void  TrackFixedSpan::_preDestroy ()
  {
    cdebug_log(155,0) << "TrackFixedSpan::_preDestroy() - " << (void*)this << endl;
    TrackElement::_preDestroy();
  }


  TrackFixedSpan* TrackFixedSpan::create ( Net* net, const Box& bb, Track* track )
  {
    TrackFixedSpan* trackTerminal = nullptr;
    if (not track) 
      throw Error( "TrackFixedSpan::create(): NULL track argument." );
    
    cdebug_log(159,0) << "TrackFixedSpan::create() track:" << track << endl;
    trackTerminal = new TrackFixedSpan ( net, bb, track );
    trackTerminal->_postCreate();
    cdebug_log(159,0) << trackTerminal << endl;
    Session::addInsertEvent( trackTerminal, track, track->getAxis() );
    return trackTerminal;
  }


  AutoSegment*   TrackFixedSpan::base             () const { return nullptr; }
  Segment*       TrackFixedSpan::getSegment       () const { return nullptr; }
  DbU::Unit      TrackFixedSpan::getAxis          () const { return getTrack()->getAxis(); }
  bool           TrackFixedSpan::isHorizontal     () const { return getTrack()->isHorizontal(); }
  bool           TrackFixedSpan::isVertical       () const { return getTrack()->isVertical(); }
  bool           TrackFixedSpan::isFixed          () const { return true; }
  bool           TrackFixedSpan::isPriorityLocked () const { return false; }
  bool           TrackFixedSpan::isFixedSpan      () const { return true; }
  Net*           TrackFixedSpan::getNet           () const { return _net; }
  Flags          TrackFixedSpan::getDirection     () const { return getTrack()->getDirection(); }
  DbU::Unit      TrackFixedSpan::getWidth         () const { return _boundingBox.getWidth(); }
  const Layer*   TrackFixedSpan::getLayer         () const { return getTrack()->getLayer(); }
  unsigned int   TrackFixedSpan::getDepth         () const { return Session::getLayerDepth(getLayer()); }
  Interval       TrackFixedSpan::getFreeInterval  () const { return Interval(); }
  size_t         TrackFixedSpan::getTrackSpan     () const { return 1; }


  DbU::Unit  TrackFixedSpan::getSourceAxis () const
  { return (isHorizontal()) ? _boundingBox.getXMin() : _boundingBox.getYMin(); }


  DbU::Unit  TrackFixedSpan::getTargetAxis () const
  { return (isHorizontal()) ? _boundingBox.getXMax() : _boundingBox.getYMax(); }


  unsigned long  TrackFixedSpan::getId () const
  {
    cerr << Error("TrackFixedSpan::getId() called on %s.",_getString().c_str()) << endl;
    return 0;
  }


  TrackElement* TrackFixedSpan::getNext () const
  {
    size_t dummy = _track->find( this );
    return _track->getNext( dummy, getNet() );
  }


  TrackElement* TrackFixedSpan::getPrevious () const
  {
    size_t dummy = _track->find( this );
    return _track->getPrevious( dummy, getNet() );
  }


  void  TrackFixedSpan::addOverlapCost ( TrackCost& cost ) const
  { cost.getTrack()->addOverlapCost( cost ); }


  float  TrackFixedSpan::getPriority () const
  { return 0.0; }


  void  TrackFixedSpan::setPriorityLock ( bool )
  { }


  void  TrackFixedSpan::forcePriority ( float )
  { }


  void  TrackFixedSpan::computePriority ()
  { }


  void  TrackFixedSpan::computeAlignedPriority ()
  { }


  void  TrackFixedSpan::detach ( TrackSet& removeds )
  {
    // cerr << Error( "TrackFixedSpan::detach(): Must never be called on %s."
    //              , getString(this).c_str()) << endl;
  }


  string  TrackFixedSpan::_getTypeName () const
  { return "TrackFixedSpan"; }


  string  TrackFixedSpan::_getString () const
  {
    string s1 = string("<TrackFixedSpan");
    s1 += " \"" + (_net ? getString(_net->getName()) : "net") + "\" ";
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


  Record* TrackFixedSpan::_getRecord () const
  {
    Record* record = TrackElement::_getRecord();
    record->add( getSlot( "_net",          _net         ) );
    record->add( getSlot( "_boundingBox", &_boundingBox ) );
    return record;
  }


} // Katana namespace.
