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
      static const unsigned int  AllowDoglegReuse;
      static const unsigned int  DataSelf;
      static const unsigned int  Nearest;
      static const unsigned int  Force;
      static const unsigned int  ResetCount;
      static const unsigned int  WithConstraints;
      static const unsigned int  MoveToLeft;
      static const unsigned int  MoveToRight;
      static const unsigned int  LoadingStage;
      static const unsigned int  SlowMotion;
      static const unsigned int  PreRoutedStage;
    public:
      inline  Flags ( unsigned int );
      inline  Flags ( const Super& );
  };


  inline  Flags::Flags (       unsigned int  flags ) : Super(flags) { }
  inline  Flags::Flags ( const Flags::Super& base  ) : Super(base)  { }


} // Katana namespace.

#endif  // KATANA_CONSTANTS_H
