// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |              F o e h n  -  DAG Toolbox                          |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./foehn/DagProperty.h"                        |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include "hurricane/Name.h"
#include "hurricane/Property.h"
#include "hurricane/Slot.h"
namespace Hurricane {
  class Net;
  class Instance;
}


namespace Foehn {

  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::Net;
  using Hurricane::Instance;


// -------------------------------------------------------------------
// Class  :  "Foehn::NetDatas".

  class DagProperty : public Hurricane::PrivateProperty {
    public:
      typedef Hurricane::PrivateProperty  Super;
      static  Name  _name;
    public:
      static  DagProperty*      create          ( DBo* );
      static  Name              getPropertyName ();
      virtual Name              getName         () const;
              bool              isNetOwned      () const;
              bool              isInstanceOwned () const;
      inline  uint64_t          getFlags        () const;
      inline  int32_t           getMinDepth     () const;
      inline  int32_t           getMaxDepth     () const;
      inline  Instance*         getDriver       () const;
      inline  void              setMinDepth     ( int32_t );
      inline  void              setMaxDepth     ( int32_t );
      inline  void              setFlags        ( uint64_t );
      inline  void              resetFlags      ( uint64_t );
      inline  void              setDriver       ( Instance* );
      inline  void              incref          ();
      inline  void              decref          ();
      virtual void              onReleasedBy    ( DBo* owner );
      virtual std::string       _getTypeName    () const;
      virtual std::string       _getString      () const;
      virtual Record*           _getRecord      () const;
    protected:                  
      virtual void              _preDestroy     ();
    protected:
      uint64_t   _flags;
      int32_t    _minDepth;
      int32_t    _maxDepth;
      uint16_t   _refcount;
      Instance*  _driver;
    protected:
      inline  DagProperty ();
  };


  inline  DagProperty::DagProperty ()
    : PrivateProperty()
    , _flags   ()
    , _minDepth(-1)
    , _maxDepth(-1)
    , _refcount(1)
    , _driver  (nullptr)
  { }


  inline  int32_t   DagProperty::getMinDepth () const { return _minDepth; }
  inline  int32_t   DagProperty::getMaxDepth () const { return _maxDepth; }
  inline  void      DagProperty::setMinDepth ( int32_t depth ) { _minDepth = depth; }
  inline  void      DagProperty::setMaxDepth ( int32_t depth ) { _minDepth = depth; }
  inline  uint64_t  DagProperty::getFlags    () const { return _flags; }
  inline  void      DagProperty::setFlags    ( uint64_t flags ) { _flags |= flags; }
  inline  void      DagProperty::resetFlags  ( uint64_t flags ) { _flags &= ~flags; }
  inline  Instance* DagProperty::getDriver   () const { return _driver; }
  inline  void      DagProperty::setDriver   ( Instance* driver ) { _driver = driver; }
  inline  void      DagProperty::incref      () { ++_refcount; }
  inline  void      DagProperty::decref      () { --_refcount; if (_refcount == 0) destroy(); }


// -------------------------------------------------------------------
// Class  :  "Foehn::DagExtension".

  class DagExtension {
    public:
      static const uint64_t  Ignore  = (1<<0);
      static const uint64_t  Reached = (1<<1);
    public:
      static         DagProperty* get             ( const DBo* );
      static         DagProperty* create          (       DBo* );
      static inline  bool         isPresent       ( const DBo* );
      static inline  bool         isNetOwned      ( const DBo* );
      static inline  bool         isInstanceOwned ( const DBo* );
      static inline  uint64_t     getFlags        ( const DBo* );
      static inline  int32_t      getMinDepth     ( const DBo* );
      static inline  int32_t      getMaxDepth     ( const DBo* );
      static inline  Instance*    getDriver       ( const DBo* );
      static inline  void         setFlags        ( const DBo*, uint64_t );
      static inline  void         resetFlags      ( const DBo*, uint64_t );
      static inline  void         setMinDepth     ( const DBo*, int32_t );
      static inline  void         setMaxDepth     ( const DBo*, int32_t );
      static inline  void         setDriver       ( const DBo*, Instance* );
  };

  
  inline  bool  DagExtension::isPresent ( const DBo* dbo )
  {
    DagProperty* property = get( dbo );
    return (property != nullptr);
  }

  
  inline  bool  DagExtension::isNetOwned ( const DBo* dbo )
  {
    DagProperty* property = get( dbo );
    if (not property) return false;
    return property->isNetOwned();
  }

  
  inline  bool  DagExtension::isInstanceOwned ( const DBo* dbo )
  {
    DagProperty* property = get( dbo );
    if (not property) return false;
    return property->isInstanceOwned();
  }

  
  inline  uint64_t  DagExtension::getFlags ( const DBo* dbo )
  {
    DagProperty* property = get( dbo );
    if (not property) return 0;
    return property->getFlags();
  }

  
  inline  int32_t  DagExtension::getMinDepth ( const DBo* dbo )
  {
    DagProperty* property = get( dbo );
    if (not property) return -1;
    return property->getMinDepth();
  }

  
  inline  int32_t  DagExtension::getMaxDepth ( const DBo* dbo )
  {
    DagProperty* property = get( dbo );
    if (not property) return -1;
    return property->getMaxDepth();
  }

  
  inline  Instance* DagExtension::getDriver ( const DBo* dbo )
  {
    DagProperty* property = get( dbo );
    if (not property) return nullptr;
    return property->getDriver();
  }

  
  inline  void  DagExtension::setFlags ( const DBo* dbo, uint64_t flags )
  {
    DagProperty* property = get( dbo );
    if (not property) return;
    property->setFlags( flags );
  }

  
  inline  void  DagExtension::resetFlags ( const DBo* dbo, uint64_t flags )
  {
    DagProperty* property = get( dbo );
    if (not property) return;
    property->resetFlags( flags );
  }

  
  inline  void  DagExtension::setMinDepth ( const DBo* dbo, int32_t depth )
  {
    DagProperty* property = get( dbo );
    if (not property) return;
    property->setMinDepth( depth );
  }

  
  inline  void  DagExtension::setMaxDepth ( const DBo* dbo, int32_t depth )
  {
    DagProperty* property = get( dbo );
    if (not property) return;
    property->setMaxDepth( depth );
  }

  
  inline  void  DagExtension::setDriver ( const DBo* dbo, Instance* instance )
  {
    DagProperty* property = get( dbo );
    if (not property) return;
    property->setDriver( instance );
  }


}  // Foehn Namespace.


INSPECTOR_P_SUPPORT(Foehn::DagProperty);
