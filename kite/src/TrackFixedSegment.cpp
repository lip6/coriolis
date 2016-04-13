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
#include "katabatic/AutoContact.h"
#include "katabatic/GCellGrid.h"
#include "crlcore/RoutingGauge.h"
#include "kite/DataNegociate.h"
#include "kite/TrackFixedSegment.h"
#include "kite/TrackCost.h"
#include "kite/Track.h"
#include "kite/Session.h"
#include "kite/RoutingEvent.h"
#include "kite/NegociateWindow.h"
#include "kite/KiteEngine.h"


namespace Kite {

  using namespace std;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::tab;
  using Hurricane::Warning;
  using Hurricane::ForEachIterator;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Net;
  using Hurricane::Name;
  using Hurricane::RegularLayer;
  using Hurricane::Technology;
  using Hurricane::DataBase;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;


// -------------------------------------------------------------------
// Class  :  "TrackFixedSegment".


  Net* TrackFixedSegment::_blockageNet = NULL;


  TrackFixedSegment::TrackFixedSegment ( Track* track, Segment* segment )
    : TrackElement  (NULL)
    , _segment      (segment)
  {
    Box boundingBox = segment->getBoundingBox();

    unsigned int flags = TElemFixed | ((segment->getNet() == _blockageNet) ? TElemBlockage : 0);
    setFlags( flags );

    if (track) {
      unsigned int  depth      = track->getDepth();
      Technology*   technology = DataBase::getDB()->getTechnology();
      const Layer*  layer1     = track->getLayer()->getBlockageLayer();
      RegularLayer* layer2     = dynamic_cast<RegularLayer*>(technology->getLayer(layer1->getMask()));
      if ( layer2 ) {
      //DbU::Unit extention = layer2->getExtentionCap();
        if (track->getDirection() == KbHorizontal) {
          Interval uside = track->getKiteEngine()->getGCellGrid()->getUSide( KbHorizontal );

          _sourceU = max( boundingBox.getXMin(), uside.getVMin());
          _targetU = min( boundingBox.getXMax(), uside.getVMax());

          Katabatic::GCell* gcell   = track->getKiteEngine()->getGCellGrid()->getGCell( Point(_sourceU,track->getAxis()) );
          Katabatic::GCell* end     = track->getKiteEngine()->getGCellGrid()->getGCell( Point(_targetU,track->getAxis()) );
          Katabatic::GCell* right   = NULL;
          Interval          guside  = gcell->getSide( KbHorizontal );
          Interval          segside ( boundingBox.getXMin(), boundingBox.getXMax() );

          if (gcell) {
            while ( gcell and (gcell != end) ) {
              right = gcell->getRight();
              if (right == NULL) break;

              guside = gcell->getSide( KbHorizontal );
              Interval usedLength = guside.getIntersection( segside );

              gcell->addBlockage   ( depth, usedLength.getSize() );
            //gcell->addBlockedAxis( depth, track->getAxis() );
              gcell = right;
            }
            if (end) {
              guside = gcell->getSide( KbHorizontal );
              Interval usedLength = guside.getIntersection( segside );

              end->addBlockage   ( depth, usedLength.getSize() );
            //end->addBlockedAxis( depth, track->getAxis() );
            }
          } else
            cerr << Warning("TrackFixedSegment(): TrackFixedElement outside GCell grid.") << endl;
        } else {
          Interval uside = track->getKiteEngine()->getGCellGrid()->getUSide( KbVertical );

          _sourceU = max( boundingBox.getYMin(), uside.getVMin());
          _targetU = min( boundingBox.getYMax(), uside.getVMax());

          Katabatic::GCell* gcell = track->getKiteEngine()->getGCellGrid()->getGCell( Point(track->getAxis(),_sourceU) );
          if (gcell) {
            Katabatic::GCell* end     = track->getKiteEngine()->getGCellGrid()->getGCell( Point(track->getAxis(),_targetU) );
            Katabatic::GCell* up      = NULL;
            Interval          guside  = gcell->getSide( KbVertical );
            Interval          segside ( boundingBox.getYMin(), boundingBox.getYMax() );

            while ( gcell and (gcell != end) ) {
              up = gcell->getUp();
              if (up == NULL) break;

              guside = gcell->getSide( KbVertical );
              Interval usedLength = guside.getIntersection( segside );

              gcell->addBlockage   ( depth, usedLength.getSize() );
            //gcell->addBlockedAxis( depth, track->getAxis() );
              gcell = up;
            }
            if ( end ) {
              guside = gcell->getSide( KbVertical /*, true*/ );
              Interval usedLength = guside.getIntersection( segside );

              end->addBlockage   ( depth, usedLength.getSize() );
            //end->addBlockedAxis( depth, track->getAxis() );
            }
          } else
            cerr << Warning("TrackFixedSegment(): TrackFixedElement outside GCell grid.") << endl;
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
    ltrace(90) << "TrackFixedSegment::_preDestroy() - " << (void*)this << endl;
    TrackElement::_preDestroy();
  }


  TrackElement* TrackFixedSegment::create ( Track* track, Segment* segment )
  {
    if ( not _blockageNet ) _blockageNet = Session::getBlockageNet();

    TrackFixedSegment* trackFixedSegment = NULL;
    if (track) { 
      trackFixedSegment = new TrackFixedSegment ( track, segment );
      trackFixedSegment->_postCreate();

      ltrace(190) << "Adding: " << segment << " on " << track << endl;
      ltrace(200) << "TrackFixedSegment::create(): " << trackFixedSegment << endl;

      Session::addInsertEvent( trackFixedSegment, track );
    }
    return trackFixedSegment;
  }


  AutoSegment*   TrackFixedSegment::base            () const { return NULL; }
  DbU::Unit      TrackFixedSegment::getAxis         () const { return getTrack()->getAxis(); }
  bool           TrackFixedSegment::isHorizontal    () const { return getTrack()->isHorizontal(); }
  bool           TrackFixedSegment::isVertical      () const { return getTrack()->isVertical(); }
  bool           TrackFixedSegment::isFixed         () const { return true; }
  unsigned int   TrackFixedSegment::getDirection    () const { return getTrack()->getDirection(); }
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


} // Kite namespace.
