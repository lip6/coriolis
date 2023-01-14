// -*- mode: C++; explicit-buffer-name: "Session.h<katana>" -*-
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
// |  C++ Header  :  "./katana/Session.h"                            |
// +-----------------------------------------------------------------+


#pragma  once
#include <set>
#include <vector>
#include <string>

namespace Hurricane {
  class Record;
  class Net;
  class Contact;
  class Segment;
}

#include "anabatic/Session.h"
namespace Anabatic {
  class GCell;
  class AutoContact;
  class AutoSegment;
}


namespace Katana {

  using std::set;
  using std::vector;
  using std::string;
  using Hurricane::Record;
  using Hurricane::Net;
  using Hurricane::Contact;
  using Hurricane::Segment;
  using Hurricane::DbU;
  using Hurricane::Interval;
  using Anabatic::AutoContact;
  using Anabatic::AutoSegment;

  class Track;
  class TrackElement;
  class TrackMarker;
  class NegociateWindow;
  class Configuration;
  class KatanaEngine;


// -------------------------------------------------------------------
// Class  :  "Session".

  class Session : public Anabatic::Session {
    public:
      typedef  Anabatic::Session  Super;
    public:
             static Session*            get                  ( const char* message=NULL );
      inline static Super*              base                 ();
      inline static bool                isEmpty              ();
      inline static bool                disableStackedVias   ();
             static uint32_t            getStage             ();
      inline static KatanaEngine*       getKatanaEngine      ();
             static Configuration*      getConfiguration     ();
      inline static Net*                getBlockageNet       ();
      inline static NegociateWindow*    getNegociateWindow   ();
      inline static uint32_t            getRipupCost         ();
      inline static Anabatic::GCell*    getGCellUnder        ( DbU::Unit, DbU::Unit );
      inline static const std::vector<TrackElement*>&        
                                        getIndirectInvalids  ();
             static void                setInterrupt         ( bool );
             static void                setStage             ( uint32_t );
      inline static Interval&           toAxisInterval       ( Interval&, size_t depth );
      inline static void                addIndirectInvalid   ( TrackElement* );
      inline static void                addInsertEvent       ( TrackMarker*  , Track* );
      inline static void                addInsertEvent       ( TrackElement* , Track*, DbU::Unit axis, bool check=true );
      inline static void                addRemoveEvent       ( TrackElement* );
      inline static void                addMoveEvent         ( TrackElement* , Track*, DbU::Unit axis );
      inline static void                addSortEvent         ( Track*, bool forced=false );
      inline static void                addLockEvent         ( TrackElement* );
      inline static size_t              revalidate           ();
             static AutoContact*        lookup               ( Contact* );
             static TrackElement*       lookup               ( Segment* );
             static TrackElement*       lookup               ( AutoSegment* );
             static void                addShortDogleg       ( TrackElement*, TrackElement* );
             static TrackElement*       getDoglegPaired      ( TrackElement* );
             static Session*            _open                ( KatanaEngine* );
    private:                                                 
                    KatanaEngine*       _getKatanaEngine     ();
                    Net*                _getBlockageNet      ();
                    uint32_t            _getRipupCost        ();
                    Anabatic::GCell*    _getGCellUnder       ( DbU::Unit, DbU::Unit );
                    Interval&           _toAxisInterval      ( Interval&, size_t depth ) const;
                    void                _doLockEvents        ();
                    void                _doRemovalEvents     ( bool reschedule=false );
      virtual       size_t              _revalidate          ();
                    bool                _isEmpty             () const;
                    bool                _disableStackedVias  ();
                    NegociateWindow*    _getNegociateWindow  ();
      inline        const std::vector<TrackElement*>&
                                        _getIndirectInvalids ();
      inline        void                _addIndirectInvalid  ( TrackElement* );
                    void                _addInsertEvent      ( TrackMarker*  , Track* );
                    void                _addInsertEvent      ( TrackElement* , Track*, DbU::Unit axis, bool check );
                    void                _addRemoveEvent      ( TrackElement* );
                    void                _addMoveEvent        ( TrackElement* , Track*, DbU::Unit axis );
                    void                _addSortEvent        ( Track*, bool forced );
                    void                _addLockEvent        ( TrackElement* );
      virtual       Record*             _getRecord           () const;
      virtual       string              _getTypeName         () const;
    protected:
    // Internal Classes.
      class Event {
        public:
          inline Event ( TrackElement*, Track*, DbU::Unit );
          inline Event ( TrackMarker* , Track* );
        public:
          TrackElement* _segment;
          TrackMarker*  _marker;
          Track*        _track;
          DbU::Unit     _axis;
      };
    protected:
    // Attributes.
      vector<TrackElement*>  _indirectInvalids;
      vector<Event>          _insertEvents;
      vector<Event>          _removeEvents;
      vector<Event>          _lockEvents;
      vector<Track*>         _sortEvents;
    protected:
    // Constructors & Destructors.
                       Session     ( KatanaEngine* );
      virtual         ~Session     ();
      virtual void     _postCreate ();
      virtual void     _preDestroy ();
    private:
                       Session     ( const Session& );
              Session& operator=   ( const Session& );
  };


// Inline Functions.
  inline Session::Event::Event ( TrackElement* segment , Track* track, DbU::Unit axis )
    : _segment(segment)
    , _marker (NULL)
    , _track  (track)
    , _axis   (axis)
  { }

