// -*- mode: C++; explicit-buffer-name: "Configuration.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Configuration.cpp"                           |
// +-----------------------------------------------------------------+


#include <iostream>
#include <iomanip>
#include <vector>
#include "vlsisapd/configuration/Configuration.h"
#include "hurricane/Warning.h"
#include "hurricane/Error.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "crlcore/CellGauge.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/RoutingLayerGauge.h"
#include "crlcore/AllianceFramework.h"
#include "anabatic/Configuration.h"



namespace Anabatic {


  using  std::cout;
  using  std::cerr;
  using  std::endl;
  using  std::setprecision;
  using  std::ostringstream;
  using  std::vector;
  using  Hurricane::tab;
  using  Hurricane::Warning;
  using  Hurricane::Error;
  using  Hurricane::Technology;
  using  Hurricane::DataBase;
  using  Hurricane::RegularLayer;
  using  CRL::AllianceFramework;
  using  CRL::RoutingGauge;
  using  CRL::RoutingLayerGauge;


// -------------------------------------------------------------------
// Class  :  "Anabatic::Configuration".


  Configuration::Configuration () { }
  Configuration::~Configuration () { }


// -------------------------------------------------------------------
// Class  :  "Anabatic::ConfigurationConcrete".


  ConfigurationConcrete::ConfigurationConcrete ( const CellGauge* cg, const RoutingGauge* rg )
    : Configuration   ()
    , _cg             (NULL)
    , _rg             (NULL)
    , _extensionCaps  ()
    , _allowedDepth   (0)
    , _edgeLength     (DbU::fromLambda(Cfg::getParamInt("anabatic.edgeLength",24)->asInt()))
    , _edgeWidth      (DbU::fromLambda(Cfg::getParamInt("anabatic.edgeWidth" , 4)->asInt()))
    , _edgeCostH      (Cfg::getParamDouble("anabatic.edgeCostH",  9.0)->asDouble())
    , _edgeCostK      (Cfg::getParamDouble("anabatic.edgeCostK",-10.0)->asDouble())
  {
    if (cg == NULL) cg = AllianceFramework::get()->getCellGauge();
    if (rg == NULL) rg = AllianceFramework::get()->getRoutingGauge();
    _cg = cg->getClone();
    _rg = rg->getClone();

    if (Cfg::hasParameter("anabatic.topRoutingLayer")) {
      _setTopRoutingLayer( Cfg::getParamString("anabatic.topRoutingLayer")->asString() );
    } else
      _allowedDepth = rg->getDepth()-1;

    _gmetalh  = DataBase::getDB()->getTechnology()->getLayer("gmetalh");
    _gmetalv  = DataBase::getDB()->getTechnology()->getLayer("gmetalv");
    _gcontact = DataBase::getDB()->getTechnology()->getLayer("gcontact");

    if (_gcontact == NULL) cerr << Warning("Cannot get \"gcontact\" layer from the Technology.") << endl;
    if (_gmetalv  == NULL) cerr << Warning("Cannot get \"gmetalv\" layer from the Technology.") << endl;
    if (_gmetalh  == NULL) cerr << Warning("Cannot get \"gmetalh\" layer from the Technology.") << endl;

  //DbU::Unit sliceHeight = _cg->getSliceHeight();

    const vector<RoutingLayerGauge*>& layerGauges = rg->getLayerGauges();
    for ( size_t depth=0 ; depth < layerGauges.size() ; ++depth ) {
      const RegularLayer* regularLayer = dynamic_cast<const RegularLayer*>( layerGauges[depth]->getLayer() );
      if (regularLayer)
        _extensionCaps.push_back( regularLayer->getExtentionCap() );
      else {
        _extensionCaps.push_back( 0 );
        cerr << Warning( "Routing layer at depth %d is *not* a RegularLayer, cannot guess extension cap.\n"
                         "          (%s)"
                       , depth
                       , getString(layerGauges[depth]->getLayer()).c_str()
                       ) << endl;
      }
    }
  }


