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
// |  C++ Header  :  "./bora/PyDSlicingNode.h"                       |
// +-----------------------------------------------------------------+


#ifndef  BORA_PY_DSLICING_NODE_H
#define  BORA_PY_DSLICING_NODE_H

#include "bora/PySlicingNode.h"
#include "bora/DSlicingNode.h"


namespace  Bora {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyDSlicingNode".

  typedef struct {
      PySlicingNode  _baseObject;
  } PyDSlicingNode;


// -------------------------------------------------------------------
// Functions & Types exported to "PyBora.cpp".

  extern PyTypeObject  PyTypeDSlicingNode;
  extern PyMethodDef   PyDSlicingNode_Methods[];

  extern PyObject* PyDSlicingNode_NEW        ( DSlicingNode* node );
  extern void      PyDSlicingNode_LinkPyType ();


#define IsPyDSlicingNode(v)    ( (v)->ob_type == &PyTypeDSlicingNode )
#define PYDSLICINGNODE(v)      ( (PyDSlicingNode*)(v) )
#define PYDSLICINGNODE_O(v)    ( dynamic_cast<DSlicingNode*>(PYDSLICINGNODE(v)->_baseObject._object) )


}  // extern "C".

}  // Bora namespace.

#endif  // BORA_PY_DSLICING_NODE_H
