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
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/NegociateWindow.h"                    |
// +-----------------------------------------------------------------+


#pragma  once
#include <set>
#include <queue>
#include <vector>

namespace Hurricane {
  class Cell;
}

#include "katana/RoutingEventQueue.h"
#include "katana/RoutingEventHistory.h"
#include "katana/RoutingEventLoop.h"


namespace Katana {

  class TrackElement;
  class KatanaEngine;


// -------------------------------------------------------------------
// Class  :  "Statistics".

  class Statistics {
    public:
      inline             Statistics       ();
      inline size_t      getGCellsCount   () const;
      inline size_t      getSegmentsCount () const;
      inline size_t      getEventsCount   () const;
      inline void        setGCellsCount   ( size_t );
      inline void        setSegmentsCount ( size_t );
      inline void        setEventsCount   ( size_t );
      inline void        incGCellCount    ( size_t );
      inline void        incSegmentsCount ( size_t );
      inline void        incEventsCount   ( size_t );
      inline Statistics& operator+=       ( const Statistics& );
    private:
      size_t  _gcellsCount;
      size_t  _segmentsCount;
      size_t  _eventsCount;

  };


  inline   Statistics::Statistics ()
    : _gcellsCount   (0)
    , _segmentsCount (0)
    , _eventsCount   (0)
  { }

  inline size_t  Statistics::getGCellsCount   () const { return _gcellsCount; }
  inline size_t  Statistics::getSegmentsCount () const { return _segmentsCount; }
  inline size_t  Statistics::getEventsCount   () const { return _eventsCount; }
  inline void    Statistics::setGCellsCount   ( size_t count ) { _gcellsCount = count; }
  inline void    Statistics::setSegmentsCount ( size_t count ) { _segmentsCount = count; }
  inline void    Statistics::setEventsCount   ( size_t count ) { _eventsCount = count; }
  inline void    Statistics::incGCellCount    ( size_t count ) { _gcellsCount += count; }
  inline void    Statistics::incSegmentsCount ( size_t count ) { _segmentsCount += count; }
  inline void    Statistics::incEventsCount   ( size_t count ) { _eventsCount += count; }

  inline Statistics& Statistics::operator+= ( const Statistics& other )
  {
    _gcellsCount   += other._gcellsCount;
    _segmentsCount += other._segmentsCount;
    _eventsCount   += other._eventsCount;
    return *this;
  }


// -------------------------------------------------------------------
// Class  :  "Katana::NegociateWindow".

  class NegociateWindow {
    public:
      static NegociateWindow*              create               ( KatanaEngine* );
             void                          destroy              ();
      inline bool                          isInterrupted        () const;
      inline KatanaEngine*                 getKatanaEngine      () const;
             Hurricane::Cell*              getCell              () const;
      inline const vector<GCell*>&         getGCells            () const;
      inline RoutingEventQueue&            getEventQueue        ();
      inline RoutingEventHistory&          getEventHistory      ();
      inline RoutingEventLoop&             getEventLoop         ();
             void                          setGCells            ( const vector<GCell*>& );
      inline void                          setInterrupt         ( bool );
             double                        computeWirelength    ();
             TrackElement*                 createTrackSegment   ( AutoSegment*, Flags flags );
             void                          addRoutingEvent      ( TrackElement*, uint32_t level );
      inline void                          rescheduleEvent      ( RoutingEvent*, uint32_t level );
             void                          run                  ( Flags flags );
             void                          printStatistics      () const;
             void                          _createRouting       ( Anabatic::GCell* );
             void                          _computePriorities   ();
             void                          _associateSymmetrics ();
             void                          _pack                ( size_t& count, bool last );
             size_t                        _negociate           ();
             void                          _negociateRepair     ();
             Hurricane::Record*            _getRecord           () const;
             std::string                   _getString           () const;
      inline std::string                   _getTypeName         () const;

    private:
    // Attributes.
      Flags                       _flags;
      bool                        _interrupt;
      KatanaEngine*               _katana;
      vector<GCell*>              _gcells;
      std::vector<TrackElement*>  _segments;
      RoutingEventQueue           _eventQueue;
      RoutingEventHistory         _eventHistory;
      RoutingEventLoop            _eventLoop;
      Statistics                  _statistics;

    // Constructors.
    protected:
                       NegociateWindow ( KatanaEngine* );
                      ~NegociateWindow ();
    private:
                       NegociateWindow ( const NegociateWindow& );
      NegociateWindow& operator=       ( const NegociateWindow& );
  };


// Inline Functions.
  inline bool                  NegociateWindow::isInterrupted   () const { return _interrupt; }
  inline KatanaEngine*         NegociateWindow::getKatanaEngine () const { return _katana; }
  inline const vector<GCell*>& NegociateWindow::getGCells       () const { return _gcells; }
  inline RoutingEventQueue&    NegociateWindow::getEventQueue   () { return _eventQueue; }
  inline RoutingEventHistory&  NegociateWindow::getEventHistory () { return _eventHistory; }
  inline void                  NegociateWindow::setInterrupt    ( bool state ) { _interrupt = state; }
  inline void                  NegociateWindow::rescheduleEvent ( RoutingEvent* event, uint32_t level ) { event->reschedule(_eventQueue,level); }
  inline std::string           NegociateWindow::_getTypeName    () const { return "NegociateWindow"; }


}  // Katana namespace.
