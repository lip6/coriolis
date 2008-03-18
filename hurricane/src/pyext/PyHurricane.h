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
// ===================================================================
//
// $Id: PyHurricane.h,v 1.41 2008/02/07 17:09:41 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PyHurricane.h"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |     Damien DUPUIS                                26/04/2005     |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef __PYHURRICANE__
#define __PYHURRICANE__


#include "Python.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "Error.h"
#include "Warning.h"

#include "ProxyProperty.h"

namespace Isobar {


#if PY_VERSION_HEX < 0x02040000
    #define Py_RETURN_NONE return Py_INCREF(Py_None), Py_None
#endif


using namespace std;


// -------------------------------------------------------------------
// Class  :  "::ConverterState".

  class ConverterState {

    public:
      struct ObjectType {
        static char*         _inheritStop;
               char          _id    [11];
               char          _idBase[11];
               PyTypeObject* _pyType;
               char*         _name;
               bool          _isPythonType;
               unsigned      _index;
  
               bool          PyEqual  ( PyTypeObject* pyType );
               ObjectType*   PyBase   ();
               ObjectType*   PyBase   ( PyTypeObject* pyType );
               char*         PyBaseId ( PyTypeObject* pyType );
      };

    typedef vector<ObjectType*> ObjectTypeVector;

    protected: string            _objectIds;
    protected: char*             _function;
    protected: ObjectTypeVector  _types;
     

    public: ConverterState () : _objectIds("")
                              , _function("<NoFunction>")
                              , _types()
                              { }
    public: ~ConverterState ();

    public: ObjectType*             getObject     ( char* id );
    public: char*                   getObjectId   ( PyObject* object );
    public: char*                   getObjectName ( string id );
    public: string                  getObjectIds  () const { return ( _objectIds ); };
    public: char*                   getFunction   () const { return ( _function ); };
    public: int                     getSize       () const { return ( _objectIds.size() ); };
    public: const ObjectTypeVector& getTypes      () const { return ( _types ); };
    public: void                    AddObject     ( PyObject* object ) { _objectIds += getObjectId ( object ); };
    public: void                    AddId         ( char* id ) { _objectIds += ":"; _objectIds += id; };
    public: void                    SetFunction   ( char* function ) { _function = function; };
    public: void                    Init          ( char* function, char* inheritStop="comp" );
    public: void                    AddType       ( char* id, PyTypeObject* pyType, char* name, bool isPythonType, char* idBase="" );
    public: static string           getObjectType ( string objectsTypes, unsigned n );
  };




// -------------------------------------------------------------------
// Isobar Global Functions.

  int    Converter        ( PyObject* object, void** pArg );
  bool   ParseOneArg      ( char* function, PyObject* args, string format, PyObject** arg );
  bool   ParseTwoArg      ( char* function, PyObject* args, string format, PyObject** arg0, PyObject** arg1 );
  bool   ParseThreeArg    ( char* function, PyObject* args, string format, PyObject** arg0, PyObject** arg1, PyObject** arg2 );



// -------------------------------------------------------------------
// Isobar Global variables.

