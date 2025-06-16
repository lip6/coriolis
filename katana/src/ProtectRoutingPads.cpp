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
// |  C++ Module  :       "./ProtectRoutingPads.cpp"                 |
// +-----------------------------------------------------------------+


#include <map>
#include <list>
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DebugSession.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Pin.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Rectilinear.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Occurrence.h"
#include "hurricane/Cell.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/NetRoutingProperty.h"
#include "crlcore/Catalog.h"
#include "anabatic/AutoContactTerminal.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/GCell.h"
#include "anabatic/AnabaticEngine.h"
#include "katana/RoutingPlane.h"
#include "katana/TrackSegment.h"
#include "katana/TrackFixedSegment.h"
#include "katana/TrackFixedSpan.h"
#include "katana/TrackFixedSpanRp.h"
#include "katana/Track.h"
#include "katana/KatanaEngine.h"


namespace {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DebugSession;
  using Hurricane::tab;
  using Hurricane::ForEachIterator;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Interval;
  using Hurricane::Go;
  using Hurricane::Layer;
  using Hurricane::BasicLayer;
  using Hurricane::RegularLayer;
  using Hurricane::Pin;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::Rectilinear;
  using Hurricane::Plug;
  using Hurricane::Transformation;
  using Hurricane::RoutingPad;
  using Hurricane::Occurrence;
  using Hurricane::Path;
  using Hurricane::NetExternalComponents;
  using Hurricane::NetRoutingExtension;
  using Hurricane::NetRoutingState;
  using CRL::CatalogExtension;
  using Anabatic::AutoContact;
  using Anabatic::AutoContactTerminal;
  using Anabatic::AutoSegment;
  using namespace Katana;


  class CompareIntervalByVMin {
    public:
      inline bool operator () ( const Interval& lhs, const Interval& rhs ) const
      {
        if (lhs.getVMin() < rhs.getVMin())
          return true;
        else {
          if (   (lhs.getVMin() == rhs.getVMin())
             and (lhs.getVMax() >  rhs.getVMax()) )
            return true;
        }
        return false;
      }
  };


  class CompareTrackByAxis {
    public:
      inline bool operator () ( const Track* lhs, const Track* rhs ) const
      { return lhs->getAxis() < rhs->getAxis(); }
  };


  void  checkForLoopHV ( const vector<AutoContactTerminal*>& terminals )
  {
    AutoContactTerminal* prefTerm    = nullptr;
    AutoContactTerminal* nonprefTerm = nullptr;
    AutoSegment*         pref        = nullptr;
    AutoSegment*         nonpref     = nullptr;
    for ( AutoContactTerminal* terminal : terminals ) {
      if (terminal->getSegment()->isHorizontal()) {
        prefTerm = terminal;
        pref     = terminal->getSegment();
      } else {
        nonprefTerm = terminal;
        nonpref     = terminal->getSegment();
      }
    }
    if (not pref or not nonpref) return;
    
    AutoSegment* ppPref    =    pref->getOppositeAnchor(    prefTerm )->getPerpandicular(    pref ); 
    AutoSegment* ppNonpref = nonpref->getOppositeAnchor( nonprefTerm )->getPerpandicular( nonpref ); 
    if (not ppPref or not ppNonpref) return;

    if (ppPref->isReduced()) {
      cdebug_log(145,0) << "Loop suppresion on " << ppPref << endl;
      ppPref->setAxis( nonpref->getAxis() );
    }
  }


