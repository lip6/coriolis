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
// |  C++ Header  :  "./hurricane/configuration/PyConfiguration.h"   |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/configuration/PyTypeManager.h"
#include "hurricane/configuration/Configuration.h"


namespace  Cfg {


extern "C" {


// -------------------------------------------------------------------
// Functions & Types exported to "PyConfiguration.cpp".

  extern PyMethodDef  PyConfiguration_Methods[];
  extern PyGetSetDef  PyConfiguration_Getsets[];


}  // extern "C".

}  // Cfc namespace.
