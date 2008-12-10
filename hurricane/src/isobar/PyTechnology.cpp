// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :          Damien.Dupuis@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyTechnology.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "hurricane/isobar/PyDataBase.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyTechnology.h"


namespace Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Technology,techno,function)


// x=================================================================x
// |             "PyTechnology" Python Module Code Part              |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |              "PyTechnology" Attribute Methods               |
  // x-------------------------------------------------------------x




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTechnology_getName ()"

  static PyObject* PyTechnology_getName(PyTechnology *self) {
    trace << "PyTechnology_getName ()" << endl;

    HTRY
    METHOD_HEAD("Technology.getName()")
    return PyString_FromString(getString(techno->getName()).c_str());
    HCATCH

    return NULL;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTechnology_setName ()"

  static PyObject* PyTechnology_setName(PyTechnology *self, PyObject* args) {
    trace << "Technology.setName()" << endl;

    HTRY
    METHOD_HEAD ( "Technology.setName()" )
    char* name = NULL;
    if (PyArg_ParseTuple(args,"s:Technology.setName", &name)) {
        techno->setName(Name(name));
    } else {
        PyErr_SetString(ConstructorError, "invalid number of parameters for Technology.setName.");
        return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTechnology_getLayer ()"

  static PyObject* PyTechnology_getLayer ( PyTechnology *self, PyObject* args ) {
    trace << "Technology.getLayer()" << endl;
    METHOD_HEAD("Technology.getLayer()")

    Layer* layer = NULL;
    
    HTRY
    char* name = NULL;
    if (PyArg_ParseTuple(args,"s:Technology.getLayer", &name)) {
        layer = techno->getLayer(Name(name));
    } else {
        PyErr_SetString(ConstructorError, "invalid number of parameters for getLayer.");
        return NULL;
    }
    HCATCH
    
    return PyLayer_Link(layer);
  }




  // Standart Accessors (Attributes).

  // Standard destroy (Attribute).
  DBoDestroyAttribute(PyTechnology_destroy, PyTechnology)




  // ---------------------------------------------------------------
  // PyTechnology Attribute Method table.

  PyMethodDef PyTechnology_Methods[] =
    { { "getName"      , (PyCFunction)PyTechnology_getName      , METH_NOARGS , "Returns the name of the technology." }
    , { "setName"      , (PyCFunction)PyTechnology_setName      , METH_VARARGS, "Allows to change the technology name." }
    , { "getLayer"     , (PyCFunction)PyTechnology_getLayer     , METH_VARARGS, "Returns the layer named name." }
    , { "destroy"      , (PyCFunction)PyTechnology_destroy      , METH_NOARGS
                       , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |               "PyTechnology" Object Methods                 |
  // x-------------------------------------------------------------x


  DBoDeleteMethod(Technology)
  PyTypeObjectLinkPyType(Technology)

#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyTechnology" Shared Library Code Part              |
// x=================================================================x


  // Link/Creation Method.
  DBoLinkCreateMethod(Technology)

  // ---------------------------------------------------------------
  // PyTechnology Object Definitions.
  PyTypeObjectDefinitions(Technology)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
