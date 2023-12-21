// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingGauge.cpp"                       |
// +-----------------------------------------------------------------+


#include <limits>
#include <sstream>
#include <algorithm>
#include "hurricane/Commons.h"
#include "hurricane/ViaLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "crlcore/RoutingLayerGauge.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/AllianceFramework.h"


namespace {

  const char* dupLayerGauge =
    "RoutingGauge::AddLayerGauge() :\n\n"
    "    Attempt to re-define layer gauge %s in routing gauge %s.\n";


} // End of anonymous namespace.



namespace CRL {

  using namespace std;
  using Hurricane::JsonTypes;
  using Hurricane::JsonArray;
  using Hurricane::DataBase;
  using Hurricane::BasicLayer;
  using Hurricane::ViaLayer;
  using Hurricane::getCollection;


// -------------------------------------------------------------------
// Class  :  "RoutingGauge".


  const size_t  RoutingGauge::nlayerdepth = std::numeric_limits<size_t>::max();


  RoutingGauge::RoutingGauge ( const char* name )
    : _name             (name)
    , _layerGauges      ()
    , _viaLayers        ()
    , _technology       (DataBase::getDB()->getTechnology())
    , _isSymbolic       (true)
    , _isSuperPitched   (true)
    , _firstRoutingLayer(0)
    , _usableLayers     (0)
  { }


  RoutingGauge::RoutingGauge ( const RoutingGauge& gauge )
    : _name             (gauge._name)
    , _layerGauges      ()
    , _viaLayers        ()
    , _technology       (gauge._technology)
    , _isSymbolic       (gauge._isSymbolic)
    , _isSuperPitched   (gauge._isSuperPitched)
    , _firstRoutingLayer(0)
    , _usableLayers     (0)
  {
  // Make a deep copy of the map.
    for ( size_t i=0 ; i<gauge._layerGauges.size() ; i++ ) {
      addLayerGauge ( RoutingLayerGauge::create 
                      ( gauge._layerGauges[i]->getLayer()
                      , gauge._layerGauges[i]->getDirection()
                      , gauge._layerGauges[i]->getType()
                      , gauge._layerGauges[i]->getDepth()
                      , gauge._layerGauges[i]->getDensity()
                      , gauge._layerGauges[i]->getOffset()
                      , gauge._layerGauges[i]->getPitch()
                      , gauge._layerGauges[i]->getWireWidth()
                      , gauge._layerGauges[i]->getPWireWidth()
                      , gauge._layerGauges[i]->getViaWidth()
                      , gauge._layerGauges[i]->getObstacleDw() )
                    );
    }
  }


  RoutingGauge* RoutingGauge::create ( const char* name )
  {
    RoutingGauge* gauge = new RoutingGauge ( name );

    return gauge;
  }


  void  RoutingGauge::_preDestroy () {
    for ( size_t i=0 ; i<_layerGauges.size() ; i++ )
      _layerGauges[i]->destroy ();
  }


  void  RoutingGauge::destroy() {
    _preDestroy ();

    delete this;
  }


  RoutingGauge::~RoutingGauge ()
  {
  }


  RoutingGauge* RoutingGauge::getClone () const
  {
    return new RoutingGauge ( *this );
  }


  bool  RoutingGauge::hasLayer ( const Layer* layer ) const
  { return (getLayerGauge(layer) != NULL) or (getViaDepth(layer) != nlayerdepth); }


  RoutingLayerGauge* RoutingGauge::getPowerSupplyGauge () const
  {
    size_t i = _layerGauges.size();
    if (i == 0) return NULL;
    do {
      --i;
      if (_layerGauges[i]->getType() == Constant::PowerSupply)
        return _layerGauges[i];
    } while ( i > 0);
    return NULL;
  }


  RoutingLayerGauge* RoutingGauge::getFirstRoutingGauge () const
  {
    for ( RoutingLayerGauge* rlg : _layerGauges ) {
      if (rlg->getType() == Constant::LayerGaugeType::Default)
        return rlg;
    }
    return nullptr;
  }


  RoutingLayerGauge* RoutingGauge::getHorizontalGauge () const
  {
    RoutingLayerGauge* pinOnly = NULL;
    for ( size_t i=_firstRoutingLayer ; i<_layerGauges.size() ; ++i ) {
      RoutingLayerGauge* gauge = _layerGauges[i];
      if (gauge->isHorizontal()) {
        if (gauge->isUsable())
          return gauge;
        else if (not pinOnly)
          pinOnly = gauge;
      }
    }
    return pinOnly;
  }


  RoutingLayerGauge* RoutingGauge::getVerticalGauge () const
  {
    RoutingLayerGauge* pinOnly = NULL;
    for ( size_t i=_firstRoutingLayer ; i<_layerGauges.size() ; ++i ) {
      RoutingLayerGauge* gauge = _layerGauges[i];
      if (gauge->isVertical()) {
        if (gauge->isUsable())
          return gauge;
        else if (not pinOnly)
          pinOnly = gauge;
      }
    }
    return pinOnly;
  }


