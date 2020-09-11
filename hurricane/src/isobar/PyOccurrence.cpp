// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2013-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyOccurrence.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyTransformation.h"
#include "hurricane/isobar/PyPath.h"
#include "hurricane/isobar/PyOccurrence.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PySegment.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Occurrence,occurrence,function)


// x=================================================================x
// |             "PyOccurrence" Python Module Code Part              |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |              "PyOccurrence" Attribute Methods               |
  // x-------------------------------------------------------------x



  // Standart Accessors (Attributes).


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute  (PyOccurrence_isValid         ,isValid         ,PyOccurrence,Occurrence)
  DirectGetBoolAttribute  (PyOccurrence_hasProperty     ,hasProperty     ,PyOccurrence,Occurrence)
  DirectGetStringAttribute(PyOccurrence_getCompactString,getCompactString,PyOccurrence,Occurrence)


  // Standart destroy (Attribute).
  DirectDestroyAttribute(PyOccurrence_destroy, PyOccurrence)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_NEW ()"

  PyObject* PyOccurrence_NEW ( PyObject *module, PyObject *args )
  {
    cdebug_log(20,0) << "PyOccurrence_NEW()" << endl;

    Occurrence* occurrence;
    PyObject*   arg0;
    PyObject*   arg1;

    __cs.init( "Occurrence.Occurrence" );
    if (not PyArg_ParseTuple( args,"|O&O&:Occurrence.Occurrence"
                            , Converter, &arg0
                            , Converter, &arg1
                           )) {
        PyErr_SetString( ConstructorError, "More than two parameters given to Occurrence constructor." );
        return NULL;
    }

    PyOccurrence* pyOccurrence = NULL;
    HTRY
      if (  (__cs.getObjectIds() == ENT_ARG      )
         or (__cs.getObjectIds() == COMP_ARG     ) ) { occurrence = new Occurrence (  EntityCast(arg0) ); }
      else if (__cs.getObjectIds() == NO_ARG       ) { occurrence = new Occurrence (); }
      else if (__cs.getObjectIds() == COMP_PATH_ARG) { occurrence = new Occurrence (  EntityCast(arg0)
                                                                                   , *PYPATH_O(arg1) ); }
      else if (__cs.getObjectIds() == ENT_PATH_ARG ) { occurrence = new Occurrence (  EntityCast(arg0)
                                                                                   , *PYPATH_O(arg1) ); }
      else {
        PyErr_SetString( ConstructorError, "Bad type(s) of parameters given to Occurrence constructor." );
        return NULL;
      }
  
      pyOccurrence = PyObject_NEW( PyOccurrence, &PyTypeOccurrence );
      if (pyOccurrence == NULL) return NULL;
      
      pyOccurrence->_object = occurrence;
    HCATCH

    return (PyObject*)pyOccurrence;
  }


  static int  PyOccurrence_Init ( PyOccurrence* self, PyObject* args, PyObject* kwargs )
  {
    cdebug_log(20,0) << "PyOccurrence_Init(): " << (void*)self << endl;
    return 0;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_getEntity ()"

  static PyObject* PyOccurrence_getEntity ( PyOccurrence *self ) {
    cdebug_log(20,0) << "PyOccurrence_getEntity()" << endl;

    METHOD_HEAD ( "Occurrence.getEntity()" )

    PyObject* pyEntity = NULL;

    HTRY
    Entity* entity = occurrence->getEntity();
    if ( entity == NULL )
        Py_RETURN_NONE;

    pyEntity = PyEntity_NEW ( entity );
    HCATCH    

    return pyEntity;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_getOwnerCell ()"

  static PyObject* PyOccurrence_getOwnerCell ( PyOccurrence *self ) {
    cdebug_log(20,0) << "PyOccurrence_getOwnerCell()" << endl;

    METHOD_HEAD ( "Occurernce.getOwnerCell()" )

    Cell* cell = NULL;

    HTRY
    cell = occurrence->getOwnerCell();
    HCATCH    

    return PyCell_Link ( cell );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_getMasterCell ()"

  static PyObject* PyOccurrence_getMasterCell ( PyOccurrence *self )
  {
    cdebug_log(20,0) << "PyOccurrence_getMasterCell()" << endl;

    METHOD_HEAD ( "Occurrence.getMasterCell()" )

    Cell* cell = NULL;

    HTRY
    cell = occurrence->getMasterCell();
    HCATCH    

    return PyCell_Link ( cell );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_getPath ()"

  static PyObject* PyOccurrence_getPath ( PyOccurrence *self )
  {
    cdebug_log(20,0) << "PyOccurrence_getPath ()" << endl;
    
    METHOD_HEAD ( "Occurrence.getPath()" )

    PyPath* pyPath = PyObject_NEW ( PyPath, &PyTypePath );
    if ( pyPath == NULL ) { return NULL; }

    HTRY
    pyPath->_object = new Path ( occurrence->getPath() );
    HCATCH
    
    return ( (PyObject*)pyPath );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_getName ()"

  GetNameMethod(Occurrence, occurrence)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_getBoundingBox ()"

  static PyObject* PyOccurrence_getBoundingBox ( PyOccurrence *self )
  {
    cdebug_log(20,0) << "PyOccurrence_getBoundingBox ()" << endl;
    
    METHOD_HEAD ( "Occurrence.getBoundingBox()" )

    PyBox* pyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if ( pyBox == NULL ) { return NULL; }

    HTRY
    pyBox->_object = new Box ( occurrence->getBoundingBox() );
    HCATCH
    
    return ( (PyObject*)pyBox );
  }




  // ---------------------------------------------------------------
  // PyOccurrence Attribute Method table.

  PyMethodDef PyOccurrence_Methods[] =
    { { "getEntity"       , (PyCFunction)PyOccurrence_getEntity       , METH_NOARGS, "Returns the referenced entity." }
    , { "getPath"         , (PyCFunction)PyOccurrence_getPath         , METH_NOARGS, "Returns the hierarchical instaciation path." }
    , { "getName"         , (PyCFunction)PyOccurrence_getName         , METH_NOARGS, "Returns the occurrence name (concatenation of path and entity name." }
    , { "getCompactString", (PyCFunction)PyOccurrence_getCompactString, METH_NOARGS, "Returns the occurrence name (compact format)." }
    , { "getOwnerCell"    , (PyCFunction)PyOccurrence_getOwnerCell    , METH_NOARGS, "Returns the occurrence owner cell." }
    , { "getMasterCell"   , (PyCFunction)PyOccurrence_getMasterCell   , METH_NOARGS, "Returns the cell owning the referenced entity." }
    , { "getBoundingBox"  , (PyCFunction)PyOccurrence_getBoundingBox  , METH_NOARGS, "Returns the occurrence bounding box." }
    , { "isValid"         , (PyCFunction)PyOccurrence_isValid         , METH_NOARGS, "Returns true if the occurrence is valid." }
    , { "hasProperty"     , (PyCFunction)PyOccurrence_hasProperty     , METH_NOARGS, "Returns true if the occurrence owns some properties." }
    , { "destroy"         , (PyCFunction)PyOccurrence_destroy         , METH_NOARGS
                          , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |               "PyOccurrence" Object Methods                 |
  // x-------------------------------------------------------------x



  DirectDeleteMethod    (PyOccurrence_DeAlloc,PyOccurrence)
  DirectReprMethod      (PyOccurrence_Repr   ,PyOccurrence,   Occurrence)
  DirectStrMethod       (PyOccurrence_Str    ,PyOccurrence,   Occurrence)
  DirectCmpByValueMethod(PyOccurrence_Cmp    ,IsPyOccurrence, PyOccurrence)
  DirectHashMethod      (PyOccurrence_Hash   ,Occurrence)

  extern void  PyOccurrence_LinkPyType ()
  {
    cdebug_log(20,0) << "PyOccurrence_LinkType()" << endl;

    PyTypeOccurrence.tp_dealloc = (destructor) PyOccurrence_DeAlloc;
    PyTypeOccurrence.tp_compare = (cmpfunc)    PyOccurrence_Cmp;
    PyTypeOccurrence.tp_repr    = (reprfunc)   PyOccurrence_Repr;
    PyTypeOccurrence.tp_str     = (reprfunc)   PyOccurrence_Str;
    PyTypeOccurrence.tp_hash    = (hashfunc)   PyOccurrence_Hash;
    PyTypeOccurrence.tp_new     = (newfunc)    PyOccurrence_NEW;
    PyTypeOccurrence.tp_init    = (initproc)   PyOccurrence_Init;
    PyTypeOccurrence.tp_methods = PyOccurrence_Methods;
  }

//PyTypeObjectLinkPyTypeNewInit(Occurrence)
//PyTypeObjectLinkPyType(Occurrence)


#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyOccurrence" Shared Library Code Part              |
// x=================================================================x



  // ---------------------------------------------------------------
  // PyOccurrence Object Definitions.
  
  PyTypeObjectDefinitions(Occurrence)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
