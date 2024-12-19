// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./KatanaEngine.cpp"                       |
// +-----------------------------------------------------------------+


#include <Python.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "flute.h"
#include "hurricane/utilities/Path.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Timer.h"
#include "hurricane/Layer.h"
#include "hurricane/Net.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/isobar/Script.h"
#include "crlcore/Measures.h"
#include "crlcore/AllianceFramework.h"
#include "anabatic/AutoContact.h"
#include "katana/Block.h"
#include "katana/DataNegociate.h"
#include "katana/RoutingPlane.h"
#include "katana/Session.h"
#include "katana/TrackSegment.h"
#include "katana/NegociateWindow.h"
#include "katana/KatanaEngine.h"
#include "katana/PyKatanaEngine.h"


namespace {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::DBo;
  using Hurricane::NetRoutingState;
  using Hurricane::NetRoutingExtension;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Hurricane::Segment;
  using Hurricane::Plug;
  using Katana::Session;
  using Katana::TrackSegment;


#if THIS_IS_DISABLED
  void  setSymmetricSelf ( Cell* cell, string name )
  {
    Net* net = cell->getNet( name );
    if (not net)
      throw Error( "::setSymmetricSelf() Net \"%s\" not found." , name.c_str() );

    NetRoutingState* state = NetRoutingExtension::get( net );
    if (not state) state = NetRoutingExtension::create( net );

    state->setFlags  ( NetRoutingState::AutomaticGlobalRoute
                     | NetRoutingState::Symmetric
                     | NetRoutingState::Vertical );
    state->setSymAxis( cell->getAbutmentBox().getCenter().getX() );
  }


  void  setSymmetricPair ( Cell* cell, string masterName, string slaveName )
  {
    Net* masterNet = cell->getNet( masterName );
    if (not masterNet)
      throw Error( "::setSymmetricPair() Net \"%s\" not found." , masterName.c_str() );

    NetRoutingState* masterState = NetRoutingExtension::get( masterNet );
    if (not masterState) masterState = NetRoutingExtension::create( masterNet );

    masterState->setFlags  ( NetRoutingState::AutomaticGlobalRoute
                           | NetRoutingState::Symmetric
                           | NetRoutingState::SymmetricMaster
                           | NetRoutingState::Vertical );
    masterState->setSymAxis( cell->getAbutmentBox().getCenter().getX() );

    Net* slaveNet = cell->getNet( slaveName );
    if (not slaveNet)
      throw Error( "KatanaEngine::runTest() Net \"%s\" not found." , slaveName.c_str() );

    NetRoutingState* slaveState = NetRoutingExtension::get( slaveNet );
    if (not slaveState) slaveState = NetRoutingExtension::create( slaveNet );

    slaveState ->setFlags  ( NetRoutingState::AutomaticGlobalRoute
                           | NetRoutingState::Symmetric
                           | NetRoutingState::Vertical );
    slaveState ->setSymAxis(cell->getAbutmentBox().getCenter().getX() );
    slaveState ->setSymNet ( masterNet );
    masterState->setSymNet (  slaveNet );
  }
#endif


  void  rsetNoExtractFlag ( Cell* cell, size_t depth=0 )
  {
    static set<Cell*,DBo::CompareById> processeds;

    if (not depth) processeds.clear();
    else {
      if (processeds.find(cell) != processeds.end()) return;
      processeds.insert( cell );
    }

    if (cell->isRouted() or cell->isTerminalNetlist()) return;
    if (depth) cell->setNoExtractConsistent( true );

    for ( Instance* instance : cell->getInstances() )
      rsetNoExtractFlag( instance->getMasterCell(), depth+1 );

    if (not depth) processeds.clear();
  }


}  // Anonymous namespace.


namespace Katana {

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::dec;
  using std::setw;
  using std::setfill;
  using std::left;
  using std::ostream;
  using std::ofstream;
  using std::ostringstream;
  using std::setprecision;
  using std::fixed;
  using std::defaultfloat;
  using std::vector;
  using std::make_pair;
  using Hurricane::dbo_ptr;
  using Hurricane::UpdateSession;
  using Hurricane::DebugSession;
  using Hurricane::tab;
  using Hurricane::ForEachIterator;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Timer;
  using Hurricane::Box;
  using Hurricane::Torus;
  using Hurricane::Layer;
  using Hurricane::Entity;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Hurricane::NetRoutingState;
  using Hurricane::NetRoutingExtension;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using CRL::System;
  using CRL::Catalog;
  using CRL::AllianceFramework;
  using CRL::addMeasure;
  using CRL::Measures;
  using CRL::MeasuresSet;
  using Anabatic::Edge;
  using Anabatic::EdgeCapacity;
  using Anabatic::EngineState;
  using Anabatic::GCellsUnder;
  using Anabatic::AutoContact;
  using Anabatic::AutoSegmentLut;
  using Anabatic::ChipTools;


