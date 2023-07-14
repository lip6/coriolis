// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
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
  using Hurricane::Exception;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::getPyHash;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::PyLibrary;
  using Isobar::PyLibrary_Link;
  using Isobar::PyCell;
  using Isobar::PyCell_Link;
  using Isobar::PyTypeCell;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(Catalog::State,state,function)


// x=================================================================x
// |           "PyCatalogState" Python Module Code Part              |
// x=================================================================x

#if defined(__PYTHON_MODULE__)

  
  // Standart Accessors (Attributes).
  DirectGetBoolAttribute(PyCatalogState_isTerminalNetlist,isTerminalNetlist,PyCatalogState,Catalog::State)
  DirectGetBoolAttribute(PyCatalogState_isFeed           ,isFeed           ,PyCatalogState,Catalog::State)
  DirectGetBoolAttribute(PyCatalogState_isGds            ,isGds            ,PyCatalogState,Catalog::State)
  DirectGetBoolAttribute(PyCatalogState_isDelete         ,isDelete         ,PyCatalogState,Catalog::State)
  DirectGetBoolAttribute(PyCatalogState_isPhysical       ,isPhysical       ,PyCatalogState,Catalog::State)
  DirectGetBoolAttribute(PyCatalogState_isLogical        ,isLogical        ,PyCatalogState,Catalog::State)
  DirectGetBoolAttribute(PyCatalogState_isInMemory       ,isInMemory       ,PyCatalogState,Catalog::State)

  DirectSetBoolAttribute(PyCatalogState_setTerminalNetlist,setTerminalNetlist,PyCatalogState,Catalog::State)
  DirectSetBoolAttribute(PyCatalogState_setFeed           ,setFeed           ,PyCatalogState,Catalog::State)
  DirectSetBoolAttribute(PyCatalogState_setGds            ,setGds            ,PyCatalogState,Catalog::State)
  DirectSetBoolAttribute(PyCatalogState_setDelete         ,setDelete         ,PyCatalogState,Catalog::State)
  DirectSetBoolAttribute(PyCatalogState_setPhysical       ,setPhysical       ,PyCatalogState,Catalog::State)
  DirectSetBoolAttribute(PyCatalogState_setLogical        ,setLogical        ,PyCatalogState,Catalog::State)
  DirectSetBoolAttribute(PyCatalogState_setInMemory       ,setInMemory       ,PyCatalogState,Catalog::State)


  static PyObject* PyCatalogState_setCell ( PyCatalogState* self, PyObject* args )
  {
    cdebug_log(20,0) << "PyCatalogState.setCell()" << endl;
    METHOD_HEAD("PyCatalogState.setCell()")
    HTRY
      PyObject* pyCell = NULL;

      if (PyArg_ParseTuple(args,"O:State.setCell()",&pyCell) ) {
        if (not IsPyCell(pyCell)) {
          PyErr_SetString ( ConstructorError, "State.setCell(): Argument is not of type Cell." );
          return NULL;
        }
        state->setCell( PYCELL_O(pyCell) );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to State.setCell()." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyCatalogState_destroy,PyCatalog)


  PyMethodDef PyCatalogState_Methods[] =
    { { "isTerminalNetlist" , (PyCFunction)PyCatalogState_isTerminalNetlist, METH_NOARGS
                            , "Return true if the Cell must not be explored for hierarchy." }
    , { "isFeed"            , (PyCFunction)PyCatalogState_isFeed, METH_NOARGS
                            , "Return true if the Cell is a feed through." }
    , { "isGds"             , (PyCFunction)PyCatalogState_isGds, METH_NOARGS
                            , "Return true if the Cell is a GDSII import." }
    , { "isDelete"          , (PyCFunction)PyCatalogState_isDelete, METH_NOARGS
                            , "Return true if the Cell has the Catalog Delete attribute." }
    , { "isPhysical"        , (PyCFunction)PyCatalogState_isPhysical, METH_NOARGS
                            , "Return true if the Cell possesses a physical (layout) view." }
    , { "isLogical"         , (PyCFunction)PyCatalogState_isLogical, METH_NOARGS
                            , "Return true if the Cell possesses a logical (netlist) view." }
    , { "isInMemory"        , (PyCFunction)PyCatalogState_isInMemory, METH_NOARGS
                            , "Return true if the Cell is already loaded in memory." }
    , { "setCell"           , (PyCFunction)PyCatalogState_setCell, METH_VARARGS
                            , "Set the cell associated with this state." }
    , { "setTerminalNetlist", (PyCFunction)PyCatalogState_setTerminalNetlist, METH_VARARGS
                            , "Sets/reset the TerminalNetlist flag of a Cell." }
    , { "setFeed"           , (PyCFunction)PyCatalogState_setFeed, METH_VARARGS
                            , "Sets/reset the Feed flag of a Cell." }
    , { "setGds"            , (PyCFunction)PyCatalogState_setGds, METH_VARARGS
                            , "Sets/reset the Gds flag of a Cell." }
    , { "setDelete"         , (PyCFunction)PyCatalogState_setDelete, METH_VARARGS
                            , "Sets/reset the Delete flag of a Cell." }
    , { "setPhysical"       , (PyCFunction)PyCatalogState_setPhysical, METH_VARARGS
                            , "Sets/reset the Pysical flag of a Cell." }
    , { "setLogical"        , (PyCFunction)PyCatalogState_setLogical, METH_VARARGS
                            , "Sets/reset the Logical flag of a Cell." }
    , { "setInMemory"       , (PyCFunction)PyCatalogState_setInMemory, METH_VARARGS
                            , "Sets/reset the in memory flag of a Cell." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  PythonOnlyDeleteMethod(CatalogState)

  DirectReprMethod    (PyCatalogState_Repr, PyCatalogState,   Catalog::State)
  DirectStrMethod     (PyCatalogState_Str,  PyCatalogState,   Catalog::State)
  DirectCmpByPtrMethod(PyCatalogState_Cmp,  IsPyCatalogState, PyCatalogState)
  DirectHashMethod    (PyCatalogState_Hash, CatalogState)

  extern void  PyCatalogState_LinkPyType() {
    cdebug_log(30,0) << "PyCatalogState_LinkType()" << endl;
    PyTypeCatalogState.tp_dealloc     = (destructor) PyCatalogState_DeAlloc;
    PyTypeCatalogState.tp_richcompare = (richcmpfunc)PyCatalogState_Cmp;
    PyTypeCatalogState.tp_repr        = (reprfunc)   PyCatalogState_Repr;
    PyTypeCatalogState.tp_str         = (reprfunc)   PyCatalogState_Str;
    PyTypeCatalogState.tp_hash        = (hashfunc)   PyCatalogState_Hash;
    PyTypeCatalogState.tp_methods     = PyCatalogState_Methods;
  }


#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyCatalogState" Shared Library Code Part            |
// x=================================================================x


  extern void  PyCatalogState_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::TerminalNetlist     ,"TerminalNetlist");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::Feed                ,"Feed");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::GDS                 ,"GDS");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::Delete              ,"Delete");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::Logical             ,"Logical");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::Physical            ,"Physical");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::InMemory            ,"InMemory");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::Foreign             ,"Foreign");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::VstUseConcat        ,"VstUseConcat");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::VstNoLowerCase      ,"VstNoLowerCase");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::VstUniquifyUpperCase,"VstUniquifyUpperCase");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::VstNoLinkage        ,"VstNoLinkage");
    LoadObjectConstant(PyTypeCatalogState.tp_dict,Catalog::State::Views               ,"Views");
  }



  // Link/Creation Method.
  PyTypeObjectDefinitions(CatalogState)

    
  PyObject* PyCatalogState_Link ( Catalog::State* object )
  {
    if (not object) Py_RETURN_NONE;
    PyCatalogState* pyObject = NULL;
    HTRY
      pyObject = PyObject_NEW( PyCatalogState, &PyTypeCatalogState );
      if (not pyObject) return NULL;

      pyObject->_object = object;
      cdebug_log(20,0) << "PyCatalogState_Link(" << (void*)pyObject << ") "
                       << (void*)object << ":" << object << endl;
    HCATCH
    return (PyObject*)pyObject;
  }


#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of CRL namespace.
 