  void  postProtectRoutingPadHV ( RoutingPad* rp )
  {
    cdebug_log(145,1) << "::postProtectRoutingPadHV() " << rp << endl;

    RoutingGauge* rg         = Session::getRoutingGauge();
    RoutingPlane* planeM2    = Session::getKatanaEngine()->getRoutingPlaneByIndex( 1 );
    DbU::Unit     m2spacing  = planeM2->getLayer()->getMinimalSpacing();
    const Layer*  via12      = Session::getContactLayer( 0 );
    DbU::Unit     viaVShrink = rg->getViaWidth( (size_t)0 )/2 + via12->getBottomEnclosure( Layer::EnclosureV );
    Box           bb         = rp->getBoundingBox();

    bb.inflate( m2spacing/2, -viaVShrink );
    Track*        track      = planeM2->getTrackByPosition( bb.getYMin(), Constant::Superior );

    Track*   trackFree  = nullptr;
    size_t   freeAccess = 0;
    Interval rpInterval ( bb.getXMin(), bb.getXMax() );
    for ( ; track and (track->getAxis() <= bb.getYMax()) ; track = track->getNextTrack() ) {
      Interval freeInterval = track->getFreeInterval( bb.getXCenter(), rp->getNet() );
      if (not freeInterval.isEmpty() and freeInterval.contains(rpInterval)) {
        freeAccess++;
        if (not trackFree) trackFree = track;
      }
    }

    if (freeAccess < 2) {
      cdebug_log(145,0) << "RoutingPad is almost fully obstructed" << endl;
      if (trackFree) {
        Box metal2bb ( bb.getXMin(), trackFree->getAxis()-1, bb.getXMax(), trackFree->getAxis()+1 );
        TrackFixedSpanRp* element = TrackFixedSpanRp::create( rp, metal2bb, trackFree );
        cdebug_log(145,0) << "| " << element << endl;
      }
    }
    
    cdebug_tabw(145,-1);
  }


  void  protectRoutingPadHV ( RoutingPad* rp )
  {
    cdebug_log(145,1) << "::protectRoutingPadHV() " << rp << endl;

    RoutingPlane* planeM1 = Session::getKatanaEngine()->getRoutingPlaneByIndex( 0 );
    RoutingPlane* planeM2 = Session::getKatanaEngine()->getRoutingPlaneByIndex( 1 );
    DbU::Unit     pitch   = planeM2->getLayerGauge()->getPitch();
    Box           bb      = rp->getBoundingBox();
    if (not rp->isVSmall() and (bb.getWidth() < 3*pitch)) {
      cdebug_tabw(145,-1);
      return;
    }
    // if (not rp->isPunctual()) {
    //   cdebug_tabw(145,-1);
    //   return;
    // }

    DbU::Unit     m1spacing      = planeM1->getLayer()->getMinimalSpacing();
    DbU::Unit     m2spacing      = planeM2->getLayer()->getMinimalSpacing();
    Track*        track          = planeM2->getTrackByPosition( bb.getYCenter(), Constant::Nearest );
    DbU::Unit     halfViaBotSide = AutoSegment::getViaToBottomCap( 1 );
    DbU::Unit     halfViaTopSide = AutoSegment::getViaToTopCap( 1 );
    Box           metal2bb       = Box( bb.getXMin(), bb.getYCenter(), bb.getXMax(), bb.getYCenter() );

    if (bb.getWidth() < pitch)
    //metal2bb.inflate( halfViaTopSide - halfViaBotSide, 0 );
      metal2bb.inflate( (m1spacing - m2spacing)/2, 0 );
    else
      metal2bb.inflate( - m2spacing / 2, 0 );

    // Box           metal2bb       = Box( bb.getXMin() - halfViaTopSide + halfViaBotSide
    //                                   , bb.getYCenter()
    //                                   , bb.getXMax() + halfViaTopSide - halfViaBotSide
    //                                   , bb.getYCenter()
    //                                   );

  //bb.inflate( 0, Session::getLayerGauge((size_t)1)->getPitch() );
    TrackFixedSpanRp* element = TrackFixedSpanRp::create( rp, metal2bb, track );
    cdebug_log(145,0) << "halfViaTopSide=" << DbU::getValueString(halfViaTopSide) << endl;
    cdebug_log(145,0) << "halfViaBotSide=" << DbU::getValueString(halfViaBotSide) << endl;
    cdebug_log(145,0) << "| " << element << endl;
    
    cdebug_tabw(145,-1);
  }


