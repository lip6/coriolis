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
// |  C++ Header  :  "./bora/PyVSlicingNode.h"                       |
// +-----------------------------------------------------------------+


#ifndef  BORA_PY_VSLICING_NODE_H
#define  BORA_PY_VSLICING_NODE_H

#include "bora/PySlicingNode.h"
#include "bora/VSlicingNode.h"


namespace  Bora {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyVSlicingNode".

  typedef struct {
      PySlicingNode  _baseObject;
  } PyVSlicingNode;


// -------------------------------------------------------------------
// Functions & Types exported to "PyBora.cpp".

  extern PyTypeObject  PyTypeVSlicingNode;
  extern PyMethodDef   PyVSlicingNode_Methods[];

  extern PyObject* PyVSlicingNode_NEW        ( VSlicingNode* node );
  extern void      PyVSlicingNode_LinkPyType ();


#define IsPyVSlicingNode(v)    ( (v)->ob_type == &PyTypeVSlicingNode )
#define PYVSLICINGNODE(v)      ( (PyVSlicingNode*)(v) )
#define PYVSLICINGNODE_O(v)    ( dynamic_cast<VSlicingNode*>(PYVSLICINGNODE(v)->_baseObject._object) )


}  // extern "C".

}  // Bora namespace.

#endif  // BORA_PY_VSLICING_NODE_H
