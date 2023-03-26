// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Authors     :                    Jean-Paul CHAPUT              |
// |                                      Damien DUPUIS              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyHurricane.h"              |
// +-----------------------------------------------------------------+


// Enable Python debugging.
// #define  DEBUG  1

#pragma  once
#include "Python.h"
#include <type_traits>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DbU.h"
#include "hurricane/isobar/ProxyProperty.h"


inline long  PyInt_AsLong ( PyObject* object )
{ return PyLong_AsLong(object); }


inline bool  PyString_Check ( PyObject* object )
{ return PyUnicode_Check(object); }


inline std::string  PyString_AsString ( PyObject* object )
{
  PyObject* pyBytes = PyUnicode_AsASCIIString( object );
  std::string s = PyBytes_AsString( pyBytes );
  Py_DECREF( pyBytes );
  return s;
}


inline  PyObject* PyString_FromString ( const char* s )
{ return PyUnicode_FromString( s ); }


namespace Isobar {


  using namespace std;
  using Hurricane::DbU;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DBo;


// -------------------------------------------------------------------
// Class  :  "::ConverterState".

  class ConverterState {

    public:
      struct ObjectType {
        static const char*   _inheritStop;
               char          _id    [11];
               char          _idBase[11];
               PyTypeObject* _pyType;
               const char*         _name;
               bool          _isPythonType;
               unsigned      _index;
  
               bool          PyEqual  ( PyTypeObject* pyType );
               ObjectType*   PyBase   ();
               ObjectType*   PyBase   ( PyTypeObject* pyType );
               char*         PyBaseId ( PyTypeObject* pyType );
      };

    typedef vector<ObjectType*> ObjectTypeVector;

    protected: string            _objectIds;
    protected: const char*       _function;
    protected: ObjectTypeVector  _types;
     

    public: ConverterState () : _objectIds("")
                              , _function("<NoFunction>")
                              , _types()
                              { }
    public: ~ConverterState ();

    public: ObjectType*             getObject     ( char* id );
    public: const char*             getObjectId   ( PyObject* object ) const;
    public: const char*             getObjectName ( string id ) const;
    public: string                  getObjectIds  () const { return ( _objectIds ); };
    public: const char*             getFunction   () const { return ( _function ); };
    public: int                     getSize       () const { return ( _objectIds.size() ); };
    public: const ObjectTypeVector& getTypes      () const { return ( _types ); };
    public: void                    addObject     ( PyObject* object ) { _objectIds += getObjectId ( object ); };
    public: void                    addId         ( char* id ) { _objectIds += ":"; _objectIds += id; };
    public: void                    setFunction   ( const char* function ) { _function = function; };
    public: void                    init          ( const char* function, const char* inheritStop="comp" );
    public: void                    addType       ( const char* id, PyTypeObject* pyType, const char* name, bool isPythonType, const char* idBase="" );
    public: static string           getObjectType ( string objectsTypes, unsigned n );
  };




// -------------------------------------------------------------------
// Isobar Global Functions.

  int    Converter        ( PyObject* object, void** pArg );
  bool   ParseOneArg      ( const char* function, PyObject* args, string format, PyObject** arg );
  bool   ParseTwoArg      ( const char* function, PyObject* args, string format, PyObject** arg0, PyObject** arg1 );
  bool   ParseThreeArg    ( const char* function, PyObject* args, string format, PyObject** arg0, PyObject** arg1, PyObject** arg2 );



// -------------------------------------------------------------------
// Isobar Global variables.

  extern ConverterState             __cs;
  extern int                        __objectOffset;


  inline int  PyAny_AsInt ( PyObject* object )
  {
    long value = 0;
    if (PyObject_IsInstance(object,(PyObject*)&PyLong_Type)) value = PyLong_AsLong( object );
    return (int)value;
  }


  inline uint32_t  PyAny_AsUInt32 ( PyObject* object )
  {
    int64_t value = 0;
    if (PyObject_IsInstance(object,(PyObject*)&PyLong_Type)) value = PyLong_AsLong( object );
    return (uint32_t)value;
  }


  template< typename T = DbU::Unit, typename enable_if< is_same<T,long>::value, T >::type = 0 >
  inline T  PyAny_AsLong ( PyObject* object )
  {
    T  value = 0;
    if (PyObject_IsInstance(object,(PyObject*)&PyLong_Type)) value = PyLong_AsLong( object );
    return value;
  }


  template< typename T = DbU::Unit, typename enable_if< is_same<T,long long>::value, T >::type = 0 >
  inline T  PyAny_AsLong ( PyObject* object )
  {
    T  value = 0;
    if (PyObject_IsInstance(object,(PyObject*)&PyLong_Type)) value = PyLong_AsLongLong( object );

  //if (value > numeric_limits<int>::max()) {
  //  throw Error( "PyAny_AsLong(): Suspiciously big int %s, db:%lli"
  //             , DbU::getValueString(value).c_str(), value
  //             );
  //}
    return value;
  }


  template< typename T = DbU::Unit, typename enable_if< is_same<T,long>::value, T >::type = 0 >
  inline PyObject* PyDbU_FromLong ( T unit ) { return PyLong_FromLong( unit ); }


  template< typename T = DbU::Unit, typename enable_if< is_same<T,long long>::value, T >::type = 0 >
  inline PyObject* PyDbU_FromLong ( T unit ) { return PyLong_FromLongLong( unit ); }


  template< typename T , typename enable_if<is_base_of<DBo,T>::value,int>::type = 0 >
  static int  getPyHash ( T* cppObject ) { return cppObject->getId(); }


