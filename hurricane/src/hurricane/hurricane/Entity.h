// ****************************************************************************************************
// File: ./hurricane/Entity.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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

#pragma  once
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
      virtual Cell*         getCell             () const = 0;
      virtual Box           getBoundingBox      () const = 0;
      virtual void          _toJson             ( JsonWriter* ) const;
      virtual string        _getString          () const;
      virtual Record*       _getRecord          () const;
              Quark*        _getQuark           ( SharedPath* sharedPath = NULL ) const;
    protected:                                  
                            Entity              ();
      virtual              ~Entity              ();
      virtual void          _postCreate         ();
      virtual void          _preDestroy         ();
  };


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

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
