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

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;
    PyObject* arg4;
    PyObject* arg5;
    PyObject* arg6;
    Vertical* vertical = NULL;

    HTRY
    __cs.init ("Vertical.new");
    if (!PyArg_ParseTuple(args,"O&O&O&|O&O&O&O&:Vertical.new"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3
                           ,Converter,&arg4
                           ,Converter,&arg5
                           ,Converter,&arg6
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Vertical constructor." );
        return NULL;
    }


    if      ( __cs.getObjectIds() == NET_LAYER_INT_ARG )
      vertical = Vertical::create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2) );
    else if ( __cs.getObjectIds() == NET_LAYER_INTS2_ARG )
      vertical = Vertical::create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2)
                                  , PyInt_AsLong(arg3) );
    else if ( __cs.getObjectIds() == COMPS2_LAYER_INT_ARG )
      vertical = Vertical::create ( ComponentCast(arg0)
                                  , ComponentCast(arg1)
                                  , PYLAYER_O(arg2)
                                  , PyInt_AsLong(arg3) );
    else if ( __cs.getObjectIds() == NET_LAYER_INTS3_ARG )
      vertical = Vertical::create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2)
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4) );
    else if ( __cs.getObjectIds() == COMPS2_LAYER_INTS2_ARG )
      vertical = Vertical::create ( ComponentCast(arg0)
                                  , ComponentCast(arg1)
                                  , PYLAYER_O(arg2)
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4) );
    else if ( __cs.getObjectIds() == NET_LAYER_INTS4_ARG )
      vertical = Vertical::create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyInt_AsLong(arg2)
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4)
                                  , PyInt_AsLong(arg5) );
    else if ( __cs.getObjectIds() == COMPS2_LAYER_INTS3_ARG )
      vertical = Vertical::create ( ComponentCast(arg0)
                                  , ComponentCast(arg1)
                                  , PYLAYER_O(arg2)
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4)
                                  , PyInt_AsLong(arg5) );
    else if ( __cs.getObjectIds() == COMPS2_LAYER_INTS4_ARG )
      vertical = Vertical::create ( ComponentCast(arg0)
                                  , ComponentCast(arg1)
                                  , PYLAYER_O(arg2)
                                  , PyInt_AsLong(arg3)
                                  , PyInt_AsLong(arg4)
                                  , PyInt_AsLong(arg5)
                                  , PyInt_AsLong(arg6) );
    else {
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
 
