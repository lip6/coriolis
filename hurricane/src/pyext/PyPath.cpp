
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


USING_NAMESPACE_HURRICANE

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
  DirectGetBoolAttribute(PyPath_IsEmpty   ,IsEmpty   ,PyPath,Path)


  // Standart Delete (Attribute).
  DirectDeleteAttribute(PyPath_Delete,PyPath)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_GetHeadInstance ()"

  static PyObject* PyPath_GetHeadInstance ( PyPath *self )
  {
    trace << "PyPath_GetHeadInstance()" << endl;

    METHOD_HEAD ( "Path.GetHeadInstance()" )

    Instance* instance = NULL;

    HTRY
    instance = path->GetHeadInstance();
    HCATCH    

    return PyInstance_Link ( instance );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_GetTailInstance ()"

  static PyObject* PyPath_GetTailInstance ( PyPath *self )
  {
    trace << "PyPath_GetTailInstance()" << endl;

    METHOD_HEAD ( "Path.GetTailInstance()" )

    Instance* instance = NULL;

    HTRY
    instance = path->GetTailInstance();
    HCATCH    

    return PyInstance_Link ( instance );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_GetOwnerCell ()"

  static PyObject* PyPath_GetOwnerCell ( PyPath *self )
  {
    trace << "PyPath_GetOwnerCell()" << endl;

    METHOD_HEAD ( "Path.GetOwnerCell()" )

    Cell* cell = NULL;

    HTRY
    cell = path->GetOwnerCell();
    HCATCH    

    return PyCell_Link ( cell );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_GetMasterCell ()"

  static PyObject* PyPath_GetMasterCell ( PyPath *self )
  {
    trace << "PyPath_GetMasterCell()" << endl;

    METHOD_HEAD ( "Path.GetMasterCell()" )

    Cell* cell = NULL;

    HTRY
    cell = path->GetMasterCell();
    HCATCH    

    return PyCell_Link ( cell );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_GetName ()"

  static PyObject* PyPath_GetName ( PyPath *self )
  {
    trace << "PyPath_GetName ()" << endl;
    
    METHOD_HEAD ( "Path.GetName()" )

    PyName* pyName = PyObject_NEW ( PyName, &PyTypeName );
    if ( pyName == NULL ) { return NULL; }

    HTRY
    pyName->_object = new Name ( path->GetName() );
    HCATCH
    
    return ( (PyObject*)pyName );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_GetHeadPath ()"

  static PyObject* PyPath_GetHeadPath ( PyPath *self )
  {

    trace << "PyPath_GetHeadPath ()" << endl;
    
    METHOD_HEAD ( "Path.GetHeadPath()" )

    PyPath* pyPath = PyObject_NEW ( PyPath, &PyTypePath );
    if ( pyPath == NULL ) { return NULL; }

    HTRY
    pyPath->_object = new Path ( path->GetHeadPath() );
    HCATCH
    
    return ( (PyObject*)pyPath );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_GetTailPath ()"

  static PyObject* PyPath_GetTailPath ( PyPath *self )
  {

    trace << "PyPath_GetTailPath ()" << endl;
    
    METHOD_HEAD ( "Path.GetTailPath()" )

    PyPath* pyPath = PyObject_NEW ( PyPath, &PyTypePath );
    if ( pyPath == NULL ) { return NULL; }

    HTRY
    pyPath->_object = new Path ( path->GetTailPath() );
    HCATCH
    
    return ( (PyObject*)pyPath );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_GetTransformation ()"

  static PyObject* PyPath_GetTransformation ( PyPath *self, PyObject* args )
  {
    trace << "PyPath_GetTransformation ()" << endl;
    
    METHOD_HEAD ( "Instance.GetTransformation()" );

    PyObject*         arg0;
    PyTransformation* pyTransf = PyObject_NEW ( PyTransformation, &PyTypeTransformation );
    if ( pyTransf == NULL ) { return NULL; }

    __cs.Init ("Path.GetTransformation");
    if ( ! PyArg_ParseTuple(args,"|O&:Path.GetTransformation",Converter,&arg0) ) return ( NULL );

    HTRY
    if      ( __cs.GetObjectIds() == NO_ARG )
      { pyTransf->_object = new Transformation ( path->GetTransformation () ); }
    else if ( __cs.GetObjectIds() == TRANS_ARG )
      { pyTransf->_object = new Transformation ( path->GetTransformation (*PYTRANSFORMATION_O(arg0)) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Path constructor." );
      return ( NULL );
    }
    
    HCATCH

    return ( (PyObject*)pyTransf );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPath_GetInstancesLocator ()"

  static PyObject* PyPath_GetInstancesLocator ( PyPath *self )
  {

    trace << "PyPath_GetInstancesLocator ()" << endl;

    METHOD_HEAD ( "Path.GetInstancesLocator()" )

    Instances instances = path->GetInstances ();

    PyInstanceLocator* pyInstanceLocator = PyObject_NEW ( PyInstanceLocator, &PyTypeInstanceLocator );
    if (pyInstanceLocator == NULL) { return NULL; }

    HTRY
    pyInstanceLocator->_object = instances.GetLocator ();
    HCATCH

    return ( (PyObject*)pyInstanceLocator );
  }




  // ---------------------------------------------------------------
  // PyPath Attribute Method table.

  PyMethodDef PyPath_Methods[] =
    { { "GetHeadInstance"    , (PyCFunction)PyPath_GetHeadInstance    , METH_NOARGS , "Return the head instance." }
    , { "GetTailInstance"    , (PyCFunction)PyPath_GetTailInstance    , METH_NOARGS , "Return the tail instance." }
    , { "GetHeadPath"        , (PyCFunction)PyPath_GetHeadPath        , METH_NOARGS , "Return the head path." }
    , { "GetTailPath"        , (PyCFunction)PyPath_GetTailPath        , METH_NOARGS , "Return the tail path." }
    , { "GetOwnerCell"       , (PyCFunction)PyPath_GetOwnerCell       , METH_NOARGS , "Return the cell owning the head instance." }
    , { "GetMasterCell"      , (PyCFunction)PyPath_GetMasterCell      , METH_NOARGS , "Returns the master cell referenced by the last instance of the path." }
    , { "GetName"            , (PyCFunction)PyPath_GetName            , METH_NOARGS , "Returns the concatenation of instances names." }
    , { "GetTransformation"  , (PyCFunction)PyPath_GetTransformation  , METH_VARARGS, "Return the resulting transformation." }
    , { "GetInstancesLocator", (PyCFunction)PyPath_GetInstancesLocator, METH_NOARGS , "Returns the collection of instances defining the path." }
    , { "IsEmpty"            , (PyCFunction)PyPath_IsEmpty            , METH_NOARGS , "Return true if the path is empty." }
    , { "Delete"             , (PyCFunction)PyPath_Delete             , METH_NOARGS
                             , "Delete associated hurricane object, the python object remains." }
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

    if      ( __cs.GetObjectIds() == NO_ARG          ) { path = new Path (); }
    else if ( __cs.GetObjectIds() == INST_ARG        ) { path = new Path (  PYINSTANCE_O(arg0) ); }
    else if ( __cs.GetObjectIds() == INST_PATH_ARG   ) { path = new Path (  PYINSTANCE_O(arg0)
                                                                         , *PYPATH_O(arg1) ); }
    else if ( __cs.GetObjectIds() == PATH_INST_ARG   ) { path = new Path ( *PYPATH_O(arg0)
                                                                         ,  PYINSTANCE_O(arg1) ); }
    else if ( __cs.GetObjectIds() == CELL_STRING_ARG ) { path = new Path (  PYCELL_O(arg0) 
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
 
