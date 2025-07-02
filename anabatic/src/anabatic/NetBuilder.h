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
// |  C++ Header  :  "./anabatic/NetBuilder.h"                       |
// +-----------------------------------------------------------------+

#pragma  once
#include <vector>
#include <map>

namespace Hurricane {
  class Hook;
  class Net;
  class RoutingPad;
  class Component;
}


namespace Anabatic {

  using std::string;
  using std::vector;
  using std::map;
  using std::endl;
  using Hurricane::Hook;
  using Hurricane::Net;
  using Hurricane::RoutingPad;
  using Hurricane::Component;
  class GCell;
  class AutoContact;
  class AutoSegment;
  class AnabaticEngine;
  class NetData;
  

// -------------------------------------------------------------------
// Class  :  "ForkStack".

  class ForkStack {
    public:
      inline void         push       ( Hook* from, AutoContact* contact, uint64_t flags );
      inline void         pop        ();
      inline Hook*        getFrom    () const;
      inline AutoContact* getContact () const;
      inline uint64_t     getFlags   () const;
      inline void         setFlags   ( uint64_t );
    private:
      struct Element {
                 Hook*         _from;
                 AutoContact*  _contact;
                 uint64_t      _flags;
          inline               Element ( Hook* from, AutoContact* contact, uint64_t flags );
      };
    private:
      list<Element>  _stack;
  };


  inline              ForkStack::Element::Element ( Hook* from, AutoContact* contact, uint64_t flags ) : _from(from), _contact(contact), _flags(flags) {}
  inline void         ForkStack::pop              () { if (not _stack.empty()) _stack.pop_back(); }
  inline Hook*        ForkStack::getFrom          ()  const { return _stack.empty() ? NULL : _stack.back()._from; }
  inline AutoContact* ForkStack::getContact       ()  const { return _stack.empty() ? NULL : _stack.back()._contact; }
  inline uint64_t     ForkStack::getFlags         ()  const { return _stack.empty() ? 0    : _stack.back()._flags; }
  inline void         ForkStack::setFlags         ( uint64_t flags ) { if (not _stack.empty()) _stack.back()._flags |= flags; }


  inline void  ForkStack::push ( Hook* from, AutoContact* contact, uint64_t flags )
  {
    cdebug_log(145,0) << "  Stacking: " << endl;
    cdebug_log(145,0) << "  + " << from << endl;
    cdebug_log(145,0) << "  + " << contact << endl;
    cdebug_log(145,0) << "  + " << flags << endl;
    _stack.push_back( Element(from,contact,flags) );
  }


// -------------------------------------------------------------------
// Class  :  "NetBuilder".

  
  const uint64_t  BitsG    = 8;
  const uint64_t  BitsM1   = 4;
  const uint64_t  BitsM2   = 4;
  const uint64_t  BitsM3   = 4;
  const uint64_t  BitsM4   = 4;
  const uint64_t  BitsM5   = 4;
  const uint64_t  BitsM6   = 4;
  const uint64_t  BitsPads = 4;
  const uint64_t  BitsPins = 4;


  static constexpr uint64_t  connexConfig ( uint64_t Gs
                                          , uint64_t M1s
                                          , uint64_t M2s
                                          , uint64_t M3s
                                          , uint64_t M4s
                                          , uint64_t M5s
                                          , uint64_t M6s
                                          , uint64_t Pads
                                          , uint64_t Pins )
  {
    return Gs + (M1s  << (BitsG))
              + (M2s  << (BitsG + BitsM1))
              + (M3s  << (BitsG + BitsM1 + BitsM2))
              + (M4s  << (BitsG + BitsM1 + BitsM2 + BitsM3))
              + (M5s  << (BitsG + BitsM1 + BitsM2 + BitsM3 + BitsM4))
              + (M6s  << (BitsG + BitsM1 + BitsM2 + BitsM3 + BitsM4 + BitsM5))
              + (Pads << (BitsG + BitsM1 + BitsM2 + BitsM3 + BitsM4 + BitsM5 + BitsM6))
              + (Pins << (BitsG + BitsM1 + BitsM2 + BitsM3 + BitsM4 + BitsM5 + BitsM6 + BitsPads));
  };