  const char* missingRW =
    "%s:\n\n"
    "    Cell %s do not have any KatanaEngine (or not yet created).\n";


// -------------------------------------------------------------------
// Class  :  "Katana::KatanaEngine".

  Name  KatanaEngine::_toolName = "Katana";


  const Name& KatanaEngine::staticGetName ()
  { return _toolName; }


  KatanaEngine* KatanaEngine::get ( const Cell* cell )
  { return static_cast<KatanaEngine*>(ToolEngine::get(cell,staticGetName())); }


  KatanaEngine::KatanaEngine ( Cell* cell )
    : Super           (cell)
    , _viewer         (NULL)
    , _blocks         ()
    , _routingPlanes  ()
    , _negociateWindow(NULL)
    , _minimumWL      (0.0)
    , _shortDoglegs   ()
    , _symmetrics     ()
    , _stage          (StageNegociate)
    , _successState   (0)
  { }


  void  KatanaEngine::_postCreate ()
  {
    Super::_postCreate();

  //Entity::setMemoryLimit( 1024 ); // 1Gb.
    addMeasure<size_t>( "Gates"
                      , AllianceFramework::getInstancesCount(getCell()
                                                            ,AllianceFramework::IgnoreNonLogic
                                                            |AllianceFramework::TerminalNetlist
                                                            |AllianceFramework::Recursive) );

  // Flute: load POWV9.dat & POST9.dat
    Flute::readLUT( System::getPath( "coriolis_top" ).toString() );
    rsetNoExtractFlag( getCell() );
  }


  Configuration* KatanaEngine::_createConfiguration ()
  { return new Configuration(); }


  void  KatanaEngine::_runKatanaInit ()
  {
    dbo_ptr<Isobar::Script> script = Isobar::Script::create( "coriolis.katana.initHook" );
    script->addKwArgument( "katana"    , (PyObject*)PyKatanaEngine_Link(this) );
    script->runFunction  ( "katanaHook", getCell() );
  }


  void  KatanaEngine::digitalInit ( Flags flags )
  {
    cdebug_log(155,1) << "KatanaEngine::_initDataBase()" << endl;
    _runKatanaInit();

    setRoutingMode( DigitalMode );

    Super::chipPrep();

    setupChannelMode();
    setupGlobalGraph( flags );
    if (not isChannelStyle()) {
      setupRoutingPlanes();
    }
    setupSpecialNets();
    if (not setupPreRouteds()) {
      setState( Anabatic::EngineDriving );
      throw Error( "KatanaEngine::digitalInit(): All nets are already routed, doing nothing." );
    }
    cdebug_tabw(155,-1);
  }


  void  KatanaEngine::analogInit ()
  {
    cdebug_log(155,1) << "KatanaEngine::_initDataBase()" << endl;

    resetRoutingMode( DigitalMode );
    setRoutingMode( AnalogMode );

    Super::chipPrep();

    setupRoutingPlanes();
    _runKatanaInit();

    cdebug_tabw(155,-1);
  }


  void  KatanaEngine::setupChannelMode ()
  {
    cdebug_log(155,1) << "KatanaEngine::setupChannelMode()" << endl;

    RoutingGauge* rg = getConfiguration()->getRoutingGauge();
    if (isChannelStyle()) {
      cmess1 << "     - Running in channel mode." << endl;

      size_t maxDepth = rg->getDepth();
      if (maxDepth < 2) {
        throw Error( "KatanaEngine::setupChannelMode(): Layer gauge %s must contains at least two layers (%u)."
                   , getString(rg->getName()).c_str(), maxDepth );
      }
    }

    cdebug_tabw(155,-1);
  }


  void  KatanaEngine::setupRoutingPlanes ()
  {
    cdebug_log(155,1) << "KatanaEngine::setupRoutingPlanes()" << endl;

    bool sessionReUse = Session::isOpen();
    if (not sessionReUse) openSession();

    size_t maxDepth = getConfiguration()->getRoutingGauge()->getDepth();
    _routingPlanes.reserve( maxDepth );
    for ( size_t depth=0 ; depth < maxDepth ; depth++ ) {
      _routingPlanes.push_back( RoutingPlane::create( this, depth ) );
      cdebug_log(155,0) << _routingPlanes.back() << endl;
    }

    if (not sessionReUse) Session::close();
    cdebug_tabw(155,-1);
  }


