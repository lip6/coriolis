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
// |  C++ Header  :  "./bora/PyHSlicingNode.h"                       |
// +-----------------------------------------------------------------+


#ifndef  BORA_PY_HSLICING_NODE_H
#define  BORA_PY_HSLICING_NODE_H

#include "bora/PySlicingNode.h"
#include "bora/HSlicingNode.h"


namespace  Bora {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyHSlicingNode".

  typedef struct {
      PySlicingNode  _baseObject;
  } PyHSlicingNode;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricaneAMS.cpp".

  extern PyTypeObject  PyTypeHSlicingNode;
  extern PyMethodDef   PyHSlicingNode_Methods[];

  extern PyObject* PyHSlicingNode_NEW        ( HSlicingNode* node );
  extern void      PyHSlicingNode_LinkPyType ();


#define IsPyHSlicingNode(v)    ( (v)->ob_type == &PyTypeHSlicingNode )
#define PYHSLICINGNODE(v)      ( (PyHSlicingNode*)(v) )
#define PYHSLICINGNODE_O(v)    ( dynamic_cast<HSlicingNode*>(PYHSLICINGNODE(v)->_baseObject._object) )


}  // extern "C".

}  // Bora namespace.

#endif  // BORA_PY_HSLICING_NODE_H
