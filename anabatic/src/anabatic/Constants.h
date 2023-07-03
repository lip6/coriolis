// -*- mode: C++; explicit-buffer-name: "Constants.h<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
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


#pragma  once
#include "hurricane/Flags.h"

namespace Anabatic {


// -------------------------------------------------------------------
// Class  :  "Anabatic::Flags".

  class Flags : public Hurricane::BaseFlags {
    public:
      static const BaseFlags  NoFlags             ; // =  0;
    // Flags used for both objects states & functions arguments.
      static const BaseFlags  Horizontal          ; // = (1 <<  0);
      static const BaseFlags  Vertical            ; // = (1 <<  1);
      static const BaseFlags  Source              ; // = (1 <<  2);
      static const BaseFlags  Target              ; // = (1 <<  3);
      static const BaseFlags  Invalidated         ; // = (1 <<  4);
    // Flags for GCell objects states only.                        
      static const BaseFlags  DeviceGCell         ; // = (1 <<  5);
      static const BaseFlags  HChannelGCell       ; // = (1 <<  6);
      static const BaseFlags  VChannelGCell       ; // = (1 <<  7);
      static const BaseFlags  StrutGCell          ; // = (1 <<  8);
      static const BaseFlags  MatrixGCell         ; // = (1 <<  9);
      static const BaseFlags  IoPadGCell          ; // = (1 << 10);
      static const BaseFlags  Saturated           ; // = (1 << 11);
      static const BaseFlags  StdCellRow          ; // = (1 << 12);
      static const BaseFlags  ChannelRow          ; // = (1 << 13);
      static const BaseFlags  HRailGCell          ; // = (1 << 14);
      static const BaseFlags  VRailGCell          ; // = (1 << 15);
      static const BaseFlags  GoStraight          ; // = (1 << 16);
    // Flags for Edge objects states only.                      
      static const BaseFlags  NullCapacity        ; // = (1 <<  5);
      static const BaseFlags  InfiniteCapacity    ; // = (1 <<  6);
    // Flags for Anabatic objects states only.                      
      static const BaseFlags  DemoMode            ; // = (1 <<  5);
      static const BaseFlags  WarnOnGCellOverload ; // = (1 <<  6);
      static const BaseFlags  DestroyGCell        ; // = (1 <<  7);
      static const BaseFlags  DestroyBaseContact  ; // = (1 <<  8);
      static const BaseFlags  DestroyBaseSegment  ; // = (1 <<  9);
      static const BaseFlags  DisableCanonize     ; // = (1 << 10);
    // Flags for NetDatas objects states only.                      
      static const BaseFlags  GlobalFixed         ; // = (1 <<  5);
      static const BaseFlags  GlobalEstimated     ; // = (1 <<  6);
      static const BaseFlags  GlobalRouted        ; // = (1 <<  7);
      static const BaseFlags  DetailRouted        ; // = (1 <<  8);
      static const BaseFlags  ExcludeRoute        ; // = (1 <<  9);
    // Masks.                                      
      static const BaseFlags  WestSide            ; // = Horizontal|Target;
      static const BaseFlags  EastSide            ; // = Horizontal|Source;
      static const BaseFlags  SouthSide           ; // = Vertical  |Target;
      static const BaseFlags  NorthSide           ; // = Vertical  |Source;
      static const BaseFlags  AllSides            ; // = WestSide|EastSide|SouthSide|NorthSide ;
      static const BaseFlags  EndsMask            ; // = Source|Target;
      static const BaseFlags  DirectionMask       ; // = Horizontal|Vertical;
      static const BaseFlags  DestroyMask         ; // = DestroyGCell|DestroyBaseContact|DestroyBaseSegment;
      static const BaseFlags  GCellTypeMask       ; // = DeviceGCell|HChannelGCell|VChannelGCell|StrutGCell|MatrixGCell|IoPadGCell|HRailGCell|VRailGCell;
      static const BaseFlags  RowGCellMask        ; // = StdCellRow|ChannelRow;
      static const BaseFlags  AnalogGCellMask     ; // = DeviceGCell|HChannelGCell|VChannelGCell|StrutGCell|HRailGCell|VRailGCell;
      static const BaseFlags  EdgeCapacityMask    ; // = Horizontal|Vertical|NullCapacity|InfiniteCapacity ;
    // Flags for functions arguments only.           
      static const BaseFlags  Create              ; // = (1 <<  5);
      static const BaseFlags  WithPerpands        ;
      static const BaseFlags  WithDoglegs         ;
      static const BaseFlags  WithSelf            ;
      static const BaseFlags  AboveLayer          ;
      static const BaseFlags  BelowLayer          ;
      static const BaseFlags  OpenSession         ;
      static const BaseFlags  Realignate          ;
      static const BaseFlags  NativeConstraints   ;
      static const BaseFlags  ForceMove           ;
      static const BaseFlags  WarnOnError         ;
      static const BaseFlags  Topology            ;
      static const BaseFlags  GlobalSegment       ;
      static const BaseFlags  AllowTerminal       ;
      static const BaseFlags  AllowLocal          ;
      static const BaseFlags  IgnoreContacts      ;
      static const BaseFlags  Propagate           ;
      static const BaseFlags  Superior            ;
      static const BaseFlags  DoglegOnLeft        ;
      static const BaseFlags  DoglegOnRight       ;
      static const BaseFlags  WithNeighbors       ;
      static const BaseFlags  NoCheckLayer        ;
      static const BaseFlags  HalfSlacken         ;
      static const BaseFlags  NoGCellShrink       ;
      static const BaseFlags  CParanoid           ;
      static const BaseFlags  CheckLowDensity     ;
      static const BaseFlags  CheckLowUpDensity   ;
      static const BaseFlags  NoUpdate            ;
      static const BaseFlags  NorthPath           ;
      static const BaseFlags  UseNonPref          ;
      static const BaseFlags  Force               ;
      static const BaseFlags  LayerCapOnly        ;
      static const BaseFlags  NoMinLength         ;
      static const BaseFlags  NoSegExt            ;
      static const BaseFlags  NullLength          ;
      static const BaseFlags  OnVSmall            ;
    public:
      inline               Flags        ( uint64_t flags = NoFlags );
      inline               Flags        ( const Hurricane::BaseFlags& );
      virtual             ~Flags        ();
      virtual std::string  asString     ( uint32_t mode ) const;
      virtual std::string  _getTypeName () const;
      virtual std::string  _getString   () const;
  };


