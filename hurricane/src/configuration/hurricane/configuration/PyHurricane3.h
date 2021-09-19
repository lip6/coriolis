// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
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


// Enable Python debugging.
// #define  DEBUG  1

#pragma  once
#include "Python.h"
#include <cstddef>
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
#include "hurricane/configuration/ProxyProperty.h"

namespace Isobar3 {


  using namespace std;
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
  extern PyObject* CoriolisError;


  typedef struct {
      PyObject_HEAD
      void* _object;
  } PyVoidPointer;


  typedef struct {
      PyObject_HEAD
      void*          _object;    // pointer to iterator.
      PyVoidPointer* _container;
  } PyIteratorPointer;


  extern void        _tpDeAlloc     ( PyObject* );
  extern PyObject*   _tpStr         ( PyObject* );
  extern long        _tpHash        ( PyObject* );
  extern PyObject*   _tpRichCompare ( PyObject*, PyObject*, int );
  extern Py_ssize_t  _sqLength      ( PyVoidPointer* ); 
  extern PyObject*   _sqConcat      ( PyVoidPointer* , PyVoidPointer*  ); 
  extern PyObject*   _sqRepeat      ( PyVoidPointer* , Py_ssize_t );
  extern PyObject*   _sqItem        ( PyVoidPointer* , Py_ssize_t );
  extern int         _sqContains    ( PyObject*, PyObject* );
  extern Py_ssize_t  _mpLength      ( PyObject* ); 
  extern PyObject*   _mpSubscript   ( PyObject*, PyObject* ); 
  extern Py_ssize_t  _mpLength      ( PyObject* ); 
  extern PyObject*   _tpIter        ( PyObject* );
  extern PyObject*   _tpIterNext    ( PyObject* );


} // "C" linkage.


// -------------------------------------------------------------------
// PyTypeObject post-creation default hook.


  inline void  addConstant ( PyTypeObject* typeObject, std::string name, long value )
  {
    PyObject* constant = PyLong_FromLong( value );
    PyDict_SetItemString( typeObject->tp_dict, name.c_str(), constant );
    Py_DECREF( constant );
  }
  

  template< typename CppT >
  inline void  pyTypePostModuleInit ( PyTypeObject* typeObject )
  {
  //cerr << "pyTypePostModuleInit<CppT>() default for <"
  //      << demangle(typeid(CppT).name()) << ">." << endl;
  }


// -------------------------------------------------------------------
// Basic PyTypeObject & PyObject wrapper / manager.


  inline PyVoidPointer*     asVPtr ( PyObject*      o ) { return reinterpret_cast<PyVoidPointer    *>(o); }
  inline PyIteratorPointer* asIPtr ( PyObject*      o ) { return reinterpret_cast<PyIteratorPointer*>(o); }
  inline PyIteratorPointer* asIPtr ( PyVoidPointer* o ) { return reinterpret_cast<PyIteratorPointer*>(o); }


  class PyTypeManager {
    public:
      typedef std::map<std::type_index,PyTypeManager*>  ManagerByCppTypes;
      typedef std::map<PyTypeObject*  ,PyTypeManager*>  ManagerByPyTypes;
    public:
      static const uint64_t  NoFlags     =  0;
      static const uint64_t  IsDBo       = (1 << 0);
      static const uint64_t  IsIterator  = (1 << 0);
      static const uint64_t  NoCppDelete = (1 << 1);
    public:
      inline   PyTypeManager ( PyMethodDef*, PyGetSetDef*, uint64_t flags );
      virtual ~PyTypeManager ();
    public:
                                      inline  bool               _isDBo            () const;
                                      inline  bool               _isIterator       () const;
                                      inline  std::string        _getCppTypeName   () const;
                                      inline  std::string        _getPyTypeName    () const;
                                      inline  PyTypeObject*      _getTypeObject    ();
                                      inline  PyMethodDef*       _getMethods       ();
                                      inline  PyGetSetDef*       _getGetsets       ();
                                              void               _setTypeNames     ( std::string className );
                                              void               _addToModule      ( PyObject* );
                                      virtual void               _getTpDeAlloc     ( PyVoidPointer* ) = 0;
                                      virtual PyObject*          _getTpStr         ( PyVoidPointer* ) = 0;
                                      virtual long               _getTpHash        ( PyVoidPointer* ) = 0;
                                      virtual PyObject*          _getTpRichCompare ( PyObject*, PyObject*, int );
                                      virtual Py_ssize_t         _getSqLength      ( PyVoidPointer* );
                                      virtual PyObject*          _getSqConcat      ( PyVoidPointer*, PyVoidPointer* );
                                      virtual PyObject*          _getSqRepeat      ( PyVoidPointer*, Py_ssize_t );
                                      virtual PyObject*          _getSqItem        ( PyVoidPointer*, Py_ssize_t );
                                      virtual int                _getSqContains    ( PyObject*, PyObject* );
                                      virtual Py_ssize_t         _getMpLength      ( PyObject* );
                                      virtual PyObject*          _getMpSubscript   ( PyObject*, PyObject* );
                                      virtual PyObject*          _getTpIter        ( PyObject* );
                                      virtual PyObject*          _getTpIterNext    ( PyObject* );
                                      virtual void               _setupPyType      ();
                                      inline  void               _setMethods       ( PyMethodDef* );
    public:                                   
                              static  inline  ManagerByCppTypes& getManagerByCppTypes ();
                                      inline  uint64_t           flags           () const;
                              static  inline  PyTypeManager*     get             ( PyTypeObject* );
      template<typename CppT> static  inline  PyTypeManager*     _get            ();
      template<typename CppT> static  inline  PyObject*          link            ( CppT* );
      template<typename CppT> static          void               add             ( PyObject*, PyTypeManager* );
    private:
      uint64_t      _flags;
      std::string   _pyTypeName;
      std::string   _cppTypeName;
      PyTypeObject  _typeObject;
      PyMethodDef*  _methods;
      PyGetSetDef*  _getsets;
    private:
      static ManagerByCppTypes  _managerByCppTypes;
      static ManagerByPyTypes   _managerByPyTypes;
  };


  inline PyTypeManager* PyTypeManager::get ( PyTypeObject* obType )
  {
    auto element = _managerByPyTypes.find( obType );
    if (element == _managerByPyTypes.end())
      throw Error( "PyTypeManager::get(PyTypeObject*): Unregistered type <%s>."
                 , obType->tp_name );
    return (*element).second;
  }


  template<typename CppT> inline PyTypeManager* PyTypeManager::_get ()
  {
    auto element = _managerByCppTypes.find( std::type_index(typeid(CppT)) );
    if (element == _managerByCppTypes.end())
      throw Error( "PyTypeManager<CppT>::_get(): Unregistered type <%s>."
                 , demangle(typeid(CppT).name()).c_str() );
    return (*element).second;
  }


  inline  PyTypeManager::PyTypeManager ( PyMethodDef *methods, PyGetSetDef* getsets, uint64_t flags )
    : _flags      (flags)
    , _pyTypeName ()
    , _cppTypeName()
    , _typeObject { PyVarObject_HEAD_INIT(&PyType_Type,0)
                    .tp_name        = NULL
                  , .tp_basicsize   = sizeof(PyVoidPointer)
                  , .tp_itemsize    = 0
                  , .tp_dealloc     = NULL
                  , .tp_print       = NULL
                  , .tp_getattr     = NULL
                  , .tp_setattr     = NULL
                  , .tp_as_async    = NULL
                  , .tp_repr        = NULL
                  , .tp_as_number   = NULL
                  , .tp_as_sequence = NULL
                  , .tp_as_mapping  = NULL
                  , .tp_hash        = NULL
                  , .tp_call        = NULL
                  , .tp_str         = NULL
                  , .tp_getattro    = NULL
                  , .tp_setattro    = NULL
                  , .tp_as_buffer   = NULL
                  , .tp_flags       = Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE
                  , .tp_doc         = "Created through PyTypeManager"
                  }
    , _methods    (methods)
    , _getsets    (getsets)
  {
    if (_isIterator()) _typeObject.tp_basicsize = sizeof(PyIteratorPointer);
  }


  inline bool               PyTypeManager::_isDBo               () const { return _flags & IsDBo; }
  inline bool               PyTypeManager::_isIterator          () const { return _flags & IsIterator; }
  inline std::string        PyTypeManager::_getCppTypeName      () const { return _cppTypeName; }
  inline std::string        PyTypeManager::_getPyTypeName       () const { return _pyTypeName; }
  inline PyTypeObject*      PyTypeManager::_getTypeObject       () { return &_typeObject; }
  inline PyMethodDef*       PyTypeManager::_getMethods          () { return _methods; }
  inline PyGetSetDef*       PyTypeManager::_getGetsets          () { return _getsets; }
  inline uint64_t           PyTypeManager::flags                () const { return _flags; }
  inline void               PyTypeManager::_setMethods          ( PyMethodDef* methods ) { _methods=methods; }
  inline PyTypeManager::ManagerByCppTypes&
                            PyTypeManager::getManagerByCppTypes () { return _managerByCppTypes; }


  template< typename CppT >
  void  PyTypeManager::add ( PyObject* module, PyTypeManager* manager )
  {
    if (ProxyProperty::getOffset() == (size_t)-1)
      ProxyProperty::setOffset( offsetof( PyVoidPointer, _object ));

    // std::cerr << "PyTypeManager::add<" << demangle(typeid(CppT).name())
    //           << "> hash=" << std::type_index(typeid(CppT)).hash_code()
    //           << " manager=" << manager << std::endl;
    _managerByCppTypes[ std::type_index(typeid(CppT)) ] = manager;
    _managerByPyTypes [ manager->_getTypeObject()     ] = manager;
    manager->_addToModule( module );
    pyTypePostModuleInit<CppT>( manager->_getTypeObject() );
    // std::cerr << "_managerByCppTypes.size()=" << _managerByCppTypes.size() << std::endl;
  }


