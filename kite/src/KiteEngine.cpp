
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./KiteEngine.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <sstream>
#include  <iomanip>

#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Layer.h"
#include  "hurricane/Net.h"
#include  "hurricane/Pad.h"
#include  "hurricane/Plug.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Instance.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/UpdateSession.h"

#include  "knik/KnikEngine.h"
#include  "katabatic/AutoContact.h"
#include  "kite/DataNegociate.h"
#include  "kite/GCellGrid.h"
#include  "kite/RoutingPlane.h"
#include  "kite/Session.h"
#include  "kite/GCellRoutingSet.h"
#include  "kite/NegociateWindow.h"
#include  "kite/KiteEngine.h"


namespace Kite {


  using std::cout;
  using std::cerr;
  using std::endl;
  using std::setw;
  using std::left;
  using std::ostringstream;
  using std::setprecision;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::ForEachIterator;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Layer;
  using Hurricane::Cell;
  using Knik::KnikEngine;


  const char* missingRW =
    "%s :\n\n"
    "    Cell %s do not have any KiteEngine (or not yet created).\n";


// -------------------------------------------------------------------
// Class  :  "Kite::KiteEngine".


  Name  KiteEngine::_toolName = "Kite";


  const Name& KiteEngine::staticGetName ()
  { return _toolName; }


  KiteEngine* KiteEngine::get ( const Cell* cell )
  {
    return static_cast<KiteEngine*>(ToolEngine::get(cell,staticGetName()));
  }


  KiteEngine::KiteEngine ( const RoutingGauge* gauge, Cell* cell )
    : KatabaticEngine  (gauge,cell)
    , _knik            (NULL)
    , _obstacleNet     (NULL)
    , _configuration   (getKatabaticConfiguration())
    , _routingPlanes   ()
    , _kiteGrid        (NULL)
    , _negociateWindow (NULL)
    , _trackSegmentLut ()
    , _minimumWL       (0.0)
    , _toolSuccess     (false)
  {
  }


  void  KiteEngine::_postCreate ()
  {
    KatabaticEngine::_postCreate ();

#ifdef KNIK_NOT_EMBEDDED
    size_t maxDepth = getRoutingGauge()->getDepth();

    _kiteGrid  = GCellGrid::create ( this );

    _routingPlanes.reserve ( maxDepth );
    for ( size_t depth=0 ; depth < maxDepth ; depth++ ) {
      _routingPlanes.push_back ( RoutingPlane::create ( this, depth ) );
    }
#endif
  }


  KiteEngine* KiteEngine::create ( const RoutingGauge* gauge, Cell* cell )
  {
    KiteEngine* kite = new KiteEngine ( gauge, cell );

    kite->_postCreate ();
    return kite;
  }


  void  KiteEngine::_preDestroy ()
  {
    ltrace(90) << "KiteEngine::_preDestroy ()" << endl;
    ltracein(90);

    cmess1 << "  o  Deleting ToolEngine<" << getName() << "> from Cell <"
           << _cell->getName() << ">" << endl;

    if ( getState() < Katabatic::StateGutted )
      setState ( Katabatic::StatePreDestroying );

    _gutKite ();
    KatabaticEngine::_preDestroy ();

    cmess2 << "     - RoutingEvents := " << RoutingEvent::getAllocateds() << endl;

  //_knik->destroy ();

    ltraceout(90);
  }


  KiteEngine::~KiteEngine ()
  { }


  const Name& KiteEngine::getName () const
  { return _toolName; }


  Configuration* KiteEngine::getConfiguration ()
  { return &_configuration; }


