
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
// |         ICCAD04 IBM Standard Cell Benchmarks                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./crlcore/Iccad04Lefdef.h"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __CRL_ICCAD04_LEFDEF__
#define  __CRL_ICCAD04_LEFDEF__

#include  <string>

namespace Hurricane {
  class Cell;
}


namespace CRL {

  using Hurricane::Cell;


  class Iccad04Lefdef {
    public:
      enum Flags { NoFlags=0x0 };
    public:
      static Cell*  load ( std::string benchmark, unsigned int flags=0 );
  };


} // End of CRL namespace.


#endif  // __CRL_ICCAD04_LEFDEF__
