
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
// |        C a d e n c e   D E F   E x p o r t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./crlcore/LefExport.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __CRL_LEF_EXPORT__
#define  __CRL_LEF_EXPORT__


namespace Hurricane {
  class Cell;
  class Library;
}


namespace CRL {


  class LefExport {
    public:
      enum Flag { WithTechnology=0x1, WithSpacers=0x2 };
    public:
      static void  drive ( Hurricane::Cell*   , unsigned int flags );
      static void  drive ( Hurricane::Library*, unsigned int flags );
  };


} // End of CRL namespace.


#endif  // __CRL_LEF_EXPORT__
