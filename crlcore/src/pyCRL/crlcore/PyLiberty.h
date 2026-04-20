// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2013-2026, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                             Hippolyte MELICA     |
// |  E-mail      :  hippolyte.melica@etu.sorbonne-universite.fr     |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyLiberty.h"                         |
// +-----------------------------------------------------------------+


#ifndef __CRL_PY_LIBERTY__
#define __CRL_PY_LIBERTY__

#include "hurricane/isobar/PyHurricane.h"

// Forward declarations - avoid circular includes
namespace Liberty {
  class Library;
  class Group;
  class Attribute;
  class Value;
}


namespace  CRL {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyLibertyLibrary".

  typedef struct {
      PyObject_HEAD
      Liberty::Library* _object;
  } PyLibertyLibrary;


// -------------------------------------------------------------------
// Python Object  :  "PyLibertyGroup".

  typedef struct {
      PyObject_HEAD
      Liberty::Group* _object;
  } PyLibertyGroup;


// -------------------------------------------------------------------
// Python Object  :  "PyLibertyAttribute".

  typedef struct {
      PyObject_HEAD
      Liberty::Attribute* _object;
  } PyLibertyAttribute;


// -------------------------------------------------------------------
// Python Object  :  "PyLibertyValue".

  typedef struct {
      PyObject_HEAD
      Liberty::Value* _object;
  } PyLibertyValue;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.cpp".

  extern  PyTypeObject  PyTypeLibertyLibrary;
  extern  PyTypeObject  PyTypeLibertyGroup;
  extern  PyTypeObject  PyTypeLibertyAttribute;
  extern  PyTypeObject  PyTypeLibertyValue;

  extern  PyMethodDef   PyLibertyLibrary_Methods[];
  extern  PyMethodDef   PyLibertyGroup_Methods[];
  extern  PyMethodDef   PyLibertyAttribute_Methods[];
  extern  PyMethodDef   PyLibertyValue_Methods[];

  extern  PyObject* PyLibertyLibrary_Link           ( Liberty::Library* );
  extern  void      PyLibertyLibrary_LinkPyType     ();

  extern  PyObject* PyLibertyGroup_Link             ( Liberty::Group* );
  extern  void      PyLibertyGroup_LinkPyType       ();

  extern  PyObject* PyLibertyAttribute_Link         ( Liberty::Attribute* );
  extern  void      PyLibertyAttribute_LinkPyType   ();

  extern  PyObject* PyLibertyValue_Link             ( Liberty::Value* );
  extern  void      PyLibertyValue_LinkPyType       ();

  extern  PyObject* PyCRL_getLibertyGroupFromCell   ( PyObject*, PyObject* );


#define IsPyLibertyLibrary(v)      ( (v)->ob_type == &PyTypeLibertyLibrary )
#define PYLIBERTY_LIBRARY(v)       ( (PyLibertyLibrary*)(v) )
#define PYLIBERTY_LIBRARY_O(v)     ( PYLIBERTY_LIBRARY(v)->_object )

#define IsPyLibertyGroup(v)        ( (v)->ob_type == &PyTypeLibertyGroup )
#define PYLIBERTY_GROUP(v)         ( (PyLibertyGroup*)(v) )
#define PYLIBERTY_GROUP_O(v)       ( PYLIBERTY_GROUP(v)->_object )

#define IsPyLibertyAttribute(v)    ( (v)->ob_type == &PyTypeLibertyAttribute )
#define PYLIBERTY_ATTRIBUTE(v)     ( (PyLibertyAttribute*)(v) )
#define PYLIBERTY_ATTRIBUTE_O(v)   ( PYLIBERTY_ATTRIBUTE(v)->_object )

#define IsPyLibertyValue(v)        ( (v)->ob_type == &PyTypeLibertyValue )
#define PYLIBERTY_VALUE(v)         ( (PyLibertyValue*)(v) )
#define PYLIBERTY_VALUE_O(v)       ( PYLIBERTY_VALUE(v)->_object )


}  // extern "C".

}  // namespace CRL.


#endif  // __CRL_PY_LIBERTY__
