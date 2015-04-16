// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NegociateWindow.cpp"                    |
// +-----------------------------------------------------------------+


#include <vector>
#include <algorithm>
#include <iomanip>
#include "hurricane/Warning.h"
#include "hurricane/Bug.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/Measures.h"
#include "crlcore/Histogram.h"
#include "katabatic/AutoContact.h"
#include "katabatic/GCellGrid.h"
#include "kite/DataNegociate.h"
#include "kite/TrackElement.h"
#include "kite/TrackMarker.h"
#include "kite/TrackCost.h"
#include "kite/Track.h"
#include "kite/TrackSegment.h"
#include "kite/RoutingPlane.h"
#include "kite/RoutingEventQueue.h"
#include "kite/RoutingEventHistory.h"
#include "kite/RoutingEventLoop.h"
#include "kite/NegociateWindow.h"
#include "kite/KiteEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;
  using namespace Kite;


  void  NegociateOverlapCost ( const TrackElement* segment, TrackCost& cost )
  {
    ltrace(500) << "Deter| NegociateOverlapCost() " << segment << endl;
    Interval intersect = segment->getCanonicalInterval();

    if (not intersect.intersect(cost.getInterval())) return;

    if (segment->isBlockage() or segment->isFixed()) {
      ltrace(200) << "Infinite cost from: " << segment << endl;
      cost.setInfinite   ();
      cost.setOverlap    ();
      cost.setHardOverlap();
      cost.setBlockage   ();
      return;
    }

    if (cost.getInterval().getVMax() > intersect.getVMax()) cost.setLeftOverlap();
    if (cost.getInterval().getVMin() < intersect.getVMin()) cost.setRightOverlap();

    if (not intersect.contains(cost.getInterval()))
      intersect.intersection( cost.getInterval() );
    else {
      cost.setLonguestOverlap( intersect.getSize() );
      cost.setGlobalEnclosed();
    }

    DataNegociate* data = segment->getDataNegociate();
    if (not data) return;

    cost.mergeRipupCount( data->getRipupCount() );
    if ( segment->isLocal() ) {
      cost.mergeDataState( data->getState() );
      if (data->getState() >=  DataNegociate::LocalVsGlobal) {
        ltrace(200) << "MaximumSlack/LocalVsGlobal for " << segment << endl;
      }
    }

    if (segment->isGlobal()) {
      cost.setOverlapGlobal();
      if (   (cost.getFlags() & TrackCost::LocalAndTopDepth)
         and (data->getState() >= DataNegociate::MoveUp) ) {
        cost.setInfinite   ();
        cost.setOverlap    ();
        cost.setHardOverlap();
        return;
      }
    }

    cost.setOverlap();
    if ( segment->isLocal()
       or (cost.isForGlobal() and (Session::getRoutingGauge()->getLayerDepth(segment->getLayer()) < 3)) ) {
      ltrace(500) << "Deter|     incTerminals() " << boolalpha << cost.isForGlobal() << " " << (data->getTerminals()*100) << endl;
      cost.incTerminals( data->getTerminals()*100 );
    } else {
      ltrace(500) << "Deter|     isForGlobal() " << boolalpha << cost.isForGlobal() << endl;
    }

    ltrace(200) << "| Increment Delta: " << DbU::getValueString(intersect.getSize()) << endl;
    cost.incDelta( intersect.getSize() );
  }


  void  loadRoutingPads ( NegociateWindow* nw )
  {
    AllianceFramework* af = AllianceFramework::get ();
    RoutingGauge*      rg = nw->getKiteEngine()->getRoutingGauge();

    forEach ( Net*, inet, nw->getCell()->getNets() ) {
      if (inet->getType() == Net::Type::POWER ) continue;
      if (inet->getType() == Net::Type::GROUND) continue;
      if (inet->getType() == Net::Type::CLOCK ) continue;
      if (af->isBLOCKAGE(inet->getName())) continue;

      forEach ( RoutingPad*, irp, inet->getRoutingPads() ) {
        size_t depth = rg->getLayerDepth(irp->getLayer());
        if (depth >  0) continue;
        if (depth == 0) 
          TrackMarker::create( *irp, 1 );
      }
    }
  }


} // Anonymous namespace.