  Flags::Flags (                  uint64_t   flags ) : BaseFlags(flags) { }
  Flags::Flags ( const Hurricane::BaseFlags& flags ) : BaseFlags(flags) { }


// -------------------------------------------------------------------
// Class  :  "Anabatic::StyleFlags".

  class StyleFlags : public Hurricane::BaseFlags {
    public:
      static const BaseFlags  NoStyle; // =  0;
      static const BaseFlags  HV     ; // = (1 <<  0);
      static const BaseFlags  VH     ; // = (1 <<  1);
      static const BaseFlags  OTH    ; // = (1 <<  2);
      static const BaseFlags  Channel; // = (1 <<  3);
      static const BaseFlags  Hybrid ; // = (1 <<  4);
    public:
      inline               StyleFlags   ( std::string );
      inline               StyleFlags   ( uint64_t flags = NoStyle );
      inline               StyleFlags   ( const Hurricane::BaseFlags& );
      virtual             ~StyleFlags   ();
      static  StyleFlags   toFlag       ( std::string );
              StyleFlags   from         ( std::string );
      virtual std::string  asString     () const;
      virtual std::string  _getTypeName () const;
      virtual std::string  _getString   () const;
  };


  StyleFlags::StyleFlags (             std::string textFlags ) : BaseFlags(NoStyle) { from(textFlags); }
  StyleFlags::StyleFlags (                  uint64_t   flags ) : BaseFlags(flags) { }
  StyleFlags::StyleFlags ( const Hurricane::BaseFlags& flags ) : BaseFlags(flags) { }


// -------------------------------------------------------------------
// Misc. enums.


  enum FlagsMode       { FlagsFunction = 1
                       };


  enum EngineState     { EngineCreation      = 1
                       , EngineGlobalLoaded  = 2
                       , EngineActive        = 3
                       , EngineDriving       = 4
                       , EnginePreDestroying = 5
                       , EngineGutted        = 6
                       };

  enum EngineAlgorithm { EngineLoadGrByNet            = (1 <<  0)
                       , EngineLoadGrByGCell          = (1 <<  1)
                       , EngineLayerAssignByLength    = (1 <<  2)
                       , EngineLayerAssignByTrunk     = (1 <<  3)
                       , EngineLayerAssignNoGlobalM2V = (1 <<  4)
                       , EngineNoNetLayerAssign       = (1 <<  5)
                       };


  inline Flags  perpandicularTo ( Flags direction )
  {
    if (direction & Flags::Horizontal) return Flags::Vertical;
    if (direction & Flags::Vertical  ) return Flags::Horizontal;
    return Flags::NoFlags;
  }


}  // Anabatic namespace.


INSPECTOR_PR_SUPPORT(Anabatic::Flags);
