// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        C a d e n c e   D E F   I m p o r t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./crlcore/DefImport.h"                    |
// +-----------------------------------------------------------------+


#ifndef  CRL_DEF_IMPORT_H
#define  CRL_DEF_IMPORT_H

#include <string>

namespace Hurricane {
  class Cell;
}


namespace CRL {


  class DefImport {
    public:
      enum Flags { FitAbOnCells=0x1 };
    public:
      static void             reset ();
      static Hurricane::Cell* load  ( std::string design, unsigned int flags );
  };


} // End of CRL namespace.


#endif  // CRL_DEF_IMPORT_H