namespace Kite {

  using std::cerr;
  using std::endl;
  using std::setw;
  using std::left;
  using std::right;
  using std::setprecision;
  using Hurricane::Warning;
  using Hurricane::Bug;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::ForEachIterator;
  using CRL::Histogram;
  using CRL::addMeasure;
  using Katabatic::AutoContact;
  using Katabatic::AutoSegmentLut;
  using Katabatic::perpandicularTo;


// -------------------------------------------------------------------
// Class  :  "NegociateWindow".


  NegociateWindow::NegociateWindow ( KiteEngine* kite )
    : _flags       (KtNoFlags)
    , _interrupt   (false)
    , _kite        (kite)
    , _gcells      ()
    , _segments    ()
    , _eventQueue  ()
    , _eventHistory()
    , _eventLoop   (10,50)
  { }


  NegociateWindow* NegociateWindow::create ( KiteEngine* kite )
  {
    NegociateWindow* negociateWindow = new NegociateWindow ( kite );
    return negociateWindow;
  }


  NegociateWindow::~NegociateWindow ()
  { }


  void  NegociateWindow::destroy ()
  { delete this; }


  Cell* NegociateWindow::getCell () const
  { return _kite->getCell(); }


  void  NegociateWindow::setGCells ( const Katabatic::GCellVector& gcells )
  {
    _gcells = gcells;

    loadRoutingPads( this );
    Session::revalidate();

    TrackElement*            segment;
    AutoSegmentLut           lut = Session::getKiteEngine()->_getAutoSegmentLut();
    AutoSegmentLut::iterator it  = lut.begin ();
    for ( ; it != lut.end() ; it++ ) {
      segment = Session::lookup( it->second );
      if (segment) segment->getDataNegociate()->update();
    }

    _statistics.setGCellsCount( _gcells.size() );
  }


  void  NegociateWindow::addRoutingEvent ( TrackElement* segment, unsigned int level )
  {
    DataNegociate* data = segment->getDataNegociate();
    if (not data or not data->hasRoutingEvent())
      _eventQueue.add( segment, level );
    else
      cerr << Bug( "NegociateWidow::addRoutingEvent(): Try to adds twice the same TrackElement event."
                   "\n       %p:%s."
                 , (void*)segment->base()->base()
                 , getString(segment).c_str()
                 ) << endl;
  }


