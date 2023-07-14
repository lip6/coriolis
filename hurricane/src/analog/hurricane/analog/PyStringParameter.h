// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/PyStringParameter.h"        |
// +-----------------------------------------------------------------+


#pragma once
#include "hurricane/analog/PyParameter.h"
#include "hurricane/analog/StringParameter.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyStringParameter".

  typedef struct {
      PyParameter  _baseObject;
  } PyStringParameter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeStringParameter;
  extern PyMethodDef   PyStringParameter_Methods[];

  extern PyObject* PyStringParameter_Link       ( Analog::StringParameter* object );
  extern void      PyStringParameter_LinkPyType ();


#define IsPyStringParameter(v)  ( (v)->ob_type == &PyTypeStringParameter )
#define PYSTRINGPARAMETER(v)    ( (PyStringParameter*)(v) )
#define PYSTRINGPARAMETER_O(v)  ( PYSTRINGPARAMETER(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.
