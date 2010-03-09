
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
// |  C++ Header  :       "./Session.h"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KITE_SESSION__
#define  __KITE_SESSION__

#include  <set>
#include  <vector>
#include  <string>

namespace Hurricane {
  class Record;
  class Net;
  class Segment;
}

#include  "katabatic/Session.h"
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
  class GCell;
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
      inline static KiteEngine*         getKiteEngine       ();
             static Configuration*      getConfiguration    ();
      inline static Net*                getBlockageNet      ();
      inline static NegociateWindow*    getNegociateWindow  ();
      inline static unsigned int        getRipupCost        ();
      inline static GCell*              getGCellUnder       ( DbU::Unit, DbU::Unit );
      inline static unsigned int        getOrder            ();
      inline static void                setOrder            ( unsigned int );
      inline static void                addInsertEvent      ( TrackMarker*  , Track* );
      inline static void                addInsertEvent      ( TrackElement* , Track* );
      inline static void                addRemoveEvent      ( TrackElement* );
      inline static void                addMoveEvent        ( TrackElement* , Track* );
      inline static void                addSortEvent        ( Track*, bool forced=false );
      inline static size_t              revalidate          ();
      inline static bool                isEmpty             ();
             static void                setInterrupt        ( bool );
             static void                link                ( TrackElement* );
             static void                unlink              ( TrackElement* );
             static TrackElement*       lookup              ( Segment* );
             static TrackElement*       lookup              ( AutoSegment* );
             static GCell*              lookup              ( Katabatic::GCell* );
    private:                                                
                    KiteEngine*         _getKiteEngine      ();
                    Net*                _getBlockageNet     ();
                    NegociateWindow*    _getNegociateWindow ();
                    unsigned int        _getRipupCost       ();
                    GCell*              _getGCellUnder      ( DbU::Unit, DbU::Unit );
                    unsigned int        _getOrder           () const;
                    void                _setOrder           ( unsigned int );
                    GCell*              _lookup             ( Katabatic::GCell* );
                    void                _addInsertEvent     ( TrackMarker*  , Track* );
                    void                _addInsertEvent     ( TrackElement* , Track* );
                    void                _addRemoveEvent     ( TrackElement* );
                    void                _addMoveEvent       ( TrackElement* , Track* );
                    void                _addSortEvent       ( Track*, bool forced );
      virtual       size_t              _revalidate         ();
                    bool                _isEmpty            () const;
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
      unsigned int   _order;
      vector<Event>  _insertEvents;
      vector<Event>  _removeEvents;
      set<Track*>    _sortEvents;

    protected:
    // Constructors & Destructors.
                       Session     ( KiteEngine* );
      virtual         ~Session     ();
      virtual void     _postCreate ();
      virtual size_t   _preDestroy ();
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

  inline GCell* Session::getGCellUnder ( DbU::Unit x, DbU::Unit y )
  { return get("getGCellUnder()")->_getGCellUnder(x,y); }

  inline unsigned int  Session::getOrder ()
  { return get("getOrder()")->_getOrder(); }

  inline void  Session::setOrder ( unsigned int order )
  { get("setOrder()")->_setOrder(order); }

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


} // End of Kite namespace.


#endif  // __KITE_SESSION__
