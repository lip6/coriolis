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
// |  C++ Module  :       "./NegociateWindow.cpp"                    |
// +-----------------------------------------------------------------+


#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include "hurricane/Breakpoint.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Warning.h"
#include "hurricane/Bug.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/Measures.h"
#include "crlcore/Histogram.h"
#include "anabatic/AutoContact.h"
#include "katana/DataNegociate.h"
#include "katana/TrackElement.h"
#include "katana/TrackMarker.h"
#include "katana/TrackCost.h"
#include "katana/Track.h"
#include "katana/TrackSegment.h"
#include "katana/RoutingPlane.h"
#include "katana/RoutingEventQueue.h"
#include "katana/RoutingEventHistory.h"
#include "katana/RoutingEventLoop.h"
#include "katana/NegociateWindow.h"
#include "katana/KatanaEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;
  using namespace Katana;


  void  NegociateOverlapCost ( const TrackElement* segment, TrackCost& cost )
  {
    cdebug_log(9000,0) << "Deter| NegociateOverlapCost() " << segment << endl;
    Interval intersect = segment->getCanonicalInterval();

    if (not intersect.intersect(cost.getInterval())) return;

    if (segment->isBlockage() or segment->isFixed()) {
      cdebug_log(159,0) << "Infinite cost from: " << segment << endl;
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
      DbU::Unit beginOverlap = cost.getInterval().getVMin() - intersect.getVMin();
      DbU::Unit   endOverlap = intersect.getVMax() - cost.getInterval().getVMax();

      cost.setLonguestOverlap( std::min( beginOverlap, endOverlap ) );
      cost.setGlobalEnclosed();
    }

    DataNegociate* data = segment->getDataNegociate();
    if (not data) return;

    TrackElement*  refSegment = cost.getRefElement();
    DataNegociate* refData    = refSegment->getDataNegociate();
    AutoSegment*   refBase    = refSegment->base();
    AutoSegment*      base    =    segment->base();

    if (base and refBase and refData) {
      if (   (   base->getRpDistance() == 0)
         and (refBase->getRpDistance() >  0)
         and (refData->getState()      >  DataNegociate::RipupPerpandiculars)
         and (   data->getState()      == DataNegociate::RipupPerpandiculars)
         and (   data->getRipupCount() >  4)) {
        cost.setAtRipupLimit();
      }

      if (   refSegment->isNonPref()
         and segment->isUnbreakable()
         and (data->getState()      >= DataNegociate::Minimize)
         and (data->getStateCount() >= 2) ) {
        cost.setAtRipupLimit();
      }
    }

    cost.mergeRipupCount( data->getRipupCount() );
    if (segment->isLocal()) {
      cost.mergeDataState( data->getState() );
      if (data->getState() >=  DataNegociate::LocalVsGlobal) {
        cdebug_log(159,0) << "MaximumSlack/LocalVsGlobal for " << segment << endl;
        cost.setAtRipupLimit();
      }
    }

    if (data->getState() == DataNegociate::MaximumSlack) {
      cost.setAtRipupLimit();
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
      cdebug_log(9000,0) << "Deter|     incTerminals() " << boolalpha << cost.isForGlobal() << " " << (data->getTerminals()*100) << endl;
      cost.incTerminals( data->getTerminals()*100 );
    } else {
      cdebug_log(9000,0) << "Deter|     isForGlobal() " << boolalpha << cost.isForGlobal() << endl;
    }

    cdebug_log(159,0) << "| Increment Delta: " << DbU::getValueString(intersect.getSize()) << endl;
    cost.incDelta( intersect.getSize() );

    // if (segment->base()->getId() == 70433) {
    //   cdebug_log(159,0) << "| G:" << cost.isForGlobal()
    //                     << " L:" << segment->isLocal()
    //                     << " rpD:" << segment->base()->getRpDistance()
    //                     << " state:" << data->getState()
    //                     << " (Dogleg:" << DataNegociate::Dogleg
    //                     << ") ripup:" << data->getRipupCount()
    //                     << endl;
    // }

    if (   cost.isForGlobal()
       and segment->isLocal()
       and (segment->base()->getRpDistance() <  2)
       and (data->getState()      >= DataNegociate::Dogleg)
     //and (data->getRipupCount() >  Session::getConfiguration()->getRipupLimit(Configuration::LocalRipupLimit) - 2)
       ) {
      cost.setInfinite();
      cdebug_log(159,0) << "Infinite cost from (RP access)" << segment << endl;
    }
      
  }


  void  loadRoutingPads ( NegociateWindow* nw )
  {
    AllianceFramework* af   = AllianceFramework::get ();
    RoutingGauge*      rg   = nw->getKatanaEngine()->getConfiguration()->getRoutingGauge();
  //bool               isVH = rg->isVH();

    for( Net* net : nw->getCell()->getNets() ) {
      if (net->getType() == Net::Type::POWER ) continue;
      if (net->getType() == Net::Type::GROUND) continue;
      if (net->getType() == Net::Type::CLOCK ) continue;
      if (af->isBLOCKAGE(net->getName())) continue;

      for( RoutingPad* rp : net->getRoutingPads() ) {
        size_t depth = rg->getLayerDepth(rp->getLayer());
        if (depth == 0) {
          TrackMarker::create( rp, 1 );
        //if (isVH) TrackMarker::create( rp, 2 );
        }
        if (depth == 1) {
          TrackMarker::create( rp, 1 );
        }
      }
    }
  }


  class CompareByPriority {
    public:
      inline bool operator() ( const TrackSegment* lhs, const TrackSegment* rhs )
      { return lhs->getPriority() > rhs->getPriority(); }
  };
 

  void  computeNetPriority ( Net* net )
  {
    DebugSession::open( net, 159, 160 );

    cdebug_log(159,1) << "::computeNetPriority() " << net << endl;

    vector<TrackSegment*> segments;
    for ( Segment* segment : net->getSegments() ) {
      TrackSegment* canonical = dynamic_cast<TrackSegment*>( Session::lookup( segment ) );
      if (canonical) segments.push_back( canonical );
    }

    for ( TrackSegment* segment : segments ) segment->computePriority();
    sort( segments.begin(), segments.end(), CompareByPriority() );

    for ( TrackSegment* segment : segments ) {
      segment->computeAlignedPriority();
    }

    cdebug_tabw(159,-1);

    DebugSession::close();
  }


} // Anonymous namespace.


