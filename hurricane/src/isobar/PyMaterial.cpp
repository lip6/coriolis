
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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyMaterial.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyTechnology.h"
#include "hurricane/isobar/PyMaterial.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(BasicLayer::Material,material,function)


// +=================================================================+
// |             "PyMaterial" Python Module Code Part                |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |              "PyMaterial" Attribute Methods                 |
  // +-------------------------------------------------------------+


  static PyObject* PyMaterial_new ( PyTypeObject* type, PyObject* args, PyObject* kwds )
  {
    trace << "PyMaterial_new()" << endl;

    BasicLayer::Material* material   = NULL;
    PyMaterial*           pyMaterial = (PyMaterial*)type->tp_alloc(type,0);

    HTRY
    if ( pyMaterial ) {
      int code = 0;
      if (PyArg_ParseTuple(args,"i:BasicLayer.Material.new", &code)) {
        BasicLayer::Material::Code  enumCode;
        if (code <= (int)BasicLayer::Material::other) enumCode = (BasicLayer::Material::Code)code;
        else {
          PyErr_SetString(ConstructorError,"BasicLayer::Material(): Invalid code value.");
          return NULL;
        }
        material = new BasicLayer::Material(enumCode);
      } else {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for BasicLayer.Material()." );
        return NULL;
      }
      pyMaterial->_object = material;
    }
    HCATCH

    return (PyObject*)pyMaterial;
  }


  PyObject* PyMaterial_getCode ( PyMaterial* self )
  {
    trace << "PyMaterial_getCode()" << endl;

    int code = 0;

    HTRY
    METHOD_HEAD("Material.getCode()")
    code = (int)material->getCode();
    HCATCH

    return Py_BuildValue("i",code);
  }



  // ---------------------------------------------------------------
  // PyMaterial Attribute Method table.

  PyMethodDef PyMaterial_Methods[] =
    { { "getCode"             , (PyCFunction)PyMaterial_getCode, METH_NOARGS
                              , "Returns the numerical code of the material (enum)." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |                "PyMaterial" Object Methods                  |
  // +-------------------------------------------------------------+

  DirectHashMethod  (PyMaterial_Hash   , PyMaterial)
  DirectReprMethod  (PyMaterial_Repr   , PyMaterial, BasicLayer::Material)
  DirectStrMethod   (PyMaterial_Str    , PyMaterial, BasicLayer::Material)
  DirectStrMethod   (PyMaterial_Cmp    , PyMaterial, BasicLayer::Material)
  DirectDeleteMethod(PyMaterial_DeAlloc, PyMaterial)

  extern void  PyMaterial_LinkPyType() {
    trace << "PyMaterial_LinkType()" << endl;

    PyTypeMaterial.tp_new     =              PyMaterial_new;
    PyTypeMaterial.tp_dealloc = (destructor) PyMaterial_DeAlloc;
    PyTypeMaterial.tp_repr    = (reprfunc)   PyMaterial_Repr;
    PyTypeMaterial.tp_str     = (reprfunc)   PyMaterial_Str;
    PyTypeMaterial.tp_compare = (cmpfunc)    PyMaterial_Cmp;
    PyTypeMaterial.tp_hash    = (hashfunc)   PyMaterial_Hash;
    PyTypeMaterial.tp_methods = PyMaterial_Methods;
  }


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyMaterial" Shared Library Code Part                |
// +=================================================================+


  // Link/Creation Method.
  PyObject* PyMaterial_Link ( BasicLayer::Material* object ) {
    if ( object == NULL ) Py_RETURN_NONE;

    PyMaterial* pyObject = NULL;
    HTRY
    pyObject = PyObject_NEW(PyMaterial, &PyTypeMaterial);
    if (pyObject == NULL) return NULL;

    pyObject->ACCESS_OBJECT = object;
    HCATCH

    return (PyObject*)pyObject;
  }


  extern void  PyMaterial_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeMaterial.tp_dict,BasicLayer::Material::nWell   ,"nWell"   );
    LoadObjectConstant(PyTypeMaterial.tp_dict,BasicLayer::Material::pWell   ,"pWell"   );
    LoadObjectConstant(PyTypeMaterial.tp_dict,BasicLayer::Material::nImplant,"nImplant");
    LoadObjectConstant(PyTypeMaterial.tp_dict,BasicLayer::Material::pImplant,"pImplant");
    LoadObjectConstant(PyTypeMaterial.tp_dict,BasicLayer::Material::active  ,"active"  );
    LoadObjectConstant(PyTypeMaterial.tp_dict,BasicLayer::Material::poly    ,"poly"    );
    LoadObjectConstant(PyTypeMaterial.tp_dict,BasicLayer::Material::cut     ,"cut"     );
    LoadObjectConstant(PyTypeMaterial.tp_dict,BasicLayer::Material::metal   ,"metal"   );
    LoadObjectConstant(PyTypeMaterial.tp_dict,BasicLayer::Material::blockage,"blockage");
    LoadObjectConstant(PyTypeMaterial.tp_dict,BasicLayer::Material::other   ,"other"   );
  }


  PyTypeObjectDefinitions(Material)


#endif  // End of Shared Library Code Part.


}  // extern "C".

}  // Isobar namespace. 
