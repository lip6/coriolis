// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./LoadGrByNet.cpp"                        |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DebugSession.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/RoutingPads.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/Measures.h"
#include "katabatic/AutoContactTerminal.h"
#include "katabatic/AutoContactTurn.h"
#include "katabatic/AutoContactHTee.h"
#include "katabatic/AutoContactVTee.h"
#include "katabatic/AutoSegment.h"
#include "katabatic/GCellGrid.h"
#include "katabatic/KatabaticEngine.h"


namespace {

  using  Katabatic::AutoContactTerminal;


/*! \defgroup     LoadGlobalRouting  Global Routing Loading
 *  \brief        Translation rules to build detailed routing from global
 *
 *                This module documents how the global routing built by \c Knik is
 *                loaded into the \c Katabatic data-base. It is intented for developpers
 *                only.
 */

//! \addtogroup   LoadGlobalRouting
//! \{

//! \enum         LocalFunctionFlag
//!               A set of flags for all functions of the LoadGrByNet module.
//!               They can be combined to form the \e flags argument of functions.
//!               the functions will ignore flags that are not intended to them.
//!
//!               For \c HSmall, \c VSmall & \c Punctual see checkRoutingPadSize().

//! \var          LocalFunctionFlag::NoFlags
//!               A simple alias over zero to explicitly tell that no flag at all is
//!               passed to the function.

//! \var          LocalFunctionFlag::HAccess
//!               The constructed topology will be accessed through an horizontal
//!               segment. The absence of this flag tell that the access will be done
//!               trough a vertical.

//! \var          LocalFunctionFlag::VSmall
//!               The RoutingPad vertically covers a very small number of access points,
//!               so it is likely overconstrained for direct horizontal connexion.

//! \var          LocalFunctionFlag::HSmall
//!               The RoutingPad horizontally covers a very small number of access points,
//!               so it is likely overconstrained for direct vertical connexion.

//! \var          LocalFunctionFlag::Punctual
//!               The RoutingPad covers only an access point in either direction.

//! \var          LocalFunctionFlag::DoSourceContact
//!               When creating Katabatic::AutoContactTerminal on non-punctual RoutingPad, this flag
//!               request the creation of a contact <em>on the source point</em>.

//! \var          LocalFunctionFlag::DoTargetContact
//!               When creating Katabatic::AutoContactTerminal on non-punctual RoutingPad, this flag
//!               request the creation of a contact <em>on the target point</em>.


//! \function     unsigned int  checkRoutingPadSize ( Component* rp );
//!
//!               Look at the geometrical size of the Component and assess if
//!               it's span is too narrow either horizontally or vertically.
//!               Return a combination of flags indicating it's state:
//!                 - HSmall : less than 3 pitches in horizontal direction.
//!                 - VSmall : less than 3 pitches in vertical direction.
//!                 - Punctual : one pitch in either directions.
//!
//!               The component can be a RoutingPad, a Vertical or an Horizontal.
//!
//!               \image html  checkRoutingPadSize.png "checkRoutingPadSize()"

/*! \class        GCellTopology
 *
 *  \brief        Build the wiring for a Net inside a GCell (\b internal).
 *
 *                As this class is called to initially construct the Katabatic wiring,
 *                it must build a \b connex wiring. That is without gaps in layer depth,
 *                because the topology restauration mechanism (AutoContact::updateTopology())
 *                of the AutoContact cannot work until all AutoSegments are revalidated at
 *                least once. The topology restauration work by creating doglegs which in turn,
 *                call the canonization, which needs all the caches to be up to date.
 */

//! \function     void  GCellTopology::doRp_AutoContacts ( GCell* gcell, Component* rp, AutoContact*& source, AutoContact*& target, unsigned int flags );
//! \param        gcell   The GCell into which create the AutoContact.
//! \param        rp      The Component we want to access.
//! \param        source  The AutoContact created on the \c source (\e returned).
//! \param        target  The AutoContact created on the \c target (\e returned).
//! \param        flags   Managed by this function:
//!                         - LocalFunctionFlag::DoSourceContact
//!                         - LocalFunctionFlag::DoTargetContact
//!
//!               Create the AutoContact directly anchored on the Component (terminal).
//!               Three cases are manageds:
//!                 -# <b>Ordinary (non-punctual) \c METAL1 terminal</b>: an AutoContactTerminal
//!                    is anchored on the RoutingPad.
//!                 -# <b>Punctual \c METAL1 terminal</b>, the access must never be blocked
//!                    by other routing. To ensure it, we create a fixed AutoSegment (anchored
//!                    on two AutoContactTerminal) to cover it. The \e normal AutoContactTerminal
//!                    is also created.
//!                 -# <b>non \c METAL1 terminal</b>, as for the punctual \c METAL1, a
//!                    fixed protection is added over the RoutingPad. If we access
//!                    horizontally a vertical RoutingPad or vertically an horizontal
//!                    one, an extra AutoContactTerminal is added (to allow is displacement
//!                    along the RoutingPad).
//!
//!               To avoid creating a fixed protection over a RoutingPad multiple times,
//!               the RoutingPad and it's associated protection is stored in a static
//!               \c map : \c __routingPadAutoSegments.
//!
//!               Conversely, because an AutoContactTerminal can only be connected to one
//!               segment, each time this function is called a new terminal will be created
//!               (or maybe two in case of non-punctual terminals). If only one AutoContact
//!               is requested, it is created centered on the RoutingPad. The initial
//!               position of AutoContact <em>do not prevent them to move afterwards</em>,
//!               even those created on source/target on a non-punctual RoutingPad.
//!
//! \remark       For clarity we describe the layer management of this function in term
//!               of \c METAL, but it is the RoutingGauge depth which is actually used.
//!
//!               \image html  doRp_AutoContacts.png "doRp_AutoContacts()"

//! \function     AutoContact* GCellTopology::doRp_Access ( GCell* gcell, Component* rp, unsigned int flags );
//! \param        gcell   The GCell into which create the AutoContact.
//! \param        rp      The Component onto which anchor the access contact.
//! \param        flags   Relevant flags are:
//!                         - HAccess, the terminal is to be accessed through an horizontal
//!                           segment.
//!                         - VSmall, force the terminal to be considered as small in the
//!                           vertical direction.
//!
//!               If \c HAccess is set, the Component is to be accessed trough an horizontal
//!               segment. If unset, the access is done vertically.
//!
//!               Create an AutoContact to access a Component (terminal). If the Component
//!               is not to be accessed through an horizontal segment, and do not cover a
//!               large span in the horizontal direction (flag \c VSmall), a local horizontal
//!               AutoSegment is added to slacken the vertical constraints.
//!
//!               \image html  doRp_Access.png "doRp_Access()"

//! \function     AutoContact*  GCellTopology::doRp_AccessPad ( RoutingPad* rp, unsigned int flags );
//! \param        rp      The Component onto which anchor the access contact.
//! \param        flags   Relevant flags are:
//!                         - HAccess, the terminal is to be accessed through an horizontal
//!                           segment.
//!                         - VSmall, force the terminal to be considered as small in the
//!                           vertical direction.
//! \return       A Katabatic::AutoContactTerminal .
//!
//!               The Component \c rp is a RoutingPad which belongs to a pad cell. This case
//!               occurs when we are routing a complete chip. This method build, from the
//!               \c rp a stack of articulated punctual segments and contacts to reach the
//!               default H/V routing layers (usually \c METAL2 & \c METAL3). This may be
//!               needed when the pad terminal is in \c METAL5, for instance.
//!
//!               The returned AutoContactTerminal is anchored on the last punctual segment
//!               build.
//!
//!               The GCell into which the AutoContactTerminal is created may be under the
//!               pads area. However, it will be right on the border of the GCell.
//!               The global router vertexes of GCell under the pad area are marked as
//!               blocked so will never be used for routing.
//!
//! \remark       The segments and contacts added to ensure the layer connexity are not
//!               put into the Katabatic database. They are plain Hurricane objects, invisibles
//!               from it.

//! \function     void  GCellTopology::doRp_StairCaseH ( GCell* gcell, Component* rp1, Component* rp2 );
//!
//!               Build the wiring to connect to horizontal Component. Two cases:
//!                 - The Component are aligneds, then only a straight wire is created.
//!                 - They are \e not aligned, then a complete dogleg is created.
//!
//!               \image html  doRp_StairCaseH.png "doRp_StairCaseH()"

//! \function     void  GCellTopology::doRp_StairCaseV ( GCell* gcell, Component* rp1, Component* rp2 );
//!
//!               Build the wiring to connect to vertical Components. Two cases:
//!                 - The Components are aligneds, then only a straight wire is created.
//!                 - They are \e not aligned, then a complete dogleg is created.
//!
//!               \image html  doRp_StairCaseV.png "doRp_StairCaseV()"

//! \function     void  GCellTopology::_do_xG_1Pad ();
//!
//!               Construct the topology, when there is only global wires and one local
//!               terminal, but coming from a Pad. As thoses connectors will always be
//!               on one border of the GCell they can be considered as a kind of global.
//!
//!               So this method mostly calls GCellTopology::doRp_AccessPad() to create
//!               the AutoContactTerminal, then calls GCellTopology::_do_xG(), except
//!               for straight lines which are managed directly.

//! \function     void  GCellTopology::_do_xG ();
//!
//!               Construct the topology, when there is only global wires (no local terminals).
//!               
//!               Some topology are not handled because they must not be managed by this
//!               function:
//!               <ul>
//!                 <li>One global: nonsensical because there also must be a terminal.
//!                 <li>Two aligned globals: in that case we do a straight wire whithout
//!                     any AutoContact (handled by the source/target of the wire).
//!               </ul>
//!
//!               \image html  _do_xG.png "_do_xG()"

//! \function     void  GCellTopology::_do_1G_1M1 ();
//!
//!               Construct a topology where there is \e one global and one RoutingPad
//!               in \c METAL1. The \c METAL1 is assumed to be vertical.
//!
//! \remark       When accessing the RoutingPad through an horizontal global segment
//!               and the vertical extension of the segment is small, the global is
//!               still directly attached to the terminal, inducing a high constraint
//!               on it. We left to job of slackening it to the router.
//!
//!               \image html  _do_1G_1M1.png "_do_1G_1M1()"

//! \function     void  GCellTopology::_do_1G_xM1 ();
//!
//!               Construct a topology where there is \e one global and any number of
//!               RoutingPad in \c METAL1. The \c METAL1 is assumed to be vertical.
//!
//!               The RoutingPads are linked together two by two. If the horizontal
//!               segments are not aligned by the router, part of the routage will be
//!               done through the RoutingPad itself. The global incoming segment will
//!               connected to the leftmost, rightmost or centermost RoutingPad according
//!               from wich side it comes from.
//!
//!               \image html  _do_1G_xM1.png "_do_1G_xM1()"

//! \function     void  GCellTopology::_do_xG_1M1_1M2 ();
//!
//!               Construct a topology where there is at least one global (and up to 4),
//!               one \c METAL1 RoutingPad (assumed V) and one \c METAL2 RoutingPad (assumed H).
//!
//!               In this topology, we want to try to reuse the \c METAL2 RoutingPad as a
//!               feedtrough in the horizontal routage. Thus:
//!                 - The \c METAL1 and \c METAL2 RoutingPad are connected through a separate wiring.
//!                 - The south & west global wiring is attached to the leftmost contact of
//!                   the \c METAL2.
//!                 - The north & east global wiring is attached to the rightmost contact of
//!                   the \c METAL2.
//!
//!               South/west and north/south can be build independantly. Depending on the number
//!               of globals, they can consist of:
//!                 - Nothing (no south nor west).
//!                 - An AutoContact (west present).
//!                 - An horizontal plus a turn (south present).
//!                 - An horizontal plus a HTee (south & west present).
//!
//! \remark       Not all configurations are represented below.
//!
//!               \image html  _do_xG_1M1_1M2.png "_do_xG_1M1_1M2()"

//! \function     void  GCellTopology::_do_xG_xM1_xM3 ();
//!
//!               Construct a topology where there is at least one global (and up to 4),
//!               at least one \c METAL1 RoutingPad (assumed V) and at least one \c METAL3
//!               RoutingPad (assumed V).
//!
//!               In this topology, we want to try to reuse the \c METAL3 RoutingPad as a
//!               feedtrough in the vertical routage. Thus:
//!                 - The \c METAL1 and \c METAL3 RoutingPad are connected through a separate
//!                   wiring made of separate horizontals.
//!                 - The south-west global wiring is attached to the leftmost RoutingPad
//!                   if there isn't south or to the first \c METAL3 otherwise.
//!                 - The north-east global wiring is attached to the rightmost RoutingPad
//!                   if there isn't north or to the first \c METAL3 otherwise.
//!
//!               South/west and north/south can be build independantly. Depending on the number
//!               of globals, they can consist of:
//!                 - Nothing (no south nor west).
//!                 - An AutoContact on the leftmost RoutingPad (west present).
//!                 - An AutoContact on the first \c METAL3 (only south present).
//!                 - An AutoContact plus a vertical plus a VTee (south & west present).
//!
//!               \image html  _do_xG_xM1_xM3.png "_do_xG_xM1_xM3()"

//! \function     void  GCellTopology::_do_xG_xM2 ();
//!
//!               Construct a topology where there is at least one global (and up to 4),
//!               and any number of \c METAL2 RoutingPads (assumeds H).
//!
//!               In this topology, we want to try to reuse the \c METAL2 RoutingPad as a
//!               feedtrough in the horizontal routage. Thus:
//!                 - The RoutingPad are connecteds trough a separate staircase (or
//!                   straight wire if aligneds).
//!                 - The south-west global wiring is attached to the leftmost RoutingPad
//!                   if there isn't south or to the biggest horizontal RoutingPad otherwise.
//!                 - The north-east global wiring is attached to the rightmost RoutingPad
//!                   if there isn't south or to the biggest horizontal RoutingPad otherwise.
//!
//!               \image html  _do_xG_xM2.png "_do_xG_xM2()"

//! \function     void  GCellTopology::_do_1G_1M3 ();
//!
//!               Construct a topology where there is one global and one \c METAL3 RoutingPad
//!               (assumeds V).
//!
//!               In this topology, we reuse the \c METAL3 RoutingPad as a feedtrough in the
//!               vertical routage. Thus:
//!                 - If the global is either north or south, we directly connect to the
//!                   north end or south end of the RoutingPad. \red{The vertical global will}
//!                   \red{have no slack at all we assume that METAL3 terminals are only from}
//!                   \red{blocks and are aligneds vertically.}
//!                 - If the global is east or west \e and the RoutingPad is sufficiently
//!                   extended in the vertical direction, we connect an horizontal in the
//!                   normal way.
//!                 - If the global is not sufficiently extended, we add a turn to give some
//!                   slack to the global.
//!
//!
//!               \image html  _do_1G_1M3.png "_do_1G_1M3()"

//! \function     void  GCellTopology::_do_xG_xM3 ();
//!
//!               Construct a topology where there at least one global and two \c METAL3 RoutingPad
//!               (assumed V).
//!
//!               In this topology, we reuse the \c METAL3 RoutingPad as a feedtrough in the
//!               vertical routage. \red{We assume that the most likely relative position}
//!               \red{of the RoutingPads is to be aligned vertically}.
//!               Thus:
//!                 - All RoutingPads are linked two by two trough vertical staircases.
//!                 - The south-west global wiring is attached to the bottommost RoutingPad
//!                   (without vertical slack). If a misalignment is detected, then a
//!                   dogleg is added.
//!                 - The north-east global wiring is attached to the topmost RoutingPad
//!                   (without vertical slack).
//!
//!               South/west and north/south can be build independantly. Depending on the number
//!               of globals, they can consist of:
//!                 - Nothing (no south nor west).
//!                 - An sliding AutoContact on the bottommost RoutingPad (west present).
//!                 - An fixed AutoContact on the bottommost RoutingPad (only south present).
//!                 - An fixed AutoContact plus a vertical plus a VTee (south & west present).
//!
//!               \image html  _do_xG_xM3.png "_do_xG_xM3()"

//! \function     void  singleGCell ( KatabaticEngine* ktbt, Net* net );
//!
//!               All the RoutingPads of the net are concentrated under a single
//!               GCell. This function assumes that all the terminals are in
//!               \c METAL1 (vertical), and link them two by two by horizontal
//!               wires.

//! \}


