// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
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
                            NetBuilderHV           ();
      virtual              ~NetBuilderHV           ();
      virtual void          doRp_AutoContacts      ( GCell*, Component*, AutoContact*& source, AutoContact*& target, uint64_t flags );
      virtual AutoContact*  doRp_Access            ( GCell*, Component*, uint64_t  flags );
              AutoContact*  doRp_AccessNorthPin    ( GCell*, RoutingPad* );
              AutoContact*  doRp_AccessEastWestPin ( GCell*, RoutingPad* );
    private:                                     
      virtual bool          _do_1G_1M1             ();
      virtual bool          _do_1G_xM1             ();
      virtual bool          _do_xG                 ();
      virtual bool          _do_2G                 ();
      virtual bool          _do_2G_1M1             ();
      virtual bool          _do_xG_1Pad            ();
      virtual bool          _do_1G_1PinM1          ();
      virtual bool          _do_2G_1PinM1          ();
      virtual bool          _do_1G_1PinM2          ();
      virtual bool          _do_xG_1PinM2          ();
      virtual bool          _do_1G_1PinM3          ();
      virtual bool          _do_xG_1PinM3          ();
      virtual bool          _do_xG_1M1             ();
      virtual bool          _do_xG_1M1_1M2         ();
      virtual bool          _do_xG_xM1_xM3         ();
      virtual bool          _do_4G_1M2             ();
      virtual bool          _do_xG_xM2             ();
      virtual bool          _do_1G_1M3             ();
      virtual bool          _do_xG_xM3             ();
      virtual bool          _do_1G_xM1_1PinM2      ();
      virtual bool          _do_2G_xM1_1PinM2      ();
      virtual bool          _do_1G_1M1_1PinM3      ();
      virtual bool          _do_2G_xM1_1PinM3      ();
      virtual bool          _do_3G_xM1_1PinM3      ();
      virtual bool          _do_globalSegment      ();
      virtual void          singleGCell            ( AnabaticEngine*, Net* );
    public:                                        
      virtual string        getTypeName            () const;
  };


}  // Anabatic namespace.

#endif  // ANABATIC_NET_BUILDER_HV_H
