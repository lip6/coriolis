// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        A n a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ChipTools.cpp"                          |
// +-----------------------------------------------------------------+


#include <string>
#include <sstream>
#include "hurricane/Warning.h"
#include "hurricane/Bug.h"
#include "hurricane/Torus.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/RoutingPad.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/AllianceFramework.h"
#include "anabatic/Session.h"
#include "anabatic/AutoContact.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/AutoHorizontal.h"
#include "anabatic/AutoVertical.h"
#include "anabatic/GCell.h"
#include "anabatic/AnabaticEngine.h"


namespace {

  using namespace std;
  using namespace CRL;
  using namespace Hurricane;
  using namespace Anabatic;


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


namespace Anabatic {


  void  AnabaticEngine::chipPrep ()
  {
    if (isChip()) {
      reselectPadRp( getCell() );
    }
  }


  ChipTools::ChipTools ( Cell* cell )
    : _cell         (cell)
    , _core         (NULL)
    , _referencePad (NULL)
    , _isChip       (false)
    , _chipBb       (cell->getBoundingBox())
    , _leftPadsBb   ()
    , _rightPadsBb  ()
    , _topPadsBb    ()
    , _bottomPadsBb ()
    , _chipCorona   ()
    , _padWidth     (0)
    , _padHeight    (0)
    , _padPowerWidth(0)
    , _padClockWidth(0)
  {
    _isChip = ::isChip( _cell, _core, _referencePad );

    if (_isChip) {
      _padHeight = _referencePad->getAbutmentBox().getHeight();
      _padWidth  = _referencePad->getAbutmentBox().getWidth();

      Box outer = _cell->getBoundingBox().inflate ( -_padHeight );
      _chipCorona   = Torus ( outer, _core->getBoundingBox() );
      _leftPadsBb   = Box   ( _chipBb.getXMin()                  , _chipBb.getYMin(), _chipCorona.getOuterBox().getXMin(), _chipBb.getYMax() );
      _rightPadsBb  = Box   ( _chipCorona.getOuterBox().getXMax(), _chipBb.getYMin(), _chipBb.getXMax(),                   _chipBb.getYMax() );
      _bottomPadsBb = Box   ( _chipBb.getXMin()                  , _chipBb.getYMin(), _chipBb.getXMax(), _chipCorona.getOuterBox().getYMin() );
      _topPadsBb    = Box   ( _chipBb.getXMin(), _chipCorona.getOuterBox().getYMax(), _chipBb.getXMax(),                   _chipBb.getYMax() );

      Layer* metal3 = DataBase::getDB()->getTechnology()->getLayer( "METAL3" );
      Net*   net    = _referencePad->getNet( "ck" );
      forEach ( Horizontal*, ihorizontal, net->getHorizontals() ) {
        if (ihorizontal->getLayer() == metal3) {
          _padClockWidth = ihorizontal->getWidth();
          break;
        }
      }
      net = _referencePad->getNet( "vddi" );
      forEach ( Horizontal*, ihorizontal, net->getHorizontals() ) {
        if (ihorizontal->getLayer() == metal3) {
          _padPowerWidth = ihorizontal->getWidth();
          break;
        }
      }

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


}  // End of Anabatic namespace.
