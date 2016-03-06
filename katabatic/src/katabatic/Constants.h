// -*- mode: C++; explicit-buffer-name: "Constants.h<katabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2013-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/Constants.h"                       |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_CONSTANTS_H
#define  KATABATIC_CONSTANTS_H

namespace Katabatic {

  enum FunctionFlag    { KbNoFlags           = 0x00000000
                       , KbOpenSession       = 0x00000001
                       , KbRealignate        = 0x00000002
                       , KbNativeConstraints = 0x00000004
                       , KbForceMove         = 0x00000008
                       , KbHorizontal        = 0x00000010
                       , KbVertical          = 0x00000020
                       , KbWithPerpands      = 0x00000040
                       , KbSource            = 0x00000080
                       , KbTarget            = 0x00000100
                       , KbWarnOnError       = 0x00000200
                       , KbTopology          = 0x00000400
                       , KbGlobalSegment     = 0x00000800
                       , KbAllowTerminal     = 0x00001000
                       , KbAllowLocal        = 0x00002000
                       , KbIgnoreContacts    = 0x00004000
                       , KbPropagate         = 0x00008000
                       , KbSuperior          = 0x00010000
                       , KbUseAboveLayer     = 0x00020000
                       , KbUseBelowLayer     = 0x00040000
                       , KbDoglegOnLeft      = 0x00080000
                       , KbDoglegOnRight     = 0x00100000
                       , KbWithNeighbors     = 0x00200000
                       , KbNoCheckLayer      = 0x00400000
                       , KbHalfSlacken       = 0x00800000
                       , KbNoGCellShrink     = 0x01000000
                       , KbCParanoid         = 0x02000000
                       , KbCreate            = 0x04000000
                       , KbDirectionMask     = KbHorizontal|KbVertical
                       };

  enum EngineState     { EngineCreation      = 1
                       , EngineGlobalLoaded  = 2
                       , EngineActive        = 3
                       , EngineDriving       = 4
                       , EnginePreDestroying = 5
                       , EngineGutted        = 6
                       };
                       
  enum EngineFlag      { EngineDemoMode            = 0x00000001
                       , EngineWarnOnGCellOverload = 0x00000002
                       , EngineDestroyBaseContact  = 0x00000004
                       , EngineDestroyBaseSegment  = 0x00000008
                       , EngineDestroyMask         = EngineDestroyBaseContact|EngineDestroyBaseSegment
                       };

  enum EngineAlgorithm { EngineLoadGrByNet         = 0x00000001
                       , EngineLoadGrByGCell       = 0x00000002
                       , EngineLayerAssignByLength = 0x00000004
                       , EngineLayerAssignByTrunk  = 0x00000008
                       , EngineNoNetLayerAssign    = 0x00000010
                       };


  inline unsigned int  perpandicularTo ( unsigned int direction )
  {
    switch ( direction & KbDirectionMask ) {
      case KbHorizontal: return KbVertical;
      case KbVertical:   return KbHorizontal;
    }
    return 0;
  }


} // Katabatic namespace.

#endif  // KATABATIC_CONSTANTS_H









































































