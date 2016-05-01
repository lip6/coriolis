// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyCatalogState.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyLibrary.h"
#include "crlcore/Catalog.h"
#include "crlcore/PyCatalogState.h"


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Hurricane::Exception;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::PyLibrary;
  using Isobar::PyLibrary_Link;
  using Isobar::PyCell;
  using Isobar::PyCell_Link;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(CatalogState,state,function)


// x=================================================================x
// |           "PyCatalogState" Python Module Code Part              |
// x=================================================================x

#if defined(__PYTHON_MODULE__)

  
  // Standart Accessors (Attributes).
  DirectGetBoolAttribute(PyCatalogState_isFlattenLeaf,isFlattenLeaf,PyCatalogState,Catalog::State)
  DirectGetBoolAttribute(PyCatalogState_isFeed       ,isFeed       ,PyCatalogState,Catalog::State)
  DirectGetBoolAttribute(PyCatalogState_isGds        ,isGds        ,PyCatalogState,Catalog::State)
  DirectGetBoolAttribute(PyCatalogState_isDelete     ,isDelete     ,PyCatalogState,Catalog::State)
  DirectGetBoolAttribute(PyCatalogState_isPhysical   ,isPhysical   ,PyCatalogState,Catalog::State)
  DirectGetBoolAttribute(PyCatalogState_isLogical    ,isFlattenLeaf,PyCatalogState,Catalog::State)

  DirectSetBoolAttribute(PyCatalogState_setFlattenLeaf,setFlattenLeaf,PyCatalogState,Catalog::State)
  DirectSetBoolAttribute(PyCatalogState_setFeed       ,setFeed       ,PyCatalogState,Catalog::State)
  DirectSetBoolAttribute(PyCatalogState_setGds        ,setGds        ,PyCatalogState,Catalog::State)
  DirectSetBoolAttribute(PyCatalogState_setDelete     ,setDelete     ,PyCatalogState,Catalog::State)
  DirectSetBoolAttribute(PyCatalogState_setPhysical   ,setPhysical   ,PyCatalogState,Catalog::State)
  DirectSetBoolAttribute(PyCatalogState_setLogical    ,setFlattenLeaf,PyCatalogState,Catalog::State)


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyCatalogState_destroy,PyCatalog)


  PyMethodDef PyCatalogState_Methods[] =
    { { "isFlattenLeaf" , (PyCFunction)PyCatalogState_isFlattenLeaf, METH_NOARGS
                        , "Return true if the Cell must not be explored for hierarchy." }
    , { "isFeed"        , (PyCFunction)PyCatalogState_isFeed, METH_NOARGS
                        , "Return true if the Cell is a feed through." }
    , { "isGds"         , (PyCFunction)PyCatalogState_isGds, METH_NOARGS
                        , "Return true if the Cell is a GDSII import." }
    , { "isDelete"      , (PyCFunction)PyCatalogState_isDelete, METH_NOARGS
                        , "Return true if the Cell has the Catalog Delete attribute." }
    , { "isPhysical"    , (PyCFunction)PyCatalogState_isPhysical, METH_NOARGS
                        , "Return true if the Cell possesses a physical (layout) view." }
    , { "isLogical"     , (PyCFunction)PyCatalogState_isLogical, METH_NOARGS
                        , "Return true if the Cell possesses a logical (netlist) view." }
    , { "setFlattenLeaf", (PyCFunction)PyCatalogState_setFlattenLeaf, METH_VARARGS
                        , "Sets/reset the FlattenLeaf flag of a Cell." }
    , { "setFeed"       , (PyCFunction)PyCatalogState_setFeed, METH_VARARGS
                        , "Sets/reset the Feed flag of a Cell." }
    , { "setGds"        , (PyCFunction)PyCatalogState_setGds, METH_VARARGS
                        , "Sets/reset the Gds flag of a Cell." }
    , { "setDelete"     , (PyCFunction)PyCatalogState_setDelete, METH_VARARGS
                        , "Sets/reset the Delete flag of a Cell." }
    , { "setPhysical"   , (PyCFunction)PyCatalogState_setPhysical, METH_VARARGS
                        , "Sets/reset the Pysical flag of a Cell." }
    , { "setLogical"    , (PyCFunction)PyCatalogState_setLogical, METH_VARARGS
                        , "Sets/reset the Logical flag of a Cell." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  PythonOnlyDeleteMethod(CatalogState)

  DirectReprMethod(PyCatalogState_Repr, PyCatalogState,   Catalog::State)
  DirectStrMethod (PyCatalogState_Str,  PyCatalogState,   Catalog::State)
  DirectCmpMethod (PyCatalogState_Cmp,  IsPyCatalogState, PyCatalogState)
  DirectHashMethod(PyCatalogState_Hash, PyCatalogState)

  extern void  PyCatalogState_LinkPyType() {
    trace << "PyCatalogState_LinkType()" << endl;
    PyTypeCatalogState.tp_dealloc = (destructor) PyCatalogState_DeAlloc;
    PyTypeCatalogState.tp_compare = (cmpfunc)    PyCatalogState_Cmp;
    PyTypeCatalogState.tp_repr    = (reprfunc)   PyCatalogState_Repr;
    PyTypeCatalogState.tp_str     = (reprfunc)   PyCatalogState_Str;
    PyTypeCatalogState.tp_hash    = (hashfunc)   PyCatalogState_Hash;
    PyTypeCatalogState.tp_methods = PyCatalogState_Methods;
  }


#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyCatalogState" Shared Library Code Part            |
// x=================================================================x


  extern void  PyCatalogState_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::FlattenLeaf,"FlattenLeaf");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::Feed       ,"Feed");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::GDS        ,"GDS");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::Delete     ,"Delete");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::Logical    ,"Logical");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::Physical   ,"Physical");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::InMemory   ,"InMemory");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::Views      ,"Views");
  }



  // Link/Creation Method.
  PyTypeObjectDefinitions(CatalogState)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of CRL namespace.
 