// -------------------------------------------------------------------
// PyTypeObject & PyObject wrapper / manager.


  template<typename CppT>
  class PyTypeManagerVTrunk : public PyTypeManager {
    public:
      inline   PyTypeManagerVTrunk ( PyMethodDef*, PyGetSetDef*, uint64_t flags );
      virtual ~PyTypeManagerVTrunk ();
    public:
      virtual  PyObject* _getTpStr ( PyVoidPointer* );
      virtual  PyObject* _link     ( CppT* );
  };
  

  template< typename CppT >
  inline PyTypeManagerVTrunk<CppT>::PyTypeManagerVTrunk ( PyMethodDef* methods, PyGetSetDef* getsets, uint64_t flags )
    : PyTypeManager(methods,getsets,flags)
  { }
  

  template< typename CppT >
  PyTypeManagerVTrunk<CppT>::~PyTypeManagerVTrunk () { }


  template< typename CppT >
  PyObject* PyTypeManagerVTrunk<CppT>::_getTpStr ( PyVoidPointer *self )
  {
    CppT* object = reinterpret_cast<CppT*>( self->_object );
    if (not object) {
      std::ostringstream repr;
      repr << "<" << _getPyTypeName() <<  " [" << (void*)self << " <-> NULL] unbound>";
      return PyUnicode_FromString( repr.str().c_str() );
    }
    return PyUnicode_FromString(getString(object).c_str() );
  }


  template< typename CppT >
  PyObject* PyTypeManagerVTrunk<CppT>::_link ( CppT* object )
  {
    if (not object) Py_RETURN_NONE;

    PyVoidPointer* pyObject = NULL;
    if (_isIterator())
      pyObject = (PyVoidPointer*)PyObject_NEW( PyIteratorPointer, _getTypeObject() );
    else
      pyObject = PyObject_NEW( PyVoidPointer, _getTypeObject() );
    if (pyObject == NULL) return NULL;

    pyObject->_object = (void*)object;
    cdebug_log(20,0) << "PyTypeManager::_link(): " << _getPyTypeName() << " "
                     << (void*)pyObject << ") " << (void*)object << ":" << object << endl;

    return (PyObject*)pyObject;
  }


// PyTypeManager methods requiring PyTypeManagerVTrunk.
  
  template< typename CppT >
  inline PyObject* PyTypeManager::link ( CppT* object )
  {
    // std::cerr << "PyTypeManager<CppT>::link() " << demangle(typeid(CppT).name())
    //           << "* object = " << (void*)object << ":" << object << std::endl;
    if (not object) Py_RETURN_NONE;
    PyTypeManagerVTrunk<CppT>* manager = dynamic_cast< PyTypeManagerVTrunk<CppT>* >( _get<CppT>() );
    // std::cerr << "_get<CppT>()=" << _get<CppT>() << endl;
    // std::cerr << demangle(typeid(PyTypeManagerVTrunk<CppT>*).name()) << endl;
    if (not manager)
      throw Error( "PyTypeManager<CppT>::link(): No manager for type <%s>."
                 , demangle(typeid(CppT).name()).c_str() );
    return manager->_link( object );
  }