  inline Session::Event::Event ( TrackMarker* marker , Track* track )
    : _segment(NULL)
    , _marker (marker)
    , _track  (track)
    , _axis   (0)
  { }


  inline Session::Super* Session::base ()
  { return static_cast<Super*>(get("base()")); }

  inline KatanaEngine* Session::getKatanaEngine ()
  { return get("getKatanaEngine()")->_getKatanaEngine(); }

  inline Net* Session::getBlockageNet ()
  { return get("getBlockageNet()")->_getBlockageNet(); }

  inline NegociateWindow* Session::getNegociateWindow ()
  { return get("getNegociateWindow()")->_getNegociateWindow(); }

  inline uint32_t  Session::getRipupCost ()
  { return get("getRipupCost()")->_getRipupCost(); }

  inline Anabatic::GCell* Session::getGCellUnder ( DbU::Unit x, DbU::Unit y )
  { return get("getGCellUnder()")->_getGCellUnder(x,y); }

  inline const std::vector<TrackElement*>& Session::getIndirectInvalids ()
  { return get("getIndirectInvalids()")->_getIndirectInvalids(); }

  inline Interval& Session::toAxisInterval ( Interval& interval, size_t depth )
  { return get("getGCellUnder()")->_toAxisInterval(interval,depth); }

  inline void  Session::addIndirectInvalid  ( TrackElement* element )
  { get("addIndirectInvalid(TrackElement*)")->_addIndirectInvalid(element); }

  inline void  Session::addInsertEvent ( TrackMarker* marker, Track* track )
  { get("addInsertEvent(TrackMarker*)")->_addInsertEvent(marker,track); }

  inline void  Session::addInsertEvent ( TrackElement* segment, Track* track, DbU::Unit axis, bool check )
  { get("addInsertEvent(TrackElement*)")->_addInsertEvent(segment,track,axis,check); }

  inline void  Session::addRemoveEvent ( TrackElement* segment )
  { get("addRemoveEvent()")->_addRemoveEvent(segment); }

  inline void  Session::addMoveEvent ( TrackElement* segment, Track* track, DbU::Unit axis )
  { get("addMoveEvent()")->_addMoveEvent(segment,track,axis); }

  inline void  Session::addLockEvent ( TrackElement* segment )
  { get("addLockEvent()")->_addLockEvent(segment); }

  inline void  Session::addSortEvent ( Track* track, bool forced )
  { get("addSortEvent()")->_addSortEvent(track,forced); }

  inline size_t  Session::revalidate ()
  { return get("revalidate()")->_revalidate(); }

  inline bool  Session::isEmpty ()
  { return get("isEmpty()")->_isEmpty(); }

  inline bool  Session::disableStackedVias ()
  { return get("disableStackedVias()")->_disableStackedVias(); }

  inline const std::vector<TrackElement*>& Session::_getIndirectInvalids ()
  { return _indirectInvalids; }

  inline void  Session::_addIndirectInvalid  ( TrackElement* element )
  { _indirectInvalids.push_back( element ); }


}  // Katana namespace.