  using namespace std;
  using namespace CRL;
  using namespace Hurricane;
  using namespace Katabatic;


  // ---------------------------------------------------------------
  // Local Enum/Types.

  enum LocalFunctionFlag { NoFlags         = 0x00000000
                         , SortDecreasing  = 0x00000001
                         , HAccess         = 0x00000002
                         , VSmall          = 0x00000004
                         , HSmall          = 0x00000008
                         , Punctual        = 0x00000010
                         , HCollapse       = 0x00000020
                         , VCollapse       = 0x00000040
                         , Terminal        = 0x00000080
                         , DoSourceContact = 0x00000100
                         , DoTargetContact = 0x00000200
                         , SouthBound      = 0x00010000
                         , NorthBound      = 0x00020000
                         , WestBound       = 0x00040000
                         , EastBound       = 0x00080000
                         };


  // ---------------------------------------------------------------
  // Local Variables.

  const char* invalidGCell =
    "Katabatic::GCellTopology () :\n\n"
    "    No GCell under point.\n";

  const char* mismatchGCell =
    "Katabatic::GCellTopology () :\n\n"
    "    Contacts under two different GCells.\n";

  const char* missingGCell =
    "Katabatic::GCellTopology () :\n\n"
    "    No Contact in GCell.\n";


  map<Component*,AutoSegment*> __routingPadAutoSegments;


  // ---------------------------------------------------------------
  // LoadGrByNet Local Classes.


  struct NetCompareByName {
      inline bool operator() ( const Net* lhs, const Net* rhs ) const;
  };

  inline bool NetCompareByName::operator() ( const Net* lhs, const Net* rhs ) const
  { return lhs->getName() < rhs->getName(); }


  // ---------------------------------------------------------------
  // LoadGrByNet Local Functions.


  void  lookupClear ()
  { __routingPadAutoSegments.clear (); }


  void  getPositions ( Component* anchor, Point& source, Point& target )
  {
    Segment* segment = dynamic_cast<Segment*>( anchor );
    if (segment) {
      source = segment->getSourcePosition();
      target = segment->getTargetPosition();
      return;
    }

    RoutingPad* rp = dynamic_cast<RoutingPad*>( anchor );
    if (rp) {
      source = rp->getSourcePosition();
      target = rp->getTargetPosition();
      return;
    }

    source = anchor->getPosition();
    target = anchor->getPosition();
  }


  unsigned int  checkRoutingPadSize ( Component* anchor )
  {
    Point  source;
    Point  target;

    size_t anchorDepth = Session::getLayerDepth( anchor->getLayer() );
    if (anchorDepth == 0) ++anchorDepth;

    getPositions( anchor, source, target );

    DbU::Unit width  = abs( target.getX() - source.getX() );
    DbU::Unit height = abs( target.getY() - source.getY() );

    unsigned int flags = 0;
    flags |= (width  < 3*Session::getPitch(anchorDepth)) ? HSmall   : 0;
    flags |= (height < 3*Session::getPitch(anchorDepth)) ? VSmall   : 0;
    flags |= ((width == 0) && (height == 0))             ? Punctual : 0;

    ltrace(99) << "::checkRoutingPadSize(): pitch[" << anchorDepth << "]:"
               << DbU::toLambda(Session::getPitch(anchorDepth)) << " "
               << ((flags & HSmall) ? "HSmall " : " ")
               << ((flags & VSmall) ? "VSmall " : " ")
               << endl;

    return flags;
  }


  Hook* getSegmentOppositeHook ( Hook* hook )
  {
    Segment* segment = static_cast<Segment*>( hook->getComponent() );
    return segment->getOppositeHook( hook );
  }


  unsigned int  getSegmentHookType ( Hook* hook )
  {
    Horizontal* horizontal = dynamic_cast<Horizontal*>( hook->getComponent() );
    if (horizontal) {
      if (horizontal->getSourceX() > horizontal->getTargetX())
        cerr << Warning("Bad orientation of %s",getString(horizontal).c_str()) << endl;

      if (dynamic_cast<Segment::SourceHook*>(hook))
        return EastBound;
      return WestBound;
    } else {
      Vertical* vertical = dynamic_cast<Vertical*>( hook->getComponent() );
      if (vertical) {
        if (vertical->getSourceY() > vertical->getTargetY())
          cerr << Warning("Bad orientation of %s",getString(vertical).c_str()) << endl;

        if (dynamic_cast<Segment::SourceHook*>(hook))
          return NorthBound;
      } else {
        cerr << Warning("Unmanaged Hook %s",getString(hook).c_str()) << endl;
      }
    }
    return SouthBound;
  }


  // ---------------------------------------------------------------
  // Class :  "SortRpByX".

  class SortRpByX {
    public:
      inline       SortRpByX  ( unsigned int flags );
      inline bool  operator() ( Component* rp1, Component* rp2 );
    protected:
      unsigned int  _flags;
  };