  TrackElement* NegociateWindow::createTrackSegment ( AutoSegment* autoSegment, unsigned int flags )
  {
    ltrace(200) << "NegociateWindow::createTrackSegment() - " << autoSegment << endl;
    ltracein(159);

  // Special case: fixed AutoSegments must not interfere with blockages.
  // Ugly: uses of getExtensionCap().
    if (autoSegment->isFixed()) {
      RoutingPlane* plane = Session::getKiteEngine()->getRoutingPlaneByLayer(autoSegment->getLayer());
      Track*        track = plane->getTrackByPosition( autoSegment->getAxis() );
      size_t        begin;
      size_t        end;
      Interval      fixedSpan;
      Interval      blockageSpan;

      autoSegment->getCanonical( fixedSpan );
      fixedSpan.inflate( Session::getExtensionCap(autoSegment->getLayer())-1 );

      track->getOverlapBounds( fixedSpan, begin, end );
      for ( ; (begin < end) ; begin++ ) {

        TrackElement* other = track->getSegment(begin);
        ltrace(200) << "| overlap: " << other << endl;

        if (not other->isBlockage()) continue;

        other->getCanonical( blockageSpan );
        blockageSpan.inflate( Session::getExtensionCap(autoSegment->getLayer()) );

        ltrace(200) << "  fixed:" << fixedSpan << " vs. blockage:" << blockageSpan << endl;

        if (not fixedSpan.intersect(blockageSpan)) continue;

      // Overlap between fixed & blockage.
        ltrace(200) << "* Blockage overlap: " << autoSegment << endl;
        Session::destroyRequest( autoSegment );

        cerr << Warning( "Overlap between fixed %s and blockage at %s."
                       , getString(autoSegment).c_str()
                       , getString(blockageSpan).c_str() ) << endl;
        ltraceout(159);
        return NULL;
      }
    }

    Interval span;
    autoSegment = autoSegment->getCanonical( span );

    bool           created;
    TrackElement*  trackSegment  = TrackSegment::create( autoSegment, NULL, created );

    if (not (flags & KtLoadingStage))
      ltrace(159) << "* lookup: " << autoSegment << endl;

    if (created) {
      ltrace(159) << "* " << trackSegment << endl;

      RoutingPlane* plane = Session::getKiteEngine()->getRoutingPlaneByLayer(autoSegment->getLayer());
      Track*        track = plane->getTrackByPosition ( autoSegment->getAxis() );
      Interval      uside = autoSegment->getAutoSource()->getGCell()->getSide( perpandicularTo(autoSegment->getDirection()) );

      if (track->getAxis() > uside.getVMax()) track = track->getPreviousTrack();
      if (track->getAxis() < uside.getVMin()) track = track->getNextTrack();

      ltrace(159) << "* GCell U-side " << uside << endl;
      ltrace(159) << "* " << plane << endl;
      ltrace(159) << "* " << track << endl;

      trackSegment->setAxis( track->getAxis(), Katabatic::SegAxisSet );
      trackSegment->invalidate();

      if (trackSegment->isFixed()) {
        Session::addInsertEvent( trackSegment, track );
      } else {
        _segments.push_back( trackSegment );
      }
    }

    if (not created and not (flags & KtLoadingStage)) {
      ltrace(200) << "TrackSegment already exists (and not in loading stage)." << endl;
    }

    ltraceout(159);

    return trackSegment;
  }


  double  NegociateWindow::computeWirelength ()
  {
    set<TrackElement*> accounteds;
    double totalWL = 0.0;

    for ( size_t igcell=0 ; igcell<_gcells.size() ; ++igcell ) {
      double        gcellWL = 0.0;
      Segment*      segment;
      TrackElement* trackSegment;

      const vector<AutoContact*>& contacts = _gcells[igcell]->getContacts();
      for ( size_t i=0 ; i<contacts.size() ; i++ ) {
        forEach ( Hook*, ihook, contacts[i]->getBodyHook()->getSlaveHooks() ) {
          Hook* sourceHook = dynamic_cast<Segment::SourceHook*>(*ihook);
          if (not sourceHook) continue;
          
          segment       = dynamic_cast<Segment*>(sourceHook->getComponent());
          trackSegment  = Session::lookup( segment );
          if (trackSegment) {
            if (accounteds.find(trackSegment) != accounteds.end()) continue;

            accounteds.insert( trackSegment );
            gcellWL += DbU::getLambda( trackSegment->getLength() );
          }
        }
      }

    // Partial sum to limit rounding errors.
      totalWL += gcellWL;
    }

    return totalWL;
  }


  void  NegociateWindow::_createRouting  ( Katabatic::GCell* gcell )
  {
    ltrace(200) << "NegociateWindow::_createRouting() - " << gcell << endl;
    ltracein(200);

    Segment*     segment;
    AutoSegment* autoSegment;

    ltrace(149) << "AutoSegments from AutoContacts" << endl;
    const vector<AutoContact*>& contacts = gcell->getContacts();
    for ( size_t i=0 ; i<contacts.size() ; i++ ) {
      forEach ( Component*, component, contacts[i]->getSlaveComponents() ) {
        segment      = dynamic_cast<Segment*>(*component);
        autoSegment  = Session::base()->lookup( segment );
        ltrace(149) << autoSegment << endl;
        if (autoSegment and autoSegment->isCanonical()) {
          createTrackSegment( autoSegment, KtLoadingStage );
        }
      }
    }

    ltrace(149) << "_segments.size():" << _segments.size() << endl;
    ltraceout(200);
  }


