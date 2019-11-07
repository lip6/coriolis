// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyAnalog.cpp"                                |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/analog/PyDevice.h"
#include "hurricane/analog/PyTransistorFamily.h"
#include "hurricane/analog/PyTransistor.h"
#include "hurricane/analog/PyTransistorPair.h"
#include "hurricane/analog/PyCommonDrain.h"
#include "hurricane/analog/PyCommonGatePair.h"
#include "hurricane/analog/PyCommonSourcePair.h"
#include "hurricane/analog/PyCrossCoupledPair.h"
#include "hurricane/analog/PyDifferentialPair.h"
#include "hurricane/analog/PyLevelShifter.h"
#include "hurricane/analog/PySimpleCurrentMirror.h"
#include "hurricane/analog/PyCascode.h"

#include "hurricane/analog/PyCapacitorFamily.h"
#include "hurricane/analog/PyMultiCapacitor.h"

#include "hurricane/analog/PyMatrix.h"
#include "hurricane/analog/PyParameter.h"
#include "hurricane/analog/PyChoiceParameter.h"
#include "hurricane/analog/PyFormFactorParameter.h"
#include "hurricane/analog/PyMCheckBoxParameter.h"
#include "hurricane/analog/PySpinBoxParameter.h"
#include "hurricane/analog/PyStepParameter.h"
#include "hurricane/analog/PyCapacitiesParameter.h"
#include "hurricane/analog/PyMatrixParameter.h"
#include "hurricane/analog/PyLayoutGenerator.h"


