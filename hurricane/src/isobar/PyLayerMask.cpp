
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
// |  C++ Module  :  "./PyLayerMask.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLayerMask.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


// +=================================================================+
// |             "PyLayerMask" Python Module Code Part               |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


#define  methodRMaskFromMask(FUNC_NAME) \
  PyObject* PyLayerMask_##FUNC_NAME ( PyLayerMask* self, PyObject* args )    \
  {                                                                          \
    PyObject* arg0;                                                          \
    if (PyArg_ParseTuple( args, "O:Layer.Mask."#FUNC_NAME"()", &arg0)) {     \
      if (not IsPyLayerMask(arg0)) {                                         \
        PyErr_SetString ( ConstructorError                                   \
                        , "Layer.Mask."#FUNC_NAME"(): Argument is not of Layer.Mask type." );  \
        return NULL;                                                         \
      }                                                                      \
    } else {                                                                 \
      PyErr_SetString ( ConstructorError                                     \
                      , "Invalid number of parameters passed to Layer.Mask."#FUNC_NAME"()." );  \
      return NULL;                                                           \
    }                                                                        \
    self->_object.FUNC_NAME(PYLAYERMASK_O(arg0));                            \
    Py_INCREF(self);                                                         \
    return (PyObject*)self;                                                  \
  }


#define  predicateFromMask(FUNC_NAME) \
  PyObject* PyLayerMask_##FUNC_NAME ( PyLayerMask* self, PyObject* args )    \
  {                                                                          \
    PyObject* arg0;                                                          \
    if (PyArg_ParseTuple( args, "O:Layer.Mask."#FUNC_NAME"()", &arg0)) {     \
      if (not IsPyLayerMask(arg0)) {                                         \
        PyErr_SetString ( ConstructorError                                   \
                        , "Layer.Mask."#FUNC_NAME"(): Argument is not of Layer.Mask type." );  \
        return NULL;                                                         \
      }                                                                      \
    } else {                                                                 \
      PyErr_SetString ( ConstructorError                                     \
                      , "Invalid number of parameters passed to Layer.Mask."#FUNC_NAME"()." );  \
      return NULL;                                                           \
    }                                                                        \
    if (self->_object.FUNC_NAME(PYLAYERMASK_O(arg0))) Py_RETURN_TRUE;        \
    Py_RETURN_FALSE;                                                         \
  }


  // ---------------------------------------------------------------
  // PyLayer Attribute Methods.

  static PyObject* PyLayerMask_new ( PyTypeObject* type, PyObject* args, PyObject* kwds )
  {
    cdebug_log(20,0) << "PyLayerMask_new()" << endl;

    PyLayerMask* pyMask = (PyLayerMask*)type->tp_alloc(type,0);

    if ( pyMask ) {
      unsigned long long  value = 0;
      if (PyArg_ParseTuple(args,"K:LayerMask.new", &value)) {
        pyMask->_object = Layer::Mask(value);
      } else {
        PyErr_SetString ( ConstructorError, "Invalid/bad number of parameters for LayerMask()." );
        return NULL;
      }
    }
    return (PyObject*)pyMask;
  }


  PyObject* PyLayerMask_zero ( PyLayerMask* self )
  {
    self->_object.zero();
    Py_RETURN_NONE;
  }


  PyObject* PyLayerMask_nthbit ( PyLayerMask* self, PyObject* pyInt )
  {
    Layer::Mask result;
    result = self->_object.nthbit(PyAny_AsLong(pyInt));
    return PyLayerMask_Link(result);
  }


  predicateFromMask  (isSet)
  predicateFromMask  (contains)
  predicateFromMask  (intersect)
  methodRMaskFromMask(set)
  methodRMaskFromMask(unset)


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
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyLayer Number Protocol Methods.


#define  binaryInFunctionMaskMask(FUNC_NAME,OP) \
  PyObject* PyLayerMask_##FUNC_NAME ( PyLayerMask* pyMask0, PyLayerMask* pyMask1 ) \
  {                                                                                \
    pyMask0->_object OP pyMask1->_object;                                         \
    Py_INCREF(pyMask0);                                                            \
    return (PyObject*)pyMask0;                                                     \
  }


#define  binaryInFunctionMaskInt(FUNC_NAME,OP) \
  PyObject* PyLayerMask_##FUNC_NAME ( PyLayerMask* pyMask0, PyObject* pyInt ) \
  {                                                                           \
    pyMask0->_object OP= PyAny_AsLong(pyInt);                                 \
    Py_INCREF(pyMask0);                                                       \
    return (PyObject*)pyMask0;                                                \
  }


#define  binaryFunctionMaskMask(FUNC_NAME,OP) \
  PyObject* PyLayerMask_##FUNC_NAME ( PyLayerMask* pyMask0, PyLayerMask* pyMask1 ) \
  {                                                                                \
    Layer::Mask result;                                                            \
    result = pyMask0->_object OP pyMask1->_object;                                 \
    return PyLayerMask_Link(result);                                               \
  }


#define  binaryFunctionMaskInt(FUNC_NAME,OP) \
  PyObject* PyLayerMask_##FUNC_NAME ( PyLayerMask* pyMask0, PyObject* pyInt ) \
  {                                                                           \
    Layer::Mask result;                                                       \
    result = pyMask0->_object OP PyAny_AsLong(pyInt);                         \
    return PyLayerMask_Link(result);                                          \
  }


  int  PyLayerMask_nonzero ( PyLayerMask* pyMask )
  { return pyMask->_object != Layer::Mask(0); }


  int  PyLayerMask_invert ( PyLayerMask* pyMask )
  { return not pyMask->_object; }


  binaryFunctionMaskInt   (lshift,<<)
  binaryFunctionMaskInt   (rshift,>>)
  binaryFunctionMaskMask  (and   ,bitand)
  binaryFunctionMaskMask  (xor   ,^)
  binaryFunctionMaskMask  (or    ,bitor)
  binaryInFunctionMaskMask(andIn ,&=)
  binaryInFunctionMaskMask(orIn  ,|=)


  PyNumberMethods PyLayerMask_NumberMethods = {
       0                                // binaryfunc  nb_add;
     , 0                                // binaryfunc  nb_subtract;
     , 0                                // binaryfunc  nb_multiply;
     , 0                                // binaryfunc  nb_remainder;
     , 0                                // binaryfunc  nb_divmod;
     , 0                                // ternaryfunc nb_power;
     , 0                                // unaryfunc   nb_negative;
     , 0                                // unaryfunc   nb_positive;
     , 0                                // unaryfunc   nb_absolute;
     , (inquiry)   PyLayerMask_nonzero  // inquiry     nb_bool;      -- Used by PyObject_IsTrue
     , (unaryfunc) PyLayerMask_invert   // unaryfunc   nb_invert;
     , (binaryfunc)PyLayerMask_lshift   // binaryfunc  nb_lshift;
     , (binaryfunc)PyLayerMask_rshift   // binaryfunc  nb_rshift;
     , (binaryfunc)PyLayerMask_and      // binaryfunc  nb_and;
     , (binaryfunc)PyLayerMask_xor      // binaryfunc  nb_xor;
     , (binaryfunc)PyLayerMask_or       // binaryfunc  nb_or;
     , 0                                // unaryfunc   nb_int;
     , NULL                             // void*       nb_reserved;
     , 0                                // unaryfunc   nb_float;
                                        // Added in release 2.0
     , 0                                // binaryfunc  nb_inplace_add;
     , 0                                // binaryfunc  nb_inplace_subtract;
     , 0                                // binaryfunc  nb_inplace_multiply;
     , 0                                // binaryfunc  nb_inplace_remainder;
     , 0                                // ternaryfunc nb_inplace_power;
     , 0                                // binaryfunc  nb_inplace_lshift;
     , 0                                // binaryfunc  nb_inplace_rshift;
     , (binaryfunc)PyLayerMask_andIn    // binaryfunc  nb_inplace_and;
     , 0                                // binaryfunc  nb_inplace_xor;
     , (binaryfunc)PyLayerMask_orIn     // binaryfunc  nb_inplace_or;
                                        // Added in release 2.2
     , 0                                // binaryfunc  nb_floor_divide;
     , 0                                // binaryfunc  nb_true_divide;
     , 0                                // binaryfunc  nb_inplace_floor_divide;
     , 0                                // binaryfunc  nb_inplace_true_divide;
                                        // Added in release 2.5
     , 0                                // unaryfunc   nb_index;
     , 0                                // binaryfunc  nb_matrix_multiply;
     , 0                                // binaryfunc  nb_inplce_matrix_multiply;
    };


  // ---------------------------------------------------------------
  // PyLayer Type Methods.


  static PyObject* PyLayerMask_Repr ( PyLayerMask *self )
  {
    ostringstream repr;
    repr << "[" << hex << self << "<->" << (void*)&(self->_object) << " " << self->_object << "]";
    return ( PyString_FromString(repr.str().c_str()) );
  }


  static PyObject* PyLayerMask_Str ( PyLayerMask *self )
  {
    return ( PyString_FromString(getString(self->_object).c_str()) );
  }


  static void PyLayerMask_DeAlloc ( PyLayerMask *self )
  {
    cdebug_log(20,0) << "PyLayerMask_DeAlloc(" << hex << self << ") " << self->_object << endl;
    PyObject_DEL ( self );
  }


  static PyObject* PyLayerMask_Cmp ( PyLayerMask *self, PyObject* other, int op )
  {
    if ( not IsPyLayerMask(other) ) Py_RETURN_FALSE;

    PyLayerMask* otherPyObject = (PyLayerMask*)other;
    if ((op == Py_LT) and (self->_object <  otherPyObject->_object)) Py_RETURN_TRUE;
    if ((op == Py_LE) and (self->_object <= otherPyObject->_object)) Py_RETURN_TRUE;
    if ((op == Py_EQ) and (self->_object == otherPyObject->_object)) Py_RETURN_TRUE;
    if ((op == Py_NE) and (self->_object != otherPyObject->_object)) Py_RETURN_TRUE;
    if ((op == Py_GT) and (self->_object >  otherPyObject->_object)) Py_RETURN_TRUE;
    if ((op == Py_GE) and (self->_object >= otherPyObject->_object)) Py_RETURN_TRUE;

    Py_RETURN_FALSE;
  }


  static int  PyLayerMask_Hash ( PyLayerMask *self)
  {
    return (long)&(self->_object);
  }


  extern void  PyLayerMask_LinkPyType()
  {
    cdebug_log(20,0) << "PyLayerMask_LinkType()" << endl;

    PyTypeLayerMask.tp_new         =              PyLayerMask_new;
    PyTypeLayerMask.tp_dealloc     = (destructor) PyLayerMask_DeAlloc;
    PyTypeLayerMask.tp_richcompare = (richcmpfunc)PyLayerMask_Cmp;
    PyTypeLayerMask.tp_repr        = (reprfunc)   PyLayerMask_Repr;
    PyTypeLayerMask.tp_str         = (reprfunc)   PyLayerMask_Str;
    PyTypeLayerMask.tp_hash        = (hashfunc)   PyLayerMask_Hash;
    PyTypeLayerMask.tp_methods     =  PyLayerMask_Methods;
    PyTypeLayerMask.tp_as_number   = &PyLayerMask_NumberMethods;
  }


#else  // End of Python Module Code Part.


// x=================================================================x
// |              "PyLayer" Shared Library Code Part                 |
// x=================================================================x


  // Link/Creation Method.
  PyObject* PyLayerMask_Link ( const Layer::Mask& mask )
  {
    PyLayerMask* pyMask = NULL;
    pyMask = PyObject_NEW(PyLayerMask, &PyTypeLayerMask);
    if (pyMask == NULL) return NULL;

    pyMask->_object = mask;

    return (PyObject*)pyMask;
  }


  // ---------------------------------------------------------------
  // PyLayer Object Definitions.

  PyTypeObject  PyTypeLayerMask =
    { PyVarObject_HEAD_INIT(NULL,0)
      "Hurricane.Layer.Mask"          /* tp_name.          */
    , sizeof(PyLayerMask)             /* tp_basicsize.     */
    , 0                               /* tp_itemsize.      */
    /* methods. */
    , 0                               /* tp_dealloc.       */
    , 0                               /* tp_print.         */
    , 0                               /* tp_getattr.       */
    , 0                               /* tp_setattr.       */
    , 0                               /* tp_compare.       */
    , 0                               /* tp_repr.          */
    , 0                               /* tp_as_number.     */
    , 0                               /* tp_as_sequence.   */
    , 0                               /* tp_as_mapping.    */
    , 0                               /* tp_hash.          */
    , 0                               /* tp_call.          */
    , 0                               /* tp_str            */
    , 0                               /* tp_getattro.      */
    , 0                               /* tp_setattro.      */
    , 0                               /* tp_as_buffer.     */
    , Py_TPFLAGS_DEFAULT              /* tp_flags          */
    , "Layer::Mask objects"           /* tp_doc.           */
  };


#endif  // End of Shared Library Code Part.


}  // extern "C".

}  // Isobar namespace. 
