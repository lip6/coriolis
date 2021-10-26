
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
// |  C++ Header  :  "./hurricane/isobar/PyCollection.h"             |
// +-----------------------------------------------------------------+


#pragma  once

namespace Isobar3 {

  
// -------------------------------------------------------------------
// PyTypeObject & PyObject for Locator<T*> objects.


  template< typename CppT >
  class PyTypeManagerLocator : public PyTypeManagerVTrunk< typename Hurricane::Locator<CppT*> > {
    public:
      using PyTypeManager::_getTypeObject;
      using PyTypeManager::_setMethods;
    public:
      inline        PyTypeManagerLocator ( uint64_t flags );
      virtual      ~PyTypeManagerLocator ();
    public:
      static  PyTypeManagerLocator* create         ( PyObject* module, uint64_t flags );
      virtual PyObject*             _getTpIter     ( PyObject* );
      virtual PyObject*             _getTpIterNext ( PyObject* );
      virtual void                  _getTpDeAlloc  ( PyObject* );
      virtual long                  _getTpHash     ( PyObject* );
    private:
      PyMethodDef* _noMethods;
  };
  

  template< typename CppT >
  inline PyTypeManagerLocator<CppT>::PyTypeManagerLocator ( uint64_t flags )
    : PyTypeManagerVTrunk< typename Hurricane::Locator<CppT*> >(NULL,NULL,flags|PyTypeManager::IsIterator)
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


  template< typename CppT >
  PyTypeManagerLocator<CppT>::~PyTypeManagerLocator () { }

    
  template< typename CppT >
  void  PyTypeManagerLocator<CppT>::_getTpDeAlloc ( PyObject* self )
  {
    Py_XDECREF( object2(self) );
    typename Hurricane::Locator<CppT*>* plocator = NULL;
    pyToC( (PyObject*)self, &plocator );
    delete plocator;
    PyObject_DEL( self );
  }

    
  template< typename CppT >
  PyObject* PyTypeManagerLocator<CppT>::_getTpIter ( PyObject* self )
  {
    return PyObject_SelfIter( self );
  }

    
  template< typename CppT >
  PyObject* PyTypeManagerLocator<CppT>::_getTpIterNext ( PyObject* self )
  {
    typename Hurricane::Locator   <CppT*>* plocator    = NULL;
    typename Hurricane::Collection<CppT*>* pcollection = NULL;
    pyToC( self, &plocator );
    pyToC( (PyObject*)asIPtr(self)->_object2, &pcollection );
                                                                                             
  //std::cerr << "Collection::Iterator::_getTpIterNext() on " << (void*)pcollection << std::endl;
    if (plocator and plocator->isValid()) {
      PyObject* item = cToPy( plocator->getElement() );
      plocator->progress();
      return item;
    }
    return NULL;
  }


  template< typename CppT >
  long  PyTypeManagerLocator<CppT>::_getTpHash ( PyObject* self )
  { return (long)object1(self); }


  template< typename CppT >
  PyTypeManagerLocator<CppT>* PyTypeManagerLocator<CppT>::create ( PyObject* module, uint64_t flags )
  {
    // cerr << "PyTypeManagerCollection<"
    //      << ::Hurricane::demangle(typeid(typename Hurricane::Locator<CppT*>)) << ">::create()" << endl;
    PyTypeManagerLocator<CppT>* manager = new PyTypeManagerLocator<CppT>( flags );

    string elementName = ::Hurricane::demangle(typeid(CppT));
    size_t cppScope    = elementName.find_last_of( "::" );
    if (cppScope != std::string::npos) elementName = elementName.substr( cppScope+1 );

    manager->_setTypeNames( "LocatorOf" + elementName );
    manager->_setupPyType();
    PyTypeObject* ob_type = manager->_getTypeObject();
    ob_type->tp_iter     = PyObject_SelfIter;
    ob_type->tp_iternext = (iternextfunc)&::Isobar3::_tpIterNext;

    PyTypeManager::add< typename Hurricane::Locator<CppT*> >( module, manager );

    return manager;
  }

  
// -------------------------------------------------------------------
// PyTypeObject & PyObject for vector<T*> objects.