namespace  Isobar {


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyHurricane" Shared Library Code Part              |
// +=================================================================+

#else

extern "C" {


  static PyMethodDef PyAnalog_Methods[] =
    { { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // ---------------------------------------------------------------
  // Module Initialization  :  "initAnalog ()"

  DL_EXPORT(void) initAnalog () {
  //trace_on();
    cdebug.log(49) << "initAnalog()" << endl;

    PyDevice_LinkPyType();
    PyTransistorFamily_LinkPyType();
    PyTransistor_LinkPyType();
    PyTransistorPair_LinkPyType();
    PyCommonDrain_LinkPyType();
    PyCommonGatePair_LinkPyType();
    PyCommonSourcePair_LinkPyType();
    PyCrossCoupledPair_LinkPyType();
    PyDifferentialPair_LinkPyType();
    PyLevelShifter_LinkPyType();
    PySimpleCurrentMirror_LinkPyType();
    PyCascode_LinkPyType();

    PyMatrix_LinkPyType();
    PyParameter_LinkPyType();
    PyChoiceParameter_LinkPyType();
    PyFormFactorParameter_LinkPyType();
    PyMCheckBoxParameter_LinkPyType();
    PySpinBoxParameter_LinkPyType();
    PyStepParameter_LinkPyType();
    PyCapacitiesParameter_LinkPyType();
    PyMatrixParameter_LinkPyType();
    PyCapacitorFamily_LinkPyType();
    PyLayoutGenerator_LinkPyType();
    PyMultiCapacitor_LinkPyType();

    PYTYPE_READY( Matrix )
    PYTYPE_READY( Parameter )
    PYTYPE_READY( LayoutGenerator )

    PYTYPE_READY_SUB( Device               , Cell )
    PYTYPE_READY_SUB( TransistorFamily     , Device )
    PYTYPE_READY_SUB( Transistor           , TransistorFamily )
    PYTYPE_READY_SUB( TransistorPair       , TransistorFamily )
    PYTYPE_READY_SUB( CommonDrain          , TransistorPair )
    PYTYPE_READY_SUB( CommonGatePair       , TransistorPair )
    PYTYPE_READY_SUB( CommonSourcePair     , TransistorPair )
    PYTYPE_READY_SUB( CrossCoupledPair     , TransistorPair )
    PYTYPE_READY_SUB( DifferentialPair     , TransistorPair )
    PYTYPE_READY_SUB( LevelShifter         , TransistorPair )
    PYTYPE_READY_SUB( SimpleCurrentMirror  , TransistorPair )
    PYTYPE_READY_SUB( Cascode              , TransistorPair )
                                           
    PYTYPE_READY_SUB( CapacitorFamily      , Device )
    PYTYPE_READY_SUB( MultiCapacitor       , CapacitorFamily )

    PYTYPE_READY_SUB( ChoiceParameter      , Parameter )
    PYTYPE_READY_SUB( FormFactorParameter  , Parameter )
    PYTYPE_READY_SUB( MCheckBoxParameter   , Parameter )
    PYTYPE_READY_SUB( SpinBoxParameter     , Parameter )
    PYTYPE_READY_SUB( StepParameter        , Parameter )
    PYTYPE_READY_SUB( CapacitiesParameter  , Parameter )
    PYTYPE_READY_SUB( MatrixParameter      , Parameter )

    // Identifier string can take up to 10 characters !
    __cs.addType( "device"   , &PyTypeDevice                  , "<Device>"                  , false, "cell" );
    __cs.addType( "tfamily"  , &PyTypeTransistorFamily        , "<TransistorFamily>"        , false, "device" );
    __cs.addType( "transis"  , &PyTypeTransistor              , "<Transistor>"              , false, "tfamily" );
    __cs.addType( "transpair", &PyTypeTransistorPair          , "<TransistorPair>"          , false, "tfamily" );
    __cs.addType( "cdrnpair" , &PyTypeCommonDrain             , "<CommonDrain>"             , false, "transpair" );
    __cs.addType( "cgatepair", &PyTypeCommonGatePair          , "<CommonGatePair>"          , false, "transpair" );
    __cs.addType( "csrcpair" , &PyTypeCommonSourcePair        , "<CommonSourcePair>"        , false, "transpair" );
    __cs.addType( "ccpair"   , &PyTypeCrossCoupledPair        , "<CrossCoupledPair>"        , false, "transpair" );
    __cs.addType( "diffpair" , &PyTypeDifferentialPair        , "<DifferentialPair>"        , false, "transpair" );
    __cs.addType( "levshift" , &PyTypeLevelShifter            , "<LevelShifter>"            , false, "transpair" );
    __cs.addType( "scurmirr" , &PyTypeSimpleCurrentMirror     , "<SimpleCurrentMirror>"     , false, "transpair" );
    __cs.addType( "cascode"  , &PyTypeCascode                 , "<Cascode>"                 , false, "transpair" );

    __cs.addType( "cfamily"  , &PyTypeCapacitorFamily         , "<CapacitorFamily>"         , false, "device" );
    __cs.addType( "mulcapa"  , &PyTypeMultiCapacitor          , "<MultiCapacitor>"          , false, "cfamily" );

    __cs.addType( "matrix"   , &PyTypeMatrix                  , "<Matrix>"                  , false );
    __cs.addType( "parameter", &PyTypeParameter               , "<Parameter>"               , false );
    __cs.addType( "choicepar", &PyTypeChoiceParameter         , "<CapacitorParameter>"      , false, "parameter" );
    __cs.addType( "ffpar"    , &PyTypeFormFactorParameter     , "<FormFactorParameter>"     , false, "parameter" );
    __cs.addType( "mcboxpar" , &PyTypeMCheckBoxParameter      , "<MCheckBoxParameter>"      , false, "parameter" );
    __cs.addType( "sboxpar"  , &PyTypeSpinBoxParameter        , "<SpinBoxParameter>"        , false, "parameter" );
    __cs.addType( "steppar"  , &PyTypeStepParameter           , "<StepParameter>"           , false, "parameter" );
    __cs.addType( "capspar"  , &PyTypeCapacitiesParameter     , "<CapacitiesParameter>"     , false, "parameter" );
    __cs.addType( "matrpar"  , &PyTypeMatrixParameter         , "<MatrixParameter>"         , false, "parameter" );
    __cs.addType( "laygen"   , &PyTypeLayoutGenerator         , "<LayoutGenerator>"         , false );


    PyObject* module = Py_InitModule( "Analog", PyAnalog_Methods );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Analog module." << endl;
      return;
    }

    Py_INCREF( &PyTypeTransistorFamily );
    PyModule_AddObject( module, "Device"             , (PyObject*)&PyTypeDevice );
    Py_INCREF( &PyTypeDevice );
    PyModule_AddObject( module, "TransistorFamily"   , (PyObject*)&PyTypeTransistorFamily );
    Py_INCREF( &PyTypeTransistor );
    PyModule_AddObject( module, "Transistor"         , (PyObject*)&PyTypeTransistor );
    Py_INCREF( &PyTypeTransistorPair );
    PyModule_AddObject( module, "TransistorPair"     , (PyObject*)&PyTypeTransistorPair );
    Py_INCREF( &PyTypeCommonDrain );
    PyModule_AddObject( module, "CommonDrain"        , (PyObject*)&PyTypeCommonDrain );
    Py_INCREF( &PyTypeCommonGatePair );
    PyModule_AddObject( module, "CommonGatePair"     , (PyObject*)&PyTypeCommonGatePair );
    Py_INCREF( &PyTypeCommonSourcePair );
    PyModule_AddObject( module, "CommonSourcePair"   , (PyObject*)&PyTypeCommonSourcePair );
    Py_INCREF( &PyTypeCrossCoupledPair );
    PyModule_AddObject( module, "CrossCoupledPair"   , (PyObject*)&PyTypeCrossCoupledPair );
    Py_INCREF( &PyTypeDifferentialPair );
    PyModule_AddObject( module, "DifferentialPair"   , (PyObject*)&PyTypeDifferentialPair );
    Py_INCREF( &PyTypeLevelShifter );
    PyModule_AddObject( module, "LevelShifter"       , (PyObject*)&PyTypeLevelShifter );
    Py_INCREF( &PyTypeSimpleCurrentMirror );
    PyModule_AddObject( module, "SimpleCurrentMirror", (PyObject*)&PyTypeSimpleCurrentMirror );
    Py_INCREF( &PyTypeCascode );
    PyModule_AddObject( module, "Cascode"            , (PyObject*)&PyTypeCascode );

    Py_INCREF( &PyTypeCapacitorFamily );
    PyModule_AddObject( module, "CapacitorFamily"   , (PyObject*)&PyTypeCapacitorFamily );
    Py_INCREF( &PyTypeMultiCapacitor );
    PyModule_AddObject( module, "MultiCapacitor"    , (PyObject*)&PyTypeMultiCapacitor );

    Py_INCREF( &PyTypeMatrix );
    PyModule_AddObject( module, "Matrix"             , (PyObject*)&PyTypeMatrix );
    Py_INCREF( &PyTypeParameter );
    PyModule_AddObject( module, "Parameter"          , (PyObject*)&PyTypeParameter );
    Py_INCREF( &PyTypeParameter );
    PyModule_AddObject( module, "ChoiceParameter"    , (PyObject*)&PyTypeChoiceParameter );
    Py_INCREF( &PyTypeFormFactorParameter );
    PyModule_AddObject( module, "FormFactorParameter", (PyObject*)&PyTypeFormFactorParameter );
    Py_INCREF( &PyTypeMCheckBoxParameter );
    PyModule_AddObject( module, "MCheckBoxParameter" , (PyObject*)&PyTypeMCheckBoxParameter );
    Py_INCREF( &PyTypeSpinBoxParameter );
    PyModule_AddObject( module, "SpinBoxParameter"   , (PyObject*)&PyTypeSpinBoxParameter );
    Py_INCREF( &PyTypeStepParameter );
    PyModule_AddObject( module, "CapacitiesParameter", (PyObject*)&PyTypeCapacitiesParameter );
    Py_INCREF( &PyTypeCapacitiesParameter );
    PyModule_AddObject( module, "MatrixParameter"    , (PyObject*)&PyTypeMatrixParameter );
    Py_INCREF( &PyTypeMatrixParameter );
    PyModule_AddObject( module, "StepParameter"      , (PyObject*)&PyTypeStepParameter );
    Py_INCREF( &PyTypeLayoutGenerator );
    PyModule_AddObject( module, "LayoutGenerator"    , (PyObject*)&PyTypeLayoutGenerator );

    PyDevice_postModuleInit();
    PyTransistorFamily_postModuleInit();
    PyCapacitorFamily_postModuleInit();
    PyParameter_postModuleInit();
    PyLayoutGenerator_postModuleInit();

  //Py_INCREF( &PyTypeDevice );
  //PyModule_AddObject( module, "Device"             , (PyObject*)&PyTypeDevice );
  
  //PyObject* dictionnary = PyModule_GetDict ( module );
  //trace_off();
  }

  
} // extern "C".

#endif // End of Python Module Code Part.

}  // Isobar namespace.
