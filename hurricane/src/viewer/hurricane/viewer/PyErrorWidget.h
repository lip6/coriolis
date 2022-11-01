// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/PyErrorWidget.cpp"          |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/ErrorWidget.h"


namespace  Hurricane {


extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyErrorWidget".

  typedef struct {
      PyObject_HEAD
      Hurricane::ErrorWidget* _object;
  } PyErrorWidget;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeErrorWidget;
  extern PyMethodDef   PyErrorWidget_Methods[];

  extern PyObject* PyErrorWidget_create         ( PyObject* self, PyObject* args );
  extern void      PyErrorWidget_LinkPyType     ();


#define  IsPyErrorWidget(v)     ( (v)->ob_type == &PyTypeErrorWidget )
#define  PYERRORWIDGET(v)       ( (PyErrorWidget*)(v) )
#define  PYERRORWIDGET_O(v)     ( PYERRORWIDGET(v)->_object )


}  // extern "C".


}  // Isobar namespace.
