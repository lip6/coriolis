
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
// |  C++ Header  :  "./hurricane/viewer/PyDrawingStyle.h"           |
// +-----------------------------------------------------------------+


#ifndef __VIEWER_PY_DRAWINGSTYLE__
#define __VIEWER_PY_DRAWINGSTYLE__

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/DisplayStyle.h"


namespace  Hurricane {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyRawDrawingStyle".

  typedef struct {
      PyObject_HEAD
      RawDrawingStyle* _object;
  } PyRawDrawingStyle;


// -------------------------------------------------------------------
// Functions & Types exported to "PyViewer.ccp".

  extern  PyTypeObject  PyTypeRawDrawingStyle;
  extern  PyMethodDef   PyRawDrawingStyle_Methods[];

  extern  PyObject* PyRawDrawingStyle_Link       ( DrawingStyle );
  extern  void      PyRawDrawingStyle_LinkPyType ();


#define IsPyRawDrawingStyle(v)  ( (v)->ob_type == &PyTypeRawDrawingStyle )
#define PYRAWDRAWINGSTYLE(v)    ( (PyRawDrawingStyle*)(v) )
#define PYRAWDRAWINGSTYLE_O(v)  ( PYRAWDRAWINGSTYLE(v)->_object )


// Vector of DrawingStyles.
  declareVectorValObject(DrawingStyle);


}  // extern "C".


}  // Hurricane namespace.


#endif  // __VIEWER_PY_DRAWINGSTYLE__
