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
// |  C++ Module  :  "./bora/PyRHSlicingNode.cpp"                    |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "bora/PyRHSlicingNode.h"


namespace  Bora {

  using namespace Hurricane;
  using namespace Isobar;


extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(RHSlicingNode,node,function)


// +=================================================================+
// |              "PyRHSlicingNode" Python Module Code Part           |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |             "PyRHSlicingNode" Attribute Methods              |
  // +-------------------------------------------------------------+


  static PyObject* PyRHSlicingNode_create ( PyObject* , PyObject* args )
  {
   
    PyObject*      pyNet   = NULL;
    PyObject*      pyLayer = NULL;
    int            npitch  = 0;
    const char*    cname;    
    const char*    iname;    
    RHSlicingNode* node    = NULL;

    HTRY
      if (not PyArg_ParseTuple( args,"OOiss|O:RHSlicingNode.create"
                              , &pyNet
                              , &pyLayer
                              , &npitch
                              , &cname
                              , &iname) ) {
        PyErr_SetString ( ConstructorError, "RHSlicingNode.create(): Invalid/bad number of parameters ." );
        return NULL;
      }
      if (not IsPyNet(pyNet)) {
        PyErr_SetString( ConstructorError, "RHSlicingNode.create(): First argument *must* be of type Net." );
        return NULL;
      }
      Layer* layer = PYDERIVEDLAYER_O( pyLayer );
      if (not layer) {
        PyErr_SetString( ConstructorError, "RHSlicingNode.create(): Fifth argument *must* be of type Layer." );
        return NULL;
      }

      Net*   net   = PYNET_O( pyNet );

      node = RHSlicingNode::create( net, layer, npitch, cname, iname );
    HCATCH
    return PyRHSlicingNode_NEW(node);
  }

  // Standart Destroy (Attribute).


  // ---------------------------------------------------------------
  // PyRHSlicingNode Attribute Method table.

  PyMethodDef PyRHSlicingNode_Methods[] =
    { { "create"            , (PyCFunction)PyRHSlicingNode_create   , METH_VARARGS|METH_STATIC
                            , "Create a new RHSlicingNode." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |               "PyRHSlicingNode" Object Methods               |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(RHSlicingNode)
  PyTypeObjectLinkPyType(RHSlicingNode)


#else  // End of Python Module Code Part.

// +=================================================================+
// |             "PyRHSlicingNode" Shared Library Code Part           |
// +=================================================================+


  extern PyObject* PyRHSlicingNode_NEW ( RHSlicingNode* node )
  {
    if (not node) Py_RETURN_NONE;

    PyRHSlicingNode* pyNode = NULL;
    HTRY
      pyNode = PyObject_NEW( PyRHSlicingNode, &PyTypeRHSlicingNode );
      if (not pyNode) return NULL;
      pyNode->ACCESS_OBJECT = node;
    HCATCH
    return (PyObject*)pyNode;
  }
  

  PyTypeInheritedObjectDefinitions(RHSlicingNode, SlicingNode)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Bora namespace.
