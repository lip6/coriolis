// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
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


#ifndef  HURRICANE_DBO_H
#define  HURRICANE_DBO_H

#include  "hurricane/DBos.h"
#include  "hurricane/Name.h"
#include  "hurricane/Properties.h"


namespace Hurricane {

  class JsonDBo;


// -------------------------------------------------------------------
// Class  :  "Hurricane::DBo".

  class DBo {
      friend class JsonDBo;
    public:
      virtual void               destroy           ();
      inline  set<Property*>&    _getPropertySet   ();
              void               _onDestroyed      ( Property* property );
              Property*          getProperty       ( const Name& ) const;
              Properties         getProperties     () const;
      inline  bool               hasProperty       () const;
              void               put               ( Property* );
              void               remove            ( Property* );
              void               removeProperty    ( const Name& );
              void               clearProperties   ();
      virtual void               _toJson           ( JsonWriter* ) const;
      virtual void               _toJsonProperties ( JsonWriter* ) const;
      virtual string             _getTypeName      () const;
      virtual string             _getString        () const;
      virtual Record*            _getRecord        () const;
                                 
    private:                     
      mutable set<Property*>     _propertySet;
                                 
    protected:                   
                                 DBo               ();
      virtual                   ~DBo               ();
      virtual void               _postCreate       ();
      virtual void               _preDestroy       ();
                           
    private:               
                                 DBo               ( const DBo& );
              DBo&               operator=         ( const DBo& );
  };


// Inline Functions.
  inline set<Property*>& DBo::_getPropertySet () { return _propertySet; }
  inline bool            DBo::hasProperty     () const { return !_propertySet.empty(); }


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonDBo".

  class JsonDBo : public JsonObject {
    public:
      JsonDBo ( unsigned long flags );
};


} // Hurricane namespace.


// inline void  jsonWrite ( JsonWriter* w, const Hurricane::DBo* dbo )
// {
//   w->startObject();
//   std::string tname = dbo->_getTypeName();
//   if (w->issetFlags(JsonWriter::UsePlugReference) and (tname == "Plug")) {
//     tname.insert( 0, "&" );
//   }
//   jsonWrite( w, "@typename", tname );
//   dbo->_toJson( w );
//   dbo->_toJsonProperties( w );
//   w->endObject();
// }

INSPECTOR_P_SUPPORT(Hurricane::DBo);

#endif // HURRICANE_DBO_H
