// ****************************************************************************************************
// File: ./hurricane/Entity.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#ifndef HURRICANE_ENTITY
#define HURRICANE_ENTITY

#include <functional>
#include "hurricane/DBo.h"
#include "hurricane/Entities.h"
#include "hurricane/Box.h"

namespace Hurricane {

  class Cell;
  class Quark;
  class SharedPath;


// -------------------------------------------------------------------
// Class  :  "Hurricane::Entity".

  class Entity : public DBo
  {
    public:
      typedef DBo Inherit;
    public:
      enum Flags { ForcedIdMode = (1<<0)
                 , NextIdSet    = (1<<1)
                 };
    public:
      static  unsigned int  getIdCounter        ();
              unsigned int  getNextId           ();
      static  void          setNextId           ( unsigned int );
      static  bool          inForcedIdMode      ();
      static  void          enableForcedIdMode  ();
      static  void          disableForcedIdMode ();
    public:                                
      inline  unsigned int  getId               () const;
      virtual Cell*         getCell             () const = 0;
      virtual Box           getBoundingBox      () const = 0;
              void          setId               ( unsigned int );
      virtual void          _toJson             ( JsonWriter* ) const;
      virtual string        _getString          () const;
      virtual Record*       _getRecord          () const;
              Quark*        _getQuark           ( SharedPath* sharedPath = NULL ) const;
    protected:                                  
                            Entity              ();
      virtual void          _preDestroy         ();
    private:
      static  unsigned long  _flags;
      static  unsigned int   _nextId;
      static  unsigned int   _idCounter;
              unsigned int   _id;

    public:
      struct CompareById : public std::binary_function<const Entity*,const Entity*,bool> {
          inline bool  operator() ( const Entity* lhs, const Entity* rhs ) const;
      };
  };


  inline  unsigned int  Entity::getId () const { return _id; }
  inline  bool          Entity::CompareById::operator() ( const Entity* lhs, const Entity* rhs ) const
                                                        { return ((lhs)?lhs->getId():0) < ((rhs)?rhs->getId():0); }


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonEntity".

  class JsonEntity : public JsonDBo {
    public:
                                         JsonEntity ( unsigned long flags );
      template<typename T> inline  void  update     ( JsonStack&, T );
  };


  template<typename T> inline void  JsonEntity::update ( JsonStack& stack, T hobject )
  {
    unsigned int jsonId = get<int64_t>(stack,"_id");

    JsonDBo::update<T>( stack, hobject );
    stack.addEntity( jsonId, hobject );
  }


// -------------------------------------------------------------------
// Class  :  "JsonEntityRef".

  class JsonEntityRef : public JsonObject {
    public:
      static  void           initialize    ();
                             JsonEntityRef ( unsigned long flags );
      virtual string         getTypeName   () const;
      virtual JsonEntityRef* clone         ( unsigned long ) const;
      virtual void           toData        ( JsonStack& ); 
  };


} // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Entity);


#endif // HURRICANE_ENTITY

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
