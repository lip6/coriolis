
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
// $Id: PyReference.cpp,v 1.8 2008/02/07 17:09:42 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                   Jean-Paul Chaput               |
// |  E-mail      :      Jean-Paul.Chaput@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyReference.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# include  "PyReference.h"
# include  "PyPoint.h"
# include  "PyName.h"
# include  "PyCell.h"
# include  "PyNet.h"


namespace  Isobar {


USING_NAMESPACE_HURRICANE


extern "C" {


#undef   ACCESS_OBJECT
#undef   ACCESS_CLASS
#define  ACCESS_OBJECT           _baseObject._object
#define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define  METHOD_HEAD(function)    GENERIC_METHOD_HEAD(Reference,reference,function)


// x=================================================================x
// |             "PyReference" Python Module Code Part               |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |              "PyReference" Attribute Methods                |
  // x-------------------------------------------------------------x


  DBoDestroyAttribute(PyReference_destroy, PyReference)

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyReference_getName ()"

  static PyObject* PyReference_getName ( PyReference *self ) {

    trace << "PyReference_getName ()" << endl;
    
    METHOD_HEAD ( "Reference.getName()" )

    PyName* pyName = PyObject_NEW ( PyName, &PyTypeName );
    if ( pyName == NULL ) { return NULL; }

    HTRY
    
    trace_in ();
    trace << "new PyName [" << hex << pyName << "]" << endl;
    trace_out ();

    pyName->_object = new Name ( reference->getName() );
  
    HCATCH
    
    return ( (PyObject*)pyName );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyReference_getPoint ()"

  static PyObject* PyReference_getPoint ( PyReference *self )
  {
    trace << "PyReference_getPoint()" << endl;

    METHOD_HEAD ( "Reference.getPoint()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( reference->getPoint() );
    HCATCH    

    return ( (PyObject*)pyPoint );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyReference_Translate ()"

  static PyObject* PyReference_Translate ( PyReference *self, PyObject* args )
  {
    trace << "PyReference_Translate ()" << endl;
    
    METHOD_HEAD ( "Reference.Translate()" )

    PyObject* arg0;
    PyObject* arg1;

    HTRY

    if ( ! ParseTwoArg ( "Reference.Translate", args, INTS2_ARG, &arg0, &arg1 ) ) return ( NULL );

    reference->Translate ( PyInt_AsLong(arg0), PyInt_AsLong(arg1) );

    HCATCH

    Py_RETURN_NONE;
  }




  // ---------------------------------------------------------------
  // PyReference Attribute Method table.

  PyMethodDef PyReference_Methods[] =
    { { "destroy"        , (PyCFunction)PyReference_destroy        , METH_NOARGS
                         , "destroy associated hurricane object, the python object remains." }
    , { "getName"        , (PyCFunction)PyReference_getName        , METH_NOARGS , "Returns the name of the reference." }
    , { "getPoint"       , (PyCFunction)PyReference_getPoint       , METH_NOARGS , "Return the reference point." }
    , { "Translate"      , (PyCFunction)PyReference_Translate      , METH_VARARGS, "Translate the reference of dx and dy." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |               "PyReference" Object Methods                  |
  // x-------------------------------------------------------------x

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyReference_new ()"

  static PyObject* PyReference_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyReference_new()" << endl;

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;
    Reference*  reference = NULL;

    HTRY

    __cs.Init ("Reference.new");
    if ( ! PyArg_ParseTuple(args,"O&O&O&|O&:Reference.new"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3
                           ) ) return ( NULL );

    if      ( __cs.getObjectIds() == CELL_NAME_INTS2_ARG )
      reference = Reference::create (  PYCELL_O(arg0)
                                    , *PYNAME_O(arg1)
                                    ,  PyInt_AsLong(arg2)
                                    ,  PyInt_AsLong(arg3) );
    else if ( __cs.getObjectIds() == CELL_NAME_POINT_ARG )
      reference = Reference::create (  PYCELL_O(arg0)
                                    , *PYNAME_O(arg1)
                                    , *PYPOINT_O(arg2) );
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Reference constructor." );
      return ( NULL );
    }

    if (reference == NULL)  {  printf("error of creation of reference \n"); return (NULL) ; }

    HCATCH

    return PyReference_Link ( reference );
  }



  DBoDeleteMethod(Reference)
  PyTypeObjectLinkPyType(Reference)
  PyTypeObjectConstructor(Reference)


# else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyReference" Shared Library Code Part               |
// x=================================================================x
  



  // Link/Creation Method.
  DBoLinkCreateMethod(PyReference_Link,PyReference,PyTypeReference,Reference)



  // ---------------------------------------------------------------
  // PyReference Object Definitions.
  PyTypeInheritedObjectDefinitions(Reference, Entity)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