  unsigned int  KiteEngine::getRipupLimit ( const TrackElement* segment ) const
  {
    if ( segment->isBlockage() ) return 0;

    if ( segment->getDataNegociate() ) {
      if ( segment->getDataNegociate()->isBorder() )
        return _configuration.getRipupLimit(Configuration::BorderRipupLimit);

      if ( segment->getDataNegociate()->isRing() )
        return _configuration.getRipupLimit(Configuration::GlobalRipupLimit);
    }

    if ( segment->isStrap () ) return _configuration.getRipupLimit(Configuration::StrapRipupLimit);
    if ( segment->isGlobal() ) {
      vector<GCell*> gcells;
      segment->getGCells(gcells);
      if ( gcells.size() > 2 )
        return _configuration.getRipupLimit(Configuration::LongGlobalRipupLimit);
      return _configuration.getRipupLimit(Configuration::GlobalRipupLimit);
    }
    return _configuration.getRipupLimit(Configuration::LocalRipupLimit);
  }


  RoutingPlane* KiteEngine::getRoutingPlaneByIndex ( size_t index ) const
  {
    if ( index >= getRoutingPlanesSize() ) return NULL;

    return _routingPlanes[index];
  }


  RoutingPlane* KiteEngine::getRoutingPlaneByLayer ( const Layer* layer ) const
  {
    for ( size_t index=0 ; index < getRoutingPlanesSize() ; index++ ) {
      if ( _routingPlanes[index]->getLayer() == layer )
        return _routingPlanes[index];
    }
    return NULL;
  }


  Track* KiteEngine::getTrackByPosition ( const Layer* layer, DbU::Unit axis, unsigned int mode ) const
  {
    RoutingPlane* plane = getRoutingPlaneByLayer ( layer );
    if ( !plane ) return NULL;

    return plane->getTrackByPosition ( axis, mode );
  }


  void  KiteEngine::setInterrupt ( bool state )
  {
    if ( _negociateWindow ) {
      _negociateWindow->setInterrupt ( state ); 
      cerr << "Interrupt [CRTL+C] of " << this << endl;
    }
  }


  void  KiteEngine::createGlobalGraph ( unsigned int mode )
  {
    Cell* cell = getCell();
    if ( not _knik ) {
      if ( cell->getRubbers().getFirst() == NULL )
        cell->flattenNets ( (mode==BuildGlobalSolution) );
      KnikEngine::setEdgeCapacityPercent ( getEdgeCapacityPercent() );
      _knik = KnikEngine::create ( cell
                                 , 1     // _congestion
                                 , 2     // _preCongestion
                                 , false // _benchMode
                                 , true  // _useSegments
                                 , 2.5   // _edgeCost
                                 );
    //if ( mode == LoadGlobalSolution )
      _knik->createRoutingGraph ();
    }
  }


  void  KiteEngine::createDetailedGrid ()
  {
    KatabaticEngine::createDetailedGrid ();
    _kiteGrid  = GCellGrid::create ( this );

    size_t maxDepth = getRoutingGauge()->getDepth();

    _routingPlanes.reserve ( maxDepth );
    for ( size_t depth=0 ; depth < maxDepth ; depth++ ) {
      _routingPlanes.push_back ( RoutingPlane::create ( this, depth ) );
    }
  }


  void  KiteEngine::saveGlobalSolution ()
  {
    if ( getState() < Katabatic::StateGlobalLoaded )
      throw Error ("KiteEngine::saveGlobalSolution() : global routing not present yet.");

    if ( getState() > Katabatic::StateGlobalLoaded )
      throw Error ("KiteEngine::saveGlobalSolution() : cannot save after detailed routing.");

    _knik->saveSolution ();
  }


