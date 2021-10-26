// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  : "./hurricane/configuration/PyLayoutDescription.h"|
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/configuration/PyTypeManager.h"
#include "hurricane/configuration/LayoutDescription.h"


namespace  Cfg {


extern "C" {


  extern PyMethodDef   PyLayoutDescription_Methods[];


}  // extern "C".

}  // Cfg namespace.
