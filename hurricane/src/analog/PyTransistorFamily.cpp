// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyTransistorFamily.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyTransistorFamily.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(TransistorFamily,family,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |            "PyTransistorFamily" Python Module Code Part         |
// +=================================================================+


  static PyObject* PyTransistorFamily_getType ( PyTransistorFamily* self )
  {
    TransistorFamily::Type  type = TransistorFamily::NMOS;
      
    HTRY
      METHOD_HEAD ( "TransistorFamily.getType()" )
      type = family->getType();
    HCATCH

      return Py_BuildValue( "i", (int)type );
  }


  DirectGetBoolAttribute  (PyTransistorFamily_isNMOS          ,isNMOS          ,PyTransistorFamily,TransistorFamily)
  DirectGetBoolAttribute  (PyTransistorFamily_isPMOS          ,isPMOS          ,PyTransistorFamily,TransistorFamily)
  DirectGetBoolAttribute  (PyTransistorFamily_isBulkConnected ,isBulkConnected ,PyTransistorFamily,TransistorFamily)
  DirectGetBoolAttribute  (PyTransistorFamily_isSourceFirst   ,isSourceFirst   ,PyTransistorFamily,TransistorFamily)
  DirectGetLongAttribute  (PyTransistorFamily_getBulkType     ,getBulkType     ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getWmin         ,getWmin         ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getWmax         ,getWmax         ,PyTransistorFamily,TransistorFamily)
  DirectGetIntAttribute   (PyTransistorFamily_getNfing        ,getNfing        ,PyTransistorFamily,TransistorFamily)
  DirectGetLongAttribute  (PyTransistorFamily_getW            ,getW            ,PyTransistorFamily,TransistorFamily)
  DirectGetLongAttribute  (PyTransistorFamily_getL            ,getL            ,PyTransistorFamily,TransistorFamily)
  DirectGetLongAttribute  (PyTransistorFamily_getM            ,getM            ,PyTransistorFamily,TransistorFamily)
  DirectGetLongAttribute  (PyTransistorFamily_getExternalDummy,getExternalDummy,PyTransistorFamily,TransistorFamily)

  DirectSetBoolAttribute  (PyTransistorFamily_setSourceFirst  ,setSourceFirst  ,PyTransistorFamily,TransistorFamily)
  DirectSetLongAttribute  (PyTransistorFamily_setBulkType     ,setBulkType     ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setWmin         ,setWmin         ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setWmax         ,setWmax         ,PyTransistorFamily,TransistorFamily)
  DirectSetIntAttribute   (PyTransistorFamily_setNfing        ,setNfing        ,PyTransistorFamily,TransistorFamily)
  DirectSetLongAttribute  (PyTransistorFamily_setExternalDummy,setExternalDummy,PyTransistorFamily,TransistorFamily)

  DirectGetDoubleAttribute(PyTransistorFamily_getWE           ,getWE           ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getLE           ,getLE           ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getIDS          ,getIDS          ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getVGS          ,getVGS          ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getVDS          ,getVDS          ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getVBS          ,getVBS          ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getVG           ,getVG           ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getVD           ,getVD           ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getVS           ,getVS           ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getVB           ,getVB           ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getVEG          ,getVEG          ,PyTransistorFamily,TransistorFamily)
  DirectGetDoubleAttribute(PyTransistorFamily_getVTH          ,getVTH          ,PyTransistorFamily,TransistorFamily)

  DirectSetDoubleAttribute(PyTransistorFamily_setWE           ,setWE           ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setLE           ,setLE           ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setIDS          ,setIDS          ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setVGS          ,setVGS          ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setVDS          ,setVDS          ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setVBS          ,setVBS          ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setVG           ,setVG           ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setVD           ,setVD           ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setVS           ,setVS           ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setVB           ,setVB           ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setVEG          ,setVEG          ,PyTransistorFamily,TransistorFamily)
  DirectSetDoubleAttribute(PyTransistorFamily_setVTH          ,setVTH          ,PyTransistorFamily,TransistorFamily)


  // ---------------------------------------------------------------
  // PyTransistorFamily Attribute Method table.

  PyMethodDef PyTransistorFamily_Methods[] =
    { { "getType"           , (PyCFunction)PyTransistorFamily_getType         , METH_NOARGS
                            , "Returns the type of the transistor (as a numric constant)." }
    , { "isNMOS"            , (PyCFunction)PyTransistorFamily_isNMOS          , METH_NOARGS
                            , "Returns True if it is a NMOS transistor." }
    , { "isPMOS"            , (PyCFunction)PyTransistorFamily_isPMOS          , METH_NOARGS
                            , "Returns True if it is a PMOS transistor." }
    , { "isBulkConnected"   , (PyCFunction)PyTransistorFamily_isBulkConnected , METH_NOARGS
                            , "Self explanatory." }
    , { "isSourceFirst"     , (PyCFunction)PyTransistorFamily_isSourceFirst   , METH_NOARGS
                            , "Self explanatory." }
    , { "getBulkType"       , (PyCFunction)PyTransistorFamily_getBulkType     , METH_NOARGS
                            , "Self explanatory." }
    , { "getWmin"           , (PyCFunction)PyTransistorFamily_getWmin         , METH_NOARGS
                            , "Transistor mininal W, technology defined (rule:transistorMinL)." }
    , { "getWmax"           , (PyCFunction)PyTransistorFamily_getWmax         , METH_NOARGS
                            , "Transistor maximal W, technology defined (rule:transistorMinL)." }
    , { "getNfing"          , (PyCFunction)PyTransistorFamily_getNfing        , METH_NOARGS
                            , "Self explanatory." }
    , { "getW"              , (PyCFunction)PyTransistorFamily_getW            , METH_NOARGS
                            , "Transistor effective W in the layout." }
    , { "getL"              , (PyCFunction)PyTransistorFamily_getL            , METH_NOARGS
                            , "Transistor effective L in the layout." }
    , { "getM"              , (PyCFunction)PyTransistorFamily_getM            , METH_NOARGS
                            , "Total number of fingers in the stack (counting all transistors and dummies)." }
    , { "getExternalDummy"  , (PyCFunction)PyTransistorFamily_getExternalDummy, METH_NOARGS
                            , "Self explanatory." }
    , { "setSourceFirst"    , (PyCFunction)PyTransistorFamily_setSourceFirst  , METH_VARARGS
                            , "Self explanatory." }
    , { "setWmin"           , (PyCFunction)PyTransistorFamily_setWmin         , METH_VARARGS
                            , "Self explanatory." }
    , { "setWmax"           , (PyCFunction)PyTransistorFamily_setWmax         , METH_VARARGS
                            , "Self explanatory." }
    , { "setNfing"          , (PyCFunction)PyTransistorFamily_setNfing        , METH_VARARGS
                            , "Self explanatory." }
    , { "setExternalDummy"  , (PyCFunction)PyTransistorFamily_setExternalDummy, METH_VARARGS
                            , "Self explanatory." }
    , { "setBulkType"       , (PyCFunction)PyTransistorFamily_setBulkType     , METH_VARARGS
                            , "Self explanatory." }
    , { "getWE"             , (PyCFunction)PyTransistorFamily_getWE           , METH_NOARGS
                            , "Transistor electrical W (requested)." }
    , { "getLE"             , (PyCFunction)PyTransistorFamily_getLE           , METH_NOARGS
                            , "Transistor electrical L (requested)." }
    , { "getIDS"            , (PyCFunction)PyTransistorFamily_getIDS          , METH_NOARGS
                            , "Self explanatory." }
    , { "getVGS"            , (PyCFunction)PyTransistorFamily_getVGS          , METH_NOARGS
                            , "Self explanatory." }
    , { "getVDS"            , (PyCFunction)PyTransistorFamily_getVDS          , METH_NOARGS
                            , "Self explanatory." }
    , { "getVBS"            , (PyCFunction)PyTransistorFamily_getVBS          , METH_NOARGS
                            , "Self explanatory." }
    , { "getVG"             , (PyCFunction)PyTransistorFamily_getVG           , METH_NOARGS
                            , "Self explanatory." }
    , { "getVD"             , (PyCFunction)PyTransistorFamily_getVD           , METH_NOARGS
                            , "Self explanatory." }
    , { "getVS"             , (PyCFunction)PyTransistorFamily_getVS           , METH_NOARGS
                            , "Self explanatory." }
    , { "getVB"             , (PyCFunction)PyTransistorFamily_getVB           , METH_NOARGS
                            , "Self explanatory." }
    , { "getVEG"            , (PyCFunction)PyTransistorFamily_getVEG          , METH_NOARGS
                            , "Self explanatory." }
    , { "getVTH"            , (PyCFunction)PyTransistorFamily_getVTH          , METH_NOARGS
                            , "Self explanatory." }
    , { "setWE"             , (PyCFunction)PyTransistorFamily_setWE           , METH_VARARGS
                            , "Transistor electrical W (requested)." }
    , { "setLE"             , (PyCFunction)PyTransistorFamily_setLE           , METH_VARARGS
                            , "Transistor electrical L (requested)." }
    , { "setIDS"            , (PyCFunction)PyTransistorFamily_setIDS          , METH_VARARGS
                            , "Self explanatory." }
    , { "setVGS"            , (PyCFunction)PyTransistorFamily_setVGS          , METH_VARARGS
                            , "Self explanatory." }
    , { "setVDS"            , (PyCFunction)PyTransistorFamily_setVDS          , METH_VARARGS
                            , "Self explanatory." }
    , { "setVBS"            , (PyCFunction)PyTransistorFamily_setVBS          , METH_VARARGS
                            , "Self explanatory." }
    , { "setVG"             , (PyCFunction)PyTransistorFamily_setVG           , METH_VARARGS
                            , "Self explanatory." }
    , { "setVD"             , (PyCFunction)PyTransistorFamily_setVD           , METH_VARARGS
                            , "Self explanatory." }
    , { "setVS"             , (PyCFunction)PyTransistorFamily_setVS           , METH_VARARGS
                            , "Self explanatory." }
    , { "setVB"             , (PyCFunction)PyTransistorFamily_setVB           , METH_VARARGS
                            , "Self explanatory." }
    , { "setVEG"            , (PyCFunction)PyTransistorFamily_setVEG          , METH_VARARGS
                            , "Self explanatory." }
    , { "setVTH"            , (PyCFunction)PyTransistorFamily_setVTH          , METH_VARARGS
                            , "Self explanatory." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |             "PyTransistorFamily" Object Methods             |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(TransistorFamily)
  PyTypeObjectLinkPyType(TransistorFamily)


#else  // End of Python Module Code Part.

// +=================================================================+
// |           "PyTransistorFamily" Shared Library Code Part         |
// +=================================================================+
  

  DBoLinkCreateMethod(TransistorFamily)
  PyTypeInheritedObjectDefinitions(TransistorFamily, Device)


  extern  void  PyTransistorFamily_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeTransistorFamily.tp_dict,TransistorFamily::NMOS,"NMOS");
    LoadObjectConstant(PyTypeTransistorFamily.tp_dict,TransistorFamily::PMOS,"PMOS");
  }


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
