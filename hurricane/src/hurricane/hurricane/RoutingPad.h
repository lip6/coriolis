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
// |  Authors     :          Hugo Clement & Marek Sroka              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/RoutingPad.h"                      |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_ROUTINGPAD_H
#define HURRICANE_ROUTINGPAD_H

#include "hurricane/Component.h"
#include "hurricane/Occurrence.h"
#include "hurricane/Pin.h"


namespace Hurricane {

  class Segment;


// -------------------------------------------------------------------
// Class  :  "RoutingPad".

  class RoutingPad : public Component {
    public:
      typedef Component Inherit;
      enum    Flags { BiggestArea       = 0x0001
                    , HighestLayer      = 0x0002
                    , LowestLayer       = 0x0004
                    , ComponentSelection= BiggestArea|HighestLayer|LowestLayer
                    , ShowWarning       = 0x0008
                    };
    public:
      static RoutingPad*   create                ( Net*, Occurrence, unsigned int flags=0 );
      static RoutingPad*   create                ( Pin* );
    public:
    // Accessors.
              bool         isPlacedOccurrence    ( unsigned int flags ) const;
      inline  Occurrence   getOccurrence         () const { return _occurrence; };
              Occurrence   getPlugOccurrence     ();
      virtual const Layer* getLayer              () const;
      virtual DbU::Unit    getX                  () const;
      virtual DbU::Unit    getY                  () const;
      virtual Box          getBoundingBox        () const;
      virtual Box          getBoundingBox        ( const BasicLayer* ) const;
      virtual Point        getCenter             () const;
      virtual Point        getPosition           () const;
              Point        getSourcePosition     () const;
              Point        getTargetPosition     () const;
              DbU::Unit    getSourceX            () const;
              DbU::Unit    getSourceY            () const;
              DbU::Unit    getTargetX            () const;
              DbU::Unit    getTargetY            () const;
    // Mutators.                                 
      virtual void         translate             ( const DbU::Unit& dx, const DbU::Unit& dy );
              void         setExternalComponent  ( Component* );
              Component*   setOnBestComponent    ( unsigned int flags );
              void         restorePlugOccurrence ();
    // Miscellaeous.
              Component*   _getEntityAsComponent () const;
              Segment*     _getEntityAsSegment   () const;
      virtual void         _toJson               ( JsonWriter* ) const;
      virtual std::string  _getTypeName          () const {return _TName("RoutingPad");};
      virtual std::string  _getString            () const;
      virtual Record*      _getRecord            () const;
    protected:
      virtual void         _postCreate           ();
      virtual void         _preDestroy           ();
    private:
                           RoutingPad            ( Net*, Occurrence occurrence=Occurrence() );
    private:
    // Attributes.
      Occurrence  _occurrence;
  };


// -------------------------------------------------------------------
// Class  :  "JsonRoutingPad".

  class JsonRoutingPad : public JsonComponent {
    public:
      static  void            initialize     ();
                              JsonRoutingPad ( unsigned long flags );
      virtual std::string     getTypeName    () const;
      virtual JsonRoutingPad* clone          ( unsigned long flags ) const;
      virtual void            toData         ( JsonStack& ); 
  };

} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::RoutingPad);

#endif  // HURRICANE_ROUTINGPAD_H
