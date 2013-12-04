// -*- mode: C++; explicit-buffer-name: "Session.cpp<kite>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Session.cpp"                            |
// +-----------------------------------------------------------------+


#include  "hurricane/Bug.h"
#include  "hurricane/Point.h"
#include  "hurricane/Error.h"
#include  "katabatic/GCellGrid.h"
#include  "kite/Session.h"
#include  "kite/Track.h"
#include  "kite/TrackElement.h"
#include  "kite/KiteEngine.h"


namespace {

  using namespace Kite;


  const char* reopenSession =
    "Kite::Session::open() :\n\n"
    "    Session already open for %s (internal error).";


} // Anonymous namespace.


namespace Kite {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::Error;
  using Hurricane::Bug;
  using Hurricane::Point;


// -------------------------------------------------------------------
// Class  :  "Session".

  Session::Session ( KiteEngine* kite )
    : Katabatic::Session(kite)
    , _insertEvents()
    , _removeEvents()
    , _sortEvents  ()
  { }


  void  Session::_postCreate ()
  { Katabatic::Session::_postCreate(); }


  Session::~Session ()
  { }


  void  Session::_preDestroy ()
  {
    _isEmpty();
    Katabatic::Session::_preDestroy();
  }


  Session* Session::open ( KiteEngine* kite )
  {
    ltrace(110) << "Kite::Session::open()" << endl;

    Session* session = Session::get();
    if (session) {
      if (session->_getKiteEngine() != kite)
        throw Error( reopenSession, getString(session->getKiteEngine()).c_str() );

      return session;
    }

    session = new Session ( kite );
    session->_postCreate();

    return session;
  }


  Session* Session::get ( const char* message )
  { return dynamic_cast<Session*>( Katabatic::Session::get(message) ); }


  Configuration* Session::getConfiguration ()
  { return Session::getKiteEngine()->getConfiguration(); }


  TrackElement* Session::lookup ( Segment* segment )
  { return Session::get("Session::lookup(Segment*)")->_getKiteEngine()->_lookup(segment); }


  TrackElement* Session::lookup ( AutoSegment* segment )
  { return Session::get("lookup(AutoSegment*)")->_getKiteEngine()->_lookup ( segment ); }


  void  Session::setInterrupt ( bool state )
  { Session::get("setInterrupt()")->_getKiteEngine()->setInterrupt(state); }


  KiteEngine* Session::_getKiteEngine ()
  { return static_cast<KiteEngine*>(_katabatic); }


  Net* Session::_getBlockageNet ()
  { return _getKiteEngine()->getBlockageNet(); };


  NegociateWindow* Session::_getNegociateWindow ()
  { return _getKiteEngine()->getNegociateWindow(); };


  unsigned int  Session::_getRipupCost ()
  { return _getKiteEngine()->getRipupCost(); };


  Katabatic::GCell* Session::_getGCellUnder ( DbU::Unit x, DbU::Unit y )
  { return _getKiteEngine()->getGCellGrid()->getGCell(Point(x,y)); };


