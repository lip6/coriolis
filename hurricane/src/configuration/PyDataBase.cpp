// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2020-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyDataBase.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyDataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Library.h"
#include "hurricane/Cell.h"


namespace  Isobar3 {

using namespace Hurricane;

extern "C" {


// Thin wrappers for overloadeds member functions.
  
  static Cell* getCell2 ( DataBase* db, string rpath, unsigned int flags )
  { return db->getCell( rpath, flags ); }

  static Cell* getCell1 ( DataBase* db, string name )
  { return db->getCell( name ); }


// Python methods.

  static PyObject* PyDataBase_create ( PyObject* self, PyObject* args )
  { return callFunction("DataBase.create",&DataBase::create,args); }

  static PyObject* PyDataBase_getDB ( PyObject* self, PyObject* args )
  { return callFunction("DataBase.getDB",&DataBase::getDB,args); }

  static PyObject* PyDataBase_getTechnology ( PyObject* self, PyObject* args )
  { return callMethod("DataBase.getTechnology",&DataBase::getTechnology,self,args); }

  static PyObject* PyDataBase_getRootLibrary ( PyObject* self, PyObject* args )
  { return callMethod("DataBase.getRootLibrary",&DataBase::getRootLibrary,self,args); }

  static PyObject* PyDataBase_getCell ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("DataBase.getCell",&getCell2,self,args);
    if (not rvalue) rvalue = callMethod("DataBase.getCell",&getCell1,self,args);
    return rvalue;
  }

  static PyObject* PyDataBase_clear ( PyObject* self, PyObject* args )
  { return callMethod("DataBase.clear",&DataBase::clear,self,args); }

  static PyObject* PyDataBase_destroy ( PyObject* self, PyObject* args )
  { return callMethod("DataBase.destroy",&DataBase::destroy,self,args); }


  // ---------------------------------------------------------------
  // PyDataBase Method table.

  PyMethodDef PyDataBase_Methods[] =
    { { "create"        , (PyCFunction)PyDataBase_create        , METH_NOARGS|METH_STATIC
                        , "Create the DataBase (only the first call created it)" }
    , { "getDB"         , (PyCFunction)PyDataBase_getDB         , METH_NOARGS|METH_STATIC
                        , "Get the DataBase" }
    , { "getTechnology" , (PyCFunction)PyDataBase_getTechnology , METH_NOARGS , "Return the Technology" }
    , { "getRootLibrary", (PyCFunction)PyDataBase_getRootLibrary, METH_NOARGS , "Return the root library" }
    , { "getCell"       , (PyCFunction)PyDataBase_getCell       , METH_VARARGS, "Return a Cell" }
    , { "clear"         , (PyCFunction)PyDataBase_clear         , METH_NOARGS , "Clear all the cells, keeps technology" }
    , { "destroy"       , (PyCFunction)PyDataBase_destroy       , METH_NOARGS
                        , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  PyMethodDef PyDataBaseFlags_Methods   [] = { {NULL, NULL, 0, NULL}   /* sentinel */ };


}  // extern "C".

}  // Isobar3 namespace.
