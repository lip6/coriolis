
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
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :          Damien.Dupuis@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyTechnology.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyDataBase.h"
#include "hurricane/isobar/PyTechnology.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyLayerMask.h"
#include "hurricane/isobar/PyBasicLayer.h"
#include "hurricane/isobar/PyRegularLayer.h"
#include "hurricane/isobar/PyViaLayer.h"
#include "hurricane/isobar/PyLayerCollection.h"
#include "hurricane/isobar/PyBasicLayerCollection.h"
#include "hurricane/isobar/PyRegularLayerCollection.h"
#include "hurricane/isobar/PyViaLayerCollection.h"
#include "hurricane/isobar/PyPhysicalRule.h"
#include "hurricane/isobar/PyDeviceDescriptor.h"


namespace Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Technology,techno,function)


// +=================================================================+
// |             "PyTechnology" Python Module Code Part              |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |              "PyTechnology" Attribute Methods               |
  // +-------------------------------------------------------------+


  static PyObject* PyTechnology_create ( PyTechnology*, PyObject* args )
  {
    cdebug_log(20,0) << "Technology.create()" << endl;

    Technology* technology = NULL;

    HTRY
    PyDataBase* pyDb = NULL;
    char*       name = NULL;
    if (PyArg_ParseTuple(args,"Os:Technology.create", &pyDb, &name)) {
      if (not IsPyDataBase(pyDb) ) {
        PyErr_SetString(ConstructorError,"Technology.create(): First parameter is not a DataBase.");
        return NULL;
      }
      technology = Technology::create( PYDATABASE_O(pyDb), Name(name) );
    } else {
      PyErr_SetString(ConstructorError, "Technology.create(): Bad parameter(s) number or type.");
      return NULL;
    }
    HCATCH
    
    return PyTechnology_Link(technology);
  }


  static PyObject* PyTechnology_getDataBase ( PyTechnology* self )
  {
    cdebug_log(20,0) << "PyTechnology_getDataBase()" << endl;

    DataBase* db = NULL;

    HTRY
    METHOD_HEAD("Technology.getDataBase()")
    db = techno->getDataBase();
    HCATCH

    if (db == NULL) Py_RETURN_NONE;
    return PyDataBase_Link(db);
  }


  static PyObject* PyTechnology_getLayer ( PyTechnology *self, PyObject* args )
  {
    cdebug_log(20,0) << "Technology.getLayer()" << endl;

    METHOD_HEAD("Technology.getLayer()")

    Layer* layer = NULL;
    
    HTRY
    PyObject* arg0 = NULL;
    PyObject* arg1 = NULL;
    if (PyArg_ParseTuple(args,"O|OO:Technology.getLayer", &arg0, &arg1)) {
      if (PyString_Check(arg0)) {
        layer = techno->getLayer(Name(PyString_AsString(arg0)));
      } else if (IsPyLayerMask(arg0)) {
        bool useSymbolic = (arg1 != NULL) ? PyObject_IsTrue(arg1) : true;
        layer = techno->getLayer(PYLAYERMASK_O(arg0), useSymbolic);
      } else {
        PyErr_SetString(ConstructorError, "invalid number of parameters for getLayer.");
        return NULL;
      }
    } else {
      PyErr_SetString(ConstructorError, "Hurricane.getLayer(): Bad parameter(s) type.");
      return NULL;
    }
    HCATCH
    
    return PyLayer_LinkDerived(layer);
  }


  static PyObject* PyTechnology_getBasicLayers ( PyTechnology* self, PyObject* args )
  {
    cdebug_log(20,0) << "PyTechnology_getBasicLayers()" << endl;

    PyBasicLayerCollection* pyObjects = NULL;

    HTRY
    METHOD_HEAD("Technology.getBasicLayers()")

    PyObject*    pyMask  = NULL;
    BasicLayers* objects = NULL;

    if (PyArg_ParseTuple( args, "O:Technology.getBasicLayers()", &pyMask)) {
      if (not IsPyLayerMask(pyMask)) {
        PyErr_SetString ( ConstructorError
                        , "Technology.getBasicLayers(): First argument is not of Layer.Mask type." );
        return NULL;
      }
    } else {
      PyErr_SetString ( ConstructorError
                      , "Invalid number of parameters passed to Technology.getBasicLayers()." );
      return NULL;
    }

    if (pyMask != NULL)
      objects = new BasicLayers(techno->getBasicLayers(PYLAYERMASK_O(pyMask)));
    else
      objects = new BasicLayers(techno->getBasicLayers());

    pyObjects = PyObject_NEW(PyBasicLayerCollection,&PyTypeBasicLayerCollection);
    if (pyObjects == NULL) return NULL;
    pyObjects->_object = objects;
    HCATCH

    return (PyObject*)pyObjects;
  }


  static PyObject* PyTechnology_setSymbolicLayer ( PyTechnology *self, PyObject* args ) {
    cdebug_log(20,0) << "Technology.setSymbolicLayer()" << endl;

    METHOD_HEAD("Technology.setSymbolicLayer()")

    bool rvalue = false;

    HTRY
    PyObject* arg0 = NULL;
    if (PyArg_ParseTuple(args,"O:Technology.setSymbolicLayer", &arg0)) {
      if (PyString_Check(arg0)) {
        rvalue = techno->setSymbolicLayer(Name(PyString_AsString(arg0)));
      } else if (IsPyLayer(arg0)) {
        rvalue = techno->setSymbolicLayer(PYLAYER_O(arg0));
      } else {
        PyErr_SetString(ConstructorError, "Hurricane.setSymbolicLayer(): Invalid number of parameters.");
        return NULL;
      }
    } else {
      PyErr_SetString(ConstructorError, "Hurricane.setSymbolicLayer(): Bad parameter type.");
      return NULL;
    }
    HCATCH
    
    if (rvalue) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  static PyObject* PyTechnology_getUnitRule ( PyTechnology *self, PyObject* args )
  {
    cdebug.log(49) << "PyTechnology_getUnitRule()" << endl;
    METHOD_HEAD("Technology.getUnitRule()")

    PhysicalRule* rule = NULL;

    HTRY
      char *arg0 = NULL;
      if (not PyArg_ParseTuple(args,"s:Technology.getUnitRule", &arg0)) {
        PyErr_SetString( ProxyError
                       , "invalid number of parameters for getUnitRule on Technology." );
        return NULL;
      }
      if (arg0) {
        rule = techno->getUnitRule( arg0 );
      } else {
        PyErr_SetString( ProxyError, "invalid number of parameters for getUnitRule on Technology." );
        return NULL;
      }
    HCATCH

    return PyRule_LinkDerived(rule);
  }


  static PyObject* PyTechnology_getPhysicalRule ( PyTechnology *self, PyObject* args )
  {
    cdebug.log(49) << "PyTechnology_getPhysicalRule()" << endl;
    METHOD_HEAD("Technology.getPhysicalRule()")

    PhysicalRule* rule = NULL;

    HTRY
      char *arg0=NULL, *arg1=NULL, *arg2=NULL;
      if (not PyArg_ParseTuple(args,"s|ss:Technology.getPhysicalRule", &arg0, &arg1, &arg2)) {
        PyErr_SetString( ProxyError
                       , "invalid number of parameters for getPhysicalRule on Technology." );
        return NULL;
      }
      if (arg2) {
        rule = techno->getPhysicalRule( arg0, arg1, arg2 );
      } else if (arg1) {
        rule = techno->getPhysicalRule( arg0, arg1 );
      } else if (arg0) {
        rule = techno->getPhysicalRule( arg0 );
      } else {
        PyErr_SetString( ProxyError, "invalid number of parameters for getPhysicalRule on Technology." );
        return NULL;
      }
    HCATCH

    return PyRule_LinkDerived(rule);
  }


  static PyObject* PyTechnology_addUnitRule ( PyTechnology* self, PyObject *args )
  {
    cdebug.log(49) << "PyTechnology_addUnitRule()" << endl;
    METHOD_HEAD("Technology.addUnitRule()")

    PyObject*     arg0 = NULL;
    PyObject*     arg1 = NULL;
    PhysicalRule* rule = NULL;

    HTRY
      __cs.init ("Technology.addUnitRule");
      if (not PyArg_ParseTuple(args,"O&O&:Technology.addUnitRule"
                              ,Converter,&arg0
                              ,Converter,&arg1
                              )) {
        PyErr_SetString( ConstructorError, "Technology.addUnitRule(): Invalid number of parameters." );
        return NULL;
      }
      if (__cs.getObjectIds() == ":string:string")
        rule = techno->addUnitRule( PyString_AsString(arg0)
                                  , PyString_AsString(arg1) );
      else {
        PyErr_SetString( ConstructorError, "Technology.addUnitRule(): Bad type of parameters." );
        return NULL;
      }
    HCATCH

    return PyRule_LinkDerived(rule);
  }


  static PyObject* PyTechnology_addPhysicalRule ( PyTechnology* self, PyObject *args )
  {
    cdebug.log(49) << "PyTechnology_addPhysicalRule()" << endl;
    METHOD_HEAD("Technology.addPhysicalRule()")

    PyObject*     arg0 = NULL;
    PyObject*     arg1 = NULL;
    PyObject*     arg2 = NULL;
    PyObject*     arg3 = NULL;
    PhysicalRule* rule = NULL;

    HTRY
      __cs.init ("Technology.addPhysicalRule");
      if (not PyArg_ParseTuple(args,"O&O&|O&O&:Technology.addPhysicalRule"
                              ,Converter,&arg0
                              ,Converter,&arg1
                              ,Converter,&arg2
                              ,Converter,&arg3
                              )) {
        PyErr_SetString( ConstructorError, "Technology.addPhysicalRule(): invalid number of parameters." );
        return NULL;
      }
      if (__cs.getObjectIds() == ":string:string")
        rule = techno->addPhysicalRule( PyString_AsString(arg0)
                                      , PyString_AsString(arg1) );
      else if (__cs.getObjectIds() == ":string:string:string")
        rule = techno->addPhysicalRule( PyString_AsString(arg0)
                                      , PyString_AsString(arg1)
                                      , PyString_AsString(arg2) );
      else if (__cs.getObjectIds() == ":string:string:string:string")
        rule = techno->addPhysicalRule( PyString_AsString(arg0)
                                      , PyString_AsString(arg1)
                                      , PyString_AsString(arg2)
                                      , PyString_AsString(arg3) );
      else {
        PyErr_SetString( ConstructorError, "Technology.addPhysicalRule(): Invalid number or bad type of parameters." );
        return NULL;
      }
    HCATCH

    return PyRule_LinkDerived(rule);
  }


  extern PyObject* PyTechnology_addDeviceDescriptor ( PyTechnology* self, PyObject* args )
  {
    cdebug.log(49) << "PyTechnology_addDeviceDescriptor ()" << endl;
    DeviceDescriptor* devDesc = NULL;

    HTRY
      METHOD_HEAD("Technology.addDeviceDescriptor()")
  
      char* name   = NULL;
      if (PyArg_ParseTuple(args,"s:Technology.addDeviceDescriptor", &name)) {
        devDesc = techno->addDeviceDescriptor( name );
      } else {
        PyErr_SetString( ConstructorError
                       , "Technology::addDeviceDescriptor(): Invalid number of parameters." );
        return NULL;
      }
    HCATCH

    return PyDeviceDescriptor_Link( devDesc );
  }


  extern PyObject* PyTechnology_getDeviceDescriptor ( PyTechnology* self, PyObject* args )
  {
    cdebug.log(49) << "PyTechnology_getDeviceDescriptor ()" << endl;
    DeviceDescriptor* devDesc = NULL;

    HTRY
      METHOD_HEAD("Technology.getDeviceDescriptor()")
  
      char* name   = NULL;
      if (PyArg_ParseTuple(args,"s:Technology.getDeviceDescriptor", &name)) {
        devDesc = techno->getDeviceDescriptor( name );
      } else {
        PyErr_SetString( ConstructorError
                       , "Technology::getDeviceDescriptor(): Invalid number of parameters." );
        return NULL;
      }
    HCATCH

    return PyDeviceDescriptor_Link( devDesc );
  }


  // Standart Accessors (Attributes).
  GetNameMethod                     (Technology,techno)
  SetNameMethod                     (Technology,techno)
  predicateFromLayer                (isMetal         ,PyTechnology,Technology)
  accessorAnyLayerFromName          (getBasicLayer   ,PyTechnology,Technology,BasicLayer  )
  accessorAnyLayerFromName          (getRegularLayer ,PyTechnology,Technology,RegularLayer)
  accessorAnyLayerFromName          (getViaLayer     ,PyTechnology,Technology,ViaLayer    )
  accessorCollectionFromVoid        (getLayers       ,PyTechnology,Technology,Layer       )
  accessorCollectionFromVoid        (getRegularLayers,PyTechnology,Technology,RegularLayer)
  accessorCollectionFromVoid        (getViaLayers    ,PyTechnology,Technology,ViaLayer    )
  accessorLayerFromLayerOptBool     (getMetalAbove   ,PyTechnology,Technology)
  accessorLayerFromLayerOptBool     (getMetalBelow   ,PyTechnology,Technology)
  accessorLayerFromLayerOptBool     (getCutAbove     ,PyTechnology,Technology)
  accessorLayerFromLayerOptBool     (getCutBelow     ,PyTechnology,Technology)
  accessorLayerFromLayerLayerOptBool(getViaBetween   ,PyTechnology,Technology)
  accessorLayerFromInt              (getNthMetal     ,PyTechnology,Technology)

  // Standard destroy (Attribute).
  DBoDestroyAttribute(PyTechnology_destroy, PyTechnology)


  // ---------------------------------------------------------------
  // PyTechnology Attribute Method table.

  PyMethodDef PyTechnology_Methods[] =
    { { "create"             , (PyCFunction)PyTechnology_create               , METH_VARARGS|METH_STATIC
                             , "Create the Technology object." }
    , { "isMetal"            , (PyCFunction)PyTechnology_isMetal              , METH_VARARGS
                             , "Tells if the given layer is of metal kind (Material)." }
    , { "getDataBase"        , (PyCFunction)PyTechnology_getDataBase          , METH_NOARGS
                             , "Returns the associated DataBase." }
    , { "getName"            , (PyCFunction)PyTechnology_getName              , METH_NOARGS
                             , "Returns the name of the technology." }
    , { "getLayer"           , (PyCFunction)PyTechnology_getLayer             , METH_VARARGS
                             , "Returns the layer named name." }
    , { "getBasicLayer"      , (PyCFunction)PyTechnology_getBasicLayer        , METH_VARARGS
                             , "Returns the BasicLayer named name." }
    , { "getRegularLayer"    , (PyCFunction)PyTechnology_getRegularLayer      , METH_VARARGS
                             , "Returns the RegularLayer named name." }
    , { "getViaLayer"        , (PyCFunction)PyTechnology_getViaLayer          , METH_VARARGS
                             , "Returns the ViaLayer named name." }
    , { "getLayers"          , (PyCFunction)PyTechnology_getLayers            , METH_NOARGS
                             , "Returns the collection of all Layers." }
    , { "getBasicLayers"     , (PyCFunction)PyTechnology_getBasicLayers       , METH_VARARGS
                             , "Returns the collection of all BasicLayers." }
    , { "getRegularLayers"   , (PyCFunction)PyTechnology_getRegularLayers     , METH_NOARGS
                             , "Returns the collection of all RegularLayers." }
    , { "getViaLayers"       , (PyCFunction)PyTechnology_getViaLayers         , METH_NOARGS
                             , "Returns the collection of all BasicLayers." }
    , { "getMetalAbove"      , (PyCFunction)PyTechnology_getMetalAbove        , METH_VARARGS
                             , "Returns the metal layer immediatly above this one." }
    , { "getMetalBelow"      , (PyCFunction)PyTechnology_getMetalBelow        , METH_VARARGS
                             , "Returns the metal layer immediatly below this one." }
    , { "getCutAbove"        , (PyCFunction)PyTechnology_getCutAbove          , METH_VARARGS
                             , "Returns the cut layer immediatly above this one." }
    , { "getCutBelow"        , (PyCFunction)PyTechnology_getCutBelow          , METH_VARARGS
                             , "Returns the cut layer immediatly below." }
    , { "getViaBetween"      , (PyCFunction)PyTechnology_getViaBetween        , METH_VARARGS
                             , "Returns the VIA between those two layers (must be adjacent)." }
    , { "getNthMetal"        , (PyCFunction)PyTechnology_getNthMetal          , METH_VARARGS
                             , "Returns Nth metal (zero is nearest substrate)." }
    , { "setName"            , (PyCFunction)PyTechnology_setName              , METH_VARARGS
                             , "Allows to change the technology name." }
    , { "setSymbolicLayer"   , (PyCFunction)PyTechnology_setSymbolicLayer     , METH_VARARGS
                             , "Mark a Layer as the symbolic one (by name or by Layer)." }
    , { "getUnitRule"        , (PyCFunction)PyTechnology_getUnitRule          , METH_VARARGS
                             , "Returns the Unit Rule named name." }
    , { "getPhysicalRule"    , (PyCFunction)PyTechnology_getPhysicalRule      , METH_VARARGS
                             , "Returns the Physical Rule named name." }
    , { "getLayer"           , (PyCFunction)PyTechnology_getLayer             , METH_VARARGS
                             , "Returns the Layer named name." }
    , { "addPhysicalRule"    , (PyCFunction)PyTechnology_addPhysicalRule      , METH_VARARGS
                             , "Adds a new physical rule." }
    , { "addUnitRule"        , (PyCFunction)PyTechnology_addUnitRule          , METH_VARARGS
                             , "Adds a new Unit rule." }
    , { "getDeviceDescriptor", (PyCFunction)PyTechnology_getDeviceDescriptor  , METH_VARARGS
                             , "Get the DeviceDescriptor <name>." }
    , { "addDeviceDescriptor", (PyCFunction)PyTechnology_addDeviceDescriptor  , METH_VARARGS
                             , "Add (create) the DeviceDescriptor <name>." }
    , { "destroy"            , (PyCFunction)PyTechnology_destroy              , METH_NOARGS
                             , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  DBoDeleteMethod(Technology)
  PyTypeObjectLinkPyType(Technology)


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyTechnology" Shared Library Code Part              |
// +=================================================================+

  // Link/Creation Method.
  DBoLinkCreateMethod(Technology)

  // ---------------------------------------------------------------
  // PyTechnology Object Definitions.

  PyTypeObjectDefinitions(Technology)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
 