  size_t  Session::_revalidate ()
  {
    ltrace(90) << "Kite::Session::_revalidate()" << endl;
    ltracein(90);

    set<Track*> packTracks;

    for ( size_t i=0 ; i<_removeEvents.size() ; ++i ) {
      if (not _removeEvents[i]._segment->getTrack()) continue;

      packTracks.insert( _removeEvents[i]._segment->getTrack() );
      _removeEvents[i]._segment->detach();
    }
    _removeEvents.clear();

    for ( set<Track*>::iterator it=packTracks.begin() ; it != packTracks.end() ; ++it )
      (*it)->doRemoval();

    for ( size_t i=0 ; i<_insertEvents.size() ; ++i ) {
      if (_insertEvents[i]._segment) {
        _insertEvents[i]._track->insert( _insertEvents[i]._segment );
      }
      if (_insertEvents[i]._marker) _insertEvents[i]._track->insert( _insertEvents[i]._marker );
    }
    _insertEvents.clear();

  // Check if to be destroyeds are not associateds with TrackSegments.
    const set<AutoSegment*>&          destroyeds = getDestroyeds();
    set<AutoSegment*>::const_iterator idestroyed = destroyeds.begin();
    for ( ; idestroyed != destroyeds.end() ; ++idestroyed ) {
      if (lookup(*idestroyed)) {
        ltraceout(90);
        throw Error( "Destroyed AutoSegment is associated with a TrackSegment\n"
                     "        (%s)"
                   , getString(*idestroyed).c_str());
      }
    }
    
    size_t count = Katabatic::Session::_revalidate();

    Interval                    span;
    const vector<AutoSegment*>& revalidateds     = getRevalidateds();
  //const set<Net*>&            netsModificateds = getNetsModificateds();

    for ( size_t i=0 ; i<revalidateds.size() ; ++i ) {
      if (not revalidateds[i]->isCanonical()) continue;
      
    //Net*          currentNet   = NULL;
      TrackElement* trackSegment = lookup( revalidateds[i] );

      if (trackSegment and trackSegment->isInvalidated()) {
        trackSegment->revalidate();
      }
    }
    _doglegReset();

# if defined(CHECK_DATABASE)
    unsigned int overlaps = 0;
# endif
    for ( set<Track*>::iterator it=_sortEvents.begin() ; it!=_sortEvents.end() ; ++it ) {
      (*it)->doReorder();
# if defined(CHECK_DATABASE)
      (*it)->check( overlaps, "Session::_revalidate() - track sorting." );
# endif
    }

# if defined(CHECK_DATABASE)
    for ( set<Track*>::iterator it=packTracks.begin() ; it != packTracks.end() ; ++it )
      (*it)->check( overlaps, "Session::_revalidate() - on packed track." );

  //_getKiteEngine()->_showOverlap ();
# endif

    _sortEvents.clear();

#if THIS_IS_DISABLED
    if (not faileds.empty()) {
      set<TrackElement*>::iterator ifailed = faileds.begin();
      Katabatic::GCellVector gcells;
      for ( ; ifailed != faileds.end() ; ++ifailed ) {
        (*ifailed)->getGCells ( gcells );
        (*ifailed)->makeDogLeg( gcells[0] );
      }

      count += _revalidate();
    }
#endif

    ltraceout(90);
    return count;
  }


  bool  Session::_isEmpty () const
  {
    if ( not _insertEvents.empty() or not _removeEvents.empty() or not _sortEvents.empty() ) {
      cerr << Bug( " Session::checkEmpty() failed :\n"
                   "        %u inserts, %u removes and %u sort events remains."
                 , _insertEvents.size()
                 , _removeEvents.size()
                 , _sortEvents  .size() ) << endl;
      return false;
    }

    return true;
  }


  void  Session::_addInsertEvent ( TrackMarker* marker, Track* track )
  {
    _insertEvents.push_back( Event(marker,track) );
    _addSortEvent( track, true );
  }


  void  Session::_addInsertEvent ( TrackElement* segment, Track* track )
  {
    ltrace(200) <<  "addInsertEvent() " << segment
                << "\n               @" << track << endl;

    if ( segment->getTrack() != NULL ) {
      cerr << Bug("Session::addInsertEvent(): Segment already in Track."
                 "\n      %s."
                 "\n      to %s."
                 ,getString(segment).c_str()
                 ,getString(track).c_str()
                 ) << endl;
      return;
    }

    _insertEvents.push_back( Event(segment,track) );
    _addSortEvent( track, true );
  }


  void  Session::_addRemoveEvent ( TrackElement* segment )
  {
    if (not segment->getTrack()) {
      cerr << Bug( " Kite::Session::addRemoveEvent() : %s is not in any Track."
                 , getString(segment).c_str() ) << endl;
      return;
    }

    ltrace(200) << "Ripup: @" << DbU::getValueString(segment->getAxis()) << " " << segment << endl;
    _removeEvents.push_back( Event(segment,segment->getTrack()) );
    _addSortEvent( segment->getTrack(), true );
  }


  void  Session::_addMoveEvent ( TrackElement* segment, Track* track )
  {
    if (not segment->getTrack()) {
      cerr << Bug( " Kite::Session::addMoveEvent() : %s has no target Track."
                 , getString(segment).c_str() ) << endl;
      return;
    }

    _addRemoveEvent( segment );
    _addInsertEvent( segment, track );
  }


  void  Session::_addSortEvent ( Track* track, bool forced )
  {
    if (not track ) {
      cerr << Bug( " Kite::Session::addSortEvent() : no Track to sort." ) << endl;
      return;
    }
    if (forced) track->invalidate();
    _sortEvents.insert( track );
  }


  string  Session::_getTypeName () const
  { return "Kite::Session"; }


  Record* Session::_getRecord () const
  {
    Record* record = Session::_getRecord ();
    record->add( getSlot( "_sortEvents"  , &_sortEvents ) );
                                     
    return record;
  }


} // Kite namespace.
