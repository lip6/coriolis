#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyPad.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Pad,pad,function)


// x=================================================================x
// |              "PyPad" Python Module Code Part                |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |               "PyPad" Attribute Methods                 |
  // x-------------------------------------------------------------x


  // Standard Accessors (Attributes).
  DirectGetLongAttribute(PyPad_getX, getX, PyPad, Pad)
  DirectGetLongAttribute(PyPad_getY, getY, PyPad, Pad)

  // Standard Destroy (Attribute).
  DBoDestroyAttribute(PyPad_destroy, PyPad)

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPad_getSourcePosition ()"

  static PyObject* PyPad_getBoundingBox( PyPad *self ) {
    trace << "PyPad_getBoundingBox()" << endl;

    METHOD_HEAD ( "Pad.BoundingBox()" )

    PyBox* pyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (pyBox == NULL) { return NULL; }
    
    HTRY
    pyBox->_object = new Box ( pad->getBoundingBox() );
    HCATCH    

    return ( (PyObject*)pyBox );
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPad_setBoudingBox()"

  static PyObject* PyPad_setBoundingBox(PyPad *self, PyObject* args) {
    trace << "Pad.setBoudingBox()" << endl;

    HTRY
    METHOD_HEAD ( "Pad.setBoudingBox()" )

    PyBox* box;
    if (!ParseOneArg("Pad.setBoundingBox", args, BOX_ARG, (PyObject**)&box)) {
        return NULL;
    }
    pad->setBoundingBox(*PYBOX_O(box));
    HCATCH

    Py_RETURN_NONE;
  }

  // ---------------------------------------------------------------
  // PyPad Attribute Method table.

  PyMethodDef PyPad_Methods[] =
    { { "getX"                 , (PyCFunction)PyPad_getX       , METH_NOARGS, "Return the Pad X value." }
    , { "getY"                 , (PyCFunction)PyPad_getY       , METH_NOARGS, "Return the Pad Y value." }
    , { "getBoundingBox"       , (PyCFunction)PyPad_getBoundingBox, METH_NOARGS, "Return the Pad Bounding Box." }
    , { "setBoundingBox"       , (PyCFunction)PyPad_setBoundingBox, METH_VARARGS, "Sets the Pad Bounding Box." }
    , { "destroy"              , (PyCFunction)PyPad_destroy          , METH_NOARGS
                               , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // x-------------------------------------------------------------x
  // |                "PyPad" Object Methods                       |
  // x-------------------------------------------------------------x

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPad_new ()"

  static PyObject* PyPad_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyPad_new()" << endl;

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    Pad* pad = NULL;

    HTRY
    __cs.init ("Pad.new");
    if (!PyArg_ParseTuple(args,"O&O&O&:Pad.new"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Pad constructor." );
        return NULL;
    }

    if ( __cs.getObjectIds() == ":ent:layer:box") {
      pad = Pad::create(PYNET_O(arg0), PYLAYER_O(arg1), *PYBOX_O(arg2));
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Pad constructor." );
      return NULL;
    }

    HCATCH

    return PyPad_Link(pad);
  }


  DBoDeleteMethod(Pad)
  PyTypeObjectLinkPyType(Pad)
  PyTypeObjectConstructor(Pad)


#else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyPad" Shared Library Code Part                    |
// x=================================================================x


  // Link/Creation Method.
  DBoLinkCreateMethod(Pad)


  // ---------------------------------------------------------------
  // PyPad Object Definitions.


  PyTypeObjectDefinitions(Pad)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
