
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
// |  C++ Header  :  "./hurricane/viewer/PyDrawingGroup.h"           |
// +-----------------------------------------------------------------+


#ifndef __VIEWER_PY_DRAWINGGROUP__
#define __VIEWER_PY_DRAWINGGROUP__

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/DisplayStyle.h"


namespace  Hurricane {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyDrawingGroup".

  typedef struct {
      PyObject_HEAD
      DrawingGroup* _object;
  } PyDrawingGroup;


// -------------------------------------------------------------------
// Functions & Types exported to "PyViewer.ccp".

  extern  PyTypeObject  PyTypeDrawingGroup;
  extern  PyMethodDef   PyDrawingGroup_Methods[];

  extern  PyObject* PyDrawingGroup_Link       ( DrawingGroup* );
  extern  void      PyDrawingGroup_LinkPyType ();


#define IsPyDrawingGroup(v)  ( (v)->ob_type == &PyTypeDrawingGroup )
#define PYDRAWINGGROUP(v)    ( (PyDrawingGroup*)(v) )
#define PYDRAWINGGROUP_O(v)  ( PYDRAWINGGROUP(v)->_object )


// Vector of DrawingStyles.
  declareVectorObject(DrawingGroup);


}  // extern "C".


}  // Hurricane namespace.


#endif  // __VIEWER_PY_DRAWINGGROUP__
