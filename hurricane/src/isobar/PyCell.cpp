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
// |  C++ Module  :  "./PyCell.cpp"                                  |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/isobar/PyOccurrence.h"
#include "hurricane/isobar/ProxyProperty.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyNetCollection.h"
#include "hurricane/isobar/PyReferenceCollection.h"
#include "hurricane/isobar/PyInstanceCollection.h"
#include "hurricane/isobar/PyComponentCollection.h"
#include "hurricane/isobar/PyOccurrenceCollection.h"

namespace  Isobar {


using namespace Hurricane;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Cell,cell,function)


// x=================================================================x
// |                "PyCell" Python Module Code Part                 |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                "PyCell" Attribute Methods                   |
  // x-------------------------------------------------------------x
  

  // Standart Accessors (Attributes).


  // Standart Delete (Attribute).
  DBoDestroyAttribute(PyCell_destroy,PyCell)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_create ()"

  PyObject* PyCell_create ( PyObject*, PyObject *args ) {
    trace << "PyCell_create()" << endl;

    char* name = NULL;
    PyLibrary* pyLibrary = NULL;
    Cell* cell = NULL;

    HTRY
    if (PyArg_ParseTuple(args,"O!s:Cell.create", &PyTypeLibrary, &pyLibrary, &name)) {
        cell = Cell::create(PYLIBRARY_O(pyLibrary), Name(name));
    } else {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Cell constructor.");
        return NULL;
    }
    HCATCH

    return PyCell_Link(cell);
  }

  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getLibrary ()"

  static PyObject* PyCell_getLibrary ( PyCell *self ) {
    trace << "PyCell_getLibrary ()" << endl;

    Library* library = NULL;

    HTRY
    METHOD_HEAD ( "Cell.getLibrary()" )
    library = cell->getLibrary ();
    HCATCH
    
    return PyLibrary_Link(library);
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getName ()"

  GetNameMethod(Cell, cell)

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getInstance ()"

  static PyObject* PyCell_getInstance ( PyCell *self, PyObject* args ) {
    trace << "PyCell_getInstance ()" << endl;
    METHOD_HEAD("Cell.getInstance()")

    Instance* instance = NULL;
    
    HTRY
    char* name;
    if (PyArg_ParseTuple(args,"s:Cell.getInstance", &name)) {
        instance = cell->getInstance(Name(name));
    } else {
        PyErr_SetString(ConstructorError, "invalid number of parameters for Cell.getInstance." );
        return NULL;
    }
    HCATCH
    
    return PyInstance_Link(instance);
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getInstances()"

  static PyObject* PyCell_getInstances( PyCell *self ) {
    trace << "PyCell_getInstances()" << endl;

    METHOD_HEAD ( "Cell.getInstances()" )

    PyInstanceCollection* pyInstanceCollection = NULL;

    HTRY
    Instances* instances = new Instances(cell->getInstances());

    pyInstanceCollection = PyObject_NEW(PyInstanceCollection, &PyTypeInstanceCollection);
    if (pyInstanceCollection == NULL) { 
        return NULL;
    }

    pyInstanceCollection->_object = instances;
    HCATCH
    
    return (PyObject*)pyInstanceCollection;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getInstancesUnder()"

  static PyObject* PyCell_getInstancesUnder(PyCell *self, PyObject* args) {
    trace << "PyCell_getInstancesUnder()" << endl;

    METHOD_HEAD("Cell.getInstancesUnder()")

    PyBox* pyBox;
    if (!PyArg_ParseTuple(args,"O!:Cell.getInstancesUnder", &PyTypeBox, &pyBox)) {
        return NULL;
    }

    PyInstanceCollection* pyInstanceCollection = NULL;

    HTRY
    Instances* instances = new Instances(cell->getInstancesUnder(*PYBOX_O(pyBox)));

    pyInstanceCollection = PyObject_NEW(PyInstanceCollection, &PyTypeInstanceCollection);
    if (pyInstanceCollection == NULL) { 
        return NULL;
    }

    pyInstanceCollection->_object = instances;
    HCATCH
    
    return (PyObject*)pyInstanceCollection;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getSlaveInstances()"

  static PyObject* PyCell_getSlaveInstances(PyCell *self) {
    trace << "PyCell_getSlaveInstances()" << endl;

    METHOD_HEAD("Cell.getSlaveInstances()")

    PyInstanceCollection* pyInstanceCollection = NULL;

    HTRY
    Instances* instances = new Instances(cell->getSlaveInstances());

    pyInstanceCollection = PyObject_NEW(PyInstanceCollection, &PyTypeInstanceCollection);
    if (pyInstanceCollection == NULL) { 
        return NULL;
    }

    pyInstanceCollection->_object = instances;
    HCATCH
    
    return (PyObject*)pyInstanceCollection;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getComponents()"

  static PyObject* PyCell_getComponents(PyCell *self) {
    trace << "PyCell_getComponents()" << endl;

    METHOD_HEAD("Cell.getComponents()")

    PyComponentCollection* pyComponentCollection = NULL;

    HTRY
    Components* components = new Components(cell->getComponents());

    pyComponentCollection = PyObject_NEW(PyComponentCollection, &PyTypeComponentCollection);
    if (pyComponentCollection == NULL) { 
        return NULL;
    }

    pyComponentCollection->_object = components;
    HCATCH
    
    return (PyObject*)pyComponentCollection;
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getComponentsUnder()"

  static PyObject* PyCell_getComponentsUnder(PyCell *self, PyObject* args) {
    trace << "PyCell_getComponentsUnder()" << endl;

    METHOD_HEAD("Cell.getComponentsUnder()")

    PyBox* pyBox;
    if (!PyArg_ParseTuple(args,"O!:Cell.getInstancesUnder", &PyTypeBox, &pyBox)) {
        return NULL;
    }
    PyComponentCollection* pyComponentCollection = NULL;

    HTRY
    Components* components = new Components(cell->getComponentsUnder(*PYBOX_O(pyBox)));

    pyComponentCollection = PyObject_NEW(PyComponentCollection, &PyTypeComponentCollection);
    if (pyComponentCollection == NULL) { 
        return NULL;
    }

    pyComponentCollection->_object = components;
    HCATCH
    
    return (PyObject*)pyComponentCollection;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getOccurrences()"

  static PyObject* PyCell_getOccurrences(PyCell *self) {
    trace << "PyCell_getOccurrences()" << endl;

    METHOD_HEAD("Cell.getOccurrences()")

    PyOccurrenceCollection* pyOccurrenceCollection = NULL;

    HTRY
    Occurrences* occurrences = new Occurrences(cell->getOccurrences());

    pyOccurrenceCollection = PyObject_NEW(PyOccurrenceCollection, &PyTypeOccurrenceCollection);
    if (pyOccurrenceCollection == NULL) { 
        return NULL;
    }

    pyOccurrenceCollection->_object = occurrences;
    HCATCH
    
    return (PyObject*)pyOccurrenceCollection;
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getOccurrencesUnder()"

  static PyObject* PyCell_getOccurrencesUnder(PyCell *self, PyObject* args) {
    trace << "PyCell_getOccurrencesUnder()" << endl;

    METHOD_HEAD("Cell.getOccurrencesUnder()")

    PyBox* pyBox;
    if (!PyArg_ParseTuple(args,"O!:Cell.getInstancesUnder", &PyTypeBox, &pyBox)) {
        return NULL;
    }
    PyOccurrenceCollection* pyOccurrenceCollection = NULL;

    HTRY
    Occurrences* occurrences = new Occurrences(cell->getOccurrencesUnder(*PYBOX_O(pyBox)));

    pyOccurrenceCollection = PyObject_NEW(PyOccurrenceCollection, &PyTypeOccurrenceCollection);
    if (pyOccurrenceCollection == NULL) { 
        return NULL;
    }

    pyOccurrenceCollection->_object = occurrences;
    HCATCH
    
    return (PyObject*)pyOccurrenceCollection;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getLeafInstanceOccurrences()"

  static PyObject* PyCell_getLeafInstanceOccurrences(PyCell *self) {
    trace << "PyCell_getLeafInstanceOccurrences()" << endl;

    METHOD_HEAD ( "Cell.getLeafInstanceOccurrences()" )

    PyOccurrenceCollection* pyOccurrenceCollection = NULL;

    HTRY
    Occurrences* occurrences = new Occurrences(cell->getLeafInstanceOccurrences());

    pyOccurrenceCollection = PyObject_NEW(PyOccurrenceCollection, &PyTypeOccurrenceCollection);
    if (pyOccurrenceCollection == NULL) { 
        return NULL;
    }

    pyOccurrenceCollection->_object = occurrences;
    HCATCH
    
    return (PyObject*)pyOccurrenceCollection;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getLeafInstanceOccurrencesUnder()"

  static PyObject* PyCell_getLeafInstanceOccurrencesUnder(PyCell *self, PyObject* args) {
    trace << "PyCell_getLeafInstanceOccurrencesUnder()" << endl;

    METHOD_HEAD ( "Cell.getLeafInstanceOccurrencesUnder()" )

    PyBox* pyBox;
    if (!PyArg_ParseTuple(args,"O!:Cell.getInstancesUnder", &PyTypeBox, &pyBox)) {
        return NULL;
    }

    PyOccurrenceCollection* pyOccurrenceCollection = NULL;

    HTRY
    Occurrences* occurrences = new Occurrences(cell->getLeafInstanceOccurrencesUnder(*PYBOX_O(pyBox)));

    pyOccurrenceCollection = PyObject_NEW(PyOccurrenceCollection, &PyTypeOccurrenceCollection);
    if (pyOccurrenceCollection == NULL) { 
        return NULL;
    }

    pyOccurrenceCollection->_object = occurrences;
    HCATCH
    
    return (PyObject*)pyOccurrenceCollection;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getReferences()"

  static PyObject* PyCell_getReferences(PyCell *self) {
    trace << "PyCell_getReferences()" << endl;

    METHOD_HEAD("Cell.getReferences()")

    PyReferenceCollection* pyReferenceCollection = NULL;

    HTRY
    References* references = new References(cell->getReferences());

    pyReferenceCollection = PyObject_NEW(PyReferenceCollection, &PyTypeReferenceCollection);
    if (pyReferenceCollection == NULL) { 
        return NULL;
    }

    pyReferenceCollection->_object = references;
    HCATCH
    
    return (PyObject*)pyReferenceCollection;
  }

  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getHyperNets()"

  static PyObject* PyCell_getHyperNets(PyCell *self) {
    trace << "PyCell_getHyperNets()" << endl;

    METHOD_HEAD ( "Cell.getHyperNets()" )

    PyOccurrenceCollection* pyOccurrenceCollection = NULL;

    HTRY
    Occurrences* occurrences = new Occurrences(cell->getHyperNetRootNetOccurrences());

    pyOccurrenceCollection = PyObject_NEW(PyOccurrenceCollection, &PyTypeOccurrenceCollection);
    if (pyOccurrenceCollection == NULL) { 
        return NULL;
    }

    pyOccurrenceCollection->_object = occurrences;
    HCATCH
    
    return ( (PyObject*)pyOccurrenceCollection);
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getNet ()"

  static PyObject* PyCell_getNet(PyCell *self, PyObject* args) {
      trace << "PyCell_getNet ()" << endl;
      METHOD_HEAD ( "Cell.getNet()" )

      Net* net = NULL;

      HTRY
      char* name = NULL;
      if (!PyArg_ParseTuple(args,"s:Cell.getNet", &name)) {
          return NULL;
      } 
      net = cell->getNet(Name(name));
      HCATCH
      
      return PyNet_Link(net);
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getNets ()"

  static PyObject* PyCell_getNets ( PyCell *self ) {
    trace << "PyCell_getNets()" << endl;

    METHOD_HEAD("Cell.getNets()")

    PyNetCollection* pyNetCollection = NULL;

    HTRY
    Nets* nets = new Nets(cell->getNets());

    pyNetCollection = PyObject_NEW(PyNetCollection, &PyTypeNetCollection);
    if (pyNetCollection == NULL) { 
        return NULL;
    }

    pyNetCollection->_object = nets;
    HCATCH
    
    return ( (PyObject*)pyNetCollection);
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getExternalNets()"

  static PyObject* PyCell_getExternalNets(PyCell *self) {
    trace << "PyCell_getExternalNets()" << endl;

    METHOD_HEAD("Cell.getExternalNets()")

    PyNetCollection* pyNetCollection = NULL;

    HTRY
    Nets* nets = new Nets(cell->getExternalNets());

    pyNetCollection = PyObject_NEW(PyNetCollection, &PyTypeNetCollection);
    if (pyNetCollection == NULL) { 
        return NULL;
    }
    pyNetCollection->_object = nets;

    HCATCH

    return ((PyObject*)pyNetCollection);
  }  

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getClockNets()"

  static PyObject* PyCell_getClockNets(PyCell *self) {
    trace << "PyCell_getClockNets()" << endl;

    METHOD_HEAD("Cell.getClockNets")

    PyNetCollection* pyNetCollection = NULL;

    HTRY
    Nets* nets = new Nets(cell->getClockNets());

    pyNetCollection = PyObject_NEW(PyNetCollection, &PyTypeNetCollection);
    if (pyNetCollection == NULL) { 
        return NULL;
    }
    pyNetCollection->_object = nets;

    HCATCH

    return ((PyObject*)pyNetCollection);
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getSupplyNets()"

  static PyObject* PyCell_getSupplyNets(PyCell *self) {
    trace << "PyCell_getSupplyNets()" << endl;

    METHOD_HEAD ( "Cell.getSupplyNets()" )

    PyNetCollection* pyNetCollection = NULL;

    HTRY
    Nets* nets = new Nets(cell->getSupplyNets());

    pyNetCollection = PyObject_NEW(PyNetCollection, &PyTypeNetCollection);
    if (pyNetCollection == NULL) { 
        return NULL;
    }
    pyNetCollection->_object = nets;

    HCATCH

    return ((PyObject*)pyNetCollection);
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getPowerNets()"

  static PyObject* PyCell_getPowerNets(PyCell *self) {
    trace << "PyCell_getPowerNets()" << endl;

    METHOD_HEAD ( "Cell.getPowerNets()" )

    PyNetCollection* pyNetCollection = NULL;

    HTRY
    Nets* nets = new Nets(cell->getPowerNets());

    pyNetCollection = PyObject_NEW(PyNetCollection, &PyTypeNetCollection);
    if (pyNetCollection == NULL) { 
        return NULL;
    }
    pyNetCollection->_object = nets;

    HCATCH

    return ((PyObject*)pyNetCollection);
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getGroundNets()"

  static PyObject* PyCell_getGroundNets(PyCell *self) {
    trace << "PyCell_getGroundNets()" << endl;

    METHOD_HEAD ( "Cell.getGroundNets()" )

    PyNetCollection* pyNetCollection = NULL;

    HTRY
    Nets* nets = new Nets(cell->getGroundNets());

    pyNetCollection = PyObject_NEW(PyNetCollection, &PyTypeNetCollection);
    if (pyNetCollection == NULL) { 
        return NULL;
    }
    pyNetCollection->_object = nets;

    HCATCH

    return ((PyObject*)pyNetCollection);
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getAbutmentBox ()"

  static PyObject* PyCell_getAbutmentBox ( PyCell *self ) {
    trace << "PyCell_getAbutmentBox()" << endl;
    
    METHOD_HEAD ( "Cell.getAbutmentBox()" )

    PyBox* abutmentBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (abutmentBox == NULL) { return NULL; }

    HTRY
    abutmentBox->_object = new Box ( cell->getAbutmentBox() );
    HCATCH

    return ( (PyObject*)abutmentBox );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_setName ()"

  SetNameMethod(Cell, cell)

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_setAbutmentBox ()"

  static PyObject* PyCell_setAbutmentBox ( PyCell *self, PyObject* args ) {
    trace << "Cell.setAbutmentBox()" << endl;

    HTRY
    METHOD_HEAD ( "Cell.setAbutmentBox()" )

    PyBox* pyBox;
    if (!PyArg_ParseTuple(args,"O!:Cell.setAbutmentBox", &PyTypeBox, &pyBox)) {
        return NULL;
    }
    cell->setAbutmentBox ( *PYBOX_O(pyBox) );
    HCATCH

    Py_RETURN_NONE;
  }
  
  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_setTerminal ()"

  static PyObject* PyCell_setTerminal ( PyCell *self, PyObject* args ) {
    trace << "PyCell_setTerminal ()" << endl;

    HTRY
    METHOD_HEAD ( "Cell.setTerminal()" )
    PyObject* arg0;
    if (!PyArg_ParseTuple(args,"O:Cell.setTerminal", &arg0) && PyBool_Check(arg0)) {
      return NULL;
    }
    PyObject_IsTrue(arg0)?cell->setTerminal(true):cell->setTerminal(false);
    HCATCH
    Py_RETURN_NONE;
  }
  
  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_uniquify ()"

  static PyObject* PyCell_uniquify ( PyCell *self, PyObject* args ) {
    trace << "PyCell_uniquify ()" << endl;

    HTRY
      METHOD_HEAD ( "Cell.uniquify()" )
      unsigned int  depth;
      if (not PyArg_ParseTuple(args,"I:Cell.uniquify", &depth)) {
        PyErr_SetString(ConstructorError, "Cell.uniquify(): Invalid number/bad type of parameter.");
        return NULL;
      }
      cell->uniquify( depth );
    HCATCH
    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getClone ()"

  static PyObject* PyCell_getClone ( PyCell *self ) {
    trace << "PyCell_getClone ()" << endl;

    Cell* cloneCell = NULL;
    HTRY
      METHOD_HEAD( "Cell.getClone()" )
      cloneCell = cell->getClone();
    HCATCH

    return PyCell_Link( cloneCell );
  }


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyCell_isTerminal, isTerminal ,PyCell,Cell)
  DirectGetBoolAttribute(PyCell_isLeaf,         isLeaf ,PyCell,Cell)

  GetBoundStateAttribute(PyCell_isPyBound              ,PyCell,Cell)

  // ---------------------------------------------------------------
  // PyCell Attribute Method table.

  PyMethodDef PyCell_Methods[] =
    { { "create"              , (PyCFunction)PyCell_create               , METH_VARARGS|METH_STATIC
                              , "Create a new cell." }
    , { "getLibrary"          , (PyCFunction)PyCell_getLibrary           , METH_NOARGS , "Returns the library owning the cell." }
    , { "getName"             , (PyCFunction)PyCell_getName              , METH_NOARGS , "Returns the name of the cell." }
    , { "getInstance"         , (PyCFunction)PyCell_getInstance          , METH_VARARGS, "Returns the instance of name <name> if it exists, else NULL." }
    , { "getInstances"        , (PyCFunction)PyCell_getInstances         , METH_NOARGS , "Returns the locator of the collection of all instances called by the cell." } // getInstances
    , { "getInstancesUnder"   , (PyCFunction)PyCell_getInstancesUnder    , METH_VARARGS, "Returns the locator of the collection of all instances of the cell intersecting the given rectangular area." } // getInstancesUnder
    , { "getSlaveInstances"   , (PyCFunction)PyCell_getSlaveInstances    , METH_NOARGS , "Returns the locator of the collection of instances whose master is this cell." } // getSlaveInstances
    , { "getComponents"       , (PyCFunction)PyCell_getComponents        , METH_NOARGS , "Returns the collection of all components belonging to the cell." }
    , { "getComponentsUnder"  , (PyCFunction)PyCell_getComponentsUnder   , METH_VARARGS, "Returns the collection of all components belonging to this cell and intersecting the given rectangular area." }
    , { "getOccurrences"      , (PyCFunction)PyCell_getOccurrences       , METH_NOARGS , "Returns the collection of all occurrences belonging to the cell." }
    , { "getOccurrencesUnder" , (PyCFunction)PyCell_getOccurrencesUnder  , METH_VARARGS, "Returns the collection of all occurrences belonging to this cell and intersecting the given rectangular area." }
    , { "getLeafInstanceOccurrences"     , (PyCFunction)PyCell_getLeafInstanceOccurrences     , METH_NOARGS
      , "Returns the collection of all occurrences belonging to the cell." }
    , { "getLeafInstanceOccurrencesUnder", (PyCFunction)PyCell_getLeafInstanceOccurrencesUnder, METH_VARARGS
      , "Returns the collection of all occurrences belonging to this cell and intersecting the given rectangular area." }
    , { "getReferences"       , (PyCFunction)PyCell_getReferences       , METH_VARARGS, "Returns the collection of all references belonging to the cell." }
    , { "getHyperNets"        , (PyCFunction)PyCell_getHyperNets        , METH_VARARGS, "Returns the collection of all hyperNets belonging to the cell." }
    , { "getNet"              , (PyCFunction)PyCell_getNet              , METH_VARARGS, "Returns the net of name <name> if it exists, else NULL." }
    , { "getNets"             , (PyCFunction)PyCell_getNets             , METH_NOARGS , "Returns the collection of all nets of the cell." }
    , { "getExternalNets"     , (PyCFunction)PyCell_getExternalNets     , METH_NOARGS , "Returns the collection of all external nets of the cell." }
    , { "getClockNets"        , (PyCFunction)PyCell_getClockNets        , METH_NOARGS , "Returns the collection of all clock nets of the cell." }
    , { "getSupplyNets"       , (PyCFunction)PyCell_getSupplyNets       , METH_NOARGS , "Returns the collection of all supply nets of the cell." }
    , { "getPowerNets"        , (PyCFunction)PyCell_getPowerNets        , METH_NOARGS , "Returns the collection of all power nets of the cell." }
    , { "getGroundNets"       , (PyCFunction)PyCell_getGroundNets       , METH_NOARGS , "Returns the collection of all ground nets of the cell." }
    , { "getAbutmentBox"      , (PyCFunction)PyCell_getAbutmentBox      , METH_NOARGS , "Returns the abutment box of the cell(which is defined by the designer unlike the bounding box which is managed dynamically)" }
    , { "isTerminal"          , (PyCFunction)PyCell_isTerminal          , METH_NOARGS , "Returns true if the cell is marked as terminal, else false." }
    , { "isLeaf"              , (PyCFunction)PyCell_isLeaf              , METH_NOARGS , "Returns true if the cell is a leaf of the hierarchy, else false." }
    , { "isBound"             , (PyCFunction)PyCell_isPyBound           , METH_NOARGS , "Returns true if the cell is bounded to the hurricane cell" }    
    , { "setName"             , (PyCFunction)PyCell_setName             , METH_VARARGS, "Allows to change the cell name." }
    , { "setAbutmentBox"      , (PyCFunction)PyCell_setAbutmentBox      , METH_VARARGS, "Sets the cell abutment box." }
    , { "setTerminal"         , (PyCFunction)PyCell_setTerminal         , METH_VARARGS, "Sets the cell terminal status." }
    , { "uniquify"            , (PyCFunction)PyCell_uniquify            , METH_VARARGS, "Uniquify the Cell and it's instances up to <depth>." }
    , { "getClone"            , (PyCFunction)PyCell_getClone            , METH_NOARGS , "Return a copy of the Cell (placement only)." }
    , { "destroy"             , (PyCFunction)PyCell_destroy             , METH_NOARGS
                              , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // x-------------------------------------------------------------x
  // |                  "PyCell" Object Methods                    |
  // x-------------------------------------------------------------x

  DBoDeleteMethod(Cell)
  PyTypeObjectLinkPyType(Cell)


#else  // End of Python Module Code Part.


// x=================================================================x
// |               "PyCell" Shared Library Code Part                 |
// x=================================================================x

  // Link/Creation Method.
  DBoLinkCreateMethod(Cell)
  PyTypeInheritedObjectDefinitions(Cell, Entity)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
