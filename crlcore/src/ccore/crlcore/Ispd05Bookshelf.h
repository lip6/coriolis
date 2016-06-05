// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |         ISPD05 IBM Standard Cell Benchmarks                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./crlcore/Ispd05Bookshelf.h"              |
// +-----------------------------------------------------------------+


#ifndef  CRL_ISPD05_BOOKSHELF_H
#define  CRL_ISPD05_BOOKSHELF_H

#include  <string>

namespace Hurricane {
  class Cell;
}


namespace CRL {

  using Hurricane::Cell;


  class Ispd05 {
    public:
      static Cell*  load ( std::string benchmark );
  };


} // End of CRL namespace.


#endif  // CRL_ISPD05_BOOKSHELF

