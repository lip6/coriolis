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
// |  C++ Module  :  "./bora/PyRVSlicingNode.cpp"                    |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "bora/PyRVSlicingNode.h"


namespace  Bora {

  using namespace Hurricane;
  using namespace Isobar;


extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(RVSlicingNode,node,function)


// +=================================================================+
// |             "PyRVSlicingNode" Python Module Code Part           |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |            "PyRVSlicingNode" Attribute Methods              |
  // +-------------------------------------------------------------+


  static PyObject* PyRVSlicingNode_create ( PyObject* , PyObject* args )
  {
   
    PyObject*      pyNet   = NULL;
    PyObject*      pyLayer = NULL;
    int            npitch  = 0;
    const char*    cname   = "";    
    const char*    iname   = "";    
    RVSlicingNode* node    = NULL;

    HTRY
      if (not PyArg_ParseTuple( args,"OOiss|O:RVSlicingNode.create"
                              , &pyNet
                              , &pyLayer
                              , &npitch
                              , &cname
                              , &iname) ) {
        PyErr_SetString ( ConstructorError, "RVSlicingNode.create(): Invalid/bad number of parameters ." );
        return NULL;
      }
      if (not IsPyNet(pyNet)) {
        PyErr_SetString( ConstructorError, "RVSlicingNode.create(): First argument *must* be of type Net." );
        return NULL;
      }
      Layer* layer = PYDERIVEDLAYER_O( pyLayer );
      if (not layer) {
        PyErr_SetString( ConstructorError, "RVSlicingNode.create(): Fifth argument *must* be of type Layer." );
        return NULL;
      }

      Net*   net   = PYNET_O( pyNet );

      node = RVSlicingNode::create( net, layer, npitch, cname, iname );
    HCATCH

    return PyRVSlicingNode_NEW(node);
  }

  // Standart Destroy (Attribute).


  // ---------------------------------------------------------------
  // PyRVSlicingNode Attribute Method table.

  PyMethodDef PyRVSlicingNode_Methods[] =
    { { "create"            , (PyCFunction)PyRVSlicingNode_create   , METH_VARARGS|METH_STATIC
                            , "Create a new RVSlicingNode." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |              "PyRVSlicingNode" Object Methods               |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(RVSlicingNode)
  PyTypeObjectLinkPyType(RVSlicingNode)


#else  // End of Python Module Code Part.

// +=================================================================+
// |            "PyRVSlicingNode" Shared Library Code Part           |
// +=================================================================+


  extern PyObject* PyRVSlicingNode_NEW ( RVSlicingNode* node )
  {
    if (not node) Py_RETURN_NONE;

    PyRVSlicingNode* pyNode = NULL;
    HTRY
      pyNode = PyObject_NEW( PyRVSlicingNode, &PyTypeRVSlicingNode );
      if (not pyNode) return NULL;
      pyNode->ACCESS_OBJECT = node;
    HCATCH
    return (PyObject*)pyNode;
  }
  

  PyTypeInheritedObjectDefinitions(RVSlicingNode, SlicingNode)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Bora namespace.
