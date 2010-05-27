
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Configuartion.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iostream>
#include  <iomanip>

#include  "hurricane/Technology.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Cell.h"
#include  "crlcore/Utilities.h"
#include  "crlcore/AllianceFramework.h"
#include  "katabatic/Configuration.h"



namespace Katabatic {


  using  std::cout;
  using  std::cerr;
  using  std::endl;
  using  std::setprecision;
  using  std::ostringstream;
  using  Hurricane::tab;
  using  Hurricane::inltrace;
  using  Hurricane::Technology;
  using  Hurricane::DataBase;
  using  CRL::AllianceFramework;


// -------------------------------------------------------------------
// Class  :  "Katabatic::Configuration".


  Configuration* Configuration::_default = NULL;


  Configuration* Configuration::getDefault ()
  {
    if ( _default == NULL ) {
      _default = new ConfigurationConcrete ( AllianceFramework::get()->getRoutingGauge() );
    }
    return _default;
  }


  Configuration::Configuration () { }
  Configuration::~Configuration () { }


// -------------------------------------------------------------------
// Class  :  "Katabatic::ConfigurationConcrete".


  ConfigurationConcrete::ConfigurationConcrete ( const RoutingGauge* rg )
    : Configuration()
    , _rg                (NULL)
    , _extensionCap      (DbU::lambda(1.5))
    , _saturateRatio     (0.80)
    , _globalThreshold   (29*DbU::lambda(50.0)) // Ugly: direct uses of SxLib gauge.
    , _allowedDepth      (0)
  {
    if ( rg ) {
      _rg           = rg->getClone();
      _allowedDepth = rg->getDepth();
    }

    _gmetalh  = DataBase::getDB()->getTechnology()->getLayer("gmetalh");
    _gmetalv  = DataBase::getDB()->getTechnology()->getLayer("gmetalv");
    _gcontact = DataBase::getDB()->getTechnology()->getLayer("gcontact");
  }


  ConfigurationConcrete::ConfigurationConcrete ( const ConfigurationConcrete& other )
    : Configuration()
    , _gmetalh           (other._gmetalh)
    , _gmetalv           (other._gmetalv)
    , _gcontact          (other._gcontact)
    , _rg                (NULL)
    , _extensionCap      (other._extensionCap)
    , _saturateRatio     (other._saturateRatio)
    , _globalThreshold   (other._globalThreshold)
    , _allowedDepth      (other._allowedDepth)
  {
    if ( other._rg ) _rg = other._rg->getClone();
  }


  ConfigurationConcrete::~ConfigurationConcrete ()
  {
    ltrace(89) << "About to delete attribute _rg (RoutingGauge)." << endl;
    _rg->destroy ();
  }


  ConfigurationConcrete* ConfigurationConcrete::clone () const
  { return new ConfigurationConcrete(*this); }


  bool  ConfigurationConcrete::isGMetal ( const Layer* layer ) const
  {
    if ( !layer ) return false;
    if (    ( layer != _gmetalh  )
         && ( layer != _gmetalv  )
         && ( layer != _gcontact ) )
      return false;

    return true;
  }


  size_t  ConfigurationConcrete::getDepth () const
  { return _rg->getDepth(); }


  size_t  ConfigurationConcrete::getAllowedDepth () const
  { return _allowedDepth; }


  size_t  ConfigurationConcrete::getLayerDepth ( const Layer* layer ) const
  { return _rg->getLayerDepth(layer); }


  RoutingGauge* ConfigurationConcrete::getRoutingGauge () const
  { return _rg; }


  RoutingLayerGauge* ConfigurationConcrete::getLayerGauge ( size_t depth ) const
  { return _rg->getLayerGauge(depth); }


  const Layer* ConfigurationConcrete::getRoutingLayer ( size_t depth ) const
  { return _rg->getRoutingLayer(depth); }


  Layer* ConfigurationConcrete::getContactLayer ( size_t depth ) const
  { return _rg->getContactLayer(depth); }


  DbU::Unit  ConfigurationConcrete::getExtensionCap () const
  { return _extensionCap; }


  float  ConfigurationConcrete::getSaturateRatio () const
  { return _saturateRatio; }


  DbU::Unit  ConfigurationConcrete::getGlobalThreshold () const
  { return _globalThreshold; }


  void  ConfigurationConcrete::setAllowedDepth ( size_t allowedDepth )
  { _allowedDepth = (allowedDepth > getDepth()) ? getDepth() : allowedDepth; }


  void  ConfigurationConcrete::setSaturateRatio ( float ratio )
  { _saturateRatio = ratio; }


  void  ConfigurationConcrete::setGlobalThreshold ( DbU::Unit threshold )
  { _globalThreshold = threshold; }


  void  ConfigurationConcrete::print ( Cell* cell ) const
  {
    cout << "  o  Configuration of ToolEngine<Katabatic> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asIdentifier("     - Routing Gauge"               ,getString(_rg->getName())) << endl;
    cout << Dots::asPercentage("     - GCell saturation threshold"  ,_saturateRatio) << endl;
    cout << Dots::asDouble    ("     - Long global length threshold",DbU::getLambda(_globalThreshold)) << endl;
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
    record->add ( getSlot           ( "_rg"              ,  _rg              ) );
    record->add ( getSlot           ( "_gmetalh"         , _gmetalh          ) );
    record->add ( getSlot           ( "_gmetalv"         , _gmetalv          ) );
    record->add ( getSlot           ( "_gcontact"        , _gcontact         ) );
    record->add ( getSlot           ( "_saturateRatio"   , _saturateRatio    ) );
    record->add ( DbU::getValueSlot ( "_globalThreshold" , &_globalThreshold ) );
                                     
    return ( record );
  }



}  // End of katabatic namespace.