  extern ConverterState             __cs;
  extern int                        __objectOffset;

extern "C" {


  extern PyObject* ConstructorError;
  extern PyObject* ProxyError;
  extern PyObject* HurricaneError;
  extern PyObject* HurricaneWarning;
  extern PyObject* CoriolisError;


// -------------------------------------------------------------------
// Defines arguments strings

#define NO_ARG                   ""
#define BOX_ARG                  ":box"
#define CELL_ARG                 ":ent"
#define POINT_ARG                ":point"
#define POINTS2_ARG              ":point:point"
#define POINT_INT_ARG            ":point:int"
#define INT_ARG                  ":int"
#define INTS2_ARG                ":int:int"
#define INTS3_ARG                ":int:int:int"
#define INTS4_ARG                ":int:int:int:int"
#define INTS2_FUNCTION_ARG       ":int:int:function"
#define FLOAT_ARG                ":float"
#define BOOL_ARG                 ":bool"
#define NAME_ARG                 ":name"
#define INSTANCE_ARG             ":ent"
#define HOOK_ARG                 ":hook"
#define TRANS_ARG                ":transfo"
#define STRING_ARG               ":string"
#define STRING_CELL_ARG          ":string:ent"
#define NET_ARG                  ":ent"
#define NET_NAME_ARG             ":ent:name"
#define NET_LAYER_INT_ARG        ":ent:layer:int"
#define NET_LAYER_INTS2_ARG      ":ent:layer:int:int"
#define NET_LAYER_INTS3_ARG      ":ent:layer:int:int:int"
#define NET_LAYER_INTS4_ARG      ":ent:layer:int:int:int:int"
#define COMP_LAYER_INTS2_ARG     ":comp:layer:int:int"
#define COMP_LAYER_INTS3_ARG     ":comp:layer:int:int:int"
#define COMP_LAYER_INTS4_ARG     ":comp:layer:int:int:int:int"
#define COMPS2_LAYER_INT_ARG     ":comp:comp:layer:int"
#define COMPS2_LAYER_INTS2_ARG   ":comp:comp:layer:int:int"
#define COMPS2_LAYER_INTS3_ARG   ":comp:comp:layer:int:int:int"
#define COMPS2_LAYER_INTS4_ARG   ":comp:comp:layer:int:int:int:int"
#define CELL_BOX_ARG             ":ent:box"
#define CELL_INT_ARG             ":ent:int"
#define CELL_NAME_ARG            ":ent:name"
#define CELL_NAME_CELL_ARG       ":ent:name:ent"
#define CELL_NAME_CELL_TRANS_ARG ":ent:name:ent:transfo"
#define CELL_NAME_INTS2_ARG      ":ent:name:int:int"
#define CELL_NAME_POINT_ARG      ":ent:name:point"
#define INST_ARG                 ":ent"
#define INST_PATH_ARG            ":ent:path"
#define PATH_INST_ARG            ":path:ent"
#define CELL_STRING_ARG          ":ent:string"
#define ENT_ARG                  ":ent"
#define ENT_PATH_ARG             ":ent:path"
#define COMP_PATH_ARG            ":comp:path"

// -------------------------------------------------------------------
// Miscellaneous.


# define  DEFERRED_ADDRESS(ADDR)  0


// This macro must be redefined in derived classes.
// Example : _baseOject._object
# define  ACCESS_OBJECT            _object
# define  ACCESS_CLASS(_pyObject)  _pyObject


# define  LOAD_CONSTANT(CONSTANT_VALUE,CONSTANT_NAME)             \
  constant = PyInt_FromLong ( (long)CONSTANT_VALUE );             \
  PyDict_SetItemString ( dictionnary, CONSTANT_NAME, constant );  \
  Py_DECREF ( constant );




// -------------------------------------------------------------------
// Generic Method Header.

# define  GENERIC_METHOD_HEAD(SELF_TYPE,SELF_OBJECT,function)               \
    if ( self->ACCESS_OBJECT == NULL ) {                                    \
      PyErr_SetString ( ProxyError, "Attempt to call " function " on an unbound hurricane object" ); \
      return ( NULL );                                                      \
    }                                                                       \
    SELF_TYPE* SELF_OBJECT = dynamic_cast<SELF_TYPE*>(self->ACCESS_OBJECT); \
    if ( self->ACCESS_OBJECT == NULL ) {                                    \
      PyErr_SetString ( ProxyError, "Invalid dynamic_cast while calling " function "" ); \
      return ( NULL );                                                      \
    }




// -------------------------------------------------------------------
// Attribute Method Macro For Checking Bound.

# define  GetBoundStateAttribute(PY_FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self )                \
  {                                                                   \
    long  result = 1;                                                 \
    if ( self->ACCESS_OBJECT == NULL ) result = 0;                    \
                                                                      \
    return ( Py_BuildValue ("i",result) );                            \
  }




// -------------------------------------------------------------------
// Attribute Method Macro For Booleans.

# define  DirectGetBoolAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args )          \
  {                                                                             \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,"DirectGetBoolAttribute()")           \
    return ( Py_BuildValue ("i",cobject->FUNC_NAME()) );                        \
  }




// -------------------------------------------------------------------
// Attribute Method Macro For Long.

# define  DirectGetLongAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args )          \
  {                                                                             \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,"DirectGetLongAttribute()")           \
    return ( Py_BuildValue ("l",cobject->FUNC_NAME()) );                        \
  }


