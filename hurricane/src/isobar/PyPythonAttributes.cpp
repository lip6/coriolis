// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Uiversité 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyPythonAttributes.cpp"                 |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyEntity.h"
#include "hurricane/isobar/PyPythonAttributes.h"
#include "hurricane/isobar/PythonAttributes.h"
#include <sstream>


namespace  Isobar {

using namespace Hurricane;

extern "C" {

  using std::ostringstream;


#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(PythonAttributes,property,function)


// +=================================================================+
// |          "PyPythonAttributes" Python Module Code Part           |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  static void  PyPythonAttributes_DeAlloc ( PyPythonAttributes* self )
  {
    cdebug_log(20,0) << "PyPythonAttributes_DeAlloc(" << hex << self << ")" << endl;
  }


  static PyObject* PyPythonAttributes_get ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyPythonAttributes_get()" << endl;

    PyAttributesHolder* pyHolder = NULL;
    HTRY
      PyObject* pyDBo = NULL;
    
      if (PyArg_ParseTuple( args, "O:PythonAttributes.get", &pyDBo )) {
        Entity* entity = EntityCast( pyDBo );
        if (not entity) {
          PyErr_SetString( ConstructorError, "PythonAttributes.get(): First argument is not a DBo." );
          return NULL;
        }
        pyHolder = PythonAttributes::get( entity );
        if (not pyHolder) {
          ostringstream os;
          // os << "PythonAttributes.get(): Object " << entity
          //    << " has no ancillary property\n";
          // cerr << os.str() << endl;
          Py_RETURN_NONE;
        }
      } else {
        PyErr_SetString( ConstructorError, "PythonAttributes.get(): Bad parameters." );
        return NULL;
      }
    HCATCH

    Py_INCREF( pyHolder );
    return (PyObject*)pyHolder;
  }


  static PyObject* PyPythonAttributes_delattr ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyPythonAttributes_delattr()" << endl;

    size_t dictSize = 0;
    HTRY
      PyObject* pyDBo    = NULL;
      char*     attrName = NULL;
    
    if (PyArg_ParseTuple( args, "Os:PythonAttributes.delattr", &pyDBo, &attrName )) {
        Entity* entity = EntityCast( pyDBo );
        if (not entity) {
          PyErr_SetString( ConstructorError, "PythonAttributes.delattr(): First argument is not a DBo." );
          return NULL;
        }
        dictSize = PythonAttributes::delattr( entity, attrName );
      } else {
        PyErr_SetString( ConstructorError, "PythonAttributes.delattr(): Bad parameters." );
        return NULL;
      }
    HCATCH
    return Py_BuildValue( "i", dictSize );
  }


  static PyObject* PyPythonAttributes_enable ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyPythonAttributes_enable()" << endl;

    HTRY
      PyObject* pyDBo = NULL;
    
      if (PyArg_ParseTuple( args, "O:PythonAttributes.enable", &pyDBo )) {
        Entity* entity = EntityCast( pyDBo );
        if (not entity) {
          PyErr_SetString( ConstructorError, "PythonAttributes.enable(): First argument is not a DBo." );
          return NULL;
        }
        PythonAttributes::enable( entity );
      } else {
        PyErr_SetString( ConstructorError, "PythonAttributes.enable(): Bad parameters." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyPythonAttributes_disable ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyPythonAttributes_disable()" << endl;

    HTRY
      PyObject* pyDBo = NULL;
    
      if (PyArg_ParseTuple( args, "O:PythonAttributes.disable", &pyDBo )) {
        Entity* entity = EntityCast( pyDBo );
        if (not entity) {
          PyErr_SetString( ConstructorError, "PythonAttributes.disable(): First argument is not a DBo." );
          return NULL;
        }
        PythonAttributes::disable( entity );
      } else {
        PyErr_SetString( ConstructorError, "PythonAttributes.disable(): Bad parameters." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyPythonAttributes_disableAll ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyPythonAttributes_disableAll()" << endl;

    HTRY
      char* attrName = NULL;
      if (PyArg_ParseTuple( args, "|s:PythonAttributes.disableAll", &attrName )) {
        if (attrName) PythonAttributes::disableAll( attrName );
        else          PythonAttributes::disableAll();
      } else {
        PyErr_SetString( ConstructorError, "PythonAttributes.disableAll(): Bad parameters." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyPythonAttributes_count ( PyObject* )
  {
    cdebug_log(20,0) << "PyPythonAttributes_count()" << endl;
    int32_t count = 0;
    HTRY
      count = PythonAttributes::count();
    HCATCH
    return Py_BuildValue( "i", count );
  }


  PyMethodDef PyPythonAttributes_Methods[] =
    { { "get"             , (PyCFunction)PyPythonAttributes_get       , METH_VARARGS|METH_CLASS
                          , "Retrieve the ancillary Python object (stored in a Property)." }
    , { "delattr"         , (PyCFunction)PyPythonAttributes_delattr   , METH_VARARGS|METH_CLASS
                          , "Remove an attribute from the ancillary Python object." }
    , { "enable"          , (PyCFunction)PyPythonAttributes_enable    , METH_VARARGS|METH_CLASS
                          , "Create the ancillary Python object (and the Property)." }
    , { "disable"         , (PyCFunction)PyPythonAttributes_disable   , METH_VARARGS|METH_CLASS
                          , "Remove the ancillary Python object." }
    , { "disableAll"      , (PyCFunction)PyPythonAttributes_disableAll, METH_VARARGS|METH_CLASS
                          , "Remove all the ancillary Python objects." }
    , { "count"           , (PyCFunction)PyPythonAttributes_count     , METH_NOARGS|METH_CLASS
                          , "Return the number of currently allocated the ancillary Python objects." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  PyTypeObjectLinkPyTypeWithoutObject(PythonAttributes,PythonAttributes)


#else  // End of Python Module Code Part.


// +=================================================================+
// |         "PyPythonAttributes" Shared Library Code Part           |
// +=================================================================+


  PyTypeObjectDefinitions(PythonAttributes)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // Isobar namespace.
 
