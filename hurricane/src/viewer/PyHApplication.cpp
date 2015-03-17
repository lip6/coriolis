// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyHApplication.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/viewer/PyHApplication.h"
#include <cstring>
#include <QtGui>
#if (QT_VERSION >= QT_VERSION_CHECK(4,5,0)) and \
    (QT_VERSION <  QT_VERSION_CHECK(5,0,0)) and \
    not defined (__APPLE__) and not defined(__CYGWIN__)
#  include  <QGtkStyle>
#endif


namespace  Hurricane {

  using std::cerr;
  using std::endl;
  using std::hex;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(HApplication,ha,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |           "PyHApplication" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyHApplication_create ( PyObject*, PyObject* args )
  {
    static int            argc = 0;
    static char**         argv = NULL;
           vector<char*>  vargv;

    trace << "PyHApplication_create()" << endl;

    HApplication*   ha             = NULL;
    PyHApplication* pyHApplication = NULL;
    PyObject*       pyArgv         = NULL;
    
    HTRY
    if (not PyArg_ParseTuple(args,"O:HApplication.create()", &pyArgv)) {
      PyErr_SetString ( ConstructorError, "HApplication.create(): Takes exactly one argument." );
      return NULL;
    }

    if (not PyList_Check(pyArgv)) {
      PyErr_SetString(ConstructorError,"HApplication.create(): Argument is not a list.");
      return NULL;
    }

    Py_ssize_t size = PyList_Size(pyArgv);
    for( Py_ssize_t i=0 ; i<size ; ++i ) {
      PyObject* pyStr = PyObject_Str( PyList_GetItem(pyArgv,i) );
      if (pyStr == NULL) {
        PyErr_SetString(ConstructorError,"HApplication.create(): Cannot convert argument to string.");
        return NULL;
      }
      char*       strCopy = NULL;
      char*       strData = NULL;
      Py_ssize_t  strSize = 0;

      PyString_AsStringAndSize(pyStr, &strData, &strSize);
      strCopy = new char[(size_t)strSize+1];
      strncpy(strCopy, strData, (size_t)strSize+1);
      
      vargv.push_back(strCopy);
    }

    argv = new char* [vargv.size()];
    argc = (int)vargv.size();
    for( size_t i=0 ; i<vargv.size() ; ++i )
      argv[i] = vargv[i];

    ha = new HApplication(argc,argv);
#if (QT_VERSION >= QT_VERSION_CHECK(4,5,0)) and \
    (QT_VERSION <  QT_VERSION_CHECK(5,0,0)) and \
    not defined (__APPLE__) and not defined (__CYGWIN__)
    ha->setStyle ( new QGtkStyle() );
#endif

    pyHApplication = PyObject_NEW ( PyHApplication, &PyTypeHApplication );
    if ( ha == NULL ) return NULL;

    pyHApplication->_object = ha;
    HCATCH

    return (PyObject*)pyHApplication;
  }


  static PyObject* PyHApplication_qtExec ( PyHApplication* self, PyObject* args )
  {
    trace << "PyHApplication_qtExec()" << endl;

    int returnCode = 0;

    HTRY
    METHOD_HEAD("HApplication.qtExec()")
    returnCode = ha->exec();
    HCATCH

    return Py_BuildValue("i",returnCode);
  }


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyHApplication_destroy,PyHApplication)


  PyMethodDef PyHApplication_Methods[] =
    { { "create"              , (PyCFunction)PyHApplication_create          , METH_VARARGS |METH_STATIC
                              , "Create a new HApplication (must be done only once)." }
    , { "qtExec"              , (PyCFunction)PyHApplication_qtExec          , METH_VARARGS
                              , "Launch the Qt main event loop." }
  //, { "destroy"             , (PyCFunction)PyHApplication_destroy         , METH_NOARGS
  //                          , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  DirectDeleteMethod(PyHApplication_DeAlloc,PyHApplication)
  PyTypeObjectLinkPyType(HApplication)


#else  // End of Python Module Code Part.

// +=================================================================+
// |            "PyHApplication" Shared Library Code Part            |
// +=================================================================+

  // Link/Creation Method.
  PyTypeObjectDefinitions(HApplication)


#endif  // End of Shared Library Code Part.


}  // extern "C".


}  // Hurricane namespace.
 
