// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Configuration.cpp"                      |
// +-----------------------------------------------------------------+


#include <iostream>
#include <iomanip>
#include "vlsisapd/configuration/Configuration.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "crlcore/AllianceFramework.h"
#include "mauka/Configuration.h"



namespace Mauka {


  using  std::cout;
  using  std::cerr;
  using  std::endl;
  using  std::setprecision;
  using  std::ostringstream;
  using  std::string;
  using  Hurricane::tab;
  using  Hurricane::inltrace;
  using  CRL::AllianceFramework;


// -------------------------------------------------------------------
// Class  :  "Mauka::Configuration".


  Configuration::Configuration ( CellGauge* cg )
    : _cellGauge                 (NULL)
    , _refreshCb                 ()
    , _standardSimulatedAnnealing(Cfg::getParamBool      ("mauka.standardAnnealing",false)->asBool())
    , _ignorePins                (Cfg::getParamBool      ("mauka.ignorePins"       ,false)->asBool())
    , _plotBins                  (Cfg::getParamBool      ("mauka.plotBins"         ,true )->asBool())
    , _insertFeeds               (Cfg::getParamBool      ("mauka.insertFeeds"      ,true )->asBool())
    , _searchRatio               (Cfg::getParamPercentage("mauka.searchRatio"      , 50.0)->asDouble())
    , _annealingNetMult          (Cfg::getParamPercentage("mauka.annealingNetMult" , 90.0)->asDouble())
    , _annealingBinMult          (Cfg::getParamPercentage("mauka.annealingBinMult" ,  5.0)->asDouble())
    , _annealingRowMult          (Cfg::getParamPercentage("mauka.annealingRowMult" ,  5.0)->asDouble())
  {
    _cellGauge = (cg != NULL) ? cg->getClone() : AllianceFramework::get()->getCellGauge();
  }


  Configuration::Configuration ( const Configuration& other )
    : _cellGauge                 (other._cellGauge->getClone())
    , _refreshCb                 (other._refreshCb)
    , _standardSimulatedAnnealing(other._standardSimulatedAnnealing)
    , _ignorePins                (other._ignorePins)
    , _plotBins                  (other._plotBins)
    , _searchRatio               (other._searchRatio)
    , _annealingNetMult          (other._annealingNetMult)
    , _annealingBinMult          (other._annealingBinMult)
    , _annealingRowMult          (other._annealingRowMult)
  { }


  Configuration::~Configuration ()
  { }


  void  Configuration::print ( Cell* cell ) const
  {
    cout << "  o  Configuration of ToolEngine<Mauka> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asIdentifier("     - Cell Gauge"                        ,getString(_cellGauge->getName())) << endl;
    cout << Dots::asBool      ("     - Use standard simulated annealing"  ,_standardSimulatedAnnealing) << endl;
    cout << Dots::asBool      ("     - Ignore Pins"                       ,_ignorePins) << endl;
    cout << Dots::asBool      ("     - Plot Bins"                         ,_plotBins) << endl;
    cout << Dots::asPercentage("     - Search Ratio"                      ,_searchRatio) << endl;
  }


  string  Configuration::_getTypeName () const
  {
    return "Mauka::Configuration";
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
    record->add ( getSlot( "_cellGauge"                 , _cellGauge                 ) );
    record->add ( getSlot( "_standardSimulatedAnnealing", _standardSimulatedAnnealing) );
    record->add ( getSlot( "_ignorePins"                , _ignorePins                ) );
    record->add ( getSlot( "_plotBins"                  , _plotBins                  ) );
    record->add ( getSlot( "_searchRatio"               , _searchRatio               ) );
                                     
    return ( record );
  }



}  // End of Mauka namespace.
