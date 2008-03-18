
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
// $Id: PyPoint.cpp,v 1.17 2006/09/22 11:27:32 tsunami Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyPoint.cpp"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "PyPoint.h"


namespace Isobar {


USING_NAMESPACE_HURRICANE


extern "C" {


#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Point,point,function)


// x=================================================================x
// |                "PyPoint" Python Module Code Part                |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |               "PyPoint" Attribute Methods                   |
  // x-------------------------------------------------------------x


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPoint_Translate ()"

  static PyObject* PyPoint_Translate ( PyPoint *self, PyObject* args )
  {
    trace << "PyPoint_Translate()" << endl;

    HTRY

    METHOD_HEAD ( "Box.Translate()" )

    PyObject* arg0;
    PyObject* arg1;
    if ( ! ParseTwoArg ( "Box.Translate", args, INTS2_ARG, &arg0, &arg1 ) ) return ( NULL );

    point->translate ( PyInt_AsLong(arg0), PyInt_AsLong(arg1) );

    HCATCH

    Py_RETURN_NONE;
  }




  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyPoint_getX,getX,PyPoint,Point)
  DirectGetLongAttribute(PyPoint_getY,getY,PyPoint,Point)
  DirectSetLongAttribute(PyPoint_SetX,setX,"Point.setX",PyPoint,Point)
  DirectSetLongAttribute(PyPoint_SetY,setY,"Point.setY",PyPoint,Point)


  // Standart destroy (Attribute).
  DirectDestroyAttribute(PyPoint_destroy, PyPoint)




  // ---------------------------------------------------------------
  // PyPoint Attribute Method table.

  PyMethodDef PyPoint_Methods[] =
    { { "getX"     , (PyCFunction)PyPoint_getX     , METH_NOARGS , "Return the Point X value." }
    , { "getY"     , (PyCFunction)PyPoint_getY     , METH_NOARGS , "Return the Point Y value." }
    , { "SetX"     , (PyCFunction)PyPoint_SetX     , METH_VARARGS, "Modify the Point X value." }
    , { "SetY"     , (PyCFunction)PyPoint_SetY     , METH_VARARGS, "Modify the Point Y value." }
    , { "Translate", (PyCFunction)PyPoint_Translate, METH_VARARGS, "Translate the point of dx and dy." }
    , { "destroy"  , (PyCFunction)PyPoint_destroy  , METH_NOARGS
                   , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                 "PyPoint" Object Methods                    |
  // x-------------------------------------------------------------x

  static PyObject* PyPoint_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyPoint_new()" << endl;

    Point*    point;
    PyObject* arg0;
    PyObject* arg1;

    __cs.Init ("Point.create");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Point.create"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ) ) return ( NULL );

    if      ( __cs.getObjectIds() == NO_ARG    ) { point = new Point (); }
    else if ( __cs.getObjectIds() == POINT_ARG ) { point = new Point ( *PYPOINT_O(arg0) ); }
    else if ( __cs.getObjectIds() == INTS2_ARG ) { point = new Point ( PyInt_AsLong(arg0)
                                                                     , PyInt_AsLong(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Point constructor." );
      return ( NULL );
    }

    PyPoint* pyPoint = PyObject_NEW(PyPoint, &PyTypePoint);
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = point;
    HCATCH

    return ( (PyObject*)pyPoint );
  }


  DirectDeleteMethod(PyPoint_DeAlloc,PyPoint)
  PyTypeObjectLinkPyType(Point)
  PyTypeObjectConstructor(Point)

#else  // End of Python Module Code Part.


// x=================================================================x
// |               "PyPoint" Shared Library Code Part                |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyPoint Object Definitions.
  
  PyTypeRootObjectDefinitions(Point)


#endif  // End of Shared Library Code Part.




}  // End of extern "C".




}  // End of Isobar namespace.
 
