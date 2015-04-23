// -*- mode: C++; explicit-buffer-name: "Configuration.cpp<etesian>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2015, All Rights Reserved
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

  Configuration::Configuration ( const CellGauge* cg )
    : _cg           ( NULL )
    , _placeEffort  ( static_cast<Effort>        (Cfg::getParamEnumerate ("etesian.effort"        , Standard   )->asInt()) )
    , _updateConf   ( static_cast<GraphicUpdate> (Cfg::getParamEnumerate ("etesian.graphics"      , LowerBound )->asInt()) )
    , _spreadingConf(                             Cfg::getParamBool      ("etesian.uniformDensity", false      )->asBool()? ForceUniform : MaxDensity )
    , _routingDriven(                             Cfg::getParamBool      ("etesian.routingDriven", false      )->asBool())
    , _spaceMargin  (                             Cfg::getParamPercentage("etesian.spaceMargin"   ,  5.0)->asDouble() )
    , _aspectRatio  (                             Cfg::getParamPercentage("etesian.aspectRatio"   ,100.0)->asDouble() )
  {
    if ( cg == NULL ) cg = AllianceFramework::get()->getCellGauge();

    _cg = cg->getClone();
  }


  Configuration::Configuration ( const Configuration& other )
    : _cg           (NULL)
    , _placeEffort  ( other._placeEffort   )
    , _updateConf   ( other._updateConf    )
    , _spreadingConf( other._spreadingConf )
    , _spaceMargin  ( other._spaceMargin   )
    , _aspectRatio  ( other._aspectRatio   )
  {
    if ( other._cg ) _cg = other._cg->getClone();
  }


  Configuration::~Configuration ()
  {
    ltrace(89) << "About to delete attribute _cg (CellGauge)." << endl;
    _cg->destroy ();
  }


  Configuration* Configuration::clone () const { return new Configuration(*this); }


  void  Configuration::print ( Cell* cell ) const
  {
    cout << "  o  Configuration of ToolEngine<Etesian> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asIdentifier("     - Cell Gauge"    ,getString(_cg->getName())) << endl;
    cout << Dots::asInt       ("     - Place Effort"  ,_placeEffort  ) << endl;
    cout << Dots::asInt       ("     - Update Conf"   ,_updateConf   ) << endl;
    cout << Dots::asInt       ("     - Spreading Conf",_spreadingConf) << endl;
    cout << Dots::asBool      ("     - Routing driven",_routingDriven) << endl;
    cout << Dots::asPercentage("     - Space Margin"  ,_spaceMargin  ) << endl;
    cout << Dots::asPercentage("     - Aspect Ratio"  ,_aspectRatio  ) << endl;
  }


  string  Configuration::_getTypeName () const
  { return "Configuration"; }


  string  Configuration::_getString () const
  {
    ostringstream  os;

    os << "<" << _getTypeName() << " " << _cg->getName() << ">";

    return os.str();
  }


  Record* Configuration::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot( "_cg"              ,       _cg            ) );
    record->add ( getSlot( "_placeEffort"     ,  (int)_placeEffort   ) );
    record->add ( getSlot( "_updateConf"      ,  (int)_updateConf    ) );
    record->add ( getSlot( "_spreadingConf"   ,  (int)_spreadingConf ) );
    record->add ( getSlot( "_spaceMargin"     ,       _spaceMargin   ) );
    record->add ( getSlot( "_aspectRatio"     ,       _aspectRatio   ) );
    return record;
  }


}  // etesian namespace.
