// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyNetRoutingState.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNetRoutingState.h"
#include "hurricane/isobar/PyNet.h"


namespace  Isobar {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::Error;
  using Hurricane::Warning;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(NetRoutingState,state,function)


// +=================================================================+
// |           "PyNetRoutingState" Python Module Code Part           |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  static PyObject* PyNetRoutingState_getNet ( PyNetRoutingState *self )
  {
    cdebug_log(20,0) << "PyNetRoutingState_getNet()" << endl;
    Net* net = NULL;
    HTRY
      METHOD_HEAD( "NetRoutingState.getNet()" )
      net = state->getNet();
    HCATCH
    return PyNet_Link( net );
  }


  static PyObject* PyNetRoutingState_getSymNet ( PyNetRoutingState *self )
  {
    cdebug_log(20,0) << "PyNetRoutingState_getSymNet()" << endl;
    Net* net = NULL;
    HTRY
      METHOD_HEAD( "NetRoutingState.getSymNet()" )
      net = state->getSymNet();
    HCATCH
    return PyNet_Link( net );
  }



  // Standart Accessors (Attributes).
  DirectGetBoolAttribute(PyNetRoutingState_isExcluded            ,isExcluded            ,PyNetRoutingState,NetRoutingState)
  DirectGetBoolAttribute(PyNetRoutingState_isFixed               ,isFixed               ,PyNetRoutingState,NetRoutingState)
  DirectGetBoolAttribute(PyNetRoutingState_isUnconnected         ,isUnconnected         ,PyNetRoutingState,NetRoutingState)
  DirectGetBoolAttribute(PyNetRoutingState_isManualGlobalRoute   ,isManualGlobalRoute   ,PyNetRoutingState,NetRoutingState)
  DirectGetBoolAttribute(PyNetRoutingState_isAutomaticGlobalRoute,isAutomaticGlobalRoute,PyNetRoutingState,NetRoutingState)
  DirectGetBoolAttribute(PyNetRoutingState_isMixedPreRoute       ,isMixedPreRoute       ,PyNetRoutingState,NetRoutingState)
  DirectGetBoolAttribute(PyNetRoutingState_isSymmetric           ,isSymmetric           ,PyNetRoutingState,NetRoutingState)
  DirectGetBoolAttribute(PyNetRoutingState_isSymHorizontal       ,isSymHorizontal       ,PyNetRoutingState,NetRoutingState)
  DirectGetBoolAttribute(PyNetRoutingState_isSymVertical         ,isSymVertical         ,PyNetRoutingState,NetRoutingState)
  DirectGetBoolAttribute(PyNetRoutingState_isSymMaster           ,isSymMaster           ,PyNetRoutingState,NetRoutingState)
  DirectGetBoolAttribute(PyNetRoutingState_isSymSlave            ,isSymSlave            ,PyNetRoutingState,NetRoutingState)
  DirectGetBoolAttribute(PyNetRoutingState_isSelfSym             ,isSelfSym             ,PyNetRoutingState,NetRoutingState)
  DirectGetBoolAttribute(PyNetRoutingState_isAnalog              ,isAnalog              ,PyNetRoutingState,NetRoutingState)
  DirectGetUIntAttribute(PyNetRoutingState_getFlags              ,getFlags              ,PyNetRoutingState,NetRoutingState)
  DirectGetLongAttribute(PyNetRoutingState_getSymAxis            ,getSymAxis            ,PyNetRoutingState,NetRoutingState)
  DirectGetUIntAttribute(PyNetRoutingState_getWPitch             ,getWPitch             ,PyNetRoutingState,NetRoutingState)

  // Standart Mutators (Attributes).
  DirectSetUInt32Attribute(PyNetRoutingState_setFlags  ,setFlags  ,PyNetRoutingState,NetRoutingState)
  DirectSetUInt32Attribute(PyNetRoutingState_unsetFlags,unsetFlags,PyNetRoutingState,NetRoutingState)
  DirectSetLongAttribute  (PyNetRoutingState_setSymAxis,setSymAxis,PyNetRoutingState,NetRoutingState)
  DirectSetUInt32Attribute(PyNetRoutingState_setWPitch ,setWPitch ,PyNetRoutingState,NetRoutingState)


