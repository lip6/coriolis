
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./NetExtension.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __NET_EXTENSION__
#define  __NET_EXTENSION__

#include  <set>

#include  "hurricane/Property.h"

namespace Hurricane {
  class Net;
  class Cell;
}


namespace CRL {


  using std::set;

  using Hurricane::Name;
  using Hurricane::StandardPrivateProperty;
  using Hurricane::Cell;
  using Hurricane::Net;


  typedef set<Name> PortSet;


// -------------------------------------------------------------------
// Class  :  "CRL::NetExtensionDatas".


  class NetExtensionDatas {
    public:
                    NetExtensionDatas ();
    public:
      PortSet       _ports;
  };


// -------------------------------------------------------------------
// Class  :  "CRL::NetExtension".


  class NetExtension {
    public:
      typedef StandardPrivateProperty<NetExtensionDatas> Extension;
    public:
      static void           addPort      ( Net* , const Name& );
      static const Name&    getPort      ( const Net* );
      static const PortSet* getPorts     ( const Net* );
      static const Net*     getNetByPort ( const Cell* , const Name& );
    protected:
      static Extension*     _getOrCreate ( Net* );
  };


} // End of CRL namespace.


#endif  // __NET_EXTENSION__
