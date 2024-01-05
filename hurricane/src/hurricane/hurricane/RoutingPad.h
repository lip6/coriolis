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
// |  Authors     :          Hugo Clement & Marek Sroka              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/RoutingPad.h"                      |
// +-----------------------------------------------------------------+


#pragma  once
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
      static const uint32_t BiggestArea        = (1 << 0); 
      static const uint32_t HighestLayer       = (1 << 1); 
      static const uint32_t LowestLayer        = (1 << 2); 
      static const uint32_t ShowWarning        = (1 << 3); 
      static const uint32_t IsOnSegment        = (1 << 4); 
      static const uint32_t IsOnPad            = (1 << 5); 
      static const uint32_t ComponentSelection = BiggestArea|HighestLayer|LowestLayer;
      static const uint32_t HSmall             = (1 << 6); 
      static const uint32_t VSmall             = (1 << 7); 
      static const uint32_t Punctual           = (1 << 8); 
      static const uint32_t M1Offgrid          = (1 << 9); 
      static const uint32_t UserCenter         = (1 << 10); 
      static const uint32_t SizeFlags          = HSmall|VSmall|Punctual|M1Offgrid;
      static const uint32_t SelectedComponent  = (1 << 10); 
    public:
      static RoutingPad*   create                ( Net*, Occurrence, uint32_t flags=0 );
      static RoutingPad*   create                ( Pin* );
    public:
    // Accessors.
              bool         isPlacedOccurrence    ( uint32_t flags ) const;
      inline  bool         isAtTopLevel          () const;
      inline  bool         isHSmall              () const;
      inline  bool         isVSmall              () const;
      inline  bool         isPunctual            () const;
      inline  bool         isM1Offgrid           () const;
      inline  bool         hasUserCenter         () const;
      inline  bool         hasSelectedComponent  () const;
      inline  Point        getUserCenter         () const;
      inline  uint32_t     getFlags              () const;
      inline  Occurrence   getOccurrence         () const;
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
              std::string  getStringFlags        () const;
    // Mutators.                                 
      virtual void         translate             ( const DbU::Unit& dx, const DbU::Unit& dy );
              void         setExternalComponent  ( Component* );
              Component*   setOnBestComponent    ( uint32_t flags );
      inline  void         setUserCenter         ( const Point& );
              void         restorePlugOccurrence ();
      inline  void         setFlags              ( uint32_t );
      inline  void         unsetFlags            ( uint32_t );
    // Miscellaeous.
      template<typename T>
              T*           _getEntityAs          () const;
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
      uint32_t    _flags;
      Point       _userCenter;
  };

  
  inline  bool        RoutingPad::isAtTopLevel         () const { return _occurrence.getPath().isEmpty(); }
  inline  bool        RoutingPad::isHSmall             () const { return (_flags & HSmall); }
  inline  bool        RoutingPad::isVSmall             () const { return (_flags & VSmall); }
  inline  bool        RoutingPad::isPunctual           () const { return (_flags & Punctual); }
  inline  bool        RoutingPad::isM1Offgrid          () const { return (_flags & M1Offgrid); }
  inline  bool        RoutingPad::hasSelectedComponent () const { return (_flags & SelectedComponent); }
  inline  bool        RoutingPad::hasUserCenter        () const { return (_flags & UserCenter); }
  inline  Point       RoutingPad::getUserCenter        () const { return (hasUserCenter()) ? _userCenter : getCenter(); }
  inline  uint32_t    RoutingPad::getFlags             () const { return _flags; }
  inline  Occurrence  RoutingPad::getOccurrence        () const { return _occurrence; };
  inline  void        RoutingPad::setFlags             ( uint32_t flags ) { _flags |=  flags; }
  inline  void        RoutingPad::unsetFlags           ( uint32_t flags ) { _flags &= ~flags; }
  inline  void        RoutingPad::setUserCenter        ( const Point& center ) { _flags |=  UserCenter; _userCenter = center; }


  template<typename T>
  inline T* RoutingPad::_getEntityAs () const
  {
    if (not _occurrence.isValid()) return nullptr;
    return dynamic_cast<T*>( _occurrence.getEntity() );
  }


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
