
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

#include "PyCell.h"
#include "PyBox.h"
#include "PyName.h"
#include "PyLibrary.h"
#include "PyInstance.h"
#include "PyOccurrence.h"
#include "ProxyProperty.h"
#include "PyNet.h"
#include "PyNetLocator.h"
#include "PyReferenceLocator.h"
#include "PyInstanceLocator.h"
#include "PyOccurrenceLocator.h"

namespace  Isobar {


USING_NAMESPACE_HURRICANE

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
  DBoDeleteAttribute(PyCell_Delete,PyCell)



  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetLibrary ()"

  static PyObject* PyCell_GetLibrary ( PyCell *self ) {
    trace << "PyCell_GetLibrary ()" << endl;

    Library* library = NULL;

    HTRY
    METHOD_HEAD ( "Cell.GetLibrary()" )
    library = cell->GetLibrary ();
    HCATCH
    
    return PyLibrary_Link ( library );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetName ()"

  static PyObject* PyCell_GetName(PyCell *self) {
    trace << "PyCell_GetName ()" << endl;
    
    METHOD_HEAD ( "Cell.GetName()" )

    PyName* pyName = PyObject_NEW ( PyName, &PyTypeName );
    if ( pyName == NULL ) { return NULL; }

    HTRY
    pyName->_object = new Name ( cell->GetName() );
    HCATCH
    
    return ( (PyObject*)pyName );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetInstance ()"

  static PyObject* PyCell_GetInstance ( PyCell *self, PyObject* args ) {
    trace << "PyCell_GetInstance ()" << endl;

    Instance* instance = NULL;
    
    HTRY
    METHOD_HEAD ( "Cell.GetInstance()" )

    PyName* arg0;
    if ( ! ParseOneArg ( "Cell.GetInstance", args, NAME_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    instance = cell->GetInstance ( *PYNAME_O(arg0) );
    HCATCH
    
    return PyInstance_Link ( instance );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetInstancesLocator ()"

  static PyObject* PyCell_GetInstancesLocator ( PyCell *self ) {
    trace << "PyCell_GetInstancesLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetInstancesLocator()" )

    Instances instances = cell->GetInstances ();

    PyInstanceLocator* pyInstanceLocator = PyObject_NEW ( PyInstanceLocator, &PyTypeInstanceLocator );
    if (pyInstanceLocator == NULL) { return NULL; }

    HTRY
    pyInstanceLocator->_object = instances.GetLocator ();
    HCATCH

    return ( (PyObject*)pyInstanceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetInstancesUnderLocator ()"

  static PyObject* PyCell_GetInstancesUnderLocator ( PyCell *self, PyObject* args ) {
    PyInstanceLocator* pyInstanceLocator;

    trace << "PyCell_GetInstancesUnderLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetInstancesUnderLocator()" )

    PyBox* arg0;
    if ( ! ParseOneArg ( "Cell.GetInstancesUnderLocator", args, BOX_ARG, (PyObject**)&arg0 ) ) return ( NULL );        

    Instances instances = cell->GetInstancesUnder ( *PYBOX_O(arg0) );

    pyInstanceLocator = PyObject_NEW ( PyInstanceLocator, &PyTypeInstanceLocator );
    if (pyInstanceLocator == NULL) { return NULL; }
    
    HTRY
    pyInstanceLocator->_object = instances.GetLocator ();
    HCATCH

    return ( (PyObject*)pyInstanceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetSlaveInstancesLocator ()"

  static PyObject* PyCell_GetSlaveInstancesLocator ( PyCell *self ) {
    trace << "PyCell_GetSlaveInstancesLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetSlaveInstancesLocator()" )

    Instances instances = cell->GetSlaveInstances ();

    PyInstanceLocator* pyInstanceLocator = PyObject_NEW ( PyInstanceLocator, &PyTypeInstanceLocator );
    if (pyInstanceLocator == NULL) { return NULL; }
    
    HTRY
    pyInstanceLocator->_object = instances.GetLocator ();
    HCATCH

    return ( (PyObject*)pyInstanceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetOccurrencesLocator ()"

  static PyObject* PyCell_GetOccurrencesLocator ( PyCell *self ) {
    trace << "PyCell_GetOccurrencesLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetOccurrencesLocator()" )

    Occurrences  occurrences = cell->GetOccurrences ();

    PyOccurrenceLocator* pyOccurrenceLocator = PyObject_NEW ( PyOccurrenceLocator, &PyTypeOccurrenceLocator );
    if (pyOccurrenceLocator == NULL) { return NULL; }

    HTRY
    pyOccurrenceLocator->_object = occurrences.GetLocator ();
    HCATCH

    return ( (PyObject*)pyOccurrenceLocator );
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetOccurrencesUnderLocator ()"

  static PyObject* PyCell_GetOccurrencesUnderLocator ( PyCell *self, PyObject* args ) {
    trace << "PyCell_GetOccurrencesUnderLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetOccurrencesUnderLocator()" )

    PyBox* arg0;
    if ( ! ParseOneArg ( "Cell.GetInstancesUnderLocator", args, BOX_ARG, (PyObject**)&arg0 ) ) return ( NULL );        

    PyOccurrenceLocator* pyOccurrenceLocator = PyObject_NEW ( PyOccurrenceLocator, &PyTypeOccurrenceLocator );
    if (pyOccurrenceLocator == NULL) { return NULL; }

    HTRY
    Occurrences  occurrences = cell->GetOccurrencesUnder ( *PYBOX_O(arg0) );
    pyOccurrenceLocator->_object = occurrences.GetLocator ();
    HCATCH

    return ( (PyObject*)pyOccurrenceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetLeafInstanceOccurrencesLocator ()"

  static PyObject* PyCell_GetLeafInstanceOccurrencesLocator ( PyCell *self ) {
    trace << "PyCell_GetLeafInstanceOccurrencesLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetLeafInstanceOccurrencesLocator()" )

    Occurrences occurrences = cell->GetLeafInstanceOccurrences ();

    PyOccurrenceLocator* pyOccurrenceLocator = PyObject_NEW ( PyOccurrenceLocator, &PyTypeOccurrenceLocator );
    if (pyOccurrenceLocator == NULL) { return NULL; }

    HTRY
    pyOccurrenceLocator->_object = occurrences.GetLocator ();
    HCATCH

    return ( (PyObject*)pyOccurrenceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetLeafInstanceOccurrencesUnderLocator ()"

  static PyObject* PyCell_GetLeafInstanceOccurrencesUnderLocator ( PyCell *self, PyObject* args ) {
    trace << "PyCell_GetLeafInstanceOccurrencesUnderLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetLeafInstanceOccurrencesUnderLocator()" )

    PyBox* arg0;
    if ( ! ParseOneArg ( "Cell.GetLeafInstanceOccurencesUnderLocator", args, BOX_ARG, (PyObject**)&arg0 ) ) return ( NULL );        

    PyOccurrenceLocator* pyOccurrenceLocator = PyObject_NEW ( PyOccurrenceLocator, &PyTypeOccurrenceLocator );
    if (pyOccurrenceLocator == NULL) { return NULL; }

    HTRY
    Occurrences  occurrences = cell->GetLeafInstanceOccurrencesUnder ( *PYBOX_O(arg0) );
    //pyOccurrenceLocator->_object = new locator ( instances.GetLocator () );
    pyOccurrenceLocator->_object = occurrences.GetLocator ();
    HCATCH

    return ( (PyObject*)pyOccurrenceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetReferencesLocator ()"

  static PyObject* PyCell_GetReferencesLocator ( PyCell *self ) {
    trace << "PyCell_GetReferencesLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetReferencesLocator()" )

    References references = cell->GetReferences ();

    PyReferenceLocator* pyReferenceLocator = PyObject_NEW ( PyReferenceLocator, &PyTypeReferenceLocator );
    if (pyReferenceLocator == NULL) { return NULL; }

    HTRY
    pyReferenceLocator->_object = references.GetLocator ();
    HCATCH

    return ( (PyObject*)pyReferenceLocator );
  }

  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetHyperNetsLocator ()"

  static PyObject* PyCell_GetHyperNetsLocator ( PyCell *self ) {
    trace << "PyCell_GetHyperNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetHyperNetsLocator()" )

    Occurrences  occurrences = cell->GetHyperNetRootNetOccurrences();

    PyOccurrenceLocator* pyOccurrenceLocator = PyObject_NEW ( PyOccurrenceLocator, &PyTypeOccurrenceLocator );
    if (pyOccurrenceLocator == NULL) { return NULL; }

    HTRY
    pyOccurrenceLocator->_object = occurrences.GetLocator ();
    HCATCH

    return ( (PyObject*)pyOccurrenceLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetNet ()"

  static PyObject* PyCell_GetNet ( PyCell *self, PyObject* args ) {
    trace << "PyCell_GetNet ()" << endl;

    Net* net = NULL;

    HTRY
    METHOD_HEAD ( "Cell.GetNet()" )

    PyName* arg0;
    if ( ! ParseOneArg ( "Cell.GetNet", args, NAME_ARG, (PyObject**)&arg0 ) ) return ( NULL );
    
    net = cell->GetNet ( *PYNAME_O(arg0) );
    HCATCH
    
    return PyNet_Link ( net );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetNetsLocator ()"

  static PyObject* PyCell_GetNetsLocator ( PyCell *self ) {
    trace << "PyCell_GetNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetNetsLocator()" )

    PyNetLocator* pyNetLocator = NULL;

    HTRY
    Nets nets = cell->GetNets ();

    pyNetLocator = PyObject_NEW ( PyNetLocator, &PyTypeNetLocator );
    if (pyNetLocator == NULL) { return NULL; }

    pyNetLocator->_object = nets.GetLocator ();
    HCATCH

    return ( (PyObject*)pyNetLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetExternalNetsLocator ()"

  static PyObject* PyCell_GetExternalNetsLocator ( PyCell *self ) {
    trace << "PyCell_GetExternalNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetExternalNetsLocator()" )

    PyNetLocator* pyNetLocator = NULL;

    HTRY
    Nets nets = cell->GetExternalNets ();

    pyNetLocator = PyObject_NEW ( PyNetLocator, &PyTypeNetLocator );
    if (pyNetLocator == NULL) { return NULL; }

    pyNetLocator->_object = nets.GetLocator ();
    HCATCH

    return ( (PyObject*)pyNetLocator );
  }  

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetClockNetsLocator ()"

  static PyObject* PyCell_GetClockNetsLocator ( PyCell *self ) {
    trace << "PyCell_GetClockNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetClockNetsLocator()" )

    PyNetLocator* pyNetLocator = NULL;

    HTRY
    Nets nets = cell->GetClockNets ();

    pyNetLocator = PyObject_NEW ( PyNetLocator, &PyTypeNetLocator );
    if (pyNetLocator == NULL) { return NULL; }

    pyNetLocator->_object = nets.GetLocator ();
    HCATCH

    return ( (PyObject*)pyNetLocator );
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetSupplyNetsLocator ()"

  static PyObject* PyCell_GetSupplyNetsLocator ( PyCell *self ) {
    trace << "PyCell_GetSupplyNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetSupplyNetsLocator()" )

    PyNetLocator* pyNetLocator = NULL;

    HTRY
    Nets nets = cell->GetSupplyNets ();

    pyNetLocator = PyObject_NEW ( PyNetLocator, &PyTypeNetLocator );
    if (pyNetLocator == NULL) { return NULL; }

    pyNetLocator->_object = nets.GetLocator ();
    HCATCH

    return ( (PyObject*)pyNetLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetPowerNetsLocator ()"

  static PyObject* PyCell_GetPowerNetsLocator ( PyCell *self )
  {
    trace << "PyCell_GetPowerNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetPowerNetsLocator()" )

    PyNetLocator* pyNetLocator = NULL;

    HTRY
    Nets nets = cell->GetPowerNets ();

    pyNetLocator = PyObject_NEW ( PyNetLocator, &PyTypeNetLocator );
    if (pyNetLocator == NULL) { return NULL; }

    pyNetLocator->_object = nets.GetLocator ();
    HCATCH

    return ( (PyObject*)pyNetLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetGroundNetsLocator ()"

  static PyObject* PyCell_GetGroundNetsLocator ( PyCell *self ) {
    trace << "PyCell_GetGroundNetsLocator ()" << endl;

    METHOD_HEAD ( "Cell.GetGroundNetsLocator()" )

    PyNetLocator* pyNetLocator = NULL;

    HTRY
    Nets nets = cell->GetGroundNets ();

    pyNetLocator = PyObject_NEW ( PyNetLocator, &PyTypeNetLocator );
    if (pyNetLocator == NULL) { return NULL; }

    pyNetLocator->_object = nets.GetLocator ();
    HCATCH

    return ( (PyObject*)pyNetLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_GetAbutmentBox ()"

  static PyObject* PyCell_GetAbutmentBox ( PyCell *self ) {
    trace << "PyCell_GetAbutmentBox()" << endl;
    
    METHOD_HEAD ( "Cell.GetAbutmentBox()" )

    PyBox* abutmentBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (abutmentBox == NULL) { return NULL; }

    HTRY
    abutmentBox->_object = new Box ( cell->GetAbutmentBox() );
    HCATCH

    return ( (PyObject*)abutmentBox );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_SetName ()"

  static PyObject* PyCell_SetName ( PyCell *self, PyObject* args )
  {
    trace << "Cell.SetName()" << endl;

    HTRY
    METHOD_HEAD ( "Cell.SetName()" )

    PyName* name;
    if ( ! ParseOneArg ( "Cell.SetName", args, NAME_ARG, (PyObject**)&name ) ) return ( NULL );
    cell->SetName ( *PYNAME_O(name) );
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_SetAbutmentBox ()"

  static PyObject* PyCell_SetAbutmentBox ( PyCell *self, PyObject* args ) {
    trace << "Cell.SetAbutmentBox()" << endl;

    HTRY
    METHOD_HEAD ( "Cell.SetAbutmentBox()" )

    PyBox* abutmentBox;
    if ( ! ParseOneArg ( "Cell.SetAbutmentBox", args, BOX_ARG, (PyObject**)&abutmentBox ) ) return ( NULL );
    cell->SetAbutmentBox ( *PYBOX_O(abutmentBox) );
    HCATCH

    Py_RETURN_NONE;
  }
  
  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyCell_SetTerminal ()"

  static PyObject* PyCell_SetTerminal ( PyCell *self, PyObject* args ) {
    trace << "PyCell_SetTerminal ()" << endl;

    HTRY
    METHOD_HEAD ( "Cell.SetTerminal()" )

    PyObject* arg0;
    if ( ! ParseOneArg ( "Cell.SetTerminal", args, INT_ARG, (PyObject**)&arg0 ) ) return ( NULL );
    cell->SetTerminal ( PyInt_AsLong(arg0) != 0 );
    HCATCH

    Py_RETURN_NONE;
  }


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyCell_IsTerminal, IsTerminal ,PyCell,Cell)
  DirectGetBoolAttribute(PyCell_IsLeaf,         IsLeaf ,PyCell,Cell)

  GetBoundStateAttribute(PyCell_IsPyBound              ,PyCell,Cell)

  // ---------------------------------------------------------------
  // PyCell Attribute Method table.

  PyMethodDef PyCell_Methods[] =
    { { "GetLibrary"          , (PyCFunction)PyCell_GetLibrary           , METH_NOARGS , "Returns the library owning the cell." }
    , { "GetName"             , (PyCFunction)PyCell_GetName              , METH_NOARGS , "Returns the name of the cell." }
    , { "GetInstance"         , (PyCFunction)PyCell_GetInstance          , METH_VARARGS, "Returns the instance of name <name> if it exists, else NULL." }
    , { "GetInstancesLocator" , (PyCFunction)PyCell_GetInstancesLocator  , METH_NOARGS , "Returns the locator of the collection of all instances called by the cell." } // GetInstances
    , { "GetInstancesUnderLocator"  , (PyCFunction)PyCell_GetInstancesUnderLocator  , METH_VARARGS, "Returns the locator of the collection of all instances of the cell intersecting the given rectangular area." } // GetInstancesUnder
    , { "GetSlaveInstancesLocator"  , (PyCFunction)PyCell_GetSlaveInstancesLocator  , METH_NOARGS , "Returns the locator of the collection of instances whose master is this cell." } // GetSlaveInstances
    , { "GetOccurrencesLocator"     , (PyCFunction)PyCell_GetOccurrencesLocator     , METH_VARARGS, "Returns the collection of all occurrences belonging to the cell." }
    , { "GetOccurrencesUnderLocator", (PyCFunction)PyCell_GetOccurrencesUnderLocator, METH_NOARGS , "Returns the collection of all occurrences belonging to this cell and intersecting the given rectangular area." }
    , { "GetLeafInstanceOccurrencesLocator", (PyCFunction)PyCell_GetLeafInstanceOccurrencesLocator, METH_VARARGS, "Returns the collection of all occurrences belonging to the cell." }
    , { "GetLeafInstanceOccurrencesUnderLocator", (PyCFunction)PyCell_GetLeafInstanceOccurrencesUnderLocator, METH_NOARGS , "Returns the collection of all occurrences belonging to this cell and intersecting the given rectangular area." }
    , { "GetReferencesLocator"      , (PyCFunction)PyCell_GetReferencesLocator      , METH_VARARGS, "Returns the collection of all references belonging to the cell." }
    , { "GetHyperNetsLocator"       , (PyCFunction)PyCell_GetHyperNetsLocator       , METH_VARARGS, "Returns the collection of all hyperNets belonging to the cell." }
    , { "GetNet"              , (PyCFunction)PyCell_GetNet              , METH_VARARGS, "Returns the net of name <name> if it exists, else NULL." }
    , { "GetNetsLocator"      , (PyCFunction)PyCell_GetNetsLocator      , METH_NOARGS , "Returns the collection of all nets of the cell." }
    //, { "GetGlobalNets"       , (PyCFunction)PyCell_GetGlobalNets       , METH_NOARGS , "Returns the collection of all global nets of the cell." }
    , { "GetExternalNetsLocator", (PyCFunction)PyCell_GetExternalNetsLocator, METH_NOARGS , "Returns the collection of all external nets of the cell." }
    //, { "GetInternalNets"     , (PyCFunction)PyCell_GetInternalNets     , METH_NOARGS , "Returns the collection of all internal nets of the cell." }
    , { "GetClockNetsLocator" , (PyCFunction)PyCell_GetClockNetsLocator , METH_NOARGS , "Returns the collection of all clock nets of the cell." }
    , { "GetSupplyNetsLocator", (PyCFunction)PyCell_GetSupplyNetsLocator, METH_NOARGS , "Returns the collection of all supply nets of the cell." }
    , { "GetPowerNetsLocator" , (PyCFunction)PyCell_GetPowerNetsLocator , METH_NOARGS , "Returns the collection of all power nets of the cell." }
    , { "GetGroundNetsLocator", (PyCFunction)PyCell_GetGroundNetsLocator, METH_NOARGS , "Returns the collection of all ground nets of the cell." }
    //, { "GetComponents"       , (PyCFunction)PyCell_GetComponents       , METH_NOARGS , "Returns the collection of all components of the cell." }
    //, { "GetComponentsUnder"  , (PyCFunction)PyCell_GetComponentsUnder  , METH_VARARGS, "Returns the collection of cell components which intersect the given rectangular area." }
    , { "GetAbutmentBox"      , (PyCFunction)PyCell_GetAbutmentBox      , METH_NOARGS , "Returns the abutment box of the cell(which is defined by the designer unlike the bounding box which is managed dynamically)" }
    //, { "GetSymbol"           , (PyCFunction)PyCell_GetSymbol           , METH_NOARGS , "Returns the symbol associated to the cell." }
    //, { "IsCalledBy"          , (PyCFunction)PyCell_IsCalledBy          , METH_VARARGS, "Returns true if the cell <this> is directly or indirectly called by the cell <cell>." }
    , { "IsTerminal"          , (PyCFunction)PyCell_IsTerminal          , METH_NOARGS , "Returns true if the cell is marked as terminal, else false." }
    , { "IsLeaf"              , (PyCFunction)PyCell_IsLeaf              , METH_NOARGS , "Returns true if the cell is a leaf of the hierarchy, else false." }
    , { "IsBound"             , (PyCFunction)PyCell_IsPyBound           , METH_NOARGS, "Returns true if the cell is bounded to the hurricane cell" }    
    , { "SetName"             , (PyCFunction)PyCell_SetName             , METH_VARARGS, "Allows to change the cell name." }
    , { "SetAbutmentBox"      , (PyCFunction)PyCell_SetAbutmentBox      , METH_VARARGS, "Sets the cell abutment box." }
    , { "SetTerminal"         , (PyCFunction)PyCell_SetTerminal         , METH_VARARGS, "Sets the cell terminal status." }
    //, { "SetSymbol"           , (PyCFunction)PyCell_SetSymbol           , METH_VARARGS, "Associates the symbol <symbol> to the cell." }
    //, { "Materialize"         , (PyCFunction)PyCell_Materialize         , METH_NOARGS , "Materializes all components of all the nets of the cell." }
    //, { "Unmaterialize"       , (PyCFunction)PyCell_Unmaterialize       , METH_NOARGS , "De-materializes all components of all the nets of the cell." }
    , { "Delete"              , (PyCFunction)PyCell_Delete              , METH_NOARGS
                              , "Delete associated hurricane object The python object remains." }
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
    cell = Cell::Create(PYLIBRARY_O(arg0), GetString(*PYNAME_O(arg1)));
    HCATCH

    return PyCell_Link(cell);
  }

  static void PyCell_DeAlloc ( PyCell *self ) {                                                                      
    trace << "PyDbObject_DeAlloc(" << hex << self << ") "                
          << self->ACCESS_OBJECT << endl;                                
                                                                         
    if ( self->ACCESS_OBJECT != NULL ) {                                 
        ProxyProperty* proxy = dynamic_cast<ProxyProperty*>              
                               ( self->ACCESS_OBJECT->GetProperty ( ProxyProperty::GetPropertyName() ) ); \
        if (proxy == NULL) {                                             
          ostringstream  message;                                        
          message << "deleting a Python object with no Proxy attached "; 
          cerr << "yes man" << endl;
          PyErr_SetString ( ProxyError, message.str().c_str() );         
        }                                                                
        self->ACCESS_OBJECT->Remove ( proxy );                           
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
 

