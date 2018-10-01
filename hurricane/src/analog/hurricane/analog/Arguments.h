// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricaneAMS/devices/Arguments.h"            |
// +-----------------------------------------------------------------+


#ifndef ANALOG_ARGUMENTS_H
#define ANALOG_ARGUMENTS_H

namespace Analog {


  class Arguments {
    public:
      virtual Arguments* getClone  () = 0;
      virtual void       setM      ( unsigned int ) { };
    protected:
                         Arguments () {};
      virtual           ~Arguments () {};
  };


}  // Analog namespace.

#endif  // ANALOG_ARGUMENTS_H
