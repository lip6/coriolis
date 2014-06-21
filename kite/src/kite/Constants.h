// -*- mode: C++; explicit-buffer-name: "Constants.h<kite>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./kite/Constants.h"                            |
// +-----------------------------------------------------------------+


#ifndef  KITE_CONSTANTS_H
#define  KITE_CONSTANTS_H

#include "katabatic/Constants.h"

namespace Kite {

  using Katabatic::KbOpenSession;
  using Katabatic::KbRealignate;
  using Katabatic::KbNativeConstraints;
  using Katabatic::KbForceMove;
  using Katabatic::KbHorizontal;
  using Katabatic::KbVertical;
  using Katabatic::KbWithPerpands;
  using Katabatic::KbSource;
  using Katabatic::KbTarget;
  using Katabatic::KbWarnOnError;
  using Katabatic::perpandicularTo;

  enum FunctionFlags { KtNoFlags            = 0x00000000
                     , KtLoadGlobalRouting  = 0x00000001
                     , KtBuildGlobalRouting = 0x00000002
                     , KtAllowDoglegReuse   = 0x00000004
                     , KtDataSelf           = 0x00000008
                     , KtNearest            = 0x00000010
                     , KtForce              = 0x00000020
                     , KtResetCount         = 0x00000040
                     , KtWithPerpands       = 0x00000080
                     , KtWithConstraints    = 0x00000100
                     , KtMoveToLeft         = 0x00000200
                     , KtMoveToRight        = 0x00000400
                     , KtLoadingStage       = 0x00000800
                     , KtSlowMotion         = 0x00001000
                     , KtPreRoutedStage     = 0x00002000
                     , };

} // Kite namespace.

#endif  // KITE_CONSTANTS_H