// -------------------------------------------------------------------
// PyTypeObject & PyObject for non-DBo objects.


  template<typename CppT>
  class PyTypeManagerNonDBo : public PyTypeManagerVTrunk<CppT> {
    public:
      inline        PyTypeManagerNonDBo ( PyMethodDef*, PyGetSetDef*, uint64_t flags );
      virtual      ~PyTypeManagerNonDBo ();
    public:
      static  PyTypeManagerNonDBo* create            ( PyObject*    module
                                                     , PyMethodDef* methods
                                                     , PyGetSetDef* getsets
                                                     , uint64_t     flags );
      virtual void                 _getTpDeAlloc     ( PyVoidPointer* );
      virtual long                 _getTpHash        ( PyVoidPointer* );
      virtual PyObject*            _getTpRichCompare ( PyObject*, PyObject*, int );
  };
  

  template< typename CppT >
  inline PyTypeManagerNonDBo<CppT>::PyTypeManagerNonDBo ( PyMethodDef* methods, PyGetSetDef* getsets, uint64_t flags )
    : PyTypeManagerVTrunk<CppT>(methods,getsets,flags)
  { }


  template< typename CppT >
  PyTypeManagerNonDBo<CppT>::~PyTypeManagerNonDBo () { }

    
  template< typename CppT >
  void  PyTypeManagerNonDBo<CppT>::_getTpDeAlloc ( PyVoidPointer* self )
  {
    if (not (PyTypeManager::flags() & PyTypeManager::NoCppDelete))
      delete reinterpret_cast<CppT*>( self->_object );
    PyObject_DEL( self );
  }


  template< typename CppT >
  long  PyTypeManagerNonDBo<CppT>::_getTpHash ( PyVoidPointer *self )
  { return (long)(self->_object); }


  template< typename CppT >
  PyObject* PyTypeManagerNonDBo<CppT>::_getTpRichCompare ( PyObject *self, PyObject* other, int op )
  {
    if (Py_TYPE(self) != Py_TYPE(other)) {
      std::string message = "PyTypeManagerNonDBo<" + getString(demangle(typeid(CppT).name()))
                          + "> cannot compare to \"" + Py_TYPE(other)->tp_name + "\".";
      PyErr_SetString( PyExc_TypeError, message.c_str() );
      return NULL;
    }
    CppT* cself  = reinterpret_cast<CppT*>( asVPtr(self )->_object );
    CppT* cother = reinterpret_cast<CppT*>( asVPtr(other)->_object );

    bool result = false;
    if ((op == Py_LT) and (cself <  cother)) result = true;
    if ((op == Py_LE) and (cself <= cother)) result = true;
    if ((op == Py_EQ) and (cself == cother)) result = true;
    if ((op == Py_NE) and (cself != cother)) result = true;
    if ((op == Py_GT) and (cself >  cother)) result = true;
    if ((op == Py_GE) and (cself >= cother)) result = true;
    
    if (result) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  template< typename CppT >
  PyTypeManagerNonDBo<CppT>* PyTypeManagerNonDBo<CppT>::create ( PyObject*    module
                                                               , PyMethodDef* methods
                                                               , PyGetSetDef* getsets
                                                               , uint64_t     flags )
  {
    PyTypeManagerNonDBo<CppT>* manager = new PyTypeManagerNonDBo<CppT>( methods, getsets, flags );
    manager->_setTypeNames( ::Hurricane::demangle(typeid(CppT)) );
    manager->_setupPyType();
    PyTypeManager::add<CppT>( module, manager );
    return manager;
  }


// -------------------------------------------------------------------
// PyTypeObject & PyObject for DBo objects.


  template<typename CppT>
  class PyTypeManagerDBo : public PyTypeManagerVTrunk<CppT> {
    public:
      using         PyTypeManager::_getTypeObject;
      inline        PyTypeManagerDBo ( PyMethodDef*, PyGetSetDef*, uint64_t flags );
      virtual      ~PyTypeManagerDBo ();
    public:
      static  PyTypeManagerDBo* create            ( PyObject*    module
                                                  , PyMethodDef* methods
                                                  , PyGetSetDef* getsets
                                                  , uint64_t     flags );
                                                  
      virtual PyObject*         _link             ( CppT* );
      virtual void              _getTpDeAlloc     ( PyVoidPointer* );
      virtual long              _getTpHash        ( PyVoidPointer* );
      virtual PyObject*         _getTpRichCompare ( PyObject*, PyObject*, int );
  };
  

  template< typename CppT >
  inline PyTypeManagerDBo<CppT>::PyTypeManagerDBo ( PyMethodDef* methods, PyGetSetDef* getsets, uint64_t flags )
    : PyTypeManagerVTrunk<CppT>(methods,getsets,flags|PyTypeManager::IsDBo)
  { }


  template< typename CppT >
  PyTypeManagerDBo<CppT>::~PyTypeManagerDBo () { }


  template< typename CppT >
  PyObject* PyTypeManagerDBo<CppT>::_link( CppT* object )
  {
    if (not object) Py_RETURN_NONE;

    PyVoidPointer* pyObject = NULL;
    ProxyProperty* proxy = static_cast<ProxyProperty*>
      ( object->getProperty ( ProxyProperty::getPropertyName() ) );

    if (not proxy) {
      pyObject = PyObject_NEW( PyVoidPointer, _getTypeObject() );
      if (not pyObject) return NULL;

      proxy = ProxyProperty::create( (void*)pyObject );
      pyObject->_object = (void*)object;
      object->put( proxy );
    } else {
      pyObject = (PyVoidPointer*)proxy->getShadow ();
      Py_INCREF( pyObject );
    }
    cdebug_log(20,0) << "PyTypeManagerDBo<" << demangle(typeid(CppT).name()) << ">() "
                     << (void*)pyObject << " <-> "
                     << (void*)object << ":" << object << endl;
    return (PyObject*)pyObject;
  }
    

  template< typename CppT >
  void  PyTypeManagerDBo<CppT>::_getTpDeAlloc ( PyVoidPointer* self )
  {
    DBo* object = reinterpret_cast<DBo*>( self->_object );
    cdebug_log(20,0) << "PyTypeManager_DeAlloc(" << (void*)self << ") "
                     << (void*)(self->_object) << ":" << object << endl;

    Isobar3::ProxyProperty* proxy =
      static_cast<Isobar3::ProxyProperty*>( object->getProperty( Isobar3::ProxyProperty::getPropertyName() ) );

    if (not proxy) {
      ostringstream  message;
      message << "deleting a Python object with no Proxy attached ";
      PyErr_SetString( ProxyError, message.str().c_str() );
    }
    object->remove ( proxy );
    
    PyObject_DEL( self );
  }


  template< typename CppT >
  long PyTypeManagerDBo<CppT>::_getTpHash ( PyVoidPointer *self )
  {
    DBo* object = reinterpret_cast<DBo*>( self->_object );
    return object->getId();
  }


  template< typename CppT >
  PyObject* PyTypeManagerDBo<CppT>::_getTpRichCompare ( PyObject *self, PyObject* other, int op )
  {
    if (Py_TYPE(self) != Py_TYPE(other)) {
      std::string message = "PyTypeManagerDBo<" + getString(demangle(typeid(CppT).name()))
                          + "> cannot compare to \"" + Py_TYPE(other)->tp_name + "\".";
      PyErr_SetString( PyExc_TypeError, message.c_str() );
      return NULL;
    }
    CppT* cself  = reinterpret_cast<CppT*>( asVPtr(self )->_object );
    CppT* cother = reinterpret_cast<CppT*>( asVPtr(other)->_object );

    bool result = false;
    if ((op == Py_LT) and (cself->getId() <  cother->getId())) result = true;
    if ((op == Py_LE) and (cself->getId() <= cother->getId())) result = true;
    if ((op == Py_EQ) and (cself->getId() == cother->getId())) result = true;
    if ((op == Py_NE) and (cself->getId() != cother->getId())) result = true;
    if ((op == Py_GT) and (cself->getId() >  cother->getId())) result = true;
    if ((op == Py_GE) and (cself->getId() >= cother->getId())) result = true;
    
    if (result) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  template< typename CppT >
  PyTypeManagerDBo<CppT>* PyTypeManagerDBo<CppT>::create ( PyObject*    module
                                                         , PyMethodDef* methods
                                                         , PyGetSetDef* getsets
                                                         , uint64_t     flags )
  {
    if (std::is_base_of<Hurricane::Entity,CppT>::value)
      throw ::Hurricane::Error( "PyManagerDBo<CppT>::create(): The C++ class <%s> is *not* derived from DBo."
                              , ::Hurricane::demangle(typeid(CppT).name()) );
    
    PyTypeManagerDBo<CppT>* manager = new PyTypeManagerDBo   <CppT>( methods, getsets, flags );
    manager->_setTypeNames( ::Hurricane::demangle(typeid(CppT)) );
    manager->_setupPyType();
    PyTypeManager::add<CppT>( module, manager );
    return manager;
  }


// -------------------------------------------------------------------
// Top level link template.
  

  template< typename CppT >
  inline PyObject* objectLink ( CppT object )
  {
    std::string message = "Overload for Isobar3::objectLink< "
                        + demangle(typeid(CppT).name()) + " >() is missing.";
    // std::cerr << message << std::endl;
    PyErr_SetString( HurricaneError, message.c_str() );
    return NULL;
  }
  

  template< typename CppT
          , typename std::enable_if<   !std::is_same<CppT,bool>::value
                                    && !std::is_same<CppT,int >::value
                                    && !std::is_same<CppT,std::string>::value
                                    && !std::is_same<CppT,const std::string>::value,bool>::type = true >
  inline PyObject* objectLink ( CppT* object )
  { return PyTypeManager::link<CppT>( object ); }
  

  template< typename CppT
          , typename std::enable_if<   !std::is_same<CppT,bool>::value
                                    && !std::is_same<CppT,int >::value
                                    && !std::is_same<CppT,std::string>::value
                                    && !std::is_same<CppT,const std::string>::value,bool>::type = true >
  inline PyObject* objectLink ( const CppT* object )
  { return PyTypeManager::link<CppT>( const_cast<CppT*>( object )); }
  

  template<>
  inline PyObject* objectLink<bool> ( bool b )
  { if (b) Py_RETURN_TRUE; Py_RETURN_FALSE; }
  

  template<> inline PyObject* objectLink<      std::string>  (       std::string  s ) { return PyUnicode_FromString( s.c_str() ); }
  template<> inline PyObject* objectLink<const std::string > ( const std::string  s ) { return PyUnicode_FromString( s.c_str() ); }
  template<> inline PyObject* objectLink<const std::string*> ( const std::string* s ) { return PyUnicode_FromString( s->c_str() ); }

  template<> inline PyObject* objectLink<      int > (       int  i ) { return PyLong_FromLong(  i ); }
  template<> inline PyObject* objectLink<const int > ( const int  i ) { return PyLong_FromLong(  i ); }
  template<> inline PyObject* objectLink<const int*> ( const int* i ) { return PyLong_FromLong( *i ); }

  template<> inline PyObject* objectLink<      long > (       long  l ) { return PyLong_FromLong(  l ); }
  template<> inline PyObject* objectLink<const long > ( const long  l ) { return PyLong_FromLong(  l ); }
  template<> inline PyObject* objectLink<const long*> ( const long* l ) { return PyLong_FromLong( *l ); }

  template<> inline PyObject* objectLink<      float > (       float  f ) { return PyFloat_FromDouble(  f ); }
  template<> inline PyObject* objectLink<const float > ( const float  f ) { return PyFloat_FromDouble(  f ); }
  template<> inline PyObject* objectLink<const float*> ( const float* f ) { return PyFloat_FromDouble( *f ); }

  template<> inline PyObject* objectLink<      double > (       double  d ) { return PyFloat_FromDouble(  d ); }
  template<> inline PyObject* objectLink<const double > ( const double  d ) { return PyFloat_FromDouble(  d ); }
  template<> inline PyObject* objectLink<const double*> ( const double* d ) { return PyFloat_FromDouble( *d ); }


// Forward declaration for PyVector.h
  template< typename CppT > inline PyObject* objectLink ( const typename std::vector<CppT>::iterator* );
  template< typename CppT > inline PyObject* objectLink ( const std::vector<CppT>& );

// Forward declaration for PyMap.h
  template< typename CppK, typename CppT > inline PyObject* objectLink ( const typename std::map<CppK,CppT>::iterator* );
  template< typename CppK, typename CppT > inline PyObject* objectLink ( const std::map<CppK,CppT>& vobject );

// Forward declaration for PyCollection.h
  template< typename CppT > inline PyObject* objectLink ( const typename Hurricane::Locator<CppT>* );
  template< typename CppT > inline PyObject* objectLink ( const Hurricane::Collection<CppT>* );


}  // Isobar3 namespace.


namespace Isobar3 {
  
// -------------------------------------------------------------------
// Wrap C++ function call to pass arguments through a tuple<>.

  
// Flavor for "return by value" (seems to match std::is_object<>)
  template< typename TR, typename... TArgs
          , typename std::enable_if<   !std::is_reference<TR>::value
                                    && !std::is_pointer  <TR>::value
                                    && !std::is_void     <TR>::value,bool>::type = true >
  inline PyObject* _callFunctionReturn ( TR(* func)(TArgs...), TArgs... args )
  {
    TR value = func( args... );
    return objectLink( value );
  }

  
// Flavor for "return by reference".
  template< typename TR, typename... TArgs
          , typename std::enable_if<    std::is_reference<TR>::value
                                    && !std::is_pointer  <TR>::value
                                    && !std::is_void     <TR>::value,bool>::type = true >
  inline PyObject* _callFunctionReturn ( TR(* func)(TArgs...), TArgs... args )
  {
    TR rvalue = func( args... );
    return objectLink( rvalue );
  }

  
// Flavor for "return by pointer".
  template< typename TR, typename... TArgs
          , typename std::enable_if<std::is_pointer<TR>::value,bool>::type = true >
  inline PyObject* _callFunctionReturn ( TR(* func)(TArgs...), TArgs... args )
  {
    TR pvalue = func( args... );
    return objectLink( pvalue );
  }

  
// Flavor for "return void".
  template< typename TR, typename... TArgs
          , typename std::enable_if<std::is_void<TR>::value,bool>::type = true >
  inline PyObject* _callFunctionReturn ( TR(* func)(TArgs...), TArgs... args )
  {
    func( args... );
    Py_RETURN_NONE;
  }
  

  template< typename TR >
  PyObject* _callFunction ( TR(* func)(), std::tuple<> args )
  { return _callFunctionReturn( func ); }

  
  template< typename TR, typename TA0 >
  PyObject* _callFunction ( TR(* func)(TA0), std::tuple<TA0> args )
  { return _callFunctionReturn( func, std::get<0>(args) ); }

  
  template< typename TR, typename TA0, typename TA1 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1), std::tuple<TA0,TA1> args )
  { return _callFunctionReturn( func, std::get<0>(args), std::get<1>(args) ); }

  
  template< typename TR, typename TA0, typename TA1, typename TA2 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2), std::tuple<TA0,TA1,TA2> args )
  { return _callFunctionReturn( func, std::get<0>(args), std::get<1>(args), std::get<2>(args) ); }

  
  template< typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3), std::tuple<TA0,TA1,TA2,TA3> args )
  { return _callFunctionReturn( func, std::get<0>(args)
                                    , std::get<1>(args)
                                    , std::get<2>(args)
                                    , std::get<3>(args) ); }


  template< typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3,TA4), std::tuple<TA0,TA1,TA2,TA3,TA4> args )
  { return _callFunctionReturn( func, std::get<0>(args)
                                    , std::get<1>(args)
                                    , std::get<2>(args)
                                    , std::get<3>(args)
                                    , std::get<4>(args) ); }


  template< typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3,TA4,TA5), std::tuple<TA0,TA1,TA2,TA3,TA4,TA5> args )
  { return _callFunctionReturn( func, std::get<0>(args)
                                    , std::get<1>(args)
                                    , std::get<2>(args)
                                    , std::get<3>(args)
                                    , std::get<4>(args)
                                    , std::get<5>(args) ); }


  template< typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3,TA4,TA5,TA6), std::tuple<TA0,TA1,TA2,TA3,TA4,TA5,TA6> args )
  { return _callFunctionReturn( func, std::get<0>(args)
                                    , std::get<1>(args)
                                    , std::get<2>(args)
                                    , std::get<3>(args)
                                    , std::get<4>(args)
                                    , std::get<5>(args)
                                    , std::get<6>(args) ); }


  template< typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6
          , typename TA7 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7), std::tuple<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7> args )
  { return _callFunctionReturn( func, std::get<0>(args)
                                    , std::get<1>(args)
                                    , std::get<2>(args)
                                    , std::get<3>(args)
                                    , std::get<4>(args)
                                    , std::get<5>(args)
                                    , std::get<6>(args)
                                    , std::get<7>(args) ); }


  template< typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6
          , typename TA7
          , typename TA8 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8), std::tuple<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8> args )
  { return _callFunctionReturn( func, std::get<0>(args)
                                    , std::get<1>(args)
                                    , std::get<2>(args)
                                    , std::get<3>(args)
                                    , std::get<4>(args)
                                    , std::get<5>(args)
                                    , std::get<6>(args)
                                    , std::get<7>(args)
                                    , std::get<8>(args) ); }


  template< typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6
          , typename TA7
          , typename TA8
          , typename TA9 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8,TA9), std::tuple<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8,TA9> args )
  { return _callFunctionReturn( func, std::get<0>(args)
                                    , std::get<1>(args)
                                    , std::get<2>(args)
                                    , std::get<3>(args)
                                    , std::get<4>(args)
                                    , std::get<5>(args)
                                    , std::get<6>(args)
                                    , std::get<7>(args)
                                    , std::get<8>(args)
                                    , std::get<9>(args) ); }


