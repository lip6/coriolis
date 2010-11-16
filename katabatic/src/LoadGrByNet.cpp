
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
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./LoadGrByNet.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <cstdlib>
#include  <sstream>

#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/DebugSession.h"
#include  "hurricane/Layer.h"
#include  "hurricane/Technology.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Net.h"
#include  "hurricane/NetExternalComponents.h"
#include  "hurricane/RoutingPad.h"
#include  "hurricane/RoutingPads.h"
#include  "hurricane/Pad.h"
#include  "hurricane/Plug.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Instance.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Horizontal.h"

#include  "crlcore/AllianceFramework.h"
#include  "crlcore/RoutingGauge.h"
#include  "crlcore/Measures.h"

#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoSegment.h"
#include  "katabatic/GCellGrid.h"
#include  "katabatic/KatabaticEngine.h"


namespace {

  /*! \defgroup     loadGlobalRouting 2. Global Routing Loading (internal)
   *
   *                This module documents how the global routing built by \c Knik is
   *                loaded into the \c Katabatic data-base. It is intented for developpers
   *                only.
   */
 
  //! \addtogroup   loadGlobalRouting
  //! \{
 
  /*! \enum         GCellConfiguration::Topology
   *                set of flags used to build the topology of a GCell.
   *  \see          GCellConfiguration::_topology
   */
 
  /*! \var          GCellConfiguration::GLOBAL_VERTICAL_END
   *                The GCell has exactly one global, which is either from the north
   *                or south side.
   */
 
  /*! \var          GCellConfiguration::GLOBAL_HORIZONTAL_END
   *                The GCell has exactly one global, which is either from the east
   *                or west side.
   */
 
  /*! \var          GCellConfiguration::GLOBAL_HORIZONTAL
   *                The GCell has exactly two global, which are east \& west
   *                (straight horizontal).
   */
 
  /*! \var          GCellConfiguration::GLOBAL_VERTICAL
   *                The GCell has exactly two global, which are north \& south
   *                (straight vertical).
   */
 
  /*! \var          GCellConfiguration::GLOBAL_BEND
   *                The GCell has exactly two global, which are perpandicular.
   *                For example : east \& south.
   */
 
  /*! \var          GCellConfiguration::GLOBAL_FORK
   *                The GCell has three or four globals.
   */
 
  /*! \var          GCellConfiguration::GLOBAL_END
   *                Mask value : the GCell has one global, either vertical or horizontal.
   */
 
  /*! \var          GCellConfiguration::GLOBAL_SPLIT
   *                Mask value used by some functions as a mask argument to
   *                _GCell_GlobalContacts() tell if AutoContact must be splitted or not.
   *                
   */
 
 
  /*! \union        GCellConfiguration::UState
   *  \brief        State of the GCellConfiguration (\b internal).
   *
   *                This union allows the GCellConfiguration to be accessed as
   *                separate fields :
   *                <ul>
   *                  <li>\b UState.fields.globals : number of \b GLOBAL AutoSegment.
   *                  <li>\b UState.fields.L1 : number of terminal in \e metal1. 
   *                  <li>\b UState.fields.L2 : number of terminal in \e metal2.
   *                  <li>\b UState.fields.L3 : number of terminal in \e metal3.
   *                </ul>
   *                And as a unique integer value :
   *                <ul>
   *                  <li>\b UState.state : composite variable, to be used in \c switches.
   *                </ul>
   *                This implementation mixing \c union and \c struct should be portable,
   *                I've faith in compilers :-)
   *
   *  \see          GCellConfiguration.
   */
 
 
  /*! \class        GCellConfiguration
   *  \brief        Build the wiring for a Net inside a GCell (\b internal).
   *
   *  \see          \ref buildRules.
   */
 
  /*! \var          UState GCellConfiguration::_state;
   *                An integer value summarizing the state of the \c GCell. It counts
   *                global wires, and \c Plug/Pin by layer.
   */
 
  /*! \var          unsigned int  GCellConfiguration::_topology;
   *                An integer value summarizing the topology of the globals AutoSegment
   *                of the GCell.
   *
   *  \see          Topology, _GCell_GlobalContacts().
   */
 
  /*! \var          Net* GCellConfiguration::_net;
   *                The current \c Net we are building (guessed from the \c fromSplitter).
   */
 
  /*! \var          CGell* GCellConfiguration::_gcell;
   *                The \c GCell in which we are (guessed from the \c GCell).
   */
 
  /*! \var          unsigned int GCellConfiguration::_fromHook;
   *                By which side of the \c GCell are we coming in.
   */
 
  /*! \var          AutoContact* GCellConfiguration::_sourceContact;
   *                The AutoContact from the previously processed \c GCell.
   */
 
  /*! \var          AutoContact* GCellConfiguration::_southWestContact;
   *                The <em>South West</em> AutoContact of the current \c GCell.
   */
 
  /*! \var          AutoContact* GCellConfiguration::_northEastContact;
   *                The <em>North East</em> AutoContact of the current \c GCell.
   *                (may be equal to _southWestContact, if there's only one AutoContact).
   */
 
  /*! \var          Hook* GCellConfiguration::_east;
   *                The SplitterContact of the east side of the \c GCell (may be \c NULL
   *                if none is present).
   */
 
  /*! \var          Hook* GCellConfiguration::_west;
   *                The SplitterContact of the west side of the \c GCell (may be \c NULL
   *                if none is present).
   */
 
  /*! \var          Hook* GCellConfiguration::_north;
   *                The SplitterContact of the north side of the \c GCell (may be \c NULL
   *                if none is present).
   */
 
  /*! \var          Hook* GCellConfiguration::_south;
   *                The SplitterContact of the south side of the \c GCell (may be \c NULL
   *                if none is present).
   */
 
  /*! \var          vector<RoutingPad*> GCellConfiguration::_routingPads;
   *                The table of \c RoutingPad associated to \c Plug/Pin.  
   */
 
  /*! \function     GCellConfiguration::GCellConfiguration ( GCellGrid* gcellGrid, Hook* fromHook, AutoContact* sourceContact=NULL )
   *  \param        gcellGrid     The \c GCell where we are.
   *  \param        fromHook      From where do we enter the \c GCell.
   *  \param        sourceContact The global routing AutoContact from the previously
   *                              processed \c GCell. May be \c NULL for the first
   *                              \c GCell of a \c Net.
   *
   *                Constructor (see \ref secUsingGCell).
   */
 
  /*! \function     unsigned int  GCellConfiguration::getStateG () const;
   *  \return       The composite state value.
   */
 
  /*! \function     void  GCellConfiguration::construct ( ForkStack& forks );
   *                Build the GCell wires (see \ref secUsingGCell).
   */
 
  /*! \function     void  GCellConfiguration::_GCell_GlobalContacts ( bool split, AutoContact* southWestContact=NULL, AutoContact* northEastContact=NULL );
   *                create AutoContact needed for global wiring. If \e split is
   *                \False one contact is created and set into both _southWestContact
   *                _northEastContact. Otherwise two separated contacts are created.
   *
   *  \see          GCellConfiguration::_topology, Topology.
   */
 
  /*! \function     AutoContact* GCellConfiguration::_GCell_rp_L2H ( RoutingPad* rp, AutoContact* target=NULL, bool hcollapse=false )
   *  \param        rp         Source RoutingPad.
   *  \param        target     Ending AutoContact (created if needed).
   *  \param        hcollapse  collapse the horizontal AutoSegment.
   *
   *                Draw horizontal AutoSegment from the center of a RoutingPad.
   *                \image html  GCellConfiguration-10.png "_GCell_rp_L2H"
   *                \image latex GCellConfiguration-10.pdf "_GCell_rp_L2H"   width=0.2\textwidth
   */
 
  /*! \function     AutoContact* GCellConfiguration::_GCell_rp_L2H_L3V ( RoutingPad* rp, AutoContact* target=NULL, bool hcollapse=false, bool vcollapse=false )
   *  \param        rp         Source RoutingPad.
   *  \param        target     Ending AutoContact (created if needed).
   *  \param        hcollapse  collapse the horizontal AutoSegment.
   *  \param        vcollapse  collapse the vertical AutoSegment.
   *
   *                Draw a simple bend from the center of a RoutingPad. The horizontal
   *                AutoSegment comes first (starting from the source). Both AutoSegments
   *                are flagged as \e terminal.
   *                \image html  GCellConfiguration-11.png "_GCell_rp_L2H_L3V"
   *                \image latex GCellConfiguration-11.pdf "_GCell_rp_L2H_L3V"   width=0.2\textwidth
   */
 
  /*! \function     AutoContact* GCellConfiguration::_GCell_rp_StairCaseH ( RoutingPad* rp1, RoutingPad* rp2 )
   *
   *                Draw an horizontal staircase (\b HVH) between two RoutingPad.
   *                \image html  GCellConfiguration-12.png "_GCell_rp_StairCaseH"
   *                \image latex GCellConfiguration-12.pdf "_GCell_rp_StairCaseH"   width=0.4\textwidth
   */
 
  /*! \function     AutoContact* GCellConfiguration::_GCell_rp_StairCaseV ( RoutingPad* rp1, RoutingPad* rp2 )
   *                Draw a vertical staircase (\b VHV) between two RoutingPad.
   *                \image html  GCellConfiguration-13.png "_GCell_rp_StairCaseV"
   *                \image latex GCellConfiguration-13.pdf "_GCell_rp_StairCaseV"   width=0.1\textwidth
   */
 
  /*! \function     AutoContact* GCellConfiguration::_GCell_L3V_L2H ( AutoContact* source, AutoContact* target=NULL, bool hcollapse=false, bool vcollapse=false, bool terminal=false )
   *  \param        source     The starting AutoContact.
   *  \param        target     The ending AutoContact (created if needed).
   *  \param        hcollapse  collapse the horizontal AutoSegment.
   *  \param        vcollapse  collapse the vertical AutoSegment.
   *  \param        terminal   collapse the horizontal AutoSegment.
   *
   *                Draw a simple bend source to target AutoContact. If target is \e NULL, create
   *                the target. The \e terminal parameter will apply to both horizontal \& vertical
   *                AutoSegment. The horizontal AutoSegment comes first.
   *                \image html  GCellConfiguration-15.png "_GCell_L3V_L2H"
   *                \image latex GCellConfiguration-15.pdf "_GCell_L3V_L2H"   width=0.2\textwidth
   */
 
  /*! \function     AutoContact* GCellConfiguration::_GCell_L2H_L3V ( AutoContact* source, AutoContact* target=NULL, bool hcollapse=false, bool vcollapse=false, bool terminal=false )
   *  \param        source     The starting AutoContact.
   *  \param        target     The ending AutoContact (created if needed).
   *  \param        hcollapse  collapse the horizontal AutoSegment.
   *  \param        vcollapse  collapse the vertical AutoSegment.
   *  \param        terminal   collapse the horizontal AutoSegment.
   *
   *                Draw a simple bend source to target AutoContact. If target is \e NULL, create
   *                the target. The \e terminal parameter will apply to both horizontal \& vertical
   *                AutoSegment. The vertical AutoSegment comes first.
   *                \image html  GCellConfiguration-16.png "_GCell_L2H_L3V"
   *                \image latex GCellConfiguration-16.pdf "_GCell_L2H_L3V"   width=0.2\textwidth
   */
 
  /*! \function     void  GCellConfiguration::_GCell_1G_1L1 ()
   *                Optimized topology for one \e metal1 terminal and one global AutoSegment.
   */
 
  /*! \function     void  GCellConfiguration::_GCell_1G_xL1 ()
   *
   *                Topology for one global AutoSegment and any number of \e metal1 terminals.
   *                \image html  GCellConfiguration-3.png "_GCell_1G_xL1"
   *                \image latex GCellConfiguration-3.pdf "_GCell_1G_xL1"   width=0.8\textwidth
   */
 
