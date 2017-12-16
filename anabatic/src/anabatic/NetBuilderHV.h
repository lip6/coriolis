// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/NetBuilderHV.h"                     |
// +-----------------------------------------------------------------+

#ifndef  ANABATIC_NET_BUILDER_HV_H
#define  ANABATIC_NET_BUILDER_HV_H

#include "anabatic/NetBuilder.h"


namespace Anabatic {


// -----------------------------------------------------------------
// Class  :  "NetBuilderHV".

  class NetBuilderHV : public NetBuilder {
    public:
                            NetBuilderHV       ();
      virtual              ~NetBuilderHV       ();
      virtual void          doRp_AutoContacts  ( GCell*, Component*, AutoContact*& source, AutoContact*& target, uint64_t flags );
      virtual AutoContact*  doRp_Access        ( GCell*, Component*, uint64_t  flags );
    private:                                     
      virtual void          _do_1G_1M1         ();
      virtual void          _do_1G_xM1         ();
  };


}  // Anabatic namespace.

#endif  // ANABATIC_NET_BUILDER_HV_H
