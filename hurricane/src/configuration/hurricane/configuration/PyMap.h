
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Universite 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Authors     :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyMap.h"                    |
// +-----------------------------------------------------------------+


#pragma  once

namespace Isobar3 {

  
// -------------------------------------------------------------------
// PyTypeObject & PyObject for map<T*,K*>::iterator objects.


  template< typename CppK, typename CppT >
  class PyTypeManagerMapIterator : public PyTypeManagerVTrunk< typename std::map<CppK,CppT>::iterator > {
    public:
      using PyTypeManager::_getTypeObject;
      using PyTypeManager::_setMethods;
    public:
      inline        PyTypeManagerMapIterator ( uint64_t flags );
      virtual      ~PyTypeManagerMapIterator ();
    public:
      static  PyTypeManagerMapIterator* create         ( PyObject* module, uint64_t flags );
      virtual PyObject*                 _getTpIter     ( PyObject* );
      virtual PyObject*                 _getTpIterNext ( PyObject* );
      virtual void                      _getTpDeAlloc  ( PyObject* );
      virtual long                      _getTpHash     ( PyObject* );
    private:
      PyMethodDef* _noMethods;
  };
  

  template< typename CppK, typename CppT >
  inline PyTypeManagerMapIterator<CppK,CppT>::PyTypeManagerMapIterator ( uint64_t flags )
    : PyTypeManagerVTrunk< typename std::map<CppK,CppT>::iterator >(NULL,NULL,flags|PyTypeManager::IsIterator)
    , _noMethods( NULL )
  {
    _getTypeObject()->tp_basicsize = sizeof(PyTwoVoid);
    _noMethods = new PyMethodDef;
    _noMethods[0].ml_name  = NULL;
    _noMethods[0].ml_meth  = NULL;
    _noMethods[0].ml_flags = 0;
    _noMethods[0].ml_doc   = NULL;
    _setMethods( _noMethods );
  }


  template< typename CppK, typename CppT >
  PyTypeManagerMapIterator<CppK,CppT>::~PyTypeManagerMapIterator () { }

    
  template< typename CppK, typename CppT >
  void  PyTypeManagerMapIterator<CppK,CppT>::_getTpDeAlloc ( PyObject* self )
  {
    Py_XDECREF( object2(self) );
    typename std::map<CppK,CppT>::iterator* piterator = NULL;
    pyToC( self, &piterator );
    delete piterator;
    PyObject_DEL( self );
  }

    
  template< typename CppK, typename CppT >
  PyObject* PyTypeManagerMapIterator<CppK,CppT>::_getTpIter ( PyObject* self )
  {
    // std::cerr << "**************************************************  " << (void*)self << std::endl;
    // std::cerr << "PyTypeManagerMapIterator<CppT>::_getTpIter() on " << (void*)self << std::endl;
    return PyObject_SelfIter( self );
  }

    
  template< typename CppK, typename CppT >
  PyObject* PyTypeManagerMapIterator<CppK,CppT>::_getTpIterNext ( PyObject* self )
  {
    typename std::map<CppK,CppT>::iterator* piterator = NULL;
    typename std::map<CppK,CppT>*           pmap      = NULL;
    pyToC( self, &piterator );
    pyToC( object2(self), &pmap );

    if ((*piterator) != pmap->end()) {
      PyObject* pyKey   = cToPy( &((**piterator).first ) );
      PyObject* pyValue = NULL;
      if (std::is_pointer<CppT>::value) pyValue = cToPy(   (**piterator).second  );
      else                              pyValue = cToPy( &((**piterator).second) );
      (*piterator)++;
      return PyTuple_Pack( 2, pyKey, pyValue );
    }
    return NULL;
  }


  template< typename CppK, typename CppT >
  long  PyTypeManagerMapIterator<CppK,CppT>::_getTpHash ( PyObject *self )
  { return (long)(object1(self)); }


