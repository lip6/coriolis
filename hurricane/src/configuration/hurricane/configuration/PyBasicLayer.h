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
// |  C++ Header  :  "./hurricane/configuration/PyBasicLayer.h"      |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/configuration/PyTypeManager.h"
#include "hurricane/configuration/PyMaterial.h"
#include "hurricane/BasicLayer.h"


namespace  Isobar3 {


extern "C" {


  extern PyMethodDef   PyBasicLayer_Methods[];


}  // extern "C".


  template<>
  inline void  pyTypePostInit<Hurricane::BasicLayer> ( PyTypeObject* typeObject )
  {
    PyTypeManagerNonDBo<Hurricane::BasicLayer::Material>::create( (PyObject*)typeObject
                                                                , PyMaterial_Methods
                                                                , NULL
                                                                , PyTypeManager::NoCppDelete
                                                                , "Material"
                                                                , PyMaterial_NEW
                                                                , PyMaterial_Init
                                                                );
  }

}  // Isobar3 namespace.
