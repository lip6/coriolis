// -*- mode: C++; explicit-buffer-name: "Configuration.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
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
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Pin.h"
#include "hurricane/NetExternalComponents.h"
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
  using  Hurricane::Transformation;
  using  Hurricane::Technology;
  using  Hurricane::DataBase;
  using  Hurricane::BasicLayer;
  using  Hurricane::RegularLayer;
  using  Hurricane::Segment;
  using  Hurricane::Pin;
  using  Hurricane::Plug;
  using  Hurricane::Path;
  using  Hurricane::Occurrence;
  using  Hurricane::NetExternalComponents;
  using  CRL::AllianceFramework;
  using  CRL::RoutingGauge;
  using  CRL::RoutingLayerGauge;


// -------------------------------------------------------------------
// Class  :  "Anabatic::Configuration".


  Configuration::Configuration ( const CellGauge* cg, const RoutingGauge* rg )
    : _gdepthv          (ndepth)
    , _gdepthh          (ndepth)
    , _ddepthv          (ndepth)
    , _ddepthh          (ndepth)
    , _ddepthc          (ndepth)
    , _cg               (NULL)
    , _rg               (NULL)
    , _extensionCaps    ()
    , _saturateRatio    (Cfg::getParamPercentage("anabatic.saturateRatio",80.0)->asDouble())
    , _saturateRp       (Cfg::getParamInt       ("anabatic.saturateRp"   ,8   )->asInt())
    , _globalThreshold  (0)
    , _allowedDepth     (0)
    , _edgeLength       (DbU::fromLambda(Cfg::getParamInt("anabatic.edgeLength",24)->asInt()))
    , _edgeWidth        (DbU::fromLambda(Cfg::getParamInt("anabatic.edgeWidth" , 4)->asInt()))
    , _edgeCostH        (Cfg::getParamDouble("anabatic.edgeCostH"       ,      9.0)->asDouble())
    , _edgeCostK        (Cfg::getParamDouble("anabatic.edgeCostK"       ,    -10.0)->asDouble())
    , _edgeHInc         (Cfg::getParamDouble("anabatic.edgeHInc"        ,      1.5)->asDouble())
    , _edgeHScaling     (Cfg::getParamDouble("anabatic.edgeHScaling"    ,      1.0)->asDouble())
    , _globalIterations (Cfg::getParamInt   ("anabatic.globalIterations",     10  )->asInt())
    , _diodeName        (Cfg::getParamString("etesian.diodeName"        , "dio_x0")->asString() )
    , _antennaGateMaxWL (Cfg::getParamInt   ("etesian.antennaGateMaxWL" ,      0  )->asInt())
    , _antennaDiodeMaxWL(Cfg::getParamInt   ("etesian.antennaDiodeMaxWL",      0  )->asInt())
  {
    GCell::setDisplayMode( Cfg::getParamEnumerate("anabatic.gcell.displayMode", GCell::Boundary)->asInt() );

    string gaugeName = Cfg::getParamString("anabatic.routingGauge","sxlib")->asString();
    if (cg == NULL) {
      cg = AllianceFramework::get()->getCellGauge( gaugeName );
      if (cg == NULL) 
        throw Error( "AnabaticEngine::Configuration(): Unable to find default cell gauge." );
    }

    if (rg == NULL) {
      rg = AllianceFramework::get()->getRoutingGauge( gaugeName );
      if (rg == NULL) 
        throw Error( "AnabaticEngine::Configuration(): No routing gauge named \"%s\"", gaugeName.c_str() );
    }
    _cg = cg->getClone();
    _rg = rg->getClone();

    _allowedDepth = rg->getDepth()-1;
    if (Cfg::hasParameter("anabatic.topRoutingLayer"))
      _setTopRoutingLayer( Cfg::getParamString("anabatic.topRoutingLayer")->asString() );

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
        const BasicLayer* basicLayer = dynamic_cast<const BasicLayer*>( layerGauges[depth]->getLayer() );
        if (basicLayer) {
          _extensionCaps.push_back( layerGauges[depth]->getHalfWireWidth() );
        } else {
          _extensionCaps.push_back( 0 );
          cerr << Warning( "Routing layer at depth %d is *not* a RegularLayer, cannot guess extension cap.\n"
                           "          (%s)"
                         , depth
                         , getString(layerGauges[depth]->getLayer()).c_str()
                         ) << endl;
        }
      }
    }

    if (_antennaGateMaxWL and not _antennaDiodeMaxWL) {
      _antennaDiodeMaxWL = _antennaGateMaxWL;
      cerr << Warning( "Anabatic::Configuration(): \"etesian.antennaGateMaxWL\" is defined but not \"etesian.antennaDiodeMaxWL\".\n"
                      "          Setting both to %s"
                     , DbU::getValueString(_antennaGateMaxWL).c_str()
                     ) << endl;
    }
  }


  Configuration::Configuration ( const Configuration& other )
    : _gmetalh          (other._gmetalh)
    , _gmetalv          (other._gmetalv)
    , _gcontact         (other._gcontact)
    , _gdepthv          (other._gdepthv)
    , _gdepthh          (other._gdepthh)
    , _ddepthv          (other._ddepthv)
    , _ddepthh          (other._ddepthh)
    , _ddepthc          (other._ddepthc)
    , _cg               (NULL)
    , _rg               (NULL)
    , _extensionCaps    (other._extensionCaps)
    , _saturateRatio    (other._saturateRatio)
    , _globalThreshold  (other._globalThreshold)
    , _allowedDepth     (other._allowedDepth)
    , _edgeCostH        (other._edgeCostH)
    , _edgeCostK        (other._edgeCostK)
    , _edgeHInc         (other._edgeHInc)
    , _edgeHScaling     (other._edgeHScaling)
    , _globalIterations (other._globalIterations)
    , _diodeName        (other._diodeName)
    , _antennaGateMaxWL (other._antennaGateMaxWL)
    , _antennaDiodeMaxWL(other._antennaDiodeMaxWL)
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


  DbU::Unit  Configuration::getPWireWidth ( const Layer* layer ) const
  { return getPWireWidth( getLayerDepth(layer) ); }


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


  DbU::Unit  Configuration::getPWireWidth ( size_t depth ) const
  { return _rg->getLayerPWireWidth(depth); } 

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


  float  Configuration::getEdgeHScaling () const
  { return _edgeHScaling; }


  int  Configuration::getGlobalIterations () const
  { return _globalIterations; }


  DbU::Unit  Configuration::isOnRoutingGrid ( RoutingPad* rp ) const
  {
    Box   ab     = rp->getCell()->getBoundingBox();
    Box   bb     = rp->getBoundingBox();
    Point center = rp->getCenter();

    RoutingLayerGauge* gauge = getLayerGauge( 1 );
    if (gauge->isHorizontal()) return 0;

    DbU::Unit nearestX = gauge->getTrackPosition( ab.getXMin(), ab.getXMax(), center.getX(), Constant::Nearest );
    if ( (nearestX >= bb.getXMin()) and (nearestX <= bb.getXMax()) ) return 0;

    return nearestX;
  }


  bool  Configuration::selectRpComponent ( RoutingPad* rp ) const
  {
    cdebug_log(112,1) << "selectRpComponent(): " << rp << endl;

    if (rp->isAtTopLevel()) {
      cdebug_log(112,0) << "> RP is at top level, must not change it." << endl;
      cdebug_tabw(112,-1);
      return true;
    }

#define BETTER_FOR_TSMC 0
#if BETTER_FOR_TSMC
    rp->setOnBestComponent( RoutingPad::BiggestArea );
    cdebug_tabw(112,-1);
    return true;
#else
    Box                ab             = rp->getCell()->getAbutmentBox();
    const Layer*       metal1         = getLayerGauge( 0 )->getLayer();
    RoutingLayerGauge* gauge          = getLayerGauge( 1 );
    Occurrence         occurrence     = rp->getPlugOccurrence();
    Plug*              plug           = dynamic_cast<Plug*>( occurrence.getEntity() );
    Net*               masterNet      = plug->getMasterNet();
    Path               path           = Path( occurrence.getPath(), plug->getInstance() );
    Transformation     transformation = path.getTransformation();
    Segment*           current        = dynamic_cast<Segment*>( rp->getOccurrence().getEntity() );

    if (current and (current->getLayer()->getMask() != metal1->getMask())) {
      cdebug_log(112,0) << "> using default non-metal1 segment." << endl;
      cdebug_tabw(112,-1);
      return true;
    }

    DbU::Unit  bestSpan      = 0;
    Component* bestComponent = NULL;

    cdebug_log(112,0) << "Looking into: " << masterNet->getCell() << endl;
    for ( Component* component : masterNet->getComponents() ) {
      cdebug_log(112,0) << "@ " << component << endl;
      if (not NetExternalComponents::isExternal(component)) {
        cdebug_log(112,0) << "  Not an external component, skip." << endl;
        continue;
      }

      if (dynamic_cast<Pin*>(component)) {
        cdebug_log(112,0) << "  Pins are always considered best candidates:" << component << endl;
        bestComponent = component;
        break;
      }

      Component* candidate = dynamic_cast<Segment*>(component);
      if (not candidate
         or  (candidate->getLayer()->getMask() != metal1->getMask()) )
        candidate = dynamic_cast<Pin*>(component);
      if (not candidate) continue;

      Box        bb       = transformation.getBox( candidate->getBoundingBox() );
      DbU::Unit  trackPos = 0;
      DbU::Unit  minPos   = DbU::Max;
      DbU::Unit  maxPos   = DbU::Min;
      
      if (gauge->isVertical()) {
        trackPos = gauge->getTrackPosition( ab.getXMin()
                                          , ab.getXMax()
                                          , bb.getCenter().getX()
                                          , Constant::Nearest );
        minPos = bb.getXMin();
        maxPos = bb.getXMax();

        cdebug_log(112,0) << "Vertical gauge: " << gauge << endl;
        cdebug_log(112,0) << "ab.getXMin():   " << DbU::getValueString(bb.getXMin()) << endl;
        cdebug_log(112,0) << "ab.getXMax():   " << DbU::getValueString(bb.getXMax()) << endl;
        cdebug_log(112,0) << "bb.getCenter(): " << DbU::getValueString(bb.getCenter().getX()) << endl;
      } else {
        trackPos = gauge->getTrackPosition( ab.getYMin()
                                          , ab.getYMax()
                                          , bb.getCenter().getY()
                                          , Constant::Nearest );
        minPos = bb.getYMin();
        maxPos = bb.getYMax();

        cdebug_log(112,0) << "Horizontal gauge: " << gauge << endl;
        cdebug_log(112,0) << "ab.getYMin():   " << DbU::getValueString(bb.getYMin()) << endl;
        cdebug_log(112,0) << "ab.getYMax():   " << DbU::getValueString(bb.getYMax()) << endl;
        cdebug_log(112,0) << "bb.getCenter(): " << DbU::getValueString(bb.getCenter().getY()) << endl;
      }

      cdebug_log(112,0) << "| " << occurrence.getPath() << endl;
      cdebug_log(112,0) << "| " << transformation << endl;
      cdebug_log(112,0) << "| " << bb << " of:" << candidate << endl;
      cdebug_log(112,0) << "| Nearest Pos: " << DbU::getValueString(trackPos) << endl;
        
      if ( (trackPos >= minPos) and (trackPos <= maxPos) ) {
        if (not bestComponent or (bestSpan < maxPos-minPos)) {
          bestComponent = component;
          bestSpan      = maxPos - minPos;
        }
      }
    }

    if (bestComponent) {
      rp->setExternalComponent( bestComponent );
      cdebug_log(112,0) << "Using best candidate:" << bestComponent << endl;
      cdebug_tabw(112,-1);
      return true;
    }

    cdebug_tabw(112,-1);
    return false;
#endif
  }
  

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
    cout << Dots::asUInt      ("     - Maximum GR iterations"       ,_globalIterations) << endl;
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
    record->add( getSlot( "_gdepthh"         , _gdepthh          ) );
    record->add( getSlot( "_gdepthv"         , _gdepthv          ) );
    record->add( getSlot( "_rg"              , _rg               ) );
    record->add( getSlot( "_gmetalh"         , _gmetalh          ) );
    record->add( getSlot( "_gmetalv"         , _gmetalv          ) );
    record->add( getSlot( "_gcontact"        , _gcontact         ) );
    record->add( getSlot( "_allowedDepth"    , _allowedDepth     ) );
    record->add( getSlot( "_edgeCostH"       , _edgeCostH        ) );
    record->add( getSlot( "_edgeCostK"       , _edgeCostK        ) );
    record->add( getSlot( "_edgeHInc"        , _edgeHInc         ) );
    record->add( getSlot( "_edgeHScaling"    , _edgeHScaling     ) );
    record->add( getSlot( "_globalIterations", _globalIterations ) );
    record->add( DbU::getValueSlot( "_antennaGateMaxWL" , &_antennaGateMaxWL  ) );
    record->add( DbU::getValueSlot( "_antennaDiodeMaxWL", &_antennaDiodeMaxWL ) );
                                     
    return record;
  }


}  // Anabatic namespace.
