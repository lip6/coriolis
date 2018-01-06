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


#ifndef  KATANA_SESSION_H
#define  KATANA_SESSION_H

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
             static Session*            get                 ( const char* message=NULL );
      inline static Super*              base                ();
      inline static bool                isEmpty             ();
      inline static KatanaEngine*       getKatanaEngine     ();
             static Configuration*      getConfiguration    ();
      inline static Net*                getBlockageNet      ();
      inline static NegociateWindow*    getNegociateWindow  ();
      inline static uint32_t            getRipupCost        ();
      inline static Anabatic::GCell*    getGCellUnder       ( DbU::Unit, DbU::Unit );
             static void                setInterrupt        ( bool );
      inline static void                addInsertEvent      ( TrackMarker*  , Track* );
      inline static void                addInsertEvent      ( TrackElement* , Track* );
      inline static void                addRemoveEvent      ( TrackElement* );
      inline static void                addMoveEvent        ( TrackElement* , Track* );
      inline static void                addSortEvent        ( Track*, bool forced=false );
      inline static size_t              revalidate          ();
             static AutoContact*        lookup              ( Contact* );
             static TrackElement*       lookup              ( Segment* );
             static TrackElement*       lookup              ( AutoSegment* );
             static Session*            _open               ( KatanaEngine* );
    private:                                                
                    KatanaEngine*       _getKatanaEngine    ();
                    Net*                _getBlockageNet     ();
                    uint32_t            _getRipupCost       ();
                    Anabatic::GCell*    _getGCellUnder      ( DbU::Unit, DbU::Unit );
                    void                _doRemovalEvents    ();
      virtual       size_t              _revalidate         ();
                    bool                _isEmpty            () const;
                    NegociateWindow*    _getNegociateWindow ();
                    void                _addInsertEvent     ( TrackMarker*  , Track* );
                    void                _addInsertEvent     ( TrackElement* , Track* );
                    void                _addRemoveEvent     ( TrackElement* );
                    void                _addMoveEvent       ( TrackElement* , Track* );
                    void                _addSortEvent       ( Track*, bool forced );
      virtual       Record*             _getRecord          () const;
      virtual       string              _getTypeName        () const;
    protected:
    // Internal Classes.
      class Event {
        public:
          inline Event ( TrackElement*, Track* );
          inline Event ( TrackMarker* , Track* );
        public:
          TrackElement* _segment;
          TrackMarker*  _marker;
          Track*        _track;
      };
    protected:
    // Attributes.
      vector<Event>  _insertEvents;
      vector<Event>  _removeEvents;
      set<Track*>    _sortEvents;
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
  inline Session::Event::Event ( TrackElement* segment , Track* track )
    : _segment(segment)
    , _marker (NULL)
    , _track  (track)
  { }

  inline Session::Event::Event ( TrackMarker* marker , Track* track )
    : _segment(NULL)
    , _marker (marker)
    , _track  (track)
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

  inline void  Session::addInsertEvent ( TrackMarker* marker, Track* track )
  { get("addInsertEvent(TrackMarker*)")->_addInsertEvent(marker,track); }

  inline void  Session::addInsertEvent ( TrackElement* segment, Track* track )
  { get("addInsertEvent(TrackElement*)")->_addInsertEvent(segment,track); }

  inline void  Session::addRemoveEvent ( TrackElement* segment )
  { get("addRemoveEvent()")->_addRemoveEvent(segment); }

  inline void  Session::addMoveEvent ( TrackElement* segment, Track* track )
  { get("addMoveEvent()")->_addMoveEvent(segment,track); }

  inline void  Session::addSortEvent ( Track* track, bool forced )
  { get("addSortEvent()")->_addSortEvent(track,forced); }

  inline size_t  Session::revalidate ()
  { return get("revalidate()")->_revalidate(); }

  inline bool  Session::isEmpty ()
  { return get("isEmpty()")->_isEmpty(); }


}  // Katana namespace.

#endif  // KATANA_SESSION_H
