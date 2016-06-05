
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
// |  C++ Module  :  "./PyContactLayer.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyContactLayer.h"
#include "hurricane/isobar/PyTechnology.h"
#include "hurricane/isobar/PyBasicLayer.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
# define  METHOD_HEAD(function)    GENERIC_METHOD_HEAD(ContactLayer,contactLayer,function)


// +=================================================================+
// |            "PyContactLayer" Python Module Code Part             |
// +=================================================================+

# if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |            "PyContactLayer" Attribute Methods               |
  // +-------------------------------------------------------------+


  static PyObject* PyContactLayer_create ( PyObject*, PyObject* args )
  {
    cdebug.log(20) << "PyContactLayer_create()" << endl;

    ContactLayer*   contactLayer     = NULL;
    
    HTRY
    PyObject*     pyTechnology   = NULL;
    char*         name           = NULL;
    PyObject*     pyMetal        = NULL;
    PyObject*     pyCut          = NULL;
    PyObject*     pyActive       = NULL;
    PyObject*     pyDiffusion    = NULL;
    PyObject*     pyWell         = NULL;
    
    if (PyArg_ParseTuple( args
                        , "OsOOOOO:ContactLayer.create"
                        , &pyTechnology
                        , &name
                        , &pyMetal
                        , &pyCut
                        , &pyActive
                        , &pyDiffusion
                        , &pyWell
                        )) {
      if (not IsPyTechnology(pyTechnology)) {
        PyErr_SetString ( ConstructorError, "Contact.create(): First argument is not of type Technology." );
        return NULL;
      }
      if (not IsPyBasicLayer(pyMetal)) {
        PyErr_SetString ( ConstructorError, "Contact.create(): Third argument is not of type BasicLayer." );
        return NULL;
      }
      if (not IsPyBasicLayer(pyCut)) {
        PyErr_SetString ( ConstructorError, "Contact.create(): Fourth argument is not of type BasicLayer." );
        return NULL;
      }
      if (not IsPyBasicLayer(pyActive)) {
        PyErr_SetString ( ConstructorError, "Contact.create(): Fifth argument is not of type BasicLayer." );
        return NULL;
      }
      if (not IsPyBasicLayer(pyDiffusion)) {
        PyErr_SetString ( ConstructorError, "Contact.create(): Sixth argument is not of type BasicLayer." );
        return NULL;
      }
      BasicLayer* xwell = NULL;
      if (pyWell != Py_None) {
        if (not IsPyBasicLayer(pyWell)) {
          PyErr_SetString ( ConstructorError, "Contact.create(): Seventh argument is not of type BasicLayer." );
          return NULL;
        }
        xwell = PYBASICLAYER_O(pyWell);
      }

      contactLayer = ContactLayer::create( PYTECHNOLOGY_O(pyTechnology)
                                         , Name(name)
                                         , PYBASICLAYER_O(pyMetal)
                                         , PYBASICLAYER_O(pyCut)
                                         , PYBASICLAYER_O(pyActive)
                                         , PYBASICLAYER_O(pyDiffusion)
                                         , xwell
                                         );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to ContactLayer.create()." );
      return NULL;
    }
    HCATCH

    return PyContactLayer_Link(contactLayer);
  }


  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyContactLayer_destroy, PyContactLayer)


  // ---------------------------------------------------------------
  // PyContactLayer Attribute Method table.

  PyMethodDef PyContactLayer_Methods[] =
    { { "create"     , (PyCFunction)PyContactLayer_create     , METH_VARARGS|METH_STATIC
                     , "Create a new ContactLayer." }
    , { "destroy"    , (PyCFunction)PyContactLayer_destroy    , METH_NOARGS
                     , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  DBoDeleteMethod(ContactLayer)
  PyTypeObjectLinkPyType(ContactLayer)


#else  // End of Python Module Code Part.


// +=================================================================+
// |           "PyContactLayer" Shared Library Code Part             |
// +=================================================================+


  // Link/Creation Method.
  DBoLinkCreateMethod(ContactLayer)


  PyTypeInheritedObjectDefinitions(ContactLayer, Layer)


#endif  // End of Shared Library Code Part.


}  // extern "C".

}  // Isobar namespace.
 
