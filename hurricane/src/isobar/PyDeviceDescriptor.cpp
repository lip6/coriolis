// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyDeviceDescriptor.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyDeviceDescriptor.h"


namespace Isobar {

  using namespace Hurricane;

  extern "C" {

#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(DeviceDescriptor,devdesc,function)


// +=================================================================+
// |          "PyDeviceDescriptor" Python Module Code Part           |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  extern PyObject* PyDeviceDescriptor_setSpiceFilePath ( PyDeviceDescriptor* self, PyObject* args )
  {
    cdebug.log(49) << "PyDeviceDescriptor_setSpiceFilePath ()" << endl;

    HTRY
      METHOD_HEAD("DeviceDescriptor.setSpiceFilePath()")
  
      char* name   = NULL;
      if (PyArg_ParseTuple(args,"s:DeviceDescriptor.setSpiceFilePath", &name)) {
        devdesc->setSpiceFilePath( name );
      } else {
        PyErr_SetString( ConstructorError
                       , "DeviceDescriptor::setSpiceFilePath(): Invalid number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  extern PyObject* PyDeviceDescriptor_addConnector ( PyDeviceDescriptor* self, PyObject* args )
  {
    cdebug.log(49) << "PyDeviceDescriptor_addConnector ()" << endl;

    HTRY
      METHOD_HEAD("DeviceDescriptor.addConnector()")
  
      char* name   = NULL;
      if (PyArg_ParseTuple(args,"s:DeviceDescriptor.addConnector", &name)) {
        devdesc->addConnector( name );
      } else {
        PyErr_SetString( ConstructorError
                       , "DeviceDescriptor::addConnector(): Invalid number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  extern PyObject* PyDeviceDescriptor_addLayout ( PyDeviceDescriptor* self, PyObject* args )
  {
    cdebug.log(49) << "PyDeviceDescriptor_addLayout ()" << endl;

    HTRY
      METHOD_HEAD("DeviceDescriptor.addLayout()")
  
      char* layoutName = NULL;
      char* moduleName = NULL;
      if (PyArg_ParseTuple(args,"ss:DeviceDescriptor.addLayout", &layoutName, &moduleName)) {
        devdesc->addLayout( layoutName, moduleName );
      } else {
        PyErr_SetString( ConstructorError
                       , "DeviceDescriptor::addLayout(): Invalid number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  // Standart Accessors (Attributes).

  // Standard destroy (Attribute).
  //DirectDestroyAttribute(PyDeviceDescriptor_destroy, PyDeviceDescriptor) //not needed


  PyMethodDef PyDeviceDescriptor_Methods[] =
    { { "setSpiceFilePath"  , (PyCFunction)PyDeviceDescriptor_setSpiceFilePath, METH_VARARGS
                            , "Set the spice model associated to this DeviceDescriptor." }
    , { "addConnector"      , (PyCFunction)PyDeviceDescriptor_addConnector    , METH_VARARGS
                            , "Add a new connector <name> to the DeviceDescriptor." }
    , { "addLayout"         , (PyCFunction)PyDeviceDescriptor_addLayout       , METH_VARARGS
                            , "Add a new layout generator <name>, done by <module>." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  PythonOnlyDeleteMethod(DeviceDescriptor)
  PyTypeObjectLinkPyType(DeviceDescriptor)


#else  // End of Python Module Code Part.


// +=================================================================+
// |         "PyDeviceDescriptor" Shared Library Code Part           |
// +=================================================================+


  LinkCreateMethod(DeviceDescriptor)
  PyTypeObjectDefinitions(DeviceDescriptor)


#endif  // End of Shared Library Code Part.

  }  // extern "C".

}  // Isobar namespace.
 
