// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2004-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        Yosys & Blif / Hurricane  Interface                      |
// |                                                                 |
// |  Author      :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./alliance/vst/VstDriver.cpp"                 |
// +-----------------------------------------------------------------+


#include <cstddef>
#include <cstdio>
#include "hurricane/Timer.h"
#include "hurricane/Warning.h"
#include "hurricane/Cell.h"
#include "hurricane/Net.h"
#include "hurricane/Instance.h"
#include "hurricane/Plug.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/Catalog.h"
#include "crlcore/NetExtension.h"
#include "crlcore/ToolBox.h"
#include "crlcore/VhdlBit.h"
#include "crlcore/VhdlSignal.h"
#include "crlcore/VhdlEntity.h"
#include "Vst.h"


namespace CRL {

  using namespace std;
  using Hurricane::Timer;


  void  vstDriver ( const string cellPath, Cell *cell, unsigned int& saveState )
  {
    // Timer timer;
    // timer.start();

    NamingScheme ns (NamingScheme::FromVerilog);
    unsigned int entityFlags = Vhdl::Entity::EntityMode /* | Vhdl::Entity::IeeeMode */;
    if (saveState & Catalog::State::VstUseConcat        ) entityFlags |= Vhdl::Entity::VstUseConcat;
    if (saveState & Catalog::State::VstNoLowerCase      ) entityFlags |= Vhdl::Entity::VstNoLowerCase;
    if (saveState & Catalog::State::VstNoLinkage        ) entityFlags |= Vhdl::Entity::VstNoLinkage;
    if (saveState & Catalog::State::VstUniquifyUpperCase) {
      entityFlags |= Vhdl::Entity::VstUniquifyUpperCase;
      ns.setUniquifyUpperCase( true );
    }
    
    // NamingScheme::toVhdl( cell, NamingScheme::FromVerilog );
    Vhdl::Entity* vhdlEntity = Vhdl::EntityExtension::create( cell, entityFlags );

    string celltest = cellPath;
    if (not (saveState & Catalog::State::VstNoLowerCase)) {
      size_t  slash = cellPath.find_last_of( "/" );
      size_t  dot   = cellPath.find_last_of( "." );
      string  path  = "";
      string  file  = "";
      string  ext   = cellPath.substr( dot+1 );
      if (slash != string::npos) {
        path = cellPath.substr( 0, slash );
        file = cellPath.substr( slash+1, dot-slash-1 );
      } else {
        file = cellPath;
      }
      file     = getString( ns.convert(file) );
      celltest = path + '/' + file + '.' + ext;

      if (cellPath != celltest) remove( cellPath.c_str() );
    }
    ofstream cellStream ( celltest.c_str() );

    vhdlEntity->toEntity( cellStream );
    cellStream << endl;
    cellStream.close();

    // Vhdl::EntityExtension::destroy( cell );
    Vhdl::EntityExtension::destroyAll();
    // timer.stop();
    // ostringstream result;

    // result <<  Timer::getStringTime(timer.getCombTime()) 
    //        << ", " << Timer::getStringMemory(timer.getIncrease());
    // cmess1 << Dots::asString( "     - Done in", result.str() ) << endl;
  }


}  // CRL namespace.
