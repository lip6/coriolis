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
// |  C++ Header  :  "./hurricane/isobar/PyTypeManager.h"            |
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
#include "hurricane/configuration/ProxyProperty.h"

namespace Isobar3 {


  using namespace std;
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
      void* _object1;
  } PyOneVoid;


  typedef struct {
      PyObject_HEAD
      void*     _object1;    // pointer to iterator.
      PyObject* _object2;    // pointer to container.
  } PyTwoVoid;


  extern void        _tpDeAlloc     ( PyObject* );
  extern PyObject*   _tpStr         ( PyObject* );
  extern long        _tpHash        ( PyObject* );
  extern Py_ssize_t  _sqLength      ( PyObject* ); 
  extern PyObject*   _sqConcat      ( PyObject* , PyObject*  ); 
  extern PyObject*   _sqRepeat      ( PyObject* , Py_ssize_t );
  extern PyObject*   _sqItem        ( PyObject* , Py_ssize_t );
  extern int         _sqContains    ( PyObject*, PyObject* );
  extern Py_ssize_t  _mpLength      ( PyObject* ); 
  extern PyObject*   _mpSubscript   ( PyObject*, PyObject* ); 
  extern Py_ssize_t  _mpLength      ( PyObject* ); 
  extern PyObject*   _tpIter        ( PyObject* );
  extern PyObject*   _tpIterNext    ( PyObject* );


} // "C" linkage.


  inline PyOneVoid* asVPtr  ( PyObject*  o ) { return reinterpret_cast<PyOneVoid*>(o); }
  inline PyTwoVoid* asIPtr  ( PyObject*  o ) { return reinterpret_cast<PyTwoVoid*>(o); }
  inline void*&     object1 ( PyObject*  o ) { return asVPtr(o)->_object1; }
  inline void*&     object1 ( PyOneVoid* o ) { return o->_object1; }
  inline void*&     object1 ( PyTwoVoid* o ) { return o->_object1; }
  inline PyObject*& object2 ( PyObject*  o ) { return asIPtr(o)->_object2; }
  inline PyObject*& object2 ( PyTwoVoid* o ) { return o->_object2; }


// -------------------------------------------------------------------
// PyTypeObject post-creation default hook.


  inline void  addConstant ( PyTypeObject* typeObject, std::string name, long value )
  {
    PyObject* constant = PyLong_FromLong( value );
    PyDict_SetItemString( typeObject->tp_dict, name.c_str(), constant );
    Py_DECREF( constant );
  }
  

  template< typename CppT >
  inline void  pyTypePostInit ( PyTypeObject* typeObject )
  {
  //cerr << "pyTypePostInit<CppT>() default for <"
  //      << demangle(typeid(CppT).name()) << ">." << endl;
  }


