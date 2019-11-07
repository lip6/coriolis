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
// |  C++ Module  :  "./PyMatrix.cpp"                                |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyMatrix.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Matrix,matrix,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |               "PyMatrix" Python Module Code Part                |
// +=================================================================+


  DirectGetUIntAttribute(PyMatrix_rows   , rows   , PyMatrix, Matrix)
  DirectGetUIntAttribute(PyMatrix_columns, columns, PyMatrix, Matrix)


  static PyObject* PyMatrix_NEW ( PyObject* , PyObject* args )
  {
    unsigned int  rows     = 0;
    unsigned int  columns  = 0;
    Matrix*       matrix   = NULL;
    PyMatrix*     pyMatrix = NULL;

    HTRY
      if (not PyArg_ParseTuple( args, "II:Matrix_NEW", &rows, &columns ) ) {
        PyErr_SetString ( ConstructorError, "Matrix.create(): Invalid/bad number of parameters ." );
        return NULL;
      }

      pyMatrix = PyObject_NEW( PyMatrix, &PyTypeMatrix );
      if (pyMatrix == NULL) { return NULL; }

      pyMatrix->_object = new Matrix( rows, columns );
    HCATCH

    return (PyObject*)pyMatrix;
  }


  static int  PyMatrix_Init ( PyMatrix* self, PyObject* args, PyObject* kwargs )
  {
    cdebug_log(20,0) << "PyMatrix_Init(): " << (void*)self << endl;
    return 0;
  }


  static PyObject* PyMatrix_resize ( PyMatrix *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyMatrix.resize()" << endl;
   
    HTRY
    METHOD_HEAD ( "Matrix.resize()" )
      unsigned int  rows    = 0;
      unsigned int  columns = 0;
      if (PyArg_ParseTuple(args,"II:Matrix.resize",&rows,&columns)) {
        matrix->resize( (size_t)rows, (size_t)columns );
      } else {
        PyErr_SetString( ConstructorError, "Matrix.resize(): Invalid number/bad type of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyMatrix_getValue ( PyMatrix *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyMatrix.getValue()" << endl;
   
    size_t value = 0;

    HTRY
    METHOD_HEAD ( "Matrix.getValue()" )
      unsigned int  row    = 0;
      unsigned int  column = 0;
      if (PyArg_ParseTuple(args,"II:Matrix.getValue",&row,&column)) {
        value = matrix->at( (size_t)row, (size_t)column );
      } else {
        PyErr_SetString( ConstructorError, "Matrix.getValue(): Invalid number/bad type of parameters." );
        return NULL;
      }
    HCATCH

    return Py_BuildValue("I",(unsigned int)value);
  }


  static PyObject* PyMatrix_setValue ( PyMatrix *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyMatrix.setValue()" << endl;
   
    HTRY
    METHOD_HEAD ( "Matrix.setValue()" )
      unsigned int  row    = 0;
      unsigned int  column = 0;
      unsigned int  value  = 0;
      if (PyArg_ParseTuple(args,"III:Matrix.setValue",&row,&column,&value)) {
        matrix->at( (size_t)row, (size_t)column ) = value;
      } else {
        PyErr_SetString( ConstructorError, "Matrix.setValue(): Invalid number/bad type of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyMatrix Attribute Method table.

  PyMethodDef PyMatrix_Methods[] =
    { { "rows"              , (PyCFunction)PyMatrix_rows   , METH_NOARGS
                            , "Self explanatory." }
    , { "columns"           , (PyCFunction)PyMatrix_columns, METH_NOARGS
                            , "Self explanatory." }
    , { "resize"            , (PyCFunction)PyMatrix_resize    , METH_VARARGS
                            , "Self explanatory." }
    , { "getValue"          , (PyCFunction)PyMatrix_getValue  , METH_VARARGS
                            , "Self explanatory." }
    , { "setValue"          , (PyCFunction)PyMatrix_setValue  , METH_VARARGS
                            , "Self explanatory." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |                "PyMatrix" Object Methods                    |
  // +-------------------------------------------------------------+


  DirectDeleteMethod(PyMatrix_DeAlloc,PyMatrix)
  PyTypeObjectLinkPyTypeNewInit(Matrix)


#else  // End of Python Module Code Part.

// +=================================================================+
// |              "PyMatrix" Shared Library Code Part                |
// +=================================================================+
  

  PyTypeObjectDefinitions(Matrix)


  extern Matrix  Matrix_FromListOfList ( PyObject* pyLoList )
  {
    Matrix matrix;
    
    if (not PyList_Check(pyLoList)) {
      PyErr_SetString( ConstructorError, "Matrix_FromListOfList(): First level type is *not* a list." );
      return matrix;
    }

    Py_ssize_t rows = PyList_Size( pyLoList );
    Py_ssize_t cols = 0;
    
    for ( Py_ssize_t rowIndex = 0; rowIndex<rows ; ++rowIndex ) {
      PyObject* row = PyList_GetItem( pyLoList, rowIndex );
      if (not PyList_Check(row)) {
        ostringstream message;
        message << "Matrix_FromListOfList(): Row [" << rowIndex << "] type is *not* list.";
        PyErr_SetString( ConstructorError, message.str().c_str() );
        return matrix.makeEmpty();
      }

      if (not cols) {
        cols = PyList_Size( row );
        matrix.resize( rows, cols );
      } else {
        if (cols != PyList_Size(row)) {
          ostringstream message;
          message << "Matrix_FromListOfList(): Row [0] and [" << rowIndex << "] have different sizes ("
                  << cols << " vs. " << PyList_Size(row) << ").";
          PyErr_SetString( ConstructorError, message.str().c_str() );
          return matrix.makeEmpty();
        }
      }

      for ( Py_ssize_t colIndex = 0; colIndex<cols ; ++colIndex ) {
        PyObject* element = PyList_GetItem( row, colIndex );
        if (not PyInt_Check(element)) {
          ostringstream message;
          message << "Matrix_FromListOfList(): Element [" << colIndex << "," << rowIndex << "] is not an integer.";
          PyErr_SetString( ConstructorError, message.str().c_str() );
          return matrix.makeEmpty();
        }

        matrix.at( rowIndex, colIndex ) = (size_t)PyInt_AsLong( element );
      }
    }

    return matrix;
  }


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
