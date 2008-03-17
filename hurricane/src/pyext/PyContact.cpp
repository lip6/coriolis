
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
// $Id: PyContact.cpp,v 1.21 2008/02/07 17:09:41 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                   Jean-Paul Chaput               |
// |  E-mail      :      Jean-Paul.Chaput@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyContact.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "PyNet.h"
#include "PyLayer.h"
#include "PyContact.h"
#include "PyHorizontal.h"
#include "PyVertical.h"


namespace  Isobar {


USING_NAMESPACE_HURRICANE


extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject) &(_pyObject->_baseObject._baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Contact,contact,function)


// x=================================================================x
// |              "PyContact" Python Module Code Part                |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |               "PyContact" Attribute Methods                 |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyContact_getWidth     , getWidth     , PyContact,Contact)
  DirectGetLongAttribute(PyContact_getHalfWidth , getHalfWidth , PyContact,Contact)
  DirectGetLongAttribute(PyContact_getHeight    , getHeight    , PyContact,Contact)
  DirectGetLongAttribute(PyContact_getHalfHeight, getHalfHeight, PyContact,Contact)

  DirectGetLongAttribute(PyContact_getDx, getDx, PyContact,Contact)
  DirectGetLongAttribute(PyContact_getDy, getDy, PyContact,Contact)
   
  
  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyContact_destroy, PyContact)


  // ---------------------------------------------------------------
  // PyContact Attribute Method table.

  PyMethodDef PyContact_Methods[] =
    { { "destroy"         , (PyCFunction)PyContact_destroy         , METH_NOARGS
                         , "Destroy associated hurricane object, the python object remains." }
    , { "getWidth"       , (PyCFunction)PyContact_getWidth       , METH_NOARGS , "Return the contact width." }
    , { "getHalfWidth"   , (PyCFunction)PyContact_getHalfWidth   , METH_NOARGS , "Return the contact half width." }
    , { "getHeight"      , (PyCFunction)PyContact_getHeight      , METH_NOARGS , "Return the contact height." }
    , { "getHalfHeight"  , (PyCFunction)PyContact_getHalfHeight  , METH_NOARGS , "Return the contact half height." }
    , { "getDx"          , (PyCFunction)PyContact_getDx          , METH_NOARGS , "Return the contact dx value." }
    , { "getDy"          , (PyCFunction)PyContact_getDy          , METH_NOARGS , "Return the contact dy value." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                "PyContact" Object Methods                   |
  // x-------------------------------------------------------------x
  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyContact_new ()"

  static PyObject* PyContact_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyContact_new()" << endl;

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;
    PyObject* arg4;
    PyObject* arg5;
    Contact*  contact = NULL;

    HTRY

    __cs.Init ("Contact.new");
    if ( ! PyArg_ParseTuple(args,"O&O&O&O&|O&O&:Contact.new"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3
                           ,Converter,&arg4
                           ,Converter,&arg5
                           ) ) return ( NULL );


    //cerr << "Format := " << __cs.getObjectIds() << endl;
    if      ( __cs.getObjectIds() == NET_LAYER_INTS2_ARG )
      contact = Contact::Create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2)
                                  , PyInt_AsLong(arg3) );

    else if ( __cs.getObjectIds() == NET_LAYER_INTS3_ARG )
      contact = Contact::Create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2)
                                  , PyInt_AsLong(arg3) 
                                  , PyInt_AsLong(arg4) );

    else if ( __cs.getObjectIds() == NET_LAYER_INTS4_ARG )
      contact = Contact::Create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2)
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4)
                                  , PyInt_AsLong(arg5) );

    else if ( __cs.getObjectIds() == COMP_LAYER_INTS2_ARG )
      contact = Contact::Create ( PYCOMPONENT_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2)
                                  , PyInt_AsLong(arg3) );

    else if ( __cs.getObjectIds() == COMP_LAYER_INTS3_ARG )
      contact = Contact::Create ( PYCOMPONENT_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2)
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4) );

    else if ( __cs.getObjectIds() == COMP_LAYER_INTS4_ARG )
      contact = Contact::Create ( PYCOMPONENT_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2) 
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4)
                                  , PyInt_AsLong(arg5) );           
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Contact constructor." );
      return ( NULL );
    }

    if (contact == NULL)  {  printf("error of creation of contact \n") ;  return (NULL) ; }

    HCATCH

    return PyContact_Link ( contact );
  }



  DBoDeleteMethod(Contact)
  PyTypeObjectLinkPyType(Contact)
  PyTypeObjectConstructor(Contact)


#else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyContact" Shared Library Code Part                |
// x=================================================================x
  



  // Link/Creation Method.
  DBoLinkCreateMethod(PyContact_Link,PyContact,PyTypeContact,Contact)



  // ---------------------------------------------------------------
  // PyContact Object Definitions.

  PyTypeInheritedObjectDefinitions(Contact, Component)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
