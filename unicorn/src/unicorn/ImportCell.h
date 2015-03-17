// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ImportCell.h"                           |
// +-----------------------------------------------------------------+


#ifndef  UNICORN_IMPORT_CELL_H
#define  UNICORN_IMPORT_CELL_H

#include <string>

namespace Hurricane {
  class Cell;
}

namespace Unicorn {


  class ImportCell {
    public:
      enum Formats { AcmSigda=1, Ispd04, Ispd05, Iccad04, AllianceDef };
    public:
      static Hurricane::Cell* load ( const std::string&, int format );
  };


}  // Unicorn namespace.

#endif
