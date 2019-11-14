// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyLayoutGenerator.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyBox.h"
#include "hurricane/analog/PyDevice.h"
#include "hurricane/analog/PyLayoutGenerator.h"
#include <sstream>


namespace  Isobar {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using namespace Analog;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(LayoutGenerator,generator,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |          "PyLayoutGenerator" Python Module Code Part            |
// +=================================================================+


  static PyObject* PyLayoutGenerator_getVerboseLevel ( PyObject*  )
  { return Py_BuildValue( "i", LayoutGenerator::getVerboseLevel() ); }


  static PyObject* PyLayoutGenerator_setVerboseLevel ( PyObject*, PyObject* args  )
  {
    int  verboseLevel = 0;
    if (PyArg_ParseTuple( args, "i:LayoutGenerator.setVerboseLevel", &verboseLevel )) {
      switch ( verboseLevel ) {
        case LayoutGenerator::Quiet:
        case LayoutGenerator::Verbose:
        case LayoutGenerator::VeryVerbose:
        case LayoutGenerator::Debug: break;
        default:
          PyErr_SetString( ConstructorError, "LayoutGenerator.setVerboseLevel(): Unsupported integer value." );
          return NULL;
      }
      LayoutGenerator::setVerboseLevel( verboseLevel );
    } else {
      PyErr_SetString( ConstructorError, "LayoutGenerator.setVerboseLevel(): Bad parameter type." );
      return NULL;
    }

    Py_RETURN_NONE;
  }


  static PyObject* PyLayoutGenerator_getDeviceBox ( PyLayoutGenerator *self )
  {
    METHOD_HEAD( "LayoutGenerator.getDeviceBox()" )

    PyBox* pyBox = PyObject_NEW( PyBox, &PyTypeBox );
    if (pyBox == NULL) return NULL;

    HTRY
      pyBox->_object = new Box ( self->ACCESS_OBJECT->getDeviceBox() );
    HCATCH

    return (PyObject*)pyBox;
  }


  static PyObject* PyLayoutGenerator_getActiveBox ( PyLayoutGenerator *self )
  {
    METHOD_HEAD( "LayoutGenerator.getActiveBox()" )

    PyBox* pyBox = PyObject_NEW( PyBox, &PyTypeBox );
    if (pyBox == NULL) return NULL;

    HTRY
      pyBox->_object = new Box ( self->ACCESS_OBJECT->getDeviceBox() );
    HCATCH

    return (PyObject*)pyBox;
  }


  static PyObject* PyLayoutGenerator_getRow ( PyLayoutGenerator *self, PyObject* args )
  {
    METHOD_HEAD( "LayoutGenerator.getRow()" )

    int       row   = 0;
    PyObject* pyRow = NULL;

    HTRY
      if (PyArg_ParseTuple( args, "i:LayoutGenerator.getRow", &row )) {
        pyRow = generator->getRow( row );
        if (not pyRow) Py_RETURN_NONE;
        Py_INCREF( pyRow );
      } else {
        PyErr_SetString( ConstructorError, "LayoutGenerator.getRow(): Bad parameter type." );
        return NULL;
      }
    HCATCH
 
    return pyRow;
  }


  static PyObject* PyLayoutGenerator_getDic ( PyLayoutGenerator *self, PyObject* args )
  {
    METHOD_HEAD( "LayoutGenerator.getDic()" )

    int       column = 0;
    PyObject* pyRow  = NULL;
    PyObject* pyDic  = NULL;

    HTRY
      if (PyArg_ParseTuple( args, "Oi:LayoutGenerator.getDic", &pyRow, &column )) {
        pyDic = generator->getDic( pyRow, column );
        if (not pyDic) Py_RETURN_NONE;
        Py_INCREF( pyDic );
      } else {
        PyErr_SetString( ConstructorError, "LayoutGenerator.getDic(): Bad parameter type." );
        return NULL;
      }
    HCATCH
 
    return pyDic;
  }


  static PyObject* PyLayoutGenerator_getParamValue ( PyLayoutGenerator *self, PyObject* args )
  {
    METHOD_HEAD( "LayoutGenerator.getParamValue()" )

    char*     paramName    = NULL;
    PyObject* pyDic        = NULL;
    PyObject* pyParamValue = NULL;

    HTRY
      if (PyArg_ParseTuple( args, "Os:LayoutGenerator.getParamValue", &pyDic, &paramName )) {
        pyParamValue = generator->getParamValue( pyDic, paramName );
        if (not pyParamValue) Py_RETURN_NONE;
        Py_INCREF( pyParamValue );
      } else {
        PyErr_SetString( ConstructorError, "LayoutGenerator.getParamValue(): Bad parameter type." );
        return NULL;
      }
    HCATCH
 
    return pyParamValue;
  }


  static PyObject* PyLayoutGenerator_getParameterValue ( PyLayoutGenerator *self, PyObject* args )
  {
    METHOD_HEAD( "LayoutGenerator.getParameterValue()" )

    bool      status     = false;
    double    paramValue = 0.0;
    PyObject* arg0       = NULL;
    PyObject* arg1       = NULL;
    PyObject* arg2       = NULL;

    HTRY
      __cs.init( "LayoutGenerator.getParameterValue" );
      if (not PyArg_ParseTuple( args, "O&O&|O&:LayoutGenerator.getParameterValue"
                              , Converter, &arg0
                              , Converter, &arg1
                              , Converter, &arg2
                              ) ) {
        PyErr_SetString( ConstructorError, "LayoutGenerator.getParameterValue(): Invalid number of parameters." );
        return NULL;
    //}
    //if (__cs.getObjectIds() == ":string:string") {
    //  paramValue = generator->getParameterValue( PyString_AsString(arg0), PyString_AsString(arg1), status );
      } if (__cs.getObjectIds() == ":int:int:string") {
        paramValue = generator->getParameterValue( PyInt_AsLong(arg0)
                                                 , PyInt_AsLong(arg1)
                                                 , PyString_AsString(arg2)
                                                 , status );
      } else {
        PyErr_SetString( ConstructorError, "LayoutGenerator.getParameterValue(): Bad parameter type." );
        return NULL;
      }
      if (not status) Py_RETURN_NONE;
    HCATCH
 
    return Py_BuildValue( "d", paramValue );
  }


  static PyObject* PyLayoutGenerator_getMatrix ( PyLayoutGenerator* self )
  {
    METHOD_HEAD("LayoutGenerator.getMatrix()")
    PyObject* matrix = generator->getMatrix();
    if (not matrix) Py_RETURN_NONE;
    Py_INCREF( matrix );
    return matrix;
  }


  static PyObject* PyLayoutGenerator_new ( PyTypeObject* type, PyObject* args, PyObject* kwds )
  {
    cdebug.log(49) << "PyLayoutGenerator_new()" << endl;

    LayoutGenerator*   generator         = NULL;
    PyLayoutGenerator* pyLayoutGenerator = (PyLayoutGenerator*)type->tp_alloc(type,0);

    HTRY
    if (pyLayoutGenerator) {
      generator = new LayoutGenerator();
      pyLayoutGenerator->_object = generator;
    }
    HCATCH

    return (PyObject*)pyLayoutGenerator;
  }


  static PyObject* PyLayoutGenerator_Repr ( PyLayoutGenerator* self )
  {
    LayoutGenerator* generator = self->_object;

    if ( generator == NULL )
      return PyString_FromString("<PyObject unbound>");

    ostringstream s;
    s << "<LayoutGenerator ";
    if (generator->getDevice()) s << getString(generator->getDevice());
    else                        s << "NULL [Device*]";
    s << ">";
    return PyString_FromString(s.str().c_str());
  }


  static PyObject* PyLayoutGenerator_getDevice ( PyLayoutGenerator* self )
  {
    METHOD_HEAD("LayoutGenerator.getDevice()")
    return PyDevice_Link( generator->getDevice() );
  }


  static PyObject* PyLayoutGenerator_setDevice ( PyLayoutGenerator* self, PyObject* args )
  {
    HTRY
    METHOD_HEAD("LayoutGenerator.setDevice()")

    PyObject* pyDevice = NULL;

    if (PyArg_ParseTuple( args, "O", &pyDevice )) {
      if (not PyObject_IsInstance(pyDevice,(PyObject*)&PyTypeDevice)) {
        PyErr_SetString( ConstructorError, "LayoutGenerator.setDevice(): Argument is not of type Device." );
        return NULL;
      }
      generator->setDevice( PYDEVICE_O(pyDevice) );
    } else {
      PyErr_SetString( ConstructorError, "Bad parameter given to LayoutGenerator.setDevice()." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }

  
  DirectGetBoolAttribute(PyLayoutGenerator_checkScript        ,checkScript        ,PyLayoutGenerator,LayoutGenerator)
  DirectGetBoolAttribute(PyLayoutGenerator_checkFunctions     ,checkFunctions     ,PyLayoutGenerator,LayoutGenerator)
  DirectGetBoolAttribute(PyLayoutGenerator_drawLayout         ,drawLayout         ,PyLayoutGenerator,LayoutGenerator)
  DirectGetUIntAttribute(PyLayoutGenerator_getNumberTransistor,getNumberTransistor,PyLayoutGenerator,LayoutGenerator)
  DirectGetUIntAttribute(PyLayoutGenerator_getNumberStack     ,getNumberStack     ,PyLayoutGenerator,LayoutGenerator)


  PyMethodDef PyLayoutGenerator_Methods[] =
    { { "getVerboseLevel"    , (PyCFunction)PyLayoutGenerator_getVerboseLevel    , METH_NOARGS|METH_STATIC, "Return the verbosity level." }
    , { "setVerboseLevel"    , (PyCFunction)PyLayoutGenerator_setVerboseLevel    ,             METH_STATIC, "Sets the verbosity level." }
    , { "getDevice"          , (PyCFunction)PyLayoutGenerator_getDevice          , METH_NOARGS , "Return the Device currently loaded." }
    , { "getNumberTransistor", (PyCFunction)PyLayoutGenerator_getNumberTransistor, METH_NOARGS , "Return how many real transistors (fingers) are useds." }
    , { "getNumberStack"     , (PyCFunction)PyLayoutGenerator_getNumberStack     , METH_NOARGS , "Return how many transistor stacks are useds." }
    , { "getDeviceBox"       , (PyCFunction)PyLayoutGenerator_getDeviceBox       , METH_NOARGS , "Return the box encompassing all components." }
    , { "getActiveBox"       , (PyCFunction)PyLayoutGenerator_getActiveBox       , METH_NOARGS , "Return the box ecompassing only active areas." }
    , { "getMatrix"          , (PyCFunction)PyLayoutGenerator_getMatrix          , METH_NOARGS , "Return the whole result matrix." }
    , { "getRow"             , (PyCFunction)PyLayoutGenerator_getRow             , METH_VARARGS, "Matrix access, details unknown." }
    , { "getDic"             , (PyCFunction)PyLayoutGenerator_getDic             , METH_VARARGS, "Matrix access, details unknown." }
    , { "getParamValue"      , (PyCFunction)PyLayoutGenerator_getParamValue      , METH_VARARGS, "Matrix access, details unknown." }
    , { "getParameterValue"  , (PyCFunction)PyLayoutGenerator_getParameterValue  , METH_VARARGS, "Matrix access, details unknown." }
    , { "checkScript"        , (PyCFunction)PyLayoutGenerator_checkScript        , METH_NOARGS , "Look for the Python layout script." }
    , { "checkFunctions"     , (PyCFunction)PyLayoutGenerator_checkFunctions     , METH_NOARGS , "Look for the mandatories functions in the script." }
    , { "drawLayout"         , (PyCFunction)PyLayoutGenerator_drawLayout         , METH_NOARGS , "Draw the layout of the loaded device." }
    , { "setDevice"          , (PyCFunction)PyLayoutGenerator_setDevice          , METH_VARARGS, "Set the device to handle." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  DirectDeleteMethod(PyLayoutGenerator_DeAlloc,PyLayoutGenerator)
  DirectHashMethod(PyLayoutGenerator_Hash,LayoutGenerator)

  extern void  PyLayoutGenerator_LinkPyType() {
    cdebug.log(49) << "PyLayoutGenerator_LinkType()" << endl;

    PyTypeLayoutGenerator.tp_new     =              PyLayoutGenerator_new;
    PyTypeLayoutGenerator.tp_dealloc = (destructor) PyLayoutGenerator_DeAlloc;
    PyTypeLayoutGenerator.tp_repr    = (reprfunc)   PyLayoutGenerator_Repr;
    PyTypeLayoutGenerator.tp_str     = (reprfunc)   PyLayoutGenerator_Repr;
    PyTypeLayoutGenerator.tp_hash    = (hashfunc)   PyLayoutGenerator_Hash;
    PyTypeLayoutGenerator.tp_methods = PyLayoutGenerator_Methods;
  }


#else  // End of Python Module Code Part.

// +=================================================================+
// |          "PyLayoutGenerator" Shared Library Code Part           |
// +=================================================================+

  // Link/Creation Method.
  PyObject* PyLayoutGenerator_Link ( LayoutGenerator* object )
  {
    if ( object == NULL ) Py_RETURN_NONE;

    PyLayoutGenerator* pyObject = NULL;
    HTRY
    pyObject = PyObject_NEW(PyLayoutGenerator, &PyTypeLayoutGenerator);
    if (pyObject == NULL) return NULL;

    pyObject->_object = object;
    HCATCH

    return (PyObject*)pyObject;
  }

  PyTypeObjectDefinitions(LayoutGenerator)


  extern  void  PyLayoutGenerator_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeLayoutGenerator.tp_dict,LayoutGenerator::Quiet        ,"Quiet"        );
    LoadObjectConstant(PyTypeLayoutGenerator.tp_dict,LayoutGenerator::Verbose      ,"Verbose"      );
    LoadObjectConstant(PyTypeLayoutGenerator.tp_dict,LayoutGenerator::VeryVerbose  ,"VeryVerbose"  );
    LoadObjectConstant(PyTypeLayoutGenerator.tp_dict,LayoutGenerator::Debug        ,"Debug"        );
    LoadObjectConstant(PyTypeLayoutGenerator.tp_dict,LayoutGenerator::NoFlags      ,"NoFlags"      );
    LoadObjectConstant(PyTypeLayoutGenerator.tp_dict,LayoutGenerator::ShowTimeTag  ,"ShowTimeTag"  );
    LoadObjectConstant(PyTypeLayoutGenerator.tp_dict,LayoutGenerator::ShowError    ,"ShowError"    );
    LoadObjectConstant(PyTypeLayoutGenerator.tp_dict,LayoutGenerator::StatusOk     ,"StatusOk"     );
    LoadObjectConstant(PyTypeLayoutGenerator.tp_dict,LayoutGenerator::ComputeBbOnly,"ComputeBbOnly");
  }


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
