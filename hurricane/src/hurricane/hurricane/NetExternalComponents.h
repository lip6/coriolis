// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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
// |  Author      :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/NetExternalComponents.h"           |
// +-----------------------------------------------------------------+


#pragma   once
#include  "hurricane/Component.h"
#include  "hurricane/Relation.h"


namespace Hurricane {

// -------------------------------------------------------------------
// Class  :  "NetExternalComponents".

  class NetExternalComponents {
    public:
      static Components        get         ( const Net* );
      static void              setInternal ( Component* );
      static void              setExternal ( Component* );
      static bool              isExternal  ( const Component* );
      static void              toJson      ( JsonWriter*, const Net* );
    protected:
      static StandardRelation* getRelation ( const Net* );
    private:
      static const Name  _name;
  };


// -------------------------------------------------------------------
// Class  :  "JsonNetExternalComponents".

  class JsonNetExternalComponents : public JsonBaseArray<Entity*> {
    public:
      static  void                       initialize                ();
                                         JsonNetExternalComponents ( unsigned long flags );
      virtual                           ~JsonNetExternalComponents ();
      virtual string                     getTypeName               () const;
      virtual JsonNetExternalComponents* clone                     ( unsigned long ) const;
      virtual void                       toData                    ( JsonStack& ); 
  };


} // Hurricane namespace.
