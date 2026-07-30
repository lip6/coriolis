// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2021, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/DBo.h"                             |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Error.h"
#include "hurricane/DBos.h"
#include "hurricane/Name.h"
#include "hurricane/Properties.h"
#include "hurricane/Collection.h"

#include <algorithm>

namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::DBo".

  class DBo {
    private:
      static  FastRTTI  _fastRTTI;
    public:
      static  inline const FastRTTI& fastRTTI  (); 
      virtual        const FastRTTI& vfastRTTI () const; 
    public:
      enum DBoFlags { ForcedIdMode  = (1<<0)
                    , NextIdSet     = (1<<1)
                    };
    public:
      static  void               setMemoryLimit      ( unsigned int );
      static  void               setIdCounterLimit   ( unsigned int );
      static  unsigned int       getIdCounter        ();
              unsigned int       getNextId           ();
      static  void               setNextId           ( unsigned int );
      static  void               resetId             ();
      static  bool               inForcedIdMode      ();
      static  void               enableForcedIdMode  ();
      static  void               disableForcedIdMode ();
      static  void               useIdCounter2       ();
    public:
      virtual void               destroy             ();
      inline  set<Property*>&    _getPropertySet     ();
              void               _onDestroyed        ( Property* property );
      inline  unsigned int       getId               () const;
              Property*          getProperty         ( const Name& ) const;
              Properties         getProperties       () const;
      inline  bool               hasProperty         () const;
              void               setId               ( unsigned int );
              void               put                 ( Property* );
              void               remove              ( Property* );
              void               removeProperty      ( const Name& );
              void               clearProperties     ();
      virtual string             _getTypeName        () const;
      virtual string             _getString          () const;
      virtual Record*            _getRecord          () const;
      virtual void               _toJson             ( JsonWriter* ) const;
      virtual void               _toJsonCollections  ( JsonWriter* ) const;
      virtual void               _toJsonSignature    ( JsonWriter* ) const;
              void               toJson              ( JsonWriter* ) const;
              void               toJsonSignature     ( JsonWriter* ) const;
    protected:                   
                                 DBo                 ();
      virtual                   ~DBo                 () noexcept(false);
      virtual void               _postCreate         ();
      virtual void               _preDestroy         ();
    private:                                         
                                 DBo                 ( const DBo& ) = delete;
              DBo&               operator=           ( const DBo& ) = delete;
    private:                     
      static  unsigned int       _memoryLimit;
      static  unsigned long      _flags;
      static  unsigned int       _nextId;
      static  unsigned int       _idCount;
      static  unsigned int       _idCounter;
      static  unsigned int       _idCounterLimit;
              unsigned int       _id;
      mutable set<Property*>     _propertySet;
    public:
      struct CompareById {
          template<typename Key>
          inline bool  operator() ( const Key* lhs, const Key* rhs ) const;
      };
  };


// Inline Functions.
  inline const FastRTTI& DBo::fastRTTI        () { return _fastRTTI; }
  inline set<Property*>& DBo::_getPropertySet () { return _propertySet; }
  inline bool            DBo::hasProperty     () const { return !_propertySet.empty(); }
  inline unsigned int    DBo::getId           () const { return _id; }
  
  template<typename Key>
  inline bool  DBo::CompareById::operator() ( const Key* lhs, const Key* rhs ) const
  { return ((lhs)?lhs->getId():0) < ((rhs)?rhs->getId():0); }


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonDBo".

  class JsonDBo : public JsonObject {
    public:
                                         JsonDBo ( unsigned int flags );
      template<typename T> inline  void  update  ( JsonStack&, T );
  };


  template<typename T> inline void  JsonDBo::update ( JsonStack& stack, T hobject )
  {
    JsonObject::update<T>( stack, hobject );
    stack.push_back_dbo( dynamic_cast<DBo*>(hobject) );
  }


