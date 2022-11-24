
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/ChipDatas.h"                       |
// +-----------------------------------------------------------------+


#ifndef  __KATABATIC_CHIP_DATAS__
#define  __KATABATIC_CHIP_DATAS__

#include  <string>

#include  "hurricane/DbU.h"
#include  "hurricane/Torus.h"
namespace Hurricane {
  class Cell;
}


namespace Katabatic {

  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Torus;
  using Hurricane::Cell;


  class ChipDatas {
    public:
                          ChipDatas       ( const Cell* );
      inline bool         isChip          () const;
      inline const Box&   getChipBb       () const;
      inline const Box&   getLeftPadsBb   () const;
      inline const Box&   getRightPadsBb  () const;
      inline const Box&   getTopPadsBb    () const;
      inline const Box&   getBottomPadsBb () const;
      inline const Torus& getChipCorona   () const;
      inline bool         intersectVPads  ( const Box& ) const;
      inline bool         intersectHPads  ( const Box& ) const;
    public:
             Record*      _getRecord      () const;
             std::string  _getString      () const;
      inline std::string  _getTypeName    () const;
    private:
      Cell*  _cell;
      bool   _isChip;
      Box    _chipBb;
      Box    _leftPadsBb;
      Box    _rightPadsBb;
      Box    _topPadsBb;
      Box    _bottomPadsBb;
      Torus  _chipCorona;
  };


// Inline Functions.
  inline bool         ChipDatas::isChip          () const { return _isChip; }
  inline const Box&   ChipDatas::getChipBb       () const { return _chipBb; }
  inline const Box&   ChipDatas::getLeftPadsBb   () const { return _leftPadsBb; };
  inline const Box&   ChipDatas::getRightPadsBb  () const { return _rightPadsBb; };
  inline const Box&   ChipDatas::getTopPadsBb    () const { return _topPadsBb; };
  inline const Box&   ChipDatas::getBottomPadsBb () const { return _bottomPadsBb; };
  inline const Torus& ChipDatas::getChipCorona   () const { return _chipCorona; };
  inline std::string  ChipDatas::_getTypeName    () const { return "ChipDatas"; }

  inline bool  ChipDatas::intersectVPads ( const Box& box ) const
  { return _leftPadsBb.intersect(box) or _rightPadsBb.intersect(box); }

  inline bool  ChipDatas::intersectHPads ( const Box& box ) const;
  { return _topPadsBb.intersect(box) or _bottomPadsBb.intersect(box); }


} // End of Katabatic namespace.

INSPECTOR_PV_SUPPORT(Katabatic::ChipDatas);

#endif  // __KATABATIC_CHIP_DATAS__
