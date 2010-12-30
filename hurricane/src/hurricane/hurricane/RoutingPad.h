
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2010, All Rights Reserved
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
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Authors     :          Hugo Clement & Marek Sroka              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/RegularLayer.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __HURRICANE_ROUTINGPAD__
#define __HURRICANE_ROUTINGPAD__

#include "hurricane/Component.h"
#include "hurricane/Occurrence.h"
#include "hurricane/Pin.h"


namespace Hurricane {

  class Segment;


  class RoutingPad : public Component {
    public:
      typedef Component Inherit;
      enum    Flags { BiggestArea       =0x1
                    , HighestLayer      =0x2
                    , LowestLayer       =0x4
                    , ComponentSelection=BiggestArea|HighestLayer|LowestLayer
                    };
    public:
      static RoutingPad*   create                ( Net*, Occurrence, unsigned int flags=0 );
      static RoutingPad*   create                ( Pin* );
    public:
    // Accessors.
      virtual DbU::Unit    getX                  () const;
      virtual DbU::Unit    getY                  () const;
      virtual Box          getBoundingBox        () const;
      virtual const Layer* getLayer              () const;
      virtual Box          getBoundingBox        ( const BasicLayer* ) const;
      virtual Point        getCenter             () const;
      inline  Occurrence   getOccurrence         () const { return _occurrence; };
              Occurrence   getPlugOccurrence     ();
              Point        getSourcePosition     () const;
              Point        getTargetPosition     () const;
              DbU::Unit    getSourceX            () const;
              DbU::Unit    getSourceY            () const;
              DbU::Unit    getTargetX            () const;
              DbU::Unit    getTargetY            () const;
    // Mutators.                                 
      virtual void         translate             ( const DbU::Unit& dx, const DbU::Unit& dy );
              void         setX                  ( const DbU::Unit& );
              void         setY                  ( const DbU::Unit& );
              void         setPosition           ( const DbU::Unit& x, const DbU::Unit& y );
              void         setPosition           ( const Point& position );
              void         setOffset             ( const DbU::Unit& dx, const DbU::Unit& dy );
              void         setExternalComponent  ( Component* );
              Component*   setOnBestComponent    ( unsigned int flags );
              void         restorePlugOccurrence ();
    // Miscellaeous.
              Component*   _getEntityAsComponent () const;
              Segment*     _getEntityAsSegment   () const;
      virtual std::string  _getTypeName          () const {return _TName("RoutingPad");};
      virtual std::string  _getString            () const;
      virtual Record*      _getRecord            () const;
    protected:
      virtual void         _postCreate           ();
      virtual void         _preDestroy           ();
    private:
                           RoutingPad            ( Net*, const Point&, Occurrence occurrence=Occurrence() );
    private:
    // Attributes.
      DbU::Unit   _x;
      DbU::Unit   _y;
      Occurrence  _occurrence;
  };


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::RoutingPad);

#endif  // __HURRICANE_ROUTINGPAD__
