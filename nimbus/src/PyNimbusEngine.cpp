
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      N i m b u s  -  Global Routing Framework                   |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyNimbusEngine.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"
#include "nimbus/PyNimbusEngine.h"

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(NimbusEngine,nimbus,function)


namespace  Nimbus {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::PyCell;
  using Isobar::PyCell_Link;
  using CRL::PyToolEngine;


extern "C" {

#if defined(__PYTHON_MODULE__)


// +=================================================================+
// |           "PyNimbusEngine" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyNimbusEngine_create ( PyObject*, PyObject* args )
  {
    trace << "PyNimbusEngine_create()" << endl;

    NimbusEngine* nimbus = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Nimbus.create", args, CELL_ARG, &arg0)) return NULL;

    Cell* cell = PYCELL_O(arg0);
    nimbus = NimbusEngine::get(cell);

    if (nimbus == NULL) {
      nimbus = NimbusEngine::create(cell);
      if (cmess1.enabled())
        nimbus->getConfiguration()->print(cell);
    } else
      cerr << Warning("%s already has a Nimbus engine.",getString(cell).c_str()) << endl;
    HCATCH

    return PyNimbusEngine_Link(nimbus);
  }


  // Standart Accessors (Attributes).
  DirectVoidMethod(NimbusEngine,nimbus,printConfiguration)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyNimbusEngine_destroy,PyNimbusEngine)


  PyMethodDef PyNimbusEngine_Methods[] =
    { { "create"            , (PyCFunction)PyNimbusEngine_create            , METH_VARARGS|METH_STATIC
                            , "Create a Nimbus engine on this cell." }
    , { "printConfiguration", (PyCFunction)PyNimbusEngine_printConfiguration, METH_NOARGS
                            , "Display on the console the configuration of Nimbus." }
    , { "destroy"           , (PyCFunction)PyNimbusEngine_destroy           , METH_NOARGS
                            , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  DBoDeleteMethod(NimbusEngine)
  PyTypeObjectLinkPyType(NimbusEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyNimbusEngine" Shared Library Code Part            |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(NimbusEngine,PyToolEngine)
  DBoLinkCreateMethod(NimbusEngine)


#endif  // End of Shared Library Code Part.

}  // End of extern "C".

}  // End of Nimbus namespace.
 
