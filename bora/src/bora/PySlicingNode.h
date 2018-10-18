// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/PySlicingNode.h"                        |
// +-----------------------------------------------------------------+


#ifndef BORA_PY_SLICING_NODE_H
#define BORA_PY_SLICING_NODE_H

#include "hurricane/isobar/PyHurricane.h"
#include "bora/SlicingNode.h"


namespace  Bora {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PySlicingNode".

    typedef struct {
        PyObject_HEAD
        SlicingNode* _object;
    } PySlicingNode;


// -------------------------------------------------------------------
// Functions & Types exported to "PyBora.ccp".

    extern  PyObject* PySlicingNode_NEW            ( SlicingNode* node );
    extern  void      PySlicingNode_LinkPyType     ();
    extern  void      PySlicingNode_postModuleInit ();

    extern  PyTypeObject  PyTypeSlicingNode;
    extern  PyMethodDef   PySlicingNode_Methods[];


#define IsPySlicingNode(v)   ( (v)->ob_type == &PyTypeSlicingNode )
#define PYSLICINGNODE(v)     ( (PySlicingNode*)(v) )
#define PYSLICINGNODE_O(v)   ( PYSLICINGNODE(v)->_object )

  // Vector of DrawingGroups.
    declareVectorObject(SlicingNode);


  }  // extern "C".


  SlicingNode*  SlicingNodeCast ( PyObject* derivedObject );


}  // Bora namespace.

#endif  // BORA_PY_SLICING_NODE_H
