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

#ifndef  ANABATIC_NET_BUILDER_H
#define  ANABATIC_NET_BUILDER_H

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
  

// -------------------------------------------------------------------
// Class  :  "ForkStack".

  class ForkStack {
    public:
      inline void         push       ( Hook* from, AutoContact* contact );
      inline void         pop        ();
      inline Hook*        getFrom    () const;
      inline AutoContact* getContact () const;
    private:
      struct Element {
                 Hook*         _from;
                 AutoContact*  _contact;
          inline               Element ( Hook* from, AutoContact* contact );
      };
    private:
      list<Element>  _stack;
  };


  inline              ForkStack::Element::Element ( Hook* from, AutoContact* contact ) : _from(from), _contact(contact) {}
  inline void         ForkStack::pop              () { if (not _stack.empty()) _stack.pop_back(); }
  inline Hook*        ForkStack::getFrom          ()  const { return _stack.empty() ? NULL : _stack.back()._from; }
  inline AutoContact* ForkStack::getContact       ()  const { return _stack.empty() ? NULL : _stack.back()._contact; }


  inline void  ForkStack::push ( Hook* from, AutoContact* contact )
  {
    cdebug_log(145,0) << "    Stacking " << from << " + " << contact << endl;
    _stack.push_back( Element(from,contact) );
  }


