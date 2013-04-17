
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyDataBase.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyDataBase.h"
#include "hurricane/isobar/PyTechnology.h"
#include "hurricane/isobar/PyLibrary.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(DataBase,db,function)


// +=================================================================+
// |             "PyDataBase" Python Module Code Part                |
// +=================================================================+


#if defined(__PYTHON_MODULE__)


  PyObject* PyDataBase_getDB ( PyObject* module ) {
    trace << "PyDataBase_getDB()" << endl;

    DataBase* db = NULL;

    HTRY
    db = DataBase::getDB ();
    if ( db == NULL )
      PyErr_SetString ( HurricaneError, "DataBase has not been created yet" );
    HCATCH

    return PyDataBase_Link ( db );
  }


  PyObject* PyDataBase_getTechnology ( PyDataBase* self ) {
    trace << "PyDataBase_getTechnology()" << endl;

    Technology* techno = NULL;

    HTRY
    METHOD_HEAD("DataBase.getTechnology()")

    techno = db->getTechnology ();
    
    if ( techno == NULL )
        Py_RETURN_NONE;
    HCATCH

    return PyTechnology_Link ( techno );
  }


  static PyObject* PyDataBase_getRootLibrary ( PyDataBase *self ) {
    trace << "PyDataBase_getRootLibrary ()" << endl;

    Library* library = NULL;

    HTRY
    METHOD_HEAD ( "DataBase.getRootLibrary()" )
    library = db->getRootLibrary ();
    HCATCH
    
    return PyLibrary_Link(library);
  }

  
  // Standart Accessors (Attributes).
  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyDataBase_destroy,PyDataBase)


  // ---------------------------------------------------------------
  // PyDataBase Attribute Method table.

  PyMethodDef PyDataBase_Methods[] =
    { { "getTechnology" , (PyCFunction)PyDataBase_getTechnology , METH_NOARGS, "Return the Technology" }
    , { "getRootLibrary", (PyCFunction)PyDataBase_getRootLibrary, METH_NOARGS, "Return the root library" }
    , { "destroy"       , (PyCFunction)PyDataBase_destroy       , METH_NOARGS
                        , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  DBoDeleteMethod(DataBase)
  PyTypeObjectLinkPyType(DataBase)

#else  // End of Python Module Code Part.


// +=================================================================+
// |               "PyDataBase" Shared Library Code Part             |
// +=================================================================+


  PyObject* PyDataBase_create ( PyObject *module ) {
    trace << "PyDataBase_create()" << endl;

    DataBase* db = NULL;
    
    HTRY
    db = DataBase::create ();
    HCATCH

    return PyDataBase_Link(db);
  }


  // Link/Creation Method.
  DBoLinkCreateMethod(DataBase)

      
  PyTypeObjectDefinitions(DataBase)

#endif  // End of Shared Library Code Part.


}  // extern "C".


}  // Isobar namespace.
 
