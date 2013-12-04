
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/ChipTools.h"                       |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_CHIP_TOOLS_H
#define  KATABATIC_CHIP_TOOLS_H

#include <string>
#include "hurricane/DbU.h"
#include "hurricane/Torus.h"
namespace Hurricane {
  class Cell;
  class Instance;
}


namespace Katabatic {

  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Torus;
  using Hurricane::Cell;
  using Hurricane::Instance;


  class ChipTools {
    public:
                          ChipTools       ( Cell* );
      inline bool         isChip          () const;
      inline Cell*        getCell         () const;
      inline Instance*    getCore         () const;
      inline const Box&   getChipBb       () const;
      inline const Box&   getLeftPadsBb   () const;
      inline const Box&   getRightPadsBb  () const;
      inline const Box&   getTopPadsBb    () const;
      inline const Box&   getBottomPadsBb () const;
      inline const Torus& getCorona       () const;
      inline bool         intersectVPads  ( const Box& ) const;
      inline bool         intersectHPads  ( const Box& ) const;
    public:
             Record*      _getRecord      () const;
             std::string  _getString      () const;
      inline std::string  _getTypeName    () const;
    private:
      Cell*     _cell;
      Instance* _core;
      bool      _isChip;
      Box       _chipBb;
      Box       _leftPadsBb;
      Box       _rightPadsBb;
      Box       _topPadsBb;
      Box       _bottomPadsBb;
      Torus     _chipCorona;
  };


// Inline Functions.
  inline bool         ChipTools::isChip          () const { return _isChip; }
  inline Cell*        ChipTools::getCell         () const { return _cell; }
  inline Instance*    ChipTools::getCore         () const { return _core; }
  inline const Box&   ChipTools::getChipBb       () const { return _chipBb; }
  inline const Box&   ChipTools::getLeftPadsBb   () const { return _leftPadsBb; };
  inline const Box&   ChipTools::getRightPadsBb  () const { return _rightPadsBb; };
  inline const Box&   ChipTools::getTopPadsBb    () const { return _topPadsBb; };
  inline const Box&   ChipTools::getBottomPadsBb () const { return _bottomPadsBb; };
  inline const Torus& ChipTools::getCorona       () const { return _chipCorona; };
  inline std::string  ChipTools::_getTypeName    () const { return "ChipTools"; }

  inline bool  ChipTools::intersectVPads ( const Box& box ) const
  { return _leftPadsBb.intersect(box) or _rightPadsBb.intersect(box); }

  inline bool  ChipTools::intersectHPads ( const Box& box ) const
  { return _topPadsBb.intersect(box) or _bottomPadsBb.intersect(box); }


} // Katabatic namespace.

INSPECTOR_PV_SUPPORT(Katabatic::ChipTools);

#endif  // KATABATIC_CHIP_TOOLS_H