  KatanaEngine* KatanaEngine::create ( Cell* cell )
  {
    AllianceFramework* af = AllianceFramework::get();
    for ( Instance* instance : cell->getInstances() ) {
      if (af->isPad(instance->getMasterCell())) {
        throw Error( "KatanaEngine::create(): Must not be run at chip level, but a corona level.\n"
                     "      Guessing \"%s\" is a chip because instance \"%s\" is a pad (\"%s\")."
                   , getString(cell->getName()).c_str()
                   , getString(instance->getName()).c_str()
                   , getString(instance->getMasterCell()->getName()).c_str()
                   );
      }
    }
    
    KatanaEngine* katana = new KatanaEngine ( cell );
    katana->_postCreate();

    return katana;
  }


  void  KatanaEngine::_preDestroy ()
  {
    cdebug_log(155,1) << "KatanaEngine::_preDestroy()" << endl;

    cmess1 << "  o  Deleting ToolEngine<" << getName() << "> from Cell <"
           << _cell->getName() << ">" << endl;

    if (getState() < EngineState::EngineGutted)
      setState( EngineState::EnginePreDestroying );

    _gutKatana();
    Super::_preDestroy();

    cmess2 << "     - RoutingEvents := " << RoutingEvent::getAllocateds() << endl;

    cdebug_tabw(155,-1);
  }


  KatanaEngine::~KatanaEngine ()
  { }


  const Name& KatanaEngine::getName () const
  { return _toolName; }


  Configuration* KatanaEngine::getConfiguration ()
  { return static_cast<Configuration*>( Super::getConfiguration() ); }


  const Configuration* KatanaEngine::getConfiguration () const
  { return static_cast<const Configuration*>( Super::getConfiguration() ); }


  uint32_t  KatanaEngine::getRipupLimit ( const TrackElement* segment ) const
  {
    if (segment->isBlockage()) return 0;

    if (segment->isNonPref    ()) return getConfiguration()->getRipupLimit( Configuration::NonPrefRipupLimit );
    if (segment->isStrap      ()) return getConfiguration()->getRipupLimit( Configuration::StrapRipupLimit );
    if (segment->isUnbreakable()) return getConfiguration()->getRipupLimit( Configuration::StrapRipupLimit );
    if (segment->isShortNet   ()) return getConfiguration()->getRipupLimit( Configuration::ShortNetRipupLimit );
    if (segment->isGlobal     ()) {
      vector<GCell*> gcells;
      segment->getGCells( gcells );
      if (gcells.size() > 2)
        return getConfiguration()->getRipupLimit( Configuration::LongGlobalRipupLimit );
      return getConfiguration()->getRipupLimit( Configuration::GlobalRipupLimit );
    }
    return getConfiguration()->getRipupLimit( Configuration::LocalRipupLimit );
  }


  RoutingPlane* KatanaEngine::getRoutingPlaneByIndex ( size_t index ) const
  {
    if (index >= getRoutingPlanesSize() ) return NULL;
    return _routingPlanes[index];
  }


  RoutingPlane* KatanaEngine::getRoutingPlaneByLayer ( const Layer* layer ) const
  {
    for ( size_t index=0 ; index < getRoutingPlanesSize() ; index++ ) {
      if (_routingPlanes[index]->getLayer() == layer)
        return _routingPlanes[index];
    }
    return NULL;
  }


  Track* KatanaEngine::getTrackByPosition ( const Layer* layer, DbU::Unit axis, uint32_t mode ) const
  {
    RoutingPlane* plane = getRoutingPlaneByLayer( layer );
    if (not plane) return NULL;

    return plane->getTrackByPosition( axis, mode );
  }


  DataSymmetric* KatanaEngine::getDataSymmetric ( Net* net )
  {
    auto idata = _symmetrics.find( net );
    if (idata != _symmetrics.end()) return (*idata).second;
    return NULL;
  }


  void  KatanaEngine::openSession ()
  { Session::_open(this); }


  void  KatanaEngine::setInterrupt ( bool state )
  {
    if (_negociateWindow) {
      _negociateWindow->setInterrupt( state ); 
      cerr << "Interrupt [CRTL+C] of " << this << endl;
    }
  }


  DataSymmetric* KatanaEngine::addDataSymmetric ( Net* net )
  {
    DataSymmetric* data = getDataSymmetric( net );
    if (data) {
      cerr << Error( "KatanaEngine::addDataSymmetric(): Try to add twice Net \"%s\" (ignored)."
                   , getString(net->getName()).c_str() ) << endl;
      return data;
    }

    data = DataSymmetric::create( net );
    if (data) {
      _symmetrics.insert( make_pair(net,data) );
      if (data->getSymNet())
        _symmetrics.insert( make_pair(data->getSymNet(),data) );
    }

    return data;
  }


