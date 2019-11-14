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
// |  C++ Module  :  "./bora/PyBora.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "bora/PyParameterRange.h"
#include "bora/PyStepParameterRange.h"
#include "bora/PyMatrixParameterRange.h"
#include "bora/PySlicingNode.h"
#include "bora/PyHSlicingNode.h"
#include "bora/PyVSlicingNode.h"
#include "bora/PyDSlicingNode.h"
#include "bora/PyRHSlicingNode.h"
#include "bora/PyRVSlicingNode.h"
#include "bora/PyBoraEngine.h"
#include "bora/PyGraphicBoraEngine.h"


namespace Bora {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Isobar::getPyHash;
  using Isobar::__cs;
  using CRL::PyTypeToolEngine;
  using CRL::PyTypeGraphicTool;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |                "PyBora" Shared Library Code Part                |
// +=================================================================+


#else // PyBora Shared Library Code Part.


// +=================================================================+
// |                 "PyBora" Python Module Code Part                |
// +=================================================================+


extern "C" {


  static PyMethodDef PyBora_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };


  // ---------------------------------------------------------------
  // Module Initialization  :  "initBora ()"

  DL_EXPORT(void) initBora ()
  {
    cdebug.log(61) << "initBora()" << endl;

    PyParameterRange_LinkPyType();
    PyStepParameterRange_LinkPyType();
    PyMatrixParameterRange_LinkPyType();
    PySlicingNode_LinkPyType();
    PyHSlicingNode_LinkPyType();
    PyVSlicingNode_LinkPyType();
    PyDSlicingNode_LinkPyType();
    PyRHSlicingNode_LinkPyType();
    PyRVSlicingNode_LinkPyType();
    PyBoraEngine_LinkPyType();
    PyGraphicBoraEngine_LinkPyType();

    PYTYPE_READY( ParameterRange )
    PYTYPE_READY( SlicingNode )

    PYTYPE_READY_SUB( StepParameterRange  , ParameterRange );
    PYTYPE_READY_SUB( MatrixParameterRange, ParameterRange );
    PYTYPE_READY_SUB( HSlicingNode        , SlicingNode    );
    PYTYPE_READY_SUB( VSlicingNode        , SlicingNode    );
    PYTYPE_READY_SUB( DSlicingNode        , SlicingNode    );
    PYTYPE_READY_SUB( RHSlicingNode       , SlicingNode    );
    PYTYPE_READY_SUB( RVSlicingNode       , SlicingNode    );
    PYTYPE_READY_SUB( BoraEngine          , ToolEngine     );
    PYTYPE_READY_SUB( GraphicBoraEngine   , GraphicTool    );


    PyObject* module = Py_InitModule( "Bora", PyBora_Methods );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Bora module." << endl;
      return;
    }

    Py_INCREF( &PyTypeParameterRange );
    PyModule_AddObject( module, "ParameterRange"      , (PyObject*)&PyTypeParameterRange );
    Py_INCREF( &PyTypeStepParameterRange );
    PyModule_AddObject( module, "StepParameterRange"  , (PyObject*)&PyTypeStepParameterRange );
    Py_INCREF( &PyTypeMatrixParameterRange );
    PyModule_AddObject( module, "MatrixParameterRange", (PyObject*)&PyTypeMatrixParameterRange );
    Py_INCREF( &PyTypeSlicingNode );
    PyModule_AddObject( module, "SlicingNode"         , (PyObject*)&PyTypeSlicingNode );
    Py_INCREF( &PyTypeHSlicingNode );                 
    PyModule_AddObject( module, "HSlicingNode"        , (PyObject*)&PyTypeHSlicingNode );
    Py_INCREF( &PyTypeVSlicingNode );                 
    PyModule_AddObject( module, "VSlicingNode"        , (PyObject*)&PyTypeVSlicingNode );
    Py_INCREF( &PyTypeDSlicingNode );                 
    PyModule_AddObject( module, "DSlicingNode"        , (PyObject*)&PyTypeDSlicingNode );
    Py_INCREF( &PyTypeRHSlicingNode );                
    PyModule_AddObject( module, "RHSlicingNode"       , (PyObject*)&PyTypeRHSlicingNode );
    Py_INCREF( &PyTypeRVSlicingNode );                
    PyModule_AddObject( module, "RVSlicingNode"       , (PyObject*)&PyTypeRVSlicingNode );
    Py_INCREF( &PyTypeBoraEngine );                   
    PyModule_AddObject( module, "BoraEngine"          , (PyObject*)&PyTypeBoraEngine );
    Py_INCREF( &PyTypeGraphicBoraEngine );            
    PyModule_AddObject( module, "GraphicBoraEngine"   , (PyObject*)&PyTypeGraphicBoraEngine );

    PySlicingNode_postModuleInit();
    PyBoraEngine_postModuleInit();
  }

  
} // extern "C".


#endif // Python Module Code Part.

}  // Bora namespace.
