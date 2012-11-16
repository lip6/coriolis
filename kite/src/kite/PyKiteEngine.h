
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./kite/PyKiteEngine.cpp"                       |
// +-----------------------------------------------------------------+


#ifndef __PY_KITE_ENGINE__
#define __PY_KITE_ENGINE__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "kite/KiteEngine.h"


namespace  Kite {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyKiteEngine".

  typedef struct {
      CRL::PyToolEngine  _baseObject;
  } PyKiteEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyKite.ccp".

  extern  PyTypeObject  PyTypeKiteEngine;
  extern  PyMethodDef   PyKiteEngine_Methods[];

  extern  PyObject* PyKiteEngine_Link       ( Kite::KiteEngine* );
  extern  void      PyKiteEngine_LinkPyType ();


#define IsPyKiteEngine(v)  ( (v)->ob_type == &PyTypeKiteEngine )
#define PYKITEENGINE(v)    ( (PyKiteEngine*)(v) )
#define PYKITEENGINE_O(v)  ( PYKITEENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Kite namespace.

#endif  // __PY_KITE_ENGINE__