  void  KatanaEngine::annotateGlobalGraph ()
  {
    cmess1 << "  o  Back annotate global routing graph." << endl;
  //DebugSession::open( 159, 160 );

    for ( size_t depth=0 ; depth<_routingPlanes.size() ; ++depth ) {
      RoutingPlane* rp = _routingPlanes[depth];
      if (rp->getLayerGauge()->getType() != Constant::Default) continue;
      if (rp->getLayerGauge()->getDepth() > getConfiguration()->getAllowedDepth()) continue;

      int    elementCapacity = 1;
      size_t tracksSize      = rp->getTracksSize();
      for ( size_t itrack=0 ; itrack<tracksSize ; ++itrack ) {
        Track*     track = rp->getTrackByIndex( itrack );
        DbU::Unit  axis  = track->getAxis();
        Flags      side  = (track->getDirection() == Flags::Vertical) ? Flags::NorthSide
                                                                      : Flags::EastSide;
        Point      source;
        Point      target;
        cdebug_log(159,0) << "Capacity from: " << track << endl;

        Interval uspan;
        for ( size_t ielement=0 ; ielement<track->getSize() ; ++ielement ) {
          TrackElement* element = track->getSegment( ielement );
         
          if (element->getNet() == NULL) {
            cdebug_log(159,0) << "Reject capacity from (not Net): " << element << endl;
            continue;
          }
          if (   (not element->isFixed())
             and (not element->isBlockage())
             and (not element->isUserDefined()) ) {
            cmess2 << "Reject capacity from (neither fixed, blockage nor user defined): " << element << endl;
            continue;
          }

          cdebug_log(159,0) << "Capacity from: " << element << ":" << elementCapacity << endl;
          Interval  segmentUSpan = Interval( element->getSourceU(), element->getTargetU() );
          if (uspan.isEmpty()) {
            uspan = segmentUSpan;
            continue;
          } else {
            if (uspan.contains(segmentUSpan)) continue;
            if (uspan.intersect(segmentUSpan)) {
              uspan.merge( segmentUSpan );
              continue;
            }
          }

          cdebug_log(159,0) << "  Processing whole segment set, span " << uspan << endl;
          if (track->getDirection() == Flags::Vertical) {
            source = Point( axis, uspan.getVMin() );
            target = Point( axis, uspan.getVMax() );
          } else {
            source = Point( uspan.getVMin(), axis );
            target = Point( uspan.getVMax(), axis );
          }

          GCellsUnder gcells = getGCellsUnder( source, target );
          if (not gcells->empty()) {
            cdebug_log(159,0) << "  -> Under " << gcells->size() << " 0:" << gcells->gcellAt(0) << endl;
            cdebug_log(159,0) << "  -> source=" << source << " target=" << target << endl;
            for ( size_t i=0 ; i<gcells->size()-1 ; ++i ) {
              Edge* edge = gcells->gcellAt(i)->getEdgeAt( side, axis );
              edge->decreaseCapacity( elementCapacity, depth );
            //edge->reserveCapacity( elementCapacity );
            }
          }

          uspan = segmentUSpan;
        }

        if (not uspan.isEmpty()) {
          if (track->getDirection() == Flags::Vertical) {
            source = Point( axis, uspan.getVMin() );
            target = Point( axis, uspan.getVMax() );
          } else {
            source = Point( uspan.getVMin(), axis );
            target = Point( uspan.getVMax(), axis );
          }

          GCellsUnder gcells = getGCellsUnder( source, target );
          if (not gcells->empty()) {
            for ( size_t i=0 ; i<gcells->size()-1 ; ++i ) {
              Edge* edge = gcells->gcellAt(i)->getEdgeAt( side, axis );
              edge->decreaseCapacity( elementCapacity, depth );
            //edge->reserveCapacity( elementCapacity );
            }
          }
        }
      }
    }

    if (Session::isChannelStyle()) {
      for ( GCell* gcell : getGCells() ) {
        if (not gcell->isStdCellRow()) continue;
      
        set<DbU::Unit>  terminalsX;
        for ( Component* component : getCell()->getComponentsUnder(gcell->getBoundingBox()) ) {
          RoutingPad* rp = dynamic_cast<RoutingPad*>( component );
          if (rp) terminalsX.insert( rp->getX() );
        }

        unsigned int capacity = 0;
        Edge*        edge     = gcell->getNorthEdge();
        if (edge) {
          capacity = edge->getCapacity();
          if (terminalsX.size() < capacity) capacity = terminalsX.size();
          edge->reserveCapacity( capacity );
        }
        edge = gcell->getSouthEdge();
        if (edge) {
          capacity = edge->getCapacity();
          if (terminalsX.size()/2 < capacity) capacity = terminalsX.size()/2;
          edge->reserveCapacity( capacity );
        }
      }
    } else {
      uint32_t hReservedMin = getConfiguration()->getHTracksReservedMin();
      uint32_t vReservedMin = getConfiguration()->getVTracksReservedMin();

      for ( GCell* gcell : getGCells() ) {
        if (not gcell->isMatrix()) continue;

        for ( Edge* edge : gcell->getEdges( Flags::NorthSide) ) {
          if (edge->getReservedCapacity() < vReservedMin) {
            edge->reserveCapacity( vReservedMin - edge->getReservedCapacity() );
          }
        }
        for ( Edge* edge : gcell->getEdges( Flags::EastSide) ) {
          if (edge->getReservedCapacity() < hReservedMin)
            edge->reserveCapacity( hReservedMin - edge->getReservedCapacity()  );
        }
        gcell->postGlobalAnnotate();
      }
    }

    ostringstream result;
    result << EdgeCapacity::getAllocateds() << " (" << getEdgeCapacities().size() << "u)";
    cmess1 << ::Dots::asString("     - Edge capacities",result.str()) << endl;

  //DebugSession::close();
  }


