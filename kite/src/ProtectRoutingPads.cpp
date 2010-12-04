
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
// |  C++ Module  :       "./ProtectRoutingPads.cpp"                 |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <map>
#include  <list>

#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/BasicLayer.h"
#include  "hurricane/RegularLayer.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/RoutingPad.h"
#include  "hurricane/Occurrence.h"
#include  "hurricane/Cell.h"
#include  "hurricane/NetExternalComponents.h"
#include  "crlcore/Catalog.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoSegment.h"
#include  "katabatic/GCell.h"
#include  "katabatic/GCellGrid.h"
#include  "katabatic/KatabaticEngine.h"
#include  "kite/RoutingPlane.h"
#include  "kite/TrackSegment.h"
#include  "kite/TrackFixedSegment.h"
#include  "kite/Track.h"
#include  "kite/KiteEngine.h"


namespace {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::inltrace;
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
  using Hurricane::Transformation;
  using Hurricane::RoutingPad;
  using Hurricane::Occurrence;
  using Hurricane::Path;
  using Hurricane::NetExternalComponents;
  using CRL::CatalogExtension;
  using Katabatic::GCellGrid;
  using Katabatic::AutoContact;
  using Katabatic::AutoSegment;
  using namespace Kite;


  void  protectRoutingPad ( RoutingPad* rp )
  {
    Component*      usedComponent  = rp->_getEntityAsComponent();
    Path            path           = rp->getOccurrence().getPath();
    Net*            masterNet      = usedComponent->getNet();
    Transformation  transformation = path.getTransformation();

    if ( CatalogExtension::isPad(masterNet->getCell()) ) return;

    forEach ( Segment*, isegment, masterNet->getSegments() ) {
      RoutingPlane* plane = Session::getKiteEngine()->getRoutingPlaneByLayer(isegment->getLayer());
      if ( plane == NULL ) {
        continue;
      }

      unsigned int direction = plane->getDirection();
      DbU::Unit    wireWidth = plane->getLayerGauge()->getWireWidth();
      DbU::Unit    delta     =   plane->getLayerGauge()->getHalfPitch()
                               + wireWidth/2
                               - DbU::lambda(0.1);
      DbU::Unit    extension = isegment->getLayer()->getExtentionCap();

      if ( usedComponent == dynamic_cast<Component*>(*isegment) ) continue;
      if ( not NetExternalComponents::isExternal(*isegment) ) continue;

    //cinfo << "Protecting " << *isegment << endl;

      Box bb ( (*isegment)->getBoundingBox() );
      transformation.applyOn ( bb );

    //cinfo << "bb: " << bb << endl;

      if ( direction == Constant::Horizontal ) {
        DbU::Unit axisMin = bb.getYMin() - delta;
        DbU::Unit axisMax = bb.getYMax() + delta;

        Track* track = plane->getTrackByPosition ( axisMin, Constant::Superior );
        for ( ; track and (track->getAxis() <= axisMax) ; track = track->getNext() ) {
          Horizontal* segment = Horizontal::create ( rp->getNet()
                                                   , isegment->getLayer()
                                                   , track->getAxis()
                                                   , wireWidth
                                                   , bb.getXMin()+extension
                                                   , bb.getXMax()-extension
                                                   );
        // TrackElement* element =
            TrackFixedSegment::create ( track, segment );
        //cinfo << "  Rp Protect:" << track << "+" << element << endl;
        }

        // Track* track = plane->getTrackByPosition ( axisMin, Constant::Superior );
        // for ( ; track and (track->getAxis() <= axisMax) ; track = track->getNext() ) {
        //   Point sourcePosition (bb.getXMin()+extension,track->getAxis());
        //   Point targetPosition (bb.getXMax()-extension,track->getAxis());

        //   Katabatic::GCell* sourceGCell = Session::getKatabatic()->getGCellGrid()->getGCell ( sourcePosition );
        //   Katabatic::GCell* targetGCell = Session::getKatabatic()->getGCellGrid()->getGCell ( targetPosition );

        //   cinfo << "    S: " << sourceGCell << " from " << sourcePosition << endl;
        //   cinfo << "    T: " << targetGCell << " from " << targetPosition << endl;

        //   unsigned int segmentType
        //     = (sourceGCell == targetGCell) ? AutoSegment::Local : AutoSegment::Global;

        //   AutoContact* source = AutoContact::fromRp ( sourceGCell
        //                                             , rp
        //                                             , rp->getLayer()
        //                                             , sourcePosition
        //                                             , DbU::lambda(1.0), DbU::lambda(1.0)
        //                                             , true
        //                                             );

        //   AutoContact* target = AutoContact::fromRp ( targetGCell
        //                                             , rp
        //                                             , rp->getLayer()
        //                                             , targetPosition
        //                                             , DbU::lambda(1.0), DbU::lambda(1.0)
        //                                             , true
        //                                             );

        //   AutoSegment* segment = AutoSegment::create ( source
        //                                              , target
        //                                              , Constant::Horizontal
        //                                              , segmentType
        //                                              , true
        //                                              , false
        //                                              );
        //   segment->setLayer ( isegment->getLayer() );
        //   segment->setFixed ( true );

        //   bool          created = true;
        //   TrackElement* element = TrackSegment::create ( segment, track, created );
        //   cinfo << "    Rp Protect " << track << "+" << element << endl;
        // }
      } else {
        DbU::Unit axisMin = bb.getXMin() - delta;
        DbU::Unit axisMax = bb.getXMax() + delta;

        Track* track = plane->getTrackByPosition ( axisMin, Constant::Superior );
        for ( ; track and (track->getAxis() <= axisMax) ; track = track->getNext() ) {
          Vertical* segment = Vertical::create ( rp->getNet()
                                               , isegment->getLayer()
                                               , track->getAxis()
                                               , wireWidth
                                               , bb.getYMin()+extension
                                               , bb.getYMax()-extension
                                               );
        // TrackElement* element =
            TrackFixedSegment::create ( track, segment );
        //cinfo << "  Rp Protect:" << track << "+" << element << endl;
        }

        // Track* track = plane->getTrackByPosition ( axisMin, Constant::Superior );
        // for ( ; track and (track->getAxis() <= axisMax) ; track = track->getNext() ) {
        //   cinfo << "    Track Axis: " << DbU::getValueString(track->getAxis()) << endl;

        //   Point sourcePosition (track->getAxis(),bb.getYMin()+extension);
        //   Point targetPosition (track->getAxis(),bb.getYMax()-extension);

        //   Katabatic::GCell* sourceGCell = Session::getKatabatic()->getGCellGrid()->getGCell ( sourcePosition );
        //   Katabatic::GCell* targetGCell = Session::getKatabatic()->getGCellGrid()->getGCell ( targetPosition );

        //   cinfo << "    S: " << sourceGCell << " from " << sourcePosition << endl;
        //   cinfo << "    T: " << targetGCell << " from " << targetPosition << endl;

        //   unsigned int segmentType
        //     = (sourceGCell == targetGCell) ? AutoSegment::Local : AutoSegment::Global;

        //   AutoContact* source = AutoContact::fromRp ( sourceGCell
        //                                             , rp
        //                                             , rp->getLayer()
        //                                             , sourcePosition
        //                                             , DbU::lambda(1.0), DbU::lambda(1.0)
        //                                             , true
        //                                             );

        //   AutoContact* target = AutoContact::fromRp ( targetGCell
        //                                             , rp
        //                                             , rp->getLayer()
        //                                             , targetPosition
        //                                             , DbU::lambda(1.0), DbU::lambda(1.0)
        //                                             , true
        //                                             );

        //   AutoSegment* segment = AutoSegment::create ( source
        //                                              , target
        //                                              , Constant::Vertical
        //                                              , segmentType
        //                                              , true
        //                                              , false
        //                                              );
        //   segment->setLayer ( isegment->getLayer() );
        //   segment->setFixed ( true );

        //   bool          created = true;
        //   TrackElement* element = TrackSegment::create ( segment, track, created );
        //   cinfo << "    Rp Protect: " << track << "+" << element << endl;
        // }
      }
    }
  }


} // End of anonymous namespace.


namespace Kite {


  using Hurricane::DataBase;
  using Hurricane::Technology;
  using Hurricane::BasicLayer;
  using Hurricane::ForEachIterator;
  using Hurricane::Cell;


  void  KiteEngine::protectRoutingPads ()
  {
    cmess1 << "  o  Protect external components not useds as RoutingPads." << endl;

    forEach ( Net*, inet, getCell()->getNets() ) {
      if ( (*inet)->isSupply() ) continue;

      forEach ( RoutingPad*, irp, (*inet)->getRoutingPads() ) {
        protectRoutingPad ( *irp );
      }
    }

    Session::revalidate ();
  }


} // End of Kite namespace.
