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
// |  C++ Header  :  "./hurricane/viewer/PyAboutWindow.cpp"          |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/AboutWindow.h"


namespace  Hurricane {


extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyAboutWindow".

  typedef struct {
      PyObject_HEAD
      Hurricane::AboutWindow* _object;
  } PyAboutWindow;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeAboutWindow;
  extern PyMethodDef   PyAboutWindow_Methods[];

  extern PyObject* PyAboutWindow_create         ( PyObject* self, PyObject* args );
  extern void      PyAboutWindow_LinkPyType     ();


#define  IsPyAboutWindow(v)     ( (v)->ob_type == &PyTypeAboutWindow )
#define  PYABOUTWINDOW(v)       ( (PyAboutWindow*)(v) )
#define  PYABOUTWINDOW_O(v)     ( PYABOUTWINDOW(v)->_object )


}  // extern "C".


}  // Isobar namespace.
