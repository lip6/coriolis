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
// |  C++ Header  :  "./hurricane/configuration/PyMaterial.h"        |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/configuration/PyTypeManager.h"
#include "hurricane/BasicLayer.h"


namespace  Isobar3 {


extern "C" {


  extern PyMethodDef   PyMaterial_Methods[];
  extern PyObject*     PyMaterial_NEW  ( PyTypeObject* pyType, PyObject* args, PyObject* kwargs );
  extern int           PyMaterial_Init ( PyObject* self, PyObject* args, PyObject* kwargs );


}  // extern "C".

} // Isobar3 namespace.


namespace  Isobar3 {
  

  template<>
  inline void  pyTypePostInit<Hurricane::BasicLayer::Material> ( PyTypeObject* typeObject )
  {
    addConstant( typeObject, "nWell"   , BasicLayer::Material::nWell    );
    addConstant( typeObject, "pWell"   , BasicLayer::Material::pWell    );
    addConstant( typeObject, "nImplant", BasicLayer::Material::nImplant );
    addConstant( typeObject, "pImplant", BasicLayer::Material::pImplant );
    addConstant( typeObject, "active"  , BasicLayer::Material::active   );
    addConstant( typeObject, "poly"    , BasicLayer::Material::poly     );
    addConstant( typeObject, "cut"     , BasicLayer::Material::cut      );
    addConstant( typeObject, "metal"   , BasicLayer::Material::metal    );
    addConstant( typeObject, "blockage", BasicLayer::Material::blockage );
    addConstant( typeObject, "info"    , BasicLayer::Material::info     );
    addConstant( typeObject, "other"   , BasicLayer::Material::other    );
  }


}  // Isobar3 namespace.