  class NetBuilder {
    public:
      enum FunctionFlags { NoFlags         = 0
                         , SortDecreasing  = (1 <<  0)
                         , HAccess         = (1 <<  1)
                         , HAccessEW       = (1 <<  2)
                         , VLarge          = (1 <<  3)
                         , VSmall          = (1 <<  4)
                         , HSmall          = (1 <<  5)
                         , Punctual        = (1 <<  6)
                         , M1Offgrid       = (1 <<  7)
                         , HCollapse       = (1 <<  8)
                         , VCollapse       = (1 <<  9)
                         , Terminal        = (1 << 10)
                         , DoSourceContact = (1 << 11)
                         , DoTargetContact = (1 << 12)
                         , SouthBound      = (1 << 13)
                         , NorthBound      = (1 << 14)
                         , WestBound       = (1 << 15)
                         , EastBound       = (1 << 16)
                         , Middle          = (1 << 17)
                         , UseNonPref      = (1 << 18)
                         , NoProtect       = (1 << 19)
                         , ToUpperRouting  = (1 << 20)
                         , AddHNonPref     = (1 << 21)
                         , HBothAccess     = HAccess|HAccessEW
                         , SouthWest       = SouthBound|WestBound
                         , NorthEast       = NorthBound|EastBound
                         };
      enum TopologyFlag  { Global_Vertical_End   = (1 << 0)
                         , Global_Horizontal_End = (1 << 1)
                         , Global_Horizontal     = (1 << 2)
                         , Global_Vertical       = (1 << 3)
                         , Global_Turn           = (1 << 4)
                         , Global_Fork           = (1 << 5)
                         , Global_Fixed          = (1 << 6)
                         , Global_End            = Global_Vertical_End | Global_Horizontal_End
                         , Global_Split          = Global_Horizontal | Global_Vertical | Global_Fork
                         };

