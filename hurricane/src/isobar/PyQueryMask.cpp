
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
// |  C++ Module  :  "./PyQueryMask.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyQueryMask.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


// +=================================================================+
// |             "PyQueryMask" Python Module Code Part               |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


#define  methodRMaskFromMask(FUNC_NAME) \
  PyObject* PyQueryMask_##FUNC_NAME ( PyQueryMask* self, PyObject* args )    \
  {                                                                          \
    PyObject* arg0;                                                          \
    if (PyArg_ParseTuple( args, "O:Query.Mask."#FUNC_NAME"()", &arg0)) {     \
      if (not IsPyQueryMask(arg0)) {                                         \
        PyErr_SetString ( ConstructorError                                   \
                        , "Query.Mask."#FUNC_NAME"(): Argument is not of Query.Mask type." );  \
        return NULL;                                                         \
      }                                                                      \
    } else {                                                                 \
      PyErr_SetString ( ConstructorError                                     \
                      , "Invalid number of parameters passed to Query.Mask."#FUNC_NAME"()." );  \
      return NULL;                                                           \
    }                                                                        \
    self->_object.FUNC_NAME(PYQUERYMASK_O(arg0));                            \
    Py_INCREF(self);                                                         \
    return (PyObject*)self;                                                  \
  }


#define  predicateFromMask(FUNC_NAME) \
  PyObject* PyQueryMask_##FUNC_NAME ( PyQueryMask* self, PyObject* args )    \
  {                                                                          \
    PyObject* arg0;                                                          \
    if (PyArg_ParseTuple( args, "O:Query.Mask."#FUNC_NAME"()", &arg0)) {     \
      if (not IsPyQueryMask(arg0)) {                                         \
        PyErr_SetString ( ConstructorError                                   \
                        , "Query.Mask."#FUNC_NAME"(): Argument is not of Query.Mask type." );  \
        return NULL;                                                         \
      }                                                                      \
    } else {                                                                 \
      PyErr_SetString ( ConstructorError                                     \
                      , "Invalid number of parameters passed to Query.Mask."#FUNC_NAME"()." );  \
      return NULL;                                                           \
    }                                                                        \
    if (self->_object.FUNC_NAME(PYQUERYMASK_O(arg0))) Py_RETURN_TRUE;        \
    Py_RETURN_FALSE;                                                         \
  }


  // ---------------------------------------------------------------
  // PyQuery Attribute Methods.

  static PyObject* PyQueryMask_new ( PyTypeObject* type, PyObject* args, PyObject* kwds )
  {
    trace << "PyQueryMask_new()" << endl;

    PyQueryMask* pyMask = (PyQueryMask*)type->tp_alloc(type,0);

    if ( pyMask ) {
      unsigned long long  value = 0;
      if (PyArg_ParseTuple(args,"K:QueryMask.new", &value)) {
        pyMask->_object = Query::Mask(value);
      } else {
        PyErr_SetString ( ConstructorError, "Invalid/bad number of parameters for QueryMask()." );
        return NULL;
      }
    }
    return (PyObject*)pyMask;
  }


  PyObject* PyQueryMask_zero ( PyQueryMask* self )
  {
    self->_object.zero();
    Py_RETURN_NONE;
  }


  PyObject* PyQueryMask_nthbit ( PyQueryMask* self, PyObject* pyInt )
  {
    Query::Mask result;
    result = self->_object.nthbit(PyAny_AsLong(pyInt));
    return PyQueryMask_Link(result);
  }


  predicateFromMask  (isSet)
  predicateFromMask  (contains)
  predicateFromMask  (intersect)
  methodRMaskFromMask(set)
  methodRMaskFromMask(unset)


  PyMethodDef PyQueryMask_Methods[] =
    { { "zero"                , (PyCFunction)PyQueryMask_zero      , METH_NOARGS
                              , "Reset all the bits." }
    , { "set"                 , (PyCFunction)PyQueryMask_set       , METH_VARARGS
                              , "Sets to one the bits given in the argument (aka bitwise or)." }
    , { "unset"               , (PyCFunction)PyQueryMask_unset     , METH_VARARGS
                              , "Sets to zero the bits given in the argument (aka bitwise and with complement)." }
    , { "isSet"               , (PyCFunction)PyQueryMask_isSet     , METH_VARARGS
                              , "Returns true if some of bits at the mask's position are set (aka bitwise and)." }
    , { "contains"            , (PyCFunction)PyQueryMask_contains  , METH_VARARGS
                              , "Returns true if all the bits at the mask's position are set (aka bitwise and)." }
    , { "intersect"           , (PyCFunction)PyQueryMask_intersect , METH_VARARGS
                              , "Returns true if some of bits at the mask's position are set (aka bitwise and)." }
    , { "nthbit"              , (PyCFunction)PyQueryMask_nthbit    , METH_VARARGS
                              , "Returns a new mask with only the Nth bit copied." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyQuery Number Protocol Methods.


#define  binaryInFunctionMaskMask(FUNC_NAME,OP) \
  PyObject* PyQueryMask_##FUNC_NAME ( PyQueryMask* pyMask0, PyQueryMask* pyMask1 ) \
  {                                                                                \
    pyMask0->_object OP pyMask1->_object;                                         \
    Py_INCREF(pyMask0);                                                            \
    return (PyObject*)pyMask0;                                                     \
  }


#define  binaryInFunctionMaskInt(FUNC_NAME,OP) \
  PyObject* PyQueryMask_##FUNC_NAME ( PyQueryMask* pyMask0, PyObject* pyInt ) \
  {                                                                           \
    pyMask0->_object OP= PyAny_AsLong(pyInt);                                 \
    Py_INCREF(pyMask0);                                                       \
    return (PyObject*)pyMask0;                                                \
  }


#define  binaryFunctionMaskMask(FUNC_NAME,OP) \
  PyObject* PyQueryMask_##FUNC_NAME ( PyQueryMask* pyMask0, PyQueryMask* pyMask1 ) \
  {                                                                                \
    Query::Mask result;                                                            \
    result = pyMask0->_object OP pyMask1->_object;                                 \
    return PyQueryMask_Link(result);                                               \
  }


#define  binaryFunctionMaskInt(FUNC_NAME,OP) \
  PyObject* PyQueryMask_##FUNC_NAME ( PyQueryMask* pyMask0, PyObject* pyInt ) \
  {                                                                           \
    Query::Mask result;                                                       \
    result = pyMask0->_object OP PyAny_AsLong(pyInt);                         \
    return PyQueryMask_Link(result);                                          \
  }


  int  PyQueryMask_nonzero ( PyQueryMask* pyMask )
  { return pyMask->_object != Query::Mask(0); }


  int  PyQueryMask_invert ( PyQueryMask* pyMask )
  { return not pyMask->_object; }


  binaryFunctionMaskInt   (lshift,<<)
  binaryFunctionMaskInt   (rshift,>>)
  binaryFunctionMaskMask  (and   ,bitand)
  binaryFunctionMaskMask  (xor   ,^)
  binaryFunctionMaskMask  (or    ,bitor)
  binaryInFunctionMaskMask(andIn ,&=)
  binaryInFunctionMaskMask(orIn  ,|=)


  PyNumberMethods PyQueryMask_NumberMethods = {
       0                                // binaryfunc  nb_add;
     , 0                                // binaryfunc  nb_subtract;
     , 0                                // binaryfunc  nb_multiply;
     , 0                                // binaryfunc  nb_divide;
     , 0                                // binaryfunc  nb_remainder;
     , 0                                // binaryfunc  nb_divmod;
     , 0                                // ternaryfunc nb_power;
     , 0                                // unaryfunc   nb_negative;
     , 0                                // unaryfunc   nb_positive;
     , 0                                // unaryfunc   nb_absolute;
     , (inquiry)   PyQueryMask_nonzero  // inquiry     nb_nonzero;      -- Used by PyObject_IsTrue
     , (unaryfunc) PyQueryMask_invert   // unaryfunc   nb_invert;
     , (binaryfunc)PyQueryMask_lshift   // binaryfunc  nb_lshift;
     , (binaryfunc)PyQueryMask_rshift   // binaryfunc  nb_rshift;
     , (binaryfunc)PyQueryMask_and      // binaryfunc  nb_and;
     , (binaryfunc)PyQueryMask_xor      // binaryfunc  nb_xor;
     , (binaryfunc)PyQueryMask_or       // binaryfunc  nb_or;
     , 0                                // coercion    nb_coerce;       -- Used by the coerce() function
     , 0                                // unaryfunc   nb_int;
     , 0                                // unaryfunc   nb_long;
     , 0                                // unaryfunc   nb_float;
     , 0                                // unaryfunc   nb_oct;
     , 0                                // unaryfunc   nb_hex;
                                        // Added in release 2.0
     , 0                                // binaryfunc  nb_inplace_add;
     , 0                                // binaryfunc  nb_inplace_subtract;
     , 0                                // binaryfunc  nb_inplace_multiply;
     , 0                                // binaryfunc  nb_inplace_divide;
     , 0                                // binaryfunc  nb_inplace_remainder;
     , 0                                // ternaryfunc nb_inplace_power;
     , 0                                // binaryfunc  nb_inplace_lshift;
     , 0                                // binaryfunc  nb_inplace_rshift;
     , (binaryfunc)PyQueryMask_andIn    // binaryfunc  nb_inplace_and;
     , 0                                // binaryfunc  nb_inplace_xor;
     , (binaryfunc)PyQueryMask_orIn     // binaryfunc  nb_inplace_or;
                                        // Added in release 2.2
     , 0                                // binaryfunc  nb_floor_divide;
     , 0                                // binaryfunc  nb_true_divide;
     , 0                                // binaryfunc  nb_inplace_floor_divide;
     , 0                                // binaryfunc  nb_inplace_true_divide;
                                        // Added in release 2.5
     , 0                                // unaryfunc   nb_index;
    };


  // ---------------------------------------------------------------
  // PyQuery Type Methods.


  static PyObject* PyQueryMask_Repr ( PyQueryMask *self )
  {
    ostringstream repr;
    repr << "[" << hex << self << "<->" << (void*)&(self->_object) << " " << self->_object << "]";
    return ( PyString_FromString(repr.str().c_str()) );
  }


  static PyObject* PyQueryMask_Str ( PyQueryMask *self )
  {
    return ( PyString_FromString(getString(self->_object).c_str()) );
  }


  static void PyQueryMask_DeAlloc ( PyQueryMask *self )
  {
    trace << "PyQueryMask_DeAlloc(" << hex << self << ") " << self->_object << endl;
    PyObject_DEL ( self );
  }


  static int  PyQueryMask_Cmp ( PyQueryMask *self, PyObject* other )
  {
    if ( not IsPyQueryMask(other) ) return -1;

    PyQueryMask* otherPyObject = (PyQueryMask*)other;
    if ( self->_object == otherPyObject->_object ) return  0;
    if ( self->_object <  otherPyObject->_object ) return -1;

    return 1;
  }


  static int  PyQueryMask_Hash ( PyQueryMask *self)
  {
    return (long)&(self->_object);
  }


  extern void  PyQueryMask_LinkPyType()
  {
    trace << "PyQueryMask_LinkType()" << endl;

    PyTypeQueryMask.tp_new       =              PyQueryMask_new;
    PyTypeQueryMask.tp_dealloc   = (destructor) PyQueryMask_DeAlloc;
    PyTypeQueryMask.tp_compare   = (cmpfunc)    PyQueryMask_Cmp;
    PyTypeQueryMask.tp_repr      = (reprfunc)   PyQueryMask_Repr;
    PyTypeQueryMask.tp_str       = (reprfunc)   PyQueryMask_Str;
    PyTypeQueryMask.tp_hash      = (hashfunc)   PyQueryMask_Hash;
    PyTypeQueryMask.tp_methods   =  PyQueryMask_Methods;
    PyTypeQueryMask.tp_as_number = &PyQueryMask_NumberMethods;
  }


#else  // End of Python Module Code Part.


// x=================================================================x
// |              "PyQuery" Shared Library Code Part                 |
// x=================================================================x


  // Link/Creation Method.
  PyObject* PyQueryMask_Link ( const Query::Mask& mask )
  {
    PyQueryMask* pyMask = NULL;
    pyMask = PyObject_NEW(PyQueryMask, &PyTypeQueryMask);
    if (pyMask == NULL) return NULL;

    pyMask->_object = mask;

    return (PyObject*)pyMask;
  }


  // ---------------------------------------------------------------
  // PyQuery Object Definitions.

  PyTypeObject  PyTypeQueryMask =
    { PyObject_HEAD_INIT(NULL)
      0                               /* ob_size.          */
    , "Hurricane.Query.Mask"          /* tp_name.          */
    , sizeof(PyQueryMask)             /* tp_basicsize.     */
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
    , "Query::Mask objects"           /* tp_doc.           */
  };


#endif  // End of Shared Library Code Part.


}  // extern "C".

}  // Isobar namespace. 
