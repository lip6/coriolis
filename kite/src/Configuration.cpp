
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Configuartion.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "kite/Configuration.h"
#include  "kite/KiteEngine.h"



namespace Kite {


  using  std::cerr;
  using  std::endl;
  using  std::ostringstream;
  using  Hurricane::tab;
  using  Hurricane::inltrace;
  using  Hurricane::Error;
  using  Hurricane::Technology;


// -------------------------------------------------------------------
// Class  :  "Kite::Configuration".


  float  Configuration::_defaultEdgeCapacity = 0.80;


  void  Configuration::setDefaultEdgeCapacity ( float percent )
  {
    if ( percent > 1.0 )
      throw Error("Configuration::setDefaultEdgeCapacity(): edge capacity ratio greater than 1.0 (%.1f)."
                 ,percent);

    _defaultEdgeCapacity = percent;
  }


  Configuration::Configuration ( Katabatic::Configuration* base )
    : Katabatic::Configuration()
    , _base               (base)
    , _postEventCb        ()
    , _edgeCapacityPercent(_defaultEdgeCapacity)
    , _expandStep         (0.30)
    , _ripupLimits        ()
    , _ripupCost          (3)
  {
    _ripupLimits[BorderRipupLimit]     = 26;
    _ripupLimits[StrapRipupLimit]      = 16;
    _ripupLimits[LocalRipupLimit]      =  7;
    _ripupLimits[GlobalRipupLimit]     =  7;
    _ripupLimits[LongGlobalRipupLimit] =  7;
  }


  Configuration::~Configuration ()
  { }


  bool  Configuration::isGMetal ( const Layer* layer ) const
  { return _base->isGMetal(layer); }


  size_t  Configuration::getDepth () const
  { return _base->getDepth(); }


  size_t  Configuration::getLayerDepth ( const Layer* layer ) const
  { return _base->getLayerDepth(layer); }


  RoutingGauge* Configuration::getRoutingGauge () const
  { return _base->getRoutingGauge(); }


  RoutingLayerGauge* Configuration::getLayerGauge ( size_t depth ) const
  { return _base->getLayerGauge(depth); }


  const Layer* Configuration::getRoutingLayer ( size_t depth ) const
  { return _base->getRoutingLayer(depth); }


  Layer* Configuration::getContactLayer ( size_t depth ) const
  { return _base->getContactLayer(depth); }


  DbU::Unit  Configuration::getExtensionCap () const
  { return _base->getExtensionCap(); }


  float  Configuration::getSaturateRatio () const
  { return _base->getSaturateRatio(); }


  DbU::Unit  Configuration::getGlobalThreshold () const
  { return _base->getGlobalThreshold(); }


  void  Configuration::setSaturateRatio ( float ratio )
  { _base->setSaturateRatio(ratio); }


  void  Configuration::setGlobalThreshold ( DbU::Unit threshold )
  { _base->setGlobalThreshold(threshold); }


  void  Configuration::setRipupLimit ( unsigned int limit, unsigned int type )
  {
    if ( type >= RipupLimitsTableSize ) {
      cerr << Error("setRipupLimit(): Bad ripup limit index: %ud (> %ud)."
                   ,type,RipupLimitsTableSize) << endl;
      return;
    }

    _ripupLimits [ type ] = limit;
  }


  unsigned int  Configuration::getRipupLimit ( unsigned int type ) const
  {
    if ( type >= RipupLimitsTableSize ) {
      cerr << Error("getRipupLimit(): Bad ripup limit index: %ud (> %ud)."
                   ,type,RipupLimitsTableSize) << endl;
      return 0;
    }
    return _ripupLimits[type];
  }


  string  Configuration::_getTypeName () const
  {
    return "Configuration";
  }


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
                                     
    return record;
  }



}  // End of Kite namespace.
