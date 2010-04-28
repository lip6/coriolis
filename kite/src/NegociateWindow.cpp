
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
// |  C++ Module  :       "./NegociateWindow.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <algorithm>
#include  <iomanip>

#include  "hurricane/Bug.h"
#include  "hurricane/RoutingPad.h"
#include  "hurricane/Net.h"
#include  "hurricane/Cell.h"
#include  "crlcore/Utilities.h"
#include  "crlcore/AllianceFramework.h"

#include  "kite/DataNegociate.h"
#include  "kite/TrackElement.h"
#include  "kite/TrackMarker.h"
#include  "kite/TrackCost.h"
#include  "kite/Track.h"
#include  "kite/RoutingPlane.h"
#include  "kite/GCellGrid.h"
#include  "kite/GCellRoutingSet.h"
#include  "kite/RoutingEventQueue.h"
#include  "kite/RoutingEventHistory.h"
#include  "kite/NegociateWindow.h"
#include  "kite/KiteEngine.h"


namespace {


  using namespace std;
  using namespace Hurricane;
  using namespace CRL;
  using namespace Kite;


  void  NegociateOverlapCost ( const TrackElement* segment, TrackCost& cost )
  {
    Interval intersect = segment->getCanonicalInterval();

    if ( not intersect.intersect ( cost.getInterval() ) ) return;

    if ( segment->isBlockage() ) {
    //ltrace(200) << "Infinite cost from: " << segment << endl;
      cost.setInfinite    ();
      cost.setOverlap     ();
      cost.setHardOverlap ();
      cost.setBlockage    ();
      return;
    }

    if ( cost.getInterval().getVMax() > intersect.getVMax() ) cost.setLeftOverlap();
    if ( cost.getInterval().getVMin() < intersect.getVMin() ) cost.setRightOverlap();

    if ( not intersect.contains(cost.getInterval()) )
      intersect.intersection ( cost.getInterval() );

    DataNegociate* data = segment->getDataNegociate ();
    if ( not data ) return;

    if ( data->getGCellOrder() >= Session::getOrder() ) {
      cost.mergeRipupCount ( data->getRipupCount() );
      if ( segment->isLocal() ) {
        cost.mergeDataState ( data->getState() );
        if ( data->getState() >=  DataNegociate::LocalVsGlobal ) {
          ltrace(200) << "MaximumSlack/LocalVsGlobal for " << segment << endl;
        }
      }
    }

    if ( /*(data->getGCellOrder() < Session::getOrder()) ||*/ segment->isFixed()
       or ((data->isRing() or data->isBorder()) and (data->getRipupCount() > 3)) ) {
      ltrace(200) << "Infinite cost from: " << segment << endl;
      cost.setFixed       ();
      cost.setInfinite    ();
      cost.setOverlap     ();
      cost.setHardOverlap ();
      return;
    }

    if (  ( data->getGCellOrder() < Session::getOrder() )
       or (   ( data->getCost().getRightMinExtend() >= cost.getInterval().getVMin() )
          and ( data->getCost().getLeftMinExtend () <= cost.getInterval().getVMax() ) ) )
      cost.setHardOverlap ();

    cost.setOverlap   ();
    cost.incTerminals ( data->getCost().getTerminals()*100 );
    cost.incDelta     ( intersect.getSize() );
  }


  void  loadRoutingPads ( NegociateWindow* nw )
  {
    AllianceFramework* af = AllianceFramework::get ();
    RoutingGauge*      rg = nw->getKiteEngine()->getRoutingGauge();

    forEach ( Net*, inet, nw->getCell()->getNets() ) {
      if ( inet->getType() == Net::Type::POWER  ) continue;
      if ( inet->getType() == Net::Type::GROUND ) continue;
      if ( inet->getType() == Net::Type::CLOCK  ) continue;
      if ( af->isOBSTACLE(inet->getName()) ) continue;

      forEach ( RoutingPad*, irp, inet->getRoutingPads() ) {
        size_t depth = rg->getLayerDepth(irp->getLayer());
        if ( depth >  0 ) continue;
        if ( depth == 0 ) {
          TrackMarker::create ( *irp, 1 );
        }
      }
    }
  }


} // End of local namespace.


namespace Kite {


  using std::cerr;
  using std::endl;
  using std::setw;
  using std::left;
  using std::right;
  using std::setprecision;
  using Hurricane::Bug;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::ForEachIterator;


// -------------------------------------------------------------------
// Class  :  "NegociateWindow::RingSegment".


  bool  NegociateWindow::RingSegment::orderReached ( const NegociateWindow::RingSegment& segment )
  {
    return ( segment.getOrder() <= Session::getOrder() );
  }
  


