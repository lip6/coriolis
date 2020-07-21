// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/ChipTools.h"                        |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include "hurricane/DbU.h"
#include "hurricane/Torus.h"
namespace Hurricane {
  class Cell;
  class Instance;
}


namespace Anabatic {

  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Torus;
  using Hurricane::Cell;
  using Hurricane::Instance;


  class ChipTools {
    public:
                                ChipTools        ( Cell* );
      inline       bool         isChip           () const;
      inline       Cell*        getCell          () const;
      inline       Instance*    getCore          () const;
      inline       Cell*        getReferencePad  () const;
      inline       DbU::Unit    getPadWidth      () const;
      inline       DbU::Unit    getPadHeight     () const;
      inline       DbU::Unit    getPadPowerWidth () const;
      inline       DbU::Unit    getPadClockWidth () const;
      inline const Box&         getChipBb        () const;
      inline const Box&         getLeftPadsBb    () const;
      inline const Box&         getRightPadsBb   () const;
      inline const Box&         getTopPadsBb     () const;
      inline const Box&         getBottomPadsBb  () const;
      inline const Torus&       getCorona        () const;
      inline const Box&         getCoronaBb      () const;
      inline       bool         intersectVPads   ( const Box& ) const;
      inline       bool         intersectHPads   ( const Box& ) const;
      inline       bool         vPadsEnclosed    ( const Box& ) const;
      inline       bool         hPadsEnclosed    ( const Box& ) const;
    public:                                      
                   Record*      _getRecord       () const;
                   std::string  _getString       () const;
      inline       std::string  _getTypeName     () const;
    private:
      Cell*      _cell;
      Instance*  _core;
      Cell*      _referencePad;
      bool       _isChip;
      Box        _chipBb;
      Box        _leftPadsBb;
      Box        _rightPadsBb;
      Box        _topPadsBb;
      Box        _bottomPadsBb;
      Torus      _chipCorona;
      DbU::Unit  _padWidth;
      DbU::Unit  _padHeight;
      DbU::Unit  _padPowerWidth;
      DbU::Unit  _padClockWidth;
  };


// Inline Functions.
  inline bool         ChipTools::isChip           () const { return _isChip; }
  inline Cell*        ChipTools::getCell          () const { return _cell; }
  inline Instance*    ChipTools::getCore          () const { return _core; }
  inline Cell*        ChipTools::getReferencePad  () const { return _referencePad; }
  inline DbU::Unit    ChipTools::getPadWidth      () const { return _padWidth; }
  inline DbU::Unit    ChipTools::getPadHeight     () const { return _padHeight; }
  inline DbU::Unit    ChipTools::getPadPowerWidth () const { return _padPowerWidth; }
  inline DbU::Unit    ChipTools::getPadClockWidth () const { return _padClockWidth; }
  inline const Box&   ChipTools::getChipBb        () const { return _chipBb; }
  inline const Box&   ChipTools::getLeftPadsBb    () const { return _leftPadsBb; };
  inline const Box&   ChipTools::getRightPadsBb   () const { return _rightPadsBb; };
  inline const Box&   ChipTools::getTopPadsBb     () const { return _topPadsBb; };
  inline const Box&   ChipTools::getBottomPadsBb  () const { return _bottomPadsBb; };
  inline const Torus& ChipTools::getCorona        () const { return _chipCorona; };
  inline const Box&   ChipTools::getCoronaBb      () const { return _chipCorona.getOuterBox(); }
  inline std::string  ChipTools::_getTypeName     () const { return "ChipTools"; }

  inline bool  ChipTools::intersectVPads ( const Box& box ) const
  { return _leftPadsBb.intersect(box) or _rightPadsBb.intersect(box); }

  inline bool  ChipTools::intersectHPads ( const Box& box ) const
  { return _topPadsBb.intersect(box) or _bottomPadsBb.intersect(box); }

  inline bool  ChipTools::vPadsEnclosed ( const Box& box ) const
  { return _leftPadsBb.contains(box) or _rightPadsBb.contains(box); }

  inline bool  ChipTools::hPadsEnclosed ( const Box& box ) const
  { return _topPadsBb.contains(box) or _bottomPadsBb.contains(box); }


} // Anabatic namespace.

INSPECTOR_PR_SUPPORT(Anabatic::ChipTools);
