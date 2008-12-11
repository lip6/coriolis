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

#include "hurricane/Error.h"
#include "hurricane/Warning.h"

#include "hurricane/isobar/ProxyProperty.h"

namespace Isobar {


using namespace std;


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


// This macro must be redefined in derived classes.
// Example : _baseOject._object
#define  ACCESS_OBJECT            _object
#define  ACCESS_CLASS(_pyObject)  _pyObject


#define  LOAD_CONSTANT(CONSTANT_VALUE,CONSTANT_NAME)             \
 constant = PyInt_FromLong ( (long)CONSTANT_VALUE );             \
 PyDict_SetItemString ( dictionnary, CONSTANT_NAME, constant );  \
 Py_DECREF ( constant );




// -------------------------------------------------------------------
// Generic Method Header.

#define GENERIC_METHOD_HEAD(SELF_TYPE,SELF_OBJECT,function)                \
    if ( self->ACCESS_OBJECT == NULL ) {                                   \
      PyErr_SetString ( ProxyError, "Attempt to call " function " on an unbound hurricane object" ); \
      return ( NULL );                                                     \
    }                                                                      \
    SELF_TYPE* SELF_OBJECT = dynamic_cast<SELF_TYPE*>(self->ACCESS_OBJECT);\
    if ( self->ACCESS_OBJECT == NULL ) {                                   \
      PyErr_SetString ( ProxyError, "Invalid dynamic_cast while calling " function "" ); \
      return ( NULL );                                                     \
    }




// -------------------------------------------------------------------
// Attribute Method Macro For Checking Bound.

#define  GetBoundStateAttribute(PY_FUNC_NAME,PY_SELF_TYPE,SELF_TYPE) \
  static PyObject* PY_FUNC_NAME ( PY_SELF_TYPE *self )               \
  {                                                                  \
    long  result = 1;                                                \
    if ( self->ACCESS_OBJECT == NULL ) result = 0;                   \
                                                                     \
    return ( Py_BuildValue ("i",result) );                           \
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

#define  DirectSetLongAttribute(PY_FUNC_NAME,FUNC_NAME,PY_FORMAT,PY_SELF_TYPE,SELF_TYPE) \
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
// Attribute Method Macro For Names.

#define GetNameMethod(SELF_TYPE, SELF)                                 \
  static PyObject* Py##SELF_TYPE##_getName(Py##SELF_TYPE* self) {      \
    trace << "Py"#SELF_TYPE"_getName()" << endl;                       \
    HTRY                                                               \
    METHOD_HEAD (#SELF_TYPE".getName()")                               \
    return PyString_FromString(getString(SELF->getName()).c_str());    \
    HCATCH                                                             \
    return NULL;                                                       \
  }

#define SetNameMethod(SELF_TYPE, SELF)                                                 \
  static PyObject* Py##SELF_TYPE##_setName(Py##SELF_TYPE* self, PyObject* args) {      \
    trace << "Py"#SELF_TYPE"_setName()" << endl;                                       \
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

#define  DirectDeleteMethod(PY_FUNC_NAME, PY_SELF_TYPE)     \
  static void PY_FUNC_NAME ( PY_SELF_TYPE *self )           \
  {                                                         \
    trace << #PY_SELF_TYPE"_DeAlloc(" << hex << self << ") "    \
          << self->ACCESS_OBJECT << endl;                   \
                                                            \
    if ( self->ACCESS_OBJECT ) delete self->ACCESS_OBJECT;  \
    PyObject_DEL ( self );                                  \
  }



// -------------------------------------------------------------------
// Attribute Method For Deletion.

#define PlugDeleteMethod(PY_FUNC_NAME,PY_SELF_TYPE)              \
  static void PY_FUNC_NAME ( PY_SELF_TYPE *self )                \
  {                                                              \
    trace << "PyHObject_DeAlloc(" << hex << self << ") "         \
          << self->ACCESS_OBJECT << endl;                        \
                                                                 \
    PyObject_DEL ( self );                                       \
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
      trace << "Py" #SELF_TYPE "_" #FUNC_NAME "()" << endl;             \
      HTRY                                                              \
      METHOD_HEAD(#SELF_TYPE "." #FUNC_NAME "()")                       \
      SELF_OBJECT->FUNC_NAME();                                         \
      HCATCH                                                            \
      Py_RETURN_NONE;                                                   \
  }
  

// -------------------------------------------------------------------
// Collection and Locator macros


#define CollectionMethods(TYPE)                                                        \
  static PyObject* GetLocator(Py##TYPE##Collection* collection) {                      \
      Py##TYPE##CollectionLocator* cl =                                                \
          PyObject_New(Py##TYPE##CollectionLocator, &PyType##TYPE##CollectionLocator); \
      if (cl == NULL) {                                                                \
          return NULL;                                                                 \
      }                                                                                \
      cl->_collection = collection;                                                    \
      cl->_object = collection->_object->getLocator();                                 \
      Py_INCREF(collection);                                                           \
      return (PyObject *)cl;                                                           \
  }                                                                                    \
                                                                                       \
  static void Py##TYPE##CollectionLocatorDeAlloc(Py##TYPE##CollectionLocator* locator) {         \
      Py_XDECREF(locator->_collection);                                                \
      if (locator->_object) {                                                          \
          delete locator->_object;                                                     \
      }                                                                                \
      PyObject_Del(locator);                                                           \
  }                                                                                    \
                                                                                       \
  extern void Py##TYPE##Collection_LinkPyType () {                                     \
    trace << "Py"#TYPE"Collection_LinkType()" << endl;                                 \
    PyType##TYPE##Collection.tp_iter = (getiterfunc)GetLocator;      /* tp_iter */     \
    PyType##TYPE##Collection.tp_dealloc = (destructor)Py##TYPE##Collection_DeAlloc;    \
    PyType##TYPE##CollectionLocator.tp_dealloc = (destructor)Py##TYPE##CollectionLocatorDeAlloc; \
    PyType##TYPE##CollectionLocator.tp_iter = PyObject_SelfIter;                       \
    PyType##TYPE##CollectionLocator.tp_iternext = (iternextfunc)Py##TYPE##LocatorNext; \
  }

#define LocatorNextMethod(TYPE)                                         \
  static PyObject* Py##TYPE##LocatorNext(Py##TYPE##CollectionLocator* pyLocator) {  \
      Locator<TYPE*>* locator = pyLocator->_object;                     \
      HTRY                                                              \
      if (locator->isValid()) {                                         \
          TYPE* object = locator->getElement();                         \
          locator->progress();                                          \
          return Py##TYPE##_Link(object);                               \
      }                                                                 \
      HCATCH                                                            \
      return NULL;                                                      \
  }


#define EntityLocatorNextMethod(TYPE)                                         \
  static PyObject* Py##TYPE##LocatorNext(Py##TYPE##CollectionLocator* pyLocator) {  \
      Locator<TYPE*>* locator = pyLocator->_object;                     \
      HTRY                                                              \
      if (locator->isValid()) {                                         \
          TYPE* object = locator->getElement();                         \
          locator->progress();                                          \
          return PyEntity_NEW(object);                                  \
      }                                                                 \
      HCATCH                                                            \
      return NULL;                                                      \
  }



