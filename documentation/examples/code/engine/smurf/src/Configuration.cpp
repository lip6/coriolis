// -*- mode: C++; explicit-buffer-name: "Configuration.cpp<smurf>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   S m u r f  -  A  D e m o   T o o l E n g i n e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Configuration.cpp"                      |
// +-----------------------------------------------------------------+


#include  <iostream>
#include  <iomanip>
#include  <vector>

#include  "vlsisapd/configuration/Configuration.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Technology.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Cell.h"
#include  "crlcore/Utilities.h"
#include  "crlcore/RoutingLayerGauge.h"
#include  "crlcore/AllianceFramework.h"
#include  "smurf/Configuration.h"



namespace Smurf {


  using  std::cout;
  using  std::cerr;
  using  std::endl;
  using  std::setprecision;
  using  std::ostringstream;
  using  std::vector;
  using  Hurricane::tab;
  using  Hurricane::inltrace;
  using  Hurricane::Warning;
  using  Hurricane::Technology;
  using  Hurricane::DataBase;
  using  CRL::AllianceFramework;
  using  CRL::RoutingGauge;
  using  CRL::RoutingLayerGauge;


// -------------------------------------------------------------------
// Class  :  "Smurf::Configuration".


  Configuration::Configuration () { }
  Configuration::~Configuration () { }


// -------------------------------------------------------------------
// Class  :  "Smurf::ConfigurationConcrete".


  ConfigurationConcrete::ConfigurationConcrete ( const RoutingGauge* rg )
    : Configuration()
    , _rg                (NULL)
  {
    if ( rg == NULL ) rg = AllianceFramework::get()->getRoutingGauge();

    _rg = rg->getClone();
  }


  ConfigurationConcrete::ConfigurationConcrete ( const ConfigurationConcrete& other )
    : Configuration()
    , _rg          (NULL)
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


  RoutingGauge* ConfigurationConcrete::getRoutingGauge () const
  { return _rg; }


  void  ConfigurationConcrete::print ( Cell* cell ) const
  {
    cout << "  o  Configuration of ToolEngine<Smurf> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asIdentifier("     - Routing Gauge"               ,getString(_rg->getName())) << endl;
  }


  string  ConfigurationConcrete::_getTypeName () const
  { return "ConfigurationConcrete"; }


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
                                     
    return ( record );
  }


}  // smurf namespace.