  void  KiteEngine::annotateGlobalGraph ()
  {
    cmess1 << "  o  Back annotate global routing graph." << endl;

    for ( size_t depth=0 ; depth<_routingPlanes.size() ; ++depth ) {
      RoutingPlane* rp = _routingPlanes[depth];
      if ( rp->getLayerGauge()->getType() == Constant::PinOnly ) continue;

      size_t tracksSize = rp->getTracksSize();
      for ( size_t itrack=0 ; itrack<tracksSize ; ++itrack ) {
        Track*        track   = rp->getTrackByIndex ( itrack );
        TrackElement* element = track->getSegment ( (size_t)0 );

        if ( element == NULL ) continue;
        if ( element->getNet() == NULL ) continue;
        if ( not element->getNet()->isSupply() ) continue;

        cinfo << "Capacity from: " << element << endl;

        if ( track->getDirection() == Constant::Horizontal ) {
          for ( ; element != NULL ; element = element->getNext() ) {
            GCell* gcell = _kiteGrid->getGCell ( Point(element->getSourceU(),track->getAxis()) );
            GCell* end   = _kiteGrid->getGCell ( Point(element->getTargetU(),track->getAxis()) );
            GCell* right = NULL;
            if ( not gcell ) {
              cerr << Warning("annotageGlobalGraph(): TrackElement outside GCell grid.") << endl;
              continue;
            }
            while ( gcell and (gcell != end) ) {
              right = gcell->getRight();
              if ( right == NULL ) break;

              _knik->increaseEdgeCapacity ( gcell->getColumn()
                                          , gcell->getRow()
                                          , right->getColumn()
                                          , right->getRow()
                                          , -1 );
              gcell = right;
            }
          }
        } else {
          for ( ; element != NULL ; element = element->getNext() ) {
            GCell* gcell = _kiteGrid->getGCell ( Point(track->getAxis(),element->getSourceU()) );
            GCell* end   = _kiteGrid->getGCell ( Point(track->getAxis(),element->getTargetU()) );
            GCell* up    = NULL;
            if ( not gcell ) {
              cerr << Warning("annotageGlobalGraph(): TrackElement outside GCell grid.") << endl;
              continue;
            }
            while ( gcell and (gcell != end) ) {
              up = gcell->getUp();
              if ( up == NULL ) break;

              _knik->increaseEdgeCapacity ( gcell->getColumn()
                                          , gcell->getRow()
                                          , up->getColumn()
                                          , up->getRow()
                                          , -1 );

              gcell = up;
            }
          }
        }
      }
    }
  }


  void  KiteEngine::runGlobalRouter ( unsigned int mode )
  {
    if ( getState() >= Katabatic::StateGlobalLoaded )
      throw Error ("KiteEngine::runGlobalRouter() : global routing already done or loaded.");

    Session::open ( this );

    createGlobalGraph ( mode );
    createDetailedGrid ();
    buildBlockages ();
    buildPowerRails ();

    if ( mode == LoadGlobalSolution ) {
      _knik->loadSolution ();
    } else {
      annotateGlobalGraph ();
      _knik->run ();
    }

    setState ( Katabatic::StateGlobalLoaded );

    Session::close ();
  }

  
  void  KiteEngine::loadGlobalRouting ( unsigned int method, vector<Net*>& nets )
  {
    KatabaticEngine::loadGlobalRouting ( method, nets );

    Session::open ( this );
    getGCellGrid()->checkEdgeSaturation ( getEdgeCapacityPercent() );
    Session::close ();
  }


  void  KiteEngine::runNegociate ( unsigned int slowMotion )
  {
    if ( _negociateWindow ) return;

    unsigned int  rows    = _kiteGrid->getRows();
    unsigned int  columns = _kiteGrid->getColumns();

    startMeasures ();

    Session::open ( this );

    cmess1 << "  o  Running Negociate Algorithm" << endl;

    _negociateWindow = NegociateWindow::create ( this, 0, 0, columns, rows );
    _negociateWindow->loadRouting ();
    preProcess ();
    _computeCagedConstraints ();
    _negociateWindow->run ( slowMotion );
    _negociateWindow->printStatistics ();
    _negociateWindow->destroy ();
    _negociateWindow = NULL;

    Session::close ();
  //if ( _editor ) _editor->refresh ();

    stopMeasures ();
    printMeasures ();
    printCompletion ();

    Session::open ( this );
    unsigned int overlaps     = 0;
    float        edgeCapacity = 1.0;
    KnikEngine* knik          = KnikEngine::get ( getCell() );

    if ( knik )
      edgeCapacity = knik->getEdgeCapacityPercent();

    cmess2 << "  o  Post-checking Knik capacity overload " << (edgeCapacity*100.0) << "%." << endl;

    _kiteGrid->base()->checkEdgeSaturation ( edgeCapacity );
    _check ( overlaps );
    Session::close ();

    _toolSuccess = _toolSuccess and (overlaps == 0);
  }


