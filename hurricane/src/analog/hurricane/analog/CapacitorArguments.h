// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2011-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/CapacitorArguments.h"       |
// +-----------------------------------------------------------------+


#ifndef ANALOG_CAPACITOR_ARGUMENTS_H
#define ANALOG_CAPACITOR_ARGUMENTS_H

#include "hurricane/DbU.h"
#include "hurricane/analog/Arguments.h"

namespace Analog {
  

  class CapacitorArguments : public Arguments {
    public:
      typedef Arguments Super;
    public:
                                  CapacitorArguments ( bool isMIM, bool isPIP, bool isMOM, double c );
                                  CapacitorArguments ( const CapacitorArguments& cArgs );
      virtual CapacitorArguments* getClone           ();
      inline  double              getC               () const; 
      inline  bool                isMIM              () const;
      inline  bool                isPIP              () const;
      inline  bool                isMOM              () const;
    private:
      bool    _isMIM;
      bool    _isPIP;
      bool    _isMOM;
      double  _c;
  };
  
  
  inline double  CapacitorArguments::getC  () const { return _c; }
  inline bool    CapacitorArguments::isMIM () const { return _isMIM;}
  inline bool    CapacitorArguments::isPIP () const { return _isPIP;}
  inline bool    CapacitorArguments::isMOM () const { return _isMOM;}
  

}  // Analog namespace.

#endif  // ANALOG_CAPACITOR_ARGUMENTS_H 
