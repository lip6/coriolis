
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


#include  <vector>
#include  <algorithm>
#include  <iomanip>

#include  <boost/filesystem/operations.hpp>
#include  <boost/filesystem/fstream.hpp>
namespace bfs = boost::filesystem;

#include  "hurricane/Warning.h"
#include  "hurricane/Bug.h"
#include  "hurricane/RoutingPad.h"
#include  "hurricane/Net.h"
#include  "hurricane/Cell.h"
#include  "crlcore/Utilities.h"
#include  "crlcore/AllianceFramework.h"
#include  "crlcore/Measures.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/GCellGrid.h"

#include  "kite/DataNegociate.h"
#include  "kite/TrackElement.h"
#include  "kite/TrackMarker.h"
#include  "kite/TrackCost.h"
#include  "kite/Track.h"
#include  "kite/TrackSegment.h"
#include  "kite/RoutingPlane.h"
#include  "kite/RoutingEventQueue.h"
#include  "kite/RoutingEventHistory.h"
#include  "kite/NegociateWindow.h"
#include  "kite/KiteEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;
  using namespace Kite;


  class Histogram {
    public:
            Histogram ( double range, double step, size_t nbSets );
           ~Histogram ();
      void  addSample ( double, size_t iset );
      void  toStream  ( ostream& );
      void  toFile    ( bfs::path& );
      void  toGnuplot ( string design );
      void  normalize ( double totalSamples, size_t iset );
    private:
      double                   _range;
      double                   _step;
      vector< vector<float> >  _sets;
  };


  Histogram::Histogram ( double range, double step, size_t nbSets )
    : _range  (range)
    , _step   (step)
    , _sets   ()
  {
    size_t binSize = (size_t)rint ( _range / _step );
    for ( size_t iset=0 ; iset<nbSets ; ++iset ) {
      _sets.push_back ( vector<float>() );
      for ( size_t i=0 ; i<binSize ; ++i ) _sets.back().push_back(0);
    }
  }


  Histogram::~Histogram ()
  { }


  void  Histogram::addSample ( double sample, size_t iset )
  {
    size_t binIndex = (size_t)rint ( sample / _step );
    if ( binIndex > _sets.front().size() ) binIndex = _sets.front().size() - 1;

    _sets[iset][binIndex] += 1.0;
  }


  void  Histogram::normalize ( double totalSamples, size_t iset )
  {
    for ( size_t i=0 ; i<_sets[iset].size() ; ++i ) _sets[iset][i] /= totalSamples;
  }


  void  Histogram::toStream ( ostream& o )
  {
    o << setprecision(3);

    for ( size_t i=0 ; i<_sets.front().size() ; ++i ) {
      for ( size_t iset=0 ; iset<_sets.size() ; ++iset ) {
        o << _sets[iset][i] << " ";
      }
      o << "\n";
    }
  }


  void  Histogram::toFile ( bfs::path& path )
  {
    bfs::ofstream fd ( path );
    toStream ( fd );
    fd.close ();
  }


  void  Histogram::toGnuplot ( string design )
  {
    bfs::path datFile = design + ".densityHist.dat";
    toFile ( datFile );

    bfs::path pltFile = design + ".densityHist.plt";
    bfs::ofstream fd ( pltFile );

    fd << "set grid\n";
    fd << "set grid noxtics\n";
    fd << "set xrange [-0.5:9.5]\n";
    fd << "set xtics ( ";
    for ( size_t i=0 ; i<10 ; ++i ) {
      fd << ((i) ? " ," : "") << "\"<" << ((i+1)*10) << "%%\" " << i;
    }
    fd << " )\n";

    fd << "set yrange [0:.4]\n";
    fd << "set ytics ( ";
    for ( float i=0.0 ; i<=40.0 ; i+=10.0 ) {
      fd << ((i != 0.0) ? " ," : "") << "\"" << i << "%%\" " << (i/100.0);
    }
    fd << " )\n";

    fd << "set style histogram cluster gap 1\n";
    fd << "set style fill solid noborder\n";
    fd << "set boxwidth 1\n";
    fd << "plot \"" << design << ".densityHist.dat\" using 1 title \"Avg. density\" with histogram linecolor rgb \"green\", \\\n";
    fd << "     \"" << design << ".densityHist.dat\" using 2 title \"Peak. density\" with histogram linecolor rgb \"red\"\n";

    fd.close ();
  }


  void  NegociateOverlapCost ( const TrackElement* segment, TrackCost& cost )
  {
    Interval intersect = segment->getCanonicalInterval();

    if ( not intersect.intersect ( cost.getInterval() ) ) return;

    if ( segment->isBlockage() or segment->isFixed() ) {
      ltrace(200) << "Infinite cost from: " << segment << endl;
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

    cost.mergeRipupCount ( data->getRipupCount() );
    if ( segment->isLocal() ) {
      cost.mergeDataState ( data->getState() );
      if ( data->getState() >=  DataNegociate::LocalVsGlobal ) {
        ltrace(200) << "MaximumSlack/LocalVsGlobal for " << segment << endl;
      }
    }

    // if ( data->getRipupCount() > 3 ) {
    //   ltrace(200) << "Infinite cost from: " << segment << endl;
    //   cost.setFixed       ();
    //   cost.setInfinite    ();
    //   cost.setOverlap     ();
    //   cost.setHardOverlap ();
    //   return;
    // }

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
      if ( af->isBLOCKAGE(inet->getName()) ) continue;

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
  using Hurricane::Warning;
  using Hurricane::Bug;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::ForEachIterator;
  using CRL::addMeasure;
  using Katabatic::AutoContact;


// -------------------------------------------------------------------
// Class  :  "NegociateWindow".


  NegociateWindow::NegociateWindow ( KiteEngine* kite )
    : _slowMotion  (0)
    , _interrupt   (false)
    , _kite        (kite)
    , _gcells      ()
    , _segments    ()
    , _eventQueue  ()
    , _eventHistory()
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
  //sort ( _gcells.begin(), _gcells.end(), Katabatic::GCell::CompareGCellById() );

    loadRoutingPads ( this );
    Session::revalidate ();

    TrackElement*             segment;
    TrackElementLut           lut = Session::getKiteEngine()->_getTrackElementLut();
    TrackElementLut::iterator it  = lut.begin ();
    for ( ; it != lut.end() ; it++ ) {
      segment = it->second;
      segment->getDataNegociate()->update();
    }

    _statistics.setGCellsCount ( _gcells.size() );
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


  TrackElement* NegociateWindow::addTrackSegment ( AutoSegment* autoSegment, bool loading )
  {
    ltrace(200) << "NegociateWindow::addTrackSegment() - " << autoSegment << endl;
    ltracein(159);

  // Special case: fixed AutoSegments must not interfere with blockages.
  // Ugly: uses of getExtensionCap().
    if ( autoSegment->isFixed() ) {
      RoutingPlane* plane = Session::getKiteEngine()->getRoutingPlaneByLayer(autoSegment->getLayer());
      Track*        track = plane->getTrackByPosition ( autoSegment->getAxis() );
      size_t        begin;
      size_t        end;
      Interval      fixedSpan;
      Interval      blockageSpan;

      autoSegment->getCanonical ( fixedSpan );
      fixedSpan.inflate ( Session::getExtensionCap()-1 );

      track->getOverlapBounds ( fixedSpan, begin, end );
      for ( ; (begin < end) ; begin++ ) {

        TrackElement* other = track->getSegment(begin);
        ltrace(200) << "| overlap: " << other << endl;

        if ( not other->isBlockage() ) continue;

        other->getCanonical ( blockageSpan );
        blockageSpan.inflate(Session::getExtensionCap());

        ltrace(200) << "  fixed:" << fixedSpan << " vs. blockage:" << blockageSpan << endl;

        if ( not fixedSpan.intersect(blockageSpan) ) continue;

      // Overlap between fixed & blockage.
        ltrace(200) << "* Blockage overlap: " << autoSegment << endl;
        Session::destroyRequest ( autoSegment );

        cerr << Warning("Overlap between fixed %s and blockage at %s."
                       ,getString(autoSegment).c_str(),getString(blockageSpan).c_str()) << endl;

        return NULL;
      }
    }

    Interval span;
    autoSegment = autoSegment->getCanonical ( span );

    bool           created;
    TrackElement*  trackSegment  = TrackSegment::create ( autoSegment, NULL, created );

    if ( not loading )
      ltrace(159) << "* lookup: " << autoSegment << endl;

    if ( created ) {
      ltrace(159) << "* " << trackSegment << endl;

      RoutingPlane* plane = Session::getKiteEngine()->getRoutingPlaneByLayer(autoSegment->getLayer());
      Track*        track = plane->getTrackByPosition ( autoSegment->getAxis() );
      Interval      uside = autoSegment->getAutoSource()->getGCell()->getUSide ( Constant::perpandicular(autoSegment->getDirection())/*, false */);

      if ( track->getAxis() > uside.getVMax() ) track = track->getPrevious();
      if ( track->getAxis() < uside.getVMin() ) track = track->getNext();

      trackSegment->setAxis ( track->getAxis(), Katabatic::Realignate|Katabatic::AxisSet );
      trackSegment->invalidate ();

      if ( trackSegment->isFixed() ) {
        Session::addInsertEvent ( trackSegment, track );
      } else {
        _segments.push_back ( trackSegment );
      }
    }

    if ( not created and not loading ) {
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

      vector<AutoContact*>* contacts = _gcells[igcell]->getContacts();
      for ( size_t i=0 ; i<contacts->size() ; i++ ) {
        forEach ( Hook*, ihook, (*contacts)[i]->getBodyHook()->getSlaveHooks() ) {
          Hook* sourceHook = dynamic_cast<Segment::SourceHook*>(*ihook);
          if ( not sourceHook ) continue;
          
          segment       = dynamic_cast<Segment*>(sourceHook->getComponent());
          trackSegment  = Session::lookup ( segment );
          if ( trackSegment ) {
            if ( accounteds.find(trackSegment) != accounteds.end() ) continue;

            accounteds.insert ( trackSegment );
            gcellWL += DbU::getLambda ( trackSegment->getLength() );
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
    vector<AutoContact*>* contacts = gcell->getContacts();
    for ( size_t i=0 ; i<contacts->size() ; i++ ) {
      forEach ( Component*, component, (*contacts)[i]->getSlaveComponents() ) {
        segment      = dynamic_cast<Segment*>(*component);
        autoSegment  = Session::base()->lookup ( segment );
        ltrace(149) << autoSegment << endl;
        if ( autoSegment and autoSegment->isCanonical() ) {
          addTrackSegment ( autoSegment, true );
        }
      }
    }

    ltrace(149) << "_segments.size():" << _segments.size() << endl;
    ltraceout(200);
  }


  size_t  NegociateWindow::_negociate ()
  {
    ltrace(150) << "NegociateWindow::_negociate() - " << _segments.size() << endl;
    ltracein(149);

    unsigned long limit = _kite->getEventsLimit();

    _eventHistory.clear();
    _eventQueue.load ( _segments );

    size_t count = 0;
    while ( not _eventQueue.empty() and not isInterrupted() ) {
      RoutingEvent* event = _eventQueue.pop ();

      event->process ( _eventQueue, _eventHistory );
      if (tty::enabled()) {
        cmess1 << "       <event:" << tty::bold << setw(7) << setfill('0')
               << RoutingEvent::getProcesseds() << setfill(' ') << tty::reset << ">" << tty::cr;
        cmess1.flush ();
      } else {
        cmess2 << "       <event:" << setw(7) << setfill('0')
               << RoutingEvent::getProcesseds() << setfill(' ') << "> id:"
               << event->getSegment()->getId() << " "
               << event->getSegment()->getNet()->getName()
               << endl;
        cmess2.flush();
      }

      if ( RoutingEvent::getProcesseds() >= limit ) setInterrupt ( true );
      count++;
    }
    if (count and tty::enabled()) cmess1 << endl;
    count = 0;

    ltrace(200) << "Dumping history." << endl;
    for ( size_t i=0 ; (i<_eventHistory.size()) and not isInterrupted() ; i++ ) {
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
          cmess1 << "       <event:"
                 << tty::bold << tty::fgcolor(tty::Red) << setw(7) << setfill('0')
                 << RoutingEvent::getProcesseds() << setfill(' ') << tty::reset << ">" << tty::cr;
          cmess1.flush ();
        } else {
          cmess1 << "       <event:" << setw(7) << setfill('0')
                 << RoutingEvent::getProcesseds() << setfill(' ') << ">" << endl;
        }
      }
    }
    if (count and tty::enabled()) cmess1 << endl;

    size_t eventsCount = _eventHistory.size();

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

    _statistics.setEventsCount ( eventsCount );
    ltraceout(149);

    return eventsCount;
  }


  void  NegociateWindow::run ( int slowMotion )
  {
    ltrace(150) << "NegociateWindow::run()" << endl;
    ltracein(149);

    TrackElement::setOverlapCostCB ( NegociateOverlapCost );
    RoutingEvent::resetProcesseds ();

    for ( size_t igcell=0 ; igcell<_gcells.size() ; ++igcell ) {
      _createRouting ( _gcells[igcell] );
    }
    Session::revalidate ();

    getKiteEngine()->setMinimumWL ( computeWirelength() );

#if defined(CHECK_DATABASE)
    unsigned int overlaps = 0;
    Session::getKiteEngine()->_check(overlaps,"after _createRouting(GCell*)");
#endif 

    _slowMotion = slowMotion;
    _negociate ();

    Session::get()->isEmpty();

# if defined(CHECK_DATABASE)
    _kite->_check ( overlaps, "after negociation" );
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

    addMeasure<size_t>( getCell(), "Events" , RoutingEvent::getProcesseds(), 12 );
    addMeasure<size_t>( getCell(), "UEvents", RoutingEvent::getProcesseds()-RoutingEvent::getCloneds(), 12 );

    Histogram densityHistogram ( 1.0, 0.1, 2 );

    const Katabatic::GCellVector* gcells = getKiteEngine()->getGCellGrid()->getGCellVector();

    getKiteEngine()->getGCellGrid()->setDensityMode ( Katabatic::GCellGrid::AverageHVDensity );
    for ( size_t igcell=0 ; igcell<(*gcells).size() ; ++igcell ) {
      densityHistogram.addSample ( (*gcells)[igcell]->getDensity(), 0 );
    }

    getKiteEngine()->getGCellGrid()->setDensityMode ( Katabatic::GCellGrid::MaxDensity );
    for ( size_t igcell=0 ; igcell<(*gcells).size() ; ++igcell ) {
      densityHistogram.addSample ( (*gcells)[igcell]->getDensity(), 1 );
    }

    densityHistogram.normalize ( (*gcells).size(), 0 );
    densityHistogram.normalize ( (*gcells).size(), 1 );
    densityHistogram.toGnuplot ( getString(getCell()->getName()) );
  }


  string  NegociateWindow::_getString () const
  {
    ostringstream  os;

    os << "<" << _getTypeName() << ">";
    return ( os.str() );
  }


  Record* NegociateWindow::_getRecord () const
  {
    Record* record = new Record ( _getString() );
                                     
    record->add ( getSlot ( "_gcells", _gcells ) );
    return ( record );
  }


} // End of Kite namespace.
