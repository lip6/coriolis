
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
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
// |  C++ Module  :       "./TrackFixedSegment.cpp"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include  <sstream>

#include  "hurricane/Bug.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Net.h"
#include  "hurricane/Name.h"
#include  "hurricane/RegularLayer.h"
#include  "hurricane/Technology.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "katabatic/AutoContact.h"
#include  "crlcore/RoutingGauge.h"
#include  "kite/GCell.h"
#include  "kite/DataNegociate.h"
#include  "kite/TrackFixedSegment.h"
#include  "kite/TrackCost.h"
#include  "kite/Track.h"
#include  "kite/Session.h"
#include  "kite/RoutingEvent.h"
#include  "kite/NegociateWindow.h"
#include  "kite/GCellGrid.h"
#include  "kite/KiteEngine.h"


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

    if ( track ) {
      unsigned int  depth      = track->getDepth();
      Technology*   technology = DataBase::getDB()->getTechnology();
      const Layer*  layer1     = track->getLayer()->getBlockageLayer();
      RegularLayer* layer2     = dynamic_cast<RegularLayer*>(technology->getLayer(layer1->getMask()));
      if ( layer2 ) {
        DbU::Unit extention = layer2->getExtentionCap();
        if ( track->getDirection() == Constant::Horizontal ) {
          Interval uside = track->getKiteEngine()->getGCellGrid()->getUSide ( Constant::Horizontal );

          _sourceU = max ( boundingBox.getXMin()-extention, uside.getVMin());
          _targetU = min ( boundingBox.getXMax()+extention, uside.getVMax());

          GCell*   gcell   = track->getKiteEngine()->getGCellGrid()->getGCell ( Point(_sourceU,track->getAxis()) );
          GCell*   end     = track->getKiteEngine()->getGCellGrid()->getGCell ( Point(_targetU,track->getAxis()) );
          GCell*   right   = NULL;
          Interval guside  = gcell->getUSide ( Constant::Horizontal, true );
          Interval segside ( boundingBox.getXMin(), boundingBox.getXMax() );

          if ( gcell ) {
            while ( gcell and (gcell != end) ) {
              right = gcell->getRight();
              if ( right == NULL ) break;

              guside = gcell->getUSide ( Constant::Horizontal, true );
              Interval usedLength = guside.getIntersection ( segside );

              gcell->addBlockage ( depth, (float)usedLength.getSize()/(float)guside.getSize() );
              gcell = right;
            }
            if ( end ) {
              guside = gcell->getUSide ( Constant::Horizontal, true );
              Interval usedLength = guside.getIntersection ( segside );

              end->addBlockage ( depth, (float)usedLength.getSize()/(float)guside.getSize() );
            }
          } else
            cerr << Warning("TrackFixedSegment(): TrackFixedElement outside GCell grid.") << endl;
        } else {
          Interval uside = track->getKiteEngine()->getGCellGrid()->getUSide ( Constant::Vertical );

          _sourceU = max ( boundingBox.getYMin()-extention, uside.getVMin());
          _targetU = min ( boundingBox.getYMax()+extention, uside.getVMax());

          GCell*   gcell      = track->getKiteEngine()->getGCellGrid()->getGCell ( Point(track->getAxis(),_sourceU) );
          GCell*   end        = track->getKiteEngine()->getGCellGrid()->getGCell ( Point(track->getAxis(),_targetU) );
          GCell*   up         = NULL;
          Interval guside     = gcell->getUSide ( Constant::Vertical, true );
          Interval segside    ( boundingBox.getYMin(), boundingBox.getYMax() );
          if ( gcell ) {
            while ( gcell and (gcell != end) ) {
              up = gcell->getUp();
              if ( up == NULL ) break;

              guside = gcell->getUSide ( Constant::Vertical, true );
              Interval usedLength = guside.getIntersection ( segside );

              gcell->addBlockage ( depth, (float)usedLength.getSize()/(float)guside.getSize() );
              gcell = up;
            }
            if ( end ) {
              guside = gcell->getUSide ( Constant::Vertical, true );
              Interval usedLength = guside.getIntersection ( segside );

              end->addBlockage ( depth, (float)usedLength.getSize()/(float)guside.getSize() );
            }
          } else
            cerr << Warning("TrackFixedSegment(): TrackFixedElement outside GCell grid.") << endl;
        }
      }
    }
  }


  void  TrackFixedSegment::_postCreate ()
  { TrackElement::_postCreate (); }


  TrackFixedSegment::~TrackFixedSegment ()
  { }


  void  TrackFixedSegment::_preDestroy ()
  {
    ltrace(90) << "TrackFixedSegment::_preDestroy() - " << (void*)this << endl;
    TrackElement::_preDestroy ();
  }


  TrackElement* TrackFixedSegment::create ( Track* track, Segment* segment )
  {
    if ( not _blockageNet ) _blockageNet = Session::getBlockageNet();

    TrackFixedSegment* trackFixedSegment = NULL;
    if ( track ) { 
      trackFixedSegment = new TrackFixedSegment ( track, segment );
      trackFixedSegment->_postCreate ();
      Session::addInsertEvent ( trackFixedSegment, track );

      ltrace(190) << "Adding: " << segment << " on " << track << endl;
      ltrace(200) << "TrackFixedSegment::create(): " << trackFixedSegment << endl;
    }
    return trackFixedSegment;
  }


  AutoSegment*   TrackFixedSegment::base            () const { return NULL; }
  bool           TrackFixedSegment::isFixed         () const { return true; }
  bool           TrackFixedSegment::isBlockage      () const { return true; }
  DbU::Unit      TrackFixedSegment::getAxis         () const { return getTrack()->getAxis(); }
  bool           TrackFixedSegment::isHorizontal    () const { return getTrack()->isHorizontal(); }
  bool           TrackFixedSegment::isVertical      () const { return getTrack()->isVertical(); }
  unsigned int   TrackFixedSegment::getDirection    () const { return getTrack()->getDirection(); }
  const Layer*   TrackFixedSegment::getLayer        () const { return _segment->getLayer(); }
  Interval       TrackFixedSegment::getFreeInterval ( bool useOrder ) const { return Interval(); }


  unsigned long  TrackFixedSegment::getId () const
  {
    cerr << Error("::getId() called on %s.",_getString().c_str()) << endl;
    return 0;
  }


  Net* TrackFixedSegment::getNet () const
  {
    Net* realNet = _segment->getNet();
    if ( realNet->isSupply() or realNet->isClock() )
      return _blockageNet;
    return _segment->getNet();
  }


  TrackElement* TrackFixedSegment::getNext () const
  {
    size_t dummy = _index;
    return _track->getNext ( dummy, getNet() );
  }


  TrackElement* TrackFixedSegment::getPrevious () const
  {
    size_t dummy = _index;
    return _track->getPrevious ( dummy, getNet() );
  }


  string  TrackFixedSegment::_getTypeName () const
  { return "TrackFixedSegment"; }


  string  TrackFixedSegment::_getString () const
  {
    string s1 = _segment->_getString();
    string s2 = " ["   + DbU::getValueString(_sourceU)
              +  ":"   + DbU::getValueString(_targetU) + "]"
              +  " "   + DbU::getValueString(_targetU-_sourceU)
              + " ["   + ((_track) ? getString(_index) : "npos") + "]";
    s1.insert ( s1.size()-1, s2 );

    return s1;
  }


  Record* TrackFixedSegment::_getRecord () const
  {
    Record* record = TrackElement::_getRecord ();
    record->add ( getSlot ( "_segment", _segment ) );

    return record;
  }


} // End of Kite namespace.
