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
// |  C++ Header  :  "./hurricane/configuration/PyDataBase.h"        |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/configuration/PyTypeManager.h"
#include "hurricane/DataBase.h"


namespace  Isobar3 {


extern "C" {


  extern PyMethodDef   PyDataBase_Methods[];
  extern PyMethodDef   PyDataBaseFlags_Methods[];


}  // extern "C".

}  // Isobar3 namespace.


template<>
inline PyObject* cToPy<Hurricane::DataBase::Flags>( Hurricane::DataBase::Flags flags )
{ return Py_BuildValue( "i", flags ); }
  

namespace Isobar3 {


  template<>
  inline void  pyTypePostInit<Hurricane::DataBase> ( PyTypeObject* typeObject )
  {
    PyTypeManagerNonDBo<Hurricane::DataBase::Flags>::create( (PyObject*)typeObject
                                                           , PyDataBaseFlags_Methods
                                                           , NULL
                                                           , PyTypeManager::NoCppDelete
                                                           , "Flags" );
  }
  

  template<>
  inline void  pyTypePostInit<Hurricane::DataBase::Flags> ( PyTypeObject* typeObject )
  {
  // Parameter::Flags enum.
    addConstant( typeObject, "NoFlags"      , Hurricane::DataBase::NoFlags );
    addConstant( typeObject, "CreateLib"    , Hurricane::DataBase::CreateLib );
    addConstant( typeObject, "WarnCreateLib", Hurricane::DataBase::WarnCreateLib );
  }


}  // Isobar3 namespace.