  inline  SortRpByX::SortRpByX ( unsigned int  flags )
    : _flags(flags)
  { }


  inline bool  SortRpByX::operator() ( Component* rp1, Component* rp2 )
  {
    DbU::Unit x1 = rp1->getCenter().getX();
    DbU::Unit x2 = rp2->getCenter().getX();

    if (x1 == x2) return false;
    return (_flags & SortDecreasing) xor (x1 < x2);
  }


  // ---------------------------------------------------------------
  // Class  :  "SortRpByY".

  class SortRpByY {
    public:
      inline       SortRpByY  ( unsigned int flags );
      inline bool  operator() ( Component* rp1, Component* rp2 );
    protected:
      unsigned int  _flags;
  };


  inline  SortRpByY::SortRpByY  ( unsigned int flags )
    : _flags(flags)
  { }


  inline bool  SortRpByY::operator() ( Component* rp1, Component* rp2 )
  {
    DbU::Unit y1 = rp1->getCenter().getY();
    DbU::Unit y2 = rp2->getCenter().getY();

    if (y1 == y2) return false;
    return (_flags & SortDecreasing) xor (y1 < y2);
  }


  // ---------------------------------------------------------------
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
    ltrace(80) << "    Stacking " << from << " + " << contact << endl;
    _stack.push_back( Element(from,contact) );
  }


  // ---------------------------------------------------------------
  // Class  :  "GGellTopology".

  class GCellTopology {

    public:
      static void          init              ( unsigned int degree );
      static void          fixSegments       ();
                           GCellTopology     ( GCellGrid*, Hook* fromHook, AutoContact* sourceContact=NULL );
             void          construct         ( ForkStack& forks );
      inline unsigned int  getStateG         () const;
      inline GCell*        getGCell          () const;
      static void          doRp_AutoContacts ( GCell*, Component*, AutoContact*& source, AutoContact*& target, unsigned int flags );
      static AutoContact*  doRp_Access       ( GCell*, Component*, unsigned int  flags );
      static AutoContact*  doRp_AccessPad    ( RoutingPad*, unsigned int flags );
      static void          doRp_StairCaseH   ( GCell*, Component* rp1, Component* rp2 );
      static void          doRp_StairCaseV   ( GCell*, Component* rp1, Component* rp2 );
    private:                                    
             void          _do_xG            ();
             void          _do_xG_1Pad       ();
             void          _do_1G_1PinM2     ();
             void          _do_1G_1M1        ();
             void          _do_1G_xM1        ();
             void          _do_xG_xM1_xM3    ();
             void          _do_xG_1M1_1M2    ();
             void          _do_xG_xM2        ();
             void          _do_1G_1M3        ();
             void          _do_xG_xM3        ();

    private:
      enum ConnexityBits { GlobalBSize = 3
                         , Metal1BSize = 3
                         , Metal2BSize = 3
                         , Metal3BSize = 3
                         , PadsBSize   = 3
                         , PinsBSize   = 3
                         };

#define CONNEXITY_VALUE( Gs, M1s, M2s, M3s, pads, pins )                 \
      Gs + ((M1s ) <<  GlobalBSize)                                      \
         + ((M2s ) << (GlobalBSize+Metal1BSize))                         \
         + ((M3s ) << (GlobalBSize+Metal1BSize+Metal2BSize))             \
         + ((pads) << (GlobalBSize+Metal1BSize+Metal2BSize+Metal3BSize)) \
         + ((pins) << (GlobalBSize+Metal1BSize+Metal2BSize+Metal3BSize+PadsBSize))

    //                     Connexity Name                   | G|M1|M2|M2|Pad|Pin|
      enum ConnexityFlag { Conn_0G          = CONNEXITY_VALUE( 0, 0, 0, 0, 0 , 0 )
                         , Conn_2G          = CONNEXITY_VALUE( 2, 0, 0, 0, 0 , 0 )
                         , Conn_3G          = CONNEXITY_VALUE( 3, 0, 0, 0, 0 , 0 )
                         , Conn_4G          = CONNEXITY_VALUE( 4, 0, 0, 0, 0 , 0 )
                         , Conn_0G_2M1      = CONNEXITY_VALUE( 0, 2, 0, 0, 0 , 0 )
                         , Conn_1G_1M1      = CONNEXITY_VALUE( 1, 1, 0, 0, 0 , 0 )
                         , Conn_1G_2M1      = CONNEXITY_VALUE( 1, 2, 0, 0, 0 , 0 )
                         , Conn_1G_3M1      = CONNEXITY_VALUE( 1, 3, 0, 0, 0 , 0 )
                         , Conn_1G_4M1      = CONNEXITY_VALUE( 1, 4, 0, 0, 0 , 0 )
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
                         // Connexity Name                   | G|M1|M2|M2|Pad|Pin|
                         , Conn_2G_1M1      = CONNEXITY_VALUE( 2, 1, 0, 0, 0 , 0 )
                         , Conn_2G_2M1      = CONNEXITY_VALUE( 2, 2, 0, 0, 0 , 0 )
                         , Conn_2G_3M1      = CONNEXITY_VALUE( 2, 3, 0, 0, 0 , 0 )
                         , Conn_2G_4M1      = CONNEXITY_VALUE( 2, 4, 0, 0, 0 , 0 )
                         , Conn_2G_1M2      = CONNEXITY_VALUE( 2, 0, 1, 0, 0 , 0 )
                         , Conn_2G_2M2      = CONNEXITY_VALUE( 2, 0, 2, 0, 0 , 0 )
                         , Conn_2G_3M2      = CONNEXITY_VALUE( 2, 0, 3, 0, 0 , 0 )
                         , Conn_2G_4M2      = CONNEXITY_VALUE( 2, 0, 4, 0, 0 , 0 )
                         , Conn_2G_1M3      = CONNEXITY_VALUE( 2, 0, 0, 1, 0 , 0 )
                         , Conn_2G_2M3      = CONNEXITY_VALUE( 2, 0, 0, 2, 0 , 0 )
                         , Conn_2G_3M3      = CONNEXITY_VALUE( 2, 0, 0, 3, 0 , 0 )
                         , Conn_2G_4M3      = CONNEXITY_VALUE( 2, 0, 0, 4, 0 , 0 )
                         , Conn_2G_1M1_1M2  = CONNEXITY_VALUE( 2, 1, 1, 0, 0 , 0 )
                         // Connexity Name                   | G|M1|M2|M2|Pad|Pin|
                         , Conn_3G_1M1      = CONNEXITY_VALUE( 3, 1, 0, 0, 0 , 0 )
                         , Conn_3G_2M1      = CONNEXITY_VALUE( 3, 2, 0, 0, 0 , 0 )
                         , Conn_3G_3M1      = CONNEXITY_VALUE( 3, 3, 0, 0, 0 , 0 )
                         , Conn_3G_4M1      = CONNEXITY_VALUE( 3, 4, 0, 0, 0 , 0 )
                         , Conn_3G_1M2      = CONNEXITY_VALUE( 3, 0, 1, 0, 0 , 0 )
                         , Conn_3G_1M3      = CONNEXITY_VALUE( 3, 0, 0, 1, 0 , 0 )
                         , Conn_3G_2M3      = CONNEXITY_VALUE( 3, 0, 0, 2, 0 , 0 )
                         , Conn_3G_3M3      = CONNEXITY_VALUE( 3, 0, 0, 3, 0 , 0 )
                         , Conn_3G_4M3      = CONNEXITY_VALUE( 3, 0, 0, 4, 0 , 0 )
                         // Connexity Name                   | G|M1|M2|M2|Pad|Pin|
                         , Conn_4G_1M1      = CONNEXITY_VALUE( 4, 1, 0, 0, 0 , 0 )
                         , Conn_4G_2M1      = CONNEXITY_VALUE( 4, 2, 0, 0, 0 , 0 )
                         , Conn_4G_3M1      = CONNEXITY_VALUE( 4, 3, 0, 0, 0 , 0 )
                         , Conn_4G_4M1      = CONNEXITY_VALUE( 4, 4, 0, 0, 0 , 0 )
                         , Conn_4G_1M3      = CONNEXITY_VALUE( 4, 0, 0, 1, 0 , 0 )
                         , Conn_1G_1Pad     = CONNEXITY_VALUE( 1, 0, 0, 0, 1 , 0 )
                         , Conn_2G_1Pad     = CONNEXITY_VALUE( 2, 0, 0, 0, 1 , 0 )
                         , Conn_3G_1Pad     = CONNEXITY_VALUE( 3, 0, 0, 0, 1 , 0 )
                         , Conn_1G_1PinM2   = CONNEXITY_VALUE( 1, 0, 1, 0, 0 , 1 )
                         };

    // Connexity Union Type.
      union UConnexity {
        unsigned int connexity;
        struct {
          unsigned int globals : GlobalBSize;
          unsigned int M1      : Metal1BSize;
          unsigned int M2      : Metal2BSize;
          unsigned int M3      : Metal3BSize;
          unsigned int Pad     : PadsBSize;
          unsigned int Pin     : PinsBSize;
        } fields;
      };

      enum TopologyFlag { Global_Vertical_End   = 0x00000001
                        , Global_Horizontal_End = 0x00000002
                        , Global_Horizontal     = 0x00000004
                        , Global_Vertical       = 0x00000008
                        , Global_Turn           = 0x00000010
                        , Global_Fork           = 0x00000020
                        , Global_Fixed          = 0x00000040
                        , Global_End            = Global_Vertical_End | Global_Horizontal_End
                        , Global_Split          = Global_Horizontal | Global_Vertical | Global_Fork
                        };

    // Attributes.
    private:
      static vector<AutoSegment*>  _toFixSegments;
      static unsigned int          _degree;
             UConnexity            _connexity;
             unsigned int          _topology;
             Net*                  _net;
             GCell*                _gcell;
             AutoContact*          _sourceContact;
             AutoContact*          _southWestContact;
             AutoContact*          _northEastContact;
             Hook*                 _fromHook;
             Hook*                 _east;
             Hook*                 _west;
             Hook*                 _north;
             Hook*                 _south;
             vector<RoutingPad*>   _routingPads;
  };


  inline unsigned int  GCellTopology::getStateG () const { return _connexity.fields.globals; }
  inline GCell*        GCellTopology::getGCell  () const { return _gcell; }


  vector<AutoSegment*>  GCellTopology::_toFixSegments;
  unsigned int          GCellTopology::_degree         = 0;


  void  GCellTopology::fixSegments ()
  {
    for ( size_t i=0 ; i<_toFixSegments.size() ; ++i )
      _toFixSegments[i]->setFlags( SegFixed );
    _toFixSegments.clear();
  }


  void  GCellTopology::init ( unsigned int degree )
  {
    _degree = degree;
    _toFixSegments.clear();
  }


  GCellTopology::GCellTopology ( GCellGrid*   gcellGrid
                               , Hook*        fromHook
                               , AutoContact* sourceContact )
    : _connexity       ()
    , _topology        (0)
    , _gcell           (NULL)
    , _sourceContact   (sourceContact)
    , _southWestContact(NULL)
    , _northEastContact(NULL)
    , _fromHook        (fromHook)
    , _east            (NULL)
    , _west            (NULL)
    , _north           (NULL)
    , _south           (NULL)
    , _routingPads     ()
  {
    _connexity.connexity = 0;

    ltrace(99) << "GCellTopology::GCellTopology()" << endl;
    ltracein(99);
    ltrace(99) << getString(fromHook) << endl;
    ltrace(99) << sourceContact << endl;

    Segment* fromSegment = static_cast<Segment*>( _fromHook->getComponent() );
    _net = fromSegment->getNet();

    forEach ( Hook*, hook, fromHook->getHooks() ) {
      Segment* toSegment = dynamic_cast<Segment*>( hook->getComponent() );

      if (toSegment) {
        switch ( getSegmentHookType(*hook) ) {
          case WestBound:  _west  = *hook; break;
          case EastBound:  _east  = *hook; break;
          case SouthBound: _south = *hook; break;
          case NorthBound: _north = *hook; break;
        }

        _connexity.fields.globals++;
      } else {
        Component*  anchor = hook->getComponent();
        RoutingPad* rp     = dynamic_cast<RoutingPad*>( anchor );

        ltrace(99) << "| Looking for Anchor:" << anchor << " rp:" << rp << endl;

        if (anchor) {
          Contact* contact = dynamic_cast<Contact*>( anchor );
          if (contact
             and (  Session::getKatabatic()->isGContact( anchor->getLayer() )
                 or Session::getKatabatic()->isGMetal  ( anchor->getLayer() )) ) {
          // Global routing articulation contact are in <gmetalh> not <gcut> ?
            GCell* gcell = gcellGrid->getGCell( contact->getCenter() );
            ltrace(99) << "* Global Routing Articulation: " << contact << endl;
            ltrace(99) << "| " << gcell << endl;
            if (gcell == NULL)
              throw Error( invalidGCell );
            if (_gcell == NULL) _gcell = gcell;
            else if (_gcell != gcell) {
              throw Error( mismatchGCell );
            }
          } else {
            if (rp and AllianceFramework::get()->isPad(rp->_getEntityAsComponent()->getCell())) {
              _connexity.fields.Pad++;
            } else {
              const Layer* layer = anchor->getLayer();

              if      (layer == Session::getRoutingLayer(0)) _connexity.fields.M1++; // M1 V
              else if (layer == Session::getRoutingLayer(1)) _connexity.fields.M2++; // M2 H
              else if (layer == Session::getRoutingLayer(2)) _connexity.fields.M3++; // M3 V
              else if (layer == Session::getRoutingLayer(3)) _connexity.fields.M2++; // M4 H
              else if (layer == Session::getRoutingLayer(4)) _connexity.fields.M3++; // M5 V
              else {
                cerr << Warning( "Terminal layer \"%s\" of %s is not managed yet (ignored)."
                               , getString(layer->getName()).c_str()
                               , getString(anchor).c_str() )
                     << endl;
                continue;
              }

              if (dynamic_cast<Pin*>(rp->getOccurrence().getEntity())) _connexity.fields.Pin++; 
            }

            ltrace(99) << "| Component to connect: " << anchor << endl;
            _routingPads.push_back( rp );
          }
        }
      }
    }
    ltrace(99) << "east: " << _east  << endl;
    ltrace(99) << "west: " << _west  << endl;
    ltrace(99) << "north:" << _north << endl;
    ltrace(99) << "south:" << _south << endl;

    if (_connexity.fields.globals == 1) {
      if ( _north or _south ) _topology |= Global_Vertical_End;
      else                    _topology |= Global_Horizontal_End;
    } else if (_connexity.fields.globals == 2) {
      if      ( _east  && _west  ) _topology |= Global_Horizontal;
      else if ( _north && _south ) _topology |= Global_Vertical;
      else                         _topology |= Global_Turn;
    } else {
      _topology |= Global_Fork;
    }

    ltraceout(99);

    if (_gcell == NULL) throw Error( missingGCell );
  }


  void  GCellTopology::construct ( ForkStack& forks )
  {
    ltrace(99) << "GCellTopology::construct() [" << _connexity.connexity << "] in " << _gcell << endl;
    ltracein(99);

    _southWestContact = NULL;
    _northEastContact = NULL;

    bool straightLine = false;

    switch ( _connexity.connexity ) {
      case Conn_1G_1Pad:
      case Conn_2G_1Pad:
      case Conn_3G_1Pad:   _do_xG_1Pad();   break;
      case Conn_1G_1PinM2: _do_1G_1PinM2(); break;
      case Conn_1G_1M1:    _do_1G_1M1();    break;
      case Conn_1G_2M1:
      case Conn_1G_3M1:
      case Conn_1G_4M1:  _do_1G_xM1(); break;
      case Conn_1G_1M2:
      case Conn_1G_2M2:
      case Conn_1G_3M2:
      case Conn_1G_4M2:  _do_xG_xM2(); break;
      case Conn_1G_1M3:  _do_1G_1M3(); break;
      case Conn_1G_2M3:
      case Conn_1G_3M3:
      case Conn_1G_4M3:     _do_xG_xM3    (); break;
      case Conn_1G_1M1_1M2: _do_xG_1M1_1M2(); break;
      case Conn_1G_1M1_1M3: _do_1G_xM1    (); break;
      case Conn_2G_1M1:
      case Conn_2G_2M1:
      case Conn_2G_3M1:
      case Conn_2G_4M1:
      case Conn_3G_1M1:
      case Conn_3G_2M1:
      case Conn_3G_3M1:
      case Conn_3G_4M1:
      case Conn_3G_2M3:
      case Conn_3G_3M3:
      case Conn_3G_4M3:
      case Conn_4G_1M1:
      case Conn_4G_2M1:
      case Conn_4G_3M1:
      case Conn_4G_4M1: _do_xG_xM1_xM3(); break;
      case Conn_2G_1M2:
      case Conn_2G_2M2:
      case Conn_2G_3M2:
      case Conn_2G_4M2:
      case Conn_3G_1M2: _do_xG_xM2(); break;
      case Conn_2G_1M3:
      case Conn_2G_2M3:
      case Conn_2G_3M3:
      case Conn_2G_4M3:
      case Conn_3G_1M3:     _do_xG_xM3    (); break;
      case Conn_2G_1M1_1M2: _do_xG_1M1_1M2(); break;
      case Conn_2G:
        if ( (_east and _west) or (_north and _south) ) {
          straightLine = true;
          break;
        }
      case Conn_3G:
      case Conn_4G:
        _do_xG();
        break;
      default:
        throw Bug( "Unmanaged Configuration [%d] = [%d+%d+%d+%d,%d+%d] %s in %s\n"
                   "      The global routing seems to be defective."
                 , _connexity.connexity
                 , _connexity.fields.globals
                 , _connexity.fields.M1     
                 , _connexity.fields.M2     
                 , _connexity.fields.M3
                 , _connexity.fields.Pin
                 , _connexity.fields.Pad
                 , _net->_getString().c_str()
                 , getString(_gcell).c_str()
                 );
        _do_xG();
    }

    if (straightLine) {
    // This a global router problem.
      cerr << Bug( "Unmanaged configuration: straight line in %s,\n"
                   "      The global routing seems to be defective."
                 , _net->_getString().c_str()
                 ) << endl;
      return;
    } else {
      if (_sourceContact) {
        AutoContact* targetContact
          = ( getSegmentHookType(_fromHook) & (NorthBound|EastBound) )
            ? _northEastContact : _southWestContact ;
        AutoSegment* globalSegment = AutoSegment::create( _sourceContact
                                                        , targetContact
                                                        , static_cast<Segment*>( _fromHook->getComponent() )
                                                        );
        globalSegment->setFlags( (_degree == 2) ? SegBipoint : 0 );
        
        ltrace(99) << "Create global segment: " << globalSegment << endl;

#if THIS_IS_DEPRECATED
        if ( globalSegment->isHorizontal()
           and (  (Session::getRoutingGauge()->getLayerDepth(_sourceContact->getLayer()->getBottom()) > 1)
               or (Session::getRoutingGauge()->getLayerDepth(targetContact ->getLayer()->getBottom()) > 1)) ) {
          globalSegment->setLayer ( Session::getRoutingLayer(3) );
          ltrace(99) << "Source:" << _sourceContact << endl;
          ltrace(99) << "Target:" << targetContact << endl;
          ltrace(99) << "Moving up global:" << globalSegment << endl;
        }
#endif
      // HARDCODED VALUE.
        if ( (_topology & Global_Fixed) and (globalSegment->getLength() < 2*Session::getSliceHeight()) )
          _toFixSegments.push_back( globalSegment );
        
        if (_connexity.fields.globals < 2) {
          ltraceout(99);
          return;
        }
      } else
        _fromHook = NULL;
    }

    if ( _east and (_fromHook != _east) ) {
      Hook* toHook = getSegmentOppositeHook( _east );
      ltrace(99) << "Pushing East (to)   " << getString(toHook) << endl;
      ltrace(99) << "Pushing East (from) " << _northEastContact << endl;
      forks.push( toHook, _northEastContact );
    }
    if ( _west and (_fromHook != _west) ) {
      Hook* toHook = getSegmentOppositeHook( _west );
      ltrace(99) << "Pushing West (to)   " << getString(toHook) << endl;
      ltrace(99) << "Pushing West (from) " << _southWestContact << endl;
      forks.push( toHook, _southWestContact );
    }
    if ( _north and (_fromHook != _north) ) {
      Hook* toHook = getSegmentOppositeHook( _north );
      ltrace(99) << "Pushing North (to)   " << getString(toHook) << endl;
      ltrace(99) << "Pushing North (from) " << _northEastContact << endl;
      forks.push( toHook, _northEastContact );
    }
    if ( _south and (_fromHook != _south) ) {
      Hook* toHook = getSegmentOppositeHook( _south );
      ltrace(99) << "Pushing South (to)   " << getString(toHook) << endl;
      ltrace(99) << "Pushing South (from) " << _southWestContact << endl;
      forks.push( toHook, _southWestContact );
    }

    ltraceout(99);
  }


  void  GCellTopology::doRp_AutoContacts ( GCell*        gcell
                                         , Component*    rp
                                         , AutoContact*& source
                                         , AutoContact*& target
                                         , unsigned int  flags
                                         )
  {
    ltrace(99) << "doRp_AutoContacts()" << endl;
    ltracein(99);
    ltrace(99) << rp << endl;

    source = target = NULL;

    Point        sourcePosition;
    Point        targetPosition;
    const Layer* rpLayer        = rp->getLayer();
    size_t       rpDepth        = Session::getLayerDepth( rp->getLayer() );
    unsigned int direction      = Session::getDirection ( rpDepth );
    DbU::Unit    viaSide        = Session::getWireWidth ( rpDepth );

    getPositions( rp, sourcePosition, targetPosition );

    if (sourcePosition.getX() > targetPosition.getX()) swap( sourcePosition, targetPosition );
    if (sourcePosition.getY() > targetPosition.getY()) swap( sourcePosition, targetPosition );

    GCell* sourceGCell = Session::getKatabatic()->getGCellGrid()->getGCell( sourcePosition );
    GCell* targetGCell = Session::getKatabatic()->getGCellGrid()->getGCell( targetPosition );

    if (rpDepth == 0) {
      rpLayer   = Session::getContactLayer(0);
      direction = KbHorizontal;
      viaSide   = Session::getViaWidth( rpDepth );
    }

  // Non-M1 terminal or punctual M1 protections.
    if ((rpDepth != 0) or (sourcePosition == targetPosition)) {
      map<Component*,AutoSegment*>::iterator irp = __routingPadAutoSegments.find( rp );
      if (irp == __routingPadAutoSegments.end()) {
        AutoContact* sourceProtect = AutoContactTerminal::create( sourceGCell
                                                                , rp
                                                                , rpLayer
                                                                , sourcePosition
                                                                , viaSide, viaSide
                                                                );
        AutoContact* targetProtect = AutoContactTerminal::create( targetGCell
                                                                , rp
                                                                , rpLayer
                                                                , targetPosition
                                                                , viaSide, viaSide
                                                                );
        sourceProtect->setFlags( CntFixed );
        targetProtect->setFlags( CntFixed );

        AutoSegment* segment = AutoSegment::create( sourceProtect, targetProtect, direction );
        segment->setFlags( SegFixed );

        __routingPadAutoSegments.insert( make_pair(rp,segment) );
      }
    }

    if (sourcePosition != targetPosition) {
      if (flags & DoSourceContact)
        source = AutoContactTerminal::create( sourceGCell
                                            , rp
                                            , rpLayer
                                            , sourcePosition
                                            , viaSide, viaSide
                                            );
      if (flags & DoTargetContact)
        target = AutoContactTerminal::create( targetGCell
                                            , rp
                                            , rpLayer
                                            , targetPosition
                                            , viaSide, viaSide
                                            );
    }

    if (not source and not target) {
      source = target = AutoContactTerminal::create( gcell
                                                   , rp
                                                   , rpLayer
                                                   , rp->getCenter()
                                                   , viaSide, viaSide
                                                   );
    }

    ltraceout(99);
    return;
  }


  AutoContact* GCellTopology::doRp_Access ( GCell* gcell, Component* rp, unsigned int flags )
  {
    ltrace(99) << "doRp_Access() - flags:" << flags << endl;
    ltracein(99);

    AutoContact* rpContactSource;
    AutoContact* rpContactTarget;

    flags |= checkRoutingPadSize( rp );

    doRp_AutoContacts( gcell, rp, rpContactSource, rpContactTarget, flags );

    if (flags & HAccess) {
      if (flags & VSmall) {
        AutoContact* subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getContactLayer(1) );
        AutoContact* subContact2 = AutoContactTurn::create( gcell, rp->getNet(), Session::getContactLayer(1) );
        AutoSegment::create( rpContactSource, subContact1, KbHorizontal );
        AutoSegment::create( subContact1,     subContact2, KbVertical );
        rpContactSource = subContact2;
      }
    } else {
      if (flags & HSmall) {
        AutoContact* subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getContactLayer(1) );
        AutoSegment::create( rpContactSource, subContact1, KbHorizontal );
        rpContactSource = subContact1;
      }
    }

    ltraceout(99);

    return rpContactSource;
  }


  AutoContact* GCellTopology::doRp_AccessPad ( RoutingPad* rp, unsigned int flags )
  {
    ltrace(99) << "doRp_AccessPad()" << endl;
    ltracein(99);
    ltrace(99) << rp << endl;

  // Hardcoded: H access is METAL2 (depth=1), V access is METAL3 (depth=2).
    size_t  accessDepth = (flags & HAccess) ? 1 : 2 ;
    size_t  padDepth    = Session::getLayerDepth(rp->getLayer());
    if (padDepth > Session::getAllowedDepth()) {
      cerr << Error( "GCellTopology::doRp_AccessPad(): Pad RoutingPad %s\n"
                     "        has a layer unreachable by the router (top layer is: %s)"
                   , getString(rp).c_str()
                   , getString(Session::getRoutingLayer(Session::getAllowedDepth())).c_str()
                   ) << endl;
      padDepth = Session::getAllowedDepth();
    }

    rp->getBodyHook()->detach();

    Point rpPosition = rp->getCenter();
    Point position   = rp->getCenter();
    Box   rpbb       = rp->getBoundingBox();
    if (  (rpbb.getWidth () > 2*Session::getWireWidth(padDepth)) 
       or (rpbb.getHeight() > 2*Session::getWireWidth(padDepth)) ) {
    //cerr << "doRp_AccessPad(): connecting to non-punctual connector (RoutingPad).\n"
    //     << "                  " << rp->getNet() << "pad:" << rp->getOccurrence().getMasterCell() << endl;

      Transformation transf = rp->getOccurrence().getPath().getTransformation();
      switch ( transf.getOrientation() ) {
        case Transformation::Orientation::ID: position.setY( rpbb.getYMin() ); break;
        case Transformation::Orientation::MY: position.setY( rpbb.getYMax() ); break;
        case Transformation::Orientation::YR:
        case Transformation::Orientation::R3: position.setX( rpbb.getXMin() ); break;
        case Transformation::Orientation::R1: position.setX( rpbb.getXMax() ); break;
        default:
          break;
      }
    }

    GCell*     gcell  = Session::getKatabatic()->getGCellGrid()->getGCell(position);
    Component* anchor = rp;

    if (padDepth != accessDepth) {
      if (padDepth > accessDepth) {
      // Go *down* from the pad's RoutingPad.
        --padDepth;

        Contact*   target   = NULL;
        Contact*   source   = Contact::create ( rp
                                              , Session::getContactLayer(padDepth)
                                              , position.getX() - rpPosition.getX()
                                              , position.getY() - rpPosition.getY()
                                              , Session::getViaWidth(padDepth)
                                              , Session::getViaWidth(padDepth)
                                              );
      
        for ( size_t depth = padDepth ; depth >= accessDepth ; --depth ) {
          const Layer* segmentLayer = Session::getRoutingLayer(depth);
          const Layer* targetLayer  = (depth == accessDepth) ? segmentLayer
                                                             : Session::getContactLayer(depth-1);
          DbU::Unit    targetSide   = (depth == accessDepth) ? Session::getWireWidth(depth)
                                                             : Session::getViaWidth (depth-1);

          target = Contact::create( rp->getNet()
                                  , targetLayer
                                  , position.getX()
                                  , position.getY()
                                  , targetSide
                                  , targetSide
                                  );
          if (Session::getDirection(depth) == KbHorizontal) {
            anchor = Horizontal::create( source
                                       , target
                                       , segmentLayer
                                       , position.getY()
                                       , Session::getWireWidth(depth)
                                       );
          } else {
            anchor = Vertical::create( source
                                     , target
                                     , segmentLayer
                                     , position.getX()
                                     , Session::getWireWidth(depth)
                                     );
          }
          ltrace(99) << "Pad strap: " << anchor << endl;
          source = target;
        }
      } else {
      // Go *up* from the pad's RoutingPad.
        Contact*   target   = NULL;
        Contact*   source   = Contact::create ( rp
                                              , Session::getContactLayer(padDepth)
                                              , 0
                                              , 0
                                              , Session::getViaWidth(padDepth)
                                              , Session::getViaWidth(padDepth)
                                              );
      
        for ( size_t depth = padDepth ; depth <= accessDepth ; ++depth ) {
          const Layer* segmentLayer = Session::getRoutingLayer(depth);
          const Layer* targetLayer  = (depth == accessDepth) ? segmentLayer
                                                             : Session::getContactLayer(depth);
          DbU::Unit    targetSide   = (depth == accessDepth) ? Session::getWireWidth(depth)
                                                             : Session::getViaWidth (depth);

          target = Contact::create( rp->getNet()
                                  , targetLayer
                                  , position.getX()
                                  , position.getY()
                                  , targetSide
                                  , targetSide
                                  );
          if (Session::getDirection(depth) == KbHorizontal) {
            anchor = Horizontal::create( source
                                       , target
                                       , segmentLayer
                                       , position.getY()
                                       , Session::getWireWidth(depth)
                                       );
          } else {
            anchor = Vertical::create( source
                                     , target
                                     , segmentLayer
                                     , position.getX()
                                     , Session::getWireWidth(depth)
                                     );
          }
          ltrace(99) << "Pad strap: " << anchor << endl;
          source = target;
        }
      }
    }

    AutoContact* autoSource
      = AutoContactTerminal::create ( gcell
                                    , anchor
                                    , Session::getRoutingLayer(accessDepth)
                                    , position
                                    , Session::getWireWidth(accessDepth)
                                    , Session::getWireWidth(accessDepth)
                                    );
    ltraceout(99);
    return autoSource;
  }


  void  GCellTopology::doRp_StairCaseH ( GCell* gcell, Component* rp1, Component* rp2 )
  {
    ltrace(99) << "doRp_StairCaseH()" << endl;

    if (rp1->getCenter().getX() > rp2->getCenter().getX()) swap( rp1, rp2 );

    AutoContact* rp1ContactSource = NULL;
    AutoContact* rp1ContactTarget = NULL;
    AutoContact* rp2ContactSource = NULL;
    AutoContact* rp2ContactTarget = NULL;
    const Layer* viaLayer         = NULL;

    doRp_AutoContacts( gcell, rp1, rp1ContactSource, rp1ContactTarget, DoTargetContact );
    doRp_AutoContacts( gcell, rp2, rp2ContactSource, rp2ContactTarget, DoSourceContact );

    if (rp1ContactTarget->getY() == rp2ContactSource->getY()) {
      ltrace(99) << "Aligned horizontal routing pads : straight wire" << endl;

      viaLayer = rp1->getLayer();
      AutoSegment::create( rp1ContactTarget, rp2ContactSource, KbHorizontal );
      return;
    }

    viaLayer = Session::getContactLayer(1);

    AutoContact* subContact1 = AutoContactTurn::create( gcell, rp1->getNet(), viaLayer );
    AutoContact* subContact2 = AutoContactTurn::create( gcell, rp1->getNet(), viaLayer );

    AutoSegment::create( rp1ContactTarget, subContact1     , KbHorizontal );
    AutoSegment::create( subContact1     , subContact2     , KbVertical   );
    AutoSegment::create( subContact1     , rp2ContactSource, KbHorizontal );
  }


  void  GCellTopology::doRp_StairCaseV ( GCell* gcell, Component* rp1, Component* rp2 )
  {
    ltrace(99) << "doRp_StairCaseV()" << endl;

    if (rp1->getCenter().getY() > rp2->getCenter().getY()) swap( rp1, rp2 );

    AutoContact* rp1ContactSource = NULL;
    AutoContact* rp1ContactTarget = NULL;
    AutoContact* rp2ContactSource = NULL;
    AutoContact* rp2ContactTarget = NULL;
    const Layer* viaLayer         = NULL;

    doRp_AutoContacts( gcell, rp1, rp1ContactSource, rp1ContactTarget, DoTargetContact );
    doRp_AutoContacts( gcell, rp2, rp2ContactSource, rp2ContactTarget, DoSourceContact );

    if (rp1ContactTarget->getX() == rp2ContactSource->getX()) {
      ltrace(99) << "Aligned vertical routing pads : straight wire" << endl;

      viaLayer = rp1->getLayer();
      AutoSegment::create( rp1ContactTarget, rp2ContactSource, KbVertical );
      return;
    }

    viaLayer = Session::getContactLayer(1);

    AutoContact* subContact1 = AutoContactTurn::create( gcell, rp1->getNet(), viaLayer );
    AutoContact* subContact2 = AutoContactTurn::create( gcell, rp1->getNet(), viaLayer );

    AutoSegment::create( rp1ContactTarget, subContact1     , KbVertical   );
    AutoSegment::create( subContact1     , subContact2     , KbHorizontal );
    AutoSegment::create( subContact1     , rp2ContactSource, KbVertical   );
  }


  void  GCellTopology::_do_xG ()
  {
    ltrace(99) << "_do_xG()" << endl;
    ltracein(99);

    if (_connexity.fields.globals == 2) {
      _southWestContact
        = _northEastContact
        = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
    } else if (_connexity.fields.globals == 3) {
      if (_east and _west) {
        _southWestContact = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        _northEastContact = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(1) );
        if (_south) swap( _southWestContact, _northEastContact );

        AutoSegment::create( _southWestContact, _northEastContact, KbVertical );
      } else {
        _southWestContact = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        _northEastContact = AutoContactHTee::create( _gcell, _net, Session::getContactLayer(1) );
        if (_west) swap( _southWestContact, _northEastContact );

        AutoSegment::create( _southWestContact, _northEastContact, KbHorizontal );
      }
    } else { // fields.globals == 4.
      AutoContact* turn = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
      _southWestContact = AutoContactHTee::create( _gcell, _net, Session::getContactLayer(1) );
      _northEastContact = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(1) );
      AutoSegment::create( _southWestContact, turn, KbHorizontal );
      AutoSegment::create( turn, _northEastContact, KbVertical   );
    } 
    ltraceout(99);
  }


  void  GCellTopology::_do_xG_1Pad ()
  {
    ltrace(99) << "_do_xG_1Pad() [Managed Configuration - Optimized] " << _topology << endl;
    ltracein(99);
    ltrace(99) << "_connexity.globals:" << _connexity.fields.globals << endl;

    unsigned int  flags       = NoFlags;
    bool          eastPad     = false;
    bool          westPad     = false;
    bool          northPad    = false;
    bool          southPad    = false;
    Instance*     padInstance = _routingPads[0]->getOccurrence().getPath().getHeadInstance();

    switch ( padInstance->getTransformation().getOrientation() ) {
      case Transformation::Orientation::ID: northPad = true; break;
      case Transformation::Orientation::MY: southPad = true; break;
      case Transformation::Orientation::YR:
      case Transformation::Orientation::R3: eastPad  = true; flags |= HAccess; break;
      case Transformation::Orientation::R1: westPad  = true; flags |= HAccess; break;
      default:
        cerr << Warning( "Unmanaged orientation %s for pad <%s>."
                       , getString(padInstance->getTransformation().getOrientation()).c_str()
                       , getString(padInstance).c_str() ) << endl;
        break;
    }
    ltrace(99) << "eastPad:"  << eastPad  << ", "
               << "westPad:"  << westPad  << ", "
               << "northPad:" << northPad << ", "
               << "southPad:" << southPad
               << endl;

    AutoContact* source = doRp_AccessPad( _routingPads[0], flags );
    // Point        position     = _routingPads[0]->getCenter();
    // AutoContact* source       = NULL;
    // GCell*       gcell        = Session::getKatabatic()->getGCellGrid()->getGCell(position);

    // source = AutoContactTerminal::create ( gcell
    //                                      , _routingPads[0]
    //                                      , Session::getContactLayer(3)
    //                                      , position
    //                                      , Session::getViaWidth(3), Session::getViaWidth(3)
    //                                      );
    // source->setFlags( CntFixed );

    // if (northPad or eastPad) {
    //   _southWestContact = _northEastContact = source;
    //   ltraceout(99);
    //   return;
    // }

  // Check for straight lines, which are not managed by _do_xG().
    if (_connexity.fields.globals == 1) {
      if (  (westPad and (_east != NULL))
         or (eastPad and (_west != NULL)) ) {
        AutoContact* turn = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        _northEastContact = _southWestContact
                          = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        AutoSegment::create( source, turn, KbHorizontal );
        AutoSegment::create( turn, _northEastContact, KbVertical );
        ltraceout(99);
        return;
      } else if (  (southPad and (_north != NULL))
                or (northPad and (_south != NULL)) ) {
        AutoContact* turn = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        _northEastContact = _southWestContact
                          = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        AutoSegment::create( source, turn, KbVertical );
        AutoSegment::create( turn, _northEastContact, KbHorizontal );
        ltraceout(99);
        return;
      }
    }

    ++_connexity.fields.globals;
    --_connexity.fields.Pad;

    if (westPad ) _west  = source->getBodyHook();
    if (eastPad ) _east  = source->getBodyHook();
    if (southPad) _south = source->getBodyHook();
    if (northPad) _north = source->getBodyHook();

    _do_xG();

    if (westPad) {
      AutoSegment::create( source, _southWestContact, KbHorizontal );
      _west = NULL;
    }
    if (eastPad) {
      AutoSegment::create( source, _northEastContact, KbHorizontal );
      _east = NULL;
    }
    if (southPad) {
      AutoSegment::create( source, _southWestContact, KbVertical );
      _south = NULL;
    }
    if (northPad) {
      AutoSegment::create( source, _northEastContact, KbVertical );
      _north = NULL;
    }
    --_connexity.fields.globals;

    ltraceout(99);
  }


  void  GCellTopology::_do_1G_1PinM2 ()
  {
    ltrace(99) << "_do_1G_1PinM2() [Managed Configuration - Optimized] " << _topology << endl;
    ltracein(99);

    AutoContact* rpContact = doRp_Access( _gcell, _routingPads[0], NoFlags );
    AutoContact* turn1     = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
    AutoSegment::create( rpContact, turn1, KbVertical );

    if (_north or _south) {
      AutoContact* turn2 = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
      AutoSegment::create( turn1, turn2, KbHorizontal );
      turn1 = turn2;
    }
    _southWestContact = _northEastContact = turn1;

    ltraceout(99);
  }


  void  GCellTopology::_do_1G_1M1 ()
  {
    ltrace(99) << "_do_1G_1M1() [Managed Configuration - Optimized] " << _topology << endl;
    ltracein(99);

    unsigned int  flags      = NoFlags;
    if      (_east ) { flags |= HAccess; }
    else if (_west ) { flags |= HAccess; }
    else if (_north) { flags |= VSmall; }
    else if (_south) { flags |= VSmall; }

    _southWestContact = _northEastContact = doRp_Access( _gcell, _routingPads[0], flags );

    ltraceout(99);
  }


  void  GCellTopology::_do_1G_xM1 ()
  {
    ltrace(99) << "_do_1G_" << _connexity.fields.M1 << "M1() [Managed Configuration]" << endl;
    ltracein(99);

    sort( _routingPads.begin(), _routingPads.end(), SortRpByX(NoFlags) ); // increasing X.
    for ( unsigned int i=1 ; i<_routingPads.size() ; ++i ) {
      AutoContact* leftContact  = doRp_Access( _gcell, _routingPads[i-1], HAccess );
      AutoContact* rightContact = doRp_Access( _gcell, _routingPads[i  ], HAccess );
      AutoSegment::create( leftContact, rightContact, KbHorizontal );
    }

    Component* globalRp = NULL;
    if      (_east) globalRp = _routingPads[_routingPads.size()-1];
    else if (_west) globalRp = _routingPads[0];
    else {
      globalRp = _routingPads[0];

      ltrace(99) << "| Initial N/S Global RP: " << globalRp << endl;
      for ( unsigned int i=1 ; i<_routingPads.size() ; ++i ) {
        if (_routingPads[i]->getBoundingBox().getHeight() > globalRp->getBoundingBox().getHeight()) {
          ltrace(99) << "| Better RP: " << globalRp << endl;
          globalRp = _routingPads[i];
        }
      }
    }
    
    AutoContact* globalContact = doRp_Access( _gcell, globalRp, HAccess );

    if (_north or _south) {
      AutoContact* turn = globalContact;
      globalContact = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
      AutoSegment::create( globalContact, turn, KbHorizontal );
    }

    _northEastContact = _southWestContact = globalContact;

    ltraceout(99);
  }


  void  GCellTopology::_do_xG_1M1_1M2 ()
  {
    ltrace(99) << "_do_xG_1M1_1M2() [Managed Configuration]" << endl;
    ltracein(99);

    Component* rpL1;
    Component* rpL2;
    if (_routingPads[0]->getLayer() == Session::getRoutingLayer(0)) {
      rpL1 = _routingPads[0];
      rpL2 = _routingPads[1];
    } else {
      rpL1 = _routingPads[1];
      rpL2 = _routingPads[0];
    }
    ltrace(99) << "rpL1 := " << rpL1 << endl;
    ltrace(99) << "rpL2 := " << rpL2 << endl;

    AutoContact* rpL1ContactSource = NULL;
    AutoContact* rpL1ContactTarget = NULL;
    AutoContact* rpL2ContactSource = NULL;
    AutoContact* rpL2ContactTarget = NULL;

    doRp_AutoContacts( _gcell, rpL1, rpL1ContactSource, rpL1ContactTarget, NoFlags );
    doRp_AutoContacts( _gcell, rpL2, rpL2ContactSource, rpL2ContactTarget, NoFlags );

    AutoContact* subContact = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
    AutoSegment::create( rpL1ContactSource, subContact, KbHorizontal );
    AutoSegment::create( rpL2ContactSource, subContact, KbVertical );

    if (_south or _west) {
      doRp_AutoContacts( _gcell, rpL2, rpL2ContactSource, rpL2ContactTarget, DoSourceContact );
      if (_south and _west) {
        _southWestContact = AutoContactHTee::create( _gcell, _net, Session::getContactLayer(2) );
        AutoSegment::create( rpL2ContactSource, _southWestContact, KbHorizontal );
      } else {
        if (_south) {
          _southWestContact = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(2) );
          AutoSegment::create( rpL2ContactSource, _southWestContact, KbHorizontal );
        } else {
          _southWestContact = rpL2ContactSource;
        }
      }
    }

    if (_north or _east) {
      doRp_AutoContacts( _gcell, rpL2, rpL2ContactSource, rpL2ContactTarget, DoTargetContact );
      if (_north and _east) {
        _northEastContact = AutoContactHTee::create( _gcell, _net, Session::getContactLayer(2) );
        AutoSegment::create( rpL2ContactTarget, _northEastContact, KbHorizontal );
      } else {
        if (_north) {
          _northEastContact = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(2) );
          AutoSegment::create( rpL2ContactTarget, _northEastContact, KbHorizontal );
        } else {
          _northEastContact = rpL2ContactTarget;
        }
      }
    }

    ltraceout(99);
  }


  void  GCellTopology::_do_xG_xM1_xM3 ()
  {
    ltrace(99) << "_do_xG_"  << _connexity.fields.M1
               << "M1_"      << _connexity.fields.M3
               << "M3() [G:" << _connexity.fields.globals << " Managed Configuration]" << endl;
    ltracein(99);
    ltrace(99) << "_connexity: " << _connexity.connexity << endl;
    ltrace(99) << "_north:     " << _north << endl;
    ltrace(99) << "_south:     " << _south << endl;
    ltrace(99) << "_east:      " << _east << endl;
    ltrace(99) << "_west:      " << _west << endl;

    Component* rpM3 = NULL;
    if (_routingPads[0]->getLayer() == Session::getRoutingLayer(2))
      rpM3 = _routingPads[0];

    sort( _routingPads.begin(), _routingPads.end(), SortRpByX(NoFlags) ); // increasing X.
    for ( unsigned int i=1 ; i<_routingPads.size() ; ++i ) {
      AutoContact* leftContact  = doRp_Access( _gcell, _routingPads[i-1], HAccess );
      AutoContact* rightContact = doRp_Access( _gcell, _routingPads[i  ], HAccess );
      AutoSegment::create( leftContact, rightContact, KbHorizontal );

      if (not rpM3 and (_routingPads[i]->getLayer() == Session::getRoutingLayer(2)))
        rpM3 = _routingPads[i];
    }

    AutoContact* unusedContact = NULL;

    if (rpM3) {
    // At least one M3 RoutingPad is present: use it.
      if (_west and not _south) {
        _southWestContact = doRp_Access( _gcell, _routingPads[0], HAccess );
      } else if (not _west and _south) {
        doRp_AutoContacts( _gcell, rpM3, _southWestContact, unusedContact, DoSourceContact );
      } else if (_west and _south) {
        AutoContact* rpContact = NULL;
        doRp_AutoContacts( _gcell, rpM3, rpContact, unusedContact, DoSourceContact );
        _southWestContact = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(1) );
        AutoSegment::create( rpContact, _southWestContact, KbVertical );
      }

      if (_east and not _north) {
        _northEastContact = doRp_Access( _gcell, _routingPads[_routingPads.size()-1], HAccess );
      } else if (not _east and _north) {
        doRp_AutoContacts( _gcell, rpM3, unusedContact, _northEastContact, DoTargetContact );
      } else if (_east and _north) {
        AutoContact* rpContact = NULL;
        doRp_AutoContacts( _gcell, rpM3, unusedContact, rpContact, DoTargetContact );
        _northEastContact = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(1) );
        AutoSegment::create( rpContact, _northEastContact, KbVertical );
      }
    } else {
    // All RoutingPad are M1.
      Component* southWestRp = _routingPads[0];
      ltrace(99) << "| Initial S-W Global RP: " << southWestRp << endl;
      for ( unsigned int i=1 ; i<_routingPads.size() ; ++i ) {
        if (southWestRp->getBoundingBox().getHeight() >= 4*Session::getPitch(1)) break;
        if (_routingPads[i]->getBoundingBox().getHeight() > southWestRp->getBoundingBox().getHeight()) {
          ltrace(99) << "| Better RP: " << southWestRp << endl;
          southWestRp = _routingPads[i];
        }
      }

      if (_west and not _south) {
        _southWestContact = doRp_Access( _gcell, southWestRp, HAccess );
      } else if (not _west and _south) {
        AutoContact* rpContact = doRp_Access( _gcell, southWestRp, HAccess );
        _southWestContact = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        AutoSegment::create( rpContact, _southWestContact, KbHorizontal );
      } else if (_west and _south) {
        AutoContact* rpContact = doRp_Access( _gcell, southWestRp, HAccess );
        _southWestContact = AutoContactHTee::create( _gcell, _net, Session::getContactLayer(1) );
        AutoSegment::create( rpContact, _southWestContact, KbHorizontal );
      }

      Component* northEastRp = _routingPads[_routingPads.size()-1];
      ltrace(99) << "| Initial N-E Global RP: " << northEastRp << endl;

      if (_routingPads.size() > 1) {
        for ( unsigned int i=_routingPads.size()-1; i != 0 ; ) {
          i -= 1;
          if (northEastRp->getBoundingBox().getHeight() >= 4*Session::getPitch(1)) break;
          if (_routingPads[i]->getBoundingBox().getHeight() > northEastRp->getBoundingBox().getHeight()) {
            ltrace(99) << "| Better RP: " << northEastRp << endl;
            northEastRp = _routingPads[i];
          }
        } 
      }

      if (_east and not _north) {
        _northEastContact = doRp_Access( _gcell, northEastRp, HAccess );
      } else if (not _east and _north) {
        AutoContact* rpContact = doRp_Access( _gcell, northEastRp, HAccess );
        _northEastContact = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        AutoSegment::create( rpContact, _northEastContact, KbHorizontal );
      } else if (_east and _north) {
        AutoContact* rpContact = doRp_Access( _gcell, northEastRp, HAccess );
        _northEastContact = AutoContactHTee::create( _gcell, _net, Session::getContactLayer(1) );
        AutoSegment::create( rpContact, _northEastContact, KbHorizontal );
      }
    }

    ltraceout(99);
  }


  void  GCellTopology::_do_xG_xM2 ()
  {
    ltrace(99) << "_do_"
               << _connexity.fields.globals << "G_"
               << _connexity.fields.M2 << "M2() [Managed Configuration - x]" << endl;
    ltracein(99);

    Component* biggestRp = _routingPads[0];
    for ( unsigned int i=1 ; i<_routingPads.size() ; ++i ) {
      doRp_StairCaseH( _gcell, _routingPads[i-1], _routingPads[i] );
      if (_routingPads[i]->getBoundingBox().getWidth() > biggestRp->getBoundingBox().getWidth())
        biggestRp = _routingPads[i];
    }

    AutoContact* unusedContact = NULL;

    if (_west and not _south) {
      doRp_AutoContacts( _gcell, _routingPads[0], _southWestContact, unusedContact, DoSourceContact );
    } else if (not _west and _south) {
      _southWestContact = doRp_Access( _gcell, biggestRp, NoFlags );
    } else if (_west and _south) {
      AutoContact* rpContact = doRp_Access( _gcell, biggestRp, NoFlags );
      _southWestContact = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(1) );
      AutoSegment::create( rpContact, _southWestContact, KbVertical );
    }

    if (_east and not _north) {
      doRp_AutoContacts( _gcell, _routingPads[_routingPads.size()-1], _northEastContact, unusedContact, DoSourceContact );
    } else if (not _east and _north) {
      _northEastContact = doRp_Access( _gcell, biggestRp, NoFlags );
    } else if (_east and _north) {
      AutoContact* rpContact = doRp_Access( _gcell, biggestRp, NoFlags );
      _northEastContact = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(1) );
      AutoSegment::create( rpContact, _northEastContact, KbVertical );
    }

    ltraceout(99);
  }


  void  GCellTopology::_do_1G_1M3 ()
  {
    ltrace(99) << "_do_1G_1M3() [Optimised Configuration]" << endl;
    ltracein(99);

    unsigned int flags = (_east or _west) ? HAccess : NoFlags;
    flags |= (_north) ? DoTargetContact : NoFlags;
    flags |= (_south) ? DoSourceContact : NoFlags;

    doRp_AutoContacts( _gcell
                     , _routingPads[0]
                     , _southWestContact
                     , _northEastContact
                     , flags
                     );
    if (not _southWestContact) _southWestContact = _northEastContact;
    if (not _northEastContact) _northEastContact = _southWestContact;

    ltrace(99) << "_southWest: " << _southWestContact << endl;
    ltrace(99) << "_northEast: " << _northEastContact << endl;

    if (flags & HAccess) {
    // HARDCODED VALUE.
      if (_routingPads[0]->getBoundingBox().getHeight() < 3*Session::getPitch(1)) {
        AutoContact* subContact = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        AutoSegment::create( _southWestContact, subContact, KbVertical );

        _southWestContact = _northEastContact = subContact;
      }
    } else {
      if (_sourceContact) {
        if (_sourceContact->getX() != _southWestContact->getX()) {
          AutoContactTurn* turn1 = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
          AutoContactTurn* turn2 = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
          AutoSegment::create( _southWestContact, turn1, KbVertical );
          AutoSegment::create( turn1            , turn2, KbHorizontal );
          _southWestContact = _northEastContact = turn2;
        }
      }
    }
    ltraceout(99);
  }


  void  GCellTopology::_do_xG_xM3 ()
  {
    ltrace(99) << "_do_xG_" << _connexity.fields.M3
               << "M3() [Managed Configuration]" << endl;
    ltracein(99);
    ltrace(99) << "_west:"  << _west  << endl;
    ltrace(99) << "_east:"  << _east  << endl;
    ltrace(99) << "_south:" << _south << endl;
    ltrace(99) << "_north:" << _north << endl;

    sort( _routingPads.begin(), _routingPads.end(), SortRpByY(NoFlags) ); // increasing Y.
    for ( unsigned int i=1 ; i<_routingPads.size() ; i++ ) {
      doRp_StairCaseV( _gcell, _routingPads[i-1], _routingPads[i] );
    }

    AutoContact* unusedContact = NULL;
    Component*   rp            = _routingPads[0];

    if (_west and not _south) {
      _southWestContact = doRp_Access( _gcell, rp, HAccess );
    } else if (not _west and _south) {
      doRp_AutoContacts( _gcell, rp, _southWestContact, unusedContact, DoSourceContact );
      if (_sourceContact) {
        if (_sourceContact->getX() != _southWestContact->getX()) {
          ltrace(200) << "Misaligned South: _source:" << DbU::getValueString(_sourceContact->getX())
                      << "_southWest:"                << DbU::getValueString(_southWestContact->getX()) << endl;

          AutoContactTurn* turn1 = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
          AutoContactTurn* turn2 = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
          AutoSegment::create( _southWestContact, turn1, KbVertical );
          AutoSegment::create( turn1            , turn2, KbHorizontal );
          _southWestContact = turn2;
        }
      }
    } else if (_west and _south) {
      AutoContact* rpContact = NULL;
      doRp_AutoContacts( _gcell, rp, rpContact, unusedContact, DoSourceContact );
      _southWestContact = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(1) );
      AutoSegment::create( rpContact, _southWestContact, KbVertical );
    }

    rp = _routingPads[_routingPads.size()-1];
    if (_east and not _north) {
      _northEastContact = doRp_Access( _gcell, rp, HAccess );
    } else if (not _east and _north) {
      doRp_AutoContacts( _gcell, rp, unusedContact, _northEastContact, DoTargetContact );
      if (_sourceContact) {
        if (_sourceContact->getX() != _northEastContact->getX()) {
          ltrace(200) << "Misaligned North: _source:" << DbU::getValueString(_sourceContact->getX())
                      << "_southWest:"                << DbU::getValueString(_northEastContact->getX()) << endl;

          AutoContactTurn* turn1 = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
          AutoContactTurn* turn2 = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
          AutoSegment::create( _northEastContact, turn1, KbVertical   );
          AutoSegment::create( turn1            , turn2, KbHorizontal );
          _northEastContact = turn2;
        }
      }
    } else if (_east and _north) {
      AutoContact* rpContact = NULL;
      doRp_AutoContacts( _gcell, rp, unusedContact, rpContact, DoTargetContact );
      _northEastContact = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(1) );
      AutoSegment::create( rpContact, _northEastContact, KbVertical );
    }

    ltraceout(99);
  }


  void  singleGCell ( KatabaticEngine* ktbt, Net* net )
  {
    ltrace(99) << "singleGCell() " << net << endl;
    ltracein(99);

    vector<Component*>  rpM1s;
    Component*          rpM2 = NULL;

    forEach ( RoutingPad*, irp, net->getRoutingPads() ) {
      if (Session::getRoutingGauge()->getLayerDepth(irp->getLayer()) == 1)
        rpM2 = *irp;
      else
        rpM1s.push_back( *irp );
    }

    if ((rpM1s.size() < 2) and not rpM2) {
      cerr << Error( "For %s, less than two Plugs/Pins (%d)."
                   , getString(net).c_str()
                   , rpM1s.size() ) << endl;
      ltraceout(99);
      return;
    }

    sort( rpM1s.begin(), rpM1s.end(), SortRpByX(NoFlags) ); // increasing X.

    GCell* gcell = ktbt->getGCellGrid()->getGCell( (*rpM1s.begin ())->getCenter()
                                                 , (*rpM1s.rbegin())->getCenter() );

    if (not gcell) {
      cerr << Error( "No GCell under %s.", getString(rpM1s[0]).c_str() ) << endl;
      ltraceout(99);
      return;
    }

    ltrace(80) << "singleGCell " << gcell << endl;

    AutoContact* turn   = NULL;
    AutoContact* source = NULL;
    AutoContact* target = NULL;

    for ( size_t irp=1 ; irp<rpM1s.size() ; ++irp ) {
      GCellTopology::doRp_AutoContacts( gcell, rpM1s[irp-1], source, turn, DoSourceContact );
      GCellTopology::doRp_AutoContacts( gcell, rpM1s[irp  ], target, turn, DoSourceContact );
      AutoSegment::create( source, target, KbHorizontal );
    }

    if (rpM2) {
      GCellTopology::doRp_AutoContacts( gcell, rpM1s[0], source, turn, DoSourceContact );
      GCellTopology::doRp_AutoContacts( gcell, rpM2    , target, turn, DoSourceContact );
      turn = AutoContactTurn::create( gcell, rpM2->getNet(), Session::getContactLayer(1) );
      AutoSegment::create( source, turn  , KbHorizontal );
      AutoSegment::create( turn  , target, KbVertical );
    }

    ltraceout(99);
  }


} // Anonymous namespace.




