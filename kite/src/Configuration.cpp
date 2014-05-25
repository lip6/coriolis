// -*- mode: C++; explicit-buffer-name: "Configuration.cpp<kite>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Configuration.cpp"                      |
// +-----------------------------------------------------------------+


#include <string>
#include "vlsisapd/configuration/Configuration.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "kite/Configuration.h"
#include "kite/KiteEngine.h"


namespace Kite {

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::Error;
  using Hurricane::Technology;


// -------------------------------------------------------------------
// Class  :  "Kite::Configuration".


  Configuration::Configuration ( Katabatic::Configuration* base )
    : Katabatic::Configuration()
    , _base                (base)
    , _postEventCb         ()
    , _hEdgeCapacityPercent(Cfg::getParamPercentage("kite.hEdgeCapacity", 80.0)->asDouble())
    , _vEdgeCapacityPercent(Cfg::getParamPercentage("kite.vEdgeCapacity", 80.0)->asDouble())
    , _ripupLimits         ()
    , _ripupCost           (Cfg::getParamInt("kite.ripupCost"   ,      3)->asInt())
    , _eventsLimit         (Cfg::getParamInt("kite.eventsLimit" ,4000000)->asInt())
  {
    _ripupLimits[StrapRipupLimit]      = Cfg::getParamInt("kite.strapRipupLimit"      ,16)->asInt();
    _ripupLimits[LocalRipupLimit]      = Cfg::getParamInt("kite.localRipupLimit"      , 7)->asInt();
    _ripupLimits[GlobalRipupLimit]     = Cfg::getParamInt("kite.globalRipupLimit"     , 5)->asInt();
    _ripupLimits[LongGlobalRipupLimit] = Cfg::getParamInt("kite.longGlobalRipupLimit" , 5)->asInt();

    // for ( size_t i=0 ; i<MaxMetalDepth ; ++i ) {
    //   ostringstream paramName;
    //   paramName << "kite.metal" << (i+1) << "MinBreak";

    //   int threshold = 29*50;
    //   switch ( i ) {
    //     case 0:
    //     case 1:
    //       threshold = 2*50;
    //       break;
    //     default:
    //       threshold = 30*50;
    //       break;
    //   }

    //   Cfg::getParamDouble(paramName.str())->setDouble(threshold);
    //   _globalMinBreaks[i] = DbU::lambda (Cfg::getParamDouble(paramName.str())->asDouble());
    // }
  }


  Configuration::Configuration ( const Configuration& other, Katabatic::Configuration* base )
    : Katabatic::Configuration()
    , _base                (base)
    , _postEventCb         (other._postEventCb)
    , _hEdgeCapacityPercent(other._hEdgeCapacityPercent)
    , _vEdgeCapacityPercent(other._vEdgeCapacityPercent)
    , _ripupLimits         ()
    , _ripupCost           (other._ripupCost)
    , _eventsLimit         (other._eventsLimit)
  {
    if ( _base == NULL ) _base = other._base->clone();

    _ripupLimits[StrapRipupLimit]      = other._ripupLimits[StrapRipupLimit];
    _ripupLimits[LocalRipupLimit]      = other._ripupLimits[LocalRipupLimit];
    _ripupLimits[GlobalRipupLimit]     = other._ripupLimits[GlobalRipupLimit];
    _ripupLimits[LongGlobalRipupLimit] = other._ripupLimits[LongGlobalRipupLimit];
  }


  Configuration::~Configuration ()
  { }


  Configuration* Configuration::clone () const
  { return new Configuration(*this); }


  Configuration* Configuration::clone ( KiteEngine* kite ) const
  { return new Configuration(*this,kite->base()->getKatabaticConfiguration()); }


  bool  Configuration::isGMetal ( const Layer* layer ) const
  { return _base->isGMetal(layer); }


  bool  Configuration::isGContact ( const Layer* layer ) const
  { return _base->isGContact(layer); }


  size_t  Configuration::getDepth () const
  { return _base->getDepth(); }


  size_t  Configuration::getAllowedDepth () const
  { return _base->getAllowedDepth(); }


  size_t  Configuration::getLayerDepth ( const Layer* layer ) const
  { return _base->getLayerDepth(layer); }


  CellGauge* Configuration::getCellGauge () const
  { return _base->getCellGauge(); }


  RoutingGauge* Configuration::getRoutingGauge () const
  { return _base->getRoutingGauge(); }


  DbU::Unit  Configuration::getSliceHeight () const
  { return _base->getSliceHeight(); }


  DbU::Unit  Configuration::getSliceStep () const
  { return _base->getSliceStep(); }


  RoutingLayerGauge* Configuration::getLayerGauge ( size_t depth ) const
  { return _base->getLayerGauge(depth); }


  const Layer* Configuration::getRoutingLayer ( size_t depth ) const
  { return _base->getRoutingLayer(depth); }


  Layer* Configuration::getContactLayer ( size_t depth ) const
  { return _base->getContactLayer(depth); }


  DbU::Unit  Configuration::getPitch ( size_t depth, unsigned int flags ) const
  { return _base->getPitch(depth,flags); }


  DbU::Unit  Configuration::getOffset ( size_t depth ) const
  { return _base->getOffset(depth); }


  DbU::Unit  Configuration::getWireWidth ( size_t depth ) const
  { return _base->getWireWidth(depth); }


  DbU::Unit  Configuration::getExtensionCap ( size_t depth ) const
  { return _base->getExtensionCap(depth); }


  unsigned int  Configuration::getDirection ( size_t depth ) const
  { return _base->getDirection(depth); }


  DbU::Unit  Configuration::getPitch ( const Layer* layer, unsigned int flags ) const
  { return _base->getPitch(layer,flags); }


