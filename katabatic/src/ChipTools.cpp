// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ChipTools.cpp"                          |
// +-----------------------------------------------------------------+


#include  <string>
#include  <sstream>
#include  "hurricane/Warning.h"
#include  "hurricane/Bug.h"
#include  "hurricane/Torus.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/RoutingPad.h"
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


  bool  isChip ( Cell* cell, Instance*& core, Cell*& referencePad )
  {
    AllianceFramework* af = AllianceFramework::get();
    int pads  = 0;
    int cores = 0;

    core         = NULL;
    referencePad = NULL;
    forEach ( Instance*, iinstance, cell->getInstances() ) {
      if ( af->isPad(iinstance->getMasterCell()) ) {
        ++pads;
        if (not referencePad)
          referencePad = iinstance->getMasterCell();
      } else {
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

      if ( (flags & KbHorizontal) and (flags & KbGlobalSegment) ) {
        const vector<AutoSegment*>& hsegments = gcell->getHSegments();
        for ( size_t i=0 ; i<hsegments.size() ; ++i ) {
          if ( not (hsegments[i]->getLayer()->getMask() & mask) ) continue;
          
          ltrace(200) << "Pre-break: " << hsegments[i] << " @" << gcell << endl;
#ifdef THIS_IS_DISABLED
          hsegments[i]->makeDogLeg ( gcell, true );
#endif
        }
        
        if ( gcell == end ) break;
        gcell = gcell->getUp();
      }

      if ( (flags & KbVertical) and (flags & KbGlobalSegment) ) {
        const vector<AutoSegment*>& vsegments = gcell->getVSegments();
        for ( size_t i=0 ; i<vsegments.size() ; ++i ) {
          if ( not (vsegments[i]->getLayer()->getMask() & mask) ) continue;
          
          ltrace(200) << "Pre-break: " << vsegments[i] << " @" << gcell << endl;
#if THIS_IS_DISABLED
          vsegments[i]->makeDogLeg ( gcell, true );
#endif
        }
        
        if ( gcell == end ) break;
        gcell = gcell->getUp();
      }

      if (flags & KbGlobalSegment) {
        const vector<AutoContact*>& contacts = gcell->getContacts();
        vector<AutoSegment*>        segments;
        for ( size_t i=0 ; i<contacts.size() ; i++ ) {
          forEach ( Component*, component, contacts[i]->getSlaveComponents() ) {
            Segment*     segment      = dynamic_cast<Segment*>(*component);
            AutoSegment* autoSegment  = Session::lookup( segment );

            if ( autoSegment and (autoSegment->getDirection() & flags) )
              segments.push_back( autoSegment );
          }
        }

        for ( size_t i=0 ; i<segments.size() ; ++i ) {
          ltrace(200) << "Pre-break: "
                      << segments[i]->getDirection() << "&" << flags
                      << " " << segments[i] << endl;
#if THIS_IS_DISABLED
          segments[i]->makeDogLeg( gcell, true );
#endif
        }
      }
    }

    Session::revalidate ();
  }


  void  reselectPadRp ( Cell* cell )
  {
    AllianceFramework* af = AllianceFramework::get();

    forEach ( Net*, inet, cell->getNets() ) {
      if ( inet->getType() == Net::Type::GROUND ) continue;
      if ( inet->getType() == Net::Type::POWER  ) continue;
      if ( inet->getType() == Net::Type::CLOCK  ) continue;

      forEach ( RoutingPad*, irp, inet->getRoutingPads() ) {
        Instance* instance = irp->getOccurrence().getPath().getTailInstance();
        if ( instance ) {
          Cell* masterCell = instance->getMasterCell();
          if ( af->isPad(masterCell) )
            irp->setOnBestComponent(RoutingPad::LowestLayer);
        }
      } // RoutingPad*.
    } // Net*.
  }


}  // End of anonymous namespace.


namespace Katabatic {


  void  KatabaticEngine::slackenBorder ( Box bb, Layer::Mask mask, unsigned int flags )
  {
    GCell* begin = getGCellGrid()->getGCell ( Point(bb.getXMin(),bb.getYMin()) );
    GCell* end   = getGCellGrid()->getGCell ( Point(bb.getXMin(),bb.getYMax()) );

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
    slackenBorder ( core->getBoundingBox().inflate(Session::getSliceHeight())
                  , mask
                  , GlobalSegments|Constant::Horizontal
                  );
  }


