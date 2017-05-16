// -*- mode: C++; explicit-buffer-name: "Constants.h<katana>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/Constants.h"                          |
// +-----------------------------------------------------------------+


#ifndef  KATANA_CONSTANTS_H
#define  KATANA_CONSTANTS_H

#include "anabatic/Constants.h"

namespace Katana {


  class Flags : public Anabatic::Flags {
    public:
      typedef  Anabatic::Flags  Super;
    public:
      static const uint64_t  AllowDoglegReuse;
      static const uint64_t  DataSelf;
      static const uint64_t  Nearest;
      static const uint64_t  Force;
      static const uint64_t  ResetCount;
      static const uint64_t  WithConstraints;
      static const uint64_t  MoveToLeft;
      static const uint64_t  MoveToRight;
      static const uint64_t  LoadingStage;
      static const uint64_t  SlowMotion;
      static const uint64_t  PreRoutedStage;
      static const uint64_t  SymmetricStage;
    public:
      inline  Flags ( uint64_t );
      inline  Flags ( const Super& );
      inline  Flags ( const Hurricane::BaseFlags& );
  };


  inline  Flags::Flags (       uint64_t              flags ) : Super(flags) { }
  inline  Flags::Flags ( const Flags::Super&         flags ) : Super(flags) { }
  inline  Flags::Flags ( const Hurricane::BaseFlags& flags ) : Super(flags) { }


} // Katana namespace.

#endif  // KATANA_CONSTANTS_H
