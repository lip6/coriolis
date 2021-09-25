// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2020-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/configuration/PyDbU.h"             |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/configuration/PyTypeManager.h"
#include "hurricane/DbU.h"


namespace  Isobar3 {


extern "C" {


  extern PyMethodDef   PyDbU_Methods[];


}  // extern "C".
  

  template<>
  inline void  pyTypePostInit<Hurricane::DbU> ( PyTypeObject* typeObject )
  {
    addConstant( typeObject, "SnapModeInferior"      , Hurricane::DbU::Inferior );
    addConstant( typeObject, "SnapModeSuperior"      , Hurricane::DbU::Superior );
    addConstant( typeObject, "SnapModeNearest"       , Hurricane::DbU::Nearest );
    addConstant( typeObject, "UnitPowerPico"         , Hurricane::DbU::Pico );
    addConstant( typeObject, "UnitPowerNano"         , Hurricane::DbU::Nano );
    addConstant( typeObject, "UnitPowerMicro"        , Hurricane::DbU::Micro );
    addConstant( typeObject, "UnitPowerMilli"        , Hurricane::DbU::Milli );
    addConstant( typeObject, "UnitPowerUnity"        , Hurricane::DbU::Unity );
    addConstant( typeObject, "UnitPowerKilo"         , Hurricane::DbU::Kilo );
    addConstant( typeObject, "StringModeDb"          , Hurricane::DbU::Db );
    addConstant( typeObject, "StringModeGrid"        , Hurricane::DbU::Grid );
    addConstant( typeObject, "StringModeSymbolic"    , Hurricane::DbU::Symbolic );
    addConstant( typeObject, "StringModePhysical"    , Hurricane::DbU::Physical );
    addConstant( typeObject, "StringModeSmartTrucate", Hurricane::DbU::SmartTruncate );
  }

}  // Isobar3 namespace.


inline bool  pyToC ( PyObject* pyArg, Hurricane::DbU::UnitPower* arg )
{
  if (not PyLong_Check(pyArg)) return false;
  long power = PyLong_AsLong( pyArg );
  switch ( power ) {
    case Hurricane::DbU::Pico:
    case Hurricane::DbU::Nano:
    case Hurricane::DbU::Micro:
    case Hurricane::DbU::Milli:
    case Hurricane::DbU::Unity:
    case Hurricane::DbU::Kilo:
      *arg = (Hurricane::DbU::UnitPower)power;
      break;
    default:
      return false;
  }
  return true;
}


template<>
inline PyObject* cToPy<Hurricane::DbU::UnitPower>( Hurricane::DbU::UnitPower power )
{ return Py_BuildValue( "i", power ); }
