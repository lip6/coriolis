// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Authors     :                    Jean-Paul CHAPUT              |
// |                                      Damien DUPUIS              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyHurricane.h"              |
// +-----------------------------------------------------------------+


#pragma once

// Enable Python debugging.
// #define  DEBUG  1

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

namespace Isobar2 {


  using namespace std;
  using Hurricane::demangle;
  using Hurricane::DbU;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Exception;
  using Hurricane::DBo;

  
  extern "C" {

    extern PyObject* ConstructorError;
    extern PyObject* ProxyError;
    extern PyObject* HurricaneError;
    extern PyObject* HurricaneWarning;

  }


  inline int  PyAny_AsInt ( PyObject* object )
  {
    long value = 0;
    if      (PyObject_IsInstance(object,(PyObject*)&PyInt_Type )) value = PyInt_AsLong ( object );
    else if (PyObject_IsInstance(object,(PyObject*)&PyLong_Type)) value = PyLong_AsLong( object );
    return (int)value;
  }


  inline uint32_t  PyAny_AsUInt32 ( PyObject* object )
  {
    int64_t value = 0;
    if      (PyObject_IsInstance(object,(PyObject*)&PyInt_Type )) value = PyInt_AsLong ( object );
    else if (PyObject_IsInstance(object,(PyObject*)&PyLong_Type)) value = PyLong_AsLong( object );
    return (uint32_t)value;
  }


  template< typename T = DbU::Unit, typename enable_if< is_same<T,long>::value, T >::type = 0 >
  inline T  PyAny_AsLong ( PyObject* object )
  {
    T  value = 0;
    if      (PyObject_IsInstance(object,(PyObject*)&PyInt_Type )) value = PyInt_AsLong ( object );
    else if (PyObject_IsInstance(object,(PyObject*)&PyLong_Type)) value = PyLong_AsLong( object );
    return value;
  }


