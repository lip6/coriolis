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
    /*if (saveState & Catalog::State::VstUseConcat)*/ entityFlags |= Vhdl::Entity::VstUseConcat;
    
  //NamingScheme::toVhdl( cell, NamingScheme::FromVerilog );
    Vhdl::Entity* vhdlEntity = Vhdl::EntityExtension::create( cell, entityFlags );
    string        celltest   = cellPath;
    ofstream      ccelltest  ( celltest.c_str() );

    vhdlEntity->toEntity( ccelltest );
    ccelltest << endl;
    ccelltest.close();

    Vhdl::EntityExtension::destroy( cell );
  }


}  // CRL namespace.
