
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyQuery.cpp"                                 |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyTransformation.h"
#include "hurricane/isobar/PyBasicLayer.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyEntity.h"
#include "hurricane/isobar/PyQuery.h"
#include "hurricane/isobar/PyQueryMask.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


  class BaseQuery : public Query {
    public:
                    BaseQuery              ( PyQuery* );
    public:
      virtual bool  hasGoCallback          () const;
      virtual bool  hasMarkerCallback      () const;
      virtual bool  hasRubberCallback      () const;
      virtual bool  hasExtensionGoCallback () const;
      virtual bool  hasMasterCellCallback  () const;
      virtual void  goCallback             ( Go* );
      virtual void  markerCallback         ( Marker* );
      virtual void  rubberCallback         ( Rubber* );
      virtual void  extensionGoCallback    ( Go* );
      virtual void  masterCellCallback     ();
      inline  void  setGoCallback          ( PyObject* );      
      inline  void  setMarkerCallback      ( PyObject* );      
      inline  void  setRubberCallback      ( PyObject* );      
      inline  void  setExtensionGoCallback ( PyObject* );      
      inline  void  setMasterCellCallback  ( PyObject* );      
    private:
      PyQuery*  _self;
      PyObject* _goCallback;
      PyObject* _markerCallback;
      PyObject* _rubberCallback;
      PyObject* _extensionGoCallback;
      PyObject* _masterCellCallback;
  };


  BaseQuery::BaseQuery ( PyQuery* self )
    : Query()
    , _self               (self)
    , _goCallback         (NULL)
    , _markerCallback     (NULL)
    , _rubberCallback     (NULL)
    , _extensionGoCallback(NULL)
    , _masterCellCallback (NULL)
  { }

  inline void  BaseQuery::setGoCallback          ( PyObject* cb ) { _goCallback          = cb; cerr << "BaseQuery::setGoCallback()" << endl; }
  inline void  BaseQuery::setMarkerCallback      ( PyObject* cb ) { _markerCallback      = cb; }
  inline void  BaseQuery::setRubberCallback      ( PyObject* cb ) { _rubberCallback      = cb; }
  inline void  BaseQuery::setExtensionGoCallback ( PyObject* cb ) { _extensionGoCallback = cb; }
  inline void  BaseQuery::setMasterCellCallback  ( PyObject* cb ) { _masterCellCallback  = cb; }

  bool  BaseQuery::hasGoCallback          () const { cerr << "BaseQuery::hasGoCallback():" << _goCallback << endl; return _goCallback          != NULL; }
  bool  BaseQuery::hasMarkerCallback      () const { return _markerCallback      != NULL; }
  bool  BaseQuery::hasRubberCallback      () const { return _rubberCallback      != NULL; }
  bool  BaseQuery::hasExtensionGoCallback () const { return _extensionGoCallback != NULL; }
  bool  BaseQuery::hasMasterCellCallback  () const { return _masterCellCallback  != NULL; }

  void  BaseQuery::goCallback ( Go* go )
  {
    cerr << "BaseQuery::goCallback():" << go << endl;
    if (PyCallable_Check(_goCallback)) {
      PyObject_CallFunctionObjArgs( _goCallback, _self, PyEntity_NEW(go), NULL );
    }
  }

  void  BaseQuery::markerCallback ( Marker* marker )
  {
    if (PyCallable_Check(_markerCallback))
      PyObject_CallFunctionObjArgs( _markerCallback, _self, NULL );
  }

  void  BaseQuery::rubberCallback ( Rubber* rubber )
  {
    if (PyCallable_Check(_rubberCallback))
      PyObject_CallFunctionObjArgs( _rubberCallback, _self, NULL );
  }

  void  BaseQuery::extensionGoCallback ( Go* go )
  {
    if (PyCallable_Check(_extensionGoCallback))
      PyObject_CallFunctionObjArgs( _extensionGoCallback, _self, NULL );
  }

  void  BaseQuery::masterCellCallback ()
  {
    if (PyCallable_Check(_masterCellCallback))
      PyObject_CallFunctionObjArgs( _masterCellCallback, _self, NULL );
  }


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Query,query,function)


