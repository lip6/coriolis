
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
// |  C++ Module  :  "./PyViaLayer.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyViaLayer.h"
#include "hurricane/isobar/PyTechnology.h"
#include "hurricane/isobar/PyBasicLayer.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
# define  METHOD_HEAD(function)    GENERIC_METHOD_HEAD(ViaLayer,viaLayer,function)


// +=================================================================+
// |              "PyViaLayer" Python Module Code Part               |
// +=================================================================+

# if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |              "PyViaLayer" Attribute Methods                 |
  // +-------------------------------------------------------------+


  static PyObject* PyViaLayer_create ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyViaLayer_create()" << endl;

    ViaLayer*   viaLayer     = NULL;
    
    HTRY
    PyObject*     pyTechnology = NULL;
    char*         name         = NULL;
    PyObject*     pyBottom     = NULL;
    PyObject*     pyCut        = NULL;
    PyObject*     pyTop        = NULL;
    
    if (PyArg_ParseTuple( args
                        , "OsOOO:ViaLayer.create"
                        , &pyTechnology
                        , &name
                        , &pyBottom
                        , &pyCut
                        , &pyTop
                        )) {
      if (not IsPyTechnology(pyTechnology)) {
        PyErr_SetString ( ConstructorError, "Via.create(): First argument is not of type Technology." );
        return NULL;
      }
      if (not IsPyBasicLayer(pyBottom)) {
        PyErr_SetString ( ConstructorError, "Via.create(): Third argument is not of type BasicLayer." );
        return NULL;
      }
      if (not IsPyBasicLayer(pyCut)) {
        PyErr_SetString ( ConstructorError, "Via.create(): Fourth argument is not of type BasicLayer." );
        return NULL;
      }
      if (not IsPyBasicLayer(pyTop)) {
        PyErr_SetString ( ConstructorError, "Via.create(): Fifth argument is not of type BasicLayer." );
        return NULL;
      }

      viaLayer = ViaLayer::create( PYTECHNOLOGY_O(pyTechnology)
                                 , Name(name)
                                 , PYBASICLAYER_O(pyBottom)
                                 , PYBASICLAYER_O(pyCut)
                                 , PYBASICLAYER_O(pyTop)
                                 );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to ViaLayer.create()." );
      return NULL;
    }
    HCATCH

    return PyViaLayer_Link(viaLayer);
  }


  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyViaLayer_destroy, PyViaLayer)


  // ---------------------------------------------------------------
  // PyViaLayer Attribute Method table.

  PyMethodDef PyViaLayer_Methods[] =
    { { "create"     , (PyCFunction)PyViaLayer_create     , METH_VARARGS|METH_STATIC
                     , "Create a new ViaLayer." }
    , { "destroy"    , (PyCFunction)PyViaLayer_destroy    , METH_NOARGS
                     , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  DBoDeleteMethod(ViaLayer)
  PyTypeObjectLinkPyType(ViaLayer)


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyViaLayer" Shared Library Code Part               |
// +=================================================================+


  // Link/Creation Method.
  DBoLinkCreateMethod(ViaLayer)


  PyTypeInheritedObjectDefinitions(ViaLayer, Layer)


#endif  // End of Shared Library Code Part.


}  // extern "C".

}  // Isobar namespace.
 
