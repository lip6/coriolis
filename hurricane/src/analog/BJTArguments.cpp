// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2011-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                   Stephanie Youssef              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./BJTArguments.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/analog/BJTArguments.h"


namespace Analog {
  

  BJTArguments::BJTArguments ( bool isNPN, bool isExternalDummy, unsigned m, unsigned n )
    : Super()
    , _isNPN          (isNPN) 
    , _isExternalDummy(isExternalDummy)
    , _m              (m)
    , _n              (n)
  { }

  
  BJTArguments::BJTArguments ( const BJTArguments& bjtArgs )
    : Super()
    , _isNPN          (bjtArgs.isNPN())
    , _isExternalDummy(bjtArgs.isExternalDummy())
    , _m              (bjtArgs.getM())
    , _n              (bjtArgs.getN())
  { }
  

  BJTArguments* BJTArguments::getClone ()
  { return new BJTArguments( *this ); }


}  // Analog namespace.
