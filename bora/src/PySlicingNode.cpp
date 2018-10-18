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
// |  C++ Module  :  "./bora/PySlicingNode.cpp"                      |
// +-----------------------------------------------------------------+


#include "bora/PySlicingNode.h"
#include "bora/PyHSlicingNode.h"
#include "bora/PyVSlicingNode.h"
#include "bora/PyDSlicingNode.h"
#include "bora/PyRHSlicingNode.h"
#include "bora/PyRVSlicingNode.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyNet.h"
#include "crlcore/PyRoutingGauge.h"


namespace Bora {

  using namespace Hurricane;
  using namespace Isobar;
  using CRL::PyRoutingGauge;
  using CRL::PyRoutingGauge_Link;
  using CRL::PyTypeRoutingGauge;


extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(SlicingNode,node,function)


// +=================================================================+
// |            "PySlicingNode" Python Module Code Part              |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |             "PySlicingNode" Attribute Methods               |
  // +-------------------------------------------------------------+

  // Standart destroy (Attribute).
  //DBoDestroyAttribute(PySlicingNode_destroy ,PySlicingNode)


  static PyObject* PySlicingNode_getCell ( PySlicingNode *self )
  {
    cdebug_log(20,0) << "PySlicingNode_getCell ()" << endl;
    Cell* cell = NULL;
    HTRY
      METHOD_HEAD( "SlicingNode.getCell()" )
      cell = node->getCell();
    HCATCH
    return PyCell_Link( cell );
  }


  static PyObject* PySlicingNode_getRoot ( PySlicingNode *self )
  {
    cdebug_log(20,0) << "PySlicingNode_getRoot ()" << endl;
    SlicingNode* node = NULL;
    HTRY
      METHOD_HEAD( "SlicingNode.getRoot()" )
      node = node->getRoot();
    HCATCH
    return PySlicingNode_NEW( node );
  }


  static PyObject* PySlicingNode_getParent ( PySlicingNode *self )
  {
    cdebug_log(20,0) << "PySlicingNode_getParent ()" << endl;
    SlicingNode* node = NULL;
    HTRY
      METHOD_HEAD( "SlicingNode.getParent()" )
      node = node->getParent();
    HCATCH
    return PySlicingNode_NEW( node );
  }


