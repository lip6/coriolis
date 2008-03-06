
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
// $Id: PyVertical.cpp,v 1.18 2008/02/07 17:09:42 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Sophie BELLOEIL               |
// |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyVertical.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# include  "PyNet.h"
# include  "PyLayer.h"
# include  "PyVertical.h"


namespace  Isobar {


USING_NAMESPACE_HURRICANE


extern "C" {


# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._baseObject._baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject)
# define  METHOD_HEAD(function)    GENERIC_METHOD_HEAD(Vertical,vertical,function)

// x=================================================================x
// |              "PyVertical" Python Module Code Part               |
// x=================================================================x

# if defined(__PYTHON_MODULE__)

  
  // x-------------------------------------------------------------x
  // |              "PyVertical" Attribute Methods                 |
  // x-------------------------------------------------------------x


  // Standart Delete (Attribute).
  DBoDeleteAttribute(PyVertical_Delete,PyVertical)


  // ---------------------------------------------------------------
  // PyVertical Attribute Method table.

  PyMethodDef PyVertical_Methods[] =
    { { "Delete"               , (PyCFunction)PyVertical_Delete                     , METH_NOARGS
                               , "Delete associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                "PyVertical" Object Methods                  |
  // x-------------------------------------------------------------x
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyVertical_new ()"

  static PyObject* PyVertical_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyVertical_new()" << endl;

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;
    PyObject* arg4;
    PyObject* arg5;
    PyObject* arg6;
    Vertical* vertical = NULL;

    HTRY
    __cs.Init ("Vertical.new");
    if (!PyArg_ParseTuple(args,"O&O&O&|O&O&O&O&:Vertical.new"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3
                           ,Converter,&arg4
                           ,Converter,&arg5
                           ,Converter,&arg6
                           ) ) return ( NULL );


    //cerr << "Format := " << __cs.GetObjectIds() << endl;
    if      ( __cs.GetObjectIds() == NET_LAYER_INT_ARG )
      vertical = Vertical::Create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2) );
    else if ( __cs.GetObjectIds() == NET_LAYER_INTS2_ARG )
      vertical = Vertical::Create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2)
                                  , PyInt_AsLong(arg3) );
    else if ( __cs.GetObjectIds() == COMPS2_LAYER_INT_ARG )
      vertical = Vertical::Create ( ComponentCast(arg0)
                                  , ComponentCast(arg1)
                                  , PYLAYER_O(arg2)
                                  , PyInt_AsLong(arg3) );
    else if ( __cs.GetObjectIds() == NET_LAYER_INTS3_ARG )
      vertical = Vertical::Create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2)
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4) );
    else if ( __cs.GetObjectIds() == COMPS2_LAYER_INTS2_ARG )
      vertical = Vertical::Create ( ComponentCast(arg0)
                                  , ComponentCast(arg1)
                                  , PYLAYER_O(arg2)
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4) );
    else if ( __cs.GetObjectIds() == NET_LAYER_INTS4_ARG )
      vertical = Vertical::Create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2)
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4)
                                  , PyInt_AsLong(arg5) );
    else if ( __cs.GetObjectIds() == COMPS2_LAYER_INTS3_ARG )
      vertical = Vertical::Create ( ComponentCast(arg0)
                                  , ComponentCast(arg1)
                                  , PYLAYER_O(arg2)
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4)
                                  , PyInt_AsLong(arg5) );
    else if ( __cs.GetObjectIds() == COMPS2_LAYER_INTS4_ARG )
      vertical = Vertical::Create ( ComponentCast(arg0)
                                  , ComponentCast(arg1)
                                  , PYLAYER_O(arg2)
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4)
                                  , PyInt_AsLong(arg5)
                                  , PyInt_AsLong(arg6) );
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Vertical constructor." );
      return ( NULL );
    }

    HCATCH

    return PyVertical_Link ( vertical );
  }

  DBoDeleteMethod(Vertical)
  PyTypeObjectLinkPyType(Vertical)
  PyTypeObjectConstructor(Vertical)


#else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyVertical" Shared Library Code Part               |
// x=================================================================x



  // Link/Creation Method.
  DBoLinkCreateMethod(PyVertical_Link,PyVertical,PyTypeVertical,Vertical)




  // ---------------------------------------------------------------
  // PyVertical Object Definitions.

  PyTypeInheritedObjectDefinitions(Vertical, Segment)

# endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
