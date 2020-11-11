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
// |  C++ Module  :       "./TrackFixedSegment.cpp"                  |
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
#include "katana/TrackFixedSegment.h"
#include "katana/TrackCost.h"
#include "katana/Track.h"
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
// Class  :  "TrackFixedSegment".


  TrackFixedSegment::TrackFixedSegment ( Track* track, Segment* segment )
    : TrackElement  (NULL)
    , _segment      (segment)
  {
    cdebug_log(159,0) << "TrackFixedSegment::TrackFixedSegment() track:" << track << endl;
    Box boundingBox = segment->getBoundingBox();

    uint32_t flags = TElemFixed | ((segment->getNet() == Session::getBlockageNet()) ? TElemBlockage : 0);
    setFlags( flags );

    if (track) {
      uint32_t      depth      = track->getDepth();
      Technology*   technology = DataBase::getDB()->getTechnology();
      const Layer*  layer1     = track->getLayer()->getBlockageLayer();
    //RegularLayer* layer2     = dynamic_cast<RegularLayer*>(technology->getLayer(layer1->getMask()));
    //if (layer2) {
      //cerr << track->getLayer() << " minSpace:" << DbU::getValueString(track->getLayer()->getMinimalSpacing()) << endl;

        Interval   segside;
        Interval   uside   = track->getKatanaEngine()->getUSide( track->getDirection() );
        DbU::Unit  cap     = track->getLayer()->getMinimalSpacing()/2 /*+ track->getLayer()->getExtentionCap()*/;
        cdebug_log(159,0) << "uside:" << uside << " cap:" << DbU::getValueString(cap) << endl;
        cdebug_log(159,0) << "bb:" << boundingBox << endl;
        if (track->getDirection() == Flags::Horizontal) {
          segside  = Interval( boundingBox.getXMin(), boundingBox.getXMax() );
          _sourceU = max( boundingBox.getXMin() - cap, uside.getVMin());
          _targetU = min( boundingBox.getXMax() + cap, uside.getVMax());
        } else {
          segside  = Interval( boundingBox.getYMin(), boundingBox.getYMax() );
          _sourceU = max( boundingBox.getYMin() - cap, uside.getVMin());
          _targetU = min( boundingBox.getYMax() + cap, uside.getVMax());
        }

        Flags gcellFlags = Flags::NoFlags;
        if (segment->getNet()->isSupply() and (depth > 0)) {
          gcellFlags |= Flags::GoStraight;
        }

        GCellsUnder gcells = track->getKatanaEngine()->getGCellsUnder( segment );
        for ( size_t i=0 ; i<gcells->size() ; ++i ) {
          GCell* gcell = gcells->gcellAt(i);
          gcell->addBlockage
            ( depth, gcell->getSide( track->getDirection() ).getIntersection( segside ).getSize() );
          gcell->flags() |= gcellFlags;
        }
    //}
    } else
      cdebug_log(159,0) << "No track specified!" << endl;
  }


  void  TrackFixedSegment::_postCreate ()
  { TrackElement::_postCreate(); }


  TrackFixedSegment::~TrackFixedSegment ()
  { }


  void  TrackFixedSegment::_preDestroy ()
  {
    cdebug_log(155,0) << "TrackFixedSegment::_preDestroy() - " << (void*)this << endl;
    TrackElement::_preDestroy();
  }


  TrackElement* TrackFixedSegment::create ( Track* track, Segment* segment )
  {
    TrackFixedSegment* trackFixedSegment = NULL;
    if (track) { 
      cdebug_log(159,0) << "TrackFixedSegment::create() track:" << track << endl;
      trackFixedSegment = new TrackFixedSegment ( track, segment );
      trackFixedSegment->_postCreate();

      cdebug_log(159,0) << "Adding: " << segment << " on " << track << endl;
      cdebug_log(159,0) << "TrackFixedSegment::create(): " << trackFixedSegment << endl;

      Session::addInsertEvent( trackFixedSegment, track, track->getAxis() );
    }
    return trackFixedSegment;
  }


  AutoSegment*   TrackFixedSegment::base             () const { return NULL; }
  Segment*       TrackFixedSegment::getSegment       () const { return _segment; }
  DbU::Unit      TrackFixedSegment::getAxis          () const { return getTrack() ? getTrack()->getAxis() : 0; }
  bool           TrackFixedSegment::isHorizontal     () const { return getTrack()->isHorizontal(); }
  bool           TrackFixedSegment::isVertical       () const { return getTrack()->isVertical(); }
  bool           TrackFixedSegment::isFixed          () const { return true; }
  bool           TrackFixedSegment::isPriorityLocked () const { return false; }
  Flags          TrackFixedSegment::getDirection     () const { return getTrack()->getDirection(); }
  DbU::Unit      TrackFixedSegment::getWidth         () const { return _segment->getWidth(); }
  const Layer*   TrackFixedSegment::getLayer         () const { return _segment->getLayer(); }
  unsigned int   TrackFixedSegment::getDepth         () const { return Session::getLayerDepth(getLayer()); }
  Interval       TrackFixedSegment::getFreeInterval  () const { return Interval(); }
  size_t         TrackFixedSegment::getTrackSpan     () const { return 1; }


  DbU::Unit  TrackFixedSegment::getSourceAxis () const
  {
    const Horizontal* horizontal = dynamic_cast<const Horizontal*>( _segment );
    if (horizontal) return horizontal->getSourceX();

    const Vertical* vertical = dynamic_cast<const Vertical*>( _segment );
    if (vertical) return vertical->getSourceY();

    return 0;
  }


  DbU::Unit  TrackFixedSegment::getTargetAxis () const
  {
    const Horizontal* horizontal = dynamic_cast<const Horizontal*>( _segment );
    if (horizontal) return horizontal->getTargetX();

    const Vertical* vertical = dynamic_cast<const Vertical*>( _segment );
    if (vertical) return vertical->getTargetY();

    return 0;
  }
  

  unsigned long  TrackFixedSegment::getId () const
  {
    cerr << Error("::getId() called on %s.",_getString().c_str()) << endl;
    return 0;
  }


  Net* TrackFixedSegment::getNet () const
  {
    Net* realNet = _segment->getNet();
    if (realNet->isSupply() or realNet->isClock())
      return Session::getBlockageNet();
    return realNet;
  }


  TrackElement* TrackFixedSegment::getNext () const
  {
    size_t dummy = _track->find( this );
    return _track->getNext( dummy, getNet() );
  }


  TrackElement* TrackFixedSegment::getPrevious () const
  {
    size_t dummy = _track->find( this );
    return _track->getPrevious( dummy, getNet() );
  }


  void  TrackFixedSegment::addOverlapCost ( TrackCost& cost ) const
  {
    Track* track = cost.getTrack();

    if (not track) return;
    track->addOverlapCost( cost );
  }


  float  TrackFixedSegment::getPriority () const
  { return 0.0; }


  void  TrackFixedSegment::setPriorityLock ( bool )
  { }


  void  TrackFixedSegment::forcePriority ( float )
  { }


  void  TrackFixedSegment::computePriority ()
  { }


  void  TrackFixedSegment::computeAlignedPriority ()
  { }


  void  TrackFixedSegment::detach ( TrackSet& removeds )
  {
    // cerr << Error( "TrackFixedSegment::detach(): Must never be called on %s."
    //              , getString(this).c_str()) << endl;
  }

  string  TrackFixedSegment::_getTypeName () const
  { return "TrackFixedSegment"; }


  string  TrackFixedSegment::_getString () const
  {
    string s1 = _segment->_getString();
    string s2 = " ["   + DbU::getValueString(_sourceU)
              +  ":"   + DbU::getValueString(_targetU) + "]"
              +  " "   + DbU::getValueString(_targetU-_sourceU)
              + " F"
              + ((isBlockage()) ? "B" : "-");
    s1.insert ( s1.size()-1, s2 );

    return s1;
  }


  Record* TrackFixedSegment::_getRecord () const
  {
    Record* record = TrackElement::_getRecord();
    record->add( getSlot( "_segment", _segment ) );
    return record;
  }


} // Katana namespace.
