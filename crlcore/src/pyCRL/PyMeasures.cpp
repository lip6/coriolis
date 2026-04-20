// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) LIP6 2015-2024, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Roselyne CHOTIN               |
// |  E-mail      :            roselyne.chotin@lip6.fr               |
// | =============================================================== |
// |  C++ Module  :  "./PyMeasures.cpp"                              |
// +-----------------------------------------------------------------+


#include "crlcore/PyMeasures.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/configuration/PyName.h"

//#include <string>
//#include <sstream>

using namespace Isobar;

namespace  CRL {

  using std::cerr;
  using std::endl;
  using Hurricane::Exception;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;


extern "C" {
  
#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyMeasures" Python Module Code Part                |
// +=================================================================+


  static PyObject* PyMeasures_get ( PyObject*, PyObject* args )
  {
    cdebug_log(30,0) << "PyMeasures_get()" << endl;

    PyObject* pyDict = PyDict_New();

    HTRY
      PyObject* pyCell = NULL;
      MeasuresSet* set = NULL;
      if (PyArg_ParseTuple( args, "O:Measures.get", &pyCell )) {
	set = const_cast<MeasuresSet*>(Measures::get(PYCELL_O(pyCell)));
	for ( const auto &p : *set ) {
	  PyDict_SetItem( pyDict, cToPy(p.first), PyUnicode_FromString(p.second->toString(0).c_str()) );
	}
	
      } else {
	PyErr_SetString ( ConstructorError, "Measures.get(): Bad type or bad number of parameters." );
	return NULL;
      }
    HCATCH
      
    return pyDict;
  }

  // Standart Destroy (Attribute).


  PyMethodDef PyMeasures_Methods[] =
    { { "get"                 , (PyCFunction)PyMeasures_get     , METH_VARARGS|METH_STATIC
                              , "Get set of Measures from object." }
  //, { "destroy"             , (PyCFunction)PyMeasures_destroy  , METH_VARARGS
  //                          , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  NoObjectDeleteMethod(Measures)
  PyTypeObjectLinkPyTypeWithoutObject(Measures,Measures)


#else  // End of Python Module Code Part.


// +=================================================================+
// |              "PyMeasures" Shared Library Code Part              |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,Measures)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
