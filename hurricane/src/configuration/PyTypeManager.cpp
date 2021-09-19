// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Universite 2020-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyTypeManager.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyTypeManager.h"


namespace Isobar3 {


  extern "C" {

    PyObject* ConstructorError = NULL;
    PyObject* ProxyError       = NULL;
    PyObject* HurricaneError   = NULL;
    PyObject* HurricaneWarning = NULL;


    void  _tpDeAlloc ( PyObject* self )
    { PyTypeManager::get( Py_TYPE(self) )->_getTpDeAlloc( asVPtr(self) ); }

    
    PyObject* _tpStr ( PyObject* self )
    { return PyTypeManager::get( Py_TYPE(self) )->_getTpStr( asVPtr(self) ); }

    
    long  _tpHash ( PyObject* self )
    { return PyTypeManager::get( Py_TYPE(self) )->_getTpHash( asVPtr(self) ); }


    Py_ssize_t  _sqLength ( PyVoidPointer* self ) 
    { return PyTypeManager::get( Py_TYPE(self) )->_getSqLength( self ); }


    PyObject* _sqConcat ( PyVoidPointer* a, PyVoidPointer* b ) 
    { return PyTypeManager::get( Py_TYPE(a) )->_getSqConcat( a, b ); }


    PyObject* _sqRepeat ( PyVoidPointer* a, Py_ssize_t count ) 
    { return PyTypeManager::get( Py_TYPE(a) )->_getSqRepeat( a, count ); }


    PyObject* _sqItem ( PyVoidPointer* self, Py_ssize_t count ) 
    { return PyTypeManager::get( Py_TYPE(self) )->_getSqItem( self, count ); }


    int  _sqContains ( PyObject* self, PyObject* item ) 
    { return PyTypeManager::get( Py_TYPE(self) )->_getSqContains( self, item ); }


    Py_ssize_t  _mpLength ( PyObject* self ) 
    { return PyTypeManager::get( Py_TYPE(self) )->_getMpLength( self ); }


    PyObject* _mpSubscript ( PyObject* self, PyObject* key ) 
    { return PyTypeManager::get( Py_TYPE(self) )->_getMpSubscript( self, key ); }


    PyObject* _tpIter ( PyObject* self ) 
    { return PyTypeManager::get( Py_TYPE(self) )->_getTpIter( self ); }


    PyObject* _tpIterNext ( PyObject* self ) 
    { return PyTypeManager::get( Py_TYPE(self) )->_getTpIterNext( self ); }

    
  }


  BaseArg::~BaseArg ()
  { }


// -------------------------------------------------------------------
// PyTypeObject & PyObject wrapper / manager.

  
  std::map< std::type_index, PyTypeManager* >  PyTypeManager::_managerByCppTypes;
  std::map< PyTypeObject*  , PyTypeManager* >  PyTypeManager::_managerByPyTypes;


  PyTypeManager::~PyTypeManager ()
  { }


  void  PyTypeManager::_setTypeNames ( std::string className )
  {
    // cerr << "_setTypeNames(): " << className << " -> ";
    size_t cppScope = className.find_first_of( "::" );
    if (cppScope != std::string::npos) className = className.substr( cppScope+2 );
    //cerr << "| " << className << endl;
    size_t subClass = className.find_last_of( "::" );
    if (subClass != std::string::npos) className = className.replace( subClass-1, 2, "_" );
    //cerr << "| " << className << endl;

    _cppTypeName = className;
    _pyTypeName  = "Py"+_cppTypeName;
    // cerr << _cppTypeName << "/" << _pyTypeName << endl;
  }

  
  Py_ssize_t PyTypeManager::_getSqLength ( PyVoidPointer* )
  {
    throw Error( "PyTypeManager::_getSqLength(): Not implemented on <%s>.", _getCppTypeName().c_str() );
    return 0;
  }

  
  PyObject* PyTypeManager::_getSqConcat ( PyVoidPointer*, PyVoidPointer* )
  {
    throw Error( "PyTypeManager::_getSqConcat(): Not implemented on <%s>.", _getCppTypeName().c_str() );
    return NULL;
  }

  
  PyObject* PyTypeManager::_getSqRepeat ( PyVoidPointer*, Py_ssize_t )
  {
    throw Error( "PyTypeManager::_getSqRepeat(): Not implemented on <%s>.", _getCppTypeName().c_str() );
    return NULL;
  }

  
  PyObject* PyTypeManager::_getSqItem ( PyVoidPointer*, Py_ssize_t )
  {
    throw Error( "PyTypeManager::_getSqItem(): Not implemented on <%s>.", _getCppTypeName().c_str() );
    return NULL;
  }

  
  int  PyTypeManager::_getSqContains ( PyObject*, PyObject* )
  {
    throw Error( "PyTypeManager::_getSqContains(): Not implemented on <%s>.", _getCppTypeName().c_str() );
    return -1;
  }
  

  Py_ssize_t  PyTypeManager::_getMpLength ( PyObject* )
  {
    throw Error( "PyTypeManager::_getMpItem(): Not implemented on <%s>.", _getCppTypeName().c_str() );
    return 0;
  }

  
  PyObject* PyTypeManager::_getMpSubscript ( PyObject*, PyObject* )
  {
    throw Error( "PyTypeManager::_getMpSubscript(): Not implemented on <%s>.", _getCppTypeName().c_str() );
    return NULL;
  }

  
  PyObject* PyTypeManager::_getTpIter ( PyObject* )
  {
    throw Error( "PyTypeManager::_getTpIter(): Not implemented on <%s>.", _getCppTypeName().c_str() );
    return NULL;
  }

  
  PyObject* PyTypeManager::_getTpIterNext ( PyObject* )
  {
    throw Error( "PyTypeManager::_getTpIterNext(): Not implemented on <%s>.", _getCppTypeName().c_str() );
    return NULL;
  }


