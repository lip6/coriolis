
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
// $Id: PyCell.cpp,v 1.34 2008/02/07 17:09:41 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyCell.cpp"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyName.h"
#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/isobar/PyOccurrence.h"
#include "hurricane/isobar/ProxyProperty.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyNetLocator.h"
#include "hurricane/isobar/PyReferenceLocator.h"
#include "hurricane/isobar/PyInstanceLocator.h"
#include "hurricane/isobar/PyOccurrenceLocator.h"

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
  // Attribute Method  :  "PyCell_getLibrary ()"

  static PyObject* PyCell_getLibrary ( PyCell *self ) {
    trace << "PyCell_getLibrary ()" << endl;

    Library* library = NULL;

    HTRY
    METHOD_HEAD ( "Cell.getLibrary()" )
    library = cell->getLibrary ();
    HCATCH
    
    return PyLibrary_Link ( library );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getName ()"

  static PyObject* PyCell_getName(PyCell *self) {
    trace << "PyCell_getName ()" << endl;
    
    METHOD_HEAD ( "Cell.getName()" )

    PyName* pyName = PyObject_NEW ( PyName, &PyTypeName );
    if ( pyName == NULL ) { return NULL; }

    HTRY
    pyName->_object = new Name ( cell->getName() );
    HCATCH
    
    return ( (PyObject*)pyName );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getInstance ()"

  static PyObject* PyCell_getInstance ( PyCell *self, PyObject* args ) {
    trace << "PyCell_getInstance ()" << endl;

    Instance* instance = NULL;
    
    HTRY
    METHOD_HEAD ( "Cell.getInstance()" )

    PyName* arg0;
    if ( ! ParseOneArg ( "Cell.getInstance", args, NAME_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    instance = cell->getInstance ( *PYNAME_O(arg0) );
    HCATCH
    
    return PyInstance_Link ( instance );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getInstancesLocator ()"

  static PyObject* PyCell_getInstancesLocator ( PyCell *self ) {
    trace << "PyCell_getInstancesLocator ()" << endl;

    METHOD_HEAD ( "Cell.getInstancesLocator()" )

    Instances instances = cell->getInstances ();

    PyInstanceLocator* pyInstanceLocator = PyObject_NEW ( PyInstanceLocator, &PyTypeInstanceLocator );
    if (pyInstanceLocator == NULL) { return NULL; }

    HTRY
    pyInstanceLocator->_object = instances.getLocator ();
    HCATCH

    return ( (PyObject*)pyInstanceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getInstancesUnderLocator ()"

  static PyObject* PyCell_getInstancesUnderLocator ( PyCell *self, PyObject* args ) {
    PyInstanceLocator* pyInstanceLocator;

    trace << "PyCell_getInstancesUnderLocator ()" << endl;

    METHOD_HEAD ( "Cell.getInstancesUnderLocator()" )

    PyBox* arg0;
    if ( ! ParseOneArg ( "Cell.getInstancesUnderLocator", args, BOX_ARG, (PyObject**)&arg0 ) ) return ( NULL );        

    Instances instances = cell->getInstancesUnder ( *PYBOX_O(arg0) );

    pyInstanceLocator = PyObject_NEW ( PyInstanceLocator, &PyTypeInstanceLocator );
    if (pyInstanceLocator == NULL) { return NULL; }
    
    HTRY
    pyInstanceLocator->_object = instances.getLocator ();
    HCATCH

    return ( (PyObject*)pyInstanceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getSlaveInstancesLocator ()"

  static PyObject* PyCell_getSlaveInstancesLocator ( PyCell *self ) {
    trace << "PyCell_getSlaveInstancesLocator ()" << endl;

    METHOD_HEAD ( "Cell.getSlaveInstancesLocator()" )

    Instances instances = cell->getSlaveInstances ();

    PyInstanceLocator* pyInstanceLocator = PyObject_NEW ( PyInstanceLocator, &PyTypeInstanceLocator );
    if (pyInstanceLocator == NULL) { return NULL; }
    
    HTRY
    pyInstanceLocator->_object = instances.getLocator ();
    HCATCH

    return ( (PyObject*)pyInstanceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getOccurrencesLocator ()"

  static PyObject* PyCell_getOccurrencesLocator ( PyCell *self ) {
    trace << "PyCell_getOccurrencesLocator ()" << endl;

    METHOD_HEAD ( "Cell.getOccurrencesLocator()" )

    Occurrences  occurrences = cell->getOccurrences ();

    PyOccurrenceLocator* pyOccurrenceLocator = PyObject_NEW ( PyOccurrenceLocator, &PyTypeOccurrenceLocator );
    if (pyOccurrenceLocator == NULL) { return NULL; }

    HTRY
    pyOccurrenceLocator->_object = occurrences.getLocator ();
    HCATCH

    return ( (PyObject*)pyOccurrenceLocator );
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getOccurrencesUnderLocator ()"

  static PyObject* PyCell_getOccurrencesUnderLocator ( PyCell *self, PyObject* args ) {
    trace << "PyCell_getOccurrencesUnderLocator ()" << endl;

    METHOD_HEAD ( "Cell.getOccurrencesUnderLocator()" )

    PyBox* arg0;
    if ( ! ParseOneArg ( "Cell.getInstancesUnderLocator", args, BOX_ARG, (PyObject**)&arg0 ) ) return ( NULL );        

    PyOccurrenceLocator* pyOccurrenceLocator = PyObject_NEW ( PyOccurrenceLocator, &PyTypeOccurrenceLocator );
    if (pyOccurrenceLocator == NULL) { return NULL; }

    HTRY
    Occurrences  occurrences = cell->getOccurrencesUnder ( *PYBOX_O(arg0) );
    pyOccurrenceLocator->_object = occurrences.getLocator ();
    HCATCH

    return ( (PyObject*)pyOccurrenceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getLeafInstanceOccurrencesLocator ()"

  static PyObject* PyCell_getLeafInstanceOccurrencesLocator ( PyCell *self ) {
    trace << "PyCell_getLeafInstanceOccurrencesLocator ()" << endl;

    METHOD_HEAD ( "Cell.getLeafInstanceOccurrencesLocator()" )

    Occurrences occurrences = cell->getLeafInstanceOccurrences ();

    PyOccurrenceLocator* pyOccurrenceLocator = PyObject_NEW ( PyOccurrenceLocator, &PyTypeOccurrenceLocator );
    if (pyOccurrenceLocator == NULL) { return NULL; }

    HTRY
    pyOccurrenceLocator->_object = occurrences.getLocator ();
    HCATCH

    return ( (PyObject*)pyOccurrenceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getLeafInstanceOccurrencesUnderLocator ()"

  static PyObject* PyCell_getLeafInstanceOccurrencesUnderLocator ( PyCell *self, PyObject* args ) {
    trace << "PyCell_getLeafInstanceOccurrencesUnderLocator ()" << endl;

    METHOD_HEAD ( "Cell.getLeafInstanceOccurrencesUnderLocator()" )

    PyBox* arg0;
    if ( ! ParseOneArg ( "Cell.getLeafInstanceOccurencesUnderLocator", args, BOX_ARG, (PyObject**)&arg0 ) ) return ( NULL );        

    PyOccurrenceLocator* pyOccurrenceLocator = PyObject_NEW ( PyOccurrenceLocator, &PyTypeOccurrenceLocator );
    if (pyOccurrenceLocator == NULL) { return NULL; }

    HTRY
    Occurrences  occurrences = cell->getLeafInstanceOccurrencesUnder ( *PYBOX_O(arg0) );
    //pyOccurrenceLocator->_object = new locator ( instances.getLocator () );
    pyOccurrenceLocator->_object = occurrences.getLocator ();
    HCATCH

    return ( (PyObject*)pyOccurrenceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getReferencesLocator ()"

  static PyObject* PyCell_getReferencesLocator ( PyCell *self ) {
    trace << "PyCell_getReferencesLocator ()" << endl;

    METHOD_HEAD ( "Cell.getReferencesLocator()" )

    References references = cell->getReferences ();

    PyReferenceLocator* pyReferenceLocator = PyObject_NEW ( PyReferenceLocator, &PyTypeReferenceLocator );
    if (pyReferenceLocator == NULL) { return NULL; }

    HTRY
    pyReferenceLocator->_object = references.getLocator ();
    HCATCH

    return ( (PyObject*)pyReferenceLocator );
  }

  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getHyperNetsLocator ()"

  static PyObject* PyCell_getHyperNetsLocator ( PyCell *self ) {
    trace << "PyCell_getHyperNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.getHyperNetsLocator()" )

    Occurrences  occurrences = cell->getHyperNetRootNetOccurrences();

    PyOccurrenceLocator* pyOccurrenceLocator = PyObject_NEW ( PyOccurrenceLocator, &PyTypeOccurrenceLocator );
    if (pyOccurrenceLocator == NULL) { return NULL; }

    HTRY
    pyOccurrenceLocator->_object = occurrences.getLocator ();
    HCATCH

    return ( (PyObject*)pyOccurrenceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getNet ()"

  static PyObject* PyCell_getNet ( PyCell *self, PyObject* args ) {
    trace << "PyCell_getNet ()" << endl;

    Net* net = NULL;

    HTRY
    METHOD_HEAD ( "Cell.getNet()" )

    PyName* arg0;
    if ( ! ParseOneArg ( "Cell.getNet", args, NAME_ARG, (PyObject**)&arg0 ) ) return ( NULL );
    
    net = cell->getNet ( *PYNAME_O(arg0) );
    HCATCH
    
    return PyNet_Link ( net );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getNetsLocator ()"

  static PyObject* PyCell_getNetsLocator ( PyCell *self ) {
    trace << "PyCell_getNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.getNetsLocator()" )

    PyNetLocator* pyNetLocator = NULL;

    HTRY
    Nets nets = cell->getNets ();

    pyNetLocator = PyObject_NEW ( PyNetLocator, &PyTypeNetLocator );
    if (pyNetLocator == NULL) { return NULL; }

    pyNetLocator->_object = nets.getLocator ();
    HCATCH

    return ( (PyObject*)pyNetLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getExternalNetsLocator ()"

  static PyObject* PyCell_getExternalNetsLocator ( PyCell *self ) {
    trace << "PyCell_getExternalNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.getExternalNetsLocator()" )

    PyNetLocator* pyNetLocator = NULL;

    HTRY
    Nets nets = cell->getExternalNets ();

    pyNetLocator = PyObject_NEW ( PyNetLocator, &PyTypeNetLocator );
    if (pyNetLocator == NULL) { return NULL; }

    pyNetLocator->_object = nets.getLocator ();
    HCATCH

    return ( (PyObject*)pyNetLocator );
  }  

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getClockNetsLocator ()"

  static PyObject* PyCell_getClockNetsLocator ( PyCell *self ) {
    trace << "PyCell_getClockNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.getClockNetsLocator()" )

    PyNetLocator* pyNetLocator = NULL;

    HTRY
    Nets nets = cell->getClockNets ();

    pyNetLocator = PyObject_NEW ( PyNetLocator, &PyTypeNetLocator );
    if (pyNetLocator == NULL) { return NULL; }

    pyNetLocator->_object = nets.getLocator ();
    HCATCH

    return ( (PyObject*)pyNetLocator );
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getSupplyNetsLocator ()"

  static PyObject* PyCell_getSupplyNetsLocator ( PyCell *self ) {
    trace << "PyCell_getSupplyNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.getSupplyNetsLocator()" )

    PyNetLocator* pyNetLocator = NULL;

    HTRY
    Nets nets = cell->getSupplyNets ();

    pyNetLocator = PyObject_NEW ( PyNetLocator, &PyTypeNetLocator );
    if (pyNetLocator == NULL) { return NULL; }

    pyNetLocator->_object = nets.getLocator ();
    HCATCH

    return ( (PyObject*)pyNetLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getPowerNetsLocator ()"

  static PyObject* PyCell_getPowerNetsLocator ( PyCell *self )
  {
    trace << "PyCell_getPowerNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.getPowerNetsLocator()" )

    PyNetLocator* pyNetLocator = NULL;

    HTRY
    Nets nets = cell->getPowerNets ();

    pyNetLocator = PyObject_NEW ( PyNetLocator, &PyTypeNetLocator );
    if (pyNetLocator == NULL) { return NULL; }

    pyNetLocator->_object = nets.getLocator ();
    HCATCH

    return ( (PyObject*)pyNetLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_getGroundNetsLocator ()"

  static PyObject* PyCell_getGroundNetsLocator ( PyCell *self ) {
    trace << "PyCell_getGroundNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.getGroundNetsLocator()" )

    PyNetLocator* pyNetLocator = NULL;

    HTRY
    Nets nets = cell->getGroundNets ();

    pyNetLocator = PyObject_NEW ( PyNetLocator, &PyTypeNetLocator );
    if (pyNetLocator == NULL) { return NULL; }

    pyNetLocator->_object = nets.getLocator ();
    HCATCH

    return ( (PyObject*)pyNetLocator );
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

  static PyObject* PyCell_setName ( PyCell *self, PyObject* args )
  {
    trace << "Cell.setName()" << endl;

    HTRY
    METHOD_HEAD ( "Cell.setName()" )

    PyName* name;
    if ( ! ParseOneArg ( "Cell.setName", args, NAME_ARG, (PyObject**)&name ) ) return ( NULL );
    cell->setName ( *PYNAME_O(name) );
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_setAbutmentBox ()"

  static PyObject* PyCell_setAbutmentBox ( PyCell *self, PyObject* args ) {
    trace << "Cell.setAbutmentBox()" << endl;

    HTRY
    METHOD_HEAD ( "Cell.setAbutmentBox()" )

    PyBox* abutmentBox;
    if ( ! ParseOneArg ( "Cell.setAbutmentBox", args, BOX_ARG, (PyObject**)&abutmentBox ) ) return ( NULL );
    cell->setAbutmentBox ( *PYBOX_O(abutmentBox) );
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
    if ( ! ParseOneArg ( "Cell.setTerminal", args, INT_ARG, (PyObject**)&arg0 ) ) return ( NULL );
    cell->setTerminal ( PyInt_AsLong(arg0) != 0 );
    HCATCH

    Py_RETURN_NONE;
  }


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyCell_isTerminal, isTerminal ,PyCell,Cell)
  DirectGetBoolAttribute(PyCell_isLeaf,         isLeaf ,PyCell,Cell)

  GetBoundStateAttribute(PyCell_isPyBound              ,PyCell,Cell)

  // ---------------------------------------------------------------
  // PyCell Attribute Method table.

  PyMethodDef PyCell_Methods[] =
    { { "getLibrary"          , (PyCFunction)PyCell_getLibrary           , METH_NOARGS , "Returns the library owning the cell." }
    , { "getName"             , (PyCFunction)PyCell_getName              , METH_NOARGS , "Returns the name of the cell." }
    , { "getInstance"         , (PyCFunction)PyCell_getInstance          , METH_VARARGS, "Returns the instance of name <name> if it exists, else NULL." }
    , { "getInstancesLocator" , (PyCFunction)PyCell_getInstancesLocator  , METH_NOARGS , "Returns the locator of the collection of all instances called by the cell." } // getInstances
    , { "getInstancesUnderLocator"  , (PyCFunction)PyCell_getInstancesUnderLocator  , METH_VARARGS, "Returns the locator of the collection of all instances of the cell intersecting the given rectangular area." } // getInstancesUnder
    , { "getSlaveInstancesLocator"  , (PyCFunction)PyCell_getSlaveInstancesLocator  , METH_NOARGS , "Returns the locator of the collection of instances whose master is this cell." } // getSlaveInstances
    , { "getOccurrencesLocator"     , (PyCFunction)PyCell_getOccurrencesLocator     , METH_VARARGS, "Returns the collection of all occurrences belonging to the cell." }
    , { "getOccurrencesUnderLocator", (PyCFunction)PyCell_getOccurrencesUnderLocator, METH_NOARGS , "Returns the collection of all occurrences belonging to this cell and intersecting the given rectangular area." }
    , { "getLeafInstanceOccurrencesLocator", (PyCFunction)PyCell_getLeafInstanceOccurrencesLocator, METH_VARARGS, "Returns the collection of all occurrences belonging to the cell." }
    , { "getLeafInstanceOccurrencesUnderLocator", (PyCFunction)PyCell_getLeafInstanceOccurrencesUnderLocator, METH_NOARGS , "Returns the collection of all occurrences belonging to this cell and intersecting the given rectangular area." }
    , { "getReferencesLocator"      , (PyCFunction)PyCell_getReferencesLocator      , METH_VARARGS, "Returns the collection of all references belonging to the cell." }
    , { "getHyperNetsLocator"       , (PyCFunction)PyCell_getHyperNetsLocator       , METH_VARARGS, "Returns the collection of all hyperNets belonging to the cell." }
    , { "getNet"              , (PyCFunction)PyCell_getNet              , METH_VARARGS, "Returns the net of name <name> if it exists, else NULL." }
    , { "getNetsLocator"      , (PyCFunction)PyCell_getNetsLocator      , METH_NOARGS , "Returns the collection of all nets of the cell." }
    , { "getExternalNetsLocator", (PyCFunction)PyCell_getExternalNetsLocator, METH_NOARGS , "Returns the collection of all external nets of the cell." }
    , { "getClockNetsLocator" , (PyCFunction)PyCell_getClockNetsLocator , METH_NOARGS , "Returns the collection of all clock nets of the cell." }
    , { "getSupplyNetsLocator", (PyCFunction)PyCell_getSupplyNetsLocator, METH_NOARGS , "Returns the collection of all supply nets of the cell." }
    , { "getPowerNetsLocator" , (PyCFunction)PyCell_getPowerNetsLocator , METH_NOARGS , "Returns the collection of all power nets of the cell." }
    , { "getGroundNetsLocator", (PyCFunction)PyCell_getGroundNetsLocator, METH_NOARGS , "Returns the collection of all ground nets of the cell." }
    , { "getAbutmentBox"      , (PyCFunction)PyCell_getAbutmentBox      , METH_NOARGS , "Returns the abutment box of the cell(which is defined by the designer unlike the bounding box which is managed dynamically)" }
    , { "isTerminal"          , (PyCFunction)PyCell_isTerminal          , METH_NOARGS , "Returns true if the cell is marked as terminal, else false." }
    , { "isLeaf"              , (PyCFunction)PyCell_isLeaf              , METH_NOARGS , "Returns true if the cell is a leaf of the hierarchy, else false." }
    , { "isBound"             , (PyCFunction)PyCell_isPyBound           , METH_NOARGS, "Returns true if the cell is bounded to the hurricane cell" }    
    , { "setName"             , (PyCFunction)PyCell_setName             , METH_VARARGS, "Allows to change the cell name." }
    , { "setAbutmentBox"      , (PyCFunction)PyCell_setAbutmentBox      , METH_VARARGS, "Sets the cell abutment box." }
    , { "setTerminal"         , (PyCFunction)PyCell_setTerminal         , METH_VARARGS, "Sets the cell terminal status." }
    , { "destroy"             , (PyCFunction)PyCell_destroy             , METH_NOARGS
                              , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // x-------------------------------------------------------------x
  // |                  "PyCell" Object Methods                    |
  // x-------------------------------------------------------------x

  static PyObject* PyCell_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyCell_new()" << endl;

    PyObject* arg0;
    PyObject* arg1;
    string lib_name_arg = ":library:name";
    if (!ParseTwoArg("Cell.init", args, lib_name_arg, &arg0, &arg1)) {
        cerr << "argument error" << endl;
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Cell constructor." );
        return(NULL);
    }

    Cell* cell = NULL;

    HTRY
    cell = Cell::create(PYLIBRARY_O(arg0), getString(*PYNAME_O(arg1)));
    HCATCH

    return PyCell_Link(cell);
  }

  static void PyCell_DeAlloc ( PyCell *self ) {                                                                      
    trace << "PyDbObject_DeAlloc(" << hex << self << ") "                
          << self->ACCESS_OBJECT << endl;                                
                                                                         
    if ( self->ACCESS_OBJECT != NULL ) {                                 
        ProxyProperty* proxy = dynamic_cast<ProxyProperty*>              
                               ( self->ACCESS_OBJECT->getProperty ( ProxyProperty::getPropertyName() ) ); \
        if (proxy == NULL) {                                             
          ostringstream  message;                                        
          message << "deleting a Python object with no Proxy attached "; 
          cerr << "yes man" << endl;
          PyErr_SetString ( ProxyError, message.str().c_str() );         
        }                                                                
        self->ACCESS_OBJECT->remove ( proxy );                           
    } else {
        cerr << "ACCESS_OBJECT is NULL" << endl;
    }
    PyObject_DEL ( self );                                               
  }

  PyTypeObjectLinkPyType(Cell)
  PyTypeObjectConstructor(Cell)

#else  // End of Python Module Code Part.


// x=================================================================x
// |               "PyCell" Shared Library Code Part                 |
// x=================================================================x


  // Link/Creation Method.
  DBoLinkCreateMethod(PyCell_Link,PyCell,PyTypeCell,Cell)


  // ---------------------------------------------------------------
  // PyCell Object Definitions.
  PyTypeInheritedObjectDefinitions(Cell, Entity)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