  /*! \function     void  GCellConfiguration::_GCell_xG_xL1_xL3 ()
   *
   *                Topology for two or more global AutoSegment and any number of \e metal1 or
   *                \e metal3 terminals. They share the same topology since they are are both
   *                vertical and connected through \e metal2.
   *
   *                <b>Building rules :</b>
   *                <ul>
   *                  <li>Global AutoContact are always splitted except when in the
   *                      \e Bend topology.
   *                  <li>Local terminal AutoSegment are always attached to the South
   *                      West AutoContact except in the <em>Straight Horizontal</em>
   *                      topology or when there is no South global segment.
   *                </ul>
   *
   *                \image html  GCellConfiguration-14.png "Straight Horizontal"
   *                \image html  GCellConfiguration-21.png "Straight Vertical \& Fork (L1 horizontal)"
   *                \image html  GCellConfiguration-22.png "Forks (L1 horizontal)"
   *                \image html  GCellConfiguration-23.png "Forks (L1 vertical)"
   *                \image html  GCellConfiguration-24.png "Bend"
   *                \image latex GCellConfiguration-14.pdf "Straight Horizontal"               width=0.4\textwidth
   *                \image latex GCellConfiguration-21.pdf "Straight Vertical (L1 horizontal)" width=0.8\textwidth
   *                \image latex GCellConfiguration-22.pdf "Forks (L1 horizontal)"             width=0.8\textwidth
   *                \image latex GCellConfiguration-23.pdf "Forks (L1 vertical)"               width=0.8\textwidth
   *                \image latex GCellConfiguration-24.pdf "Bend"                              width=0.4\textwidth
   */
 
  /*! \function     void  GCellConfiguration::_GCell_xG_xL2()
   *
   *                Topology for any global AutoSegment and any number of \e metal2 terminals.
   *
   *                <b>Building rules :</b>
   *                <ul>
   *                  <li>Global AutoContact are always splitteds, except for the
   *                      \e Bend and \e End topology.
   *                  <li>Anchor the local connecting AutoContact on the biggest 
   *                      \RoutingPad, except for the <em>Straight Horizontal</em>
   *                      and the \e End (horizontal) topology. In thoses cases,
   *                      uses the leftmost or rightmost \RoutingPad.
   *                  <li>Prefers vertical AutoSegment to start from the \RoutingPad,
   *                      this needs East and/or West global to be present.
   *                </ul>
   *
   *                \image html  GCellConfiguration-30.png "Straight H/V"
   *                \image html  GCellConfiguration-31.png "Forks with East \& West"
   *                \image html  GCellConfiguration-32.png "Forks without East or West"
   *                \image html  GCellConfiguration-33.png "Complete Fork"
   *                \image html  GCellConfiguration-34.png "All Bends"
   *                \image html  GCellConfiguration-35.png "East or West End"
   *                \image html  GCellConfiguration-36.png "North or South End"
   *                \image latex GCellConfiguration-30.pdf "Straight H/V"               width=0.95\textwidth
   *                \image latex GCellConfiguration-31.pdf "Forks with East \& West"    width=0.95\textwidth
   *                \image latex GCellConfiguration-32.pdf "Forks without East or West" width=0.95\textwidth
   *                \image latex GCellConfiguration-33.pdf "Complete Fork"              width=0.48\textwidth
   *                \image latex GCellConfiguration-34.pdf "All Bends"                  width=0.48\textwidth
   *                \image latex GCellConfiguration-35.pdf "East or West End"           width=0.95\textwidth
   *                \image latex GCellConfiguration-36.pdf "North or South End"         width=0.48\textwidth
   */
 
  /*! \function     void  GCellConfiguration::_GCell_xG_1L1_1L2 ()
   *
   *                Topology for one or more global AutoSegment, one \e metal1 and one
   *                \e metal2 terminals.
   *
   *                <b>Building rules :</b>
   *                <ul>
   *                  <li>Global AutoContact are always splitted except when in the
   *                      \e Bend or \e End topology.
   *                  <li>Local terminal AutoSegment are always attached to an
   *                      AutoContact with one vertical global, the South West
   *                      whenever possible (South global present).
   *                </ul>
   *
   *                \image html  GCellConfiguration-40.png "End H/V"
   *                \image html  GCellConfiguration-41.png "Straight H/V"
   *                \image html  GCellConfiguration-42.png "Forks without East or West"
   *                \image html  GCellConfiguration-43.png "Forks without North or South"
   *                \image html  GCellConfiguration-44.png "Complete Fork"
   *                \image html  GCellConfiguration-45.png "Any Bend"
   *                \image latex GCellConfiguration-40.pdf "End H/V"                      width=0.95\textwidth
   *                \image latex GCellConfiguration-41.pdf "Straight H/V"                 width=0.95\textwidth
   *                \image latex GCellConfiguration-42.pdf "Forks without East or West"   width=0.95\textwidth
   *                \image latex GCellConfiguration-43.pdf "Forks without North or South" width=0.95\textwidth
   *                \image latex GCellConfiguration-44.pdf "Complete Fork"                width=0.48\textwidth
   *                \image latex GCellConfiguration-45.pdf "Any Bend"                     width=0.48\textwidth
   */
 
 
  /*! \class        SortRpByX
   *  \brief        \c RoutingPad \b Compare functor (\b internal)
   */
 
  /*! \function     inline SortRpByX::SortRpByX ( bool decreasing );
   *  \param        decreasing Tells if the sort is done in decreasing order.
   *
   *                This object is a \c Compare functor for the \c sort \c algorithm
   *                over \c STL container made of \c RoutingPad pointers. If \c decreasing
   *                is false the container elements will be sorted by increasing X
   *                coordinates. Otherwise the sort is decreasing.
   *
   *                See GCellConfiguration class.
   */
 
 
  /*! \class        SortRpByY
   *  \brief        \c RoutingPad \b Compare functor (\b internal)
   */
  /*! \function     inline SortRpByY::SortRpByY ( bool decreasing );
   *  \param        decreasing Tells if the sort is done in decreasing order.
   *
   *                This object is a \c Compare functor for the \c sort \c algorithm
   *                over \c STL container made of \c RoutingPad pointers. If \c decreasing
   *                is false the container elements will be sorted by increasing Y
   *                coordinates. Otherwise the sort is decreasing.
   *
   *                See GCellConfiguration class.
   */
 
 
  /*  \function     RoutingPad* lookupOrCreate ( Plug* plug );
   *  \param        plug         A net's plug.
   *  \return       A RoutingPad associated to the plug.
   *
   *                create a \RoutingPad for the plug or find an already created
   *                one. To keep track 
   */
 
  /*  \function     RoutingPad* getRoutingPad ( Plug* plug, const Box& boundingBox );
   *  \param        plug         A net's plug.
   *  \param        boundingBox  An area.
   *  \return       A RoutingPad associated to the plug.
   *
   *                creates a RoutingPad built on plug. Select the best external
   *                component of the master net to uses : the largest component
   *                in the highest layer in the given boundingBox area.
   *
   *                If no component is found under the boundingBox area a
   *                warning is issued and a component outside the area will be
   *                used.
   */
 
  /*  \function     RoutingPad* getRoutingPad ( Pin* pin );
   *  \param        pin          A net's pin.
   *  \return       A RoutingPad associated to the pin.
   *
   *                Unlike for the RoutingPad from a plug, there is no choice
   *                for the external component : it's the pin itself.
   */
  
  /*  \function     bool  arePerpandicular ( unsigned int dir1, unsigned int dir2 );
   *  \param        dir1  First direction.
   *  \param        dir2  Second direction.
   *  \return       \True if the two direction are perpandiculars. As it proceed
   *                with bits operators first and second direction could contains
   *                other flags than \HORIZONTAL and \VERTICAL.
   */
 
  /*  \function     unsigned int  areOppositeSPs ( SplitterContact* spc1, SplitterContact* spc2 );
   *  \param        spc1  First SplitterContact.
   *  \param        spc2  Second SplitterContact.
   *  \return       \True if the two SplitterContact are from the opposites Fences
   *                of a Nimbox.
   */
 
  /*  \function     Point  getEastPosition ( RoutingPad* rp );
   *  \return       Between the source and target point, the one with the greatest
   *                X coordinate (source, if equal).
   */
 
  /*  \function     Point  getWestPosition ( RoutingPad* rp );
   *  \return       Between the source and target point, the one with the lowest
   *                X coordinate (target, if equal).
   */
 
  /*  \function     Point  getNorthPosition ( RoutingPad* rp );
   *  \return       Between the source and target point, the one with the greatest
   *                Y coordinate (source, if equal).
   */
 
  /*  \function     Point  getSouthPosition ( RoutingPad* rp );
   *  \return       Between the source and target point, the one with the lowest
   *                Y coordinate (target, if equal).
   */
 
  /*! \function     void singleGCell ( KatabaticEngine* ktbt, Net* net );
   *  \param        ktbt  A Katabatic \ToolEngine (gives the grid).
   *  \param        net   The net for which to build the topology.
   *
   *                This function handle the special case where a whole net
   *                is included inside only one GCell.
   *
   *  \important    For the time being we assumes that the net is a two
   *                terminal net only. If this is not the case the topology
   *                will be incomplete an so the routing.
   */
 
 
  /*! \class        ForkStack
   *  \brief        Stack of \c Hook / AutoContact (\b internal).
   *
   *                A simple stack of pair of \c Hook / AutoContact. It's used
   *                to handle the recursivity while building a Net's initial wiring in
   *                _loadNetGlobalRouting().
   */
 
  /*! \function     void ForkStack::push ( Hook* from, AutoContact* contact );
   *                Stack a new pair of \c Hook / AutoContact.
   */
 
  /*! \function     void ForkStack::pop ();
   *                Pop an element. The popped element is \b not returned, it's contents are lost.
   */
 
  /*! \function     Hook* ForkStack::getFrom () const;
   *  \return       The \c Hook on top of the stack. \c NULL if
   *                the stack is empty.
   */
 
  /*! \function     Contact* ForkStack::getContact () const;
   *  \return       The \c Contact on top of the stack. \c NULL if
   *                the stack is empty.
   */
 
