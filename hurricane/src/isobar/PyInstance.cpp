// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyInstance.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyTransformation.h"
#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/isobar/PyPlacementStatus.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyPlug.h"
#include "hurricane/isobar/PyPlugCollection.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#undef   ACCESS_OBJECT
#undef   ACCESS_CLASS
#define  ACCESS_OBJECT           _baseObject._object
#define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Instance,instance,function)

#define  LOAD_CONSTANT(CONSTANT_VALUE,CONSTANT_NAME)              \
  constant = PyInt_FromLong ( (long)CONSTANT_VALUE );             \
  PyDict_SetItemString ( dictionnary, CONSTANT_NAME, constant );  \
  Py_DECREF ( constant );


// +=================================================================+
// |              "PyInstance" Python Module Code Part               |
// +=================================================================+

#if defined(__PYTHON_MODULE__)
      

  static Instance::PlacementStatus  PyInt_AsPlacementStatus ( PyObject* object )
  {
    switch ( PyAny_AsLong(object) ) {
      case Instance::PlacementStatus::UNPLACED : return ( Instance::PlacementStatus(Instance::PlacementStatus::UNPLACED) );
      case Instance::PlacementStatus::PLACED   : return ( Instance::PlacementStatus(Instance::PlacementStatus::PLACED) );
      case Instance::PlacementStatus::FIXED    : return ( Instance::PlacementStatus(Instance::PlacementStatus::FIXED) );
    }
      
    return ( Instance::PlacementStatus(Instance::PlacementStatus::UNPLACED) );
  }


  // Standart Accessors (Attributes).

  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyInstance_destroy,PyInstance)

  GetNameMethod(Instance, instance)


  static PyObject* PyInstance_create ( PyObject*, PyObject *args )
  {
    trace << "PyInstance_create ()" << endl;
    
    Instance* instance = NULL;
    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;

    HTRY
    __cs.init ("Instance.create");
    if ( ! PyArg_ParseTuple(args,"O&O&O&|O&:Instance.new"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Instance constructor." );
        return NULL;
    }

    if      ( __cs.getObjectIds() == ":ent:string:ent") {
        instance = Instance::create(
                PYCELL_O(arg0),
                Name(PyString_AsString(arg1)),
                PYCELL_O(arg2) );
    } else if ( __cs.getObjectIds() == ":ent:string:ent:transfo") {
        instance = Instance::create(
                PYCELL_O(arg0),
                Name(PyString_AsString(arg1)),
                PYCELL_O(arg2),
                *PYTRANSFORMATION_O(arg3),
                Instance::PlacementStatus::PLACED);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Instance constructor." );
      return NULL;
    }
    HCATCH

    return PyInstance_Link ( instance );
  }


  static PyObject* PyInstance_getMasterCell ( PyInstance *self )
  {
    trace << "PyInstance_getMasterCell ()" << endl;
    
    Cell* cell = NULL;

    HTRY
    METHOD_HEAD ( "Instance.getMasterCell()" )
    cell = instance->getMasterCell ();
    HCATCH

    return PyCell_Link ( cell );
  }


  static PyObject* PyInstance_getPlacementStatus ( PyInstance *self )
  {
    trace << "PyInstance_getPlacementStatus ()" << endl;
    
    METHOD_HEAD ( "Instance.getPlacementStatus()" );

    PyObject* pyObject = NULL;
    HTRY
    pyObject = (PyObject*)PyLong_FromLong((long)instance->getPlacementStatus().getCode());
    HCATCH

    return pyObject;
  }


  static PyObject* PyInstance_setPlacementStatus ( PyInstance *self, PyObject* args )
  {
    trace << "PyInstance_setPlacementStatus()" << endl;
    METHOD_HEAD ( "Instance.setPlacementStatus()" )
      
    HTRY
    PyObject* arg0;
    if ( ! ParseOneArg ( "Instance.setPlacementStatus()", args, INT_ARG, (PyObject**)&arg0 ) ) return ( NULL );
      
    instance->setPlacementStatus ( PyInt_AsPlacementStatus(arg0) );
    HCATCH
      
    Py_RETURN_NONE;
  }


  static PyObject* PyInstance_getTransformation ( PyInstance *self )
  {
    trace << "PyInstance_getTransformation ()" << endl;
    METHOD_HEAD ( "Instance.getTransformation()" );

    PyTransformation* resultPyTransf = PyObject_NEW ( PyTransformation, &PyTypeTransformation );
    if ( resultPyTransf == NULL ) { return NULL; }

    HTRY
    resultPyTransf->_object = new Transformation ( instance->getTransformation () );
    HCATCH

    return ( (PyObject*)resultPyTransf );
  }


  static PyObject* PyInstance_getPlug ( PyInstance *self, PyObject* args )
  {
    trace << "PyInstance_getPlug ()" << endl;

    Plug* plug = NULL;
    
    HTRY
    METHOD_HEAD ( "Instance.getPlug()" )

    PyNet* masterNet;
    if ( ! ParseOneArg ( "Instance.getPlug", args, NET_ARG, (PyObject**)&masterNet ) ) return ( NULL );
 
    plug = instance->getPlug( PYNET_O(masterNet) );
    HCATCH

    return PyPlug_Link ( plug );
  }


  static PyObject* PyInstance_getPlugs(PyInstance *self )
  {
    trace << "PyInstance_getPlugs()" << endl;

    METHOD_HEAD ( "Instance.getPlugs()" )

    PyPlugCollection* pyPlugCollection = NULL;

    HTRY
    Plugs* plugs = new Plugs(instance->getPlugs());

    pyPlugCollection = PyObject_NEW(PyPlugCollection, &PyTypePlugCollection);
    if (pyPlugCollection == NULL) { 
        return NULL;
    }

    pyPlugCollection->_object = plugs;
    HCATCH
    
    return (PyObject*)pyPlugCollection;
  }


  static PyObject* PyInstance_getConnectedPlugs(PyInstance *self)
  {
    trace << "PyInstance_getConnectedPlugs ()" << endl;

    METHOD_HEAD ( "Instance.getConnectedPlugs()")

    PyPlugCollection* pyPlugCollection = NULL;

    HTRY
    Plugs* plugs = new Plugs(instance->getConnectedPlugs());

    pyPlugCollection = PyObject_NEW(PyPlugCollection, &PyTypePlugCollection);
    if (pyPlugCollection == NULL) { 
        return NULL;
    }

    pyPlugCollection->_object = plugs;
    HCATCH
    
    return (PyObject*)pyPlugCollection;
  }


  static PyObject* PyInstance_getUnconnectedPlugs(PyInstance *self)
  {
    trace << "PyInstance_getUnconnectedPlugs ()" << endl;

    METHOD_HEAD ( "Instance.getUnconnectedPlugs()")

    PyPlugCollection* pyPlugCollection = NULL;

    HTRY
    Plugs* plugs = new Plugs(instance->getUnconnectedPlugs());

    pyPlugCollection = PyObject_NEW(PyPlugCollection, &PyTypePlugCollection);
    if (pyPlugCollection == NULL) { 
        return NULL;
    }

    pyPlugCollection->_object = plugs;
    HCATCH
    
    return (PyObject*)pyPlugCollection;
  }


  static PyObject* PyInstance_getAbutmentBox ( PyInstance *self )
  {
    trace << "PyInstance_getAbutmentBox ()" << endl;
    METHOD_HEAD ( "Instance.getAbutmentBox()" )

    PyBox* pyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (pyBox == NULL) { return NULL; }

    HTRY
    pyBox->_object = new Box ( instance->getAbutmentBox() );
    HCATCH

    return ( (PyObject*)pyBox );
  }
  

  static PyObject* PyInstance_uniquify ( PyInstance *self )
  {
    trace << "PyInstance_uniquify ()" << endl;

    HTRY
      METHOD_HEAD ( "Instance.uniquify()" )
      instance->uniquify();
    HCATCH
    Py_RETURN_NONE;
  }



  static PyObject* PyInstance_getClone ( PyInstance *self, PyObject* args )
  {
    trace << "PyInstance_getClone ()" << endl;

    Instance* cloneInstance = NULL;
    HTRY
      METHOD_HEAD( "Instance.getClone()" )
      PyCell* pyCloneCell;
      if (PyArg_ParseTuple(args, "O!:Instance.getClone", &PyTypeCell, &pyCloneCell)) {
        cloneInstance = instance->getClone( PYCELL_O(pyCloneCell) );
      } else {
        PyErr_SetString (ConstructorError, "Instance.getClone(): invalid number/bad type of parameter(s).");
        return NULL;
      }
    HCATCH

    return PyInstance_Link( cloneInstance );
  }


  SetNameMethod(Instance, instance)


  static PyObject* PyInstance_setTransformation ( PyInstance *self, PyObject* args )
  {
    trace << "PyInstance_setTransformation()" << endl;
    METHOD_HEAD ( "Instance.setTransformation()" )

    HTRY
    PyTransformation* transformation;
    if ( ! ParseOneArg ( "Instance.setTransformation", args, TRANS_ARG, (PyObject**)&transformation ) ) return ( NULL );

    instance->setTransformation ( *PYTRANSFORMATION_O(transformation) );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyInstance_setMasterCell ( PyInstance *self, PyObject* args )
  {
    trace << "Instance.setMasterCell()" << endl;
    METHOD_HEAD ( "Instance.setMasterCell()" )

    HTRY
    PyCell* masterCell;
    if ( ! ParseOneArg ( "Instance.setMasterCell", args, CELL_ARG, (PyObject**)&masterCell ) ) return ( NULL );

    instance->setMasterCell ( PYCELL_O(masterCell) );
    HCATCH

    Py_RETURN_NONE;
  }


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyInstance_isTerminal ,isTerminal ,PyInstance,Instance)
  DirectGetBoolAttribute(PyInstance_isLeaf     ,isLeaf     ,PyInstance,Instance)

  GetBoundStateAttribute(PyInstance_isPyBound              ,PyInstance,Instance)

  
  // ---------------------------------------------------------------
  // PyInstance Attribute Method table.

  PyMethodDef PyInstance_Methods[] =
    { { "create"                    , (PyCFunction)PyInstance_create                    , METH_VARARGS|METH_STATIC
                                    , "Create a new Instance." }
    , { "getName"                   , (PyCFunction)PyInstance_getName                   , METH_NOARGS , "Returns the instance name." }
    , { "getMasterCell"             , (PyCFunction)PyInstance_getMasterCell             , METH_NOARGS , "Returns the cell model referenced by the instance." }
    , { "getTransformation"         , (PyCFunction)PyInstance_getTransformation         , METH_NOARGS , "Returns the transformation associated to the instance." }
    , { "getPlacementStatus"        , (PyCFunction)PyInstance_getPlacementStatus        , METH_NOARGS , "Returns the placement status of the instance." }
    , { "getPlug"                   , (PyCFunction)PyInstance_getPlug                   , METH_VARARGS, "Returns the plug associated to the <masterNet> if it exists or else NULL (if the net is not external)." }
    , { "getPlugs"                  , (PyCFunction)PyInstance_getPlugs                  , METH_NOARGS , "Returns the collection of instance plugs." }
    , { "getConnectedPlugs"         , (PyCFunction)PyInstance_getConnectedPlugs         , METH_NOARGS , "Returns the collection of instance plugs which are effectively connected." }
    , { "getUnconnectedPlugs"       , (PyCFunction)PyInstance_getUnconnectedPlugs       , METH_NOARGS , "Returns the collection of instance plugs which are not connected." }
    , { "getAbutmentBox"            , (PyCFunction)PyInstance_getAbutmentBox            , METH_NOARGS , "Returns the abutment box of the instance, that is the abutment box of the master cell to which has been applied the instance transformation." }
    , { "isTerminal"                , (PyCFunction)PyInstance_isTerminal                , METH_NOARGS , "Returns true if the instance is a terminal instance." }
    , { "isLeaf"                    , (PyCFunction)PyInstance_isLeaf                    , METH_NOARGS , "Returns true if the instance is a leaf instance." }
    , { "isBound"                   , (PyCFunction)PyInstance_isPyBound                 , METH_NOARGS , "Returns true if the instance is bounded to the hurricane instance" }
    , { "setName"                   , (PyCFunction)PyInstance_setName                   , METH_VARARGS, "Allows to change the instance name." }
    , { "setTransformation"         , (PyCFunction)PyInstance_setTransformation         , METH_VARARGS, "Allows to modify the instance transformation." }
    , { "setPlacementStatus"        , (PyCFunction)PyInstance_setPlacementStatus        , METH_VARARGS, "Allows to modify the instance placement status." }
    , { "setMasterCell"             , (PyCFunction)PyInstance_setMasterCell             , METH_VARARGS, "Allows to change the cell referenced by this instance." }
    , { "uniquify"                  , (PyCFunction)PyInstance_uniquify                  , METH_NOARGS , "Uniquify the Instance (clone it's master Cell)." }
    , { "getClone"                  , (PyCFunction)PyInstance_getClone                  , METH_VARARGS, "Create a clone of this Instance into the cloned Cell (placement only)." }
    , { "destroy"                   , (PyCFunction)PyInstance_destroy                   , METH_NOARGS , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  DBoDeleteMethod(Instance)
  PyTypeObjectLinkPyType(Instance)


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyInstance" Shared Library Code Part               |
// +=================================================================+


  // Link/Creation Method.
  DBoLinkCreateMethod(Instance)


  PyTypeInheritedObjectDefinitions(Instance, Entity)


  extern  void  PyInstance_postModuleInit ()
  {
    PyPlacementStatus_postModuleInit();

    PyDict_SetItemString( PyTypeInstance.tp_dict, "PlacementStatus", (PyObject*)&PyTypePlacementStatus );
  }


#endif  // End of Shared Library Code Part.

}  // End of extern "C".

}  // End of Isobar namespace.
