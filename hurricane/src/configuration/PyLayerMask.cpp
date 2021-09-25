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
// |  C++ Module  :       "./PyLayerMask.cpp"                        |
// +-----------------------------------------------------------------+


#include <functional>
#include "hurricane/configuration/PyLayerMask.h"


namespace  Isobar3 {

using namespace Hurricane;


extern "C" {


// Thin wrappers for overloadeds member functions.

  static Layer::Mask* ctorMask0 ()
  { return new Layer::Mask(); }

  static Layer::Mask* ctorMask1m ( Layer::Mask other )
  { return new Layer::Mask( other ); }

  static Layer::Mask* ctorMask1l ( unsigned long long value )
  { return new Layer::Mask( value ); }

  static Layer::Mask* invert ( Layer::Mask* self )
  { return new Layer::Mask( ~(*self) ); }

  static void  inplaceBitand ( Layer::Mask* self, Layer::Mask* other )
  { (*self) &= (*other); }

  static void  inplaceBitor ( Layer::Mask* self, Layer::Mask* other )
  { (*self) |= (*other); }


// Python methods.

  static PyObject* PyLayerMask_zero ( PyObject* self, PyObject* args )
  { return callMethod("PyLayerMask.zero",&Layer::Mask::zero,self,args); }

  static PyObject* PyLayerMask_set ( PyObject* self, PyObject* args )
  { return callMethod("PyLayerMask.set",&Layer::Mask::set,self,args); }

  static PyObject* PyLayerMask_unset ( PyObject* self, PyObject* args )
  { return callMethod("PyLayerMask.unset",&Layer::Mask::unset,self,args); }

  static PyObject* PyLayerMask_isSet ( PyObject* self, PyObject* args )
  { return callMethod("PyLayerMask.isSet",&Layer::Mask::isSet,self,args); }

  static PyObject* PyLayerMask_contains ( PyObject* self, PyObject* args )
  { return callMethod("PyLayerMask.contains",&Layer::Mask::contains,self,args); }

  static PyObject* PyLayerMask_intersect ( PyObject* self, PyObject* args )
  { return callMethod("PyLayerMask.intersect",&Layer::Mask::intersect,self,args); }

  static PyObject* PyLayerMask_nthbit ( PyObject* self, PyObject* args )
  { return callMethod("PyLayerMask.nthbit",&Layer::Mask::nthbit,self,args); }

  static PyObject* PyLayerMask_fromString ( PyObject*, PyObject* args )
  { return callFunction("PyLayerMask.fromString",&Layer::Mask::fromString,args); }

  PyObject* PyLayerMask_NEW ( PyTypeObject* pyType, PyObject* args, PyObject* kwargs )
  {
    PyObject*       rvalue = callFunction("Layer::Mask",&ctorMask1m,args);
    if (not rvalue) rvalue = callFunction("Layer::Mask",&ctorMask1l,args);
    if (not rvalue) rvalue = callFunction("Layer::Mask",&ctorMask0 ,args);
    return rvalue;
  }

  int  PyLayerMask_Init ( PyObject* self, PyObject* args, PyObject* kwargs )
  {
    cdebug_log(20,0) << "PyLayerMask_Init(): " << (void*)self << endl;
    return 0;
  }


  PyObject* tpRichCompareLayerMask ( PyObject *self, PyObject* other, int op )
  {
    if (Py_TYPE(self) != Py_TYPE(other)) {
      std::string message = "tpRichCompareLayerMask<" + getString(demangle(typeid(Layer::Mask).name()))
                          + "> cannot compare to \"" + Py_TYPE(other)->tp_name + "\".";
      PyErr_SetString( PyExc_TypeError, message.c_str() );
      return NULL;
    }
    Layer::Mask* cself  = reinterpret_cast<Layer::Mask*>( object1(self ) );
    Layer::Mask* cother = reinterpret_cast<Layer::Mask*>( object1(other) );

    bool result = false;
    if ((op == Py_LT) and (*cself <  *cother)) result = true;
    if ((op == Py_LE) and (*cself <= *cother)) result = true;
    if ((op == Py_EQ) and (*cself == *cother)) result = true;
    if ((op == Py_NE) and (*cself != *cother)) result = true;
    if ((op == Py_GT) and (*cself >  *cother)) result = true;
    if ((op == Py_GE) and (*cself >= *cother)) result = true;
    
    if (result) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  // ---------------------------------------------------------------
  // PyLayerMask Method table.

  PyMethodDef PyLayerMask_Methods[] =
    { { "zero"                , (PyCFunction)PyLayerMask_zero      , METH_NOARGS
                              , "Reset all the bits." }
    , { "set"                 , (PyCFunction)PyLayerMask_set       , METH_VARARGS
                              , "Sets to one the bits given in the argument (aka bitwise or)." }
    , { "unset"               , (PyCFunction)PyLayerMask_unset     , METH_VARARGS
                              , "Sets to zero the bits given in the argument (aka bitwise and with complement)." }
    , { "isSet"               , (PyCFunction)PyLayerMask_isSet     , METH_VARARGS
                              , "Returns true if some of bits at the mask's position are set (aka bitwise and)." }
    , { "contains"            , (PyCFunction)PyLayerMask_contains  , METH_VARARGS
                              , "Returns true if all the bits at the mask's position are set (aka bitwise and)." }
    , { "intersect"           , (PyCFunction)PyLayerMask_intersect , METH_VARARGS
                              , "Returns true if some of bits at the mask's position are set (aka bitwise and)." }
    , { "nthbit"              , (PyCFunction)PyLayerMask_nthbit    , METH_VARARGS
                              , "Returns a new mask with only the Nth bit copied." }
    , { "fromString"          , (PyCFunction)PyLayerMask_fromString, METH_VARARGS|METH_STATIC
                              , "Build a mask from an hexadecimal string." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  static int  PyLayerMask_nonzero ( PyObject* object )
  { return callPredicate( "Layer::Mask::nonzero", &Layer::Mask::zero,object ); }

  static PyObject*  PyLayerMask_invert ( PyObject* object )
  { return callMethod( "Layer::Mask::invert", &invert, object, NULL ); }

  static PyObject* PyLayerMask_bitand ( PyObject* pyLhs, PyObject* pyRhs )
  { return callOperator<Layer::Mask,std::bit_and>( "Layer::Mask::bitand", pyLhs, pyRhs ); }

  static PyObject* PyLayerMask_bitxor ( PyObject* pyLhs, PyObject* pyRhs )
  { return callOperator<Layer::Mask,std::bit_xor>( "Layer::Mask::bitxor", pyLhs, pyRhs ); }

  static PyObject* PyLayerMask_bitor ( PyObject* pyLhs, PyObject* pyRhs )
  { return callOperator<Layer::Mask,std::bit_or>( "Layer::Mask::bitor", pyLhs, pyRhs ); }

  static PyObject* PyLayerMask_rshift ( PyObject* self, PyObject* arg )
  { return callBinaryMethod( "Layer::Mask::rshift", &Layer::Mask::rshift, self, arg ); }

  static PyObject* PyLayerMask_lshift ( PyObject* self, PyObject* arg )
  { return callBinaryMethod( "Layer::Mask::lshift", &Layer::Mask::lshift, self, arg ); }

  static PyObject* PyLayerMask_inplaceBitand ( PyObject* pyLhs, PyObject* pyRhs )
  { return callOperator( "Layer::Mask::inplace_bitand", &inplaceBitand, pyLhs, pyRhs ); }

  static PyObject* PyLayerMask_inplaceBitor ( PyObject* pyLhs, PyObject* pyRhs )
  { return callOperator( "Layer::Mask::inplace_bitor", &inplaceBitor, pyLhs, pyRhs ); }
  

  PyNumberMethods PyLayerMask_NumberMethods =
    { .nb_add               = 0
    , .nb_subtract          = 0
    , .nb_multiply          = 0
    , .nb_remainder         = 0
    , .nb_divmod            = 0
    , .nb_power             = 0
    , .nb_negative          = 0
    , .nb_positive          = 0
    , .nb_absolute          = 0
    , .nb_bool              = PyLayerMask_nonzero
    , .nb_invert            = PyLayerMask_invert
    , .nb_lshift            = PyLayerMask_lshift
    , .nb_rshift            = PyLayerMask_rshift
    , .nb_and               = PyLayerMask_bitand
    , .nb_xor               = PyLayerMask_bitxor
    , .nb_or                = PyLayerMask_bitor
    , .nb_int               = 0
    , .nb_reserved          = NULL
    , .nb_float             = 0
    , .nb_inplace_add       = 0
    , .nb_inplace_subtract  = 0
    , .nb_inplace_multiply  = 0
    , .nb_inplace_remainder = 0
    , .nb_inplace_power     = 0
    , .nb_inplace_lshift    = 0
    , .nb_inplace_rshift    = 0
    , .nb_inplace_and       = PyLayerMask_inplaceBitand
    , .nb_inplace_xor       = 0
    , .nb_inplace_or        = PyLayerMask_inplaceBitor
    };


}  // extern "C".

}  // Isobar3 namespace.
