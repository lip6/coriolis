// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                     Sophie BELLOEIL              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/isobar/PyEntity.h"            |
// +-----------------------------------------------------------------+


#ifndef ISOBAR_PY_ENTITY_H
#define ISOBAR_PY_ENTITY_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Entity.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyEntity".

    typedef struct {
        PyObject_HEAD
        Hurricane::Entity* _object;
    } PyEntity;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyObject* PyEntity_NEW        ( Hurricane::Entity* entity );
  extern  void      PyEntity_LinkPyType ();

  extern  PyTypeObject  PyTypeEntity;
  extern  PyMethodDef   PyEntity_Methods[];


#define IsPyEntity(v)    ( (v)->ob_type == &PyTypeEntity )
#define PYENTITY(v)      ( (PyEntity*)(v) )
#define PYENTITY_O(v)    ( PYENTITY(v)->_object )


  }  // extern "C".


  Hurricane::Entity*  EntityCast ( PyObject* derivedObject );


}  // Isobar namespace.

#endif  // ISOBAR_PY_ENTITY_H