namespace Katabatic {

  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::ltracelevel;
  using Hurricane::Name;
  using Hurricane::DebugSession;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Bug;
  using CRL::addMeasure;
  using CRL::getMeasure;


  void  KatabaticEngine::_loadGrByNet ()
  {
    cmess1 << "  o  Loading Nets global routing from Knik." << endl;
    cmess1 << Dots::asDouble("     - Saturation",getMeasure<double>(getCell(),"Sat.")->getData()) << endl;

    startMeasures();
    Session::open( this );

    forEach ( Net*, inet, getCell()->getNets() ) {
      if (NetRoutingExtension::isAutomaticGlobalRoute(*inet)) {
        DebugSession::open( *inet, 80 );
        _loadNetGlobalRouting( *inet );
        Session::revalidate();
        DebugSession::close();
      } 
    } // forEach(Net*)

#if defined(CHECK_DATABASE)
    _check ( "after Katabatic loading" );
#endif

    _print();
    Session::close();

    stopMeasures();
    printMeasures( "load" );

    addMeasure<size_t>( getCell(), "Globals", AutoSegment::getGlobalsCount() );
    addMeasure<size_t>( getCell(), "Edges"  , AutoSegment::getAllocateds() );
  }


  void  KatabaticEngine::_loadNetGlobalRouting ( Net* net )
  {
    ltrace(100) << "Katabatic::_loadNetGlobalRouting( " << net << " )" << endl;
    ltracein(99);

    ForkStack    forks;
    Hook*        sourceHook    = NULL;
    AutoContact* sourceContact = NULL;

    lookupClear();

    RoutingPads routingPads = net->getRoutingPads();
    size_t      degree      = routingPads.getSize();

    if (degree == 0) {
        cmess2 << Warning("Net \"%s\" do not have any RoutingPad (ignored)."
                         ,getString(net->getName()).c_str()) << endl;
      ltraceout(99);
      return;
    }
    if (degree < 2) {
#if 0
      if ( !getDemoMode() )
        cmess2 << Warning("Net \"%s\" have less than 2 plugs/pins (ignored)."
                         ,getString(net->getName()).c_str()) << endl;
#endif
      ltraceout(99);
      return;
    }

    ltracein(99);
    Hook*  startHook    = NULL;
    GCell* lowestGCell  = NULL;
    size_t unconnecteds = 0;
    size_t connecteds   = 0;

    GCellTopology::init( degree );

    ltrace(99) << "Start RoutingPad Ring" << endl;
    forEach ( RoutingPad*, startRp, routingPads ) {
      bool segmentFound = false;

      forEach ( Hook*, ihook, startRp->getBodyHook()->getHooks() ) {
        ltrace(99) << "Component " << ihook->getComponent() << endl;
        Segment* segment = dynamic_cast<Segment*>( ihook->getComponent() );

        if (segment) {
          ++connecteds;
          segmentFound = true;

          GCellTopology  gcellConf ( getGCellGrid(), *ihook, NULL );
          if (gcellConf.getStateG() == 1) {
            if ( (lowestGCell == NULL) or (lowestGCell->getIndex() > gcellConf.getGCell()->getIndex()) ) {
              ltrace(99) << "Starting from GCell " << gcellConf.getGCell() << endl;
              lowestGCell = gcellConf.getGCell();
              startHook   = *ihook;
            }
            break;
          }
        }
      }

      unconnecteds += (segmentFound) ? 0 : 1;
      if ( (unconnecteds > 10) and (connecteds == 0) ) {
        cerr << Warning("More than 10 unconnected RoutingPads (%u) on %s, missing global routing?"
                       ,unconnecteds, getString(net->getName()).c_str() ) << endl;

        NetRoutingExtension::create( net )->setFlags  ( NetRoutingState::Excluded );
        NetRoutingExtension::create( net )->unsetFlags( NetRoutingState::AutomaticGlobalRoute );
        ltraceout(99);
        return;
      }
    // Uncomment the next line to disable the lowest GCell search.
    // (takes first GCell with exactly one global).
    //if (startHook) break;
    }
    ltraceout(99);

    if (startHook == NULL) { singleGCell( this, net ); ltraceout(99); return; }

    GCellTopology  startGCellConf ( getGCellGrid(), startHook, NULL );
    startGCellConf.construct( forks );

    sourceHook    = forks.getFrom   ();
    sourceContact = forks.getContact();
    forks.pop();

    while ( sourceHook ) {
      GCellTopology  gcellConf ( getGCellGrid(), sourceHook, sourceContact );
      gcellConf.construct( forks );

      sourceHook    = forks.getFrom();
      sourceContact = forks.getContact();
      forks.pop();

      ltrace(99) << "Popping (from) " << sourceHook << endl;
      ltrace(99) << "Popping (to)   " << sourceContact << endl;
    }

    lookupClear();
    Session::revalidate();
    
#if THIS_IS_DISABLED
    set<AutoSegment*>::iterator iover = overconstraineds.begin();
    for ( ; iover != overconstraineds.end() ; ++iover ) {
      (*iover)->makeDogLeg( (*iover)->getAutoSource()->getGCell(), true );
    }
#endif

    Session::revalidate();
    GCellTopology::fixSegments();
    ltraceout(99);
  }


} // Katabatic namespace.
