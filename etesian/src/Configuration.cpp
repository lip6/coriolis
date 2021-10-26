// -*- mode: C++; explicit-buffer-name: "Configuration.cpp<etesian>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2018, All Rights Reserved
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


#include <iostream>
#include <iomanip>
#include <vector>
#include "hurricane/configuration/Configuration.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "crlcore/CellGauge.h"
#include "crlcore/AllianceFramework.h"
#include "etesian/Configuration.h"
#include "etesian/EtesianEngine.h"


namespace Etesian {

  using  std::cout;
  using  std::cerr;
  using  std::endl;
  using  std::setprecision;
  using  std::ostringstream;
  using  std::vector;
  using  Hurricane::tab;
  using  Hurricane::Error;
  using  Hurricane::Warning;
  using  Hurricane::Technology;
  using  Hurricane::DataBase;
  using  CRL::AllianceFramework;
  using  CRL::CellGauge;


// -------------------------------------------------------------------
// Class  :  "Etesian::Configuration".

  Configuration::Configuration ( const RoutingGauge* rg, const CellGauge* cg )
    : _rg               ( NULL )
    , _cg               ( NULL )
    , _placeEffort      ( static_cast<Effort>
                          (Cfg::getParamEnumerate ("etesian.effort"         , Standard   )->asInt()) )
    , _updateConf       ( static_cast<GraphicUpdate>                        
                          (Cfg::getParamEnumerate ("etesian.graphics"       , LowerBound )->asInt()) )
    , _spreadingConf    (  Cfg::getParamBool      ("etesian.uniformDensity" , false      )->asBool()? ForceUniform : MaxDensity )
    , _routingDriven    (  Cfg::getParamBool      ("etesian.routingDriven"  , false      )->asBool())
    , _spaceMargin      (  Cfg::getParamPercentage("etesian.spaceMargin"    ,  5.0)->asDouble() )
    , _aspectRatio      (  Cfg::getParamPercentage("etesian.aspectRatio"    ,100.0)->asDouble() )
    , _antennaInsertThreshold
                        (  Cfg::getParamDouble    ("etesian.antennaInsertThreshold",       50.0)->asDouble() )
    , _tieName          (  Cfg::getParamString    ("etesian.tieName"        ,"tie_x0"          )->asString() )
    , _feedNames        (  Cfg::getParamString    ("etesian.feedNames"      ,"tie_x0,rowend_x0")->asString() )
    , _diodeName        (  Cfg::getParamString    ("etesian.diodeName"      ,"dio_x0"          )->asString() )
    , _spareBufferName  (  Cfg::getParamString    ("spares.buffer"          ,"buf_x8"          )->asString() )
    , _bloat            (  Cfg::getParamString    ("etesian.bloat"          ,"disabled"        )->asString() )
    , _latchUpDistance  (  Cfg::getParamInt       ("etesian.latchUpDistance",0                 )->asInt() )
    , _antennaGateMaxWL (  Cfg::getParamInt       ("etesian.antennaGateMaxWL"   ,0                 )->asInt() )
    , _antennaDiodeMaxWL(  Cfg::getParamInt       ("etesian.antennaDiodeMaxWL"   ,0                 )->asInt() )
  {
    string gaugeName = Cfg::getParamString("anabatic.routingGauge","sxlib")->asString();
    if (cg == NULL) {
      cg = AllianceFramework::get()->getCellGauge( gaugeName );
      if (cg == NULL) 
        throw Error( "AnabaticEngine::Configuration(): Unable to find default cell gauge \"%s\"."
                   , gaugeName.c_str() );
    }

    if (rg == NULL) {
      rg = AllianceFramework::get()->getRoutingGauge( gaugeName );
      if (rg == NULL) 
        throw Error( "AnabaticEngine::Configuration(): No routing gauge named \"%s\"", gaugeName.c_str() );
    }

    _rg = rg->getClone();
    _cg = cg->getClone();

    if (_antennaGateMaxWL and not _antennaDiodeMaxWL) {
      _antennaDiodeMaxWL = _antennaGateMaxWL;
      cerr << Warning( "Etesian::Configuration(): \"etesian.antennaGateMaxWL\" is defined but not \"etesian.antennaDiodeMaxWL\".\n"
                      "          Setting both to %s"
                     , DbU::getValueString(_antennaGateMaxWL).c_str()
                     ) << endl;
    }
  }