  void  KatanaEngine::runNegociate ( Flags flags )
  {
    if (_negociateWindow) return;

    addMeasure<size_t>( "GCells", getGCells().size() );

    startMeasures();
    openSession();

    _negociateWindow = NegociateWindow::create( this );
    _negociateWindow->setGCells( getGCells() );
    _negociateWindow->run( flags );
    _negociateWindow->destroy();
    _negociateWindow = NULL;
    getCell()->setTerminalNetlist( true );

    Session::close();
    stopMeasures();
  //if ( _editor ) _editor->refresh ();

    printMeasures( "algo" );

    openSession();
    uint32_t overlaps             = 0;
    // size_t       hTracksReservedLocal = getHTracksReservedLocal();
    // size_t       vTracksReservedLocal = getVTracksReservedLocal();

    // if (cparanoid.enabled()) {
    //   cparanoid << "  o  Post-checking Katana capacity overload h:" << hTracksReservedLocal
    //             << " v:." << vTracksReservedLocal << endl;
    //   getGCellGrid()->checkEdgeOverflow( hTracksReservedLocal, vTracksReservedLocal );
    // }

    _check( overlaps );
    Session::close();

    setDetailedRoutingSuccess( isDetailedRoutingSuccess() and (overlaps == 0) );
  }


  void  KatanaEngine::runTest ()
  {
#if PUT_HERE_WHATEVER_YOU_WANT_TO_TEST
    if (getCell()->getName() != "gmchamla")
      throw Error( "KatanaEngine::runTest() Work only on \"gmchamla\" (loaded:\"%s\")."
                 , getString(getCell()->getName()).c_str()
                 );

    setSymmetricSelf( getCell(), "Vc"  );
    setSymmetricSelf( getCell(), "vb5" );
    setSymmetricSelf( getCell(), "vb7" );
    setSymmetricPair( getCell(), "ampp_73", "ampn_72" );
    setSymmetricPair( getCell(), "ampp_71", "ampn_71" );
    setSymmetricPair( getCell(), "m2n_in" , "m2p_in"  );
    setSymmetricPair( getCell(), "ampp_4" , "ampn_4"  );
    setSymmetricPair( getCell(), "ampp_2" , "ampn_2"  );
    setSymmetricPair( getCell(), "ampp_1" , "ampn_1"  );
#endif
  }