  static PyObject* PySlicingNode_setCell ( PySlicingNode* self, PyObject* args )
  {
    PyObject* pyCell = NULL;

    HTRY
      METHOD_HEAD( "SlicingNode.setCell()" )
      if (not PyArg_ParseTuple(args,"O:SlicingNode.setCell", &pyCell)) {
        PyErr_SetString( ConstructorError, "SlicingNode.setCell(): Invalid/bad type parameters ." );
        return NULL;
      }
      if (not IsPyCell(pyCell)) {
        PyErr_SetString( ConstructorError, "SlicingNode.setCell(): Argument *must* be of Cell type." );
        return NULL;
      }
      node->setCell( PYCELL_O(pyCell) );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PySlicingNode_setParent ( PySlicingNode* self, PyObject* args )
  {
    PyObject* pyNode = NULL;

    HTRY
      METHOD_HEAD( "SlicingNode.setParent()" )
      if (not PyArg_ParseTuple(args,"O:SlicingNode.setParent", &pyNode)) {
        PyErr_SetString( ConstructorError, "SlicingNode.setParent(): Invalid/bad type parameters ." );
        return NULL;
      }
      if (not IsPySlicingNode(pyNode)) {
        PyErr_SetString( ConstructorError, "SlicingNode.setParent(): Argument *must* be of SlicingNode type." );
        return NULL;
      }
      node->setParent( PYSLICINGNODE_O(pyNode) );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PySlicingNode_setRoutingGauge ( PySlicingNode* self, PyObject* args )
  {
    PyObject* pyRG = NULL;

    HTRY
      METHOD_HEAD( "SlicingNode.setRoutingGauge()" )
      if (not PyArg_ParseTuple(args,"O:SlicingNode.setRoutingGauge", &pyRG)) {
        PyErr_SetString( ConstructorError, "SlicingNode.setRoutingGauge(): Invalid/bad type parameters ." );
        return NULL;
      }
      if (not IsPyRoutingGauge(pyRG)) {
        PyErr_SetString( ConstructorError, "SlicingNode.setRoutingGauge(): Argument *must* be of RoutingGauge type." );
        return NULL;
      }
      node->setRoutingGauge( PYROUTINGGAUGE_O(pyRG) );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PySlicingNode_push_back ( PySlicingNode* self, PyObject* args )
  {
    PyObject* pyNode  = NULL;
    PyObject* pyReset = NULL;

    HTRY
      METHOD_HEAD( "SlicingNode.push_back()" )
      if (not PyArg_ParseTuple(args,"O|O:SlicingNode.push_back", &pyNode, &pyReset)) {
        PyErr_SetString( ConstructorError, "SlicingNode.push_back(): Invalid/bad type parameters ." );
        return NULL;
      }
      PyObject* pyRepr = PyObject_Repr( pyNode );
      string    repr   = PyString_AsString( pyRepr );

      SlicingNode* child = SlicingNodeCast( pyNode );
      if (not child) {
        PyErr_SetString( ConstructorError, "SlicingNode.push_back(): Argument *must* be of SlicingNode type." );
        return NULL;
      }
      bool reset = false;
      if (pyReset) reset = (PyObject_IsTrue(pyReset) == 1);
      node->push_back( child, reset );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PySlicingNode_addSymmetryNet ( PySlicingNode* self, PyObject* args )
  {
    unsigned int type = 0;
    PyObject* pyNet1 = NULL;
    PyObject* pyNet2 = NULL;

    HTRY
      METHOD_HEAD( "SlicingNode.addSymmetryNet()" )
      if (not PyArg_ParseTuple(args,"IO|O:SlicingNode.addSymmetryNet", &type, &pyNet1, &pyNet2)) {
        PyErr_SetString( ConstructorError, "SlicingNode.addSymmetryNet(): Invalid/bad type parameters." );
        return NULL;
      }
    
      if (not IsPyNet(pyNet1)) {
        PyErr_SetString( ConstructorError, "SlicingNode.setParent(): Argument *must* be of Net type." );
        return NULL;
      }
      if (pyNet2){
        if (not IsPyNet(pyNet2)) {
          PyErr_SetString( ConstructorError, "SlicingNode.setParent(): Argument *must* be of Net type." );
          return NULL;
        }
      //cerr << "type  : " << type << endl;
      // cerr << "pyNet1: " << pyNet1 << endl;
      //cerr << "pyNet2: " << pyNet2 << endl;
        node->addSymmetryNet( type, PYNET_O(pyNet1), PYNET_O(pyNet2) );
      } else {
      //cerr << "type  : " << type << endl;
      //cerr << "pyNet1: " << pyNet1 << endl;
      //cerr << "pyNet2: " << pyNet2 << endl;
        node->addSymmetryNet( type, PYNET_O(pyNet1) );
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PySlicingNode_addSymmetry ( PySlicingNode* self, PyObject* args )
  {
    int       childIndex = 0;
    int       copyIndex  = 0;
    PyObject* pyReset    = NULL;

    HTRY
      METHOD_HEAD( "SlicingNode.addSymmetry()" )
      if (not PyArg_ParseTuple(args,"ii|O:SlicingNode.addSymmetry", &childIndex, &copyIndex, &pyReset)) {
        PyErr_SetString( ConstructorError, "SlicingNode.addSymmetry(): Invalid/bad type parameters ." );
        return NULL;
      }
      bool reset = true;
      if (pyReset) reset = (PyObject_IsTrue(pyReset) == 1);
      node->addSymmetry( childIndex, copyIndex, reset );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PySlicingNode_setGlobalSize ( PySlicingNode* self, PyObject* args )
  {
    unsigned int bsIndex  = 0;
    PyObject*    pyWidth  = 0;
    PyObject*    pyHeight = 0;

    HTRY
      METHOD_HEAD( "SlicingNode.setGlobalSize()" )
      if (PyArg_ParseTuple(args,"OO:SlicingNode.setGlobalSize", &pyWidth, &pyHeight)) {
        node->setGlobalSize( PyAny_AsLong(pyWidth), PyAny_AsLong(pyHeight) );
      } else if (PyArg_ParseTuple(args,"I:SlicingNode.setGlobalSize", &bsIndex)) {
        node->setGlobalSize( bsIndex );
      } else {
        PyErr_SetString( ConstructorError, "SlicingNode.setGlobalSize(): Invalid/bad type parameters ." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }

  // Standart Accessors (Attributes).
  DirectGetBoolAttribute(PySlicingNode_isPreset          ,isPreset          ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isSet             ,isSet             ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isPlaced          ,isPlaced          ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isAlignLeft       ,isAlignLeft       ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isAlignRight      ,isAlignRight      ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isAlignCenter     ,isAlignCenter     ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isAlignTop        ,isAlignTop        ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isAlignBottom     ,isAlignBottom     ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isHorizontal      ,isHorizontal      ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isVertical        ,isVertical        ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isDevice          ,isDevice          ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isRouting         ,isRouting         ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isEmpty           ,isEmpty           ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isOverCell        ,isOverCell        ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isRoutingCreated  ,isRoutingCreated  ,PySlicingNode,SlicingNode)
  DirectGetBoolAttribute(PySlicingNode_isRoutingEstimated,isRoutingEstimated,PySlicingNode,SlicingNode)

  DirectGetUIntAttribute(  PySlicingNode_getId             ,getId             ,PySlicingNode,SlicingNode)
  DirectGetUIntAttribute(  PySlicingNode_getPreset         ,getPreset         ,PySlicingNode,SlicingNode)
  DirectGetUIntAttribute(  PySlicingNode_getSet            ,getSet            ,PySlicingNode,SlicingNode)
  DirectGetUIntAttribute(  PySlicingNode_getPlaced         ,getPlaced         ,PySlicingNode,SlicingNode)
  DirectGetUIntAttribute(  PySlicingNode_getAlignment      ,getAlignment      ,PySlicingNode,SlicingNode)
  DirectGetUIntAttribute(  PySlicingNode_getType           ,getType           ,PySlicingNode,SlicingNode)
  DirectGetIntAttribute(   PySlicingNode_getCpt            ,getCpt            ,PySlicingNode,SlicingNode)
  DirectGetDoubleAttribute(PySlicingNode_getRatio          ,getRatio          ,PySlicingNode,SlicingNode)
  DirectGetDoubleAttribute(PySlicingNode_getArea           ,getArea           ,PySlicingNode,SlicingNode)
  DirectGetLongAttribute(  PySlicingNode_getHeight         ,getHeight         ,PySlicingNode,SlicingNode)
  DirectGetLongAttribute(  PySlicingNode_getWidth          ,getWidth          ,PySlicingNode,SlicingNode)
  DirectGetLongAttribute(  PySlicingNode_getX              ,getX              ,PySlicingNode,SlicingNode)
  DirectGetLongAttribute(  PySlicingNode_getY              ,getY              ,PySlicingNode,SlicingNode)
  DirectGetLongAttribute(  PySlicingNode_getXCenter        ,getXCenter        ,PySlicingNode,SlicingNode)
  DirectGetLongAttribute(  PySlicingNode_getYCenter        ,getYCenter        ,PySlicingNode,SlicingNode)
  DirectGetLongAttribute(  PySlicingNode_getToleranceRatioH,getToleranceRatioH,PySlicingNode,SlicingNode)
  DirectGetLongAttribute(  PySlicingNode_getToleranceRatioW,getToleranceRatioW,PySlicingNode,SlicingNode)
  DirectGetLongAttribute(  PySlicingNode_getToleranceBandH ,getToleranceBandH ,PySlicingNode,SlicingNode)
  DirectGetLongAttribute(  PySlicingNode_getToleranceBandW ,getToleranceBandW ,PySlicingNode,SlicingNode)

  DirectSetIntAttribute (PySlicingNode_setPreset          ,setPreset          ,PySlicingNode,SlicingNode)
  DirectSetIntAttribute (PySlicingNode_setSet             ,setSet             ,PySlicingNode,SlicingNode)
  DirectSetIntAttribute (PySlicingNode_setPlaced          ,setPlaced          ,PySlicingNode,SlicingNode)
  DirectSetIntAttribute (PySlicingNode_setAlignment       ,setAlignment       ,PySlicingNode,SlicingNode)
  DirectSetIntAttribute (PySlicingNode_setType            ,setType            ,PySlicingNode,SlicingNode)
  DirectSetIntAttribute (PySlicingNode_setOverCell        ,setOverCell        ,PySlicingNode,SlicingNode)
  DirectSetIntAttribute (PySlicingNode_setRoutingCreated  ,setRoutingCreated  ,PySlicingNode,SlicingNode)
  DirectSetIntAttribute (PySlicingNode_setRoutingEstimated,setRoutingEstimated,PySlicingNode,SlicingNode)
  DirectSetLongAttribute(PySlicingNode_setX               ,setX               ,PySlicingNode,SlicingNode)
  DirectSetLongAttribute(PySlicingNode_setY               ,setY               ,PySlicingNode,SlicingNode)
  DirectSetLongAttribute(PySlicingNode_setToleranceRatioH ,setToleranceRatioH ,PySlicingNode,SlicingNode)
  DirectSetLongAttribute(PySlicingNode_setToleranceRatioW ,setToleranceRatioW ,PySlicingNode,SlicingNode)
  DirectSetLongAttribute(PySlicingNode_setToleranceBandH  ,setToleranceBandH  ,PySlicingNode,SlicingNode)
  DirectSetLongAttribute(PySlicingNode_setToleranceBandW  ,setToleranceBandW  ,PySlicingNode,SlicingNode)

  DirectVoidMethod(SlicingNode,node,updateGlobalSize)
  DirectVoidMethod(SlicingNode,node,clearNodeSets)
  DirectVoidMethod(SlicingNode,node,clearParentsNodeSets)
  DirectVoidMethod(SlicingNode,node,removeParent)
  DirectVoidMethod(SlicingNode,node,resetSlicingTree)
  DirectVoidMethod(SlicingNode,node,print)
  DirectVoidMethod(SlicingNode,node,updateNetConstraints)

  accessorVectorFromVoid(getChildren,PySlicingNode,SlicingNode,SlicingNode)


  // ---------------------------------------------------------------
  // PySlicingNode Attribute Method table.

  PyMethodDef PySlicingNode_Methods[] =
    { { "isPreset"             , (PyCFunction)PySlicingNode_isPreset            , METH_NOARGS , "To be documented." }
    , { "isSet"                , (PyCFunction)PySlicingNode_isSet               , METH_NOARGS , "To be documented." }
    , { "isPlaced"             , (PyCFunction)PySlicingNode_isPlaced            , METH_NOARGS , "To be documented." }
    , { "isAlignLeft"          , (PyCFunction)PySlicingNode_isAlignLeft         , METH_NOARGS , "To be documented." }
    , { "isAlignRight"         , (PyCFunction)PySlicingNode_isAlignRight        , METH_NOARGS , "To be documented." }
    , { "isAlignCenter"        , (PyCFunction)PySlicingNode_isAlignCenter       , METH_NOARGS , "To be documented." }
    , { "isAlignTop"           , (PyCFunction)PySlicingNode_isAlignTop          , METH_NOARGS , "To be documented." }
    , { "isAlignBottom"        , (PyCFunction)PySlicingNode_isAlignBottom       , METH_NOARGS , "To be documented." }
    , { "isHorizontal"         , (PyCFunction)PySlicingNode_isHorizontal        , METH_NOARGS , "To be documented." }
    , { "isVertical"           , (PyCFunction)PySlicingNode_isVertical          , METH_NOARGS , "To be documented." }
    , { "isDevice"             , (PyCFunction)PySlicingNode_isDevice            , METH_NOARGS , "To be documented." }
    , { "isRouting"            , (PyCFunction)PySlicingNode_isRouting           , METH_NOARGS , "To be documented." }
    , { "isEmpty"              , (PyCFunction)PySlicingNode_isEmpty             , METH_NOARGS , "To be documented." }
    , { "isPreset"             , (PyCFunction)PySlicingNode_isPreset            , METH_NOARGS , "To be documented." }
    , { "isOverCell"           , (PyCFunction)PySlicingNode_isOverCell          , METH_NOARGS , "To be documented." }
    , { "isRoutingCreated"     , (PyCFunction)PySlicingNode_isRoutingCreated    , METH_NOARGS , "To be documented." }
    , { "isRoutingEstimated"   , (PyCFunction)PySlicingNode_isRoutingEstimated  , METH_NOARGS , "To be documented." }
    , { "getId"                , (PyCFunction)PySlicingNode_getId               , METH_NOARGS , "To be documented." }
    , { "getPreset"            , (PyCFunction)PySlicingNode_getPreset           , METH_NOARGS , "To be documented." }
    , { "getSet"               , (PyCFunction)PySlicingNode_getSet              , METH_NOARGS , "To be documented." }
    , { "getPlaced"            , (PyCFunction)PySlicingNode_getPlaced           , METH_NOARGS , "To be documented." }
    , { "getAlignment"         , (PyCFunction)PySlicingNode_getAlignment        , METH_NOARGS , "To be documented." }
    , { "getType"              , (PyCFunction)PySlicingNode_getType             , METH_NOARGS , "To be documented." }
    , { "getCpt"               , (PyCFunction)PySlicingNode_getCpt              , METH_NOARGS , "To be documented." }
    , { "getRatio"             , (PyCFunction)PySlicingNode_getRatio            , METH_NOARGS , "To be documented." }
    , { "getArea"              , (PyCFunction)PySlicingNode_getArea             , METH_NOARGS , "To be documented." }
    , { "getHeight"            , (PyCFunction)PySlicingNode_getHeight           , METH_NOARGS , "To be documented." }
    , { "getWidth"             , (PyCFunction)PySlicingNode_getWidth            , METH_NOARGS , "To be documented." }
    , { "getX"                 , (PyCFunction)PySlicingNode_getX                , METH_NOARGS , "To be documented." }
    , { "getY"                 , (PyCFunction)PySlicingNode_getY                , METH_NOARGS , "To be documented." }
    , { "getXCenter"           , (PyCFunction)PySlicingNode_getXCenter          , METH_NOARGS , "To be documented." }
    , { "getYCenter"           , (PyCFunction)PySlicingNode_getYCenter          , METH_NOARGS , "To be documented." }
    , { "getCell"              , (PyCFunction)PySlicingNode_getCell             , METH_NOARGS , "To be documented." }
    , { "getRoot"              , (PyCFunction)PySlicingNode_getRoot             , METH_NOARGS , "To be documented." }
    , { "getToleranceRatioH"   , (PyCFunction)PySlicingNode_getToleranceRatioH  , METH_NOARGS , "To be documented." }
    , { "getToleranceRatioW"   , (PyCFunction)PySlicingNode_getToleranceRatioW  , METH_NOARGS , "To be documented." }
    , { "getToleranceBandH"    , (PyCFunction)PySlicingNode_getToleranceBandW   , METH_NOARGS , "To be documented." }
    , { "getToleranceBandW"    , (PyCFunction)PySlicingNode_getToleranceBandH   , METH_NOARGS , "To be documented." }
    , { "getParent"            , (PyCFunction)PySlicingNode_getParent           , METH_NOARGS , "To be documented." }
    , { "setPreset"            , (PyCFunction)PySlicingNode_setPreset           , METH_VARARGS, "To be documented." }
    , { "setSet"               , (PyCFunction)PySlicingNode_setSet              , METH_VARARGS, "To be documented." }
    , { "setPlaced"            , (PyCFunction)PySlicingNode_setPlaced           , METH_VARARGS, "To be documented." }
    , { "setAlignment"         , (PyCFunction)PySlicingNode_setAlignment        , METH_VARARGS, "To be documented." }
    , { "setType"              , (PyCFunction)PySlicingNode_setType             , METH_VARARGS, "To be documented." }
    , { "setOverCell"          , (PyCFunction)PySlicingNode_setOverCell         , METH_VARARGS, "To be documented." }
    , { "setRoutingCreated"    , (PyCFunction)PySlicingNode_setRoutingCreated   , METH_VARARGS, "To be documented." }
    , { "setRoutingEstimated"  , (PyCFunction)PySlicingNode_setRoutingEstimated , METH_VARARGS, "To be documented." }
    , { "setX"                 , (PyCFunction)PySlicingNode_setX                , METH_VARARGS, "To be documented." }
    , { "setY"                 , (PyCFunction)PySlicingNode_setY                , METH_VARARGS, "To be documented." }
    , { "updateGlobalSize"     , (PyCFunction)PySlicingNode_updateGlobalSize    , METH_NOARGS , "To be documented." }
    , { "setGlobalSize"        , (PyCFunction)PySlicingNode_setGlobalSize       , METH_VARARGS, "To be documented." }
    , { "clearNodeSets"        , (PyCFunction)PySlicingNode_clearNodeSets       , METH_NOARGS , "To be documented." }
    , { "clearParentsNodeSets" , (PyCFunction)PySlicingNode_clearParentsNodeSets, METH_NOARGS , "To be documented." }
    , { "removeParent"         , (PyCFunction)PySlicingNode_removeParent        , METH_NOARGS , "To be documented." }
    , { "resetSlicingTree"     , (PyCFunction)PySlicingNode_resetSlicingTree    , METH_NOARGS , "To be documented." }
    , { "setCell"              , (PyCFunction)PySlicingNode_setCell             , METH_VARARGS, "To be documented." }
    , { "setParent"            , (PyCFunction)PySlicingNode_setParent           , METH_VARARGS, "To be documented." }
    , { "setRoutingGauge"      , (PyCFunction)PySlicingNode_setRoutingGauge     , METH_VARARGS, "To be documented." }
    , { "setToleranceRatioH"   , (PyCFunction)PySlicingNode_setToleranceRatioH  , METH_VARARGS, "To be documented." }
    , { "setToleranceRatioW"   , (PyCFunction)PySlicingNode_setToleranceRatioW  , METH_VARARGS, "To be documented." }
    , { "setToleranceBandH"    , (PyCFunction)PySlicingNode_setToleranceBandW   , METH_VARARGS, "To be documented." }
    , { "setToleranceBandW"    , (PyCFunction)PySlicingNode_setToleranceBandH   , METH_VARARGS, "To be documented." }
    , { "addSymmetry"          , (PyCFunction)PySlicingNode_addSymmetry         , METH_VARARGS, "To be documented." }
    , { "push_back"            , (PyCFunction)PySlicingNode_push_back           , METH_VARARGS, "To be documented." }
    , { "getChildren"          , (PyCFunction)PySlicingNode_getChildren         , METH_NOARGS , "To be documented." }
    , { "cprint"               , (PyCFunction)PySlicingNode_print               , METH_NOARGS , "To be documented." }
    , { "updateNetConstraints" , (PyCFunction)PySlicingNode_updateNetConstraints, METH_NOARGS , "To be documented." }
    , { "addSymmetryNet"       , (PyCFunction)PySlicingNode_addSymmetryNet      , METH_VARARGS, "To be documented." }
  //, { "destroy"              , (PyCFunction)PySlicingNode_destroy             , METH_NOARGS
  //                           , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |              "PySlicingNode" Object Methods                 |
  // +-------------------------------------------------------------+

  PythonOnlyDeleteMethod(SlicingNode)
  PyTypeObjectLinkPyType(SlicingNode) 


  static PyObject* PySlicingNodeIteratorNext ( PySlicingNodeVectorIterator* pyIterator )
  {
    HTRY
      if (pyIterator->_iterator != pyIterator->_pyContainer->_container->end()) {
        SlicingNode* object = *(pyIterator->_iterator);
        ++(pyIterator->_iterator);
        return PySlicingNode_NEW(object);
      }
    HCATCH
    return NULL;
  }


  VectorMethods     (SlicingNode)


#else  // End of Python Module Code Part.


// +=================================================================+
// |           "PySlicingNode" Shared Library Code Part              |
// +=================================================================+


  // ---------------------------------------------------------------
  // Allocator Method  :  "PySlicingNode_NEW ()"
  //
  // No PySlicingNode should ever be created, it's not a terminal object
  // of the class hierarchy. Instead create the real underlying PyObject.

  PyObject* PySlicingNode_NEW ( SlicingNode* node ) {
    if (not node) {
      PyErr_SetString ( HurricaneError, "PySlicingNode_NEW(): NULL node argument." );
      return NULL;
    }

    // HVSlicingNode* hvnode = dynamic_cast<HVSlicingNode*>(node);
    // if (hvnode) return PyHVSlicingNode_NEW( hvnode );
    
    HSlicingNode* hnode = dynamic_cast<HSlicingNode*>(node);
    if (hnode) return PyHSlicingNode_NEW( hnode );
    
    VSlicingNode* vnode = dynamic_cast<VSlicingNode*>(node);
    if (vnode) return PyVSlicingNode_NEW( vnode );
    
    DSlicingNode* dnode = dynamic_cast<DSlicingNode*>(node);
    if (dnode) return PyDSlicingNode_NEW( dnode );
    
    RHSlicingNode* rhnode = dynamic_cast<RHSlicingNode*>(node);
    if (rhnode) return PyRHSlicingNode_NEW( rhnode );
    
    RVSlicingNode* rvnode = dynamic_cast<RVSlicingNode*>(node);
    if (rvnode) return PyRVSlicingNode_NEW( rvnode );

    Py_RETURN_NONE;
  }


  PyTypeRootObjectDefinitions(SlicingNode)

  PyTypeVectorObjectDefinitions(SlicingNodeVector)
  PyTypeVectorObjectDefinitions(SlicingNodeVectorIterator)


  extern void  PySlicingNode_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeSlicingNode.tp_dict,Preset           ,"Preset"           );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,Set              ,"Set"              );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,Placed           ,"Placed"           );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,UnknownAlignment ,"UnknownAlignment" );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,AlignLeft        ,"AlignLeft"        );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,AlignRight       ,"AlignRight"       );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,AlignCenter      ,"AlignCenter"      );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,AlignTop         ,"AlignTop"         );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,AlignBottom      ,"AlignBottom"      );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,UnknownType      ,"UnknownType"      );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,HorizontalSNode  ,"HorizontalSNode"  );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,VerticalSNode    ,"VerticalSNode"    );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,DeviceSNode      ,"DeviceSNode"      );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,RoutingSNode     ,"RoutingSNode"     );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,None             ,"None"             );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,MX               ,"MX"               );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,MY               ,"MY"               );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,FixedRoutingSNode,"FixedRoutingSNode");
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,OverCell         ,"OverCell"         );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,RoutingEstimated ,"RoutingEstimated" );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,RoutingCreated   ,"RoutingCreated"   );
  //LoadObjectConstant(PyTypeSlicingNode.tp_dict,HasDijkstra      ,"HasDijkstra"      );

    LoadObjectConstant(PyTypeSlicingNode.tp_dict,presetMask          ,"presetMask"         );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,setMask             ,"setMask"            );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,placedMask          ,"placedMask"         );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,alignmentMask       ,"alignmentMask"      );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,slicingTypeMask     ,"slicingTypeMask"    );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,overCellMask        ,"overCellMask"       );
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,routingEstimatedMask,"routingEstimateMask");
    LoadObjectConstant(PyTypeSlicingNode.tp_dict,routingCreatedMask  ,"routingCreateMask"  );
  }


#endif  // Shared Library Code Part.

}  // extern "C".


// +=================================================================+
// |           "PySlicingNode" Shared Library Code Part              |
// +=================================================================+


# if !defined(__PYTHON_MODULE__)

SlicingNode* SlicingNodeCast ( PyObject* derivedObject ) {
//if (IsPyHVSlicingNode(derivedObject)) return PYHVSLICINGNODE_O(derivedObject);
  if (IsPyHSlicingNode(derivedObject))  return PYHSLICINGNODE_O(derivedObject);
  if (IsPyVSlicingNode(derivedObject))  return PYVSLICINGNODE_O(derivedObject);
  if (IsPyDSlicingNode(derivedObject))  return PYDSLICINGNODE_O(derivedObject);
  if (IsPyRHSlicingNode(derivedObject)) return PYRHSLICINGNODE_O(derivedObject);
  if (IsPyRVSlicingNode(derivedObject)) return PYRVSLICINGNODE_O(derivedObject);

  return NULL;
}

#endif

}  // Bora namespace.