  size_t  NegociateWindow::_negociate ()
  {
    ltrace(500) << "Deter| NegociateWindow::_negociate()" << endl;
    ltrace(150) << "NegociateWindow::_negociate() - " << _segments.size() << endl;
    ltracein(149);

    cmess1 << "     o  Negociation Stage." << endl;

    unsigned long limit = _kite->getEventsLimit();

    _eventHistory.clear();
    _eventQueue.load( _segments );
    cmess2 << "        <queue:" <<  right << setw(8) << setfill('0') << _eventQueue.size() << ">" << endl;
    if (inltrace(500)) _eventQueue.dump();

    size_t count = 0;
    RoutingEvent::setStage( RoutingEvent::Negociate );
    while ( not _eventQueue.empty() and not isInterrupted() ) {
      RoutingEvent* event = _eventQueue.pop();

      if (tty::enabled()) {
        cmess2 << "        <event:" << tty::bold << right << setw(8) << setfill('0')
               << RoutingEvent::getProcesseds() << tty::reset
               << " remains:" << right << setw(8) << setfill('0')
               << _eventQueue.size()
               << setfill(' ') << tty::reset << ">" << tty::cr;
        cmess2.flush ();
      } else {
        cmess2 << "        <event:" << right << setw(8) << setfill('0')
               << RoutingEvent::getProcesseds() << setfill(' ') << " "
               << event->getEventLevel() << ":" << event->getPriority() << "> "
               << event->getSegment()
               << endl;
        cmess2.flush();
      }

      event->process( _eventQueue, _eventHistory, _eventLoop );

      count++;
      if (RoutingEvent::getProcesseds() >= limit) setInterrupt( true );
    }
    if (count and cmess2.enabled() and tty::enabled()) cmess1 << endl;

    ltrace(500) << "Deter| Repair Stage" << endl;
    cmess1 << "     o  Repair Stage." << endl;

    ltrace(200) << "Loadind Repair queue." << endl;
    RoutingEvent::setStage( RoutingEvent::Repair );
    for ( size_t i=0 ; (i<_eventHistory.size()) and not isInterrupted() ; i++ ) {
      RoutingEvent* event = _eventHistory.getNth(i);

      if (not event->isCloned() and event->isUnimplemented()) {
        event->reschedule( _eventQueue, 0 );
      }
    }
    _eventQueue.commit();
    cmess2 << "        <repair.queue:" <<  right << setw(8) << setfill('0')
           << _eventQueue.size() << ">" << endl;

    count = 0;
  //_eventQueue.prepareRepair();
    while ( not _eventQueue.empty() and not isInterrupted() ) {
      RoutingEvent* event = _eventQueue.pop();

      if (tty::enabled()) {
        cmess2 << "        <repair.event:" << tty::bold << setw(8) << setfill('0')
               << RoutingEvent::getProcesseds() << tty::reset
               << " remains:" << right << setw(8) << setfill('0')
               << _eventQueue.size() << ">"
               << setfill(' ') << tty::reset << tty::cr;
        cmess2.flush();
      } else {
        cmess2 << "        <repair.event:" << setw(8) << setfill('0')
               << RoutingEvent::getProcesseds() << setfill(' ') << " "
               << event->getEventLevel() << ":" << event->getPriority() << "> "
               << event->getSegment()
               << endl;
        cmess2.flush();
      }

      event->process( _eventQueue, _eventHistory, _eventLoop );

      count++;
      if (RoutingEvent::getProcesseds() >= limit ) setInterrupt( true );
    }

    if (count and cmess2.enabled() and tty::enabled()) cmess1 << endl;

    size_t eventsCount = _eventHistory.size();

    _eventHistory.clear();
    _eventQueue.clear();

    if (RoutingEvent::getAllocateds() > 0) {
      cerr << Bug( "%d events remains after clear.", RoutingEvent::getAllocateds() ) << endl;
    }

    _statistics.setEventsCount( eventsCount );
    ltraceout(149);

    return eventsCount;
  }