  //! \}
 
 
  /*! \defgroup     buildRules 1. Rules for building wires (internal)
   *
   *              
   *  \section      secACConf AutoContact configurations.
   *
   *                In this section we details how an AutoContact resise itself, and
   *                introduce some terminology.
   *              
   *                First we distinguish two kinds of segments attached to an AutoContact,
   *                segments that crosses the GCell boundary are considered as <em>global</em>
   *                and others are <em>locals</em>.
   *
   *                As globals AutoSegments crosses the GCell boundaries, when their
   *                extention is adjusted by the AutoContact we have the guarantee that
   *                they will span <em>from the border of the GCell to the AutoContact</em>.
   *                The AutoContact relies strongly on this hypothesis.
   *              
   *                The configuration of an AutoContact is computed in two stages :
   *                <ol>
   *                  <li>We compute the smallest box enclosing all the intersections of
   *                      global segments axis, this box is drawn in red in the figures
   *                      below. We refers this box as the <em>"Junction Box"</em>.
   *                        In some cases the enclosing box is not sufficent to make additionnal
   *                      connections with the local segments (see figures G2.1 and G3.1),
   *                      only in those case we take them in account in the junction box.
   *                  <li>In the second stage we extend the local segments to reach the
   *                      skeleton made by globals segments. In most cases, we do not have
   *                      choices for the extension, but in G4.3 and G4.4.
   *                </ol>
   *              
   *                orientation meaning : (west, east, south and north)
   *                <ul>
   *                  <li>For global segments, it indicates which side of the GCell it
   *                      crosses.
   *                  <li>For local segments, it tells it's relative position to the
   *                      junction box. An horizontal segment will be south if it's axis
   *                      is inferior to the YMin of the junction box and north otherwise. 
   *                      In the same way a vertical segment will be west if is axis is
   *                      inferior to the XMin of the junction box and east otherwise.
   *                </ul>
   *              
   *                \image html  AutoContactG1-1.png "One Global Routing"
   *                \image html  AutoContactG2-1.png "Two Global Routing"
   *                \image html  AutoContactG3-1.png "Three Global Routing"
   *                \image html  AutoContactG3-2.png "Three Global Routing"
   *                \image html  AutoContactG4-1.png "Four Global Routing"
   *                \image html  AutoContactG4-2.png "Four Global Routing"
   *                \image html  AutoContactG4-3.png "Four Global Routing"
   *                \image latex AutoContactG1-1.pdf "One Global Routing"   width=0.3\textwidth
   *                \image latex AutoContactG2-1.pdf "Two Global Routing"   width=0.8\textwidth
   *                \image latex AutoContactG3-1.pdf "Three Global Routing" width=0.8\textwidth
   *                \image latex AutoContactG3-2.pdf "Three Global Routing" width=0.8\textwidth
   *                \image latex AutoContactG4-1.pdf "Four Global Routing"  width=0.8\textwidth
   *                \image latex AutoContactG4-2.pdf "Four Global Routing"  width=0.8\textwidth
   *                \image latex AutoContactG4-3.pdf "Four Global Routing"  width=0.8\textwidth
   *
   *              
   *                <b>The "four sides" box problem :</b>
   *              
   *                  In cases G4.3 and G4.4 we must uses three sides of the junction box
   *                to perform the connection. To minimize wirelength we uses the two small
   *                sides and one of the long size. Which one is the question... For the
   *                moment we systematically choose the lower one (that is west or south).
   *                A problem arises when, by displacing segments, the router change which
   *                side is the shortest one. A Solution to this problem is proposed
   *                in \ref ssecFaultyAutoContact.
   *
   *
   *  \section      secSegStruct Routing Segments Organisation.
   *
   *                Router movements :
   *                <ul>
   *                  <li>Horizontal segments : only translated vertically,
   *                      extensions are not changed, the segment do not
   *                      shrink neither grow.
   *                  <li>Vertical segments : only translated horizontally.
   *                      Extensions remains the same.
   *                </ul>
   *              
   *                Autocontact adjustements :
   *              
   *                When an horizontal segment is vertically moved, vertical ones
   *                that are linked to it through AutoContact must have their
   *                extension adapted : either shrinked or elongated.
   *              
   *                \image html  AutoContact-2.png   "Segment Structure"
   *                \image html  AutoContact-3.png   "Segment Displacement"
   *                \image latex AutoContact-2.pdf   "Segment Structure"    width=0.5\textwidth
   *                \image latex AutoContact-3.pdf   "Segment Displacement" width=0.5\textwidth
   *
   *
   *  \subsection   ssecFaultyAutoContact  AutoContact Geometry restrictions.
   *
   *  \important    The property we want to emphasis here is that whenever a
   *                segment is moved by the router <em>the size of it's
   *                source or target extensions must not change</em>. Only the
   *                extensions of segments perpandicularly connected to it will
   *                change.
   *                Unfortunatly, not all topologies will ensure that property.
   *                The following figures shows all those invalid topologies, and
   *                their correct counterparts. We are looking here from an AutoContact
   *                point of view : how does the AutoContact resizes when an AutoSegment
   *                moves.
   *                
   *                <b>First case</b>
   *
   *                Three globals (\b G1, \b G2, \b G3) and one local
   *                (\b L1). \b L1 is perpandicular to \b G1 and \b G3. According to the
   *                AutoContact sizing specification, \b L1 will be extended to reach
   *                either \b G1 or \b G3, according to it's relative position from
   *                \b G2. Problem arises when the router moves \b L1 and crosses the
   *                position of \b G2. In the figure below, \b L1 is moved up so it
   *                will extend to \b G3 instead of \b G1. So \b L1 horizontal size
   *                will change as it is moved.
   *                
   *                \image html  AutoContactG3-3.png "Three Globals : Problem"
   *                \image latex AutoContactG3-3.pdf "Three Globals : Problem" width=0.8\textwidth
   *
   *                To avoid the problem, simply split \b AC1 in two AutoContact (\b AC-SW \&
   *                \b AC-NE). This way, \b L1 will always be extented to reach the same
   *                global AutoSegment (here : \b G1).
   *
   *                \image html  AutoContactG3-4.png "Three Globals : Solution"
   *                \image latex AutoContactG3-4.pdf "Three Globals : Solution" width=0.8\textwidth
   *
   *                <b>Second case</b>
   *
   *                Four globals (\b G1 to \b G4). This is the
   *                <em>four side box problem</em> (see \ref secACConf). To minimize wirelength,
   *                the \b AC1 AutoContact will uses only three side of the junction box :
   *                the two shortest and one of the longest. The problem shows when \b G3
   *                is moved to the right changing shortest and longest sides.
   *                \b G2 is then shrinked (which is not a problem because it's perpandicular
   *                to \b G3), and \b G3 is extended, which is the problem.
   *                
   *                \image html  AutoContactG4-4.png "Four Globals : Problem"
   *                \image latex AutoContactG4-4.pdf "Four Globals : Problem" width=0.8\textwidth
   *
   *                As for the first case, we choose to split the \b AC1 AutoContact in two
   *                (\b AC-SW \& \b AC-NE) and link them with one local AutoSegment (horizontal).
   *                We arbitrarily group the globals in a <em>South West</em> AutoContact
   *                (\b AC-SW) and <em>North East</em> AutoContact (\b AC-NE). The connexion
   *                between them is horizontal because there is slighly more horizontal
   *                resources.
   *                
   *                \image html  AutoContactG4-5.png "Four Globals : Solution"
   *                \image latex AutoContactG4-5.pdf "Four Globals : Solution" width=0.8\textwidth
   *
   *  \important    We now can express a more synthetic building rule : an AutoContact
   *                must never contain more than two global segments.
   *                This rule allow a simpler managment mechanism for the AutoContact
   *                self sizing procedure (i.e. : speedup).
   *
   *
   *   \subsection  ssecFaultyTopologies Topologies Leading to Gaps.
   *
   *                <b>First Case</b>
   *
   *                The figure "incomplete AutoContact 1" shows how two contiguous local
   *                AutoSegments could lead to a gap in the AutoContact generated
   *                geometry. The local AutoSegment \b L2 is the only vertical component of
   *                the red AutoContact, thus it's source point will be moved to ensure
   *                the vertical connection between \b G1 and \b G2. In the other hand,
   *                the target of \b L2 is bound by the horizontal position of \b L1.
   *                So, if \b G2 (or \b G1) is moved above \b L1 a gap will appear whithin the
   *                AutoContact geometry.
   *
   *                \image html  GCellConfiguration-1.png "incomplete AutoContact 1"
   *                \image latex GCellConfiguration-1.pdf "incomplete AutoContact 1" width=0.8\textwidth
   *
   *                To avoid this problem, the red AutoContact have to be split in two
   *                AutoContacts linked together though a third local AutoSegment \b L3,
   *                as shown on figure "Correct topology". Note that, in this figure we
   *                present an unlikely case : most of the time \b L3 will have a zero
   *                size, and if not, would uses the same track as \b L2. The \b L3 AutoSegment
   *                will have the correct length because is source moves with \b G2 and it's
   *                target with \b G1 (or the other way around, according to the relative
   *                horizontal positions of \b G1 and \b G2). Another point to note is that
   *                there can only be (at most) two global horizontal AutoSegment, one on
   *                left and one on the right. And finally, one say that we could have
   *                suppressed the \b L2 AutoSegment, but in this case it would forces
   *                the alignement of either \b G1 or \b G2 with \b L1, which could be a
   *                severe constraint.
   *
   *                \image html  GCellConfiguration-2.png "Correct Topology 1"
   *                \image latex GCellConfiguration-2.pdf "Correct Topology 1"   width=0.8\textwidth
   *
   *                <b>Second Case</b>
   *
   *                The following figure "Invalid Configuration 2" illustrate the problem
   *                that arises when a local AutoSegment is used to connect more than two
   *                vertical AutoSegments. In other words, is not a mere "dog leg" (double bend).
   *                This implies that on a least one supporting AutoContact there is more
   *                than one vertical AutoSegment attached (\e AC2 in our case).
   *
   *                As per definition, the \e L4 AutoSegment has it's source X position sets
   *                by \e AC1 and it's target X position sets by \e AC2. On \e AC1 there will
   *                be no problems : the X position is given by \e L3 and we always can
   *                extend \e L4 to reach it. On the other end, for \e AC2, as the connexity
   *                is ensured only by extending/shrinking the AutoSegment target X position
   *                we will never be able to reach both \e L7 \& L6 as they are on either
   *                side of \e L3. Only one of them will be reached, \e L6 or \e L7
   *                depending on how the AutoContact will expand.
   *
   *                To avoid this problem, we introduce the AutoContact horizontal and/or
   *                vertical locking. For instance, when an AutoContact is vertically
   *                locked (says \e AC2), then \e L6 \& \e L7 will be kept at the same
   *                X coordinate. Note that this is not done by the AutoContact itself
   *                but rather by declaring \e L6 \& \e L7 as collapsed (as if they where
   *                connected through a collapsed horizontal AutoSegment).
   *
   *                \image html  GCellConfiguration-20.png "incomplete Topology 2 (detail)"
   *                \image latex GCellConfiguration-20.pdf "incomplete Topology 2 (detail)" width=0.8\textwidth 
   *
   *                \image html  GCellConfiguration-18.png "incomplete Topology 2 (context)"
   *                \image latex GCellConfiguration-18.pdf "incomplete Topology 2 (context)" width=0.8\textwidth
   *
   *                \image html  GCellConfiguration-19.png "Correct Topology 2"
   *                \image latex GCellConfiguration-19.pdf "Correct Topology 2" width=0.8\textwidth
   *
   *                For more details about the AutoSegment collapsing whereabouts, see
   *                \ref collapseCanonical.
   *
   *              
   *  \section      secLegalCatalog  Catalog of Legal Topologies.
   *
   *                Summarize the set of legal topologies regarding the following
   *                rules :
   *                <ul>
   *                  <li><b>Rule 1</b> : An AutoContact must have at least one
   *                      horizontal and one vertical attached to it. With the only
   *                      expeption of those anchored on RoutingPad.
   *                  <li><b>Rule 2</b> : An AutoContact must never have more than
   *                      two global AutoSegment. With the only exception of
   *                      AutoContacts with exactly three globals (and no locals).
   *                  <li><b>Rule 3</b> : When the side of a Junction Box is made
   *                      of a local AutoSegment, perpandicular AutoSegments must be
   *                      locked together.
   *                  <li><b>Rule 4</b> : AutoContact with two globals and any number
   *                      of local must be lockeds in at least one direction.
   *                </ul>
   *
   *                <b>Topologies for Dog-Leg (no-fork).</b>
   *
   *                First figure illustrate why rule 1 is needed. Second, the dog
   *                leg general case, and the latest how to circumvent rule 1 by
   *                collapsing \b L2.
   *
   *                \image html  LegalConstruct-1.png "Legal Construct : dog leg"
   *                \image latex LegalConstruct-1.pdf "Legal Construct : dog leg" width=0.8\textwidth
   *
   *                <b>Topologies for Elementary Local Fork.</b>
   *
   *                Illustrate how to build local fork, according to the number of global
   *                AutoSegments part of the fork. First and second case with the side of
   *                the Junction Box made of a global AutoSegment : no constraint.
   *                Third case : the side of the Junction Box is made of a local
   *                AutoSegment, then perpandicular AutoSegment on \b AC2 must be
   *                linked (here : vertical link) in compliance with Rule 3.
   *
   *                \image html  LegalConstruct-2.png "Legal Construct : elementary local fork"
   *                \image latex LegalConstruct-2.pdf "Legal Construct : elementary local fork" width=0.8\textwidth
   *
   *                <b>Topologies for Global Fork (3 branches).</b>
   *
   *                To comply with Rule 2, we must split the AutoContact in two :
   *                the South West (\b AC-SW) and North East (\b AC-NE). Whenever it's possible
   *                we join them through an horizontal AutoSegment. It's not possible if either
   *                \b G2 or \b G3 is missing.
   *
   *                \image html  LegalConstruct-4.png "Legal Construct : Global fork (3 branches, 1)"
   *                \image html  LegalConstruct-5.png "Legal Construct : Global fork (3 branches, 2)"
   *                \image latex LegalConstruct-4.pdf "Legal Construct : Global fork (3 branches, 1)" width=0.8\textwidth
   *                \image latex LegalConstruct-5.pdf "Legal Construct : Global fork (3 branches, 2)" width=0.8\textwidth
   *
   *                <b>Topology for Global Fork (4 branches).</b>
   *
   *                Only one possibility, as shown in the figure below. Two AutoContact
   *                \b AC-SW \& \b AC-NE, and one horizontal AutoSegment.
   *
   *                \image html  LegalConstruct-6.png "Legal Construct : Global fork (4 branches)"
   *                \image latex LegalConstruct-6.pdf "Legal Construct : Global fork (4 branches)" width=0.8\textwidth
   *
   *  \important    If there are local connections inside a global fork, the local
   *                AutoSegment \b L1 will be replaced by a more complex topology.
   *
   *
   *  \section      secGCellConfiguration Using GCellConfiguration
   *
   *                After the global routing stage, the final router needs to achieve
   *                the routing topology in each GCell, for each Net.
   *                This wiring must connect the global wires (that crosses the
   *                GCell boundary) with any number of local terminals.
   *
   *                This object is the atomic action of a recursive walk through
   *                the \c GCells of a \c Net. The recursivity is handled through
   *                a stack : see \c ForkStack.
   *
   *                This classes uses a dictionnary of pre-defined shapes to build
   *                the interconnect. The dictionnary relies on the following
   *                hypothesis :
   *                <ol>
   *                  <li>The GCell is one slice height. This implies
   *                      that in almost all cases, internal terminals in the same
   *                      layer can be ordered form left to right.
   *                  <li>If there is two or more horizontal terminals, they are
   *                      most likely aligned.
   *                  <li>If there is two or more vertical terminals they are
   *                      not on top of each other, but rather side by side
   *                  <li>If there are terminals in layer other than M1, we must
   *                      route through it.
   *                </ol>
   *
   *
   *  \section      secUsingGCell Using a GCellConfiguration object
   *
   *                Obviously, we need the Nimbus global routing structure to be present, as we
   *                uses \c Splitter to progress through the global routing. Using a
   *                GCellConfiguration is simple enough :
   *
   *                <ul>
   *                  <li>create a new GCellConfiguration. At this time we must
   *                      supply the \c GCell to be processed, the \Splitter from which the
   *                      \c GCell is entered and optionally a source AutoContact (from the
   *                      previously processed \c GCell). The constructor go through the
   *                      Net's ring inside the \GCell, finding \SplitterContact, \Plug and
   *                      \Pin. Location of \SplitterContact are stored according to
   *                      their positions (east/west/north/south), \RoutingPad are created
   *                      for each \c Plug/Pin and stored into a \vector. The \c _state and
   *                      \c _topology values are also computed, summarizing respectively
   *                      the number of globals and locals (by layers) and the geometry of
   *                      globals (end, straight, bend, fork) 
   *
   *                  <li>The second step is to call the construct() method. \c construct()
   *                      works in three steps :
   *
   *                      <ol>
   *                        <li>Draw the internal wiring of the \c GCell, using the appropriate
   *                            \c _GCell_xG_xLx() fonction.
   *
   *                            Calls \c _GCell_GlobalContacts(), which create the south west
   *                            (\c _southWestContact ) AutoContact and, if needed the north east
   *                            one (\c _northEastContact ). If only \c _southWestContact is created,
   *                            \c _northEastContact is set to the same value (i.e. is never \NULL).
   *                            Those AutoContacts will supply the support for global \c AutoSegment
   *                            (AutoSegment that goes to and from this \c GCell ).
   *
   *                            If the \c GCell has no internal terminal, then only global wiring
   *                            has to be created, and no \c _GCell_xG_xLx() function is
   *                            to be call. All we have to do is to call the \c _GCell_GlobalContacts().
   *                            function. And in the case we go through the GCell in straight line
   *                            (aligned \c Splitter) we don't even do that.
   *
   *                        <li>Draw the global routing from to previous \c GCell to the
   *                            current one. That is from \c _sourceContact to \c _southWestContact
   *                            or \c _northEastContact according to where we came from.
   *
   *                        <li>Stack the pair of \c SplitterContact/AutoContact into
   *                            the forks stack, except the one we come from (tagged by
   *                            the \c _fromSP ). Here again, we stack either \c _southWestContact
   *                            or \c _northEastContact according to where we came from.
   *                      </ol>
   *                </ul>
   */


