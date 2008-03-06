
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
// $Id: PySegment.cpp,v 1.12 2008/02/07 17:09:42 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                   Jean-Paul Chaput               |
// |  E-mail      :      Jean-Paul.Chaput@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PySegment.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# include  "PyNet.h"
# include  "PyLayer.h"
# include  "PySegment.h"
# include  "PyHorizontal.h"
# include  "PyVertical.h"
# include  "PyPoint.h"


namespace  Isobar {


USING_NAMESPACE_HURRICANE


extern "C" {


# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT           _baseObject._baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject)
# define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Segment,segment,function)


// x=================================================================x
// |              "PySegment" Python Module Code Part                |
// x=================================================================x

# if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |               "PySegment" Attribute Methods                 |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PySegment_GetSourceX,GetSourceX,PySegment,Segment)
  DirectGetLongAttribute(PySegment_GetSourceY,GetSourceY,PySegment,Segment)
  DirectGetLongAttribute(PySegment_GetWidth  ,GetWidth,  PySegment,Segment)

  // Standart Delete (Attribute).
  DBoDeleteAttribute(PySegment_Delete,PySegment)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PySegment_GetSourcePosition ()"

  static PyObject* PySegment_GetSourcePosition ( PySegment *self )
  {
    trace << "PySegment_GetSourcePosition()" << endl;

    METHOD_HEAD ( "Segment.SourcePosition()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( segment->GetSourcePosition() );
    HCATCH    

    return ( (PyObject*)pyPoint );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PySegment_GetTargetPosition ()"

  static PyObject* PySegment_GetTargetPosition ( PySegment *self )
  {
    trace << "PySegment_GetTargetPosition()" << endl;

    METHOD_HEAD ( "Segment.TargetPosition()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( segment->GetTargetPosition() );
    HCATCH    

    return ( (PyObject*)pyPoint );
  }


  // ---------------------------------------------------------------
  // PySegment Attribute Method table.

  PyMethodDef PySegment_Methods[] =
    { { "GetSourceX"           , (PyCFunction)PySegment_GetSourceX       , METH_NOARGS , "Return the Segment source X value." }
    , { "GetSourceY"           , (PyCFunction)PySegment_GetSourceY       , METH_NOARGS , "Return the Segment source Y value." }
    , { "GetSourcePosition"    , (PyCFunction)PySegment_GetSourcePosition, METH_NOARGS , "Return the Segment source point value." }
    , { "GetTargetPosition"    , (PyCFunction)PySegment_GetTargetPosition, METH_NOARGS , "Return the Segment target point value." }
    , { "GetWidth"             , (PyCFunction)PySegment_GetWidth         , METH_NOARGS , "Return the segment width." }
    , { "Delete"               , (PyCFunction)PySegment_Delete           , METH_NOARGS
                               , "Delete associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // x-------------------------------------------------------------x
  // |                "PySegment" Object Methods                   |
  // x-------------------------------------------------------------x


  DBoDeleteMethod(Segment)
  PyTypeObjectLinkPyType(Segment)


# else  // End of Python Module Code Part.


// x=================================================================x
// |             "PySegment" Shared Library Code Part                |
// x=================================================================x




  // ---------------------------------------------------------------
  // PySegment Object Definitions.


  PyTypeInheritedObjectDefinitions(Segment, Component)

# endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
