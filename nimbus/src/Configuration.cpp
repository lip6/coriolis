
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

#include  "hurricane/Cell.h"
#include  "crlcore/Utilities.h"
#include  "nimbus/Configuration.h"



namespace Nimbus {


  using  std::cout;
  using  std::cerr;
  using  std::endl;
  using  std::setprecision;
  using  std::ostringstream;
  using  std::string;
  using  Hurricane::tab;
  using  Hurricane::inltrace;


// -------------------------------------------------------------------
// Class  :  "Nimbus::Configuration".


  Configuration* Configuration::_default = NULL;


  Configuration* Configuration::getDefault ()
  {
    if ( _default == NULL ) {
      _default = new Configuration ();
    }
    return _default;
  }


  Configuration::Configuration ()
    : _pinsPlacement(false)
    , _aspectRatio  (1.00)
    , _margin       (0.40)
    , _workZone     ()
  { }


  Configuration::Configuration ( const Configuration& other )
    : _pinsPlacement(other._pinsPlacement)
    , _aspectRatio  (other._aspectRatio)
    , _margin       (other._margin)
    , _workZone     (other._workZone)
  { }


  Configuration::~Configuration ()
  { }


  Configuration* Configuration::clone () const
  { return new Configuration(*this); }


  void  Configuration::print ( Cell* cell ) const
  {
    cout << "  o  Configuration of ToolEngine<Nimbus> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asBool      ("     - Pins Placement"                    ,_pinsPlacement) << endl;
    cout << Dots::asPercentage("     - Margin Ratio"                      ,_margin) << endl;
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
