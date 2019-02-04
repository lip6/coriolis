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
// |  C++ Module  :  "./BJTFamily.cpp"                               |
// +-----------------------------------------------------------------+


#include "hurricane/Library.h"
#include "hurricane/Name.h"
#include "hurricane/analog/BJTFamily.h"


namespace Analog {
  
  using namespace Hurricane;


  BJTFamily::BJTFamily ( Library* library, const Name& name, const Type& type )
    : Super(library,name)
    , _type         (type)
    , _operatorIndex(0)
  { }


}  // Analog namespace.
