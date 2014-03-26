// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2014, All Rights Reserved
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
#include "crlcore/AcmSigda.h"
#include "crlcore/Ispd04Bookshelf.h"
#include "crlcore/Ispd05Bookshelf.h"
#include "crlcore/Iccad04Lefdef.h"
#include "crlcore/DefImport.h"
#include "unicorn/ImportCell.h"


namespace Unicorn {

  using std::cerr;
  using std::endl;
  using std::string;
  using Hurricane::Error;
  using Hurricane::Cell;
  using CRL::AcmSigda;
  using CRL::Ispd04;
  using CRL::Ispd05;
  using CRL::Iccad04Lefdef;
  using CRL::DefImport;


  Cell*  ImportCell::load ( const string& cellName, int format )
  {
    Cell* cell = NULL; 

    switch ( format ) {
      case AcmSigda:
        cell = AcmSigda::load( cellName );
        break;
      case Ispd04:
        cell = Ispd04::load( cellName );
        break;
      case Ispd05:
        cell = Ispd05::load( cellName );
        break;
      case Iccad04:
        cell = Iccad04Lefdef::load( cellName, 0 );
        break;
      case AllianceDef:
        cell = DefImport::load( cellName.c_str() , DefImport::FitAbOnCells );
        break;
    }

    if (not cell) {
      cerr << Error( "Cell not found: %s", cellName.c_str() ) << endl;
    }

    return cell;
  }


}  // Unicorn namespace.
