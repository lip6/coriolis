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
#include "hurricane/Warning.h"
#include "hurricane/Cell.h"
#include "hurricane/Net.h"
#include "hurricane/Instance.h"
#include "hurricane/Plug.h"
using namespace Hurricane;

#include "crlcore/Catalog.h"
#include "crlcore/NetExtension.h"
#include "crlcore/ToolBox.h"
#include "crlcore/VhdlBit.h"
#include "crlcore/VhdlSignal.h"
#include "crlcore/VhdlEntity.h"
#include "Vst.h"


namespace CRL {

  using namespace std;


  void  vstDriver ( const string cellPath, Cell *cell, unsigned int& saveState )
  {
    unsigned int entityFlags = Vhdl::Entity::EntityMode /* | Vhdl::Entity::IeeeMode */;
    if (saveState & Catalog::State::VstUseConcat        ) entityFlags |= Vhdl::Entity::VstUseConcat;
    if (saveState & Catalog::State::VstNoLowerCase      ) entityFlags |= Vhdl::Entity::VstNoLowerCase;
    if (saveState & Catalog::State::VstUniquifyUpperCase) entityFlags |= Vhdl::Entity::VstUniquifyUpperCase;
    if (saveState & Catalog::State::VstNoLinkage        ) entityFlags |= Vhdl::Entity::VstNoLinkage;
    
  //NamingScheme::toVhdl( cell, NamingScheme::FromVerilog );
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
      NamingScheme ns (NamingScheme::FromVerilog);
      file     = getString( ns.convert(file) );
      celltest = path + '/' + file + '.' + ext;

      if (cellPath != celltest) remove( cellPath.c_str() );
    }
    ofstream cellStream ( celltest.c_str() );

    vhdlEntity->toEntity( cellStream );
    cellStream << endl;
    cellStream.close();

  //Vhdl::EntityExtension::destroy( cell );
    Vhdl::EntityExtension::destroyAll();
  }


}  // CRL namespace.
