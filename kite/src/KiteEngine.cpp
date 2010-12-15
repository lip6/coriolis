
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
#include  <fstream>
#include  <iomanip>

#include  "hurricane/DebugSession.h"
#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Breakpoint.h"
#include  "hurricane/Layer.h"
#include  "hurricane/Net.h"
#include  "hurricane/Pad.h"
#include  "hurricane/Plug.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Instance.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/UpdateSession.h"
#include  "crlcore/Measures.h"
#include  "knik/Vertex.h"
#include  "knik/Edge.h"
#include  "knik/Graph.h"
#include  "knik/KnikEngine.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/GCellGrid.h"
#include  "kite/DataNegociate.h"
#include  "kite/RoutingPlane.h"
#include  "kite/Session.h"
#include  "kite/NegociateWindow.h"
#include  "kite/KiteEngine.h"


namespace Kite {


  using std::cout;
  using std::cerr;
  using std::endl;
  using std::setw;
  using std::left;
  using std::ofstream;
  using std::ostringstream;
  using std::setprecision;
  using Hurricane::DebugSession;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::ForEachIterator;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Torus;
  using Hurricane::Layer;
  using Hurricane::Cell;
  using CRL::addMeasure;
  using CRL::Measures;
  using CRL::MeasuresSet;
  using Knik::KnikEngine;
  using Katabatic::AutoContact;
  using Katabatic::ChipTools;


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


  KiteEngine::KiteEngine ( Cell* cell )
    : KatabaticEngine  (cell)
    , _knik            (NULL)
    , _blockageNet     (NULL)
    , _configuration   (new Configuration(getKatabaticConfiguration()))
    , _routingPlanes   ()
    , _negociateWindow (NULL)
    , _trackSegmentLut ()
    , _minimumWL       (0.0)
    , _toolSuccess     (false)
  {
  //_configuration->setAllowedDepth ( 3 );
  }


  void  KiteEngine::_postCreate ()
  {
    KatabaticEngine::_postCreate ();

#ifdef KNIK_NOT_EMBEDDED
    size_t maxDepth = getRoutingGauge()->getDepth();

    _routingPlanes.reserve ( maxDepth );
    for ( size_t depth=0 ; depth < maxDepth ; depth++ ) {
      _routingPlanes.push_back ( RoutingPlane::create ( this, depth ) );
    }
#endif
  }


