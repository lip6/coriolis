
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


#include  "hurricane/Technology.h"
#include  "hurricane/DataBase.h"
#include  "katabatic/Configuration.h"



namespace Katabatic {


  using  std::cerr;
  using  std::endl;
  using  std::ostringstream;
  using  Hurricane::tab;
  using  Hurricane::inltrace;
  using  Hurricane::Technology;
  using  Hurricane::DataBase;


// -------------------------------------------------------------------
// Class  :  "Katabatic::Configuration".


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
  {
    if ( rg ) _rg = rg->getClone();

    _gmetalh  = DataBase::getDB()->getTechnology()->getLayer("gmetalh");
    _gmetalv  = DataBase::getDB()->getTechnology()->getLayer("gmetalv");
    _gcontact = DataBase::getDB()->getTechnology()->getLayer("gcontact");
  }


  ConfigurationConcrete::~ConfigurationConcrete ()
  {
    ltrace(89) << "About to delete attribute _rg (RoutingGauge)." << endl;
    _rg->destroy ();
  }


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


  void  ConfigurationConcrete::setSaturateRatio ( float ratio )
  { _saturateRatio = ratio; }


  void  ConfigurationConcrete::setGlobalThreshold ( DbU::Unit threshold )
  { _globalThreshold = threshold; }


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