// +=================================================================+
// |               "PyQuery" Python Module Code Part                 |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


#if 0
  template< typename PySelf, typename Self, typename RType, typename... Args >
  class PySetFunctor {
    public:
      typedef PyObject* (PySet_fp)          ( PySelf*, PyObject* );
      typedef RType     (Self::* CppSet_fp) ( Args... );
    public:
      inline PySet_fp wrapper() const;
    private:
      static PyObject* _wrapper ( PySelf* self, PyObject* args )
      {
      }
    private:
      CppSet_fp  _fp;
      string     _name;
  };

  template< typename PySelf, typename Self, typename RType, typename... Args >
  inline PySetFunctor<PySelf,Self,RType,Args>::PySet_fp  PySetFunctor<PySelf,Self,RType,Args>::wrapper() const
  { return _wrapper; }
#endif


  // +-------------------------------------------------------------+
  // |                "PyQuery" Attribute Methods                  |
  // +-------------------------------------------------------------+


#if WORK_IN_PROGRESS
  static MethodDefTable<PyQuery,64>  pyObjectMethods;

  template< typename CppType, typename PySelf >
  inline CppType* getCppObject ( PySelf* self, const string& methodName )
  {
    if (self._object == NULL) {
      string message = "Attempt to call "+methodName+"on an unbound hurricane object.";
      PyErr_SetString( ProxyError, message.c_str() );
      return NULL;
    }

    CppType* cppObject = dynamic_cast<CppType*>(self._object);
    if (cppObject == NULL) {
      string message = "Invalid dynamic_cast<>() in "+methodName+" (internal error).";
      PyErr_SetString( ProxyError, message.c_str() );
      return NULL;
    }
    return cppObject;
  }
  


  template< typename CppType, typename PySelf, typename ReturnType, typename PyReturnType, int fdefindex >
  PyType* accessor ( PySelf* self ) {
    string methodName = pyObjectMethods.getFullMethodName(fdefindex);

    trace << methodName << endl;
    
    ReturnType* value = NULL;
    try {
      CppType* cppThis = getCppObject<CppType,PySelf>( self );
      if (cppThis == NULL) return NULL;

      
    }
  } 
