
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
// |     N i m b u s  -  Partionnement Infrastructure                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Configuration.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iostream>
#include  <iomanip>

#include  "vlsisapd/configuration/Configuration.h"
#include  "hurricane/Cell.h"
#include  "crlcore/Utilities.h"
#include  "crlcore/AllianceFramework.h"
#include  "nimbus/Configuration.h"



namespace Nimbus {


  using  std::cout;
  using  std::cerr;
  using  std::endl;
  using  std::setprecision;
  using  std::ostringstream;
  using  std::string;
  using  Cfg::Parameter;
  using  Hurricane::tab;
  using  Hurricane::inltrace;
  using  CRL::AllianceFramework;


// -------------------------------------------------------------------
// Class  :  "Nimbus::Configuration".


  Configuration::Configuration ( CellGauge* cg, RoutingGauge* rg )
    : _cellGauge    (NULL)
    , _routingGauge (NULL)
    , _pinsPlacement(Cfg::getParamBool      ("nimbus.pinsPlacement",false)->asBool  ())
    , _aspectRatio  (Cfg::getParamPercentage("nimbus.aspectRatio"  ,100.0)->asDouble())
    , _margin       (Cfg::getParamPercentage("nimbus.spaceMargin"  , 40.0)->asDouble())
    , _workZone     ()
  {
    _cellGauge    = (cg != NULL) ? cg->getClone() : AllianceFramework::get()->getCellGauge();
    _routingGauge = (rg != NULL) ? rg->getClone() : AllianceFramework::get()->getRoutingGauge();
  }


  Configuration::Configuration ( const Configuration& other )
    : _cellGauge    (other._cellGauge->getClone())
    , _routingGauge (other._routingGauge->getClone())
    , _pinsPlacement(other._pinsPlacement)
    , _aspectRatio  (other._aspectRatio)
    , _margin       (other._margin)
    , _workZone     (other._workZone)
  { }


  Configuration::~Configuration ()
  { }


  void  Configuration::print ( Cell* cell ) const
  {
    cout << "  o  Configuration of ToolEngine<Nimbus> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asIdentifier("     - Cell Gauge"    ,getString(_cellGauge->getName())) << endl;
    cout << Dots::asIdentifier("     - Routing Gauge" ,getString(_routingGauge->getName())) << endl;
    cout << Dots::asBool      ("     - Pins Placement",_pinsPlacement) << endl;
    cout << Dots::asPercentage("     - Margin Ratio"  ,_margin) << endl;
  }


  string  Configuration::_getTypeName () const
  {
    return "Nimbus::Configuration";
  }


  string  Configuration::_getString () const
  {
    ostringstream  os;

    os << "<" << _getTypeName() << ">";

    return os.str();
  }


  Record* Configuration::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot( "_pinsPlacement", _pinsPlacement) );
    record->add ( getSlot( "_margin"       , _margin       ) );
    record->add ( getSlot( "_workZone"     , _workZone     ) );
                                     
    return ( record );
  }



}  // End of Nimbus namespace.