  NegociateWindow::RingSegment::RingSegment ( TrackElement* segment )
    : _segment(segment), _order(0)
  {
    DataNegociate* data = segment->getDataNegociate ();
    if ( data ) _order = data->getGCellOrder ();
  }


// -------------------------------------------------------------------
// Class  :  "NegociateWindow".


  NegociateWindow::NegociateWindow ( KiteEngine*   kite
                                   , unsigned int  columnMin
                                   , unsigned int  rowMin
                                   , unsigned int  columnMax
                                   , unsigned int  rowMax
                                   )
    : _slowMotion      (0)
    , _interrupt       (false)
    , _kite            (kite)
    , _gridBox         (NULL)
    , _criticalGCells  ()
    , _gcellOrder      (0)
    , _gcellRoutingSets()
    , _eventQueue      ()
    , _eventHistory    ()
    , _ring            ()
  {
    _gridBox = GridBox<GCell>::create ( _kite->getGCellGrid()
                                      , columnMin
                                      , rowMin
                                      , columnMax
                                      , rowMax
                                      );
  }


  NegociateWindow* NegociateWindow::create ( KiteEngine*   kite
                                           , unsigned int  columnMin
                                           , unsigned int  rowMin
                                           , unsigned int  columnMax
                                           , unsigned int  rowMax
                                           )
  {
    NegociateWindow* negociateWindow = new NegociateWindow ( kite
                                                           , columnMin
                                                           , rowMin
                                                           , columnMax
                                                           , rowMax
                                                           );
    return negociateWindow;
  }


  NegociateWindow::~NegociateWindow ()
  {
    for ( size_t i=0 ; i<_gcellRoutingSets.size() ; i++ )
      _gcellRoutingSets[i]->destroy ();

    if ( _gridBox ) delete _gridBox;
  }


  void  NegociateWindow::destroy ()
  { delete this; }


  Cell* NegociateWindow::getCell () const
  { return _kite->getCell(); }


  void  NegociateWindow::addToRing ( TrackElement* segment )
  {
    ltrace(200) << "addToRing: " << segment << endl;

    _ring.push_back ( RingSegment(segment) );

    DataNegociate* data = segment->getDataNegociate ();
    data->setRing ( true );
    data->setGCellOrder ( Session::getOrder() );

    _eventQueue.add ( segment, 0 );
  }


  void  NegociateWindow::loadRouting ()
  {
    vector<GCell*> gcells;
    GCellGrid*     grid = getKiteEngine()->getGCellGrid();

    forEach ( GCell*, igcell, grid->getGCells() ) {
      gcells.push_back ( *igcell );
      igcell->updateDensity ();
    }

    sort ( gcells.begin(), gcells.end(), GCell::CompareByDensity() );

#if defined(CHECK_DETERMINISM)
    cerr << "Order: After sort<>" << endl;
    for ( size_t i=0 ; i < gcells.size() ; i++ ) {
      cerr << "Order: "
           << setw(9) << left  << setprecision(6) << gcells[i]->base()->getDensity()
           << setw(5) << right << gcells[i]->getIndex()
           << " " << gcells[i] << endl;
    }
#endif

    unsigned int order = 0;
    for ( size_t i=0 ; i < gcells.size() ; i++ ) {
      if ( not gcells[i]->isInRoutingSet() ) {
        Session::setOrder ( order );
        GCellRoutingSet* rs = GCellRoutingSet::create ( gcells[i], _kite->getExpandStep() );
        rs->expand      ( grid  );
        rs->loadRouting ( order );

        _gcellRoutingSets.push_back ( rs );
      }
    }

    loadRoutingPads ( this );
    Session::revalidate ();

    TrackElement*             segment;
    TrackElementLut           lut = Session::getKiteEngine()->_getTrackElementLut();
    TrackElementLut::iterator it  = lut.begin ();
    for ( ; it != lut.end() ; it++ ) {
      segment = it->second;
      segment->getDataNegociate()->update();
    }

    getKiteEngine()->setMinimumWL ( grid->getTotalWireLength() );

#if defined(CHECK_DATABASE)
    unsigned int overlaps = 0;
    Session::getKiteEngine()->_check(overlaps,"after LoadRouting");
#endif 
  }


  void  NegociateWindow::addInsertEvent ( TrackElement* segment, unsigned int level )
  {
    DataNegociate* data = segment->getDataNegociate();
    if ( not data or not data->hasRoutingEvent() ) 
      _eventQueue.add ( segment, level );
    else
      cerr << Bug("NegociateWidow::addInsertEvent(): Try to adds twice the same TrackElement event."
                 "\n       %p:%s."
                 ,(void*)segment->base()->base()
                 ,getString(segment).c_str()
                 ) << endl;
  }


