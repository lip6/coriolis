// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyBox.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyBox.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(Box,box,function)


// x=================================================================x
// |                 "PyBox" Python Module Code Part                 |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                "PyBox" Attribute Methods                    |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyBox_getXMin      ,getXMin      ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getYMin      ,getYMin      ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getXMax      ,getXMax      ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getYMax      ,getYMax      ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getXCenter   ,getXCenter   ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getYCenter   ,getYCenter   ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getWidth     ,getWidth     ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getHalfWidth ,getHalfWidth ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getHeight    ,getHeight    ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getHalfHeight,getHalfHeight,PyBox,Box)


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyBox_isEmpty   ,isEmpty   ,PyBox,Box)
  DirectGetBoolAttribute(PyBox_isFlat    ,isFlat    ,PyBox,Box)
  DirectGetBoolAttribute(PyBox_isPonctual,isPonctual,PyBox,Box)


  // Standart Destroy (Attribute).
  DirectDestroyAttribute(PyBox_destroy, PyBox)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_getCenter ()"

  static PyObject* PyBox_getCenter ( PyBox *self ) {
    trace << "PyBox_getCenter()" << endl;

    METHOD_HEAD ( "Box.Center()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( box->getCenter() );
    HCATCH    

    return (PyObject*)pyPoint;
  }





  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_getUnion ()"

  static PyObject* PyBox_getUnion ( PyBox *self, PyObject* args ) {
    trace << "PyBox_getUnion()" << endl;

    METHOD_HEAD ( "Box.getUnion()" )

    PyBox* otherPyBox;
    if (PyArg_ParseTuple(args,"O!:Box.getUnion", &PyTypeBox, &otherPyBox)) {
        PyBox* unionPyBox = PyObject_NEW ( PyBox, &PyTypeBox );
        if (unionPyBox == NULL) {
            return NULL;
        }
        HTRY
        unionPyBox->_object = new Box ( box->getUnion(*PYBOX_O(otherPyBox)));
        HCATCH
        return (PyObject*)unionPyBox;
    } else {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.getUnion.");
    }
    return NULL;

  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_getIntersection ()"

  static PyObject* PyBox_getIntersection ( PyBox *self, PyObject* args ) {
    trace << "PyBox_getIntersection()" << endl;

    METHOD_HEAD ( "Box.getIntersection()" )

    PyBox* otherPyBox;
    if (PyArg_ParseTuple(args,"O!:Box.getIntersection", &PyTypeBox, &otherPyBox)) {
        PyBox* intersectionPyBox = PyObject_NEW ( PyBox, &PyTypeBox );
        if (intersectionPyBox == NULL) {
            return NULL;
        }
        HTRY
        intersectionPyBox->_object = new Box(box->getIntersection(*PYBOX_O(otherPyBox)));
        HCATCH
        return (PyObject*)intersectionPyBox;
    } else {
        PyErr_SetString(ConstructorError, "invalid number of parameters for Box.getIntersection.");
    }
    return NULL;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_contains ()"

  static PyObject* PyBox_contains ( PyBox *self, PyObject* args ) {
    trace << "PyBox_contains ()" << endl;
    
    bool result = false;
    HTRY

    PyBox* pyBox = NULL;
    PyPoint* pyPoint = NULL;
    DbU::Unit x = 0, y = 0;

    METHOD_HEAD ( "Box.contains()" )

    if (PyArg_ParseTuple(args,"O!:Box.contains", &PyTypeBox, &pyBox)) {
        result = box->contains(*PYBOX_O(pyBox));
    } else if (PyArg_ParseTuple(args,"O!:Box.contains", &PyTypePoint, &pyPoint)) {
        result = box->contains(*PYPOINT_O(pyPoint));
    } else if (PyArg_ParseTuple(args,"ll:Box.contains", &x, &y)) {
        result = box->contains(x,y);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.contains." );
      return NULL;
    }

    HCATCH
    if (result) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_intersect ()"

  static PyObject* PyBox_intersect ( PyBox *self, PyObject* args ) {
    trace << "PyBox_intersect ()" << endl;
    
    bool result = false;
    HTRY
    PyBox* pyBox = NULL;
    METHOD_HEAD ( "Box.intersect()" )

    if (PyArg_ParseTuple(args,"O!:Box.intersects", &PyTypeBox, &pyBox)) {
        result = box->intersect(*PYBOX_O(pyBox));
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.intersect." );
      return NULL;
    }

    HCATCH
    if (result) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }

  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_isConstrainedBy ()"

  static PyObject* PyBox_isConstrainedBy ( PyBox *self, PyObject* args ) {
    trace << "PyBox_isConstrainedBy ()" << endl;
    
    bool result = false;
    HTRY
    PyBox* pyBox = NULL;
    METHOD_HEAD("Box.isConstrainedBy()")

    if (PyArg_ParseTuple(args,"O!:Box.isConstrainedBy", &PyTypeBox, &pyBox)) {
        result = box->isConstrainedBy(*PYBOX_O(pyBox));
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.isConstrainedBy.");
      return NULL;
    }

    HCATCH
    if (result) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_makeEmpty ()"

  static PyObject* PyBox_makeEmpty ( PyBox *self, PyObject* args ) {
    trace << "PyBox_makeEmpty ()" << endl;

    HTRY
    METHOD_HEAD ( "Box.makeEmpty()" )

    box->makeEmpty ();
    HCATCH
    
    Py_INCREF ( self ); //FIXME ??
    return (PyObject*)self;
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_inflate ()"

  static PyObject* PyBox_inflate(PyBox *self, PyObject* args) {
    trace << "PyBox_inflate ()" << endl;
    
    METHOD_HEAD("Box.inflate()")

    HTRY
    DbU::Unit arg0=0, arg1=0, arg2=0, arg3=0;

    if (PyArg_ParseTuple(args,"l:Box.inflate", &arg0)) {
        box->inflate(arg0);
    } else if (PyArg_ParseTuple(args,"ll:Box.inflate", &arg0, &arg1)) {
        box->inflate(arg0, arg1);
    } else if (PyArg_ParseTuple(args,"llll:Box.inflate", &arg0, &arg1, &arg2, &arg3)) {
        box->inflate(arg0, arg1, arg2, arg3);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.inflate()" );
      return NULL;
    }
    HCATCH

    Py_INCREF ( self ); //FIXME ??
    return (PyObject*)self;
  }





  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_merge ()"

  static PyObject* PyBox_merge ( PyBox *self, PyObject* args ) {
    trace << "Box_merge()" << endl;
    
    METHOD_HEAD ( "Box.merge()" )
    

    HTRY
    PyBox* pyBox = NULL;
    PyPoint* pyPoint = NULL;
    DbU::Unit arg0=0, arg1=0, arg2=0, arg3=0;

    if (PyArg_ParseTuple(args,"O!:Box.merge", &PyTypeBox, &pyBox)) {
        box->merge(*PYBOX_O(pyBox));
    } else if (PyArg_ParseTuple(args,"O!:Box.merge", &PyTypePoint, &pyPoint)) {
        box->merge(*PYPOINT_O(pyPoint));
    } else if (PyArg_ParseTuple(args,"ll:Box.merge", &arg0, &arg1)) {
        box->merge(arg0, arg1);
    } else if (PyArg_ParseTuple(args,"llll:Box.merge", &arg0, &arg1, &arg2, &arg3)) {
        box->merge(arg0, arg1, arg2, arg3);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.merge()" );
      return NULL;
    }
    
    HCATCH

    Py_INCREF ( self );
    return (PyObject*)self;
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_translate ()"

  static PyObject* PyBox_translate ( PyBox *self, PyObject* args ) {
    trace << "PyBox_translate ()" << endl;
    
    HTRY
    METHOD_HEAD ( "Box.translate()" )
    DbU::Unit dx=0, dy=0;
    if (PyArg_ParseTuple(args,"ll:Box.translate", &dx, &dy)) {
        box->translate(dx, dy);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.translate()" );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }




  // ---------------------------------------------------------------
  // PyBox Attribute Method table.

  PyMethodDef PyBox_Methods[] =
    { { "getXMin"        , (PyCFunction)PyBox_getXMin        , METH_NOARGS , "Return the XMin value." }
    , { "getYMin"        , (PyCFunction)PyBox_getYMin        , METH_NOARGS , "Return the YMin value." }
    , { "getXMax"        , (PyCFunction)PyBox_getXMax        , METH_NOARGS , "Return the XMax value." }
    , { "getYMax"        , (PyCFunction)PyBox_getYMax        , METH_NOARGS , "Return the YMax value." }
    , { "getXCenter"     , (PyCFunction)PyBox_getXCenter     , METH_NOARGS , "Return the X box center value." }
    , { "getYCenter"     , (PyCFunction)PyBox_getYCenter     , METH_NOARGS , "Return the Y box center value." }
    , { "getCenter"      , (PyCFunction)PyBox_getCenter      , METH_NOARGS , "Return the box center Point." }
    , { "getWidth"       , (PyCFunction)PyBox_getWidth       , METH_NOARGS , "Return the box width." }
    , { "getHalfWidth"   , (PyCFunction)PyBox_getHalfWidth   , METH_NOARGS , "Return the box half width." }
    , { "getHeight"      , (PyCFunction)PyBox_getHeight      , METH_NOARGS , "Return the box height." }
    , { "getHalfHeight"  , (PyCFunction)PyBox_getHalfHeight  , METH_NOARGS , "Return the box half height." }
    , { "getUnion"       , (PyCFunction)PyBox_getUnion       , METH_VARARGS, "Return the smallest enclosing box." }
    , { "getIntersection", (PyCFunction)PyBox_getIntersection, METH_VARARGS, "Return the overlapping area." }
    , { "isEmpty"        , (PyCFunction)PyBox_isEmpty        , METH_NOARGS , "Return true if the box is empty." }
    , { "isFlat"         , (PyCFunction)PyBox_isFlat         , METH_NOARGS , "Return true if the box is flat." }
    , { "isPonctual"     , (PyCFunction)PyBox_isPonctual     , METH_NOARGS , "Return true if the box reduced to a point." }
    , { "contains"       , (PyCFunction)PyBox_contains       , METH_VARARGS, "Return true if the box contains the argument." }
    , { "intersect"      , (PyCFunction)PyBox_intersect      , METH_VARARGS, "Return true if two boxes overlap." }
    , { "isConstrainedBy", (PyCFunction)PyBox_isConstrainedBy, METH_VARARGS, "Return true if the argment box is included and share at least a side." }
    , { "makeEmpty"      , (PyCFunction)PyBox_makeEmpty      , METH_NOARGS , "Transform the box in an empty one." }
    , { "inflate"        , (PyCFunction)PyBox_inflate        , METH_VARARGS, "Expand the box to contains the arguments." }
    , { "merge"          , (PyCFunction)PyBox_merge          , METH_VARARGS, "Expand or contract the box to contains the arguments." }
    , { "translate"      , (PyCFunction)PyBox_translate      , METH_VARARGS, "translate the box od dx ans dy." }
    , { "destroy"        , (PyCFunction)PyBox_destroy        , METH_NOARGS
                         , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_new ()"

  PyObject* PyBox_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyBox_new()" << endl;

    PyBox* pyBox=NULL;
    HTRY
    Box* box = NULL;
    PyPoint *pyPoint1=NULL, *pyPoint2=NULL;
    DbU::Unit arg0=0, arg1=0, arg2=0, arg3=0;

    if (PyArg_ParseTuple(args, ":Box.new")) {
        box = new Box;
    } else if (PyArg_ParseTuple(args, "O!:Box.new", &PyTypePoint, &pyPoint1)) {
        box = new Box(*PYPOINT_O(pyPoint1));
    } else if (PyArg_ParseTuple(args, "O!:Box.new", &PyTypeBox, &pyBox)) {
        box = new Box(*PYBOX_O(pyBox));
    } else if (PyArg_ParseTuple(args, "O!O!:Box.new", &PyTypePoint, &pyPoint1, &PyTypePoint, &pyPoint2)) {
        box = new Box(*PYPOINT_O(pyPoint1), *PYPOINT_O(pyPoint2));
    } else if (PyArg_ParseTuple(args, "ll:Box.new", &arg0, &arg1)) {
        box = new Box(arg0, arg1);
    } else if (PyArg_ParseTuple(args, "llll:Box.new", &arg0, &arg1, &arg2, &arg3)) {
        box = new Box(arg0, arg1, arg2, arg3);
    } else {
      PyErr_SetString(ConstructorError, "invalid number of parameters for Box constructor." );
      return ( NULL );
    }
    pyBox = PyObject_NEW(PyBox, &PyTypeBox);
    if (pyBox == NULL) return NULL;

    pyBox->_object = box;
    HCATCH

    return (PyObject*)pyBox;
  }



  // x-------------------------------------------------------------x
  // |                  "PyBox" Object Methods                     |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyBox_DeAlloc,PyBox)
  PyTypeObjectLinkPyType(Box)
  PyTypeObjectConstructor(Box)

#else  // End of Python Module Code Part.


// x=================================================================x
// |                "PyBox" Shared Library Code Part                 |
// x=================================================================x
  





  // ---------------------------------------------------------------
  // PyBox Object Definitions.
  
  PyTypeObjectDefinitions(Box)

# endif  // End of Shared Library Code Part.




}  // End of extern "C".





}  // End of Isobar namespace.
 
