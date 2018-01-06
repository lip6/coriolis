// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./NetExtension.h"                         |
// +-----------------------------------------------------------------+


#ifndef  CRL_NET_EXTENSION_H
#define  CRL_NET_EXTENSION_H

#include <set>
#include "hurricane/Property.h"

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


} // CRL namespace.

#endif  // CRL_NET_EXTENSION_H
