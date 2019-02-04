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
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyDevice.cpp"                                |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyVertical.h"
#include "hurricane/isobar/PyHorizontal.h"
#include "hurricane/isobar/PyPad.h"
#include "hurricane/isobar/PyContact.h"
#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyReference.h"
#include "hurricane/analog/PyDevice.h"
#include "hurricane/analog/PyParameter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject)
#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(Device,device,function)


// +=================================================================+
// |                 "PyDevice" Python Module Code Part              |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |                "PyDevice" Attribute Methods                 |
  // +-------------------------------------------------------------+


  static PyObject* PyDevice_getParameter ( PyDevice* self, PyObject* args )
  {
    cdebug.log(49) << "PyDevice_getParameter()" << endl;

    Parameter* parameter = NULL;
      
    HTRY
    METHOD_HEAD ( "Device.getParameter()" )

    char* id   = NULL;
    
    if (PyArg_ParseTuple( args, "s:Device.getParameter", &id )) {
      parameter = device->getParameter( id );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to Device.getParameter()." );
      return NULL;
    }
    HCATCH

    return PyParameter_LinkDerived(parameter);
  }


  static PyObject* PyDevice_setRestrictions ( PyDevice* self, PyObject* args )
  {
    cdebug.log(49) << "PyDevice_setRestrictions()" << endl;

    PyObject*     pyNet = NULL;
    unsigned int  flags = 0;
      
    HTRY
      METHOD_HEAD ( "Device.setRestrictions()" )
  
      if (PyArg_ParseTuple( args, "OI:Device.setRestrictions", &pyNet, &flags )) {
        if (not IsPyNet(pyNet)) {
          PyErr_SetString ( ConstructorError, "Device.setRestrictions(): First argument is not of type Net." );
          return NULL;
        }
        device->setRestrictions2( PYNET_O(pyNet), (unsigned int)flags );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters number or type given to Device.setRestrictions()." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyDevice_getRestrictions ( PyDevice* self, PyObject* args )
  {
    cdebug.log(49) << "PyDevice_getRestrictions()" << endl;

    PyObject*     pyNet = NULL;
    unsigned int  flags = 0;
      
    HTRY
      METHOD_HEAD ( "Device.getRestrictions()" )
  
      if (PyArg_ParseTuple( args, "O:Device.getRestrictions", &pyNet )) {
        if (not IsPyNet(pyNet)) {
          PyErr_SetString ( ConstructorError, "Device.getRestrictions(): First argument is not of type Net." );
          return NULL;
        }
        flags = device->getRestrictions2( PYNET_O(pyNet) );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters number or type given to Device.setRestrictions()." );
        return NULL;
      }
    HCATCH

    return Py_BuildValue( "I", flags );
  }


  // Standart Accessors (Attributes).
  DirectGetNameAttribute  (PyDevice_getDeviceName            ,getDeviceName            ,PyDevice,Device)
  DirectGetStringAttribute(PyDevice_getLayoutScript          ,getLayoutScript          ,PyDevice,Device)
//DirectGetStringAttribute(PyDevice_checkLayoutOnPhysicalGrid,checkLayoutOnPhysicalGrid,PyDevice,Device)
  DirectGetDoubleAttribute(PyDevice_getTemperature           ,getTemperature           ,PyDevice,Device)
  DirectSetDoubleAttribute(PyDevice_setTemperature           ,setTemperature           ,PyDevice,Device)

  DirectVoidMethod(Device,device,destroyLayout)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyDevice_destroy, PyDevice)


  // ---------------------------------------------------------------
  // PyDevice Attribute Method table.

  PyMethodDef PyDevice_Methods[] =
    { { "getDeviceName"     , (PyCFunction)PyDevice_getDeviceName   , METH_NOARGS
                            , "Returns the name of the electrical circuit implemented by the device." }
    , { "getLayoutScript"   , (PyCFunction)PyDevice_getLayoutScript , METH_NOARGS
                            , "Returns the name of the Python script actually used to generate the layout." }
  //, { "checkLayoutOnPhysicalGrid"
  //                        , (PyCFunction)PyDevice_checkLayoutOnPhysicalGrid, METH_NOARGS
  //                        , "Return a list of all the components (rectangles) not on foundry grid." }
    , { "destroyLayout"     , (PyCFunction)PyDevice_destroyLayout   , METH_NOARGS
                            , "Remove the layout (the netlist ." }
    , { "getParameter"      , (PyCFunction)PyDevice_getParameter    , METH_VARARGS
                            , "Get a parameter by it's id." }
    , { "getTemperature"    , (PyCFunction)PyDevice_getTemperature  , METH_NOARGS
                            , "Returns the temperature." }
    , { "setTemperature"    , (PyCFunction)PyDevice_setTemperature  , METH_VARARGS
                            , "Sets the temperature." }
    , { "setRestrictions"   , (PyCFunction)PyDevice_setRestrictions , METH_VARARGS
                            , "Sets the access restrictions for the given net." }
    , { "getRestrictions"   , (PyCFunction)PyDevice_getRestrictions , METH_VARARGS
                            , "Returns the access restrictions for the given Net." }
    , { "destroy"           , (PyCFunction)PyDevice_destroy         , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |                  "PyDevice" Object Methods                  |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(Device)
  PyTypeObjectLinkPyType(Device)


#else  // End of Python Module Code Part.

// +=================================================================+
// |                "PyDevice" Shared Library Code Part              |
// +=================================================================+
  

  DBoLinkCreateMethod(Device)
  PyTypeInheritedObjectDefinitions(Device, Cell)


  extern void  PyDevice_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeDevice.tp_dict,Device::NorthBlocked,"NorthBlocked");
    LoadObjectConstant(PyTypeDevice.tp_dict,Device::SouthBlocked,"SouthBlocked");
    LoadObjectConstant(PyTypeDevice.tp_dict,Device::EastBlocked ,"EastBlocked" );
    LoadObjectConstant(PyTypeDevice.tp_dict,Device::WestBlocked ,"WestBlocked" );
    LoadObjectConstant(PyTypeDevice.tp_dict,Device::AllBlocked  ,"AllBlocked"  );
  }


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