  template< typename CppK, typename CppT >
  PyTypeManagerMapIterator<CppK,CppT>* PyTypeManagerMapIterator<CppK,CppT>::create ( PyObject* module, uint64_t flags )
  {
    // cerr << "PyTypeManagerMap<"
    //      << ::Hurricane::demangle(typeid(typename std::map<CppK,CppT>::iterator)) << ">::create()" << endl;
    PyTypeManagerMapIterator<CppK,CppT>* manager = new PyTypeManagerMapIterator<CppK,CppT>( flags );

    string elementName = ::Hurricane::demangle(typeid(CppT));
    size_t cppScope    = elementName.find_last_of( "::" );
    if (cppScope != std::string::npos) elementName = elementName.substr( cppScope+1 );
    string keyName = ::Hurricane::demangle(typeid(CppK));
    cppScope       = keyName.find_last_of( "::" );
    if (cppScope != std::string::npos) keyName = keyName.substr( cppScope+1 );

    manager->_setTypeNames( "MapIteratorOf" + elementName + "By" + keyName );
    manager->_setupPyType();
    PyTypeObject* ob_type = manager->_getTypeObject();
    ob_type->tp_iter     = PyObject_SelfIter;
    ob_type->tp_iternext = (iternextfunc)&::Isobar3::_tpIterNext;

    PyTypeManager::add< typename std::map<CppK,CppT>::iterator >( module, manager );

    return manager;
  }

  
// -------------------------------------------------------------------
// PyTypeObject & PyObject for map<T*,K*> objects.


  template< typename CppK, typename CppT >
  class PyTypeManagerMap : public PyTypeManagerVTrunk< std::map<CppK,CppT> > {
    public:
      using PyTypeManager::_getTypeObject;
      using PyTypeManager::_setMethods;
    public:
      inline   PyTypeManagerMap ( uint64_t flags );
      virtual ~PyTypeManagerMap ();
    public:
      static  PyTypeManagerMap* create          ( PyObject* module, uint64_t flags );
      virtual int               _getSqContains  ( PyObject*, PyObject* );
      virtual Py_ssize_t        _getMpLength    ( PyObject* );
      virtual PyObject*         _getMpSubscript ( PyObject*, PyObject* );
      virtual void              _getTpDeAlloc   ( PyObject* );
      virtual long              _getTpHash      ( PyObject* );
      virtual PyObject*         _getTpIter      ( PyObject* );
    private:
      PyMethodDef*       _noMethods;
      PyMappingMethods   _mappingMethods;
      PySequenceMethods  _sequenceMethods;
  };
  

  template< typename CppK, typename CppT >
  inline PyTypeManagerMap<CppK,CppT>::PyTypeManagerMap ( uint64_t flags )
    : PyTypeManagerVTrunk< std::map<CppK,CppT> >(NULL,NULL,flags)
    , _noMethods      ( NULL )
    , _mappingMethods { (lenfunc)     _mpLength     // mp_length         (lenfunc).
                      , (binaryfunc)  _mpSubscript  // mp_subsript       (binaryfunc).
                      , NULL                        // mp_ass_subscript  (objobjargproc).
                      }
    , _sequenceMethods{}
  {
    _noMethods = new PyMethodDef;
    _noMethods[0].ml_name  = NULL;
    _noMethods[0].ml_meth  = NULL;
    _noMethods[0].ml_flags = 0;
    _noMethods[0].ml_doc   = NULL;
    _setMethods( _noMethods );
    _sequenceMethods.sq_contains = (objobjproc)_sqContains;
  }


  template< typename CppK, typename CppT >
  PyTypeManagerMap<CppK,CppT>::~PyTypeManagerMap () { }

    
  template< typename CppK, typename CppT >
  void  PyTypeManagerMap<CppK,CppT>::_getTpDeAlloc ( PyObject* self )
  {
    PyObject_DEL( self );
  }


  template< typename CppK, typename CppT >
  int  PyTypeManagerMap<CppK,CppT>::_getSqContains ( PyObject* self, PyObject* pyKey )
  {
    std::map<CppK,CppT>* pmap = NULL;
    if (not pyToC(self,&pmap)) {
      std::string message = "PyTypeManagerMap<CppK,CppT>::_getMpSubscript(): pyToC<> failed. \".";
      PyErr_SetString( PyExc_KeyError, message.c_str() );
      return -1;
    }
    CppK key;
    if (not pyToC( pyKey, &key )) return 0;
    if ((*pmap).find(key) == (*pmap).end()) return 0;
    return 1;
  }
    