  void  PyTypeManager::_setupPyType ()
  {
    PyTypeObject* ob_type = _getTypeObject();
    ob_type->tp_name        = _getPyTypeName().c_str();
    ob_type->tp_dealloc     = (destructor) &::Isobar3::_tpDeAlloc;
    ob_type->tp_str         = (reprfunc)   &::Isobar3::_tpStr;
    ob_type->tp_hash        = (hashfunc)   &::Isobar3::_tpHash;
    ob_type->tp_methods     = _getMethods();
    ob_type->tp_getset      = _getGetsets();
    // cerr << "_setupPyType() on \"" << _getPyTypeName()
    //      << "\" ob_type=" << (void*)ob_type << endl;
  }


  void  PyTypeManager::_addToModule ( PyObject* module )
  {
    if (PyType_Ready(_getTypeObject()) < 0) {
      cerr << "[ERROR] Failed to initialize <" << _getPyTypeName() << ">." << endl;
      return;
    }

    if (PyModule_Check(module)) {
      Py_INCREF( _getTypeObject() );
      PyModule_AddObject( module
                        , _getCppTypeName().c_str()
                        , (PyObject*)_getTypeObject() );
    } else if (PyType_Check(module)) {
      PyDict_SetItemString( ((PyTypeObject*)module)->tp_dict
                          , _getCppTypeName().c_str()
                          , (PyObject*)_getTypeObject() );
    } else {
      throw Error( "PyTypeManager::_addToModule(): \"module\" argument is neither a module nor a type" );
    }
  }

  
// -------------------------------------------------------------------
// PyWrapper, C++ function & function member exception wrapper.
  

  PyWrapper::~PyWrapper ()
  { }


  PyObject* PyWrapper::call ( PyObject* self, PyObject* args )
  {
    throw Error( "PyWrapper::call(PyObject*,PyObject*): Base class method must never be called." );
    return NULL;
  }


  PyObject* PyWrapper::call ( PyObject* args )
  {
    throw Error( "PyWrapper::call(PyObject*): Base class method must never be called." );
    return NULL;
  }


  int  PyWrapper::predicate ( PyObject* args )
  {
    throw Error( "PyWrapper::predicate(PyObject*): Base class method must never be called." );
    return 0;
  }


  PyObject* exceptionWrapper ( PyWrapper& wrapper, PyObject* self, PyObject* args )
  {
    try {
      return wrapper.call( self, args );
    } catch ( const Warning& w ) {
      wrapper.message() += "\n" + getString(w);
    } catch ( const Error& e ) {
      wrapper.message() += "\n" + getString(e);
      if (not e.where().empty()) wrapper.message() += "\n" + e.where();
    } catch ( const Bug& e ) {
      wrapper.message() += "\n" + getString(e);
    } catch ( const Exception& e ) {
      wrapper.message() += "\nUnknown Hurricane::Exception";
    } catch ( const std::exception& e )  {
      wrapper.message() += "\n" + std::string(e.what());
    } catch ( ... ) {
      wrapper.message() += "\nUnmanaged exception, neither a Hurricane::Error nor"
                           " a std::exception.";
    }
    PyErr_SetString( HurricaneError, wrapper.message().c_str() );
    return NULL;
  }


  PyObject* exceptionWrapper ( PyWrapper& wrapper, PyObject* args )
  {
    try {
      return wrapper.call( args );
    } catch ( const Warning& w ) {
      wrapper.message() += "\n" + getString(w);
    } catch ( const Error& e ) {
      wrapper.message() += "\n" + getString(e);
      if (not e.where().empty()) wrapper.message() += "\n" + e.where();
    } catch ( const Bug& e ) {
      wrapper.message() += "\n" + getString(e);
    } catch ( const Exception& e ) {
      wrapper.message() += "\nUnknown Hurricane::Exception";
    } catch ( const std::exception& e )  {
      wrapper.message() += "\n" + std::string(e.what());
    } catch ( ... ) {
      wrapper.message() += "\nUnmanaged exception, neither a Hurricane::Error nor"
                           " a std::exception.";
    }
    PyErr_SetString( HurricaneError, wrapper.message().c_str() );
    return NULL;
  }


  int  exceptionPredicateWrapper ( PyWrapper& wrapper, PyObject* self )
  {
    try {
      return wrapper.predicate( self );
    } catch ( const Warning& w ) {
      wrapper.message() += "\n" + getString(w);
    } catch ( const Error& e ) {
      wrapper.message() += "\n" + getString(e);
      if (not e.where().empty()) wrapper.message() += "\n" + e.where();
    } catch ( const Bug& e ) {
      wrapper.message() += "\n" + getString(e);
    } catch ( const Exception& e ) {
      wrapper.message() += "\nUnknown Hurricane::Exception";
    } catch ( const std::exception& e )  {
      wrapper.message() += "\n" + std::string(e.what());
    } catch ( ... ) {
      wrapper.message() += "\nUnmanaged exception, neither a Hurricane::Error nor"
                           " a std::exception.";
    }
    PyErr_SetString( HurricaneError, wrapper.message().c_str() );
    return 0;
  }


}  // Isobar namespace.
