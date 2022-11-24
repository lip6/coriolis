
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |              K n i k  -  Global Router                          |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./NetExtension.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KNIK_NET_EXTENSION__
#define  __KNIK_NET_EXTENSION__

#include  "hurricane/Property.h"

namespace Hurricane {
  class Net;
}


namespace Knik {


  using Hurricane::Name;
  using Hurricane::StandardPrivateProperty;
  using Hurricane::Net;


// -------------------------------------------------------------------
// Class  :  "Knik::NetExtensionDatas".


  class NetExtensionDatas {
    public:
                     NetExtensionDatas ();
    public:
      long           _id;
      unsigned long  _wireLength;
  };


// -------------------------------------------------------------------
// Class  :  "Knik::NetExtension".


  class NetExtension {
    public:
      typedef StandardPrivateProperty<NetExtensionDatas> Extension;
    public:
      static void           setWireLength ( Net* , unsigned long );
      static unsigned long  getWireLength ( const Net* );
      static void           setId         ( Net* , long );
      static long           getId         ( const Net* );
      static int            compare       ( const Net* , const Net* );
  };


} // End of Knik namespace.


#endif  // __KNIK_NET_EXTENSION__
