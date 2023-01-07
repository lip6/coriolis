// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |               C o r e   L i b r a r y                           |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./RoutingLayerGauge.h"                    |
// +-----------------------------------------------------------------+


#pragma  once
#include <map>
#include "hurricane/Commons.h"
#include "hurricane/Error.h"
#include "hurricane/DbU.h"
#include "hurricane/Collection.h"
#include "hurricane/Slot.h"
#include "crlcore/Utilities.h"

namespace Hurricane {
  class Layer;
}


namespace Constant {

  enum Direction      { Horizontal        = (1 << 0)
                      , Vertical          = (1 << 1)
                      };

  enum LayerGaugeType { Bottom            = (1 << 3)
                      , Unusable          = (1 << 4)
                      , PowerSupply       = (1 << 5)
                      , PinOnly           = (1 << 6)
                      , Default           = (1 << 7)
                      , BottomPowerSupply = (1 << 8)
                      };

  enum Round          { Superior          = (1 << 10)
                      , Inferior          = (1 << 11)
                      , Nearest           = (1 << 12)
                      , Exact             = (1 << 13)
                      };

  Direction  perpandicular ( unsigned int );

}



namespace CRL {

  using std::map;
  using Hurricane::JsonObject;
  using Hurricane::JsonStack;
  using Hurricane::Initializer;
  using Hurricane::GenericCollection;
  using Hurricane::GenericLocator;
  using Hurricane::GenericFilter;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Layer;


  class RoutingGauge;


// -------------------------------------------------------------------
// Class  :  "RoutingLayerGauge".
 

  class RoutingLayerGauge {

    public:
    // Constructors & Destructors.
      static  RoutingLayerGauge*        create           ( const Layer*              layer
                                                         , Constant::Direction       direction
                                                         , Constant::LayerGaugeType  type
                                                         , unsigned int              depth
                                                         , double                    density
                                                         , DbU::Unit                 offset
                                                         , DbU::Unit                 pitch
                                                         , DbU::Unit                 wireWidth
                                                         , DbU::Unit                 pwireWidth
                                                         , DbU::Unit                 viaWidth
                                                         , DbU::Unit                 obsDw );
      virtual void                      destroy          ();
    // Accessors.                       
      inline  bool                      isHorizontal     () const;
      inline  bool                      isVertical       () const;
      inline  const Layer*              getLayer         () const;
      inline  const Layer*              getBlockageLayer () const;
              unsigned int              getDepth         () const;
      inline  Constant::Direction       getDirection     () const;
      inline  Constant::LayerGaugeType  getType          () const;
      inline  double                    getDensity       () const;
      inline  DbU::Unit                 getOffset        () const;
      inline  DbU::Unit                 getPitch         () const;
      inline  DbU::Unit                 getHalfPitch     () const;
      inline  DbU::Unit                 getWireWidth     () const;
      inline  DbU::Unit                 getPWireWidth    () const;
      inline  DbU::Unit                 getHalfWireWidth () const;
      inline  DbU::Unit                 getViaWidth      () const;
      inline  DbU::Unit                 getHalfViaWidth  () const;
      inline  DbU::Unit                 getObstacleDw    () const;
              void                      divide           ( DbU::Unit dividend, long& quotient, long& modulo ) const;
              unsigned int              getTrackNumber   ( DbU::Unit start, DbU::Unit stop ) const;
              long                      getTrackIndex    ( DbU::Unit start, DbU::Unit stop, DbU::Unit position, unsigned mode ) const;
      inline  DbU::Unit                 getTrackPosition ( DbU::Unit start, DbU::Unit stop, DbU::Unit position, unsigned mode ) const;
              DbU::Unit                 getTrackPosition ( DbU::Unit start, long index ) const;
      inline  void                      setRoutingGauge  ( RoutingGauge* );
      inline  void                      setPWireWidth    ( DbU::Unit );
      inline  void                      setType          ( uint32_t );
    // Hurricane Managment.             
              void                      toJson           ( JsonWriter* ) const;
      virtual string                    _getTypeName     () const;
      virtual string                    _getString       () const;
      virtual Record*                   _getRecord       () const;