  void  KatanaEngine::printCompletion () const
  {
    size_t                 routeds          = 0;
    uint64_t               totalWireLength  = 0;
    uint64_t               routedWireLength = 0;
    uint32_t               hunrouteds       = 0;
    uint32_t               vunrouteds       = 0;
    vector<TrackElement*>  unrouteds;
    vector<TrackElement*>  reduceds;
    ostringstream          result;
    bool                   isSymbolic =
      const_cast<KatanaEngine*>(this)->getConfiguration()->getRoutingGauge()->isSymbolic();
  // Max symbolic wire: 100000L, max real wire: 5mm.
    uint64_t               maxWL = (isSymbolic) ? DbU::fromLambda( 100000.0 )
                                                : DbU::fromPhysical( 5.0, DbU::UnitPower::Milli );

    AutoSegmentLut::const_iterator ilut = _getAutoSegmentLut().begin();
    for ( ; ilut != _getAutoSegmentLut().end() ; ilut++ ) {
      TrackElement* segment = _lookup( ilut->second );
      if (segment == NULL) continue;

      uint64_t wl = 0;
      if (isSymbolic)
        wl = (uint64_t)DbU::toLambda( segment->getLength() );
      else
        wl = (uint64_t)DbU::toPhysical( segment->getLength(), DbU::UnitPower::Nano );
      if (wl > maxWL) {
        cerr << Error("KatanaEngine::printCompletion(): Suspiciously long wire: %llu for %p:%s"
                     ,wl,ilut->first,getString(segment).c_str()) << endl;
        continue;
      }

      if (segment->isBlockage()) continue;
      if (segment->isReduced()) reduceds.push_back( segment );

      totalWireLength += wl;
      if ( (segment->getTrack() != NULL) or (segment->isReduced()) ) {
        routeds++;
        routedWireLength += wl;
        continue;
      }

      unrouteds.push_back( segment );
      if (segment->isHorizontal()) ++hunrouteds;
      if (segment->isVertical  ()) ++vunrouteds;
    }

    float segmentRatio     = (float)(routeds)          / (float)(routeds+unrouteds.size()) * 100.0;
    float wireLengthRatio  = (float)(routedWireLength) / (float)(totalWireLength)   * 100.0;

    setDetailedRoutingSuccess( unrouteds.empty() );

    if (not unrouteds.empty()) {
      cerr << "  o  Routing did not complete, unrouted segments:" << endl;
      for ( size_t i=0; i<unrouteds.size() ; ++i ) {
        cerr << "   " << dec << setw(4) << (i+1) << "| " << unrouteds[i] << endl;
      }
    }

    // if (not reduceds.empty()) {
    //   cerr << "  o  Reduced segments:" << endl;
    //   for ( size_t i=0; i<reduceds.size() ; ++i ) {
    //     cerr << "   " << dec << setw(4) << (i+1) << "| " << reduceds[i] << endl;
    //   }
    // }

  //DbU::Unit wlMetricUnit = const_cast<KatanaEngine*>(this)->getConfiguration()->getPitch( 1 );
    DbU::Unit wlMetricUnit = const_cast<KatanaEngine*>(this)
      ->getConfiguration()->getCellGauge()->getSliceHeight();
    float     wlMetric     = 1.0;
    string    units        = "L";
    if (isSymbolic) {
      wlMetric = DbU::toLambda( wlMetricUnit );
    } else {
      units = "um";
      totalWireLength  /= 1000;
      routedWireLength /= 1000;
      wlMetric = DbU::toPhysical( wlMetricUnit, DbU::UnitPower::Nano ) / 1000;
    }
    float normedWireLength = (float)(totalWireLength) / wlMetric;

    result << setprecision(4) << segmentRatio
           << "% [" << routeds << "+" << unrouteds.size() << "]";
    cmess1 << Dots::asString( "     - Track Segment Completion Ratio", result.str() ) << endl;

    result.str("");
    result << setprecision(4) << wireLengthRatio
           << "% [" << totalWireLength << units << "+"
           << (totalWireLength - routedWireLength) << "]";
    cmess1 << Dots::asString( "     - Wire Length Completion Ratio", result.str() ) << endl;

    float expandRatio = 1.0;
    if (_minimumWL != 0.0) {
      expandRatio = ((totalWireLength-_minimumWL) / _minimumWL) * 100.0;

      result.str("");
      result << setprecision(3) << expandRatio
             << "% [min:" << setprecision(9) << _minimumWL << units << "]";
      cmess1 << Dots::asString( "     - Wire Length Expand Ratio", result.str() ) << endl;
    }

    for ( RoutingPlane* rp : _routingPlanes ) {
      DbU::Unit  rpTotalWL = rp->getTotalWL();
      DbU::Unit  rpUsedWL  = rp->getUsedWL ();
      
      result.str("");
      result << fixed << setprecision(2) << (((float)rpUsedWL / rpTotalWL) * 100.0) << "%";

      ostringstream title;
      title << "     - " << rp->getLayer()->getName() << " WL usage";
      cmess1 << Dots::asString( title.str(), result.str() ) << endl;
    }
    

    result.str("");
    result << fixed << setprecision(0) << normedWireLength;
    cmess1 << Dots::asString( "     - Normed Wire Length", result.str() ) << endl;
    result.str("");
    result << (normedWireLength / getNetDatas().size());
    cmess1 << Dots::asString( "     - Normed Wire Length / Net", result.str() ) << endl;
    result << defaultfloat;
    result.str("");
    result << defaultfloat << AutoSegment::getMovedUp()
           << " (" << setprecision(2) << fixed << setfill(' ') << right << setw(5)
           << ((float)AutoSegment::getMovedUp() / AutoSegment::getAllocateds()) * 100.0 << "%)";
    cmess1 << Dots::asString( "     - Moved up segments", result.str() ) << endl;

    float ratio = 0.0;
    if (not unrouteds.empty())
      ratio = ((float)hunrouteds / (float)unrouteds.size()) * 100.0;
    result.str("");
    result << ratio << "% [" << hunrouteds << "]";
    cmess1 << Dots::asString( "     - Unrouted horizontals", result.str() ) << endl;

    if (not unrouteds.empty())
      ratio = ((float)vunrouteds / (float)unrouteds.size()) * 100.0;
    result.str("");
    result << ratio << "% [" << vunrouteds << "]";
    cmess1 << Dots::asString( "     - Unrouted verticals", result.str() ) << endl;

    addMeasure<size_t>  ( "Segs"   , routeds+unrouteds.size() );
    addMeasure<uint64_t>( "DWL"    , totalWireLength                  , 12 );
    addMeasure<uint64_t>( "fWL"    , totalWireLength-routedWireLength , 12 );
    addMeasure<double>  ( "WLER(%)", expandRatio );
  }


