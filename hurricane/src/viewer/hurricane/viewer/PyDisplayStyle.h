
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
// |  C++ Header  :  "./hurricane/viewer/PyDisplayStyle.h"           |
// +-----------------------------------------------------------------+


#ifndef __VIEWER_PY_DISPLAY_STYLE__
#define __VIEWER_PY_DISPLAY_STYLE__

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/DisplayStyle.h"


namespace  Hurricane {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyDisplayStyle".

  typedef struct {
      PyObject_HEAD
      DisplayStyle* _object;
  } PyDisplayStyle;


// -------------------------------------------------------------------
// Functions & Types exported to "PyViewer.ccp".

  extern  PyTypeObject  PyTypeDisplayStyle;
  extern  PyMethodDef   PyDisplayStyle_Methods[];

  extern  PyObject* PyDisplayStyle_Link           ( DisplayStyle* );
  extern  void      PyDisplayStyle_LinkPyType     ();
  extern  void      PyDisplayStyle_postModuleInit ();


#define IsPyDisplayStyle(v)   ( (v)->ob_type == &PyTypeDisplayStyle )
#define PYDISPLAY_STYLE(v)    ( (PyDisplayStyle*)(v) )
#define PYDISPLAY_STYLE_O(v)  ( PYDISPLAY_STYLE(v)->_object )


// Vector of DrawingStyles.
  declareVectorObject(DisplayStyle);


}  // extern "C".


}  // Hurricane namespace.


#endif  // __VIEWER_PY_DISPLAY_STYLE__