// -------------------------------------------------------------------
// Attribute Method For Repr.


#define DirectReprMethod(PY_FUNC_NAME,PY_SELF_TYPE,SELF_TYPE)            \
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
    ProxyProperty* proxy = static_cast<ProxyProperty*>                                   \
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
      CHECK_OFFSET ( pyObject )                                                \
                                                                               \
      pyObject->ACCESS_OBJECT = object;                                        \
      object->put ( proxy );                                                   \
    } else {                                                                   \
      pyObject = (Py##SELF_TYPE*)proxy->getShadow ();                          \
      Py_INCREF ( ACCESS_CLASS(pyObject) );                                    \
    }                                                                          \
    HCATCH                                                                     \
                                                                               \
    return ( (PyObject*)pyObject );                                            \
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

#define PyTypeObjectDefinitions(SELF_TYPE)                              \
  PyTypeObject  PyType##SELF_TYPE =                                     \
    { PyObject_HEAD_INIT(NULL)                                          \
      0                               /* ob_size.          */           \
    , "Hurricane."#SELF_TYPE          /* tp_name.          */           \
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
    , Py_TPFLAGS_DEFAULT                                                \
    | Py_TPFLAGS_BASETYPE             /* tp_flags.         */           \
    , "#SELF_TYPE objects"            /* tp_doc.           */           \
  };

#define PyTypeCollectionObjectDefinitions(SELF_TYPE)                    \
  PyTypeObject  PyType##SELF_TYPE =                                     \
    { PyObject_HEAD_INIT(NULL)                                          \
      0                               /* ob_size.          */           \
    , "Hurricane."#SELF_TYPE          /* tp_name.          */           \
    , sizeof(Py ##SELF_TYPE)           /* tp_basicsize.     */           \
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

#define  PYTYPE_READY(TYPE)                       \
  if ( PyType_Ready( &PyType##TYPE ) < 0 ) {      \
    cerr << "[ERROR]\n"                           \
         << "  Failed to initialize <Py" #TYPE ">." << endl; \
    return;                                       \
  }


#define  PYTYPE_READY_SUB(TYPE, TYPE_BASE)        \
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
