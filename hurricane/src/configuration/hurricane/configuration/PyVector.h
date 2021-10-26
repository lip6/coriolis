
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
// |  C++ Header  :  "./hurricane/isobar/PyVector.h"                 |
// +-----------------------------------------------------------------+


#pragma  once

namespace Isobar3 {

  
// -------------------------------------------------------------------
// PyTypeObject & PyObject for vector<T*>::iterator objects.


  template< typename CppT >
  class PyTypeManagerVectorIterator : public PyTypeManagerVTrunk< typename std::vector<CppT>::iterator > {
    public:
      using PyTypeManager::_getTypeObject;
      using PyTypeManager::_setMethods;
    public:
      inline        PyTypeManagerVectorIterator ( uint64_t flags );
      virtual      ~PyTypeManagerVectorIterator ();
    public:
      static  PyTypeManagerVectorIterator* create         ( PyObject* module, uint64_t flags );
      virtual PyObject*                    _getTpIter     ( PyObject* );
      virtual PyObject*                    _getTpIterNext ( PyObject* );
      virtual void                         _getTpDeAlloc  ( PyObject* );
      virtual long                         _getTpHash     ( PyObject* );
    private:
      PyMethodDef* _noMethods;
  };
  

  template< typename CppT >
  inline PyTypeManagerVectorIterator<CppT>::PyTypeManagerVectorIterator ( uint64_t flags )
    : PyTypeManagerVTrunk< typename std::vector<CppT>::iterator >(NULL,NULL,flags|PyTypeManager::IsIterator)
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
  PyTypeManagerVectorIterator<CppT>::~PyTypeManagerVectorIterator () { }

    
  template< typename CppT >
  void  PyTypeManagerVectorIterator<CppT>::_getTpDeAlloc ( PyObject* self )
  {
    Py_XDECREF( object2(self) );
    typename std::vector<CppT>::iterator* piterator = NULL;
    pyToC( self, &piterator );
    delete piterator;
    PyObject_DEL( self );
  }

    
  template< typename CppT >
  PyObject* PyTypeManagerVectorIterator<CppT>::_getTpIter ( PyObject* self )
  {
    // std::cerr << "**************************************************  " << (void*)self << std::endl;
    // std::cerr << "PyTypeManagerVectorIterator<CppT>::_getTpIter() on " << (void*)self << std::endl;
    return PyObject_SelfIter( self );
  }

    
  template< typename CppT >
  PyObject* PyTypeManagerVectorIterator<CppT>::_getTpIterNext ( PyObject* self )
  {
    // std::cerr << "**************************************************  " << (void*)self << std::endl;
    // std::cerr << "PyTypeManagerVectorIterator<CppT>::_getTpIterNext() on " << (void*)self << std::endl;
    typename std::vector<CppT>::iterator* piterator = NULL;
    typename std::vector<CppT>* pvector = NULL;
    pyToC( self, &piterator );
    pyToC( object2(self), &pvector );
                                                                                             
    if ((*piterator) != pvector->end()) {
      PyObject* item = cToPy( **piterator );
      (*piterator)++;
      return item;
    }
    // std::cerr << "  End reached" << std::endl;
    return NULL;
  }


  template< typename CppT >
  long  PyTypeManagerVectorIterator<CppT>::_getTpHash ( PyObject *self )
  { return (long)(object1(self)); }


  template< typename CppT >
  PyTypeManagerVectorIterator<CppT>* PyTypeManagerVectorIterator<CppT>::create ( PyObject* module, uint64_t flags )
  {
    // cerr << "PyTypeManagerVector<"
    //      << ::Hurricane::demangle(typeid(typename std::vector<CppT>::iterator)) << ">::create()" << endl;
    PyTypeManagerVectorIterator<CppT>* manager = new PyTypeManagerVectorIterator<CppT>( flags );

    string elementName = ::Hurricane::demangle(typeid(CppT));
    size_t cppScope    = elementName.find_last_of( "::" );
    if (cppScope != std::string::npos) elementName = elementName.substr( cppScope+1 );

    manager->_setTypeNames( "VectorIteratorOf" + elementName );
    manager->_setupPyType();
    PyTypeObject* ob_type = manager->_getTypeObject();
    ob_type->tp_iter     = PyObject_SelfIter;
    ob_type->tp_iternext = (iternextfunc)&::Isobar3::_tpIterNext;

    PyTypeManager::add< typename std::vector<CppT>::iterator >( module, manager );

    return manager;
  }

  
// -------------------------------------------------------------------
// PyTypeObject & PyObject for vector<T*> objects.


