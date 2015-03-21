// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2015, All Rights Reserved
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
#include "hurricane/Error.h"
#include "unicorn/ImportCell.h"
#include "unicorn/ImportCellDialog.h"


namespace Unicorn {

  using std::cerr;
  using std::endl;
  using std::string;
  using Hurricane::Error;
  using Hurricane::Cell;


  ImportCell::ImportCell ()
    : _count (0)
    , _lut   ()
    , _dialog(NULL)
  { }


  void  ImportCell::addImporter ( std::string menuName, std::function<Cell*(std::string)> callback )
  {
    _lut.insert( make_pair( _count, make_pair(menuName,callback) ) );
    _dialog->addFormat( menuName.c_str(), _count++ );
  }


  Cell*  ImportCell::load ( const string& cellName, int format )
  {
    Cell* cell = NULL; 

    ImportLut::iterator iimport = _lut.find( format );
    if (iimport == _lut.end()) {
      cerr << Error( "Importer id:%d for cell %s not found.", format, cellName.c_str() ) << endl;
      return NULL;
    }

    cell = iimport->second.second( cellName );

    if (not cell) {
      cerr << Error( "Cell not found: %s", cellName.c_str() ) << endl;
    }

    return cell;
  }


}  // Unicorn namespace.
