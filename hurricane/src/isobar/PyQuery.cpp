// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
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
#include "hurricane/isobar/PyPath.h"
#include "hurricane/isobar/PyTransformation.h"
#include "hurricane/isobar/PyBasicLayer.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyInstance.h"
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


  inline void  BaseQuery::setGoCallback          ( PyObject* cb ) { _goCallback          = cb; }
  inline void  BaseQuery::setMarkerCallback      ( PyObject* cb ) { _markerCallback      = cb; }
  inline void  BaseQuery::setRubberCallback      ( PyObject* cb ) { _rubberCallback      = cb; }
  inline void  BaseQuery::setExtensionGoCallback ( PyObject* cb ) { _extensionGoCallback = cb; }
  inline void  BaseQuery::setMasterCellCallback  ( PyObject* cb ) { _masterCellCallback  = cb; }

  BaseQuery::BaseQuery ( PyQuery* self )
    : Query()
    , _self               (self)
    , _goCallback         (NULL)
    , _markerCallback     (NULL)
    , _rubberCallback     (NULL)
    , _extensionGoCallback(NULL)
    , _masterCellCallback (NULL)
  { }

  bool  BaseQuery::hasGoCallback          () const { return _goCallback          != NULL; }
  bool  BaseQuery::hasMarkerCallback      () const { return _markerCallback      != NULL; }
  bool  BaseQuery::hasRubberCallback      () const { return _rubberCallback      != NULL; }
  bool  BaseQuery::hasExtensionGoCallback () const { return _extensionGoCallback != NULL; }
  bool  BaseQuery::hasMasterCellCallback  () const { return _masterCellCallback  != NULL; }

  void  BaseQuery::goCallback ( Go* go )
  {
    if (PyCallable_Check(_goCallback)) {
      if (not PyObject_CallFunctionObjArgs( _goCallback, _self, PyEntity_NEW(go), NULL )) {
        PyErr_Print();
        PyErr_Clear();
      }
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


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(BaseQuery,query,function)


// +=================================================================+
// |               "PyQuery" Python Module Code Part                 |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  DirectSetLongAttribute(PyQuery_setThreshold    ,setThreshold    ,PyQuery,Query)
  DirectSetLongAttribute(PyQuery_setStartLevel   ,setStartLevel   ,PyQuery,Query)
  DirectSetLongAttribute(PyQuery_setStopLevel    ,setStopLevel    ,PyQuery,Query)
  DirectSetLongAttribute(PyQuery_setStopCellFlags,setStopCellFlags,PyQuery,Query)


  static PyObject* PyQuery_getMasterCell ( PyQuery *self )
  {
    cdebug_log(20,0) << "PyQuery.getMasterCell()" << endl;
    Cell* cell = NULL;
    HTRY
      METHOD_HEAD("PyQuery.getMasterCell()")
      cell = query->getMasterCell();
    HCATCH

    return PyCell_Link(cell);
  }


  static PyObject* PyQuery_getInstance ( PyQuery *self )
  {
    cdebug_log(20,0) << "PyQuery.getInstance()" << endl;
    Instance* instance = NULL;
    HTRY
      METHOD_HEAD("PyQuery.getInstance()")
      instance = query->getInstance();
    HCATCH

    return PyInstance_Link(instance);
  }


  static PyObject* PyQuery_getPath ( PyQuery *self )
  {
    cdebug_log(20,0) << "PyQuery_getPath ()" << endl;
    METHOD_HEAD( "PyQuery.getPath()" )
    PyPath* pyPath = PyObject_NEW( PyPath, &PyTypePath );
    if (pyPath == NULL) return NULL;
    HTRY
      pyPath->_object = new Path ( query->getPath() );
    HCATCH
    
    return (PyObject*)pyPath;
  }


  static PyObject* PyQuery_getTransformation ( PyQuery *self )
  {
    cdebug_log(20,0) << "PyQuery_getTransformation ()" << endl;
    METHOD_HEAD( "PyQuery.getTransformation()" )
    PyTransformation* pyTransformation = PyObject_NEW( PyTransformation, &PyTypeTransformation );
    if (pyTransformation == NULL) return NULL;
    HTRY
      pyTransformation->_object = new Transformation ( query->getTransformation() );
    HCATCH
    
    return (PyObject*)pyTransformation;
  }


  static PyObject* PyQuery_setCell ( PyQuery* self, PyObject* args )
  {
    cdebug_log(20,0) << "PyQuery.setCell()" << endl;
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


  static PyObject* PyQuery_setFilter ( PyQuery* self, PyObject* args )
  {
    cdebug_log(20,0) << "PyQuery.setFilter()" << endl;
    METHOD_HEAD("PyQuery.setFilter()")
    HTRY
      int  mask = 0;
      if (PyArg_ParseTuple(args,"i:Query.setFilter()",&mask) ) {
        query->setFilter( mask );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to Query.setFilter()." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyQuery_setArea ( PyQuery* self, PyObject* args )
  {
    cdebug_log(20,0) << "PyQuery.setArea()" << endl;
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
    cdebug_log(20,0) << "PyQuery.setTransformation()" << endl;
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
    cdebug_log(20,0) << "PyQuery.setBasicLayer()" << endl;
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
    cdebug_log(20,0) << "PyQuery.setMasterCellCallback()" << endl;
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
    cdebug_log(20,0) << "PyQuery.setGoCallback()" << endl;
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
    cdebug_log(20,0) << "PyQuery.setMarkerCallback()" << endl;
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
    cdebug_log(20,0) << "PyQuery.setRubberCallback()" << endl;
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
    cdebug_log(20,0) << "PyQuery.setExtensionGoCallback()" << endl;
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
    cdebug_log(20,0) << "PyQuery.new()" << endl;
    BaseQuery* query   = NULL;
    PyQuery*   pyQuery = NULL;
    HTRY
      pyQuery = PyObject_NEW( PyQuery, &PyTypeQuery );
      if (pyQuery == NULL) return NULL;
      query = new BaseQuery( pyQuery );
      pyQuery->_object = query;
    HCATCH

    return (PyObject*)pyQuery;
  }


  int  PyQuery_Init ( PyQuery* self, PyObject* args, PyObject* kwargs )
  {
    return 0;
  }


  DirectVoidMethod(Query,query,doQuery)

  // Standart destroy (Attribute).
  //DBoDestroyAttribute(PyQuery_destroy, PyQuery)


  PyMethodDef PyQuery_Methods[] =
    { { "getMasterCell"         , (PyCFunction)PyQuery_getMasterCell        , METH_NOARGS
                                , "Returns the master Cell currently under exploration." }
    , { "getInstance"           , (PyCFunction)PyQuery_getInstance          , METH_NOARGS
                                , "Returns the Instance currently under exploration." }
    , { "getPath"               , (PyCFunction)PyQuery_getPath              , METH_NOARGS
                                , "Returns the Path to the Instance (inclusive) under exploration." }
    , { "getTransformation"     , (PyCFunction)PyQuery_getTransformation    , METH_NOARGS
                                , "Returns the Transformation to the master Cell under exploration." }
    , { "setCell"               , (PyCFunction)PyQuery_setCell              , METH_VARARGS
                                , "Set the cell upon which perform the query." }
    , { "setFilter"             , (PyCFunction)PyQuery_setFilter            , METH_VARARGS
                                , "Set what classes of objects will be took into account." }
    , { "setArea"               , (PyCFunction)PyQuery_setArea              , METH_VARARGS
                                , "Set the area into which perform the query." }
    , { "setTransformation"     , (PyCFunction)PyQuery_setTransformation    , METH_VARARGS
                                , "Set the initial transformation applied to the query area." }
    , { "setBasicLayer"         , (PyCFunction)PyQuery_setBasicLayer        , METH_VARARGS
                                , "Set the BasicLayer on which perform the query." }
    , { "setThreshold"          , (PyCFunction)PyQuery_setThreshold         , METH_VARARGS
                                , "Quadtree leafs below this size will be pruned." }
    , { "setStartLevel"         , (PyCFunction)PyQuery_setStartLevel        , METH_VARARGS
                                , "Hierarchical depth from which to start the query." }
    , { "setStopLevel"          , (PyCFunction)PyQuery_setStopLevel         , METH_VARARGS
                                , "Hierarchical depth below this one will be pruned." }
    , { "setStopCellFlags"      , (PyCFunction)PyQuery_setStopCellFlags     , METH_VARARGS
                                , "Instances below such flagged master cells wiil be pruned." }
    , { "setGoCallback"         , (PyCFunction)PyQuery_setGoCallback        , METH_VARARGS
                                , "Set the callback function for the Gos." }
    , { "setMarkerCallback"     , (PyCFunction)PyQuery_setMarkerCallback    , METH_VARARGS
                                , "Set the callback function for the Markers." }
    , { "setRubberCallback"     , (PyCFunction)PyQuery_setRubberCallback    , METH_VARARGS
                                , "Set the callback function for the Rubbers." }
    , { "setExtensionGoCallback", (PyCFunction)PyQuery_setExtensionGoCallback, METH_VARARGS
                                , "Set the callback function for the ExtensionGos." }
    , { "setMasterCellCallback" , (PyCFunction)PyQuery_setMasterCellCallback , METH_VARARGS
                                , "Set the callback function for the MasterCell." }
    , { "doQuery"               , (PyCFunction)PyQuery_doQuery               , METH_NOARGS
                                , "Perform the actual Query walk." }
    , {NULL, NULL, 0, NULL}     /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyQuery Type Methods.


  DirectDeleteMethod(PyQuery_DeAlloc,PyQuery)
  PyTypeObjectLinkPyTypeNewInit(Query)


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
