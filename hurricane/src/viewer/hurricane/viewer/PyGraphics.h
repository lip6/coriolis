
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/PyGraphics.h"               |
// +-----------------------------------------------------------------+


#ifndef __VIEWER_PY_GRAPHICS__
#define __VIEWER_PY_GRAPHICS__

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/Graphics.h"


namespace  Hurricane {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyGraphics".

  typedef struct {
      PyObject_HEAD
      Graphics* _object;
  } PyGraphics;


// -------------------------------------------------------------------
// Functions & Types exported to "PyViewer.ccp".

  extern  PyTypeObject  PyTypeGraphics;
  extern  PyMethodDef   PyGraphics_Methods[];

  extern  PyObject* PyGraphics_Link       ( Graphics* );
  extern  void      PyGraphics_LinkPyType ();


#define IsPyGraphics(v)  ( (v)->ob_type == &PyTypeGraphics )
#define PYGRAPHICS(v)    ( (PyGraphics*)(v) )
#define PYGRAPHICS_O(v)  ( PYGRAPHICS(v)->_object )


}  // extern "C".


}  // Hurricane namespace.


#endif  // __VIEWER_PY_GRAPHICS__
