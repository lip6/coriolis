// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyMatrixParameter.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyMatrix.h"
#include "hurricane/analog/PyMatrixParameter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(MatrixParameter,matrixParameter,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |           "PyMatrixParameter" Python Module Code Part           |
// +=================================================================+


  DirectGetUIntAttribute(PyMatrixParameter_getRows   , getRows   , PyMatrixParameter, MatrixParameter)
  DirectGetUIntAttribute(PyMatrixParameter_getColumns, getColumns, PyMatrixParameter, MatrixParameter)


  static PyObject* PyMatrixParameter_resize ( PyMatrixParameter *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyMatrixParameter.resize()" << endl;
   
    HTRY
    METHOD_HEAD ( "MatrixParameter.resize()" )
      unsigned int  rows    = 0;
      unsigned int  columns = 0;
      if (PyArg_ParseTuple(args,"II:MatrixParameter.resize",&rows,&columns)) {
        matrixParameter->resize( (size_t)rows, (size_t)columns );
      } else {
        PyErr_SetString( ConstructorError, "MatrixParameter.resize(): Invalid number/bad type of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyMatrixParameter_getValue ( PyMatrixParameter *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyMatrixParameter.getValue()" << endl;
   
    size_t value = 0;

    HTRY
    METHOD_HEAD ( "MatrixParameter.getValue()" )
      unsigned int  row    = 0;
      unsigned int  column = 0;
      if (PyArg_ParseTuple(args,"II:MatrixParameter.getValue",&row,&column)) {
        value = matrixParameter->getValue( (size_t)row, (size_t)column );
      } else {
        PyErr_SetString( ConstructorError, "MatrixParameter.getValue(): Invalid number/bad type of parameters." );
        return NULL;
      }
    HCATCH

    return Py_BuildValue("I",(unsigned int)value);
  }


  static PyObject* PyMatrixParameter_setValue ( PyMatrixParameter *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyMatrixParameter.setValue()" << endl;
   
    HTRY
    METHOD_HEAD ( "MatrixParameter.setValue()" )
      unsigned int  row    = 0;
      unsigned int  column = 0;
      unsigned int  value  = 0;
      if (PyArg_ParseTuple(args,"III:MatrixParameter.setValue",&row,&column,&value)) {
        matrixParameter->setValue( (size_t)row, (size_t)column, (size_t)value );
      } else {
        PyErr_SetString( ConstructorError, "MatrixParameter.setValue(): Invalid number/bad type of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyMatrixParameter_setMatrix ( PyMatrixParameter *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyMatrixParameter.setMatrix()" << endl;
   
    HTRY
    METHOD_HEAD ( "MatrixParameter.setMatrix()" )
      PyObject* pyMatrix = NULL;
      if (not PyArg_ParseTuple(args,"O:MatrixParameter.setMatrix",&pyMatrix)) {
        PyErr_SetString( ConstructorError, "MatrixParameter.setValue(): Invalid number/bad type of parameters." );
        return NULL;
      }
      if (not IsPyMatrix(pyMatrix)) {
        if (not PyList_Check(pyMatrix)) {
          PyErr_SetString( ConstructorError, "MatrixParameter.setMatrix(): Argument is neither of type Matrix not list of list." );
          return NULL;
        }

        Matrix m = Matrix_FromListOfList( pyMatrix );
        if (m.empty()) return NULL; 

        matrixParameter->setMatrix( &m );
      } else {
        matrixParameter->setMatrix( PYMATRIX_O(pyMatrix) );
      }
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyMatrixParameter Attribute Method table.

  PyMethodDef PyMatrixParameter_Methods[] =
    { { "getRows"           , (PyCFunction)PyMatrixParameter_getRows   , METH_NOARGS
                            , "Self explanatory." }
    , { "getColumns"        , (PyCFunction)PyMatrixParameter_getColumns, METH_NOARGS
                            , "Self explanatory." }
    , { "resize"            , (PyCFunction)PyMatrixParameter_resize    , METH_VARARGS
                            , "Self explanatory." }
    , { "getValue"          , (PyCFunction)PyMatrixParameter_getValue  , METH_VARARGS
                            , "Self explanatory." }
    , { "setValue"          , (PyCFunction)PyMatrixParameter_setValue  , METH_VARARGS
                            , "Self explanatory." }
    , { "setMatrix"         , (PyCFunction)PyMatrixParameter_setMatrix , METH_VARARGS
                            , "Self explanatory." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |            "PyMatrixParameter" Object Methods               |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(MatrixParameter)
  PyTypeObjectLinkPyType(MatrixParameter)


#else  // End of Python Module Code Part.

// +=================================================================+
// |          "PyMatrixParameter" Shared Library Code Part           |
// +=================================================================+
  

  LinkCreateMethod(MatrixParameter)
  PyTypeInheritedObjectDefinitions(MatrixParameter, Parameter)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
