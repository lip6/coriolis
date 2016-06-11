// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyBasicLayer.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyBasicLayer.h"
#include "hurricane/isobar/PyMaterial.h"
#include "hurricane/isobar/PyTechnology.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
# define  METHOD_HEAD(function)    GENERIC_METHOD_HEAD(BasicLayer,basicLayer,function)


// +=================================================================+
// |             "PyBasicLayer" Python Module Code Part              |
// +=================================================================+

# if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |             "PyBasicLayer" Attribute Methods                |
  // +-------------------------------------------------------------+


  static PyObject* PyBasicLayer_create ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyBasicLayer_create()" << endl;

    BasicLayer*   basicLayer     = NULL;
    
    HTRY
    PyObject*     pyTechnology   = NULL;
    char*         name           = NULL;
    PyObject*     pyMaterial     = NULL;
    unsigned int  extractNumber  = 0;
    long          minimalSize    = 0;
    long          minimalSpacing = 0;
    
    if (PyArg_ParseTuple( args
                        , "OsO|Ill:BasicLayer.create"
                        , &pyTechnology
                        , &name
                        , &pyMaterial
                        , &extractNumber
                        , &minimalSize
                        , &minimalSpacing
                        )) {
      if (not IsPyTechnology(pyTechnology)) {
        PyErr_SetString ( ConstructorError, "Hurricane.create(): First argument is not of type Technology." );
        return NULL;
      }
      if (not IsPyMaterial(pyMaterial)) {
        PyErr_SetString ( ConstructorError, "Hurricane.create(): Third argument is not of type Material." );
        return NULL;
      }

      basicLayer = BasicLayer::create( PYTECHNOLOGY_O(pyTechnology)
                                     , Name(name)
                                     , *PYMATERIAL_O(pyMaterial)
                                     , extractNumber
                                     , minimalSize
                                     , minimalSpacing
                                     );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to BasicLayer.create()." );
      return NULL;
    }
    HCATCH

    return PyBasicLayer_Link(basicLayer);
  }


  static PyObject* PyBasicLayer_getMaterial ( PyBasicLayer *self )
  {
    cdebug_log(20,0) << "PyBasicLayer_getMaterial ()" << endl;
    METHOD_HEAD ( "BasicLayer.getMaterial()" )

    BasicLayer::Material* material = NULL;
    HTRY
    material = new BasicLayer::Material( basicLayer->getMaterial() );
    HCATCH

    return (PyObject*)PyMaterial_Link( material );
  }


  updatorFromBasicLayer    (setBlockageLayer,PyBasicLayer,BasicLayer)
  accessorLayerFromVoid    (getBlockageLayer,PyBasicLayer,BasicLayer)
  DirectSetLongAttribute   (PyBasicLayer_setExtractNumber,setExtractNumber,PyBasicLayer,BasicLayer)
  DirectSetCStringAttribute(PyBasicLayer_setRealName     ,setRealName     ,PyBasicLayer,BasicLayer)


  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyBasicLayer_destroy, PyBasicLayer)


  // ---------------------------------------------------------------
  // PyBasicLayer Attribute Method table.

  PyMethodDef PyBasicLayer_Methods[] =
    { { "create"             , (PyCFunction)PyBasicLayer_create           , METH_VARARGS|METH_STATIC
                             , "Create a new BasicLayer." }
    , { "getMaterial"        , (PyCFunction)PyBasicLayer_getMaterial      , METH_NOARGS
                             , "Returns the type of Material." }
    , { "getBlockageLayer"   , (PyCFunction)PyBasicLayer_getBlockageLayer , METH_NOARGS
                             , "Returns the associated blockage layer, if any." }
    , { "setBlockageLayer"   , (PyCFunction)PyBasicLayer_setBlockageLayer , METH_VARARGS
                             , "Sets the blockage layer associated to this one." }
    , { "setExtractNumber"   , (PyCFunction)PyBasicLayer_setExtractNumber , METH_VARARGS
                             , "Sets the layer extract number (for GDSII)." }
    , { "setRealName"        , (PyCFunction)PyBasicLayer_setRealName      , METH_VARARGS
                             , "Sets the real name of this layer (as seen in GDSII)." }
    , { "destroy"            , (PyCFunction)PyBasicLayer_destroy          , METH_NOARGS
                             , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |               "PyHorizontal" Object Methods                 |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(BasicLayer)
  PyTypeObjectLinkPyType(BasicLayer)


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyBasicLayer" Shared Library Code Part              |
// +=================================================================+


  // Link/Creation Method.
  DBoLinkCreateMethod(BasicLayer)


  PyTypeInheritedObjectDefinitions(BasicLayer, Layer)


  extern  void  PyBasicLayer_postModuleInit ()
  {
    PyMaterial_postModuleInit ();

    PyDict_SetItemString ( PyTypeBasicLayer.tp_dict, "Material", (PyObject*)&PyTypeMaterial );
  }


#endif  // End of Shared Library Code Part.


}  // extern "C".

}  // Isobar namespace.
 