    // Connexity Union Type.
      union UConnexity {
        uint64_t connexity;
        struct {
          unsigned int globals : BitsG;
          unsigned int M1      : BitsM1;
          unsigned int M2      : BitsM2;
          unsigned int M3      : BitsM3;
          unsigned int M4      : BitsM4;
          unsigned int M5      : BitsM5;
          unsigned int M6      : BitsM6;
          unsigned int Pad     : BitsPads;
          unsigned int Pin     : BitsPins;
        } fields;
      };
    public:
      template< typename BuilderT >
      static  void                          load                   ( AnabaticEngine*, Net* );
      static  uint64_t                      checkRoutingPadSize    ( RoutingPad* rp );
      static  Hook*                         getSegmentOppositeHook ( Hook* hook );
      static  uint64_t                      getSegmentHookType     ( Hook* hook );
      static  void                          sortHookByX            ( vector<Hook*>&       , uint64_t flags=NoFlags );
      static  void                          sortHookByY            ( vector<Hook*>&       , uint64_t flags=NoFlags );
      static  void                          sortRpByX              ( vector<RoutingPad*>& , uint64_t flags=NoFlags );
      static  void                          sortRpByY              ( vector<RoutingPad*>& , uint64_t flags=NoFlags );
    public:
                                            NetBuilder             ();
      virtual                              ~NetBuilder             ();
              void                          clear                  ();
      inline  bool                          isStrictChannel        () const;
      inline  bool                          isUpperMetalRp         () const;
      inline  AnabaticEngine*               getAnabatic            () const;
      inline  bool                          useVSmallAsOffgrid     () const;
      inline  unsigned int                  getDegree              () const;
      inline  void                          setDegree              ( unsigned int degree );
              void                          fixSegments            ();
              NetBuilder&                   setStartHook           ( AnabaticEngine*
                                                                   , Hook*        fromHook
                                                                   , AutoContact* sourceContact=NULL
                                                                   , uint64_t     sourceFlags=0 );
              void                          construct              ();
      inline  unsigned int                  getStateG              () const;
      inline  UConnexity                    getConnexity           () const;
      inline  UConnexity&                   getConnexity           ();
      inline  Net*                          getNet                 () const;
      inline  NetData*                      getNetData             () const;
      inline  GCell*                        getGCell               () const;
      inline  AutoContact*                  getSourceContact       () const;
      inline  AutoContact*                  getWestContact         () const;
      inline  AutoContact*                  getEastContact         () const;
      inline  AutoContact*                  getNorthContact        () const;
      inline  AutoContact*                  getSouthContact        () const;
      inline  Hook*                         getFromHook            () const;
      inline  uint64_t                      getSourceFlags         () const;
      inline  uint64_t                      getFlags               () const;
      inline  ForkStack&                    getForks               ();
      inline  vector<RoutingPad*>&          getRoutingPads         ();
      inline  map<Component*,AutoSegment*>& getRpLookup            ();
      inline  unsigned int                  getTopology            () const;
      inline  vector<Hook*>&                getNorths              ();
      inline  vector<Hook*>&                getSouths              ();
      inline  Hook*                         north                  ( size_t i=0 ) const;
      inline  Hook*                         south                  ( size_t i=0 ) const;
      inline  Hook*                         east                   ( size_t i=0 ) const;
      inline  Hook*                         west                   ( size_t i=0 ) const;
      inline  void                          addToNorths            ( Hook* );
      inline  void                          addToSouths            ( Hook* );
      inline  void                          addToEasts             ( Hook* );
      inline  void                          addToWests             ( Hook* );
      inline  void                          clearNorths            ();
      inline  void                          clearSouths            ();
      inline  void                          clearEasts             ();
      inline  void                          clearWests             ();
      inline  void                          setFlags               ( uint64_t );
      inline  void                          setFromHook            ( Hook* );
      inline  void                          setEastContact         ( AutoContact* );
      inline  void                          setWestContact         ( AutoContact* );
      inline  void                          setNorthContact        ( AutoContact* );
      inline  void                          setSouthContact        ( AutoContact* );
      inline  void                          setSouthWestContact    ( AutoContact* );
      inline  void                          setNorthEastContact    ( AutoContact* );
      inline  void                          setBothCornerContacts  ( AutoContact* );
      inline  void                          swapCornerContacts     ();
      inline  void                          addToFixSegments       ( AutoSegment* );
              bool                          push                   ( Hook* to, AutoContact* contact, uint64_t flags=0 );
              bool                          isInsideBlockage       ( GCell*, Component* ) const;
      virtual void                          doRp_AutoContacts      ( GCell*, RoutingPad*, AutoContact*& source, AutoContact*& target, uint64_t flags ) = 0;
      virtual AutoContact*                  doRp_Access            ( GCell*, RoutingPad*, uint64_t  flags ) = 0;
      virtual AutoContact*                  doRp_AccessPad         ( RoutingPad*, uint64_t flags );
      virtual AutoContact*                  doRp_AccessAnalog      ( GCell*, RoutingPad*, uint64_t flags );
              void                          doRp_StairCaseH        ( GCell*, RoutingPad* rp1, RoutingPad* rp2 );
              void                          doRp_StairCaseV        ( GCell*, RoutingPad* rp1, RoutingPad* rp2 );
              void                          _load                  ( AnabaticEngine*, Net* );
    private:                                                       
      virtual bool                          _do_xG                 ();
      virtual bool                          _do_2G                 ();
      virtual bool                          _do_xG_1Pad            ();
      virtual bool                          _do_1G_1PinM1          ();
      virtual bool                          _do_2G_1PinM1          ();
      virtual bool                          _do_1G_1PinM2          ();
      virtual bool                          _do_xG_1PinM2          ();
      virtual bool                          _do_1G_1PinM3          ();
      virtual bool                          _do_xG_1PinM3          ();
      virtual bool                          _do_1G_1M1             ();
      virtual bool                          _do_2G_1M1             ();
      virtual bool                          _do_1G_xM1             ();
      virtual bool                          _do_xG_1M1             ();
      virtual bool                          _do_xG_xM1_xM3         ();
      virtual bool                          _do_xG_1M1_1M2         ();
      virtual bool                          _do_4G_1M2             ();
      virtual bool                          _do_xG_xM2             ();
      virtual bool                          _do_1G_1M3             ();
      virtual bool                          _do_xG_xM3             ();
      virtual bool                          _do_xG_1M4             ();
      virtual bool                          _do_1G_1M5             ();
      virtual bool                          _do_2G_1M5             ();
      virtual bool                          _do_1G_xM1_1PinM1      ();
      virtual bool                          _do_1G_xM1_1PinM2      ();
      virtual bool                          _do_2G_xM1_1PinM2      ();
      virtual bool                          _do_1G_1M1_1PinM3      ();
      virtual bool                          _do_2G_xM1_1PinM3      ();
      virtual bool                          _do_3G_xM1_1PinM3      ();
      virtual bool                          _do_globalSegment      ();
      virtual void                          singleGCell            ( AnabaticEngine*, Net* );
              AutoContact*                  _doHChannel            ();
              AutoContact*                  _doVChannel            ();
              AutoContact*                  _doStrut               ();
              AutoContact*                  _doDevice              ();
              AutoContact*                  _doHRail               ();
              AutoContact*                  _doVRail               ();
              void                          _doIoPad               ();
              unsigned int                  getNumberGlobals       ();
              unsigned int                  getDeviceNeighbourBound();
      virtual string                        getTypeName            () const;
    private:

    //                           Connexity Name                   | G|M1|M2|M3|M4|M5|M6|Pad|Pin |
      static constexpr uint64_t  Conn_0G            = connexConfig( 0, 0, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G            = connexConfig( 2, 0, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G            = connexConfig( 3, 0, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_4G            = connexConfig( 4, 0, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_5G            = connexConfig( 5, 0, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_6G            = connexConfig( 6, 0, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_0G_2M1        = connexConfig( 0, 2, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_1M1        = connexConfig( 1, 1, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_2M1        = connexConfig( 1, 2, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_3M1        = connexConfig( 1, 3, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_4M1        = connexConfig( 1, 4, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_5M1        = connexConfig( 1, 5, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_6M1        = connexConfig( 1, 6, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_7M1        = connexConfig( 1, 7, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_8M1        = connexConfig( 1, 8, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_9M1        = connexConfig( 1, 9, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_1M2        = connexConfig( 1, 0, 1, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_2M2        = connexConfig( 1, 0, 2, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_3M2        = connexConfig( 1, 0, 3, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_4M2        = connexConfig( 1, 0, 4, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_1M3        = connexConfig( 1, 0, 0, 1, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_2M3        = connexConfig( 1, 0, 0, 2, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_3M3        = connexConfig( 1, 0, 0, 3, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_4M3        = connexConfig( 1, 0, 0, 4, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_1M4        = connexConfig( 1, 0, 0, 0, 1, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_1M5        = connexConfig( 1, 0, 0, 0, 0, 1, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_1M1_1M2    = connexConfig( 1, 1, 1, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_1M1_1M3    = connexConfig( 1, 1, 0, 1, 0, 0, 0,  0,  0 );
    //                           Connexity Name                   | G|M1|M2|M3|M4|M5|M6|Pad|Pin |
      static constexpr uint64_t  Conn_2G_1M1        = connexConfig( 2, 1, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_2M1        = connexConfig( 2, 2, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_3M1        = connexConfig( 2, 3, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_4M1        = connexConfig( 2, 4, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_5M1        = connexConfig( 2, 5, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_6M1        = connexConfig( 2, 6, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_7M1        = connexConfig( 2, 7, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_8M1        = connexConfig( 2, 8, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_9M1        = connexConfig( 2, 9, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_1M2        = connexConfig( 2, 0, 1, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_2M2        = connexConfig( 2, 0, 2, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_3M2        = connexConfig( 2, 0, 3, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_4M2        = connexConfig( 2, 0, 4, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_1M3        = connexConfig( 2, 0, 0, 1, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_2M3        = connexConfig( 2, 0, 0, 2, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_3M3        = connexConfig( 2, 0, 0, 3, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_4M3        = connexConfig( 2, 0, 0, 4, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_1M4        = connexConfig( 2, 0, 0, 0, 1, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_1M5        = connexConfig( 2, 0, 0, 0, 0, 1, 0,  0,  0 );
      static constexpr uint64_t  Conn_2G_1M1_1M2    = connexConfig( 2, 1, 1, 0, 0, 0, 0,  0,  0 );
    //                           Connexity Name                   | G|M1|M2|M3|M4|M5|M6|Pad|Pin |
      static constexpr uint64_t  Conn_3G_1M1        = connexConfig( 3, 1, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_2M1        = connexConfig( 3, 2, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_3M1        = connexConfig( 3, 3, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_4M1        = connexConfig( 3, 4, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_5M1        = connexConfig( 3, 5, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_6M1        = connexConfig( 3, 6, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_7M1        = connexConfig( 3, 7, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_8M1        = connexConfig( 3, 8, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_9M1        = connexConfig( 3, 9, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_1M2        = connexConfig( 3, 0, 1, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_2M2        = connexConfig( 3, 0, 2, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_1M3        = connexConfig( 3, 0, 0, 1, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_2M3        = connexConfig( 3, 0, 0, 2, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_3M3        = connexConfig( 3, 0, 0, 3, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_4M3        = connexConfig( 3, 0, 0, 4, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_1M4        = connexConfig( 3, 0, 0, 0, 1, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_2M4        = connexConfig( 3, 0, 0, 0, 2, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_3G_3M4        = connexConfig( 3, 0, 0, 0, 3, 0, 0,  0,  0 );
    //                           Connexity Name                   | G|M1|M2|M3|M4|M5|M6|Pad|Pin |
      static constexpr uint64_t  Conn_4G_1M1        = connexConfig( 4, 1, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_4G_2M1        = connexConfig( 4, 2, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_4G_3M1        = connexConfig( 4, 3, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_4G_4M1        = connexConfig( 4, 4, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_4G_5M1        = connexConfig( 4, 5, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_4G_6M1        = connexConfig( 4, 6, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_4G_7M1        = connexConfig( 4, 7, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_4G_8M1        = connexConfig( 4, 8, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_4G_9M1        = connexConfig( 4, 9, 0, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_4G_1M2        = connexConfig( 4, 0, 1, 0, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_4G_1M3        = connexConfig( 4, 0, 0, 1, 0, 0, 0,  0,  0 );
      static constexpr uint64_t  Conn_1G_1Pad       = connexConfig( 1, 0, 0, 0, 0, 0, 0,  1,  0 );
      static constexpr uint64_t  Conn_2G_1Pad       = connexConfig( 2, 0, 0, 0, 0, 0, 0,  1,  0 );
      static constexpr uint64_t  Conn_3G_1Pad       = connexConfig( 3, 0, 0, 0, 0, 0, 0,  1,  0 );
      static constexpr uint64_t  Conn_1G_1PinM1     = connexConfig( 1, 1, 0, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_2G_1PinM1     = connexConfig( 2, 1, 0, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_1PinM2     = connexConfig( 1, 0, 1, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_2G_1PinM2     = connexConfig( 2, 0, 1, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_3G_1PinM2     = connexConfig( 3, 0, 1, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_1M1_1PinM1 = connexConfig( 1, 1, 0, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_2M1_1PinM1 = connexConfig( 1, 2, 0, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_1M1_1PinM2 = connexConfig( 1, 1, 1, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_2M1_1PinM2 = connexConfig( 1, 2, 1, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_3M1_1PinM2 = connexConfig( 1, 3, 1, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_4M1_1PinM2 = connexConfig( 1, 4, 1, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_5M1_1PinM2 = connexConfig( 1, 5, 1, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_2G_1M1_1PinM2 = connexConfig( 2, 1, 1, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_2G_2M1_1PinM2 = connexConfig( 2, 2, 1, 0, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_1PinM3     = connexConfig( 1, 0, 0, 1, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_2G_1PinM3     = connexConfig( 2, 0, 0, 1, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_3G_1PinM3     = connexConfig( 3, 0, 0, 1, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_1M1_1PinM3 = connexConfig( 1, 1, 0, 1, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_2M1_1PinM3 = connexConfig( 1, 2, 0, 1, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_2G_1M1_1PinM3 = connexConfig( 2, 1, 0, 1, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_2G_2M1_1PinM3 = connexConfig( 2, 2, 0, 1, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_2G_3M1_1PinM3 = connexConfig( 2, 3, 0, 1, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_3G_1M1_1PinM3 = connexConfig( 3, 1, 0, 1, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_3G_2M1_1PinM3 = connexConfig( 3, 2, 0, 1, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_3G_3M1_1PinM3 = connexConfig( 3, 3, 0, 1, 0, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_1PinM4     = connexConfig( 1, 0, 0, 0, 1, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_2G_1PinM4     = connexConfig( 2, 0, 0, 0, 1, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_3G_1PinM4     = connexConfig( 3, 0, 0, 0, 1, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_1M1_1PinM4 = connexConfig( 1, 1, 0, 0, 1, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_2M1_1PinM4 = connexConfig( 1, 2, 0, 0, 1, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_3M1_1PinM4 = connexConfig( 1, 3, 0, 0, 1, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_4M1_1PinM4 = connexConfig( 1, 4, 0, 0, 1, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_1G_5M1_1PinM4 = connexConfig( 1, 5, 0, 0, 1, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_2G_1M1_1PinM4 = connexConfig( 2, 1, 0, 0, 1, 0, 0,  0,  1 );
      static constexpr uint64_t  Conn_2G_2M1_1PinM4 = connexConfig( 2, 2, 0, 0, 1, 0, 0,  0,  1 );

    // Attributes.
    private:
             AnabaticEngine*              _anabatic;
             StyleFlags                   _routingStyle;
             ForkStack                    _forks;
             UConnexity                   _connexity;
             unsigned int                 _topology;
             Net*                         _net;
             NetData*                     _netData;
             GCell*                       _gcell;
             AutoContact*                 _sourceContact;
             AutoContact*                 _eastContact;
             AutoContact*                 _westContact;
             AutoContact*                 _northContact;
             AutoContact*                 _southContact;
             Hook*                        _fromHook;
             vector<Hook*>                _easts;
             vector<Hook*>                _wests;
             vector<Hook*>                _norths;
             vector<Hook*>                _souths;
             vector<RoutingPad*>          _routingPads;
             map<Component*,AutoSegment*> _routingPadAutoSegments;
             vector<AutoSegment*>         _toFixSegments;
             unsigned int                 _degree;
             bool                         _isStrictChannel;
             uint64_t                     _sourceFlags;
             uint64_t                     _flags;
  };


  inline bool                          NetBuilder::isStrictChannel        () const { return _isStrictChannel; }
  inline AnabaticEngine*               NetBuilder::getAnabatic            () const { return _anabatic; }
  inline bool                          NetBuilder::useVSmallAsOffgrid     () const { return _routingStyle & StyleFlags::VSmallAsOffgrid; }
  inline unsigned int                  NetBuilder::getDegree              () const { return _degree; }
  inline NetBuilder::UConnexity        NetBuilder::getConnexity           () const { return _connexity; }
  inline NetBuilder::UConnexity&       NetBuilder::getConnexity           ()       { return _connexity; }
  inline ForkStack&                    NetBuilder::getForks               () { return _forks; }
  inline unsigned int                  NetBuilder::getStateG              () const { return _connexity.fields.globals; }
  inline GCell*                        NetBuilder::getGCell               () const { return _gcell; }
  inline Net*                          NetBuilder::getNet                 () const { return _net; }
  inline NetData*                      NetBuilder::getNetData             () const { return _netData; }
  inline AutoContact*                  NetBuilder::getSourceContact       () const { return _sourceContact; }
  inline AutoContact*                  NetBuilder::getEastContact         () const { return _eastContact; }
  inline AutoContact*                  NetBuilder::getWestContact         () const { return _westContact; }
  inline AutoContact*                  NetBuilder::getNorthContact        () const { return _northContact; }
  inline AutoContact*                  NetBuilder::getSouthContact        () const { return _southContact; }
  inline Hook*                         NetBuilder::getFromHook            () const { return _fromHook; }
  inline uint64_t                      NetBuilder::getSourceFlags         () const { return _sourceFlags; }
  inline uint64_t                      NetBuilder::getFlags               () const { return _flags; }
  inline unsigned int                  NetBuilder::getTopology            () const { return _topology; }
  inline vector<RoutingPad*>&          NetBuilder::getRoutingPads         () { return _routingPads; }
  inline map<Component*,AutoSegment*>& NetBuilder::getRpLookup            () { return _routingPadAutoSegments; }
  inline vector<Hook*>&                NetBuilder::getNorths              () { return _norths; }
  inline vector<Hook*>&                NetBuilder::getSouths              () { return _souths; }
  inline Hook*                         NetBuilder::north                  ( size_t i ) const { return (i<_norths.size()) ? _norths[i] : NULL; }
  inline Hook*                         NetBuilder::south                  ( size_t i ) const { return (i<_souths.size()) ? _souths[i] : NULL; }
  inline Hook*                         NetBuilder::east                   ( size_t i ) const { return (i<_easts .size()) ? _easts [i] : NULL; }
  inline Hook*                         NetBuilder::west                   ( size_t i ) const { return (i<_wests .size()) ? _wests [i] : NULL; }
  inline void                          NetBuilder::setFlags               ( uint64_t flags ) { _flags |= flags; }
  inline void                          NetBuilder::setDegree              ( unsigned int degree ) { _degree = degree; }
  inline void                          NetBuilder::setFromHook            ( Hook* hook ) { _fromHook = hook; }
  inline void                          NetBuilder::setBothCornerContacts  ( AutoContact* ac ) { _eastContact = _westContact = _northContact = _southContact = ac; }
  inline void                          NetBuilder::setSouthWestContact    ( AutoContact* ac ) { _westContact = _southContact = ac; }
  inline void                          NetBuilder::setNorthEastContact    ( AutoContact* ac ) { _eastContact = _northContact = ac; }
  inline void                          NetBuilder::setEastContact         ( AutoContact* ac ) { _eastContact = ac; if (not _northContact) _northContact = ac; }
  inline void                          NetBuilder::setWestContact         ( AutoContact* ac ) { _westContact = ac; if (not _southContact) _southContact = ac; }
  inline void                          NetBuilder::setNorthContact        ( AutoContact* ac ) { _northContact = ac; if (not _eastContact) _eastContact = ac; }
  inline void                          NetBuilder::setSouthContact        ( AutoContact* ac ) { _southContact = ac; if (not _westContact) _westContact = ac; }
  inline void                          NetBuilder::swapCornerContacts     () { std::swap( _southContact, _northContact ); std::swap( _westContact, _eastContact );  }
  inline void                          NetBuilder::addToFixSegments       ( AutoSegment* as ) { _toFixSegments.push_back(as); }
  inline void                          NetBuilder::addToNorths            ( Hook* hook ) { _norths.push_back(hook); }
  inline void                          NetBuilder::addToSouths            ( Hook* hook ) { _souths.push_back(hook); }
  inline void                          NetBuilder::addToEasts             ( Hook* hook ) { _easts .push_back(hook); }
  inline void                          NetBuilder::addToWests             ( Hook* hook ) { _wests .push_back(hook); }
  inline void                          NetBuilder::clearNorths            () { _norths.clear(); }
  inline void                          NetBuilder::clearSouths            () { _souths.clear(); }
  inline void                          NetBuilder::clearEasts             () { _easts .clear(); }
  inline void                          NetBuilder::clearWests             () { _wests .clear(); }

  template< typename BuilderT >
  void  NetBuilder::load ( AnabaticEngine* engine, Net* net ) {  BuilderT()._load(engine,net); }

}