  void  KiteEngine::printCompletion () const
  {
    cout << "  o  Computing Completion ratios." << endl;
    cout << "     - Unrouted segments :" << endl;

    size_t             routeds          = 0;
    size_t             unrouteds        = 0;
    unsigned long long totalWireLength  = 0;
    unsigned long long routedWireLength = 0;

    TrackElementLut::const_iterator ilut = _trackSegmentLut.begin();
    for ( ; ilut != _trackSegmentLut.end() ; ilut++ ) {
      unsigned long long wl = (unsigned long long)DbU::getLambda(ilut->second->getLength());
      if ( wl > 100000 ) {
        cerr << Error("KiteEngine::printCompletion(): Suspiciously long wire: %llu for %p:%s"
                     ,wl,ilut->first,getString(ilut->second).c_str()) << endl;
        continue;
      }
      totalWireLength += wl;
      if ( ilut->second->getTrack() != NULL ) {
        routeds++;
        routedWireLength += wl;
      } else {
        cout << "   " << setw(4) << ++unrouteds << "| " << ilut->second << endl;
      }
    }

    float segmentRatio    = (float)(routeds) / (float)(_trackSegmentLut.size())  * 100.0;
    float wireLengthRatio = (float)(routedWireLength) / (float)(totalWireLength) * 100.0;

    cout << "     - Track Segment Completion Ratio := "
         << setprecision(4) << segmentRatio
         << "% [" << routeds << "/" << _trackSegmentLut.size() << "] "
         << (_trackSegmentLut.size() - routeds) << " remains." << endl;
    cout << "     - Wire Length Completion Ratio   := "
         << setprecision(4) << wireLengthRatio
         << "% [" << totalWireLength << "] "
         << (totalWireLength - routedWireLength) << " remains." << endl;

    if ( _minimumWL != 0.0 ) {
      float expandRatio = totalWireLength / _minimumWL;
      cout << "     - Wire Length Expand Ratio       := "
           << setprecision(4) << expandRatio
           << "% [min:" << setprecision(9) << _minimumWL << "] "
           << endl;
    }

    _toolSuccess = (unrouteds == 0);
  }


  bool  KiteEngine::_check ( unsigned int& overlap, const char* message ) const
  {
    cmess1 << "  o  Checking Kite Database coherency." << endl;

    bool coherency = true;
    coherency = coherency && KatabaticEngine::_check ( message );
    for ( size_t i=0 ; i<_routingPlanes.size() ; i++ )
      coherency = _routingPlanes[i]->_check(overlap) && coherency;

    Katabatic::Session* ktbtSession = Session::base ();
    forEach ( Net*, inet, getCell()->getNets() ) {
      forEach ( Segment*, isegment, inet->getComponents().getSubSet<Segment*>() ) {
        AutoSegment* autoSegment = ktbtSession->lookup ( *isegment );
        if ( !autoSegment ) continue;
        if ( !autoSegment->isCanonical() ) continue;

        TrackElement* trackSegment = Session::lookup ( *isegment );
        if ( !trackSegment ) {
          coherency = false;
          cerr << Bug("%p %s whithout Track Segment"
                     ,autoSegment
                     ,getString(autoSegment).c_str()
                     ) << endl;
        } else
          trackSegment->_check ();
      }
    }

#if defined(CHECK_DATABASE)
  //Session::getKiteEngine()->setInterrupt ( !coherency );
#endif

    return coherency;
  }


