// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyPath.cpp"                                  |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyTransformation.h"
#include "hurricane/isobar/PyPath.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/isobar/PyInstanceCollection.h"
#include "hurricane/Cell.h"
using namespace Hurricane;

namespace Isobar {


extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Path,path,function)


// x=================================================================x
// |                "PyPath" Python Module Code Part                 |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                "PyPath" Attribute Methods                   |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyPath_isEmpty   ,isEmpty   ,PyPath,Path)


  // Standart Destroy (Attribute).
  DirectDestroyAttribute(PyPath_destroy, PyPath)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_NEW ()"

  static PyObject* PyPath_NEW ( PyObject *module, PyObject *args ) {
    trace << "PyPath_NEW()" << endl;

    Path*     path   = NULL;
    PyObject* arg0   = NULL;
    PyObject* arg1   = NULL;
    PyPath*   pyPath = NULL;

    __cs.init ("Path.Path");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Path.Path"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Path constructor." );
        return NULL;
    }

    HTRY

    if      ( __cs.getObjectIds() == NO_ARG          ) { path = new Path (); }
    else if ( __cs.getObjectIds() == INST_ARG        ) { path = new Path (  PYINSTANCE_O(arg0) ); }
    else if ( __cs.getObjectIds() == INST_PATH_ARG   ) { path = new Path (  PYINSTANCE_O(arg0)
                                                                         , *PYPATH_O(arg1) ); }
    else if ( __cs.getObjectIds() == PATH_INST_ARG   ) { path = new Path ( *PYPATH_O(arg0)
                                                                         ,  PYINSTANCE_O(arg1) ); }
    else if ( __cs.getObjectIds() == CELL_STRING_ARG ) { path = new Path (  PYCELL_O(arg0) 
                                                                         ,  PyString_AsString(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Path constructor." );
      return ( NULL );
    }
    
    pyPath = PyObject_NEW(PyPath, &PyTypePath);
    if (pyPath == NULL) return NULL;
    
    pyPath->_object = path;

    HCATCH

    return ( (PyObject*)pyPath );
  }


  static int  PyPath_Init ( PyPath* self, PyObject* args, PyObject* kwargs )
  {
    trace << "PyPath_Init(): " << (void*)self << endl;
    return 0;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getHeadInstance ()"

  static PyObject* PyPath_getHeadInstance ( PyPath *self ) {
    trace << "PyPath_getHeadInstance()" << endl;

    METHOD_HEAD ( "Path.getHeadInstance()" )

    Instance* instance = NULL;

    HTRY
    instance = path->getHeadInstance();
    HCATCH    

    return PyInstance_Link ( instance );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getTailInstance ()"

  static PyObject* PyPath_getTailInstance ( PyPath *self ) {
    trace << "PyPath_getTailInstance()" << endl;

    METHOD_HEAD ( "Path.getTailInstance()" )

    Instance* instance = NULL;

    HTRY
    instance = path->getTailInstance();
    HCATCH    

    return PyInstance_Link ( instance );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getOwnerCell ()"

  static PyObject* PyPath_getOwnerCell ( PyPath *self ) {
    trace << "PyPath_getOwnerCell()" << endl;

    METHOD_HEAD ( "Path.getOwnerCell()" )

    Cell* cell = NULL;

    HTRY
    cell = path->getOwnerCell();
    HCATCH    

    return PyCell_Link ( cell );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getMasterCell ()"

  static PyObject* PyPath_getMasterCell ( PyPath *self ) {
    trace << "PyPath_getMasterCell()" << endl;

    METHOD_HEAD ( "Path.getMasterCell()" )

    Cell* cell = NULL;

    HTRY
    cell = path->getMasterCell();
    HCATCH    

    return PyCell_Link ( cell );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getName ()"

  GetNameMethod(Path, path)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getHeadPath ()"

  static PyObject* PyPath_getHeadPath ( PyPath *self ) {

    trace << "PyPath_getHeadPath ()" << endl;
    
    METHOD_HEAD ( "Path.getHeadPath()" )

    PyPath* pyPath = PyObject_NEW ( PyPath, &PyTypePath );
    if ( pyPath == NULL ) { return NULL; }

    HTRY
    pyPath->_object = new Path ( path->getHeadPath() );
    HCATCH
    
    return ( (PyObject*)pyPath );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getTailPath ()"

  static PyObject* PyPath_getTailPath ( PyPath *self )
  {

    trace << "PyPath_getTailPath ()" << endl;
    
    METHOD_HEAD ( "Path.getTailPath()" )

    PyPath* pyPath = PyObject_NEW ( PyPath, &PyTypePath );
    if ( pyPath == NULL ) { return NULL; }

    HTRY
    pyPath->_object = new Path ( path->getTailPath() );
    HCATCH
    
    return ( (PyObject*)pyPath );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getTransformation ()"

  static PyObject* PyPath_getTransformation ( PyPath *self, PyObject* args )
  {
    trace << "PyPath_getTransformation ()" << endl;
    
    METHOD_HEAD ( "Instance.getTransformation()" );

    PyObject*         arg0;
    PyTransformation* pyTransf = PyObject_NEW ( PyTransformation, &PyTypeTransformation );
    if ( pyTransf == NULL ) { return NULL; }

    __cs.init ("Path.getTransformation");
    if ( ! PyArg_ParseTuple(args,"|O&:Path.getTransformation",Converter,&arg0) ) return ( NULL );

    HTRY
    if      ( __cs.getObjectIds() == NO_ARG )
      { pyTransf->_object = new Transformation ( path->getTransformation () ); }
    else if ( __cs.getObjectIds() == TRANS_ARG )
      { pyTransf->_object = new Transformation ( path->getTransformation (*PYTRANSFORMATION_O(arg0)) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Path constructor." );
      return ( NULL );
    }
    
    HCATCH

    return ( (PyObject*)pyTransf );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getInstances()"

  static PyObject* PyPath_getInstances(PyPath *self) {

    trace << "PyPath_getInstances()" << endl;

    METHOD_HEAD ( "Path.getInstances()" )

    PyInstanceCollection* pyInstanceCollection = NULL;

    HTRY
    Instances* instances = new Instances(path->getInstances());

    pyInstanceCollection = PyObject_NEW(PyInstanceCollection, &PyTypeInstanceCollection);
    if (pyInstanceCollection == NULL) { 
        return NULL;
    }

    pyInstanceCollection->_object = instances;
    HCATCH
    
    return (PyObject*)pyInstanceCollection;
  }




  // ---------------------------------------------------------------
  // PyPath Attribute Method table.

  PyMethodDef PyPath_Methods[] =
    { { "getHeadInstance"    , (PyCFunction)PyPath_getHeadInstance    , METH_NOARGS , "Return the head instance." }
    , { "getTailInstance"    , (PyCFunction)PyPath_getTailInstance    , METH_NOARGS , "Return the tail instance." }
    , { "getHeadPath"        , (PyCFunction)PyPath_getHeadPath        , METH_NOARGS , "Return the head path." }
    , { "getTailPath"        , (PyCFunction)PyPath_getTailPath        , METH_NOARGS , "Return the tail path." }
    , { "getOwnerCell"       , (PyCFunction)PyPath_getOwnerCell       , METH_NOARGS , "Return the cell owning the head instance." }
    , { "getMasterCell"      , (PyCFunction)PyPath_getMasterCell      , METH_NOARGS , "Returns the master cell referenced by the last instance of the path." }
    , { "getName"            , (PyCFunction)PyPath_getName            , METH_NOARGS , "Returns the concatenation of instances names." }
    , { "getTransformation"  , (PyCFunction)PyPath_getTransformation  , METH_VARARGS, "Return the resulting transformation." }
    , { "getInstances", (PyCFunction)PyPath_getInstances, METH_NOARGS , "Returns the collection of instances defining the path." }
    , { "isEmpty"            , (PyCFunction)PyPath_isEmpty            , METH_NOARGS , "Return true if the path is empty." }
    , { "destroy"            , (PyCFunction)PyPath_destroy            , METH_NOARGS
                             , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                  "PyPath" Object Methods                    |
  // x-------------------------------------------------------------x
  
  DirectDeleteMethod(PyPath_DeAlloc,PyPath)
  PyTypeObjectLinkPyTypeNewInit(Path)
//PyTypeObjectLinkPyType(Path)


# else  // End of Python Module Code Part.


// x=================================================================x
// |               "PyPath" Shared Library Code Part                 |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyPath Object Definitions.
  
  PyTypeObjectDefinitions(Path)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
