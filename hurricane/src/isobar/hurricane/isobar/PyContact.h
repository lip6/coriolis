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
// |  C++ Header  :  "./hurricane/isobar/PyContact.h"                |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyComponent.h"
#include "hurricane/Contact.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyContact".

    typedef struct {
        PyComponent _baseObject;
    } PyContact;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject  PyTypeContact;
    extern PyMethodDef   PyContact_Methods[];
  
    extern PyObject* PyContact_Link       ( Hurricane::Contact* object );
    extern void      PyContact_LinkPyType ();


#define IsPyContact(v)    ( (v)->ob_type == &PyTypeContact )
#define PYCONTACT(v)      ( (PyContact*)(v) )
#define PYCONTACT_O(v)    ( PYCONTACT(v)->_baseObject._baseObject._object )


  }  // extern "C".

}  // Isobar namespace.
