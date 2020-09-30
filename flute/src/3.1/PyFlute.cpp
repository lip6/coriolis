// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       F l u t e  -  Flute / Python Interface                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyFlute.cpp"                                 |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPoint.h"
#include "crlcore/Utilities.h"
#include <sstream>
#include <stddef.h>
#include "flute.h"


namespace  Flute {

  using namespace Hurricane;
  using namespace Isobar;
  using namespace std;
  using CRL::System;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |               "PyFlute" Shared Library Code Part                |
// +=================================================================+


# else // PyFlute Shared Library Code Part.

// +=================================================================+
// |                "PyFlute" Python Module Code Part                |
// +=================================================================+


extern "C" {

  static PyObject* PyFlute_readLUT ( PyObject* self, PyObject* )
  {
    HTRY
      Flute::readLUT( System::getPath( "coriolis_top" ).toString() );
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyFlute_flute ( PyObject* self, PyObject* args )
  {
    PyObject* treeTuple = NULL;
    HTRY
      PyObject* pyPoints = NULL;
      if (PyArg_ParseTuple(args,"O:Flute.flute()",&pyPoints)) {
      } else {
        PyErr_SetString( ConstructorError, "Flute.flute(): Takes only one argument." );
        return NULL;
      }
      if (not PyList_Check(pyPoints)) {
        PyErr_SetString( ConstructorError, "Flute.flute(): Argument must be a list." );
        return NULL;
      }
      size_t   size     = PyList_Size( pyPoints );
      int      accuracy = 3;
      int64_t* xs       = new int64_t [size];
      int64_t* ys       = new int64_t [size];

      for ( size_t i=0 ; i<size ; ++i ) {
        PyObject* pyPoint = PyList_GetItem( pyPoints, i );
        if (not PyTuple_Check(pyPoint) or (PyTuple_Size(pyPoint) != 2)) {
          ostringstream message;
          message << "Flute.flute(): Item " << i << " of the list is *not* a 2 elements tuple.";
          PyErr_SetString( Isobar::ConstructorError, message.str().c_str() );
          return NULL;
        }
        xs[ i ] = (int64_t)PyInt_AsLong( PyTuple_GetItem( pyPoint, 0 ) );
        ys[ i ] = (int64_t)PyInt_AsLong( PyTuple_GetItem( pyPoint, 1 ) );
      }

      Tree tree = flute( size, xs, ys, accuracy );

      treeTuple = PyTuple_New( 2*tree.deg - 2 );
      for ( size_t i=0 ; (int)i < 2*tree.deg - 2 ; ++i ) {
        PyObject* flutePoint = PyTuple_New( 3 );
        PyTuple_SetItem( flutePoint, 0, PyInt_FromLong(           tree.branch[i].n) );
        PyTuple_SetItem( flutePoint, 1, PyDbU_FromLong((DbU::Unit)tree.branch[i].x) );
        PyTuple_SetItem( flutePoint, 2, PyDbU_FromLong((DbU::Unit)tree.branch[i].y) );
        PyTuple_SetItem( treeTuple, i, flutePoint);
      }
    HCATCH

    return treeTuple;
  }


  static PyMethodDef PyFlute_Methods[] =
    { { "flute"             , PyFlute_flute  , METH_VARARGS, "Call Flute on a set of points." }
    , { "readLUT"           , PyFlute_readLUT, METH_NOARGS , "Load POWV9.dat & POST9.dat." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  // ---------------------------------------------------------------
  // Module Initialization  :  "initFlute ()"

  DL_EXPORT(void) initFlute ()
  {
    cdebug_log(20,0) << "initFlute()" << endl;

    PyObject* module = Py_InitModule( "Flute", PyFlute_Methods );
    if (not module) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Flute module." << endl;
      return;
    }

    cdebug_log(20,0) << "Flute.so loaded " << (void*)&typeid(string) << endl;
  }

  
} // extern "C".

#endif  // Python Module Code Part.

}  // Isobar namespace.