// -------------------------------------------------------------------
// Attribute Method Macro For Double.

# define  DirectGetDoubleAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args )          \
  {                                                                             \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,"DirectGetDoubleAttribute()")         \
    return ( Py_BuildValue ("d",cobject->FUNC_NAME()) );                        \
  }


// -------------------------------------------------------------------
// Attribute Method Macro For Long.

# define  DirectSetLongAttribute(PY_FUNC_NAME,FUNC_NAME,PY_FORMAT,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args ) \
  {                                                                    \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,"DirectSetLongAttribute()")  \
                                                                       \
    PyObject* arg0;                                                    \
    if ( ! PyArg_ParseTuple ( args, "O:" PY_FORMAT, &arg0 ) )          \
      return ( NULL );                                                 \
    cobject->FUNC_NAME ( PyInt_AsLong(arg0) );                         \
                                                                       \
    Py_RETURN_NONE;                                                    \
  }




// -------------------------------------------------------------------
// Attribute Macro For Deletion.

# define  DirectDestroyAttribute(PY_FUNC_NAME,PY_SELF_TYPE)             \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args ) \
  {                                                                    \
    if ( self->ACCESS_OBJECT != NULL ) {                               \
      delete self->ACCESS_OBJECT;                                      \
      self->ACCESS_OBJECT = NULL;                                      \
    }                                                                  \
                                                                       \
    Py_RETURN_NONE;                                                    \
  }




// -------------------------------------------------------------------
// Attribute Method For Deletion.

# define  DirectDeleteMethod(PY_FUNC_NAME,PY_SELF_TYPE)     \
  static void PY_FUNC_NAME ( PY_SELF_TYPE *self )           \
  {                                                         \
    trace << "PyHObject_DeAlloc(" << hex << self << ") "    \
          << self->ACCESS_OBJECT << endl;                   \
                                                            \
    if ( self->ACCESS_OBJECT ) delete self->ACCESS_OBJECT;  \
    PyObject_DEL ( self );                                  \
  }



// -------------------------------------------------------------------
// Attribute Method For Deletion.

# define  PlugDeleteMethod(PY_FUNC_NAME,PY_SELF_TYPE)            \
  static void PY_FUNC_NAME ( PY_SELF_TYPE *self )                \
  {                                                              \
    trace << "PyHObject_DeAlloc(" << hex << self << ") "         \
          << self->ACCESS_OBJECT << endl;                        \
                                                                 \
    PyObject_DEL ( self );                                       \
  }


  

// -------------------------------------------------------------------
// Attribute Method For getting Attributes.

# define  DirectGetAttrMethod(PY_FUNC_NAME,PY_OBJECT_METHODS)  \
  static PyObject* PY_FUNC_NAME ( PyObject* self, char* name ) \
  {                                                            \
    return ( Py_FindMethod(PY_OBJECT_METHODS,self,name) );     \
  }


