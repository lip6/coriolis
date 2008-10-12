
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id: PyPin.cpp,v 1.18 2008/02/07 17:09:41 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :          Damien.Dupuis@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :            "./PyPin.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "hurricane/isobar/PyPin.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyName.h"
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
  // |                  Global Constants Loading                   |
  // x-------------------------------------------------------------x


  extern void  PinLoadConstants ( PyObject* dictionnary ) {
    PyObject* constant;
      
    LOAD_CONSTANT ( Pin::PlacementStatus::UNPLACED , "PinPlacementStatusUNPLACED"  )
    LOAD_CONSTANT ( Pin::PlacementStatus::PLACED   , "PinPlacementStatusPLACED"    )
    LOAD_CONSTANT ( Pin::PlacementStatus::FIXED    , "PinPlacementStatusFIXED"     )
    LOAD_CONSTANT ( Pin::AccessDirection::UNDEFINED, "PinAccessDirectionUNDEFINED" )
    LOAD_CONSTANT ( Pin::AccessDirection::NORTH    , "PinAccessDirectionNORTH"     )
    LOAD_CONSTANT ( Pin::AccessDirection::SOUTH    , "PinAccessDirectionSOUTH"     )
    LOAD_CONSTANT ( Pin::AccessDirection::EAST     , "PinAccessDirectionEAST"      )
    LOAD_CONSTANT ( Pin::AccessDirection::WEST     , "PinAccessDirectionWEST"      )
  }

    

  // x-------------------------------------------------------------x
  // |                 "PyPin" Attribute Methods                   |
  // x-------------------------------------------------------------x
  

  // Standart Accessors (Attributes).
  
  PyObject* PyPin_getAccessDirection( PyPin* self ) {
      trace << "PyNet_getAccessDirection ()" << endl;

      METHOD_HEAD ( "Net.getAccessDirection()" )
  
      return (PyObject *)Py_BuildValue("l",  pin->getAccessDirection().getCode() );

  }
    

  // ---------------------------------------------------------------
  // PyPin Attribute Method table.

  PyMethodDef PyPin_Methods[] =
    { { "getAccessDirection"  , (PyCFunction)PyPin_getAccessDirection  , METH_NOARGS
                              , "Returns the pin accessdirection (by default set to UNDEFINED) ." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                  "PyPin" Object Methods                     |
  // x-------------------------------------------------------------x

  static Pin::PlacementStatus  PyInt_AsPlacementStatus ( PyObject* object ) {
    switch ( PyInt_AsLong(object) ) {
      case Pin::PlacementStatus::UNPLACED : return ( Pin::PlacementStatus(Pin::PlacementStatus::UNPLACED) );
      case Pin::PlacementStatus::PLACED   : return ( Pin::PlacementStatus(Pin::PlacementStatus::PLACED) );
      case Pin::PlacementStatus::FIXED    : return ( Pin::PlacementStatus(Pin::PlacementStatus::FIXED) );
   }

    return ( Pin::PlacementStatus(Pin::PlacementStatus::UNPLACED) );
  }

  static Pin::AccessDirection  PyInt_AsAccessDirection ( PyObject* object ) {
    switch ( PyInt_AsLong(object) ) {
      case Pin::AccessDirection::UNDEFINED : return ( Pin::AccessDirection(Pin::AccessDirection::UNDEFINED) );
      case Pin::AccessDirection::NORTH     : return ( Pin::AccessDirection(Pin::AccessDirection::NORTH) );
      case Pin::AccessDirection::SOUTH     : return ( Pin::AccessDirection(Pin::AccessDirection::SOUTH) );
      case Pin::AccessDirection::EAST      : return ( Pin::AccessDirection(Pin::AccessDirection::EAST) );
      case Pin::AccessDirection::WEST      : return ( Pin::AccessDirection(Pin::AccessDirection::WEST) );
    }

    return ( Pin::AccessDirection(Pin::AccessDirection::UNDEFINED) );
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPin_new ()"

  static PyObject* PyPin_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    Pin* pin = NULL;
    
    HTRY

    trace << "PyPin_new()" << endl;

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;
    PyObject* arg4;
    PyObject* arg5;
    PyObject* arg6;
    PyObject* arg7;
    PyObject* arg8;

    __cs.Init ("Pin.new");
    if ( ! PyArg_ParseTuple(args,"O&O&O&O&O&O&O&|O&O&:Pin.new"
                           , Converter, &arg0
                           , Converter, &arg1
                           , Converter, &arg2
                           , Converter, &arg3
                           , Converter, &arg4
                           , Converter, &arg5
                           , Converter, &arg6
                           , Converter, &arg7
                           , Converter, &arg8
                           ) ) return ( NULL );

    string pin_arg1 = ":ent:name:int:int:layer:int:int:int:int";
    string pin_arg2 = ":ent:name:int:int:layer:int:int:int";
    string pin_arg3 = ":ent:name:int:int:layer:int:int";
    if      ( __cs.getObjectIds() == pin_arg1 ) { pin = Pin::create ( PYNET_O ( arg0 )
                                                                    , *PYNAME_O ( arg1 )
 											        				, PyInt_AsAccessDirection ( arg2 )
  											        				, PyInt_AsPlacementStatus ( arg3 )
                                                                    , PYLAYER_O ( arg4 )
                                                                    , PyInt_AsLong ( arg5 )
                                                                    , PyInt_AsLong ( arg6 )
                                                                    , PyInt_AsLong ( arg7 )
                                                                    , PyInt_AsLong ( arg8 ) ); }
    else if ( __cs.getObjectIds() == pin_arg2 ) { pin = Pin::create ( PYNET_O ( arg0 )
                                                                    , *PYNAME_O ( arg1 )
 											        				, PyInt_AsAccessDirection ( arg2 )
  											        				, PyInt_AsPlacementStatus ( arg3 )
                                                                    , PYLAYER_O ( arg4 )
                                                                    , PyInt_AsLong ( arg5 )
                                                                    , PyInt_AsLong ( arg6 )
                                                                    , PyInt_AsLong ( arg7 ) ); }
    else if ( __cs.getObjectIds() == pin_arg3 ) { pin = Pin::create ( PYNET_O ( arg0 )
                                                                    , *PYNAME_O ( arg1 )
 											        				, PyInt_AsAccessDirection ( arg2 )
  											        				, PyInt_AsPlacementStatus ( arg3 )
                                                                    , PYLAYER_O ( arg4 )
                                                                    , PyInt_AsLong ( arg5 )
                                                                    , PyInt_AsLong ( arg6 ) ); }
    else {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Pin constructor." );
        return ( NULL );
    }

    HCATCH

    return PyPin_Link ( pin );
  }



  DBoDeleteMethod(Pin)
  PyTypeObjectLinkPyType(Pin)
  PyTypeObjectConstructor(Pin)


#else  // End of Python Module Code Part.


// x=================================================================x
// |                "PyPin" Shared Library Code Part                 |
// x=================================================================x



  // Link/Creation Method.
  DBoLinkCreateMethod(Pin)




  // ---------------------------------------------------------------
  // PyPin Object Definitions.

  PyTypeObjectDefinitions(Pin)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Isobar namespace.
 