// -------------------------------------------------------------------
// PyTypeObject tp_richcompare various flavors.


  template< typename CppT >
  PyObject* tpRichCompareByPtr ( PyObject *self, PyObject* other, int op )
  {
    if (Py_TYPE(self) != Py_TYPE(other)) {
      std::string message = "tpRichCompareByPtr<" + getString(demangle(typeid(CppT).name()))
                          + "> cannot compare to \"" + Py_TYPE(other)->tp_name + "\".";
      PyErr_SetString( PyExc_TypeError, message.c_str() );
      return NULL;
    }
    CppT* cself  = reinterpret_cast<CppT*>( object1(self ) );
    CppT* cother = reinterpret_cast<CppT*>( object1(other) );

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
  PyObject* tpRichCompareByValue ( PyObject *self, PyObject* other, int op )
  {
    if (Py_TYPE(self) != Py_TYPE(other)) {
      std::string message = "tpRichCompareByValue<" + getString(demangle(typeid(CppT).name()))
                          + "> cannot compare to \"" + Py_TYPE(other)->tp_name + "\".";
      PyErr_SetString( PyExc_TypeError, message.c_str() );
      return NULL;
    }
    CppT* cself  = reinterpret_cast<CppT*>( object1(self ) );
    CppT* cother = reinterpret_cast<CppT*>( object1(other) );

    bool result = false;
    if ((op == Py_LT) and (  cself  <    cother )) result = true;
    if ((op == Py_LE) and (  cself  <=   cother )) result = true;
    if ((op == Py_EQ) and ((*cself) == (*cother))) result = true;
    if ((op == Py_NE) and ((*cself) != (*cother))) result = true;
    if ((op == Py_GT) and (  cself  >    cother )) result = true;
    if ((op == Py_GE) and (  cself  >=   cother )) result = true;
    
    if (result) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  template< typename CppT >
  PyObject* tpRichCompareByDBo ( PyObject *self, PyObject* other, int op )
  {
    if (Py_TYPE(self) != Py_TYPE(other)) {
      std::string message = "getTpRichCompareByDBo<" + getString(demangle(typeid(CppT).name()))
                          + "> cannot compare to \"" + Py_TYPE(other)->tp_name + "\".";
      PyErr_SetString( PyExc_TypeError, message.c_str() );
      return NULL;
    }
    CppT* cself  = reinterpret_cast<CppT*>( object1(self ) );
    CppT* cother = reinterpret_cast<CppT*>( object1(other) );

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


// -------------------------------------------------------------------
// Basic PyTypeObject & PyObject wrapper / manager.


  class PyTypeManager {
    public:
      typedef std::map<size_t       ,PyTypeManager*>  ManagerByCppTypes;
      typedef std::map<PyTypeObject*,PyTypeManager*>  ManagerByPyTypes;
    public:
      static const uint64_t  NoFlags      =  0;
      static const uint64_t  IsDBo        = (1 << 0);
      static const uint64_t  IsIterator   = (1 << 1);
      static const uint64_t  NoCppDelete  = (1 << 3);
    public:
      inline   PyTypeManager ( PyMethodDef*, PyGetSetDef*, uint64_t flags );
      virtual ~PyTypeManager ();
    public:
                                      inline  bool                  _isDBo            () const;
                                      inline  bool                  _isIterator       () const;
                                      inline  const std::string&    _getCppTypeName   () const;
                                      inline  const std::string&    _getPyTypeName    () const;
                                      inline  std::string           _getTypeInfo      () const;
                                      inline  PyTypeObject*         _getTypeObject    ();
                                      inline  PyMethodDef*          _getMethods       ();
                                      inline  PyGetSetDef*          _getGetsets       ();
                                              void                  _setTypeNames     ( std::string className );
                                              void                  _addToModule      ( PyObject* );
                                      virtual void                  _getTpDeAlloc     ( PyObject* ) = 0;
                                      virtual PyObject*             _getTpStr         ( PyObject* ) = 0;
                                      virtual long                  _getTpHash        ( PyObject* ) = 0;
                                      virtual Py_ssize_t            _getSqLength      ( PyObject* );
                                      virtual PyObject*             _getSqConcat      ( PyObject*, PyObject* );
                                      virtual PyObject*             _getSqRepeat      ( PyObject*, Py_ssize_t );
                                      virtual PyObject*             _getSqItem        ( PyObject*, Py_ssize_t );
                                      virtual int                   _getSqContains    ( PyObject*, PyObject* );
                                      virtual Py_ssize_t            _getMpLength      ( PyObject* );
                                      virtual PyObject*             _getMpSubscript   ( PyObject*, PyObject* );
                                      virtual PyObject*             _getTpIter        ( PyObject* );
                                      virtual PyObject*             _getTpIterNext    ( PyObject* );
                                      virtual PyTypeManager*        _getBaseManager   ();
                                      virtual void                  _setupPyType      ();
                                      inline  void                  _setMethods       ( PyMethodDef* );
                                      inline  void                  _setTypeInfo      ( std::string );
    public:                                   
                              static  inline  ManagerByCppTypes& getManagerByCppTypes ();
                                      inline  uint64_t           flags           () const;
                              static  inline  PyTypeManager*     get             ( PyTypeObject* );
      template<typename CppT> static  inline  bool               hasType         ();
      template<typename CppT> static  inline  PyTypeManager*     _get            ();
      template<typename CppT> static  inline  PyObject*          link            ( CppT* );
      template<typename CppT> static          void               add             ( PyObject*, PyTypeManager*, size_t hashCode=0 );
    private:
      uint64_t                     _flags;
      std::string                  _pyTypeName;
      std::string                  _cppTypeName;
      std::string                  _typeInfo;
      PyTypeObject                 _typeObject;
      PyMethodDef*                 _methods;
      PyGetSetDef*                 _getsets;
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


  template<typename CppT> inline bool  PyTypeManager::hasType ()
  {
    auto element = _managerByCppTypes.find( typeid(CppT).hash_code() );
    return (element != _managerByCppTypes.end());
  }


  template<typename CppT> inline PyTypeManager* PyTypeManager::_get ()
  {
    auto element = _managerByCppTypes.find( typeid(CppT).hash_code() );
    if (element == _managerByCppTypes.end()) {
      std::cerr << "PyTypeManager<CppT>::_get(): Unregistered type <"
                << demangle(typeid(CppT).name()) << ">."  << std::endl;
      for ( auto item : _managerByCppTypes ) {
        std::cerr << "| " << std::setw(30) << std::right << item.first
                  << ":"  << item.second->_getTypeInfo() << std::endl;
      }
      throw Error( "PyTypeManager<CppT>::_get(): Unregistered type <%s>."
                 , demangle(typeid(CppT).name()).c_str() );
    }
    return (*element).second;
  }


  inline  PyTypeManager::PyTypeManager ( PyMethodDef *methods, PyGetSetDef* getsets, uint64_t flags )
    : _flags        (flags)
    , _pyTypeName   ()
    , _cppTypeName  ()
    , _typeInfo     ()
    , _typeObject   { PyVarObject_HEAD_INIT(&PyType_Type,0)
                      .tp_name        = NULL
                    , .tp_basicsize   = sizeof(PyOneVoid)
                    , .tp_itemsize    = 0
                    , .tp_dealloc     = NULL
#if PY_VERSION_HEX < 0x03080000
                    , .tp_print       = NULL
#endif
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
    , _methods      (methods)
    , _getsets      (getsets)
  {
    if (_isIterator()) _typeObject.tp_basicsize = sizeof(PyTwoVoid);
  }


  inline bool               PyTypeManager::_isDBo               () const { return _flags & IsDBo; }
  inline bool               PyTypeManager::_isIterator          () const { return _flags & IsIterator; }
  inline const std::string& PyTypeManager::_getCppTypeName      () const { return _cppTypeName; }
  inline const std::string& PyTypeManager::_getPyTypeName       () const { return _pyTypeName; }
  inline PyTypeObject*      PyTypeManager::_getTypeObject       () { return &_typeObject; }
  inline PyMethodDef*       PyTypeManager::_getMethods          () { return _methods; }
  inline PyGetSetDef*       PyTypeManager::_getGetsets          () { return _getsets; }
  inline uint64_t           PyTypeManager::flags                () const { return _flags; }
  inline void               PyTypeManager::_setMethods          ( PyMethodDef* methods ) { _methods=methods; }
  inline PyTypeManager::ManagerByCppTypes&
                            PyTypeManager::getManagerByCppTypes () { return _managerByCppTypes; }
  inline  std::string       PyTypeManager::_getTypeInfo         () const { return _typeInfo; }
  inline  void              PyTypeManager::_setTypeInfo         ( std::string ti ) { _typeInfo=ti; }


  template< typename CppT >
  void  PyTypeManager::add ( PyObject* module, PyTypeManager* manager, size_t hashCode )
  {
    if (ProxyProperty::getOffset() == (size_t)-1)
      ProxyProperty::setOffset( offsetof( PyOneVoid, _object1 ));

    // std::cerr << "PyTypeManager::add<" << demangle(typeid(CppT).name())
    //           << "> hash=" << std::type_index(typeid(CppT)).hash_code()
    //           << " manager=" << manager << std::endl;
    if (not hashCode) hashCode = typeid(CppT).hash_code();
    _managerByCppTypes[ hashCode                  ] = manager;
    _managerByPyTypes [ manager->_getTypeObject() ] = manager;
    manager->_addToModule( module );
    pyTypePostInit<CppT>( manager->_getTypeObject() );
    // std::cerr << "_managerByCppTypes.size()=" << _managerByCppTypes.size() << std::endl;
  }


// -------------------------------------------------------------------
// PyTypeObject & PyObject wrapper / manager.


  template<typename CppT>
  class PyTypeManagerVTrunk : public PyTypeManager {
    public:
      inline   PyTypeManagerVTrunk       ( PyMethodDef*, PyGetSetDef*, uint64_t flags );
      virtual ~PyTypeManagerVTrunk       ();
    public:
      template<typename BaseT>
      inline   bool      _canDynamicCast ( BaseT* ) const;
      virtual  PyObject* _getTpStr       ( PyObject* );
      virtual  PyObject* _link           ( CppT* );
  };
  

  template< typename CppT >
  inline PyTypeManagerVTrunk<CppT>::PyTypeManagerVTrunk ( PyMethodDef* methods, PyGetSetDef* getsets, uint64_t flags )
    : PyTypeManager(methods,getsets,flags)
  {
    _setTypeInfo( ::Hurricane::demangle( typeid(CppT).name() ));
  }
  

  template< typename CppT >
  PyTypeManagerVTrunk<CppT>::~PyTypeManagerVTrunk () { }


  template< typename CppT >
  template< typename BaseT >
  bool  PyTypeManagerVTrunk<CppT>::_canDynamicCast ( BaseT *cppObject ) const
  { return (dynamic_cast<CppT*>(cppObject) != NULL); }


  template< typename CppT >
  PyObject* PyTypeManagerVTrunk<CppT>::_getTpStr ( PyObject *self )
  {
    CppT* object = reinterpret_cast<CppT*>( object1(self) );
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

    PyOneVoid* pyObject = NULL;
    if (_isIterator())
      pyObject = (PyOneVoid*)PyObject_NEW( PyTwoVoid, _getTypeObject() );
    else
      pyObject = PyObject_NEW( PyOneVoid, _getTypeObject() );
    if (pyObject == NULL) return NULL;

    object1(pyObject) = (void*)object;
    cdebug_log(20,0) << "PyTypeManager::_link(): " << _getPyTypeName() << " "
                     << (void*)pyObject << ") " << (void*)object << ":" << object << std::endl;

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
      static  PyTypeManagerNonDBo* create        ( PyObject*        module
                                                 , PyMethodDef*     methods
                                                 , PyGetSetDef*     getsets
                                                 , uint64_t         flags
                                                 , std::string      objectName=""
                                                 , newfunc          tpNew =NULL
                                                 , initproc         tpInit=NULL
                                                 , richcmpfunc      tpRichCompare=tpRichCompareByPtr<CppT>
                                                 , PyNumberMethods* tpAsNumber=NULL
                                                 );
      virtual void                 _getTpDeAlloc ( PyObject* );
      virtual long                 _getTpHash    ( PyObject* );
  };
  

  template< typename CppT >
  inline PyTypeManagerNonDBo<CppT>::PyTypeManagerNonDBo ( PyMethodDef* methods, PyGetSetDef* getsets, uint64_t flags )
    : PyTypeManagerVTrunk<CppT>(methods,getsets,flags)
  { }


  template< typename CppT >
  PyTypeManagerNonDBo<CppT>::~PyTypeManagerNonDBo () { }

    
  template< typename CppT >
  void  PyTypeManagerNonDBo<CppT>::_getTpDeAlloc ( PyObject* self )
  {
    if (not (PyTypeManager::flags() & PyTypeManager::NoCppDelete))
      delete reinterpret_cast<CppT*>( object1(self) );
    PyObject_DEL( self );
  }


  template< typename CppT >
  long  PyTypeManagerNonDBo<CppT>::_getTpHash ( PyObject *self )
  { return (long)(object1(self)); }


  template< typename CppT >
  PyTypeManagerNonDBo<CppT>* PyTypeManagerNonDBo<CppT>::create ( PyObject*        module
                                                               , PyMethodDef*     methods
                                                               , PyGetSetDef*     getsets
                                                               , uint64_t         flags
                                                               , std::string      objectName
                                                               , newfunc          tpNew
                                                               , initproc         tpInit 
                                                               , richcmpfunc      tpRichCompare
                                                               , PyNumberMethods* tpAsNumber )
  {
    PyTypeManagerNonDBo<CppT>* manager = new PyTypeManagerNonDBo<CppT>( methods, getsets, flags );
    if (objectName.empty()) objectName = ::Hurricane::demangle(typeid(CppT));
    manager->_setTypeNames( objectName );
    manager->_setupPyType();
    PyTypeObject* ob_type = manager->_getTypeObject();
    if (tpNew )        ob_type->tp_new         = tpNew;
    if (tpInit)        ob_type->tp_init        = tpInit;
    if (tpRichCompare) ob_type->tp_richcompare = tpRichCompare;
    if (tpAsNumber)    ob_type->tp_as_number   = tpAsNumber;
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
                                                  , uint64_t     flags
                                                  , richcmpfunc  tpRichCompare=tpRichCompareByDBo<CppT>
                                                  );
      virtual PyObject*         _link             ( CppT* );
      virtual void              _getTpDeAlloc     ( PyObject* );
      virtual long              _getTpHash        ( PyObject* );
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

    PyOneVoid*     pyObject = NULL;
    ProxyProperty* proxy    = static_cast<ProxyProperty*>
      ( object->getProperty ( ProxyProperty::getPropertyName() ) );

    if (not proxy) {
      pyObject = PyObject_NEW( PyOneVoid, _getTypeObject() );
      if (not pyObject) return NULL;

      proxy = ProxyProperty::create( (void*)pyObject );
      object1(pyObject) = (void*)object;
      object->put( proxy );
    } else {
      pyObject = (PyOneVoid*)proxy->getShadow ();
      Py_INCREF( pyObject );
    }
    cdebug_log(20,0) << "PyTypeManagerDBo<" << demangle(typeid(CppT).name()) << ">() "
                     << (void*)pyObject << " <-> "
                     << (void*)object << ":" << object << endl;
    return (PyObject*)pyObject;
  }
    

  template< typename CppT >
  void  PyTypeManagerDBo<CppT>::_getTpDeAlloc ( PyObject* self )
  {
    DBo* object = reinterpret_cast<DBo*>( object1(self) );
    cdebug_log(20,0) << "PyTypeManager_DeAlloc(" << (void*)self << ") "
                     << object1(self) << ":" << object << endl;

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
  long PyTypeManagerDBo<CppT>::_getTpHash ( PyObject *self )
  {
    DBo* object = reinterpret_cast<DBo*>( object1(self) );
    return object->getId();
  }


  template< typename CppT >
  PyTypeManagerDBo<CppT>* PyTypeManagerDBo<CppT>::create ( PyObject*    module
                                                         , PyMethodDef* methods
                                                         , PyGetSetDef* getsets
                                                         , uint64_t     flags
                                                         , richcmpfunc  tpRichCompare )
  {
    if (std::is_base_of<Hurricane::Entity,CppT>::value)
      throw ::Hurricane::Error( "PyManagerDBo<CppT>::create(): The C++ class <%s> is *not* derived from DBo."
                              , ::Hurricane::demangle(typeid(CppT).name()).c_str() );
    
    PyTypeManagerDBo<CppT>* manager = new PyTypeManagerDBo   <CppT>( methods, getsets, flags );
    manager->_setTypeNames( ::Hurricane::demangle(typeid(CppT)) );
    manager->_setupPyType();
    PyTypeObject* ob_type = manager->_getTypeObject();
    if (tpRichCompare) ob_type->tp_richcompare = tpRichCompare;
    PyTypeManager::add<CppT>( module, manager );
    return manager;
  }


// -------------------------------------------------------------------
// PyTypeObject & PyObject for DBo objects with inheritance.


  template<typename CppT, typename BaseT >
  class PyTypeManagerDerivedDBo : public PyTypeManagerDBo<CppT> {
    public:
      using         PyTypeManager::_getTypeObject;
      inline        PyTypeManagerDerivedDBo ( PyMethodDef*, PyGetSetDef*, uint64_t flags );
      virtual      ~PyTypeManagerDerivedDBo ();
    public:
      static  PyTypeManagerDerivedDBo* create          ( PyObject*    module
                                                       , PyMethodDef* methods
                                                       , PyGetSetDef* getsets
                                                       , uint64_t     flags
                                                       , richcmpfunc  tpRichCompare=tpRichCompareByDBo<CppT>
                                                       );
      virtual PyTypeManager*           _getBaseManager ();
    private:
      PyTypeManager* _baseManager;
  };


  template< typename CppT, typename BaseT >
  inline PyTypeManagerDerivedDBo<CppT,BaseT>::PyTypeManagerDerivedDBo ( PyMethodDef* methods, PyGetSetDef* getsets, uint64_t flags )
    : PyTypeManagerDBo<CppT>(methods,getsets,flags|PyTypeManager::IsDBo)
    , _baseManager(NULL)
  { }


  template< typename CppT, typename BaseT >
  PyTypeManagerDerivedDBo<CppT,BaseT>::~PyTypeManagerDerivedDBo () { }


  template< typename CppT, typename BaseT >
  PyTypeManager* PyTypeManagerDerivedDBo<CppT,BaseT>::_getBaseManager ()
  { return _baseManager; }


  template< typename CppT, typename BaseT >
  PyTypeManagerDerivedDBo<CppT,BaseT>* PyTypeManagerDerivedDBo<CppT,BaseT>::create ( PyObject*    module
                                                                                   , PyMethodDef* methods
                                                                                   , PyGetSetDef* getsets
                                                                                   , uint64_t     flags
                                                                                   , richcmpfunc  tpRichCompare )
  {
    // if (not std::is_base_of<Hurricane::Entity,CppT>::value)
    //   throw ::Hurricane::Error( "PyManagerDerivedDBo<CppT>::create(): The C++ class <%s> is *not* derived from DBo."
    //                           , ::Hurricane::demangle(typeid(CppT).name()).c_str() );
    if (not std::is_base_of<BaseT,CppT>::value)
      throw ::Hurricane::Error( "PyManagerDerivedDBo<CppT>::create(): The C++ class <%s> is *not* derived from <%s>."
                              , ::Hurricane::demangle(typeid(CppT ).name()).c_str()
                              , ::Hurricane::demangle(typeid(BaseT).name()).c_str()
                              );
    PyTypeManager* baseManager = PyTypeManager::_get<BaseT>();
    if (not baseManager)
      throw ::Hurricane::Error( "PyManagerDerivedDBo<CppT>::create(): The Python base class <%s> for <%s> has not been registered first."
                              , ::Hurricane::demangle(typeid(BaseT).name()).c_str()
                              , ::Hurricane::demangle(typeid(CppT  ).name()).c_str()
                              );
    
    PyTypeManagerDerivedDBo<CppT,BaseT>* manager = new PyTypeManagerDerivedDBo<CppT,BaseT>( methods, getsets, flags );
    manager->_baseManager = baseManager;
    manager->_setTypeNames( ::Hurricane::demangle(typeid(CppT)) );
    manager->_setupPyType();
    PyTypeObject* ob_type = manager->_getTypeObject();
    if (tpRichCompare) ob_type->tp_richcompare = tpRichCompare;
    ob_type->tp_base = baseManager->_getTypeObject();
    PyTypeManager::add<CppT>( module, manager );
    return manager;
  }


}  // Isobar3 namespace.


// -------------------------------------------------------------------
// Top level link template.
  

template< typename CppT >
inline PyObject* cToPy ( CppT object )
{
  if (not Isobar3::PyTypeManager::hasType<CppT>()) {
    std::string message = "Overload for Isobar3::cToPy< "
                        + Hurricane::demangle(typeid(CppT).name()) + " >() Type not registered in the manager.";
    PyErr_SetString( Isobar3::HurricaneError, message.c_str() );
    return NULL;
  }
  return Isobar3::PyTypeManager::link<CppT>( new CppT (object) );
}


template< typename CppT
        , typename std::enable_if<   !std::is_same<CppT,bool>::value
                                  && !std::is_same<CppT,int >::value
                                  && !std::is_same<CppT,std::string>::value
                                  && !std::is_same<CppT,const std::string>::value,bool>::type = true >
inline PyObject* cToPy ( CppT* object )
{ return Isobar3::PyTypeManager::link<CppT>( object ); }


template< typename CppT
        , typename std::enable_if<   !std::is_same<CppT,bool>::value
                                  && !std::is_same<CppT,int >::value
                                  && !std::is_same<CppT,std::string>::value
                                  && !std::is_same<CppT,const std::string>::value,bool>::type = true >
inline PyObject* cToPy ( const CppT* object )
{ return Isobar3::PyTypeManager::link<CppT>( const_cast<CppT*>( object )); }


template<>
inline PyObject* cToPy<bool> ( bool b )
{ if (b) Py_RETURN_TRUE; Py_RETURN_FALSE; }


template<> inline PyObject* cToPy<      std::string>  (       std::string  s ) { return PyUnicode_FromString( s.c_str() ); }
template<> inline PyObject* cToPy<const std::string > ( const std::string  s ) { return PyUnicode_FromString( s.c_str() ); }
template<> inline PyObject* cToPy<const std::string*> ( const std::string* s ) { return PyUnicode_FromString( s->c_str() ); }

template<> inline PyObject* cToPy<      int > (       int  i ) { return PyLong_FromLong(  i ); }
template<> inline PyObject* cToPy<const int > ( const int  i ) { return PyLong_FromLong(  i ); }
template<> inline PyObject* cToPy<const int*> ( const int* i ) { return PyLong_FromLong( *i ); }

template<> inline PyObject* cToPy<      long > (       long  l ) { return PyLong_FromLong(  l ); }
template<> inline PyObject* cToPy<const long > ( const long  l ) { return PyLong_FromLong(  l ); }
template<> inline PyObject* cToPy<const long*> ( const long* l ) { return PyLong_FromLong( *l ); }

template<> inline PyObject* cToPy<      float > (       float  f ) { return PyFloat_FromDouble(  f ); }
template<> inline PyObject* cToPy<const float > ( const float  f ) { return PyFloat_FromDouble(  f ); }
template<> inline PyObject* cToPy<const float*> ( const float* f ) { return PyFloat_FromDouble( *f ); }

template<> inline PyObject* cToPy<      double > (       double  d ) { return PyFloat_FromDouble(  d ); }
template<> inline PyObject* cToPy<const double > ( const double  d ) { return PyFloat_FromDouble(  d ); }
template<> inline PyObject* cToPy<const double*> ( const double* d ) { return PyFloat_FromDouble( *d ); }


// Forward declaration for PyVector.h
template< typename CppT >
inline PyObject* cToPy ( const typename std::vector<CppT>::iterator* pit )
{ return Isobar3::PyTypeManager::link< typename std::vector<CppT>::iterator >
    ( std::addressof(const_cast< typename std::vector<CppT>::iterator* >(pit)) ); }


template< typename CppT >
inline PyObject* cToPy ( const std::vector<CppT>& vobject )
{ return Isobar3::PyTypeManager::link< std::vector<CppT> >( std::addressof(const_cast< std::vector<CppT>& >(vobject)) ); }


// Forward declaration for PyMap.h
template< typename CppK, typename CppT >
inline PyObject* cToPy ( const typename std::map<CppK,CppT>::iterator* pit )
{ return Isobar3::PyTypeManager::link< typename std::map<CppK,CppT>::iterator >
    ( std::addressof(const_cast< typename std::map<CppK,CppT>::iterator* >(pit)) ); }


template< typename CppK, typename CppT >
inline PyObject* cToPy ( const std::map<CppK,CppT>& vobject )
{
  return Isobar3::PyTypeManager::link< std::map<CppK,CppT> >
    ( std::addressof(const_cast< std::map<CppK,CppT>& >(vobject)) );
}


// Forward declaration for PyCollection.h
template< typename CppT >
inline PyObject* cToPy ( const typename Hurricane::Locator<CppT*>* plocator )
{ return Isobar3::PyTypeManager::link< typename Hurricane::Locator<CppT*> >
    ( std::addressof(const_cast< typename Hurricane::Locator<CppT*>* >(plocator)) ); }


template< typename CppT >
inline PyObject* cToPy ( Hurricane::GenericCollection<CppT*> collection )
{
  Hurricane::Collection<CppT*>* clone = collection.getClone();
  return Isobar3::PyTypeManager::link< Hurricane::Collection<CppT*> >( clone );
}


namespace Isobar3 {
  
// -------------------------------------------------------------------
// Convert a template list of type into a runtime vector of objects
// of different types, with a base class (simpler std::any<>).
//
// * BaseArg     : Common base to all templated arguments, so a table
//                 can be alloctated.
// * Arg<T>      : Template to hold an argument of type T. The stored
//                 attribute is ValueT, which is T sripped of it's
//                 const/volatile/reference qualifiers.
//                 Note: There is a limitation on possible T types,
//                       it must have a default constructor.
// * Args<Ts...> : A tailored fixed size array corresponding to their
//                 list of template types (in same sequential order).


  class BaseArg {
    public:
      inline   BaseArg () {};
      virtual ~BaseArg ();
  };


  template< typename T >
  class Arg : public BaseArg {
    public:
      typedef typename remove_cv< remove_reference<T> >::type::type  ValueT;
    public:
      inline           Arg      ()            : BaseArg(), _element() { }
      inline           Arg      ( T element ) : BaseArg(), _element(element) { }
      virtual         ~Arg      ();
      inline   ValueT& as       () { return _element; }
      inline   T       as_exact () { return _element; }
    private:
      ValueT  _element;
  };


  template< typename T > Arg<T>::~Arg ()
  { }
  

  template< typename... Ts >
  class Args {
    public:
                      Args       ();
                     ~Args       ();
      inline size_t   size       () const { return sizeof...(Ts); }
             bool     parse      ( PyObject* pyArgs[], std::string message );
             BaseArg* operator[] ( size_t index ) { return _args[index]; }
      template<typename T> T at ( size_t );
    private:
      BaseArg** _args;
  };
  

  template< size_t index > void  init_args ( BaseArg** args )
  { }
  

  template< size_t index, typename T, typename... Tail >
  void  init_args ( BaseArg** args )
  {
    args[index] = new Arg<T> ();
    init_args<index+1,Tail...>( args );
  }
  

  template< typename... Ts >
  Args<Ts...>::Args ()
    : _args(NULL)
  {
    if (size()) {
      _args = new BaseArg* [ size() ];
      init_args<0,Ts...>( _args );
    }
  }
  

  template< typename... Ts >
  Args<Ts...>::~Args ()
  {
    if (not _args) return;
    for ( size_t i=0 ; i<size() ; ++i ) delete _args[i];
    delete [] _args;
  }
  

  template<typename... Ts>
  template<typename T>
  T  Args<Ts...>::at ( size_t index )
  { return dynamic_cast< Arg<T>* >( _args[index] )->as(); }
  

  template<typename T>
  typename Arg<T>::ValueT& as ( BaseArg* arg )
  { return dynamic_cast< Arg<T>* >(arg)->as(); }


  template<typename T>
  T as_exact ( BaseArg* arg )
  { return dynamic_cast< Arg<T>* >(arg)->as_exact(); }
  

// Args<Ts...>::parse() is delayed until pyToC<> is defined.
  

// -------------------------------------------------------------------
// Wrap C++ function call to pass arguments through a Args<>.

  
// Flavor for "return by value" (seems to match std::is_object<>)
  template< typename TR, typename... TArgsF, typename... TArgsW
          , typename std::enable_if<   !std::is_reference<TR>::value
                                    && !std::is_pointer  <TR>::value
                                    && !std::is_void     <TR>::value,bool>::type = true >
  inline PyObject* _callFunctionReturn ( TR(* func)(TArgsF...), TArgsW... args )
  {
    TR value = func( args... );
    return cToPy( value );
  }

  
// Flavor for "return by reference".
  template< typename TR, typename... TArgsF, typename... TArgsW
          , typename std::enable_if<    std::is_reference<TR>::value
                                    && !std::is_pointer  <TR>::value
                                    && !std::is_void     <TR>::value,bool>::type = true >
  inline PyObject* _callFunctionReturn ( TR(* func)(TArgsF...), TArgsW... args )
  {
    TR rvalue = func( args... );
    return cToPy( rvalue );
  }

  
// Flavor for "return by pointer".
  template< typename TR, typename... TArgsF, typename... TArgsW
          , typename std::enable_if<std::is_pointer<TR>::value,bool>::type = true >
  inline PyObject* _callFunctionReturn ( TR(* func)(TArgsF...), TArgsW... args )
  {
    TR pvalue = func( args... );
    return cToPy( pvalue );
  }

  
// Flavor for "return void".
  template< typename TR, typename... TArgsF, typename... TArgsW
          , typename std::enable_if<std::is_void<TR>::value,bool>::type = true >
  inline PyObject* _callFunctionReturn ( TR(* func)(TArgsF...), TArgsW... args )
  {
    func( args... );
    Py_RETURN_NONE;
  }
  

  template< typename TR >
  PyObject* _callFunction ( TR(* func)(), Args<>& args )
  { return _callFunctionReturn( func ); }

  
  template< typename TR, typename TA0 >
  PyObject* _callFunction ( TR(* func)(TA0), Args<TA0>& args )
  { return _callFunctionReturn( func, as<TA0>( args[0] )); }

  
  template< typename TR, typename TA0, typename TA1 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1), Args<TA0,TA1>& args )
  { return _callFunctionReturn( func, as<TA0>( args[0] ), as<TA1>( args[1] ) ); }

  
  template< typename TR, typename TA0, typename TA1, typename TA2 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2), Args<TA0,TA1,TA2>& args )
  { return _callFunctionReturn( func, as<TA0>( args[0] ), as<TA1>( args[1] ), as<TA2>( args[2] ) ); }

  
  template< typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3), Args<TA0,TA1,TA2,TA3>& args )
  { return _callFunctionReturn( func, as<TA0>( args[0] )
                                    , as<TA1>( args[1] )
                                    , as<TA2>( args[2] )
                                    , as<TA3>( args[3] ) ); }


  template< typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3,TA4), Args<TA0,TA1,TA2,TA3,TA4>& args )
  { return _callFunctionReturn( func, as<TA0>( args[0] )
                                    , as<TA1>( args[1] )
                                    , as<TA2>( args[2] )
                                    , as<TA3>( args[3] )
                                    , as<TA4>( args[4] ) ); }


  template< typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3,TA4,TA5), Args<TA0,TA1,TA2,TA3,TA4,TA5>& args )
  { return _callFunctionReturn( func, as<TA0>( args[0] )
                                    , as<TA1>( args[1] )
                                    , as<TA2>( args[2] )
                                    , as<TA3>( args[3] )
                                    , as<TA4>( args[4] )
                                    , as<TA5>( args[5] ) ); }


  template< typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3,TA4,TA5,TA6), Args<TA0,TA1,TA2,TA3,TA4,TA5,TA6>& args )
  { return _callFunctionReturn( func, as<TA0>( args[0] )
                                    , as<TA1>( args[1] )
                                    , as<TA2>( args[2] )
                                    , as<TA3>( args[3] )
                                    , as<TA4>( args[4] )
                                    , as<TA5>( args[5] )
                                    , as<TA6>( args[6] ) ); }


  template< typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5
          , typename TA6
          , typename TA7 >
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7), Args<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7>& args )
  { return _callFunctionReturn( func, as<TA0>( args[0] )
                                    , as<TA1>( args[1] )
                                    , as<TA2>( args[2] )
                                    , as<TA3>( args[3] )
                                    , as<TA4>( args[4] )
                                    , as<TA5>( args[5] )
                                    , as<TA6>( args[6] )
                                    , as<TA7>( args[7] ) ); }


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
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8), Args<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8>& args )
  { return _callFunctionReturn( func, as<TA0>( args[0] )
                                    , as<TA1>( args[1] )
                                    , as<TA2>( args[2] )
                                    , as<TA3>( args[3] )
                                    , as<TA4>( args[4] )
                                    , as<TA5>( args[5] )
                                    , as<TA6>( args[6] )
                                    , as<TA7>( args[7] )
                                    , as<TA8>( args[8] ) ); }


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
  PyObject* _callFunction ( TR(* func)(TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8,TA9), Args<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8,TA9>& args )
  { return _callFunctionReturn( func, as<TA0>( args[0] )
                                    , as<TA1>( args[1] )
                                    , as<TA2>( args[2] )
                                    , as<TA3>( args[3] )
                                    , as<TA4>( args[4] )
                                    , as<TA5>( args[5] )
                                    , as<TA6>( args[6] )
                                    , as<TA7>( args[7] )
                                    , as<TA8>( args[8] )
                                    , as<TA9>( args[9] ) ); }


