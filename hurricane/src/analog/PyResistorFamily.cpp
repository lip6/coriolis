// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyResistorFamily.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyResistorFamily.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(ResistorFamily,family,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyResistorFamily" Python Module Code Part          |
// +=================================================================+


  static PyObject* PyResistorFamily_getType ( PyResistorFamily* self )
  {
    ResistorFamily::Type  type = ResistorFamily::LOWRES;
      
    HTRY
      METHOD_HEAD ( "ResistorFamily.getType()" )
      type = family->getType();
    HCATCH

      return Py_BuildValue( "i", (int)type );
  }


  DirectGetBoolAttribute  (PyResistorFamily_isLOWRES,isLOWRES,PyResistorFamily,ResistorFamily)
  DirectGetBoolAttribute  (PyResistorFamily_isHIRES ,isHIRES ,PyResistorFamily,ResistorFamily)
  DirectGetIntAttribute   (PyResistorFamily_getBends,getBends,PyResistorFamily,ResistorFamily)
  DirectGetLongAttribute  (PyResistorFamily_getW    ,getW    ,PyResistorFamily,ResistorFamily)
  DirectGetLongAttribute  (PyResistorFamily_getL    ,getL    ,PyResistorFamily,ResistorFamily)

  DirectSetIntAttribute   (PyResistorFamily_setBends,setBends,PyResistorFamily,ResistorFamily)

  DirectGetDoubleAttribute(PyResistorFamily_getR    ,getR    ,PyResistorFamily,ResistorFamily)
  DirectGetDoubleAttribute(PyResistorFamily_getWE   ,getWE   ,PyResistorFamily,ResistorFamily)
  DirectGetDoubleAttribute(PyResistorFamily_getLE   ,getLE   ,PyResistorFamily,ResistorFamily)

  DirectSetDoubleAttribute(PyResistorFamily_setR    ,setR    ,PyResistorFamily,ResistorFamily)
  DirectSetDoubleAttribute(PyResistorFamily_setWE   ,setWE   ,PyResistorFamily,ResistorFamily)
  DirectSetDoubleAttribute(PyResistorFamily_setLE   ,setLE   ,PyResistorFamily,ResistorFamily)


  // ---------------------------------------------------------------
  // PyResistorFamily Attribute Method table.

  PyMethodDef PyResistorFamily_Methods[] =
    { { "getType"           , (PyCFunction)PyResistorFamily_getType         , METH_NOARGS
                            , "Returns the type of the resistor (as a numric constant)." }
    , { "isLOWRES"          , (PyCFunction)PyResistorFamily_isLOWRES          , METH_NOARGS
                            , "Returns True if it is a LOWRES resistor." }
    , { "isHIRES"           , (PyCFunction)PyResistorFamily_isHIRES          , METH_NOARGS
                            , "Returns True if it is a HIRES resistor." }
    , { "getBends"          , (PyCFunction)PyResistorFamily_getBends        , METH_NOARGS
                            , "Self explanatory." }
    , { "getW"              , (PyCFunction)PyResistorFamily_getW            , METH_NOARGS
                            , "Resistor effective W in the layout." }
    , { "getL"              , (PyCFunction)PyResistorFamily_getL            , METH_NOARGS
                            , "Resistor effective L in the layout." }
    , { "getR"              , (PyCFunction)PyResistorFamily_getR            , METH_NOARGS
                            , "Resistor effective R." }
    , { "setBends"          , (PyCFunction)PyResistorFamily_setBends        , METH_VARARGS
                            , "Self explanatory." }
    , { "getWE"             , (PyCFunction)PyResistorFamily_getWE           , METH_NOARGS
                            , "Resistor electrical W (requested)." }
    , { "getLE"             , (PyCFunction)PyResistorFamily_getLE           , METH_NOARGS
                            , "Resistor electrical L (requested)." }
    , { "setWE"             , (PyCFunction)PyResistorFamily_setWE           , METH_VARARGS
                            , "Resistor electrical W (requested)." }
    , { "setLE"             , (PyCFunction)PyResistorFamily_setLE           , METH_VARARGS
                            , "Resistor electrical L (requested)." }
    , { "setR"              , (PyCFunction)PyResistorFamily_setR            , METH_VARARGS
                            , "Resistor electrical R." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |              "PyResistorFamily" Object Methods              |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(ResistorFamily)
  PyTypeObjectLinkPyType(ResistorFamily)


#else  // End of Python Module Code Part.

// +=================================================================+
// |            "PyResistorFamily" Shared Library Code Part          |
// +=================================================================+
  

  DBoLinkCreateMethod(ResistorFamily)
  PyTypeInheritedObjectDefinitions(ResistorFamily, Device)


  extern  void  PyResistorFamily_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeResistorFamily.tp_dict,ResistorFamily::LOWRES,"LOWRES");
    LoadObjectConstant(PyTypeResistorFamily.tp_dict,ResistorFamily::HIRES ,"HIRES" );
  }


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
