// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2018-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          G D S I I / Hurricane  Interface                       |
// |                                                                 |
// |  Author      :                  Jean-Paul CHAPUT                |
// |  E-mail      :          Jean-Paul.Chaput@lip6.fr                |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/Gds.h"                               |
// +-----------------------------------------------------------------+


#ifndef  CRL_GDS_H
#define  CRL_GDS_H

#include  <string>


namespace Hurricane {
  class Cell;
  class Library;
}


namespace CRL {

  using Hurricane::Cell;
  using Hurricane::Library;


  class Gds {
    public:
      static bool  save ( Cell* );
      static bool  load ( Library*, std::string gdsPath );
  };


} // CRL namespace.

# endif