// -------------------------------------------------------------------
// Wrap C++ member method call to pass arguments through a tuple<>.

  
// Flavor for "return by value" (seems to match std::is_object<>)
  template< typename TC, typename TR, typename... TArgsF, typename... TArgsW
          , typename std::enable_if<   !std::is_reference<TR>::value
                                    && !std::is_pointer  <TR>::value
                                    && !std::is_void     <TR>::value,bool>::type = true >
  inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgsF...), TC* cppObject, TArgsW... args )
  {
    // std::cerr << "_callMethodReturn() By value, TR=<" << demangle(typeid(TR).name()) << ">" << std::endl;
    TR value = (cppObject->*method)( args... );
    return cToPy( value );
  }

  
// Flavor for "return by reference"
  template< typename TC, typename TR, typename... TArgsF, typename... TArgsW
          , typename std::enable_if<    std::is_reference<TR>::value
                                    && !std::is_pointer  <TR>::value
                                    && !std::is_void     <TR>::value,bool>::type = true >
  inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgsF...), TC* cppObject, TArgsW... args )
  {
    // std::cerr << "_callMethodReturn() By reference, TR=<" << demangle(typeid(TR).name()) << ">" << std::endl;
    TR rvalue = (cppObject->*method)( args... );
    return cToPy( rvalue );
  }

  
