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
// |  C++ Header  :  "./bora/PyRHSlicingNode.h"                      |
// +-----------------------------------------------------------------+


#ifndef  BORA_PY_RHSLICING_NODE_H
#define  BORA_PY_RHSLICING_NODE_H

#include "bora/PySlicingNode.h"
#include "bora/RHSlicingNode.h"


namespace  Bora {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyRHSlicingNode".

  typedef struct {
      PySlicingNode  _baseObject;
  } PyRHSlicingNode;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricaneAMS.cpp".

  extern PyTypeObject  PyTypeRHSlicingNode;
  extern PyMethodDef   PyRHSlicingNode_Methods[];

  extern PyObject* PyRHSlicingNode_NEW        ( RHSlicingNode* node );
  extern void      PyRHSlicingNode_LinkPyType ();


#define IsPyRHSlicingNode(v)    ( (v)->ob_type == &PyTypeRHSlicingNode )
#define PYRHSLICINGNODE(v)      ( (PyRHSlicingNode*)(v) )
#define PYRHSLICINGNODE_O(v)    ( dynamic_cast<RHSlicingNode*>(PYRHSLICINGNODE(v)->_baseObject._object) )


}  // extern "C".

}  // Bora namespace.

#endif  // BORA_PY_RHSLICING_NODE_H
