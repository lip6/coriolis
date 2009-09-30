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

#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyComponent.h"
#include "hurricane/isobar/PyPlug.h"
#include "hurricane/isobar/PyHorizontal.h"
#include "hurricane/isobar/PyVertical.h"
#include "hurricane/isobar/PyContact.h"
#include "hurricane/isobar/PyPin.h"

namespace  Isobar {


using namespace Hurricane;


extern "C" {


#undef   ACCESS_OBJECT
#undef   ACCESS_CLASS
#define  ACCESS_OBJECT           _baseObject._object
#define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Component,component,function)


// x=================================================================x
// |             "PyComponent" Python Module Code Part               |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


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
  static PyObject* PyComponent_getPosition ( PyComponent *self ) {
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

  static PyObject* PyComponent_getNet ( PyComponent *self ) {
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

  static PyObject* PyComponent_getLayer ( PyComponent *self ) {
    trace << "PyComponent_getLayer ()" << endl;
    METHOD_HEAD ( "Component.getLayer()" )

    Layer* layer = NULL;

    HTRY
    layer = const_cast<Layer*>(component->getLayer ());
    HCATCH

    return PyLayer_Link ( layer );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyComponent_getBoundingBox ()"

  static PyObject* PyComponent_getBoundingBox ( PyComponent *self ) {
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


#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyComponent" Shared Library Code Part               |
// x=================================================================x



  // ---------------------------------------------------------------
  // PyComponent Object Definitions.

  PyTypeInheritedObjectDefinitions(Component, Entity)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