  bool  RoutingGauge::isUsable ( const Layer* layer ) const
  {
    for ( size_t i=0 ; i < _layerGauges.size() ; i++ ) {
      if (_layerGauges[i]->getLayer()->getMask() == layer->getMask())
        return _layerGauges[i]->isUsable();
    }
    return false;
  }
  
 
  RoutingLayerGauge* RoutingGauge::getLayerGauge ( size_t depth ) const
  {
    if ( depth >= _layerGauges.size() ) return NULL;
    return _layerGauges[depth];
  }


  RoutingLayerGauge* RoutingGauge::getLayerGauge ( const Layer* layer ) const
  {
    for ( size_t i=0 ; i < _layerGauges.size() ; i++ ) {
      if (_layerGauges[i]->getLayer()->getMask() == layer->getMask())
        return _layerGauges[i];
    }
    return NULL;
  }


  const Layer* RoutingGauge::getRoutingLayer ( size_t depth ) const
  {
    if ( depth >= _layerGauges.size() ) return NULL;
    return _layerGauges[depth]->getLayer();
  }


  Layer* RoutingGauge::getContactLayer ( size_t depth ) const
  {
    if ( depth >= _viaLayers.size() ) return NULL;
    return _viaLayers[depth];
  }


  unsigned int  RoutingGauge::getLayerType ( const Layer* layer ) const
  {
    RoutingLayerGauge* layerGauge = getLayerGauge(layer);
    if (not layerGauge) return 0;

    return layerGauge->getType();
  }


  unsigned int  RoutingGauge::getLayerDirection ( const Layer* layer ) const
  {
    RoutingLayerGauge* layerGauge = getLayerGauge(layer);
    if (not layerGauge) return 0;

    return layerGauge->getDirection();
  }


  size_t  RoutingGauge::getViaDepth ( const Layer* layer ) const
  {
    const Layer* bottomLayer = layer;
    const Layer* viaLayer    = dynamic_cast<const ViaLayer*>(layer);

    if (viaLayer) bottomLayer = viaLayer->getBottom();

    for ( size_t i=0 ; i < _layerGauges.size() ; i++ ) {
      if (_layerGauges[i]->getLayer()->getMask() == bottomLayer->getMask()) {
        return i;
      }
    }
    return nlayerdepth;
  }


  size_t  RoutingGauge::getLayerDepth ( const Layer* layer ) const
  {
    for ( size_t i=0 ; i < _layerGauges.size() ; i++ ) {
      if (_layerGauges[i]->getLayer()->getMask() == layer->getMask()) {
        return i;
      }
    }
    return nlayerdepth;
  }


  DbU::Unit  RoutingGauge::getPitch ( const Layer* layer ) const
  {
    size_t depth = getLayerDepth( layer );
    return (depth != nlayerdepth) ? _layerGauges[depth]->getPitch() : 0;
  }


  DbU::Unit  RoutingGauge::getOffset ( const Layer* layer ) const
  {
    size_t depth = getLayerDepth( layer );
    return (depth != nlayerdepth) ? _layerGauges[depth]->getOffset() : 0;
  }


  DbU::Unit  RoutingGauge::getWireWidth ( const Layer* layer ) const
  {
    size_t depth = getLayerDepth( layer );
    return (depth != nlayerdepth) ? _layerGauges[depth]->getWireWidth() : 0;
  }


  DbU::Unit  RoutingGauge::getPWireWidth ( const Layer* layer ) const
  {
    size_t depth = getLayerDepth( layer );
    return (depth != nlayerdepth) ? _layerGauges[depth]->getPWireWidth() : 0;
  }


  DbU::Unit  RoutingGauge::getViaWidth ( const Layer* layer ) const
  {
    size_t depth = getLayerDepth( layer );
    return (depth != nlayerdepth) ? _layerGauges[depth]->getViaWidth() : 0;
  }


  const vector<RoutingLayerGauge*>&  RoutingGauge::getLayerGauges () const
  {
    return _layerGauges;
  }


