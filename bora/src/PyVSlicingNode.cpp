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
// |  C++ Module  :  "./bora/PyVSlicingNode.cpp"                     |
// +-----------------------------------------------------------------+


#include "bora/PyVSlicingNode.h"


namespace  Isobar {

  using namespace Hurricane;
  using namespace Bora;


extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(VSlicingNode,node,function)


// +=================================================================+
// |              "PyVSlicingNode" Python Module Code Part           |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |             "PyVSlicingNode" Attribute Methods              |
  // +-------------------------------------------------------------+


  static PyObject* PyVSlicingNode_create ( PyObject* , PyObject* args )
  {
    unsigned int  alignment = AlignBottom;
    VSlicingNode* node      = NULL;

    HTRY
      if (not PyArg_ParseTuple(args,"I:VSlicingNode.create", &alignment) ) {
        PyErr_SetString ( ConstructorError, "VSlicingNode.create(): Invalid/bad type parameters ." );
        return NULL;
      }
      node = VSlicingNode::create( alignment );
    HCATCH

    return PyVSlicingNode_NEW(node);
  }


  // Standart Destroy (Attribute).


  // ---------------------------------------------------------------
  // PyVSlicingNode Attribute Method table.

  PyMethodDef PyVSlicingNode_Methods[] =
    { { "create"            , (PyCFunction)PyVSlicingNode_create   , METH_VARARGS|METH_STATIC
                            , "Create a new VSlicingNode." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |               "PyVSlicingNode" Object Methods               |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(VSlicingNode)
  PyTypeObjectLinkPyType(VSlicingNode)


#else  // End of Python Module Code Part.

// +=================================================================+
// |             "PyVSlicingNode" Shared Library Code Part           |
// +=================================================================+


  extern PyObject* PyVSlicingNode_NEW ( VSlicingNode* node )
  {
    if (not node) Py_RETURN_NONE;

    PyVSlicingNode* pyNode = NULL;
    HTRY
      pyNode = PyObject_NEW( PyVSlicingNode, &PyTypeVSlicingNode );
      if (not pyNode) return NULL;
      pyNode->ACCESS_OBJECT = node;
    HCATCH
    return (PyObject*)pyNode;
  }
  

  PyTypeInheritedObjectDefinitions(VSlicingNode, SlicingNode)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
