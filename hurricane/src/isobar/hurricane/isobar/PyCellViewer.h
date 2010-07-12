
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PyCellViewer.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __ISOBAR_CELL_VIEWER__
#define  __ISOBAR_CELL_VIEWER__

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/CellViewer.h"


namespace  Isobar {


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

  extern PyObject* PyCellViewer_create     ( PyObject* self, PyObject* args );
  extern void      PyCellViewer_LinkPyType ();


#define  IsPyCellViewer(v)      ( (v)->ob_type == &PyTypeCellViewer )
#define  ISOBARCELLVIEWER(v)    ( (PyCellViewer*)(v) )
#define  ISOBARCELLVIEWER_O(v)  ( ISOBAR_CELL_VIEWER(v)->_object )


}  // End of extern "C".


}  // End of Isobar namespace.


#endif  // __ISOBAR_CELL_VIEWER__
