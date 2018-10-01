// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/PyLayoutGenerator.h"        |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_LAYOUT_GENERATOR_H
#define  ANALOG_PY_LAYOUT_GENERATOR_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/analog/LayoutGenerator.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyLayoutGenerator".

  typedef struct {
      PyObject_HEAD
      Analog::LayoutGenerator* _object;
  } PyLayoutGenerator;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeLayoutGenerator;
  extern PyMethodDef   PyLayoutGenerator_Methods[];

  extern PyObject* PyLayoutGenerator_Link           ( Analog::LayoutGenerator* object );
  extern void      PyLayoutGenerator_LinkPyType     ();
  extern void      PyLayoutGenerator_postModuleInit ();


#define IsPyLayoutGenerator(v)    ( (v)->ob_type == &PyTypeLayoutGenerator )
#define PYLAYOUTGENERATOR(v)      ( (PyLayoutGenerator*)(v) )
#define PYLAYOUTGENERATOR_O(v)    ( PYLAYOUTGENERATOR(v)->_object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_LAYOUT_GENERATOR_H
