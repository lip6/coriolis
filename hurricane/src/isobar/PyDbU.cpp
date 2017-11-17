
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
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyDbU.cpp"                                   |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyDbU.h"


namespace Isobar {

using namespace Hurricane;

extern "C" {


// +=================================================================+
// |                 "PyDbU" Python Module Code Part                 |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // static DbU::SnapMode PyInt_AsSnapMode ( PyObject* object ) {
  //   switch ( PyAny_AsLong(object) ) {
  //     case DbU::Inferior : return ( DbU::Inferior );
  //     case DbU::Superior : return ( DbU::Superior );
  //     case DbU::Nearest  : return ( DbU::Nearest );
  //   }

  //   return ( DbU::Superior );
  // }

  static DbU::UnitPower PyInt_AsUnitPower ( PyObject* object ) {
    switch ( PyAny_AsLong(object) ) {
        case DbU::Pico  : return ( DbU::Pico );
        case DbU::Nano  : return ( DbU::Nano );
        case DbU::Micro : return ( DbU::Micro );
        case DbU::Milli : return ( DbU::Milli );
        case DbU::Unity : return ( DbU::Unity );
        case DbU::Kilo  : return ( DbU::Kilo );
    }

    return ( DbU::Micro );
  }


  // +-------------------------------------------------------------+
  // |                  Global Constants Loading                   |
  // +-------------------------------------------------------------+


  // +-------------------------------------------------------------+
  // |                  "PyDbU" General Methods                    |
  // +-------------------------------------------------------------+


  extern PyObject* PyDbU_fromDb ( PyObject* , PyObject* args )
  {
    PyObject*  arg0;
    DbU::Unit  result = 0;

    HTRY
    __cs.init ( "DbU.fromDb" );
    if ( ! PyArg_ParseTuple(args,"|O&:DbU.fromDb",Converter,&arg0) )
      return ( NULL );
    
    if ( __cs.getObjectIds() == INT_ARG   ) { result = DbU::fromDb ( PyAny_AsLong ( arg0 ) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters or bad type for DbU.fromDb converter." );
      return ( NULL );
    }
    HCATCH
    
    return PyDbU_FromLong(result);
  }


  extern PyObject* PyDbU_fromGrid ( PyObject* , PyObject* args )
  {
    PyObject*  arg0;
    DbU::Unit  result = 0;

    HTRY
    __cs.init ( "DbU.grid" );
    if ( ! PyArg_ParseTuple(args,"|O&:DbU.fromGrid",Converter,&arg0) )
      return ( NULL );
    
    if      ( __cs.getObjectIds() == FLOAT_ARG )  { result = DbU::fromGrid ( PyFloat_AsDouble ( arg0 ) ); }
    else if ( __cs.getObjectIds() == INT_ARG   )  { result = DbU::fromGrid (     PyAny_AsLong ( arg0 ) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters or bad type for DbU.fromGrid converter." );
      return ( NULL );
    }
    HCATCH
    
    return PyDbU_FromLong(result);
  }


  extern PyObject* PyDbU_fromLambda ( PyObject* , PyObject* args )
  {
    PyObject*  arg0;
    DbU::Unit  result = 0;

    HTRY
    __cs.init ( "DbU.fromLambda" );
    if ( ! PyArg_ParseTuple(args,"|O&:DbU.fromLambda",Converter,&arg0) )
      return ( NULL );
    
    if      ( __cs.getObjectIds() == FLOAT_ARG ) { result = DbU::fromLambda ( PyFloat_AsDouble ( arg0 ) ); }
    else if ( __cs.getObjectIds() == INT_ARG   ) { result = DbU::fromLambda ( PyLong_AsLong    ( arg0 ) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters or bad type for DbU.fromLambda converter." );
      return ( NULL );
    }
    HCATCH
    
    return PyDbU_FromLong(result);
  }


  extern PyObject* PyDbU_fromPhysical ( PyObject* , PyObject* args )
  {
    double     value   = 0.0;
    int        power   = DbU::Micro;
    DbU::Unit  result  = 0;

    HTRY
    if (not PyArg_ParseTuple(args,"di:DbU.fromPhysical", &value, &power) ) {
      PyErr_SetString ( ConstructorError, "DbU.fromPhysical(): Invalid/bad type parameters ." );
      return NULL;
    }
    result = DbU::fromPhysical(value,(DbU::UnitPower)power);
    HCATCH

    return PyDbU_FromLong(result);
  }


  extern PyObject* PyDbU_getPrecision ( PyObject* )
  { return Py_BuildValue("I",DbU::getPrecision()); }


  extern PyObject* PyDbU_getMaximalPrecision ( PyObject* )
  { return Py_BuildValue("I",DbU::getMaximalPrecision()); }


  extern PyObject* PyDbU_getResolution ( PyObject* )
  { return Py_BuildValue("d",DbU::getResolution()); }


  extern PyObject* PyDbU_setPrecision ( PyObject* , PyObject* args )
  {
    unsigned int  precision = 0;

    HTRY
    if (not PyArg_ParseTuple(args,"I:DbU.setPrecision", &precision) ) {
      PyErr_SetString ( ConstructorError, "DbU.setPrecision(): Invalid/bad type parameters ." );
      return NULL;
    }
    DbU::setPrecision(precision);
    HCATCH

    Py_RETURN_NONE;
  }


  extern PyObject* PyDbU_getUnitPower ( PyObject* , PyObject* args )
  {
    double        value = 0.0;
    unsigned int  power = 0;

    HTRY
    if (not PyArg_ParseTuple(args,"i:DbU.getUnitPower", &power) ) {
      PyErr_SetString ( ConstructorError, "DbU.getUnitPower(): Invalid/bad type parameters ." );
      return NULL;
    }
    value = DbU::getUnitPower((DbU::UnitPower)power);
    HCATCH

    return Py_BuildValue("d",value);
  }


  extern PyObject* PyDbU_setPhysicalsPerGrid ( PyObject* , PyObject* args )
  {
    double        physicalsPerGrid = 0.0;
    unsigned int  power            = 0;

    HTRY
    if (not PyArg_ParseTuple(args,"di:DbU.setPhysicalsPerGrid", &physicalsPerGrid, &power) ) {
      PyErr_SetString ( ConstructorError, "DbU.setPhysicalsPerGrid(): Invalid/bad type parameters ." );
      return NULL;
    }
    DbU::setPhysicalsPerGrid(physicalsPerGrid,(DbU::UnitPower)power);
    HCATCH

    Py_RETURN_NONE;
  }


  extern PyObject* PyDbU_getPhysicalsPerGrid ( PyObject* )
  { return Py_BuildValue("d",DbU::getPhysicalsPerGrid()); }


  extern PyObject* PyDbU_physicalToGrid ( PyObject* , PyObject* args )
  {
    double  value   = 0.0;
    int     power   = DbU::Micro;
    double  result  = 0;

    HTRY
    if (not PyArg_ParseTuple(args,"di:DbU.physicalToGrid", &value, &power) ) {
      PyErr_SetString ( ConstructorError, "DbU.physicalToGrid(): Invalid/bad type parameters ." );
      return NULL;
    }
    result = DbU::physicalToGrid(value,(DbU::UnitPower)power);
    HCATCH

    return Py_BuildValue("d",result);
  }


  extern PyObject* PyDbU_setGridsPerLambda ( PyObject* , PyObject* args )
  {
    double  gridsPerLambda = 0.0;

    HTRY
    if (not PyArg_ParseTuple(args,"d:DbU.setGridsPerLambda", &gridsPerLambda) ) {
      PyErr_SetString ( ConstructorError, "DbU.setGridsPerLambda(): Invalid/bad type parameters ." );
      return NULL;
    }
    DbU::setGridsPerLambda(gridsPerLambda);
    HCATCH

    Py_RETURN_NONE;
  }


  extern PyObject* PyDbU_getGridsPerLambda ( PyObject* )
  { return Py_BuildValue("d",DbU::getGridsPerLambda()); }


  extern PyObject* PyDbU_getRealSnapGridStep ( PyObject* )
  { return PyDbU_FromLong(DbU::getRealSnapGridStep()); }


  extern PyObject* PyDbU_getOnRealSnapGrid ( PyObject* , PyObject* args )
  {
    PyObject*  value  = NULL;
    int        snap   = DbU::Nearest;
    DbU::Unit  result = 0;

    HTRY
    if (not PyArg_ParseTuple(args,"O|i:DbU.getOnRealSnapGrid", &value, &snap) ) {
      PyErr_SetString ( ConstructorError, "DbU.getOnRealSnapGrid(): Invalid/bad type parameters ." );
      return NULL;
    }
    result = DbU::getOnRealSnapGrid(PyAny_AsLong(value),(DbU::SnapMode)snap);
    HCATCH

    return PyDbU_FromLong(result);
  }


  extern PyObject* PyDbU_setRealSnapGridStep ( PyObject* , PyObject* args )
  {
    PyObject* step = NULL;

    HTRY
    if (not PyArg_ParseTuple(args,"O:DbU.setRealSnapGridStep", &step) ) {
      PyErr_SetString ( ConstructorError, "DbU.setRealSnapGridStep(): Invalid/bad type parameters ." );
      return NULL;
    }
    DbU::setRealSnapGridStep(PyAny_AsLong(step));
    HCATCH

    Py_RETURN_NONE;
  }


  extern PyObject* PyDbU_getSymbolicSnapGridStep ( PyObject* )
  { return PyDbU_FromLong(DbU::getSymbolicSnapGridStep()); }


  extern PyObject* PyDbU_getOnSymbolicSnapGrid ( PyObject* , PyObject* args )
  {
    PyObject*  pyValue = NULL;
    int        snap    = DbU::Nearest;
    DbU::Unit  result  = 0;

    HTRY
    if (not PyArg_ParseTuple(args,"O|i:DbU.getOnSymbolicSnapGrid", &pyValue, &snap) ) {
      PyErr_SetString ( ConstructorError, "DbU.getOnSymbolicSnapGrid(): Invalid/bad type parameters ." );
      return NULL;
    }
    result = DbU::getOnSymbolicSnapGrid(PyAny_AsLong(pyValue),(DbU::SnapMode)snap);
    HCATCH

    return PyDbU_FromLong(result);
  }


  extern PyObject* PyDbU_setSymbolicSnapGridStep ( PyObject* , PyObject* args )
  {
    PyObject* step = NULL;

    HTRY
    if (not PyArg_ParseTuple(args,"O:DbU.setSymbolicSnapGridStep", &step) ) {
      PyErr_SetString ( ConstructorError, "DbU.setSymbolicSnapGridStep(): Invalid/bad type parameters ." );
      return NULL;
    }
    DbU::setSymbolicSnapGridStep(PyAny_AsLong(step));
    HCATCH

    Py_RETURN_NONE;
  }


  extern PyObject* PyDbU_getOnCustomGrid ( PyObject* , PyObject* args )
  {
    PyObject* pyValue = NULL;
    PyObject* pyStep  = NULL;
    DbU::Unit result  = 0;
    int       snap    = DbU::Nearest;

    HTRY
    if (not PyArg_ParseTuple(args,"OO|i:DbU.getOnCustomGrid", &pyValue, &pyStep, &snap) ) {
      PyErr_SetString ( ConstructorError, "DbU.getOnCustomGrid(): Invalid/bad type parameters ." );
      return NULL;
    }
    result = DbU::getOnCustomGrid(PyAny_AsLong(pyValue),PyAny_AsLong(pyStep),(DbU::SnapMode)snap);
    HCATCH

    return PyDbU_FromLong(result);
  }


  extern PyObject* PyDbU_getOnPhysicalGrid ( PyObject* , PyObject* args )
  {
    PyObject*  pyValue = NULL;
    int        snap    = DbU::Superior;
    DbU::Unit  result  = 0;

    HTRY
    if (not PyArg_ParseTuple(args,"O|i:DbU.getOnPhysicalGrid", &pyValue, &snap) ) {
      PyErr_SetString ( ConstructorError, "DbU.getOnPhysicalGrid(): Invalid/bad type parameters ." );
      return NULL;
    }
    result = DbU::getOnPhysicalGrid(PyAny_AsLong(pyValue),(DbU::SnapMode)snap);
    HCATCH

    return PyDbU_FromLong(result);
  }


  extern PyObject* PyDbU_toDb ( PyObject* , PyObject* args )
  {
    PyObject* arg0;
    if ( not ParseOneArg( "DbU.toDb", args,INT_ARG, &arg0 ) ) return NULL;
    return PyDbU_FromLong(DbU::toDb((DbU::Unit)PyAny_AsLong(arg0)));
  }


  extern PyObject* PyDbU_toGrid ( PyObject* , PyObject* args )
  {
    PyObject* arg0;
    if ( not ParseOneArg( "DbU.toGrid", args,INT_ARG, &arg0 ) ) return NULL;
    return Py_BuildValue("d",DbU::toGrid((DbU::Unit)PyAny_AsLong(arg0)));
  }


  extern PyObject* PyDbU_toLambda ( PyObject* , PyObject* args )
  {
    PyObject* arg0;
    if ( not ParseOneArg( "DbU.toLambda", args,INT_ARG, &arg0 ) ) return NULL;
    return Py_BuildValue("d",DbU::toLambda((DbU::Unit)PyAny_AsLong(arg0)));
  }


  extern PyObject* PyDbU_toPhysical ( PyObject* , PyObject* args )
  {
    PyObject* arg0;
    PyObject* arg1;
    if ( not ParseTwoArg( "DbU.toPhysical", args,INTS2_ARG, &arg0, &arg1 ) ) return NULL;
    return Py_BuildValue("d",DbU::toPhysical((DbU::Unit)PyAny_AsLong(arg0), PyInt_AsUnitPower(arg1)));
  }


  extern PyObject* PyDbU_getValueString ( PyObject* , PyObject* args )
  {
    PyObject* value   = NULL;
    int       mode    = DbU::SmartTruncate;
    string    result  = "";

    HTRY
    if (not PyArg_ParseTuple(args,"O|i:DbU.getValueString", &value, &mode) ) {
      PyErr_SetString ( ConstructorError, "DbU.getValueString(): Invalid/bad type parameters ." );
      return NULL;
    }
    result = DbU::getValueString(PyAny_AsLong(value),mode);
    HCATCH

    return Py_BuildValue("s",result.c_str());
  }


  extern PyObject* PyDbU_setStringMode ( PyObject* , PyObject* args )
  {
    unsigned int  mode  = 0;
    unsigned int  power = DbU::Nano;

    HTRY
    if (not PyArg_ParseTuple(args,"I|i:DbU.setStringMode", &mode, &power) ) {
      PyErr_SetString ( ConstructorError, "DbU.setStringMode(): Invalid/bad type parameters ." );
      return NULL;
    }
    DbU::setStringMode(mode,(DbU::UnitPower)power);
    HCATCH

    Py_RETURN_NONE;
  }


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
    , {NULL, NULL, 0, NULL}      /* sentinel */
    };


  extern void  PyDbU_LinkPyType()
  {
    cdebug_log(20,0) << "PyDbU_LinkType()" << endl;

    PyTypeDbU.tp_methods   =  PyDbU_Methods;
  }


#else  // End of Python Module Code Part.


// +=================================================================+
// |               "PyDbU" Shared Library Code Part                  |
// +=================================================================+


  PyTypeObject  PyTypeDbU =
    { PyObject_HEAD_INIT(NULL)
      0                                       /* ob_size.          */
    , "Hurricane.DbU"                         /* tp_name.          */
    , sizeof(PyDbU)                           /* tp_basicsize.     */
    , 0                                       /* tp_itemsize.      */
    /* methods. */                            
    , 0                                       /* tp_dealloc.       */
    , 0                                       /* tp_print.         */
    , 0                                       /* tp_getattr.       */
    , 0                                       /* tp_setattr.       */
    , 0                                       /* tp_compare.       */
    , 0                                       /* tp_repr.          */
    , 0                                       /* tp_as_number.     */
    , 0                                       /* tp_as_sequence.   */
    , 0                                       /* tp_as_mapping.    */
    , 0                                       /* tp_hash.          */
    , 0                                       /* tp_call.          */
    , 0                                       /* tp_str            */
    , 0                                       /* tp_getattro.      */
    , 0                                       /* tp_setattro.      */
    , 0                                       /* tp_as_buffer.     */
    , Py_TPFLAGS_DEFAULT                      /* tp_flags          */
    , "Hurricane.DbU (do not uses as object)" /* tp_doc.           */
  };


  extern void  DbULoadConstants ( PyObject* dictionnary ) {
    PyObject* constant;

    LoadObjectConstant ( dictionnary, DbU::Inferior     , "SnapModeInferior" )
    LoadObjectConstant ( dictionnary, DbU::Superior     , "SnapModeSuperior" )
    LoadObjectConstant ( dictionnary, DbU::Nearest      , "SnapModeNearest" )
    LoadObjectConstant ( dictionnary, DbU::Pico         , "UnitPowerPico" )
    LoadObjectConstant ( dictionnary, DbU::Nano         , "UnitPowerNano" )
    LoadObjectConstant ( dictionnary, DbU::Micro        , "UnitPowerMicro" )
    LoadObjectConstant ( dictionnary, DbU::Milli        , "UnitPowerMilli" )
    LoadObjectConstant ( dictionnary, DbU::Unity        , "UnitPowerUnity" )
    LoadObjectConstant ( dictionnary, DbU::Kilo         , "UnitPowerKilo" )
    LoadObjectConstant ( dictionnary, DbU::Db           , "StringModeDb" )
    LoadObjectConstant ( dictionnary, DbU::Grid         , "StringModeGrid" )
    LoadObjectConstant ( dictionnary, DbU::Symbolic     , "StringModeSymbolic" )
    LoadObjectConstant ( dictionnary, DbU::Physical     , "StringModePhysical" )
    LoadObjectConstant ( dictionnary, DbU::SmartTruncate, "StringModeSmartTruncate" )
  }


  extern  void  PyDbU_postModuleInit ()
  {
    DbULoadConstants(PyTypeDbU.tp_dict);
  }


#endif  // End of Python Module Code Part.
      

}  // End of extern "C".


}  // End of Isobar namespace.
 
