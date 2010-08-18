
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |        C a d e n c e   D E F   I m p o r t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./crlcore/DefImport.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __CRL_DEF_IMPORT__
#define  __CRL_DEF_IMPORT__

#include  <string>

namespace Hurricane {
  class Cell;
}


namespace CRL {


  class DefImport {
    public:
      enum Flags { FitAbOnCells=0x1 };
    public:
      static Hurricane::Cell* load ( std::string design, unsigned int flags );
  };


} // End of CRL namespace.


#endif  // __CRL_DEF_IMPORT__