  template< typename T = DbU::Unit, typename enable_if< is_same<T,long long>::value, T >::type = 0 >
  inline T  PyAny_AsLong ( PyObject* object )
  {
    T  value = 0;
    if      (PyObject_IsInstance(object,(PyObject*)&PyInt_Type )) value = PyInt_AsLong     ( object );
    else if (PyObject_IsInstance(object,(PyObject*)&PyLong_Type)) value = PyLong_AsLongLong( object );

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


  typedef struct {
      PyObject_HEAD
      void* _object;
  } PyVoidPointer;



  class PyTypeManager {
    public:
      static const uint64_t  NoFlags =  0;
      static const uint64_t  IsDBo   = (1 << 0);
    public:
      inline   PyTypeManager ( PyMethodDef*, uint64_t flags );
      virtual ~PyTypeManager ();
    public:
                                      inline       bool           _isDBo          () const;
                                      inline const std::string&   _getCppTypeName () const;
                                      inline const std::string&   _getPyTypeName  () const;
                                      inline       PyTypeObject*  _getTypeObject  ();
                                      inline       PyMethodDef*   _getMethods     ();
                                                   void           _setTypeNames   ( std::string className, std::string nspace );
                                      virtual      void           _addToModule    ( PyObject* ) { };
                                      virtual      void           _deAlloc        ( PyVoidPointer* ) { };
    public:                           
                              static  inline       PyTypeManager* get             ( PyTypeObject* );
      template<typename CppT> static  inline       PyTypeManager* _get            ();
      template<typename CppT> static  inline       PyObject*      link            ( CppT* );
      template<typename CppT> static  inline       void           addToModule     ( PyObject* );
      template<typename CppT> static               PyTypeManager* add             ( PyObject*, PyMethodDef*, uint64_t flags );
    private:
      uint64_t        _flags;
      std::string     _pyTypeName;
      std::string     _cppTypeName;
      PyTypeObject    _typeObject;
      PyMethodDef*    _methods;
    private:
      static std::map< std::type_index, PyTypeManager* >  _managerByCppTypes;
      static std::map< PyTypeObject*  , PyTypeManager* >  _managerByPyTypes;
  };


  template<typename CppT>
  class PyTypeManagerAny : public PyTypeManager {
    public:
      inline   PyTypeManagerAny ( PyMethodDef*, uint64_t flags );
      virtual ~PyTypeManagerAny ();
    public:
      static  inline PyTypeManagerAny* create       ( PyMethodDef*, uint64_t flags, std::string nspace="" );
              inline PyObject*         _link        ( CppT* );
      virtual        void              _addToModule ( PyObject* );
  };


  template<typename CppT>
  class PyTypeManagerDBo : public PyTypeManagerAny<CppT> {
    public:
      inline        PyTypeManagerDBo ( PyMethodDef*, uint64_t flags );
      virtual      ~PyTypeManagerDBo ();
    public:
      virtual void  _deAlloc         ( PyVoidPointer* );
  };


  template<typename CppT>
  class PyTypeManagerNonDBo : public PyTypeManagerAny<CppT> {
    public:
      inline        PyTypeManagerNonDBo ( PyMethodDef*, uint64_t flags );
      virtual      ~PyTypeManagerNonDBo ();
    public:
      virtual void  _deAlloc            ( PyVoidPointer* );
  };


  inline PyTypeManager* PyTypeManager::get ( PyTypeObject* obType )
  {return _managerByPyTypes[ obType ]; }


  template<typename CppT> inline PyTypeManager* PyTypeManager::_get ()
  { return _managerByCppTypes[ std::type_index(typeid(CppT)) ]; }


  inline  PyTypeManager::PyTypeManager ( PyMethodDef *methods, uint64_t flags )
    : _flags      (flags)
    , _pyTypeName ()
    , _cppTypeName()
    , _typeObject({ PyObject_HEAD_INIT(&PyType_Type)
                      0                               /* ob_size.          */
                    ,  NULL                           /* tp_name.          */
                    ,  sizeof(PyVoidPointer)          /* tp_basicsize.     */
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
                    , Py_TPFLAGS_DEFAULT
                    | Py_TPFLAGS_BASETYPE             /* tp_flags.         */
                    , "#SELF_TYPE objects"            /* tp_doc.           */
                    })
    , _methods    (methods)
  { }


  inline bool  PyTypeManager::_isDBo () const
  { return _flags & IsDBo; }

  
  inline const std::string& PyTypeManager::_getCppTypeName () const
  { return _cppTypeName; }

  
  inline const std::string& PyTypeManager::_getPyTypeName () const
  { return _pyTypeName; }


  inline PyTypeObject* PyTypeManager::_getTypeObject ()
  { return &_typeObject; }


  inline PyMethodDef* PyTypeManager::_getMethods ()
  { return _methods; }


  template< typename CppT >
  inline PyObject* PyTypeManager::link ( CppT* object )
  { return static_cast< PyTypeManagerAny<CppT>* >(_get<CppT>())->_link( object ); }


  template< typename CppT >
  inline void  PyTypeManager::addToModule ( PyObject* module )
  { _get<CppT>()->_addToModule( module ); }


  template< typename CppT >
  PyTypeManager* PyTypeManager::add ( PyObject* module, PyMethodDef* methods, uint64_t flags )
  {
    PyTypeManagerAny<CppT>* manager = PyTypeManagerAny<CppT>::create( methods, flags );
    _managerByCppTypes[ std::type_index(typeid(CppT)) ] = manager;
    _managerByPyTypes [ manager->_getTypeObject()     ] = manager;
    manager->_addToModule( module );
    return manager;
  }

  extern "C" {

    extern void  _deAlloc ( PyVoidPointer* );
    
  }
  

  template< typename CppT >
  inline PyTypeManagerAny<CppT>::PyTypeManagerAny ( PyMethodDef* methods, uint64_t flags )
    : PyTypeManager(methods,flags)
  { }
  

  template< typename CppT >
  PyTypeManagerAny<CppT>::~PyTypeManagerAny () { }


  template< typename CppT >
  inline PyTypeManagerAny<CppT>* PyTypeManagerAny<CppT>::create ( PyMethodDef* methods, uint64_t flags, std::string nspace )
  {
    if (std::is_base_of<Hurricane::Entity,CppT>::value) flags |= PyTypeManager::IsDBo;
    
    PyTypeManagerAny<CppT>* manager = NULL;
    if (flags & PyTypeManager::IsDBo) manager = new PyTypeManagerDBo   <CppT>( methods, flags );
    else                              manager = new PyTypeManagerNonDBo<CppT>( methods, flags );

    manager->_setTypeNames( demangle(typeid(CppT)), nspace );

    PyTypeObject* ob_type = manager->_getTypeObject();
    ob_type->tp_name    = manager->_getPyTypeName().c_str();
    ob_type->tp_dealloc = (destructor)&::Isobar2::_deAlloc;
    ob_type->tp_methods = manager->_getMethods();

    return manager;
  }


  template< typename CppT >
  void  PyTypeManagerAny<CppT>::_addToModule ( PyObject* module )
  {
    if (PyType_Ready(_getTypeObject()) < 0) {
      cerr << "[ERROR] Failed to initialize <" << _getPyTypeName() << ">." << endl;
      return;
    }

    Py_INCREF( _getTypeObject() );
    PyModule_AddObject( module, _getCppTypeName().c_str(), (PyObject*)_getTypeObject() );
  }


  template< typename CppT >
  PyObject* PyTypeManagerAny<CppT>::_link ( CppT* object )
  {
    if (not object) Py_RETURN_NONE;

    PyVoidPointer* pyObject = NULL;
    pyObject = PyObject_NEW( PyVoidPointer, _getTypeObject() );
    if (pyObject == NULL) return NULL;

    pyObject->_object = object;
    cdebug_log(20,0) << _getPyTypeName() << "_Link(" << (void*)pyObject << ") "
                     << (void*)object << ":" << object << endl;

    return (PyObject*)pyObject;
  }
  

  template< typename CppT >
  inline PyTypeManagerDBo<CppT>::PyTypeManagerDBo ( PyMethodDef* methods, uint64_t flags )
    : PyTypeManagerAny<CppT>(methods,flags)
  { }


  template< typename CppT >
  PyTypeManagerDBo<CppT>::~PyTypeManagerDBo () { }

    
  template< typename CppT >
  void  PyTypeManagerDBo<CppT>::_deAlloc ( PyVoidPointer* self )
  {
    DBo* object = reinterpret_cast<DBo*>( self->_object );
    cdebug_log(20,0) << "PyTypeManager_DeAlloc(" << (void*)self << ") "
                     << (void*)(self->_object) << ":" << object << endl;

    Isobar::ProxyProperty* proxy =
      static_cast<Isobar::ProxyProperty*>( object->getProperty( Isobar::ProxyProperty::getPropertyName() ) );

    if (not proxy) {
      ostringstream  message;
      message << "deleting a Python object with no Proxy attached ";
      PyErr_SetString( ProxyError, message.str().c_str() );
    }
    object->remove ( proxy );
    
    PyObject_DEL( self );
  }
  

  template< typename CppT >
  inline PyTypeManagerNonDBo<CppT>::PyTypeManagerNonDBo ( PyMethodDef* methods, uint64_t flags )
    : PyTypeManagerAny<CppT>(methods,flags)
  { }


  template< typename CppT >
  PyTypeManagerNonDBo<CppT>::~PyTypeManagerNonDBo () { }

    
  template< typename CppT >
  void  PyTypeManagerNonDBo<CppT>::_deAlloc ( PyVoidPointer* self )
  {
    delete reinterpret_cast<CppT*>( self->_object );
    PyObject_DEL( self );
  }

  
  typedef std::function<PyObject*(PyObject*)>  PyStaticFunction;
  

  class PyFunctionWrapper {
    public:
      inline            PyFunctionWrapper ();
      virtual PyObject* call              ( PyObject* ) = 0;
  };
  
  inline PyFunctionWrapper::PyFunctionWrapper() { }


  template< typename CppT >
  class PyFunctionWrapperAny : public PyFunctionWrapper {
    public:
      inline            PyFunctionWrapperAny( CppT* (*function)(void) );
      virtual PyObject* call                ( PyObject* );
    private:
      CppT* (*_function)(void);
  };

  
  template< typename CppT >
  inline PyFunctionWrapperAny<CppT>::PyFunctionWrapperAny( CppT* (*function)(void) )
    : PyFunctionWrapper(), _function(function)
  { }

  
  template< typename CppT >
  inline PyObject* PyFunctionWrapperAny<CppT>::call ( PyObject* )
  {
    std::cerr << "call called" << endl;
    return PyTypeManager::link<CppT>( _function() );
  }


  inline PyObject* staticMethod ( const char*        methodName
                                , PyFunctionWrapper* wrapper
                                , PyObject*          args )
  {
    cdebug_log(30,0) << methodName << endl;
    std::cerr << "staticMethod called on " << methodName << endl;
    PyObject* robject = NULL;
    try {
      robject = wrapper->call( args );
    }
    catch ( const Warning& w ) {
      std::string message = getString(w);
      PyErr_Warn( HurricaneWarning, const_cast<char*>(message.c_str()) );
    }
    catch ( const Error& e ) {
      std::string message = getString(e);
      if (not e.where().empty()) message += "n" + e.where();
      PyErr_SetString( HurricaneError, message.c_str() );
    }
    catch ( const Bug& e ) {
      std::string message = getString(e);
      PyErr_SetString( HurricaneError, message.c_str() );
    }
    catch ( const Exception& e ) {
      std::string message = "Unknown Hurricane::Exception";
      PyErr_SetString( HurricaneError, message.c_str() );
    }
    catch ( const std::exception& e )  {
      std::string message = std::string(e.what());
      PyErr_SetString( HurricaneError, message.c_str() );
    }
    catch ( ... ) {
      std::string message = "Unmanaged exception, neither a Hurricane::Error nor a std::exception.";
      PyErr_SetString( HurricaneError, message.c_str() );
    }

    return robject;
  }


extern "C" {


  extern PyObject* ConstructorError;
  extern PyObject* ProxyError;
  extern PyObject* HurricaneError;
  extern PyObject* HurricaneWarning;
  extern PyObject* CoriolisError;

  

}  // extern "C".

}  // Isobar namespace.
