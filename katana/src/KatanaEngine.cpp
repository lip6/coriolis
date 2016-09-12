// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
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
#include "vlsisapd/utilities/Path.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Layer.h"
#include "hurricane/Net.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/viewer/Script.h"
#include "crlcore/Measures.h"
#include "anabatic/AutoContact.h"
#include "katana/DataNegociate.h"
#include "katana/RoutingPlane.h"
#include "katana/Session.h"
#include "katana/TrackSegment.h"
#include "katana/NegociateWindow.h"
#include "katana/KatanaEngine.h"
#include "katana/PyKatanaEngine.h"


namespace Katana {

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::dec;
  using std::setw;
  using std::left;
  using std::ostream;
  using std::ofstream;
  using std::ostringstream;
  using std::setprecision;
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
  using Hurricane::Box;
  using Hurricane::Torus;
  using Hurricane::Layer;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::NetRoutingExtension;
  using Hurricane::Cell;
  using CRL::System;
  using CRL::addMeasure;
  using CRL::Measures;
  using CRL::MeasuresSet;
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
    , _configuration  (new Configuration())
    , _routingPlanes  ()
    , _negociateWindow(NULL)
    , _minimumWL      (0.0)
    , _toolSuccess    (false)
  { }


  void  KatanaEngine::_postCreate ()
  {
    Super::_postCreate ();
  }


  void  KatanaEngine::_runKatanaInit ()
  {
    Utilities::Path pythonSitePackages = System::getPath("pythonSitePackages");
    Utilities::Path systemConfDir      = pythonSitePackages / "katana";
    Utilities::Path systemConfFile     = systemConfDir      / "katanaInit.py";

    if (systemConfFile.exists()) {
      Isobar::Script::addPath( systemConfDir.toString() );

      dbo_ptr<Isobar::Script> script = Isobar::Script::create( systemConfFile.stem().toString() );
      script->addKwArgument( "katana"    , (PyObject*)PyKatanaEngine_Link(this) );
      script->runFunction  ( "katanaHook", getCell() );

      Isobar::Script::removePath( systemConfDir.toString() );
    } else {
      cerr << Warning( "Katana system configuration file:\n  <%s> not found."
                     , systemConfFile.toString().c_str() ) << endl;
    }
  }


  void  KatanaEngine::digitalInit ()
  {
    cdebug_log(155,1) << "KatanaEngine::_initDataBase()" << endl;

    setupGlobalGraph( Flags::NoFlags );
    setupSpecialNets();
    setupPreRouteds();
    setupPowerRails();
    protectRoutingPads();
    _runKatanaInit();

    cdebug_tabw(155,-1);
  }