namespace Katana {

  using std::ofstream;
  using std::cerr;
  using std::endl;
  using std::setw;
  using std::left;
  using std::right;
  using std::setprecision;
  using Hurricane::Breakpoint;
  using Hurricane::Warning;
  using Hurricane::Bug;
  using Hurricane::tab;
  using Hurricane::ForEachIterator;
  using Hurricane::DebugSession;
  using Hurricane::UpdateSession;
  using CRL::Histogram;
  using CRL::addMeasure;
  using Anabatic::AutoContact;
  using Anabatic::AutoSegment;
  using Anabatic::AutoSegmentLut;
  using Anabatic::perpandicularTo;


// -------------------------------------------------------------------
// Class  :  "NegociateWindow".


  NegociateWindow::NegociateWindow ( KatanaEngine* katana )
    : _flags       (Flags::NoFlags)
    , _interrupt   (false)
    , _katana        (katana)
    , _gcells      ()
    , _segments    ()
    , _eventQueue  ()
    , _eventHistory()
    , _eventLoop   (10,70)
  { }


  NegociateWindow* NegociateWindow::create ( KatanaEngine* katana )
  {
    NegociateWindow* negociateWindow = new NegociateWindow ( katana );
    return negociateWindow;
  }


  NegociateWindow::~NegociateWindow ()
  { }


  void  NegociateWindow::destroy ()
  { delete this; }


  Cell* NegociateWindow::getCell () const
  { return _katana->getCell(); }


  void  NegociateWindow::setGCells ( const vector<GCell*>& gcells )
  {
    _gcells = gcells;

    if (not Session::getKatanaEngine()->isChannelMode()) loadRoutingPads( this );
    Session::revalidate();

    for ( auto element : Session::getKatanaEngine()->_getAutoSegmentLut() ) {
      TrackElement* segment = Session::lookup( element.second );
      if (segment) segment->getDataNegociate()->update();
    }

    _statistics.setGCellsCount( _gcells.size() );
  }


  void  NegociateWindow::addRoutingEvent ( TrackElement* segment, uint32_t level )
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


