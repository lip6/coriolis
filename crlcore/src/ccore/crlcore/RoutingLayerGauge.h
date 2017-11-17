// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
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


#ifndef  CRL_ROUTING_LAYER_GAUGE_H
#define  CRL_ROUTING_LAYER_GAUGE_H


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

  enum Direction      { Horizontal   = (1<<0)
                      , Vertical     = (1<<1)
                      };

  enum LayerGaugeType { Default      = (1<<0)
                      , PinOnly      = (1<<1)
                      };

  enum Round          { Superior     = (1<<2)
                      , Inferior     = (1<<3)
                      , Nearest      = (1<<4)
                      , Exact        = (1<<5)
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
                                                         , DbU::Unit                 viaWidth
                                                         , DbU::Unit                 obsDw );
      virtual void                      destroy          ();
    // Accessors.                       
      inline  bool                      isHorizontal     () const;
      inline  bool                      isVertical       () const;
      inline  const Layer*              getLayer         () const;
      inline  const Layer*              getBlockageLayer () const;
      inline  unsigned int              getDepth         () const;
      inline  Constant::Direction       getDirection     () const;
      inline  Constant::LayerGaugeType  getType          () const;
      inline  double                    getDensity       () const;
      inline  DbU::Unit                 getOffset        () const;
      inline  DbU::Unit                 getPitch         () const;
      inline  DbU::Unit                 getHalfPitch     () const;
      inline  DbU::Unit                 getWireWidth     () const;
      inline  DbU::Unit                 getHalfWireWidth () const;
      inline  DbU::Unit                 getViaWidth      () const;
      inline  DbU::Unit                 getHalfViaWidth  () const;
      inline  DbU::Unit                 getObstacleDw    () const;
              void                      divide           ( DbU::Unit dividend, long& quotient, long& modulo ) const;
              unsigned int              getTrackNumber   ( DbU::Unit start, DbU::Unit stop ) const;
              unsigned int              getTrackIndex    ( DbU::Unit start, DbU::Unit stop, DbU::Unit position, unsigned mode ) const;
              DbU::Unit                 getTrackPosition ( DbU::Unit start, unsigned depth ) const;
    // Hurricane Managment.             
              void                      toJson           ( JsonWriter* ) const;
      virtual string                    _getTypeName     () const;
      virtual string                    _getString       () const;
      virtual Record*                   _getRecord       () const;

    protected:
    // Internal - Attributes.
              const Layer*              _layer;
              const Layer*              _blockageLayer;
              Constant::Direction       _direction;
              Constant::LayerGaugeType  _type;
              unsigned int              _depth;
              double                    _density;
              DbU::Unit                 _offset;
              DbU::Unit                 _pitch;
              DbU::Unit                 _wireWidth;
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
                                                   , DbU::Unit                 viaWidth
                                                   , DbU::Unit                 obsDw );
      virtual                   ~RoutingLayerGauge ();
      virtual void               _preDestroy();
              RoutingLayerGauge& operator=         ( const RoutingLayerGauge& );

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
  inline  const Layer*              RoutingLayerGauge::getBlockageLayer () const { return _blockageLayer; }
  inline  Constant::Direction       RoutingLayerGauge::getDirection     () const { return _direction; }
  inline  Constant::LayerGaugeType  RoutingLayerGauge::getType          () const { return _type; }
  inline  unsigned int              RoutingLayerGauge::getDepth         () const { return _depth; }
  inline  double                    RoutingLayerGauge::getDensity       () const { return _density; }
  inline  DbU::Unit                 RoutingLayerGauge::getOffset        () const { return _offset; }
  inline  DbU::Unit                 RoutingLayerGauge::getPitch         () const { return _pitch; }
  inline  DbU::Unit                 RoutingLayerGauge::getHalfPitch     () const { return _pitch>>1; }
  inline  DbU::Unit                 RoutingLayerGauge::getWireWidth     () const { return _wireWidth; }
  inline  DbU::Unit                 RoutingLayerGauge::getHalfWireWidth () const { return _wireWidth>>1; }
  inline  DbU::Unit                 RoutingLayerGauge::getViaWidth      () const { return _viaWidth; }
  inline  DbU::Unit                 RoutingLayerGauge::getHalfViaWidth  () const { return _viaWidth>>1; }
  inline  DbU::Unit                 RoutingLayerGauge::getObstacleDw    () const { return _obstacleDw; }


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
  if (s == "PinOnly") type = Constant::PinOnly;
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
    case Constant::Default: return "Default";
    case Constant::PinOnly: return "PinOnly";
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
    case Constant::Default: return "Default";
    case Constant::PinOnly: return "PinOnly";
  }
  return "Unknown Constant::LayerGaugeType";
}


template<>
inline std::string  getString<Constant::LayerGaugeType>
                            ( Constant::LayerGaugeType layerGaugeType )
{ return getString<const Constant::LayerGaugeType>(layerGaugeType); }


IOSTREAM_POINTER_SUPPORT(Constant::LayerGaugeType);


#endif  // CRL_ROUTING_LAYER_GAUGE_H
