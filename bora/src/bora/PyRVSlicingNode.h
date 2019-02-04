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
// |  C++ Header  :  "./bora/PyRVSlicingNode.h"                      |
// +-----------------------------------------------------------------+


#ifndef  BORA_PY_RVSLICING_NODE_H
#define  BORA_PY_RVSLICING_NODE_H

#include "bora/PySlicingNode.h"
#include "bora/RVSlicingNode.h"


namespace  Bora {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyRVSlicingNode".

  typedef struct {
      PySlicingNode  _baseObject;
  } PyRVSlicingNode;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricaneAMS.cpp".

  extern PyTypeObject  PyTypeRVSlicingNode;
  extern PyMethodDef   PyRVSlicingNode_Methods[];

  extern PyObject* PyRVSlicingNode_NEW        ( RVSlicingNode* node );
  extern void      PyRVSlicingNode_LinkPyType ();


#define IsPyRVSlicingNode(v)    ( (v)->ob_type == &PyTypeRVSlicingNode )
#define PYRVSLICINGNODE(v)      ( (PyRVSlicingNode*)(v) )
#define PYRVSLICINGNODE_O(v)    ( dynamic_cast<RVSlicingNode*>(PYRVSLICINGNODE(v)->_baseObject._object) )


}  // extern "C".

}  // Bora namespace.

#endif  // BORA_PY_RVSLICING_NODE_H