  void  NegociateWindow::_unloadRing ()
  {
    _ring.erase ( remove_if(_ring.begin(),_ring.end(),RingSegment::orderReached), _ring.end() );
    for ( size_t i=0 ; i<_ring.size() ; ++i ) {
      if ( _ring[i].getSegment()->getTrack() != NULL )
        Session::addRemoveEvent ( _ring[i].getSegment() );
    }

    Session::revalidate ();
  }


  void  NegociateWindow::_loadRing ()
  {
    unsigned int  order = Session::getOrder ();
    for ( size_t i=0 ; i<_ring.size() ; i++ ) {
      TrackElement*  segment = _ring[i].getSegment();
      DataNegociate* data    = segment->getDataNegociate ();

      data->resetRipupCount ();
      data->resetStateCount ();
      data->setGCellOrder ( order );
      if ( _ring[i].getOrder() == order ) {
        ltrace(200) << "Removing from ring: " << segment << endl;
        data->setRing ( false );
      }

      _eventQueue.add ( segment, 0 );
    }
    _eventQueue.commit ();
  }


  size_t  NegociateWindow::_negociate ( const vector<TrackElement*>& segments )
  {
    ltrace(150) << "NegociateWindow::_negociate() - " << segments.size() << endl;
    ltracein(149);

    unsigned long limit = _kite->getEventsLimit();

    _eventHistory.clear();
    _eventQueue.load ( segments );
    _loadRing ();

    size_t count = 0;
    while ( not _eventQueue.empty() and not isInterrupted() ) {
      RoutingEvent* event = _eventQueue.pop ();

      event->process ( _eventQueue, _eventHistory );
      if (tty::enabled()) {
        cmess1 << "       <FirstPass:Negociation - event:" << tty::bold << setw(7) << setfill('0')
               << RoutingEvent::getProcesseds() << setfill(' ') << tty::reset << ">" << tty::cr;
        cmess1.flush ();
      } else {
        cmess2 << "       <FirstPass:Negociation - event:" << setw(7) << setfill('0')
               << RoutingEvent::getProcesseds() << setfill(' ') << "> id:"
               << event->getSegment()->getId() << " "
               << event->getSegment()->getNet()->getName()
               << endl;
        cmess2.flush();
      }

      if ( RoutingEvent::getProcesseds() >= limit ) setInterrupt ( true );
      count++;

#if ENABLE_STIFFNESS
      if ( not (RoutingEvent::getProcesseds() % 1000) ) {
        sort ( _criticalGCells.begin(), _criticalGCells.end(), GCell::CompareByStiffness() );
        for ( size_t igcell=0 ; igcell<_criticalGCells.size() ; ++igcell ) {
          if ( _criticalGCells[igcell]->getStiffness   () <  0.7 ) break;
          if ( _criticalGCells[igcell]->getSegmentCount() < 20   ) continue;

          cerr << "       - Anticipate: " << _criticalGCells[igcell]
               << ":" << _criticalGCells[igcell]->getStiffness() << endl;

          _criticalGCells[igcell]->anticipateRouting (  Session::getOrder() );
          _eventQueue.load ( _criticalGCells[igcell]->getOwnedSegments() );
          _criticalGCells[igcell]->setRouted ( true );
        }
      }
#endif
    }
    if (count and tty::enabled()) cmess1 << endl;
    count = 0;

    ltrace(200) << "Dumping history." << endl;
    for ( size_t i=0 ; (i<_eventHistory.size()) && !isInterrupted() ; i++ ) {
      RoutingEvent* event = _eventHistory.getNth(i);
      ltrace(200) << (void*)event << " ["
                  << (event->isCloned       ()?"C":"-")
                  << (event->isDisabled     ()?"d":"-")
                  << (event->isUnimplemented()?"u":"-") << "] "
                  << event->getSegment() << endl;
      if ( !event->isCloned() and event->isUnimplemented() ) {
        count++;
        event->setProcessed ( false );
        event->setMode ( RoutingEvent::PostPack );
        event->process ( _eventQueue, _eventHistory );

        if (tty::enabled()) {
          cmess1 << "       <SecondPass:Packing - event:"
                 << tty::bold << tty::fgcolor(tty::Red) << setw(7) << setfill('0')
                 << RoutingEvent::getProcesseds() << setfill(' ') << tty::reset << ">" << tty::cr;
          cmess1.flush ();
        } else {
          cmess1 << "       <SecondPass:Packing - event:" << setw(7) << setfill('0')
                 << RoutingEvent::getProcesseds() << setfill(' ') << ">" << endl;
        }
      }
    }
    if (count and tty::enabled()) cmess1 << endl;

    size_t eventsCount = _eventHistory.size();

    _unloadRing ();
    _eventHistory.clear();
    _eventQueue.clear();

    if ( RoutingEvent::getAllocateds() > 0 ) {
      cerr << Bug("%d events remains after clear.",RoutingEvent::getAllocateds()) << endl;
    }

  //   if ( _slowMotion && getCellWidget() ) {
  //     Session::close ();
  //     getCellWidget()->refresh();
  //     Session::open ( _kiteEngine );
  //   }

    ltraceout(149);

    return eventsCount;
  }


