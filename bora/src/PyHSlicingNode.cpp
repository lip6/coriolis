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
// |  C++ Module  :  "./bora/PyHSlicingNode.cpp"                     |
// +-----------------------------------------------------------------+


#include "bora/PyHSlicingNode.h"


namespace  Bora {

  using namespace Hurricane;
  using namespace Isobar;


extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(HSlicingNode,node,function)


// +=================================================================+
// |              "PyHSlicingNode" Python Module Code Part           |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |             "PyHSlicingNode" Attribute Methods              |
  // +-------------------------------------------------------------+


  static PyObject* PyHSlicingNode_create ( PyObject* , PyObject* args )
  {
    unsigned int  alignment = AlignLeft;
    HSlicingNode* node      = NULL;

    HTRY
      if (not PyArg_ParseTuple(args,"I:HSlicingNode.create", &alignment) ) {
        PyErr_SetString ( ConstructorError, "HSlicingNode.create(): Invalid/bad type parameters ." );
        return NULL;
      }
      node = HSlicingNode::create( alignment );
    HCATCH

    return PyHSlicingNode_NEW(node);
  }


  // Standart Destroy (Attribute).


  // ---------------------------------------------------------------
  // PyHSlicingNode Attribute Method table.

  PyMethodDef PyHSlicingNode_Methods[] =
    { { "create"            , (PyCFunction)PyHSlicingNode_create   , METH_VARARGS|METH_STATIC
                            , "Create a new HSlicingNode." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |               "PyHSlicingNode" Object Methods               |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(HSlicingNode)
  PyTypeObjectLinkPyType(HSlicingNode)


#else  // End of Python Module Code Part.

// +=================================================================+
// |             "PyHSlicingNode" Shared Library Code Part           |
// +=================================================================+


  extern PyObject* PyHSlicingNode_NEW ( HSlicingNode* node )
  {
    if (not node) Py_RETURN_NONE;

    PyHSlicingNode* pyNode = NULL;
    HTRY
      pyNode = PyObject_NEW( PyHSlicingNode, &PyTypeHSlicingNode );
      if (not pyNode) return NULL;
      pyNode->ACCESS_OBJECT = node;
    HCATCH
    return (PyObject*)pyNode;
  }
  

  PyTypeInheritedObjectDefinitions(HSlicingNode, SlicingNode)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Bora namespace.
