// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Sophie BELLOEIL               |
// |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyHorizontal.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyHorizontal.h"


namespace  Isobar {


using namespace Hurricane;


extern "C" {


# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._baseObject._baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject)
# define  METHOD_HEAD(function)    GENERIC_METHOD_HEAD_SUB(Horizontal,horizontal,function)


// x=================================================================x
// |             "PyHorizontal" Python Module Code Part              |
// x=================================================================x

# if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |             "PyHorizontal" Attribute Methods                |
  // x-------------------------------------------------------------x


  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyHorizontal_destroy, PyHorizontal)


  // ---------------------------------------------------------------
  // PyHorizontal Attribute Method table.

  PyMethodDef PyHorizontal_Methods[] =
    { { "destroy"              , (PyCFunction)PyHorizontal_destroy                     , METH_NOARGS
                               , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |               "PyHorizontal" Object Methods                 |
  // x-------------------------------------------------------------x

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyHorizontal_new ()"

  static PyObject* PyHorizontal_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyHorizontal_new()" << endl;
    Horizontal* horizontal = NULL;

    HTRY
    PyNet* pyNet=NULL;
    PyComponent *pySource = NULL, *pyTarget = NULL;
    PyLayer* pyLayer = NULL;
    long y = 0, width = 0, dxSource = 0, dxTarget = 0;
    if (PyArg_ParseTuple(args,"O!O!l|lll:Horizontal.New", &PyTypeNet, &pyNet,
                &PyTypeLayer, &pyLayer, &y, &width, &dxSource, &dxTarget)) {
        horizontal = Horizontal::create(PYNET_O(pyNet), PYLAYER_O(pyLayer), y, width, dxSource, dxTarget);
    } else if (PyArg_ParseTuple(args,"O!O!O!l|lll:Horizontal.New",
                &PyTypeComponent, &pySource, &PyTypeComponent, &pyTarget,
                &PyTypeLayer, &pyLayer,
                &y, &width, &dxSource, &dxTarget)) {
        horizontal = Horizontal::create(PYCOMPONENT_O(pySource), PYCOMPONENT_O(pyTarget), PYLAYER_O(pyLayer),
                y, width, dxSource, dxTarget);
    } else { 
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Horizontal constructor." );
      return NULL;
    }
    HCATCH

    return PyHorizontal_Link ( horizontal );
  }

  DBoDeleteMethod(Horizontal)
  PyTypeObjectLinkPyType(Horizontal)
  PyTypeObjectConstructor(Horizontal)


#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyHorizontal" Shared Library Code Part              |
// x=================================================================x





  // Link/Creation Method.
  DBoLinkCreateMethod(Horizontal)


  // ---------------------------------------------------------------
  // PyHorizontal Object Definitions.

  PyTypeObjectDefinitions(Horizontal)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
