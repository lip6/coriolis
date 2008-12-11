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




#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyVertical.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(Vertical,vertical,function)

// x=================================================================x
// |              "PyVertical" Python Module Code Part               |
// x=================================================================x

#if defined(__PYTHON_MODULE__)

  
  // x-------------------------------------------------------------x
  // |              "PyVertical" Attribute Methods                 |
  // x-------------------------------------------------------------x


  // Standard destroy (Attribute).
  DBoDestroyAttribute(PyVertical_destroy, PyVertical)


  // ---------------------------------------------------------------
  // PyVertical Attribute Method table.

  PyMethodDef PyVertical_Methods[] =
    { { "destroy"              , (PyCFunction)PyVertical_destroy                     , METH_NOARGS
                               , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                "PyVertical" Object Methods                  |
  // x-------------------------------------------------------------x

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyVertical_new ()"

  static PyObject* PyVertical_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyVertical_new()" << endl;
    Vertical* vertical = NULL;

    HTRY
    PyNet* pyNet=NULL;
    PyComponent *pySource = NULL, *pyTarget = NULL;
    PyLayer* pyLayer = NULL;
    long x = 0, width = 0, dySource = 0, dyTarget = 0;
    if (PyArg_ParseTuple(args,"O!O!l|lll:Vertical.New", &PyTypeNet, &pyNet,
                &PyTypeLayer, &pyLayer, &x, &width, &dySource, &dyTarget)) {
        vertical = Vertical::create(PYNET_O(pyNet), PYLAYER_O(pyLayer), x, width, dySource, dyTarget);
    } else if (PyArg_ParseTuple(args,"O!O!O!l|lll:Vertical.New",
                &PyTypeComponent, &pySource, &PyTypeComponent, &pyTarget,
                &PyTypeLayer, &pyLayer,
                &x, &width, &dySource, &dyTarget)) {
        vertical = Vertical::create(PYCOMPONENT_O(pySource), PYCOMPONENT_O(pyTarget), PYLAYER_O(pyLayer),
                x, width, dySource, dyTarget);
    } else { 
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Vertical constructor." );
      return NULL;
    }
    HCATCH
    return PyVertical_Link(vertical);

  }

  DBoDeleteMethod(Vertical)
  PyTypeObjectLinkPyType(Vertical)
  PyTypeObjectConstructor(Vertical)


#else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyVertical" Shared Library Code Part               |
// x=================================================================x



  // Link/Creation Method.
  DBoLinkCreateMethod(Vertical)

  // ---------------------------------------------------------------
  // PyVertical Object Definitions.

  PyTypeObjectDefinitions(Vertical)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
