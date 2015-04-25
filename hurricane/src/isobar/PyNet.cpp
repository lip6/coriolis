// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                     Sophie BELLOEIL              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyNet.cpp"                                   |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyNetType.h"
#include "hurricane/isobar/PyNetDirection.h"
#include "hurricane/isobar/PyCell.h" 
#include "hurricane/isobar/PyPoint.h" 
#include "hurricane/isobar/PyPlugCollection.h" 
#include "hurricane/isobar/PySegmentCollection.h" 
#include "hurricane/isobar/PyComponentCollection.h" 
#include "hurricane/isobar/PyPinCollection.h" 
#include "hurricane/Cell.h"
#include "hurricane/NetExternalComponents.h"
using namespace Hurricane;


namespace  Isobar {


extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Net,net,function)

#define  LOAD_CONSTANT(CONSTANT_VALUE,CONSTANT_NAME)             \
 constant = PyInt_FromLong ( (long)CONSTANT_VALUE );             \
 PyDict_SetItemString ( dictionnary, CONSTANT_NAME, constant );  \
 Py_DECREF ( constant );


// x=================================================================x
// |                 "PyNet" Python Module Code Part                 |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyNet" Local Functions                     |
  // x-------------------------------------------------------------x

  
  // ---------------------------------------------------------------
  // Local Function  :  "PyInt_AsType ()"
  
  static Net::Type  PyInt_AsType ( PyObject* object ) {
    switch ( PyAny_AsLong(object) ) {
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
    switch ( PyAny_AsLong(object) ) {
      case Net::Direction::UNDEFINED      : return ( Net::Direction(Net::Direction::UNDEFINED) );
      case Net::Direction::IN             : return ( Net::Direction(Net::Direction::IN) );
      case Net::Direction::OUT            : return ( Net::Direction(Net::Direction::OUT) );
      case Net::Direction::INOUT          : return ( Net::Direction(Net::Direction::INOUT) );
      case Net::Direction::TRISTATE       : return ( Net::Direction(Net::Direction::TRISTATE) );
    }

    return ( Net::Direction(Net::Direction::UNDEFINED) );
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

  GetNameMethod(Net, net)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_create ()"

  static PyObject* PyNet_create ( PyObject*, PyObject *args ) {
    trace << "PyNet_create()" << endl;

    char* name = NULL;
    PyCell* pyCell = NULL;
    Net* net = NULL;
    
    HTRY
    if (PyArg_ParseTuple(args,"O!s:Net.create", &PyTypeCell, &pyCell, &name)) {
        net = Net::create(PYCELL_O(pyCell), Name(name));
    } else {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Net constructor." );
        return NULL;
    }
    HCATCH

    return PyNet_Link(net);
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getType ()"

  static PyObject* PyNet_getType ( PyNet *self )
  {
    trace << "PyNet_getType ()" << endl;
    
    METHOD_HEAD ( "Net.getType()" )

    return ( (PyObject*)PyLong_FromLong((long)net->getType().getCode()) );
  }

  

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getDirection ()"

  static PyObject* PyNet_getDirection ( PyNet *self )
  {
    trace << "PyNet_getDirection ()" << endl;
   
    METHOD_HEAD ( "Net.getDirection()" )

    return ( (PyObject*)PyLong_FromLong((long)net->getDirection().getCode()) );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getPlugs()"

  static PyObject* PyNet_getPlugs(PyNet *self) {
    trace << "PyNet_getPlugs()" << endl;

    METHOD_HEAD("Net.getPlugs()")
        
    PyPlugCollection* pyPlugCollection = NULL;

    HTRY
    Plugs* plugs = new Plugs(net->getPlugs());

    pyPlugCollection = PyObject_NEW(PyPlugCollection, &PyTypePlugCollection);
    if (pyPlugCollection == NULL) { 
        return NULL;
    }

    pyPlugCollection->_object = plugs;
    HCATCH
    
    return (PyObject*)pyPlugCollection;
  }

    
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getSegments()"

  static PyObject* PyNet_getSegments(PyNet *self) {
    trace << "PyNet_getSegments()" << endl;

    METHOD_HEAD ("Net.getSegments()")

    PySegmentCollection* pySegmentCollection = NULL;

    HTRY
    Segments* nets = new Segments(net->getSegments());

    pySegmentCollection = PyObject_NEW(PySegmentCollection, &PyTypeSegmentCollection);
    if (pySegmentCollection == NULL) { 
        return NULL;
    }

    pySegmentCollection->_object = nets;
    HCATCH
    
    return (PyObject*)pySegmentCollection;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getPins()"

  static PyObject* PyNet_getPins(PyNet *self) {
    trace << "PyNet_getPins()" << endl;

    METHOD_HEAD ("Net.getPins()")

    PyPinCollection* pyPinCollection = NULL;

    HTRY
    Pins* pins = new Pins(net->getPins());

    pyPinCollection = PyObject_NEW(PyPinCollection, &PyTypePinCollection);
    if (pyPinCollection == NULL) { 
        return NULL;
    }

    pyPinCollection->_object = pins;
    HCATCH
    
    return (PyObject*)pyPinCollection;
  }
  

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getComponents()"

  static PyObject* PyNet_getComponents(PyNet *self) {
    trace << "PyNet_getComponents()" << endl;

    METHOD_HEAD ( "Net.getComponents()" )

    PyComponentCollection* pyComponentCollection = NULL;

    HTRY
    Components* components = new Components(net->getComponents());

    pyComponentCollection = PyObject_NEW(PyComponentCollection, &PyTypeComponentCollection);
    if (pyComponentCollection == NULL) return NULL;

    pyComponentCollection->_object = components;
    HCATCH
    
    return (PyObject*)pyComponentCollection;
  }
  

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_getExternalComponents()"

  static PyObject* PyNet_getExternalComponents(PyNet *self) {
    trace << "PyNet_getExternalComponents()" << endl;

    METHOD_HEAD ( "Net.getExternalcomponents()" )

    PyComponentCollection* pyComponentCollection = NULL;

    HTRY
    Components* components = new Components(NetExternalComponents::get(net));

    pyComponentCollection = PyObject_NEW(PyComponentCollection, &PyTypeComponentCollection);
    if (pyComponentCollection == NULL) return NULL;

    pyComponentCollection->_object = components;
    HCATCH
    
    return (PyObject*)pyComponentCollection;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_setName ()"

  SetNameMethod(Net, net)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_setGlobal ()"

  static PyObject* PyNet_setGlobal ( PyNet *self, PyObject* args ) {
    trace << "PyNet_setGlobal()" << endl;

    HTRY
    METHOD_HEAD ( "Net.setGlobal()" )
    PyObject* arg0;
    if (PyArg_ParseTuple(args,"O:Net.setGlobal", &arg0) && PyBool_Check(arg0)) {
      PyObject_IsTrue(arg0)?net->setGlobal(true):net->setGlobal(false);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Net.setGlobal." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_setExternal ()"

  static PyObject* PyNet_setExternal ( PyNet *self, PyObject* args ) {
    trace << "PyNet_setExternal()" << endl;

    HTRY
    METHOD_HEAD ( "Net.setExternal()" )
    PyObject* arg0;
    if (PyArg_ParseTuple(args,"O:Net.setExternal", &arg0) && PyBool_Check(arg0)) {
      PyObject_IsTrue(arg0)?net->setExternal(true):net->setExternal(false);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Net.setExternal." );
      return NULL;
    }
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
  // Attribute Method  :  "PyNet_addAlias ()"

  static PyObject* PyNet_addAlias ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_addAlias()" << endl;

    HTRY
      METHOD_HEAD ( "Net.addAlias()" )

      char* name;
      if (PyArg_ParseTuple(args,"s:Net.addAlias",args,name)) {
        if (net->addAlias(Name(name))) Py_RETURN_TRUE;
      } else {
        PyErr_SetString( ConstructorError, "Bad parameters given to Net.addAlias()." );
        return NULL;
      }

    HCATCH

    Py_RETURN_FALSE;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_removeAlias ()"

  static PyObject* PyNet_removeAlias ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_removeAlias()" << endl;

    HTRY
      METHOD_HEAD ( "Net.removeAlias()" )

      char* name;
      if (PyArg_ParseTuple(args,"s:Net.removeAlias",args,name)) {
        if (net->removeAlias(Name(name))) Py_RETURN_TRUE;
      } else {
        PyErr_SetString( ConstructorError, "Bad parameters given to Net.removeAlias()." );
        return NULL;
      }

    HCATCH

    Py_RETURN_FALSE;
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
      PyNet* pyNetToMerge;
      METHOD_HEAD ( "Net.merge()" )
      if (PyArg_ParseTuple(args, "O!:Net.merge", &PyTypeNet, &pyNetToMerge)) {
        net->merge(PYNET_O(pyNetToMerge));
      } else {
        PyErr_SetString (ConstructorError, "invalid number of parameters for Net.merge.");
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_merge ()"

  static PyObject* PyNet_getClone ( PyNet *self, PyObject* args ) {
    trace << "PyNet_getClone()" << endl;

    Net* cloneNet = NULL;
    HTRY
      PyCell* pyCloneCell;
      METHOD_HEAD ( "Net.getClone()" )
      if (PyArg_ParseTuple(args, "O!:Net.getClone", &PyTypeCell, &pyCloneCell)) {
        cloneNet = net->getClone( PYCELL_O(pyCloneCell) );
      } else {
        PyErr_SetString (ConstructorError, "Net.getClone(): invalid number/bad type of parameter(s).");
        return NULL;
      }
    HCATCH
    
    return PyNet_Link(cloneNet);
  }


  // ---------------------------------------------------------------
  // PyNet Attribute Method table.

  PyMethodDef PyNet_Methods[] =
    { { "create"               , (PyCFunction)PyNet_create                   , METH_VARARGS|METH_STATIC
                               , "Create a new Net." }
    , { "getName"              , (PyCFunction)PyNet_getName                  , METH_NOARGS , "Returns the net name." }
    , { "getType"              , (PyCFunction)PyNet_getType                  , METH_NOARGS , "Returns the signal type (by default set to UNDEFINED)." }
    , { "getDirection"         , (PyCFunction)PyNet_getDirection             , METH_NOARGS , "Returns the signal direction (by default set to UNDEFINED)." }
    , { "getX"                 , (PyCFunction)PyNet_getX                     , METH_NOARGS , "Returns net abscissa." }
    , { "getY"                 , (PyCFunction)PyNet_getY                     , METH_NOARGS , "Returns net ordinate." }
    , { "getComponents"        , (PyCFunction)PyNet_getComponents            , METH_NOARGS , "Returns the collection of net's external components. (only for an external net)" }
    , { "getExternalComponents", (PyCFunction)PyNet_getExternalComponents    , METH_NOARGS , "Returns the collection of net's external components. (only for an external net)" }
    , { "getPlugs"             , (PyCFunction)PyNet_getPlugs                 , METH_NOARGS , "Returns the collection of net's plugs." }
    , { "getPins"              , (PyCFunction)PyNet_getPins                  , METH_NOARGS , "Returns the collection of net's pins." }
    , { "getSegments"          , (PyCFunction)PyNet_getSegments              , METH_NOARGS , "Returns the collection of net's segments." }
    , { "isGlobal"             , (PyCFunction)PyNet_isGlobal                 , METH_NOARGS , "return true if the net is global" }
    , { "isExternal"           , (PyCFunction)PyNet_isExternal               , METH_NOARGS , "return true if the the net is external." }
    , { "isLogical"            , (PyCFunction)PyNet_isLogical                , METH_NOARGS , "return true if the net is logical ." }
    , { "isClock"              , (PyCFunction)PyNet_isClock                  , METH_NOARGS , "return true if the net is a clock" }
    , { "isPower"              , (PyCFunction)PyNet_isPower                  , METH_NOARGS , "return true if the net is a power" }
    , { "isGround"             , (PyCFunction)PyNet_isGround                 , METH_NOARGS , "return true if the net is a ground" }
    , { "isSupply"             , (PyCFunction)PyNet_isSupply                 , METH_NOARGS , "return true if the net is a supply" }
    , { "isBound"              , (PyCFunction)PyNet_IsPyBound                , METH_NOARGS , "return true if the net is bounded to the hurricane net" }
    , { "setName"              , (PyCFunction)PyNet_setName                  , METH_VARARGS, "Allows to change net name." }
    , { "setGlobal"            , (PyCFunction)PyNet_setGlobal                , METH_VARARGS, "set the net global." }
    , { "setExternal"          , (PyCFunction)PyNet_setExternal              , METH_VARARGS, "set the net external." }
    , { "setType"              , (PyCFunction)PyNet_setType                  , METH_VARARGS, "set the type of the net." }
    , { "setDirection"         , (PyCFunction)PyNet_setDirection             , METH_VARARGS, "set the direction of the net." }
    , { "setPosition"          , (PyCFunction)PyNet_setPosition              , METH_VARARGS, "set the X,Y location of the net." }
    , { "addAlias"             , (PyCFunction)PyNet_addAlias                 , METH_VARARGS, "Add an alias name to the net." }
    , { "removeAlias"          , (PyCFunction)PyNet_removeAlias              , METH_VARARGS, "Remove an alias name from the net." }
    , { "merge"                , (PyCFunction)PyNet_merge                    , METH_VARARGS, "Merges the net <net> to the net <this> which keeps its characteristics (arity, global, external and direction)." }
    , { "getClone"             , (PyCFunction)PyNet_getClone                 , METH_VARARGS, "Create a clone of this net into the cloned cell (components not cloneds)." }
    , { "destroy"              , (PyCFunction)PyNet_destroy                  , METH_NOARGS , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                  "PyNet" Object Methods                     |
  // x-------------------------------------------------------------x

  DBoDeleteMethod(Net)
  PyTypeObjectLinkPyType(Net)


#else  // End of Python Module Code Part.


// x=================================================================x
// |                "PyNet" Shared Library Code Part                 |
// x=================================================================x


  DBoLinkCreateMethod(Net)
  PyTypeInheritedObjectDefinitions(Net, Entity)


  extern  void  PyNet_postModuleInit ()
  {
    PyNetType_postModuleInit();
    PyNetDirection_postModuleInit();

    PyDict_SetItemString( PyTypeNet.tp_dict, "Type"     , (PyObject*)&PyTypeNetType );
    PyDict_SetItemString( PyTypeNet.tp_dict, "Direction", (PyObject*)&PyTypeNetDirection );
  }


# endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
