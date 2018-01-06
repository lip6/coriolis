// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyInstance.h"               |
// +-----------------------------------------------------------------+


#ifndef PY_INSTANCE_H
#define PY_INSTANCE_H


#include "hurricane/isobar/PyEntity.h"
#include "hurricane/Instance.h"


namespace  Isobar {

  using namespace Hurricane;


  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyInstance".

    typedef struct {
        PyEntity  _baseObject;
    } PyInstance;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypeInstance;
  extern  PyMethodDef   PyInstance_Methods[];

  extern  PyObject* PyInstance_Link           ( Hurricane::Instance* object);
  extern  void      PyInstance_LinkPyType     ();
  extern  void      PyInstance_postModuleInit ();


#define IsPyInstance(v) ((v)->ob_type == &PyTypeInstance )
#define PYINSTANCE(v)   ((PyInstance*)(v) )
#define PYINSTANCE_O(v) (static_cast<Instance*>(PYINSTANCE(v)->_baseObject._object))


  }  // extern "C".

}  // Isobar namespace.

#endif  // PY_INSTANCE_H
