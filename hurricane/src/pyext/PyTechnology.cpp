
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
// $Id: PyTechnology.cpp,v 1.11 2006/09/22 11:27:32 tsunami Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :          Damien.Dupuis@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyTechnology.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "PyDataBase.h"
#include "PyName.h"
#include "PyLayer.h"
#include "PyTechnology.h"


namespace Isobar {

USING_NAMESPACE_HURRICANE


extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Technology,techno,function)


// x=================================================================x
// |             "PyTechnology" Python Module Code Part              |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |              "PyTechnology" Attribute Methods               |
  // x-------------------------------------------------------------x




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTechnology_getName ()"

  static PyObject* PyTechnology_getName ( PyTechnology *self )
  {
    trace << "PyTechnology_getName ()" << endl;
    
    METHOD_HEAD ( "Technology.getName()" )

    PyName* pyName = PyObject_NEW ( PyName, &PyTypeName );
    if ( pyName == NULL ) { return NULL; }
    
    HTRY
    pyName->_object = new Name ( techno->getName() );
    HCATCH
    
    return ( (PyObject*)pyName );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTechnology_SetName ()"

  static PyObject* PyTechnology_SetName ( PyTechnology *self, PyObject* args )
  {
    trace << "Technology.SetName()" << endl;
    METHOD_HEAD ( "Technology.SetName()" )

    HTRY

    PyName* name;
    if ( ! ParseOneArg ( "Technology.SetName", args, NAME_ARG, (PyObject**)&name ) ) return ( NULL );

    techno->SetName ( *PYNAME_O(name) );
    HCATCH

    Py_RETURN_NONE;
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTechnology_getLayer ()"

  static PyObject* PyTechnology_getLayer ( PyTechnology *self, PyObject* args )
  {
    trace << "Technology.getLayer()" << endl;
    METHOD_HEAD ( "Technology.getLayer()" )

    Layer* layer = NULL;
    
    HTRY
    PyName* name;
    if ( ! ParseOneArg ( "Technology.getLayer", args,NAME_ARG, (PyObject**)&name ) ) return ( NULL );

    layer = techno->getLayer ( *PYNAME_O(name) );
    HCATCH

    return PyLayer_Link ( layer );
  }




  // Standart Accessors (Attributes).

  // Standard destroy (Attribute).
  DBoDestroyAttribute(PyTechnology_destroy, PyTechnology)




  // ---------------------------------------------------------------
  // PyTechnology Attribute Method table.

  PyMethodDef PyTechnology_Methods[] =
    { { "getName"      , (PyCFunction)PyTechnology_getName      , METH_NOARGS , "Returns the name of the technology." }
    , { "SetName"      , (PyCFunction)PyTechnology_SetName      , METH_VARARGS, "Allows to change the technology name." }
    , { "getLayer"     , (PyCFunction)PyTechnology_getLayer     , METH_VARARGS, "Returns the layer named name." }
    , { "destroy"      , (PyCFunction)PyTechnology_destroy      , METH_NOARGS
                       , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |               "PyTechnology" Object Methods                 |
  // x-------------------------------------------------------------x


  DBoDeleteMethod(Technology)
  PyTypeObjectLinkPyType(Technology)

#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyTechnology" Shared Library Code Part              |
// x=================================================================x


  // Link/Creation Method.
  DBoLinkCreateMethod(PyTechnology_Link,PyTechnology,PyTypeTechnology,Technology)

  // ---------------------------------------------------------------
  // PyTechnology Object Definitions.
  PyTypeRootObjectDefinitions(Technology)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