  void  protectRoutingPadVH ( RoutingPad* rp )
  {
    cdebug_log(145,1) << "::protectRoutingPadVH() " << rp << endl;
    if (not rp->isPunctual()) {
      cdebug_tabw(145,-1);
      return;
    }

    RoutingPlane* plane       = Session::getKatanaEngine()->getRoutingPlaneByIndex( 1 );
    Box           bb          = rp->getBoundingBox();
    Track*        track       = plane->getTrackByPosition( bb.getXCenter(), Constant::Nearest );
    DbU::Unit     halfViaSide = AutoSegment::getViaToTopCap( 0 );
    Box           metal2bb    = Box( bb.getXMin()
                                   , bb.getYCenter() - halfViaSide
                                   , bb.getXMax()
                                   , bb.getYCenter() + halfViaSide
                                   );

  //bb.inflate( 0, Session::getLayerGauge((size_t)1)->getPitch() );
    TrackFixedSpan* element = TrackFixedSpan::create( rp->getNet(), metal2bb, track );
    cdebug_log(145,0) << "halfViaSside=" << DbU::getValueString(halfViaSide) << endl;
    cdebug_log(145,0) << "| " << element << endl;
    
    cdebug_tabw(145,-1);
    return;
  }


  void  protectRoutingPad ( RoutingPad* rp, Flags flags )
  {
    cdebug_log(145,1) << "::protectRoutingPad() " << rp << endl;
    
    Name            padNetName     = "pad";
    Component*      usedComponent  = rp->_getEntityAs<Component>();
    Path            path           = rp->getOccurrence().getPath();
    Net*            net            = rp->getNet();
    Net*            masterNet      = usedComponent->getNet();
    Transformation  transformation = path.getTransformation();
    Box             ab             = rp->getCell()->getAbutmentBox();

    if (dynamic_cast<Plug*>(usedComponent)) {
      cerr << Error( "Katana::protectRoutingPad(): A RoutingPad of \"%s\" is still on it's Plug.\n"
                     "        (%s)"
                   , getString(net->getName()).c_str()
                   , getString(usedComponent).c_str()
                   ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    cdebug_log(145,0) << "masterCell: " << masterNet->getCell() << endl;
    cdebug_log(145,0) << "masterNet:  " << masterNet << endl;

    if (CatalogExtension::isPad(masterNet->getCell())) {
      if (   net->isPower()
         or (net->getName() == padNetName) ) {
        cdebug_tabw(145,-1);
        return;
      }
    }

    RoutingLayerGauge* rlg     = Session::getLayerGauge( rp->getLayer() );
    size_t             rpDepth = rlg->getDepth();
    if (rlg->getType() == Constant::PinOnly) {
      if (rpDepth == 0) {
        if (Session::getRoutingGauge()->isVH()) protectRoutingPadVH( rp );
        else                                    protectRoutingPadHV( rp );
      } 
      cdebug_tabw(145,-1);
      return;
    }
    if (not (  (rlg->getType() == Constant::Default  )
            or (rlg->getType() == Constant::LocalOnly))) {
      cdebug_tabw(145,-1);
      return;
    }

    vector< pair<Box,const Layer*> > bbs;
    for ( Component* component : masterNet->getComponents() ) {
      const Layer*  layer = component->getLayer();
      RoutingPlane* plane = Session::getKatanaEngine()->getRoutingPlaneByLayer( layer );
      if (not plane) continue;

      Pin* pin = dynamic_cast<Pin*>( component );
      if (pin) {
        Box bb = pin->getBoundingBox();
        transformation.applyOn( bb );
        bbs.push_back( make_pair( bb, layer ));
        cdebug_log(145,0) << "@ " << pin << " bb:" << bb << endl;
        continue;
      }

      Segment* segment = dynamic_cast<Segment*>( component );
      if (segment) {
        Box bb = segment->getBoundingBox();
        transformation.applyOn( bb );
        bbs.push_back( make_pair( bb, layer ));
        cdebug_log(145,0) << "@ " << segment << " bb:" << bb << endl;
        continue;
      }

      Rectilinear* rectilinear = dynamic_cast<Rectilinear*>( component );
      if (rectilinear) {
        cdebug_log(145,0) << "@ " << rectilinear << endl;
        vector<Box> rbbs;
        rectilinear->getAsRectangles( rbbs, (plane->isVertical() ? Rectilinear::HSliced
                                                                 : Rectilinear::VSliced ));
        for ( Box& bb : rbbs ) {
          transformation.applyOn( bb );
          bbs.push_back( make_pair( bb, layer ));
          cdebug_log(145,0) << "| bb:" << bb << endl;
        }
        continue;
      }
    }

    map<Track*, vector<Interval>, CompareTrackByAxis > intervalsByTracks;
    for ( auto item : bbs ) {
      RoutingPlane* plane     = Session::getKatanaEngine()->getRoutingPlaneByLayer( item.second );
      Flags         direction = plane->getDirection();
      DbU::Unit     ppitch    = plane->getLayerGauge()->getPitch();
      DbU::Unit     wireWidth = plane->getLayerGauge()->getWireWidth();
      DbU::Unit     delta     =   plane->getLayerGauge()->getPitch()
                                - wireWidth/2
                                - DbU::fromLambda(0.1);
      DbU::Unit     cap       = plane->getLayer()->getMinimalSpacing() / 2;
      DbU::Unit     extension = item.second->getExtentionCap();
      Box           bb        = item.first.getIntersection( ab );

      cdebug_log(145,0) << "@ bb:"  << bb << endl;
      cdebug_log(145,0) << "delta=" << DbU::getValueString(delta) << endl;
      cdebug_log(145,0) << "cap="   << DbU::getValueString(cap) << endl;
      if (bb.isEmpty()) continue;

      if (direction == Flags::Horizontal) {
        DbU::Unit axisMin = bb.getYMin() - delta;
        DbU::Unit axisMax = bb.getYMax() + delta;

        Track* track = plane->getTrackByPosition( axisMin, Constant::Superior );
        for ( ; track and (track->getAxis() <= axisMax) ; track = track->getNextTrack() ) {
          cdebug_log(145,0) << "> track " << track << endl;
          intervalsByTracks[ track ].push_back( Interval( bb.getXMin(), bb.getXMax() ));
        }
      } else {
        DbU::Unit axisMin = bb.getXMin() - delta;
        DbU::Unit axisMax = bb.getXMax() + delta;

        Track* track = plane->getTrackByPosition( axisMin, Constant::Superior );
        for ( ; track and (track->getAxis() <= axisMax) ; track = track->getNextTrack() ) {
          cdebug_log(145,0) << "> track " << track << endl;
          intervalsByTracks[ track ].push_back( Interval( bb.getYMin(), bb.getYMax() ) );
        }
      }
    }

    for ( auto item : intervalsByTracks ) {
      Track*&           track     = const_cast<Track*&>( item.first );
      vector<Interval>& intervals = item.second;
      DbU::Unit         cap       = track->getLayer()->getMinimalSpacing() / 2;
      DbU::Unit         halfWidth = track->getLayer()->getMinimalSize   () / 2;
      cdebug_log(145,0) << "> track " << track << endl;

      sort( intervals.begin(), intervals.end(), CompareIntervalByVMin() );
      Interval fullSpan = intervals[0];
      cdebug_log(145,0) << "| fullSpan " << fullSpan << endl;
      for ( size_t i=1 ; i < intervals.size() ; ) {
        if (fullSpan.contains(intervals[i])) {
          cdebug_log(145,0) << "| contained " << intervals[i] << endl;
          intervals.erase( intervals.begin() + i );
          continue;
        }
        if (fullSpan.intersect(intervals[i])) {
          cdebug_log(145,0) << "| intersect " << intervals[i] << endl;
          fullSpan.merge( intervals[i].getVMax() );
          cdebug_log(145,0) << "| fullSpan " << fullSpan << endl;
          if (  (intervals[i  ].getSize() < 2*cap)
             or (intervals[i-1].getSize() < 2*cap)) {
            cdebug_log(145,0) << "| too narrow, merge " << endl;
            intervals[i-1].merge( fullSpan.getVMax() );
            intervals.erase( intervals.begin() + i );
            continue;
          }
          cdebug_log(145,0) << "| separate" << endl;
          intervals[i-1] = Interval( intervals[i-1].getVMin()      , intervals[i].getVMin() - cap );
          intervals[i  ] = Interval( intervals[i  ].getVMin() + cap, intervals[i].getVMax() );
          ++i;
          continue;
        }
        if (fullSpan.getVMax() > intervals[i-1].getVMax()) {
          cdebug_log(145,0) << "| expand last" << endl;
          intervals[i-1].merge( fullSpan.getVMax());
        }
        
        fullSpan = intervals[i];
        ++i;
      }

      cdebug_log(145,0) << "Cleaned up on " << track << endl;
      for ( size_t i=0 ; i<intervals.size() ; ++i ) {
        cdebug_log(145,0) << "| " << intervals[i] << endl;
      }
      cdebug_log(145,0) << "end" << endl;

      for ( size_t i=0 ; i<intervals.size() ; ++i ) {
        Interval termSpan = Interval( intervals[i] ).inflate( cap );
        Interval freeSpan = track->getFreeInterval( intervals[i].getCenter(), net );
        cdebug_log(145,0) << "| termSpan " << termSpan << endl;
        cdebug_log(145,0) << "| freeSpan " << freeSpan << endl;
        bool overlap  = false;
        bool conflict = false;
        if (freeSpan.isEmpty() or not freeSpan.contains(termSpan)) {
          overlap = true;
          size_t ovBegin = 0;
          size_t ovEnd   = 0;
          track->getOverlapBounds( termSpan, ovBegin, ovEnd );
          if (ovBegin != Track::npos) {
            for ( ; ovBegin <= ovEnd ; ++ovBegin ) {
              TrackElement*       overlaped = track->getSegment( ovBegin );
              TrackBaseFixedSpan* fixedSpan = dynamic_cast<TrackBaseFixedSpan*>( overlaped );
              if (fixedSpan) {
                if (not fixedSpan->isBlockage())
                  fixedSpan->setNet( nullptr );
              } else
                conflict = true;
            }
          }
        }
        cdebug_log(145,0) << "| overlap =" << overlap << endl;
        cdebug_log(145,0) << "| conflict=" << conflict << endl;
        if (not conflict) {
          Box bb;
          if (track->isHorizontal())
            bb = Box( intervals[i].getVMin(), track->getAxis()-halfWidth
                    , intervals[i].getVMax(), track->getAxis()+halfWidth );
          else
            bb = Box( track->getAxis()-halfWidth, intervals[i].getVMin()
                    , track->getAxis()+halfWidth, intervals[i].getVMax() );
          TrackBaseFixedSpan* element = nullptr;
          if (rp->isPunctual() and not overlap)
            element = TrackFixedSpanRp::create( rp, bb, track );
          else
            element = TrackFixedSpan::create( ((overlap) ? nullptr : net), bb, track );
          cdebug_log(145,0) << "| " << element << endl;
        }

      }
    }
    Session::revalidate();
    cdebug_tabw(145,-1);
  }

  

  void  postProcessRoutingPad ( RoutingPad* rp )
  {
    cdebug_log(145,1) << "Post-process " << rp << endl;

    RoutingPlane* planeM2 = Session::getKatanaEngine()->getRoutingPlaneByIndex( 1 );
    DbU::Unit     pitch   = planeM2->getLayerGauge()->getPitch();

    vector<AutoContactTerminal*> vias;
    for ( Contact* contact : rp->getSlaveComponents().getSubSet<Contact*>() ) {
      AutoContact* via = Session::lookup( contact );
      if (via)
        vias.push_back( dynamic_cast<AutoContactTerminal*>( via ));
    }
    if (vias.size() < 2) {
      cdebug_log(145,0) << "Less than 2 VIAs attached, skipping." << endl;
      cdebug_tabw(145,-1);
      return;
    }
    if (vias.size() > 3) {
      cerr << Warning( "postProcessRoutingPads(): More than three VIAs on %s."
                     , getString(rp).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    vector< vector<AutoContactTerminal*> >  overlappings;
    for ( AutoContactTerminal* via : vias ) {
      bool sorted = false;
      for ( auto& overlapping : overlappings ) {
        for ( AutoContactTerminal* sortedVia : overlapping ) {
          DbU::Unit dx = std::abs( sortedVia->getX() - via->getX() );
          DbU::Unit dy = std::abs( sortedVia->getY() - via->getY() );
          if ((dx < pitch) and (dy < pitch)) {
            cdebug_log(145,0) << "Overlapping: " << via << endl;
            overlapping.push_back( via );
            sorted = true;
            break;
          }
        }
        if (sorted) break;
      }
      if (sorted) continue;

      cdebug_log(145,0) << "Isolated: " << via << endl;
      overlappings.push_back( vector<AutoContactTerminal*>() );
      overlappings.back().push_back( via );
    }

    for ( auto overlapping : overlappings ) {
      if (overlapping.size() < 2) {
        cdebug_log(145,0) << "Less than 2 VIAs in the overlapping set, skipping." << endl;
        continue;
      }

      for ( size_t i=1 ; i<overlapping.size() ; ++i ) {
        BasicLayer* layer = overlapping[i]->getSegment()->getLayer()->getBasicLayers().getFirst();
        Box         viaBb = Box( overlapping[i]->base()->getBoundingBox( layer ));
        overlapping[i]->setLayer( overlapping[i]->getSegment()->getLayer() );
        overlapping[i]->setSizes( viaBb.getWidth(), viaBb.getHeight() );
      }

      checkForLoopHV( overlapping );
      // set<DbU::Unit> xs;
      // set<DbU::Unit> ys;
      
      // for ( AutoContactTerminal* via : overlapping ) {
      //   if (via->getSegment()->isHorizontal()) ys.insert( via->getY() );
      //   else xs.insert( via->getX() );
      // }
      // if (xs.size() > 1) {
      //   if (ys.empty()) {
      //     cdebug_log(145,0) << "Adjust by shitfing Verticals" << endl;
      //     for ( AutoContactTerminal* via : overlapping ) {
      //       via->getSegment()->setAxis( *(xs.begin()) );
      //       cdebug_log(145,0) << "Aligned: " << via << endl;
      //     }
      //   } else {
      //     cerr << Warning( "postProcessRoutingPad(): VIAs in diagonal (X), cannot merge them.\n"
      //                      "           On %s"
      //                    , getString(rp).c_str() ) << endl;
      //   }
      //   continue;
      // }
      // if (ys.size() > 1) {
      //   if (xs.empty()) {
      //     cdebug_log(145,0) << "Adjust by shitfing Horizontals" << endl;
      //     for ( AutoContactTerminal* via : overlapping ) {
      //       via->getSegment()->setAxis( *(ys.begin()) );
      //       cdebug_log(145,0) << "Aligned: " << via << endl;
      //     }
      //   } else {
      //     cerr << Warning( "postProcessRoutingPad(): VIAs in diagonal (Y), cannot merge them.\n"
      //                      "           On %s"
      //                    , getString(rp).c_str() ) << endl;
      //   }
      //   continue;
      // }

      // if (xs.empty()) xs.insert( overlapping[0]->getX() );
      // if (ys.empty()) ys.insert( overlapping[0]->getY() );

      // cdebug_log(145,0) << "Adjust position to (" << DbU::getValueString(*(xs.begin()))
      //                   <<                    "," << DbU::getValueString(*(ys.begin()))
      //                   <<                    ")" << endl;
      // for ( AutoContactTerminal* via : overlapping ) {
      //   via->setPosition( *(xs.begin()), *(ys.begin()) );
      //   via->unsetFlags( Anabatic::CntDrag );
      //   via->invalidate();
      //   cdebug_log(145,0) << "Aligned: " << via << endl;
      //
      // }
    }
  }
  

} // End of anonymous namespace.


namespace Katana {

  using Hurricane::DataBase;
  using Hurricane::Technology;
  using Hurricane::BasicLayer;
  using Hurricane::Cell;
  using Anabatic::NetData;


  void  KatanaEngine::protectRoutingPads ( Flags flags )
  {
    cmess1 << "  o  Protect external components not useds as RoutingPads." << endl;

    openSession();

    RoutingPlane* planeM1 = Session::getKatanaEngine()->getRoutingPlaneByIndex( 0 );
    RoutingPlane* planeM2 = Session::getKatanaEngine()->getRoutingPlaneByIndex( 1 );
    DbU::Unit     pitch   = planeM2->getLayerGauge()->getPitch();

    for ( Net* net : getCell()->getNets() ) {
      if (net->isSupply()) continue;

      DebugSession::open( net, 145, 150 );
      cdebug_log(145,0) << "Protect RoutingPads of " << net << endl;

      NetData* data = getNetData( net );
      if (data and data->isFixed()) continue;

      vector<RoutingPad*> rps;
      for ( RoutingPad* rp : net->getRoutingPads() )
        rps.push_back( rp );

      for ( size_t i=0 ; i<rps.size() ; ++i )
        protectRoutingPad( rps[i], flags );

      DebugSession::close();
    }

    if (not Session::getRoutingGauge()->isVH()) {
      for ( Net* net : getCell()->getNets() ) {
        if (net->isSupply()) continue;

        DebugSession::open( net, 145, 150 );
        cdebug_log(145,0) << "Protect RoutingPads of " << net << endl;

        NetData* data = getNetData( net );
        if (data and data->isFixed()) continue;

        vector<RoutingPad*> rps;
        for ( RoutingPad* rp : net->getRoutingPads() ) {
          Box  bb = rp->getBoundingBox();
          if (rp->isVSmall() or (bb.getWidth() > 3*pitch)) continue;
          rps.push_back( rp );
        }

        for ( size_t i=0 ; i<rps.size() ; ++i )
          postProtectRoutingPadHV( rps[i] );

        DebugSession::close();
      }
    }

    Session::close();
    cerr.flush();
    cout.flush();
  }


  void  KatanaEngine::_postProcessRoutingPads ()
  {
    cmess1 << "  o  Post-process RoutingPads." << endl;

    openSession();

    RoutingPlane* planeM1 = Session::getKatanaEngine()->getRoutingPlaneByIndex( 0 );
    RoutingPlane* planeM2 = Session::getKatanaEngine()->getRoutingPlaneByIndex( 1 );
    DbU::Unit     pitch   = planeM2->getLayerGauge()->getPitch();

    for ( Net* net : getCell()->getNets() ) {
      if (net->isSupply()) continue;

      DebugSession::open( net, 145, 150 );
      cdebug_log(145,0) << "Post-process RoutingPads of " << net << endl;

      NetData* data = getNetData( net );
      if (data and data->isFixed()) continue;

      vector<RoutingPad*> rps;
      for ( RoutingPad* rp : net->getRoutingPads() )
        rps.push_back( rp );

      for ( size_t i=0 ; i<rps.size() ; ++i )
        postProcessRoutingPad( rps[i] );

      DebugSession::close();
    }

    Session::close();
  }


}  // Katana namespace.
