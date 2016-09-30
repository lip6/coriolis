// -*- mode: C++; explicit-buffer-name: "Constants.h<anabatic>" -*-
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
      static const unsigned int  NoFlags             ; // =  0;
    // Flags used for both objects states & functions arguments.
      static const unsigned int  Horizontal          ; // = (1 <<  0);
      static const unsigned int  Vertical            ; // = (1 <<  1);
      static const unsigned int  Source              ; // = (1 <<  2);
      static const unsigned int  Target              ; // = (1 <<  3);
      static const unsigned int  Invalidated         ; // = (1 <<  4);
    // Flags for GCell objects states only.                        
      static const unsigned int  DeviceGCell         ; // = (1 <<  5);
      static const unsigned int  HChannelGCell       ; // = (1 <<  6);
      static const unsigned int  VChannelGCell       ; // = (1 <<  7);
      static const unsigned int  StrutGCell          ; // = (1 <<  8);
      static const unsigned int  MatrixGCell         ; // = (1 <<  9);
      static const unsigned int  IoPadGCell          ; // = (1 << 10);
      static const unsigned int  Saturated           ; // = (1 << 11);
    // Flags for Anabatic objects states only.                      
      static const unsigned int  DemoMode            ; // = (1 <<  5);
      static const unsigned int  WarnOnGCellOverload ; // = (1 <<  6);
      static const unsigned int  DestroyGCell        ; // = (1 <<  7);
      static const unsigned int  DestroyBaseContact  ; // = (1 <<  8);
      static const unsigned int  DestroyBaseSegment  ; // = (1 <<  9);
    // Flags for NetDatas objects states only.                      
      static const unsigned int  GlobalRouted        ; // = (1 <<  5);
    // Masks.                                      
      static const unsigned int  WestSide            ; // = Horizontal|Target;
      static const unsigned int  EastSide            ; // = Horizontal|Source;
      static const unsigned int  SouthSide           ; // = Vertical  |Target;
      static const unsigned int  NorthSide           ; // = Vertical  |Source;
      static const unsigned int  AllSides            ; // = WestSide|EastSide|SouthSide|NorthSide ;
      static const unsigned int  EndsMask            ; // = Source|Target;
      static const unsigned int  DirectionMask       ; // = Horizontal|Vertical;
      static const unsigned int  DestroyMask         ; // = DestroyGCell|DestroyBaseContact|DestroyBaseSegment;
      static const unsigned int  GCellTypeMask       ; // = DeviceGCell|HChannelGCell|VChannelGCell|StrutGCell|MatrixGCell|IoPadGCell;
    // Flags for functions arguments only.           
      static const unsigned int  Create              ; // = (1 <<  5);
      static const unsigned int  WithPerpands        ;
      static const unsigned int  WithSelf            ;
      static const unsigned int  AboveLayer          ;
      static const unsigned int  BelowLayer          ;
      static const unsigned int  OpenSession         ;
      static const unsigned int  Realignate          ;
      static const unsigned int  NativeConstraints   ;
      static const unsigned int  ForceMove           ;
      static const unsigned int  WarnOnError         ;
      static const unsigned int  Topology            ;
      static const unsigned int  GlobalSegment       ;
      static const unsigned int  AllowTerminal       ;
      static const unsigned int  AllowLocal          ;
      static const unsigned int  IgnoreContacts      ;
      static const unsigned int  Propagate           ;
      static const unsigned int  Superior            ;
      static const unsigned int  DoglegOnLeft        ;
      static const unsigned int  DoglegOnRight       ;
      static const unsigned int  WithNeighbors       ;
      static const unsigned int  NoCheckLayer        ;
      static const unsigned int  HalfSlacken         ;
      static const unsigned int  NoGCellShrink       ;
      static const unsigned int  CParanoid           ;
      static const unsigned int  CheckLowDensity     ;
      static const unsigned int  CheckLowUpDensity   ;
      static const unsigned int  NoUpdate            ;
    public:
      inline               Flags        ( unsigned int flags = NoFlags );
      inline               Flags        ( BaseFlags );
      virtual             ~Flags        ();
      virtual std::string  _getTypeName () const;
      virtual std::string  _getString   () const;
  };


  Flags::Flags ( unsigned int flags ) : BaseFlags(flags) { }
  Flags::Flags ( BaseFlags    base  ) : BaseFlags(base)  { }


  enum EngineState     { EngineCreation      = 1
                       , EngineGlobalLoaded  = 2
                       , EngineActive        = 3
                       , EngineDriving       = 4
                       , EnginePreDestroying = 5
                       , EngineGutted        = 6
                       };

  enum EngineAlgorithm { EngineLoadGrByNet         = (1 <<  0)
                       , EngineLoadGrByGCell       = (1 <<  1)
                       , EngineLayerAssignByLength = (1 <<  2)
                       , EngineLayerAssignByTrunk  = (1 <<  3)
                       , EngineNoNetLayerAssign    = (1 <<  4)
                       };


  inline Flags  perpandicularTo ( Flags direction )
  {
    if (direction & Flags::Horizontal) return Flags::Vertical;
    if (direction & Flags::Vertical  ) return Flags::Horizontal;
    return Flags::NoFlags;
  }


}  // Anabatic namespace.


INSPECTOR_PV_SUPPORT(Anabatic::Flags)

#endif  // ANABATIC_CONSTANTS_H
