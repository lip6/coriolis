
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
// $Id: PyNet.cpp,v 1.27 2008/02/07 17:09:41 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Sophie BELLOEIL               |
// |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyNet.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyName.h"
#include "hurricane/isobar/PyCell.h" 
#include "hurricane/isobar/PyPoint.h" 
#include "hurricane/isobar/PyPlugLocator.h" 
#include "hurricane/isobar/PySegmentLocator.h" 
#include "hurricane/isobar/PyComponentLocator.h" 
#include "hurricane/isobar/PyPinLocator.h" 

#include "hurricane/NetExternalComponents.h"
using namespace Hurricane;



namespace  Isobar {


extern "C" {


# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT           _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
# define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Net,net,function)

# define  LOAD_CONSTANT(CONSTANT_VALUE,CONSTANT_NAME)             \
  constant = PyInt_FromLong ( (long)CONSTANT_VALUE );             \
  PyDict_SetItemString ( dictionnary, CONSTANT_NAME, constant );  \
  Py_DECREF ( constant );


// x=================================================================x
// |                 "PyNet" Python Module Code Part                 |
// x=================================================================x

# if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyNet" Local Functions                     |
  // x-------------------------------------------------------------x

  
  // ---------------------------------------------------------------
  // Local Function  :  "PyInt_AsType ()"
  
  static Net::Type  PyInt_AsType ( PyObject* object ) {
    switch ( PyInt_AsLong(object) ) {
      case Net::Type::UNDEFINED : return ( Net::Type(Net::Type::UNDEFINED) );
      case Net::Type::LOGICAL   : return ( Net::Type(Net::Type::LOGICAL) );
      case Net::Type::CLOCK     : return ( Net::Type(Net::Type::CLOCK) );
      case Net::Type::POWER     : return ( Net::Type(Net::Type::POWER) );
      case Net::Type::GROUND    : return ( Net::Type(Net::Type::GROUND) );
    }

    return ( Net::Type(Net::Type::UNDEFINED) );
  }



  // ---------------------------------------------------------------
  // Local Function  :  "PyInt_AsDirection ()"

  static Net::Direction  PyInt_AsDirection ( PyObject* object )
  {
    switch ( PyInt_AsLong(object) ) {
      case Net::Direction::UNDEFINED      : return ( Net::Direction(Net::Direction::UNDEFINED) );
      case Net::Direction::IN             : return ( Net::Direction(Net::Direction::IN) );
      case Net::Direction::OUT            : return ( Net::Direction(Net::Direction::OUT) );
      case Net::Direction::INOUT          : return ( Net::Direction(Net::Direction::INOUT) );
      case Net::Direction::TRISTATE       : return ( Net::Direction(Net::Direction::TRISTATE) );
    }

    return ( Net::Direction(Net::Direction::UNDEFINED) );
  }




  // x-------------------------------------------------------------x
  // |                  Global Constants Loading                   |
  // x-------------------------------------------------------------x

  extern void  NetLoadConstants ( PyObject* dictionnary )
  {
    PyObject* constant;

    LOAD_CONSTANT ( Net::Type::UNDEFINED        , "TypeUNDEFINED" )
    LOAD_CONSTANT ( Net::Type::LOGICAL          , "TypeLOGICAL" )
    LOAD_CONSTANT ( Net::Type::CLOCK            , "TypeCLOCK" )
    LOAD_CONSTANT ( Net::Type::POWER            , "TypePOWER" )
    LOAD_CONSTANT ( Net::Type::GROUND           , "TypeGROUND" )
    LOAD_CONSTANT ( Net::Direction::UNDEFINED   , "DirectionUNDEFINED" )
    LOAD_CONSTANT ( Net::Direction::IN          , "DirectionIN" )
    LOAD_CONSTANT ( Net::Direction::OUT         , "DirectionOUT" )
    LOAD_CONSTANT ( Net::Direction::INOUT       , "DirectionINOUT" )
    LOAD_CONSTANT ( Net::Direction::TRISTATE    , "DirectionTRISTATE" )
  }



  // x-------------------------------------------------------------x
  // |                "PyNet" Attribute Methods                    |
  // x-------------------------------------------------------------x



  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyNet_getX,getX,PyNet,Net)
  DirectGetLongAttribute(PyNet_getY,getY,PyNet,Net)  
  
  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyNet_isGlobal  ,isGlobal  ,PyNet,Net)
  DirectGetBoolAttribute(PyNet_isExternal,isExternal,PyNet,Net)
  DirectGetBoolAttribute(PyNet_isLogical ,isLogical ,PyNet,Net)
  DirectGetBoolAttribute(PyNet_isClock   ,isClock   ,PyNet,Net)
  DirectGetBoolAttribute(PyNet_isGround  ,isGround  ,PyNet,Net)
  DirectGetBoolAttribute(PyNet_isPower   ,isPower   ,PyNet,Net)
  DirectGetBoolAttribute(PyNet_isSupply  ,isSupply  ,PyNet,Net)

  GetBoundStateAttribute(PyNet_IsPyBound            ,PyNet,Net)

  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyNet_destroy, PyNet)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getName ()"

  static PyObject* PyNet_getName ( PyNet *self ) {
    trace << "PyNet_getName ()" << endl;
    
    METHOD_HEAD ( "Net.getName()" )

    PyName* pyName = PyObject_NEW ( PyName, &PyTypeName );
    if ( pyName == NULL ) { return NULL; }
  
    trace_in ();
    trace << "new PyName [" << hex << pyName << "]" << endl;
    trace_out ();

    HTRY

    pyName->_object = new Name ( net->getName() );

    HCATCH
    
    return ( (PyObject*)pyName ); 
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getType ()"

  static PyObject* PyNet_getType ( PyNet *self )
  {
    trace << "PyNet_getType ()" << endl;
    
    METHOD_HEAD ( "Net.getType()" )

    return ( (PyObject*)Py_BuildValue("i",(long)net->getType().getCode()) );
  }

  

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getDirection ()"

  static PyObject* PyNet_getDirection ( PyNet *self )
  {
    trace << "PyNet_getDirection ()" << endl;
   
    METHOD_HEAD ( "Net.getDirection()" )

    return ( (PyObject*)Py_BuildValue("i",(long)net->getDirection().getCode()) );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getPlugsLocator ()"

  static PyObject* PyNet_getPlugsLocator ( PyNet *self )
  {
    trace << "PyNet_getPlugsLocator ()" << endl;

    METHOD_HEAD ( "Net.getPlugsLocator()" )

    Plugs plugs = net->getPlugs ();

    PyPlugLocator* pyPlugLocator = PyObject_NEW ( PyPlugLocator, &PyTypePlugLocator );
    if (pyPlugLocator == NULL) { return NULL; }
  
    trace_in ();
    trace << "new PyPlugLocator [" << hex << pyPlugLocator << "]" << endl;
    trace_out ();

    HTRY

    pyPlugLocator->_object = plugs.getLocator ();

    HCATCH

    return ( (PyObject*)pyPlugLocator );
  }

    
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getSegmentsLocator ()"

  static PyObject* PyNet_getSegmentsLocator ( PyNet *self )
  {
    trace << "PyNet_getSegmentsLocator ()" << endl;

    METHOD_HEAD ( "Net.getSegmentsLocator()" )

    Segments segments = net->getSegments ();

    PySegmentLocator* pySegmentLocator = PyObject_NEW ( PySegmentLocator, &PyTypeSegmentLocator );
    if (pySegmentLocator == NULL) { return NULL; }
  
    trace_in ();
    trace << "new PySegmentLocator [" << hex << pySegmentLocator << "]" << endl;
    trace_out ();

    HTRY

    pySegmentLocator->_object = segments.getLocator ();

    HCATCH

    return ( (PyObject*)pySegmentLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getPinsLocator ()"

  static PyObject* PyNet_getPinsLocator ( PyNet *self )
  {
    trace << "PyNet_getPinsLocator ()" << endl;

    METHOD_HEAD ( "Net.getPinsLocator()" )

    Pins pins = net->getPins ();

    PyPinLocator* pyPinLocator = PyObject_NEW ( PyPinLocator, &PyTypePinLocator );
    if (pyPinLocator == NULL) { return NULL; }
  
    trace_in ();
    trace << "new PyPinLocator [" << hex << pyPinLocator << "]" << endl;
    trace_out ();

    HTRY

    pyPinLocator->_object = pins.getLocator ();

    HCATCH

    return ( (PyObject*)pyPinLocator );
  }
  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getExternalComponentsLocator ()"

  static PyObject* PyNet_getExternalComponentsLocator ( PyNet *self )
  {
    trace << "PyNet_getExternalComponentsLocator ()" << endl;

    METHOD_HEAD ( "Net.getExternalcomponentsLocator()" )

    Components externalComponents = NetExternalComponents::get(net);

    PyComponentLocator* pyExternalComponentsLocator = PyObject_NEW ( PyComponentLocator, &PyTypeComponentLocator );
    if (pyExternalComponentsLocator == NULL) { return NULL; }
  
    trace_in ();
    trace << "new PyExternalComponentsLocator [" << hex << pyExternalComponentsLocator << "]" << endl;
    trace_out ();

    HTRY

    pyExternalComponentsLocator->_object = externalComponents.getLocator ();

    HCATCH

    return ( (PyObject*)pyExternalComponentsLocator );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_setName ()"

  static PyObject* PyNet_setName ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_setName()" << endl;

    HTRY

    METHOD_HEAD ( "Net.setName()" )
    
    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.setName", args, NET_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    net->setName ( *PYNAME_O(arg0) );

    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_setGlobal ()"

  static PyObject* PyNet_setGlobal ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_setGlobal()" << endl;

    HTRY

    METHOD_HEAD ( "Net.setGlobal()" )
    
    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.setGlobal", args, INT_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    net->setGlobal ( PyInt_AsLong(arg0) != 0 );

    HCATCH

    Py_RETURN_NONE;
  }


  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_setExternal ()"

  static PyObject* PyNet_setExternal ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_setExternal()" << endl;

    HTRY

    METHOD_HEAD ( "Net.setExternal()" )
    
    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.setExternal", args, INT_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    net->setExternal ( PyInt_AsLong(arg0) != 0 );

    HCATCH

    Py_RETURN_NONE;
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_setType ()"

  static PyObject* PyNet_setType ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_setType()" << endl;

    HTRY

    METHOD_HEAD ( "Net.setType()" )
    
    PyObject* arg0;
    if (!ParseOneArg("Net.setType", args, INT_ARG, (PyObject**)&arg0)) {
        PyErr_SetString(ConstructorError, "wrong parameter for NetType.");
        return NULL;
    }
    
    net->setType(PyInt_AsType(arg0));

    HCATCH
    
    Py_RETURN_NONE;
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_setDirection ()"

  static PyObject* PyNet_setDirection ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_setDirection()" << endl;

    HTRY

    METHOD_HEAD ( "Net.setDirection()" )

    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.setDirection", args, INT_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    net->setDirection ( PyInt_AsDirection(arg0) );

    HCATCH

    Py_RETURN_NONE;
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_setPosition ()"

  static PyObject* PyNet_setPosition ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_setPosition()" << endl;

    HTRY

    METHOD_HEAD ( "Net.setPosition()" )

    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.setPosition", args, POINT_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    net->setPosition ( *PYPOINT_O(arg0) );

    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_merge ()"

  static PyObject* PyNet_merge ( PyNet *self, PyObject* args ) {
    trace << "PyNet_merge()" << endl;

    HTRY

    METHOD_HEAD ( "Net.merge()" )

    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.merge", args, NET_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    net->merge ( PYNET_O(arg0) );

    HCATCH
    
    Py_RETURN_NONE;
  }






  // ---------------------------------------------------------------
  // PyNet Attribute Method table.

  PyMethodDef PyNet_Methods[] =
    { { "getName"              , (PyCFunction)PyNet_getName                  , METH_NOARGS , "Returns the net name." }
    , { "getType"              , (PyCFunction)PyNet_getType                  , METH_NOARGS
                               , "Returns the signal type (by default set to UNDEFINED)." }
    , { "getDirection"         , (PyCFunction)PyNet_getDirection             , METH_NOARGS
                               , "Returns the signal direction (by default set to UNDEFINED)." }
    , { "getX"                 , (PyCFunction)PyNet_getX                     , METH_NOARGS , "Returns net abscissa." }
    , { "getY"                 , (PyCFunction)PyNet_getY                     , METH_NOARGS , "Returns net ordinate." }
    , { "getExternalComponents", (PyCFunction)PyNet_getExternalComponentsLocator , METH_NOARGS , "Returns the collection of net's external components. (only for an external net)" }
    , { "getPlugsLocator"      , (PyCFunction)PyNet_getPlugsLocator          , METH_NOARGS , "Returns the collection of net's plugs." }
    , { "getPinsLocator"        , (PyCFunction)PyNet_getPinsLocator            , METH_NOARGS , "Returns the collection of net's pins." }
    , { "getSegmentsLocator"    , (PyCFunction)PyNet_getSegmentsLocator        , METH_NOARGS , "Returns the collection of net's segments." }
    , { "isGlobal"             , (PyCFunction)PyNet_isGlobal                 , METH_NOARGS, "return true if the net is global" }
    , { "isExternal"           , (PyCFunction)PyNet_isExternal               , METH_NOARGS, "return true if the the net is external." }
    , { "isLogical"            , (PyCFunction)PyNet_isLogical                , METH_NOARGS, "return true if the net is logical ." }
    , { "isClock"              , (PyCFunction)PyNet_isClock                  , METH_NOARGS, "return true if the net is a clock" }
    , { "isPower"              , (PyCFunction)PyNet_isPower                  , METH_NOARGS, "return true if the net is a power" }
    , { "isGround"             , (PyCFunction)PyNet_isGround                 , METH_NOARGS, "return true if the net is a ground" }
    , { "isSupply"             , (PyCFunction)PyNet_isSupply                 , METH_NOARGS, "return true if the net is a supply" }
    , { "isBound"              , (PyCFunction)PyNet_IsPyBound                , METH_NOARGS, "return true if the net is bounded to the hurricane net" }
    , { "setName"              , (PyCFunction)PyNet_setName                  , METH_VARARGS, "Allows to change net name." }
    , { "setGlobal"            , (PyCFunction)PyNet_setGlobal                , METH_VARARGS, "set the net global." }
    , { "setExternal"          , (PyCFunction)PyNet_setExternal              , METH_VARARGS, "set the net external." }
    , { "setType"              , (PyCFunction)PyNet_setType                  , METH_VARARGS, "set the type of the net." }
    , { "setDirection"         , (PyCFunction)PyNet_setDirection             , METH_VARARGS, "set the direction of the net." }
    , { "setPosition"          , (PyCFunction)PyNet_setPosition              , METH_VARARGS, "set the X,Y location of the net." }
    , { "merge"                , (PyCFunction)PyNet_merge                    , METH_VARARGS
                               , "Merges the net <net> to the net <this> which keeps its characteristics (arity, global, external and direction)." }
    , { "destroy"              , (PyCFunction)PyNet_destroy                  , METH_NOARGS
                               , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                  "PyNet" Object Methods                     |
  // x-------------------------------------------------------------x

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_new ()"

  static PyObject* PyNet_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyNet_new()" << endl;

    Net*      net = NULL;
    PyObject* arg0;
    PyObject* arg1;
    
    if (!ParseTwoArg("Net.new", args, CELL_NAME_ARG, &arg0, &arg1)) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Net constructor." );
        return NULL;
    }

    HTRY
    net = Net::create ( PYCELL_O(arg0), *PYNAME_O(arg1) );
    HCATCH

    return PyNet_Link ( net );
  }

  DBoDeleteMethod(Net)
  PyTypeObjectLinkPyType(Net)
  PyTypeObjectConstructor(Net)


#else  // End of Python Module Code Part.


// x=================================================================x
// |                "PyNet" Shared Library Code Part                 |
// x=================================================================x




  // Link/Creation Method.
  DBoLinkCreateMethod(Net)




  // ---------------------------------------------------------------
  // PyNet Object Definitions.

  PyTypeObjectDefinitions(Net)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
