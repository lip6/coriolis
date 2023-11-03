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
using Hurricane::Instance;
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
using Meltemi::MeltemiEngine;
using Meltemi::PyMeltemiEngine;
using std::cerr;
using std::endl;
using std::hex;
using std::ostringstream;

extern "C" {

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |          "PyMeltemiEngine" Python Module Code Part              |
// +=================================================================+


DirectVoidMethod(MeltemiEngine, meltemi, setDefaultAb);
DirectVoidMethod(MeltemiEngine, meltemi, resetPlacement);
DirectVoidMethod(MeltemiEngine, meltemi, clearColoquinte);
DirectVoidMethod(MeltemiEngine, meltemi, flattenPower);
DirectVoidMethod(MeltemiEngine, meltemi, toHurricane);
DirectGetUIntAttribute(PyMeltemiEngine_doHFNS, doHFNS, PyMeltemiEngine, MeltemiEngine);
DirectSetLongAttribute(PyMeltemiEngine_setFixedAbHeight, setFixedAbHeight, PyMeltemiEngine, MeltemiEngine);
DirectSetLongAttribute(PyMeltemiEngine_setFixedAbWidth, setFixedAbWidth, PyMeltemiEngine, MeltemiEngine);
DirectSetDoubleAttribute(PyMeltemiEngine_setSpaceMargin, setSpaceMargin, PyMeltemiEngine, MeltemiEngine);
DirectSetDoubleAttribute(PyMeltemiEngine_setDensityVariation, setDensityVariation, PyMeltemiEngine, MeltemiEngine);
DirectSetDoubleAttribute(PyMeltemiEngine_setAspectRatio, setAspectRatio, PyMeltemiEngine, MeltemiEngine);
DirectGetLongAttribute(PyMeltemiEngine_getFixedAbHeight, getFixedAbHeight, PyMeltemiEngine, MeltemiEngine);
DirectGetLongAttribute(PyMeltemiEngine_getFixedAbWidth, getFixedAbWidth, PyMeltemiEngine, MeltemiEngine);
DirectSetCStringAttribute(PyMeltemiEngine_exclude, exclude, PyMeltemiEngine, MeltemiEngine);

static PyObject *PyMeltemiEngine_get(PyObject *, PyObject *args)
{
    cdebug_log(34, 0) << "PyMeltemiEngine_get()" << endl;

    MeltemiEngine *meltemi = NULL;

    HTRY PyObject *arg0;

    if (not ParseOneArg("Meltemi.get", args, CELL_ARG, &arg0)) return NULL;
    meltemi = MeltemiEngine::get(PYCELL_O(arg0));
    HCATCH

    return PyMeltemiEngine_Link(meltemi);
}

static PyObject *PyMeltemiEngine_create(PyObject *, PyObject *args)
{
    cdebug_log(34, 0) << "PyMeltemiEngine_create()" << endl;

    MeltemiEngine *meltemi = NULL;

    HTRY PyObject *arg0;

    if (not ParseOneArg("Meltemi.create", args, CELL_ARG, &arg0)) return NULL;

    Cell *cell = PYCELL_O(arg0);
    meltemi    = MeltemiEngine::get(cell);

    if (meltemi == NULL) {
        meltemi = MeltemiEngine::create(cell);
    } else
        cerr << Warning("%s already has a Meltemi engine.", getString(cell).c_str())
             << endl;
    HCATCH

    return PyMeltemiEngine_Link(meltemi);
}


static PyObject *PyMeltemiEngine_setViewer(PyMeltemiEngine *self, PyObject *args)
{
    cdebug_log(34, 0) << "PyMeltemiEngine_setViewer ()" << endl;

    HTRY
        METHOD_HEAD("MeltemiEngine.setViewer()")

            PyObject *pyViewer = NULL;
    if (not PyArg_ParseTuple(args, "O:MeltemiEngine.setViewer()", &pyViewer)) {
        PyErr_SetString(ConstructorError, "Bad parameters given to MeltemiEngine.setViewer().");
        return NULL;
    }
    if (IsPyCellViewer(pyViewer)) {
        meltemi->setViewer(PYCELLVIEWER_O(pyViewer));
    }
    HCATCH

    Py_RETURN_NONE;
}


static PyObject *PyMeltemiEngine_selectBloat(PyMeltemiEngine *self, PyObject *args)
{
    cdebug_log(34, 0) << "PyMeltemiEngine_selectBloat ()" << endl;

    HTRY
        METHOD_HEAD("MeltemiEngine.selectBloat()")

            const char *profile = NULL;
    if (not PyArg_ParseTuple(args, "s:MeltemiEngine.selectBloat()", &profile)) {
        PyErr_SetString(ConstructorError, "Bad parameters given to MeltemiEngine.selectBloat().");
        return NULL;
    }
    meltemi->selectBloat(profile);
    HCATCH

    Py_RETURN_NONE;
}


static PyObject *PyMeltemiEngine_setBlock(PyMeltemiEngine *self, PyObject *args)
{
    cdebug_log(34, 0) << "PyMeltemiEngine_setBlock()" << endl;
    HTRY
                        METHOD_HEAD("MeltemiEngine.setBlock()")
            PyInstance *pyInstance = NULL;
    if (not ParseOneArg("MeltemiEngine.setBlock", args, INST_ARG, (PyObject **)&pyInstance))
        return NULL;
    Instance *instance = dynamic_cast<Instance *>(EntityCast((PyObject *)pyInstance));
    if (not instance) {
        PyErr_SetString(ConstructorError, "MeltemiEngine.setBlock(): Parameter is not an Instance.");
        return NULL;
    }
    meltemi->setBlock(instance);
    HCATCH
    Py_RETURN_NONE;
}


static PyObject *PyMeltemiEngine_setPlaceArea(PyMeltemiEngine *self, PyObject *args)
{
    cdebug_log(34, 0) << "MeltemiEngine.setPLaceArea()" << endl;
    HTRY
                   METHOD_HEAD("MeltemiEngine.setPLaceArea()")
            PyBox *pyBox;
    if (not PyArg_ParseTuple(args, "O!:MeltemiEngine.setPLaceArea", &PyTypeBox, &pyBox)) {
        PyErr_SetString(ConstructorError, "MeltemiEngine.setPlaceArea(): Parameter is not an Box.");
        return NULL;
    }
    meltemi->setPlaceArea(*PYBOX_O(pyBox));
    HCATCH
    Py_RETURN_NONE;
}


static PyObject *PyMeltemiEngine_place(PyMeltemiEngine *self)
{
    cdebug_log(34, 0) << "PyMeltemiEngine_place()" << endl;
    HTRY
        METHOD_HEAD("MeltemiEngine.place()") if (meltemi->getViewer())
    {
        if (ExceptionWidget::catchAllWrapper(std::bind(&MeltemiEngine::place, meltemi))) {
            PyErr_SetString(HurricaneError, "MeltemiEngine::place() has thrown an exception (C++).");
            return NULL;
        }
    }
    else
    {
        meltemi->place();
    }
    HCATCH
    Py_RETURN_NONE;
}


static PyObject *PyMeltemiEngine_addTrackAvoid(PyMeltemiEngine *self, PyObject *args)
{
    cdebug_log(34, 0) << "MeltemiEngine.addTrackAvoid()" << endl;
    HTRY
                   METHOD_HEAD("MeltemiEngine.addTrackAvoid()")
            PyBox *pyBox;
    if (not PyArg_ParseTuple(args, "O!:MeltemiEngine.addTrackAvoid", &PyTypeBox, &pyBox)) {
        PyErr_SetString(ConstructorError, "MeltemiEngine.addTrackAvoid(): Parameter is not an Box.");
        return NULL;
    }
    meltemi->addTrackAvoid(*PYBOX_O(pyBox));
    HCATCH
    Py_RETURN_NONE;
}

// Standart Destroy (Attribute).
DBoDestroyAttribute(PyMeltemiEngine_destroy, PyMeltemiEngine);

  PyMethodDef PyMeltemiEngine_Methods[] =
    { { "get"               , (PyCFunction)PyMeltemiEngine_get               , METH_VARARGS|METH_STATIC
                            , "Returns the Meltemi engine attached to the Cell, None if there isn't." }
    , { "create"            , (PyCFunction)PyMeltemiEngine_create            , METH_VARARGS|METH_STATIC
                            , "Create an Meltemi engine on this cell." }
    , { "exclude"           , (PyCFunction)PyMeltemiEngine_exclude           , METH_VARARGS
                            , "Add the specified net to the exclusion list." }
    , { "getFixedAbHeight"  , (PyCFunction)PyMeltemiEngine_getFixedAbHeight  , METH_NOARGS
                            , "Returns the forced abutment box height." }
    , { "getFixedAbWidth"   , (PyCFunction)PyMeltemiEngine_getFixedAbWidth   , METH_NOARGS
                            , "Returns the forced abutment box width." }
    , { "setViewer"         , (PyCFunction)PyMeltemiEngine_setViewer         , METH_VARARGS
                            , "Associate a Viewer to this MeltemiEngine." }
    , { "selectBloat"       , (PyCFunction)PyMeltemiEngine_selectBloat       , METH_VARARGS
                            , "Select the Cell bloating profile." }
    , { "setBlock"          , (PyCFunction)PyMeltemiEngine_setBlock          , METH_VARARGS
                            , "Set the sub-block (Instance) to place." }
    , { "setPlaceArea"      , (PyCFunction)PyMeltemiEngine_setPlaceArea      , METH_VARARGS
                            , "Set the sub-area into wich place the cells." }
    , { "setDefaultAb"      , (PyCFunction)PyMeltemiEngine_setDefaultAb      , METH_NOARGS
                            , "Compute and set the abutment box using the aspect ratio and the space margin." }
    , { "setFixedAbHeight"  , (PyCFunction)PyMeltemiEngine_setFixedAbHeight  , METH_VARARGS
                            , "Use this height when computing the size of the default abutment box (disable aspect ratio)." }
    , { "setFixedAbWidth"   , (PyCFunction)PyMeltemiEngine_setFixedAbWidth   , METH_VARARGS
                            , "Use this width when computing the size of the default abutment box (disable aspect ratio)." }
    , { "setSpaceMargin"    , (PyCFunction)PyMeltemiEngine_setSpaceMargin    , METH_VARARGS
                            , "Override the configuration space margin parameter value." }
    , { "setDensityVariation"   , (PyCFunction)PyMeltemiEngine_setDensityVariation, METH_VARARGS
                            , "Override the configuration density variation parameter value." }
    , { "setAspectRatio"    , (PyCFunction)PyMeltemiEngine_setAspectRatio    , METH_VARARGS
                            , "Override the configuration aspect ratio parameter value." }
    , { "resetPlacement"    , (PyCFunction)PyMeltemiEngine_resetPlacement    , METH_NOARGS
                            , "Compute and set the abutment box using the aspect ratio and the space margin." }
    , { "clearColoquinte"   , (PyCFunction)PyMeltemiEngine_clearColoquinte   , METH_NOARGS
                            , "De-allocate the Coloquinte related data structures." }
    , { "place"             , (PyCFunction)PyMeltemiEngine_place             , METH_NOARGS
                            , "Run the placer (Meltemi)." }
    , { "flattenPower"      , (PyCFunction)PyMeltemiEngine_flattenPower      , METH_NOARGS
                            , "Build abstract interface in top cell for supply & blockages." }
    , { "doHFNS"            , (PyCFunction)PyMeltemiEngine_doHFNS            , METH_NOARGS
                            , "Perform the high fanout net synthesis." }
    , { "addTrackAvoid"     , (PyCFunction)PyMeltemiEngine_addTrackAvoid     , METH_VARARGS
                            , "Mark a vertical track under which no terminal should be present." }
    , { "toHurricane"       , (PyCFunction)PyMeltemiEngine_toHurricane       , METH_NOARGS
                            , "Build the Hurricane post-placement manipulation structure." }
    , { "destroy"           , (PyCFunction)PyMeltemiEngine_destroy           , METH_NOARGS
                            , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };

DBoDeleteMethod(MeltemiEngine);
PyTypeObjectLinkPyType(MeltemiEngine);

#else // End of Python Module Code Part.

// +=================================================================+
// |           "PyMeltemiEngine" Shared Library Code Part            |
// +=================================================================+

// Link/Creation Method.
PyTypeInheritedObjectDefinitions(MeltemiEngine, ToolEngine);
DBoLinkCreateMethod(MeltemiEngine);

extern void PyMeltemiEngine_postModuleInit()
{
}

#endif // Shared Library Code Part.

} // extern "C".

} // namespace Meltemi