  ConfigurationConcrete::ConfigurationConcrete ( const ConfigurationConcrete& other )
    : Configuration()
    , _gmetalh      (other._gmetalh)
    , _gmetalv      (other._gmetalv)
    , _gcontact     (other._gcontact)
    , _cg           (NULL)
    , _rg           (NULL)
    , _extensionCaps(other._extensionCaps)
    , _allowedDepth (other._allowedDepth)
    , _edgeCostH    (other._edgeCostH)
    , _edgeCostK    (other._edgeCostK)
  {
    if (other._cg) _cg = other._cg->getClone();
    if (other._rg) _rg = other._rg->getClone();
  }


  ConfigurationConcrete::~ConfigurationConcrete ()
  {
    cdebug_log(145,0) << "About to delete attribute _rg (RoutingGauge)." << endl;
    _cg->destroy ();
    _rg->destroy ();
  }


  ConfigurationConcrete* ConfigurationConcrete::clone () const
  { return new ConfigurationConcrete(*this); }


  bool  ConfigurationConcrete::isGMetal ( const Layer* layer ) const
  { return (layer and ((layer == _gmetalh) or (layer == _gmetalv))); }


  bool  ConfigurationConcrete::isGContact ( const Layer* layer ) const
  { return (layer and (layer == _gcontact)); }

  const Layer* ConfigurationConcrete::getGContactLayer () const
  { return _gcontact; }

  const Layer* ConfigurationConcrete::getGHorizontalLayer () const
  { return _gmetalh; }

  const Layer* ConfigurationConcrete::getGVerticalLayer () const
  { return _gmetalv; }

  size_t  ConfigurationConcrete::getDepth () const
  { return _rg->getDepth(); }


  size_t  ConfigurationConcrete::getAllowedDepth () const
  { return _allowedDepth; }


  size_t  ConfigurationConcrete::getLayerDepth ( const Layer* layer ) const
  { return _rg->getLayerDepth(layer); }


  CellGauge* ConfigurationConcrete::getCellGauge () const
  { return _cg; }


  RoutingGauge* ConfigurationConcrete::getRoutingGauge () const
  { return _rg; }


  RoutingLayerGauge* ConfigurationConcrete::getLayerGauge ( size_t depth ) const
  { return _rg->getLayerGauge(depth); }


  const Layer* ConfigurationConcrete::getRoutingLayer ( size_t depth ) const
  { return _rg->getRoutingLayer(depth); }


  Layer* ConfigurationConcrete::getContactLayer ( size_t depth ) const
  { return _rg->getContactLayer(depth); }


  DbU::Unit  ConfigurationConcrete::getSliceHeight () const
  { return _cg->getSliceHeight(); }


  DbU::Unit  ConfigurationConcrete::getSliceStep () const
  { return _cg->getSliceStep(); }


  DbU::Unit  ConfigurationConcrete::getPitch ( const Layer* layer, Flags flags ) const
  { return getPitch( getLayerDepth(layer), flags ); }


  DbU::Unit  ConfigurationConcrete::getOffset ( const Layer* layer ) const
  { return getOffset( getLayerDepth(layer) ); }


  DbU::Unit  ConfigurationConcrete::getExtensionCap ( const Layer* layer ) const
  { return getExtensionCap( getLayerDepth(layer) ); }


  DbU::Unit  ConfigurationConcrete::getWireWidth ( const Layer* layer ) const
  { return getWireWidth( getLayerDepth(layer) ); }


  Flags  ConfigurationConcrete::getDirection ( const Layer* layer ) const
  { return getDirection( getLayerDepth(layer) ); }


