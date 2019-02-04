// -*- C++ -*-
//
// This file is part of the Coriols Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/PyParameter.h"              |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_PARAMETER_H
#define  ANALOG_PY_PARAMETER_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/analog/Parameter.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyParameter".

  typedef struct {
      PyObject_HEAD
      Analog::Parameter* _object;
  } PyParameter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypeParameter;
  extern  PyMethodDef   PyParameter_Methods[];

  extern  PyObject* PyParameter_Link           ( Analog::Parameter* object );
  extern  PyObject* PyParameter_LinkDerived    ( Analog::Parameter* object );
  extern  void      PyParameter_LinkPyType     ();
  extern  void      PyParameter_postModuleInit ();

# define IsPyParameter(v)    ( (v)->ob_type == &PyTypeParameter )
# define PYPARAMETER(v)      ( (PyParameter*)(v) )
# define PYPARAMETER_O(v)    ( PYPARAMETER(v)->_object )


}  // extern "C".

}  // Isobar namespace.
 
#endif  // ANALOG_PY_PARAMETER_H