// -------------------------------------------------------------------
// Wrap C++ member method call to pass arguments through a tuple<>.

  
// Flavor for "return by value" (seems to match std::is_object<>)
  template< typename TC, typename TR, typename... TArgs
          , typename std::enable_if<   !std::is_reference<TR>::value
                                    && !std::is_pointer  <TR>::value
                                    && !std::is_void     <TR>::value,bool>::type = true >
  inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgs...), TC* cppObject, TArgs... args )
  {
    // std::cerr << "_callMethodReturn() By value, TR=<" << demangle(typeid(TR).name()) << ">" << std::endl;
    TR value = (cppObject->*method)( args... );
    return objectLink( value );
  }

  
// Flavor for "return by reference"
  template< typename TC, typename TR, typename... TArgs
          , typename std::enable_if<    std::is_reference<TR>::value
                                    && !std::is_pointer  <TR>::value
                                    && !std::is_void     <TR>::value,bool>::type = true >
  inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgs...), TC* cppObject, TArgs... args )
  {
    // std::cerr << "_callMethodReturn() By reference, TR=<" << demangle(typeid(TR).name()) << ">" << std::endl;
    TR rvalue = (cppObject->*method)( args... );
    return objectLink( rvalue );
  }

  
// Flavor for "return by pointer".
  template< typename TC, typename TR, typename... TArgs
          , typename std::enable_if<std::is_pointer<TR>::value,bool>::type = true >
  inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgs...), TC* cppObject, TArgs... args )
  {
    TR pvalue = (cppObject->*method)( args... );
    return objectLink( pvalue );
  }

  