// Flavor for "return by pointer".
  template< typename TC, typename TR, typename... TArgsF, typename... TArgsW
          , typename std::enable_if<std::is_pointer<TR>::value,bool>::type = true >
  inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgsF...), TC* cppObject, TArgsW... args )
  {
    TR pvalue = (cppObject->*method)( args... );
    return cToPy( pvalue );
  }

  
// Flavor for "return void".
  template< typename TC, typename TR, typename... TArgsF, typename... TArgsW
          , typename std::enable_if<std::is_void<TR>::value,bool>::type = true >
  inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgsF...), TC* cppObject, TArgsW... args )
  {
    (cppObject->*method)( args... );
    Py_RETURN_NONE;
  }

  
  template< typename TC, typename TR >
  inline PyObject* _callMethod ( TR(TC::* method)(), TC* cppObject, Args<>& )
  { return _callMethodReturn<TC,TR>( method, cppObject ); }

  
  template< typename TC, typename TR, typename TA0 >
  inline PyObject* _callMethod ( TR(TC::* method)(TA0), TC* cppObject, Args<TA0>& args )
  { return _callMethodReturn( method, cppObject, as<TA0>( args[0] ) ); }

  
  template< typename TC, typename TR, typename TA0, typename TA1 >
  PyObject* _callMethod ( TR(TC::* method)(TA0,TA1), TC* cppObject, Args<TA0,TA1>& args )
  { return _callMethodReturn( method, cppObject, as<TA0>( args[0] ), as<TA1>( args[1] ) ); }

  
  template< typename TC, typename TR, typename TA0, typename TA1, typename TA2 >
  inline PyObject* _callMethod ( TR(TC::* method)(TA0,TA1,TA2), TC* cppObject, Args<TA0,TA1,TA2>& args )
  { return _callMethodReturn( method, cppObject, as<TA0>( args[0] ), as<TA1>( args[1] ), as<TA2>( args[2] ) ); }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3 >
  inline PyObject* _callMethod (    TR(TC::* method)(TA0,TA1,TA2,TA3)
                               , TC* cppObject, Args<TA0,TA1,TA2,TA3>& args )
  {
    return _callMethodReturn( method, cppObject, as<TA0>( args[0] )
                                               , as<TA1>( args[1] )
                                               , as<TA2>( args[2] )
                                               , as<TA3>( args[3] ) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4 >
  inline PyObject* _callMethod (    TR(TC::* method)(TA0,TA1,TA2,TA3,TA4)
                               , TC* cppObject, Args<TA0,TA1,TA2,TA3,TA4>& args )
  {
    return _callMethodReturn( method, cppObject, as<TA0>( args[0] )
                                               , as<TA1>( args[0] )
                                               , as<TA2>( args[0] )
                                               , as<TA3>( args[0] )
                                               , as<TA4>( args[0] ) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4
          , typename TA5 >
  inline PyObject* _callMethod (    TR(TC::* method)(TA0,TA1,TA2,TA3,TA4,TA5)
                               , TC* cppObject, Args<TA0,TA1,TA2,TA3,TA4,TA5>& args )
  {
    return _callMethodReturn( method, cppObject, as<TA0>( args[0] )
                                               , as<TA1>( args[1] )
                                               , as<TA2>( args[2] )
                                               , as<TA3>( args[3] )
                                               , as<TA4>( args[4] )
                                               , as<TA5>( args[5] ) );
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
  inline PyObject* _callMethod (    TR(TC::* method)(TA0,TA1,TA2,TA3,TA4,TA5,TA6)
                               , TC* cppObject, Args<TA0,TA1,TA2,TA3,TA4,TA5,TA6>& args )
  {
    return _callMethodReturn( method, cppObject, as<TA0>( args[0] )
                                               , as<TA1>( args[1] )
                                               , as<TA2>( args[2] )
                                               , as<TA3>( args[3] )
                                               , as<TA4>( args[4] )
                                               , as<TA5>( args[5] )
                                               , as<TA6>( args[6] ) );
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
  inline PyObject* _callMethod (    TR(TC::* method)(TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7)
                               , TC* cppObject, Args<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7>& args )
  {
    return _callMethodReturn( method, cppObject, as<TA0>( args[0] )
                                               , as<TA1>( args[1] )
                                               , as<TA2>( args[2] )
                                               , as<TA3>( args[3] )
                                               , as<TA4>( args[4] )
                                               , as<TA5>( args[5] )
                                               , as<TA6>( args[6] )
                                               , as<TA7>( args[7] ) );
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
  inline PyObject* _callMethod (    TR(TC::* method)(TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8)
                               , TC* cppObject, Args<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8>& args )
  {
    return _callMethodReturn( method, cppObject, as<TA0>( args[0] )
                                               , as<TA1>( args[1] )
                                               , as<TA2>( args[2] )
                                               , as<TA3>( args[3] )
                                               , as<TA4>( args[4] )
                                               , as<TA5>( args[5] )
                                               , as<TA6>( args[6] )
                                               , as<TA7>( args[7] )
                                               , as<TA8>( args[8] ) );
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
  inline PyObject* _callMethod (    TR(TC::* method)(TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8,TA9)
                               , TC* cppObject, Args<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8,TA9>& args )
  {
    return _callMethodReturn( method, cppObject, as<TA0>( args[0] )
                                               , as<TA1>( args[1] )
                                               , as<TA2>( args[2] )
                                               , as<TA3>( args[3] )
                                               , as<TA4>( args[4] )
                                               , as<TA5>( args[5] )
                                               , as<TA6>( args[6] )
                                               , as<TA7>( args[7] )
                                               , as<TA8>( args[8] )
                                               , as<TA9>( args[9] ) );
  }


// -------------------------------------------------------------------
// Wrap C++ member method call as function to pass arguments through a tuple<>.

  
// Flavor for "return by value" (seems to match std::is_object<>)
  template< typename TC, typename TR, typename... TArgsF, typename... TArgsW
          , typename std::enable_if<   !std::is_reference<TR>::value
                                    && !std::is_pointer  <TR>::value
                                    && !std::is_void     <TR>::value,bool>::type = true >
  inline PyObject* _callFMethodReturn ( TR(* fmethod)(TC*,TArgsF...), TC* cppObject, TArgsW... args )
  {
    TR value = fmethod( cppObject, args... );
    return cToPy( value );
  }

  
// Flavor for "return by reference"
  template< typename TC, typename TR, typename... TArgsF, typename... TArgsW
          , typename std::enable_if<    std::is_reference<TR>::value
                                    && !std::is_pointer  <TR>::value
                                    && !std::is_void     <TR>::value,bool>::type = true >
  inline PyObject* _callFMethodReturn ( TR(* fmethod)(TC*,TArgsF...), TC* cppObject, TArgsW... args )
  {
    TR rvalue = fmethod( cppObject, args... );
    return cToPy( rvalue );
  }

  
// Flavor for "return by pointer".
  template< typename TC, typename TR, typename... TArgsF, typename... TArgsW
          , typename std::enable_if<std::is_pointer<TR>::value,bool>::type = true >
  inline PyObject* _callFMethodReturn ( TR(* fmethod)(TC*,TArgsF...), TC* cppObject, TArgsW... args )
  {
    TR pvalue = fmethod( cppObject, args... );
    return cToPy( pvalue );
  }

  
// Flavor for "return void".
  template< typename TC, typename TR, typename... TArgsF, typename... TArgsW
          , typename std::enable_if<std::is_void<TR>::value,bool>::type = true >
  inline PyObject* _callFMethodReturn ( TR(* fmethod)(TC*,TArgsF...), TC* cppObject, TArgsW... args )
  {
    fmethod( cppObject, args... );
    Py_RETURN_NONE;
  }
  

  template< typename TC, typename TR >
  inline PyObject* _callMethod ( TR(* fmethod)(TC*), TC* cppObject, Args<>& )
  { return _callFMethodReturn( fmethod, cppObject ); }

  
  template< typename TC, typename TR, typename TA0 >
  inline PyObject* _callMethod ( TR(* fmethod)(TC*,TA0), TC* cppObject, Args<TA0>& args )
  { return _callFMethodReturn( fmethod, cppObject, as<TA0>(args[0]) ); }

  
  template< typename TC, typename TR, typename TA0, typename TA1 >
  inline PyObject* _callMethod ( TR(* fmethod)(TC*,TA0,TA1), TC* cppObject, Args<TA0,TA1>& args )
  { return _callFMethodReturn( fmethod, cppObject, as<TA0>(args[0]), as<TA1>(args[1]) ); }

  
  template< typename TC, typename TR, typename TA0, typename TA1, typename TA2 >
  inline PyObject* _callMethod ( TR(* fmethod)(TC*,TA0,TA1,TA2), TC* cppObject, Args<TA0,TA1,TA2>& args )
  {
    // cerr << "_callMethod3 (function)" << endl;
    PyObject* object = _callFMethodReturn( fmethod, cppObject, as<TA0>(args[0])
                                                             , as<TA1>(args[1])
                                                             , as<TA2>(args[2]) );
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
                               , TC* cppObject, Args<TA0,TA1,TA2,TA3>& args )
  {
    // cerr << "_callMethod4 (function)" << endl;
    return _callFMethodReturn( fmethod, cppObject, as<TA0>(args[0])
                                                 , as<TA1>(args[1])
                                                 , as<TA2>(args[2])
                                                 , as<TA3>(args[3]) );
  }

  
  template< typename TC
          , typename TR
          , typename TA0
          , typename TA1
          , typename TA2
          , typename TA3
          , typename TA4 >
  inline PyObject* _callMethod (         TR(* fmethod)(TC*,TA0,TA1,TA2,TA3,TA4)
                               , TC* cppObject, Args<TA0,TA1,TA2,TA3,TA4>& args )
  {
    // cerr << "_callMethod5 (function)" << endl;
    return _callFMethodReturn( fmethod, cppObject, as<TA0>(args[0])
                                                 , as<TA1>(args[1])
                                                 , as<TA2>(args[2])
                                                 , as<TA3>(args[3])
                                                 , as<TA4>(args[4]) );
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
                               , TC* cppObject,       Args<TA0,TA1,TA2,TA3,TA4,TA5>& args )
  {
  //std::cerr << "_callMethod6 (function)" << std::endl;
    return _callFMethodReturn( fmethod, cppObject, as<TA0>(args[0])
                                                 , as<TA1>(args[1])
                                                 , as<TA2>(args[2])
                                                 , as<TA3>(args[3])
                                                 , as<TA4>(args[4])
                                                 , as<TA5>(args[5]) );
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
                               , TC* cppObject,       Args<TA0,TA1,TA2,TA3,TA4,TA5,TA6>& args )
  {
    // cerr << "_callMethod7 (function)" << endl;
    return _callFMethodReturn( fmethod, cppObject, as<TA0>(args[0])
                                                 , as<TA1>(args[1])
                                                 , as<TA2>(args[2])
                                                 , as<TA3>(args[3])
                                                 , as<TA4>(args[4])
                                                 , as<TA5>(args[5])
                                                 , as<TA6>(args[6]) );
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
  inline PyObject* _callMethod (   TR(* fmethod)(TC*,TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7)
                               , TC* cppObject, Args<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7>& args )
  {
    // cerr << "_callMethod8 (function)" << endl;
    return _callFMethodReturn( fmethod, cppObject, as<TA0>( args[0] )
                                                 , as<TA1>( args[1] )
                                                 , as<TA2>( args[2] )
                                                 , as<TA3>( args[3] )
                                                 , as<TA4>( args[4] )
                                                 , as<TA5>( args[5] )
                                                 , as<TA6>( args[6] )
                                                 , as<TA7>( args[7] ) );
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
  inline PyObject* _callMethod (   TR(* fmethod)(TC*,TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8)
                               , TC* cppObject, Args<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8>& args )
  {
    // cerr << "_callMethod9 (function)" << endl;
    return _callFMethodReturn( fmethod, cppObject, as<TA0>( args[0] )
                                                 , as<TA1>( args[1] )
                                                 , as<TA2>( args[2] )
                                                 , as<TA3>( args[3] )
                                                 , as<TA4>( args[4] )
                                                 , as<TA5>( args[5] )
                                                 , as<TA6>( args[6] )
                                                 , as<TA7>( args[7] )
                                                 , as<TA8>( args[8] ) );
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
  inline PyObject* _callMethod (   TR(* fmethod)(TC*,TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8,TA9)
                               , TC* cppObject, Args<TA0,TA1,TA2,TA3,TA4,TA5,TA6,TA7,TA8,TA9>& args )
  {
    // cerr << "_callMethod10 (function)" << endl;
    return _callFMethodReturn( fmethod, cppObject, as<TA0>( args[0] )
                                                 , as<TA1>( args[1] )
                                                 , as<TA2>( args[2] )
                                                 , as<TA3>( args[3] )
                                                 , as<TA4>( args[4] )
                                                 , as<TA5>( args[5] )
                                                 , as<TA6>( args[6] )
                                                 , as<TA7>( args[7] )
                                                 , as<TA8>( args[8] )
                                                 , as<TA9>( args[9] ) );
  }


}  // Isobar3 namespace.
  
// -------------------------------------------------------------------
// Standard Python to C types converters.


template< typename T
        , typename std::enable_if< !std::is_pointer<T>::value, bool >::type = true >
inline bool  pyToC ( PyObject* pyArg, T* arg )
{
  typedef typename std::remove_cv<T>::type  NonConstT;
  Isobar3::PyTypeManager* manager = Isobar3::PyTypeManager::_get<T>();
  if (not manager) {
    std::cerr << "Isobar3::pyToC<>(const T*): Unsupported type." << std::endl;
    return false;
  }
  if (Py_TYPE(pyArg) != manager->_getTypeObject()) return false;
  *(const_cast< NonConstT* >(arg)) = *(( T* )( Isobar3::object1( pyArg )));
  return true;
}


template<typename T>
inline bool  pyToC ( PyObject* pyArg, T** arg )
{
  Isobar3::PyTypeManager* manager = Isobar3::PyTypeManager::_get<T>();
  if (not manager) {
    std::cerr << "Isobar3::pyToC<T>(T*&): Unsupported type \"" << typeid(T).name() << "\"" << std::endl;
    return false;
  }
  // std::cerr << "pyToC< " << demangle(typeid(T).name()) << " >() called." << std::endl;
  *arg = (T*)( Isobar3::object1( pyArg ));
  return true;
}


inline bool  pyToC ( PyObject* pyArg, std::string* arg )
{
  if (not PyUnicode_Check(pyArg)) return false;
  PyObject* pyBytes = PyUnicode_AsASCIIString( pyArg );
  *arg = PyBytes_AsString( pyBytes );
  Py_DECREF( pyBytes );
  return true;
}


inline bool  pyToC ( PyObject* pyArg, bool* arg )
{
  if (not PyBool_Check(pyArg)) return false;
  *arg = (pyArg == Py_True);
  return true;
}


inline bool  pyToC ( PyObject* pyArg, int* arg )
{
  if (PyLong_Check(pyArg)) { *arg = PyLong_AsLong( pyArg ); }
  else return false;
  return true;
}


inline bool  pyToC ( PyObject* pyArg, unsigned int* arg )
{
  if (PyLong_Check(pyArg)) { *arg = PyLong_AsLong( pyArg ); }
  else return false;
  return true;
}


inline bool  pyToC ( PyObject* pyArg, unsigned long* arg )
{
  if (not PyLong_Check(pyArg)) return false;
  *arg = PyLong_AsLong( pyArg );
  return true;
}


inline bool  pyToC ( PyObject* pyArg, unsigned long long* arg )
{
  if (not PyLong_Check(pyArg)) return false;
  *arg = PyLong_AsLongLong( pyArg );
  return true;
}


inline bool  pyToC ( PyObject* pyArg, long* arg )
{
  if (not PyLong_Check(pyArg)) return false;
  *arg = PyLong_AsLong( pyArg );
  return true;
}


inline bool  pyToC ( PyObject* pyArg, float* arg )
{
  if (not PyFloat_Check(pyArg)) return false;
  *arg = PyFloat_AsDouble( pyArg );
  return true;
}


inline bool  pyToC ( PyObject* pyArg, double* arg )
{
  if (not PyFloat_Check(pyArg)) return false;
  *arg = PyFloat_AsDouble( pyArg );
  return true;
}


namespace Isobar3 {
  
// Complete Args<Ts...> template definition.

  template< size_t index >
  void  parse_pyobjects ( PyObject* [], BaseArg**, bool&, std::string, size_t count=0 )
  {  }


  template< size_t index, typename T, typename... Tail >
  void  parse_pyobjects ( PyObject*    pyArgs[]
                        , BaseArg**    args
                        , bool&        success
                        , std::string  message
                        , size_t       count=0 )
  {
    const std::string nth[] = { "First", "Second" , "Third", "Fourth", "Fifth"
                              , "Sixth", "Seventh", "Eight", "Ninth" , "Tenth" };

  //std::cerr << "Calling pyToC<" << demangle(typeid(typename Arg<T>::ValueT).name()) << ">" << std::endl;
  //std::cerr << "Calling pyToC<" << demangle(typeid(&(as<T>( args[count]))).name()) << ">" << std::endl;
  //success = success and pyToC< typename Arg<T>::ValueT >( pyArgs[count]
  //                                                      , &(as<T>( args[count])) );
    success = success and pyToC( pyArgs[count], &(as<T>( args[count])) );
  //std::cerr << "success=" << success << std::endl;
    if (not success) {
      message += "\n  " + getString(nth) + " X argument is not convertible to \"" + Hurricane::demangle(typeid(T).name()) + "\".";
      PyErr_SetString( Isobar3::ConstructorError, message.c_str() );
    } else {
      parse_pyobjects<index,Tail...>( pyArgs, args, success, message, count+1 );
    }
  }

  
  template< typename... Ts >
  bool  Args<Ts...>::parse ( PyObject* pyArgs[], string message )
  {
    bool success = true;
    parse_pyobjects<0,Ts...>( pyArgs, _args, success, message );
    return success;
  }


// -------------------------------------------------------------------
// C++ Exception Wrapper.


  class PyWrapper {
    public:
      inline               PyWrapper  ();
      virtual             ~PyWrapper  ();
      inline  std::string& message    ();
      inline  PyObject*    call       ( PyObject* self, PyObject* args );
      inline  PyObject*    call       ( PyObject* args );
      inline  int          predicate  ( PyObject* self );
      virtual PyObject*    _call      ( PyObject* self, PyObject* args );
      virtual PyObject*    _call      ( PyObject* args );
      virtual int          _predicate ( PyObject* self );
      inline  void         setMessage ( std::string header );
    private:
      std::string  _message;
  };


  extern PyObject* exceptionWrapper          ( PyWrapper* wrapper, PyObject* self, PyObject* args );
  extern PyObject* exceptionWrapper          ( PyWrapper* wrapper, PyObject* args );
  extern int       exceptionPredicateWrapper ( PyWrapper* wrapper, PyObject* self );

  inline               PyWrapper::PyWrapper  () : _message("Wrapper(): Base class.") {}
  inline  std::string& PyWrapper::message    () { return _message; }
  inline  void         PyWrapper::setMessage ( std::string header ) { _message = header; }
  inline PyObject*     PyWrapper::call       ( PyObject* self, PyObject* args ) { return exceptionWrapper( this, self, args ); }
  inline PyObject*     PyWrapper::call       ( PyObject* args ) { return exceptionWrapper( this, args ); }
  inline int           PyWrapper::predicate  ( PyObject* self ) { return exceptionPredicateWrapper( this, self ); }


// -------------------------------------------------------------------
// C++ Function Wrapper.
  

  template< typename TR, typename... TArgs >
  class PyFunctionWrapper : public PyWrapper {
    public:
      typedef  TR(* FunctionType )(TArgs...);
    public:
      inline               PyFunctionWrapper ( std::string fname, FunctionType method )
                                             : PyWrapper(), _funcName(fname), _method(method) { };
      inline  std::string  funcName          () const { return _funcName; };
      virtual PyObject*    _call             ( PyObject* fargs );
    private:
      std::string   _funcName;
      FunctionType  _method;
  };

  
  template< typename TR, typename... TArgs >
  PyObject* PyFunctionWrapper<TR,TArgs...>::_call ( PyObject* fargs )
  {
    PyErr_Clear();
  //std::cerr << "_call() " << demangle(typeid(FunctionType).name()) << std::endl;

    setMessage( funcName() + "(): " );
    bool    success = true;
    size_t  nargs   = sizeof...(TArgs);
    if (nargs >= 10)
      throw Error( "Isobar3::PyFunctionWrapper<>() support 10 arguments at most (%d requesteds).", nargs );
    
    Args<TArgs...> cppArgs;
    if (nargs) {
    //cerr << "Converting " << nargs << " arguments." << endl;
      std::string  format;
      for ( size_t i=0 ; i<nargs ; ++i ) format += 'O';
      format += ":" + funcName() + "()";
      PyObject* args[] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
      if (PyArg_ParseTuple( fargs, format.c_str(), &args[0], &args[1], &args[2], &args[3], &args[4]
                                                 , &args[5], &args[6], &args[7], &args[8], &args[9] )) {
        success = cppArgs.parse( args, message() );
      } else {
        size_t pynargs = (size_t)PyTuple_Size( fargs );
        message() += " Invalid number of parameters, got "
                  + getString(pynargs) + " (expected " + getString(nargs) + ").";
        PyErr_SetString( ConstructorError, message().c_str() );
        success = false;
      }
    }
    if (success) {
    //std::cerr << "_call() " << demangle(typeid(FunctionType).name()) << " (with " << nargs << " parameters)" << std::endl;
      return _callFunction<TR,TArgs...>( _method, cppArgs );
    }

    message() += "\n  Prototype \"" + demangle(typeid(FunctionType).name()) + "\".";
    PyErr_SetString( HurricaneError, message().c_str() );
    return NULL;
  }

  
// -------------------------------------------------------------------
// C++ Method Wrappers, variadic number of arguments. 
  
  template< typename TC, typename TR, typename... TArgs >
  class PyMethodWrapper : public PyWrapper {
    public:
      typedef  TR(TC::* OMethodType )(TArgs...);
      typedef  TR(* FMethodType )(TC*,TArgs...);
    public:
      inline               PyMethodWrapper ( std::string fname, OMethodType method )
                                           : PyWrapper()
                                           , _funcName(fname)
                                           , _oMethod(method)
                                           , _fMethod(NULL)
                                           { };
      inline               PyMethodWrapper ( std::string fname, FMethodType method )
                                           : PyWrapper()
                                           , _funcName(fname)
                                           , _oMethod(NULL)
                                           , _fMethod(method)
                                           { };
      inline  std::string  funcName        () const { return _funcName; };
      virtual PyObject*    _call           ( PyObject* self, PyObject* fargs );
    private:
      std::string  _funcName;
      OMethodType  _oMethod;
      FMethodType  _fMethod;
  };


  template< typename TC, typename TR, typename... TArgs >
  inline PyObject* PyMethodWrapper<TC,TR,TArgs...>::_call ( PyObject* self, PyObject* fargs )
  {
    PyErr_Clear();
    size_t nargs   = sizeof...(TArgs);
    if (nargs >= 10)
      throw Error( "Isobar3::PyMethodWrapper<>() support 10 arguments at most (%d requesteds).", nargs );

    setMessage( funcName() + "(): " );
    bool           success = true;
    TC*            cppObject = static_cast<TC*>( object1(self) );
    PyObject*      rvalue    = NULL;
    Args<TArgs...> cppArgs;
    if (nargs) {
      std::string  format;
      for ( size_t i=0 ; i<nargs ; ++i ) format += 'O';
      format += ":" + funcName() + "()";
      PyObject* args[] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
      if (PyArg_ParseTuple( fargs, format.c_str(), &args[0], &args[1], &args[2], &args[3], &args[4]
                                                 , &args[5], &args[6], &args[7], &args[8], &args[9] )) {
        success = cppArgs.parse( args, message() );
      } else {
      //cerr << fargs << endl;
        size_t pynargs = (size_t)PyTuple_Size( fargs );
        message() += "Invalid number of parameters, got "
                  + getString(pynargs) + " (expected " + getString(nargs) + ").";
        PyErr_SetString( ConstructorError, message().c_str() );
        success = false;
      }
    }
    if (success) {
      if (_oMethod) {
        rvalue = _callMethod( _oMethod, cppObject, cppArgs );
      //std::cerr << "_call() " << demangle(typeid(OMethodType).name()) << "* (with " << nargs << " parameters) rvalue=" << (void*)rvalue << std::endl;
      }
      if (_fMethod) {
        rvalue = _callMethod( _fMethod, cppObject, cppArgs );
      //std::cerr << "_call() " << demangle(typeid(FMethodType).name()) << "* (with " << nargs << " parameters) rvalue=" << (void*)rvalue << std::endl;
      }
      return rvalue;
    }
    if (_oMethod) message() += "\n  Prototype \"" + demangle(typeid(OMethodType).name()) + "\".";
    if (_fMethod) message() += "\n  Prototype \"" + demangle(typeid(FMethodType).name()) + "\".";
    PyErr_SetString( HurricaneError, message().c_str() );
    return NULL;
  }

  
// -------------------------------------------------------------------
// C++ Method Wrappers, binary function.

  template< typename TC, typename TArg >
  class PyMBinaryWrapper : public PyWrapper {
    public:
      typedef  TC(TC::* OMethodType )(TArg);
    public:
      inline               PyMBinaryWrapper ( std::string fname, OMethodType mbinary )
                                            : PyWrapper()
                                            , _funcName(fname)
                                            , _oMethod(mbinary)
                                            { };
      inline  std::string  funcName         () const { return _funcName; };
      virtual PyObject*    _call            ( PyObject* self, PyObject* arg );
    private:
      std::string  _funcName;
      OMethodType  _oMethod;
  };


  template< typename TC, typename TArg >
  inline PyObject* PyMBinaryWrapper<TC,TArg>::_call ( PyObject* pyObject, PyObject* pyArg )
  {
    PyErr_Clear();
    setMessage( funcName() + "(): " );

    TC   object;
    TArg arg;
    pyToC( pyObject, &object );
    if (not pyToC( pyArg, &arg )) {
      message() += "\n  Argument is not convertible to \"" + Hurricane::demangle(typeid(TArg).name()) + "\".";
      PyErr_SetString( Isobar3::ConstructorError, message().c_str() );
      return NULL;
    }
    TC result = (object.*_oMethod)( arg );
    return cToPy<TC>( result );
  }

  
// -------------------------------------------------------------------
// C++ Method Wrappers, operator function.

  template< typename TC, template<typename> class OperatorT >
  class PyOperatorWrapper : public PyWrapper {
    public:
      inline               PyOperatorWrapper ( std::string fname )
                                             : PyWrapper()
                                             , _funcName (fname)
                                             { };
      inline  std::string  funcName          () const { return _funcName; };
      virtual PyObject*    _call             ( PyObject* self, PyObject* arg );
    private:
      std::string   _funcName;
  };


  template< typename TC, template<typename> class OperatorT >
  inline PyObject* PyOperatorWrapper<TC,OperatorT>::_call ( PyObject* pyObject, PyObject* pyArg )
  {
    PyErr_Clear();
    setMessage( funcName() + "(): " );

    TC lhs;
    TC rhs;
    pyToC( pyObject, &lhs );
    if (not pyToC( pyArg, &rhs )) {
      string message = "PyOperatorWrapper():";
      message += "\n  RHS argument is not convertible to \"" + Hurricane::demangle(typeid(TC).name()) + "\".";
      PyErr_SetString( Isobar3::ConstructorError, message.c_str() );
      return NULL;
    }
    TC result = OperatorT<TC>()( lhs, rhs );
    return cToPy<TC>( result );
  }

  
// -------------------------------------------------------------------
// C++ Method Wrappers, in-place operator function.

  template< typename TC, typename TArg >
  class PyInPlaceOperatorWrapper : public PyWrapper {
    public:
      typedef  void(* OInPlaceType )(TC*,TArg*);
    public:
      inline               PyInPlaceOperatorWrapper ( std::string fname, OInPlaceType minplace )
                                                    : PyWrapper()
                                                    , _funcName(fname)
                                                    , _oInPlace(minplace)
                                                    { };
      inline  std::string  funcName                 () const { return _funcName; };
      virtual PyObject*    _call                    ( PyObject* self, PyObject* arg );
    private:
      std::string   _funcName;
      OInPlaceType  _oInPlace;
  };


  template< typename TC, typename TArg >
  inline PyObject* PyInPlaceOperatorWrapper<TC,TArg>::_call ( PyObject* pyObject, PyObject* pyArg )
  {
    PyErr_Clear();
    setMessage( funcName() + "(): " );

    TC*   object = NULL;
    TArg* arg    = NULL;
    pyToC( pyObject, &object );
    if (not pyToC( pyArg, &arg )) {
      string message = "PyInPlaceOperatorWrapper():";
      message += "\n  Argument is not convertible to \"" + Hurricane::demangle(typeid(TArg).name()) + "\".";
      PyErr_SetString( Isobar3::ConstructorError, message.c_str() );
      return NULL;
    }
    _oInPlace( object, arg );
    Py_INCREF( pyObject );
    return pyObject;
  }

  
// -------------------------------------------------------------------
// C++ Method Wrappers, predicate function.

  template< typename TC >
  class PyPredicateWrapper : public PyWrapper {
    public:
      typedef  bool(TC::* OPredicateType )(void) const;
    public:
      inline               PyPredicateWrapper ( std::string fname, OPredicateType pred )
                                              : PyWrapper()
                                              , _funcName(fname)
                                              , _oPredicate(pred)
                                              { };
      inline  std::string  funcName           () const { return _funcName; };
      virtual int          _predicate         ( PyObject* self );
    private:
      std::string     _funcName;
      OPredicateType  _oPredicate;
  };


  template< typename TC >
  inline int  PyPredicateWrapper<TC>::_predicate ( PyObject* self )
  {
    PyErr_Clear();
    setMessage( funcName() + "(): " );

    TC object;
    if (not pyToC<TC>( self, &object )) {
      string message = "PyPredicateWrapper():";
      message += "\n  Object is not convertible to \"" + Hurricane::demangle(typeid(TC).name()) + "\".";
      PyErr_SetString( Isobar3::ConstructorError, message.c_str() );
      return -1;
    }
    return (object.*_oPredicate)() ? 1 : 0;
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
  inline PyObject* callMethod ( std::string fname, TR(TC::* method )(TArgs...), PyObject* self, PyObject* args )
  {
    cdebug_log(30,0) << "[Python]" << fname << "()" << endl;
    return PyMethodWrapper<TC,TR,TArgs...>( fname, method ).call( (PyObject*)self, args );
  }

  
  template< typename TC, typename TR, typename... TArgs >
  inline PyObject* callMethod ( std::string fname, TR(TC::* method )(TArgs...) const, PyObject* self, PyObject* args )
  {
    return callMethod<TC,TR,TArgs...>( fname, (TR(TC::*)(TArgs...))(method), self, args );
  }

  
  template< typename TC, typename TR, typename... TArgs >
  inline PyObject* callMethod ( std::string fname, TR(* fmethod )(TC*,TArgs...), PyObject* self, PyObject* args )
  {
    cdebug_log(30,0) << "[Python]" << fname << "()" << endl;
    return PyMethodWrapper<TC,TR,TArgs...>( fname, fmethod ).call( (PyObject*)self, args );
  }

  
  template< typename TC, typename TR, typename... TArgs >
  inline PyObject* callMethod ( std::string fname, TR(* fmethod )(const TC*,TArgs...), PyObject* self, PyObject* args )
  {
    return callMethod<TC,TR,TArgs...>( fname, (TR(*)(TC*,TArgs...))(fmethod), self, args );
  }


  template< typename TC, typename TArg >
  inline PyObject* callBinaryMethod ( std::string fname, TC(TC::* mbinary)(TArg), PyObject* pyObject, PyObject* pyArg )
  {
    return PyMBinaryWrapper<TC,TArg>( fname, mbinary).call( pyObject, pyArg );
  }


  template< typename TC, typename TArg >
  inline PyObject* callBinaryMethod ( std::string fname, TC(TC::* mbinary)(TArg) const, PyObject* pyObject, PyObject* pyArg )
  {
    return callBinaryMethod( fname, (TC(TC::* )(TArg))mbinary, pyObject, pyArg );
  }


  template< typename TC, template<typename> class OperatorT >
  inline PyObject* callOperator ( std::string fname, PyObject* pyObject, PyObject* pyArg )
  {
    return PyOperatorWrapper<TC,OperatorT>( fname ).call( pyObject, pyArg );
  }


  template< typename TC, typename TArg >
  inline PyObject* callOperator ( std::string fname, void(* minplace)(TC*,TArg*), PyObject* pyObject, PyObject* pyArg )
  {
    return PyInPlaceOperatorWrapper<TC,TArg>( fname, minplace ).call( pyObject, pyArg );
  }


  template< typename TC >
  inline int  callPredicate ( std::string fname, bool(TC::* predicate)() const, PyObject* pyObject )
  {
    return PyPredicateWrapper<TC>( fname, predicate ).predicate( pyObject );
  }


}  // End of Isobar3 namespace.


#include "hurricane/configuration/PyVector.h"
#include "hurricane/configuration/PyMap.h"
