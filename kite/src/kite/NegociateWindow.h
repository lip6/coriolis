
// -*- C++ -*-
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
// |  C++ Header  :  "./kite/NegociateWindow.h"                      |
// +-----------------------------------------------------------------+


#ifndef  KITE_NEGOCIATE_WINDOW_H
#define  KITE_NEGOCIATE_WINDOW_H

#include <set>
#include <queue>
#include <vector>

namespace Hurricane {
  class Cell;
}

#include "katabatic/Grid.h"
#include "kite/RoutingEventQueue.h"
#include "kite/RoutingEventHistory.h"
#include "kite/RoutingEventLoop.h"


namespace Kite {

  class TrackElement;
  class KiteEngine;


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
// Class  :  "Kite::NegociateWindow".

  class NegociateWindow {

    public:
      enum Stage { Negociation = 1
                 , Packing     = 2
                 };
    public:
      static NegociateWindow*              create             ( KiteEngine* );
             void                          destroy            ();
      inline bool                          isInterrupted      () const;
      inline KiteEngine*                   getKiteEngine      () const;
             Hurricane::Cell*              getCell            () const;
      inline const Katabatic::GCellVector& getGCells          () const;
      inline RoutingEventQueue&            getEventQueue      ();
      inline RoutingEventHistory&          getEventHistory    ();
      inline RoutingEventLoop&             getEventLoop       ();
      inline Stage                         getStage           () const;
             void                          setGCells          ( const Katabatic::GCellVector& );
      inline void                          setInterrupt       ( bool );
      inline void                          setStage           ( Stage );
             double                        computeWirelength  ();
             TrackElement*                 createTrackSegment ( AutoSegment*, unsigned int flags );
             void                          addRoutingEvent    ( TrackElement*, unsigned int level );
      inline void                          rescheduleEvent    ( RoutingEvent*, unsigned int level );
             void                          run                ( int slowMotion=0 );
             void                          printStatistics    () const;
             void                          _createRouting     ( Katabatic::GCell* );
             size_t                        _negociate         ();
             Hurricane::Record*            _getRecord         () const;
             std::string                   _getString         () const;
      inline std::string                   _getTypeName       () const;

    private:
    // Attributes.
      unsigned int                _slowMotion;
      bool                        _interrupt;
      KiteEngine*                 _kite;
      Katabatic::GCellVector      _gcells;
      std::vector<TrackElement*>  _segments;
      RoutingEventQueue           _eventQueue;
      RoutingEventHistory         _eventHistory;
      RoutingEventLoop            _eventLoop;
      Statistics                  _statistics;

    // Constructors.
    protected:
                       NegociateWindow ( KiteEngine* );
                      ~NegociateWindow ();
    private:
                       NegociateWindow ( const NegociateWindow& );
      NegociateWindow& operator=       ( const NegociateWindow& );
  };


// Inline Functions.
  inline bool                          NegociateWindow::isInterrupted   () const { return _interrupt; }
  inline KiteEngine*                   NegociateWindow::getKiteEngine   () const { return _kite; }
  inline const Katabatic::GCellVector& NegociateWindow::getGCells       () const { return _gcells; }
  inline RoutingEventQueue&            NegociateWindow::getEventQueue   () { return _eventQueue; }
  inline RoutingEventHistory&          NegociateWindow::getEventHistory () { return _eventHistory; }
  inline void                          NegociateWindow::setInterrupt    ( bool state ) { _interrupt = state; }
  inline void                          NegociateWindow::rescheduleEvent ( RoutingEvent* event, unsigned int level ) { event->reschedule(_eventQueue,level); }
  inline std::string                   NegociateWindow::_getTypeName    () const { return "NegociateWindow"; }


}  // Kite namespace.


#endif  // KITE_NEGOCIATE_WINDOW_H