  using namespace std;
  using namespace CRL;
  using namespace Hurricane;
  using namespace Katabatic;


  // ---------------------------------------------------------------
  // Local Enum/Types.


  enum SegmentSide { SegmentSouth = (1<<0)
                   , SegmentNorth = (1<<1)
                   , SegmentWest  = (1<<2)
                   , SegmentEast  = (1<<3)
                   };


  // ---------------------------------------------------------------
  // Local Variables.


  const char* invalidGCell =
    "Katabatic::GCellConfiguration () :\n\n"
    "    No GCell under point.\n";

  const char* mismatchGCell =
    "Katabatic::GCellConfiguration () :\n\n"
    "    Contacts under two different GCells.\n";

  const char* missingGCell =
    "Katabatic::GCellConfiguration () :\n\n"
    "    No Contact in GCell.\n";


  map<RoutingPad*,AutoSegment*> __routingPadAutoSegments;


  // ---------------------------------------------------------------
  // LoadGrByNet Local Classes.


  struct NetCompareByName {
      inline bool operator() ( const Net* lhs, const Net* rhs ) const;
  };

  inline bool NetCompareByName::operator() ( const Net* lhs, const Net* rhs ) const
  {
    return lhs->getName() < rhs->getName();
  }


  // ---------------------------------------------------------------
  // LoadGrByNet Local Functions.


  void  lookupClear ()
  {
    __routingPadAutoSegments.clear ();
  }


  void  setIsRoutingPadSmall ( RoutingPad* rp, bool& hsmall, bool& vsmall, bool& punctual )
  {
    Point  source = rp->getSourcePosition();
    Point  target = rp->getTargetPosition();

    DbU::Unit   width  = abs ( target.getX() - source.getX() );
    DbU::Unit   height = abs ( target.getY() - source.getY() );

    hsmall   = ( width  < DbU::lambda(15.0));
    vsmall   = ( height < DbU::lambda(15.0));
    punctual = (width == 0) && (height == 0);
  }


  Hook* getSegmentOppositeHook ( Hook* hook )
  {
    Segment* segment = static_cast<Segment*>( hook->getComponent() );
    return segment->getOppositeHook ( hook );
  }


  unsigned int getSegmentHookType ( Hook* hook )
  {
    Horizontal* horizontal = dynamic_cast<Horizontal*>( hook->getComponent() );
    if ( horizontal ) {
      if ( horizontal->getSourceX() > horizontal->getTargetX() )
        cerr << Warning("Bad orientation of %s",getString(horizontal).c_str()) << endl;

      if ( dynamic_cast<Segment::SourceHook*>(hook) )
        return SegmentEast;
      return SegmentWest;
    }
    Vertical* vertical = dynamic_cast<Vertical*>( hook->getComponent() );
    if ( vertical->getSourceY() > vertical->getTargetY() )
      cerr << Warning("Bad orientation of %s",getString(vertical).c_str()) << endl;

    if ( dynamic_cast<Segment::SourceHook*>(hook) )
      return SegmentNorth;
    return SegmentSouth;
  }


  // ---------------------------------------------------------------
  // Class :  "SortRpByX".


  class SortRpByX {
    public:
      inline       SortRpByX  ( bool decreasing );
      inline bool  operator() ( RoutingPad* rp1, RoutingPad* rp2 );
    protected:
      bool  _decreasing;
  };


  inline  SortRpByX::SortRpByX ( bool decreasing )
    : _decreasing(decreasing)
  { }


  inline bool  SortRpByX::operator() ( RoutingPad* rp1, RoutingPad* rp2 )
  {
    DbU::Unit x1 = rp1->getCenter().getX();
    DbU::Unit x2 = rp2->getCenter().getX();

    if ( x1 == x2 ) return false;
    return _decreasing xor ( x1 < x2 );
  }


  // ---------------------------------------------------------------
  // Class  :  "SortRpByY".


  class SortRpByY {
    public:
      inline       SortRpByY  ( bool decreasing );
      inline bool  operator() ( RoutingPad* rp1, RoutingPad* rp2 );
    protected:
      bool _decreasing;
  };


  inline  SortRpByY::SortRpByY  ( bool decreasing )
    : _decreasing(decreasing)
  { }