  void  KatabaticEngine::chipPrep ()
  {
    if (isChip()) {
      reselectPadRp( getCell() );
    //slackenBlockIos( _core );

    //cmess1 << "  o  Slackening Pads-connected segments." << endl;
    //slackenBorder( _cell->getBoundingBox().inflate(DbU::lambda(-425.0))
    //             , Session::getRoutingLayer(3)->getMask()
    //             , GlobalSegments|LocalSegments|Constant::Horizontal
    //             );
    //slackenBorder( _cell->getBoundingBox().inflate(DbU::lambda(-425.0))
    //             , Session::getRoutingLayer(1)->getMask()
    //             , GlobalSegments|Constant::Horizontal
    //             );
    }
  }


  ChipTools::ChipTools ( Cell* cell )
    : _cell        (cell)
    , _core        (NULL)
    , _referencePad(NULL)
    , _isChip      (false)
    , _chipBb      (cell->getBoundingBox())
    , _leftPadsBb  ()
    , _rightPadsBb ()
    , _topPadsBb   ()
    , _bottomPadsBb()
    , _chipCorona  ()
  {
    _isChip = ::isChip( _cell, _core, _referencePad );

    if (_isChip) {
    // Ugly: hard-coded pads height.
      const DbU::Unit padHeight = _referencePad->getAbutmentBox().getHeight();

      Box outer = _cell->getBoundingBox().inflate ( -padHeight );
      _chipCorona   = Torus ( outer, _core->getBoundingBox() );
      _leftPadsBb   = Box   ( _chipBb.getXMin()                  , _chipBb.getYMin(), _chipCorona.getOuterBox().getXMin(), _chipBb.getYMax() );
      _rightPadsBb  = Box   ( _chipCorona.getOuterBox().getXMax(), _chipBb.getYMin(), _chipBb.getXMax(),                   _chipBb.getYMax() );
      _bottomPadsBb = Box   ( _chipBb.getXMin()                  , _chipBb.getYMin(), _chipBb.getXMax(), _chipCorona.getOuterBox().getYMin() );
      _topPadsBb    = Box   ( _chipBb.getXMin(), _chipCorona.getOuterBox().getYMax(), _chipBb.getXMax(),                   _chipBb.getYMax() );

      cmess1 << "  o  Design is a complete chip." << endl;
      cmess1 << "     - Core:          <" << _core->getName() << ">/<"
             << _core->getMasterCell()->getName() << ">." << endl;
      cmess1 << "     - Reference pad: <" << _referencePad->getName() << ">" << endl;
      cmess1 << "     - Corona:        " << _chipCorona << "." << endl;
    } else {
      _chipCorona = Torus ( _cell->getBoundingBox(), _cell->getBoundingBox() );
    }
  }


  string  ChipTools::_getString () const
  {
    ostringstream s;
    s << "<" << _getTypeName() << " " << _cell->getName()
      << " core:" << getString(((_core) ? _core->getName() : "NULL"))
      << ">";
    return s.str();
  }


  Record* ChipTools::_getRecord () const
  {
    Record* record = new Record ( _getString() );
                                     
    record->add ( getSlot ( "_cell"        ,  _cell         ) );
    record->add ( getSlot ( "_core"        ,  _core         ) );
    record->add ( getSlot ( "_referencePad",  _referencePad ) );
    record->add ( getSlot ( "_isChip"      , &_isChip       ) );
    record->add ( getSlot ( "_chipBb"      , &_chipBb       ) );
    record->add ( getSlot ( "_leftPadsBb"  , &_leftPadsBb   ) );
    record->add ( getSlot ( "_rightPadsBb" , &_rightPadsBb  ) );
    record->add ( getSlot ( "_topPadsBb"   , &_topPadsBb    ) );
    record->add ( getSlot ( "_bottomPadsBb", &_bottomPadsBb ) );
    record->add ( getSlot ( "_chipCorona"  , &_chipCorona   ) );
    return record;
  }


}  // End of Katabatic namespace.
