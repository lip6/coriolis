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
// |  C++ Module  :  "./PyEtesianEngine.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/viewer/PyCellViewer.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "etesian/PyEtesianEngine.h"

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(EtesianEngine,etesian,function)


namespace  Etesian {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::Exception;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::ExceptionWidget;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::getPyHash;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::EntityCast;
  using Isobar::PyBox;
  using Isobar::PyTypeBox;
  using Isobar::PyCell;
  using Isobar::PyCell_Link;
  using Isobar::PyInstance;
  using Isobar::PyInstance_Link;
  using Isobar::PyTypeInstance;
  using Isobar::PyCellViewer;
  using Isobar::PyTypeCellViewer;
  using CRL::PyToolEngine;


extern "C" {

#if defined(__PYTHON_MODULE__)


// +=================================================================+
// |          "PyEtesianEngine" Python Module Code Part              |
// +=================================================================+


  DirectVoidMethod(EtesianEngine,etesian,setDefaultAb)
  DirectVoidMethod(EtesianEngine,etesian,resetPlacement)
  DirectVoidMethod(EtesianEngine,etesian,clearColoquinte)
  DirectVoidMethod(EtesianEngine,etesian,flattenPower)
  DirectVoidMethod(EtesianEngine,etesian,toHurricane)
  DirectGetUIntAttribute   (PyEtesianEngine_doHFNS          ,doHFNS          ,PyEtesianEngine,EtesianEngine)
  DirectSetLongAttribute   (PyEtesianEngine_setFixedAbHeight,setFixedAbHeight,PyEtesianEngine,EtesianEngine)
  DirectSetLongAttribute   (PyEtesianEngine_setFixedAbWidth ,setFixedAbWidth ,PyEtesianEngine,EtesianEngine)
  DirectSetDoubleAttribute (PyEtesianEngine_setSpaceMargin  ,setSpaceMargin  ,PyEtesianEngine,EtesianEngine)
  DirectSetDoubleAttribute (PyEtesianEngine_setAspectRatio  ,setAspectRatio  ,PyEtesianEngine,EtesianEngine)
  DirectGetLongAttribute   (PyEtesianEngine_getFixedAbHeight,getFixedAbHeight,PyEtesianEngine,EtesianEngine)
  DirectGetLongAttribute   (PyEtesianEngine_getFixedAbWidth ,getFixedAbWidth ,PyEtesianEngine,EtesianEngine)
  DirectSetCStringAttribute(PyEtesianEngine_exclude         ,exclude         ,PyEtesianEngine,EtesianEngine)


  static PyObject* PyEtesianEngine_get ( PyObject*, PyObject* args )
  {
    cdebug_log(34,0) << "PyEtesianEngine_get()" << endl;

    EtesianEngine* etesian = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Etesian.get", args, CELL_ARG, &arg0)) return NULL;
    etesian = EtesianEngine::get(PYCELL_O(arg0));
    HCATCH

    return PyEtesianEngine_Link(etesian);
  }


  static PyObject* PyEtesianEngine_create ( PyObject*, PyObject* args )
  {
    cdebug_log(34,0) << "PyEtesianEngine_create()" << endl;

    EtesianEngine* etesian = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Etesian.create", args, CELL_ARG, &arg0)) return NULL;

    Cell* cell = PYCELL_O(arg0);
    etesian = EtesianEngine::get(cell);

    if (etesian == NULL) {
      etesian = EtesianEngine::create(cell);
    //if (cmess1.enabled())
    //  etesian->getEtesianConfiguration()->print(cell);
    } else
      cerr << Warning("%s already has a Etesian engine.",getString(cell).c_str()) << endl;
    HCATCH

    return PyEtesianEngine_Link(etesian);
  }


  static PyObject* PyEtesianEngine_setViewer ( PyEtesianEngine* self, PyObject* args )
  {
    cdebug_log(34,0) << "PyEtesianEngine_setViewer ()" << endl;

    HTRY
      METHOD_HEAD( "EtesianEngine.setViewer()" )
  
      PyObject* pyViewer = NULL;
      if (not PyArg_ParseTuple(args,"O:EtesianEngine.setViewer()",&pyViewer)) {
        PyErr_SetString( ConstructorError, "Bad parameters given to EtesianEngine.setViewer()." );
        return NULL;
      }
      if (IsPyCellViewer(pyViewer)) {
        etesian->setViewer( PYCELLVIEWER_O(pyViewer) );
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyEtesianEngine_selectBloat ( PyEtesianEngine* self, PyObject* args )
  {
    cdebug_log(34,0) << "PyEtesianEngine_selectBloat ()" << endl;

    HTRY
      METHOD_HEAD( "EtesianEngine.selectBloat()" )
  
      const char* profile = NULL;
      if (not PyArg_ParseTuple(args,"s:EtesianEngine.selectBloat()",&profile)) {
        PyErr_SetString( ConstructorError, "Bad parameters given to EtesianEngine.selectBloat()." );
        return NULL;
      }
      etesian->selectBloat( profile );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyEtesianEngine_setBlock ( PyEtesianEngine *self, PyObject* args )
  {
    cdebug_log(34,0) << "PyEtesianEngine_setBlock()" << endl;
    HTRY
      METHOD_HEAD ( "EtesianEngine.setBlock()" )
      PyInstance* pyInstance = NULL;
      if (not ParseOneArg("EtesianEngine.setBlock",args,INST_ARG,(PyObject**)&pyInstance) )
        return NULL;
      Instance* instance = dynamic_cast<Instance*>( EntityCast((PyObject*)pyInstance) );
      if (not instance) {
        PyErr_SetString( ConstructorError, "EtesianEngine.setBlock(): Parameter is not an Instance." );
        return NULL;
      }
      etesian->setBlock( instance );
    HCATCH
    Py_RETURN_NONE;
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyEtesianEngine_setPLaceArea ()"

  static PyObject* PyEtesianEngine_setPlaceArea ( PyEtesianEngine *self, PyObject* args )
  {
    cdebug_log(34,0) << "EtesianEngine.setPLaceArea()" << endl;
    HTRY
      METHOD_HEAD ( "EtesianEngine.setPLaceArea()" )
      PyBox* pyBox;
      if (not PyArg_ParseTuple(args,"O!:EtesianEngine.setPLaceArea", &PyTypeBox, &pyBox)) {
        PyErr_SetString( ConstructorError, "EtesianEngine.setPlaceArea(): Parameter is not an Box." );
        return NULL;
      }
      etesian->setPlaceArea( *PYBOX_O(pyBox) );
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyEtesianEngine_place ( PyEtesianEngine* self )
  {
    cdebug_log(34,0) << "PyEtesianEngine_place()" << endl;
    HTRY
    METHOD_HEAD("EtesianEngine.place()")
    if (etesian->getViewer()) {
      if (ExceptionWidget::catchAllWrapper( std::bind(&EtesianEngine::place,etesian) )) {
        PyErr_SetString( HurricaneError, "EtesianEngine::place() has thrown an exception (C++)." );
        return NULL;
      }
    } else {
      etesian->place();
    }
    HCATCH
    Py_RETURN_NONE;
  }

  // Standart Accessors (Attributes).
  // DirectVoidMethod(EtesianEngine,etesian,runNegociate)
  // DirectGetBoolAttribute(PyEtesianEngine_getToolSuccess,getToolSuccess,PyEtesianEngine,EtesianEngine)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyEtesianEngine_destroy,PyEtesianEngine)


  PyMethodDef PyEtesianEngine_Methods[] =
    { { "get"               , (PyCFunction)PyEtesianEngine_get               , METH_VARARGS|METH_STATIC
                            , "Returns the Etesian engine attached to the Cell, None if there isn't." }
    , { "create"            , (PyCFunction)PyEtesianEngine_create            , METH_VARARGS|METH_STATIC
                            , "Create an Etesian engine on this cell." }
    , { "exclude"           , (PyCFunction)PyEtesianEngine_exclude           , METH_VARARGS
                            , "Add the specified net to the exclusion list." }
    , { "getFixedAbHeight"  , (PyCFunction)PyEtesianEngine_getFixedAbHeight  , METH_NOARGS
                            , "Returns the forced abutment box height." }
    , { "getFixedAbWidth"   , (PyCFunction)PyEtesianEngine_getFixedAbWidth   , METH_NOARGS
                            , "Returns the forced abutment box width." }
    , { "setViewer"         , (PyCFunction)PyEtesianEngine_setViewer         , METH_VARARGS
                            , "Associate a Viewer to this EtesianEngine." }
    , { "selectBloat"       , (PyCFunction)PyEtesianEngine_selectBloat       , METH_VARARGS
                            , "Select the Cell bloating profile." }
    , { "setBlock"          , (PyCFunction)PyEtesianEngine_setBlock          , METH_VARARGS
                            , "Set the sub-block (Instance) to place." }
    , { "setPlaceArea"      , (PyCFunction)PyEtesianEngine_setPlaceArea      , METH_VARARGS
                            , "Set the sub-area into wich place the cells." }
    , { "setDefaultAb"      , (PyCFunction)PyEtesianEngine_setDefaultAb      , METH_NOARGS
                            , "Compute and set the abutment box using the aspect ratio and the space margin." }
    , { "setFixedAbHeight"  , (PyCFunction)PyEtesianEngine_setFixedAbHeight  , METH_VARARGS
                            , "Use this height when computing the size of the default abutment box (disable aspect ratio)." }
    , { "setFixedAbWidth"   , (PyCFunction)PyEtesianEngine_setFixedAbWidth   , METH_VARARGS
                            , "Use this width when computing the size of the default abutment box (disable aspect ratio)." }
    , { "setSpaceMargin"    , (PyCFunction)PyEtesianEngine_setSpaceMargin    , METH_VARARGS
                            , "Override the configuration space margin parameter value." }
    , { "setAspectRatio"    , (PyCFunction)PyEtesianEngine_setAspectRatio    , METH_VARARGS
                            , "Override the configuration aspect ratio parameter value." }
    , { "resetPlacement"    , (PyCFunction)PyEtesianEngine_resetPlacement    , METH_NOARGS
                            , "Compute and set the abutment box using the aspect ratio and the space margin." }
    , { "clearColoquinte"   , (PyCFunction)PyEtesianEngine_clearColoquinte   , METH_NOARGS
                            , "De-allocate the Coloquinte related data structures." }
    , { "place"             , (PyCFunction)PyEtesianEngine_place             , METH_NOARGS
                            , "Run the placer (Etesian)." }
    , { "flattenPower"      , (PyCFunction)PyEtesianEngine_flattenPower      , METH_NOARGS
                            , "Build abstract interface in top cell for supply & blockages." }
    , { "doHFNS"            , (PyCFunction)PyEtesianEngine_doHFNS            , METH_NOARGS
                            , "Perform the high fanout net synthesis." }
    , { "toHurricane"       , (PyCFunction)PyEtesianEngine_toHurricane       , METH_NOARGS
                            , "Build the Hurricane post-placement manipulation structure." }
    , { "destroy"           , (PyCFunction)PyEtesianEngine_destroy           , METH_NOARGS
                            , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  DBoDeleteMethod(EtesianEngine)
  PyTypeObjectLinkPyType(EtesianEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |           "PyEtesianEngine" Shared Library Code Part            |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(EtesianEngine,PyToolEngine)
  DBoLinkCreateMethod(EtesianEngine)


  extern void  PyEtesianEngine_postModuleInit ()
  {
    //PyObject* constant;
    //LoadObjectConstant(PyTypeEtesianEngine.tp_dict,EtesianEngine::SlowMotion,"SlowMotion");
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Etesian namespace.
 