    private:
    // Internal - Attributes.
              RoutingGauge*             _routingGauge;
              const Layer*              _layer;
              const Layer*              _blockageLayer;
              Constant::Direction       _direction;
              Constant::LayerGaugeType  _type;
              unsigned int              _depth;
              double                    _density;
              DbU::Unit                 _offset;
              DbU::Unit                 _pitch;
              DbU::Unit                 _wireWidth;
              DbU::Unit                 _pwireWidth;
              DbU::Unit                 _viaWidth;
              DbU::Unit                 _obstacleDw;

    // Internal - Constructors & Destructors.
                                 RoutingLayerGauge ( const Layer*              layer
                                                   , Constant::Direction       direction
                                                   , Constant::LayerGaugeType  type
                                                   , unsigned int              depth
                                                   , double                    density
                                                   , DbU::Unit                 offset
                                                   , DbU::Unit                 pitch
                                                   , DbU::Unit                 wireWidth
                                                   , DbU::Unit                 pwireWidth
                                                   , DbU::Unit                 viaWidth
                                                   , DbU::Unit                 obsDw );
                                 RoutingLayerGauge ( const RoutingLayerGauge& ) = delete;
      virtual                   ~RoutingLayerGauge ();
      virtual void               _preDestroy();
              RoutingLayerGauge& operator=         ( const RoutingLayerGauge& ) = delete;

    // Friends.
      friend class RoutingGauge;
  };


  // New Types.
  typedef  map<Layer*,RoutingLayerGauge*>  RoutingLayerGaugeMap;


// -------------------------------------------------------------------
// Collection  : "RoutingLayerGauges".

  typedef  GenericCollection<RoutingLayerGauge*>  RoutingLayerGauges;
  typedef  GenericLocator<RoutingLayerGauge*>     RoutingLayerGaugeLocator;
  typedef  GenericFilter<RoutingLayerGauge*>      RoutingLayerGaugeFilter;


// -------------------------------------------------------------------
// Inline Functions.

  inline  bool                      RoutingLayerGauge::isHorizontal     () const { return (_direction == Constant::Direction::Horizontal); }
  inline  bool                      RoutingLayerGauge::isVertical       () const { return (_direction == Constant::Direction::Vertical); }
  inline  const Layer*              RoutingLayerGauge::getLayer         () const { return _layer; }
  inline  unsigned int              RoutingLayerGauge::getDepth         () const { return _depth; }
  inline  const Layer*              RoutingLayerGauge::getBlockageLayer () const { return _blockageLayer; }
  inline  Constant::Direction       RoutingLayerGauge::getDirection     () const { return _direction; }
  inline  Constant::LayerGaugeType  RoutingLayerGauge::getType          () const { return _type; }
  inline  double                    RoutingLayerGauge::getDensity       () const { return _density; }
  inline  DbU::Unit                 RoutingLayerGauge::getOffset        () const { return _offset; }
  inline  DbU::Unit                 RoutingLayerGauge::getPitch         () const { return _pitch; }
  inline  DbU::Unit                 RoutingLayerGauge::getHalfPitch     () const { return _pitch>>1; }
  inline  DbU::Unit                 RoutingLayerGauge::getWireWidth     () const { return _wireWidth; }
  inline  DbU::Unit                 RoutingLayerGauge::getPWireWidth    () const { return (_pwireWidth) ? _pwireWidth : _wireWidth; }
  inline  DbU::Unit                 RoutingLayerGauge::getHalfWireWidth () const { return _wireWidth>>1; }
  inline  DbU::Unit                 RoutingLayerGauge::getViaWidth      () const { return _viaWidth; }
  inline  DbU::Unit                 RoutingLayerGauge::getHalfViaWidth  () const { return _viaWidth>>1; }
  inline  DbU::Unit                 RoutingLayerGauge::getObstacleDw    () const { return _obstacleDw; }
  inline  DbU::Unit                 RoutingLayerGauge::getTrackPosition ( DbU::Unit start, DbU::Unit stop, DbU::Unit position, unsigned mode ) const
                                                                        { return getTrackPosition( start, getTrackIndex(start,stop,position,mode) ); }
  inline  void                      RoutingLayerGauge::setPWireWidth    ( DbU::Unit pwidth ) { _pwireWidth = pwidth; }
  inline  void                      RoutingLayerGauge::setType          ( uint32_t type ) { _type = (Constant::LayerGaugeType)type; }
  inline  void                      RoutingLayerGauge::setRoutingGauge  ( RoutingGauge* rg ) { _routingGauge = rg; }


// -------------------------------------------------------------------
// Class  :  "JsonRoutingLayerGauge".

