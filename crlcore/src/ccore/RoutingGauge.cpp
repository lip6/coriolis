
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingGauge.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <climits>
#include  <sstream>
#include  <algorithm>

#include  "hurricane/Commons.h"
#include  "hurricane/Layer.h"
#include  "hurricane/Technology.h"
#include  "hurricane/DataBase.h"

#include  "crlcore/XmlParser.h"
#include  "crlcore/RoutingLayerGauge.h"
#include  "crlcore/RoutingGauge.h"


namespace {


  const char* dupLayerGauge =
    "RoutingGauge::AddLayerGauge() :\n\n"
    "    Attempt to re-define layer gauge %s in routing gauge %s.\n";


} // End of anonymous namespace.



namespace CRL {


  using Hurricane::DataBase;


// -------------------------------------------------------------------
// Class  :  "RoutingGauge".


  RoutingGauge::RoutingGauge ( const char* name )
    : _name(name)
    , _layerGauges()
    , _viaLayers()
    , _technology(DataBase::getDB()->getTechnology())
  { }


  RoutingGauge::RoutingGauge ( const RoutingGauge& gauge )
    : _name       (gauge._name)
    , _layerGauges()
    , _viaLayers  ()
    , _technology (gauge._technology)
  {
  // Make a deep copy of the map.
    for ( size_t i=0 ; i<gauge._layerGauges.size() ; i++ )
      addLayerGauge ( RoutingLayerGauge::create 
                      ( gauge._layerGauges[i]->getLayer()
                      , gauge._layerGauges[i]->getDirection()
                      , gauge._layerGauges[i]->getType()
                      , gauge._layerGauges[i]->getDepth()
                      , gauge._layerGauges[i]->getDensity()
                      , gauge._layerGauges[i]->getOffset()
                      , gauge._layerGauges[i]->getPitch()
                      , gauge._layerGauges[i]->getWireWidth()
                      , gauge._layerGauges[i]->getViaWidth() )
                    );
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


  RoutingLayerGauge* RoutingGauge::getLayerGauge ( const Layer* layer ) const
  {
    for ( size_t i=0 ; i < _layerGauges.size() ; i++ ) {
      if ( _layerGauges[i]->getLayer() == layer )
        return _layerGauges[i];
    }
    return NULL;
  }


  unsigned int  RoutingGauge::getLayerDirection ( const Layer* layer ) const
  {
    RoutingLayerGauge* layerGauge = getLayerGauge(layer);
    if ( !layerGauge ) return 0;

    return layerGauge->getDirection();
  }


  size_t  RoutingGauge::getLayerDepth ( const Layer* layer ) const
  {
    for ( size_t i=0 ; i < _layerGauges.size() ; i++ ) {
      if ( _layerGauges[i]->getLayer()->getMask() == layer->getMask() )
        return i;
    }
    return UINT_MAX;
  }


  RoutingLayerGauge* RoutingGauge::getLayerGauge ( size_t depth ) const
  {
    if ( depth >= _layerGauges.size() ) return NULL;
    return _layerGauges[depth];
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


  unsigned int  RoutingGauge::getLayerDirection ( size_t depth ) const
  { return getLayerGauge(depth)->getDirection(); }


  const vector<RoutingLayerGauge*>&  RoutingGauge::getLayerGauges () const
  {
    return _layerGauges;
  }


  void  RoutingGauge::addLayerGauge ( RoutingLayerGauge* layerGauge )
  {
    if ( getLayerGauge(layerGauge->getLayer()) != NULL )
      throw Error ( dupLayerGauge, getString(layerGauge->getLayer()->getName()).c_str()
                                 , getString(_name).c_str() );

    _layerGauges.push_back ( layerGauge );

    size_t gaugeSize = _layerGauges.size();
    if ( gaugeSize > 1 ) {
      Layer* viaLayer = _technology->getViaBetween(_layerGauges[gaugeSize-2]->getLayer()
                                                  ,_layerGauges[gaugeSize-1]->getLayer());
      if ( !viaLayer ) {
        cerr << Error("Can't find a VIA between Gauge layers %s and %s."
                     ,getString(_layerGauges[gaugeSize-2]).c_str()
                     ,getString(_layerGauges[gaugeSize-1]).c_str()) << endl;
      }
      _viaLayers.push_back ( viaLayer );
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
      if ( _layerGauges[i+1]->getType() == Constant::PinOnly ) {
        cerr << Error("In %s: only first layer can be PinOnly.\n"
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
    os << "<" << "RoutingGauge " << _name << ">";

    return ( os.str() );
  }


  Record* RoutingGauge::_getRecord ( Record* record ) const
  {
    if ( record == NULL )
      record = new Record ( getString(this) );

    record->add ( getSlot("_name"     , _name       ) );
    record->add ( getSlot("_gauges"   ,&_layerGauges) );
    return ( record );
  }


} // End of CRL namespace.
