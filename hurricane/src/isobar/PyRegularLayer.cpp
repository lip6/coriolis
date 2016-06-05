
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
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyRegularLayer.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyRegularLayer.h"
#include "hurricane/isobar/PyMaterial.h"
#include "hurricane/isobar/PyBasicLayer.h"
#include "hurricane/isobar/PyTechnology.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
# define  METHOD_HEAD(function)    GENERIC_METHOD_HEAD(RegularLayer,regularLayer,function)


// +=================================================================+
// |            "PyRegularLayer" Python Module Code Part             |
// +=================================================================+

# if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |            "PyRegularLayer" Attribute Methods               |
  // +-------------------------------------------------------------+


  static PyObject* PyRegularLayer_create ( PyObject*, PyObject* args )
  {
    cdebug.log(20) << "PyRegularLayer_create()" << endl;

    RegularLayer*   regularLayer     = NULL;
    
    HTRY
    PyObject*     pyTechnology   = NULL;
    char*         name           = NULL;
    
    if (PyArg_ParseTuple( args
                        , "Os:RegularLayer.create"
                        , &pyTechnology
                        , &name
                        )) {
      if (not IsPyTechnology(pyTechnology)) {
        PyErr_SetString ( ConstructorError, "RegularLayer.create(): First argument is not of type Technology." );
        return NULL;
      }

      regularLayer = RegularLayer::create( PYTECHNOLOGY_O(pyTechnology), Name(name) );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to RegularLayer.create()." );
      return NULL;
    }
    HCATCH

    return PyRegularLayer_Link(regularLayer);
  }


  // Standart Attribute.
  DirectGetLongAttribute(PyRegularLayer_getExtentionCap  ,getExtentionCap  ,PyRegularLayer,RegularLayer)
  DirectGetLongAttribute(PyRegularLayer_getExtentionWidth,getExtentionWidth,PyRegularLayer,RegularLayer)

  accessorLayerFromVoid(getBasicLayer         ,PyRegularLayer,RegularLayer)
  updatorFromBasicLayer(setBasicLayer         ,PyRegularLayer,RegularLayer)
  DBoDestroyAttribute  (PyRegularLayer_destroy,PyRegularLayer)


  // ---------------------------------------------------------------
  // PyRegularLayer Attribute Method table.

  PyMethodDef PyRegularLayer_Methods[] =
    { { "create"           , (PyCFunction)PyRegularLayer_create           , METH_VARARGS|METH_STATIC
                           , "Create a new RegularLayer." }
    , { "getExtentionWidth", (PyCFunction)PyRegularLayer_getExtentionWidth, METH_NOARGS
                           , "Return the width extension (delta)." }
    , { "getExtentionCap"  , (PyCFunction)PyRegularLayer_getExtentionCap  , METH_NOARGS
                           , "Return the cap extension (begin/end delta)." }
    , { "getBasicLayer"    , (PyCFunction)PyRegularLayer_getBasicLayer    , METH_NOARGS
                           , "Get the BasicLayer associated to this RegularLayer." }
    , { "setBasicLayer"    , (PyCFunction)PyRegularLayer_setBasicLayer    , METH_VARARGS
                           , "Associate a BasicLayer with this RegularLayer." }
    , { "destroy"          , (PyCFunction)PyRegularLayer_destroy          , METH_NOARGS
                           , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  DBoDeleteMethod(RegularLayer)
  PyTypeObjectLinkPyType(RegularLayer)


#else  // End of Python Module Code Part.


// +=================================================================+
// |           "PyRegularLayer" Shared Library Code Part             |
// +=================================================================+


  // Link/Creation Method.
  DBoLinkCreateMethod(RegularLayer)


  PyTypeInheritedObjectDefinitions(RegularLayer, Layer)


#endif  // End of Shared Library Code Part.


}  // extern "C".

}  // Isobar namespace.
 
