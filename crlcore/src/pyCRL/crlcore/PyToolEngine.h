
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
// |  C++ Header  :  "./crlcore/PyToolEngine.h"                      |
// +-----------------------------------------------------------------+


# ifndef  __CRL_PYTOOLENGINE__
# define  __CRL_PYTOOLENGINE__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/ToolEngine.h"

namespace  CRL {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyToolEngine".

  typedef struct {
      PyObject_HEAD
      ToolEngine* _object;
  } PyToolEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypeToolEngine;
  extern  PyMethodDef   PyToolEngine_Methods[];

  extern  PyObject* PyToolEngine_Link           ( ToolEngine* object );
  extern  void      PyToolEngine_LinkPyType     ();

# define IsPyToolEngine(v)    ( (v)->ob_type == &PyTypeToolEngine )
# define PYTOOLENGINE(v)      ( (PyToolEngine*)(v) )
# define PYTOOLENGINE_O(v)    ( PYTOOLENGINE(v)->_object )


}  // extern "C".

}  // CRL namespace.
 
# endif
