
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyCellViewer.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "hurricane/viewer/PyCellViewer.h"
#include "hurricane/viewer/CellWidget.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(CellViewer,cw,function)


// +=================================================================+
// |             "PyCellViewer" Python Module Code Part              |
// +=================================================================+

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
    if ( not ParseOneArg("CellViewer.setCell()",args,CELL_ARG,(PyObject**)&cell) ) {
      return NULL;
    }

    cw->setCell ( PYCELL_O(cell) );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyCellViewer_setApplicationName ( PyCellViewer* self, PyObject* args )
  {
    trace << "PyCellViewer_setApplicationName ()" << endl;

    HTRY
    METHOD_HEAD("CellViewer.setApplicationName()")

    char* name = NULL;
    if (not PyArg_ParseTuple(args,"s:CellViewer.setApplicationName()", &name)) {
      PyErr_SetString ( ConstructorError, "CellViewer.setApplicationName(): Takes exactly one argument." );
      return NULL;
    }

    cw->setApplicationName ( name );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyCellViewer_setLayerVisible ( PyCellViewer* self, PyObject* args )
  {
    trace << "PyCellViewer_setLayerVisible ()" << endl;

    HTRY
    METHOD_HEAD("CellViewer.setLayerVisible()")

    char*     layerName = NULL;
    PyObject* visible   = NULL;
    if (not PyArg_ParseTuple(args,"sO:CellViewer.setLayerVisible()", &layerName, &visible)) {
      PyErr_SetString ( ConstructorError, "CellViewer.setLayerVisible(): Takes exactly two argument." );
      return NULL;
    }

    cw->setLayerVisible ( layerName, (PyObject_IsTrue(visible) != 0) );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyCellViewer_setAnonNetSelectable ( PyCellViewer* self, PyObject* args )
  {
    trace << "PyCellViewer_setAnonNetSelectable ()" << endl;

    HTRY
    METHOD_HEAD("CellViewer.setAnonNetSelectable()")

    PyObject* pyState = NULL;
    if (not PyArg_ParseTuple(args,"O:CellViewer.setAnonNetSelectable()", &pyState)) {
      PyErr_SetString ( ConstructorError, "CellViewer.setAnonNetSelectable(): Takes exactly one argument." );
      return NULL;
    }

    bool state = (PyObject_IsTrue(pyState) != 0);
    cw->setAnonNetSelectable( state );
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


  static PyObject* PyCellViewer_refresh ( PyCellViewer* self )
  {
    trace << "PyCellViewer_refresh()" << endl;

    HTRY
    METHOD_HEAD("CellViewer.refresh()")
    cw->getCellWidget()->refresh();
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyCellViewer Attribute Method table.

  PyMethodDef PyCellViewer_Methods[] =
    { { "getCell"             , (PyCFunction)PyCellViewer_getCell             , METH_NOARGS
                              , "Return the currently edited Cell." }
    , { "setCell"             , (PyCFunction)PyCellViewer_setCell             , METH_VARARGS
                              , "Load a Cell into the viewer." }
    , { "setApplicationName"  , (PyCFunction)PyCellViewer_setApplicationName  , METH_VARARGS
                              , "Sets the application (binary) name." }
    , { "setAnonNetSelectable", (PyCFunction)PyCellViewer_setAnonNetSelectable, METH_VARARGS
                              , "Allow/disallow anonymous nets to be selectables." }
    , { "setLayerVisible"     , (PyCFunction)PyCellViewer_setLayerVisible     , METH_VARARGS
                              , "Sets the visibility state of the layer <name>." }
    , { "fit"                 , (PyCFunction)PyCellViewer_fit                 , METH_NOARGS
                              , "Triggers a full redraw of the visible area." }
    , { "refresh"             , (PyCFunction)PyCellViewer_refresh             , METH_NOARGS
                              , "Fit the contents to the viewer's visible area." }
    , { "destroy"             , (PyCFunction)PyCellViewer_destroy             , METH_NOARGS
                              , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  PythonOnlyDeleteMethod(CellViewer)
  PyTypeObjectLinkPyType(CellViewer)


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyCellViewer" Shared Library Code Part              |
// +=================================================================+


  PyTypeRootObjectDefinitions(CellViewer)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Isobar namespace.
