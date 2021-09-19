// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2020-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyDbU.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyDbU.h"


namespace  Isobar3 {

using namespace Hurricane;

extern "C" {


// Thin wrappers for overloadeds member functions.

  void  setPrecision1      ( long precision ) { DbU::setPrecision( precision ); }
  void  setGridsPerLambda1 ( double ratio ) { DbU::setGridsPerLambda( ratio ); }

  DbU::Unit  fromGridLong     ( long   u ) { return DbU::fromGrid( (double)u ); }
  DbU::Unit  fromGridDouble   ( double u ) { return DbU::fromGrid( u ); }
  DbU::Unit  fromLambdaLong   ( long   u ) { return DbU::fromLambda( (double)u ); }
  DbU::Unit  fromLambdaDouble ( double u ) { return DbU::fromLambda( u ); }

  double  toGridLong       ( long   u ) { return DbU::toGrid( (double)u ); }
  double  toGridDouble     ( double u ) { return DbU::toGrid( u ); }
  double  toLambdaLong     ( long   u ) { return DbU::toLambda( (double)u ); }
  double  toLambdaDouble   ( double u ) { return DbU::toLambda( u ); }
  double  toPhysicalLong   ( long   u, long p ) { return DbU::toPhysical( (double)u, (DbU::UnitPower)p ); }
  double  toPhysicalDouble ( double u, long p ) { return DbU::toPhysical( u, (DbU::UnitPower)p ); }

  DbU::Unit  getOnCustomGrid2 ( DbU::Unit u, DbU::Unit step )
  { return DbU::getOnCustomGrid( u, step ); }

  DbU::Unit  getOnCustomGrid3 ( DbU::Unit u, DbU::Unit step, long mode )
  { return DbU::getOnCustomGrid( u, step, (DbU::SnapMode)mode ); }

  DbU::Unit  getOnPhysicalGrid1 ( DbU::Unit u )
  { return DbU::getOnPhysicalGrid( u ); }

  DbU::Unit  getOnPhysicalGrid2 ( DbU::Unit u, long mode )
  { return DbU::getOnPhysicalGrid( u, (DbU::SnapMode)mode ); }

  DbU::Unit  getOnRealSnapGrid1 ( DbU::Unit u )
  { return DbU::getOnRealSnapGrid( u ); }

  DbU::Unit  getOnRealSnapGrid2 ( DbU::Unit u, long snap )
  { return DbU::getOnRealSnapGrid( u, (DbU::SnapMode)snap ); }

  DbU::Unit  getOnSymbolicSnapGrid1 ( DbU::Unit u )
  { return DbU::getOnSymbolicSnapGrid( u ); }

  DbU::Unit  getOnSymbolicSnapGrid2 ( DbU::Unit u, long snap )
  { return DbU::getOnSymbolicSnapGrid( u, (DbU::SnapMode)snap ); }

  std::string  getValueString1 ( DbU::Unit u )            { return DbU::getValueString( u ); }
  std::string  getValueString2 ( DbU::Unit u, long mode ) { return DbU::getValueString( u, mode ); }

  void  setStringMode1 ( long mode )             { DbU::setStringMode( mode ); }
  void  setStringMode2 ( long mode, long power ) { DbU::setStringMode( mode, (DbU::UnitPower)power ); }

  
// Python methods.

  static PyObject* PyDbU_fromDb ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.fromDb",&DbU::fromDb,args); }

  static PyObject* PyDbU_fromGrid ( PyVoidPointer*, PyObject* args )
  {
    PyObject* rvalue = callFunction("DbU.fromGrid",&fromGridDouble,args);
    if (not rvalue) rvalue = callFunction("DbU.fromGrid",&fromGridLong,args);
    return rvalue;
  }

  static PyObject* PyDbU_fromLambda ( PyVoidPointer*, PyObject* args )
  {
    PyObject* rvalue = callFunction("DbU.fromLambda",&fromLambdaDouble,args);
    if (not rvalue) rvalue = callFunction("DbU.fromLambda",&fromLambdaLong,args);
    return rvalue;
  }

  static PyObject* PyDbU_fromPhysical ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.fromPhysical",&DbU::fromPhysical,args); }

  static PyObject* PyDbU_getPrecision ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.getPrecision",&DbU::getPrecision,args); }

  static PyObject* PyDbU_getMaximalPrecision ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.getMaximalPrecision",&DbU::getMaximalPrecision,args); }

  static PyObject* PyDbU_getResolution ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.getResolution",&DbU::getResolution,args); }

  static PyObject* PyDbU_setPrecision ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.setPrecision",&setPrecision1,args); }

  static PyObject* PyDbU_getPolygonStep ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.getPolygonStep",&DbU::getPolygonStep,args); }

  static PyObject* PyDbU_setPolygonStep ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.setPolygonStep",&DbU::setPolygonStep,args); }

  static PyObject* PyDbU_getUnitPower ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.getUnitPower",&DbU::getUnitPower,args); }

  static PyObject* PyDbU_setPhysicalsPerGrid ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.setPhysicalsPerGrid",&DbU::setPhysicalsPerGrid,args); }

  static PyObject* PyDbU_getPhysicalsPerGrid ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.getPhysicalsPerGrid",&DbU::getPhysicalsPerGrid,args); }

  static PyObject* PyDbU_physicalToGrid ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.physicalToGrid",&DbU::physicalToGrid,args); }

  static PyObject* PyDbU_setGridsPerLambda ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.setGridsPerLambda",&setGridsPerLambda1,args); }

  static PyObject* PyDbU_getGridsPerLambda ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.getGridsPerLambda",&DbU::getGridsPerLambda,args); }

  static PyObject* PyDbU_getRealSnapGridStep ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.getRealSnapGridStep",&DbU::getRealSnapGridStep,args); }

  static PyObject* PyDbU_getOnRealSnapGrid ( PyVoidPointer*, PyObject* args )
  {
    PyObject* rvalue = callFunction("DbU.getOnRealSnapGrid",&getOnRealSnapGrid2,args);
    if (not rvalue) rvalue = callFunction("DbU.getOnRealSnapGrid",&getOnRealSnapGrid1,args);
    return rvalue;
  }

  static PyObject* PyDbU_setRealSnapGridStep ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.setRealSnapGridStep",&DbU::setRealSnapGridStep,args); }

  static PyObject* PyDbU_getSymbolicSnapGridStep ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.getSymbolicSnapGridStep",&DbU::getSymbolicSnapGridStep,args); }

  static PyObject* PyDbU_getOnSymbolicSnapGrid ( PyVoidPointer*, PyObject* args )
  {
    PyObject* rvalue = callFunction("DbU.getOnSymbolicSnapGrid",&getOnSymbolicSnapGrid2,args);
    if (not rvalue) rvalue = callFunction("DbU.getOnSymbolicSnapGrid",&getOnSymbolicSnapGrid1,args);
    return rvalue;
  }

  static PyObject* PyDbU_setSymbolicSnapGridStep ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.setSymbolicSnapGridStep",&DbU::setSymbolicSnapGridStep,args); }

  static PyObject* PyDbU_getOnCustomGrid ( PyVoidPointer*, PyObject* args )
  {
    PyObject* rvalue = callFunction("DbU.getOnCustomGrid",&getOnCustomGrid3,args);
    if (not rvalue) rvalue = callFunction("DbU.getOnCustomGrid",&getOnCustomGrid2,args);
    return rvalue;
  }

  static PyObject* PyDbU_getOnPhysicalGrid ( PyVoidPointer*, PyObject* args )
  {
    PyObject* rvalue = callFunction("DbU.getOnPhysicalGrid",&getOnPhysicalGrid2,args);
    if (not rvalue) rvalue = callFunction("DbU.getOnPhysicalGrid",&getOnPhysicalGrid1,args);
    return rvalue;
  }

  static PyObject* PyDbU_toDb ( PyVoidPointer*, PyObject* args )
  { return callFunction("DbU.toDb",&DbU::toDb,args); }

  static PyObject* PyDbU_toGrid ( PyVoidPointer*, PyObject* args )
  {
    PyObject* rvalue = callFunction("DbU.toGrid",&toGridDouble,args);
    if (not rvalue) rvalue = callFunction("DbU.toGrid",&toGridLong,args);
    return rvalue;
  }

  static PyObject* PyDbU_toLambda ( PyVoidPointer*, PyObject* args )
  {
    PyObject* rvalue = callFunction("DbU.toLambda",&toLambdaDouble,args);
    if (not rvalue) rvalue = callFunction("DbU.toLambda",&toLambdaLong,args);
    return rvalue;
  }

  static PyObject* PyDbU_toPhysical ( PyVoidPointer*, PyObject* args )
  {
    PyObject* rvalue = callFunction("DbU.toPhysical",&toPhysicalDouble,args);
    if (not rvalue) rvalue = callFunction("DbU.toPhysical",&toPhysicalLong,args);
    return rvalue;
  }

  static PyObject* PyDbU_getValueString ( PyVoidPointer*, PyObject* args )
  {
    PyObject* rvalue = callFunction("DbU.getValueString",&getValueString2,args);
    if (not rvalue) rvalue = callFunction("DbU.getValueString",&getValueString1,args);
    return rvalue;
  }

  static PyObject* PyDbU_setStringMode ( PyVoidPointer*, PyObject* args )
  {
    PyObject* rvalue = callFunction("DbU.setStringMode",&setStringMode2,args);
    if (not rvalue) rvalue = callFunction("DbU.setStringMode",&setStringMode1,args);
    return rvalue;
  }


  // ---------------------------------------------------------------
  // PyDbU Method table.

  
  PyMethodDef PyDbU_Methods[] =
    // Translate from user length to DbU. 
    { { "fromDb"                 , (PyCFunction)PyDbU_fromDb                 , METH_VARARGS|METH_STATIC
                                 , "Convert a raw unit into a DbU (C++ DbU::fromDb())." }
    , { "fromGrid"               , (PyCFunction)PyDbU_fromGrid               , METH_VARARGS|METH_STATIC
                                 , "Convert a length in grid step into a DbU (C++ DbU::fromGrid())." }
    , { "fromLambda"             , (PyCFunction)PyDbU_fromLambda             , METH_VARARGS|METH_STATIC
                                 , "Convert a length in lambda into a DbU (C++ DbU::fromLambda())." }
    , { "fromPhysical"           , (PyCFunction)PyDbU_fromPhysical           , METH_VARARGS|METH_STATIC
                                 , "Convert a physical length into a DbU (C++ DbU::fromPhysical())." }
    // Precision & Resolution Managment.
    , { "getPrecision"           , (PyCFunction)PyDbU_getPrecision           , METH_NOARGS|METH_STATIC
                                 , "Sets the precision, the fixed point for rounding computations." }
    , { "getMaximalPrecision"    , (PyCFunction)PyDbU_getMaximalPrecision    , METH_NOARGS|METH_STATIC
                                 , "Returns the maximum allowed precision." }
    , { "getResolution"          , (PyCFunction)PyDbU_getResolution          , METH_NOARGS|METH_STATIC
                                 , "Get the smallest manageable unit." }
    , { "setPrecision"           , (PyCFunction)PyDbU_setPrecision           , METH_VARARGS|METH_STATIC
                                 , "Sets the precision, the fixed point for rounding computations." }
    // Founder Grid Managment.
    , { "getUnitPower"           , (PyCFunction)PyDbU_getUnitPower           , METH_VARARGS|METH_STATIC
                                 , "Returns the numerical value associated to the symbolic power." }
    , { "setPhysicalsPerGrid"    , (PyCFunction)PyDbU_setPhysicalsPerGrid    , METH_VARARGS|METH_STATIC
                                 , "Set the physical length represented by exactly one unit." }
    , { "getPhysicalsPerGrid"    , (PyCFunction)PyDbU_getPhysicalsPerGrid    , METH_NOARGS|METH_STATIC
                                 , "Returns the physical length represented by exactly one unit." }
    , { "physicalToGrid"         , (PyCFunction)PyDbU_physicalToGrid         , METH_VARARGS|METH_STATIC
                                 , "Compute the number of grid units representing that physical length." }
    // Lambda Managment.
    , { "setGridsPerLambda"      , (PyCFunction)PyDbU_setGridsPerLambda      , METH_VARARGS|METH_STATIC
                                 , "Set the numbers of grid units in one lambda." }
    , { "getGridsPerLambda"      , (PyCFunction)PyDbU_getGridsPerLambda      , METH_NOARGS|METH_STATIC
                                 , "Returns the physical length represented by exactly one unit." }
    // Snap Grid Managment.
    , { "getRealSnapGridStep"    , (PyCFunction)PyDbU_getRealSnapGridStep    , METH_NOARGS|METH_STATIC
                                 , "Returns the step of the real snap grid." }
    , { "getOnRealSnapGrid"      , (PyCFunction)PyDbU_getOnRealSnapGrid      , METH_VARARGS|METH_STATIC
                                 , "Get a real snap grid position from this one." }
    , { "setRealSnapGridStep"    , (PyCFunction)PyDbU_setRealSnapGridStep    , METH_VARARGS|METH_STATIC
                                 , "Sets the real snap grid step." }
    , { "getSymbolicSnapGridStep", (PyCFunction)PyDbU_getSymbolicSnapGridStep, METH_NOARGS|METH_STATIC
                                 , "Returns the step of the symbolic snap grid." }
    , { "getOnSymbolicSnapGrid"  , (PyCFunction)PyDbU_getOnSymbolicSnapGrid  , METH_VARARGS|METH_STATIC
                                 , "Get a symbolic snap grid position from this one." }
    , { "setSymbolicSnapGridStep", (PyCFunction)PyDbU_setSymbolicSnapGridStep, METH_VARARGS|METH_STATIC
                                 , "Sets the symbolic snap grid step." }
    , { "getOnCustomGrid"        , (PyCFunction)PyDbU_getOnCustomGrid        , METH_VARARGS|METH_STATIC
                                 , "Get a custom snap grid position from this one." }
    , { "getOnPhysicalGrid"      , (PyCFunction)PyDbU_getOnPhysicalGrid      , METH_VARARGS|METH_STATIC
                                 , "Get a physical grid position from this one." }
    // Convert from DbU to various units (grid,lambda,physical).
    , { "toDb"                   , (PyCFunction)PyDbU_toDb                   , METH_VARARGS|METH_STATIC
                                 , "Convert a DbU into raw unit (C++ DbU::getDb())." }
    , { "toGrid"                 , (PyCFunction)PyDbU_toGrid                 , METH_VARARGS|METH_STATIC
                                 , "Convert a DbU into grid length (C++ DbU::getGrid())." }
    , { "toLambda"               , (PyCFunction)PyDbU_toLambda               , METH_VARARGS|METH_STATIC
                                 , "Convert a DbU into lambda length (C++ DbU::getLambda())." }
    , { "toPhysical"             , (PyCFunction)PyDbU_toPhysical             , METH_VARARGS|METH_STATIC
                                 , "Convert a DbU into a physcal length (metric system, C++ DbU::getPhysical())." }
    , { "getValueString"         , (PyCFunction)PyDbU_getValueString         , METH_VARARGS|METH_STATIC
                                 , "Convert a DbU into a human readable string." }
    , { "setStringMode"          , (PyCFunction)PyDbU_setStringMode          , METH_VARARGS|METH_STATIC
                                 , "Tells what unit to use when converting a DbU into a string." }
    // Polygon Step Managment.
    , { "setPolygonStep"         , (PyCFunction)PyDbU_setPolygonStep         , METH_VARARGS|METH_STATIC
                                 , "Set the approximation step for Polygons." }
    , { "getPolygonStep"         , (PyCFunction)PyDbU_getPolygonStep         , METH_NOARGS|METH_STATIC
                                 , "Returns the approximation step used for Polygons." }
    , {NULL, NULL, 0, NULL}      /* sentinel */
    };


}  // extern "C".

}  // Isobar3 namespace.
