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
#include "anabatic/GCell.h"



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


  Configuration::Configuration ( const CellGauge* cg, const RoutingGauge* rg )
    : _gdepthv        (ndepth)
    , _gdepthh        (ndepth)
    , _ddepthv        (ndepth)
    , _ddepthh        (ndepth)
    , _ddepthc        (ndepth)
    , _cg             (NULL)
    , _rg             (NULL)
    , _extensionCaps  ()
    , _saturateRatio  (Cfg::getParamPercentage("katabatic.saturateRatio",80.0)->asDouble())
    , _saturateRp     (Cfg::getParamInt       ("katabatic.saturateRp"   ,8   )->asInt())
    , _globalThreshold(0)
    , _allowedDepth   (0)
    , _edgeLength     (DbU::fromLambda(Cfg::getParamInt("anabatic.edgeLength",24)->asInt()))
    , _edgeWidth      (DbU::fromLambda(Cfg::getParamInt("anabatic.edgeWidth" , 4)->asInt()))
    , _edgeCostH      (Cfg::getParamDouble("anabatic.edgeCostH",  9.0)->asDouble())
    , _edgeCostK      (Cfg::getParamDouble("anabatic.edgeCostK",-10.0)->asDouble())
    , _edgeHInc       (Cfg::getParamDouble("anabatic.edgeHInc" , 1.5)->asDouble())
  {
    GCell::setDisplayMode( Cfg::getParamEnumerate("anabatic.gcell.displayMode", GCell::Boundary)->asInt() );

    if (cg == NULL) cg = AllianceFramework::get()->getCellGauge();
    if (rg == NULL) {
      string gaugeName = Cfg::getParamString("anabatic.routingGauge","sxlib")->asString();
      rg = AllianceFramework::get()->getRoutingGauge( gaugeName );
      if (rg == NULL) 
        throw Error( "Anabatic::Configuration(): No routing gauge named \"%s\"", gaugeName.c_str() );
    }
    _cg = cg->getClone();
    _rg = rg->getClone();

    _allowedDepth = rg->getDepth()-1;
    if (Cfg::hasParameter("katabatic.topRoutingLayer"))
      _setTopRoutingLayer( Cfg::getParamString("katabatic.topRoutingLayer")->asString() );

    _gmetalh  = DataBase::getDB()->getTechnology()->getLayer("gmetalh");
    _gmetalv  = DataBase::getDB()->getTechnology()->getLayer("gmetalv");
    _gcontact = DataBase::getDB()->getTechnology()->getLayer("gcontact");

    if (_gcontact == NULL) cerr << Warning("Cannot get \"gcontact\" layer from the Technology.") << endl;
    if (_gmetalv  == NULL) cerr << Warning("Cannot get \"gmetalv\" layer from the Technology.") << endl;
    if (_gmetalh  == NULL) cerr << Warning("Cannot get \"gmetalh\" layer from the Technology.") << endl;

  //DbU::Unit sliceHeight = _cg->getSliceHeight();

    _ddepthc = (_allowedDepth > 1) ? 1 : 0;

    const vector<RoutingLayerGauge*>& layerGauges = rg->getLayerGauges();
    for ( size_t depth=0 ; depth < layerGauges.size() ; ++depth ) {
      if ( (_gdepthh == ndepth)
         and  layerGauges[depth]->isHorizontal()
         and (layerGauges[depth]->getType() == Constant::LayerGaugeType::Default) ) {
        _gdepthh = depth;
        _ddepthh = depth;
      }

      if ( (_gdepthv == ndepth)
         and  layerGauges[depth]->isVertical()
         and (layerGauges[depth]->getType() == Constant::LayerGaugeType::Default) ) {
        _gdepthv = depth;
        _ddepthv = depth;
      }

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


  Configuration::Configuration ( const Configuration& other )
    : _gmetalh        (other._gmetalh)
    , _gmetalv        (other._gmetalv)
    , _gcontact       (other._gcontact)
    , _gdepthv        (other._gdepthv)
    , _gdepthh        (other._gdepthh)
    , _ddepthv        (other._ddepthv)
    , _ddepthh        (other._ddepthh)
    , _ddepthc        (other._ddepthc)
    , _cg             (NULL)
    , _rg             (NULL)
    , _extensionCaps  (other._extensionCaps)
    , _saturateRatio  (other._saturateRatio)
    , _globalThreshold(other._globalThreshold)
    , _allowedDepth   (other._allowedDepth)
    , _edgeCostH      (other._edgeCostH)
    , _edgeCostK      (other._edgeCostK)
    , _edgeHInc       (other._edgeHInc)
  {
    GCell::setDisplayMode( Cfg::getParamEnumerate("anabatic.gcell.displayMode", GCell::Boundary)->asInt() );

    if (other._cg) _cg = other._cg->getClone();
    if (other._rg) _rg = other._rg->getClone();
  }


  Configuration::~Configuration ()
  {
    cdebug_log(145,0) << "About to delete attribute _rg (RoutingGauge)." << endl;
    _cg->destroy ();
    _rg->destroy ();
  }


  Configuration* Configuration::clone () const
  { return new Configuration(*this); }

  bool  Configuration::isTwoMetals () const
  { return _rg->isTwoMetals(); }
  
  bool  Configuration::isHV () const
  { return _rg->isHV(); }
  
  bool  Configuration::isVH () const
  { return _rg->isVH(); }

  bool  Configuration::isGMetal ( const Layer* layer ) const
  { return (layer and ((layer == _gmetalh) or (layer == _gmetalv))); }


  bool  Configuration::isGContact ( const Layer* layer ) const
  { return (layer and (layer == _gcontact)); }

  const Layer* Configuration::getGContactLayer () const
  { return _gcontact; }

  const Layer* Configuration::getGHorizontalLayer () const
  { return _gmetalh; }

  const Layer* Configuration::getGVerticalLayer () const
  { return _gmetalv; }

  size_t  Configuration::getDepth () const
  { return _rg->getDepth(); }


  size_t  Configuration::getAllowedDepth () const
  { return _allowedDepth; }


  size_t  Configuration::getLayerDepth ( const Layer* layer ) const
  { return _rg->getLayerDepth(layer); }


  CellGauge* Configuration::getCellGauge () const
  { return _cg; }


  RoutingGauge* Configuration::getRoutingGauge () const
  { return _rg; }


  RoutingLayerGauge* Configuration::getLayerGauge ( size_t depth ) const
  { return _rg->getLayerGauge(depth); }


  const Layer* Configuration::getRoutingLayer ( size_t depth ) const
  { return _rg->getRoutingLayer(depth); }


  Layer* Configuration::getContactLayer ( size_t depth ) const
  { return _rg->getContactLayer(depth); }


  DbU::Unit  Configuration::getSliceHeight () const
  { return _cg->getSliceHeight(); }


  DbU::Unit  Configuration::getSliceStep () const
  { return _cg->getSliceStep(); }


  DbU::Unit  Configuration::getPitch ( const Layer* layer, Flags flags ) const
  { return getPitch( getLayerDepth(layer), flags ); }


  DbU::Unit  Configuration::getOffset ( const Layer* layer ) const
  { return getOffset( getLayerDepth(layer) ); }


  DbU::Unit  Configuration::getExtensionCap ( const Layer* layer ) const
  { return getExtensionCap( getLayerDepth(layer) ); }


  DbU::Unit  Configuration::getWireWidth ( const Layer* layer ) const
  { return getWireWidth( getLayerDepth(layer) ); }


  Flags  Configuration::getDirection ( const Layer* layer ) const
  { return getDirection( getLayerDepth(layer) ); }


  float  Configuration::getSaturateRatio () const
  { return _saturateRatio; }


  size_t  Configuration::getSaturateRp () const
  { return _saturateRp; }


  DbU::Unit  Configuration::getGlobalThreshold () const
  { return _globalThreshold; }


  DbU::Unit  Configuration::getPitch ( size_t depth, Flags flags ) const
  {
    if (flags == Flags::NoFlags) return _rg->getLayerPitch(depth);

    if (flags & Flags::AboveLayer) {
      if (depth < getAllowedDepth()) 
        return _rg->getLayerPitch( depth + 1 );
      else {
        if ( (depth > 0) and (_rg->getLayerType(depth-1) != Constant::PinOnly) )
          return _rg->getLayerPitch( depth - 1 );
      }
    }

    if (flags & Flags::BelowLayer) {
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


  DbU::Unit  Configuration::getOffset ( size_t depth ) const
  { return _rg->getLayerOffset(depth); }


  DbU::Unit  Configuration::getWireWidth ( size_t depth ) const
  { return _rg->getLayerWireWidth(depth); }


  DbU::Unit  Configuration::getExtensionCap ( size_t depth ) const
  { return _extensionCaps[depth]; }


  Flags  Configuration::getDirection ( size_t depth ) const
  { return _rg->getLayerDirection(depth); }


  void  Configuration::setAllowedDepth ( size_t allowedDepth )
  { _allowedDepth = (allowedDepth > getDepth()) ? getDepth() : allowedDepth; }


  void  Configuration::_setTopRoutingLayer ( Name name )
  {
    for ( size_t depth=0 ; depth<_rg->getDepth() ; ++depth ) {
      if (_rg->getRoutingLayer(depth)->getName() == name) {
        _allowedDepth = _rg->getLayerGauge(depth)->getDepth();
        return;
      }
    }
    cerr << Error( "In Configuration::Concrete::_setTopRoutingLayer():\n"
                   "        The routing gauge <%s> has no layer named <%s>"
                 , getString(_rg->getName()).c_str()
                 , getString(name).c_str() ) << endl;
  }


  void  Configuration::setSaturateRatio ( float ratio )
  { _saturateRatio = ratio; }


  void  Configuration::setSaturateRp ( size_t threshold )
  { _saturateRp = threshold; }


  void  Configuration::setGlobalThreshold ( DbU::Unit threshold )
  { _globalThreshold = threshold; }


  DbU::Unit  Configuration::getEdgeLength () const
  { return _edgeLength; }


  DbU::Unit  Configuration::getEdgeWidth () const
  { return _edgeWidth; }


  float  Configuration::getEdgeCostH () const
  { return _edgeCostH; }


  float  Configuration::getEdgeCostK () const
  { return _edgeCostK; }


  float  Configuration::getEdgeHInc () const
  { return _edgeHInc; }


  void  Configuration::print ( Cell* cell ) const
  {
    if (not cmess1.enabled()) return;

    string       topLayerName = "UNKOWN";
    const Layer* topLayer     = _rg->getRoutingLayer( _allowedDepth );
    if (topLayer)
      topLayerName = getString( topLayer->getName() );

    cout << "  o  Configuration of ToolEngine<Anabatic> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asIdentifier("     - Routing Gauge"               ,getString(_rg->getName())) << endl;
    cout << Dots::asString    ("     - Top routing layer"           ,topLayerName) << endl;
  }


  string  Configuration::_getTypeName () const
  {
    return "Configuration";
  }


  string  Configuration::_getString () const
  {
    ostringstream  os;

    os << "<" << _getTypeName() << " " << _rg->getName() << ">";

    return os.str();
  }


  Record* Configuration::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot( "_gdepthh"     ,  _gdepthh     ) );
    record->add ( getSlot( "_gdepthv"     ,  _gdepthv     ) );
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
