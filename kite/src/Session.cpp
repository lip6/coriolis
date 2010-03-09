
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
// |  C++ Module  :       "./Session.cpp"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "kite/Session.h"
#include  "kite/Track.h"
#include  "kite/TrackElement.h"
#include  "kite/GCellGrid.h"
#include  "kite/KiteEngine.h"


namespace {


  using namespace Kite;


  const char* reopenSession =
    "Kite::Session::open() :\n\n"
    "    Session already open for %s (internal error).";


} // End of local namespace.


namespace Kite {


  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::Error;
  using Hurricane::Bug;


// -------------------------------------------------------------------
// Class  :  "Session".


  Session::Session ( KiteEngine* kite )
    : Katabatic::Session(kite)
    , _order       (0)
    , _insertEvents()
    , _removeEvents()
    , _sortEvents  ()
  { }


  void  Session::_postCreate ()
  {
    Katabatic::Session::_postCreate ();
  }


  Session::~Session ()
  { }


  size_t  Session::_preDestroy ()
  {
    _isEmpty ();

    size_t count = Katabatic::Session::_preDestroy ();

    return count;
  }


  Session* Session::open ( KiteEngine* kite )
  {
    ltrace(110) << "Kite::Session::open()" << endl;

    Session* session = Session::get ();
    if ( session ) {
      if ( session->_getKiteEngine() != kite )
        throw Error ( reopenSession, getString(session->getKiteEngine()).c_str() );

      return session;
    }

    session = new Session ( kite );
    session->_postCreate ();

    return session;
  }


  Session* Session::get ( const char* message )
  { return dynamic_cast<Session*>( Katabatic::Session::get(message) ); }


  Configuration* Session::getConfiguration ()
  { return Session::getKiteEngine()->getConfiguration(); }


  void  Session::link ( TrackElement* trackSegment )
  { Session::get("link(TrackElement*)")->_getKiteEngine()->_link(trackSegment); }


  void  Session::unlink ( TrackElement* trackSegment )
  { Session::get("unlink(TrackElement*)")->_getKiteEngine()->_unlink(trackSegment); }


  TrackElement* Session::lookup ( Segment* segment )
  { return Session::get("Session::lookup(Segment*)")->_getKiteEngine()->_lookup(segment); }


  TrackElement* Session::lookup ( AutoSegment* segment )
  { return Session::get("lookup(AutoSegment*)")->_getKiteEngine()->_lookup ( segment ); }


  GCell* Session::lookup ( Katabatic::GCell* gcell )
  { return Session::get("lookup(Katabatic::GCell*)")->_getKiteEngine()->getGCellGrid()->getGCell(gcell->getIndex()); }


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


  GCell* Session::_getGCellUnder ( DbU::Unit x, DbU::Unit y )
  { return _getKiteEngine()->getGCellGrid()->getGCell(Point(x,y)); };


  unsigned int  Session::_getOrder () const
  { return _order; }


  void  Session::_setOrder ( unsigned int order )
  { _order = order; }


