// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./bora/PyDSlicingNode.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyDevice.h"
#include "crlcore/PyRoutingGauge.h"
#include "bora/PyDSlicingNode.h"


namespace  Bora {

  using namespace Hurricane;
  using namespace Isobar;
  using CRL::PyRoutingGauge;
  using CRL::PyTypeRoutingGauge;
  using CRL::RoutingGauge;
  using Analog::Device;


extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(DSlicingNode,node,function)


// +=================================================================+
// |              "PyDSlicingNode" Python Module Code Part           |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |             "PyDSlicingNode" Attribute Methods              |
  // +-------------------------------------------------------------+


  static PyObject* PyDSlicingNode_create ( PyObject* , PyObject* args )
  {
    PyObject*     pyInstance     = NULL;    
    PyObject*     pyCell         = NULL;
    PyObject*     pyRoutingGauge = NULL;
    double        start          = 0.0;
    double        step           = 0.0;
    double        count          = 0.0;
    DSlicingNode* node           = NULL;

    HTRY
      if (not PyArg_ParseTuple( args,"SOddd|O:DSlicingNode.create"
                              , &pyInstance
                              , &pyCell
                              , &start
                              , &step
                              , &count
                              , &pyRoutingGauge ) ) {
        PyErr_SetString ( ConstructorError, "DSlicingNode.create(): Invalid/bad number of parameters ." );
        return NULL;
      }
      if (not IsPyCell(pyCell)) {
        PyErr_SetString( ConstructorError, "DSlicingNode.create(): First argument *must* be of type Cell." );
        return NULL;
      }
      if (pyRoutingGauge and not IsPyRoutingGauge(pyRoutingGauge)) {
        PyErr_SetString( ConstructorError, "DSlicingNode.create(): Fifth argument *must* be of type RoutingGauge." );
        return NULL;
      }

      Cell*         cell     = PYCELL_O( pyCell );
      Instance*     instance = cell->getInstance( PyString_AsString(pyInstance) );
    //Device*       device   = dynamic_cast<Device*>( instance->getMasterCell() );
      RoutingGauge* rg       = (pyRoutingGauge) ? PYROUTINGGAUGE_O(pyRoutingGauge) : NULL;

      node = DSlicingNode::create( NodeSets::create( instance->getMasterCell(), start, step, count, rg )
                                 , UnknownAlignment
                                 , instance );
    HCATCH

    return PyDSlicingNode_NEW(node);
  }


  DirectGetIntAttribute(PyDSlicingNode_getNFing,getNFing,PyDSlicingNode,DSlicingNode)
  DirectSetIntAttribute(PyDSlicingNode_setNFing,setNFing,PyDSlicingNode,DSlicingNode)
  // Standart Destroy (Attribute).


  // ---------------------------------------------------------------
  // PyDSlicingNode Attribute Method table.

  PyMethodDef PyDSlicingNode_Methods[] =
    { { "create"            , (PyCFunction)PyDSlicingNode_create   , METH_VARARGS|METH_STATIC
                            , "Create a new DSlicingNode." }
    , { "getNFing"          , (PyCFunction)PyDSlicingNode_getNFing , METH_NOARGS , "To be documented." }
    , { "setNFing"          , (PyCFunction)PyDSlicingNode_setNFing , METH_VARARGS, "To be documented." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |               "PyDSlicingNode" Object Methods               |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(DSlicingNode)
  PyTypeObjectLinkPyType(DSlicingNode)


#else  // End of Python Module Code Part.

// +=================================================================+
// |             "PyDSlicingNode" Shared Library Code Part           |
// +=================================================================+


  extern PyObject* PyDSlicingNode_NEW ( DSlicingNode* node )
  {
    if (not node) Py_RETURN_NONE;

    PyDSlicingNode* pyNode = NULL;
    HTRY
      pyNode = PyObject_NEW( PyDSlicingNode, &PyTypeDSlicingNode );
      if (not pyNode) return NULL;
      pyNode->ACCESS_OBJECT = node;
    HCATCH
    return (PyObject*)pyNode;
  }
  

  PyTypeInheritedObjectDefinitions(DSlicingNode, SlicingNode)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Bora namespace.