// -------------------------------------------------------------------
// Class  :  "CachedVector".

  template<typename DBoT>
  class CachedVector : public Hurricane::Collection<DBoT> {

    public:
    // Sub-Class: Locator.
      class Locator : public Hurricane::Locator<DBoT> {
        public:
          inline                            Locator    ( Collection<DBoT>* dbos );
          inline                            Locator    ( const Locator& locator );
          virtual DBoT                      getElement () const;
          virtual Hurricane::Locator<DBoT>* getClone   () const;
          virtual bool                      isValid    () const;
          virtual void                      progress   ();
          virtual string                    _getString () const;
        protected:
          std::vector<DBoT>  _dbos;
          size_t             _index;
      };

    public:
    // CachedVector Methods.
                                           CachedVector ( GenericCollection<DBoT> );
                                           CachedVector ( const Collection<DBoT>& );
                                           CachedVector ( const CachedVector& );
                                          ~CachedVector ();
      virtual unsigned                     getSize      () const;
      virtual Hurricane::Collection<DBoT>* getClone     () const;
	  virtual Hurricane::Locator<DBoT>*    getLocator   () const;
      virtual string                       _getString   () const;
    private:
      Collection<DBoT>* _collection;
  };


  template<typename DBoT>
  inline CachedVector<DBoT> asCachedVector ( GenericCollection<DBoT> dbos )
  { return CachedVector( dbos ); }


  template<typename DBoT>
  inline CachedVector<DBoT> asCachedVector ( const Collection<DBoT>& dbos )
  { return CachedVector( dbos ); }


  template<typename DBoT>
  inline CachedVector<DBoT>::CachedVector ( GenericCollection<DBoT> dbos )
    : Hurricane::Collection<DBoT>()
    , _collection( dbos->getClone() )
  { }


  template<typename DBoT>
  inline CachedVector<DBoT>::CachedVector ( const Collection<DBoT>& dbos )
    : Hurricane::Collection<DBoT>()
    , _collection( dbos.getClone() )
  { }


  template<typename DBoT>
  inline CachedVector<DBoT>::CachedVector ( const CachedVector& other )
    : Hurricane::Collection<DBoT>()
    , _collection( nullptr )
  {
    if (other._collection) _collection = other._collection->getClone();
  }


  template<typename DBoT>
  inline CachedVector<DBoT>::~CachedVector ()
  { delete _collection; }


  template<typename DBoT>
  unsigned  CachedVector<DBoT>::getSize () const
  { return _collection->getSize(); }


  template<typename DBoT>
  Hurricane::Collection<DBoT>* CachedVector<DBoT>::getClone () const
  { return new CachedVector(*this); }


  template<typename DBoT>
  Hurricane::Locator<DBoT>* CachedVector<DBoT>::getLocator () const
  { return new Locator(_collection); }


  template<typename DBoT>
  CachedVector<DBoT>::Locator::Locator ( Collection<DBoT>* dbos )
    : Hurricane::Locator<DBoT>()
    , _dbos ()
    , _index(0)
  {
    for ( const DBoT dbo : *dbos ) _dbos.push_back( dbo );
    std::sort( _dbos.begin(), _dbos.end(), DBo::CompareById() );
  }


  template<typename DBoT>
  inline CachedVector<DBoT>::Locator::Locator ( const Locator &locator )
    : Hurricane::Locator<DBoT>()
    , _dbos (locator._dbos)
    , _index(locator._index)
  { }


  template<typename DBoT>
  Hurricane::Locator<DBoT>* CachedVector<DBoT>::Locator::getClone () const
  { return new Locator(*this); }


  template<typename DBoT>
  bool  CachedVector<DBoT>::Locator::isValid () const
  { return _index < _dbos.size(); }


  template<typename DBoT>
  void  CachedVector<DBoT>::Locator::progress ()
  { if (isValid()) _index++; }


  template<typename DBoT>
  string  CachedVector<DBoT>::Locator::_getString () const
  {
    string s = "<" + _TName("CachedVector::Locator") + " "
                   + ::getString(_index) + "/"
                   + ::getString(_dbos.size())
                   + ">";
    return s;
  }


  template<typename DBoT>
  DBoT  CachedVector<DBoT>::Locator::getElement () const
  { return isValid() ? _dbos[_index] : nullptr; }


  template<typename DBoT>
  string  CachedVector<DBoT>::_getString () const
  {
    string s = "<" + _TName("CachedVector") + " "
                   + ">";
    return s;
  }


} // Hurricane namespace.

INSPECTOR_P_SUPPORT(Hurricane::DBo);
