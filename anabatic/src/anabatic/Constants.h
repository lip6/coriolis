// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/Constants.h"                        |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_CONSTANTS_H
#define  ANABATIC_CONSTANTS_H

#include "hurricane/Flags.h"

namespace Anabatic {


  class Flags : public Hurricane::BaseFlags {
    public:
      enum Flag { NoFlags        = 0
                , Horizontal     = (1<<0)
                , Vertical       = (1<<1)
                , SourceGCell    = (1<<2)
                , TargetGCell    = (1<<3)
                , Invalidated    = (1<<4)
                , Destroy        = (1<<5)
                , WestSide       = Horizontal|TargetGCell
                , EastSide       = Horizontal|SourceGCell
                , SouthSide      = Vertical  |TargetGCell
                , NorthSide      = Vertical  |SourceGCell
                , AllSides       = WestSide|EastSide|SouthSide|NorthSide 
                , PitchAbove     = (1<<6)
                , PitchBelow     = (1<<7)
                };
    public:
      inline               Flags        ( unsigned int flags = NoFlags );
      inline               Flags        ( BaseFlags );
      virtual             ~Flags        ();
      virtual std::string  _getTypeName () const;
      virtual std::string  _getString   () const;
  };


  Flags::Flags ( unsigned int flags ) : BaseFlags(flags) { }
  Flags::Flags ( BaseFlags    base  ) : BaseFlags(base)  { }


}  // Anabatic namespace.


INSPECTOR_PV_SUPPORT(Anabatic::Flags)

#endif  // ANABATIC_CONSTANTS_H
