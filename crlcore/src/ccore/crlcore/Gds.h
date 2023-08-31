// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2018-2022, All Rights Reserved
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

#pragma  once
#include <string>


namespace Hurricane {
  class Cell;
  class Library;
}


namespace CRL {

  using Hurricane::Cell;
  using Hurricane::Library;


  class Gds {
    public:
      static const uint32_t     NoGdsPrefix        = (1<<0);
      static const uint32_t     Layer_0_IsBoundary = (1<<1);
      static const uint32_t     NoBlockages        = (1<<2);
      static       std::string  _topCellName;
    public:
             static bool         save           ( Cell* );
             static bool         load           ( Library*, std::string gdsPath, uint32_t flags=0 );
      inline static void         setTopCellName ( std::string );
      inline static std::string  getTopCellName ();
  };

  
  inline void         Gds::setTopCellName ( std::string topCellName ) { _topCellName=topCellName; }
  inline std::string  Gds::getTopCellName () { return _topCellName; }


} // CRL namespace.
