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
#include "hurricane/DebugSession.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Occurrence.h"
#include "hurricane/Cell.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/NetRoutingProperty.h"
#include "crlcore/Catalog.h"
#include "anabatic/AutoContact.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/GCell.h"
#include "anabatic/AnabaticEngine.h"
#include "katana/RoutingPlane.h"
#include "katana/TrackSegment.h"
#include "katana/TrackFixedSegment.h"
#include "katana/TrackFixedSpan.h"
#include "katana/Track.h"
#include "katana/KatanaEngine.h"


namespace {

  using namespace std;
  using Hurricane::Error;
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
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
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
  using Anabatic::AutoSegment;
  using namespace Katana;


  void  protectRoutingPad ( RoutingPad* rp, Flags flags )
  {
    cdebug_log(145,1) << "::protectRoutingPad() " << rp << endl;
    
    Name            padNetName     = "pad";
    Component*      usedComponent  = rp->_getEntityAs<Component>();
    Path            path           = rp->getOccurrence().getPath();
    Net*            net            = rp->getNet();
    Net*            masterNet      = usedComponent->getNet();
    Transformation  transformation = path.getTransformation();

    if (dynamic_cast<Plug*>(usedComponent)) {
      cerr << Error( "Katana::protectRoutingPad(): A RoutingPad of \"%s\" is still on it's Plug.\n"
                     "        (%s)"
                   , getString(net->getName()).c_str()
                   , getString(usedComponent).c_str()
                   ) << endl;
      cdebug_tabw(145,-1);
      return;
    }
    RoutingLayerGauge* rlg = Session::getLayerGauge( rp->getLayer() );
    if (not (  (rlg->getType() == Constant::Default  )
            or (rlg->getType() == Constant::LocalOnly))) {
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

    vector<Segment*> segments;

    for ( Segment* segment : masterNet->getSegments() ) {
      RoutingPlane* plane = Session::getKatanaEngine()->getRoutingPlaneByLayer(segment->getLayer());
      if (not plane) continue;

      if (usedComponent == dynamic_cast<Component*>(segment)
         and not (flags & Flags::ProtectSelf)) continue;
      if (not NetExternalComponents::isExternal(segment)) continue;

      segments.push_back( segment );
    }

    for ( size_t i=0 ; i<segments.size() ; ++i ) {
      RoutingPlane* plane     = Session::getKatanaEngine()->getRoutingPlaneByLayer(segments[i]->getLayer());
      Flags         direction = plane->getDirection();
      DbU::Unit     wireWidth = plane->getLayerGauge()->getWireWidth();
      DbU::Unit     delta     =   plane->getLayerGauge()->getPitch()
                                - wireWidth/2
                                - DbU::fromLambda(0.1);
      DbU::Unit     cap       = plane->getLayer()->getMinimalSpacing() / 2;
      DbU::Unit     extension = segments[i]->getLayer()->getExtentionCap();
      Box           bb        ( segments[i]->getBoundingBox() );

      transformation.applyOn ( bb );

      cdebug_log(145,0) << "@ " << segments[i] << " bb:" << bb << endl;
      cdebug_log(145,0) << "delta=" << DbU::getValueString(delta) << endl;
      cdebug_log(145,0) << "cap=" << DbU::getValueString(cap) << endl;

      if ( direction == Flags::Horizontal ) {
        DbU::Unit axisMin = bb.getYMin() - delta;
        DbU::Unit axisMax = bb.getYMax() + delta;

        Track* track = plane->getTrackByPosition ( axisMin, Constant::Superior );
        for ( ; track and (track->getAxis() <= axisMax) ; track = track->getNextTrack() ) {
          cdebug_log(145,0) << "> track " << track << endl;
          // Horizontal* segment = Horizontal::create ( net
          //                                          , segments[i]->getLayer()
          //                                          , track->getAxis()
          //                                          , wireWidth
          //                                          , bb.getXMin()+extension
          //                                          , bb.getXMax()-extension
          //                                          );
          // TrackFixedSegment::create ( track, segment );
          // cdebug_log(145,0) << "| " << segment << endl;
          Interval termSpan = Interval( bb.getXMin(), bb.getXMax() ).inflate( cap );
          Interval freeSpan = track->getFreeInterval( bb.getXCenter() );
          cdebug_log(145,0) << "| termSpan " << termSpan << endl;
          cdebug_log(145,0) << "| freeSpan " << freeSpan << endl;
          bool overlap  = false;
          bool conflict = false;
          if (not freeSpan.contains(termSpan)) {
            overlap = true;
            size_t ovBegin = 0;
            size_t ovEnd   = 0;
            track->getOverlapBounds( termSpan, ovBegin, ovEnd );
            for ( ; ovBegin <= ovEnd ; ++ovBegin ) {
              TrackElement*   overlaped = track->getSegment( ovBegin );
              TrackFixedSpan* fixedSpan = dynamic_cast<TrackFixedSpan*>( overlaped );
              if (fixedSpan) {
                if (not fixedSpan->isBlockage())
                  fixedSpan->setNet( nullptr );
              } else
                conflict = true;
            }
          }
          if (not conflict) {
            TrackFixedSpan* element = TrackFixedSpan::create( ((overlap) ? nullptr : net), bb, track );
            cdebug_log(145,0) << "| " << element << endl;
          }
        }
      } else {
        DbU::Unit axisMin = bb.getXMin() - delta;
        DbU::Unit axisMax = bb.getXMax() + delta;

        Track* track = plane->getTrackByPosition ( axisMin, Constant::Superior );
        for ( ; track and (track->getAxis() <= axisMax) ; track = track->getNextTrack() ) {
          // Vertical* segment = Vertical::create ( net
          //                                      , segments[i]->getLayer()
          //                                      , track->getAxis()
          //                                      , wireWidth
          //                                      , bb.getYMin()+extension
          //                                      , bb.getYMax()-extension
          //                                      );
          // TrackFixedSegment::create ( track, segment );
          // cdebug_log(145,0) << "| " << segment << endl;
          Interval termSpan = Interval( bb.getYMin(), bb.getYMax() );
          Interval freeSpan = track->getFreeInterval( bb.getYCenter() );
          bool     overlap  = false;
          bool     conflict = false;
          if (not freeSpan.contains(termSpan)) {
            TrackElement* overlaped = track->getSegment( bb.getXCenter() );
            if (overlaped) {
              overlap = true;
              TrackFixedSpan* fixedSpan = dynamic_cast<TrackFixedSpan*>( overlaped );
              if (fixedSpan) {
                if (not fixedSpan->isBlockage())
                  fixedSpan->setNet( nullptr );
              } else
                conflict = true;
            }
          }
          if (not conflict) {
            TrackFixedSpan* element = TrackFixedSpan::create( ((overlap) ? nullptr : net), bb, track );
            cdebug_log(145,0) << "| " << element << endl;
          }
        }
      }
    }
    Session::revalidate();
    cdebug_tabw(145,-1);
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

    Session::close();
    cerr.flush();
    cout.flush();
  }


}  // Katana namespace.