  size_t  Session::_revalidate ()
  {
    set<Track*> packTracks;

    for ( size_t i=0 ; i < _removeEvents.size() ; i++ ) {
      if ( !_removeEvents[i]._segment->getTrack() ) continue;

      packTracks.insert ( _removeEvents[i]._segment->getTrack() );
      _removeEvents[i]._segment->detach ();
    }
    _removeEvents.clear ();

    for ( set<Track*>::iterator it=packTracks.begin() ; it != packTracks.end() ; it++ )
      (*it)->pack ();

    for ( size_t i=0 ; i < _insertEvents.size() ; i++ ) {
      if ( _insertEvents[i]._segment ) {
        _insertEvents[i]._track->insert ( _insertEvents[i]._segment );
      }
      if ( _insertEvents[i]._marker  ) _insertEvents[i]._track->insert ( _insertEvents[i]._marker  );
    }
    _insertEvents.clear ();

  // Check if to be destroyeds are not associateds with TrackSegments.
    const set<AutoSegment*>&          destroyeds = getDestroyeds();
    set<AutoSegment*>::const_iterator idestroyed = destroyeds.begin();
    for ( ; idestroyed != destroyeds.end() ; idestroyed++ ) {
      if ( lookup(*idestroyed) ) {
        cerr << Error("Destroyed AutoSegment is associated with a TrackSegment\n"
                      "        (%s)"
                     ,getString(*idestroyed).c_str()) << endl;
      }
    }
    
    size_t count = Katabatic::Session::_revalidate ();

    Interval                    span;
    vector<TrackElement*>       processeds;
    const vector<AutoSegment*>& revalidateds     = getRevalidateds ();
    const set<Net*>&            netsModificateds = getNetsModificateds ();

    for ( size_t i=0 ; i<revalidateds.size() ; i++ ) {
      Net*          currentNet   = NULL;
      bool          invalidEvent = false;
      TrackElement* trackSegment = lookup ( revalidateds[i]->base() );

      if ( trackSegment && !trackSegment->isRevalidated() ) {
        if ( trackSegment->getNet() != currentNet ) {
          currentNet   = trackSegment->getNet();
          invalidEvent = (netsModificateds.find(currentNet) != netsModificateds.end());
        }
        trackSegment->revalidate ( invalidEvent );
        processeds.push_back ( trackSegment );

        Track* track = trackSegment->getTrack();
        if ( track ) {
          track->forceSort ();
          _sortEvents.insert ( track );
        }
      }
    }
    for ( size_t i=0 ; i<processeds.size() ; i++ )
      processeds[i]->setRevalidated ( false );

# if defined(CHECK_DATABASE)
    unsigned int overlaps = 0;
# endif
    for ( set<Track*>::iterator it=_sortEvents.begin()
            ; it != _sortEvents.end()
            ; it++
        ) {
      (*it)->sort ();
# if defined(CHECK_DATABASE)
      (*it)->_check ( overlaps, "Session::_revalidate() - track sorting." );
# endif
    }

    for ( set<Net*>::iterator inet=netsModificateds.begin() ; inet != netsModificateds.end() ; inet++ ) {
      _getKiteEngine()->_computeCagedConstraints ( *inet );
    }

# if defined(CHECK_DATABASE)
    for ( set<Track*>::iterator it=packTracks.begin() ; it != packTracks.end() ; it++ )
      (*it)->_check ( overlaps, "Session::_revalidate() - on packed track." );

  //_getKiteEngine()->_showOverlap ();
# endif

    _sortEvents.clear ();

    return count;
  }


  bool  Session::_isEmpty () const
  {
    if ( !_insertEvents.empty() || !_removeEvents.empty() || !_sortEvents.empty() ) {
      cerr << Bug(" Session::checkEmpty() failed :\n"
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
    _insertEvents.push_back ( Event(marker,track) );
    _addSortEvent ( track, true );
  }


  void  Session::_addInsertEvent ( TrackElement* segment, Track* track )
  {
#if defined(CHECK_DETERMINISM)
    cerr << "Order: Insert in @" << DbU::getValueString(track->getAxis()) 
         << " " << segment << endl;
#endif
    ltrace(200) << "addInsertEvent() " << segment << endl;

    if ( segment->getTrack() != NULL ) {
      cerr << Bug("Session::addInsertEvent(): Segment already in Track."
                 "\n      %s."
                 "\n      to %s."
                 ,getString(segment).c_str()
                 ,getString(track).c_str()
                 ) << endl;
      return;
    }

    _insertEvents.push_back ( Event(segment,track) );
    _addSortEvent ( track, true );
  }


  void  Session::_addRemoveEvent ( TrackElement* segment )
  {
    if ( not segment->getTrack() ) {
      cerr << Bug(" Kite::Session::addRemoveEvent() : %s is not in any Track."
                 ,getString(segment).c_str()) << endl;
      return;
    }

    ltrace(200) << "Ripup: @" << DbU::getValueString(segment->getAxis()) << " " << segment << endl;
    _removeEvents.push_back ( Event(segment,segment->getTrack()) );
    _addSortEvent ( segment->getTrack(), true );
  }


  void  Session::_addMoveEvent ( TrackElement* segment, Track* track )
  {
    if ( !segment->getTrack() ) {
      cerr << Bug(" Kite::Session::addMoveEvent() : %s has no target Track."
                 ,getString(segment).c_str()) << endl;
      return;
    }

    _addRemoveEvent ( segment );
    _addInsertEvent ( segment, track );
  }


  void  Session::_addSortEvent ( Track* track, bool forced )
  {
    if ( !track ) {
      cerr << Bug(" Kite::Session::addSortEvent() : no Track to sort.") << endl;
      return;
    }
    if ( forced ) track->forceSort ();
    _sortEvents.insert ( track );
  }


  string  Session::_getTypeName () const
  { return "Kite::Session"; }


  Record* Session::_getRecord () const
  {
    Record* record = Session::_getRecord ();
    record->add ( getSlot ( "_sortEvents"  , &_sortEvents   ) );
                                     
    return record;
  }


} // End of Kite namespace.