  void  RoutingGauge::addLayerGauge ( RoutingLayerGauge* layerGauge )
  {
    if (getLayerGauge(layerGauge->getLayer()) != NULL)
      throw Error( dupLayerGauge, getString(layerGauge->getLayer()->getName()).c_str()
                                , getString(_name).c_str() );

    _layerGauges.push_back( layerGauge );
    layerGauge->setRoutingGauge( this );

    size_t gaugeSize = _layerGauges.size();
    if (gaugeSize > 1) {
      Layer* viaLayer = _technology->getViaBetween( _layerGauges[gaugeSize-2]->getLayer()
                                                  , _layerGauges[gaugeSize-1]->getLayer()
                                                  , _layerGauges[gaugeSize-1]->getLayer()->isSymbolic() );
      if (not viaLayer) {
        cerr << Error( "RoutingGauge::addLayerGauge(): Can't find a VIA between Gauge layers %s and %s."
                     , getString(_layerGauges[gaugeSize-2]).c_str()
                     , getString(_layerGauges[gaugeSize-1]).c_str() ) << endl;
      }
      _viaLayers.push_back( viaLayer );
    }

    if ((gaugeSize > 3) and _isSuperPitched) {
      float r = ((float)layerGauge->getPitch() / (float)_layerGauges[gaugeSize-3]->getPitch());
      if (fabsf(roundf(r) - r) > 0.00001f)
        _isSuperPitched = false;
    }

    if (  (layerGauge->getType() == Constant::LayerGaugeType::BottomPowerSupply)
       or (layerGauge->getType() == Constant::LayerGaugeType::Unusable)) {
      if (_usableLayers) {
        cerr << Error( "RoutingGauge::addLayerGauge(): BottomPowerSupply & Unusable must be at the bottom {}."
                     , getString(layerGauge).c_str() ) << endl;
      } else {
        ++_firstRoutingLayer;
      }
    } else {
      if (layerGauge->getType() == Constant::LayerGaugeType::Default)
        ++_usableLayers;
    }
  }


  void  RoutingGauge::checkConnexity () const
  {
    if ( _layerGauges.empty() ) return;

    for ( size_t i=0 ; i<_viaLayers.size() ; i++ ) {
      if ( !_viaLayers[i] ) {
        cerr << Error("Gap in %s: %s and %s are not contiguous."
                     ,getString(this).c_str()
                     ,getString(_layerGauges[i  ]->getLayer()).c_str()
                     ,getString(_layerGauges[i+1]->getLayer()).c_str()) << endl;
      }
      if ( not _layerGauges[i+1]->isUsable() ) {
        cerr << Error("In %s: only first layer can be PinOnly or LocalOnly.\n"
                      "          (%s  at depth %d)"
                     ,getString(this).c_str()
                     ,getString(_layerGauges[i+1]).c_str()
                     ,i+1 ) << endl;
      }
    }
  }


  string  RoutingGauge::_getTypeName () const
  {
    return "CRL::RoutingGauge";
  }


  string  RoutingGauge::_getString () const
  {
    ostringstream  os;
    os << "<" << "RoutingGauge " << _name
       << " firstRL=" << _firstRoutingLayer
       << " usables=" << _usableLayers << ">";
    return os.str();
  }


  Record* RoutingGauge::_getRecord ( Record* record ) const
  {
    if (not record)
      record = new Record ( getString(this) );

    record->add( getSlot("_name"           , _name          ));
    record->add( getSlot("_gauges"        ,&_layerGauges    ));
    record->add( getSlot("_isSymbolic"    , _isSymbolic     ));
    record->add( getSlot("_isSuperPitched", _isSuperPitched ));
    record->add( getSlot("_usableLayers"  , _usableLayers   ));
    return ( record );
  }


  void  RoutingGauge::toJson ( JsonWriter* w ) const
  {
    w->startObject();
    jsonWrite( w, "@typename", "RoutingGauge" );
    jsonWrite( w, "_name"              , _name            );
    jsonWrite( w, "+routingLayerGauges", getCollection(getLayerGauges()) );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "JsonRoutingGauge"

  Initializer<JsonRoutingGauge>  jsonRoutingGaugeInit ( 0 );


  void  JsonRoutingGauge::initialize ()
  { JsonTypes::registerType( new JsonRoutingGauge (JsonWriter::RegisterMode) ); }


  JsonRoutingGauge::JsonRoutingGauge( unsigned long flags )
    : JsonObject(flags)
  {
    add( "_name"              , typeid(string)    );
    add( "+routingLayerGauges", typeid(JsonArray) );
  }


  string JsonRoutingGauge::getTypeName () const
  { return "RoutingGauge"; }


  JsonRoutingGauge* JsonRoutingGauge::clone ( unsigned long flags ) const
  { return new JsonRoutingGauge ( flags ); }


  void JsonRoutingGauge::toData ( JsonStack& stack )
  {
    check( stack, "JsonRoutingGauge::toData" );

    AllianceFramework* af   = get<AllianceFramework*>( stack, "_framework" );
    string             name = get<string>            ( stack, "_name"      );
    RoutingGauge*      rg   = NULL;

    if (stack.issetFlags(JsonWriter::TechnoMode)) {
      if (af) {
        if (not name.empty()) {
          rg = RoutingGauge::create( name.c_str() );
          af->addRoutingGauge( rg );
        }
      } else {
        cerr << Error( "JsonRoutingGauge::toData(): Missing \"_framework\" in stack context." ) << endl;
      }
    } else {
      rg = af->getRoutingGauge( name );
    }
  
    update( stack, rg );
  }


} // End of CRL namespace.