  void  NegociateWindow::_runOnGCellRoutingSet ( GCellRoutingSet* rs )
  {
#if defined(CHECK_DETERMINISM)
    cerr << "Order: Routing set: " << rs << endl;
#endif

    ltrace(200) << "Routing " << rs << endl;
    ltracein(200);

    cmess1 << "     - Routing " << rs << endl;

    vector<GCell*>  gcells = rs->getGCells ();
    for ( size_t i=0 ; i<gcells.size() ; i++ ) {
      ltrace(200) << gcells[i] << endl;
#if defined(CHECK_DETERMINISM)
      cerr << "Order:   GCell: " << gcells[i] << endl;
#endif
    }

    Session::setOrder ( rs->getOrder() );

    vector<TrackElement*>  segments;
    rs->loadBorder       ( getKiteEngine()->getGCellGrid() );
    rs->getOwnedSegments ( segments );
    rs->setRouted        ( true );
    rs->getStatistics    ().setEventsCount ( _negociate(segments) );
    rs->freeBorder       ();

    ltraceout(200);
  }


  void  NegociateWindow::run ( int slowMotion )
  {
    ltrace(150) << "NegociateWindow::run()" << endl;
    ltracein(149);

    _criticalGCells = *(getKiteEngine()->getGCellGrid()->getGCellVector());

    TrackElement::setOverlapCostCB ( NegociateOverlapCost );
    RoutingEvent::resetProcesseds ();

    // sort ( gcells.begin(), gcells.end(), GCell::CompareByStiffness() );
    // for ( size_t j=0 ; j<gcells.size() ; ++j ) {
    //   cerr << "     INITIAL stiff: " << gcells[j] << ":" << gcells[j]->getStiffness() << endl;
    // }

    _slowMotion = slowMotion;
    _gcellOrder = 0;
    for ( size_t i=0 ; (i<_gcellRoutingSets.size()) && !isInterrupted() ; i++ ) {
      _runOnGCellRoutingSet ( _gcellRoutingSets[i] );

      // sort ( gcells.begin(), gcells.end(), GCell::CompareByStiffness() );
      // for ( size_t j=0 ; j<gcells.size() ; ++j ) {
      //   cerr << "     stiff: " << gcells[j] << ":" << gcells[j]->getStiffness() << endl;
      // }
    }

    Session::get()->isEmpty();

# if defined(CHECK_DATABASE)
    unsigned int overlaps = 0;
    _kite->_check ( overlaps, "after negociation" );
# endif

    ltraceout(149);
  }


  void  NegociateWindow::printStatistics () const
  {
    cout << "  o  Computing statistics." << endl;

    Statistics globalStatistics;
    size_t     biggestEventsCount = 0;
    size_t     biggestRSsize      = 0;
    for ( size_t i=0; i<_gcellRoutingSets.size() ; i++ ) {
      Statistics& statistics = _gcellRoutingSets[i]->getStatistics();
      globalStatistics += statistics;

      if ( statistics.getEventsCount() > biggestEventsCount )
        biggestEventsCount = statistics.getEventsCount();

      if ( _gcellRoutingSets[i]->getGCells().size() > biggestRSsize )
        biggestRSsize = _gcellRoutingSets[i]->getGCells().size();
    }
    
    cout << Dots::asSizet("     - Processeds Events Total",RoutingEvent::getProcesseds()) << endl;
    cout << Dots::asSizet("     - Unique Events Total"
                         ,(RoutingEvent::getProcesseds() - RoutingEvent::getCloneds())) << endl;
    cout << Dots::asSizet("     - Biggest Events Chunk"   ,biggestEventsCount) << endl;
    cout << Dots::asSizet("     - Biggest Routing Set"    ,biggestRSsize) << endl;
  }


  string  NegociateWindow::_getString () const
  {
    ostringstream  os;

    os << "<" << _getTypeName() << _gridBox << ">";
    return ( os.str() );
  }


  Record* NegociateWindow::_getRecord () const
  {
    Record* record = new Record ( _getString() );
                                     
    record->add ( getSlot ( "_gridBox" ,  _gridBox  ) );
    return ( record );
  }


} // End of Kite namespace.
