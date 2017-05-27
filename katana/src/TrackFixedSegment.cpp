// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
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


  Net* TrackFixedSegment::_blockageNet = NULL;


  TrackFixedSegment::TrackFixedSegment ( Track* track, Segment* segment )
    : TrackElement  (NULL)
    , _segment      (segment)
  {
    Box boundingBox = segment->getBoundingBox();

    uint32_t flags = TElemFixed | ((segment->getNet() == _blockageNet) ? TElemBlockage : 0);
    setFlags( flags );

    if (track) {
      uint32_t      depth      = track->getDepth();
      Technology*   technology = DataBase::getDB()->getTechnology();
      const Layer*  layer1     = track->getLayer()->getBlockageLayer();
      RegularLayer* layer2     = dynamic_cast<RegularLayer*>(technology->getLayer(layer1->getMask()));
      if ( layer2 ) {
        Interval uside  = track->getKatanaEngine()->getUSide( track->getDirection() );
        Interval segside;
        if (track->getDirection() == Flags::Horizontal) {
          segside  = Interval( boundingBox.getXMin(), boundingBox.getXMax() );
          _sourceU = max( boundingBox.getXMin(), uside.getVMin());
          _targetU = min( boundingBox.getXMax(), uside.getVMax());
        } else {
          segside  = Interval( boundingBox.getYMin(), boundingBox.getYMax() );
          _sourceU = max( boundingBox.getYMin(), uside.getVMin());
          _targetU = min( boundingBox.getYMax(), uside.getVMax());
        }

        GCellsUnder gcells = track->getKatanaEngine()->getGCellsUnder( segment );
        for ( size_t i=0 ; i<gcells->size() ; ++i ) {
          GCell* gcell = gcells->gcellAt(i);
          gcell->addBlockage
            ( depth, gcell->getSide( track->getDirection() ).getIntersection( segside ).getSize() );
        }
      }
    }
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
    if ( not _blockageNet ) _blockageNet = Session::getBlockageNet();

    TrackFixedSegment* trackFixedSegment = NULL;
    if (track) { 
      trackFixedSegment = new TrackFixedSegment ( track, segment );
      trackFixedSegment->_postCreate();

      cdebug_log(159,0) << "Adding: " << segment << " on " << track << endl;
      cdebug_log(159,0) << "TrackFixedSegment::create(): " << trackFixedSegment << endl;

      Session::addInsertEvent( trackFixedSegment, track );
    }
    return trackFixedSegment;
  }


  AutoSegment*   TrackFixedSegment::base            () const { return NULL; }
  Segment*       TrackFixedSegment::getSegment      () const { return _segment; }
  DbU::Unit      TrackFixedSegment::getAxis         () const { return getTrack()->getAxis(); }
  bool           TrackFixedSegment::isHorizontal    () const { return getTrack()->isHorizontal(); }
  bool           TrackFixedSegment::isVertical      () const { return getTrack()->isVertical(); }
  bool           TrackFixedSegment::isFixed         () const { return true; }
  Flags          TrackFixedSegment::getDirection    () const { return getTrack()->getDirection(); }
  const Layer*   TrackFixedSegment::getLayer        () const { return _segment->getLayer(); }
  Interval       TrackFixedSegment::getFreeInterval () const { return Interval(); }


  unsigned long  TrackFixedSegment::getId () const
  {
    cerr << Error("::getId() called on %s.",_getString().c_str()) << endl;
    return 0;
  }


  Net* TrackFixedSegment::getNet () const
  {
    Net* realNet = _segment->getNet();
    if (realNet->isSupply() or realNet->isClock())
      return _blockageNet;
    return realNet;
  }


  TrackElement* TrackFixedSegment::getNext () const
  {
    size_t dummy = _index;
    return _track->getNext( dummy, getNet() );
  }


  TrackElement* TrackFixedSegment::getPrevious () const
  {
    size_t dummy = _index;
    return _track->getPrevious( dummy, getNet() );
  }


  float  TrackFixedSegment::getPriority () const
  { return 0.0; }


  void  TrackFixedSegment::updatePriority ( float )
  { }


  string  TrackFixedSegment::_getTypeName () const
  { return "TrackFixedSegment"; }


  string  TrackFixedSegment::_getString () const
  {
    string s1 = _segment->_getString();
    string s2 = " ["   + DbU::getValueString(_sourceU)
              +  ":"   + DbU::getValueString(_targetU) + "]"
              +  " "   + DbU::getValueString(_targetU-_sourceU)
              + " ["   + ((_track) ? getString(_index) : "npos") + "] "
              + "F"
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
