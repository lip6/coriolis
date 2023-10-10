// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyMeltemiEngine.cpp"                         |
// +-----------------------------------------------------------------+

#include "meltemi/PyMeltemiEngine.h"

#include "hurricane/Cell.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "hurricane/viewer/PyCellViewer.h"
#include "meltemi/MeltemiEngine.h"

#undef ACCESS_OBJECT
#undef ACCESS_CLASS
#define ACCESS_OBJECT _baseObject._object
#define ACCESS_CLASS(_pyObject) &(_pyObject->_baseObject)
#define METHOD_HEAD(function) \
  GENERIC_METHOD_HEAD(MeltemiEngine, meltemi, function)

namespace Meltemi {

using CRL::PyToolEngine;
using Hurricane::Bug;
using Hurricane::Error;
using Hurricane::Exception;
using Hurricane::ExceptionWidget;
using Hurricane::tab;
using Hurricane::Warning;
using Isobar::ConstructorError;
using Isobar::EntityCast;
using Isobar::getPyHash;
using Isobar::HurricaneError;
using Isobar::HurricaneWarning;
using Isobar::ParseOneArg;
using Isobar::ParseTwoArg;
using Isobar::ProxyError;
using Isobar::ProxyProperty;
using Isobar::PyBox;
using Isobar::PyCell;
using Isobar::PyCell_Link;
using Isobar::PyCellViewer;
using Isobar::PyInstance;
using Isobar::PyInstance_Link;
using Isobar::PyTypeBox;
using Isobar::PyTypeCellViewer;
using Isobar::PyTypeInstance;
using std::cerr;
using std::endl;
using std::hex;
using std::ostringstream;

extern "C" {

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |          "PyMeltemiEngine" Python Module Code Part              |
// +=================================================================+

static PyObject *PyMeltemiEngine_get(PyObject *, PyObject *args) {
  cdebug_log(34, 0) << "PyMeltemiEngine_get()" << endl;

  MeltemiEngine *meltemi = NULL;

  HTRY PyObject *arg0;

  if (not ParseOneArg("Meltemi.get", args, CELL_ARG, &arg0)) return NULL;
  meltemi = MeltemiEngine::get(PYCELL_O(arg0));
  HCATCH

  return PyMeltemiEngine_Link(meltemi);
}

static PyObject *PyMeltemiEngine_create(PyObject *, PyObject *args) {
  cdebug_log(34, 0) << "PyMeltemiEngine_create()" << endl;

  MeltemiEngine *meltemi = NULL;

  HTRY PyObject *arg0;

  if (not ParseOneArg("Meltemi.create", args, CELL_ARG, &arg0)) return NULL;

  Cell *cell = PYCELL_O(arg0);
  meltemi = MeltemiEngine::get(cell);

  if (meltemi == NULL) {
    meltemi = MeltemiEngine::create(cell);
  } else
    cerr << Warning("%s already has a Meltemi engine.", getString(cell).c_str())
         << endl;
  HCATCH

  return PyMeltemiEngine_Link(meltemi);
}

// Standart Accessors (Attributes).
// DirectVoidMethod(MeltemiEngine,meltemi,runNegociate)
// DirectGetBoolAttribute(PyMeltemiEngine_getToolSuccess,getToolSuccess,PyMeltemiEngine,MeltemiEngine)

// Standart Destroy (Attribute).
DBoDestroyAttribute(PyMeltemiEngine_destroy, PyMeltemiEngine)

    PyMethodDef PyMeltemiEngine_Methods[] = {
        {"get", (PyCFunction)PyMeltemiEngine_get, METH_VARARGS | METH_STATIC,
         "Returns the Meltemi engine attached to the Cell, None if there "
         "isn't."},
        {"create", (PyCFunction)PyMeltemiEngine_create,
         METH_VARARGS | METH_STATIC, "Create an Meltemi engine on this cell."},
        {NULL, NULL, 0, NULL} /* sentinel */
};

DBoDeleteMethod(MeltemiEngine) PyTypeObjectLinkPyType(MeltemiEngine)

#else  // End of Python Module Code Part.

// +=================================================================+
// |           "PyMeltemiEngine" Shared Library Code Part            |
// +=================================================================+

// Link/Creation Method.
PyTypeInheritedObjectDefinitions(MeltemiEngine, Etesian::EtesianEngine);
DBoLinkCreateMethod(MeltemiEngine);

extern void PyMeltemiEngine_postModuleInit() {
  // PyObject* constant;
  // LoadObjectConstant(PyTypeMeltemiEngine.tp_dict,MeltemiEngine::SlowMotion,"SlowMotion");
}

#endif  // Shared Library Code Part.

}  // extern "C".

}  // namespace Meltemi
