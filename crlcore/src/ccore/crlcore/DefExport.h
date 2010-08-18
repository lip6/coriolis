
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
// |  C++ Header  :       "./crlcore/DefExport.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __CRL_DEF_EXPORT__
#define  __CRL_DEF_EXPORT__


namespace Hurricane {
  class Cell;
}


namespace CRL {


  class DefExport {
    public:
      enum Flag { WithLEF=0x1, ExpandDieArea=0x2 };
    public:
      static void  drive ( Hurricane::Cell*,  unsigned int flags );
  };


} // End of CRL namespace.


#endif  // __CRL_DEF_EXPORT__