  void  KatanaEngine::dumpMeasures ( ostream& out ) const
  {
    vector<Name> measuresLabels;
    measuresLabels.push_back( getMeasureLabel("Gates"  ) );
    measuresLabels.push_back( getMeasureLabel("GCells" ) );
  //measuresLabels.push_back( getMeasureLabel("knikT"  ) );
  //measuresLabels.push_back( getMeasureLabel("knikS"  ) );
  //measuresLabels.push_back( getMeasureLabel("GWL"    ) );
    measuresLabels.push_back( "Etesian::EtesianEngine.placeT" );
    measuresLabels.push_back( getMeasureLabel("Area"   ) );
    measuresLabels.push_back( getMeasureLabel("Sat"    ) );
    measuresLabels.push_back( getMeasureLabel("loadT"  ) );
    measuresLabels.push_back( getMeasureLabel("loadS"  ) );
    measuresLabels.push_back( getMeasureLabel("H-ovE"  ) );
    measuresLabels.push_back( getMeasureLabel("V-ovE"  ) );
    measuresLabels.push_back( getMeasureLabel("Globals") );
    measuresLabels.push_back( getMeasureLabel("Edges"  ) );
    measuresLabels.push_back( getMeasureLabel("assignT") );
    measuresLabels.push_back( getMeasureLabel("algoT"  ) );
    measuresLabels.push_back( getMeasureLabel("algoS"  ) );
    measuresLabels.push_back( getMeasureLabel("finT"   ) );
    measuresLabels.push_back( getMeasureLabel("Segs"   ) );
    measuresLabels.push_back( getMeasureLabel("DWL"    ) );
    measuresLabels.push_back( getMeasureLabel("fWL"    ) );
    measuresLabels.push_back( getMeasureLabel("WLER(%)") );
    measuresLabels.push_back( getMeasureLabel("Events" ) );
    measuresLabels.push_back( getMeasureLabel("UEvents") );

    const MeasuresSet* measures = Measures::get( getCell() );

    if (measures) {
      out << "#" << endl;
      out << "# " << getCell()->getName() << endl;
      out << measures->toStringHeaders(measuresLabels) << endl;
      for ( size_t i=0 ; i<=getPassNumber() ; ++i ) {
        out << measures->toStringDatas(measuresLabels,i) << endl;
      
        measures->toGnuplot( getMeasureLabel("GCells Density Histogram")
                           , i, getString(getCell()->getName()) );
      }
    } else {
      cerr << Warning( "KatanaEngine::dumpMeasures(): \"%s\" has no measures yet."
                     , getString(getCell()->getName()).c_str()
                     ) << endl;
    }
  }


  void  KatanaEngine::dumpMeasures () const
  {
    cmess1 << "  o  Dumping measurements." << endl;

    ostringstream path;
    path << getCell()->getName() << ".katana.dat";

    ofstream sfile ( path.str().c_str() );
    dumpMeasures( sfile );
    sfile.close();
  }