  DbU::Unit  ConfigurationConcrete::getPitch ( size_t depth, Flags flags ) const
  {
    if (flags == Flags::NoFlags) return _rg->getLayerPitch(depth);

    if (flags & Flags::PitchAbove) {
      if (depth < getAllowedDepth()) 
        return _rg->getLayerPitch( depth + 1 );
      else {
        if ( (depth > 0) and (_rg->getLayerType(depth-1) != Constant::PinOnly) )
          return _rg->getLayerPitch( depth - 1 );
      }
    }

    if (flags & Flags::PitchBelow) {
      if ( (depth > 0) and (_rg->getLayerType(depth-1) != Constant::PinOnly) )
        return _rg->getLayerPitch( depth - 1 );
      else {
        if (depth < getAllowedDepth())
          return _rg->getLayerPitch( depth + 1 );
      }
    }

  // Should issue at least a warning here.
    return _rg->getLayerPitch(depth);
  }


  DbU::Unit  ConfigurationConcrete::getOffset ( size_t depth ) const
  { return _rg->getLayerOffset(depth); }


  DbU::Unit  ConfigurationConcrete::getWireWidth ( size_t depth ) const
  { return _rg->getLayerWireWidth(depth); }


  DbU::Unit  ConfigurationConcrete::getExtensionCap ( size_t depth ) const
  { return _extensionCaps[depth]; }


  Flags  ConfigurationConcrete::getDirection ( size_t depth ) const
  { return _rg->getLayerDirection(depth); }


  void  ConfigurationConcrete::setAllowedDepth ( size_t allowedDepth )
  { _allowedDepth = (allowedDepth > getDepth()) ? getDepth() : allowedDepth; }


  void  ConfigurationConcrete::_setTopRoutingLayer ( Name name )
  {
    for ( size_t depth=0 ; depth<_rg->getDepth() ; ++depth ) {
      if (_rg->getRoutingLayer(depth)->getName() == name) {
        _allowedDepth = _rg->getLayerGauge(depth)->getDepth();
        return;
      }
    }
    cerr << Error( "In Configuration::Concrete::_setTopRoutingLayer():\n"
                   "       The routing gauge <%s> has no layer named <%s>"
                 , getString(_rg->getName()).c_str()
                 , getString(name).c_str() ) << endl;
  }


  DbU::Unit  ConfigurationConcrete::getEdgeLength () const
  { return _edgeLength; }


  DbU::Unit  ConfigurationConcrete::getEdgeWidth () const
  { return _edgeWidth; }


  float  ConfigurationConcrete::getEdgeCostH () const
  { return _edgeCostH; }


  float  ConfigurationConcrete::getEdgeCostK () const
  { return _edgeCostK; }



  void  ConfigurationConcrete::print ( Cell* cell ) const
  {
    string       topLayerName = "UNKOWN";
    const Layer* topLayer     = _rg->getRoutingLayer( _allowedDepth );
    if (topLayer)
      topLayerName = getString( topLayer->getName() );

    cout << "  o  Configuration of ToolEngine<Anabatic> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asIdentifier("     - Routing Gauge"               ,getString(_rg->getName())) << endl;
    cout << Dots::asString    ("     - Top routing layer"           ,topLayerName) << endl;
  }


  string  ConfigurationConcrete::_getTypeName () const
  {
    return "ConfigurationConcrete";
  }


  string  ConfigurationConcrete::_getString () const
  {
    ostringstream  os;

    os << "<" << _getTypeName() << " " << _rg->getName() << ">";

    return os.str();
  }


  Record* ConfigurationConcrete::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot( "_rg"          ,  _rg          ) );
    record->add ( getSlot( "_gmetalh"     , _gmetalh      ) );
    record->add ( getSlot( "_gmetalv"     , _gmetalv      ) );
    record->add ( getSlot( "_gcontact"    , _gcontact     ) );
    record->add ( getSlot( "_allowedDepth", _allowedDepth ) );
    record->add ( getSlot( "_edgeCostH"   , _edgeCostH    ) );
    record->add ( getSlot( "_edgeCostK"   , _edgeCostK    ) );
                                     
    return ( record );
  }



}  // Anabatic namespace.
