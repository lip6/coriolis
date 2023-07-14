// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/NetBuilderHybridVH.h"               |
// +-----------------------------------------------------------------+

#pragma  once
#include "anabatic/NetBuilder.h"


namespace Anabatic {


// -----------------------------------------------------------------
// Class  :  "NetBuilderHybridVH".

  class NetBuilderHybridVH : public NetBuilder {
    public:
                            NetBuilderHybridVH       ();
      virtual              ~NetBuilderHybridVH       ();
      static  std::string   getStyle                 ();
      virtual void          doRp_AutoContacts        ( GCell*, Component*, AutoContact*& source, AutoContact*& target, uint64_t flags );
      virtual AutoContact*  doRp_Access              ( GCell*, Component*, uint64_t  flags );
              AutoContact*  doRp_AccessEastWestPin   ( GCell* , RoutingPad* );
              AutoContact*  doRp_AccessNorthSouthPin ( GCell* , RoutingPad* );
    private:                                     
              bool          doRp_xG_1M1        ( RoutingPad* );
              bool          doRp_1G_1PinM2     ( RoutingPad* );
              bool          doRp_xG_xM1_xM3    ( const std::vector<RoutingPad*>& );
      virtual bool          _do_1G_1M1         ();
      virtual bool          _do_2G_1M1         ();
      virtual bool          _do_xG_1M1         ();
      virtual bool          _do_xG             ();
              bool          _do_xG_xM1         ();
      virtual bool          _do_globalSegment  ();
    // Should never occur, so just return false.
      // virtual bool          _do_xG_1Pad        ();
      // virtual bool          _do_1G_1PinM2      ();
      virtual bool          _do_1G_xM1         ();
      // virtual bool          _do_xG_xM2         ();
      // virtual bool          _do_1G_1M3         ();
      // virtual bool          _do_xG_xM3         ();
      // virtual bool          _do_xG_1M1_1M2     ();
      virtual bool          _do_xG_xM1_xM3     ();
      virtual bool          _do_1G_xM1_1PinM1  ();
      virtual bool          _do_1G_xM1_1PinM2  ();
      // virtual bool          _do_4G_1M2         ();
      virtual bool          _do_2G             ();
      virtual bool          _do_2G_1PinM1      ();
      virtual bool          _do_1G_1PinM1      ();
      virtual bool          _do_1G_1PinM2      ();
      virtual bool          _do_xG_1PinM2      ();
      virtual void          singleGCell        ( AnabaticEngine*, Net* );
    public:
      virtual string        getTypeName        () const;
  };


}  // Anabatic namespace.
