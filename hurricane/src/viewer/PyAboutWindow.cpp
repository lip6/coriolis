// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyAboutWindow.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/viewer/PyAboutWindow.h"


namespace Hurricane {

  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;


extern "C" {


#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(AboutWindow,abw,function)


// +=================================================================+
// |             "PyAboutWindow" Python Module Code Part             |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // Standart Destroy (Attribute).
  DirectDestroyAttribute(PyAboutWindow_destroy, PyAboutWindow)


  static PyObject* PyAboutWindow_show ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyAboutWindow_show()" << endl;
    HTRY
      if (AboutWindow::show())
        Py_RETURN_TRUE;
    HCATCH
    Py_RETURN_FALSE;
  }


  // ---------------------------------------------------------------
  // PyAboutWindow Attribute Method table.

  PyMethodDef PyAboutWindow_Methods[] =
    { { "show"              , (PyCFunction)PyAboutWindow_show, METH_STATIC|METH_VARARGS
                            , "Display the About window." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  PythonOnlyDeleteMethod(AboutWindow)
  PyTypeObjectLinkPyTypeWithoutObject(AboutWindow,AboutWindow)

#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyAboutWindow" Shared Library Code Part             |
// +=================================================================+


  PyTypeRootObjectDefinitions(AboutWindow)

#endif  // Shared Library Code Part.

}  // extern "C".

}  // Hurricane namespace.
