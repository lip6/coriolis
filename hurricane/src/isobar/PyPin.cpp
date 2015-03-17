// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyPin.cpp"                                   |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPin.h"
#include "hurricane/isobar/PyPinPlacementStatus.h"
#include "hurricane/isobar/PyPinDirection.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"


namespace Isobar {

using namespace Hurricane;

extern "C" {


#undef   ACCESS_OBJECT
#undef   ACCESS_CLASS
#define  ACCESS_OBJECT           _baseObject._baseObject._baseObject._object
#define  ACCESS_CLASS(_pyObject) &(_pyObject->_baseObject._baseObject._baseObject)
#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Pin,pin,function)


// x=================================================================x
// |                "PyPin" Python Module Code Part                  |
// x=================================================================x

#if defined(__PYTHON_MODULE__)
    

  // x-------------------------------------------------------------x
  // |                 "PyPin" Attribute Methods                   |
  // x-------------------------------------------------------------x


  static Pin::PlacementStatus  PyInt_AsPlacementStatus ( PyObject* object ) {
    switch ( PyAny_AsLong(object) ) {
      case Pin::PlacementStatus::UNPLACED : return ( Pin::PlacementStatus(Pin::PlacementStatus::UNPLACED) );
      case Pin::PlacementStatus::PLACED   : return ( Pin::PlacementStatus(Pin::PlacementStatus::PLACED) );
      case Pin::PlacementStatus::FIXED    : return ( Pin::PlacementStatus(Pin::PlacementStatus::FIXED) );
   }

    return ( Pin::PlacementStatus(Pin::PlacementStatus::UNPLACED) );
  }


  static Pin::AccessDirection  PyInt_AsAccessDirection ( PyObject* object ) {
    switch ( PyAny_AsLong(object) ) {
      case Pin::AccessDirection::UNDEFINED : return ( Pin::AccessDirection(Pin::AccessDirection::UNDEFINED) );
      case Pin::AccessDirection::NORTH     : return ( Pin::AccessDirection(Pin::AccessDirection::NORTH) );
      case Pin::AccessDirection::SOUTH     : return ( Pin::AccessDirection(Pin::AccessDirection::SOUTH) );
      case Pin::AccessDirection::EAST      : return ( Pin::AccessDirection(Pin::AccessDirection::EAST) );
      case Pin::AccessDirection::WEST      : return ( Pin::AccessDirection(Pin::AccessDirection::WEST) );
    }

    return ( Pin::AccessDirection(Pin::AccessDirection::UNDEFINED) );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPin_create ()"

  static PyObject* PyPin_create ( PyObject*, PyObject *args ) {
    Pin* pin = NULL;
    
    HTRY

    trace << "PyPin_create()" << endl;
    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;
    PyObject* arg4;
    PyObject* arg5;
    PyObject* arg6;
    PyObject* arg7;
    PyObject* arg8;

    __cs.init ("Pin.create");
    if (!PyArg_ParseTuple(args,"O&O&O&O&O&O&O&|O&O&:Pin.create"
                           , Converter, &arg0
                           , Converter, &arg1
                           , Converter, &arg2
                           , Converter, &arg3
                           , Converter, &arg4
                           , Converter, &arg5
                           , Converter, &arg6
                           , Converter, &arg7
                           , Converter, &arg8
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Pin constructor." );
        return NULL;
    }

    string pin_arg1 = ":ent:string:int:int:layer:int:int:int:int";
    string pin_arg2 = ":ent:string:int:int:layer:int:int:int";
    string pin_arg3 = ":ent:string:int:int:layer:int:int";
    if ( __cs.getObjectIds() == pin_arg1 ) {
        pin = Pin::create(
                PYNET_O ( arg0 ),
                Name(PyString_AsString(arg1)),
                PyInt_AsAccessDirection ( arg2 ),
                PyInt_AsPlacementStatus ( arg3 ),
                PYLAYER_O ( arg4 ),
                PyAny_AsLong ( arg5 ),
                PyAny_AsLong ( arg6 ),
                PyAny_AsLong ( arg7 ),
                PyAny_AsLong ( arg8 ) );
    } else if ( __cs.getObjectIds() == pin_arg2 ) {
        pin = Pin::create (
                PYNET_O ( arg0 ),
                Name(PyString_AsString(arg1)),
                PyInt_AsAccessDirection ( arg2 ),
                PyInt_AsPlacementStatus ( arg3 ),
                PYLAYER_O ( arg4 ),
                PyAny_AsLong ( arg5 ),
                PyAny_AsLong ( arg6 ),
                PyAny_AsLong ( arg7 ) );
    } else if ( __cs.getObjectIds() == pin_arg3 ) {
        pin = Pin::create ( PYNET_O ( arg0 ),
                Name(PyString_AsString(arg1)),
                PyInt_AsAccessDirection ( arg2 ),
                PyInt_AsPlacementStatus ( arg3 ),
                PYLAYER_O ( arg4 ),
                PyAny_AsLong ( arg5 ),
                PyAny_AsLong ( arg6 ) );
    } else {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Pin constructor." );
        return NULL;
    }

    HCATCH

    return PyPin_Link ( pin );
  }
  

  // Standart Accessors (Attributes).
  
  PyObject* PyPin_getAccessDirection( PyPin* self ) {
      trace << "PyNet_getAccessDirection ()" << endl;

      METHOD_HEAD ( "Net.getAccessDirection()" )
  
      return (PyObject *)PyLong_FromLong( pin->getAccessDirection().getCode() );

  }
    

  // ---------------------------------------------------------------
  // PyPin Attribute Method table.

  PyMethodDef PyPin_Methods[] =
    { { "create"              , (PyCFunction)PyPin_create              , METH_VARARGS|METH_STATIC
                              , "Create a new Pin." }
    , { "getAccessDirection"  , (PyCFunction)PyPin_getAccessDirection  , METH_NOARGS
                              , "Returns the pin accessdirection (by default set to UNDEFINED) ." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                  "PyPin" Object Methods                     |
  // x-------------------------------------------------------------x


  DBoDeleteMethod(Pin)
  PyTypeObjectLinkPyType(Pin)


#else  // End of Python Module Code Part.


// x=================================================================x
// |                "PyPin" Shared Library Code Part                 |
// x=================================================================x



  // Link/Creation Method.
  DBoLinkCreateMethod(Pin)
  PyTypeInheritedObjectDefinitions(Pin, Contact)


  extern  void  PyPin_postModuleInit ()
  {
    PyPinPlacementStatus_postModuleInit();
    PyPinDirection_postModuleInit();

    PyDict_SetItemString( PyTypePin.tp_dict, "PlacementStatus", (PyObject*)&PyTypePinPlacementStatus );
    PyDict_SetItemString( PyTypePin.tp_dict, "Direction"      , (PyObject*)&PyTypePinDirection );
  }


#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Isobar namespace.
 
