// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyViewer.cpp"                                |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/PyHApplication.h"
#include "hurricane/viewer/PyGraphics.h"
#include "hurricane/viewer/PyDrawingStyle.h"
#include "hurricane/viewer/PyDrawingGroup.h"
#include "hurricane/viewer/PyDisplayStyle.h"
#include "hurricane/viewer/PyHSVr.h"
#include "hurricane/isobar/PyErrorWidget.h"
#include "hurricane/isobar/PyCellViewer.h"
#include "hurricane/viewer/PyAboutWindow.h"


namespace Hurricane {

  using std::cerr;
  using std::endl;
  using Isobar::__cs;
  using Isobar::getPyHash;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |              "PyViewer" Shared Library Code Part                |
// +=================================================================+


# else // End of PyHurricane Shared Library Code Part.

// +=================================================================+
// |               "PyViewer" Python Module Code Part                |
// +=================================================================+


extern "C" {


  // +-------------------------------------------------------------+
  // |                "PyViewer" Module Methods                    |
  // +-------------------------------------------------------------+


  static PyMethodDef PyViewer_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };


  static PyModuleDef  PyViewer_ModuleDef =
    { PyModuleDef_HEAD_INIT
    , "Viewer"            /* m_name     */
    , "Coriolis Graphical Interface."
                          /* m_doc      */
    , -1                  /* m_size     */
    , PyViewer_Methods    /* m_methods  */
    , NULL                /* m_reload   */
    , NULL                /* m_traverse */
    , NULL                /* m_clear    */
    , NULL                /* m_free     */
    };


  // ---------------------------------------------------------------
  // Module Initialization  :  "initViewer ()"

  PyMODINIT_FUNC PyInit_Viewer ( void )
  {
    cdebug_log(20,0) << "initViewer()" << endl;

    PyHSVr_LinkPyType ();
    PyRawDrawingStyle_LinkPyType ();
    PyDrawingStyleVector_LinkPyType ();
    PyDrawingGroup_LinkPyType ();
    PyDrawingGroupVector_LinkPyType ();
    PyDisplayStyle_LinkPyType ();
    PyDisplayStyleVector_LinkPyType ();
    PyHApplication_LinkPyType ();
    PyGraphics_LinkPyType ();
    PyErrorWidget_LinkPyType ();
    PyCellViewer_LinkPyType ();
    PyAboutWindow_LinkPyType ();
    
    PYTYPE_READY ( HSVr );
    PYTYPE_READY ( RawDrawingStyle );
    PYTYPE_READY ( DrawingStyleVector );
    PYTYPE_READY ( DrawingStyleVectorIterator );
    PYTYPE_READY ( DrawingGroup );
    PYTYPE_READY ( DrawingGroupVector );
    PYTYPE_READY ( DrawingGroupVectorIterator );
    PYTYPE_READY ( DisplayStyle );
    PYTYPE_READY ( DisplayStyleVector );
    PYTYPE_READY ( DisplayStyleVectorIterator );
    PYTYPE_READY ( HApplication );
    PYTYPE_READY ( Graphics );
    PYTYPE_READY ( ErrorWidget );
    PYTYPE_READY ( CellViewer );
    PYTYPE_READY ( AboutWindow );
    
    // Identifier string can take up to 10 characters.
    __cs.addType ( "hsvr"      , &PyTypeHSVr        , "<HSVr>"        , false );
    __cs.addType ( "displaySty", &PyTypeDisplayStyle, "<DisplayStyle>", false );
    __cs.addType ( "graphics"  , &PyTypeGraphics    , "<Graphics>"    , false );
    __cs.addType ( "cellView"  , &PyTypeCellViewer  , "<CellViewer>"  , false );
    
    PyObject* module = PyModule_Create( &PyViewer_ModuleDef );
    if ( module == NULL ) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Viewer module." << endl;
      return NULL;
    }
    
    Py_INCREF ( &PyTypeDisplayStyle );
    PyModule_AddObject ( module, "DisplayStyle", (PyObject*)&PyTypeDisplayStyle );
    Py_INCREF ( &PyTypeHApplication );
    PyModule_AddObject ( module, "HApplication", (PyObject*)&PyTypeHApplication );
    Py_INCREF ( &PyTypeGraphics );
    PyModule_AddObject ( module, "Graphics"    , (PyObject*)&PyTypeGraphics );
    Py_INCREF ( &PyTypeCellViewer );
    PyModule_AddObject ( module, "ErrorWidget" , (PyObject*)&PyTypeErrorWidget );
    Py_INCREF ( &PyTypeErrorWidget );
    PyModule_AddObject ( module, "CellViewer"  , (PyObject*)&PyTypeCellViewer );
    Py_INCREF ( &PyTypeAboutWindow );
    PyModule_AddObject ( module, "AboutWindow" , (PyObject*)&PyTypeAboutWindow );
    
    PyDisplayStyle_postModuleInit();
    PyCellViewer_postModuleInit();

    cdebug_log(20,0) << "Viewer.so loaded " << (void*)&typeid(string) << endl;

    return module;
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of Hurricane namespace.
