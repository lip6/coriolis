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
      static const uint64_t  NoFlags             ; // =  0;
    // Flags used for both objects states & functions arguments.
      static const uint64_t  Horizontal          ; // = (1 <<  0);
      static const uint64_t  Vertical            ; // = (1 <<  1);
      static const uint64_t  Source              ; // = (1 <<  2);
      static const uint64_t  Target              ; // = (1 <<  3);
      static const uint64_t  Invalidated         ; // = (1 <<  4);
    // Flags for GCell objects states only.                        
      static const uint64_t  DeviceGCell         ; // = (1 <<  5);
      static const uint64_t  HChannelGCell       ; // = (1 <<  6);
      static const uint64_t  VChannelGCell       ; // = (1 <<  7);
      static const uint64_t  StrutGCell          ; // = (1 <<  8);
      static const uint64_t  MatrixGCell         ; // = (1 <<  9);
      static const uint64_t  IoPadGCell          ; // = (1 << 10);
      static const uint64_t  Saturated           ; // = (1 << 11);
    // Flags for Anabatic objects states only.                      
      static const uint64_t  DemoMode            ; // = (1 <<  5);
      static const uint64_t  WarnOnGCellOverload ; // = (1 <<  6);
      static const uint64_t  DestroyGCell        ; // = (1 <<  7);
      static const uint64_t  DestroyBaseContact  ; // = (1 <<  8);
      static const uint64_t  DestroyBaseSegment  ; // = (1 <<  9);
    // Flags for NetDatas objects states only.                      
      static const uint64_t  GlobalRouted        ; // = (1 <<  5);
    // Masks.                                      
      static const uint64_t  WestSide            ; // = Horizontal|Target;
      static const uint64_t  EastSide            ; // = Horizontal|Source;
      static const uint64_t  SouthSide           ; // = Vertical  |Target;
      static const uint64_t  NorthSide           ; // = Vertical  |Source;
      static const uint64_t  AllSides            ; // = WestSide|EastSide|SouthSide|NorthSide ;
      static const uint64_t  EndsMask            ; // = Source|Target;
      static const uint64_t  DirectionMask       ; // = Horizontal|Vertical;
      static const uint64_t  DestroyMask         ; // = DestroyGCell|DestroyBaseContact|DestroyBaseSegment;
      static const uint64_t  GCellTypeMask       ; // = DeviceGCell|HChannelGCell|VChannelGCell|StrutGCell|MatrixGCell|IoPadGCell;
    // Flags for functions arguments only.           
      static const uint64_t  Create              ; // = (1 <<  5);
      static const uint64_t  WithPerpands        ;
      static const uint64_t  WithDoglegs         ;
      static const uint64_t  WithSelf            ;
      static const uint64_t  AboveLayer          ;
      static const uint64_t  BelowLayer          ;
      static const uint64_t  OpenSession         ;
      static const uint64_t  Realignate          ;
      static const uint64_t  NativeConstraints   ;
      static const uint64_t  ForceMove           ;
      static const uint64_t  WarnOnError         ;
      static const uint64_t  Topology            ;
      static const uint64_t  GlobalSegment       ;
      static const uint64_t  AllowTerminal       ;
      static const uint64_t  AllowLocal          ;
      static const uint64_t  IgnoreContacts      ;
      static const uint64_t  Propagate           ;
      static const uint64_t  Superior            ;
      static const uint64_t  DoglegOnLeft        ;
      static const uint64_t  DoglegOnRight       ;
      static const uint64_t  WithNeighbors       ;
      static const uint64_t  NoCheckLayer        ;
      static const uint64_t  HalfSlacken         ;
      static const uint64_t  NoGCellShrink       ;
      static const uint64_t  CParanoid           ;
      static const uint64_t  CheckLowDensity     ;
      static const uint64_t  CheckLowUpDensity   ;
      static const uint64_t  NoUpdate            ;
    public:
      inline               Flags        ( uint64_t flags = NoFlags );
      inline               Flags        ( const Hurricane::BaseFlags& );
      virtual             ~Flags        ();
      virtual std::string  _getTypeName () const;
      virtual std::string  _getString   () const;
  };


  Flags::Flags (                  uint64_t   flags ) : BaseFlags(flags) { }
  Flags::Flags ( const Hurricane::BaseFlags& flags ) : BaseFlags(flags) { }


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
