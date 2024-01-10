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
#include "hurricane/Rectilinear.h"
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

    vector< pair<Box,const Layer*> > bbs;
    for ( Component* component : masterNet->getComponents() ) {
      const Layer*  layer = component->getLayer();
      RoutingPlane* plane = Session::getKatanaEngine()->getRoutingPlaneByLayer( layer );
      if (not plane) continue;

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
      DbU::Unit     wireWidth = plane->getLayerGauge()->getWireWidth();
      DbU::Unit     delta     =   plane->getLayerGauge()->getPitch()
                                - wireWidth/2
                                - DbU::fromLambda(0.1);
      DbU::Unit     cap       = plane->getLayer()->getMinimalSpacing() / 2;
      DbU::Unit     extension = item.second->getExtentionCap();
      Box           bb        ( item.first );

      cdebug_log(145,0) << "@ bb:"  << bb << endl;
      cdebug_log(145,0) << "delta=" << DbU::getValueString(delta) << endl;
      cdebug_log(145,0) << "cap="   << DbU::getValueString(cap) << endl;

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
        Interval freeSpan = track->getFreeInterval( intervals[i].getCenter() );
        cdebug_log(145,0) << "| termSpan " << termSpan << endl;
        cdebug_log(145,0) << "| freeSpan " << freeSpan << endl;
        bool overlap  = false;
        bool conflict = false;
        if (freeSpan.isEmpty() or not freeSpan.contains(termSpan)) {
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
          TrackFixedSpan* element = TrackFixedSpan::create( ((overlap) ? nullptr : net), bb, track );
          cdebug_log(145,0) << "| " << element << endl;
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

    //DebugSession::open( net, 145, 150 );
      cdebug_log(145,0) << "Protect RoutingPads of " << net << endl;

      NetData* data = getNetData( net );
      if (data and data->isFixed()) continue;

      vector<RoutingPad*> rps;
      for ( RoutingPad* rp : net->getRoutingPads() )
        rps.push_back( rp );

      for ( size_t i=0 ; i<rps.size() ; ++i )
        protectRoutingPad( rps[i], flags );

    //DebugSession::close();
    }

    Session::close();
    cerr.flush();
    cout.flush();
  }


}  // Katana namespace.
