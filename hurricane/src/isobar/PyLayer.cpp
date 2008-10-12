
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
// $Id: PyLayer.cpp,v 1.10 2006/09/22 11:27:31 tsunami Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :          Damien.Dupuis@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyLayer.cpp"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyName.h"
#include "hurricane/isobar/PyTechnology.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Layer,layer,function)


// x=================================================================x
// |               "PyLayer" Python Module Code Part                 |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                "PyLayer" Attribute Methods                  |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).


  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyLayer_destroy, PyLayer)

  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyLayer_getTechnology ()"

  static PyObject* PyLayer_getTechnology ( PyLayer *self )
  {
    trace << "PyLayer_getTechnology ()" << endl;

    Technology* techno = NULL;

    HTRY
    METHOD_HEAD ( "Layer.getTechnology()" )
    techno = layer->getTechnology ();
    HCATCH
    
    return PyTechnology_Link ( techno );
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyLayer_getName ()"

  static PyObject* PyLayer_getName ( PyLayer *self )
  {

    trace << "Layer_getName ()" << endl;
    METHOD_HEAD ( "Layer.getName()" )

    PyName* pyName = PyObject_NEW ( PyName, &PyTypeName );
    if ( pyName == NULL ) { return NULL; }

    HTRY
    pyName->_object = new Name ( layer->getName() );
    HCATCH
    
    return ( (PyObject*)pyName );
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyLayer_SetName ()"

  static PyObject* PyLayer_SetName ( PyLayer *self, PyObject* args )
  {
    trace << "Layer.SetName()" << endl;
    METHOD_HEAD ( "Layer.SetName()" )

    HTRY
    PyName* name;
    if ( ! ParseOneArg ( "Layer.SetName", args, NAME_ARG, (PyObject**)&name ) ) return ( NULL );

    layer->setName ( *PYNAME_O(name) );
    HCATCH

    Py_RETURN_NONE;
  }



  // ---------------------------------------------------------------
  // PyLayer Attribute Method table.

  PyMethodDef PyLayer_Methods[] =
    { { "getTechnology"       , (PyCFunction)PyLayer_getTechnology       , METH_NOARGS , "Returns the technology owning the layer." }
    , { "getName"             , (PyCFunction)PyLayer_getName             , METH_NOARGS , "Returns the name of the layer." }
    , { "SetName"             , (PyCFunction)PyLayer_SetName             , METH_VARARGS, "Allows to change the layer name." }
    , { "destroy"             , (PyCFunction)PyLayer_destroy             , METH_NOARGS
                              , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                  "PyLayer" Object Methods                   |
  // x-------------------------------------------------------------x


  DBoDeleteMethod(Layer)
  PyTypeObjectLinkPyType(Layer)


#else  // End of Python Module Code Part.


// x=================================================================x
// |              "PyLayer" Shared Library Code Part                 |
// x=================================================================x


  // Link/Creation Method.
  DBoLinkCreateMethod(Layer)




  // ---------------------------------------------------------------
  // PyLayer Object Definitions.

  PyTypeObjectDefinitions(Layer)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Isobar namespace. 
