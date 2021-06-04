// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           Alliance / Hurricane  Interface                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./toolbox/ToVhdlName.h"                   |
// +-----------------------------------------------------------------+


#include <cctype>
#include "crlcore/ToolBox.h"
#include "hurricane/Instance.h"


namespace CRL {

  using namespace std;
  using Hurricane::ForEachIterator;
  using Hurricane::Entity;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;


  Name NamingScheme::vlogToVhdl ( const Name& vlogName, uint32_t flags )
  {
    string  vhdlName;
    string  loweredName;

    size_t parCount    = 0;
    size_t posLeftPar  = 0;
    size_t posRightPar = 0;
    for ( size_t i=0 ; i<vlogName.size() ; ++i ) {
      if (vlogName[i] == '(') { ++parCount; posLeftPar=i; }
      if (vlogName[i] == '[') { ++parCount; posLeftPar=i; }
      if (vlogName[i] == ')') { posRightPar=i; }
      if (vlogName[i] == ']') { posRightPar=i; }
      loweredName.push_back( tolower(vlogName[i]) );
      if ( (flags & UniquifyUpperCase) and (vlogName[i] != tolower(vlogName[i])) )
         loweredName.push_back( 'u' );
    }
    char leftPar  = (parCount > 1) ? '_' : '(';
    char rightPar = (parCount > 1) ? '_' : ')';

    if (parCount == 1) {
      for ( size_t i=posLeftPar+1 ; i<posRightPar ; ++i ) {
        if (not isdigit(vlogName[i])) {
          leftPar = rightPar = '_';
          break;
        }
      }
    }

  // VHDL reserved keywords (scalar).
    if (loweredName == "in"   ) return "in_v";
    if (loweredName == "out"  ) return "out_v";
    if (loweredName == "ref"  ) return "ref_v";
    if (loweredName == "inout") return "inout_v";
    if (loweredName == "true" ) return "bool_true";
    if (loweredName == "false") return "bool_false";
    if (loweredName == "undef") return "bool_undef";

    string refName = (flags & NoLowerCase) ? getString(vlogName) : loweredName;
    for ( size_t i=0 ; i<refName.size() ; ++i ) {
      char translated = refName[i];

      if ( vhdlName.empty() and (isdigit(translated)) )
        vhdlName += 'n';

      if (translated == '\\') translated = '_';
      if (translated == '/' ) translated = '_';
      if (translated == '.' ) translated = '_';
      if (translated == '%' ) translated = '_';
      if (translated == '$' ) translated = '_';
      if (translated == '?' ) translated = '_';
      if (translated == ':' ) translated = '_';
      if (translated == '[' ) translated = leftPar;
      if (translated == ']' ) translated = rightPar;

      if (translated == '_') {
        if (vhdlName.empty()      ) continue;
        if (i == refName.size()-1) break;
        if (vhdlName.back() == '_') continue;
      }

      vhdlName += translated;
    }

  // VHDL reserved keywords (vector).
    if (loweredName.substr(0,3) == "in("   ) vhdlName.insert(2,"_v");
    if (loweredName.substr(0,4) == "out("  ) vhdlName.insert(3,"_v");
    if (loweredName.substr(0,6) == "inout(") vhdlName.insert(5,"_v");
    if (loweredName             == "true"  ) vhdlName.insert(0,"value_");
    if (loweredName             == "false" ) vhdlName.insert(0,"value_");
    if (loweredName             == "undef" ) vhdlName.insert(0,"value_");

    return Name(vhdlName);
  }


  void  NamingScheme::toVhdl ( Cell* topCell, uint32_t flags )
  {
    if (not topCell) return;

    converter_t converter = nullptr;
    if (flags & FromVerilog)
      converter = vlogToVhdl;

    if (converter == nullptr) return;

    topCell->setName( converter(topCell->getName(),flags) );

    vector<Net*> nets;
    for ( Net* net : topCell->getNets() ) nets.push_back( net );
    for ( auto net : nets ) net->setName( converter( net->getName(), flags ) );
      
    vector<Instance*>               instances;
    set<Cell*,Entity::CompareById>  models;
    for ( Instance* inst : topCell->getInstances() ) {
      instances.push_back( inst );
      models.insert( inst->getMasterCell() );
    }
    for ( auto inst : instances ) inst->setName( converter( inst->getName(), flags ) );

    if (flags & Recursive)
      for ( auto model : models ) {
        if (not model->isTerminal()) toVhdl( model, flags );
      }
  }


  NamingScheme::NamingScheme ( uint32_t flags )
    : _flags    (flags)
    , _converter(nullptr)
  {
    if (_flags & FromVerilog) _converter = vlogToVhdl;
  }


  Name  NamingScheme::convert ( const Name& name ) const
  {
    if (_converter == nullptr) return name;
    return _converter(name,_flags);
  }


}  // CRL namespace.
