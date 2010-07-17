
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyViewer.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/PyCellViewer.h"


namespace Hurricane {

  using std::cerr;
  using std::endl;
  using Isobar::__cs;


#if !defined(__PYTHON_MODULE__)

// x=================================================================x
// |              "PyViewer" Shared Library Code Part                |
// x=================================================================x


# else // End of PyHurricane Shared Library Code Part.


// x=================================================================x
// |               "PyViewer" Python Module Code Part                |
// x=================================================================x


extern "C" {


  // x-------------------------------------------------------------x
  // |                "PyViewer" Module Methods                    |
  // x-------------------------------------------------------------x


  static PyMethodDef PyViewer_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };




  // ---------------------------------------------------------------
  // Module Initialization  :  "initViewer ()"

  DL_EXPORT(void) initViewer () {
    trace << "initViewer()" << endl;

    PyCellViewer_LinkPyType ();

    PYTYPE_READY ( CellViewer );
   
    // Identifier string can take up to 10 characters.
    __cs.addType ( "cellView", &PyTypeCellViewer, "<CellViewer>", false, "view" );

    PyObject* module = Py_InitModule ( "Viewer", PyViewer_Methods );
    if ( module == NULL ) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Viewer module." << endl;
      return;
    }

    Py_INCREF ( &PyTypeCellViewer );
    PyModule_AddObject ( module, "CellViewer", (PyObject*)&PyTypeCellViewer );

    PyObject* dictionnary = PyModule_GetDict ( module );

  //DbULoadConstants ( dictionnary );

    trace << "Viewer.so loaded " << (void*)&typeid(string) << endl;
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of Hurricane namespace.
