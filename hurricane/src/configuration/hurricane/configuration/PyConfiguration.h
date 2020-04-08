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
#include "hurricane/configuration/PyHurricane2.h"
#include "hurricane/configuration/Configuration.h"


namespace  Cfg2 {


extern "C" {


// -------------------------------------------------------------------
// Functions & Types exported to "PyConfiguration.cpp".

  extern PyTypeObject  PyTypeConfiguration;
  extern PyMethodDef   PyConfiguration_Methods[];


//   extern PyObject* PyConfiguration_create         ( PyObject* self, PyObject* args );
//   extern PyObject* PyConfiguration_Link           ( Hurricane::Configuration* object );
//   extern void      PyConfiguration_LinkPyType     ();
//   extern void      PyConfiguration_postModuleInit ();


// #define  IsPyConfiguration(v)   ( (v)->ob_type == &PyTypeConfiguration )
// #define  PYCELLVIEWER(v)        ( (PyConfiguration*)(v) )
// #define  PYCELLVIEWER_O(v)      ( PYCELLVIEWER(v)->_object )


}  // extern "C".

}  // Cfc namespace.