  KiteEngine* KiteEngine::create ( Cell* cell )
  {
    KiteEngine* kite = new KiteEngine ( cell );

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
  {
    delete _configuration;
  }


  const Name& KiteEngine::getName () const
  { return _toolName; }


  Configuration* KiteEngine::getConfiguration ()
  { return _configuration; }


  unsigned int  KiteEngine::getRipupLimit ( const TrackElement* segment ) const
  {
    if ( segment->isBlockage() ) return 0;

    if ( segment->isStrap () ) return _configuration->getRipupLimit(Configuration::StrapRipupLimit);
    if ( segment->isGlobal() ) {
      Katabatic::GCellVector gcells;
      segment->getGCells(gcells);
      if ( gcells.size() > 2 )
        return _configuration->getRipupLimit(Configuration::LongGlobalRipupLimit);
      return _configuration->getRipupLimit(Configuration::GlobalRipupLimit);
    }
    return _configuration->getRipupLimit(Configuration::LocalRipupLimit);
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
    Cell* cell   = getCell();
    Box   cellBb = cell->getBoundingBox();
    if ( not _knik ) {
    //if ( cell->getRubbers().getFirst() == NULL )
      cell->flattenNets ( (mode==BuildGlobalSolution) );
      
    //Breakpoint::stop ( 0, "Point d'arret:<br>&nbsp;&nbsp;<b>createGlobalGraph()</b>&nbsp;"
    //                      "after net virtual flattening." );

      KatabaticEngine::chipPrep ();

      KnikEngine::setEdgeCapacityPercent ( 1.0 );
      _knik = KnikEngine::create ( cell
                                 , 1     // _congestion
                                 , 2     // _preCongestion
                                 , false // _benchMode
                                 , true  // _useSegments
                                 , 2.5   // _edgeCost
                                 );
    //if ( mode == LoadGlobalSolution )
      _knik->createRoutingGraph ();
      KnikEngine::setEdgeCapacityPercent ( getEdgeCapacityPercent() );

    // Decrease the edge's capacity only under the core area.
      const ChipTools& chipTools     = getChipTools();
      float            corePercent   = getEdgeCapacityPercent();
      float            coronaPercent = 0.80;

      forEach ( Knik::Vertex*, ivertex, _knik->getRoutingGraph()->getVertexes() ) {
        for ( int i=0 ; i<2 ; ++i ) {
          Knik::Edge* edge = NULL;

          if ( i==0 ) {
            edge = ivertex->getHEdgeOut();
            if ( not edge ) continue;

            if ( chipTools.intersectHPads(edge->getBoundingBox()) ) {
              edge->setCapacity ( 0 );
              continue;
            }
          } else {
            edge = ivertex->getVEdgeOut();
            if ( not edge ) continue;

            if ( chipTools.intersectVPads(edge->getBoundingBox()) ) {
              edge->setCapacity ( 0 );
              continue;
            }
          }

          float edgePercent = 1.00;
          if      ( chipTools.getCorona().getInnerBox().contains(edge->getBoundingBox()) ) edgePercent = corePercent;
          else if ( chipTools.getCorona().getOuterBox().contains(edge->getBoundingBox()) ) edgePercent = coronaPercent;

          unsigned int capacity = (unsigned int)(edge->getCapacity() * edgePercent );
          edge->setCapacity ( capacity );
        }
      }
    }
  }


  void  KiteEngine::createDetailedGrid ()
  {
    KatabaticEngine::createDetailedGrid ();

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

    const Torus& chipCorona = getChipTools().getCorona();

    int hEdgeCapacity = 0;
    int vEdgeCapacity = 0;
    for ( size_t depth=0 ; depth<_routingPlanes.size() ; ++depth ) {
      RoutingPlane* rp = _routingPlanes[depth];
      if ( rp->getLayerGauge()->getType() == Constant::PinOnly ) continue;

      if ( rp->getDirection() == Constant::Horizontal ) ++hEdgeCapacity;
      else ++vEdgeCapacity;
    }

    for ( size_t depth=0 ; depth<_routingPlanes.size() ; ++depth ) {
      RoutingPlane* rp = _routingPlanes[depth];
      if ( rp->getLayerGauge()->getType() == Constant::PinOnly ) continue;

      size_t tracksSize = rp->getTracksSize();
      for ( size_t itrack=0 ; itrack<tracksSize ; ++itrack ) {
        Track*        track   = rp->getTrackByIndex ( itrack );
        Knik::Edge*   edge    = NULL;

        ltrace(300) << "Capacity from: " << track << endl;

        if ( track->getDirection() == Constant::Horizontal ) {
          for ( size_t ielement=0 ; ielement<track->getSize() ; ++ielement ) {
            TrackElement* element = track->getSegment ( ielement );
            
            if ( element->getNet() == NULL ) {
              ltrace(300) << "Reject capacity from (not Net): " << (void*)element << ":" << element << endl;
              continue;
            }
            if ( (not element->isFixed()) and (not element->isBlockage()) ) {
              ltrace(300) << "Reject capacity from (neither fixed nor blockage): " << (void*)element << ":" << element << endl;
              continue;
            }

            Box elementBb       = element->getBoundingBox();
            int elementCapacity = (chipCorona.contains(elementBb)) ? -hEdgeCapacity : -1;

            ltrace(300) << "Capacity from: " << (void*)element << ":" << element
                        << ":" << elementCapacity << endl;

            Katabatic::GCell* gcell = getGCellGrid()->getGCell ( Point(element->getSourceU(),track->getAxis()) );
            Katabatic::GCell* end   = getGCellGrid()->getGCell ( Point(element->getTargetU(),track->getAxis()) );
            Katabatic::GCell* right = NULL;
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
                                          , elementCapacity );
            // edge = _knik->getEdge ( gcell->getColumn()
            //                       , gcell->getRow()
            //                       , right->getColumn()
            //                       , right->getRow()
            //                       );
              gcell = right;
            }
          }
        } else {
          for ( size_t ielement=0 ; ielement<track->getSize() ; ++ielement ) {
            TrackElement* element = track->getSegment ( ielement );

            if ( element->getNet() == NULL ) {
              ltrace(300) << "Reject capacity from (not Net): " << (void*)element << ":" << element << endl;
              continue;
            }
            if ( (not element->isFixed()) and not (element->isBlockage()) ) {
              ltrace(300) << "Reject capacity from (neither fixed nor blockage): " << (void*)element << ":" << element << endl;
              continue;
            }

            Box elementBb       = element->getBoundingBox();
            int elementCapacity = (chipCorona.contains(elementBb)) ? -vEdgeCapacity : -1;

            ltrace(300) << "Capacity from: " << (void*)element << ":" << element
                        << ":" << elementCapacity << endl;

            Katabatic::GCell* gcell = getGCellGrid()->getGCell ( Point(track->getAxis(),element->getSourceU()) );
            Katabatic::GCell* end   = getGCellGrid()->getGCell ( Point(track->getAxis(),element->getTargetU()) );
            Katabatic::GCell* up    = NULL;
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
                                          , elementCapacity );
            // edge = _knik->getEdge ( gcell->getColumn()
            //                       , gcell->getRow()
            //                       , up->getColumn()
            //                       , up->getRow()
            //                       );
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

  //DebugSession::addToTrace ( getCell(), "nb(0)" );
  //DebugSession::addToTrace ( getCell(), "ram_adri(0)" );
  //DebugSession::addToTrace ( getCell(), "rsdnbr_sd(9)" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_1m_dp_res_re(21)" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_1m_dp_soper_se(20)" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_1m_dp_res_re(20)" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_1m_dp_addsub32_carith_se_gi_1_29" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_1m_dp_instaddbracry_sd_gi_1_21" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_1m_dp_addsub32_carith_se_pi_3_29" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_1m_dp_res_se(28)" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_core.mips_r3000_1m_dp.etat32_otheri_sd_2.enx" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_core.mips_r3000_1m_dp.yoper_se(26)" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_core.mips_r3000_1m_dp.toper_se(5)" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_core.mips_r3000_1m_dp.res_re(12)" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_core.mips_r3000_1m_dp.res_re(20)" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_core.mips_r3000_1m_dp.nextpc_rd(21)" );
  //DebugSession::addToTrace ( getCell(), "addr_i(1)" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_core.mips_r3000_1m_ct.opcod_rd(1)" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_core.mips_r3000_1m_dp.otheri_sd(29)" );
  //DebugSession::addToTrace ( getCell(), "d_in_i(11)" );
  //DebugSession::addToTrace ( getCell(), "ng_i" );
  //DebugSession::addToTrace ( getCell(), "d_out_i(14)" );
  //DebugSession::addToTrace ( getCell(), "d_out_i(19)" );
  //DebugSession::addToTrace ( getCell(), "dout_e_i(1)" );
  //DebugSession::addToTrace ( getCell(), "dout_e_i(2)" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_core.mips_r3000_1m_ct.aux44" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_core.mips_r3000_1m_ct.na4_x1_11_sig" );
  //DebugSession::addToTrace ( getCell(), "mips_r3000_core.rsdnbr_sd(14)" );
  //DebugSession::addToTrace ( getCell(), "d_out_i(27)" );

    createDetailedGrid ();
    buildPowerRails ();
    protectRoutingPads ();

    if ( mode == LoadGlobalSolution ) {
      _knik->loadSolution ();
    } else {
      annotateGlobalGraph ();
      _knik->run ();
    }

    setState ( Katabatic::StateGlobalLoaded );

    Session::close ();
  }

  
  void  KiteEngine::loadGlobalRouting ( unsigned int method, KatabaticEngine::NetSet& nets )
  {
    KatabaticEngine::loadGlobalRouting ( method, nets );

    Session::open ( this );
  //KatabaticEngine::chipPrep ();
    getGCellGrid()->checkEdgeSaturation ( getEdgeCapacityPercent() );
    Session::close ();
  }


  void  KiteEngine::runNegociate ( unsigned int slowMotion )
  {
    if ( _negociateWindow ) return;

    startMeasures ();

    Session::open ( this );

    cmess1 << "  o  Running Negociate Algorithm" << endl;

    _negociateWindow = NegociateWindow::create ( this );
    _negociateWindow->setGCells ( *(getGCellGrid()->getGCellVector()) );
    preProcess ();
    _computeCagedConstraints ();
    _negociateWindow->run ( slowMotion );
    _negociateWindow->printStatistics ();
    _negociateWindow->destroy ();
    _negociateWindow = NULL;

    Session::close ();
  //if ( _editor ) _editor->refresh ();

    stopMeasures ();
    printMeasures ( "algo" );
    printCompletion ();

    Session::open ( this );
    unsigned int overlaps     = 0;
    float        edgeCapacity = 1.0;
    KnikEngine*  knik         = KnikEngine::get ( getCell() );

    if ( knik )
      edgeCapacity = knik->getEdgeCapacityPercent();

    cmess2 << "  o  Post-checking Knik capacity overload " << (edgeCapacity*100.0) << "%." << endl;

    getGCellGrid()->checkEdgeSaturation ( edgeCapacity );
    _check ( overlaps );
    Session::close ();

    _toolSuccess = _toolSuccess and (overlaps == 0);
  }


  void  KiteEngine::printCompletion () const
  {
    cmess1 << "  o  Computing Completion ratios." << endl;
    cmess1 << "     - Unrouted segments :" << endl;

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

    cmess1 << "     - Track Segment Completion Ratio := "
           << setprecision(4) << segmentRatio
           << "% [" << routeds << "/" << _trackSegmentLut.size() << "] "
           << (_trackSegmentLut.size() - routeds) << " remains." << endl;
    cmess1 << "     - Wire Length Completion Ratio   := "
           << setprecision(4) << wireLengthRatio
           << "% [" << totalWireLength << "] "
           << (totalWireLength - routedWireLength) << " remains." << endl;

    float expandRatio = 1.0;
    if ( _minimumWL != 0.0 ) {
      expandRatio = totalWireLength / _minimumWL;
      cmess1 << "     - Wire Length Expand Ratio       := "
             << setprecision(4) << expandRatio
             << "% [min:" << setprecision(9) << _minimumWL << "] "
             << endl;
    }

    _toolSuccess = (unrouteds == 0);

    addMeasure<size_t>             ( getCell(), "Segs"   , routeds+unrouteds );
    addMeasure<unsigned long long> ( getCell(), "DWL(l)" , totalWireLength                  , 12 );
    addMeasure<unsigned long long> ( getCell(), "fWL(l)" , totalWireLength-routedWireLength , 12);
    addMeasure<double>             ( getCell(), "WLER(%)", (expandRatio-1.0)*100.0 );
  }


  void  KiteEngine::dumpMeasures ( ostream& out ) const
  {
    vector<Name> measuresLabels;
    measuresLabels.push_back ( "Gates"   );
    measuresLabels.push_back ( "GCells"  );
    measuresLabels.push_back ( "knikT"   );
    measuresLabels.push_back ( "knikS"   );
    measuresLabels.push_back ( "GWL(l)"  );
    measuresLabels.push_back ( "Area(l2)");
    measuresLabels.push_back ( "Sat."    );
    measuresLabels.push_back ( "loadT"   );
    measuresLabels.push_back ( "loadS"   );
    measuresLabels.push_back ( "Globals" );
    measuresLabels.push_back ( "Edges"   );
    measuresLabels.push_back ( "assignT" );
    measuresLabels.push_back ( "algoT"   );
    measuresLabels.push_back ( "algoS"   );
    measuresLabels.push_back ( "finT"    );
    measuresLabels.push_back ( "Segs"    );
    measuresLabels.push_back ( "DWL(l)"  );
    measuresLabels.push_back ( "fWL(l)"  );
    measuresLabels.push_back ( "WLER(%)" );
    measuresLabels.push_back ( "Events"  );
    measuresLabels.push_back ( "UEvents" );

    const MeasuresSet* measures = Measures::get(getCell());

    out << "#" << endl;
    out << "# " << getCell()->getName() << endl;
    out << measures->toStringHeaders(measuresLabels) << endl;
    out << measures->toStringDatas  (measuresLabels) << endl;

    measures->toGnuplot ( "GCells Density Histogram", getString(getCell()->getName()) );
  }


  void  KiteEngine::dumpMeasures () const
  {
    ostringstream path;
    path << getCell()->getName() << ".knik-kite.dat";

    ofstream sfile ( path.str().c_str() );
    dumpMeasures ( sfile );
    sfile.close ();
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
        if ( not autoSegment ) continue;
        if ( not autoSegment->isCanonical() ) continue;

        TrackElement* trackSegment = Session::lookup ( *isegment );
        if ( not trackSegment ) {
          coherency = false;
          cerr << Bug("%p %s without Track Segment"
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