// -------------------------------------------------------------------
// Class  :  "NetBuilder".

  class NetBuilder {
    public:
      enum FunctionFlags { NoFlags         = (1 <<  0)
                         , SortDecreasing  = (1 <<  1)
                         , HAccess         = (1 <<  2)
                         , HAccessEW       = (1 <<  3)
                         , VSmall          = (1 <<  4)
                         , HSmall          = (1 <<  5)
                         , Punctual        = (1 <<  6)
                         , HCollapse       = (1 <<  7)
                         , VCollapse       = (1 <<  8)
                         , Terminal        = (1 <<  9)
                         , DoSourceContact = (1 << 10)
                         , DoTargetContact = (1 << 11)
                         , SouthBound      = (1 << 12)
                         , NorthBound      = (1 << 13)
                         , WestBound       = (1 << 14)
                         , EastBound       = (1 << 15)
                         , Middle          = (1 << 16)
                         , UseNonPref      = (1 << 17)
                         , HBothAccess     = HAccess|HAccessEW
                         , SouthWest       = SouthBound|WestBound
                         , NorthEast       = NorthBound|EastBound
                         };
      enum TopologyFlag  { Global_Vertical_End   = 0x00000001
                         , Global_Horizontal_End = 0x00000002
                         , Global_Horizontal     = 0x00000004
                         , Global_Vertical       = 0x00000008
                         , Global_Turn           = 0x00000010
                         , Global_Fork           = 0x00000020
                         , Global_Fixed          = 0x00000040
                         , Global_End            = Global_Vertical_End | Global_Horizontal_End
                         , Global_Split          = Global_Horizontal | Global_Vertical | Global_Fork
                         };

    // Connexity Union Type.
      enum ConnexityBits { GlobalBSize = 8
                         , Metal1BSize = 4
                         , Metal2BSize = 4
                         , Metal3BSize = 4
                         , PadsBSize   = 4
                         , PinsBSize   = 4
                         };
      union UConnexity {
        uint64_t connexity;
        struct {
          unsigned int globals : GlobalBSize;
          unsigned int M1      : Metal1BSize;
          unsigned int M2      : Metal2BSize;
          unsigned int M3      : Metal3BSize;
          unsigned int Pad     : PadsBSize;
          unsigned int Pin     : PinsBSize;
        } fields;
      };
    public:
      template< typename BuilderT >
      static  void                          load                   ( AnabaticEngine*, Net* );
      static  void                          getPositions           ( Component* anchor, Point& source, Point& target );
      static  uint64_t                      checkRoutingPadSize    ( Component* anchor );
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
      inline  bool                          isTwoMetals            () const;
      inline  AnabaticEngine*               getAnabatic            () const;
      inline  unsigned int                  getDegree              () const;
      inline  void                          setDegree              ( unsigned int degree );
              void                          fixSegments            ();
              NetBuilder&                   setStartHook           ( AnabaticEngine*
                                                                   , Hook*        fromHook
                                                                   , AutoContact* sourceContact=NULL );
              void                          construct              ();
      inline  unsigned int                  getStateG              () const;
      inline  UConnexity                    getConnexity           () const;
      inline  UConnexity&                   getConnexity           ();
      inline  Net*                          getNet                 () const;
      inline  GCell*                        getGCell               () const;
      inline  AutoContact*                  getSourceContact       () const;
      inline  AutoContact*                  getSouthWestContact    () const;
      inline  AutoContact*&                 getSouthWestContact    ();
      inline  AutoContact*                  getNorthEastContact    () const;
      inline  AutoContact*&                 getNorthEastContact    ();
      inline  Hook*                         getFromHook            () const;
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
      inline  void                          setFromHook            ( Hook* );
      inline  void                          setSouthWestContact    ( AutoContact* );
      inline  void                          setNorthEastContact    ( AutoContact* );
      inline  void                          setBothCornerContacts  ( AutoContact* );
      inline  void                          swapCornerContacts     ();
      inline  void                          addToFixSegments       ( AutoSegment* );
              bool                          push                   ( Hook* to, AutoContact* contact, uint64_t flags=0 );
      virtual void                          doRp_AutoContacts      ( GCell*, Component*, AutoContact*& source, AutoContact*& target, uint64_t flags ) = 0;
      virtual AutoContact*                  doRp_Access            ( GCell*, Component*, uint64_t  flags ) = 0;
      virtual AutoContact*                  doRp_AccessPad         ( RoutingPad*, uint64_t flags );
      virtual AutoContact*                  doRp_AccessAnalog      ( GCell*, RoutingPad*, uint64_t flags );
              void                          doRp_StairCaseH        ( GCell*, Component* rp1, Component* rp2 );
              void                          doRp_StairCaseV        ( GCell*, Component* rp1, Component* rp2 );
              void                          _load                  ( AnabaticEngine*, Net* );
    private:                                                       
      virtual bool                          _do_xG                 ();
      virtual bool                          _do_2G                 ();
      virtual bool                          _do_xG_1Pad            ();
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

#define CONNEXITY_VALUE( Gs, M1s, M2s, M3s, pads, pins )                 \
      Gs + ((M1s ) <<  GlobalBSize)                                      \
         + ((M2s ) << (GlobalBSize+Metal1BSize))                         \
         + ((M3s ) << (GlobalBSize+Metal1BSize+Metal2BSize))             \
         + ((pads) << (GlobalBSize+Metal1BSize+Metal2BSize+Metal3BSize)) \
         + ((pins) << (GlobalBSize+Metal1BSize+Metal2BSize+Metal3BSize+PadsBSize))

    //                     Connexity Name                    | G|M1|M2|M3|Pad|Pin|
      enum ConnexityFlag { Conn_0G          = CONNEXITY_VALUE( 0, 0, 0, 0, 0 , 0 )
                         , Conn_2G          = CONNEXITY_VALUE( 2, 0, 0, 0, 0 , 0 )
                         , Conn_3G          = CONNEXITY_VALUE( 3, 0, 0, 0, 0 , 0 )
                         , Conn_4G          = CONNEXITY_VALUE( 4, 0, 0, 0, 0 , 0 )
                         , Conn_5G          = CONNEXITY_VALUE( 5, 0, 0, 0, 0 , 0 )
                         , Conn_6G          = CONNEXITY_VALUE( 6, 0, 0, 0, 0 , 0 )
                         , Conn_0G_2M1      = CONNEXITY_VALUE( 0, 2, 0, 0, 0 , 0 )
                         , Conn_1G_1M1      = CONNEXITY_VALUE( 1, 1, 0, 0, 0 , 0 )
                         , Conn_1G_2M1      = CONNEXITY_VALUE( 1, 2, 0, 0, 0 , 0 )
                         , Conn_1G_3M1      = CONNEXITY_VALUE( 1, 3, 0, 0, 0 , 0 )
                         , Conn_1G_4M1      = CONNEXITY_VALUE( 1, 4, 0, 0, 0 , 0 )
                         , Conn_1G_5M1      = CONNEXITY_VALUE( 1, 5, 0, 0, 0 , 0 )
                         , Conn_1G_1M2      = CONNEXITY_VALUE( 1, 0, 1, 0, 0 , 0 )
                         , Conn_1G_2M2      = CONNEXITY_VALUE( 1, 0, 2, 0, 0 , 0 )
                         , Conn_1G_3M2      = CONNEXITY_VALUE( 1, 0, 3, 0, 0 , 0 )
                         , Conn_1G_4M2      = CONNEXITY_VALUE( 1, 0, 4, 0, 0 , 0 )
                         , Conn_1G_1M3      = CONNEXITY_VALUE( 1, 0, 0, 1, 0 , 0 )
                         , Conn_1G_2M3      = CONNEXITY_VALUE( 1, 0, 0, 2, 0 , 0 )
                         , Conn_1G_3M3      = CONNEXITY_VALUE( 1, 0, 0, 3, 0 , 0 )
                         , Conn_1G_4M3      = CONNEXITY_VALUE( 1, 0, 0, 4, 0 , 0 )
                         , Conn_1G_1M1_1M2  = CONNEXITY_VALUE( 1, 1, 1, 0, 0 , 0 )
                         , Conn_1G_1M1_1M3  = CONNEXITY_VALUE( 1, 1, 0, 1, 0 , 0 )
                         // Connexity Name                   | G|M1|M2|M3|Pad|Pin|
                         , Conn_2G_1M1      = CONNEXITY_VALUE( 2, 1, 0, 0, 0 , 0 )
                         , Conn_2G_2M1      = CONNEXITY_VALUE( 2, 2, 0, 0, 0 , 0 )
                         , Conn_2G_3M1      = CONNEXITY_VALUE( 2, 3, 0, 0, 0 , 0 )
                         , Conn_2G_4M1      = CONNEXITY_VALUE( 2, 4, 0, 0, 0 , 0 )
                         , Conn_2G_5M1      = CONNEXITY_VALUE( 2, 5, 0, 0, 0 , 0 )
                         , Conn_2G_1M2      = CONNEXITY_VALUE( 2, 0, 1, 0, 0 , 0 )
                         , Conn_2G_2M2      = CONNEXITY_VALUE( 2, 0, 2, 0, 0 , 0 )
                         , Conn_2G_3M2      = CONNEXITY_VALUE( 2, 0, 3, 0, 0 , 0 )
                         , Conn_2G_4M2      = CONNEXITY_VALUE( 2, 0, 4, 0, 0 , 0 )
                         , Conn_2G_1M3      = CONNEXITY_VALUE( 2, 0, 0, 1, 0 , 0 )
                         , Conn_2G_2M3      = CONNEXITY_VALUE( 2, 0, 0, 2, 0 , 0 )
                         , Conn_2G_3M3      = CONNEXITY_VALUE( 2, 0, 0, 3, 0 , 0 )
                         , Conn_2G_4M3      = CONNEXITY_VALUE( 2, 0, 0, 4, 0 , 0 )
                         , Conn_2G_1M1_1M2  = CONNEXITY_VALUE( 2, 1, 1, 0, 0 , 0 )
                         // Connexity Name                   | G|M1|M2|M3|Pad|Pin|
                         , Conn_3G_1M1      = CONNEXITY_VALUE( 3, 1, 0, 0, 0 , 0 )
                         , Conn_3G_2M1      = CONNEXITY_VALUE( 3, 2, 0, 0, 0 , 0 )
                         , Conn_3G_3M1      = CONNEXITY_VALUE( 3, 3, 0, 0, 0 , 0 )
                         , Conn_3G_4M1      = CONNEXITY_VALUE( 3, 4, 0, 0, 0 , 0 )
                         , Conn_3G_1M2      = CONNEXITY_VALUE( 3, 0, 1, 0, 0 , 0 )
                         , Conn_3G_2M2      = CONNEXITY_VALUE( 3, 0, 2, 0, 0 , 0 )
                         , Conn_3G_1M3      = CONNEXITY_VALUE( 3, 0, 0, 1, 0 , 0 )
                         , Conn_3G_2M3      = CONNEXITY_VALUE( 3, 0, 0, 2, 0 , 0 )
                         , Conn_3G_3M3      = CONNEXITY_VALUE( 3, 0, 0, 3, 0 , 0 )
                         , Conn_3G_4M3      = CONNEXITY_VALUE( 3, 0, 0, 4, 0 , 0 )
                         // Connexity Name                   | G|M1|M2|M3|Pad|Pin|
                         , Conn_4G_1M1      = CONNEXITY_VALUE( 4, 1, 0, 0, 0 , 0 )
                         , Conn_4G_2M1      = CONNEXITY_VALUE( 4, 2, 0, 0, 0 , 0 )
                         , Conn_4G_3M1      = CONNEXITY_VALUE( 4, 3, 0, 0, 0 , 0 )
                         , Conn_4G_4M1      = CONNEXITY_VALUE( 4, 4, 0, 0, 0 , 0 )
                         , Conn_4G_1M2      = CONNEXITY_VALUE( 4, 0, 1, 0, 0 , 0 )
                         , Conn_4G_1M3      = CONNEXITY_VALUE( 4, 0, 0, 1, 0 , 0 )
                         , Conn_1G_1Pad     = CONNEXITY_VALUE( 1, 0, 0, 0, 1 , 0 )
                         , Conn_2G_1Pad     = CONNEXITY_VALUE( 2, 0, 0, 0, 1 , 0 )
                         , Conn_3G_1Pad     = CONNEXITY_VALUE( 3, 0, 0, 0, 1 , 0 )
                         , Conn_1G_1PinM2   = CONNEXITY_VALUE( 1, 0, 1, 0, 0 , 1 )
                         , Conn_2G_1PinM2   = CONNEXITY_VALUE( 2, 0, 1, 0, 0 , 1 )
                         , Conn_3G_1PinM2   = CONNEXITY_VALUE( 3, 0, 1, 0, 0 , 1 )
                         , Conn_1G_1PinM3   = CONNEXITY_VALUE( 1, 0, 0, 1, 0 , 1 )
                         , Conn_2G_1PinM3   = CONNEXITY_VALUE( 2, 0, 0, 1, 0 , 1 )
                         , Conn_3G_1PinM3   = CONNEXITY_VALUE( 3, 0, 0, 1, 0 , 1 )
                         };

#undef CONNEXITY_VALUE

    // Attributes.
    private:
             AnabaticEngine*              _anabatic;
             ForkStack                    _forks;
             UConnexity                   _connexity;
             unsigned int                 _topology;
             Net*                         _net;
             GCell*                       _gcell;
             AutoContact*                 _sourceContact;
             AutoContact*                 _southWestContact;
             AutoContact*                 _northEastContact;
             Hook*                        _fromHook;
             vector<Hook*>                _easts;
             vector<Hook*>                _wests;
             vector<Hook*>                _norths;
             vector<Hook*>                _souths;
             vector<RoutingPad*>          _routingPads;
             map<Component*,AutoSegment*> _routingPadAutoSegments;
             vector<AutoSegment*>         _toFixSegments;
             unsigned int                 _degree;
             bool                         _isTwoMetals;

    // Sort classes.
    public:
  };


  inline bool                          NetBuilder::isTwoMetals            () const { return _isTwoMetals; }
  inline AnabaticEngine*               NetBuilder::getAnabatic            () const { return _anabatic; }
  inline unsigned int                  NetBuilder::getDegree              () const { return _degree; }
  inline NetBuilder::UConnexity        NetBuilder::getConnexity           () const { return _connexity; }
  inline NetBuilder::UConnexity&       NetBuilder::getConnexity           ()       { return _connexity; }
  inline ForkStack&                    NetBuilder::getForks               () { return _forks; }
  inline unsigned int                  NetBuilder::getStateG              () const { return _connexity.fields.globals; }
  inline GCell*                        NetBuilder::getGCell               () const { return _gcell; }
  inline Net*                          NetBuilder::getNet                 () const { return _net; }
  inline AutoContact*                  NetBuilder::getSourceContact       () const { return _sourceContact; }
  inline AutoContact*                  NetBuilder::getSouthWestContact    () const { return _southWestContact; }
  inline AutoContact*&                 NetBuilder::getSouthWestContact    ()       { return _southWestContact; }
  inline AutoContact*                  NetBuilder::getNorthEastContact    () const { return _northEastContact; }
  inline AutoContact*&                 NetBuilder::getNorthEastContact    ()       { return _northEastContact; }
  inline Hook*                         NetBuilder::getFromHook            () const { return _fromHook; }
  inline unsigned int                  NetBuilder::getTopology            () const { return _topology; }
  inline vector<RoutingPad*>&          NetBuilder::getRoutingPads         () { return _routingPads; }
  inline map<Component*,AutoSegment*>& NetBuilder::getRpLookup            () { return _routingPadAutoSegments; }
  inline vector<Hook*>&                NetBuilder::getNorths              () { return _norths; }
  inline vector<Hook*>&                NetBuilder::getSouths              () { return _souths; }
  inline Hook*                         NetBuilder::north                  ( size_t i ) const { return (i<_norths.size()) ? _norths[i] : NULL; }
  inline Hook*                         NetBuilder::south                  ( size_t i ) const { return (i<_souths.size()) ? _souths[i] : NULL; }
  inline Hook*                         NetBuilder::east                   ( size_t i ) const { return (i<_easts .size()) ? _easts [i] : NULL; }
  inline Hook*                         NetBuilder::west                   ( size_t i ) const { return (i<_wests .size()) ? _wests [i] : NULL; }
  inline void                          NetBuilder::setDegree              ( unsigned int degree ) { _degree = degree; }
  inline void                          NetBuilder::setFromHook            ( Hook* hook ) { _fromHook = hook; }
  inline void                          NetBuilder::setBothCornerContacts  ( AutoContact* ac ) { _southWestContact = _northEastContact = ac; }
  inline void                          NetBuilder::setSouthWestContact    ( AutoContact* ac ) { _southWestContact = ac; }
  inline void                          NetBuilder::setNorthEastContact    ( AutoContact* ac ) { _northEastContact = ac; }
  inline void                          NetBuilder::swapCornerContacts     () { std::swap( _southWestContact, _northEastContact ); }
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

#endif  // ANABATIC_NET_BUILDER_H
