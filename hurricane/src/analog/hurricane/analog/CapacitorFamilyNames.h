// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/CapacitorFamilyNames.h"     |
// +-----------------------------------------------------------------+

#ifndef ANALOG_CAPACITOR_FAMILYNAMES_H
#define ANALOG_CAPACITOR_FAMILYNAMES_H

#include <string>

namespace Analog {


  class CapacitorFamilyNames {
    public:
      static const char* PIPCapacitor;
      static const char* MIMCapacitor;
      static const char* MOMCapacitor;
  };


}  // Analog namespace.


#endif  // ANALOG_CAPACITOR_FAMILYNAMES_H
