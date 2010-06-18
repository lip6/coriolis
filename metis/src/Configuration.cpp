
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
// |      M e t i s  -  h M e t i s   W r a p p e r                  |
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
#include  "metis/Configuration.h"


namespace Metis {


  using  std::cout;
  using  std::cerr;
  using  std::endl;
  using  std::setprecision;
  using  std::ostringstream;
  using  std::string;
  using  Cfg::Parameter;
  using  Hurricane::tab;
  using  Hurricane::inltrace;
  using  Hurricane::Cell;


// -------------------------------------------------------------------
// Class  :  "Metis::Configuration".


  Configuration::Configuration ()
    : _refreshCb                     ()
    , _partOrKWayHMetis              (Cfg::getParamBool("metis.partOrKWayHMetis"              ,true)->asBool ())
    , _numberOfInstancesStopCriterion(Cfg::getParamInt ("metis.numberOfInstancesStopCriterion",45  )->asInt  ())
    , _globalConnectionsWeightRatio  (Cfg::getParamInt ("metis.globalConnectionsWeightRatio"  ,1   )->asInt  ())
    , _ubFactor                      (Cfg::getParamInt ("metis.ubFactor"                      ,0   )->asInt  ())
    , _hmetisOptions                 ()
  {
    _hmetisOptions[CustomOptions   ] = Cfg::getParamBool     ("metis.tuneHMetisParameters"   ,true )->asBool() ? 1 : 0;
    _hmetisOptions[HMetisNRuns     ] = Cfg::getParamInt      ("metis.numberOfTriedBisections",10   )->asInt();
    _hmetisOptions[HMetisCType     ] = Cfg::getParamEnumerate("metis.CType"  ,CTypeHFC          )->asInt();
    _hmetisOptions[HMetisRType     ] = Cfg::getParamEnumerate("metis.RType"  ,RTypeFM           )->asInt();
    _hmetisOptions[HMetisVCycle    ] = Cfg::getParamEnumerate("metis.VCycle" ,VCycleDisable     )->asInt();
    _hmetisOptions[HMetisReconst   ] = Cfg::getParamEnumerate("metis.Reconst",ReconstRemoveCutHE)->asInt();
    _hmetisOptions[HMetisPreAssign ] =  1;
    _hmetisOptions[HMetisRandom    ] = -1;
    _hmetisOptions[HMetisDebugLevel] = Cfg::getParamEnumerate("metis.debug"  ,DebugDisable      )->asInt();
  }


  Configuration::Configuration ( const Configuration& other )
    : _refreshCb                     (other._refreshCb)
    , _partOrKWayHMetis              (other._partOrKWayHMetis)
    , _numberOfInstancesStopCriterion(other._numberOfInstancesStopCriterion)
    , _globalConnectionsWeightRatio  (other._globalConnectionsWeightRatio)
    , _ubFactor                      (other._ubFactor)
    , _hmetisOptions                 ()
  {
    for ( unsigned int option=0 ; option<HMetisOptionsSize ; ++option )
      _hmetisOptions[option] = other._hmetisOptions[option];
  }


  Configuration::~Configuration ()
  { }


  void  Configuration::print ( Cell* cell ) const
  {
    cout << "  o  Configuration of ToolEngine<Metis> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asBool("     - Part / KWay hMETIS"                ,_partOrKWayHMetis) << endl;
    cout << Dots::asUInt("     - Instances stop criterion"          ,_numberOfInstancesStopCriterion) << endl;
    cout << Dots::asInt ("     - Global connections weight ratio"   ,_globalConnectionsWeightRatio) << endl;
    cout << Dots::asInt ("     - UB factor"                         ,_ubFactor) << endl;
    cout << Dots::asInt ("     - hMETIS Custom options"             ,_hmetisOptions[CustomOptions]) << endl;
    cout << Dots::asInt ("     - hMETIS NRuns"                      ,_hmetisOptions[HMetisNRuns]) << endl;
    cout << Dots::asInt ("     - hMETIS CType"                      ,_hmetisOptions[HMetisCType]) << endl;
    cout << Dots::asInt ("     - hMETIS RType"                      ,_hmetisOptions[HMetisRType]) << endl;
    cout << Dots::asInt ("     - hMETIS V-Cycle"                    ,_hmetisOptions[HMetisVCycle]) << endl;
    cout << Dots::asInt ("     - hMETIS Reconstruct Edges"          ,_hmetisOptions[HMetisReconst]) << endl;
    cout << Dots::asInt ("     - hMETIS Pre-Assignment"             ,_hmetisOptions[HMetisPreAssign]) << endl;
    cout << Dots::asInt ("     - hMETIS Random"                     ,_hmetisOptions[HMetisRandom]) << endl;
    cout << Dots::asInt ("     - hMETIS Debug Level"                ,_hmetisOptions[HMetisDebugLevel]) << endl;
  }


  string  Configuration::_getTypeName () const
  {
    return "Metis::Configuration";
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
    record->add ( getSlot("_partOrKWayHMetis"               , _partOrKWayHMetis               ) );
    record->add ( getSlot("_numberOfInstancesStopCriterion" , _numberOfInstancesStopCriterion ) );
    record->add ( getSlot("_globalConnectionsWeightRatio"   , _globalConnectionsWeightRatio   ) );
    record->add ( getSlot("_ubFactor"                       , _ubFactor                       ) );
    record->add ( getSlot("_hmetisOptions[CustomOptions]"   , _hmetisOptions[CustomOptions]   ) );
    record->add ( getSlot("_hmetisOptions[HMetisNRuns]"     , _hmetisOptions[HMetisNRuns  ]   ) );
    record->add ( getSlot("_hmetisOptions[HMetisCType]"     , _hmetisOptions[HMetisCType  ]   ) );
    record->add ( getSlot("_hmetisOptions[HMetisRType]"     , _hmetisOptions[HMetisRType  ]   ) );
    record->add ( getSlot("_hmetisOptions[HMetisVCycle]"    , _hmetisOptions[HMetisVCycle ]   ) );
    record->add ( getSlot("_hmetisOptions[HMetisReconst]"   , _hmetisOptions[HMetisReconst]   ) );
    record->add ( getSlot("_hmetisOptions[HMetisPreAssign]" , _hmetisOptions[HMetisPreAssign] ) );
    record->add ( getSlot("_hmetisOptions[HMetisRandom]"    , _hmetisOptions[HMetisRandom ]   ) );
    record->add ( getSlot("_hmetisOptions[HMetisDebugLevel]", _hmetisOptions[HMetisDebugLevel]) );
                                     
    return ( record );
  }


}  // End of Metis namespace.
