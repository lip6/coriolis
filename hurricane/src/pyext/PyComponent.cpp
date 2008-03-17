
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
// $Id: PyComponent.cpp,v 1.17 2008/02/07 17:09:41 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                   Jean-Paul Chaput               |
// |  E-mail      :      Jean-Paul.Chaput@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyComponent.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "PyNet.h"
#include "PyLayer.h"
#include "PyPoint.h"
#include "PyBox.h"
#include "PyComponent.h"
#include "PyPlug.h"
#include "PyHorizontal.h"
#include "PyVertical.h"
#include "PyContact.h"
#include "PyPin.h"

namespace  Isobar {


USING_NAMESPACE_HURRICANE


extern "C" {


# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT           _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
# define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Component,component,function)


// x=================================================================x
// |             "PyComponent" Python Module Code Part               |
// x=================================================================x

# if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |              "PyComponent" Attribute Methods                |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyComponent_getX,getX,PyComponent,Component)
  DirectGetLongAttribute(PyComponent_getY,getY,PyComponent,Component)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyComponent_destroy,PyComponent)
  

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyComponent_getPosition ()"
  static PyObject* PyComponent_getPosition ( PyComponent *self )
  {
    trace << "PyComponent_getPosition ()" << endl;
    METHOD_HEAD ( "Component.getPosition()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( component->getPosition() );
    HCATCH    

    return ( (PyObject*)pyPoint );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyComponent_getNet ()"

  static PyObject* PyComponent_getNet ( PyComponent *self )
  {
    trace << "PyComponent_getNet ()" << endl;
    
    Net* net = NULL;

    HTRY
    METHOD_HEAD ( "Component.getNet()" )

    net = component->getNet ( );
    HCATCH
    
    return PyNet_Link ( net );
  }
 
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyComponent_getLayer ()"

  static PyObject* PyComponent_getLayer ( PyComponent *self )
  {
    trace << "PyComponent_getLayer ()" << endl;
    METHOD_HEAD ( "Component.getLayer()" )

    Layer* layer = NULL;

    HTRY
    layer = component->getLayer ();
    HCATCH

    return PyLayer_Link ( layer );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyComponent_getBoundingBox ()"

  static PyObject* PyComponent_getBoundingBox ( PyComponent *self )
  {
    trace << "PyComponent_getBoundingBox ()" << endl;
    METHOD_HEAD ( "Component.getBoundingBox()" )

    PyBox* pyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (pyBox == NULL) { return NULL; }
    trace_out ();

    HTRY
    pyBox->_object = new Box ( component->getBoundingBox() );
    HCATCH

    return ( (PyObject*)pyBox );
  }



  // ---------------------------------------------------------------
  // PyComponent Attribute Method table.

  PyMethodDef PyComponent_Methods[] =
    { { "getX"                 , (PyCFunction)PyComponent_getX          , METH_NOARGS , "Return the Component X value." }
    , { "getY"                 , (PyCFunction)PyComponent_getY          , METH_NOARGS , "Return the Component Y value." }
    , { "getPosition"          , (PyCFunction)PyComponent_getPosition   , METH_NOARGS , "Return the Component position." }
    , { "getNet"               , (PyCFunction)PyComponent_getNet        , METH_NOARGS , "Returns the net owning the component." }
    , { "getLayer"             , (PyCFunction)PyComponent_getLayer      , METH_NOARGS , "Return the component layer." }
    , { "getBoundingBox"       , (PyCFunction)PyComponent_getBoundingBox, METH_NOARGS , "Return the component boundingBox." }
    , { "destroy"              , (PyCFunction)PyComponent_destroy       , METH_NOARGS
                               , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // x-------------------------------------------------------------x
  // |               "PyComponent" Object Methods                  |
  // x-------------------------------------------------------------x

  DBoDeleteMethod(Component)
  PyTypeObjectLinkPyType(Component)


# else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyComponent" Shared Library Code Part               |
// x=================================================================x



  // ---------------------------------------------------------------
  // PyComponent Object Definitions.

  PyTypeInheritedObjectDefinitions(Component, Entity)

# endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
