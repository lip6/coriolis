
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2024-2024.
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./anabatic/LandingZone.h"                     |
// +-----------------------------------------------------------------+


#pragma  once
#include <cstddef>
#include <string>
#include "hurricane/Box.h"
#include "hurricane/Name.h"
#include "hurricane/Property.h"
#include "hurricane/Slot.h"
#include "hurricane/Component.h"


namespace Anabatic {

  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::Box;
  using Hurricane::DBo;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::Component;
  using Hurricane::PrivateProperty;
  class Configuration;


// -------------------------------------------------------------------
// Class  :  "Anabatic::LandingZone".

  class LandingZone : public Hurricane::PrivateProperty {
      friend class LandingZoneExtension;
    public:
      static  Name                _name;
    public:
      static  void         expandFromBb     ( const Configuration*, Net*, const Layer*, Box, std::vector<Box>& );
    public:
      static  Name         getPropertyName  ();
      virtual Name         getName          () const;
      virtual void         onReleasedBy     ( DBo* owner );
      inline  const Box&   getArea          () const;
      inline  Component*   getComponent     () const;
      virtual std::string  _getTypeName     () const;
      virtual std::string  _getString       () const;
      virtual Record*      _getRecord       () const;
    protected:
    // Attributes.
      Box  _area;
    protected:
    // Constructor.
      LandingZone ( const Configuration*, Component* );
      LandingZone ( const Configuration*, Net* );
  };


  inline  const Box&   LandingZone::getArea      () const { return _area; }
  inline  Component*   LandingZone::getComponent () const { return dynamic_cast<Component*>( getOwner() ); }


// -------------------------------------------------------------------
// Class  :  "Anabatic::LandingZoneExtension".

  class LandingZoneExtension {
    public:
      static inline  bool          isPresent ( const Component* );
      static inline  Box           getArea   ( const Component* );
      static         LandingZone*  get       ( const Component* );
      static         LandingZone*  create    ( const Configuration*, Net* );
      static         LandingZone*  create    ( const Configuration*, Component* );
    private:
      static const Component* _owner;
      static LandingZone*     _cache;
  };


  inline bool  LandingZoneExtension::isPresent ( const Component* component )
  {
    LandingZone* lz = get( component );
    return (lz != nullptr);
  }


  inline Box  LandingZoneExtension::getArea ( const Component* component )
  {
    LandingZone* lz = get( component );
    return (lz == nullptr) ? Box() : lz->getArea();
  }



}  // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::LandingZone);