// -------------------------------------------------------------------
// Attribute Method For void methods.
#define DirectVoidMethod(SELF_TYPE, SELF_OBJECT, FUNC_NAME)             \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME(Py##SELF_TYPE* self)     \
  {                                                                     \
      trace << "Py" #SELF_TYPE "_" #FUNC_NAME "()" << endl;             \
      HTRY                                                              \
      METHOD_HEAD(#SELF_TYPE "." #FUNC_NAME "()")                       \
      SELF_OBJECT->FUNC_NAME();                                         \
      HCATCH                                                            \
      Py_RETURN_NONE;                                                   \
  }
  

// -------------------------------------------------------------------
// Locator Attribute Method For Progress.

# define  LocatorProgressAttribute(SELF_TYPE)  \
  static PyObject* Py##SELF_TYPE##Locator_Progress ( Py##SELF_TYPE##Locator *self ) \
  {                                                     \
    trace << #SELF_TYPE "Locator.Progress()" << endl;   \
    METHOD_HEAD ( #SELF_TYPE "Locator.Progress()" )     \
                                                        \
    HTRY                                                \
    locator->Progress ();                               \
    HCATCH                                              \
                                                        \
    Py_RETURN_NONE;                                     \
  }


  

// -------------------------------------------------------------------
// Locator Attribute Method For getElement.

# define  LocatorGetElementAttribute(SELF_TYPE)         \
  static PyObject* Py##SELF_TYPE##Locator_getElement ( Py##SELF_TYPE##Locator *self ) \
  {                                                     \
    trace << #SELF_TYPE "Locator.getElement()" << endl; \
    METHOD_HEAD ( #SELF_TYPE "Locator.getElement()" )   \
                                                        \
    SELF_TYPE* element = NULL;                          \
                                                        \
    HTRY                                                \
    element = locator->getElement ();                   \
    HCATCH                                              \
                                                        \
    return Py##SELF_TYPE##_Link ( element );            \
  }


  

// -------------------------------------------------------------------
// Locator Attribute Method For getElement (element is PyEntity).

# define  LocatorGetElementEntityAttribute(SELF_TYPE)   \
  static PyObject* Py##SELF_TYPE##Locator_getElement ( Py##SELF_TYPE##Locator *self ) \
  {                                                     \
    trace << #SELF_TYPE "Locator.getElement()" << endl; \
    METHOD_HEAD ( #SELF_TYPE "Locator.getElement()" )   \
                                                        \
    PyObject* pyElement = NULL;                         \
                                                        \
    HTRY                                                \
    SELF_TYPE* element = locator->getElement ();        \
    if ( element == NULL )                              \
      Py_RETURN_NONE;                                   \
    pyElement = PyEntity_NEW ( element );               \
    HCATCH                                              \
                                                        \
    return pyElement;                                   \
  }


  

// -------------------------------------------------------------------
// Locator Attribute Method For getClone.

# define  LocatorGetCloneAttribute(SELF_TYPE)           \
  static PyObject* Py##SELF_TYPE##Locator_getClone ( Py##SELF_TYPE##Locator *self ) \
  {                                                     \
    trace << #SELF_TYPE "Locator.getClone()" << endl;   \
    METHOD_HEAD ( #SELF_TYPE "Locator.getClone()" )     \
                                                        \
    Py##SELF_TYPE##Locator* cloneLocator = NULL;        \
                                                        \
    HTRY                                                \
    cloneLocator = PyObject_NEW ( Py##SELF_TYPE##Locator, &PyType##SELF_TYPE##Locator ); \
    if (cloneLocator == NULL) { return NULL; }          \
                                                        \
    cloneLocator->_object = locator->getClone();        \
    HCATCH                                              \
                                                        \
    return ( (PyObject*)cloneLocator );                 \
  }




// -------------------------------------------------------------------
// Attribute Method For Repr.

# define  DirectReprMethod(PY_FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)          \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self )                   \
  {                                                                      \
    if ( self->ACCESS_OBJECT == NULL )                                   \
      return ( PyString_FromString("<PyObject unbound>") );              \
    SELF_TYPE* object = dynamic_cast<SELF_TYPE*>(self->ACCESS_OBJECT);   \
    if ( object == NULL )                                                \
      return ( PyString_FromString("<PyObject invalid dynamic-cast>") ); \
                                                                         \
    ostringstream repr;                                                  \
    repr << "[" << hex << self << "<->" << (void*)object << " " << getString(object) << "]"; \
                                                                         \
    return ( PyString_FromString(repr.str().c_str()) );                  \
  }




// -------------------------------------------------------------------
// Attribute Method For Str.

# define  DirectStrMethod(PY_FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)           \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self )                   \
  {                                                                      \
    if ( self->ACCESS_OBJECT == NULL )                                   \
      return ( PyString_FromString("<PyObject unbound>") );              \
    SELF_TYPE* object = dynamic_cast<SELF_TYPE*>(self->ACCESS_OBJECT);   \
    if ( object == NULL )                                                \
      return ( PyString_FromString("<PyObject invalid dynamic_cast>") ); \
                                                                         \
    return ( PyString_FromString(getString(object).c_str()) );           \
  }




// -------------------------------------------------------------------
// Attribute Method For Cmp.

# define  DirectCmpMethod(PY_FUNC_NAME,IS_PY_OBJECT,PY_SELF_TYPE)             \
  static int  PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* other )            \
  {                                                                           \
    if ( ! IS_PY_OBJECT(other) ) { return ( -1 ); }                           \
                                                                              \
    PY_SELF_TYPE* otherPyObject = (PY_SELF_TYPE *)other;                      \
    if ( self->ACCESS_OBJECT == otherPyObject->ACCESS_OBJECT ) return (  0 ); \
    if ( self->ACCESS_OBJECT <  otherPyObject->ACCESS_OBJECT ) return ( -1 ); \
                                                                              \
    return ( 1 );                                                             \
  }


// -------------------------------------------------------------------
// Attribute Method For Hash.

# define  DirectHashMethod(PY_FUNC_NAME,PY_SELF_TYPE)                         \
  static int  PY_FUNC_NAME ( PY_SELF_TYPE *self)                              \
  {                                                                           \
    return (long)self->ACCESS_OBJECT;                                         \
  }



// -------------------------------------------------------------------
// Attribute Macro For DBo Deletion.

# define  DBoDestroyAttribute(PY_FUNC_NAME,PY_SELF_TYPE)                                  \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self )                                   \
  {                                                                                      \
    HTRY                                                                                 \
    if ( self->ACCESS_OBJECT == NULL ) {                                                 \
      ostringstream  message;                                                            \
      message << "applying a Delete to a Python object with no HURRICANE object attached"; \
        PyErr_SetString ( ProxyError, message.str().c_str() );                           \
      return ( NULL );                                                                   \
    }                                                                                    \
    ProxyProperty* proxy = dynamic_cast<ProxyProperty*>                                  \
                           ( self->ACCESS_OBJECT->getProperty ( ProxyProperty::getPropertyName() ) ); \
    if (proxy == NULL) {                                                                 \
      ostringstream  message;                                                            \
      message << "Trying to Delete a Hurricane object of with no Proxy attached ";       \
      PyErr_SetString ( ProxyError, message.str().c_str() );                             \
      return ( NULL );                                                                   \
    }                                                                                    \
    self->ACCESS_OBJECT->destroy();                                                      \
    HCATCH                                                                               \
    Py_RETURN_NONE;                                                                      \
  }

// -------------------------------------------------------------------
// Attribute Macro For BDo Link/Creation.

# define  DBoLinkCreateMethod(PY_FUNC_NAME,PY_SELF_TYPE,PY_SELF_CLASS,SELF_TYPE) \
  PyObject* PY_FUNC_NAME ( SELF_TYPE* object )                                   \
  {                                                                              \
    if ( object == NULL )                                                        \
      Py_RETURN_NONE;                                                            \
    PY_SELF_TYPE* pyObject = NULL;                                               \
    HTRY                                                                         \
    ProxyProperty* proxy = dynamic_cast<ProxyProperty*>                          \
      ( object->getProperty ( ProxyProperty::getPropertyName() ) );              \
    if ( proxy == NULL ) {                                                       \
      pyObject = PyObject_NEW(PY_SELF_TYPE, &PY_SELF_CLASS);                     \
      if (pyObject == NULL) { return NULL; }                                     \
                                                                                 \
      proxy = ProxyProperty::create ( (void*)pyObject );                         \
      CHECK_OFFSET ( pyObject )                                                  \
                                                                                 \
      pyObject->ACCESS_OBJECT = object;                                          \
      object->put ( proxy );                                                     \
    } else {                                                                     \
      pyObject = (PY_SELF_TYPE*)proxy->getShadow ();                             \
      Py_INCREF ( ACCESS_CLASS(pyObject) );                                      \
    }                                                                            \
    HCATCH                                                                       \
                                                                                 \
    return ( (PyObject*)pyObject );                                              \
  }

// -------------------------------------------------------------------
// Attribute Method For BDo Deletion.

# define  DBoDeleteMethod(SELF_TYPE)                                     \
  static void Py##SELF_TYPE##_DeAlloc ( Py##SELF_TYPE *self )            \
  {                                                                      \
    trace << "PyDbObject_DeAlloc(" << hex << self << ") "                \
          << self->ACCESS_OBJECT << endl;                                \
                                                                         \
    if ( self->ACCESS_OBJECT != NULL ) {                                 \
        ProxyProperty* proxy = dynamic_cast<ProxyProperty*>              \
                               ( self->ACCESS_OBJECT->getProperty ( ProxyProperty::getPropertyName() ) ); \
        if (proxy == NULL) {                                             \
          ostringstream  message;                                        \
          message << "deleting a Python object with no Proxy attached "; \
          PyErr_SetString ( ProxyError, message.str().c_str() );         \
        }                                                                \
        self->ACCESS_OBJECT->remove ( proxy );                           \
    }                                                                    \
    PyObject_DEL ( self );                                               \
  }


  

// -------------------------------------------------------------------
// Initialisation Function for PyTypeObject Runtime Link.

#define PyTypeObjectLinkPyType(SELF_TYPE)                                \
  DirectReprMethod(Py##SELF_TYPE##_Repr, Py##SELF_TYPE,   SELF_TYPE)     \
  DirectStrMethod (Py##SELF_TYPE##_Str,  Py##SELF_TYPE,   SELF_TYPE)     \
  DirectCmpMethod (Py##SELF_TYPE##_Cmp,  IsPy##SELF_TYPE, Py##SELF_TYPE) \
  DirectHashMethod(Py##SELF_TYPE##_Hash, Py##SELF_TYPE)                  \
  extern void  Py##SELF_TYPE##_LinkPyType() {                            \
    trace << "Py" #SELF_TYPE "_LinkType()" << endl;                      \
                                                                         \
    PyType##SELF_TYPE.tp_dealloc = (destructor)Py##SELF_TYPE##_DeAlloc;  \
    PyType##SELF_TYPE.tp_compare = (cmpfunc)   Py##SELF_TYPE##_Cmp;      \
    PyType##SELF_TYPE.tp_repr    = (reprfunc)  Py##SELF_TYPE##_Repr;     \
    PyType##SELF_TYPE.tp_str     = (reprfunc)  Py##SELF_TYPE##_Str;      \
    PyType##SELF_TYPE.tp_hash    = (hashfunc)  Py##SELF_TYPE##_Hash;     \
    PyType##SELF_TYPE.tp_methods = Py##SELF_TYPE##_Methods;              \
  }

// -------------------------------------------------------------------
// Initialisation Function for PyTypeObject Runtime Link.
// Special method to add constructor new function
#define PyTypeObjectConstructor(SELF_TYPE)                               \
  extern void Py##SELF_TYPE##_Constructor() {                            \
      trace << "Py" #SELF_TYPE "_Constructor()" << endl;                 \
                                                                         \
      PyType##SELF_TYPE.tp_new = Py##SELF_TYPE##_new;                    \
  }


// Special Initialisation Function for Locator PyTypeObject Runtime Link.
#define LocatorPyTypeObjectLinkPyType(PY_SELF_TYPE, SELF_TYPE)                                             \
  DirectReprMethod(Py##PY_SELF_TYPE##Locator_Repr, Py##PY_SELF_TYPE##Locator,   Locator<SELF_TYPE>)        \
  DirectStrMethod (Py##PY_SELF_TYPE##Locator_Str,  Py##PY_SELF_TYPE##Locator,   Locator<SELF_TYPE>)        \
  DirectCmpMethod (Py##PY_SELF_TYPE##Locator_Cmp,  IsPy##PY_SELF_TYPE##Locator, Py##PY_SELF_TYPE##Locator) \
  extern void  Py##PY_SELF_TYPE##Locator_LinkPyType ()                                                     \
  {                                                                                                        \
    trace << "Py" #PY_SELF_TYPE "Locator_LinkType()" << endl;                                              \
                                                                                                           \
    PyType##PY_SELF_TYPE##Locator.tp_dealloc = (destructor)Py##PY_SELF_TYPE##Locator_DeAlloc;              \
    PyType##PY_SELF_TYPE##Locator.tp_compare = (cmpfunc)   Py##PY_SELF_TYPE##Locator_Cmp;                  \
    PyType##PY_SELF_TYPE##Locator.tp_repr    = (reprfunc)  Py##PY_SELF_TYPE##Locator_Repr;                 \
    PyType##PY_SELF_TYPE##Locator.tp_str     = (reprfunc)  Py##PY_SELF_TYPE##Locator_Str;                  \
    PyType##PY_SELF_TYPE##Locator.tp_methods = Py##PY_SELF_TYPE##Locator_Methods;                          \
  }

// -------------------------------------------------------------------
// CEngine Initialization
#define CEngineModuleInitialization(CENGINE)                                         \
  DL_EXPORT(void) init##CENGINE () {                                                 \
    trace << "init" #CENGINE "()" << endl;                                           \
    Py##CENGINE##_LinkPyType ();                                                     \
    PYTYPE_READY_SUB ( CENGINE, CEngine )                                            \
    __cs.AddType ( #CENGINE, &PyType##CENGINE, "<" #CENGINE ">", false, "cengine" ); \
    PyObject* module = Py_InitModule(#CENGINE, Py##CENGINE##Module_Methods);         \
    if ( module == NULL ) {                                                          \
      cerr << "[ERROR]\n"                                                            \
           << "  Failed to initialize " #CENGINE " module." << endl;                 \
    }                                                                                \
  }                

// -------------------------------------------------------------------
// Direct getCEngine() Method (no argument)
#define DirectGetCEngine(CENGINE)                                               \
  PyObject* Py##CENGINE##_get##CENGINE ( PyObject* module, PyObject* args )     \
  {                                                                             \
    PyObject* arg0;                                                             \
    CENGINE* cengine = NULL;                                                    \
    HTRY                                                                        \
    if (!ParseOneArg("get" #CENGINE, args, CELL_ARG, &arg0 ) ) return ( NULL ); \
    Cell* cell = PYCELL_O(arg0);                                                \
    cengine = dynamic_cast<CENGINE*>(getCEngine(cell, Name(#CENGINE)));         \
    if (!cengine) {                                                             \
        cengine = CENGINE::create (cell);                                       \
    }                                                                           \
    HCATCH                                                                      \
    return Py##CENGINE##_Link( cengine );                                       \
  }

// -------------------------------------------------------------------
// Object Definitions for inherited types
// root of the inheritance tree 

# define PyTypeRootObjectDefinitions(SELF_TYPE)                         \
  PyTypeObject  PyType##SELF_TYPE =                                     \
    { PyObject_HEAD_INIT(&PyType_Type)                                  \
       0                              /* ob_size.          */           \
    ,  #SELF_TYPE                     /* tp_name.          */           \
    ,  sizeof(Py##SELF_TYPE)          /* tp_basicsize.     */           \
    , 0                               /* tp_itemsize.      */           \
    /* methods. */                                                      \
    , 0                               /* tp_dealloc.       */           \
    , 0                               /* tp_print.         */           \
    , 0                               /* tp_getattr.       */           \
    , 0                               /* tp_setattr.       */           \
    , 0                               /* tp_compare.       */           \
    , 0                               /* tp_repr.          */           \
    , 0                               /* tp_as_number.     */           \
    , 0                               /* tp_as_sequence.   */           \
    , 0                               /* tp_as_mapping.    */           \
    , 0                               /* tp_hash.          */           \
    , 0                               /* tp_call.          */           \
    , 0                               /* tp_str            */           \
    , 0                               /* tp_getattro.      */           \
    , 0                               /* tp_setattro.      */           \
    , 0                               /* tp_as_buffer.     */           \
    , Py_TPFLAGS_DEFAULT                                                \
    | Py_TPFLAGS_BASETYPE             /* tp_flags.         */           \
    , 0                               /* tp_doc.           */           \
    , 0                               /* tp_traverse.      */           \
    , 0                               /* tp_clear.         */           \
    , 0                               /* tp_richcompare.   */           \
    , 0                               /* tp_weaklistoffset.*/           \
    , 0                               /* tp_iter.          */           \
    , 0                               /* tp_iternext.      */           \
    , 0                               /* tp_methods.       */           \
    , 0                               /* tp_members.       */           \
    , 0                               /* tp_getset.        */           \
    , 0                               /* tp_base.          */           \
    , 0                               /* tp_dict.          */           \
    , 0                               /* tp_descr_get.     */           \
    , 0                               /* tp_descr_set.     */           \
    , 0                               /* tp_dictoffset.    */           \
    , 0                               /* tp_init.          */           \
    , 0                               /* tp_alloc.         */           \
    , 0                               /* tp_new.           */           \
    , 0                               /* tp_free.          */           \
    , 0                               /* tp_is_gc.         */           \
  };


// -------------------------------------------------------------------
// Object Definitions for types with inheritance (from Inherited or Root)

# define PyTypeInheritedObjectDefinitions(SELF_TYPE, INHERITED_TYPE) \
  PyTypeObject  PyType##SELF_TYPE =                                     \
    { PyObject_HEAD_INIT(DEFERRED_ADDRESS(&PyType##INHERITED_TYPE))     \
       0                              /* ob_size.          */           \
    ,  #SELF_TYPE                     /* tp_name.          */           \
    ,  sizeof(Py##SELF_TYPE)          /* tp_basicsize.     */           \
    , 0                               /* tp_itemsize.      */           \
    /* methods. */                                                      \
    , 0                               /* tp_dealloc.       */           \
    , 0                               /* tp_print.         */           \
    , 0                               /* tp_getattr.       */           \
    , 0                               /* tp_setattr.       */           \
    , 0                               /* tp_compare.       */           \
    , 0                               /* tp_repr.          */           \
    , 0                               /* tp_as_number.     */           \
    , 0                               /* tp_as_sequence.   */           \
    , 0                               /* tp_as_mapping.    */           \
    , 0                               /* tp_hash.          */           \
    , 0                               /* tp_call.          */           \
    , 0                               /* tp_str            */           \
    , 0                               /* tp_getattro.      */           \
    , 0                               /* tp_setattro.      */           \
    , 0                               /* tp_as_buffer.     */           \
    , Py_TPFLAGS_DEFAULT                                                \
    | Py_TPFLAGS_BASETYPE             /* tp_flags.         */           \
    , 0                               /* tp_doc.           */           \
    , 0                               /* tp_traverse.      */           \
    , 0                               /* tp_clear.         */           \
    , 0                               /* tp_richcompare.   */           \
    , 0                               /* tp_weaklistoffset.*/           \
    , 0                               /* tp_iter.          */           \
    , 0                               /* tp_iternext.      */           \
    , 0                               /* tp_methods.       */           \
    , 0                               /* tp_members.       */           \
    , 0                               /* tp_getset.        */           \
    , DEFERRED_ADDRESS(&PyType##INHERITED_TYPE)                         \
                                      /* tp_base.          */           \
    , 0                               /* tp_dict.          */           \
    , 0                               /* tp_descr_get.     */           \
    , 0                               /* tp_descr_set.     */           \
    , 0                               /* tp_dictoffset.    */           \
    , 0                               /* tp_init.          */           \
    , 0                               /* tp_alloc.         */           \
    , 0                               /* tp_new.           */           \
    , 0                               /* tp_free.          */           \
    , 0                               /* tp_is_gc.         */           \
  };

// -------------------------------------------------------------------
//PyType_Ready Methods

# define  PYTYPE_READY(TYPE)                      \
  if ( PyType_Ready( &PyType##TYPE ) < 0 ) {      \
    cerr << "[ERROR]\n"                           \
         << "  Failed to initialize <Py" #TYPE ">." << endl; \
    return;                                       \
  }


# define  PYTYPE_READY_SUB(TYPE, TYPE_BASE)       \
  PyType##TYPE.tp_base = &PyType##TYPE_BASE;      \
  if ( PyType_Ready( &PyType##TYPE ) < 0 ) {      \
    cerr << "[ERROR]\n"                           \
         << "  Failed to initialize <Py" #TYPE ">." << endl; \
    return;                                       \
  }



// -------------------------------------------------------------------
// Hurricane Try / Catch.

# define   HTRY  try {

# define   HCATCH  \
    }                                                            \
    catch ( Error& e ) {                                         \
      PyErr_SetString ( HurricaneError, getString(e).c_str() );  \
      return ( NULL );                                           \
    }                                                            \
    catch ( Warning& w ) {                                       \
      PyErr_Warn ( HurricaneWarning, const_cast<char*>(getString(w).c_str()) );  \
    }

}  // End of extern "C".

}  // End of Isobar namespace.

# endif
