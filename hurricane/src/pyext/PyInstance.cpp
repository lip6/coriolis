
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
// $Id: PyInstance.cpp,v 1.31 2008/02/07 17:09:41 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyInstance.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "PyBox.h"
#include "PyName.h"
#include "PyTransformation.h"
#include "PyLibrary.h"
#include "PyCell.h"
#include "PyInstance.h"
#include "PyNet.h"
#include "PyPlug.h"
#include "PyPlugLocator.h"


namespace  Isobar {


USING_NAMESPACE_HURRICANE


extern "C" {


#undef   ACCESS_OBJECT
#undef   ACCESS_CLASS
#define  ACCESS_OBJECT           _baseObject._object
#define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Instance,instance,function)

#define  LOAD_CONSTANT(CONSTANT_VALUE,CONSTANT_NAME)             \
  constant = PyInt_FromLong ( (long)CONSTANT_VALUE );             \
  PyDict_SetItemString ( dictionnary, CONSTANT_NAME, constant );  \
  Py_DECREF ( constant );


// x=================================================================x
// |              "PyInstance" Python Module Code Part               |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                  Global Constants Loading                   |
  // x-------------------------------------------------------------x


  extern void  InstanceLoadConstants ( PyObject* dictionnary )
  {
    PyObject* constant;
      
    LOAD_CONSTANT ( Instance::PlacementStatus::UNPLACED, "PlacementStatusUNPLACED" )
    LOAD_CONSTANT ( Instance::PlacementStatus::PLACED  , "PlacementStatusPLACED" )
    LOAD_CONSTANT ( Instance::PlacementStatus::FIXED   , "PlacementStatusFIXED" )
  }




  // x-------------------------------------------------------------x
  // |               "PyInstance" Local Functions                  |
  // x-------------------------------------------------------------x
      
  // ---------------------------------------------------------------
  // Local Function  :  "PyInt_AsType ()"

  static Instance::PlacementStatus  PyInt_AsPlacementStatus ( PyObject* object )
  {
    switch ( PyInt_AsLong(object) ) {
      case Instance::PlacementStatus::UNPLACED : return ( Instance::PlacementStatus(Instance::PlacementStatus::UNPLACED) );
      case Instance::PlacementStatus::PLACED   : return ( Instance::PlacementStatus(Instance::PlacementStatus::PLACED) );
      case Instance::PlacementStatus::FIXED    : return ( Instance::PlacementStatus(Instance::PlacementStatus::FIXED) );
    }
      
    return ( Instance::PlacementStatus(Instance::PlacementStatus::UNPLACED) );
  }
  



  // x-------------------------------------------------------------x
  // |              "PyInstance" Attribute Methods                 |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).


  // Standart Delete (Attribute).
  DBoDeleteAttribute(PyInstance_Delete,PyInstance)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_GetName ()"

  static PyObject* PyInstance_GetName ( PyInstance *self ) {
    trace << "PyInstance_GetName ()" << endl;
    METHOD_HEAD ( "Instance.GetName()" )

    PyName* pyName = PyObject_NEW ( PyName, &PyTypeName );
    if ( pyName == NULL ) { return NULL; }

    HTRY
    pyName->_object = new Name ( instance->GetName() );
    HCATCH
    
    return ( (PyObject*)pyName ); 
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_GetMasterCell ()"

  static PyObject* PyInstance_GetMasterCell ( PyInstance *self ) {
    trace << "PyInstance_GetMasterCell ()" << endl;
    
    Cell* cell = NULL;

    HTRY
    METHOD_HEAD ( "Instance.GetMasterCell()" )
    cell = instance->GetMasterCell ();
    HCATCH

    return PyCell_Link ( cell );
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_GetPlacementStatus ()"

  static PyObject* PyInstance_GetPlacementStatus ( PyInstance *self ) {
    trace << "PyInstance_GetPlacementStatus ()" << endl;
    
    METHOD_HEAD ( "Instance.GetPlacementStatus()" );

    PyObject* pyObject = NULL;
    HTRY
    pyObject = (PyObject*)Py_BuildValue("i",(long)instance->GetPlacementStatus().GetCode());
    HCATCH

    return pyObject;
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_SetPlacementStatus ()"

  static PyObject* PyInstance_SetPlacementStatus ( PyInstance *self, PyObject* args ) {
    trace << "PyInstance_SetPlacementStatus()" << endl;
    METHOD_HEAD ( "Instance.SetPlacementStatus()" )
      
    HTRY
    PyObject* arg0;
    if ( ! ParseOneArg ( "Instance.SetPlacementStatus()", args, INT_ARG, (PyObject**)&arg0 ) ) return ( NULL );
      
    instance->SetPlacementStatus ( PyInt_AsPlacementStatus(arg0) );
    HCATCH
      
    Py_RETURN_NONE;
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_GetTransformation ()"

  static PyObject* PyInstance_GetTransformation ( PyInstance *self ) {
    trace << "PyInstance_GetTransformation ()" << endl;
    METHOD_HEAD ( "Instance.GetTransformation()" );

    PyTransformation* resultPyTransf = PyObject_NEW ( PyTransformation, &PyTypeTransformation );
    if ( resultPyTransf == NULL ) { return NULL; }

    HTRY
    resultPyTransf->_object = new Transformation ( instance->GetTransformation () );
    HCATCH

    return ( (PyObject*)resultPyTransf );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_GetPlug ()"

  static PyObject* PyInstance_GetPlug ( PyInstance *self, PyObject* args ) {
    trace << "PyInstance_GetPlug ()" << endl;

    Plug* plug = NULL;
    
    HTRY
    METHOD_HEAD ( "Instance.GetPlug()" )

    PyNet* masterNet;
    if ( ! ParseOneArg ( "Instance.GetPlug", args, NET_ARG, (PyObject**)&masterNet ) ) return ( NULL );
 
    plug = instance->GetPlug( PYNET_O(masterNet) );
    HCATCH

    return PyPlug_Link ( plug );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_GetPlugsLocator ()"

  static PyObject* PyInstance_GetPlugsLocator ( PyInstance *self ) {
    trace << "PyInstance_GetPlugsLocator ()" << endl;
    METHOD_HEAD ( "Instance.GetPlugsLocator()" )

    PyPlugLocator* pyPlugLocator = NULL;

    HTRY
    Plugs plugs = instance->GetPlugs ();

    pyPlugLocator = PyObject_NEW ( PyPlugLocator, &PyTypePlugLocator );
    if (pyPlugLocator == NULL) { return NULL; }
    
    pyPlugLocator->_object = plugs.GetLocator ();
    HCATCH

    return ( (PyObject*)pyPlugLocator );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_GetConnectedPlugsLocator ()"

  static PyObject* PyInstance_GetConnectedPlugsLocator ( PyInstance *self )
  {
    trace << "PyInstance_GetConnectedPlugsLocator ()" << endl;
    METHOD_HEAD ( "Instance.GetConnectedPlugsLocator()" )

    PyPlugLocator* pyPlugLocator = NULL;

    HTRY
    Plugs plugs = instance->GetConnectedPlugs ();

    pyPlugLocator = PyObject_NEW ( PyPlugLocator, &PyTypePlugLocator );
    if (pyPlugLocator == NULL) { return NULL; }
    
    pyPlugLocator->_object = plugs.GetLocator ();
    HCATCH

    return ( (PyObject*)pyPlugLocator );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_GetUnconnectedPlugsLocator ()"

  static PyObject* PyInstance_GetUnconnectedPlugsLocator ( PyInstance *self )
  {
    trace << "PyInstance_GetUnconnectedPlugsLocator ()" << endl;
    METHOD_HEAD ( "Instance.GetUnconnectedPlugsLocator()" )

    PyPlugLocator* pyPlugLocator = NULL;

    HTRY
    Plugs plugs = instance->GetUnconnectedPlugs ();

    pyPlugLocator = PyObject_NEW ( PyPlugLocator, &PyTypePlugLocator );
    if (pyPlugLocator == NULL) { return NULL; }
    
    pyPlugLocator->_object = plugs.GetLocator ();
    HCATCH

    return ( (PyObject*)pyPlugLocator );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_GetAbutmentBox ()"

  static PyObject* PyInstance_GetAbutmentBox ( PyInstance *self )
  {
    trace << "PyInstance_GetAbutmentBox ()" << endl;
    METHOD_HEAD ( "Instance.GetAbutmentBox()" )

    PyBox* pyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (pyBox == NULL) { return NULL; }

    HTRY
    pyBox->_object = new Box ( instance->GetAbutmentBox() );
    HCATCH

    return ( (PyObject*)pyBox );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_SetName ()"

  static PyObject* PyInstance_SetName ( PyInstance *self, PyObject* args )
  {
    trace << "PyInstance_SetName()" << endl;
    METHOD_HEAD ( "Instance.SetName()" )

    HTRY
    PyName* name;
    if ( ! ParseOneArg ( "Instance.SetName", args, NAME_ARG, (PyObject**)&name ) ) return ( NULL );

    instance->SetName ( *PYNAME_O(name) );
    HCATCH

    Py_RETURN_NONE;
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_SetTransformation ()"

  static PyObject* PyInstance_SetTransformation ( PyInstance *self, PyObject* args )
  {
    trace << "PyInstance_SetTransformation()" << endl;
    METHOD_HEAD ( "Instance.SetTransformation()" )

    HTRY
    PyTransformation* transformation;
    if ( ! ParseOneArg ( "Instance.SetTransformation", args, TRANS_ARG, (PyObject**)&transformation ) ) return ( NULL );

    instance->SetTransformation ( *PYTRANSFORMATION_O(transformation) );
    HCATCH

    Py_RETURN_NONE;
  }


  
  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_SetMasterCell ()"

  static PyObject* PyInstance_SetMasterCell ( PyInstance *self, PyObject* args )
  {
    trace << "Instance.SetMasterCell()" << endl;
    METHOD_HEAD ( "Instance.SetMasterCell()" )

    HTRY
    PyCell* masterCell;
    if ( ! ParseOneArg ( "Instance.SetMasterCell", args, CELL_ARG, (PyObject**)&masterCell ) ) return ( NULL );

    instance->SetMasterCell ( PYCELL_O(masterCell) );
    HCATCH

    Py_RETURN_NONE;
  }

  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyInstance_IsTerminal ,IsTerminal ,PyInstance,Instance)
  DirectGetBoolAttribute(PyInstance_IsLeaf     ,IsLeaf     ,PyInstance,Instance)

  GetBoundStateAttribute(PyInstance_IsPyBound              ,PyInstance,Instance)
  

  
  // ---------------------------------------------------------------
  // PyInstance Attribute Method table.

  PyMethodDef PyInstance_Methods[] =
    { { "GetName"                   , (PyCFunction)PyInstance_GetName                   , METH_NOARGS , "Returns the instance name." }
    , { "GetMasterCell"             , (PyCFunction)PyInstance_GetMasterCell             , METH_NOARGS , "Returns the cell model referenced by the instance." }
    , { "GetTransformation"         , (PyCFunction)PyInstance_GetTransformation         , METH_NOARGS , "Returns the transformation associated to the instance." }
    , { "GetPlacementStatus"        , (PyCFunction)PyInstance_GetPlacementStatus        , METH_NOARGS , "Returns the placement status of the instance." }
    , { "GetPlug"                   , (PyCFunction)PyInstance_GetPlug                   , METH_VARARGS, "Returns the plug associated to the <masterNet> if it exists or else NULL (if the net is not external)." }
    , { "GetPlugsLocator"           , (PyCFunction)PyInstance_GetPlugsLocator           , METH_NOARGS , "Returns the collection of instance plugs." }
    , { "GetConnectedPlugsLocator"  , (PyCFunction)PyInstance_GetConnectedPlugsLocator  , METH_NOARGS , "Returns the collection of instance plugs which are effectively connected." }
    , { "GetUnconnectedPlugsLocator", (PyCFunction)PyInstance_GetUnconnectedPlugsLocator, METH_NOARGS , "Returns the collection of instance plugs which are not connected." }
    //, { "GetPath"                   , (PyCFunction)PyInstance_GetPath                   , METH_NOARGS , "Returns the path composed of the instance solely." }
    //, { "GetPath"                   , (PyCFunction)PyInstance_GetPath                   , METH_VARARGS, "Returns the path resulting of the concatenation of the instance and the tail path (possibly empty)." }
    , { "GetAbutmentBox"            , (PyCFunction)PyInstance_GetAbutmentBox            , METH_NOARGS , "Returns the abutment box of the instance, that is the abutment box of the master cell to which has been applied the instance transformation." }
    , { "IsTerminal"                , (PyCFunction)PyInstance_IsTerminal                , METH_NOARGS , "Returns true if the instance is a terminal instance." }
    , { "IsLeaf"                    , (PyCFunction)PyInstance_IsLeaf                    , METH_NOARGS , "Returns true if the instance is a leaf instance." }
    , { "IsBound"                   , (PyCFunction)PyInstance_IsPyBound                 , METH_NOARGS, "Returns true if the instance is bounded to the hurricane instance" }
    , { "SetName"                   , (PyCFunction)PyInstance_SetName                   , METH_VARARGS, "Allows to change the instance name." }
    , { "SetTransformation"         , (PyCFunction)PyInstance_SetTransformation         , METH_VARARGS, "Allows to modify the instance transformation." }
    , { "SetPlacementStatus"        , (PyCFunction)PyInstance_SetPlacementStatus        , METH_VARARGS, "Allows to modify the instance placement status." }
    , { "SetMasterCell"             , (PyCFunction)PyInstance_SetMasterCell             , METH_VARARGS, "Allows to change the cell referenced by this instance." }
    , { "Delete"                    , (PyCFunction)PyInstance_Delete                    , METH_NOARGS
                                    , "Delete associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                "PyInstance" Object Methods                  |
  // x-------------------------------------------------------------x
  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_new ()"

  static PyObject* PyInstance_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyInstance_new ()" << endl;
    
    Instance* instance = NULL;
    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;

    HTRY
    __cs.Init ("Instance.new");
    if ( ! PyArg_ParseTuple(args,"O&O&O&|O&:Instance.new"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3
                           ) ) return ( NULL );

    if      ( __cs.GetObjectIds() == CELL_NAME_CELL_ARG       ) { instance = Instance::Create (  PYCELL_O(arg0)
                                                                                              , *PYNAME_O(arg1)
                                                                                              ,  PYCELL_O(arg2) ); }
    else if ( __cs.GetObjectIds() == CELL_NAME_CELL_TRANS_ARG ) { instance = Instance::Create (  PYCELL_O(arg0)
                                                                                              , *PYNAME_O(arg1)
                                                                                              ,  PYCELL_O(arg2)
                                                                                              , *PYTRANSFORMATION_O(arg3)
                                                                                              ,  Instance::PlacementStatus::PLACED); } else {
      cerr << __cs.GetObjectIds() << endl;
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Instance constructor." );
      return ( NULL );
    }
    HCATCH

      return PyInstance_Link ( instance );
  }

  DBoDeleteMethod(Instance)
  PyTypeObjectLinkPyType(Instance)
  PyTypeObjectConstructor(Instance)


# else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyInstance" Shared Library Code Part               |
// x=================================================================x





  // Link/Creation Method.
  DBoLinkCreateMethod(PyInstance_Link,PyInstance,PyTypeInstance,Instance)




  // ---------------------------------------------------------------
  // PyInstance Object Definitions.

  PyTypeInheritedObjectDefinitions(Instance, Entity)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 

