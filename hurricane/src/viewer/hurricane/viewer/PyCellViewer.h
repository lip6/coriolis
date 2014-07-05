// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/PyCellViewer.cpp"           |
// +-----------------------------------------------------------------+


#ifndef  PY_HURRICANE_CELL_VIEWER_H
#define  PY_HURRICANE_CELL_VIEWER_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/CellViewer.h"


namespace  Hurricane {


extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyCellViewer".

  typedef struct {
      PyObject_HEAD
      Hurricane::CellViewer* _object;
  } PyCellViewer;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeCellViewer;
  extern PyMethodDef   PyCellViewer_Methods[];

  extern PyObject* PyCellViewer_create         ( PyObject* self, PyObject* args );
  extern PyObject* PyCellViewer_Link           ( Hurricane::CellViewer* object );
  extern void      PyCellViewer_LinkPyType     ();
  extern void      PyCellViewer_postModuleInit ();


#define  IsPyCellViewer(v)      ( (v)->ob_type == &PyTypeCellViewer )
#define  PYCELLVIEWER(v)        ( (PyCellViewer*)(v) )
#define  PYCELLVIEWER_O(v)      ( ISOBAR_CELL_VIEWER(v)->_object )


}  // End of extern "C".


}  // End of Isobar namespace.


#endif  // PY_HURRICANE_CELL_VIEWER_H