  bool  KatanaEngine::_check ( uint32_t& overlap, const char* message ) const
  {
    cmess1 << "  o  Checking Katana Database coherency." << endl;

    bool coherency = true;
    coherency = coherency and Super::_check( message );
    for ( size_t i=0 ; i<_routingPlanes.size() ; i++ )
      coherency = _routingPlanes[i]->_check(overlap) and coherency;

    Anabatic::Session* anbtSession = Session::base ();
    for( Net* net : getCell()->getNets() ) {
      for( Segment* segment : net->getComponents().getSubSet<Segment*>() ) {
        AutoSegment* autoSegment = anbtSession->lookup( segment );
        if (not autoSegment) continue;
        if (not autoSegment->isCanonical()) continue;
        if (autoSegment->isReduced()) continue;

        TrackElement* trackSegment = Session::lookup( segment );
        if (not trackSegment) {
          if (   isChannelStyle()
             and autoSegment->isFixed()
             and autoSegment->isHorizontal()
             and autoSegment->isNonPref())
            continue;
          coherency = false;
          cerr << Bug( "%p %s without Track Segment"
                     , autoSegment
                     , getString(autoSegment).c_str() ) << endl;
        } else
          trackSegment->_check();
      }
    }

    return coherency;
  }


  void  KatanaEngine::finalizeLayout ()
  {
    cdebug_log(155,0) << "KatanaEngine::finalizeLayout()" << endl;
    if (getState() > Anabatic::EngineDriving) return;

    cdebug_tabw(155,1);
    setState( Anabatic::EngineDriving );
    _gutKatana();

    Super::finalizeLayout();
    cdebug_log(155,0) << "State: " << getState() << endl;

    getCell()->setFlags( Cell::Flags::Routed );

    cdebug_tabw(155,-1);
  }


  void  KatanaEngine::_gutKatana ()
  {
    cdebug_log(155,1) << "KatanaEngine::_gutKatana()" << endl;
    cdebug_log(155,0)   << "State: " << getState() << endl;

    if (getState() < EngineState::EngineGutted) {
      openSession();

      for ( Block* block : _blocks ) delete block;
      _blocks.clear();

      size_t maxDepth = std::min( getConfiguration()->getRoutingGauge()->getDepth(), _routingPlanes.size() );
      for ( size_t depth=0 ; depth < maxDepth ; depth++ ) {
        _routingPlanes[depth]->destroy();
      }
      _routingPlanes.clear();
      
      while ( not _symmetrics.empty() ) {
        auto element = _symmetrics.begin();

        if (element->first == element->second->getNet()) delete element->second;
        _symmetrics.erase( element->first );
      }

      Session::close();
    }

    cdebug_tabw(155,-1);
  }


  void  KatanaEngine::resetRouting ()
  {
    _gutKatana();

    UpdateSession::open();
    for ( Net* net : getCell()->getNets() ) {
      vector<Component*> removeds;

      for ( Component* component : net->getComponents() ) {
        if (dynamic_cast<Plug   *>(component)) continue;
        if (dynamic_cast<Contact*>(component)) removeds.push_back( component );
      }
      for ( Component* component : removeds ) component->destroy();
    }

    for ( Net* net : getCell()->getNets() ) {
      vector<Component*> removeds;

      for ( Component* component : net->getComponents() ) {
        if (dynamic_cast<Plug*>(component)) continue;
        removeds.push_back( component );
      }
      for ( Component* component : removeds ) component->destroy();
    }

    setState( Anabatic::EngineCreation );
    setGlobalRoutingSuccess  ( false );
    setDetailedRoutingSuccess( false );
    UpdateSession::close();
    
    getCell()->resetFlags( Cell::Flags::Routed );
  }
  

  TrackElement* KatanaEngine::_lookup ( Segment* segment ) const
  {
    AutoSegment* autoSegment = Super::_lookup( segment );
    if (not autoSegment or not autoSegment->isCanonical()) return NULL;

    return _lookup( autoSegment );
  }


  void  KatanaEngine::_check ( Net* net ) const
  {
    cerr << "     o  Checking " << net << endl;

    for( Segment* segment : net->getComponents().getSubSet<Segment*>() ) {
      TrackElement* trackSegment = _lookup( segment );
      if (trackSegment) {
        trackSegment->_check();

        AutoContact* autoContact = trackSegment->base()->getAutoSource();
        if (autoContact) autoContact->checkTopology ();

        autoContact = trackSegment->base()->getAutoTarget();
        if (autoContact) autoContact->checkTopology ();
      }
    }
  }


  string  KatanaEngine::_getTypeName () const
  { return "Katana::KatanaEngine"; }


  string  KatanaEngine::_getString () const
  {
    ostringstream  os;
    os << "<" << "KatanaEngine " << _cell->getName () << ">";
    return os.str();
  }


  Record* KatanaEngine::_getRecord () const
  {
    Record* record = Super::_getRecord ();
                                     
    if (record) {
      record->add( getSlot( "_blocks"       , &_blocks ) );
      record->add( getSlot( "_routingPlanes", &_routingPlanes ) );
      record->add( getSlot( "_symmetrics"   , &_symmetrics    ) );
    }
    return record;
  }


} // Katana namespace.
