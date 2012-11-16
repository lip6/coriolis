
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |         M E T I S  -  Wrapper around hMetis                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyMetisEngine.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "metis/PyMetisEngine.h"

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(MetisEngine,metis,function)


namespace  Metis {

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
// |            "PyMetisEngine" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyMetisEngine_create ( PyObject*, PyObject* args )
  {
    trace << "PyMetisEngine_create()" << endl;

    MetisEngine* metis = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Metis.create", args, CELL_ARG, &arg0)) return NULL;

    Cell* cell = PYCELL_O(arg0);
    metis = MetisEngine::get(cell);

    if (metis == NULL) {
      metis = MetisEngine::create(cell);
      if (cmess1.enabled())
        metis->getConfiguration()->print(cell);
    } else
      cerr << Warning("%s already has a Metis engine.",getString(cell).c_str()) << endl;
    HCATCH

    return PyMetisEngine_Link(metis);
  }


  PyObject* PyMetisEngine_doQuadriPart ( PyMetisEngine*, PyObject* args )
  {
    trace << "PyMetisEngine_doQuadriPart()" << endl;

    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Metis.doQuadriPart", args, CELL_ARG, &arg0)) return NULL;
    MetisEngine::doQuadriPart(PYCELL_O(arg0));
    HCATCH

    Py_RETURN_NONE;
  }


  // Standart Accessors (Attributes).
  DirectVoidMethod(MetisEngine,metis,printConfiguration)
  DirectGetBoolAttribute(PyMetisEngine_isHMetisCapable,isHMetisCapable,PyMetisEngine,MetisEngine)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyMetisEngine_destroy,PyMetisEngine)


  PyMethodDef PyMetisEngine_Methods[] =
    { { "create"            , (PyCFunction)PyMetisEngine_create            , METH_VARARGS|METH_STATIC
                            , "Create a Metis engine on this cell." }
    , { "isHMetisCapable"   , (PyCFunction)PyMetisEngine_isHMetisCapable   , METH_NOARGS
                            , "Return True the hMetis support is enabled." }
    , { "printConfiguration", (PyCFunction)PyMetisEngine_printConfiguration, METH_NOARGS
                            , "Display on the console the configuration of Metis." }
    , { "doQuadriPart"      , (PyCFunction)PyMetisEngine_doQuadriPart      , METH_VARARGS|METH_STATIC
                            , "Perform a recursive quadri-partition on the cell." }
    , { "destroy"           , (PyCFunction)PyMetisEngine_destroy           , METH_NOARGS
                            , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  DBoDeleteMethod(MetisEngine)
  PyTypeObjectLinkPyType(MetisEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyMetisEngine" Shared Library Code Part            |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(MetisEngine,PyToolEngine)
  DBoLinkCreateMethod(MetisEngine)


#endif  // End of Shared Library Code Part.

}  // End of extern "C".

}  // End of Metis namespace.
 