  Configuration::Configuration ( const Configuration& other )
    : _rg               (NULL)
    , _cg               (NULL)
    , _placeEffort      ( other._placeEffort     )
    , _updateConf       ( other._updateConf      )
    , _spreadingConf    ( other._spreadingConf   )
    , _spaceMargin      ( other._spaceMargin     )
    , _aspectRatio      ( other._aspectRatio     )
    , _antennaInsertThreshold( other._antennaInsertThreshold )
    , _tieName          ( other._tieName         )
    , _feedNames        ( other._feedNames       )
    , _diodeName        ( other._diodeName       )
    , _spareBufferName  ( other._spareBufferName )
    , _bloat            ( other._bloat           )
    , _latchUpDistance  ( other._latchUpDistance )
    , _antennaGateMaxWL ( other._antennaGateMaxWL    )
    , _antennaDiodeMaxWL( other._antennaDiodeMaxWL    )
  {
    if (other._rg) _rg = other._rg->getClone();
    if (other._cg) _cg = other._cg->getClone();
  }


  Configuration::~Configuration ()
  {
    cdebug_log(129,0) << "About to delete attribute _cg (CellGauge)." << endl;
    _cg->destroy ();
  }


  Configuration* Configuration::clone () const { return new Configuration(*this); }


  void  Configuration::print ( Cell* cell ) const
  {
    cmess1 << "  o  Configuration of ToolEngine<Etesian> for Cell <" << cell->getName() << ">" << endl;
    cmess1 << Dots::asIdentifier("     - Cell Gauge"       ,getString(_cg->getName())) << endl;
    cmess1 << Dots::asInt       ("     - Place Effort"     ,_placeEffort             ) << endl;
    cmess1 << Dots::asInt       ("     - Update Conf"      ,_updateConf              ) << endl;
    cmess1 << Dots::asInt       ("     - Spreading Conf"   ,_spreadingConf           ) << endl;
    cmess1 << Dots::asBool      ("     - Routing driven"   ,_routingDriven           ) << endl;
    cmess1 << Dots::asPercentage("     - Space Margin"     ,_spaceMargin             ) << endl;
    cmess1 << Dots::asPercentage("     - Aspect Ratio"     ,_aspectRatio             ) << endl;
    cmess1 << Dots::asString    ("     - Bloat model"      ,_bloat                   ) << endl;
    cmess1 << Dots::asPercentage("     - Antenna Insert"   ,_antennaInsertThreshold  ) << endl;
    cmess1 << Dots::asString    ("     - Antenna gate Max. WL" ,DbU::getValueString(_antennaGateMaxWL )) << endl;
    cmess1 << Dots::asString    ("     - Antenna diode Max. WL",DbU::getValueString(_antennaDiodeMaxWL)) << endl;
    cmess1 << Dots::asString    ("     - Latch up Distance",DbU::getValueString(_latchUpDistance)) << endl;
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
    record->add ( getSlot( "_rg"                    ,       _rg              ) );
    record->add ( getSlot( "_cg"                    ,       _cg              ) );
    record->add ( getSlot( "_placeEffort"           ,  (int)_placeEffort     ) );
    record->add ( getSlot( "_updateConf"            ,  (int)_updateConf      ) );
    record->add ( getSlot( "_spreadingConf"         ,  (int)_spreadingConf   ) );
    record->add ( getSlot( "_spaceMargin"           ,       _spaceMargin     ) );
    record->add ( getSlot( "_aspectRatio"           ,       _aspectRatio     ) );
    record->add ( getSlot( "_antennaInsertThreshold",       _antennaInsertThreshold   ) );
    record->add ( getSlot( "_tieName"             ,         _tieName         ) );
    record->add ( getSlot( "_feedNames"             ,       _feedNames       ) );
    record->add ( getSlot( "_diodeName"             ,       _diodeName       ) );
    record->add ( getSlot( "_spareBufferName"       ,       _spareBufferName ) );
    record->add ( getSlot( "_bloat"                 ,       _bloat           ) );
    record->add ( DbU::getValueSlot( "_latchUpDistance"  , &_latchUpDistance   ) );
    record->add ( DbU::getValueSlot( "_antennaGateMaxWL" , &_antennaGateMaxWL  ) );
    record->add ( DbU::getValueSlot( "_antennaDiodeMaxWL", &_antennaDiodeMaxWL ) );
    return record;
  }


}  // etesian namespace.