  KatanaEngine* KatanaEngine::create ( Cell* cell )
  {
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
  { delete _configuration; }


  const Name& KatanaEngine::getName () const
  { return _toolName; }


  Configuration* KatanaEngine::getConfiguration ()
  { return _configuration; }


  unsigned int  KatanaEngine::getRipupLimit ( const TrackElement* segment ) const
  {
    if (segment->isBlockage()) return 0;

    if (segment->isStrap ()) return _configuration->getRipupLimit( Configuration::StrapRipupLimit );
    if (segment->isGlobal()) {
      vector<GCell*> gcells;
      segment->getGCells( gcells );
      if (gcells.size() > 2)
        return _configuration->getRipupLimit( Configuration::LongGlobalRipupLimit );
      return _configuration->getRipupLimit( Configuration::GlobalRipupLimit );
    }
    return _configuration->getRipupLimit( Configuration::LocalRipupLimit );
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


  Track* KatanaEngine::getTrackByPosition ( const Layer* layer, DbU::Unit axis, unsigned int mode ) const
  {
    RoutingPlane* plane = getRoutingPlaneByLayer( layer );
    if (not plane) return NULL;

    return plane->getTrackByPosition( axis, mode );
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


  void  KatanaEngine::annotateGlobalGraph ()
  {
    cmess1 << "  o  Back annotate global routing graph." << endl;

    for ( size_t depth=0 ; depth<_routingPlanes.size() ; ++depth ) {
      RoutingPlane* rp = _routingPlanes[depth];
      if (rp->getLayerGauge()->getType() == Constant::PinOnly) continue;
      if (rp->getLayerGauge()->getDepth() > getConfiguration()->getAllowedDepth()) continue;

      size_t tracksSize = rp->getTracksSize();
      for ( size_t itrack=0 ; itrack<tracksSize ; ++itrack ) {
        Track* track = rp->getTrackByIndex( itrack );

        cdebug_log(159,0) << "Capacity from: " << track << endl;

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

          Segment*   segment = element->getSegment();
          Flags      side    = Flags::EastSide;
          DbU::Unit  axis    = segment->getY();
          if (track->getDirection() == Flags::Vertical) {
            side = Flags::NorthSide;
            axis = segment->getX();
          }

          int elementCapacity = -1;
          cdebug_log(159,0) << "Capacity from: " << element << ":" << elementCapacity << endl;

          GCellsUnder gcells  = getGCellsUnder( segment );
          if (not gcells->empty()) {
            for ( size_t i=0 ; i<gcells->size()-1 ; ++i )
              gcells->gcellAt(i)->getEdgeAt( side, axis )->incCapacity( elementCapacity );
          }
        }
      }
    }
  }


  void  KatanaEngine::runNegociate ( unsigned int flags )
  {
    if (_negociateWindow) return;

    startMeasures();
    openSession();

    _negociateWindow = NegociateWindow::create( this );
    _negociateWindow->setGCells( getGCells() );
    _computeCagedConstraints();
    _negociateWindow->run( flags );
    _negociateWindow->destroy();
    _negociateWindow = NULL;

    Session::close();
    stopMeasures();
  //if ( _editor ) _editor->refresh ();

    printMeasures( "algo" );

    openSession();
    unsigned int overlaps             = 0;
    // size_t       hTracksReservedLocal = getHTracksReservedLocal();
    // size_t       vTracksReservedLocal = getVTracksReservedLocal();

    // if (cparanoid.enabled()) {
    //   cparanoid << "  o  Post-checking Katana capacity overload h:" << hTracksReservedLocal
    //             << " v:." << vTracksReservedLocal << endl;
    //   getGCellGrid()->checkEdgeOverflow( hTracksReservedLocal, vTracksReservedLocal );
    // }

    _check( overlaps );
    Session::close();

    _toolSuccess = _toolSuccess and (overlaps == 0);
  }


  void  KatanaEngine::printCompletion () const
  {
    size_t                 routeds          = 0;
    unsigned long long     totalWireLength  = 0;
    unsigned long long     routedWireLength = 0;
    vector<TrackElement*>  unrouteds;
    vector<TrackElement*>  reduceds;
    ostringstream          result;

    AutoSegmentLut::const_iterator ilut = _getAutoSegmentLut().begin();
    for ( ; ilut != _getAutoSegmentLut().end() ; ilut++ ) {
      TrackElement* segment = _lookup( ilut->second );
      if (segment == NULL) continue;

      unsigned long long wl = (unsigned long long)DbU::toLambda( segment->getLength() );
      if (wl > 100000) {
        cerr << Error("KatanaEngine::printCompletion(): Suspiciously long wire: %llu for %p:%s"
                     ,wl,ilut->first,getString(segment).c_str()) << endl;
        continue;
      }

      if (segment->isFixed() or segment->isBlockage()) continue;
      if (segment->isReduced()) reduceds.push_back( segment );

      totalWireLength += wl;
      if ( (segment->getTrack() != NULL) or (segment->isReduced()) ) {
        routeds++;
        routedWireLength += wl;
        continue;
      }

      unrouteds.push_back( segment );
    }

    float segmentRatio    = (float)(routeds)          / (float)(routeds+unrouteds.size()) * 100.0;
    float wireLengthRatio = (float)(routedWireLength) / (float)(totalWireLength)   * 100.0;

    _toolSuccess = (unrouteds.empty());

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

    result << setprecision(4) << segmentRatio
           << "% [" << routeds << "+" << unrouteds.size() << "]";
    cmess1 << Dots::asString( "     - Track Segment Completion Ratio", result.str() ) << endl;

    result.str("");
    result << setprecision(4) << wireLengthRatio
            << "% [" << totalWireLength << "+"
            << (totalWireLength - routedWireLength) << "]";
    cmess1 << Dots::asString( "     - Wire Length Completion Ratio", result.str() ) << endl;

    float expandRatio = 1.0;
    if (_minimumWL != 0.0) {
      expandRatio = ((totalWireLength-_minimumWL) / _minimumWL) * 100.0;

      result.str("");
      result << setprecision(3) << expandRatio << "% [min:" << setprecision(9) << _minimumWL << "]";
      cmess1 << Dots::asString( "     - Wire Length Expand Ratio", result.str() ) << endl;
    }

    addMeasure<size_t>            ( getCell(), "Segs"   , routeds+unrouteds.size() );
    addMeasure<unsigned long long>( getCell(), "DWL(l)" , totalWireLength                  , 12 );
    addMeasure<unsigned long long>( getCell(), "fWL(l)" , totalWireLength-routedWireLength , 12 );
    addMeasure<double>            ( getCell(), "WLER(%)", (expandRatio-1.0)*100.0 );
}


  void  KatanaEngine::dumpMeasures ( ostream& out ) const
  {
    vector<Name> measuresLabels;
    measuresLabels.push_back( "Gates"   );
    measuresLabels.push_back( "GCells"  );
    measuresLabels.push_back( "knikT"   );
    measuresLabels.push_back( "knikS"   );
    measuresLabels.push_back( "GWL(l)"  );
    measuresLabels.push_back( "Area(l2)");
    measuresLabels.push_back( "Sat."    );
    measuresLabels.push_back( "loadT"   );
    measuresLabels.push_back( "loadS"   );
    measuresLabels.push_back( "Globals" );
    measuresLabels.push_back( "Edges"   );
    measuresLabels.push_back( "assignT" );
    measuresLabels.push_back( "algoT"   );
    measuresLabels.push_back( "algoS"   );
    measuresLabels.push_back( "finT"    );
    measuresLabels.push_back( "Segs"    );
    measuresLabels.push_back( "DWL(l)"  );
    measuresLabels.push_back( "fWL(l)"  );
    measuresLabels.push_back( "WLER(%)" );
    measuresLabels.push_back( "Events"  );
    measuresLabels.push_back( "UEvents" );

    const MeasuresSet* measures = Measures::get( getCell() );

    out << "#" << endl;
    out << "# " << getCell()->getName() << endl;
    out << measures->toStringHeaders(measuresLabels) << endl;
    out << measures->toStringDatas  (measuresLabels) << endl;

    measures->toGnuplot( "GCells Density Histogram", getString(getCell()->getName()) );
  }


  void  KatanaEngine::dumpMeasures () const
  {
    ostringstream path;
    path << getCell()->getName() << ".knik-katana.dat";

    ofstream sfile ( path.str().c_str() );
    dumpMeasures( sfile );
    sfile.close();
  }


  bool  KatanaEngine::_check ( unsigned int& overlap, const char* message ) const
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

        TrackElement* trackSegment = Session::lookup( segment );
        if (not trackSegment) {
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

      size_t maxDepth = getConfiguration()->getRoutingGauge()->getDepth();
      for ( size_t depth=0 ; depth < maxDepth ; depth++ ) {
        _routingPlanes[depth]->destroy();
      }

      Session::close();
    }

    cdebug_tabw(155,-1);
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
      record->add( getSlot( "_routingPlanes", &_routingPlanes ) );
      record->add( getSlot( "_configuration",  _configuration ) );
    }
    return record;
  }


} // Katana namespace.