  template< typename CppT >
  class PyTypeManagerCollection : public PyTypeManagerVTrunk< Hurricane::Collection<CppT*> > {
    public:
      using PyTypeManager::_getTypeObject;
      using PyTypeManager::_setMethods;
    public:
      inline        PyTypeManagerCollection ( uint64_t flags );
      virtual      ~PyTypeManagerCollection ();
    public:
      static  PyTypeManagerCollection* create       ( PyObject* module, uint64_t flags );
      virtual void                    _getTpDeAlloc ( PyObject* );
      virtual long                    _getTpHash    ( PyObject* );
      virtual PyObject*               _getTpIter    ( PyObject* );
    private:
      PyMethodDef*       _noMethods;
  };
  

  template< typename CppT >
  inline PyTypeManagerCollection<CppT>::PyTypeManagerCollection ( uint64_t flags )
    : PyTypeManagerVTrunk< Hurricane::Collection<CppT*> >(NULL,NULL,flags)
    , _noMethods( NULL )
  {
    _noMethods = new PyMethodDef;
    _noMethods[0].ml_name  = NULL;
    _noMethods[0].ml_meth  = NULL;
    _noMethods[0].ml_flags = 0;
    _noMethods[0].ml_doc   = NULL;
    _setMethods( _noMethods );
  }


  template< typename CppT >
  PyTypeManagerCollection<CppT>::~PyTypeManagerCollection () { }

    
  template< typename CppT >
  void  PyTypeManagerCollection<CppT>::_getTpDeAlloc ( PyObject* self )
  {
    Hurricane::Collection<CppT*>* pcollection = NULL;
    pyToC( self, &pcollection );
  //std::cerr << "PyCollection::_getTpDeAlloc() on " << (void*)pcollection << std::endl;
    delete pcollection;
    PyObject_DEL( self );
  }


  template< typename CppT >
  long  PyTypeManagerCollection<CppT>::_getTpHash ( PyObject *self )
  { return (long)object1(self); }

    
  template< typename CppT >
  PyObject* PyTypeManagerCollection<CppT>::_getTpIter ( PyObject* self )
  {
    Hurricane::Collection<CppT*>* pcollection = NULL;
    pyToC( self, &pcollection );
    PyTwoVoid* pyLocator = (PyTwoVoid*)cToPy( pcollection->getLocator() );
    pyLocator->_object2 = self;
    Py_INCREF( self );
  //std::cerr << "Collection::_getTpIter() on " << (void*)pcollection << std::endl;
    return (PyObject*)pyLocator;
  }


  template< typename CppT >
  PyTypeManagerCollection<CppT>* PyTypeManagerCollection<CppT>::create ( PyObject* module, uint64_t flags )
  {
    // cerr << "PyTypeManagerCollection<"
    //      << ::Hurricane::demangle(typeid(Hurricane::Collection<CppT*>)) << ">::create()" << endl;
    PyTypeManagerCollection<CppT>* manager = new PyTypeManagerCollection<CppT>( flags );

    string elementName = ::Hurricane::demangle(typeid(CppT));
    size_t cppScope    = elementName.find_last_of( "::" );
    if (cppScope != std::string::npos) elementName = elementName.substr( cppScope+1 );

    manager->_setTypeNames( "CollectionOf" + elementName );
    manager->_setupPyType();
    PyTypeObject* ob_type = manager->_getTypeObject();
    ob_type->tp_iter      = (getiterfunc)&::Isobar3::_tpIter;

    std::cerr << "Add to manager: <"
              << ::Hurricane::demangle(typeid( Hurricane::Collection<CppT*> )) << ">" << std::endl;
    PyTypeManager::add< Hurricane::Collection<CppT*> >
      ( module, manager, typeid(Hurricane::Collection<CppT*>).hash_code() );
    PyTypeManagerLocator<CppT>::create( module, flags );

    return manager;
  }


}  // Isobar3 namespace.

