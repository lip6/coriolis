// -*- mode: C++; explicit-buffer-name: "Session.h<kite>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./kite/Session.h"                              |
// +-----------------------------------------------------------------+


#ifndef  KITE_SESSION_H
#define  KITE_SESSION_H

#include <set>
#include <vector>
#include <string>

namespace Hurricane {
  class Record;
  class Net;
  class Segment;
}

#include "katabatic/Session.h"
namespace Katabatic {
  class GCell;
  class AutoSegment;
}


namespace Kite {

  using std::set;
  using std::vector;
  using std::string;
  using Hurricane::Record;
  using Hurricane::Net;
  using Hurricane::Segment;
  using Hurricane::DbU;
  using Katabatic::AutoSegment;

  class Track;
  class TrackElement;
  class TrackMarker;
  class NegociateWindow;
  class Configuration;
  class KiteEngine;


// -------------------------------------------------------------------
// Class  :  "Session".

  class Session : public Katabatic::Session {

    public:
             static Session*            open                ( KiteEngine* );
             static Session*            get                 ( const char* message=NULL );
      inline static Katabatic::Session* base                ();
      inline static bool                isEmpty             ();
      inline static KiteEngine*         getKiteEngine       ();
             static Configuration*      getConfiguration    ();
      inline static Net*                getBlockageNet      ();
      inline static NegociateWindow*    getNegociateWindow  ();
      inline static unsigned int        getRipupCost        ();
      inline static Katabatic::GCell*   getGCellUnder       ( DbU::Unit, DbU::Unit );
             static void                setInterrupt        ( bool );
      inline static void                addInsertEvent      ( TrackMarker*  , Track* );
      inline static void                addInsertEvent      ( TrackElement* , Track* );
      inline static void                addRemoveEvent      ( TrackElement* );
      inline static void                addMoveEvent        ( TrackElement* , Track* );
      inline static void                addSortEvent        ( Track*, bool forced=false );
      inline static size_t              revalidate          ();
             static TrackElement*       lookup              ( Segment* );
             static TrackElement*       lookup              ( AutoSegment* );
    private:                                                
                    KiteEngine*         _getKiteEngine      ();
                    Net*                _getBlockageNet     ();
                    unsigned int        _getRipupCost       ();
                    Katabatic::GCell*   _getGCellUnder      ( DbU::Unit, DbU::Unit );
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
                       Session     ( KiteEngine* );
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


  inline Katabatic::Session* Session::base ()
  { return static_cast<Katabatic::Session*>(get("base()")); }

  inline KiteEngine* Session::getKiteEngine ()
  { return get("getKiteEngine()")->_getKiteEngine(); }

  inline Net* Session::getBlockageNet ()
  { return get("getBlockageNet()")->_getBlockageNet(); }

  inline NegociateWindow* Session::getNegociateWindow ()
  { return get("getNegociateWindow()")->_getNegociateWindow(); }

  inline unsigned int  Session::getRipupCost ()
  { return get("getRipupCost()")->_getRipupCost(); }

  inline Katabatic::GCell* Session::getGCellUnder ( DbU::Unit x, DbU::Unit y )
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


}  // Kite namespace.

#endif  // KITE_SESSION_H
