// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ImportCell.cpp"                         |
// +-----------------------------------------------------------------+


#include <iostream>
#include <boost/any.hpp>
#include "hurricane/Error.h"
#include "unicorn/ImportCell.h"


namespace Unicorn {

  using std::cerr;
  using std::endl;
  using std::string;
  using std::function;
  using Hurricane::Error;
  using Hurricane::Cell;
  using Hurricane::Library;


  Importer:: Importer () : _rvalue() { }
  Importer::~Importer () { }


  ImportCell::ImportCell ()
    :  _importers()
    , _dialog    (NULL)
    , _activeId  (0)
  { }


  ImportCell::~ImportCell ()
  {
    for ( Importer* importer : _importers ) delete importer;
  }


  bool  ImportCell::load ( const string& cellName, int formatId )
  {
    if ( (formatId < 0) or ((size_t)formatId >= _importers.size()) ) {
      cerr << Error( "Importer id:%d for library or cell %s not found.", formatId, cellName.c_str() ) << endl;
      _activeId = _importers.size();
      return false;
    }
    _activeId = (size_t)formatId;
    
    Importer* importer = _importers[ _activeId ];
    importer->load( cellName );

    if (importer->isA<Cell*>() and not importer->getAs<Cell*>()) {
      cerr << Error( "Cell not found: %s", cellName.c_str() ) << endl;
      _activeId = _importers.size();
      return false;
    }

    if (importer->isA<Library*>() and not importer->getAs<Library*>()) {
      cerr << Error( "Library not found: %s", cellName.c_str() ) << endl;
      _activeId = _importers.size();
      return false;
    }

    return true;
  }


}  // Unicorn namespace.