  DbU::Unit  Configuration::getOffset ( const Layer* layer ) const
  { return _base->getOffset(layer); }


  DbU::Unit  Configuration::getWireWidth ( const Layer* layer ) const
  { return _base->getWireWidth(layer); }


  unsigned int  Configuration::getDirection ( const Layer* layer ) const
  { return _base->getDirection(layer); }


  DbU::Unit  Configuration::getExtensionCap ( const Layer* layer ) const
  { return _base->getExtensionCap(layer); }


  float  Configuration::getSaturateRatio () const
  { return _base->getSaturateRatio(); }


  size_t  Configuration::getSaturateRp () const
  { return _base->getSaturateRp(); }


  DbU::Unit  Configuration::getGlobalThreshold () const
  { return _base->getGlobalThreshold(); }

  size_t  Configuration::getHEdgeCapacity () const
  { return _base->getHEdgeCapacity(); }


  size_t  Configuration::getVEdgeCapacity () const
  { return _base->getVEdgeCapacity(); }


  void  Configuration::setAllowedDepth ( size_t allowedDepth )
  { _base->setAllowedDepth(allowedDepth); }


  void  Configuration::setSaturateRatio ( float ratio )
  { _base->setSaturateRatio(ratio); }


  void  Configuration::setSaturateRp ( size_t threshold )
  { _base->setSaturateRp(threshold); }


  void  Configuration::setGlobalThreshold ( DbU::Unit threshold )
  { _base->setGlobalThreshold(threshold); }


  void  Configuration::setRipupLimit ( unsigned int type, unsigned int limit )
  {
    if ( type >= RipupLimitsTableSize ) {
      cerr << Error("setRipupLimit(): Bad ripup limit index: %ud (> %ud)."
                   ,type,RipupLimitsTableSize) << endl;
      return;
    }

    _ripupLimits [ type ] = limit;
  }


  void  Configuration::setHEdgeCapacityPercent ( float percent )
  {
    if (percent > 1.0)
      throw Error( "Configuration::setHEdgeCapacityPercent(): edge capacity ratio greater than 1.0 (%.1f)."
                 , percent );

    _hEdgeCapacityPercent = percent;
  }


  void  Configuration::setVEdgeCapacityPercent ( float percent )
  {
    if (percent > 1.0)
      throw Error( "Configuration::setVEdgeCapacityPercent(): edge capacity ratio greater than 1.0 (%.1f)."
                 , percent );

    _vEdgeCapacityPercent = percent;
  }


  unsigned int  Configuration::getRipupLimit ( unsigned int type ) const
  {
    if ( type >= RipupLimitsTableSize ) {
      cerr << Error("getRipupLimit(): Bad ripup limit index: %u (> %u)."
                   ,type,RipupLimitsTableSize) << endl;
      return 0;
    }
    return _ripupLimits[type];
  }


  void  Configuration::print ( Cell* cell ) const
  {
    cout << "  o  Configuration of ToolEngine<Kite> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asPercentage("     - Global router H edge capacity"      ,_hEdgeCapacityPercent) << endl;
    cout << Dots::asPercentage("     - Global router V edge capacity"      ,_vEdgeCapacityPercent) << endl;
    cout << Dots::asULong     ("     - Events limit (iterations)"          ,_eventsLimit) << endl;
    cout << Dots::asUInt      ("     - Ripup limit, straps"                ,_ripupLimits[StrapRipupLimit]) << endl;
    cout << Dots::asUInt      ("     - Ripup limit, locals"                ,_ripupLimits[LocalRipupLimit]) << endl;
    cout << Dots::asUInt      ("     - Ripup limit, globals"               ,_ripupLimits[GlobalRipupLimit]) << endl;
    cout << Dots::asUInt      ("     - Ripup limit, long globals"          ,_ripupLimits[LongGlobalRipupLimit]) << endl;

    _base->print ( cell );
  }


  string  Configuration::_getTypeName () const
  { return "Configuration"; }


  string  Configuration::_getString () const
  {
    ostringstream  os;

    os << "<" << _getTypeName() << " " << getRoutingGauge()->getName() << ">";

    return os.str();
  }


  Record* Configuration::_getRecord () const
  {
    Record* record = _base->_getRecord();
  //record->add ( getSlot ( "_rg"       ,  _rg      ) );
    if ( record ) {
      record->add ( getSlot("_hEdgeCapacityPercent" ,_hEdgeCapacityPercent ) );
      record->add ( getSlot("_vEdgeCapacityPercent" ,_vEdgeCapacityPercent ) );
      record->add ( getSlot("_ripupCost"            ,_ripupCost            ) );
      record->add ( getSlot("_eventsLimit"          ,_eventsLimit          ) );

      record->add ( getSlot("_ripupLimits[StrapRipupLimit]"     ,_ripupLimits[StrapRipupLimit]     ) );
      record->add ( getSlot("_ripupLimits[LocalRipupLimit]"     ,_ripupLimits[LocalRipupLimit]     ) );
      record->add ( getSlot("_ripupLimits[GlobalRipupLimit]"    ,_ripupLimits[GlobalRipupLimit]    ) );
      record->add ( getSlot("_ripupLimits[LongGlobalRipupLimit]",_ripupLimits[LongGlobalRipupLimit]) );

      // for ( size_t i=0 ; i<MaxMetalDepth ; ++i ) {
      //   ostringstream paramName;
      //   paramName << "metal" << (i+1) << "MinBreak";

      //   record->add ( DbU::getValueSlot(paramName.str(),&_globalMinBreaks[i]) );
      // }
    }
                                     
    return record;
  }



}  // Kite namespace.