  template< typename T , typename enable_if<!is_base_of<DBo,T>::value,int>::type = 0 >
  static int  getPyHash ( T* cppObject ) { return (long)cppObject; }


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
#define INTV_ARG                 ":intv"
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
#define INSTANCE_ARG             ":ent"
#define HOOK_ARG                 ":hook"
#define TRANS_ARG                ":transfo"
#define STRING_ARG               ":string"
#define STRING_CELL_ARG          ":string:ent"
#define NET_ARG                  ":ent"
#define NET_LAYER_INT_ARG        ":ent:layer:int"
#define NET_LAYER_INTS2_ARG      ":ent:layer:int:int"
#define NET_LAYER_INTS3_ARG      ":ent:layer:int:int:int"
#define NET_LAYER_INTS4_ARG      ":ent:layer:int:int:int:int"
#define COMP_ARG                 ":comp"
#define COMP_LAYER_INTS2_ARG     ":comp:layer:int:int"
#define COMP_LAYER_INTS3_ARG     ":comp:layer:int:int:int"
#define COMP_LAYER_INTS4_ARG     ":comp:layer:int:int:int:int"
#define COMPS2_LAYER_INT_ARG     ":comp:comp:layer:int"
#define COMPS2_LAYER_INTS2_ARG   ":comp:comp:layer:int:int"
#define COMPS2_LAYER_INTS3_ARG   ":comp:comp:layer:int:int:int"
#define COMPS2_LAYER_INTS4_ARG   ":comp:comp:layer:int:int:int:int"
#define CELL_BOX_ARG             ":ent:box"
#define CELL_INT_ARG             ":ent:int"
#define INST_ARG                 ":ent"
#define INST_PATH_ARG            ":ent:path"
#define PATH_INST_ARG            ":path:ent"
#define CELL_STRING_ARG          ":ent:string"
#define ENT_ARG                  ":ent"
#define ENT_PATH_ARG             ":ent:path"
#define COMP_PATH_ARG            ":comp:path"

// -------------------------------------------------------------------
// Miscellaneous.
#define DEFERRED_ADDRESS(ADDR)  0


// This macro must be redefined in derived classes.
// Example : _baseOject._object
#define  ACCESS_OBJECT            _object
#define  ACCESS_CLASS(_pyObject)  _pyObject


#define  LOAD_CONSTANT(CONSTANT_VALUE,CONSTANT_NAME)             \
 constant = PyLong_FromLong ( (long)CONSTANT_VALUE );            \
 PyDict_SetItemString ( dictionnary, CONSTANT_NAME, constant );  \
 Py_DECREF ( constant );

#define  LoadObjectConstant(DICTIONARY,CONSTANT_VALUE,CONSTANT_NAME)  \
 constant = PyLong_FromLong ( (long)CONSTANT_VALUE );                 \
 PyDict_SetItemString ( DICTIONARY, CONSTANT_NAME, constant );        \
 Py_DECREF ( constant );




// -------------------------------------------------------------------
// Generic Method Header.

#define GENERIC_METHOD_HEAD(SELF_TYPE,SELF_OBJECT,function)                \
    if ( self->ACCESS_OBJECT == NULL ) {                                   \
      PyErr_SetString( ProxyError, "Attempt to call " function " on an unbound hurricane object" ); \
      return NULL;                                                         \
    }                                                                      \
    SELF_TYPE* SELF_OBJECT = dynamic_cast<SELF_TYPE*>(self->ACCESS_OBJECT);\
    if ( SELF_OBJECT == NULL ) {                                           \
      PyErr_SetString( ProxyError, "Invalid dynamic_cast<> while calling " function "" ); \
      return NULL;                                                         \
    }




// -------------------------------------------------------------------
// Attribute Method Macro For Checking Bound.

#define  GetBoundStateAttribute(PY_FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self )               \
  {                                                                  \
    long  result = 1;                                                \
    if ( self->ACCESS_OBJECT == NULL ) result = 0;                   \
                                                                     \
    return ( PyLong_FromLong(result) );                              \
  }


# define  predicateFromLayer(FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)                            \
  static PyObject* PY_SELF_TYPE##_##FUNC_NAME ( PY_SELF_TYPE* self, PyObject* args )      \
  {                                                                                       \
    cdebug_log(20,0) << #PY_SELF_TYPE "_" #FUNC_NAME "()" << endl;                        \
                                                                                          \
    HTRY                                                                                  \
      GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#SELF_TYPE"."#FUNC_NAME"()")                  \
      PyObject* arg0   = NULL;                                                            \
      bool      rvalue = false;                                                           \
      __cs.init (#SELF_TYPE"."#FUNC_NAME"()");                                            \
      if (PyArg_ParseTuple( args, "O&:"#SELF_TYPE"."#FUNC_NAME"()", Converter, &arg0)) {  \
        if (__cs.getObjectIds() == ":layer")                                              \
          rvalue = cobject->FUNC_NAME( PYLAYER_O(arg0) );                                 \
        else {                                                                            \
          PyErr_SetString ( ConstructorError                                              \
                          , #SELF_TYPE"."#FUNC_NAME"(): The sole parameter is not a Layer." );  \
          return NULL;                                                                    \
        }                                                                                 \
      } else {                                                                            \
        PyErr_SetString ( ConstructorError                                                \
                        , #SELF_TYPE"."#FUNC_NAME"(): Takes only *one* Layer parameter. " ); \
        return NULL;                                                                      \
      }                                                                                   \
      if (rvalue) Py_RETURN_TRUE;                                                         \
    HCATCH                                                                                \
                                                                                          \
    Py_RETURN_FALSE;                                                                      \
  }


// -------------------------------------------------------------------
// Attribute Method Macro For Booleans.

# define  DirectGetBoolAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args )          \
  {                                                                             \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")                       \
    if (cobject->FUNC_NAME())                                                   \
      Py_RETURN_TRUE;                                                           \
    Py_RETURN_FALSE;                                                            \
  }




// -------------------------------------------------------------------
// Attribute Method Macro For Predicates.

# define  DirectIsAFromCStringAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args )          \
  {                                                                             \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")                       \
    HTRY                                                                        \
    char* value = NULL;                                                         \
    if ( !PyArg_ParseTuple(args, "s:", &value) )                                \
      return NULL;                                                              \
    if (cobject->FUNC_NAME(value))                                              \
      Py_RETURN_TRUE;                                                           \
    HCATCH                                                                      \
    Py_RETURN_FALSE;                                                            \
  }




// -------------------------------------------------------------------
// Attribute Method Macro For Int.

# define  DirectGetIntAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)  \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args )          \
  {                                                                             \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")                       \
    return Py_BuildValue("i", cobject->FUNC_NAME());                            \
  }


// -------------------------------------------------------------------
// Attribute Method Macro For Unsigned Int.

# define  DirectGetUIntAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args )          \
  {                                                                             \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")                       \
    return Py_BuildValue ("I",cobject->FUNC_NAME());                            \
  }


// -------------------------------------------------------------------
// Attribute Method Macro For Long.

# define  DirectGetLongAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args )          \
  {                                                                             \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")                       \
    return Isobar::PyDbU_FromLong(cobject->FUNC_NAME());                        \
  }


// -------------------------------------------------------------------
// Attribute Method Macro For Double.

# define  DirectGetDoubleAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args )            \
  {                                                                               \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")                         \
    return Py_BuildValue ("d",cobject->FUNC_NAME());                              \
  }


// -------------------------------------------------------------------
// Attribute Method Macro For String.

# define  DirectGetStringAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self )                            \
  {                                                                               \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")                         \
    return Py_BuildValue ("s",cobject->FUNC_NAME().c_str());                      \
  }


// -------------------------------------------------------------------
// Attribute Method Macro For Name.

# define  DirectGetNameAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self )                          \
  {                                                                             \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")                       \
    return Py_BuildValue ("s",getString(cobject->FUNC_NAME()).c_str());         \
  }


# define  accessorLayerFromVoid(FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)                    \
  static PyObject* PY_SELF_TYPE##_##FUNC_NAME ( PY_SELF_TYPE* self )                 \
  {                                                                                  \
    cdebug_log(20,0) << #PY_SELF_TYPE "_" #FUNC_NAME "()" << endl;                     \
                                                                                     \
    Layer* rlayer = NULL;                                                            \
                                                                                     \
    HTRY                                                                             \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#SELF_TYPE"."#FUNC_NAME"()")               \
    rlayer = const_cast<Layer*>( dynamic_cast<const Layer*>(cobject->FUNC_NAME()) ); \
    HCATCH                                                                           \
                                                                                     \
    if (rlayer == NULL) Py_RETURN_NONE;                                              \
    return PyLayer_LinkDerived(rlayer);                                              \
  }


# define  accessorAnyLayerFromName(FUNC_NAME,PY_SELF_TYPE,SELF_TYPE,LAYER_TYPE)         \
  static PyObject* PY_SELF_TYPE##_##FUNC_NAME ( PY_SELF_TYPE* self, PyObject* args )    \
  {                                                                                     \
    cdebug_log(20,0) << #PY_SELF_TYPE "_" #FUNC_NAME "()" << endl;                        \
                                                                                        \
    LAYER_TYPE * rlayer = NULL;                                                         \
                                                                                        \
    HTRY                                                                                \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#SELF_TYPE"."#FUNC_NAME"()")                  \
                                                                                        \
    char* name = NULL;                                                                  \
                                                                                        \
    if (PyArg_ParseTuple( args, "s:"#SELF_TYPE"."#FUNC_NAME"()", &name)) {              \
      rlayer = const_cast< LAYER_TYPE *>( cobject->FUNC_NAME(Name(name)) );             \
    } else {                                                                            \
      PyErr_SetString ( ConstructorError                                                \
                      , "Invalid number of parameters passed to "#SELF_TYPE"."#FUNC_NAME"()." );  \
      return NULL;                                                                      \
    }                                                                                   \
    HCATCH                                                                              \
                                                                                        \
    if (rlayer == NULL) Py_RETURN_NONE;                                                 \
    return Py##LAYER_TYPE##_Link(rlayer);                                               \
  }


# define  accessorLayerFromLayerOptBool(FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)               \
  static PyObject* PY_SELF_TYPE##_##FUNC_NAME ( PY_SELF_TYPE* self, PyObject* args )    \
  {                                                                                     \
    cdebug_log(20,0) << #PY_SELF_TYPE "_" #FUNC_NAME "()" << endl;                        \
                                                                                        \
    Layer* rlayer = NULL;                                                               \
                                                                                        \
    HTRY                                                                                \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#SELF_TYPE"."#FUNC_NAME"()")                  \
                                                                                        \
    PyObject* arg0        = NULL;                                                       \
    PyObject* arg1        = NULL;                                                       \
    bool      useSymbolic = true;                                                       \
                                                                                        \
    if (PyArg_ParseTuple( args, "O|O:"#SELF_TYPE"."#FUNC_NAME"()", &arg0, &arg1)) {     \
      if (not IsPyLayer(arg0)) {                                                        \
        PyErr_SetString ( ConstructorError                                              \
                        , #SELF_TYPE"."#FUNC_NAME"(): First argument is not of Layer type." );  \
        return NULL;                                                                    \
      }                                                                                 \
      if (arg1 != NULL) {                                                               \
        useSymbolic = PyObject_IsTrue(arg1);                                            \
      }                                                                                 \
    } else {                                                                            \
      PyErr_SetString ( ConstructorError                                                \
                      , "Invalid number of parameters passed to "#SELF_TYPE"."#FUNC_NAME"()." );  \
      return NULL;                                                                      \
    }                                                                                   \
    rlayer = const_cast<Layer*>(cobject->FUNC_NAME( PYLAYER_O(arg0), useSymbolic) );    \
    HCATCH                                                                              \
                                                                                        \
    if (rlayer == NULL) Py_RETURN_NONE;                                                 \
    return PyLayer_LinkDerived(rlayer);                                                 \
  }


# define  accessorLayerFromLayerLayer(FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)                  \
  static PyObject* PY_SELF_TYPE##_##FUNC_NAME ( PY_SELF_TYPE* self, PyObject* args )     \
  {                                                                                      \
    cdebug_log(20,0) << #PY_SELF_TYPE "_" #FUNC_NAME "()" << endl;                         \
                                                                                         \
    Layer* rlayer = NULL;                                                                \
                                                                                         \
    HTRY                                                                                 \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#SELF_TYPE"."#FUNC_NAME"()")                   \
                                                                                         \
    PyObject* arg0       = NULL;                                                         \
    PyObject* arg1       = NULL;                                                         \
                                                                                         \
    if (PyArg_ParseTuple( args, "OO:"#SELF_TYPE"."#FUNC_NAME"()", &arg0, &arg1)) {       \
      if (not IsPyLayer(arg0) or not IsPyLayer(arg1)) {                                  \
        PyErr_SetString ( ConstructorError                                               \
                        , #SELF_TYPE"."#FUNC_NAME"(): First or second argument is not of Layer type." );  \
        return NULL;                                                                     \
      }                                                                                  \
    } else {                                                                             \
      PyErr_SetString ( ConstructorError                                                 \
                      , "Invalid number of parameters passed to "#SELF_TYPE"."#FUNC_NAME"()." );  \
      return NULL;                                                                       \
    }                                                                                    \
    rlayer = const_cast<Layer*>(cobject->FUNC_NAME( PYLAYER_O(arg0), PYLAYER_O(arg1)) ); \
    HCATCH                                                                               \
                                                                                         \
    if (rlayer == NULL) Py_RETURN_NONE;                                                  \
    return PyLayer_LinkDerived(rlayer);                                                  \
  }


# define  accessorLayerFromLayerLayerOptBool(FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)           \
  static PyObject* PY_SELF_TYPE##_##FUNC_NAME ( PY_SELF_TYPE* self, PyObject* args )     \
  {                                                                                      \
    cdebug_log(20,0) << #PY_SELF_TYPE "_" #FUNC_NAME "()" << endl;                       \
                                                                                         \
    Layer* rlayer = NULL;                                                                \
                                                                                         \
    HTRY                                                                                 \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#SELF_TYPE"."#FUNC_NAME"()")                   \
                                                                                         \
    PyObject* arg0        = NULL;                                                        \
    PyObject* arg1        = NULL;                                                        \
    PyObject* arg2        = NULL;                                                        \
    bool      useSymbolic = true;                                                        \
                                                                                         \
    if (PyArg_ParseTuple( args, "OO|O:"#SELF_TYPE"."#FUNC_NAME"()", &arg0, &arg1, &arg2)) { \
      if (not IsPyLayer(arg0) or not IsPyLayer(arg1)) {                                  \
        PyErr_SetString ( ConstructorError                                               \
                        , #SELF_TYPE"."#FUNC_NAME"(): First or second argument is not of Layer type." );  \
        return NULL;                                                                     \
      }                                                                                  \
      if (arg2 != NULL) {                                                                \
        useSymbolic = PyObject_IsTrue(arg2);                                             \
      }                                                                                  \
    } else {                                                                             \
      PyErr_SetString ( ConstructorError                                                 \
                      , "Invalid number of parameters passed to "#SELF_TYPE"."#FUNC_NAME"()." );  \
      return NULL;                                                                       \
    }                                                                                    \
    rlayer = const_cast<Layer*>(cobject->FUNC_NAME( PYLAYER_O(arg0), PYLAYER_O(arg1), useSymbolic ) ); \
    HCATCH                                                                               \
                                                                                         \
    if (rlayer == NULL) Py_RETURN_NONE;                                                  \
    return PyLayer_LinkDerived(rlayer);                                                  \
  }


# define  accessorLayerFromInt(FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)                         \
  static PyObject* PY_SELF_TYPE##_##FUNC_NAME ( PY_SELF_TYPE* self, PyObject* args )     \
  {                                                                                      \
    cdebug_log(20,0) << #PY_SELF_TYPE "_" #FUNC_NAME "()" << endl;                         \
                                                                                         \
    Layer* rlayer = NULL;                                                                \
    int    value  = 0;                                                                   \
                                                                                         \
    HTRY                                                                                 \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#SELF_TYPE"."#FUNC_NAME"()")                   \
                                                                                         \
    if (PyArg_ParseTuple( args, "i:"#SELF_TYPE"."#FUNC_NAME"()", &value)) {              \
      rlayer = const_cast<Layer*>(cobject->FUNC_NAME(value) );                           \
    } else {                                                                             \
      PyErr_SetString ( ConstructorError                                                 \
                      , "Invalid number of parameters passed to "#SELF_TYPE"."#FUNC_NAME"()." );  \
      return NULL;                                                                       \
    }                                                                                    \
    HCATCH                                                                               \
                                                                                         \
    if (rlayer == NULL) Py_RETURN_NONE;                                                  \
    return PyLayer_LinkDerived(rlayer);                                                  \
  }


# define  updatorFromBasicLayer(FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)                        \
  static PyObject* PY_SELF_TYPE##_##FUNC_NAME ( PY_SELF_TYPE* self, PyObject* args )     \
  {                                                                                      \
    cdebug_log(20,0) << #PY_SELF_TYPE "_" #FUNC_NAME "()" << endl;                         \
                                                                                         \
    HTRY                                                                                 \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#SELF_TYPE"."#FUNC_NAME"()")                   \
                                                                                         \
    PyObject* pyBasicLayer = NULL;                                                       \
                                                                                         \
    if (PyArg_ParseTuple( args, "O:"#SELF_TYPE"."#FUNC_NAME"()", &pyBasicLayer)) {       \
      BasicLayer* layer = PYBASICLAYER_O(pyBasicLayer);                                  \
      if (layer == NULL) {                                                               \
        PyErr_SetString ( ConstructorError                                               \
                        , #SELF_TYPE"."#FUNC_NAME"(): First parameter is not of BasicLayer type" ); \
        return NULL;                                                                     \
      }                                                                                  \
      cobject->FUNC_NAME( layer );                                                       \
    } else {                                                                             \
      PyErr_SetString ( ConstructorError                                                 \
                      , #SELF_TYPE"."#FUNC_NAME"(): Bad parameters types or numbers." ); \
      return NULL;                                                                       \
    }                                                                                    \
    HCATCH                                                                               \
                                                                                         \
    Py_RETURN_NONE;                                                                      \
  }


#define  accessorHook(FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)                        \
  static PyObject*  PY_SELF_TYPE##_##FUNC_NAME( PY_SELF_TYPE *self )           \
  {                                                                            \
    cdebug_log(20,0) << #PY_SELF_TYPE "_" #FUNC_NAME "()" << endl;               \
                                                                               \
    PyHook* pyHook = PyObject_NEW( PyHook, &PyTypeHook );                      \
    if (pyHook == NULL) return NULL;                                           \
                                                                               \
    HTRY                                                                       \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#SELF_TYPE"."#FUNC_NAME"()")         \
    pyHook->_object = cobject->FUNC_NAME();                                    \
    HCATCH                                                                     \
                                                                               \
    return (PyObject*)pyHook;                                                  \
  }                                                                            \


// -------------------------------------------------------------------
// Attribute Method Macro For Booleans.

#define  DirectSetBoolAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args )          \
  {                                                                             \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME "()")                      \
                                                                                \
    HTRY                                                                        \
    PyObject* arg0;                                                             \
    if (not PyArg_ParseTuple( args, "O:" #FUNC_NAME, &arg0 ) or not PyBool_Check(arg0) ) {  \
      PyErr_SetString(ConstructorError, #SELF_TYPE"."#FUNC_NAME"(): Argument is not a boolean."); \
      return NULL;                                                              \
    }                                                                           \
                                                                                \
    (PyObject_IsTrue(arg0)) ? cobject->FUNC_NAME (true) : cobject->FUNC_NAME (false); \
    HCATCH                                                                      \
                                                                                \
    Py_RETURN_NONE;                                                             \
  }


// -------------------------------------------------------------------
// Attribute Method Macro For Int.

#define  DirectSetIntAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args ) \
  {                                                                    \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")              \
                                                                       \
    HTRY                                                               \
    PyObject* arg0;                                                    \
    if (not PyArg_ParseTuple ( args, "O:" #SELF_TYPE"."#FUNC_NAME"()", &arg0 ) ) \
      return ( NULL );                                                 \
    cobject->FUNC_NAME ( Isobar::PyAny_AsInt(arg0) );                  \
    HCATCH                                                             \
                                                                       \
    Py_RETURN_NONE;                                                    \
  }


// -------------------------------------------------------------------
// Attribute Method Macro For uint32_t.

#define  DirectSetUInt32Attribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args ) \
  {                                                                    \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")              \
                                                                       \
    HTRY                                                               \
    PyObject* arg0;                                                    \
    if (not PyArg_ParseTuple ( args, "O:" #SELF_TYPE"."#FUNC_NAME"()", &arg0 ) ) \
      return ( NULL );                                                 \
    cobject->FUNC_NAME ( Isobar::PyAny_AsUInt32(arg0) );               \
    HCATCH                                                             \
                                                                       \
    Py_RETURN_NONE;                                                    \
  }

// -------------------------------------------------------------------
// Attribute Method Macro For Long.

#define  DirectSetLongAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args ) \
  {                                                                    \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")              \
                                                                       \
    HTRY                                                               \
    PyObject* arg0;                                                    \
    if ( ! PyArg_ParseTuple ( args, "O:" #SELF_TYPE"."#FUNC_NAME"()", &arg0 ) ) \
      return ( NULL );                                                 \
    cobject->FUNC_NAME ( Isobar::PyAny_AsLong(arg0) );                 \
    HCATCH                                                             \
                                                                       \
    Py_RETURN_NONE;                                                    \
  }

  
// -------------------------------------------------------------------
// Attribute Method Macro For Double.

#define  DirectSetDoubleAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args )   \
  {                                                                      \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")                \
                                                                         \
    HTRY                                                                 \
    PyObject* arg0;                                                      \
    if ( ! PyArg_ParseTuple ( args, "O:" #SELF_TYPE"."#FUNC_NAME"()", &arg0 ) ) \
      return ( NULL );                                                   \
    cobject->FUNC_NAME ( PyFloat_AsDouble(arg0) );                       \
    HCATCH                                                               \
                                                                         \
    Py_RETURN_NONE;                                                      \
  }


// -------------------------------------------------------------------
// Attribute Method Macro For C String (char*).

#define  DirectSetCStringAttribute(PY_FUNC_NAME,FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* args )    \
  {                                                                       \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#FUNC_NAME"()")                 \
                                                                          \
    HTRY                                                                  \
    char* value = NULL;                                                   \
    if ( !PyArg_ParseTuple(args, "s:" #SELF_TYPE"."#FUNC_NAME"()", &value) ) \
      return NULL;                                                        \
    cobject->FUNC_NAME ( value );                                         \
    HCATCH                                                                \
                                                                          \
    Py_RETURN_NONE;                                                       \
  }


// -------------------------------------------------------------------
// Attribute Method Macro For Names.

#define GetNameMethod(SELF_TYPE, SELF)                                 \
  static PyObject* Py##SELF_TYPE##_getName(Py##SELF_TYPE* self) {      \
    cdebug_log(20,0) << "Py"#SELF_TYPE"_getName()" << endl;              \
    HTRY                                                               \
    METHOD_HEAD (#SELF_TYPE".getName()")                               \
    return PyUnicode_FromString(getString(SELF->getName()).c_str());    \
    HCATCH                                                             \
    return NULL;                                                       \
  }

#define SetNameMethod(SELF_TYPE, SELF)                                                 \
  static PyObject* Py##SELF_TYPE##_setName(Py##SELF_TYPE* self, PyObject* args) {      \
    cdebug_log(20,0) << "Py"#SELF_TYPE"_setName()" << endl;                              \
    HTRY                                                                               \
    METHOD_HEAD (#SELF_TYPE".setName()")                                               \
    char* name = NULL;                                                                 \
    if (PyArg_ParseTuple(args,"s:"#SELF_TYPE".setName", &name)) {                      \
        SELF->setName(Name(name));                                                     \
    } else {                                                                           \
        PyErr_SetString(ConstructorError, "invalid number of parameters for "#SELF_TYPE".setName."); \
        return NULL;                                                                                 \
    }                                                                                                \
    HCATCH                                                                                           \
    Py_RETURN_NONE;                                                                                  \
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

#define  DirectDeleteMethod(PY_FUNC_NAME, PY_SELF_TYPE)      \
  static void PY_FUNC_NAME ( PY_SELF_TYPE *self )            \
  {                                                          \
    cdebug_log(20,0) << #PY_SELF_TYPE"_DeAlloc(" << (void*)self << ") " \
          << (void*)(self->ACCESS_OBJECT)                    \
          << ":" << self->ACCESS_OBJECT << endl;             \
                                                             \
    if ( self->ACCESS_OBJECT ) {                             \
      cdebug_log(20,0) << "C++ object := "                   \
              << &(self->ACCESS_OBJECT) << endl;             \
        delete self->ACCESS_OBJECT;                          \
    }                                                        \
    PyObject_DEL ( self );                                   \
  }



// -------------------------------------------------------------------
// Attribute Method For Deletion.

#define PlugDeleteMethod(PY_FUNC_NAME,PY_SELF_TYPE)               \
  static void PY_FUNC_NAME ( PY_SELF_TYPE *self )                 \
  {                                                               \
    cdebug_log(20,0) << "PyHObject_DeAlloc(" << (void*)self << ") " \
          << self->ACCESS_OBJECT << endl;                         \
                                                                  \
    PyObject_DEL ( self );                                        \
  }


  

// -------------------------------------------------------------------
// Attribute Method For getting Attributes.

#define DirectGetAttrMethod(PY_FUNC_NAME,PY_OBJECT_METHODS)    \
  static PyObject* PY_FUNC_NAME ( PyObject* self, char* name ) \
  {                                                            \
    return ( Py_FindMethod(PY_OBJECT_METHODS,self,name) );     \
  }


// -------------------------------------------------------------------
// Attribute Method For void methods.

#define DirectVoidMethod(SELF_TYPE, SELF_OBJECT, FUNC_NAME)             \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME(Py##SELF_TYPE* self)     \
  {                                                                     \
    cdebug_log(20,0) << "Py" #SELF_TYPE "_" #FUNC_NAME "()" << endl;    \
    HTRY                                                                \
      METHOD_HEAD(#SELF_TYPE "." #FUNC_NAME "()")                       \
      SELF_OBJECT->FUNC_NAME();                                         \
    HCATCH                                                              \
    Py_RETURN_NONE;                                                     \
  }
  

// -------------------------------------------------------------------
// Collection and Locator macros

#define CollectionMethods(TYPE)                                                          \
  static PyObject* GetLocator(Py##TYPE##Collection* pyCollection)                        \
  {                                                                                      \
      Py##TYPE##CollectionLocator* pyLocator =                                           \
          PyObject_New(Py##TYPE##CollectionLocator, &PyType##TYPE##CollectionLocator);   \
      if (pyLocator == NULL) return NULL;                                                \
                                                                                         \
      pyLocator->_collection = pyCollection;                                             \
      pyLocator->_object     = pyCollection->_object->getLocator();                      \
      Py_INCREF(pyCollection);                                                           \
      return (PyObject*)pyLocator;                                                       \
  }                                                                                      \
                                                                                         \
  static void Py##TYPE##CollectionLocatorDeAlloc(Py##TYPE##CollectionLocator* pyLocator) \
  {                                                                                      \
      Py_XDECREF(pyLocator->_collection);                                                \
      if (pyLocator->_object) delete pyLocator->_object;                                 \
      PyObject_Del(pyLocator);                                                           \
  }                                                                                      \
                                                                                         \
  extern void Py##TYPE##Collection_LinkPyType ()                                         \
  {                                                                                      \
    cdebug_log(20,0) << "Py"#TYPE"Collection_LinkType()" << endl;                        \
    PyType##TYPE##Collection.tp_iter            = (getiterfunc)GetLocator;               \
    PyType##TYPE##Collection.tp_dealloc         = (destructor)Py##TYPE##Collection_DeAlloc;       \
    PyType##TYPE##CollectionLocator.tp_dealloc  = (destructor)Py##TYPE##CollectionLocatorDeAlloc; \
    PyType##TYPE##CollectionLocator.tp_iter     = PyObject_SelfIter;                     \
    PyType##TYPE##CollectionLocator.tp_iternext = (iternextfunc)Py##TYPE##LocatorNext;   \
  }

#define LocatorNextMethod(TYPE)                                                  \
  static PyObject* Py##TYPE##LocatorNext(Py##TYPE##CollectionLocator* pyLocator) \
  {                                                                              \
      Locator<TYPE*>* locator = pyLocator->_object;                              \
      HTRY                                                                       \
      if (locator and locator->isValid()) {                                      \
          TYPE* object = locator->getElement();                                  \
          locator->progress();                                                   \
          return Py##TYPE##_Link(object);                                        \
      }                                                                          \
      HCATCH                                                                     \
      return NULL;                                                               \
  }

#define NolinkLocatorNextMethod(TYPE)                                            \
  static PyObject* Py##TYPE##LocatorNext(Py##TYPE##CollectionLocator* pyLocator) \
  {                                                                              \
      Locator<TYPE*>* locator = pyLocator->_object;                              \
      HTRY                                                                       \
      if (locator and locator->isValid()) {                                      \
          Py##TYPE* pyObject = PyObject_NEW( Py##TYPE, &PyType##TYPE );          \
          if (pyObject == NULL) return NULL;                                     \
          pyObject->_object = locator->getElement();                             \
          locator->progress();                                                   \
          return (PyObject*)pyObject;                                            \
      }                                                                          \
      HCATCH                                                                     \
      return NULL;                                                               \
  }


#define EntityLocatorNextMethod(TYPE)                                   \
  static PyObject* Py##TYPE##LocatorNext(Py##TYPE##CollectionLocator* pyLocator) {  \
      Locator<TYPE*>* locator = pyLocator->_object;                     \
      HTRY                                                              \
      if (locator and locator->isValid()) {                             \
          TYPE* object = locator->getElement();                         \
          locator->progress();                                          \
          return PyEntity_NEW(object);                                  \
      }                                                                 \
      HCATCH                                                            \
      return NULL;                                                      \
  }


# define  accessorCollectionFromVoid(FUNC_NAME,PY_SELF_TYPE,SELF_TYPE,COLL_TYPE) \
  static PyObject* PY_SELF_TYPE##_##FUNC_NAME ( PY_SELF_TYPE* self )             \
  {                                                                              \
    cdebug_log(20,0) << #PY_SELF_TYPE "_" #FUNC_NAME "()" << endl;                 \
                                                                                 \
    Py##COLL_TYPE##Collection* pyObjects = NULL;                                 \
                                                                                 \
    HTRY                                                                         \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#SELF_TYPE"."#FUNC_NAME"()")           \
    COLL_TYPE##s* objects = new COLL_TYPE##s(cobject->FUNC_NAME());              \
    pyObjects = PyObject_NEW(Py##COLL_TYPE##Collection,&PyType##COLL_TYPE##Collection); \
    if (pyObjects == NULL) return NULL;                                          \
    pyObjects->_object = objects;                                                \
    HCATCH                                                                       \
                                                                                 \
    return (PyObject*)pyObjects;                                                 \
  }
  

// -------------------------------------------------------------------
// Vector<Data*> macros

#define declareVectorObject(TYPE)  \
  typedef struct {                                      \
      PyObject_HEAD                                     \
      const std::vector<TYPE*>* _container;             \
  } Py##TYPE##Vector;                                   \
                                                        \
  typedef struct {                                      \
      PyObject_HEAD                                     \
      Py##TYPE##Vector*                   _pyContainer; \
      std::vector<TYPE*>::const_iterator  _iterator;    \
  } Py##TYPE##VectorIterator;                           \
                                                        \
  extern PyTypeObject PyType##TYPE##Vector;             \
  extern PyTypeObject PyType##TYPE##VectorIterator;     \
                                                        \
  extern void Py##TYPE##Vector_LinkPyType();


#define VectorMethods(TYPE)  \
  static PyObject* Py##TYPE##Vector_GetIterator(Py##TYPE##Vector* pyVector)            \
  {                                                                                    \
      Py##TYPE##VectorIterator* pyIterator =                                           \
          PyObject_New(Py##TYPE##VectorIterator, &PyType##TYPE##VectorIterator);       \
      if (pyIterator == NULL) return NULL;                                             \
                                                                                       \
      pyIterator->_pyContainer = pyVector;                                             \
      pyIterator->_iterator    = pyVector->_container->begin();                        \
      Py_INCREF(pyVector);                                                             \
      return (PyObject*)pyIterator;                                                    \
  }                                                                                    \
                                                                                       \
  static void Py##TYPE##Vector_DeAlloc(Py##TYPE##Vector* pyVector)                     \
  {                                                                                    \
      PyObject_Del(pyVector);                                                          \
  }                                                                                    \
                                                                                       \
  static void Py##TYPE##VectorIterator_DeAlloc(Py##TYPE##VectorIterator* pyIterator)   \
  {                                                                                    \
      Py_XDECREF(pyIterator->_pyContainer);                                            \
      PyObject_Del(pyIterator);                                                        \
  }                                                                                    \
                                                                                       \
  extern void Py##TYPE##Vector_LinkPyType ()                                           \
  {                                                                                    \
    cdebug_log(20,0) << "Py"#TYPE"Vector_LinkType()" << endl;                            \
                                                                                       \
    PyType##TYPE##Vector.tp_iter             = (getiterfunc)Py##TYPE##Vector_GetIterator;    \
    PyType##TYPE##Vector.tp_dealloc          = (destructor)Py##TYPE##Vector_DeAlloc;         \
    PyType##TYPE##VectorIterator.tp_dealloc  = (destructor)Py##TYPE##VectorIterator_DeAlloc; \
    PyType##TYPE##VectorIterator.tp_iter     = PyObject_SelfIter;                      \
    PyType##TYPE##VectorIterator.tp_iternext = (iternextfunc)Py##TYPE##IteratorNext;   \
  }

#define IteratorNextMethod(TYPE)  \
  static PyObject* Py##TYPE##IteratorNext(Py##TYPE##VectorIterator* pyIterator)     \
  {                                                                                 \
      HTRY                                                                          \
        if (pyIterator->_iterator != pyIterator->_pyContainer->_container->end()) { \
          TYPE* object = *(pyIterator->_iterator);                                  \
          ++(pyIterator->_iterator);                                                \
          return Py##TYPE##_Link(object);                                           \
        }                                                                           \
      HCATCH                                                                        \
      return NULL;                                                                  \
  }


# define  accessorVectorFromVoid(FUNC_NAME,PY_SELF_TYPE,SELF_TYPE,TYPE)  \
  static PyObject* PY_SELF_TYPE##_##FUNC_NAME ( PY_SELF_TYPE* self )   \
  {                                                                    \
    cdebug_log(20,0) << #PY_SELF_TYPE "_" #FUNC_NAME "()" << endl;       \
                                                                       \
    Py##TYPE##Vector* pyVector = NULL;                                 \
                                                                       \
    HTRY                                                               \
    GENERIC_METHOD_HEAD(SELF_TYPE,cobject,#SELF_TYPE"."#FUNC_NAME"()") \
    pyVector = PyObject_NEW(Py##TYPE##Vector,&PyType##TYPE##Vector);   \
    if (pyVector == NULL) return NULL;                                 \
    pyVector->_container = &cobject->FUNC_NAME();                      \
    HCATCH                                                             \
                                                                       \
    return (PyObject*)pyVector;                                        \
  }
  

// -------------------------------------------------------------------
// Vector<Data> macros (partials redefinition only)

#define declareVectorValObject(TYPE)  \
  typedef struct {                                      \
      PyObject_HEAD                                     \
      const std::vector<TYPE>* _container;              \
  } Py##TYPE##Vector;                                   \
                                                        \
  typedef struct {                                      \
      PyObject_HEAD                                     \
      Py##TYPE##Vector*                  _pyContainer;  \
      std::vector<TYPE>::const_iterator  _iterator;     \
  } Py##TYPE##VectorIterator;                           \
                                                        \
  extern PyTypeObject PyType##TYPE##Vector;             \
  extern PyTypeObject PyType##TYPE##VectorIterator;     \
                                                        \
  extern void Py##TYPE##Vector_LinkPyType();


#define IteratorValNextMethod(TYPE)  \
  static PyObject* Py##TYPE##IteratorNext(Py##TYPE##VectorIterator* pyIterator)     \
  {                                                                                 \
      HTRY                                                                          \
        if (pyIterator->_iterator != pyIterator->_pyContainer->_container->end()) { \
          TYPE  object = *(pyIterator->_iterator);                                  \
          ++(pyIterator->_iterator);                                                \
          return Py##TYPE##_Link(object);                                           \
        }                                                                           \
      HCATCH                                                                        \
      return NULL;                                                                  \
  }



// -------------------------------------------------------------------
// Attribute Method For Repr.


#define DirectReprMethod(PY_FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)                 \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self )                        \
  {                                                                           \
    if (self->ACCESS_OBJECT == NULL) {                                        \
      ostringstream repr;                                                     \
      repr << "<" #PY_SELF_TYPE " [" << (void*)self << " <-> NULL] unbound>"; \
      return PyUnicode_FromString( repr.str().c_str() );                       \
    }                                                                         \
    SELF_TYPE* object = dynamic_cast<SELF_TYPE*>(self->ACCESS_OBJECT);        \
    if (object == NULL)                                                       \
      return PyUnicode_FromString( "<PyObject invalid dynamic_cast>" );        \
                                                                              \
    ostringstream repr;                                                       \
    repr << "[" << (void*)self << "<->" << (void*)object << " " << getString(object) << "]"; \
                                                                              \
    return PyUnicode_FromString(repr.str().c_str() );                          \
  }




// -------------------------------------------------------------------
// Attribute Method For Str.

# define  DirectStrMethod(PY_FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)                \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self )                        \
  {                                                                           \
    if (self->ACCESS_OBJECT == NULL) {                                        \
      ostringstream repr;                                                     \
      repr << "<" #PY_SELF_TYPE " [" << (void*)self << " <-> NULL] unbound>"; \
      return PyUnicode_FromString( repr.str().c_str() );                       \
    }                                                                         \
    SELF_TYPE* object = dynamic_cast<SELF_TYPE*>(self->ACCESS_OBJECT);        \
    if (object == NULL)                                                       \
      return PyUnicode_FromString("<PyObject invalid dynamic_cast>" );         \
                                                                              \
    return PyUnicode_FromString(getString(object).c_str() );                   \
  }




// -------------------------------------------------------------------
// Attribute Method For Cmp, compare pointer value (unicity)

# define  DirectCmpByPtrMethod(PY_FUNC_NAME,IS_PY_OBJECT,PY_SELF_TYPE)                           \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* other, int op  )                 \
  {                                                                                              \
    if (not IS_PY_OBJECT(other)) Py_RETURN_FALSE;                                                \
                                                                                                 \
    PY_SELF_TYPE* otherPyObject = (PY_SELF_TYPE*)other;                                          \
    if ((op == Py_LT) and (self->ACCESS_OBJECT <  otherPyObject->ACCESS_OBJECT)) Py_RETURN_TRUE; \
    if ((op == Py_LE) and (self->ACCESS_OBJECT <= otherPyObject->ACCESS_OBJECT)) Py_RETURN_TRUE; \
    if ((op == Py_EQ) and (self->ACCESS_OBJECT == otherPyObject->ACCESS_OBJECT)) Py_RETURN_TRUE; \
    if ((op == Py_NE) and (self->ACCESS_OBJECT != otherPyObject->ACCESS_OBJECT)) Py_RETURN_TRUE; \
    if ((op == Py_GT) and (self->ACCESS_OBJECT >  otherPyObject->ACCESS_OBJECT)) Py_RETURN_TRUE; \
    if ((op == Py_GE) and (self->ACCESS_OBJECT >= otherPyObject->ACCESS_OBJECT)) Py_RETURN_TRUE; \
                                                                                                 \
    Py_RETURN_FALSE;                                                                             \
  }




// -------------------------------------------------------------------
// Attribute Method For Cmp, compare object contents

# define  DirectCmpByValueMethod(PY_FUNC_NAME,IS_PY_OBJECT,PY_SELF_TYPE)        \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self, PyObject* other, int op ) \
  {                                                                             \
    if (not IS_PY_OBJECT(other)) Py_RETURN_FALSE;                               \
                                                                                \
    PY_SELF_TYPE* otherPyObject = (PY_SELF_TYPE*)other;                         \
    if ((op == Py_LT) and  (self->ACCESS_OBJECT  <    otherPyObject->ACCESS_OBJECT)) Py_RETURN_TRUE; \
    if ((op == Py_LE) and  (self->ACCESS_OBJECT  <=   otherPyObject->ACCESS_OBJECT)) Py_RETURN_TRUE; \
    if ((op == Py_EQ) and *(self->ACCESS_OBJECT) == *(otherPyObject->ACCESS_OBJECT)) Py_RETURN_TRUE; \
    if ((op == Py_NE) and  (self->ACCESS_OBJECT  !=   otherPyObject->ACCESS_OBJECT)) Py_RETURN_TRUE; \
    if ((op == Py_GT) and  (self->ACCESS_OBJECT  >    otherPyObject->ACCESS_OBJECT)) Py_RETURN_TRUE; \
    if ((op == Py_GE) and  (self->ACCESS_OBJECT  >=   otherPyObject->ACCESS_OBJECT)) Py_RETURN_TRUE; \
                                                                                \
    Py_RETURN_FALSE;                                                            \
  }


// -------------------------------------------------------------------
// Attribute Method For Hash.

# define  DirectHashMethod(PY_FUNC_NAME,SELF_TYPE)  \
  static int PY_FUNC_NAME ( Py##SELF_TYPE* self )   \
  { return getPyHash( self->ACCESS_OBJECT ); }


// -------------------------------------------------------------------
// Attribute Macro For Simple Link/Creation.

#define LinkCreateMethod(SELF_TYPE)                                            \
  PyObject* Py##SELF_TYPE##_Link ( SELF_TYPE* object ) {                       \
    if ( object == NULL ) {                                                    \
      Py_RETURN_NONE;                                                          \
    }                                                                          \
    Py##SELF_TYPE* pyObject = NULL;                                            \
    HTRY                                                                       \
    pyObject = PyObject_NEW(Py##SELF_TYPE, &PyType##SELF_TYPE);                \
    if (pyObject == NULL) { return NULL; }                                     \
                                                                               \
    pyObject->ACCESS_OBJECT = object;                                          \
    cdebug_log(20,0) << "Py" #SELF_TYPE "_Link(" << (void*)pyObject << ") "    \
                   << (void*)object << ":" << object << endl;                  \
    HCATCH                                                                     \
                                                                               \
    return ( (PyObject*)pyObject );                                            \
  }



// -------------------------------------------------------------------
// Attribute Macro For DBo Deletion.

# define  DBoDestroyAttribute(PY_FUNC_NAME,PY_SELF_TYPE)                                   \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self )                                     \
  {                                                                                        \
    HTRY                                                                                   \
    if (self->ACCESS_OBJECT == NULL) {                                                     \
      ostringstream  message;                                                              \
      message << "applying a destroy() to a Python object with no Hurricane object attached"; \
        PyErr_SetString( ProxyError, message.str().c_str() );                              \
      return NULL;                                                                         \
    }                                                                                      \
    ProxyProperty* proxy = static_cast<ProxyProperty*>                                     \
                           ( self->ACCESS_OBJECT->getProperty( ProxyProperty::getPropertyName() ) ); \
    if (proxy == NULL) {                                                                   \
      ostringstream  message;                                                              \
      message << "Trying to destroy() a Hurricane object of with no Proxy attached ";      \
      PyErr_SetString( ProxyError, message.str().c_str() );                                \
      return NULL;                                                                         \
    }                                                                                      \
    cdebug_log(20,0) << #PY_FUNC_NAME "(" << (void*)self << ") "                           \
                     << (void*)self->ACCESS_OBJECT << ":" << self->ACCESS_OBJECT << endl;  \
    self->ACCESS_OBJECT->destroy();                                                        \
    self->ACCESS_OBJECT = NULL;                                                            \
    HCATCH                                                                                 \
    Py_RETURN_NONE;                                                                        \
  }


// -------------------------------------------------------------------
// Attribute Macro For BDo Link/Creation.

#define DBoLinkCreateMethod(SELF_TYPE)                                         \
  PyObject* Py##SELF_TYPE##_Link ( SELF_TYPE* object ) {                       \
    if ( object == NULL ) {                                                    \
      Py_RETURN_NONE;                                                          \
    }                                                                          \
    Py##SELF_TYPE* pyObject = NULL;                                            \
    HTRY                                                                       \
    ProxyProperty* proxy = static_cast<ProxyProperty*>                         \
      ( object->getProperty ( ProxyProperty::getPropertyName() ) );            \
    if ( proxy == NULL ) {                                                     \
      pyObject = PyObject_NEW(Py##SELF_TYPE, &PyType##SELF_TYPE);              \
      if (pyObject == NULL) { return NULL; }                                   \
                                                                               \
      proxy = ProxyProperty::create ( (void*)pyObject );                       \
      CHECK_OFFSET ( pyObject, SELF_TYPE )                                     \
                                                                               \
      pyObject->ACCESS_OBJECT = object;                                        \
      object->put ( proxy );                                                   \
    } else {                                                                   \
      pyObject = (Py##SELF_TYPE*)proxy->getShadow ();                          \
      Py_INCREF ( ACCESS_CLASS(pyObject) );                                    \
    }                                                                          \
    cdebug_log(20,0) << "PyDbo" #SELF_TYPE "_Link(" << (void*)pyObject << ") " \
                   << (void*)object << ":" << object << endl;                  \
    HCATCH                                                                     \
                                                                               \
    return ( (PyObject*)pyObject );                                            \
  }

// -------------------------------------------------------------------
// Attribute Method For BDo Deletion.

# define  DBoDeleteMethod(SELF_TYPE)                                     \
  static void Py##SELF_TYPE##_DeAlloc ( Py##SELF_TYPE *self )            \
  {                                                                      \
    cdebug_log(20,0) << "PyDbObject_DeAlloc(" << (void*)self << ") "     \
                   << (void*)(self->ACCESS_OBJECT) << ":" << self->ACCESS_OBJECT << endl; \
                                                                         \
    if ( self->ACCESS_OBJECT != NULL ) {                                 \
        ProxyProperty* proxy = static_cast<ProxyProperty*>               \
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
// Attribute Method For Python Only Object Deletion.

# define  PythonOnlyDeleteMethod(SELF_TYPE)                      \
  static void Py##SELF_TYPE##_DeAlloc ( Py##SELF_TYPE *self )    \
  {                                                              \
    cdebug_log(20,0) << "PythonOnlyObject_DeAlloc(" << (void*)self << ") " \
                   << (void*)(self->ACCESS_OBJECT)        \
          << ":" << self->ACCESS_OBJECT << endl;                 \
    PyObject_DEL ( self );                                       \
  }




// -------------------------------------------------------------------
// Attribute Method For Python Only Object Deletion.

# define  NoObjectDeleteMethod(SELF_TYPE)                        \
  static void Py##SELF_TYPE##_DeAlloc ( Py##SELF_TYPE *self )    \
  {                                                              \
    cdebug_log(20,0) << "PythonOnlyObject_DeAlloc(" << (void*)self << ") " \
                   << "[no object]" << endl;                     \
    PyObject_DEL ( self );                                       \
  }


  

// -------------------------------------------------------------------
// Initialisation Function for PyTypeObject Runtime Link.

#define PyTypeObjectLinkPyTypeWithoutObject(PY_SELF_TYPE,SELF_TYPE)               \
  extern void  Py##PY_SELF_TYPE##_LinkPyType() {                                  \
    cdebug_log(20,0) << "Py" #PY_SELF_TYPE "_LinkType()" << endl;                 \
                                                                                  \
    PyType##PY_SELF_TYPE.tp_dealloc = (destructor) Py##PY_SELF_TYPE##_DeAlloc;    \
    PyType##PY_SELF_TYPE.tp_methods = Py##PY_SELF_TYPE##_Methods;                 \
  }


  

// -------------------------------------------------------------------
// Initialisation Function for PyTypeObject Runtime Link.

#define PyTypeObjectLinkPyTypeWithClass(PY_SELF_TYPE,SELF_TYPE)                       \
  DirectReprMethod    (Py##PY_SELF_TYPE##_Repr, Py##PY_SELF_TYPE,   SELF_TYPE)        \
  DirectStrMethod     (Py##PY_SELF_TYPE##_Str,  Py##PY_SELF_TYPE,   SELF_TYPE)        \
  DirectCmpByPtrMethod(Py##PY_SELF_TYPE##_Cmp,  IsPy##PY_SELF_TYPE, Py##PY_SELF_TYPE) \
  DirectHashMethod    (Py##PY_SELF_TYPE##_Hash, SELF_TYPE)                            \
  extern void  Py##PY_SELF_TYPE##_LinkPyType() {                                      \
    cdebug_log(20,0) << "Py" #PY_SELF_TYPE "_LinkType()" << endl;                     \
                                                                                      \
    PyType##PY_SELF_TYPE.tp_dealloc     = (destructor) Py##PY_SELF_TYPE##_DeAlloc;    \
    PyType##PY_SELF_TYPE.tp_richcompare = (richcmpfunc)Py##PY_SELF_TYPE##_Cmp;        \
    PyType##PY_SELF_TYPE.tp_repr        = (reprfunc)   Py##PY_SELF_TYPE##_Repr;       \
    PyType##PY_SELF_TYPE.tp_str         = (reprfunc)   Py##PY_SELF_TYPE##_Str;        \
    PyType##PY_SELF_TYPE.tp_hash        = (hashfunc)   Py##PY_SELF_TYPE##_Hash;       \
    PyType##PY_SELF_TYPE.tp_methods     = Py##PY_SELF_TYPE##_Methods;                 \
  }


#define PyTypeObjectLinkPyTypeWithClassNewInit(PY_SELF_TYPE,SELF_TYPE)                 \
  DirectReprMethod     (Py##PY_SELF_TYPE##_Repr, Py##PY_SELF_TYPE,   SELF_TYPE)        \
  DirectStrMethod      (Py##PY_SELF_TYPE##_Str,  Py##PY_SELF_TYPE,   SELF_TYPE)        \
  DirectCmpByPtrMethod (Py##PY_SELF_TYPE##_Cmp,  IsPy##PY_SELF_TYPE, Py##PY_SELF_TYPE) \
  DirectHashMethod     (Py##PY_SELF_TYPE##_Hash, SELF_TYPE)                            \
  extern void  Py##PY_SELF_TYPE##_LinkPyType() {                                       \
    cdebug_log(20,0) << "Py" #PY_SELF_TYPE "_LinkType()" << endl;                      \
                                                                                       \
    PyType##PY_SELF_TYPE.tp_dealloc     = (destructor) Py##PY_SELF_TYPE##_DeAlloc;     \
    PyType##PY_SELF_TYPE.tp_richcompare = (richcmpfunc)Py##PY_SELF_TYPE##_Cmp;         \
    PyType##PY_SELF_TYPE.tp_repr        = (reprfunc)   Py##PY_SELF_TYPE##_Repr;        \
    PyType##PY_SELF_TYPE.tp_str         = (reprfunc)   Py##PY_SELF_TYPE##_Str;         \
    PyType##PY_SELF_TYPE.tp_hash        = (hashfunc)   Py##PY_SELF_TYPE##_Hash;        \
    PyType##PY_SELF_TYPE.tp_new         = (newfunc)    Py##PY_SELF_TYPE##_NEW;         \
    PyType##PY_SELF_TYPE.tp_init        = (initproc)   Py##PY_SELF_TYPE##_Init;        \
    PyType##PY_SELF_TYPE.tp_methods     = Py##PY_SELF_TYPE##_Methods;                  \
  }


#define PyTypeObjectLinkPyType(SELF_TYPE)  \
  PyTypeObjectLinkPyTypeWithClass(SELF_TYPE,SELF_TYPE)

#define PyTypeObjectLinkPyTypeNewInit(SELF_TYPE)  \
  PyTypeObjectLinkPyTypeWithClassNewInit(SELF_TYPE,SELF_TYPE)


#define PyTypeObjectLinkPyTypeAsValue(SELF_TYPE)                               \
  DirectReprMethod      (Py##SELF_TYPE##_Repr, Py##SELF_TYPE,   SELF_TYPE)     \
  DirectStrMethod       (Py##SELF_TYPE##_Str,  Py##SELF_TYPE,   SELF_TYPE)     \
  DirectCmpByValueMethod(Py##SELF_TYPE##_Cmp,  IsPy##SELF_TYPE, Py##SELF_TYPE) \
  DirectHashMethod      (Py##SELF_TYPE##_Hash, SELF_TYPE)                      \
  extern void  Py##SELF_TYPE##_LinkPyType() {                                  \
    cdebug_log(20,0) << "Py" #SELF_TYPE "_LinkType()" << endl;                 \
                                                                               \
    PyType##SELF_TYPE.tp_dealloc     = (destructor) Py##SELF_TYPE##_DeAlloc;   \
    PyType##SELF_TYPE.tp_richcompare = (richcmpfunc)Py##SELF_TYPE##_Cmp;       \
    PyType##SELF_TYPE.tp_repr        = (reprfunc)   Py##SELF_TYPE##_Repr;      \
    PyType##SELF_TYPE.tp_str         = (reprfunc)   Py##SELF_TYPE##_Str;       \
    PyType##SELF_TYPE.tp_hash        = (hashfunc)   Py##SELF_TYPE##_Hash;      \
    PyType##SELF_TYPE.tp_new         = (newfunc)    Py##SELF_TYPE##_NEW;       \
    PyType##SELF_TYPE.tp_init        = (initproc)   Py##SELF_TYPE##_Init;      \
    PyType##SELF_TYPE.tp_methods     = Py##SELF_TYPE##_Methods;                \
  }


// Special Initialisation Function for Locator PyTypeObject Runtime Link.
#define LocatorPyTypeObjectLinkPyType(PY_SELF_TYPE, SELF_TYPE)                                                 \
  DirectReprMethod    (Py##PY_SELF_TYPE##Locator_Repr, Py##PY_SELF_TYPE##Locator,   Locator<SELF_TYPE>)        \
  DirectStrMethod     (Py##PY_SELF_TYPE##Locator_Str,  Py##PY_SELF_TYPE##Locator,   Locator<SELF_TYPE>)        \
  DirectCmpByPtrMethod(Py##PY_SELF_TYPE##Locator_Cmp,  IsPy##PY_SELF_TYPE##Locator, Py##PY_SELF_TYPE##Locator) \
  extern void  Py##PY_SELF_TYPE##Locator_LinkPyType ()                                                         \
  {                                                                                                            \
    cdebug_log(20,0) << "Py" #PY_SELF_TYPE "Locator_LinkType()" << endl;                                       \
                                                                                                               \
    PyType##PY_SELF_TYPE##Locator.tp_dealloc     = (destructor) Py##PY_SELF_TYPE##Locator_DeAlloc;             \
    PyType##PY_SELF_TYPE##Locator.tp_richcompare = (richcmpfunc)Py##PY_SELF_TYPE##Locator_Cmp;                 \
    PyType##PY_SELF_TYPE##Locator.tp_repr        = (reprfunc)   Py##PY_SELF_TYPE##Locator_Repr;                \
    PyType##PY_SELF_TYPE##Locator.tp_str         = (reprfunc)   Py##PY_SELF_TYPE##Locator_Str;                 \
    PyType##PY_SELF_TYPE##Locator.tp_methods     = Py##PY_SELF_TYPE##Locator_Methods;                          \
  }

#define PyTypeObjectDefinitions(SELF_TYPE)                              \
  PyTypeObject  PyType##SELF_TYPE =                                     \
    { PyVarObject_HEAD_INIT(NULL,0)                                     \
      "Hurricane."#SELF_TYPE          /* tp_name.          */           \
    , sizeof(Py##SELF_TYPE)           /* tp_basicsize.     */           \
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
    , Py_TPFLAGS_DEFAULT              /* tp_flags          */           \
    , "#SELF_TYPE objects"            /* tp_doc.           */           \
  };

#define PyTypeObjectDefinitionsOfModule(MODULE,SELF_TYPE)               \
  PyTypeObject  PyType##SELF_TYPE =                                     \
    { PyVarObject_HEAD_INIT(NULL,0)                                     \
      #MODULE "." #SELF_TYPE          /* tp_name.          */           \
    , sizeof(Py##SELF_TYPE)           /* tp_basicsize.     */           \
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
    , Py_TPFLAGS_DEFAULT              /* tp_flags          */           \
    , "#SELF_TYPE objects"            /* tp_doc.           */           \
  };

#define PyTypeRootObjectDefinitions(SELF_TYPE)                          \
  PyTypeObject  PyType##SELF_TYPE =                                     \
    { PyVarObject_HEAD_INIT(&PyType_Type,0)                             \
       "Hurricane.Py" #SELF_TYPE      /* tp_name.          */           \
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
    , "#SELF_TYPE objects"            /* tp_doc.           */           \
  };

#define PyTypeInheritedObjectDefinitions(SELF_TYPE, INHERITED_TYPE)     \
  PyTypeObject  PyType##SELF_TYPE =                                     \
    { PyVarObject_HEAD_INIT(DEFERRED_ADDRESS(&PyType##INHERITED_TYPE),0)  \
       "Hurricane." #SELF_TYPE        /* tp_name.          */           \
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
  };

#define PyTypeCollectionObjectDefinitions(SELF_TYPE)                    \
  PyTypeObject  PyType##SELF_TYPE =                                     \
    { PyVarObject_HEAD_INIT(NULL,0)                                     \
      "Hurricane."#SELF_TYPE          /* tp_name.          */           \
    , sizeof(Py ##SELF_TYPE)          /* tp_basicsize.     */           \
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
    , PyObject_GenericGetAttr         /* tp_getattro.      */           \
    , 0                               /* tp_setattro.      */           \
    , 0                               /* tp_as_buffer.     */           \
    , Py_TPFLAGS_DEFAULT              /* tp_flags.         */           \
    , "#SELF_TYPE objects"            /* tp_doc.           */           \
  };


#define PyTypeVectorObjectDefinitions(SELF_TYPE)  \
  PyTypeObject  PyType##SELF_TYPE =                                     \
    { PyVarObject_HEAD_INIT(NULL,0)                                     \
      "Hurricane."#SELF_TYPE          /* tp_name.          */           \
    , sizeof(Py ##SELF_TYPE)          /* tp_basicsize.     */           \
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
    , PyObject_GenericGetAttr         /* tp_getattro.      */           \
    , 0                               /* tp_setattro.      */           \
    , 0                               /* tp_as_buffer.     */           \
    , Py_TPFLAGS_DEFAULT              /* tp_flags.         */           \
    , "#SELF_TYPE objects"            /* tp_doc.           */           \
  };


// -------------------------------------------------------------------
//PyType_Ready Methods

#define  PYTYPE_READY(TYPE)                                  \
  if ( PyType_Ready( &PyType##TYPE ) < 0 ) {                 \
    cerr << "[ERROR]\n"                                      \
         << "  Failed to initialize <Py" #TYPE ">." << endl; \
    return NULL;                                             \
  }


#define  PYTYPE_READY_SUB(TYPE, TYPE_BASE)                   \
  PyType##TYPE.tp_base = &PyType##TYPE_BASE;                 \
  if ( PyType_Ready( &PyType##TYPE ) < 0 ) {                 \
    cerr << "[ERROR]\n"                                      \
         << "  Failed to initialize <Py" #TYPE ">." << endl; \
    return NULL;                                             \
  }



// -------------------------------------------------------------------
// Hurricane Try / Catch.

# define   HTRY  try {

# define   HCATCH  \
    }                                                               \
    catch ( const Warning& w ) {                                    \
      std::string message = getString(w);                           \
      PyErr_Warn ( HurricaneWarning, const_cast<char*>(message.c_str()) ); \
      std::cerr << message << std::endl;                            \
    }                                                               \
    catch ( const Error& e ) {                                      \
      std::string message = getString(e);                           \
      if (not e.where().empty()) message += "\n" + e.where();       \
      PyErr_SetString ( HurricaneError, message.c_str() );          \
      std::cerr << message << std::endl;                            \
      return NULL;                                                  \
    }                                                               \
    catch ( const Bug& e ) {                                        \
      std::string message = getString(e);                           \
      PyErr_SetString ( HurricaneError, message.c_str() );          \
      std::cerr << message << std::endl;                            \
      return NULL;                                                  \
    }                                                               \
    catch ( const Exception& e ) {                                  \
      std::string message = "Unknown Hurricane::Exception";         \
      PyErr_SetString ( HurricaneError, message.c_str() );          \
      std::cerr << message << std::endl;                            \
      return NULL;                                                  \
    }                                                               \
    catch ( const std::exception& e )  {                            \
      std::string message = std::string(e.what());                  \
      PyErr_SetString ( HurricaneError, message.c_str() );          \
      std::cerr << message << std::endl;                            \
      return NULL;                                                  \
    }                                                               \
    catch ( ... ) {                                                 \
      std::string message =                                         \
        "Unmanaged exception, neither a Hurricane::Error nor"       \
        " a std::exception.";                                       \
      PyErr_SetString ( HurricaneError, message.c_str() );          \
      std::cerr << message << std::endl;                            \
      return NULL;                                                  \
    }                                                               \

}  // End of extern "C".

}  // End of Isobar namespace.