  template< typename CppT >
  class PyTypeManagerVector : public PyTypeManagerVTrunk< std::vector<CppT> > {
    public:
      using PyTypeManager::_getTypeObject;
      using PyTypeManager::_setMethods;
    public:
      inline        PyTypeManagerVector ( uint64_t flags );
      virtual      ~PyTypeManagerVector ();
    public:
      static  PyTypeManagerVector* create        ( PyObject* module, uint64_t flags );
      virtual Py_ssize_t           _getSqLength  ( PyObject* );
      virtual PyObject*            _getSqItem    ( PyObject*, Py_ssize_t );
      virtual void                 _getTpDeAlloc ( PyObject* );
      virtual long                 _getTpHash    ( PyObject* );
      virtual PyObject*            _getTpIter    ( PyObject* );
    private:
      PyMethodDef*       _noMethods;
      PySequenceMethods  _sequenceMethods;
  };
  

  template< typename CppT >
  inline PyTypeManagerVector<CppT>::PyTypeManagerVector ( uint64_t flags )
    : PyTypeManagerVTrunk< std::vector<CppT> >(NULL,NULL,flags)
    , _noMethods      ( NULL )
    , _sequenceMethods{ (lenfunc)     _sqLength  // sq_length         (lenfunc).
                      , (binaryfunc)  _sqConcat  // sq_concat         (binaryfunc).
                      , (ssizeargfunc)_sqRepeat  // sq_repeat         (ssizeargfunc).
                      , (ssizeargfunc)_sqItem    // sq_item           (ssizeargfunc).
                      , NULL                     // sq_sq_ass_item    (ssizeobjargproc).
                      , NULL                     // sq_contains       (objobjproc).
                      , NULL                     // sq_inplace_concat (binaryfunc).
                      , NULL                     // sq_inplace_repeat (ssizeargfunc).
                      }
  {
    _noMethods = new PyMethodDef;
    _noMethods[0].ml_name  = NULL;
    _noMethods[0].ml_meth  = NULL;
    _noMethods[0].ml_flags = 0;
    _noMethods[0].ml_doc   = NULL;
    _setMethods( _noMethods );
  }


  template< typename CppT >
  PyTypeManagerVector<CppT>::~PyTypeManagerVector () { }

    
  template< typename CppT >
  void  PyTypeManagerVector<CppT>::_getTpDeAlloc ( PyObject* self )
  {
    PyObject_DEL( self );
  }

    
  template< typename CppT >
  Py_ssize_t  PyTypeManagerVector<CppT>::_getSqLength ( PyObject* self )
  {
    std::vector<CppT>* pvector = NULL;
    pyToC( self, &pvector );
    return pvector->size();
  }

    
  template< typename CppT >
  PyObject* PyTypeManagerVector<CppT>::_getSqItem ( PyObject* self, Py_ssize_t index )
  {
    std::vector<CppT>* pvector = NULL;
    pyToC( self, &pvector );
    return cToPy( (*pvector)[index] );
  }


  template< typename CppT >
  long  PyTypeManagerVector<CppT>::_getTpHash ( PyObject *self )
  { return (long)( object1(self) ); }

    
  template< typename CppT >
  PyObject* PyTypeManagerVector<CppT>::_getTpIter ( PyObject* self )
  {
    // std::cerr << "**************************************************  " << (void*)self << std::endl;
    // std::cerr << "PyTypeManagerVector<CppT>::_getTpIter() on " << (void*)self << std::endl;
    std::vector<CppT>* pvector = NULL;
    pyToC( self, &pvector );
    PyTwoVoid* pyIterator = (PyTwoVoid*)cToPy
      ( new typename std::vector<CppT>::iterator(pvector->begin()) );
    object2(pyIterator) = self;
    Py_INCREF( self );
    // std::cerr << "First iterator:" << (void*)pyIterator << std::endl;
    return (PyObject*)pyIterator;
  }


  template< typename CppT >
  PyTypeManagerVector<CppT>* PyTypeManagerVector<CppT>::create ( PyObject* module, uint64_t flags )
  {
    // cerr << "PyTypeManagerVector<"
    //      << ::Hurricane::demangle(typeid(std::vector<CppT>)) << ">::create()" << endl;
    PyTypeManagerVector<CppT>* manager = new PyTypeManagerVector<CppT>( flags );

    string elementName = ::Hurricane::demangle(typeid(CppT));
    size_t cppScope    = elementName.find_last_of( "::" );
    if (cppScope != std::string::npos) elementName = elementName.substr( cppScope+1 );

    manager->_setTypeNames( "VectorOf" + elementName );
    manager->_setupPyType();
    PyTypeObject* ob_type = manager->_getTypeObject();
    ob_type->tp_as_sequence = &(manager->_sequenceMethods);
    ob_type->tp_iter        = (getiterfunc)&::Isobar3::_tpIter;

    PyTypeManager::add< std::vector<CppT> >( module, manager );
    PyTypeManagerVectorIterator<CppT>::create( module, flags );

    return manager;
  }


}  // Isobar3 namespace.

