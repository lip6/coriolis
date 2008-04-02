
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
// $Id: PyPath.cpp,v 1.6 2006/09/22 11:27:32 tsunami Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyPath.cpp"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "PyTransformation.h"
#include "PyName.h"
#include "PyPath.h"
#include "PyCell.h"
#include "PyInstance.h"
#include "PyInstanceLocator.h"


namespace Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Path,path,function)


// x=================================================================x
// |                "PyPath" Python Module Code Part                 |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                "PyPath" Attribute Methods                   |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyPath_isEmpty   ,isEmpty   ,PyPath,Path)


  // Standart Destroy (Attribute).
  DirectDestroyAttribute(PyPath_destroy, PyPath)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getHeadInstance ()"

  static PyObject* PyPath_getHeadInstance ( PyPath *self ) {
    trace << "PyPath_getHeadInstance()" << endl;

    METHOD_HEAD ( "Path.getHeadInstance()" )

    Instance* instance = NULL;

    HTRY
    instance = path->getHeadInstance();
    HCATCH    

    return PyInstance_Link ( instance );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getTailInstance ()"

  static PyObject* PyPath_getTailInstance ( PyPath *self ) {
    trace << "PyPath_getTailInstance()" << endl;

    METHOD_HEAD ( "Path.getTailInstance()" )

    Instance* instance = NULL;

    HTRY
    instance = path->getTailInstance();
    HCATCH    

    return PyInstance_Link ( instance );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getOwnerCell ()"

  static PyObject* PyPath_getOwnerCell ( PyPath *self ) {
    trace << "PyPath_getOwnerCell()" << endl;

    METHOD_HEAD ( "Path.getOwnerCell()" )

    Cell* cell = NULL;

    HTRY
    cell = path->getOwnerCell();
    HCATCH    

    return PyCell_Link ( cell );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getMasterCell ()"

  static PyObject* PyPath_getMasterCell ( PyPath *self ) {
    trace << "PyPath_getMasterCell()" << endl;

    METHOD_HEAD ( "Path.getMasterCell()" )

    Cell* cell = NULL;

    HTRY
    cell = path->getMasterCell();
    HCATCH    

    return PyCell_Link ( cell );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getName ()"

  static PyObject* PyPath_getName ( PyPath *self ) {
    trace << "PyPath_getName ()" << endl;
    
    METHOD_HEAD ( "Path.getName()" )

    PyName* pyName = PyObject_NEW ( PyName, &PyTypeName );
    if ( pyName == NULL ) { return NULL; }

    HTRY
    pyName->_object = new Name ( path->getName() );
    HCATCH
    
    return ( (PyObject*)pyName );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getHeadPath ()"

  static PyObject* PyPath_getHeadPath ( PyPath *self ) {

    trace << "PyPath_getHeadPath ()" << endl;
    
    METHOD_HEAD ( "Path.getHeadPath()" )

    PyPath* pyPath = PyObject_NEW ( PyPath, &PyTypePath );
    if ( pyPath == NULL ) { return NULL; }

    HTRY
    pyPath->_object = new Path ( path->getHeadPath() );
    HCATCH
    
    return ( (PyObject*)pyPath );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getTailPath ()"

  static PyObject* PyPath_getTailPath ( PyPath *self )
  {

    trace << "PyPath_getTailPath ()" << endl;
    
    METHOD_HEAD ( "Path.getTailPath()" )

    PyPath* pyPath = PyObject_NEW ( PyPath, &PyTypePath );
    if ( pyPath == NULL ) { return NULL; }

    HTRY
    pyPath->_object = new Path ( path->getTailPath() );
    HCATCH
    
    return ( (PyObject*)pyPath );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getTransformation ()"

  static PyObject* PyPath_getTransformation ( PyPath *self, PyObject* args )
  {
    trace << "PyPath_getTransformation ()" << endl;
    
    METHOD_HEAD ( "Instance.getTransformation()" );

    PyObject*         arg0;
    PyTransformation* pyTransf = PyObject_NEW ( PyTransformation, &PyTypeTransformation );
    if ( pyTransf == NULL ) { return NULL; }

    __cs.Init ("Path.getTransformation");
    if ( ! PyArg_ParseTuple(args,"|O&:Path.getTransformation",Converter,&arg0) ) return ( NULL );

    HTRY
    if      ( __cs.getObjectIds() == NO_ARG )
      { pyTransf->_object = new Transformation ( path->getTransformation () ); }
    else if ( __cs.getObjectIds() == TRANS_ARG )
      { pyTransf->_object = new Transformation ( path->getTransformation (*PYTRANSFORMATION_O(arg0)) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Path constructor." );
      return ( NULL );
    }
    
    HCATCH

    return ( (PyObject*)pyTransf );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_getInstancesLocator ()"

  static PyObject* PyPath_getInstancesLocator ( PyPath *self )
  {

    trace << "PyPath_getInstancesLocator ()" << endl;

    METHOD_HEAD ( "Path.getInstancesLocator()" )

    Instances instances = path->getInstances ();

    PyInstanceLocator* pyInstanceLocator = PyObject_NEW ( PyInstanceLocator, &PyTypeInstanceLocator );
    if (pyInstanceLocator == NULL) { return NULL; }

    HTRY
    pyInstanceLocator->_object = instances.getLocator ();
    HCATCH

    return ( (PyObject*)pyInstanceLocator );
  }




  // ---------------------------------------------------------------
  // PyPath Attribute Method table.

  PyMethodDef PyPath_Methods[] =
    { { "getHeadInstance"    , (PyCFunction)PyPath_getHeadInstance    , METH_NOARGS , "Return the head instance." }
    , { "getTailInstance"    , (PyCFunction)PyPath_getTailInstance    , METH_NOARGS , "Return the tail instance." }
    , { "getHeadPath"        , (PyCFunction)PyPath_getHeadPath        , METH_NOARGS , "Return the head path." }
    , { "getTailPath"        , (PyCFunction)PyPath_getTailPath        , METH_NOARGS , "Return the tail path." }
    , { "getOwnerCell"       , (PyCFunction)PyPath_getOwnerCell       , METH_NOARGS , "Return the cell owning the head instance." }
    , { "getMasterCell"      , (PyCFunction)PyPath_getMasterCell      , METH_NOARGS , "Returns the master cell referenced by the last instance of the path." }
    , { "getName"            , (PyCFunction)PyPath_getName            , METH_NOARGS , "Returns the concatenation of instances names." }
    , { "getTransformation"  , (PyCFunction)PyPath_getTransformation  , METH_VARARGS, "Return the resulting transformation." }
    , { "getInstancesLocator", (PyCFunction)PyPath_getInstancesLocator, METH_NOARGS , "Returns the collection of instances defining the path." }
    , { "isEmpty"            , (PyCFunction)PyPath_isEmpty            , METH_NOARGS , "Return true if the path is empty." }
    , { "destroy"            , (PyCFunction)PyPath_destroy            , METH_NOARGS
                             , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                  "PyPath" Object Methods                    |
  // x-------------------------------------------------------------x
  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_new ()"

  static PyObject* PyPath_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyPath_new()" << endl;

    Path*     path   = NULL;
    PyObject* arg0   = NULL;
    PyObject* arg1   = NULL;
    PyPath*   pyPath = NULL;

    __cs.Init ("Path.new");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Path.new"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ) ) return ( NULL );

    HTRY

    if      ( __cs.getObjectIds() == NO_ARG          ) { path = new Path (); }
    else if ( __cs.getObjectIds() == INST_ARG        ) { path = new Path (  PYINSTANCE_O(arg0) ); }
    else if ( __cs.getObjectIds() == INST_PATH_ARG   ) { path = new Path (  PYINSTANCE_O(arg0)
                                                                         , *PYPATH_O(arg1) ); }
    else if ( __cs.getObjectIds() == PATH_INST_ARG   ) { path = new Path ( *PYPATH_O(arg0)
                                                                         ,  PYINSTANCE_O(arg1) ); }
    else if ( __cs.getObjectIds() == CELL_STRING_ARG ) { path = new Path (  PYCELL_O(arg0) 
                                                                         ,  PyString_AsString(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Path constructor." );
      return ( NULL );
    }
    
    pyPath = PyObject_NEW(PyPath, &PyTypePath);
    if (pyPath == NULL) return NULL;
    
    pyPath->_object = path;

    HCATCH

    return ( (PyObject*)pyPath );
  }

  DirectDeleteMethod(PyPath_DeAlloc,PyPath)
  PyTypeObjectLinkPyType(Path)
  PyTypeObjectConstructor(Path)


# else  // End of Python Module Code Part.


// x=================================================================x
// |               "PyPath" Shared Library Code Part                 |
// x=================================================================x




  // ---------------------------------------------------------------
  // PyPath Object Definitions.
  
  PyTypeRootObjectDefinitions(Path)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
