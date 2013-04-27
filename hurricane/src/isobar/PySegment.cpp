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




#include "hurricane/isobar/PyHook.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PySegment.h"
#include "hurricane/isobar/PyHorizontal.h"
#include "hurricane/isobar/PyVertical.h"
#include "hurricane/isobar/PyPoint.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Segment,segment,function)


// x=================================================================x
// |              "PySegment" Python Module Code Part                |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |               "PySegment" Attribute Methods                 |
  // x-------------------------------------------------------------x


  // Standard Accessors (Attributes).
  DirectGetLongAttribute(PySegment_getSourceX,getSourceX,PySegment,Segment)
  DirectGetLongAttribute(PySegment_getSourceY,getSourceY,PySegment,Segment)
  DirectGetLongAttribute(PySegment_getWidth  ,getWidth  ,PySegment,Segment)
  DirectSetLongAttribute(PySegment_setWidth  ,setWidth  ,"Segment.setWidth",PySegment,Segment)
  DirectVoidMethod(Segment,segment,invert)
  accessorHook(getSourceHook,PySegment,Segment)
  accessorHook(getTargetHook,PySegment,Segment)

  // Standard Destroy (Attribute).
  DBoDestroyAttribute(PySegment_destroy, PySegment)


  static PyObject* PySegment_getOppositeHook ( PySegment *self, PyObject* args )
  {
    trace << "PySegment_getOppositeHook()" << endl;
    METHOD_HEAD ( "Segment.getOppositeHook()" )

    PyHook* pyReturnHook = PyObject_NEW( PyHook, &PyTypeHook );
    if (pyReturnHook == NULL) return NULL;

    PyObject* pyHook = NULL;
    if (not PyArg_ParseTuple(args,"O:Hook.merge", &pyHook)) return NULL;

    Hook* hook = PYHOOK_O(pyHook);

    HTRY
    pyReturnHook->_object = segment->getOppositeHook( hook );
    HCATCH
      
    return (PyObject*)pyReturnHook;
  }


  static PyObject* PySegment_getSource ( PySegment *self )
  {
    trace << "PySegment_getSource()" << endl;

    METHOD_HEAD( "Segment.Source()" )

    PyObject* pySource = NULL;
    
    HTRY
    Component* source = segment->getSource();
    if (source) pySource = PyEntity_NEW( source );
    else
      Py_RETURN_NONE;
    HCATCH    

    return pySource;
  }


  static PyObject* PySegment_getTarget ( PySegment *self )
  {
    trace << "PySegment_getTarget()" << endl;

    METHOD_HEAD( "Segment.Target()" )

    PyObject* pyTarget = NULL;
    
    HTRY
    Component* target = segment->getTarget();
    if (target) pyTarget = PyEntity_NEW( target );
    else
      Py_RETURN_NONE;
    HCATCH    

    return pyTarget;
  }


  static PyObject* PySegment_getSourcePosition ( PySegment *self )
  {
    trace << "PySegment_getSourcePosition()" << endl;

    METHOD_HEAD ( "Segment.SourcePosition()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( segment->getSourcePosition() );
    HCATCH    

    return ( (PyObject*)pyPoint );
  }


  static PyObject* PySegment_getTargetPosition ( PySegment *self )
  {
    trace << "PySegment_getTargetPosition()" << endl;

    METHOD_HEAD ( "Segment.TargetPosition()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( segment->getTargetPosition() );
    HCATCH    

    return ( (PyObject*)pyPoint );
  }


  // ---------------------------------------------------------------
  // PySegment Attribute Method table.

  PyMethodDef PySegment_Methods[] =
    { { "getSourceHook"        , (PyCFunction)PySegment_getSourceHook    , METH_NOARGS , "Return the nested source Hook." }
    , { "getTargetHook"        , (PyCFunction)PySegment_getTargetHook    , METH_NOARGS , "Return the nested target Hook." }
    , { "getOppositeHook"      , (PyCFunction)PySegment_getOppositeHook  , METH_VARARGS, "Return the nested Hook opposite of the argument hook." }
    , { "getSource"            , (PyCFunction)PySegment_getSource        , METH_NOARGS , "Return the Segment source component (or None)." }
    , { "getTarget"            , (PyCFunction)PySegment_getTarget        , METH_NOARGS , "Return the Segment target component (or None)." }
    , { "getSourceX"           , (PyCFunction)PySegment_getSourceX       , METH_NOARGS , "Return the Segment source X value." }
    , { "getSourceY"           , (PyCFunction)PySegment_getSourceY       , METH_NOARGS , "Return the Segment source Y value." }
    , { "getSourcePosition"    , (PyCFunction)PySegment_getSourcePosition, METH_NOARGS , "Return the Segment source point value." }
    , { "getTargetPosition"    , (PyCFunction)PySegment_getTargetPosition, METH_NOARGS , "Return the Segment target point value." }
    , { "getWidth"             , (PyCFunction)PySegment_getWidth         , METH_NOARGS , "Return the segment width." }
    , { "setWidth"             , (PyCFunction)PySegment_setWidth         , METH_VARARGS, "Modify the Segment width." }
    , { "invert"               , (PyCFunction)PySegment_invert           , METH_NOARGS , "Swap source & target anchors." }
    , { "destroy"              , (PyCFunction)PySegment_destroy          , METH_NOARGS
                               , "Destroy associated hurricane object, the python object remains." }
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
 