  template< typename CppK, typename CppT >
  Py_ssize_t  PyTypeManagerMap<CppK,CppT>::_getMpLength ( PyObject* self )
  {
    std::map<CppK,CppT>* pmap = reinterpret_cast< std::map<CppK,CppT>* >( object1(self) );
    return pmap->size();
  }

    
  template< typename CppK, typename CppT >
  PyObject* PyTypeManagerMap<CppK,CppT>::_getMpSubscript ( PyObject* self, PyObject* pyKey )
  {
    std::map<CppK,CppT>* pmap = NULL;
    if (not pyToC(self,&pmap)) {
      std::string message = "PyTypeManagerMap<CppK,CppT>::_getMpSubscript(): pyToC<> failed. \".";
      PyErr_SetString( PyExc_KeyError, message.c_str() );
      return NULL;
    }
    CppK key;
    if (not pyToC( pyKey, &key )) {
      std::string message = "PyTypeManagerMap<CppK,CppT>::_getMpSubscript(): Unable to convert key.";
      PyErr_SetString( HurricaneError, message.c_str() );
      return NULL;
    }
    // std::cerr << "_getMpSubscript() pmap=" << (void*)pmap << " key=\"" << key << "\"" << std::endl;
    if ((*pmap).find(key) == (*pmap).end()) {
      std::string message = "PyTypeManagerMap<CppK,CppT>::_getMpSubscript(): No item with key \""
                          + getString(key) + "\".";
      PyErr_SetString( PyExc_KeyError, message.c_str() );
      return NULL;
    }
    return cToPy( (*pmap)[key] );
  }


  template< typename CppK, typename CppT >
  long  PyTypeManagerMap<CppK,CppT>::_getTpHash ( PyObject* self )
  { return (long)(object1(self)); }

    
  template< typename CppK, typename CppT >
  PyObject* PyTypeManagerMap<CppK,CppT>::_getTpIter ( PyObject* self )
  {
    std::map<CppK,CppT>* pmap = NULL;
    pyToC( self, &pmap );
    PyTwoVoid* pyIterator = (PyTwoVoid*)cToPy
      ( new typename std::map<CppK,CppT>::iterator(pmap->begin()) );
    object2(pyIterator) = self;
    Py_INCREF( self );
    return (PyObject*)pyIterator;
  }


  template< typename CppK, typename CppT >
  PyTypeManagerMap<CppK,CppT>* PyTypeManagerMap<CppK,CppT>::create ( PyObject* module, uint64_t flags )
  {
    // cerr << "PyTypeManagerMap<"
    //      << ::Hurricane::demangle(typeid(std::map<CppK,CppT>)) << ">::create()" << endl;
    PyTypeManagerMap<CppK,CppT>* manager = new PyTypeManagerMap<CppK,CppT>( flags );

    string elementName = ::Hurricane::demangle(typeid(CppT));
    size_t cppScope    = elementName.find_last_of( "::" );
    if (cppScope != std::string::npos) elementName = elementName.substr( cppScope+1 );
    string keyName = ::Hurricane::demangle(typeid(CppK));
    cppScope       = keyName.find_last_of( "::" );
    if (cppScope != std::string::npos) keyName = keyName.substr( cppScope+1 );

    manager->_setTypeNames( "Map<" + keyName + "," + elementName + ">" );
    manager->_setupPyType();
    PyTypeObject* ob_type = manager->_getTypeObject();
    ob_type->tp_as_sequence = &(manager->_sequenceMethods);
    ob_type->tp_as_mapping  = &(manager->_mappingMethods);
    ob_type->tp_iter        = (getiterfunc)&::Isobar3::_tpIter;

    PyTypeManager::add< std::map<CppK,CppT> >( module, manager );
    PyTypeManagerMapIterator<CppK,CppT>::create( module, flags );

    return manager;
  }


}  // Isobar3 namespace.