#endif


  static PyObject* PyQuery_getMasterCell ( PyQuery *self ) {
    trace << "PyQuery.getMasterCell()" << endl;

    Cell* cell = NULL;

    HTRY
      METHOD_HEAD("PyQuery.getMasterCell()")
      cell = query->getMasterCell();
    HCATCH

    return PyCell_Link(cell);
  }


  static PyObject* PyQuery_setCell ( PyQuery* self, PyObject* args )
  {
    trace << "PyQuery.setCell()" << endl;
    METHOD_HEAD("PyQuery.setCell()")

    HTRY
      PyObject* pyCell = NULL;

      if (PyArg_ParseTuple(args,"O:Query.setCell()",&pyCell) ) {
        if (not IsPyCell(pyCell)) {
          PyErr_SetString ( ConstructorError, "Query.setCell(): Argument is not of type Cell." );
          return NULL;
        }
        query->setCell( PYCELL_O(pyCell) );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to Query.setCell()." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyQuery_setArea ( PyQuery* self, PyObject* args )
  {
    trace << "PyQuery.setArea()" << endl;
    METHOD_HEAD("PyQuery.setArea()")

    HTRY
      PyObject* pyBox = NULL;

      if (PyArg_ParseTuple(args,"O:Query.setArea()",&pyBox) ) {
        if (not IsPyBox(pyBox)) {
          PyErr_SetString ( ConstructorError, "Query.setArea(): Argument is not of type Box." );
          return NULL;
        }
        query->setArea( *PYBOX_O(pyBox) );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to Query.setArea()." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyQuery_setTransformation ( PyQuery* self, PyObject* args )
  {
    trace << "PyQuery.setTransformation()" << endl;
    METHOD_HEAD("PyQuery.setTransformation()")

    HTRY
      PyObject* pyTransformation = NULL;

      if (PyArg_ParseTuple(args,"O:Query.setTransformation()",&pyTransformation) ) {
        if (not IsPyTransformation(pyTransformation)) {
          PyErr_SetString ( ConstructorError, "Query.setTransformation(): Argument is not of type Transformation." );
          return NULL;
        }
        query->setTransformation( *PYTRANSFORMATION_O(pyTransformation) );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to Query.setTransformation()." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyQuery_setBasicLayer ( PyQuery* self, PyObject* args )
  {
    trace << "PyQuery.setBasicLayer()" << endl;
    METHOD_HEAD("PyQuery.setBasicLayer()")

    HTRY
      PyObject* pyBasicLayer = NULL;

      if (PyArg_ParseTuple(args,"O:Query.setBasicLayer()",&pyBasicLayer) ) {
        if (not IsPyBasicLayer(pyBasicLayer)) {
          PyErr_SetString ( ConstructorError, "Query.setBasicLayer(): Argument is not of type BasicLayer." );
          return NULL;
        }
        query->setBasicLayer( PYBASICLAYER_O(pyBasicLayer) );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to Query.setBasicLayer()." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyQuery_setMasterCellCallback ( PyQuery* self, PyObject* args )
  {
    trace << "PyQuery.setMasterCellCallback()" << endl;
    METHOD_HEAD("PyQuery.setMasterCellCallback()")

    HTRY
      PyObject* pyCallback = NULL;

      if (PyArg_ParseTuple(args,"O:Query.setMasterCellCallback()",&pyCallback) ) {
        if (not PyCallable_Check(pyCallback)) {
          PyErr_SetString ( ConstructorError, "Query.setMasterCellCallback(): Argument is not callable." );
          return NULL;
        }
        dynamic_cast<BaseQuery*>(query)->setMasterCellCallback( pyCallback );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to Query.setMasterCellCallback()." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyQuery_setGoCallback ( PyQuery* self, PyObject* args )
  {
    trace << "PyQuery.setGoCallback()" << endl;
    METHOD_HEAD("PyQuery.setGoCallback()")

    HTRY
      PyObject* pyCallback = NULL;

      if (PyArg_ParseTuple(args,"O:Query.setGoCallback()",&pyCallback) ) {
        if (not PyCallable_Check(pyCallback)) {
          PyErr_SetString ( ConstructorError, "Query.setGoCallback(): Argument is not callable." );
          return NULL;
        }
        dynamic_cast<BaseQuery*>(query)->setGoCallback( pyCallback );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to Query.setGoCallback()." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyQuery_setMarkerCallback ( PyQuery* self, PyObject* args )
  {
    trace << "PyQuery.setMarkerCallback()" << endl;
    METHOD_HEAD("PyQuery.setMarkerCallback()")

    HTRY
      PyObject* pyCallback = NULL;

      if (PyArg_ParseTuple(args,"O:Query.setMarkerCallback()",&pyCallback) ) {
        if (not PyCallable_Check(pyCallback)) {
          PyErr_SetString ( ConstructorError, "Query.setMarkerCallback(): Argument is not callable." );
          return NULL;
        }
        dynamic_cast<BaseQuery*>(query)->setMarkerCallback( pyCallback );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to Query.setMarkerCallback()." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyQuery_setRubberCallback ( PyQuery* self, PyObject* args )
  {
    trace << "PyQuery.setRubberCallback()" << endl;
    METHOD_HEAD("PyQuery.setRubberCallback()")

    HTRY
      PyObject* pyCallback = NULL;

      if (PyArg_ParseTuple(args,"O:Query.setRubberCallback()",&pyCallback) ) {
        if (not PyCallable_Check(pyCallback)) {
          PyErr_SetString ( ConstructorError, "Query.setRubberCallback(): Argument is not callable." );
          return NULL;
        }
        dynamic_cast<BaseQuery*>(query)->setRubberCallback( pyCallback );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to Query.setRubberCallback()." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyQuery_setExtensionGoCallback ( PyQuery* self, PyObject* args )
  {
    trace << "PyQuery.setExtensionGoCallback()" << endl;
    METHOD_HEAD("PyQuery.setExtensionGoCallback()")

    HTRY
      PyObject* pyCallback = NULL;

      if (PyArg_ParseTuple(args,"O:Query.setExtensionGoCallback()",&pyCallback) ) {
        if (not PyCallable_Check(pyCallback)) {
          PyErr_SetString ( ConstructorError, "Query.setExtensionGoCallback(): Argument is not callable." );
          return NULL;
        }
        dynamic_cast<BaseQuery*>(query)->setExtensionGoCallback( pyCallback );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to Query.setExtensionGoCallback()." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  PyObject* PyQuery_NEW ( PyObject* module, PyObject* args )
  {
    trace << "PyQuery.new()" << endl;

    Query*   query   = NULL;
    PyQuery* pyQuery = NULL;

    HTRY
    pyQuery = PyObject_NEW( PyQuery, &PyTypeQuery );
    if (pyQuery == NULL) return NULL;
    query = new BaseQuery( pyQuery );
    pyQuery->_object = query;

    cerr << "PyQuery_NEW(): " << (void*)pyQuery << endl;
    HCATCH

    return (PyObject*)pyQuery;
  }


  int  PyQuery_Init ( PyQuery* self, PyObject* args, PyObject* kwargs )
  {
    cerr << "PyQuery_Init(): " << (void*)self << endl;
    return 0;
  }


  DirectVoidMethod(Query,query,doQuery)

  // Standart destroy (Attribute).
  //DBoDestroyAttribute(PyQuery_destroy, PyQuery)


  PyMethodDef PyQuery_Methods[] =
    { { "getMasterCell"         , (PyCFunction)PyQuery_getMasterCell        , METH_NOARGS
                                , "Returns the master Cell currently under exploration." }
    , { "setCell"               , (PyCFunction)PyQuery_setCell              , METH_VARARGS
                                , "Set the cell upon which perform the query." }
    , { "setArea"               , (PyCFunction)PyQuery_setArea              , METH_VARARGS
                                , "Set the area into which perform the query." }
    , { "setTransformation"     , (PyCFunction)PyQuery_setTransformation    , METH_VARARGS
                                , "Set the initial transformation applied to the query area." }
    , { "setBasicLayer"         , (PyCFunction)PyQuery_setBasicLayer        , METH_VARARGS
                                , "Set the BasicLayer on which perform the query." }
    , { "setGoCallback"         , (PyCFunction)PyQuery_setGoCallback        , METH_VARARGS
                                , "Set the callback function for the Gos." }
    , { "setMarkerCallback"     , (PyCFunction)PyQuery_setMarkerCallback    , METH_VARARGS
                                , "Set the callback function for the Markers." }
    , { "setRubberCallback"     , (PyCFunction)PyQuery_setRubberCallback    , METH_VARARGS
                                , "Set the callback function for the Rubbers." }
    , { "setExtensionGoCallback", (PyCFunction)PyQuery_setExtensionGoCallback, METH_VARARGS
                                , "Set the callback function for the ExtensionGos." }
    , { "setMasterCellCallback" , (PyCFunction)PyQuery_setMasterCellCallback, METH_VARARGS
                                , "Set the callback function for the MasterCell." }
    , { "doQuery"               , (PyCFunction)PyQuery_doQuery              , METH_NOARGS
                                , "Perform the actual Query walk." }
    , {NULL, NULL, 0, NULL}     /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyQuery Type Methods.


  DirectDeleteMethod(PyQuery_DeAlloc,PyQuery)
  PyTypeObjectLinkPyTypeNewInit(Query)

#if WORK_IN_PROGRESS
  DirectReprMethod(PyQuery_Repr, PyQuery,   Query)
  DirectStrMethod (PyQuery_Str,  PyQuery,   Query)
  DirectCmpMethod (PyQuery_Cmp,  IsPyQuery, PyQuery)
  DirectHashMethod(PyQuery_Hash, PyQuery)


  extern void  PyQuery_LinkPyType ()
  {
    trace << "PyQuery_LinkType()" << endl;

    PyTypeQuery.tp_dealloc = (destructor) PyQuery_DeAlloc;
    PyTypeQuery.tp_compare = (cmpfunc)    PyQuery_Cmp;
    PyTypeQuery.tp_repr    = (reprfunc)   PyQuery_Repr;
    PyTypeQuery.tp_str     = (reprfunc)   PyQuery_Str;
    PyTypeQuery.tp_hash    = (hashfunc)   PyQuery_Hash;
    PyTypeQuery.tp_new     = (newfunc)    PyQuery_NEW;
    PyTypeQuery.tp_init    = (initproc)   PyQuery_Init;
  //PyTypeQuery.tp_methods = PyQuery_Methods;

    if (not pyObjectMethods.size()) {
      pyObjectMethods
        ( "getMasterCell"         , (PyCFunction)PyQuery_getMasterCell, METH_NOARGS
                                  , "Returns the master Cell currently under exploration." )
        ( "setCell"               , (PyCFunction)PyQuery_setCell, METH_VARARGS
                                  , "Set the cell upon which perform the query." )
        ( "setArea"               , (PyCFunction)PyQuery_setArea, METH_VARARGS
                                  , "Set the area into which perform the query." )
        ( "setTransformation"     , (PyCFunction)PyQuery_setTransformation, METH_VARARGS
                                  , "Set the initial transformation applied to the query area." )
        ( "setBasicLayer"         , (PyCFunction)PyQuery_setBasicLayer, METH_VARARGS
                                  , "Set the BasicLayer on which perform the query." )
        ( "setGoCallback"         , (PyCFunction)PyQuery_setGoCallback, METH_VARARGS
                                  , "Set the callback function for the Gos." )
        ( "setMarkerCallback"     , (PyCFunction)PyQuery_setMarkerCallback, METH_VARARGS
                                  , "Set the callback function for the Markers." )
        ( "setRubberCallback"     , (PyCFunction)PyQuery_setRubberCallback, METH_VARARGS
                                  , "Set the callback function for the Rubbers." )
        ( "setExtensionGoCallback", (PyCFunction)PyQuery_setExtensionGoCallback, METH_VARARGS
                                  , "Set the callback function for the ExtensionGos." )
        ( "setMasterCellCallback" , (PyCFunction)PyQuery_setMasterCellCallback, METH_VARARGS
                                  , "Set the callback function for the MasterCell." )
        ( "doQuery"               , (PyCFunction)PyQuery_doQuery, METH_NOARGS
                                  , "Perform the actual Query walk." );
    }

    PyTypeQuery.tp_methods = pyObjectMethods.getMethods();
  }
#endif


#else  // End of Python Module Code Part.


// +=================================================================+
// |              "PyQuery" Shared Library Code Part                 |
// +=================================================================+


  PyTypeRootObjectDefinitions(Query)


  extern  void  PyQuery_postModuleInit ()
  {
    PyDict_SetItemString ( PyTypeQuery.tp_dict, "Mask", (PyObject*)&PyTypeQueryMask );

    PyObject* constant;
    LoadObjectConstant(PyTypeQuery.tp_dict,Query::DoMasterCells  ,"DoMasterCells");
    LoadObjectConstant(PyTypeQuery.tp_dict,Query::DoTerminalCells,"DoTerminalCells");
    LoadObjectConstant(PyTypeQuery.tp_dict,Query::DoComponents   ,"DoComponents");
    LoadObjectConstant(PyTypeQuery.tp_dict,Query::DoMarkers      ,"DoMarkers");
    LoadObjectConstant(PyTypeQuery.tp_dict,Query::DoRubbers      ,"DoRubbers");
    LoadObjectConstant(PyTypeQuery.tp_dict,Query::DoExtensionGos ,"DoExtensionGos");
    LoadObjectConstant(PyTypeQuery.tp_dict,Query::DoAll          ,"DoAll");
  }


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace. 
