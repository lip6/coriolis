
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
// $Id: PyOccurrence.cpp,v 1.7 2007/05/02 11:50:46 d2 Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyOccurrence.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "PyBox.h"
#include "PyTransformation.h"
#include "PyName.h"
#include "PyPath.h"
#include "PyOccurrence.h"
#include "PyCell.h"
#include "PySegment.h"


namespace  Isobar {


USING_NAMESPACE_HURRICANE

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Occurrence,occurrence,function)


// x=================================================================x
// |             "PyOccurrence" Python Module Code Part              |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |              "PyOccurrence" Attribute Methods               |
  // x-------------------------------------------------------------x



  // Standart Accessors (Attributes).


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyOccurrence_IsValid    ,IsValid    ,PyOccurrence,Occurrence)
  DirectGetBoolAttribute(PyOccurrence_HasProperty,HasProperty,PyOccurrence,Occurrence)


  // Standart Delete (Attribute).
  DirectDeleteAttribute(PyOccurrence_Delete,PyOccurrence)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_GetEntity ()"

  static PyObject* PyOccurrence_GetEntity ( PyOccurrence *self )
  {
    trace << "PyOccurrence_GetEntity()" << endl;

    METHOD_HEAD ( "Occurrence.GetEntity()" )

    PyObject* pyEntity = NULL;

    HTRY
    Entity* entity = occurrence->GetEntity();
    if ( entity == NULL )
        Py_RETURN_NONE;

    pyEntity = PyEntity_NEW ( entity );
    HCATCH    

    return pyEntity;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_GetOwnerCell ()"

  static PyObject* PyOccurrence_GetOwnerCell ( PyOccurrence *self )
  {
    trace << "PyOccurrence_GetOwnerCell()" << endl;

    METHOD_HEAD ( "Occurernce.GetOwnerCell()" )

    Cell* cell = NULL;

    HTRY
    cell = occurrence->GetOwnerCell();
    HCATCH    

    return PyCell_Link ( cell );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_GetMasterCell ()"

  static PyObject* PyOccurrence_GetMasterCell ( PyOccurrence *self )
  {
    trace << "PyOccurrence_GetMasterCell()" << endl;

    METHOD_HEAD ( "Occurrence.GetMasterCell()" )

    Cell* cell = NULL;

    HTRY
    cell = occurrence->GetMasterCell();
    HCATCH    

    return PyCell_Link ( cell );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_GetPath ()"

  static PyObject* PyOccurrence_GetPath ( PyOccurrence *self )
  {
    trace << "PyOccurrence_GetPath ()" << endl;
    
    METHOD_HEAD ( "Occurrence.GetPath()" )

    PyPath* pyPath = PyObject_NEW ( PyPath, &PyTypePath );
    if ( pyPath == NULL ) { return NULL; }

    HTRY
    pyPath->_object = new Path ( occurrence->GetPath() );
    HCATCH
    
    return ( (PyObject*)pyPath );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_GetName ()"

  static PyObject* PyOccurrence_GetName ( PyOccurrence *self )
  {
    trace << "PyOccurrence_GetName ()" << endl;
    
    METHOD_HEAD ( "Occurrence.GetName()" )

    PyName* pyName = PyObject_NEW ( PyName, &PyTypeName );
    if ( pyName == NULL ) { return NULL; }

    HTRY
    pyName->_object = new Name ( occurrence->GetName() );
    HCATCH
    
    return ( (PyObject*)pyName );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_GetBoundingBox ()"

  static PyObject* PyOccurrence_GetBoundingBox ( PyOccurrence *self )
  {
    trace << "PyOccurrence_GetBoundingBox ()" << endl;
    
    METHOD_HEAD ( "Occurrence.GetBoundingBox()" )

    PyBox* pyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if ( pyBox == NULL ) { return NULL; }

    HTRY
    pyBox->_object = new Box ( occurrence->GetBoundingBox() );
    HCATCH
    
    return ( (PyObject*)pyBox );
  }




  // ---------------------------------------------------------------
  // PyOccurrence Attribute Method table.

  PyMethodDef PyOccurrence_Methods[] =
    { { "GetEntity"     , (PyCFunction)PyOccurrence_GetEntity     , METH_NOARGS, "Returns the referenced entity." }
    , { "GetPath"       , (PyCFunction)PyOccurrence_GetPath       , METH_NOARGS, "Returns the hierarchical instaciation path." }
    , { "GetName"       , (PyCFunction)PyOccurrence_GetName       , METH_NOARGS, "Returns the occurrence name (concatenation of path and entity name." }
    , { "GetOwnerCell"  , (PyCFunction)PyOccurrence_GetOwnerCell  , METH_NOARGS, "Returns the occurrence owner cell." }
    , { "GetMasterCell" , (PyCFunction)PyOccurrence_GetMasterCell , METH_NOARGS, "Returns the cell owning the referenced entity." }
    , { "GetBoundingBox", (PyCFunction)PyOccurrence_GetBoundingBox, METH_NOARGS, "Returns the occurrence bounding box." }
    , { "IsValid"       , (PyCFunction)PyOccurrence_IsValid       , METH_NOARGS, "Returns true if the occurrence is valid." }
    , { "HasProperty"   , (PyCFunction)PyOccurrence_HasProperty   , METH_NOARGS, "Returns true if the occurrence owns some properties." }
    , { "Delete"        , (PyCFunction)PyOccurrence_Delete        , METH_NOARGS
                        , "Delete associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |               "PyOccurrence" Object Methods                 |
  // x-------------------------------------------------------------x

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_new ()"

  static PyObject* PyOccurrence_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyOccurrence_new()" << endl;

    Occurrence* occurrence;
    PyObject*   arg0;
    PyObject*   arg1;

    __cs.Init ("Occurrence.new");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Occurrence.new"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ) ) return ( NULL );

    if      ( __cs.GetObjectIds() == NO_ARG       ) { occurrence = new Occurrence (); }
    else if ( __cs.GetObjectIds() == ENT_ARG      ) { occurrence = new Occurrence (  PYENTITY_O(arg0) ); }
    else if ( __cs.GetObjectIds() == COMP_PATH_ARG) { occurrence = new Occurrence (  PYSEGMENT_O(arg0)
                                                                                  , *PYPATH_O(arg1) ); }
    else if ( __cs.GetObjectIds() == ENT_PATH_ARG ) { occurrence = new Occurrence (  PYENTITY_O(arg0)
                                                                                  , *PYPATH_O(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Occurrence constructor. " );
      return ( NULL );
    }

    PyOccurrence* pyOccurrence = PyObject_NEW(PyOccurrence, &PyTypeOccurrence);
    if (pyOccurrence == NULL) return NULL;
    
    HTRY
    pyOccurrence->_object = occurrence;
    HCATCH

    return ( (PyObject*)pyOccurrence );
  }



  DirectDeleteMethod(PyOccurrence_DeAlloc,PyOccurrence)
  PyTypeObjectLinkPyType(Occurrence)
  PyTypeObjectConstructor(Occurrence)


# else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyOccurrence" Shared Library Code Part              |
// x=================================================================x





  // ---------------------------------------------------------------
  // PyOccurrence Object Definitions.
  
  PyTypeRootObjectDefinitions(Occurrence)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
