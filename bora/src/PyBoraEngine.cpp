// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./bora/PyBoraEngine.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "bora/PyBoraEngine.h"

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(BoraEngine,bora,function)


namespace  Bora {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Exception;
  using Isobar::__cs;
  using Isobar::Converter;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::PyCell;
  using Isobar::PyCell_Link;
  using Isobar::PyAny_AsLong;
  using CRL::PyToolEngine;
  using Bora::BoraEngine;


extern "C" {

#if defined(__PYTHON_MODULE__)


// +=================================================================+
// |            "PyBoraEngine" Python Module Code Part               |
// +=================================================================+


  static PyObject* PyBoraEngine_get ( PyObject*, PyObject* args )
  {
    cdebug.log(61) << "PyBoraEngine_get()" << endl;

    BoraEngine* bora = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Bora.get", args, CELL_ARG, &arg0)) return NULL;
    bora = BoraEngine::get(PYCELL_O(arg0));
    HCATCH

    return PyBoraEngine_Link(bora);
  }


  static PyObject* PyBoraEngine_create ( PyObject*, PyObject* args )
  {
    cdebug.log(61) << "PyBoraEngine_create()" << endl;

    BoraEngine* bora = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Bora.get", args, CELL_ARG, &arg0)) return NULL;

    Cell* cell = PYCELL_O(arg0);
    bora = BoraEngine::get(cell);

    if (bora == NULL) {
      bora = BoraEngine::create(cell);
    //if (cmess1.enabled())
    //  bora->getBoraConfiguration()->print(cell);
    } else
      cerr << Warning("%s already has a Bora engine.",getString(cell).c_str()) << endl;
    HCATCH

    return PyBoraEngine_Link(bora);
  }


  static PyObject* PyBoraEngine_updatePlacement ( PyBoraEngine* self, PyObject* args )
  {
    unsigned int bsIndex = 0;

    METHOD_HEAD( "BoraEngine.updatePlacement()" )
    HTRY
      PyObject* arg0;
      PyObject* arg1;
      __cs.init ("BoraEngine.updatePlacement");

      if (not PyArg_ParseTuple(args, "O&|O&:BoraEngine.updatePlacement"
                                   , Converter, &arg0
                                   , Converter, &arg1 )) {
        return NULL;
      }

      if      (__cs.getObjectIds() == INT_ARG  ) { bora->updatePlacement( PyAny_AsLong(arg0) ); }
      else if (__cs.getObjectIds() == INTS2_ARG) { bora->updatePlacement( PyAny_AsLong(arg0), PyAny_AsLong(arg1) ); }
      else {
        PyErr_SetString( ConstructorError, "BoraEngine.updatePlacement(): Invalid/bad type parameters ." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }

  // Standart Accessors (Attributes).
  DirectVoidMethod(BoraEngine,bora,updateSlicingTree)

  // DirectVoidMethod(BoraEngine,bora,runNegociate)
  // DirectVoidMethod(BoraEngine,bora,printConfiguration)
  // DirectVoidMethod(BoraEngine,bora,saveGlobalSolution)
  // DirectVoidMethod(BoraEngine,bora,finalizeLayout)
  // DirectVoidMethod(BoraEngine,bora,dumpMeasures)
  // DirectGetBoolAttribute(PyBoraEngine_getToolSuccess,getToolSuccess,PyBoraEngine,BoraEngine)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyBoraEngine_destroy,PyBoraEngine)


  PyMethodDef PyBoraEngine_Methods[] =
    { { "get"               , (PyCFunction)PyBoraEngine_get               , METH_VARARGS|METH_STATIC
                            , "Returns the Bora engine attached to the Cell, None if there isn't." }
    , { "create"            , (PyCFunction)PyBoraEngine_create            , METH_VARARGS|METH_STATIC
                            , "Create an Bora engine on this cell." }
  //, { "place"             , (PyCFunction)PyBoraEngine_place             , METH_NOARGS
  //                        , "Run the placer (Bora)." }
    , { "updateSlicingTree" , (PyCFunction)PyBoraEngine_updateSlicingTree   , METH_NOARGS
                            , "Update/compute slicing tree possible dimensions table." }
    , { "updatePlacement"   , (PyCFunction)PyBoraEngine_updatePlacement   , METH_VARARGS
                            , "Select placement at the given index in the table." }
    , { "destroy"           , (PyCFunction)PyBoraEngine_destroy           , METH_NOARGS
                            , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  DBoDeleteMethod(BoraEngine)
  PyTypeObjectLinkPyType(BoraEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyBoraEngine" Shared Library Code Part             |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(BoraEngine,PyToolEngine)
  DBoLinkCreateMethod(BoraEngine)


  extern void  PyBoraEngine_postModuleInit ()
  {
    //PyObject* constant;
    //LoadObjectConstant(PyTypeBoraEngine.tp_dict,BoraEngine::SlowMotion,"SlowMotion");
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Bora namespace.
 
