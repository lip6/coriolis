
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
// |  C++ Module  :       "./PyCellViewer.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyCellViewer.h"
#include "hurricane/viewer/CellWidget.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(CellViewer,cw,function)


// x=================================================================x
// |             "PyCellViewer" Python Module Code Part              |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // Standart Destroy (Attribute).
  DirectDestroyAttribute(PyCellViewer_destroy, PyCellViewer)


  static PyObject* PyCellViewer_getCell ( PyCellViewer* self )
  {
    trace << "PyCellViewer_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD("CellViewer.getCell()")

    cell = cw->getCell ();
    HCATCH

    if ( cell == NULL ) Py_RETURN_NONE;

    return PyCell_Link(cell);
  }


  static PyObject* PyCellViewer_setCell ( PyCellViewer* self, PyObject* args )
  {
    trace << "PyCellViewer_setCell ()" << endl;

    HTRY
    METHOD_HEAD("CellViewer.setCell()")

    PyCell* cell;
    if ( not ParseOneArg("CellViewer.setCell()",args,CELL_ARG,(PyObject**)&cell) ) return NULL;

    cw->setCell ( PYCELL_O(cell) );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyCellViewer_fit ( PyCellViewer* self )
  {
    trace << "PyCellViewer_fit()" << endl;

    HTRY
    METHOD_HEAD("CellViewer.fit()")
    cw->getCellWidget()->fitToContents();
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyCellViewer Attribute Method table.

  PyMethodDef PyCellViewer_Methods[] =
    { { "getCell"        , (PyCFunction)PyCellViewer_getCell        , METH_NOARGS
                         , "Return the currently edited Cell." }
    , { "setCell"        , (PyCFunction)PyCellViewer_setCell        , METH_VARARGS
                         , "Load a Cell into the viewer." }
    , { "fit"            , (PyCFunction)PyCellViewer_fit            , METH_NOARGS
                         , "Fit the contents to the viewer's visible area." }
    , { "destroy"        , (PyCFunction)PyCellViewer_destroy        , METH_NOARGS
                         , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  PythonOnlyDeleteMethod(CellViewer)
  PyTypeObjectLinkPyType(CellViewer)


#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyCellViewer" Shared Library Code Part              |
// x=================================================================x


  PyTypeObjectDefinitions(CellViewer)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Isobar namespace.
