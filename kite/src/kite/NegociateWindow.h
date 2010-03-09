
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
// |  C++ Header  :       "./NegociateWindow.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KITE_NEGOCIATE_WINDOW__
#define  __KITE_NEGOCIATE_WINDOW__

#include <queue>
#include <vector>

namespace Hurricane {
  class Cell;
}

#include "katabatic/Grid.h"
#include "kite/RoutingEventQueue.h"
#include "kite/RoutingEventHistory.h"


namespace Kite {


  using std::vector;
  using std::queue;
  using Hurricane::Cell;
  using Katabatic::GridBox;

  class GCell;
  class GCellRoutingSet;
  class TrackElement;
  class KiteEngine;


// -------------------------------------------------------------------
// Class  :  "Kite::NegociateWindow".
 

  class NegociateWindow {

    public:
      enum Stage { Negociation = 1
                 , Packing
                 };
    public:
      static NegociateWindow*     create                ( KiteEngine*   kite
                                                        , unsigned int  columnMin
                                                        , unsigned int  rowMin
                                                        , unsigned int  columnMax
                                                        , unsigned int  rowMax
                                                        );
             void                 destroy               ();
      inline bool                 isInterrupted         () const;
             Cell*                getCell               () const;
      inline GridBox<GCell>*      getGridBox            () const;
      inline KiteEngine*          getKiteEngine         () const;
      inline RoutingEventQueue&   getEventQueue         ();
      inline RoutingEventHistory& getEventHistory       ();
      inline const vector<GCellRoutingSet*>&
                                  getGCellRoutingSets   () const;
      inline Stage                getStage              () const;
      inline void                 setInterrupt          ( bool );
      inline void                 setStage              ( Stage );
             void                 loadRouting           ();
             void                 addInsertEvent        ( TrackElement*, unsigned int level );
      inline void                 rescheduleEvent       ( RoutingEvent*, unsigned int level );
             void                 run                   ( int slowMotion=0 );
             void                 addToRing             ( TrackElement* );
             void                 printStatistics       () const;
             size_t               _negociate            ( const vector<TrackElement*>& );
             void                 _runOnGCellRoutingSet ( GCellRoutingSet* );
             void                 _loadRing             ();
             void                 _unloadRing           ();
             Record*              _getRecord            () const;
             string               _getString            () const;
      inline string               _getTypeName          () const;

    private:
      class RingSegment {
        public:
          static bool          orderReached ( const RingSegment& );
        public:
                               RingSegment  ( TrackElement* segment=NULL );
          inline TrackElement* getSegment   () const;
          inline unsigned int  getOrder     () const;
        private:
          TrackElement* _segment;
          unsigned int  _order;
      };

    private:
    // Attributes.
      unsigned int              _slowMotion;
      bool                      _interrupt;
      KiteEngine*               _kite;
      GridBox<GCell>*           _gridBox;
      vector<GCell*>            _criticalGCells;
      unsigned int              _gcellOrder;
      vector<GCellRoutingSet*>  _gcellRoutingSets;
      RoutingEventQueue         _eventQueue;
      RoutingEventHistory       _eventHistory;
      vector<RingSegment>       _ring;

    // Constructors.
    protected:
                       NegociateWindow ( KiteEngine*    kite
                                       , unsigned int   columnMin
                                       , unsigned int   rowMin
                                       , unsigned int   columnMax
                                       , unsigned int   rowMax
                                       );
                      ~NegociateWindow ();
    private:
                       NegociateWindow ( const NegociateWindow& );
      NegociateWindow& operator=       ( const NegociateWindow& );
  };


// Inline Functions.
  inline bool                    NegociateWindow::isInterrupted       () const { return _interrupt; }
  inline string                  NegociateWindow::_getTypeName        () const { return "NegociateWindow"; }
  inline GridBox<GCell>*         NegociateWindow::getGridBox          () const { return _gridBox; }
  inline KiteEngine*             NegociateWindow::getKiteEngine       () const { return _kite; }
  inline RoutingEventQueue&      NegociateWindow::getEventQueue       () { return _eventQueue; }
  inline RoutingEventHistory&    NegociateWindow::getEventHistory     () { return _eventHistory; }
  inline void                    NegociateWindow::setInterrupt        ( bool state ) { _interrupt = state; }
  inline void                    NegociateWindow::rescheduleEvent     ( RoutingEvent* event, unsigned int level ) { event->reschedule(_eventQueue,level); }
  inline const vector<GCellRoutingSet*>&
                                 NegociateWindow::getGCellRoutingSets () const { return _gcellRoutingSets; }

  inline TrackElement* NegociateWindow::RingSegment::getSegment  () const { return _segment; }
  inline unsigned int  NegociateWindow::RingSegment::getOrder    () const { return _order; }


} // End of Kite namespace.


#endif  // __KITE_NEGOCIATE_WINDOW__
