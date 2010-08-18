
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
// |         ISPD04 IBM Standard Cell Benchmarks                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./crlcore/Ispd04Bookshelf.h"              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __CRL_ISPD04_BOOKSHELF__
#define  __CRL_ISPD04_BOOKSHELF__

#include  <string>

namespace Hurricane {
  class Cell;
}


namespace CRL {

  using Hurricane::Cell;


  class Ispd04 {
    public:
      static Cell*  load ( std::string benchmark );
  };


} // End of CRL namespace.


#endif  // __CRL_ISPD04_BOOKSHELF__

