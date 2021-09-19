// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/isobar/PyDataBase.h"          |
// +-----------------------------------------------------------------+


#ifndef PY_DATABASE_H
#define PY_DATABASE_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/DataBase.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyDataBase".

    typedef struct {
        PyObject_HEAD
        Hurricane::DataBase* _object;
    } PyDataBase;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypeDataBase;
    extern  PyMethodDef   PyDataBase_Methods[];

    extern  PyObject* PyDataBase_Link         ( Hurricane::DataBase* db  );
    extern  void      PyDataBase_LinkPyType   ();


#define IsPyDataBase(v)    ( Py_TYPE(v) == &PyTypeDataBase )
#define PYDATABASE(v)      ( (PyDataBase*)(v) )
#define PYDATABASE_O(v)    ( PYDATABASE(v)->_object )


  }  // extern "C".

}  // Isobar namespace.

#endif  // PY_DATABASE_H
