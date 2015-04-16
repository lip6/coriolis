// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           Alliance / Hurricane  Interface                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./toolbox/ToVhdlName.h"                   |
// +-----------------------------------------------------------------+


#include <cctype>
#include "crlcore/ToolBox.h"
#include "hurricane/Instance.h"


namespace CRL {

  using namespace std;
  using Hurricane::ForEachIterator;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;


  Name NamingScheme::vlogToVhdl ( const Name& vlogName )
  {
    string  vhdlName;

    size_t parCount = 0;
    for ( size_t i=0 ; i<vlogName.size() ; ++i ) {
      if (vlogName[i] == '(') ++parCount;
      if (vlogName[i] == '[') ++parCount;
    }
    char leftPar  = (parCount > 1) ? '_' : '(';
    char rightPar = (parCount > 1) ? '_' : ')';

    for ( size_t i=0 ; i<vlogName.size() ; ++i ) {
      char translated = tolower( vlogName[i] );

      if ( (i == 0) and (isdigit(translated)) )
        vhdlName += 'N';

      if (translated == '\\') translated = '_';
      if (translated == '$' ) translated = '_';
      if (translated == '?' ) translated = '_';
      if (translated == ':' ) translated = '_';
      if (translated == '[' ) translated = leftPar;
      if (translated == ']' ) translated = rightPar;

      if (translated == '_') {
        if (i == 0                ) continue;
        if (i == vlogName.size()-1) break;
        if (vhdlName.back() == '_') continue;
      }

      vhdlName += translated;
    }

    return Name(vhdlName);
  }


  void  NamingScheme::toVhdl ( Cell* topCell, unsigned int flags )
  {
    if (not topCell) return;

    converter_t converter = nullptr;
    if (flags & FromVerilog)
      converter = vlogToVhdl;

    if (converter == nullptr) return;

    topCell->setName( converter(topCell->getName()) );

    vector<Net*> nets;
    forEach ( Net*, inet, topCell->getNets() ) nets.push_back( *inet );
    for ( auto net : nets ) net->setName( converter( net->getName() ) );
      
    vector<Instance*> instances;
    set<Cell*>        models;
    forEach ( Instance*, iinst, topCell->getInstances() ) {
      instances.push_back( *iinst );
      models.insert( iinst->getMasterCell() );
    }
    for ( auto inst : instances ) inst->setName( converter( inst->getName() ) );

    if (flags & Recursive)
      for ( auto model : models ) toVhdl( model, flags );
  }


}  // CRL namespace.