  void  KiteEngine::finalizeLayout ()
  {
    ltrace(90) << "KiteEngine::finalizeLayout()" << endl;
    if ( getState() > Katabatic::StateDriving ) return;

    ltracein(90);

    setState ( Katabatic::StateDriving );
    _gutKite ();

    KatabaticEngine::finalizeLayout ();
    ltrace(90) << "State: " << getState() << endl;

    ltraceout(90);
  }


  void  KiteEngine::_gutKite ()
  {
    ltrace(90) << "KiteEngine::_gutKite()" << endl;
    ltracein(90);
    ltrace(90) << "State: " << getState() << endl;

    if ( getState() < Katabatic::StateGutted ) {
      Session::open ( this );

      size_t maxDepth = getRoutingGauge()->getDepth();
      for ( size_t depth=0 ; depth < maxDepth ; depth++ ) {
        _routingPlanes[depth]->destroy ();
      }

      _kiteGrid->destroy ();
      _kiteGrid = NULL;
      Session::close ();
    }

    ltraceout(90);
  }


  TrackElement* KiteEngine::_lookup ( Segment* segment ) const
  {
    TrackElementLut::const_iterator it = _trackSegmentLut.find ( segment );
    if ( it == _trackSegmentLut.end() ) {
      AutoSegment* autoSegment = KatabaticEngine::_lookup ( segment );
      if ( not autoSegment or autoSegment->isCanonical() ) return NULL;

      Interval dummy;
      autoSegment = autoSegment->getCanonical ( dummy );
      it = _trackSegmentLut.find ( autoSegment->base() );
      
      if ( it == _trackSegmentLut.end() ) return NULL;
    }
    return (*it).second;
  }


  void  KiteEngine::_link ( TrackElement* trackSegment )
  {
    if ( getState() > Katabatic::StateActive ) return;

    if ( !trackSegment ) {
      cerr << Bug("KiteEngine::_link(): Rejecting NULL TrackElement.") << endl;
      return;
    }

    _trackSegmentLut [ trackSegment->base()->base() ] = trackSegment;
  // Not needed: Canonical search is done before lookup.
//     forEach ( AutoSegment*, isegment, trackSegment->base()->getCollapseds() ) {
//       _trackSegmentLut [ isegment->base() ] = trackSegment;
//     }
  }


  void  KiteEngine::_unlink ( TrackElement* trackSegment )
  {
    if ( getState() > Katabatic::StateActive ) return;

    TrackElementLut::iterator it = _trackSegmentLut.find ( trackSegment->base()->base() );
    if ( it != _trackSegmentLut.end() )
      _trackSegmentLut.erase ( it );

  // Not needed: Canonical search is done before lookup.
//     forEach ( AutoSegment*, isegment, trackSegment->base()->getCollapseds() ) {
//       TrackElementLut::iterator it = _trackSegmentLut.find ( isegment->base() );
//       if ( it != _trackSegmentLut.end() )
//         _trackSegmentLut.erase ( it );
//     }
  }


  void  KiteEngine::_check ( Net* net ) const
  {
    cerr << "     o  Checking " << net << endl;

    forEach ( Segment*, isegment, net->getComponents().getSubSet<Segment*>() ) {
      TrackElement* trackSegment = _lookup ( *isegment );
      if ( trackSegment ) {
        trackSegment->_check ();

        AutoContact* autoContact = trackSegment->base()->getAutoSource();
        if ( autoContact ) autoContact->checkTopology ();

        autoContact = trackSegment->base()->getAutoTarget();
        if ( autoContact ) autoContact->checkTopology ();
      }
    }
  }


  string  KiteEngine::_getTypeName () const
  { return "Kite::KiteEngine"; }


  string  KiteEngine::_getString () const
  {
    ostringstream  os;

    os << "<" << "KiteEngine "
       << _cell->getName () << " "
    // << getString(_rg->getName())
       << ">";

    return os.str();
  }


  Record* KiteEngine::_getRecord () const
  {
    Record* record = KatabaticEngine::_getRecord ();
                                     
    record->add ( getSlot ( "_routingPlanes", &_routingPlanes ) );
    return record;
  }


} // End of Kite namespace.
