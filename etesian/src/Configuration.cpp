// -*- mode: C++; explicit-buffer-name: "Configuration.cpp<etesian>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
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
#include  "crlcore/CellGauge.h"
#include  "crlcore/AllianceFramework.h"
#include  "etesian/Configuration.h"
#include  "etesian/EtesianEngine.h"



namespace Etesian {


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
  using  CRL::CellGauge;


// -------------------------------------------------------------------
// Class  :  "Etesian::Configuration".


  Configuration::Configuration () { }
  Configuration::~Configuration () { }


// -------------------------------------------------------------------
// Class  :  "Etesian::ConfigurationConcrete".


  ConfigurationConcrete::ConfigurationConcrete ( const CellGauge* cg )
    : Configuration()
    , _cg          (NULL)
    , _flags       (0)
  {
    if ( cg == NULL ) cg = AllianceFramework::get()->getCellGauge();

    _cg = cg->getClone();
  }


  ConfigurationConcrete::ConfigurationConcrete ( const ConfigurationConcrete& other )
    : Configuration()
    , _cg          (NULL)
    , _flags       (other._flags)
  {
    if ( other._cg ) _cg = other._cg->getClone();
  }


  ConfigurationConcrete::~ConfigurationConcrete ()
  {
    ltrace(89) << "About to delete attribute _cg (CellGauge)." << endl;
    _cg->destroy ();
  }


  ConfigurationConcrete* ConfigurationConcrete::clone        () const { return new ConfigurationConcrete(*this); }
  CellGauge*             ConfigurationConcrete::getCellGauge () const { return _cg; }
  bool                   ConfigurationConcrete::isSlowMotion () const { return _flags & EtesianEngine::SlowMotion; }
  void                   ConfigurationConcrete::setFlags     ( unsigned int flags ) { _flags |=  flags; }
  void                   ConfigurationConcrete::unsetFlags   ( unsigned int flags ) { _flags &= ~flags; }


  void  ConfigurationConcrete::print ( Cell* cell ) const
  {
    cout << "  o  Configuration of ToolEngine<Etesian> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asIdentifier("     - Cell Gauge",getString(_cg->getName())) << endl;
  }


  string  ConfigurationConcrete::_getTypeName () const
  { return "ConfigurationConcrete"; }


  string  ConfigurationConcrete::_getString () const
  {
    ostringstream  os;

    os << "<" << _getTypeName() << " " << _cg->getName() << ">";

    return os.str();
  }


  Record* ConfigurationConcrete::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot           ( "_cg"              ,  _cg              ) );
                                     
    return ( record );
  }


}  // etesian namespace.