  void  NegociateWindow::run ( unsigned int flags )
  {
    ltrace(150) << "NegociateWindow::run()" << endl;
    ltracein(149);

    cmess1 << "  o  Running Negociate Algorithm" << endl;

    TrackElement::setOverlapCostCB( NegociateOverlapCost );
    RoutingEvent::resetProcesseds();

    for ( size_t igcell=0 ; igcell<_gcells.size() ; ++igcell ) {
      _createRouting( _gcells[igcell] );
    }
    Session::revalidate();

    if (not (flags & KtPreRoutedStage)) {
      _kite->preProcess();
      Session::revalidate();
    }

    _kite->setMinimumWL( computeWirelength() );

#if defined(CHECK_DATABASE)
    unsigned int overlaps = 0;
    Session::getKiteEngine()->_check( overlaps, "after _createRouting(GCell*)" );
#endif 

    _flags |= flags;
    _negociate();
    printStatistics();

    if (flags & KtPreRoutedStage) {
      _kite->setFixedPreRouted();
    }

    Session::get()->isEmpty();

# if defined(CHECK_DATABASE)
    _kite->_check( overlaps, "after negociation" );
# endif

    ltraceout(149);
  }


  void  NegociateWindow::printStatistics () const
  {
    cmess1 << "  o  Computing statistics." << endl;
    cmess1 << Dots::asSizet("     - Processeds Events Total",RoutingEvent::getProcesseds()) << endl;
    cmess1 << Dots::asSizet("     - Unique Events Total"
                           ,(RoutingEvent::getProcesseds() - RoutingEvent::getCloneds())) << endl;
    cmess1 << Dots::asSizet("     - # of GCells",_statistics.getGCellsCount()) << endl;
    _kite->printCompletion();

    addMeasure<size_t>( getCell(), "Events" , RoutingEvent::getProcesseds(), 12 );
    addMeasure<size_t>( getCell(), "UEvents", RoutingEvent::getProcesseds()-RoutingEvent::getCloneds(), 12 );

    Histogram* densityHistogram = new Histogram ( 1.0, 0.1, 2 );
    addMeasure<Histogram>( getCell(), "GCells Density Histogram", densityHistogram );

    densityHistogram->setFileExtension( ".density.histogram" );
    densityHistogram->setMainTitle    ( "GCell Densities" );
    densityHistogram->setTitle        ( "Avg. Density", 0 );
    densityHistogram->setTitle        ( "Peak Density", 1 );
    densityHistogram->setColor        ( "green", 0 );
    densityHistogram->setColor        ( "red"  , 1 );

    const Katabatic::GCellVector* gcells = getKiteEngine()->getGCellGrid()->getGCellVector();

    getKiteEngine()->getGCellGrid()->setDensityMode( Katabatic::GCellGrid::MaxHVDensity );
    for ( size_t igcell=0 ; igcell<(*gcells).size() ; ++igcell ) {
      densityHistogram->addSample( (*gcells)[igcell]->getDensity(), 0 );
    }

    getKiteEngine()->getGCellGrid()->setDensityMode( Katabatic::GCellGrid::MaxDensity );
    for ( size_t igcell=0 ; igcell<(*gcells).size() ; ++igcell ) {
      densityHistogram->addSample( (*gcells)[igcell]->getDensity(), 1 );
    }

    densityHistogram->normalize( 0 );
    densityHistogram->normalize( 1 );
  }


  string  NegociateWindow::_getString () const
  {
    ostringstream  os;

    os << "<" << _getTypeName() << ">";
    return os.str();
  }


  Record* NegociateWindow::_getRecord () const
  {
    Record* record = new Record ( _getString() );
                                     
    record->add( getSlot( "_gcells", _gcells ) );
    return record;
  }


} // Kite namespace.