  PyMethodDef PyNetRoutingState_Methods[] =
    { { "isExcluded"            , (PyCFunction)PyNetRoutingState_isExcluded            , METH_NOARGS  , "To be documented." }
    , { "isFixed"               , (PyCFunction)PyNetRoutingState_isFixed               , METH_NOARGS  , "To be documented." }
    , { "isUnconnected"         , (PyCFunction)PyNetRoutingState_isUnconnected         , METH_NOARGS  , "To be documented." }
    , { "isManualGlobalRoute"   , (PyCFunction)PyNetRoutingState_isManualGlobalRoute   , METH_NOARGS  , "To be documented." }
    , { "isAutomaticGlobalRoute", (PyCFunction)PyNetRoutingState_isAutomaticGlobalRoute, METH_NOARGS  , "To be documented." }
    , { "isMixedPreRoute"       , (PyCFunction)PyNetRoutingState_isMixedPreRoute       , METH_NOARGS  , "To be documented." }
    , { "isSymmetric"           , (PyCFunction)PyNetRoutingState_isSymmetric           , METH_NOARGS  , "To be documented." }
    , { "isSymHorizontal"       , (PyCFunction)PyNetRoutingState_isSymHorizontal       , METH_NOARGS  , "To be documented." }
    , { "isSymVertical"         , (PyCFunction)PyNetRoutingState_isSymVertical         , METH_NOARGS  , "To be documented." }
    , { "isSymMaster"           , (PyCFunction)PyNetRoutingState_isSymMaster           , METH_NOARGS  , "To be documented." }
    , { "isSymSlave"            , (PyCFunction)PyNetRoutingState_isSymSlave            , METH_NOARGS  , "To be documented." }
    , { "isSelfSym"             , (PyCFunction)PyNetRoutingState_isSelfSym             , METH_NOARGS  , "To be documented." }
    , { "isAnalog"              , (PyCFunction)PyNetRoutingState_isAnalog              , METH_NOARGS  , "To be documented." }
    , { "getFlags"              , (PyCFunction)PyNetRoutingState_getFlags              , METH_NOARGS  , "To be documented." }
    , { "getSymAxis"            , (PyCFunction)PyNetRoutingState_getSymAxis            , METH_NOARGS  , "To be documented." }
    , { "setFlags"              , (PyCFunction)PyNetRoutingState_setFlags              , METH_VARARGS , "To be documented." }
    , { "unsetFlags"            , (PyCFunction)PyNetRoutingState_unsetFlags            , METH_VARARGS , "To be documented." }
    , { "setSymAxis"            , (PyCFunction)PyNetRoutingState_setSymAxis            , METH_VARARGS , "To be documented." }
    , { "getNet"                , (PyCFunction)PyNetRoutingState_getNet                , METH_VARARGS , "To be documented." }
    , { "getSymNet"             , (PyCFunction)PyNetRoutingState_getSymNet             , METH_VARARGS , "To be documented." }
    , { "getWPitch"             , (PyCFunction)PyNetRoutingState_getWPitch             , METH_NOARGS  , "To be documented." }
    , { "setWPitch"             , (PyCFunction)PyNetRoutingState_setWPitch             , METH_VARARGS , "To be documented." }
    , {NULL, NULL, 0, NULL}     /* sentinel */
    };

  PythonOnlyDeleteMethod(NetRoutingState)
  PyTypeObjectLinkPyType(NetRoutingState) 


#else  // End of Python Module Code Part.


// +=================================================================+
// |           "PyNetRoutingState" Shared Library Code Part          |
// +=================================================================+

  // Link/Creation Method.
  PyTypeObjectDefinitions(NetRoutingState)


  // Link/Creation Method.
  LinkCreateMethod(NetRoutingState)


  extern  void  PyNetRoutingState_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeNetRoutingState.tp_dict,NetRoutingState::Excluded            ,"Excluded"            );
    LoadObjectConstant(PyTypeNetRoutingState.tp_dict,NetRoutingState::Fixed               ,"Fixed"               );
    LoadObjectConstant(PyTypeNetRoutingState.tp_dict,NetRoutingState::Unconnected         ,"Unconnected"         );
    LoadObjectConstant(PyTypeNetRoutingState.tp_dict,NetRoutingState::ManualGlobalRoute   ,"ManualGlobalRoute"   );
    LoadObjectConstant(PyTypeNetRoutingState.tp_dict,NetRoutingState::AutomaticGlobalRoute,"AutomaticGlobalRoute");
    LoadObjectConstant(PyTypeNetRoutingState.tp_dict,NetRoutingState::MixedPreRoute       ,"MixedPreRoute"       );
    LoadObjectConstant(PyTypeNetRoutingState.tp_dict,NetRoutingState::Horizontal          ,"Horizontal"          );
    LoadObjectConstant(PyTypeNetRoutingState.tp_dict,NetRoutingState::Vertical            ,"Vertical"            );
    LoadObjectConstant(PyTypeNetRoutingState.tp_dict,NetRoutingState::Symmetric           ,"Symmetric"           );
    LoadObjectConstant(PyTypeNetRoutingState.tp_dict,NetRoutingState::SymmetricMaster     ,"SymmetricMaster"     );
    LoadObjectConstant(PyTypeNetRoutingState.tp_dict,NetRoutingState::Analog              ,"Analog"              );
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
 