  inline bool  SortRpByY::operator() ( RoutingPad* rp1, RoutingPad* rp2 )
  {
    DbU::Unit y1 = rp1->getCenter().getY();
    DbU::Unit y2 = rp2->getCenter().getY();

    if ( y1 == y2 ) return false;
    return _decreasing xor ( y1 < y2 );
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
  inline void         ForkStack::pop              () { if ( !_stack.empty() ) _stack.pop_back(); }
  inline Hook*        ForkStack::getFrom          ()  const { return _stack.empty() ? NULL : _stack.back()._from; }
  inline AutoContact* ForkStack::getContact       ()  const { return _stack.empty() ? NULL : _stack.back()._contact; }


  inline void  ForkStack::push ( Hook* from, AutoContact* contact )
  {
    ltrace(80) << "    Stacking " << (void*)from << " " << from << " + " << contact << endl;
    _stack.push_back(Element(from,contact));
  }


  // ---------------------------------------------------------------
  // Class  :  "GGellConfiguration".


  class GCellConfiguration {

    public:
    // Methods.
                           GCellConfiguration     ( GCellGrid*   gcellGrid
                                                  , Hook*        fromHook
                                                  , AutoContact* sourceContact=NULL );
             void          construct              ( ForkStack& forks );
      inline unsigned int  getStateG              () const;
      inline GCell*        getGCell               () const;
      static bool          _GCell_rp_AutoContacts ( GCell*        gcell
                                                  , RoutingPad*   rp
                                                  , AutoContact*& source
                                                  , AutoContact*& target
                                                  , bool          haccess );
      static AutoContact*  _GCell_rp_Access       ( GCell*        gcell
                                                  , RoutingPad*   rp
                                                  , bool          haccess
                                                  , bool          forceVSmall );
      static AutoContact*  _GCell_rp_L2H          ( GCell*        gcell
                                                  , RoutingPad*   rp
                                                  , AutoContact*  target   =NULL
                                                  , bool          hcollapse=false );
      static AutoContact*  _GCell_rp_L2H_L3V      ( GCell*        gcell
                                                  , RoutingPad*   rp
                                                  , AutoContact*  target   =NULL
                                                  , bool          hcollapse=false
                                                  , bool          vcollapse=false );
      static void          _GCell_rp_StairCaseH   ( GCell*        gcell
                                                  , RoutingPad*   rp1
                                                  , RoutingPad*   rp2 );
      static void          _GCell_rp_StairCaseV   ( GCell*        gcell
                                                  , RoutingPad*   rp1
                                                  , RoutingPad*   rp2 );
      static AutoContact*  _GCell_L3V_L2H         ( GCell*        gcell
                                                  , Net*          net
                                                  , AutoContact*  source
                                                  , AutoContact*  target   =NULL
                                                  , bool          hcollapse=false
                                                  , bool          vcollapse=false
                                                  , bool          terminal =false );
      static AutoContact*  _GCell_L2H_L3V         ( GCell*        gcell
                                                  , Net*          net
                                                  , AutoContact*  source
                                                  , AutoContact*  target   =NULL
                                                  , bool          hcollapse=false
                                                  , bool          vcollapse=false
                                                  , bool          terminal =false );
    protected:                                    
    // Internal Methods.                          
             void          _GCell_GlobalContacts  ( bool          split
                                                  , AutoContact*  southWestContact=NULL
                                                  , AutoContact*  northEastContact=NULL );
             void          _GCell_xG_1Pad         ();
             void          _GCell_1G_1L1          ();
             void          _GCell_1G_xL1          ();
             void          _GCell_xG_xL1_xL3      ();
             void          _GCell_xG_1L1_1L2      ();
             void          _GCell_xG_xL2          ();
             void          _GCell_1G_1L3          ();
             void          _GCell_xG_xL3          ();

    protected:
      // State Values.
      enum State    { GCELL_0G         = 0
                    , GCELL_2G         = 2
                    , GCELL_3G         = 3
                    , GCELL_4G         = 4
                    , GCELL_0G_2L1     = 0+(2<<3)
                    , GCELL_1G_1L1     = 1+(1<<3)
                    , GCELL_1G_2L1     = 1+(2<<3)
                    , GCELL_1G_3L1     = 1+(3<<3)
                    , GCELL_1G_4L1     = 1+(4<<3)
                    , GCELL_1G_1L2     = 1+(1<<6)
                    , GCELL_1G_2L2     = 1+(2<<6)
                    , GCELL_1G_3L2     = 1+(3<<6)
                    , GCELL_1G_4L2     = 1+(4<<6)
                    , GCELL_1G_1L3     = 1+(1<<9)
                    , GCELL_1G_2L3     = 1+(2<<9)
                    , GCELL_1G_3L3     = 1+(3<<9)
                    , GCELL_1G_4L3     = 1+(4<<9)
                    , GCELL_1G_1L1_1L2 = 1+(1<<3)+(1<<6)
                    , GCELL_1G_1L1_1L3 = 1+(1<<3)+(1<<9)
                    , GCELL_2G_1L1     = 2+(1<<3)
                    , GCELL_2G_2L1     = 2+(2<<3)
                    , GCELL_2G_3L1     = 2+(3<<3)
                    , GCELL_2G_4L1     = 2+(4<<3)
                    , GCELL_2G_1L2     = 2+(1<<6)
                    , GCELL_2G_2L2     = 2+(2<<6)
                    , GCELL_2G_3L2     = 2+(3<<6)
                    , GCELL_2G_4L2     = 2+(4<<6)
                    , GCELL_2G_1L3     = 2+(1<<9)
                    , GCELL_2G_2L3     = 2+(2<<9)
                    , GCELL_2G_3L3     = 2+(3<<9)
                    , GCELL_2G_4L3     = 2+(4<<9)
                    , GCELL_2G_1L1_1L2 = 2+(1<<3)+(1<<6)
                    , GCELL_3G_1L1     = 3+(1<<3)
                    , GCELL_3G_2L1     = 3+(2<<3)
                    , GCELL_3G_3L1     = 3+(3<<3)
                    , GCELL_3G_4L1     = 3+(4<<3)
                    , GCELL_3G_1L2     = 3+(1<<6)
                    , GCELL_3G_1L3     = 3+(1<<9)
                    , GCELL_3G_2L3     = 3+(2<<9)
                    , GCELL_3G_3L3     = 3+(3<<9)
                    , GCELL_3G_4L3     = 3+(4<<9)
                    , GCELL_4G_1L1     = 4+(1<<3)
                    , GCELL_4G_2L1     = 4+(2<<3)
                    , GCELL_4G_3L1     = 4+(3<<3)
                    , GCELL_4G_4L1     = 4+(4<<3)
                    , GCELL_4G_1L3     = 4+(1<<9)
                    , GCELL_1G_1Pad    = 1+(1<<12)
                    , GCELL_2G_1Pad    = 2+(1<<12)
                    };

    protected:
      // Topologies Flags/Values.
      enum Topology { GLOBAL_VERTICAL_END   = (1<<0)
                    , GLOBAL_HORIZONTAL_END = (1<<1)
                    , GLOBAL_HORIZONTAL     = (1<<2)
                    , GLOBAL_VERTICAL       = (1<<3)
                    , GLOBAL_BEND           = (1<<4)
                    , GLOBAL_FORK           = (1<<5)
                    , GLOBAL_END            = GLOBAL_VERTICAL_END | GLOBAL_HORIZONTAL_END
                    , GLOBAL_SPLIT          = GLOBAL_HORIZONTAL | GLOBAL_VERTICAL | GLOBAL_FORK
                    };

    protected:
    // State Attribute.
      union UState {
        unsigned int state;
        struct {
          unsigned int globals : 3;
          unsigned int L1      : 3;
          unsigned int L2      : 3;
          unsigned int L3      : 3;
          unsigned int Pad     : 3;
        } fields;
      };

    // Attributes.
    protected:
      UState               _state;
      unsigned int         _topology;
      Net*                 _net;
      GCell*               _gcell;
      AutoContact*         _sourceContact;
      AutoContact*         _southWestContact;
      AutoContact*         _northEastContact;
      Hook*                _fromHook;
      Hook*                _east;
      Hook*                _west;
      Hook*                _north;
      Hook*                _south;
      vector<RoutingPad*>  _routingPads;
  };


  inline unsigned int  GCellConfiguration::getStateG () const { return _state.fields.globals; }
  inline GCell*        GCellConfiguration::getGCell  () const { return _gcell; }


  GCellConfiguration::GCellConfiguration ( GCellGrid*   gcellGrid
                                         , Hook*        fromHook
                                         , AutoContact* sourceContact )
    : _state           ()
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
    ltrace(99) << "GCellConfiguration::GCellConfiguration ()" << endl;
    ltracein(99);
    ltrace(99) << getString(fromHook) << endl;
    ltrace(99) << sourceContact << endl;

    Segment* fromSegment = static_cast<Segment*> ( _fromHook->getComponent() );

    _net         = fromSegment->getNet ();
    _state.state = 0;

    forEach ( Hook*, hook, fromHook->getHooks() ) {
      Segment* toSegment = dynamic_cast<Segment*>(hook->getComponent());
      if ( toSegment ) {
        switch ( getSegmentHookType(*hook) ) {
          case SegmentWest:  _west  = *hook; break;
          case SegmentEast:  _east  = *hook; break;
          case SegmentSouth: _south = *hook; break;
          case SegmentNorth: _north = *hook; break;
        }
        _state.fields.globals++;
      } else {
        RoutingPad* rp = dynamic_cast<RoutingPad*>(hook->getComponent());

        if ( rp ) {
          if ( AllianceFramework::get()->isPad(rp->_getEntityAsComponent()->getCell()) ) {
            _state.fields.Pad++;
          } else {
            const Layer* layer = rp->getLayer();
            if      ( layer == Session::getRoutingLayer(0) ) _state.fields.L1++; // M1 V
            else if ( layer == Session::getRoutingLayer(1) ) _state.fields.L2++; // M2 H
            else if ( layer == Session::getRoutingLayer(2) ) _state.fields.L3++; // M3 V
            else if ( layer == Session::getRoutingLayer(3) ) _state.fields.L2++; // M4 H
            else if ( layer == Session::getRoutingLayer(4) ) _state.fields.L3++; // M5 V
            else {
              cerr << Warning ( "Terminal layer %s of %s is not managed yet (ignored)."
                              , getString(layer->getName()).c_str()
                              , getString(rp).c_str() )
                   << endl;
              continue;
            }
          }

          ltrace(99) << "RoutingPad " << rp << endl;
          _routingPads.push_back ( rp );
        } else {
          Contact* contact = dynamic_cast<Contact*>(hook->getComponent());
          if ( contact ) {
            GCell* gcell = gcellGrid->getGCell ( contact->getCenter() );
            ltrace(99) << gcell << " guessed from " << contact << endl;
            if ( !gcell )
              throw Error ( invalidGCell );
            if ( !_gcell ) _gcell = gcell;
            else if ( _gcell != gcell ) {
              throw Error ( mismatchGCell );
            }
          }
        }
      }
    }

    if (_state.fields.globals == 1) {
      if ( _north || _south ) _topology |= GLOBAL_VERTICAL_END;
      else                    _topology |= GLOBAL_HORIZONTAL_END;
    } else if (_state.fields.globals == 2) {
      if      ( _east  && _west  ) _topology |= GLOBAL_HORIZONTAL;
      else if ( _north && _south ) _topology |= GLOBAL_VERTICAL;
      else                         _topology |= GLOBAL_BEND;
    } else {
      _topology |= GLOBAL_FORK;
    }

    ltraceout(99);

    if ( !_gcell ) throw Error ( missingGCell );
  }


  void  GCellConfiguration::construct ( ForkStack& forks )
  {
    ltrace(99) << "GCellConfiguration::construct () [" << _state.state << "] in " << _gcell << endl;
    ltracein(99);

    bool straightLine = false;

    switch ( _state.state ) {
      case GCELL_1G_1Pad:
      case GCELL_2G_1Pad:    _GCell_xG_1Pad (); break;
      case GCELL_1G_1L1:     _GCell_1G_1L1  (); break;
      case GCELL_1G_2L1:
      case GCELL_1G_3L1:
      case GCELL_1G_4L1:     _GCell_1G_xL1  (); break;
      case GCELL_1G_1L2:
      case GCELL_1G_2L2:
      case GCELL_1G_3L2:
      case GCELL_1G_4L2:     _GCell_xG_xL2  (); break;
      case GCELL_1G_1L3:     _GCell_1G_1L3  (); break;
      case GCELL_1G_2L3:
      case GCELL_1G_3L3:
      case GCELL_1G_4L3:     _GCell_xG_xL3  (); break;
      case GCELL_1G_1L1_1L2: _GCell_xG_1L1_1L2 (); break;
      case GCELL_1G_1L1_1L3: _GCell_1G_xL1  (); break;
      case GCELL_2G_1L1:
      case GCELL_2G_2L1:
      case GCELL_2G_3L1:
      case GCELL_2G_4L1:
      case GCELL_3G_1L1:
      case GCELL_3G_2L1:
      case GCELL_3G_3L1:
      case GCELL_3G_4L1:
      case GCELL_3G_2L3:
      case GCELL_3G_3L3:
      case GCELL_3G_4L3:
      case GCELL_4G_1L1:
      case GCELL_4G_2L1:
      case GCELL_4G_3L1:
      case GCELL_4G_4L1: _GCell_xG_xL1_xL3 (); break;
      case GCELL_2G_1L2:
      case GCELL_2G_2L2:
      case GCELL_2G_3L2:
      case GCELL_2G_4L2:
      case GCELL_3G_1L2: _GCell_xG_xL2 (); break;
      case GCELL_2G_1L3:
      case GCELL_2G_2L3:
      case GCELL_2G_3L3:
      case GCELL_2G_4L3:
      case GCELL_3G_1L3:     _GCell_xG_xL3     (); break;
      case GCELL_2G_1L1_1L2: _GCell_xG_1L1_1L2 (); break;
      case GCELL_2G:
        if ( (_east && _west) || (_north && _south) ) {
          straightLine = true;
          break;
        }
      case GCELL_3G:
        _GCell_GlobalContacts ( false );
        break;
      case GCELL_4G:
        _GCell_GlobalContacts ( true );
        AutoSegment::create ( _southWestContact
                            , _northEastContact
                            , Constant::Horizontal
                            , AutoSegment::Local
                            , false
                            , false
                            );
        break;
      default:
        cerr << Bug("Unmanaged Configuration [%d] = [%d+%d+%d+%d,%d] %s in %s"
                   ,_state.state
                   ,_state.fields.globals
                   ,_state.fields.L1     
                   ,_state.fields.L2     
                   ,_state.fields.L3
                   ,_state.fields.Pad
                   ,_net->_getString().c_str()
                   ,getString(_gcell).c_str()
                   ) << endl;
        _GCell_GlobalContacts ( false );
    }

    if ( straightLine ) {
      cerr << Error("Straight aligned segments (ignored)") << endl;
      ltraceout(99);
      return;
    }

    if ( _sourceContact ) {
      AutoContact* targetContact
        = ( getSegmentHookType(_fromHook) & (SegmentNorth|SegmentEast) )
          ? _northEastContact : _southWestContact ;
      AutoSegment* globalSegment = AutoSegment::create ( _sourceContact
                                                       , targetContact
                                                       , static_cast<Segment*>( _fromHook->getComponent() )
                                                       );

      if ( globalSegment->isHorizontal()
         and (  (Session::getRoutingGauge()->getLayerDepth(_sourceContact->getLayer()->getBottom()) > 1)
             or (Session::getRoutingGauge()->getLayerDepth(targetContact ->getLayer()->getBottom()) > 1)) ) {
        globalSegment->setLayer ( Session::getRoutingLayer(3) );
        ltrace(99) << "Source:" << _sourceContact << endl;
        ltrace(99) << "Target:" << targetContact << endl;
        ltrace(99) << "Moving up global:" << globalSegment << endl;
      }
        
      if ( _state.fields.globals < 2 ) {
        ltraceout(99);
        return;
      }
    } else
      _fromHook = NULL;

    if ( _east  && (_fromHook != _east) ) {
      Hook* toHook = getSegmentOppositeHook ( _east );
      ltrace(99) << "Pushing " << getString(toHook) << endl;
      ltrace(99) << "Pushing " << _northEastContact << endl;
      forks.push ( toHook, _northEastContact );
    }
    if ( _west  && (_fromHook != _west) ) {
      Hook* toHook = getSegmentOppositeHook ( _west );
      ltrace(99) << "Pushing " << getString(toHook) << endl;
      ltrace(99) << "Pushing " << _southWestContact << endl;
      forks.push ( toHook, _southWestContact );
    }
    if ( _north && (_fromHook != _north) ) {
      Hook* toHook = getSegmentOppositeHook ( _north );
      ltrace(99) << "Pushing " << getString(toHook) << endl;
      ltrace(99) << "Pushing " << _northEastContact << endl;
      forks.push ( toHook, _northEastContact );
    }
    if ( _south && (_fromHook != _south) ) {
      Hook* toHook = getSegmentOppositeHook ( _south );
      ltrace(99) << "Pushing " << getString(toHook) << endl;
      ltrace(99) << "Pushing " << _southWestContact << endl;
      forks.push ( toHook, _southWestContact );
    }

    ltraceout(99);
  }


  void  GCellConfiguration::_GCell_GlobalContacts ( bool         split
                                                  , AutoContact* southWestContact
                                                  , AutoContact* northEastContact
                                                  )
  {
    ltrace(99) << "GlobalContacts(" << split << ")" << endl;

    if ( southWestContact )
      _southWestContact = southWestContact;
    else {
      _southWestContact = AutoContact::create ( _gcell, _net, Session::getContactLayer(1) );
    }

    if ( split ) {
      if ( northEastContact )
        _northEastContact = northEastContact;
      else {
        _northEastContact = AutoContact::create ( _gcell, _net, Session::getContactLayer(1) );
      }
    } else
      _northEastContact = _southWestContact;
  }


