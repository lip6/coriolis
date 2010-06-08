
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
// |              M a u k a  -  P l a c e r                          |
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

#include  "hurricane/Cell.h"
#include  "crlcore/Utilities.h"
#include  "mauka/Configuration.h"



namespace Mauka {


  using  std::cout;
  using  std::cerr;
  using  std::endl;
  using  std::setprecision;
  using  std::ostringstream;
  using  std::string;
  using  Hurricane::tab;
  using  Hurricane::inltrace;


// -------------------------------------------------------------------
// Class  :  "Mauka::Configuration".


  Configuration* Configuration::_default = NULL;


  Configuration* Configuration::getDefault ()
  {
    if ( _default == NULL ) {
      _default = new Configuration ();
    }
    return _default;
  }


  Configuration::Configuration ()
    : _standardSimulatedAnnealing(false)
    , _ignorePins                (false)
    , _plotBins                  (true)
    , _searchRatio               (0.50)
    , _annealingNetMult          (0.90)
    , _annealingBinMult          (0.05)
    , _annealingRowMult          (0.05)
  { }


  Configuration::Configuration ( const Configuration& other )
    : _standardSimulatedAnnealing(other._standardSimulatedAnnealing)
    , _ignorePins                (other._ignorePins)
    , _plotBins                  (other._plotBins)
    , _searchRatio               (other._searchRatio)
    , _annealingNetMult          (other._annealingNetMult)
    , _annealingBinMult          (other._annealingBinMult)
    , _annealingRowMult          (other._annealingRowMult)
  { }


  Configuration::~Configuration ()
  { }


  Configuration* Configuration::clone () const
  { return new Configuration(*this); }


  void  Configuration::print ( Cell* cell ) const
  {
    cout << "  o  Configuration of ToolEngine<Mauka> for Cell <" << cell->getName() << ">" << endl;
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
    record->add ( getSlot( "_standardSimulatedAnnealing", _standardSimulatedAnnealing) );
    record->add ( getSlot( "_ignorePins"                , _ignorePins                ) );
    record->add ( getSlot( "_plotBins"                  , _plotBins                  ) );
    record->add ( getSlot( "_searchRatio"               , _searchRatio               ) );
                                     
    return ( record );
  }



}  // End of Mauka namespace.
