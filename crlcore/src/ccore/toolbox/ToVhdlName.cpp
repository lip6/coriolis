// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
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


#include  "crlcore/ToolBox.h"
#include  "hurricane/Instance.h"


namespace {

  using namespace std;
  using Hurricane::Name;


  Name vlogToVhdl ( const Name& vlogName )
  {
    string  vhdlName;

    for ( size_t i=0 ; i<vlogName.size() ; ++i ) {
      char translated = vlogName[i];

      if (translated == '\\') translated = '_';
      if (translated == '$' ) translated = '_';
      if (translated == '?' ) translated = '_';
      if (translated == '[' ) translated = '(';
      if (translated == ']' ) translated = ')';

      vhdlName += translated;
    }

    return Name(vhdlName);
  }
  



}  // Anonymous namespace.


namespace CRL {

  using Hurricane::ForEachIterator;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;


  void  toVhdlName ( Cell* topCell, unsigned int flags )
  {
    if (not topCell) return;

    vector<Net*> nets;
    for ( Net* net : topCell->getNets() ) nets.push_back( net );
    for ( auto net : nets ) net->setName( vlogToVhdl( net->getName() ) );
      
    vector<Instance*>               instances;
    set<Cell*,Entity::CompareById>  models;
    for ( Instance* inst : topCell->getInstances() ) {
      instances.push_back( inst );
      models.insert( inst->getMasterCell() );
    }
    for ( auto inst : instances ) inst->setName( vlogToVhdl( inst->getName() ) );

    for ( auto model : models ) toVhdlName( model, flags );
  }


}  // CRL namespace.
