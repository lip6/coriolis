// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU/LIP6 2021-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Verilog / Hurricane  Interface                         |
// |                                                                 |
// |  Authors     :          Jean-Paul CHAPUT, Serge Rabyking        |
// |  E-mail      :          Jean-Paul.Chaput@lip6.fr                |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/Verilog.h"                           |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>

namespace Hurricane {
  class Cell;
  class Library;
}

namespace CRL {

  using Hurricane::Cell;
  using Hurricane::Library;

  class Verilog {
    public:
      static const uint64_t  TopCell       = (1 << 0);
    public:
      static bool  save ( Cell*, uint64_t flags );
  };

} // CRL namespace.