  bool  GCellConfiguration::_GCell_rp_AutoContacts ( GCell*        gcell
                                                   , RoutingPad*   rp
                                                   , AutoContact*& source
                                                   , AutoContact*& target
                                                   , bool          haccess
                                                   )
  {
    ltrace(99) << "_GCell_rp_AutoContacts()" << endl;
    ltracein(99);

    const Layer* rpLayer        = Session::getContactLayer(0);
    Point        sourcePosition = rp->getSourcePosition();
    Point        targetPosition = rp->getTargetPosition();
    unsigned int direction      = Session::getRoutingGauge()->getLayerDirection(rp->getLayer());

    source = target = NULL;

  // Non-M1 terminal.
    if ( rp->getLayer() != Session::getRoutingLayer(0) ) {
      map<RoutingPad*,AutoSegment*>::iterator irp = __routingPadAutoSegments.find ( rp );
      if ( irp != __routingPadAutoSegments.end() ) {
        source = irp->second->getAutoSource();
        target = irp->second->getAutoTarget();
      } else {
      // Non-M1 Fixed protection.
        if ( sourcePosition.getX() > targetPosition.getX() ) swap ( sourcePosition, targetPosition );
        if ( sourcePosition.getY() > targetPosition.getY() ) swap ( sourcePosition, targetPosition );

        GCell* sourceGCell = Session::getKatabatic()->getGCellGrid()->getGCell ( sourcePosition );
        GCell* targetGCell = Session::getKatabatic()->getGCellGrid()->getGCell ( targetPosition );

        source = AutoContact::fromRp ( sourceGCell
                                     , rp
                                     , rp->getLayer()
                                     , sourcePosition
                                     , DbU::lambda(1.0), DbU::lambda(1.0)
                                     , true
                                     );

        target = AutoContact::fromRp ( targetGCell
                                     , rp
                                     , rp->getLayer()
                                     , targetPosition
                                     , DbU::lambda(1.0), DbU::lambda(1.0)
                                     , true
                                     );

        unsigned int segmentType
          = (sourceGCell == targetGCell) ? AutoSegment::Local : AutoSegment::Global;

        AutoSegment* segment = AutoSegment::create ( source
                                                   , target
                                                   , direction
                                                   , segmentType
                                                   , true
                                                   , false
                                                   );
        segment->setFixed ( true );
        __routingPadAutoSegments.insert ( make_pair(rp,segment) );

      // Associate a M2 fixed protection to punctual M3 terminals.
        // if (   ( rp->getLayer() == Session::getRoutingLayer(2) )
        //    and ( sourcePosition == targetPosition ) ) {
        //   AutoContact* sourceM2 = AutoContact::fromRp ( sourceGCell
        //                                               , rp
        //                                               , Session::getContactLayer(1)
        //                                               , sourcePosition
        //                                               , DbU::lambda(1.0), DbU::lambda(1.0)
        //                                               , true
        //                                               );

        //   AutoContact* targetM2 = AutoContact::fromRp ( sourceGCell
        //                                               , rp
        //                                               , Session::getContactLayer(1)
        //                                               , targetPosition
        //                                               , DbU::lambda(1.0), DbU::lambda(1.0)
        //                                               , true
        //                                               );

        //   AutoSegment* segmentM2 = AutoSegment::create ( sourceM2
        //                                                , targetM2
        //                                                , Constant::Horizontal
        //                                                , AutoSegment::Local
        //                                                , true
        //                                                , false
        //                                                );
        //   segmentM2->setFixed ( true );
        // }
      }

      if ( not (haccess xor (direction == Constant::Horizontal)) ) {
        ltraceout(99);
        return true;
      }
    }

  // Punctual M1.
    if (   ( rp->getLayer() == Session::getRoutingLayer(0) )
       and ( sourcePosition == targetPosition ) ) {
      map<RoutingPad*,AutoSegment*>::iterator irp = __routingPadAutoSegments.find ( rp );
      if ( irp == __routingPadAutoSegments.end() ) {
        GCell* sourceGCell = Session::getKatabatic()->getGCellGrid()->getGCell ( sourcePosition );

        source = AutoContact::fromRp ( sourceGCell
                                     , rp
                                     , Session::getContactLayer(0)
                                     , sourcePosition
                                     , DbU::lambda(1.0), DbU::lambda(1.0)
                                     , true
                                     );

        target = AutoContact::fromRp ( sourceGCell
                                     , rp
                                     , Session::getContactLayer(0)
                                     , targetPosition
                                     , DbU::lambda(1.0), DbU::lambda(1.0)
                                     , true
                                     );

        AutoSegment* segment = AutoSegment::create ( source
                                                   , target
                                                   , Constant::Horizontal
                                                   , AutoSegment::Local
                                                   , true
                                                   , false
                                                   );
        segment->setFixed ( true );
        __routingPadAutoSegments.insert ( make_pair(rp,segment) );
      }
    }

    if ( rp->getLayer() != Session::getRoutingLayer(0) )
      rpLayer = Session::getContactLayer(1);

    source = target = AutoContact::fromRp ( gcell
                                          , rp
                                          , rpLayer
                                          , rp->getCenter()
                                          , DbU::lambda(1.0), DbU::lambda(1.0)
                                          );
    ltraceout(99);
    return false;
  }


  AutoContact* GCellConfiguration::_GCell_rp_Access ( GCell*      gcell
                                                    , RoutingPad* rp
                                                    , bool        haccess
                                                    , bool        forceVSmall )
  {
    ltrace(99) << "_GCell_rp_Access()" << endl;
    ltracein(99);

    AutoContact* rpContactSource;
    AutoContact* rpContactTarget;
    bool         hsmall;
    bool         vsmall;
    bool         punctual;

    setIsRoutingPadSmall ( rp, hsmall, vsmall, punctual );
    vsmall = vsmall || forceVSmall;

    _GCell_rp_AutoContacts ( gcell, rp, rpContactSource, rpContactTarget, haccess );

    if ( not haccess and hsmall ) {
      AutoContact* subContact1 = AutoContact::create ( gcell, rp->getNet(), Session::getContactLayer(1) );
      AutoSegment::create ( rpContactSource
                          , subContact1
                          , Constant::Horizontal
                          , AutoSegment::Local
                          , true
                          , false
                          );
      rpContactSource = subContact1;
    }

    ltraceout(99);

    return rpContactSource;
  }


  AutoContact* GCellConfiguration::_GCell_rp_L2H ( GCell* gcell, RoutingPad* rp, AutoContact* target, bool hcollapse )
  {
    ltrace(99) << "rp_L2H " << rp << endl;

    AutoContact*  rpContactSource;
    AutoContact*  rpContactTarget;
    unsigned int  segmentType = AutoSegment::Local;

    if ( _GCell_rp_AutoContacts(gcell,rp,rpContactSource,rpContactTarget,true) ) {
      if ( gcell->getIndex() > rpContactSource->getGCell()->getIndex() )
        swap ( rpContactSource, rpContactTarget );
      if ( gcell->getIndex() != rpContactSource->getGCell()->getIndex() )
        segmentType = AutoSegment::Global;
    }

    if ( !target )
      target = AutoContact::create ( gcell, rp->getNet(), Session::getContactLayer(1) );

    AutoSegment::create ( rpContactSource, target, Constant::Horizontal, segmentType, true, hcollapse );

    return target;
  }


  AutoContact* GCellConfiguration::_GCell_rp_L2H_L3V ( GCell* gcell, RoutingPad* rp, AutoContact* target, bool hcollapse, bool vcollapse )
  {
    ltrace(99) << "rp_L2H+L3V " << rp << endl;
    ltracein(99);

    AutoContact* source = _GCell_rp_L2H ( gcell, rp, NULL, hcollapse );

    if ( !target ) {
      target = AutoContact::create ( gcell, rp->getNet(), Session::getContactLayer(1) );
    }

    AutoSegment::create ( source, target, Constant::Vertical, AutoSegment::Local, true, vcollapse );

    ltraceout(99);

    return target;
  }


  void  GCellConfiguration::_GCell_rp_StairCaseH ( GCell* gcell, RoutingPad* rp1, RoutingPad* rp2 )
  {
    ltrace(99) << "_GCell_rp_StairCaseH()" << endl;

    if ( rp1->getCenter().getX() > rp2->getCenter().getX() )
      swap ( rp1, rp2 );

    AutoContact* rp1ContactSource;
    AutoContact* rp1ContactTarget;
    AutoContact* rp2ContactSource;
    AutoContact* rp2ContactTarget;
    const Layer* viaLayer;

    _GCell_rp_AutoContacts ( gcell, rp1, rp1ContactSource, rp1ContactTarget, true );
    _GCell_rp_AutoContacts ( gcell, rp2, rp2ContactSource, rp2ContactTarget, true );

    if ( rp1ContactTarget->getY() == rp2ContactSource->getY() ) {
      ltrace(99) << "Aligned horizontal routing pads : straight wire" << endl;

      viaLayer = rp1->getLayer ();
      AutoSegment::create ( rp1ContactTarget
                          , rp2ContactSource
                          , Constant::Horizontal
                          , AutoSegment::Local
                          , true
                          );
      return;
    }

    viaLayer = Session::getContactLayer(1);

    AutoContact* subContact1 = AutoContact::create ( gcell, rp1->getNet(), Session::getContactLayer(1) );
    AutoContact* subContact2 = AutoContact::create ( gcell, rp1->getNet(), Session::getContactLayer(1) );

    AutoSegment::create ( rp1ContactTarget, subContact1     , Constant::Vertical  , AutoSegment::Local, true );
    AutoSegment::create ( subContact1     , subContact2     , Constant::Horizontal, AutoSegment::Local, true );
    AutoSegment::create ( subContact1     , rp2ContactSource, Constant::Vertical  , AutoSegment::Local, true );
  }


  void  GCellConfiguration::_GCell_rp_StairCaseV ( GCell* gcell, RoutingPad* rp1, RoutingPad* rp2 )
  {
    ltrace(99) << "_GCell_rp_StairCaseV()" << endl;

    if ( rp1->getCenter().getY() > rp2->getCenter().getY() )
      swap ( rp1, rp2 );

    AutoContact* rp1ContactSource;
    AutoContact* rp1ContactTarget;
    AutoContact* rp2ContactSource;
    AutoContact* rp2ContactTarget;
    const Layer* viaLayer;

    _GCell_rp_AutoContacts ( gcell, rp1, rp1ContactSource, rp1ContactTarget, false );
    _GCell_rp_AutoContacts ( gcell, rp2, rp2ContactSource, rp2ContactTarget, false );

    if ( rp1ContactTarget->getX() == rp2ContactSource->getX() ) {
      ltrace(99) << "Aligned vertical routing pads : straight wire" << endl;

      viaLayer = rp1->getLayer ();
      AutoSegment::create ( rp1ContactTarget
                          , rp2ContactSource
                          , Constant::Vertical
                          , AutoSegment::Local
                          , true
                          );
      return;
    }

    viaLayer = Session::getContactLayer(1);

    AutoContact* subContact1 = AutoContact::create ( gcell, rp1->getNet(), Session::getContactLayer(1) );
    AutoContact* subContact2 = AutoContact::create ( gcell, rp1->getNet(), Session::getContactLayer(1) );

    AutoSegment::create ( rp1ContactTarget, subContact1     , Constant::Horizontal, AutoSegment::Local, true );
    AutoSegment::create ( subContact1     , subContact2     , Constant::Vertical  , AutoSegment::Local, true );
    AutoSegment::create ( subContact2     , rp2ContactSource, Constant::Horizontal, AutoSegment::Local, true );
  }


  AutoContact* GCellConfiguration::_GCell_L3V_L2H ( GCell*       gcell
                                                  , Net*         net
                                                  , AutoContact* source
                                                  , AutoContact* target
                                                  , bool         hcollapse
                                                  , bool         vcollapse
                                                  , bool         terminal )
  {
    AutoContact* subContact = AutoContact::create ( gcell, net, Session::getContactLayer(1) );

    AutoSegment::create ( source, subContact, Constant::Vertical, AutoSegment::Local, terminal, vcollapse );

    if ( !target )
      target = AutoContact::create ( gcell, net, Session::getContactLayer(1) );

    AutoSegment::create ( subContact, target, Constant::Horizontal, AutoSegment::Local, terminal, hcollapse );

    return target;
  }