  TrackElement* NegociateWindow::createTrackSegment ( AutoSegment* autoSegment, Flags flags )
  {
    DebugSession::open( autoSegment->getNet(), 159, 160 );

    cdebug_log(159,1) << "NegociateWindow::createTrackSegment() - " << autoSegment << endl;
    RoutingPlane* plane     = Session::getKatanaEngine()->getRoutingPlaneByLayer(autoSegment->getLayer());
    Track*        refTrack  = plane->getTrackByPosition( autoSegment->getAxis() );
    Track*        insTrack  = NULL;
    size_t        trackSpan = 1;

  // Special case: fixed AutoSegments must not interfere with blockages.
  // Ugly: uses of getExtensionCap().
    if (autoSegment->isFixed()) {
      size_t        begin;
      size_t        end;
      Interval      fixedSpan;
      Interval      blockageSpan;

      if (refTrack->getAxis() != autoSegment->getAxis()) { 
        trackSpan = 2;
        refTrack  = plane->getTrackByPosition( autoSegment->getAxis(), Constant::Inferior );
        insTrack  = refTrack;
      }

      Track* track = refTrack;
      for ( size_t ispan=0 ; track and (ispan < trackSpan) ; ++ispan, track=track->getNextTrack() ) {
        autoSegment->getCanonical( fixedSpan );
        fixedSpan.inflate( Session::getExtensionCap(autoSegment->getLayer())-1 );

        track->getOverlapBounds( fixedSpan, begin, end );
        for ( ; (begin < end) ; begin++ ) {
          TrackElement* other = track->getSegment(begin);
          cdebug_log(159,0) << "| overlap: " << other << endl;

          if (not other->isBlockage()) continue;

          other->getCanonical( blockageSpan );
          blockageSpan.inflate( Session::getExtensionCap(autoSegment->getLayer()) );

          cdebug_log(159,0) << "  fixed:" << fixedSpan << " vs. blockage:" << blockageSpan << endl;
          if (not fixedSpan.intersect(blockageSpan)) continue;

        // Overlap between fixed & blockage.
          cdebug_log(159,0) << "* Blockage overlap: " << autoSegment << endl;
        //Session::destroyRequest( autoSegment );
          cerr << Warning( "Overlap between fixed %s and blockage at %s."
                         , getString(autoSegment).c_str()
                         , getString(blockageSpan).c_str() ) << endl;
          cdebug_tabw(159,-1);
          DebugSession::close();
          return NULL;
        }
      }
    }

    Interval span;
    autoSegment = autoSegment->getCanonical( span );

    bool           created;
    TrackElement*  trackSegment = TrackSegment::create( autoSegment, insTrack, created );

    if (not (flags & Flags::LoadingStage))
      cdebug_log(159,0) << "* lookup: " << autoSegment << endl;

    if (created) {
      cdebug_log(159,0) << "* " << trackSegment << endl;

      if (trackSegment->isNonPref()) {
        _segments.push_back( trackSegment );
        cdebug_log(159,0) << "Non-preferred direction, do not attempt to set on track." << endl;
        cdebug_tabw(159,-1);
        DebugSession::close();
        return trackSegment;
      }

      Interval uside = autoSegment->getAutoSource()->getGCell()->getSide( perpandicularTo(autoSegment->getDirection()) );
      Interval constraints;
      autoSegment->getConstraints( constraints );
      cdebug_log(159,0) << "* Constraints " << constraints << endl;

      uside.intersection( constraints );
      cdebug_log(159,0) << "* Constraints+U-side " << constraints << endl;
      cdebug_log(159,0) << "* Nearest " << refTrack << endl;

      if (not refTrack)
        throw Error( "NegociateWindow::createTrackSegment(): No track near axis of %s."
                   , getString(autoSegment).c_str() );

      if (not insTrack) {
        insTrack = refTrack;
        if (refTrack->getAxis() > uside.getVMax()) insTrack = refTrack->getPreviousTrack();
        if (refTrack->getAxis() < uside.getVMin()) insTrack = refTrack->getNextTrack();
      }
      if (not insTrack)
        throw Error( "NegociateWindow::createTrackSegment(): No track near axis of %s (after adjust)." 
                   , getString(autoSegment).c_str() );

      cdebug_log(159,0) << "* GCell U-side " << uside << endl;
      cdebug_log(159,0) << "* " << plane << endl;
      cdebug_log(159,0) << "* " << insTrack << endl;

      if (trackSpan == 1)
        trackSegment->setAxis( insTrack->getAxis(), AutoSegment::SegAxisSet );
      trackSegment->invalidate();

      if (trackSegment->isFixed() and not trackSegment->getTrack()) {
        Session::addInsertEvent( trackSegment, insTrack, refTrack->getAxis() );
      } else {
        _segments.push_back( trackSegment );
      }
    }

    if (not created and not (flags & Flags::LoadingStage)) {
      cdebug_log(159,0) << "TrackSegment already exists (and not in loading stage)." << endl;
    }

    cdebug_tabw(159,-1);
    DebugSession::close();

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
        for( Hook* hook : contacts[i]->getBodyHook()->getSlaveHooks() ) {
          Hook* sourceHook = dynamic_cast<Segment::SourceHook*>(hook);
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


  void  NegociateWindow::_computePriorities ()
  {
    for ( Net* net : getCell()->getNets() ) {
      if (NetRoutingExtension::isAnalog(net)) computeNetPriority( net );
    }
  }


  void  NegociateWindow::_createRouting  ( Anabatic::GCell* gcell )
  {
    cdebug_log(159,1) << "NegociateWindow::_createRouting() - " << gcell << endl;

    Segment*     segment;
    AutoSegment* autoSegment;

    cdebug_log(159,0) << "AutoSegments from AutoContacts" << endl;
    const vector<AutoContact*>& contacts = gcell->getContacts();
    for ( size_t i=0 ; i<contacts.size() ; i++ ) {
      for( Component* component : contacts[i]->getSlaveComponents() ) {
        segment      = dynamic_cast<Segment*>(component);
        autoSegment  = Session::base()->lookup( segment );
        cdebug_log(159,0) << autoSegment << endl;
        if (autoSegment and autoSegment->isCanonical()) {
          createTrackSegment( autoSegment, Flags::LoadingStage );
        }
      }
    }

    cdebug_log(159,0) << "_segments.size():" << _segments.size() << endl;
    cdebug_tabw(159,-1);
  }


  void  NegociateWindow::_pack ( size_t& count, bool last )
  {
    uint64_t limit     = _katana->getEventsLimit();
    uint32_t pushStage = RoutingEvent::getStage();
    RoutingEvent::setStage( RoutingEvent::Pack );

    RoutingEventQueue  packQueue;
  //for ( size_t i = (count > 600) ? count-600 : 0
  //    ; (i<_eventHistory.size()-(last ? 0 : 100)) and not isInterrupted() ; i++ ) {
    for ( size_t i=0 ; i<_eventHistory.size() ; ++i ) {
      RoutingEvent* event = _eventHistory.getNth(i);

      if ( event and not event->isCloned() ) {
        cerr << "Cloned:" << event->isCloned()
             << " UTurn:" << event->getSegment()->isUTurn() << " " << event->getSegment() << endl;
      }
          
      if ( event and not event->isCloned() and event->getSegment()->isUTurn() ) {
        event->reschedule( packQueue, 0 );
      }
    }
    packQueue.commit();

    while ( not packQueue.empty() and not isInterrupted() ) {
      RoutingEvent* event = packQueue.pop();

      if (tty::enabled()) {
        cmess2 << "        <pack.event:" << tty::bold << setw(8) << setfill('0')
               << RoutingEvent::getProcesseds() << tty::reset
               << " remains:" << right << setw(8) << setfill('0')
               << packQueue.size() << ">"
               << setfill(' ') << tty::reset << tty::cr;
        cmess2.flush();
      } else {
        cmess2 << "        <pack.event:" << setw(8) << setfill('0')
               << RoutingEvent::getProcesseds() << setfill(' ') << " "
               << event->getEventLevel() << ":" << event->getPriority() << "> "
               << event->getSegment()
               << endl;
        cmess2.flush();
      }

      event->process( packQueue, _eventHistory, _eventLoop );

      if (RoutingEvent::getProcesseds() >= limit) setInterrupt( true );
    }
  // Count will be wrong!

    RoutingEvent::setStage( pushStage );
  }


  size_t  NegociateWindow::_negociate ()
  {
    cdebug_log(9000,0) << "Deter| NegociateWindow::_negociate()" << endl;
    cdebug_log(159,1) << "NegociateWindow::_negociate() - " << _segments.size() << endl;

    cmess1 << "     o  Negociation Stage." << endl;

    unsigned long limit     = _katana->getEventsLimit();
    bool          profiling = _katana->profileEventCosts();
    ofstream      ofprofile;

    if (profiling) ofprofile.open( "katana.profile.txt" );

    _eventHistory.clear();
    _eventQueue.load( _segments );
    cmess2 << "        <queue:" <<  right << setw(8) << setfill('0') << _eventQueue.size() << ">" << endl;
    if (cdebug.enabled(9000)) _eventQueue.dump();

    size_t count = 0;
    RoutingEvent::setStage( RoutingEvent::Negociate );
    while ( not _eventQueue.empty() and not isInterrupted() ) {
      RoutingEvent* event = _eventQueue.pop();

      if (ofprofile.is_open()) {
        size_t depth = _katana->getConfiguration()->getLayerDepth( event->getSegment()->getLayer() );
        if (depth < 6) {
          ofprofile << setw(10) << right << count << " ";
          for ( size_t i=0 ; i<6 ; ++i ) {
            if (i == depth)
              ofprofile << setw(10) << right << setprecision(2) << event->getPriority() << " ";
            else
              ofprofile << setw(10) << right << setprecision(2) << 0.0 << " ";
          }

          ofprofile << setw( 2) << right << event->getEventLevel() << endl;
        }
      }

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
               << event->getEventLevel() << ":" << event->getPriority()
               << ":" << DbU::getValueString(event->getSegment()->getLength()) << "> "
               << event->getSegment()
               << endl;
        cmess2.flush();
      }

    //if (RoutingEvent::getProcesseds() == 14473)
    //  Breakpoint::stop( 0, "Before processing RoutingEvent 14473." );

      event->process( _eventQueue, _eventHistory, _eventLoop );
      count++;

      // if (event->getSegment()->getNet()->getId() == 239546) {
      //   UpdateSession::close();
      //   ostringstream message;
      //   message << "After processing an event from Net id:239546\n" << event;
      //   Breakpoint::stop( 0, message.str() );
      //   UpdateSession::open();
      // }

    //if (count and not (count % 500)) {
    //  _pack( count, false );
    //} 

      if (RoutingEvent::getProcesseds() >= limit) setInterrupt( true );
    }
  //_pack( count, true );
    if (count and cmess2.enabled() and tty::enabled()) cmess1 << endl;

  //if (not _katana->isChannelMode() ) {
      cdebug_log(9000,0) << "Deter| Repair Stage" << endl;
      cmess1 << "     o  Repair Stage." << endl;

      cdebug_log(159,0) << "Loadind Repair queue." << endl;
      RoutingEvent::setStage( RoutingEvent::Repair );
      for ( size_t i=0 ; (i<_eventHistory.size()) and not isInterrupted() ; i++ ) {
        RoutingEvent* event = _eventHistory.getNth(i);

        if (not event->isCloned() and event->isUnimplemented()) {
          event->reschedule( _eventQueue, 0 );
        }
      }
      _eventQueue.commit();
      cmess2 << "        <repair.queue:" <<  right << setw(8) << setfill('0')
             << _eventQueue.size() << ">" << setfill(' ') << endl;

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
        if (RoutingEvent::getProcesseds() >= limit) setInterrupt( true );
      }

      if (_katana->getConfiguration()->runRealignStage()) {
        cmess1 << "     o  Realign Stage." << endl;
        
        cdebug_log(159,0) << "Loadind realign queue." << endl;
        RoutingEvent::setStage( RoutingEvent::Realign );
        for ( size_t i=0 ; (i<_eventHistory.size()) and not isInterrupted() ; i++ ) {
          RoutingEvent* event = _eventHistory.getNth(i);
          if (not event->isCloned() and event->getSegment()->canRealign())
            event->reschedule( _eventQueue, 0 );
        }
        _eventQueue.commit();
        cmess2 << "        <realign.queue:" <<  right << setw(8) << setfill('0')
               << _eventQueue.size() << ">" << setfill(' ') << endl;
        count = 0;
        while ( not _eventQueue.empty() and not isInterrupted() ) {
          RoutingEvent* event = _eventQueue.pop();
          if (tty::enabled()) {
            cmess2 << "        <realign.event:" << tty::bold << setw(8) << setfill('0')
                   << RoutingEvent::getProcesseds() << tty::reset
                   << " remains:" << right << setw(8) << setfill('0')
                   << _eventQueue.size() << ">"
                   << setfill(' ') << tty::reset << tty::cr;
            cmess2.flush();
          } else {
            cmess2 << "        <realign.event:" << setw(8) << setfill('0')
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
      }

      if (count and cmess2.enabled() and tty::enabled()) cmess1 << endl;
  //}

    size_t eventsCount = _eventHistory.size();

    _eventHistory.clear();
    _eventQueue.clear();

    if (RoutingEvent::getAllocateds() > 0) {
      cerr << Bug( "%d events remains after clear.", RoutingEvent::getAllocateds() ) << endl;
    }

    if (ofprofile.is_open()) ofprofile.close();
    _statistics.setEventsCount( eventsCount );
    cdebug_tabw(159,-1);

    return eventsCount;
  }


  void  NegociateWindow::run ( Flags flags )
  {
    cdebug_log(159,1) << "NegociateWindow::run()" << endl;

    cmess1 << "  o  Running Negociate Algorithm" << endl;

    TrackElement::setOverlapCostCB( NegociateOverlapCost );
    RoutingEvent::resetProcesseds();

    for ( size_t igcell=0 ; igcell<_gcells.size() ; ++igcell ) {
      _createRouting( _gcells[igcell] );
    }
    Session::revalidate();
    _computePriorities();
    
    if (not (flags & Flags::PreRoutedStage)) {
      _katana->preProcess();
      _katana->_computeCagedConstraints();
      Session::revalidate();
    }

    _katana->setMinimumWL( computeWirelength() );

#if defined(CHECK_DATABASE)
    uint32_t overlaps = 0;
    Session::getKatanaEngine()->_check( overlaps, "after _createRouting(GCell*)" );
#endif 

    if (flags & Flags::PairSymmetrics) {
      _katana->pairSymmetrics();
      Session::revalidate();
    }

    _flags |= flags;
    _negociate();
    printStatistics();

    if (flags & Flags::PreRoutedStage) {
      _katana->setFixedPreRouted();
    }

    Session::revalidate();
    Session::get()->isEmpty();

# if defined(CHECK_DATABASE)
    _katana->_check( overlaps, "after negociation" );
# endif

    cdebug_tabw(159,-1);
  }


  void  NegociateWindow::printStatistics () const
  {
    cmess1 << "  o  Computing statistics." << endl;
    cmess1 << Dots::asSizet("     - Processeds Events Total",RoutingEvent::getProcesseds()) << endl;
    cmess1 << Dots::asSizet("     - Unique Events Total"
                           ,(RoutingEvent::getProcesseds() - RoutingEvent::getCloneds())) << endl;
    cmess1 << Dots::asSizet("     - # of GCells",_statistics.getGCellsCount()) << endl;
    _katana->printCompletion();

    _katana->addMeasure<size_t>( "Events" , RoutingEvent::getProcesseds(), 12 );
    _katana->addMeasure<size_t>( "UEvents", RoutingEvent::getProcesseds()-RoutingEvent::getCloneds(), 12 );

    Histogram* densityHistogram = new Histogram ( 1.0, 0.1, 2 );
    _katana->addMeasure<Histogram>( "GCells Density Histogram", densityHistogram );

    densityHistogram->setFileExtension( ".density.histogram" );
    densityHistogram->setMainTitle    ( "GCell Densities" );
    densityHistogram->setTitle        ( "Avg. Density", 0 );
    densityHistogram->setTitle        ( "Peak Density", 1 );
    densityHistogram->setColor        ( "green", 0 );
    densityHistogram->setColor        ( "red"  , 1 );

    const vector<GCell*>& gcells = getKatanaEngine()->getGCells();

    getKatanaEngine()->setDensityMode( Anabatic::AnabaticEngine::MaxHVDensity );
    for ( size_t igcell=0 ; igcell<gcells.size() ; ++igcell ) {
      densityHistogram->addSample( gcells[igcell]->getDensity(), 0 );
    }

    getKatanaEngine()->setDensityMode( Anabatic::AnabaticEngine::MaxDensity );
    for ( size_t igcell=0 ; igcell<gcells.size() ; ++igcell ) {
      densityHistogram->addSample( gcells[igcell]->getDensity(), 1 );
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


} // Katana namespace.
