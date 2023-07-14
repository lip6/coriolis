
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2010-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        C a d e n c e   D E F   E x p o r t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./crlcore/DefExport.h"                    |
// +-----------------------------------------------------------------+


#pragma once
namespace Hurricane {
  class Cell;
}


namespace CRL {


// -------------------------------------------------------------------
// Class  :  "CRL::DefExport".

  class DefExport {
    public:
      static const uint32_t WithLEF         = (1 << 0);
      static const uint32_t ExpandDieArea   = (1 << 1);
      static const uint32_t ProtectNetNames = (1 << 2);
    public:
      static void  drive ( Hurricane::Cell*, uint32_t flags );
  };


} // CRL namespace.
