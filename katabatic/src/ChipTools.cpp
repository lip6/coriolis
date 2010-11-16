
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
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ChipTools.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/Warning.h"
#include  "hurricane/Bug.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "crlcore/RoutingGauge.h"
#include  "crlcore/AllianceFramework.h"
#include  "katabatic/Session.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoSegment.h"
#include  "katabatic/AutoHorizontal.h"
#include  "katabatic/AutoVertical.h"
#include  "katabatic/GCell.h"
#include  "katabatic/GCellGrid.h"
#include  "katabatic/KatabaticEngine.h"


namespace {


  using namespace std;
  using namespace CRL;
  using namespace Hurricane;
  using namespace Katabatic;


  enum SegmentType { LocalSegments=0x10, GlobalSegments=0x20 };


  bool  isChip ( Cell* cell, Instance*& core )
  {
    AllianceFramework* af = AllianceFramework::get();
    int pads  = 0;
    int cores = 0;

    forEach ( Instance*, iinstance, cell->getInstances() ) {
      if ( af->isPad(iinstance->getMasterCell()) )
        ++pads;
      else {
        ++cores;
        core = *iinstance;
      }
    }

    return (pads > 0) and (cores == 1);
  }


  void  breakSegments ( GCell* begin, GCell* end, Layer::Mask mask, unsigned int flags )
  {
    for ( GCell* gcell=begin ; gcell != NULL ; ) {
      ltrace(200) << "Pre-break in " << gcell << endl;

      if ( (flags & Constant::Horizontal) and (flags & GlobalSegments) ) {
        vector<AutoSegment*>* hsegments = gcell->getHSegments();
        for ( size_t i=0 ; i<(*hsegments).size() ; ++i ) {
          if ( not ((*hsegments)[i]->getLayer()->getMask() & mask) ) continue;
          
          ltrace(200) << "Pre-break: " << (*hsegments)[i] << " @" << gcell << endl;
          (*hsegments)[i]->makeDogLeg ( gcell, true );
        }
        
        if ( gcell == end ) break;
        gcell = gcell->getUp();
      }

      if ( (flags & Constant::Vertical) and (flags & GlobalSegments) ) {
        vector<AutoSegment*>* vsegments = gcell->getVSegments();
        for ( size_t i=0 ; i<(*vsegments).size() ; ++i ) {
          if ( not ((*vsegments)[i]->getLayer()->getMask() & mask) ) continue;
          
          ltrace(200) << "Pre-break: " << (*vsegments)[i] << " @" << gcell << endl;
          (*vsegments)[i]->makeDogLeg ( gcell, true );
        }
        
        if ( gcell == end ) break;
        gcell = gcell->getUp();
      }

      if ( flags & GlobalSegments ) {
        vector<AutoContact*>* contacts = gcell->getContacts();
        vector<AutoSegment*>  segments;
        for ( size_t i=0 ; i<contacts->size() ; i++ ) {
          forEach ( Component*, component, (*contacts)[i]->getSlaveComponents() ) {
            Segment*     segment      = dynamic_cast<Segment*>(*component);
            AutoSegment* autoSegment  = Session::lookup ( segment );

            if ( autoSegment and (autoSegment->getDirection() & flags) )
              segments.push_back ( autoSegment );
          }
        }

        for ( size_t i=0 ; i<segments.size() ; ++i ) {
          ltrace(200) << "Pre-break: "
                      << segments[i]->getDirection() << "&" << flags
                      << " " << segments[i] << endl;
          segments[i]->makeDogLeg ( gcell, true );
        }
      }
    }

    Session::revalidate ();
  }


}  // End of anonymous namespace.


namespace Katabatic {


  void  KatabaticEngine::slackenBorder ( Box bb, Layer::Mask mask, unsigned int flags )
  {
    GCell*      begin = getGCellGrid()->getGCell ( Point(bb.getXMin(),bb.getYMin()) );
    GCell*      end   = getGCellGrid()->getGCell ( Point(bb.getXMin(),bb.getYMax()) );

    breakSegments ( begin, end, mask, flags );

    begin = getGCellGrid()->getGCell ( Point(bb.getXMax(),bb.getYMin()) );
    end   = getGCellGrid()->getGCell ( Point(bb.getXMax(),bb.getYMax()) );

    breakSegments ( begin, end, mask, flags );

    Session::revalidate ();
  }


  void  KatabaticEngine::slackenBlockIos ( Instance* core )
  {
    cmess1 << "  o  Slackening IOs of <" << core->getName() << ">." << endl;

    Layer::Mask mask = Session::getRoutingLayer(1)->getMask();
    slackenBorder ( core->getBoundingBox().inflate(DbU::lambda(50.0))
                  , mask
                  , GlobalSegments|Constant::Horizontal
                  );
  }


  void  KatabaticEngine::chipPrep ()
  {
    Instance* core = NULL;
    if ( isChip(getCell(),core) ) {
      slackenBlockIos ( core );

      // cmess1 << "  o  Slackening Pads-connected segments." << endl;
      // slackenBorder ( getCell()->getBoundingBox().inflate(DbU::lambda(-425.0))
      //               , Session::getRoutingLayer(3)->getMask()
      //               , GlobalSegments|LocalSegments|Constant::Horizontal
      //               );
      // slackenBorder ( getCell()->getBoundingBox().inflate(DbU::lambda(-425.0))
      //               , Session::getRoutingLayer(1)->getMask()
      //               , GlobalSegments|Constant::Horizontal
      //               );
    }
  }



}  // End of Katabatic namespace.
