// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./bora/PyMatrixParameterRange.cpp"             |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyMatrix.h"
#include "bora/PyMatrixParameterRange.h"


namespace  Bora {

  using namespace Hurricane;
  using namespace Isobar;


extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(MatrixParameterRange,range,function)


// +=================================================================+
// |          "PyMatrixParameterRange" Python Module Code Part       |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  static PyObject* PyMatrixParameterRange_NEW ( PyObject* , PyObject* args )
  {
    MatrixParameterRange* range = NULL;

    HTRY
      range = new MatrixParameterRange();
    HCATCH

    return PyMatrixParameterRange_Link(range);
  }


  static int  PyMatrixParameterRange_Init ( PyMatrixParameterRange* self, PyObject* args, PyObject* kwargs )
  {
    cdebug_log(20,0) << "PyMatrixParameterRange_Init(): " << (void*)self << endl;
    return 0;
  }


  static PyObject* PyMatrixParameterRange_addValue ( PyMatrixParameterRange* self, PyObject* args )
  {
    PyObject* pyMatrix = NULL;

    METHOD_HEAD( "MatrixParameterRange.addValue()" );
    HTRY
      if (not PyArg_ParseTuple( args,"O:MatrixParameterRange.addValue", &pyMatrix ) ) {
        PyErr_SetString ( ConstructorError, "MatrixParameterRange.addValue(): Invalid/bad number of parameters ." );
        return NULL;
      }
      if (not IsPyMatrix(pyMatrix)) {
        if (not PyList_Check(pyMatrix)) {
          PyErr_SetString( ConstructorError, "MatrixParameterRange.addValue(): Argument is neither of type Matrix not list of list." );
          return NULL;
        }

        Matrix m = Matrix_FromListOfList( pyMatrix );
        if (m.empty()) return NULL; 

        range->addValue( &m );
      } else {
        range->addValue( PYMATRIX_O(pyMatrix) );
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyMatrixParameterRange_getValue ( PyMatrixParameterRange* self, PyObject* args )
  {
    Matrix*   matrix   = NULL;
    PyMatrix* pyMatrix = NULL;

    METHOD_HEAD( "MatrixParameterRange.getValue()" );
    HTRY
      pyMatrix = PyObject_NEW( PyMatrix, &PyTypeMatrix );
      if (pyMatrix == NULL) { return NULL; }

      pyMatrix->_object = new Matrix( range->getValue() );
    HCATCH

    return (PyObject*)pyMatrix;
  }


  DirectDeleteMethod(PyMatrixParameterRange_DeAlloc,PyMatrixParameterRange)


  // ---------------------------------------------------------------
  // PyMatrixParameterRange Attribute Method table.

  PyMethodDef PyMatrixParameterRange_Methods[] =
    { { "getValue"          , (PyCFunction)PyMatrixParameterRange_getValue , METH_NOARGS , "To be documented." }
    , { "addValue"          , (PyCFunction)PyMatrixParameterRange_addValue , METH_VARARGS, "To be documented." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |           "PyMatrixParameterRange" Object Methods           |
  // +-------------------------------------------------------------+


  PyTypeObjectLinkPyTypeNewInit(MatrixParameterRange)


#else  // End of Python Module Code Part.

// +=================================================================+
// |         "PyMatrixParameterRange" Shared Library Code Part       |
// +=================================================================+


  extern LinkCreateMethod(MatrixParameterRange);
  

  PyTypeInheritedObjectDefinitions(MatrixParameterRange, ParameterRange)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Bora namespace.