  class JsonRoutingLayerGauge : public JsonObject {
    public:
      static  void                   initialize            ();
                                     JsonRoutingLayerGauge ( unsigned long flags );
      virtual string                 getTypeName           () const;
      virtual JsonRoutingLayerGauge* clone                 ( unsigned long flags ) const;
      virtual void                   toData                ( JsonStack& );
  };


} // CRL namespace.


INSPECTOR_P_SUPPORT(CRL::RoutingLayerGauge);


// -------------------------------------------------------------------
// Inspector Support for  :  "const ::Constant::Direction*".


inline void  from ( Constant::Direction& direction, const std::string& s )
{
  if (s == "Vertical") direction = Constant::Vertical;
  else {
    if (s != "Horizontal")
      std::cerr << Hurricane::Error( "::from(Direction&,string&): Unknown value \"%s\"."
                                   , s.c_str() ) << std::endl;
    direction = Constant::Horizontal;
  }
}


template<>
inline std::string  getString<const Constant::Direction*>
                            ( const Constant::Direction* direction )
{
  switch ( *direction ) {
    case Constant::Horizontal: return "Horizontal";
    case Constant::Vertical:   return "Vertical";
  }
  return "Unknown Constant::Direction";
}


template<>
inline std::string  getString<Constant::Direction>
                            ( Constant::Direction direction )
{
  switch ( direction ) {
    case Constant::Horizontal: return "Horizontal";
    case Constant::Vertical:   return "Vertical";
  }
  return "Unknown Constant::Direction";
}


IOSTREAM_POINTER_SUPPORT(Constant::Direction);


// -------------------------------------------------------------------
// Inspector Support for  :  "const Constant::LayerGaugeType*".


inline void  from ( Constant::LayerGaugeType& type, const std::string& s )
{
  if      (s == "Unusable"          ) type = Constant::Unusable;
  else if (s == "PinOnly"           ) type = Constant::PinOnly;
  else if (s == "PowerSupply"       ) type = Constant::PowerSupply;
  else if (s == "PowerSupply|Bottom") type = Constant::BottomPowerSupply;
  else {
    if (s != "Default")
      std::cerr << Hurricane::Error( "::from(LayerGaugeType&,string&): Unknown value \"%s\"."
                                   , s.c_str() ) << std::endl;
    type = Constant::Default;
  }
}


template<>
inline std::string  getString<const Constant::LayerGaugeType*>
                            ( const Constant::LayerGaugeType* layerGaugeType )
{
  switch ( *layerGaugeType ) {
    case Constant::Bottom:            return "Bottom (error)";
    case Constant::Unusable:          return "Unusable";
    case Constant::PinOnly:           return "PinOnly";
    case Constant::Default:           return "Default";
    case Constant::PowerSupply:       return "PowerSupply";
    case Constant::BottomPowerSupply: return "PowerSupply|Bottom";
  }
  return "Unknown Constant::LayerGaugeType";
}


template<>
inline std::string  getString<Constant::LayerGaugeType*>
                            ( Constant::LayerGaugeType* layerGaugeType )
{ return getString<const Constant::LayerGaugeType*>(layerGaugeType); }


template<>
inline std::string  getString<const Constant::LayerGaugeType>
                            ( const Constant::LayerGaugeType layerGaugeType )
{
  switch ( layerGaugeType ) {
    case Constant::Bottom:            return "Bottom (error)";
    case Constant::Unusable:          return "Unusable";
    case Constant::PinOnly:           return "PinOnly";
    case Constant::Default:           return "Default";
    case Constant::PowerSupply:       return "PowerSupply";
    case Constant::BottomPowerSupply: return "PowerSupply|Bottom";
  }
  return "Unknown Constant::LayerGaugeType";
}


template<>
inline std::string  getString<Constant::LayerGaugeType>
                            ( Constant::LayerGaugeType layerGaugeType )
{ return getString<const Constant::LayerGaugeType>(layerGaugeType); }


IOSTREAM_POINTER_SUPPORT(Constant::LayerGaugeType);
