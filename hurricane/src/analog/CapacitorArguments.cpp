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
// |  C++ Header  :  "./CapacitorArguments.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/analog/CapacitorArguments.h"

namespace Analog {
  

  CapacitorArguments::CapacitorArguments ( bool isMIM, bool isPIP, bool isMOM, double c ) 
	: Super()
    , _isMIM(isMIM)
	, _isPIP(isPIP)
	, _isMOM(isMOM)
	, _c	(c)
  { }


  CapacitorArguments::CapacitorArguments(const CapacitorArguments &cArgs) 
	: Super()
    , _isMIM(cArgs.isMIM())
	, _isPIP(cArgs.isPIP())
	, _isMOM(cArgs.isMOM())
	, _c	(cArgs.getC())
  { }


  CapacitorArguments* CapacitorArguments::getClone ()
  { return new CapacitorArguments( *this ); }


}  // Analog namespace.
