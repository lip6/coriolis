// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/configuration/PyParameter.h"       |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/configuration/PyTypeManager.h"
#include "hurricane/configuration/Parameter.h"
#include "hurricane/configuration/ParameterWidget.h"


namespace  Cfg {


extern "C" {


  extern PyMethodDef   PyParameter_Methods[];
  extern PyMethodDef   PyParameterPriority_Methods[];
  extern PyMethodDef   PyParameterType_Methods[];
  extern PyMethodDef   PyParameterFlags_Methods[];
  extern PyGetSetDef   PyParameter_Getsets[];


}  // extern "C".

}  // Cfg namespace.


inline bool  pyToC ( PyObject* pyArg, Cfg::Parameter::Type* arg )
{
  if (not PyLong_Check(pyArg)) return false;
  long type = PyLong_AsLong( pyArg );
  switch ( type ) {
    case Cfg::Parameter::Unknown:
    case Cfg::Parameter::String:
    case Cfg::Parameter::Bool:
    case Cfg::Parameter::Int:
    case Cfg::Parameter::Enumerate:
    case Cfg::Parameter::Double:
    case Cfg::Parameter::Percentage:
      *arg = (Cfg::Parameter::Type)type;
      break;
    default:
      return false;
  }
  return true;
}


inline bool  pyToC ( PyObject* pyArg, Cfg::Parameter::Priority* arg )
{
  if (not PyLong_Check(pyArg)) return false;
  long priority = PyLong_AsLong( pyArg );
  switch ( priority ) {
    case Cfg::Parameter::UseDefault:
    case Cfg::Parameter::ApplicationBuiltin:
    case Cfg::Parameter::ConfigurationFile:
    case Cfg::Parameter::UserFile:
    case Cfg::Parameter::CommandLine:
    case Cfg::Parameter::Interactive:
      *arg = (Cfg::Parameter::Priority)priority;
      break;
    default:
      return false;
  }
  return true;
}


template<>
inline PyObject* cToPy<Cfg::Parameter::Type>( Cfg::Parameter::Type type )
{ return Py_BuildValue( "i", type ); }


template<>
inline PyObject* cToPy<Cfg::Parameter::Priority>( Cfg::Parameter::Priority pri )
{ return Py_BuildValue( "i", pri ); }


template<>
inline PyObject* cToPy<Cfg::Parameter::Flags>( Cfg::Parameter::Flags flags )
{ return Py_BuildValue( "i", flags ); }
  

namespace Isobar3 {


  template<>
  inline void  pyTypePostInit<Cfg::Parameter> ( PyTypeObject* typeObject )
  {
    // std::cerr << "pyTypePostInit<Cfg::Parameter>()" << std::endl;
    PyTypeManagerNonDBo<Cfg::Parameter::Priority>::create( (PyObject*)typeObject
                                                         , Cfg::PyParameterPriority_Methods
                                                         , NULL
                                                         , PyTypeManager::NoCppDelete
                                                         , "Priority"
                                                         );
    PyTypeManagerNonDBo<Cfg::Parameter::Type    >::create( (PyObject*)typeObject
                                                         , Cfg::PyParameterType_Methods
                                                         , NULL
                                                         , PyTypeManager::NoCppDelete
                                                         , "Type"
                                                         );
    PyTypeManagerNonDBo<Cfg::Parameter::Flags   >::create( (PyObject*)typeObject
                                                         , Cfg::PyParameterFlags_Methods
                                                         , NULL
                                                         , PyTypeManager::NoCppDelete
                                                         , "Flags"
                                                         );
  }
  

  template<>
  inline void  pyTypePostInit<Cfg::Parameter::Priority> ( PyTypeObject* typeObject )
  {
  // Parameter::Priority enum.
    addConstant( typeObject, "UseDefault"        , Cfg::Parameter::UseDefault );
    addConstant( typeObject, "ApplicationBuiltin", Cfg::Parameter::ApplicationBuiltin );
    addConstant( typeObject, "ConfigurationFile" , Cfg::Parameter::ConfigurationFile );
    addConstant( typeObject, "UserFile"          , Cfg::Parameter::UserFile );
    addConstant( typeObject, "CommandLine"       , Cfg::Parameter::CommandLine );
    addConstant( typeObject, "Interactive"       , Cfg::Parameter::Interactive );
  }
  

  template<>
  inline void  pyTypePostInit<Cfg::Parameter::Type> ( PyTypeObject* typeObject )
  {
  // Parameter::Type enum.
    addConstant( typeObject, "Unknown"   , Cfg::Parameter::Unknown );
    addConstant( typeObject, "String"    , Cfg::Parameter::String );
    addConstant( typeObject, "Bool"      , Cfg::Parameter::Bool );
    addConstant( typeObject, "Int"       , Cfg::Parameter::Int );
    addConstant( typeObject, "Enumerate" , Cfg::Parameter::Enumerate );
    addConstant( typeObject, "Double"    , Cfg::Parameter::Double );
    addConstant( typeObject, "Percentage", Cfg::Parameter::Percentage );
  }
  

  template<>
  inline void  pyTypePostInit<Cfg::Parameter::Flags> ( PyTypeObject* typeObject )
  {
  // Parameter::Flags enum.
    addConstant( typeObject, "HasMin"         , Cfg::Parameter::HasMin );
    addConstant( typeObject, "HasMax"         , Cfg::Parameter::HasMax );
    addConstant( typeObject, "IsFile"         , Cfg::Parameter::IsFile );
    addConstant( typeObject, "IsPath"         , Cfg::Parameter::IsPath );
    addConstant( typeObject, "NeedRestart"    , Cfg::Parameter::NeedRestart );
    addConstant( typeObject, "MustExist"      , Cfg::Parameter::MustExist );
    addConstant( typeObject, "TypeCheck"      , Cfg::Parameter::TypeCheck );
    addConstant( typeObject, "FromString"     , Cfg::Parameter::FromString );
    addConstant( typeObject, "AllRequirements", Cfg::Parameter::AllRequirements );
    addConstant( typeObject, "UseSpinBox"     , Cfg::ParameterWidget::UseSpinBox );
    addConstant( typeObject, "IsFileName"     , Cfg::ParameterWidget::IsFileName );
    addConstant( typeObject, "IsPathName"     , Cfg::ParameterWidget::IsPathName );
  }


}  // Isobar3 namespace.