  AutoContact* GCellConfiguration::_GCell_L2H_L3V ( GCell*       gcell
                                                  , Net*         net
                                                  , AutoContact* source
                                                  , AutoContact* target
                                                  , bool         hcollapse
                                                  , bool         vcollapse
                                                  , bool         terminal )
  {
    AutoContact* subContact = AutoContact::create ( gcell, net, Session::getContactLayer(1) );

    AutoSegment::create ( source, subContact, Constant::Horizontal, AutoSegment::Local, terminal, hcollapse );

    if ( !target ) {
      target = AutoContact::create ( gcell, net, Session::getContactLayer(1) );
    }

    AutoSegment::create ( subContact, target, Constant::Vertical, AutoSegment::Local, terminal, vcollapse );

    return target;
  }


  void  GCellConfiguration::_GCell_xG_1Pad ()
  {
    ltrace(99) << "_GCell_1G_1Pad() [Managed Configuration - Optimized] " << _topology << endl;
    ltracein(99);

    Point        position     = _routingPads[0]->getCenter();
    AutoContact* source       = NULL;
    AutoContact* target       = NULL;
    AutoContact* swContact    = NULL;
    AutoContact* neContact    = NULL;
    GCell*       gcell        = Session::getKatabatic()->getGCellGrid()->getGCell(position);

    source = AutoContact::fromRp ( gcell
                                 , _routingPads[0]
                                 , Session::getContactLayer(3)
                                 , position
                                 , DbU::lambda(1.0), DbU::lambda(1.0)
                                 , true
                                 );
    target = AutoContact::create ( gcell, _routingPads[0]->getNet(), Session::getContactLayer(2) );

    AutoSegment* segment = AutoSegment::create ( source
                                               , target
                                               , Constant::Horizontal
                                               , AutoSegment::Local
                                               , true                 // Terminal.
                                               , false                // Collapsed.
                                               );
    segment->setLayer ( Session::getRoutingLayer(3) );

    if ( _topology & (GLOBAL_HORIZONTAL_END|GLOBAL_BEND|GLOBAL_HORIZONTAL) ) {
      source = target;
      target = AutoContact::create ( gcell, _routingPads[0]->getNet(), Session::getContactLayer(1) );

      segment = AutoSegment::create ( source
                                    , target
                                    , Constant::Vertical
                                    , AutoSegment::Local
                                    , false    // Terminal.
                                    , false    // Collapsed.
                                    );
    }

    swContact = target;
    neContact = swContact;

    if ( _topology & (GLOBAL_HORIZONTAL|GLOBAL_VERTICAL) ) {
      neContact = AutoContact::create ( gcell, _routingPads[0]->getNet(), Session::getContactLayer(1) );

      int direction;
      if ( _topology & GLOBAL_HORIZONTAL ) {
        direction = Constant::Vertical;
        swContact->setVAlignate ( true );
      } else {
        direction = Constant::Horizontal;
        swContact->setHAlignate ( true );
      }

      segment = AutoSegment::create ( swContact
                                    , neContact
                                    , direction
                                    , AutoSegment::Local
                                    , false                // Terminal.
                                    , false                // Collapsed.
                                    );
    }

    _GCell_GlobalContacts ( (_topology & (GLOBAL_HORIZONTAL|GLOBAL_VERTICAL)), swContact, neContact );

    ltraceout(99);
  }


  void  GCellConfiguration::_GCell_1G_1L1 ()
  {
    ltrace(99) << "_GCell_1G_1L1() [Managed Configuration - Optimized] " << _topology << endl;
    ltracein(99);

    bool  haccess    = false;
    Hook* globalHook = NULL;
    if      ( _east  ) { globalHook = _east; haccess = true; }
    else if ( _west  ) { globalHook = _west; haccess = true; }
    else if ( _north ) { globalHook = _north; }
    else if ( _south ) { globalHook = _south; }

  //Segment*  globalSegment = dynamic_cast<Segment*>(globalHook->getComponent());
  //DbU::Unit length        = (globalSegment) ? globalSegment->getLength() : 0;

    AutoContact* rpContact = _GCell_rp_Access ( _gcell
                                              , _routingPads[0]
                                              , (_topology & GLOBAL_HORIZONTAL_END)
                                              , haccess //(length > DbU::lambda(50.0*2))
                                              );
    _GCell_GlobalContacts ( false, rpContact );

    ltraceout(99);
  }


  void  GCellConfiguration::_GCell_1G_xL1 ()
  {
    ltrace(99) << "_GCell_1G_" << _state.fields.L1 << "L1() [Managed Configuration]" << endl;
    ltracein(99);

    AutoContact* subContact = NULL;
    if ( _topology & GLOBAL_VERTICAL_END ) {
      ltrace(99) << "Global Vertical End" << endl;

      _southWestContact = AutoContact::create ( _gcell, _net, Session::getContactLayer(1) );
    //_southWestContact->setHAlignate ( true );

      sort ( _routingPads.begin(), _routingPads.end(), SortRpByX(false) ); // increasing X.
      for ( unsigned int i = 0 ; i < _routingPads.size() ; i++ ) {
        subContact = _GCell_rp_Access ( _gcell, _routingPads[i], true, false );
        AutoSegment::create ( _southWestContact, subContact, Constant::Horizontal, AutoSegment::Local, true, false );
      }
    } else {
      ltrace(99) << "Global Horizontal End" << endl;

      if ( _east ) sort ( _routingPads.begin(), _routingPads.end(), SortRpByX(false) ); // increasing X.
      else         sort ( _routingPads.begin(), _routingPads.end(), SortRpByX(true ) ); // decreasing X.

      for ( unsigned int i = 0 ; i < _routingPads.size() ; i++ ) {
        _southWestContact = _GCell_rp_Access ( _gcell, _routingPads[i], true, false );
        _southWestContact->setHAlignate ( true );

        if ( i )
          AutoSegment::create ( subContact, _southWestContact, Constant::Horizontal, AutoSegment::Local, true, false );

        subContact = _southWestContact;
      }
    }

    _northEastContact = _southWestContact;

    ltraceout(99);
  }


  void  GCellConfiguration::_GCell_xG_1L1_1L2 ()
  {
    ltrace(99) << "_GCell_xG_1L1_1L2() [Managed Configuration]" << endl;
    ltracein(99);

    RoutingPad* rpL1;
    RoutingPad* rpL2;
    if ( _routingPads[0]->getLayer() == Session::getRoutingLayer(0) ) {
      rpL1 = _routingPads[0];
      rpL2 = _routingPads[1];
    } else {
      rpL1 = _routingPads[1];
      rpL2 = _routingPads[0];
    }
    ltrace(99) << "rpL1 := " << rpL1 << endl;
    ltrace(99) << "rpL2 := " << rpL2 << endl;

    AutoContact* rpL1ContactSource;
    AutoContact* rpL1ContactTarget;
    AutoContact* rpL2ContactSource;
    AutoContact* rpL2ContactTarget;

    _GCell_rp_AutoContacts ( _gcell, rpL1, rpL1ContactSource, rpL1ContactTarget, true );
    AutoContact* subContact = AutoContact::create ( _gcell, _net, Session::getContactLayer(1) );
    AutoSegment::create ( rpL1ContactSource, subContact, Constant::Horizontal, AutoSegment::Local, true );

    _GCell_rp_AutoContacts ( _gcell, rpL2, rpL2ContactSource, rpL2ContactTarget, false );
    AutoSegment::create ( rpL2ContactSource, subContact, Constant::Vertical, AutoSegment::Local, true );

    if ( _state.fields.globals > 0 ) {
      _GCell_rp_AutoContacts ( _gcell, rpL2, _southWestContact, subContact, (_south == NULL) );
      if ( _state.fields.globals > 2 ) {
        _GCell_rp_AutoContacts ( _gcell, rpL2, _northEastContact, subContact, (_north == NULL) );
      } else
        _northEastContact = _southWestContact;
    }

    ltraceout(99);
  }


  void  GCellConfiguration::_GCell_xG_xL1_xL3 ()
  {
    ltrace(99) << "_GCell_xG_" << _state.fields.L1
               << "L1_"        << _state.fields.L3
               << "L3() [G:"   << _state.fields.globals << " Managed Configuration]" << endl;
    ltracein(99);
    ltrace(99) << "_topology: " << _topology << endl;
    ltrace(99) << "_north:    " << _north << endl;
    ltrace(99) << "_south:    " << _south << endl;
    ltrace(99) << "_east:     " << _east << endl;
    ltrace(99) << "_west:     " << _west << endl;

    bool         hsmall   = false;
    bool         vsmall   = false;
    bool         punctual = false;
    AutoSegment* segment  = NULL;

    sort ( _routingPads.begin(), _routingPads.end(), SortRpByX(false) ); // increasing X.
    if ( _topology & GLOBAL_HORIZONTAL ) {
      AutoContact* subContact1 = NULL;
      AutoContact* subContact2 = NULL;
      for ( unsigned int i = 0 ; i < _routingPads.size() ; i++ ) {
        subContact1 = _GCell_rp_Access ( _gcell, _routingPads[i], true, false );
        subContact1->setHAlignate ( true );

        if ( i ) {
          segment = AutoSegment::create ( subContact1
                                        , subContact2
                                        , Constant::Horizontal
                                        , AutoSegment::Local
                                        , true
                                        , false );
          segment->setStrap ( true );
        } else
          _southWestContact = subContact1;

        subContact2 = subContact1;
      }
      _northEastContact = subContact1;
    } else {
      ltrace(99) << "Not straight horizontal " << _south << endl;

      _GCell_GlobalContacts ( _topology & GLOBAL_SPLIT );
      AutoContact* localContact = (_south) ? _southWestContact : _northEastContact;
    //localContact->setHAlignate ( true );

      bool doTurn = (_topology & GLOBAL_SPLIT) and (_routingPads.size() == 1);
      for ( unsigned int i = 0 ; i < _routingPads.size() ; i++ ) {
        AutoContact* rpContact = _GCell_rp_Access ( _gcell, _routingPads[i], true, false );
        AutoContact* turn1
          = (doTurn) ? AutoContact::create(_gcell,_net,Session::getContactLayer(1)) : localContact;

        segment = AutoSegment::create ( rpContact
                                      , turn1
                                      , Constant::Horizontal
                                      , AutoSegment::Local
                                      , true
                                      , false );
        setIsRoutingPadSmall ( _routingPads[i], hsmall, vsmall, punctual );
        if ( not vsmall ) segment->setStrap ( true );

        if ( doTurn ) {
          AutoContact* turn2 = AutoContact::create ( _gcell, _net, Session::getContactLayer(1) );
          segment = AutoSegment::create ( turn1
                                        , turn2
                                        , Constant::Vertical
                                        , AutoSegment::Local
                                        , true
                                        , false );
          segment->setStrap ( true );
          segment = AutoSegment::create ( turn2
                                        , localContact
                                        , Constant::Horizontal
                                        , AutoSegment::Local
                                        , true
                                        , false );
          segment->setStrap ( true );
        }
      }

      if ( _topology & (GLOBAL_VERTICAL|GLOBAL_FORK) ) {
        ltrace(99) << "Global Vertical/Global fork " << _south << endl;

        unsigned int direction = (not _south or not _north) ? Constant::Vertical : Constant::Horizontal;
        segment = AutoSegment::create ( _southWestContact
                                      , _northEastContact
                                      , direction
                                      , AutoSegment::Local
                                      , false
                                      , false
                                      );
        segment->setStrap ( true );

        if ( direction == Constant::Vertical ) {
          if ( !_south ) _northEastContact->setVAlignate ( true );
          else           _southWestContact->setVAlignate ( true );
        }
      }
    }

    ltraceout(99);
  }


