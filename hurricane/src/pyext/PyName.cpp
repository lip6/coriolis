
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
// $Id: PyName.cpp,v 1.13 2006/09/22 11:27:31 tsunami Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyName.cpp"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "PyName.h"


namespace Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Name,_object,function)


// x=================================================================x
// |                "PyName" Python Module Code Part                 |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                "PyName" Attribute Methods                   |
  // x-------------------------------------------------------------x

  //DirectGetBoolAttribute(PyName_IsEmpty,IsEmpty,PyName,Name)

  static PyObject* PyName_isEmpty ( PyName *self ) {
    trace << "PyName_isEmpty()" << endl;
    
    METHOD_HEAD ( "Name.isEmpty()" )
    return ( Py_BuildValue ("i",_object->isEmpty()) );
  }


  // Standart destroy (Attribute).
  DirectDestroyAttribute(PyName_destroy, PyName)




  // ---------------------------------------------------------------
  // PyName Attribute Method table.

  PyMethodDef PyName_Methods[] =
    { { "isEmpty"  , (PyCFunction)PyName_isEmpty  , METH_NOARGS , "True if empty." }
    , { "destroy"  , (PyCFunction)PyName_destroy   , METH_NOARGS
                   , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyName_new ()"

  static PyObject* PyName_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyName_new ()" << endl;

    Name* name = NULL;
    PyObject* arg0;

    __cs.Init ("Name.new");
    if (!PyArg_ParseTuple(args,"|O&:Name.new",Converter,&arg0) ) return ( NULL );
    
    HTRY
    if      (__cs.getObjectIds() == NO_ARG     ) { name = new Name (); }
    else if (__cs.getObjectIds() == STRING_ARG ) { name = new Name ( PyString_AsString(arg0) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Name constructor." );
      return(NULL);
    }
    HCATCH

    PyObject* pyName = type->tp_alloc(type, 0);
    if (pyName != NULL) {
      ((PyName*)pyName)->ACCESS_OBJECT = name;
    }

    return pyName;
  }


  DirectDeleteMethod(PyName_DeAlloc,PyName)
  PyTypeObjectLinkPyType(Name)
  PyTypeObjectConstructor(Name)


#else  // End of Python Module Code Part.


// x=================================================================x
// |               "PyName" Shared Library Code Part                 |
// x=================================================================x





  // ---------------------------------------------------------------
  // PyName Object Definitions.
  
  PyTypeRootObjectDefinitions(Name)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