// Flavor for "return void".
  template< typename TC, typename TR, typename... TArgs
          , typename std::enable_if<std::is_void<TR>::value,bool>::type = true >
  inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgs...), TC* cppObject, TArgs... args )
  {
    (cppObject->*method)( args... );
    Py_RETURN_NONE;
  }

  
  template< typename TC, typename TR >
  inline PyObject* _callMethod ( TR(TC::* method)(), TC* cppObject, std::tuple<> )
  { return _callMethodReturn<TC,TR>( method, cppObject ); }

  
  template< typename TC, typename TR, typename TA0 >
  inline PyObject* _callMethod ( TR(TC::* method)(TA0), TC* cppObject, std::tuple<TA0> args )
  { return _callMethodReturn( method, cppObject, std::get<0>(args) ); }

  
  template< typename TC, typename TR, typename TA0, typename TA1 >
  PyObject* _callMethod ( TR(TC::* method)(TA0,TA1), TC* cppObject, std::tuple<TA0,TA1> args )
  { return _callMethodReturn( method, cppObject, std::get<0>(args), std::get<1>(args) ); }

  
  template< typename TC, typename TR, typename TA0, typename TA1, typename TA2 >
  inline PyObject* _callMethod ( TR(TC::* method)(TA0,TA1,TA2), TC* cppObject, std::tuple<TA0,TA1,TA2> args )
  { return _callMethodReturn( method, cppObject, std::get<0>(args), std::get<1>(args), std::get<2>(args) ); }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3 >
  inline PyObject* _callMethod (          TR(TC::* method)(TA0,TA1,TA2,TA3)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3> args )
  {
    return _callMethodReturn( method, cppObject, std::get<0>(args)
                                               , std::get<1>(args)
                                               , std::get<2>(args)
                                               , std::get<3>(args) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4 >
  inline PyObject* _callMethod (          TR(TC::* method)(TA0,TA1,TA2,TA3,TA4)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3,TA4> args )
  {
    return _callMethodReturn( method, cppObject, std::get<0>(args)
                                               , std::get<1>(args)
                                               , std::get<2>(args)
                                               , std::get<3>(args)
                                               , std::get<4>(args) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5 >
  inline PyObject* _callMethod (          TR(TC::* method)(TA0,TA1,TA2,TA3,TA4,TA5)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3,TA4,TA5> args )
  {
    return _callMethodReturn( method, cppObject, std::get<0>(args)
                                               , std::get<1>(args)
                                               , std::get<2>(args)
                                               , std::get<3>(args)
                                               , std::get<4>(args)
                                               , std::get<5>(args) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6 >
  inline PyObject* _callMethod (          TR(TC::* method)(TA0,TA1,TA2,TA3,TA4,TA5,TA6)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3,TA4,TA5,TA6> args )
  {
    return _callMethodReturn( method, cppObject, std::get<0>(args)
                                               , std::get<1>(args)
                                               , std::get<2>(args)
                                               , std::get<3>(args)
                                               , std::get<4>(args)
                                               , std::get<5>(args)
                                               , std::get<6>(args) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6
          , typename TA7 >
  inline PyObject* _callMethod (          TR(TC::* method)(TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7> args )
  {
    return _callMethodReturn( method, cppObject, std::get<0>(args)
                                               , std::get<1>(args)
                                               , std::get<2>(args)
                                               , std::get<3>(args)
                                               , std::get<4>(args)
                                               , std::get<5>(args)
                                               , std::get<6>(args)
                                               , std::get<7>(args) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6
          , typename TA7
          , typename TA8 >
  inline PyObject* _callMethod (          TR(TC::* method)(TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8> args )
  {
    return _callMethodReturn( method, cppObject, std::get<0>(args)
                                               , std::get<1>(args)
                                               , std::get<2>(args)
                                               , std::get<3>(args)
                                               , std::get<4>(args)
                                               , std::get<5>(args)
                                               , std::get<6>(args)
                                               , std::get<7>(args)
                                               , std::get<8>(args) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6
          , typename TA7
          , typename TA8
          , typename TA9 >
  inline PyObject* _callMethod (          TR(TC::* method)(TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8,TA9)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8,TA9> args )
  {
    return _callMethodReturn( method, cppObject, std::get<0>(args)
                                               , std::get<1>(args)
                                               , std::get<2>(args)
                                               , std::get<3>(args)
                                               , std::get<4>(args)
                                               , std::get<5>(args)
                                               , std::get<6>(args)
                                               , std::get<7>(args)
                                               , std::get<8>(args)
                                               , std::get<9>(args) );
  }


// -------------------------------------------------------------------
// Wrap C++ member method call as function to pass arguments through a tuple<>.

  
// Flavor for "return by value" (seems to match std::is_object<>)
  template< typename TC, typename TR, typename... TArgs
          , typename std::enable_if<   !std::is_reference<TR>::value
                                    && !std::is_pointer  <TR>::value
                                    && !std::is_void     <TR>::value,bool>::type = true >
  inline PyObject* _callFMethodReturn ( TR(* fmethod)(TC*,TArgs...), TC* cppObject, TArgs... args )
  {
    TR value = fmethod( cppObject, args... );
    return objectLink( value );
  }

  
// Flavor for "return by reference"
  template< typename TC, typename TR, typename... TArgs
          , typename std::enable_if<    std::is_reference<TR>::value
                                    && !std::is_pointer  <TR>::value
                                    && !std::is_void     <TR>::value,bool>::type = true >
  inline PyObject* _callFMethodReturn ( TR(* fmethod)(TC*,TArgs...), TC* cppObject, TArgs... args )
  {
    TR rvalue = fmethod( cppObject, args... );
    return objectLink( rvalue );
  }

  
// Flavor for "return by pointer".
  template< typename TC, typename TR, typename... TArgs
          , typename std::enable_if<std::is_pointer<TR>::value,bool>::type = true >
  inline PyObject* _callFMethodReturn ( TR(* fmethod)(TC*,TArgs...), TC* cppObject, TArgs... args )
  {
    TR pvalue = fmethod( cppObject, args... );
    return objectLink( pvalue );
  }

  
// Flavor for "return void".
  template< typename TC, typename TR, typename... TArgs
          , typename std::enable_if<std::is_void<TR>::value,bool>::type = true >
  inline PyObject* _callFMethodReturn ( TR(* fmethod)(TC*,TArgs...), TC* cppObject, TArgs... args )
  {
    fmethod( cppObject, args... );
    Py_RETURN_NONE;
  }
  

  template< typename TC, typename TR >
  inline PyObject* _callMethod ( TR(* fmethod)(TC*), TC* cppObject, std::tuple<> )
  { return _callFMethodReturn( fmethod, cppObject ); }

  
  template< typename TC, typename TR, typename TA0 >
  inline PyObject* _callMethod ( TR(* fmethod)(TC*,TA0), TC* cppObject, std::tuple<TA0> args )
  { return _callFMethodReturn( fmethod, cppObject, std::get<0>(args) ); }

  
  template< typename TC, typename TR, typename TA0, typename TA1 >
  inline PyObject* _callMethod ( TR(* fmethod)(TC*,TA0,TA1), TC* cppObject, std::tuple<TA0,TA1> args )
  { return _callFMethodReturn( fmethod, cppObject, std::get<0>(args), std::get<1>(args) ); }

  
  template< typename TC, typename TR, typename TA0, typename TA1, typename TA2 >
  inline PyObject* _callMethod ( TR(* fmethod)(TC*,TA0,TA1,TA2), TC* cppObject, std::tuple<TA0,TA1,TA2> args )
  {
    // cerr << "_callMethod3 (function)" << endl;
    PyObject* object = _callFMethodReturn( fmethod, cppObject, std::get<0>(args)
                                                             , std::get<1>(args)
                                                             , std::get<2>(args) );
    // cerr << "done" << endl;
    return object;
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3 >
  inline PyObject* _callMethod (             TR(* fmethod)(TC*,TA0,TA1,TA2,TA3)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3> args )
  {
    // cerr << "_callMethod4 (function)" << endl;
    return _callFMethodReturn( fmethod, cppObject, std::get<0>(args)
                                                 , std::get<1>(args)
                                                 , std::get<2>(args)
                                                 , std::get<3>(args) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4 >
  inline PyObject* _callMethod (         TR(* fmethod)(TC*,TA0,TA1,TA2,TA3,TA4)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3,TA4> args )
  {
    // cerr << "_callMethod5 (function)" << endl;
    return _callFMethodReturn( fmethod, cppObject, std::get<0>(args)
                                                 , std::get<1>(args)
                                                 , std::get<2>(args)
                                                 , std::get<3>(args)
                                                 , std::get<4>(args) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5 >
  inline PyObject* _callMethod (         TR(* fmethod)(TC*,TA0,TA1,TA2,TA3,TA4,TA5)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3,TA4,TA5> args )
  {
  //std::cerr << "_callMethod6 (function)" << std::endl;
    return _callFMethodReturn( fmethod, cppObject, std::get<0>(args)
                                                 , std::get<1>(args)
                                                 , std::get<2>(args)
                                                 , std::get<3>(args)
                                                 , std::get<4>(args)
                                                 , std::get<5>(args) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6 >
  inline PyObject* _callMethod (         TR(* fmethod)(TC*,TA0,TA1,TA2,TA3,TA4,TA5,TA6)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3,TA4,TA5,TA6> args )
  {
    // cerr << "_callMethod7 (function)" << endl;
    return _callFMethodReturn( fmethod, cppObject, std::get<0>(args)
                                                 , std::get<1>(args)
                                                 , std::get<2>(args)
                                                 , std::get<3>(args)
                                                 , std::get<4>(args)
                                                 , std::get<5>(args)
                                                 , std::get<6>(args) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6
          , typename TA7 >
  inline PyObject* _callMethod (         TR(* fmethod)(TC*,TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7> args )
  {
    // cerr << "_callMethod8 (function)" << endl;
    return _callFMethodReturn( fmethod, cppObject, std::get<0>(args)
                                                 , std::get<1>(args)
                                                 , std::get<2>(args)
                                                 , std::get<3>(args)
                                                 , std::get<4>(args)
                                                 , std::get<5>(args)
                                                 , std::get<6>(args)
                                                 , std::get<7>(args) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6
          , typename TA7
          , typename TA8 >
  inline PyObject* _callMethod (         TR(* fmethod)(TC*,TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8> args )
  {
    // cerr << "_callMethod9 (function)" << endl;
    return _callFMethodReturn( fmethod, cppObject, std::get<0>(args)
                                                 , std::get<1>(args)
                                                 , std::get<2>(args)
                                                 , std::get<3>(args)
                                                 , std::get<4>(args)
                                                 , std::get<5>(args)
                                                 , std::get<6>(args)
                                                 , std::get<7>(args)
                                                 , std::get<8>(args) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6
          , typename TA7
          , typename TA8
          , typename TA9 >
  inline PyObject* _callMethod (         TR(* fmethod)(TC*,TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8,TA9)
                               , TC* cppObject, std::tuple<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8,TA9> args )
  {
    // cerr << "_callMethod10 (function)" << endl;
    return _callFMethodReturn( fmethod, cppObject, std::get<0>(args)
                                                 , std::get<1>(args)
                                                 , std::get<2>(args)
                                                 , std::get<3>(args)
                                                 , std::get<4>(args)
                                                 , std::get<5>(args)
                                                 , std::get<6>(args)
                                                 , std::get<7>(args)
                                                 , std::get<8>(args)
                                                 , std::get<9>(args) );
  }

  
// -------------------------------------------------------------------
// Standard Python to C types converters.


  template<typename T>
  inline bool  pyAs ( PyObject* pyArg, T& arg )
  {
    std::cerr << "Isobar3.pyAs<T>(): Unsupported type \"" << demangle(typeid(T).name()) << "\"" << endl;
    return false;
  }


  template<>
  inline bool  pyAs ( PyObject* pyArg, std::string& arg )
  {
    if (not PyUnicode_Check(pyArg)) return false;
    PyObject* pyBytes = PyUnicode_AsASCIIString( pyArg );
    arg = PyBytes_AsString( pyBytes );
    Py_DECREF( pyBytes );
    return true;
  }


  template<>
  inline bool  pyAs ( PyObject* pyArg, bool& arg )
  {
    if (not PyBool_Check(pyArg)) return false;
    arg = (pyArg == Py_True);
    return true;
  }


  template<>
  inline bool  pyAs ( PyObject* pyArg, int& arg )
  {
    if (PyLong_Check(pyArg)) { arg = PyLong_AsLong( pyArg ); }
    else return false;
    return true;
  }


  template<>
  inline bool  pyAs ( PyObject* pyArg, unsigned int& arg )
  {
    if (PyLong_Check(pyArg)) { arg = PyLong_AsLong( pyArg ); }
    else return false;
    return true;
  }


  template<>
  inline bool  pyAs ( PyObject* pyArg, unsigned long& arg )
  {
    if (not PyLong_Check(pyArg)) return false;
    arg = PyLong_AsLong( pyArg );
    return true;
  }


  template<>
  inline bool  pyAs ( PyObject* pyArg, long& arg )
  {
    if (not PyLong_Check(pyArg)) return false;
    arg = PyLong_AsLong( pyArg );
    return true;
  }


  template<>
  inline bool  pyAs ( PyObject* pyArg, float& arg )
  {
    if (not PyFloat_Check(pyArg)) return false;
    arg = PyFloat_AsDouble( pyArg );
    return true;
  }


  template<>
  inline bool  pyAs ( PyObject* pyArg, double& arg )
  {
    if (not PyFloat_Check(pyArg)) return false;
    arg = PyFloat_AsDouble( pyArg );
    return true;
  }


  template<typename T>
  inline bool  pyAs ( PyObject* pyArg, T*& arg )
  {
    PyTypeManager* manager = PyTypeManager::_get<T>();
    if (not manager) {
      std::cerr << "Isobar3.pyAs<T>(): Unsupported type \"" << typeid(T).name() << "\"" << endl;
      return false;
    }
    // std::cerr << "pyAs< " << demangle(typeid(T).name()) << " >() called." << std::endl;
    arg = (T*)( asVPtr( pyArg )->_object );
    return true;
  }


  template<typename T>
  inline bool  pyAs ( PyVoidPointer* pyArg, T*& arg )
  { return pyAs( (PyObject*)pyArg, arg ); }


  template<typename T>
  inline bool  pyArgAs ( PyObject* arg, T& cppArg, std::string nth, std::string& message )
  {
    // std::cerr << "pyArgAs<" << demangle(typeid(T).name()) << ">" << endl;
    if (not pyAs<T>(arg,cppArg)) {
      // std::cerr << "conversion failed" << std::endl;
      message += "\n  " + getString(nth) + " argument is not convertible to \"" + demangle(typeid(T).name()) + "\".";
      PyErr_SetString( ConstructorError, message.c_str() );
      return false;
    }
    return true;
  }

  
// -------------------------------------------------------------------
// Convert a table of PyObject*[] into a tuple of C++ objects.
// Recursive tuple construction.

  template< size_t N >
  std::tuple<> pyArgsAs ( PyObject* pyArgs[], bool& success, std::string& message, size_t count=0 )
  { return std::tuple<>(); }


  template< size_t N, typename T, typename... Tail >
  std::tuple<T,Tail...> pyArgsAs ( PyObject* pyArgs[], bool& success, std::string& message, size_t count=0 )
  {
    const std::string nth[] = { "First", "Second" , "Third", "Fourth", "Fifth"
                              , "Sixth", "Seventh", "Eight", "Ninth" , "Tenth" };

    T cppValue;
    success = success and pyArgAs<T>( pyArgs[count], cppValue, nth[count], message );
    return std::tuple_cat( make_tuple(cppValue), pyArgsAs<N,Tail...>(pyArgs,success,message,count+1) );
  }

  
// -------------------------------------------------------------------
// C++ Function Wrapper.
  

  template< typename TR, typename... TArgs >
  class PyFunctionWrapper {
    public:
      typedef  TR(* FunctionType )(TArgs...);
    public:
      inline               PyFunctionWrapper ( std::string fname, FunctionType method )
                                             : _funcName(fname), _method(method) { };
      inline  std::string  funcName          () const { return _funcName; };
      virtual PyObject*    call              ( PyObject* fargs );
    private:
      std::string   _funcName;
      FunctionType  _method;
  };

  
  template< typename TR, typename... TArgs >
  PyObject* PyFunctionWrapper<TR,TArgs...>::call ( PyObject* fargs )
  {
    PyErr_Clear();

    bool         success = true;
    PyObject*    rvalue  = NULL;
    std::string  message = funcName() + "(): ";
    try {
      size_t nargs = std::tuple_size< std::tuple<TArgs...> >::value;
      if (nargs >= 10)
        throw Error( "Isobar3::PyFunctionWrapper<>() support 10 arguments at most (%d requesteds).", nargs );
      
      std::tuple<TArgs...> cppArgs;
      if (nargs) {
        std::string  format;
        for ( size_t i=0 ; i<nargs ; ++i ) format += 'O';
        format += ":" + funcName() + "()";
        PyObject* args[] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
        if (PyArg_ParseTuple( fargs, format.c_str(), &args[0], &args[1], &args[2], &args[3], &args[4]
                                                   , &args[5], &args[6], &args[7], &args[8], &args[9] )) {
          cppArgs = pyArgsAs<std::tuple_size< std::tuple<TArgs...> >::value,TArgs...>( args, success, message );
        } else {
          size_t pynargs = (size_t)PyTuple_Size( fargs );
          message += " Invalid number of parameters, got "
                  + getString(pynargs) + " (expected " + getString(nargs) + ").";
          PyErr_SetString( ConstructorError, message.c_str() );
          success = false;
        }
      }
      if (success) {
        rvalue = _callFunction<TR,TArgs...>( _method, cppArgs );
        // std::cerr << "_call() " << demangle(typeid(FunctionType).name()) << "* (with " << nargs << " parameters) rvalue = " << (void*)rvalue << std::endl;
      } else {
        message += "\n  Prototype \"" + demangle(typeid(FunctionType).name()) + "\".";
        PyErr_SetString( ConstructorError, message.c_str() );
      }
    } catch ( const Warning& w ) {
      success  = false;
      message += "\n" + getString(w);
    } catch ( const Error& e ) {
      success  = false;
      message += "\n" + getString(e);
      if (not e.where().empty()) message += "\n" + e.where();
    } catch ( const Bug& e ) {
      success  = false;
      message += "\n" + getString(e);
    } catch ( const Exception& e ) {
      success  = false;
      message += "\nUnknown Hurricane::Exception";
    } catch ( const std::exception& e )  {
      success  = false;
      message += "\n" + std::string(e.what());
    } catch ( ... ) {
      success  = false;
      message += "\nUnmanaged exception, neither a Hurricane::Error nor"
                 " a std::exception.";
    }
    if (not success)
      PyErr_SetString( HurricaneError, message.c_str() );
    return rvalue;
  }

  
// -------------------------------------------------------------------
// C++ Method Wrappers.
  
  template< typename TC, typename TR, typename... TArgs >
  class PyMethodWrapper {
    public:
      typedef  TR(TC::* OMethodType )(TArgs...);
      typedef  TR(* FMethodType )(TC*,TArgs...);
    public:
      inline               PyMethodWrapper ( std::string fname, OMethodType method )
                                           : _funcName(fname)
                                           , _oMethod(method)
                                           , _fMethod(NULL)
                                           { };
      inline               PyMethodWrapper ( std::string fname, FMethodType method )
                                           : _funcName(fname)
                                           , _oMethod(NULL)
                                           , _fMethod(method)
                                           { };
      inline  std::string  funcName        () const { return _funcName; };
      inline  PyObject*    call            ( PyVoidPointer* self, PyObject* fargs );
              PyObject*    _call           ( TC*       cppObject, PyObject* fargs );
    private:
      std::string  _funcName;
      OMethodType  _oMethod;
      FMethodType  _fMethod;
  };


  template< typename TC, typename TR, typename... TArgs >
  inline PyObject* PyMethodWrapper<TC,TR,TArgs...>::call ( PyVoidPointer* self, PyObject* fargs )
  {
    PyErr_Clear();

    size_t nargs   = std::tuple_size< std::tuple<TArgs...> >::value;
    if (nargs >= 10)
      throw Error( "Isobar3::PyMethodWrapper<>() support 10 arguments at most (%d requesteds).", nargs );

    bool         success = true;
    std::string  message = funcName() + "(): ";
    try {
      TC*                  cppObject = static_cast<TC*>( self->_object );
      PyObject*            rvalue    = NULL;
      std::tuple<TArgs...> cppArgs;
      if (nargs) {
        std::string  format;
        for ( size_t i=0 ; i<nargs ; ++i ) format += 'O';
        format += ":" + funcName() + "()";
        PyObject* args[] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
        if (PyArg_ParseTuple( fargs, format.c_str(), &args[0], &args[1], &args[2], &args[3], &args[4]
                                                   , &args[5], &args[6], &args[7], &args[8], &args[9] )) {
          cppArgs = pyArgsAs<std::tuple_size< std::tuple<TArgs...> >::value,TArgs...>( args, success, message );
        } else {
          size_t pynargs = (size_t)PyTuple_Size( fargs );
          message += "Invalid number of parameters, got "
                  + getString(pynargs) + " (expected " + getString(nargs) + ").";
          PyErr_SetString( ConstructorError, message.c_str() );
          success = false;
        }
      }
      if (success) {
        if (_oMethod) {
          rvalue = _callMethod( _oMethod, cppObject, cppArgs );
          // std::cerr << "_call() " << demangle(typeid(OMethodType).name()) << "* (with " << nargs << " parameters) rvalue=" << (void*)rvalue << std::endl;
        }
        if (_fMethod) {
          rvalue = _callMethod( _fMethod, cppObject, cppArgs );
          // std::cerr << "_call() " << demangle(typeid(FMethodType).name()) << "* (with " << nargs << " parameters) rvalue=" << (void*)rvalue << std::endl;
        }
      } else {
        if (_oMethod) message += "\n  Prototype \"" + demangle(typeid(OMethodType).name()) + "\".";
        if (_fMethod) message += "\n  Prototype \"" + demangle(typeid(FMethodType).name()) + "\".";
        PyErr_SetString( ConstructorError, message.c_str() );
      }
      return rvalue;
    } catch ( const Warning& w ) {
      success  = false;
      message += "\n" + getString(w);
    } catch ( const Error& e ) {
      success  = false;
      message += "\n" + getString(e);
      if (not e.where().empty()) message += "\n" + e.where();
    } catch ( const Bug& e ) {
      success  = false;
      message += "\n" + getString(e);
    } catch ( const Exception& e ) {
      success  = false;
      message += "\nUnknown Hurricane::Exception";
    } catch ( const std::exception& e )  {
      success  = false;
      message += "\n" + std::string(e.what());
    } catch ( ... ) {
      success  = false;
      message += "\nUnmanaged exception, neither a Hurricane::Error nor"
                 " a std::exception.";
    }
    if (not success)
      PyErr_SetString( HurricaneError, message.c_str() );
    return NULL;
  }


// -------------------------------------------------------------------
// Method wrapper function encapsulation.

  
  template< typename TR, typename... TArgs >
  inline PyObject* callFunction ( std::string fname, TR(* func )(TArgs...), PyObject* args )
  {
    cdebug_log(30,0) << "[Python]" << fname << "()" << endl;
    PyObject* rvalue  = PyFunctionWrapper<TR,TArgs...>( fname, func ).call( args );
    // std::cerr << "callFunction() rvalue=" << rvalue << std::endl;
    return rvalue;
  }

  
  template< typename TC, typename TR, typename... TArgs >
  inline PyObject* callMethod ( std::string fname, TR(TC::* method )(TArgs...), PyVoidPointer* self, PyObject* args )
  {
    cdebug_log(30,0) << "[Python]" << fname << "()" << endl;
    return PyMethodWrapper<TC,TR,TArgs...>( fname, method ).call( self, args );
  }

  
  template< typename TC, typename TR, typename... TArgs >
  inline PyObject* callMethod ( std::string fname, TR(TC::* method )(TArgs...) const, PyVoidPointer* self, PyObject* args )
  {
    return callMethod<TC,TR,TArgs...>( fname, (TR(TC::*)(TArgs...))(method), self, args );
  }

  
  template< typename TC, typename TR, typename... TArgs >
  inline PyObject* callMethod ( std::string fname, TR(* fmethod )(TC*,TArgs...), PyVoidPointer* self, PyObject* args )
  {
    cdebug_log(30,0) << "[Python]" << fname << "()" << endl;
    return PyMethodWrapper<TC,TR,TArgs...>( fname, fmethod ).call( self, args );
  }

  
  template< typename TC, typename TR, typename... TArgs >
  inline PyObject* callMethod ( std::string fname, TR(* fmethod )(const TC*,TArgs...), PyVoidPointer* self, PyObject* args )
  {
    return callMethod<TC,TR,TArgs...>( fname, (TR(*)(TC*,TArgs...))(fmethod), self, args );
  }


}  // End of Isobar3 namespace.


#include "hurricane/configuration/PyVector.h"
#include "hurricane/configuration/PyMap.h"