  void  GCellConfiguration::_GCell_xG_xL2 ()
  {
    ltrace(99) << "_GCell_"
               << _state.fields.globals << "G_"
               << _state.fields.L2 << "L2() [Managed Configuration - x]" << endl;
    ltracein(99);

    unsigned int  lastRP    = _routingPads.size() - 1;
    RoutingPad*   biggestRP = _routingPads[lastRP];

    sort ( _routingPads.begin(), _routingPads.end(), SortRpByX(false) ); // increasing X.

    for ( unsigned int i = 0 ; i < lastRP ; i++ ) {
      _GCell_rp_StairCaseH ( _gcell, _routingPads[i], _routingPads[i+1] );
      if (   _routingPads[i]->getBoundingBox().getWidth()
           >       biggestRP->getBoundingBox().getWidth() )
        biggestRP = _routingPads[i];
    }

    RoutingPad*  leftRP  = biggestRP;
    RoutingPad*  rightRP = biggestRP;

    switch ( _topology ) {
      case GLOBAL_VERTICAL_END: break;
      case GLOBAL_HORIZONTAL_END:
        if ( _west ) leftRP = _routingPads[0];
        if ( _east ) leftRP = _routingPads[lastRP];
        rightRP = leftRP;
        break;
      case GLOBAL_HORIZONTAL:
        leftRP  = _routingPads[0];
        rightRP = _routingPads[lastRP];
        break;
      case GLOBAL_VERTICAL: break;
      case GLOBAL_BEND:     break;
      case GLOBAL_FORK:     break;
    }

    AutoContact* subContact;
    if ( leftRP == rightRP ) {
      _GCell_rp_AutoContacts ( _gcell, leftRP, _southWestContact, _northEastContact, (_south == NULL) && (_north == NULL) );
    //_northEastContact = _southWestContact;
    } else {
      ltrace(99) << "Using separate global contacts" << endl;
      _GCell_rp_AutoContacts ( _gcell, leftRP , _southWestContact, subContact, (_south == NULL) );
      _GCell_rp_AutoContacts ( _gcell, rightRP, subContact, _northEastContact, (_north == NULL) );
      ltrace(99) << "leftRp:  " << leftRP->getCenter() << " " << leftRP << endl;
      ltrace(99) << "rightRp: " << rightRP->getCenter() << " " << rightRP << endl;
    }

    ltraceout(99);
  }


  void  GCellConfiguration::_GCell_1G_1L3 ()
  {
    ltrace(99) << "_GCell_1G_1L3() [Optimised Configuration]" << endl;
    ltracein(99);

    bool useEnds = (_north != NULL) or (_south != NULL);

    _GCell_rp_AutoContacts ( _gcell
                           , _routingPads[0]
                           , _southWestContact
                           , _northEastContact
                           , not useEnds
                           );

    ltrace(99) << "_southWest: " << _southWestContact << endl;
    ltrace(99) << "_northEast: " << _northEastContact << endl;

    if ( useEnds ) {
      ltrace(99) << "Using Ends" << endl;
      if ( _north != NULL ) {
        ltrace(99) << _gcell << endl;
        ltrace(99) << "Using _northEast, _north: " << _north << endl;
        ltrace(99) << "                  _south: " << _south << endl;
        _southWestContact = _northEastContact;
      }

    //AutoContact* subContact = AutoContact::create ( _gcell, _net, Session::getContactLayer(1) );
    //AutoSegment::create ( _southWestContact, subContact, Constant::Horizontal, AutoSegment::Local, true );

    //_southWestContact = _northEastContact = subContact;
    } else {
      if ( _routingPads[0]->getBoundingBox().getHeight() < DbU::lambda(15) ) {
        AutoContact* subContact = AutoContact::create ( _gcell, _net, Session::getContactLayer(1) );
        AutoSegment::create ( _southWestContact, subContact, Constant::Vertical, AutoSegment::Local, true );

        _southWestContact = _northEastContact = subContact;
      }
    }
    ltraceout(99);
  }


  void  GCellConfiguration::_GCell_xG_xL3 ()
  {
    ltrace(99) << "_GCell_xG_" << _state.fields.L3
               << "L3() [Managed Configuration]" << endl;
    ltracein(99);

    AutoContact* unusedContact;

    if ( _south ) sort ( _routingPads.begin(), _routingPads.end(), SortRpByY(false) ); // increasing Y.
    else          sort ( _routingPads.begin(), _routingPads.end(), SortRpByY(true ) ); // decreasing Y.

    for ( unsigned int i = 1 ; i < _routingPads.size() ; i++ ) {
      _GCell_rp_StairCaseV ( _gcell, _routingPads[i-1], _routingPads[i] );
    }

    if ( _west or _south ) {
      _GCell_rp_AutoContacts ( _gcell, _routingPads[0], _southWestContact, unusedContact, false );
      // _southWestContact = AutoContact::fromRp ( _gcell
      //                                         , _routingPads[0]
      //                                         , Session::getContactLayer(1)
      //                                         , _routingPads[0]->getCenter()
      //                                         , DbU::lambda(1.0), DbU::lambda(1.0)
      //                                         );
    }

    if ( _west and not _south ) {
      AutoContact* subContact = AutoContact::create ( _gcell, _net, Session::getContactLayer(1) );

      AutoSegment::create ( subContact, _southWestContact, Constant::Vertical, AutoSegment::Local, true );
      _southWestContact = subContact;
    }

    if ( _east or _north ) {
      _GCell_rp_AutoContacts ( _gcell, _routingPads[0], unusedContact, _northEastContact, false );
      // _northEastContact = AutoContact::fromRp ( _gcell
      //                                         , _routingPads[_routingPads.size()-1]
      //                                         , Session::getContactLayer(1)
      //                                         , _routingPads[_routingPads.size()-1]->getCenter()
      //                                         , DbU::lambda(1.0), DbU::lambda(1.0)
      //                                         );
    }

    if ( _east and not _north ) {
      AutoContact* subContact = AutoContact::create ( _gcell, _net, Session::getContactLayer(1) );

      AutoSegment::create ( subContact, _northEastContact, Constant::Vertical, AutoSegment::Local, true );
      _northEastContact = subContact;
    }

    if ( !_southWestContact ) _southWestContact = _northEastContact;
    if ( !_northEastContact ) _northEastContact = _southWestContact;

    ltraceout(99);
  }


  void  singleGCell ( KatabaticEngine* ktbt, Net* net )
  {
    ltrace(99) << "singleGCell () " << net << endl;
    ltracein(99);

    vector<RoutingPad*>  routingPads;

    forEach ( RoutingPad*, irp, net->getRoutingPads() ) {
      routingPads.push_back ( *irp );
    }

    if ( routingPads.size() < 2 ) {
      cerr << Error("For %s, less than two Plugs/Pins (%d)."
                   ,getString(net).c_str()
                   ,routingPads.size()) << endl;
      ltraceout(99);
      return;
    }

    // if ( routingPads.size() > 2 ) {
    //   cerr << Error("For %s, more than two Plugs/Pins (%d) in single GCell."
    //                ,getString(net).c_str()
    //                ,routingPads.size()) << endl;
    //   ltraceout(99);
    //   return;
    // }

    sort ( routingPads.begin(), routingPads.end(), SortRpByX(false) ); // increasing X.

    GCell* gcell = ktbt->getGCellGrid()->getGCell ( (*routingPads.begin ())->getCenter()
                                                  , (*routingPads.rbegin())->getCenter() );

    if ( !gcell ) {
      cerr << Error("No GCell under %s.",getString(routingPads[0]).c_str()) << endl;
      ltraceout(99);
      return;
    }

    ltrace(80) << "singleGCell " << gcell << endl;

    AutoContact* dummy  = NULL;
    AutoContact* source = NULL;
    AutoContact* target = NULL;

    for ( size_t irp=1 ; irp<routingPads.size() ; ++irp ) {
      GCellConfiguration::_GCell_rp_AutoContacts ( gcell, routingPads[irp-1], source, dummy, true );
      GCellConfiguration::_GCell_rp_AutoContacts ( gcell, routingPads[irp  ], target, dummy, true );
      AutoSegment::create ( source, target, Constant::Horizontal, AutoSegment::Local, true );
    }

    // GCellConfiguration::_GCell_rp_AutoContacts ( gcell, routingPads[0], source, dummy, true );
    // GCellConfiguration::_GCell_rp_AutoContacts ( gcell, routingPads[1], target, dummy, true );

    // Box sourceBox = source->getNativeConstraintBox ();
    // Box targetBox = target->getNativeConstraintBox ();

    // if (    ( sourceBox.getYMax() < targetBox.getYMin() ) 
    //      || ( sourceBox.getYMin() > targetBox.getYMax() ) ) {
    //   AutoContact* subContact1 = AutoContact::create ( gcell, net, Session::getContactLayer(1) );
    //   AutoSegment::create ( source, subContact1, Constant::Vertical, AutoSegment::Local, true );

    //   AutoContact* subContact2 = AutoContact::create ( gcell, net, Session::getContactLayer(1) );
    //   AutoSegment::create ( target, subContact2, Constant::Vertical, AutoSegment::Local, true );

    //   AutoSegment::create ( subContact1, subContact2, Constant::Horizontal, AutoSegment::Local, false );
    // } else
    //   AutoSegment::create ( source, target, Constant::Horizontal, AutoSegment::Local, true );

    ltraceout(99);
  }


} // End of anonymous namespace.




namespace Katabatic {


  using Hurricane::Name;
  using Hurricane::DebugSession;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Bug;
  using CRL::addMeasure;


  void  KatabaticEngine::_loadGrByNet ()
  {
    cmess1 << "  o  Loading Nets global routing from Knik." << endl;
    cmess1 << Dots::asDouble("     - Saturation",getMeasure<double>(getCell(),"Sat.")->getData()) << endl;

    startMeasures ();
    Session::open ( this );

  //sort ( _routingNets.begin(), _routingNets.end(), NetCompareByName() );
    NetSet::iterator inet = _routingNets.begin();
    while ( inet != _routingNets.end() ) {
      _loadNetGlobalRouting ( *(inet++) );
    }

    Session::revalidate ();

    for ( inet=_routingNets.begin() ; inet != _routingNets.end() ; ++inet )
      _toOptimals ( *inet );

    Session::revalidate ();

#if defined(CHECK_DATABASE)
    _check ( "after Katabatic loading" );
#endif

    _print ();
  //_gcellGrid->checkEdgeSaturation ( 0.60 );
    Session::close ();

    stopMeasures ();
    printMeasures ( "load" );

    addMeasure<size_t> ( getCell(), "Globals", AutoSegment::getGlobalsCount() );
    addMeasure<size_t> ( getCell(), "Edges"  , AutoSegment::getAllocateds() );
  }


  void  KatabaticEngine::_loadNetGlobalRouting ( Net* net )
  {
    DebugSession::open ( net, 80 );

    ltrace(100) << "Katabatic::_loadNetGlobalRouting ( " << net << " )" << endl;
    ltracein(99);

  //cmess2 << "     - " << net << endl;

    ForkStack    forks;
    Hook*        sourceHook    = NULL;
    AutoContact* sourceContact = NULL;

    lookupClear ();

    RoutingPads  routingPads = net->getRoutingPads ();
    if ( routingPads.getSize() < 2 ) {
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
    ltrace(99) << "Start RoutingPad Ring" << endl;
    forEach ( RoutingPad*, startRp, routingPads ) {
      bool segmentFound = false;
      forEach ( Hook*, ihook, startRp->getBodyHook()->getHooks() ) {
        ltrace(99) << "Component " << ihook->getComponent() << endl;
        Segment* segment = dynamic_cast<Segment*>(ihook->getComponent());

        if ( segment ) {
          ++connecteds;
          segmentFound = true;

          GCellConfiguration  gcellConf ( getGCellGrid(), *ihook, NULL );
          if ( gcellConf.getStateG() == 1 ) {
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

        _routingNets.erase ( net );

        ltraceout(99);
        DebugSession::close ();

        return;
      }
    // Comment the next line to enable the lowest GCell search.
    //if ( startHook ) break;
    }
    ltraceout(99);

    if ( startHook == NULL ) { singleGCell ( this, net ); ltraceout(99); return; }

    GCellConfiguration  startGCellConf ( getGCellGrid(), startHook, NULL );
    startGCellConf.construct ( forks );

    sourceHook    = forks.getFrom    ();
    sourceContact = forks.getContact ();
    forks.pop ();

    while ( sourceHook ) {
      GCellConfiguration  gcellConf ( getGCellGrid(), sourceHook, sourceContact );
      gcellConf.construct ( forks );

      sourceHook    = forks.getFrom ();
      sourceContact = forks.getContact ();
      forks.pop ();
    }

    lookupClear ();

    set<AutoSegment*> overconstraineds;
    _computeNetConstraints ( net, overconstraineds );

    Session::revalidate ();
    
    set<AutoSegment*>::iterator iover = overconstraineds.begin();
    for ( ; iover != overconstraineds.end() ; ++iover ) {
      (*iover)->makeDogLeg ( (*iover)->getAutoSource()->getGCell(), true );
    }

    Session::revalidate ();

    ltraceout(99);

    DebugSession::close ();
  }


} // End of Katabatic namespace.
