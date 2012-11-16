
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyGraphicToolEngine.h"               |
// +-----------------------------------------------------------------+


#ifndef __CRLCORE_PY_GRAPHIC_TOOL_ENGINE__
#define __CRLCORE_PY_GRAPHIC_TOOL_ENGINE__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/GraphicToolEngine.h"


namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyGraphicToolEngine".

  typedef struct {
      PyObject_HEAD
      GraphicTool* _object;
  } PyGraphicTool;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeGraphicTool;
  extern  PyMethodDef   PyGraphicTool_Methods[];
  extern  void          PyGraphicTool_LinkPyType ();


#define IsPyGraphicTool(v)    ( (v)->ob_type == &PyTypeGraphicTool )
#define PY_GRAPHIC_TOOL(v)    ( (PyGraphicTool*)(v) )
#define PY_GRAPHIC_TOOL_O(v)  ( PY_GRAPHIC_TOOL(v)->_object )


}  // End of extern "C".

}  // CRL namespace.

#endif // __CRLCORE_PY_GRAPHIC_TOOL_ENGINE__
