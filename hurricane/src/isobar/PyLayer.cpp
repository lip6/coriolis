// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :          Damien.Dupuis@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyLayer.cpp"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyTechnology.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Layer,layer,function)


// x=================================================================x
// |               "PyLayer" Python Module Code Part                 |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                "PyLayer" Attribute Methods                  |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).


  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyLayer_destroy, PyLayer)

  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyLayer_getTechnology ()"

  static PyObject* PyLayer_getTechnology ( PyLayer *self ) {
    trace << "PyLayer_getTechnology ()" << endl;

    Technology* techno = NULL;

    HTRY
    METHOD_HEAD ( "Layer.getTechnology()" )
    techno = layer->getTechnology ();
    HCATCH
    
    return PyTechnology_Link ( techno );
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyLayer_getName ()"

  GetNameMethod(Layer, layer)

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyLayer_SetName ()"

  SetNameMethod(Layer, layer)

  // ---------------------------------------------------------------
  // PyLayer Attribute Method table.

  PyMethodDef PyLayer_Methods[] =
    { { "getTechnology"       , (PyCFunction)PyLayer_getTechnology       , METH_NOARGS , "Returns the technology owning the layer." }
    , { "getName"             , (PyCFunction)PyLayer_getName             , METH_NOARGS , "Returns the name of the layer." }
    , { "setName"             , (PyCFunction)PyLayer_setName             , METH_VARARGS, "Allows to change the layer name." }
    , { "destroy"             , (PyCFunction)PyLayer_destroy             , METH_NOARGS
                              , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                  "PyLayer" Object Methods                   |
  // x-------------------------------------------------------------x


  DBoDeleteMethod(Layer)
  PyTypeObjectLinkPyType(Layer)


#else  // End of Python Module Code Part.


// x=================================================================x
// |              "PyLayer" Shared Library Code Part                 |
// x=================================================================x


  // Link/Creation Method.
  DBoLinkCreateMethod(Layer)




  // ---------------------------------------------------------------
  // PyLayer Object Definitions.

  PyTypeObjectDefinitions(Layer)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Isobar namespace. 
