
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./unicorn/PyUnicornGui.h"                      |
// +-----------------------------------------------------------------+


#ifndef __UNICORN_PY_UNICORN_GUI_H__
#define __UNICORN_PY_UNICORN_GUI_H__

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/PyCellViewer.h"
#include "unicorn/UnicornGui.h"


namespace Unicorn {

  using Hurricane::PyCellViewer;

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyUnicornGui".

    typedef struct {
        PyCellViewer  _baseObject;
    } PyUnicornGui;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypeUnicornGui;
    extern  PyMethodDef   PyUnicornGui_Methods[];
    extern  void          PyUnicornGui_LinkPyType ();


#define IsPyUnicornGui(v)    ( (v)->ob_type == &PyTypeUnicornGui )
#define PY_UNICORN_GUI(v)    ( (PyUnicornGui*)(v) )
#define PY_UNICORN_GUI_O(v)  ( PY_UNICORN_GUI(v)->_baseObject._object )


}  // extern "C".

}  // Unicorn namespace.

#endif // __UNICORN_PY_UNICORN_GUI_H__
