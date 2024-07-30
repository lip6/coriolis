// -*- mode: C++; explicit-buffer-name: "Configuration.cpp<tramontana>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2024-2024.
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                    Christian MASSON             |
// |  First impl.  :                            Yifei WU             |
// |  Second impl. :                    Jean-Paul CHAPUT             |
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
#include "tramontana/Configuration.h"
#include "tramontana/TramontanaEngine.h"


namespace Tramontana {

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


// -------------------------------------------------------------------
// Class  :  "Tramontana::Configuration".

  Configuration::Configuration ()
    : _mergeSupplies( Cfg::getParamBool("tramontana.mergeSupplies", false)->asBool() )
  { }


  Configuration::Configuration ( const Configuration& other )
    : _mergeSupplies( other._mergeSupplies )
  { }


  Configuration::~Configuration ()
  { }


  Configuration* Configuration::clone () const { return new Configuration(*this); }


  void  Configuration::print ( Cell* cell ) const
  {
    cmess1 << "  o  Configuration of ToolEngine<Tramontana> for Cell <" << cell->getName() << ">" << endl;
    cmess1 << Dots::asBool( "     - Merge supplies" ,_mergeSupplies ) << endl;
  }


  string  Configuration::_getTypeName () const
  { return "Tramontana::Configuration"; }


  string  Configuration::_getString () const
  {
    ostringstream  os;

    os << "<" << _getTypeName() << ">";

    return os.str();
  }


  Record* Configuration::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add( getSlot( "_mergeSupplies", _mergeSupplies ) );
    return record;
  }


}  // tramontana namespace.
