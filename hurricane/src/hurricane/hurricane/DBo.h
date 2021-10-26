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


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::DBo".

  class DBo {
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
      struct CompareById : public std::binary_function<const DBo*,const DBo*,bool> {
          template<typename Key>
          inline bool  operator() ( const Key* lhs, const Key* rhs ) const;
      };
  };


// Inline Functions.
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


} // Hurricane namespace.

INSPECTOR_P_SUPPORT(Hurricane::DBo);
