
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
// |  C++ Module  :       "./RoutingEvent.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <cstdlib>
#include  <iomanip>
#include  <algorithm>

#include  "hurricane/Bug.h"
#include  "hurricane/DebugSession.h"
#include  "hurricane/Net.h"
#include  "hurricane/Layer.h"

#include  "katabatic/AutoContact.h"

#include  "kite/DataNegociate.h"
#include  "kite/TrackElement.h"
#include  "kite/Track.h"
#include  "kite/Tracks.h"
#include  "kite/GCell.h"
#include  "kite/RoutingPlane.h"
#include  "kite/RoutingEvent.h"
#include  "kite/RoutingEventHistory.h"
#include  "kite/RoutingEventQueue.h"
#include  "kite/NegociateWindow.h"
#include  "kite/Session.h"
#include  "kite/KiteEngine.h"


namespace {

  /*! \defgroup     ClassManipulator  Manipulator Class
   */
 
  //! \addtogroup   ClassManipulator
  //! \{

 /*! \class        Manipulator
  *  \brief        Handle \TrackElement ripup &amp; topological modifications.
  *
  *                Handles all \TrackElement modifications.
  */

 /*! \function     Manipulator::Manipulator ( TrackElement* segment, State& state );
  *  \param        segment  The \TrackElement to manipulate.
  *  \param        state    The \RoutingEvent state.
  */

 /*! \function     TrackElement* Manipulator::getSegment () const;
  *  \Return       The working \TrackElement.
  */

 /*! \function     DataNegociate* Manipulator::getData () const;
  *  \Return       The DataNegociate of the \TrackElement.
  */

 /*! \function     RoutingEvent* Manipulator::getEvent () const;
  *  \Return       The \RoutingEvent associated to the \TrackElement. May be
  *                \NULL if it belongs to a more prioritary GCell RoutingSet.
  */

 /*! \function     bool  Manipulator::canRipup () const;
  *  \return       \true if the maximum ripup &amp; reset count has not been reached.
  */

 /*! \function     bool  Manipulator::ripup ( Interval overlap, unsigned int type, DbU::Unit axisHint );
  *  \param        overlap    The Interval that should be cleared.
  *  \param        type       The type of ripup action.
  *  \param        axisHint   An indication as where to move the riped up segment.
  *  \return       \True if the operation has succedeed.
  *
  *                If the \TrackElement cannot be riped up (it cames from a more
  *                prioritary GCell RoutingSet), then it tries to relax it.
  */

 /*! \function     bool  Manipulator::ripup ( DbU::Unit point, unsigned int type, DbU::Unit axisHint );
  *  \param        point      The point around which the \Track should be cleared.
  *  \param        type       The type of ripup action.
  *  \param        axisHint   An indication as where to move the riped up segment.
  *  \return       \True if the operation has succedeed.
  *
  *  \see          The other ripup method.
  */

 /*! \function     bool  Manipulator::relax ( Interval overlap );
  *  \param        overlap  The overlaping interval to free.
  *  \return       \true if the operation has succeeded.
  *
  *                Finds the appropriate dogleg GCells to break the \TrackElement.
  *                There could be one or two breaks whether the overlap is fully
  *                enclosed or not.
  */

 /*! \function     bool  Manipulator::modifyTopology ();
  *  \return       \true if the operation has succeeded.
  *
  *                Try to changes the \TrackElement topology.
  */

 /*! \function     bool  Manipulator::insertInTrack ( size_t i );
  *  \param        i  The index of \Track into which insert.
  *  \return       \true if the operation has succeeded.
  *
  *                Try to insert the \TrackElement into the \Track <i>i</i>.
  *                The index is relative to the State's \Track vector.
  */

 /*! \function     bool  Manipulator::desalignate ();
  *  \return       \true if the operation has succeeded.
  *
  *                Try to desalignate the \TrackElement.
  */

 /*! \function     bool  Manipulator::slacken ();
  *  \return       \true if the operation has succeeded.
  *
  *                Try to slacken the \TrackElement.
  */

 /*! \function     bool  Manipulator::moveUp ();
  *  \return       \true if the operation has succeeded.
  *
  *                Try to move up the \TrackElement.
  */

 /*! \function     bool  Manipulator::makeDogLeg ();
  *  \return       \true if the operation has succeeded.
  *
  *                Try to make a dog leg on the \TrackElement.
  *                This method is restricted to local \TrackElement only.
  */

 /*! \function     bool  Manipulator::minimize ();
  *  \return       \true if the operation has succeeded.
  *
  *                Try to mimize the wirelength of the \TrackElement.
  *                This operation can only be done once.
  */

 /*! \function     bool  Manipulator::relax ( size_t i );
  *  \param        i  The index of the selected \Track.
  *  \return       \true if the operation has succeeded.
  *
  *                Try to make a dog leg on the \TrackElement.
  *                This method is restricted to local \TrackElement only.
  */

  //! \}


  using namespace std;
  using namespace Hurricane;
  using namespace Kite;


// -------------------------------------------------------------------
// Class  :  "LvGCandidate".


  class LvGCandidate {
    public:
      struct Compare : public binary_function<LvGCandidate&,LvGCandidate&,bool> {
          inline bool  operator() ( const LvGCandidate& lhs, const LvGCandidate& rhs ) const;
      };
    public:
      inline                  LvGCandidate ( TrackElement* segment=NULL, Interval overlap=Interval(), size_t terminals=0 );
      inline  TrackElement*   getSegment   () const;
      inline  const Interval& getOverlap   () const;
      inline  size_t          getTerminals () const;
    private:
      TrackElement* _segment;
      Interval      _overlap;
      size_t        _terminals;
  };

  inline  LvGCandidate::LvGCandidate ( TrackElement* segment, Interval overlap, size_t terminals )
    : _segment  (segment)
    , _overlap  (overlap)
    , _terminals(terminals)
  { }

  inline  TrackElement*   LvGCandidate::getSegment   () const { return _segment; }
  inline  const Interval& LvGCandidate::getOverlap   () const { return _overlap; }
  inline  size_t          LvGCandidate::getTerminals () const { return _terminals; }

  inline bool  LvGCandidate::Compare::operator() ( const LvGCandidate& lhs, const LvGCandidate& rhs ) const
  {
    if ( lhs.getTerminals() != rhs.getTerminals() )
      return lhs.getTerminals() < rhs.getTerminals();

    if ( lhs.getOverlap() != rhs.getOverlap() )
      return lhs.getOverlap().getSize() > rhs.getOverlap().getSize();

    return lhs.getSegment()->getAxis() < rhs.getSegment()->getAxis();
  }



// -------------------------------------------------------------------
// Class  :  "Cs1Candidate".


  class Cs1Candidate {
    public:
      inline           Cs1Candidate ( Track* track=NULL );
      inline Track*    getTrack     () const;
      inline size_t    getBegin     () const;
      inline size_t    getEnd       () const;
      inline size_t    getLength    () const;
      inline Interval  getConflict  ( size_t );
      inline void      setBegin     ( size_t );
      inline void      setEnd       ( size_t );
      inline void      addConflict  ( const Interval& );
    public:
      friend inline bool  operator< ( const Cs1Candidate&, const Cs1Candidate& );
    private:
      Track*            _track;
      size_t            _begin;
      size_t            _end;
      vector<Interval>  _conflicts;
  };


  inline  Cs1Candidate::Cs1Candidate ( Track* track )
    : _track    (track)
    , _begin    (0)
    , _end      (0)
    , _conflicts()
  { }

  inline Track*  Cs1Candidate::getTrack     () const { return _track; }
  inline size_t  Cs1Candidate::getBegin     () const { return _begin; }
  inline size_t  Cs1Candidate::getEnd       () const { return _end; }
  inline size_t  Cs1Candidate::getLength    () const { return _conflicts.size(); }
  inline void    Cs1Candidate::setBegin     ( size_t i ) { _begin=i; }
  inline void    Cs1Candidate::setEnd       ( size_t i ) { _end=i; }
  inline void    Cs1Candidate::addConflict  ( const Interval& conflict ) { _conflicts.push_back(conflict); }

  inline Interval  Cs1Candidate::getConflict ( size_t i )
  {
    if (i >= _conflicts.size()) return Interval();
    return _conflicts[i];
  }

  inline bool  operator< ( const Cs1Candidate& lhs, const Cs1Candidate& rhs )
  { return lhs._conflicts.size() < rhs._conflicts.size(); }


// -------------------------------------------------------------------
// Class  :  "ShearAnalyze".


  class ShearAnalyze {
    public:
             ShearAnalyze     ( TrackElement* );
      void   addPerpandicular ( DbU::Unit axis, Interval constraint );
      GCell* getShearGCell    ();
    private:
      GCell* _compute         ();
    private:
      TrackElement*    _segment;
      vector<GCell*>   _gcells;
      vector<Interval> _constraints;
      GCell*           _shearGCell;
      bool             _computed;
  };


  ShearAnalyze::ShearAnalyze ( TrackElement* segment )
    : _segment    (segment)
    , _gcells     ()
    , _constraints()
    , _shearGCell (NULL)
    , _computed   (false)
  {
    segment->getGCells ( _gcells );
    for ( size_t igcell=0 ; igcell<_gcells.size() ; igcell++ ) {
      _constraints.push_back ( Interval() );
    }
  }


  GCell* ShearAnalyze::getShearGCell ()
  {
    if ( not _computed ) _compute ();
    return _shearGCell;
  }


  void  ShearAnalyze::addPerpandicular ( DbU::Unit axis, Interval constraint )
  {
    Interval uside;
    for ( size_t igcell=0 ; igcell<_gcells.size() ; igcell++ ) {
      uside = _gcells[igcell]->getUSide(_segment->getDirection(),true);
      if ( uside.contains(axis) ) {
        _constraints[igcell].merge ( constraint );
      }
    }
  }


  GCell* ShearAnalyze::_compute ()
  {
    _shearGCell = NULL;

    Interval trunk ( false );
    for ( size_t igcell=0 ; igcell<_gcells.size() ; igcell++ ) {
      if ( not _constraints[igcell].isEmpty() ) {
        trunk.intersection ( _constraints[igcell] );
      }
    }
  // Ugly: explicit pitch.
    trunk.inflate ( DbU::lambda(5.0) );

  // 0: intialisation, 1: left constrained, 2: right constrained.
    unsigned int  previousConstraint = 0;
    size_t        ishearBegin        = 0;
    size_t        ishearEnd          = 0;
    for ( size_t igcell=0 ; igcell<_gcells.size() ; igcell++ ) {
      if ( _constraints[igcell].isEmpty() ) continue;

      bool  leftConstrained = (_constraints[igcell].getVMin() >= trunk.getVMin());
      bool rightConstrained = (_constraints[igcell].getVMax() <= trunk.getVMax());

      if ( not leftConstrained and not rightConstrained ) continue;
      if (     leftConstrained and     rightConstrained ) {
        ishearBegin = ishearEnd = 0;
        break;
      }

      if ( not previousConstraint ) {
        previousConstraint = (leftConstrained) ? 1 : 2;
        continue;
      }

      if ( leftConstrained xor (previousConstraint == 1) ) {
        if ( not ishearBegin ) {
          ishearBegin = igcell;
          continue;
        }

        ishearBegin = ishearEnd = 0;
        break;
      }
    }

    if ( ishearBegin ) {
      if ( _constraints[ishearBegin-1].isEmpty() ) {
        _shearGCell = _gcells[ishearBegin-1];
      }
    }

    return _shearGCell;
  }


// -------------------------------------------------------------------
// Class  :  "UnionItervals".


  class UnionIntervals {
    public:
      inline                                 UnionIntervals ();
             void                            addInterval    ( Interval& );
      inline size_t                          size           () const;
      inline bool                            empty          () const;
      inline list<Interval>::const_iterator  begin          () const;
      inline list<Interval>::const_iterator  end            () const;
      inline DbU::Unit                       getVMin        () const;
      inline DbU::Unit                       getVMax        () const;
             string                          _getString     ();
    private:
      list<Interval>  _intervals;
  };


  inline                                 UnionIntervals::UnionIntervals () : _intervals() { }
  inline list<Interval>::const_iterator  UnionIntervals::begin          () const { return _intervals.begin(); }
  inline list<Interval>::const_iterator  UnionIntervals::end            () const { return _intervals.end(); }
  inline size_t                          UnionIntervals::size           () const { return _intervals.size(); }
  inline bool                            UnionIntervals::empty          () const { return _intervals.empty(); }
  inline DbU::Unit                       UnionIntervals::getVMin        () const { return (empty()) ? DbU::Max : (*begin()).getVMin(); }
  inline DbU::Unit                       UnionIntervals::getVMax        () const { return (empty()) ? DbU::Min : (*begin()).getVMax(); }


  void  UnionIntervals::addInterval ( Interval& interval )
  {
    ltrace(200) << "UnionInterval::addInterval() - " << interval << endl;

    list<Interval>::iterator iintv = _intervals.begin ();

    bool merged = false;
    while ( iintv != _intervals.end() ) {
      if ( !merged ) {
        if ( interval.getVMax() < (*iintv).getVMin() ) { _intervals.insert ( iintv, interval ); return; }
        if ( interval.getVMin() > (*iintv).getVMax() ) { iintv++; continue; }

        merged   = true;
        interval = (*iintv).merge ( interval );
        iintv++;
      } else {
        if ( (*iintv).intersect ( interval ) ) {
          interval = (*iintv).merge ( interval );
          iintv    = _intervals.erase ( iintv );
          continue;
        } else
          break;
      }
    }

    if ( !merged ) _intervals.push_back ( interval );
  }


  string  UnionIntervals::_getString ()
  {
    ostringstream s;

    list<Interval>::iterator iintv = _intervals.begin();
    for ( ; iintv != _intervals.end() ; iintv++ ) {
      s << " [" << DbU::getValueString((*iintv).getVMin())
        <<  ":" << DbU::getValueString((*iintv).getVMax()) << "]";
    }
    return s.str();
  }


// -------------------------------------------------------------------
// Class  :  "RipupHistory".


  class RipupHistory {
    public:
                             RipupHistory       ( RoutingEvent* );
      inline bool            isDislodger        ( RoutingEvent* ) const;
      inline size_t          size               () const;
      inline size_t          getDislodgersCount () const;
             void            addAxis            ( DbU::Unit );
             void            addAxis            ( RoutingEvent* );
             bool            hasAxis            ( DbU::Unit ) const;
             UnionIntervals* getUnionIntervals  ( DbU::Unit );
             void            addDislodger       ( RoutingEvent* );
             void            addDislodger       ( TrackElement* );
             void            print              ( ostream& );
    private:
      RoutingEvent*                 _masterEvent;
      map<DbU::Unit,UnionIntervals> _dislodgers;
      size_t                        _dislodgersCount;
  };


  RipupHistory::RipupHistory ( RoutingEvent* event )
    : _masterEvent(event)
    , _dislodgers()
    , _dislodgersCount(0)
  {
    const Interval& perpandicular = _masterEvent->getPerpandicular();
    RoutingPlane*   plane         = Session::getKiteEngine()->getRoutingPlaneByLayer(_masterEvent->getSegment()->getLayer());
    Track*          track;
    if ( !perpandicular.isEmpty() ) {
      track = plane->getTrackByPosition(perpandicular.getVMin());

      if ( track && (track->getAxis() < perpandicular.getVMin()) ) track = track->getNext();
      for ( ; track && (track->getAxis() <= perpandicular.getVMax())
            ; track = track->getNext() )
        addAxis ( track->getAxis() );
    }

    track = plane->getTrackByPosition(_masterEvent->getSegment()->getAxis());
    if ( track ) {
      size_t   begin;
      size_t   end;
      Interval interval = _masterEvent->getSegment()->getCanonicalInterval();
      track->getOverlapBounds ( interval, begin, end );

      if ( begin != Track::NPOS ) {
        for ( ; begin < end ; begin++ ) {
          TrackElement* other = track->getSegment(begin);
          if ( other->getNet() == _masterEvent->getSegment()->getNet() ) continue;
          if ( !interval.intersect(other->getCanonicalInterval()) ) continue;

          addDislodger ( other );
        }
      }
    }
  }


  inline bool    RipupHistory::isDislodger        ( RoutingEvent* event ) const { return hasAxis(event->getSegment()->getAxis()); }
  inline size_t  RipupHistory::size               () const { return _dislodgers.size(); }
  inline size_t  RipupHistory::getDislodgersCount () const { return _dislodgersCount; }


  void  RipupHistory::addAxis ( DbU::Unit axis )
  {
    if ( hasAxis(axis) ) return;
    _dislodgers.insert ( make_pair(axis,UnionIntervals()) );
  }


  void  RipupHistory::addAxis ( RoutingEvent* event )
  {
    addAxis ( event->getAxisHistory() );
  }


  bool  RipupHistory::hasAxis ( DbU::Unit axis ) const
  {
    return _dislodgers.find(axis) != _dislodgers.end();
  }


  UnionIntervals* RipupHistory::getUnionIntervals ( DbU::Unit axis )
  {
    map<DbU::Unit,UnionIntervals>::iterator iunion = _dislodgers.find ( axis );
    if ( iunion == _dislodgers.end() ) return NULL;

    return &(iunion->second);
  }


  void  RipupHistory::addDislodger ( RoutingEvent* event )
  {
    if ( event->getSegment() == _masterEvent->getSegment() ) return;
    if ( event->getSegment()->getLayer() != _masterEvent->getSegment()->getLayer() ) return;

    UnionIntervals* intervals = getUnionIntervals ( event->getAxisHistory() );
    if ( !intervals ) return;

    Interval canonical = event->getSegment()->getCanonicalInterval();
    intervals->addInterval ( canonical );

    _dislodgersCount++;
  }


  void  RipupHistory::addDislodger ( TrackElement* segment )
  {
    if ( _masterEvent->getSegment()->getNet() == segment->getNet() ) return;

    UnionIntervals* intervals = getUnionIntervals ( segment->getAxis() );
    if ( !intervals ) return;

    Interval canonical = segment->getCanonicalInterval();
    intervals->addInterval ( canonical );

    _dislodgersCount++;
  }


  void  RipupHistory::print ( ostream& o )
  {
    o << "[HISTORY] " << _masterEvent << endl;

    map<DbU::Unit,UnionIntervals>::iterator iunion = _dislodgers.begin();
    for ( ; iunion != _dislodgers.end() ; iunion++ )
      o << "  @" << DbU::getValueString(iunion->first)
        << " " << (iunion->second)._getString() << endl;
  }


// -------------------------------------------------------------------
// Class  :  "::SegmentAction".


  class SegmentAction {
    public:
      enum Type { Self                 = (1<< 0)
                , Other                = (1<< 1)
                , Ripup                = (1<< 2)
                , Insert               = (1<< 3)
                , ResetRipup           = (1<< 4)
                , AxisHint             = (1<< 5)
                , Perpandicular        = (1<< 6)
                , EventLevel1          = (1<< 7)
                , EventLevel2          = (1<< 8)
                , EventLevel3          = (1<< 9)
                , EventLevel4          = (1<<10)
                , EventLevel5          = (1<<11)
                , PackingMode          = (1<<12)
                , ToState              = (1<<13)
                , ToRipupLimit         = (1<<14)
                , SelfInsert           = Self |Insert
                , SelfRipup            = Self |Ripup
                , SelfRipupPerpand     = Self |Ripup | Perpandicular
                , SelfRipupAndReset    = Self |Ripup | Perpandicular
                , SelfShrinkPerpand    = Self |Ripup | Perpandicular|EventLevel4
                , SelfRipupAndAxisHint = Self |Ripup | Perpandicular|EventLevel4| AxisHint
                , OtherRipup           = Other|Ripup
                , OtherPushAside       = Other|Ripup | Perpandicular|EventLevel3
                , OtherPacking         = Other|Ripup | Perpandicular|PackingMode
                };
    public:
                           SegmentAction ( TrackElement*
                                         , unsigned int  type
                                         , DbU::Unit     axisHint=0
                                         , unsigned int  toState =0
                                         );
      inline TrackElement* getSegment    () const;
      inline unsigned int  getType       () const;
      inline void          setAxisHint   ( DbU::Unit );
      inline unsigned int  setFlag       ( unsigned int );
             bool          doAction      ( RoutingEventQueue& );
    private:
      TrackElement* _segment;
      unsigned int  _type;
      DbU::Unit     _axisHint;
      unsigned int  _toState;
  };


  inline TrackElement* SegmentAction::getSegment    () const { return _segment; }
  inline unsigned int  SegmentAction::getType       () const { return _type; }
  inline void          SegmentAction::setAxisHint   ( DbU::Unit axis ) { _axisHint = axis; }
  inline unsigned int  SegmentAction::setFlag       ( unsigned int flag ) { _type |= flag; return _type; }


  SegmentAction::SegmentAction ( TrackElement* segment
                               , unsigned int  type
                               , DbU::Unit     axisHint
                               , unsigned int  toState
                               )
    : _segment (segment)
    , _type    (type)
    , _axisHint(axisHint)
    , _toState (toState)
  { }


  bool  SegmentAction::doAction ( RoutingEventQueue& queue )
  {
  // Note:
  //   "_immediate" ripup flags was associated with "perpandicular", as they
  //   must be re-inserted *before* any parallel. Must look to solve the redundancy.

    if ( _type & Perpandicular ) {
      ltrace(200) << "* Riping Pp " << _segment << endl;
    } else {
      ltrace(200) << "* Riping // " << _segment << endl;
    }

    if ( _segment->isFixed() ) return true;

    DataNegociate* data = _segment->getDataNegociate();
    if ( data == NULL ) return true;

    if ( _type & ResetRipup ) data->resetRipupCount ();

    if ( _type & ToState ) {
      data->setState      ( _toState );
      data->setRipupCount ( Session::getKiteEngine()->getRipupLimit(_segment) );
    }

    if ( data->getGCellOrder() == Session::getOrder() ) {
    // Current order, default behavior: ripup.
      if ( _segment->getTrack() )
        Session::addRemoveEvent ( _segment );
    } else if ( data->getGCellOrder() < Session::getOrder() ) {
    // Order is lower: no longer able to displace it.
      return false;
    } else {
    // Order is greater, do not route now. Ripup if needed.
      if ( _segment->getTrack() )
        Session::addRemoveEvent ( _segment );
      return true;
    }

    RoutingEvent* event = data->getRoutingEvent();
    if ( event == NULL ) {
      if ( data->getGCellOrder() == Session::getOrder() ) {
        cerr << Bug("Missing Event on %p:%s"
                   ,_segment->base()->base(),getString(_segment).c_str()) << endl;
      }
      ltrace(200) << "Not in current GCellRoutingSet, cancelled." << endl;
      return true;
    }

    if ( (_type & AxisHint) /*and not _segment->isSlackenDogLeg()*/ ) {
      ltrace(200) << "Setting Axis Hint: @" << DbU::getValueString(_axisHint) << endl;
      event->setAxisHint ( _axisHint );
    }

    if ( _type & ToRipupLimit ) {
      unsigned int limit = Session::getKiteEngine()->getRipupLimit(_segment);
      if ( limit > data->getRipupCount() )
        data->setRipupCount ( limit );
    }

    unsigned int eventLevel = 0;
    if ( _type & EventLevel1 ) eventLevel = 1;
    if ( _type & EventLevel2 ) eventLevel = 2;
    if ( _type & EventLevel3 ) eventLevel = 3;
    if ( _type & EventLevel4 ) eventLevel = 4;
    if ( _type & EventLevel5 ) eventLevel = 5;
    RoutingEvent* fork = event->reschedule ( queue, eventLevel );

    if ( fork )
      fork->setMode ( (_type&PackingMode) ? RoutingEvent::Pack : RoutingEvent::Negociate );

    return true;
  }


// -------------------------------------------------------------------
// Class Declaration  :  "::State".


  class State {

    public:
      enum StateValues  { MissingData              = (1<<0)
                        , EmptyTrackList           = (1<<1)
                        , Inserted                 = (1<<2)
                        , Self                     = (1<<3)
                        , Other                    = (1<<4)
                        , Ripup                    = (1<<5)
                        , MaximumRipup             = (1<<6)
                        , TopologyModificated      = (1<<7)
                        , SelfInserted             = Self  | Inserted
                        , SelfMaximumRipup         = Self  | MaximumRipup
                        , OtherRipup               = Other | Ripup
                        , OtherTopologyModificated = Other | TopologyModificated
                        , SelfTopologyModificated  = Self  | TopologyModificated
                        };
      enum SlackenFlags { NoRecursive  = (1<<0)
                        , NoTransition = (1<<1)
                        };

    public:
                                    State                       ( RoutingEvent*
                                                                , RoutingEventQueue&
                                                                , RoutingEventHistory&
                                                                );
      inline RoutingEvent*          getEvent                    ();
      inline RoutingEventQueue&     getQueue                    ();
      inline RoutingEventHistory&   getHistory                  ();
      inline unsigned int           getState                    ();
      inline DataNegociate*         getData                     ();
      inline Interval&              getConstraint               ();
      inline Interval&              getOptimal                  ();
      inline vector<TrackCost>&     getCosts                    ();
      inline TrackCost&             getCost                     ( size_t );
      inline Track*                 getTrack                    ( size_t );
      inline size_t                 getBegin                    ( size_t );
      inline size_t                 getEnd                      ( size_t );
      inline void                   setState                    ( unsigned int );
      inline void                   addAction                   ( TrackElement*
                                                                , unsigned int type
                                                                , DbU::Unit    axisHint=0
                                                                , unsigned int toState =0
                                                                );
      inline vector<SegmentAction>& getActions                  ();
             bool                   doActions                   ();
      inline void                   clearActions                ();
             bool                   insertInTrack               ( size_t );
             bool                   conflictSolve1              ();
             bool                   conflictSolve2              ();
             bool                   localVsGlobal               ();
             bool                   slackenTopology             ( TrackElement*, unsigned int flags=0 );

    private:
      RoutingEvent*          _event;
      RoutingEventQueue&     _queue;
      RoutingEventHistory&   _history;
      unsigned int           _state;
      DataNegociate*         _data;
      Interval               _constraint;
      Interval               _optimal;
      vector<TrackCost>      _costs;
      vector<SegmentAction>  _actions;
  };


// -------------------------------------------------------------------
// Class Declaration  :  "::Manipulator".


  class Manipulator {
    public:
      enum { ToRipupLimit=1, AllowExpand  =2, NoExpand=4, PerpandicularsFirst=8, ToMoveUp=16 };
      enum { LeftAxisHint=1, RightAxisHint=2 };
      enum { NoRingLimit=1 };
    public:
                                  Manipulator             ( TrackElement*, State& );
                                 ~Manipulator             ();
      inline TrackElement*        getSegment              () const;
      inline DataNegociate*       getData                 () const;
      inline RoutingEvent*        getEvent                () const;
             bool                 canRipup                ( unsigned int flags=0 ) const;
             bool                 isCaged                 ( DbU::Unit ) const;
             bool                 ripup                   ( Interval     overlap
                                                          , unsigned int type
                                                          , DbU::Unit    axisHint=0
                                                          , unsigned int toState =0
                                                          );
             bool                 ripup                   ( DbU::Unit    point
                                                          , unsigned int type
                                                          , DbU::Unit    axisHint=0
                                                          );
             bool                 ripupPerpandiculars     ( unsigned int flags=0 );
             bool                 ripple                  ();
             bool                 goOutsideGCell          ();
             bool                 minimize                ();
             bool                 desalignate             ();
             bool                 slacken                 ();
             bool                 pivotUp                 ();
             bool                 moveUp                  ();
             bool                 makeDogLeg              ();
             bool                 makeDogLeg              ( DbU::Unit );
             bool                 makeDogLeg              ( Interval );
             bool                 relax                   ( Interval, unsigned int flags=AllowExpand );
             bool                 relax                   ( size_t );
             bool                 insertInTrack           ( size_t );
             bool                 shrinkToTrack           ( size_t
                                                          , unsigned int flags=0
                                                          , DbU::Unit    leftAxisHint=0
                                                          , DbU::Unit    rightAxisHint=0
                                                          );
             bool                 forceToTrack            ( size_t );
             void                 reprocessPerpandiculars ();
    private:
      TrackElement*  _segment;
      DataNegociate* _data;
      RoutingEvent*  _event;
      State&         _S;
  };


// -------------------------------------------------------------------
// Class Definition  :  "::State".
  

  State::State ( RoutingEvent* event, RoutingEventQueue& queue, RoutingEventHistory& history )
    : _event              (event)
    , _queue              (queue)
    , _history            (history)
    , _state              (0)
    , _data               (NULL)
    , _constraint         ()
    , _optimal            ()
    , _costs              ()
    , _actions            ()
  {
    TrackElement* segment = _event->getSegment();
    _event->setTracksFree ( 0 );

    _data = segment->getDataNegociate();
    if ( !_data ) {
      _state = MissingData;
      return;
    }

#if defined(CHECK_DETERMINISM)
    cerr << "Order: " << _data->getGCellOrder() << " - " << _data->getCost() << endl;
#endif

    _data->update ();
    _event->invalidate ( true ); // Optimizable.
    _event->revalidate ( true );

    _constraint = _event->getConstraints();
    _optimal    = _event->getOptimal();

    const Interval& perpandicular = _event->getPerpandicular ();

    ltrace(148) << "Katabatic intervals:"                  << endl;
    ltrace(148) << "* Optimal:        "  << _optimal       << endl;
    ltrace(148) << "* Constraints:    "  << _constraint    << endl;
    ltrace(148) << "* Perpandicular:  "  << perpandicular  << endl;
    ltrace(148) << "* OptimalAxis:    "  << _event->getOptimalAxis() << endl;
    ltrace(148) << "* AxisHint:       "  << DbU::getValueString(_event->getAxisHint()) << endl;

    if ( _event->getTracksNb() ) {
      if ( _constraint.getIntersection(perpandicular).isEmpty() ) {
        ltrace(200) << "Perpandicular free is too tight." << endl;
        _state = EmptyTrackList;
      } else
        _constraint.intersection ( perpandicular );
    } else {
      ltrace(200) << "No Track in perpandicular free." << endl;
      _state = EmptyTrackList;
    }

    if ( _state == EmptyTrackList ) return;

    ltrace(148) << "Negociate intervals:" << endl;
    ltrace(148) << "* Optimal:     "      << _optimal    << endl;
    ltrace(148) << "* Constraints: "      << _constraint << endl;
    ltracein(148);

    bool inLocalDepth = (Session::getRoutingGauge()->getLayerDepth(segment->getLayer()) < 3);

    RoutingPlane* plane = Session::getKiteEngine()->getRoutingPlaneByLayer(segment->getLayer());
    forEach ( Track*, itrack, Tracks_Range::get(plane,_constraint)) {
      _costs.push_back ( itrack->getOverlapCost(segment) );
      _costs.back().setAxisWeight   ( _event->getAxisWeight(itrack->getAxis()) );
      _costs.back().incDeltaPerpand ( _data->getCost().getWiringDelta(itrack->getAxis()) );

      if ( inLocalDepth and (_costs.back().getDataState() == DataNegociate::MaximumSlack) )
        _costs.back().setInfinite ();

      _costs.back().consolidate ();
      ltrace(149) << "| " << (*itrack) << " - " << _costs.back() << endl;
    }
    ltraceout(148);

    if ( _data->isBorder() and (_costs.size() == 1) and _costs[0].isInfinite() ) {
      Interval span;
      if ( _data->isLeftBorder() ) {
        span = Interval ( segment->getCanonicalInterval().getVMin()
                        , segment->getCanonicalInterval().getVMin()+DbU::lambda(5.0) );
      } else {
        span = Interval ( segment->getCanonicalInterval().getVMax()-DbU::lambda(5.0)
                        , segment->getCanonicalInterval().getVMax() );
      }

      Track* track = _costs[0].getTrack();
      _costs[0] = track->getOverlapCost ( span, segment->getNet() );
      _costs[0].setAxisWeight   ( _event->getAxisWeight(track->getAxis()) );
      _costs[0].incDeltaPerpand ( _data->getCost().getWiringDelta(track->getAxis()) );

      if ( segment->isGlobal() and (_costs[0].getDataState() == DataNegociate::MaximumSlack) )
        _costs[0].setInfinite ();

      _costs[0].consolidate ();
      ltrace(200) << "Replace: " << _costs[0] << endl;
    }

    if ( _costs.empty() ) {
      Track* nearest = plane->getTrackByPosition(_constraint.getCenter());

      if (   (nearest->getAxis() < _constraint.getVMin())
          || (nearest->getAxis() > _constraint.getVMax()) ) {
      //setUnimplemented ();
      //cerr << "[UNIMPLEMENTED] " << segment << " no Track in constraint interval "
      //     << _constraint << " " <<  "." << endl;
      } else {
        cerr << Bug(" %s Track_Range() failed to find Tracks in %s (they exists)."
                   ,getString(segment).c_str()
                   ,getString(_constraint).c_str()
                   ) << endl;
      }
      _state = EmptyTrackList;
    }

    sort ( _costs.begin(), _costs.end() );
    size_t i=0;
    for ( ; (i<_costs.size()) and _costs[i].isFree() ; i++ );
    _event->setTracksFree ( i );
  }


  inline RoutingEvent*          State::getEvent               () { return _event; }
  inline RoutingEventQueue&     State::getQueue               () { return _queue; }
  inline RoutingEventHistory&   State::getHistory             () { return _history; }
  inline unsigned int           State::getState               () { return _state; }
  inline DataNegociate*         State::getData                () { return _data; }
  inline Interval&              State::getConstraint          () { return _constraint; }
  inline Interval&              State::getOptimal             () { return _optimal; }
  inline vector<TrackCost>&     State::getCosts               () { return _costs; }
  inline TrackCost&             State::getCost                ( size_t i ) { return _costs[i]; }
  inline Track*                 State::getTrack               ( size_t i ) { return _costs[i].getTrack(); }
  inline size_t                 State::getBegin               ( size_t i ) { return _costs[i].getBegin(); }
  inline size_t                 State::getEnd                 ( size_t i ) { return _costs[i].getEnd(); }
  inline vector<SegmentAction>& State::getActions             () { return _actions; }
  inline void                   State::setState               ( unsigned int state ) { _state = state; }
  inline void                   State::clearActions           () { _actions.clear(); }


  void  State::addAction ( TrackElement* segment
                         , unsigned int  type
                         , DbU::Unit     axisHint
                         , unsigned int  toState  )
  {
    if ( not segment->isFixed() ) {
      _actions.push_back ( SegmentAction(segment,type,axisHint,toState) );
      ltrace(200) << "State::addAction(): " << segment << endl;
    }
  }


  bool  State::doActions ()
  {
    ltrace(200) << "State::doActions() - " << _actions.size() << endl;

    bool ripupOthersParallel = false;
    for ( size_t i=0 ; i<_actions.size() ; i++ ) {
      if ( _actions[i].getType() & SegmentAction::OtherRipup ) {
        ripupOthersParallel = true;
      }
    }

    for ( size_t i=0 ; i<_actions.size() ; i++ ) {
      if ( (_actions[i].getType() & SegmentAction::SelfInsert) and ripupOthersParallel )
        _actions[i].setFlag ( SegmentAction::EventLevel3 );

      if ( not _actions[i].doAction(_queue) ) {
        cinfo << "[INFO] Failed action on " << _actions[i].getSegment() << endl;
      }
    }

    _actions.clear ();
    return true;
  }


  bool  State::insertInTrack ( size_t i )
  {
    ltrace(200) << "State::insertInTrack() istate:" << _event->getInsertState()
                << " track:" << i << endl;

    _event->incInsertState();
    switch ( _event->getInsertState() ) {
      case 1:
        if ( Manipulator(_event->getSegment(),*this).insertInTrack(i) ) return true;
        _event->incInsertState();
      case 2:
        if ( Manipulator(_event->getSegment(),*this).shrinkToTrack(i) ) return true;
        _event->incInsertState();
      case 3:
        if ( Manipulator(_event->getSegment(),*this).forceToTrack(i) ) return true;
        _event->incInsertState();
    }
    return false;
  }


  bool  State::conflictSolve2 ()
  {
    bool          success = false;
    RipupHistory  ripupHistory ( _event );
    RoutingEvent* event;
    TrackElement* segment = _event->getSegment();

    ltrace(200) << "State::conflictSolve2()" << endl;

    size_t maxDepth   = min ( getHistory().size(), (size_t)300 );
    size_t depth      = 0;
    while ( (ripupHistory.getDislodgersCount() < 3) and (depth < maxDepth) ) {
      event = getHistory().getRNth(depth++);
      if ( !event ) continue;
      if ( (event->getSegment() != segment) and ripupHistory.isDislodger(event) )
        ripupHistory.addDislodger ( event );
    }

  //ripupHistory.print ( cout );

    UnionIntervals* intervals = ripupHistory.getUnionIntervals ( segment->getAxis() );
    if ( intervals && !intervals->empty() ) {

      DbU::Unit minConflict  = intervals->getVMin();
      DbU::Unit maxConflict  = intervals->getVMax();
      Interval  canonical    = segment->getCanonicalInterval();
      bool      sourceDogLeg = canonical.contains(minConflict);
      bool      targetDogLeg = canonical.contains(maxConflict);

      if ( sourceDogLeg || targetDogLeg ) {
        Point breakPoint;
        if ( segment->isHorizontal() )
          breakPoint = Point ( (sourceDogLeg)?minConflict:maxConflict ,segment->getAxis() );
        else
          breakPoint = Point ( segment->getAxis(), (sourceDogLeg)?minConflict:maxConflict );

        GCell* dogLegGCell = Session::getGCellUnder ( breakPoint.getX(), breakPoint.getY() );
        if ( dogLegGCell ) {
          if ( segment->canDogLegAt(dogLegGCell) ) {
            if ( segment->makeDogLeg(dogLegGCell) )
              success = true;
          }
        } else {
          cerr << Bug("No GCell under %s.",getString(breakPoint).c_str()) << endl;
        }

        if ( !success ) {
          if ( segment->isHorizontal() )
            breakPoint = Point ( (targetDogLeg)?maxConflict:minConflict ,segment->getAxis() );
          else
            breakPoint = Point ( segment->getAxis(), (targetDogLeg)?maxConflict:minConflict );

          GCell* dogLegGCell = Session::getGCellUnder ( breakPoint.getX(), breakPoint.getY() );
          if ( dogLegGCell ) {
            if ( segment->canDogLegAt(dogLegGCell) ) {
              if ( segment->makeDogLeg(dogLegGCell) )
                success = true;
            }
          } else {
            cerr << Bug("No GCell under %s.",getString(breakPoint).c_str()) << endl;
          }
        }
      }
    } else {
      ltrace(200) << "No disloggers found @" << DbU::getValueString(segment->getAxis()) << endl;

      Interval freeSpan = Session::getKiteEngine()->
        getTrackByPosition(segment->getLayer(),segment->getAxis())->
        getFreeInterval(segment->getSourceU(),segment->getNet());

      if ( freeSpan.contains(segment->getCanonicalInterval()) ) {
        ltrace(200) << "Disloggers vanished, Segment can be re-inserted." << endl;
        success = true;
      }
    }

    return success;
  }


  bool  State::conflictSolve1 ()
  {
    bool                  success    = false;
    Interval              constraints;
    vector<Cs1Candidate>  candidates;
    TrackElement*         segment    = _event->getSegment();
    bool                  canMoveUp  = (segment->isLocal()) ? segment->canPivotUp(0.5) : segment->canMoveUp(0.5);
    unsigned int          relaxFlags
      = (_data and (_data->getStateCount() < 2)) ? Manipulator::AllowExpand : Manipulator::NoExpand;

    ltrace(200) << "State::conflictSolve1()" << endl;
    ltrace(200) << "| Candidates Tracks: " << endl;

    segment->base()->getConstraints ( constraints );
    Interval      overlap    = segment->getCanonicalInterval();
    RoutingPlane* plane      = Session::getKiteEngine()->getRoutingPlaneByLayer(segment->getLayer());
    Track*        track      = plane->getTrackByPosition(constraints.getVMin(),Constant::Superior);

    for ( ; track->getAxis() <= constraints.getVMax() ; track = track->getNext() ) {
      ltrace(200) << "* " << track << endl;
      candidates.push_back ( Cs1Candidate(track) );

      size_t        begin;
      size_t        end;
      TrackElement* other;
      Net*          otherNet   = NULL;
      Interval      otherOverlap;

      track->getOverlapBounds ( overlap, begin, end );
      candidates.back().setBegin ( begin );
      candidates.back().setEnd   ( end );

      for ( ; (begin < end) ; begin++ ) {
        other = track->getSegment(begin);

        if (  other->getNet() == segment->getNet() ) continue;
        if ( !other->getCanonicalInterval().intersect(overlap) ) continue;
        ltrace(200) << "  | Conflict: " << begin << " " << other << endl;
        
        if ( otherNet != other->getNet() ) {
          if ( otherNet != NULL ) {
            candidates.back().addConflict ( otherOverlap );
          }
          otherNet     = other->getNet();
          otherOverlap = other->getCanonicalInterval();
        } else {
          otherOverlap.merge(other->getCanonicalInterval());
        }
      }
      if ( not otherOverlap.isEmpty() )
        candidates.back().addConflict ( otherOverlap );
    }

    sort ( candidates.begin(), candidates.end() );

    for ( size_t icandidate=0 ; icandidate<candidates.size() ; icandidate++ ) {
      ltrace(200) << "Trying l:" << candidates[icandidate].getLength()
                  << " " << candidates[icandidate].getTrack() << endl;

      if ( candidates[icandidate].getLength() > 2 ) break;

      Interval overlap0 = candidates[icandidate].getConflict(0);
      if ( candidates[icandidate].getLength() == 1 ) {
        Track*        track = candidates[icandidate].getTrack();
        TrackElement* other = track->getSegment(candidates[icandidate].getBegin());

        if (    other->isGlobal()
           and (other->getDataNegociate()->getGCellOrder() == Session::getOrder())
           and  other->canMoveUp(0.5) ) {
          ltrace(200) << "conflictSolve1() - One conflict, other move up" << endl;
          if ( (success = other->moveUp()) ) break;
        }

        ltrace(200) << "conflictSolve1() - One conflict, relaxing self" << endl;

        if ( Manipulator(segment,*this).relax(overlap0,relaxFlags) ) {
          success = true;
          break;
        } else {
          if ( not canMoveUp
             and (relaxFlags != Manipulator::NoExpand)
             and Manipulator(segment,*this).relax(overlap0,Manipulator::NoExpand) ) {
            ltrace(200) << "Cannot move up but successful narrow breaking." << endl;
            success = true;
            break;
          }
        }
      }

      if ( candidates[icandidate].getLength() == 2 ) {
        ltrace(200) << "conflictSolve1() - Two conflict, relaxing self" << endl;

        Interval overlap1 = candidates[icandidate].getConflict(1);
      // Ugly: hard-coded half GCell side.
        if ( overlap1.getVMin() - overlap0.getVMax() < DbU::lambda(5.0) ) {
          ltrace(200) << "conflictSolve1() - Too narrow spacing: "
                      << DbU::getValueString(overlap1.getVMin() - overlap0.getVMax()) << endl;
          continue;
        }

        if ( Manipulator(segment,*this).relax(overlap0,relaxFlags) ) {
          success = true;
          break;
        } else {
          if ( not canMoveUp
             and (relaxFlags != Manipulator::NoExpand)
             and Manipulator(segment,*this).relax(overlap0,Manipulator::NoExpand) ) {
            ltrace(200) << "Cannot move up but successful narrow breaking." << endl;
            success = true;
            break;
          }
        }
      }
    }

  //if ( track && (track->getAxis() < constraints.getVMin()) ) track = track->getNext();
  //for ( ; !success && track && (track->getAxis() <= constraints.getVMax()) ; track = track->getNext() ) 

    if ( not success and (segment->getLength() >= Session::getConfiguration()->getGlobalMinBreak()) ) {
      ltrace(200) << "Long global wire, break in the middle." << endl;
      Interval span;
      segment->getCanonical ( span );
      
      success = Manipulator(segment,*this).makeDogLeg ( span.getCenter() );
    }

    if ( not success and segment->isGlobal() and (_costs.size() <= 1) ) {
      ltrace(200) << "Overconstrained perpandiculars, rip them up. On track:" << endl;
      ltrace(200) << "  " << track << endl;
      Manipulator(segment,*this).ripupPerpandiculars ();
      success = true;
    }

    return success;
  }


  bool  State::localVsGlobal ()
  {
    ltrace(200) << "State::localVsGlobal()" << endl;
    ltracein(200);

    bool                  success     = false;
    Interval              constraint  = _event->getConstraints();
    TrackElement*         segment     = _event->getSegment();
    Interval              overlap     = segment->getCanonicalInterval();
    vector<LvGCandidate>  candidates;
  //Interval              uside       = segment->getGCell()->getUSide(segment->getDirection(),true);
    Interval              uside       = overlap;

  // Ugly: Hard-wired pitch usage.
    uside.inflate ( DbU::lambda(10.0) );
    ltrace(200) << "uside: " << uside << endl;

    RoutingPlane* plane = Session::getKiteEngine()->getRoutingPlaneByLayer(segment->getLayer());
    forEach ( Track*, itrack, Tracks_Range::get(plane,constraint)) {
      ltrace(200) << "| " << (*itrack) << " - " << endl;

      size_t        begin;
      size_t        end;
      TrackElement* candidate = NULL;
      Net*          otherNet  = NULL;
      Interval      otherOverlap;
      unsigned int  weight    = 0;
      size_t        terminals = 0;

      itrack->getOverlapBounds ( overlap, begin, end );
      for ( ; (begin < end) ; begin++ ) {
        TrackElement* other = itrack->getSegment(begin);

        if ( other->getNet() == segment->getNet() ) continue;
        if ( not other->getCanonicalInterval().intersect(overlap) ) continue;
        if ( not other->isGlobal() ) { candidate = NULL; break; }
        ltrace(200) << "| Conflict: " << begin << " " << other << endl;
        
        if ( otherNet == NULL ) {
          otherNet     = other->getNet();
          otherOverlap = other->getCanonicalInterval();
          candidate    = other;

          itrack->getTerminalWeight ( uside, otherNet, terminals, weight );

          // if (   (other->base()->getAutoSource()->getAnchor() != NULL)
          //    and (overlap.contains(otherOverlap.getVMin())) )
          //   ++terminals;

          // if (   (other->base()->getAutoTarget()->getAnchor() != NULL)
          //    and (overlap.contains(otherOverlap.getVMax())) )
          //   ++terminals;

          ltrace(200) << "> Candidate " << candidate << endl;
        } else {
          if ( other->getNet() == otherNet ) {
            Interval otherCanonical = other->getCanonicalInterval();
            if ( otherOverlap.intersect(otherCanonical) ) {
              otherOverlap.merge(otherCanonical);

              // if (   (other->base()->getAutoSource()->getAnchor() != NULL)
              //    and (overlap.contains(otherCanonical.getVMin())) )
              //   ++terminals;

              // if (   (other->base()->getAutoTarget()->getAnchor() != NULL)
              //    and (overlap.contains(otherCanonical.getVMax())) )
              //   ++terminals;

              continue;
            }
          } else {
            ltrace(200) << "> Reject candidate " << candidate << endl;
            candidate = NULL;
            break;
          }
        }
      }

      if ( candidate ) {
        ltrace(200) << "> Accept candidate " << candidate << endl;
        candidates.push_back ( LvGCandidate(candidate,otherOverlap,terminals) );
      }
    }

    if ( candidates.empty() ) {
      ltraceout(200);
      return false;
    }

    sort ( candidates.begin(), candidates.end(), LvGCandidate::Compare() );
    for ( size_t i=0 ; i<candidates.size() ; ++i ) {
      ltrace(200) << i << "| " << DbU::getValueString(candidates[i].getOverlap().getSize())
                  << "+" << candidates[i].getTerminals()
                  << " " << candidates[i].getSegment() << endl;
    }

    for ( size_t i=0 ; (not success) and (i<candidates.size()) ; ++i ) {
      if ( Manipulator(candidates[i].getSegment(),*this).ripup
         (overlap,SegmentAction::OtherRipup/*|SegmentAction::ToState
                                             ,DataNegociate::MoveUp*/) ) {
        ltrace(200) << "localVsGlobal() - Successful relaxing of Other Global" << endl;
        success = true;
      }
    }

    ltraceout(200);

    return success;
  }


  bool  State::slackenTopology ( TrackElement* segment, unsigned int flags )
  {
    DebugSession::open ( segment->getNet(), 200 );

    bool           success     = false;
    bool           blocked     = false;
    bool           repush      = true;
    DataNegociate* data        = segment->getDataNegociate ();
    unsigned int   nextState   = data->getState();
    unsigned int   actionFlags = SegmentAction::SelfInsert|SegmentAction::EventLevel5;

    ltrace(200) << "Slacken Topology for " << segment->getNet()
                << " " << segment << endl;
    ltracein(200);

    if ( (not segment) or (not data) ) { ltraceout(200); DebugSession::close(); return false; }
    if ( segment == _event->getSegment() ) _event->resetInsertState();

    if ( not (data->isBorder() or data->isRing()) ) {
      data->resetRipupCount ();
    } else {
      if ( not Manipulator(segment,*this).canRipup(Manipulator::NoRingLimit) ) {
        cerr << "[UNSOLVED] " << segment << " slacken topology not allowed for border/ring." << endl;
        ltraceout(200);
        DebugSession::close ();
        return false;
      } 
    }

  // Ring cases.
    if ( data->isBorder() ) {
      ltrace(200) << "Segment is Ripup only (border), bypass to Unimplemented." << endl;
      if ( not Manipulator(segment,*this).canRipup (Manipulator::NoRingLimit) ) {
        ltrace(200) << "Cannot ripup, bypass to Unimplemented." << endl;
        data->setState ( DataNegociate::Unimplemented );
      } else {
        ltrace(200) << "Refusing to slacken border segment." << endl;
        ltraceout(200);
        DebugSession::close ();
        return false;
      }
    }

    if ( data->isRing() ) {
      ltrace(200) << "Segment is Ripup only (ring), back-propagate to perpandiculars." << endl;
    // Do not change the state.
    //data->setState ( DataNegociate::Unimplemented );
      success = Manipulator(segment,*this).ripupPerpandiculars(Manipulator::ToRipupLimit
                                                              |Manipulator::PerpandicularsFirst
                                                              |Manipulator::ToMoveUp);
      repush = false;
      if ( not success ) {
        data->setState ( DataNegociate::Unimplemented );
      }
    }

  // Normal cases.
    if ( not blocked and not success ) {
      if ( segment->isStrap() ) {
        ltrace(200) << "Strap segment FSM." << endl;
        switch ( data->getState() ) {
          case DataNegociate::RipupPerpandiculars:
            nextState = DataNegociate::Desalignate;
            success = Manipulator(segment,*this).ripupPerpandiculars();
            if ( success ) break;
          case DataNegociate::Desalignate:
          case DataNegociate::Minimize:
            nextState = DataNegociate::LocalVsGlobal;
            success = Manipulator(segment,*this).minimize();
            if ( success ) break;
          case DataNegociate::DogLeg:
          case DataNegociate::Slacken:
          case DataNegociate::ConflictSolve1:
          case DataNegociate::ConflictSolve2:
          case DataNegociate::MoveUp:
          case DataNegociate::LocalVsGlobal:
            if ( not (flags & NoRecursive) ) {
              if ( (success = localVsGlobal()) ) {
                nextState = DataNegociate::LocalVsGlobal;
                if ( data->getStateCount() >= 1 )
                  nextState = DataNegociate::MaximumSlack;
                break;
              }
            }
          case DataNegociate::MaximumSlack:
          case DataNegociate::Unimplemented:
            nextState = DataNegociate::Unimplemented;
            break;
        }

        if ( not success and (nextState != DataNegociate::Unimplemented) ) {
          success = Manipulator(segment,*this).ripupPerpandiculars(Manipulator::ToRipupLimit);
        }
      } else if ( segment->isLocal() ) {
      // Local TrackElement State Machine.
        ltrace(200) << "Local segment FSM." << endl;
        switch ( data->getState() ) {
          case DataNegociate::RipupPerpandiculars:
            nextState = DataNegociate::Desalignate;
            success = Manipulator(segment,*this).ripupPerpandiculars();
            if ( success ) break;
          case DataNegociate::Desalignate:
            nextState = DataNegociate::Minimize;
            success = Manipulator(segment,*this).desalignate();
            break;
          case DataNegociate::Minimize:
            nextState = DataNegociate::DogLeg;
            success = Manipulator(segment,*this).minimize();
            if ( success ) break;
          case DataNegociate::DogLeg:
            nextState = DataNegociate::Slacken;
            success = Manipulator(segment,*this).makeDogLeg();
            if ( success ) break;
          case DataNegociate::Slacken:
            nextState = DataNegociate::ConflictSolve2;
            success = Manipulator(segment,*this).slacken();
            if ( success ) break;
          case DataNegociate::ConflictSolve1:
          case DataNegociate::ConflictSolve2:
            if ( not (flags & NoRecursive) ) {
              nextState = DataNegociate::MoveUp;
              success = conflictSolve2 ();
              break;
            }
          case DataNegociate::MoveUp:
            nextState = DataNegociate::LocalVsGlobal;
            success = Manipulator(segment,*this).moveUp();
            if ( success ) break;
          case DataNegociate::LocalVsGlobal:
            if ( not (flags & NoRecursive) ) {
              if ( (success = localVsGlobal()) ) {
                nextState = DataNegociate::LocalVsGlobal;
                if ( data->getStateCount() >= 1 )
                  nextState = DataNegociate::MaximumSlack;
                break;
              }
            }
          case DataNegociate::MaximumSlack:
            if ( segment->isSlackenStrap() ) { 
              if ( (nextState < DataNegociate::MaximumSlack) or (data->getStateCount() < 2) ) {
                nextState = DataNegociate::MaximumSlack;
                success = conflictSolve1 ();
                if ( success ) break;
              }
            }
          case DataNegociate::Unimplemented:
            nextState = DataNegociate::Unimplemented;
            break;
        }

        if ( not success and (nextState != DataNegociate::Unimplemented) ) {
          if ( data->getStateCount() < 6 )
            success = Manipulator(segment,*this).ripupPerpandiculars(Manipulator::ToRipupLimit);
        }

      // Special case: all tracks are overlaping a blockage.
        if ( not success
           and (nextState == DataNegociate::Unimplemented)
           and segment->isSlackened() ) {
          blocked = (getCosts().size() > 0);
          for ( size_t itrack=0 ; itrack<getCosts().size() ; ++itrack ) {
            if ( (not getCost(itrack).isBlockage() and not getCost(itrack).isFixed()) ) {
              blocked = false;
              break;
            }
          }

          if ( blocked ) {
          //Interval nativeConstraints;
          //_event->getSegment()->base()->getConstraints ( nativeConstraints );
      
          //if ( nativeConstraints.getSize() < DbU::lambda(5.0) ) {
          //  blocked = true;
            success = Manipulator(segment,*this).pivotUp();
            if ( not success ) {
              cerr << "[ERROR] Tighly constrained segment overlapping a blockage." << endl;
              ltrace(200) << "Segment is hard blocked, bypass to Unimplemented." << endl;
              nextState = DataNegociate::Unimplemented;
            }
          //}
          }
        }

        // LOOPING.
        // if ( not success
        //    and (nextState == DataNegociate::Unimplemented)
        //    and segment->isSlackened() ) {
        //   ltrace(200) << "Last chance on slackened" << endl;
        //   segment->base()->setSlackened ( false );
        //   success = Manipulator(segment,*this).makeDogLeg();
        //   nextState = DataNegociate::MaximumSlack;
        // }
      } else {
      // Global TrackElement State Machine.
        switch ( data->getState() ) {
          ltrace(200) << "Global segment FSM." << endl;
          case DataNegociate::RipupPerpandiculars:
            ltrace(200) << "Global, State: RipupPerpandiculars." << endl;
            nextState = DataNegociate::Desalignate;
            break;
          case DataNegociate::Minimize:
          case DataNegociate::DogLeg:
          case DataNegociate::Desalignate:
            ltrace(200) << "Global, State: Minimize, DogLeg or Desalignate." << endl;
            if ( (success = Manipulator(segment,*this).desalignate()) ) {
              nextState = DataNegociate::Slacken;
              break;
            }
          case DataNegociate::Slacken:
            ltrace(200) << "Global, State: Slacken."  << endl;
            if ( (success = Manipulator(segment,*this).slacken()) ) {
              nextState = DataNegociate::ConflictSolve1;
              break;
            }
          case DataNegociate::ConflictSolve1:
          case DataNegociate::ConflictSolve2:
            ltrace(200) << "Global, State: ConflictSolve1 or ConflictSolve2." << endl;
            if ( not (flags & NoRecursive) ) {
              if ( (success = conflictSolve1 ()) ) {
                nextState = DataNegociate::ConflictSolve1;
                if ( data->getStateCount() > 3 )
                  nextState = DataNegociate::MoveUp;
                break;
              }
            }
          case DataNegociate::MoveUp:
            ltrace(200) << "Global, State: MoveUp." << endl;
            if ( (success = Manipulator(segment,*this).moveUp()) ) {
              nextState = DataNegociate::ConflictSolve1;
              break;
            }
            nextState = DataNegociate::MaximumSlack;
            break;
          case DataNegociate::MaximumSlack:
          case DataNegociate::Unimplemented:
            ltrace(200) << "Global, State: MaximumSlack or Unimplemented." << endl;
            nextState = DataNegociate::Unimplemented;
            break;
        }

        if ( not success and (nextState != DataNegociate::Unimplemented) ) {
          if ( data->getStateCount() < 6 )
            success = Manipulator(segment,*this).ripupPerpandiculars(Manipulator::ToRipupLimit);
        }
      }
    }

    if ( not (flags&NoTransition) ) data->setState ( nextState );

    if ( success ) {
      if ( repush ) {
        if ( not (data->isRing() or data->isBorder()) )
          actionFlags |= SegmentAction::ResetRipup;
      
        addAction ( segment, actionFlags );
      }
    } else {
      clearActions ();
      if ( data->getState() == DataNegociate::Unimplemented ) {
        cerr << "[UNSOLVED] " << segment << " unable to slacken topology." << endl;
      }
    }

    ltraceout(200);
    DebugSession::close ();

    return success;
  }


// -------------------------------------------------------------------
// Class Definition  :  "::Manipulator".


  inline TrackElement*  Manipulator::getSegment () const { return _segment; }
  inline DataNegociate* Manipulator::getData    () const { return _data; }
  inline RoutingEvent*  Manipulator::getEvent   () const { return _event; }


  Manipulator::Manipulator ( TrackElement* segment, State& S )
    : _segment(segment)
    , _data   (NULL)
    , _event  (NULL)
    , _S      (S)
  {
    if ( !_segment )
      throw Error("Manipulator::Manipulator(): cannot build upon a NULL TrackElement.");

    DebugSession::open ( _segment->getNet(), 200 );

    _data = _segment->getDataNegociate();
    if ( _data )
      _event = _data->getRoutingEvent();
  }


  Manipulator::~Manipulator ()
  {
    DebugSession::close ();
  }


  bool  Manipulator::canRipup ( unsigned int flags ) const
  {
    if ( _data ) {
      if ( not _event or _event->isUnimplemented() ) return false;

      unsigned int limit = Session::getKiteEngine()->getRipupLimit(_segment);

      if (   not ( flags & NoRingLimit )
         and (_data->isRing() or _data->isBorder())
         and not (_data->getRipupCount() < limit) )
        return false;

      return (_data->getRipupCount() < limit);
    }

    return false;
  }


  bool  Manipulator::isCaged ( DbU::Unit axis ) const
  {
    Track* track = _segment->getTrack();
    if ( not track ) return false;

    TrackElement* neighbor = _segment->getPrevious();
    if ( neighbor and (neighbor->isFixed() or neighbor->isBlockage()) ) {
      if ( abs(axis - neighbor->getTargetU()) < DbU::lambda(10.0) )
        return true;
    }

    neighbor = _segment->getNext();
    if ( neighbor and (neighbor->isFixed() or neighbor->isBlockage()) ) {
      if ( abs(axis - neighbor->getSourceU()) < DbU::lambda(10.0) )
        return true;
    }

    return false;
  }


  bool  Manipulator::ripup ( DbU::Unit point , unsigned int type, DbU::Unit axisHint )
  {
    ltrace(200) << "Manipulator::ripup(DbU::Unit) " << DbU::getValueString(point) << endl;

    Interval overlap = Interval(point);
    overlap.inflate ( Session::getExtensionCap() ); // Ugly.

    return ripup ( overlap, type, axisHint );
  }


  bool  Manipulator::ripup ( Interval overlap , unsigned int type, DbU::Unit axisHint, unsigned int toState )
  {
    ltrace(200) << "Manipulator::ripup(Interval) " << overlap << endl;
    ltrace(200) << "TrackElement:" << _data->getGCellOrder()
                << " < Session:" << Session::getOrder() << endl;

    if ( not canRipup() ) return false;

    if ( _segment->isFixed() ) return false;
    if ( _data == NULL ) return true;

    if ( _segment->getTrack() ) {
      if ( _data->getGCellOrder() < Session::getOrder() ) {
        return relax(overlap);
      }
    }

    _S.addAction ( _segment, type, axisHint, toState );
    return true;
  }


  bool  Manipulator::ripupPerpandiculars ( unsigned int flags )
  {
    ltrace(200) << "Manipulator::ripupPerpandiculars() - " << flags << endl;

    bool          success                  = true;
    bool          cagedPerpandiculars      = false;
    Interval      constraints              ( _event->getConstraints() );
    Interval      perpandicularConstraints ( constraints );
    size_t        placedPerpandiculars     = 0;
    unsigned int  parallelActionFlags      = SegmentAction::SelfRipup|SegmentAction::EventLevel4;
    unsigned int  perpandicularActionFlags = SegmentAction::SelfRipupPerpand;

    if ( flags & Manipulator::PerpandicularsFirst ) {
      parallelActionFlags      &= ~SegmentAction::EventLevel4;
      perpandicularActionFlags |=  SegmentAction::EventLevel4;
      if ( flags & Manipulator::ToRipupLimit )
        perpandicularActionFlags |=  SegmentAction::ToRipupLimit;
    } else {
      if ( flags & Manipulator::ToRipupLimit )
        parallelActionFlags |=  SegmentAction::ToRipupLimit;
    }

    ltrace(200) << "Pure constraints: " << constraints << endl;

    Track* track = NULL;
    const vector<TrackElement*>& perpandiculars = _event->getPerpandiculars();

    for ( size_t i=0 ; i < perpandiculars.size() ; i++ ) {
      track = perpandiculars[i]->getTrack();
      if ( not track ) continue;

      if ( Manipulator(perpandiculars[i],_S).isCaged(_event->getSegment()->getAxis()) ) {
        cagedPerpandiculars = true;
        break;
      }

      placedPerpandiculars++;

    // Try to ripup the perpandicular itself.
      DataNegociate* data2 = perpandiculars[i]->getDataNegociate();
      if ( data2->getGCellOrder() == Session::getOrder() ) {
        ltrace(200) << "| " << perpandiculars[i] << endl;

        if (   (flags & Manipulator::ToMoveUp)
           and (data2->getState() < DataNegociate::MoveUp) )
          data2->setState ( DataNegociate::MoveUp );
        
        if ( Manipulator(perpandiculars[i],_S).ripup(_event->getSegment()->getAxis()
                                                    ,perpandicularActionFlags) )
          continue;
      }

    // Cannot ripup the perpandicular, try to ripup it's neigbors.
      size_t begin;
      size_t end;
      track->getOverlapBounds ( constraints, begin, end );

      for ( ; (begin < end) ; begin++ ) {
        TrackElement* other = track->getSegment(begin);

        if (  other->getNet() == _event->getSegment()->getNet() ) continue;

        Interval otherCanonical ( other->getCanonicalInterval() );
        if ( not otherCanonical.intersect(constraints) ) continue;
        if (    other->getDataNegociate()
           and (other->getDataNegociate()->getGCellOrder() < Session::getOrder()) ) {
          if ( otherCanonical.getVMin() < constraints.getVMin() )
            constraints.shrinkVMin ( otherCanonical.getVMax() );
          else
            constraints.shrinkVMax ( otherCanonical.getVMin() );
          continue;
        }

      // Try to ripup conflicting neighbor.
        if ( Manipulator(other,_S).canRipup() ) {
          ltrace(200) << "  | Ripup: " << begin << " " << other << endl;
          _S.addAction ( other, SegmentAction::OtherRipup );
        } else {
          ltrace(200) << "Aborted ripup of perpandiculars, blocked by border/ring." << endl;
          return false;
        }
      }
    }

    if ( cagedPerpandiculars ) {
      ltrace(200) << "Aborted ripup of perpandiculars, constraints are due to fixed/blockage." << endl;
      _S.addAction ( _segment, SegmentAction::SelfRipup );
      return true;
    }

    if ( _segment->isLocal() and not placedPerpandiculars ) {
      ltrace(200) << "No placed perpandiculars, tight native constraints, place perpandiculars FIRST." << endl;
      for ( size_t i=0 ; i < perpandiculars.size() ; i++ ) {
        _S.addAction ( perpandiculars[i], perpandicularActionFlags|SegmentAction::EventLevel4 );
      }
      _S.addAction ( _segment, parallelActionFlags );
      return true;
    }

    RoutingPlane* plane    = Session::getKiteEngine()->getRoutingPlaneByLayer(_segment->getLayer());
    size_t        tracksNb = 0;

    track = plane->getTrackByPosition(constraints.getVMin());

    if ( track && (track->getAxis() < constraints.getVMin()) ) track = track->getNext();
    for ( ; track && (track->getAxis() <= constraints.getVMax())
          ; track = track->getNext(), tracksNb++ );

    if ( _segment->isLocal() and (tracksNb < 2) ) success = ripple();

    _S.addAction ( _segment, parallelActionFlags );
    return success;
  }


  bool  Manipulator::relax ( Interval interval, unsigned int flags )
  {
    interval.inflate ( - Session::getExtensionCap() /*+ DbU::lambda(5.0)*/ ); // Ugly.
    ltrace(200) << "Manipulator::relax() of: " << _segment << " " << interval << endl; 

    if ( _segment->isFixed() ) return false;
    if ( not interval.intersect(_segment->getCanonicalInterval()) ) return false;
    if ( not _data ) return false;
  //if ( _data->isBorder() or _data->isRing() ) return false;

    if (    _segment->isTerminal()
       and (_segment->getLayer() == Session::getRoutingGauge()->getRoutingLayer(1)) ) {
      if ( interval.contains(_segment->base()->getAutoSource()->getX()) ) return false;
      if ( interval.contains(_segment->base()->getAutoTarget()->getX()) ) return false;
    }

    bool         success      = true;
    unsigned int currentOrder = Session::getOrder ();

    ltracein(200);
    ltrace(200) << "Current o:" << currentOrder << " vs. Segment o:" << _data->getGCellOrder() << endl;

  //bool expand = (_data->getGCellOrder() < currentOrder);
    bool expand = _segment->isGlobal() and (flags&AllowExpand);
    ltrace(200) << "Expand:" << expand << endl;

  // Temp: <= replace < equal order can be relaxeds.
    if ( _data->getGCellOrder() <= currentOrder ) {
      vector<GCell*> gcells;
      _segment->getGCells ( gcells );
      if ( gcells.size() < 2 ) {
        cerr << Bug("relax() Cannot break %p:%s,\n"
                    "      only in %s (current order %d)."
                   ,_segment->base()->base()
                   ,getString(_segment).c_str()
                   ,getString(gcells[0]).c_str()
                   ,currentOrder
                   ) << endl;
        ltraceout(200);
        return false;
      }

      unsigned int  depth = Session::getRoutingGauge()->getLayerDepth(_segment->getLayer());
      Interval      uside;
      size_t        dogLegCount  = 0;
      size_t        iminconflict = gcells.size();
      size_t        imaxconflict = gcells.size();
      size_t        igcell;

    // Look for closest enclosing min & max GCells indexes.
      for ( igcell=0 ; igcell<gcells.size() ; igcell++ ) {
        uside = gcells[igcell]->getUSide(_segment->getDirection(),true);
        ltrace(200) << "| " << gcells[igcell] << " uside: " << uside << endl;

        if ( uside.contains(interval.getVMin()) ) {
          iminconflict = igcell;
          ltrace(200) << "> Min conflict: " << iminconflict << endl;
        }
        if ( uside.contains(interval.getVMax()) ) {
          imaxconflict = igcell;
          ltrace(200) << "> Max conflict: " << imaxconflict << endl;
        }
      }

    // Expand min & max to enclose GCells of greatest or equal order
    // (i.e. less saturateds)
      bool minExpanded = false;
      bool maxExpanded = false;
      if ( expand ) {
        size_t iexpand = iminconflict;
        if ( iminconflict < gcells.size() ) {
          for ( igcell=iminconflict ; true ; igcell-- ) {
            if ( gcells[igcell]->getOrder() >= currentOrder ) iexpand = igcell;
            else break;

            if ( igcell == 0 ) break;
          } 

          if ( iexpand > 0 ) {
            size_t imindepth = iexpand;
            for ( ; iexpand < iminconflict ; iexpand++ ) {
              if ( not _segment->canDogLegAt(gcells[iexpand],true) ) continue;

              ltrace(200) << "Density " << Session::getRoutingGauge()->getRoutingLayer(depth)->getName()
                          << " " << gcells[iexpand]->getDensity(depth) << endl;

              if ( gcells[imindepth]->getDensity(depth) > gcells[iexpand]->getDensity(depth) )
                imindepth = iexpand;
            }
            iexpand = imindepth;
          }
          if ( iminconflict != iexpand ) minExpanded = true;
          iminconflict = (iexpand>0) ? iexpand : gcells.size();
        }

        iexpand = imaxconflict;
        for ( igcell=imaxconflict ; igcell < gcells.size() ; igcell++ ) {
          if ( gcells[igcell]->getOrder() >= currentOrder ) iexpand = igcell;
          else break;
        }
        if ( iexpand < gcells.size()-1 ) {
          size_t imindepth = iexpand;
          for ( ; iexpand > imaxconflict ; iexpand-- ) {
            if ( not _segment->canDogLegAt(gcells[iexpand],true) ) continue;

            ltrace(200) << "Density " << Session::getRoutingGauge()->getRoutingLayer(depth)->getName()
                        << " " << gcells[iexpand]->getDensity(depth) << endl;

            if ( gcells[imindepth]->getDensity(depth) > gcells[iexpand]->getDensity(depth) )
              imindepth = iexpand;
          }
          iexpand = imindepth;
        }
        if ( imaxconflict != iexpand ) maxExpanded = true;
        imaxconflict = (iexpand < gcells.size()) ? iexpand : gcells.size();
      }
      ltrace(200) <<   "minExpanded:" << minExpanded << " (" << iminconflict
                  << ") maxExpanded:" << maxExpanded << " (" << imaxconflict << ")" << endl;

    // Check for full enclosure.
      if (   ( (iminconflict == gcells.size()) and (imaxconflict == gcells.size()  ) )
          or ( (iminconflict == 0)             and (imaxconflict == gcells.size()-1) )) {
        cinfo << "[INFO] Manipulator::relax(): Segment fully enclosed in interval." << endl;
        ltraceout(200);
        return false;
      }

    // Check order validity under the relaxed interval.
    // Should be implicitely satisfied.

    // Suppress min/max if it's the first/last.
      if ( (iminconflict < gcells.size()) and (imaxconflict == gcells.size()-1) ) imaxconflict = gcells.size();
      if ( (imaxconflict < gcells.size()) and (iminconflict == 0) ) iminconflict = gcells.size();

    // Compute number of doglegs and nature of the *first* dogleg.
    // (first can be min or max, second can only be max)
      bool firstDogLegIsMin = false;
      if ( iminconflict < gcells.size() ) { dogLegCount++; firstDogLegIsMin = true; }
      if ( imaxconflict < gcells.size() )   dogLegCount++;

      switch ( dogLegCount ) {
        case 2:
        // Compact only if the double dogleg is at beginning or end.
          if ( iminconflict == imaxconflict ) {
            if ( iminconflict == 0 ) {
            // First dogleg is max.
              dogLegCount--;
            } else if ( iminconflict == gcells.size()-1 ) {
              dogLegCount--;
              firstDogLegIsMin = true;
            }
          }
          break;
        case 1: break;
        case 0:
          cerr << Bug("Manipulator::relax() Can't find a GCell suitable for making dogleg.\n"
                     "      Cannot move either left or right of %s. Current order %d."
                     ,getString(interval).c_str()
                     ,currentOrder
                     ) << endl;
          ltraceout(200);
          return false;
      }

      ltrace(200) << "| Has to do " << dogLegCount << " doglegs." << endl;

    // Check for conflict occuring outside the current RoutingSet.
    // Ugly: 2 track pitch.
#ifdef DISABLE_NARROW_DOGLEG
      if ( (iminconflict != gcells.size())
         and (  (gcells[iminconflict]->getOrder() > currentOrder)
             or (_data->getGCellOrder()           < currentOrder) ) ) {
        uside = gcells[iminconflict]->getUSide(_segment->getDirection(),true);
        if ( interval.getVMin() - uside.getVMin() < DbU::lambda(10.0) ) {
          ltrace(200) << "Min conflict dogleg space is too narrow (<2 tracks)." << endl;
          ltraceout(200);
          return false;
        } else {
          ltrace(200) << "Min conflict has sufficent slack min:"
                      << DbU::getValueString(interval.getVMin()) << " uside:"
                      << DbU::getValueString(uside.getVMin())
                      << endl;
        }
      }
#endif

#ifdef DISABLE_NARROW_DOGLEG
      if ( (imaxconflict != gcells.size())
         and (  (gcells[imaxconflict]->getOrder() > currentOrder)
             or (_data->getGCellOrder()           < currentOrder) ) ) {
        uside = gcells[imaxconflict]->getUSide(_segment->getDirection(),true);
        if ( uside.getVMax() - interval.getVMax() < DbU::lambda(10.0) ) {
          ltrace(200) << "Max conflict dogleg space is too narrow (<2 tracks)." << endl;
          ltraceout(200);
          return false;
        } else {
          ltrace(200) << "Max conflict has sufficent slack max:"
                      << DbU::getValueString(interval.getVMax()) << " uside:"
                      << DbU::getValueString(uside.getVMax())
                      << endl;
        }
      }
#endif

    // Check of "min is less than one track close the edge" (while not expanded).
    // AND we are on the first GCell AND there's one dogleg only.
      if ( not minExpanded and (iminconflict == 0) and (imaxconflict == gcells.size()) ) {
        ltrace(200) << "Cannot break in first GCell only." << endl;
        ltraceout(200);
        return false;
      }

    // Check of "min is less than one track close the edge" (while not expanded).
      if ( /*not minExpanded and*/ (iminconflict > 0) and (iminconflict < gcells.size()) ) {
        uside = gcells[iminconflict-1]->getUSide(_segment->getDirection(),true);
        ltrace(200) << "GCell Edge Comparison (min): " << uside
                    << " vs. " << DbU::getValueString(interval.getVMin()) << endl;
      // Ugly: One lambda shrink.
        if ( interval.getVMin()-DbU::lambda(1.0) <= uside.getVMax() ) {
          if ( gcells[iminconflict-1]->getOrder() >= currentOrder ) {
            ltrace(200) << "Using previous GCell." << endl;
            iminconflict--;
          } else {
            ltrace(200) << "Cannot break in previous GCell." << endl;
            ltraceout(200);
            return false;
          }
        }
      }

    // Check if there is only one dogleg AND it's the last one.
      if ( not maxExpanded and (iminconflict == gcells.size()) and (imaxconflict == gcells.size()-1) ) {
        ltrace(200) << "Cannot break in last GCell only." << endl;
        ltraceout(200);
        return false;
      }

    // Check of "max is less than one track close the edge" (while not expanded).
    // AND we are on the last GCell AND there's one dogleg only.
      // if ( not maxExpanded and (iminconflict == gcells.size()) and (imaxconflict == gcells.size()-1) ) {
      //   uside = gcells[imaxconflict]->getUSide(_segment->getDirection(),true);
      //   ltrace(200) << "GCell Edge Comparison (max): " << uside
      //               << " vs. " << DbU::getValueString(interval.getVMax()) << endl;
      // // Ugly: Direct uses of routing pitch.
      //   if ( interval.getVMax()+DbU::lambda(10.0) >= uside.getVMax() ) {
      //     ltrace(200) << "Cannot break in last GCell only, less than 2 pitches." << endl;
      //     ltraceout(200);
      //     return false;
      //   }
      // }

    // Check of "max is less than one track close the edge" (while not expanded).
      if ( /*not maxExpanded and*/ (imaxconflict < gcells.size()-1) ) {
        uside = gcells[imaxconflict+1]->getUSide(_segment->getDirection(),true);
        ltrace(200) << "GCell Edge Comparison (max): " << uside
                    << " vs. " << DbU::getValueString(interval.getVMax()) << endl;
      // Ugly: Direct uses of routing pitch.
        if ( interval.getVMax()+DbU::lambda(5.0) >= uside.getVMin() ) {
          if ( gcells[imaxconflict+1]->getOrder() >= currentOrder ) {
            ltrace(200) << "Using next GCell." << endl;
            imaxconflict++;
          } else {
            ltrace(200) << "Cannot break in next GCell." << endl;
            ltraceout(200);
            return false;
          }
        }
      }

      size_t ifirstDogleg  = gcells.size();
      size_t isecondDogleg = gcells.size();
      if ( not firstDogLegIsMin ) {
        ifirstDogleg = imaxconflict;
      } else {
        ifirstDogleg  = iminconflict;
        isecondDogleg = imaxconflict;
      }

    // Making first dogleg.
      ltrace(200) << "Making FIRST dogleg at " << ifirstDogleg << endl;
      TrackElement* segment1     = NULL;
      TrackElement* segment2     = NULL;
      Track*        track        = _segment->getTrack();
      GCell*        dogLegGCell  = gcells[ifirstDogleg];
      TrackElement* dogleg       = NULL;
      DbU::Unit     doglegAxis;
      bool          doglegReuse1 = false;
      bool          doglegReuse2 = false;

    // Try to reuse existing dogleg if broken at either end.
      if ( ifirstDogleg == 0 )               dogleg = _segment->getSourceDogLeg();
      if ( ifirstDogleg == gcells.size()-1 ) dogleg = _segment->getTargetDogLeg();
      if ( dogleg ) {
        ltrace(200) << "Reusing dogleg." << endl;
        doglegReuse1 = true;
        segment1     = _segment;
      } else {
      // Try to create a new dogleg.
        if ( not _segment->canDogLegAt(dogLegGCell) ) {
          ltrace(200) << "Cannot create FIRST dogleg." << endl;
          ltraceout(200);
          return false;
        }
        dogleg   = _segment->makeDogLeg ( dogLegGCell );
        segment1 = Session::lookup ( Session::getDogLegs()[2] );
      }

      if ( firstDogLegIsMin ) {
        if ( minExpanded ) {
        //doglegAxis = dogLegGCell->getUSide(_segment->getDirection(),false).getVMax() - DbU::lambda(1.0);
          doglegAxis = dogLegGCell->getUSide(_segment->getDirection(),false).getCenter();
        } else {
        // Ugly: hardcoded pitch.
          doglegAxis = interval.getVMin() - DbU::lambda(5.0);
        }
      } else {
        if ( maxExpanded ) {
          doglegAxis = dogLegGCell->getUSide(_segment->getDirection(),false).getVMin();
        } else {
        // Ugly: hardcoded pitch (5.0 - 1.0).
          doglegAxis = interval.getVMax() + DbU::lambda(4.0);
        }
      }
      if ( doglegReuse1 ) _S.addAction ( dogleg, SegmentAction::OtherRipup );
      else                dogleg->setAxis ( doglegAxis );

    // If event is present, the dogleg is in the current RoutingSet.
      RoutingEvent* event = dogleg->getDataNegociate()->getRoutingEvent();
      if ( event ) {
        ltrace(200) << "Set Axis Hint: @" << DbU::getValueString(doglegAxis) << " " << dogleg << endl;
        event->setAxisHint ( doglegAxis );
      } else {
        ltrace(200) << "Dogleg has no RoutingEvent yet." << endl;
      }

      if ( (dogLegCount == 2)
         and not _segment->getTrack()
         and (_segment->getGCell()->getOrder() < Session::getOrder()) ) {
        Session::addInsertEvent ( _segment, track );
      }

    // Making second dogleg.
      if ( dogLegCount > 1 ) {
        ltrace(200) << "Making SECOND dogleg at " << isecondDogleg
                    << " on " << segment1 << endl;

        dogleg      = NULL;
        dogLegGCell = gcells[isecondDogleg];

        if ( ifirstDogleg == isecondDogleg ) {
          ltrace(200) << "Double break in same GCell." << endl;
          segment1->setSourceDogLeg(false);
        }

        if ( isecondDogleg == gcells.size()-1 ) dogleg = segment1->getTargetDogLeg();
        if ( dogleg ) {
          ltrace(200) << "Reusing dogleg." << endl;
          doglegReuse2 = true;
          segment2     = segment1;
        } else {
        // Try to create a new dogleg.
          if ( not segment1->canDogLegAt(dogLegGCell) ) {
            ltrace(200) << "Cannot create SECOND dogleg." << endl;
            ltraceout(200);
            return false;
          }
          dogleg   = segment1->makeDogLeg ( dogLegGCell );
          segment2 = Session::lookup ( Session::getDogLegs()[2] );
        }

        if ( maxExpanded ) {
        //doglegAxis = dogLegGCell->getUSide(segment1->getDirection(),false).getVMin();
          doglegAxis = dogLegGCell->getUSide(segment1->getDirection(),false).getCenter();
        } else {
        // Ugly: hardcoded pitch.
          doglegAxis = interval.getVMax() + DbU::lambda(5.0);
        }
        if ( doglegReuse2 ) _S.addAction ( dogleg, SegmentAction::OtherRipup );
        else                dogleg->setAxis ( doglegAxis );

      // If event is present, the dogleg is in the current RoutingSet.
        RoutingEvent* event = dogleg->getDataNegociate()->getRoutingEvent();
        if ( event ) {
          ltrace(200) << "Set Axis Hint: @" << DbU::getValueString(doglegAxis) << " " << dogleg << endl;
          event->setAxisHint ( doglegAxis );
        } else {
          ltrace(200) << "Dogleg has no RoutingEvent yet." << endl;
        }

        const vector<AutoSegment*>& doglegs = Session::getDogLegs();
        for ( size_t i=0 ; i<doglegs.size() ; i++ ) { 
          TrackElement* segment = Session::lookup(doglegs[i]);
          if ( ( segment->getGCell()->getOrder() < Session::getOrder() ) and not segment->getTrack() ) {
            Session::addInsertEvent ( segment, track );
          }
        }
      }

      switch ( dogLegCount ) {
        case 1:
          if ( not doglegReuse1 ) {
            if ( firstDogLegIsMin )
              _segment->getDataNegociate()->setState ( DataNegociate::RipupPerpandiculars, true );
            else
              segment1->getDataNegociate()->setState ( DataNegociate::RipupPerpandiculars, true );
          }
          break;
        case 2:
          if ( not doglegReuse1 )
            _segment->getDataNegociate()->setState ( DataNegociate::RipupPerpandiculars, true );
          if ( not doglegReuse2 )
            segment2->getDataNegociate()->setState ( DataNegociate::RipupPerpandiculars, true );
          break;
      }

    } else if ( _data->getGCellOrder() > currentOrder ) {
      cerr << Bug("relax() Routing order problem for %s."
                 ,getString(_segment).c_str()) << endl;
      success = false;
    } else {
    // The TrackElement has an order equal to the Session, it's in the
    // RoutingSet.
      if ( !Manipulator(_segment,_S).makeDogLeg(interval) ) success = false;
    }

    ltraceout(200);
    return success;
  }


  bool  Manipulator::insertInTrack ( size_t itrack )
  {
    Track*              track            = _S.getTrack(itrack);
    size_t              begin            = _S.getBegin(itrack);
    size_t              end              = _S.getEnd  (itrack);
    Net*                ownerNet         = _segment->getNet();
    Interval            toFree            (_segment->getCanonicalInterval());
    Net*                ripupNet         = NULL;
    set<TrackElement*>  canonicals;
    DbU::Unit           rightAxisHint    = 0;
    DbU::Unit           leftAxisHint     = 0;
    bool                leftIntrication  = false;
    bool                rightIntrication = false;
    bool                success          = true;
    unsigned long       maxId            = AutoSegment::getMaxId();

    ltrace(200) << "Manipulator::insertInTrack() - " << toFree << endl;

    for ( size_t i = begin ; success && (i < end) ; i++ ) {
      TrackElement* segment2 = track->getSegment(i);

      ltrace(200) << "* Looking // " << segment2 << endl;

      if ( segment2->getNet() == ownerNet  ) continue;
      if ( not toFree.intersect(segment2->getCanonicalInterval()) ) continue;
      if ( segment2->getId() >= maxId ) continue;
      if ( segment2->isBlockage() or segment2->base()->isFixed() ) {
        success = false;
        continue;
      }
    //if ( (segment2->getNet() != ripupNet )
    //   && !toFree.intersect(segment2->getCanonicalInterval()) ) continue;
      ripupNet = segment2->getNet();

      DataNegociate* data2 = segment2->getDataNegociate();
      if ( !data2 ) {
        ltrace(200) << "No DataNegociate, ignoring." << endl;
        continue;
      }

      if ( data2->getGCellOrder() < Session::getOrder() ) {
      // Interval relax.
        success = Manipulator(segment2,_S).relax ( toFree );
      } else {
        if ( data2->getState() == DataNegociate::MaximumSlack ) {
          ltrace(200) << "At " << DataNegociate::getStateString(data2)
                      << " for " << segment2 << endl;
          success = false;
          continue;
        }

        bool shrinkLeft  = false;
        bool shrinkRight = false;

        if ( data2->getCost().getRightMinExtend() < toFree.getVMin() ) {
          ltrace(200) << "- Shrink right edge (push left) " << segment2 << endl;
          shrinkRight = true;
          TrackElement* rightNeighbor2 = track->getSegment(i+1);
          if ( rightNeighbor2 && (rightNeighbor2->getNet() == segment2->getNet()) ) {
            Interval interval1 = segment2->getCanonicalInterval();
            Interval interval2 = rightNeighbor2->getCanonicalInterval();

            if ( interval1.intersect(interval2) && (interval2.getVMax() > interval1.getVMax()) )
              shrinkLeft = true;
          }
        }

        if ( data2->getCost().getLeftMinExtend() > toFree.getVMax() ) {
          ltrace(200) << "- Shrink left edge (push right) " << segment2 << endl;
          shrinkLeft = true;
          if ( i > 0 ) {
            TrackElement* leftNeighbor2 = track->getSegment(i-1);
            if ( leftNeighbor2 && (leftNeighbor2->getNet() == segment2->getNet()) ) {
              Interval interval1 = segment2->getCanonicalInterval();
              Interval interval2 = leftNeighbor2->getCanonicalInterval();

              if ( interval1.intersect(interval2) && (interval2.getVMin() < interval1.getVMin()) )
                shrinkRight = true;
            }
          }
        }

        if ( not (shrinkLeft xor shrinkRight) ) {
          ltrace(200) << "- Hard overlap/enclosure/shrink " << segment2 << endl;
          if ( not (success = Manipulator(segment2,_S).ripup(toFree,SegmentAction::OtherRipup)) )
            continue;
        }

        canonicals.clear ();
        forEach ( TrackElement*, isegment3
                , segment2->getCollapsedPerpandiculars().getSubSet(TrackElements_UniqCanonical(canonicals)) ) {
          DataNegociate* data3 = isegment3->getDataNegociate();
          if ( not data3 ) continue;

          RoutingEvent* event3 = data3->getRoutingEvent();
          if ( not event3 ) continue;

          if ( not toFree.intersect(event3->getConstraints()) ) {
            ltrace(200) << "  . " << *isegment3 << endl;
            continue;
          }

          ltrace(200) << "  | " << *isegment3 << endl;

          if ( shrinkRight xor shrinkLeft ) {
            if ( shrinkRight ) {
              if ( not (success=Manipulator(*isegment3,_S).ripup ( track->getAxis()
                                                                 , SegmentAction::OtherPushAside
                                                                 | SegmentAction::AxisHint
                                                                 , toFree.getVMin() - DbU::lambda(1.0)
                                                                 )) )
                break;

              if ( event3->getTracksFree() == 1 ) {
                ltrace(200) << "Potential left intrication with other perpandicular." << endl;
                if ( isegment3->getAxis() == segment2->getTargetU() - Session::getExtensionCap() ) {
                  leftIntrication = true;
                  leftAxisHint    = isegment3->getAxis();
                }
              }
            }
            if ( shrinkLeft  ) {
              if ( not (success=Manipulator(*isegment3,_S).ripup ( track->getAxis()
                                                                 , SegmentAction::OtherPushAside
                                                                 | SegmentAction::AxisHint
                                                                 , toFree.getVMax() + DbU::lambda(1.0)
                                                                 )) )
                break;
              if ( event3->getTracksFree() == 1 ) {
                ltrace(200) << "Potential right intrication with other perpandicular." << endl;
                if ( isegment3->getAxis() == segment2->getSourceU() + Session::getExtensionCap() ) {
                  rightIntrication = true;
                  rightAxisHint    = isegment3->getAxis();
                }
              }
            }
          } else {
            // DbU::Unit axisHint
            //   = (event3->getAxisHint() - toFree.getVMin() < toFree.getVMax() - event3->getAxisHint()) 
            //   ? (toFree.getVMin() - DbU::lambda(1.0)) : (toFree.getVMax() + DbU::lambda(1.0));
            if ( not (success=Manipulator(*isegment3,_S).ripup ( track->getAxis()
                                                               , SegmentAction::OtherRipup
                                                               | SegmentAction::EventLevel3
                                                             //| SegmentAction::AxisHint, axisHint
                                                               )) )
              break;
          }
        }
        if ( not success ) break;
      }
    }

    if ( success ) {
      _S.setState  ( State::OtherRipup );
      _S.addAction ( _segment, SegmentAction::SelfInsert /*| SegmentAction::EventLevel3*/ );
      _segment->setAxis ( _S.getCost(itrack).getTrack()->getAxis() );

      unsigned int flags = 0;
      if ( rightIntrication ) flags |= RightAxisHint;
      if ( leftIntrication  ) flags |= LeftAxisHint;
      if ( flags )
        Manipulator(_segment,_S).shrinkToTrack(itrack,flags,leftAxisHint,rightAxisHint);
    } else
      _S.clearActions ();

    return success;
  }


  bool  Manipulator::forceToTrack ( size_t itrack )
  {
    Track*              track      = _S.getTrack(itrack);
    size_t              begin      = _S.getBegin(itrack);
    size_t              end        = _S.getEnd  (itrack);
    Net*                ownerNet   = _segment->getNet();
    Interval            toFree      (_segment->getCanonicalInterval());
    Net*                ripupNet   = NULL;
    set<TrackElement*>  canonicals;
    bool                success    = true;

    ltrace(200) << "Manipulator::forceToTrack() - " << toFree << endl;

    for ( size_t i = begin ; success && (i < end) ; i++ ) {
      TrackElement* segment2 = track->getSegment(i);

      ltrace(200) << "* Looking // " << segment2 << endl;

      if (  segment2->getNet() == ownerNet  ) continue;
      if ( !toFree.intersect(segment2->getCanonicalInterval()) ) continue;
      if (  segment2->base()->isFixed() ) {
        success = false;
        continue;
      }
    //if ( (segment2->getNet() != ripupNet )
    //   && !toFree.intersect(segment2->getCanonicalInterval()) ) continue;
      ripupNet = segment2->getNet();

      DataNegociate* data2 = segment2->getDataNegociate();
      if ( !data2 ) {
        ltrace(200) << "No DataNegociate, ignoring." << endl;
        continue;
      }

      if ( data2->getGCellOrder() < Session::getOrder() ) {
      // Interval relax.
        success = Manipulator(segment2,_S).relax ( toFree );
      } else {
        ltrace(200) << "- Forced ripup " << segment2 << endl;
        if ( not (success=Manipulator(segment2,_S).ripup(toFree,SegmentAction::OtherRipup)) )
          continue;

        canonicals.clear ();
        forEach ( TrackElement*, isegment3
                , segment2->getCollapsedPerpandiculars().getSubSet(TrackElements_UniqCanonical(canonicals)) ) {
          DataNegociate* data3 = isegment3->getDataNegociate();
          if ( !data3 ) continue;

          RoutingEvent* event3 = data3->getRoutingEvent();
          if ( !event3 ) continue;

          if ( Manipulator(*isegment3,_S).canRipup() )
            _S.addAction ( *isegment3, SegmentAction::OtherRipup );
        }
      }
    }

    if ( success ) {
      _S.setState  ( State::OtherRipup );
      _S.addAction ( _segment, SegmentAction::SelfInsert );
      _segment->setAxis ( _S.getCost(itrack).getTrack()->getAxis() );
    }

    return success;
  }


  bool  Manipulator::shrinkToTrack ( size_t i, unsigned int flags, DbU::Unit leftAxisHint, DbU::Unit rightAxisHint )
  {
    Track*              track       = _S.getTrack(i);
    size_t              begin       = _S.getBegin(i);
    size_t              end         = _S.getEnd  (i);
    Net*                ownerNet    = _segment->getNet();
    set<TrackElement*>  canonicals;
    bool                success     = true;
    DbU::Unit           leftExtend  = _segment->getSourceU() + Session::getExtensionCap();
    DbU::Unit           rightExtend = _segment->getSourceU() - Session::getExtensionCap();

    ltrace(200) << "Manipulator::shrinkToTrack()" << endl;

    if ( _segment->isLocal() ) return false;
  //Interval shrunkFree ( _segment->getSourceConstraints().getVMax()
  //                    , _segment->getTargetConstraints().getVMin() );
    Interval  shrunkFree = _segment->base()->getMinSpanU();

    ltrace(200) << "* " << shrunkFree << endl;

    for ( size_t i = begin ; success && (i < end) ; i++ ) {
      TrackElement* segment2 = track->getSegment(i);

      ltrace(200) << "* Looking // " << segment2 << endl;

      if (  segment2->getNet() == ownerNet  ) continue;
      if (  segment2->isFixed() ) { success = false; continue; }
      if ( !shrunkFree.intersect(segment2->getCanonicalInterval()) ) continue;

      success = false;
    }

    if ( success ) {
      set<TrackElement*>           perpandiculars;
      set<TrackElement*>::iterator iperpand;

      DbU::Unit axisHint;
      if ( not (flags & LeftAxisHint ) ) leftAxisHint  = shrunkFree.getCenter();
      if ( not (flags & RightAxisHint) ) rightAxisHint = shrunkFree.getCenter();

      _segment->getPerpandicularsBound ( perpandiculars );
      for ( iperpand = perpandiculars.begin() ; iperpand != perpandiculars.end() ; iperpand++ ) {
        DataNegociate* data2 = (*iperpand)->getDataNegociate();
        if ( data2 && (data2->getGCellOrder() >= Session::getOrder()) ) {

          ltrace(200) << "| perpandicular bound:" << *iperpand << endl;
          success = Manipulator(*iperpand,_S).ripup ( track->getAxis(), SegmentAction::SelfRipupAndAxisHint );
          if ( success ) {
            if      ( (*iperpand)->getAxis() == leftExtend  ) axisHint = leftAxisHint;
            else if ( (*iperpand)->getAxis() == rightExtend ) axisHint = rightAxisHint;
            else {
              cinfo << "[INFO] Bound Axis is neither left nor right\n      "
                    << (*iperpand) << endl;
              axisHint = shrunkFree.getCenter();
            }

            _S.getActions()[_S.getActions().size()-1].setAxisHint ( axisHint );
          }
        }
      }

      _S.addAction ( _segment, SegmentAction::SelfInsert );
      _S.setState  ( State::OtherRipup );

      ltrace(200) << "Successful shrinkToTrack." << endl;
      return true;
    }

    return false;
  }


  bool  Manipulator::desalignate ()
  {
    ltrace(200) << "Manipulator::desalignate() " << _segment << endl; 

    if (  _segment->isFixed       () ) return false;
    if ( !_segment->canDesalignate() ) return false;

    _segment->desalignate ();
    return true;
  }


  bool  Manipulator::slacken ()
  {
    ltrace(200) << "Manipulator::slacken() " << _segment << endl; 

    if (  _segment->isFixed   () ) return false;
    if ( !_segment->canSlacken() ) return false;

    _segment->slacken ();
    return true;
  }


  bool  Manipulator::goOutsideGCell ()
  {
    ltrace(200) << "Manipulator::goOutsideGCell() " << _segment << endl; 
    return false;

#if 0
    if (  _segment->isFixed          () ) return false;
    if ( !_segment->canGoOutsideGCell() ) return false;

    GCell*    sourceGCell = _segment->getGCell();
    GCell*    leftGCell   = NULL;
    GCell*    rightGCell  = NULL;
    Interval  uside;
    bool      goLeft      = false;
    bool      goRight     = false;

    if ( _segment->isHorizontal() ) {
      uside      = sourceGCell->getUSide ( Constant::Vertical, false );
      leftGCell  = sourceGCell->getDown();
      rightGCell = sourceGCell->getUp  ();
    } else {
      uside      = sourceGCell->getUSide ( Constant::Horizontal, false );
      leftGCell  = sourceGCell->getLeft ();
      rightGCell = sourceGCell->getRight();
    }

    Interval constraints;
    _segment->base()->getConstraints ( constraints );

  // Ugly: Must use the right compensator for VMax.
    if ( rightGCell and (uside.getVMax() <= constraints.getVMax())+DbU::lambda(1.0) ) goRight = true;
    if ( leftGCell  and (uside.getVMin() >= constraints.getVMin())                  ) goLeft  = true;

    if ( goRight and goLeft ) {
      DbU::Unit distanceToLeft  = _segment->getAxis() - uside.getVMin();
      DbU::Unit distanceToRight = uside.getVMax() - _segment->getAxis();
      if ( distanceToLeft < distanceToRight ) goRight = false;
      else                                    goLeft  = false;
    }

    _segment->moveAside ( goLeft );

    return true;
#endif
  }


  bool  Manipulator::ripple ()
  {
    ltrace(200) << "Manipulator::ripple() from " << _segment << endl; 

  //if ( not _segment->canRipple() ) return false;
    if ( not _segment->isLocal() ) return false;

    Net*     net   = _segment->getNet();
    Interval uside = _segment->getGCell()->getUSide
      ( Constant::perpandicular(_segment->getDirection()), false );
    RoutingPlane* plane = Session::getKiteEngine()->getRoutingPlaneByLayer(_segment->getLayer());

    ltracein(200);
    forEach ( Track*, itrack, Tracks_Range::get(plane,uside)) {
      size_t begin;
      size_t end;

      itrack->getOverlapBounds ( _segment->getCanonicalInterval(), begin, end );
      for ( ; begin < end ; begin++ ) {
        TrackElement* other = itrack->getSegment(begin);
        ltrace(200) << "| " << other << endl;

        if ( other->getNet() == net ) continue;
        if ( not other->canRipple() ) continue;

        DataNegociate* otherData = other->getDataNegociate();
        if ( not otherData ) continue;
        if ( otherData->getGCellOrder() < Session::getOrder() ) continue;

        DbU::Unit     shiftedAxisHint;
        RoutingEvent* otherEvent = otherData->getRoutingEvent();

        if ( other->getAxis() < _segment->getAxis() ) {
        // Ugly: routing pitch.
          shiftedAxisHint = otherEvent->getAxisHint() - DbU::lambda(5.0);
          if ( shiftedAxisHint < uside.getVMin() )
            shiftedAxisHint = uside.getVMin();
        } else {
        // Ugly: routing pitch.
          shiftedAxisHint = otherEvent->getAxisHint() + DbU::lambda(5.0);
          if ( shiftedAxisHint > uside.getVMax() )
            shiftedAxisHint = uside.getVMax();
        }

        otherEvent->setAxisHint ( shiftedAxisHint );
        _S.addAction ( other, SegmentAction::OtherRipup );
      }
    }
    ltraceout(200);

    return true;
  }


  bool  Manipulator::pivotUp ()
  {
    ltrace(200) << "Manipulator::pivotUp() " << _segment << endl; 

    if (     _segment->isFixed  ()    ) return false;
    if ( not _segment->canMoveUp(0.0) ) return false;

    _segment->moveUp ();
    return true;
  }


  bool  Manipulator::moveUp ()
  {
    ltrace(200) << "Manipulator::moveUp() " << _segment << endl; 

    if ( _segment->isFixed  () ) return false;
    if ( _segment->isLocal() and not _segment->canPivotUp(0.0) ) return false;
    if ( not _segment->canMoveUp(0.5) ) return false;

#if DISABLED
    ltrace(200) << "| Repack Tracks: " << endl;

    Interval      constraints;
    Interval      overlap     = _segment->getCanonicalInterval();
    RoutingPlane* plane       = Session::getKiteEngine()->getRoutingPlaneByLayer(_segment->getLayer());
    size_t        begin;
    size_t        end;
    TrackElement* other;

    constraints = _segment->getGCell()->getUSide(Constant::perpandicular(_segment->getDirection()),true);
    forEach ( Track*, itrack, Tracks_Range::get(plane,constraints)) {
      itrack->getOverlapBounds ( overlap, begin, end );
      for ( ; (begin < end) ; begin++ ) {
        other = itrack->getSegment(begin);

        if ( other->isGlobal() ) continue;
        if (    other->getDataNegociate()
           and (other->getDataNegociate()->getGCellOrder() != Session::getOrder()) ) continue;

        ltrace(200) << "  | Ripup for repack: " << begin << " " << other << endl;
        _S.addAction ( other, SegmentAction::OtherRipup );
      }
    }
#endif

    _segment->moveUp ();
    return true;
  }


  bool  Manipulator::makeDogLeg ()
  {
    ltrace(200) << "Manipulator::makeDogLeg() " << _segment << endl; 

    if (  _segment->isFixed() ) return false;
    if ( !_segment->isLocal() ) return false;

    if ( _S.getCosts().size() ) {
      Track*    track      = _S.getTrack(0);
      size_t    begin      = _S.getBegin(0);
      size_t    end        = _S.getEnd  (0);
      Net*      ownerNet   = _segment->getNet();
      Interval  toFree      (_segment->getCanonicalInterval());
      Interval  overlap;

      for ( size_t i = begin ; i < end ; i++ ) {
        TrackElement* segment2 = track->getSegment(i);

        ltrace(200) << "* Looking // " << segment2 << endl;

        if (  segment2->getNet() == ownerNet  ) continue;
        if ( !toFree.intersect(segment2->getCanonicalInterval()) ) continue;

        if ( overlap.isEmpty() )
          overlap = segment2->getCanonicalInterval();
        else
          overlap.merge ( segment2->getCanonicalInterval() );
      }

      if ( !overlap.isEmpty() && makeDogLeg(overlap) ) return true;
    }

    if ( !_segment->canDogLeg() ) return false;
    _segment->makeDogLeg ();

    return true;
  }


  bool  Manipulator::makeDogLeg ( Interval overlap )
  {
    ltrace(200) << "Manipulator::makeDogLeg(Interval) " << _segment << endl; 
    ltrace(200) << overlap << endl;

  //if ( !_segment->isLocal() || !_segment->canDogLeg() ) return false;

    if (  _segment->isFixed  ()        ) return false;
    if ( !_segment->canDogLeg(overlap) ) return false;

    bool          pushLeft;
    bool          isTerminal = _segment->isTerminal();
    TrackElement* dogleg     = _segment->makeDogLeg(overlap,pushLeft);
    if ( dogleg ) {
      ltrace(200) << "Manipulator::makeDogLeg(Interval) - Push dogleg to the "
                  << ((pushLeft)?"left":"right") << endl;
      if ( isTerminal ) {
        AutoContact*  contact  = (pushLeft) ? _segment->base()->getAutoSource()
                                            : _segment->base()->getAutoTarget();
        DbU::Unit     axisHint = (_segment->isHorizontal()) ? contact->getX() : contact->getY();
        RoutingEvent* event    = dogleg->getDataNegociate()->getRoutingEvent();
        if ( event ) {
          event->setAxisHint     ( axisHint );
          event->setForcedToHint ( true );
          ltrace(200) << "Forced to axis hint @" << DbU::getValueString(axisHint) << endl;
        }
      }
      return true;
    }

    return false;
  }


  bool  Manipulator::makeDogLeg ( DbU::Unit position )
  {
    ltrace(200) << "Manipulator::makeDogLeg(position) " << _segment << endl; 
    ltrace(200) << "Breaking position: " << DbU::getValueString(position) << endl;

    if ( _segment->isFixed() ) return false;

    vector<GCell*> gcells;
    _segment->getGCells ( gcells );

    size_t igcell = 0;
    for ( ; igcell<gcells.size() ; igcell++ ) {
      if ( gcells[igcell]->getUSide(_segment->getDirection(),true).contains(position) )
        break;
    }
    if ( igcell == gcells.size() ) return false;
    if ( not _segment->canDogLegAt(gcells[igcell]) ) return false;

    TrackElement* dogleg = _segment->makeDogLeg(gcells[igcell]);
    return ( dogleg != NULL );
  }


  bool  Manipulator::minimize ()
  {
    ltrace(200) << "Manipulator::minimize() " << _segment << endl; 

    if ( _segment->isFixed() ) return false;
    if ( !_event->canMinimize() ) return false;

    DbU::Unit  minSpan = DbU::Max;
    DbU::Unit  maxSpan = DbU::Min;
    Interval   punctualSpan ( false );

    if ( _segment->base()->getAutoSource()->getAnchor() ) {
      ltrace(200) << "  | " << _segment->base()->getAutoSource() << endl;
      Interval constraints ( _segment->base()->getAutoSource()->getUConstraints
                             (Constant::perpandicular(_segment->getDirection())) );
      ltrace(200) << "  | Constraints: " << constraints << endl;

      minSpan = min ( minSpan, constraints.getVMax() );
      maxSpan = max ( maxSpan, constraints.getVMin() );
      punctualSpan.intersection ( constraints );
    }

    if ( _segment->base()->getAutoTarget()->getAnchor() ) {
      ltrace(200) << "  | " << _segment->base()->getAutoTarget() << endl;
      Interval constraints ( _segment->base()->getAutoTarget()->getUConstraints
                             (Constant::perpandicular(_segment->getDirection())) );
      ltrace(200) << "  | Constraints: " << constraints << endl;

      minSpan = min ( minSpan, constraints.getVMax() );
      maxSpan = max ( maxSpan, constraints.getVMin() );
      punctualSpan.intersection ( constraints );
    }

    const vector<TrackElement*>& perpandiculars = _event->getPerpandiculars();
    for ( size_t i=0 ; i<perpandiculars.size() ; i++ ) {
      DataNegociate* data2 = perpandiculars[i]->getDataNegociate();
      if ( !data2 ) continue;

      ltrace(200) << "  | " << perpandiculars[i] << endl;
      if ( data2->getGCellOrder() < Session::getOrder() ) {
        ltrace(200) << "Reject: bad order." << endl;
        continue;
      }

      RoutingEvent* event2 = data2->getRoutingEvent();
      if ( !event2 ) continue;

      ltrace(200) << "  | Constraints: " << event2->getConstraints() << endl;

      minSpan = min ( minSpan, event2->getConstraints().getVMax() );
      maxSpan = max ( maxSpan, event2->getConstraints().getVMin() );
      punctualSpan.intersection ( event2->getConstraints() );
    }
    if ( minSpan > maxSpan ) swap ( minSpan, maxSpan );

    ltrace(200) << "punctualSpan: " << punctualSpan
                << " min/max span: [" << DbU::getValueString(minSpan)
                <<                ":" << DbU::getValueString(minSpan) << "]" << endl;

    vector<Interval>   holes;
    for ( size_t itrack=0 ; itrack<_S.getCosts().size() ; itrack++ ) {
      size_t  begin = _S.getBegin(itrack);
      size_t  end   = _S.getEnd  (itrack);
      Track*  track = _S.getTrack(itrack);

      if ( end < track->getSize() ) end++;

      ltrace(200) << "Looking for holes in " << _S.getCost(itrack) << endl;

      TrackElement*    otherPrevious = NULL;
    // ToDo: Manage disjoint but subsequent segment of a Net.
    //       (currently, that hole will not be found).
      for ( ; begin < end ; begin++ ) {
        TrackElement* otherSegment = track->getSegment(begin);
        if ( otherSegment->getNet() == _segment->getNet() ) continue;
        if ( !otherPrevious ) {
          holes.push_back ( Interval(track->getMin()
                                    ,otherSegment->getSourceU()) );
          ltrace(200) << "| First hole: " << holes.back() << " " << otherSegment << endl;
        } else {
          if ( otherSegment->getNet() == otherPrevious->getNet() ) continue;

          holes.push_back ( Interval(otherPrevious->getTargetU()
                                    ,otherSegment ->getSourceU()) );
          ltrace(200) << "| Found hole: " << holes.back()
                      << " " << otherPrevious << " <-> " << " " << otherSegment << endl;
        }
        otherPrevious = otherSegment;
      }
    }

    if ( holes.empty() ) {
      ltrace(200) << "No holes found to minimize into." << endl;
      return false;
    }

    Interval  currentSpan = _segment->getCanonicalInterval();
    Interval  biggestHole;
    for ( size_t i=0 ; i<holes.size() ; i++ ) {
      if ( currentSpan.getIntersection(holes[i]   ).getSize()
         > currentSpan.getIntersection(biggestHole).getSize() )
        biggestHole = holes[i];
    }

    if ( !punctualSpan.isEmpty() ) {
      bool success = false;

      if ( biggestHole.intersect(punctualSpan) ) {
        ltrace(200) << "Go as punctual into biggest hole: " << biggestHole << endl;
        success = true;
      } else {
        for ( size_t i=0 ; i<holes.size() ; i++ ) {
          ltrace(200) << "Trying secondary hole: " << holes[i] << endl;
          if ( holes[i].intersect(punctualSpan) ) {
            biggestHole = holes[i];
            ltrace(200) << "Go as punctual into secondary hole: " << biggestHole << endl;
            success = true;
          }
        }
      }

      if ( !success ) {
        ltrace(200) << "No suitable hole found." << endl;
        return false;
      }
    } else {
      biggestHole.intersection ( Interval(minSpan,maxSpan) );
      if ( biggestHole.isEmpty() ) {
        ltrace(200) << "Biggest hole is empty (under span ["
                    << DbU::getValueString(minSpan) << " "
                    << DbU::getValueString(maxSpan) << "])" << endl;
        return false;
      } else {
        ltrace(200) << "Go into biggest hole: " << biggestHole << endl;
      }
    }

    DbU::Unit axisHint = biggestHole.intersection(punctualSpan).getCenter();
    ltrace(200) << "Axis Hint: " << DbU::getValueString(axisHint) << endl;

    for ( size_t i=0 ; i<perpandiculars.size() ; i++ ) {
      DataNegociate* data2 = perpandiculars[i]->getDataNegociate();
      if ( !data2 ) continue;

      ltrace(200) << "  | " << perpandiculars[i] << endl;
      if ( data2->getGCellOrder() < Session::getOrder() ) continue;

      RoutingEvent* event2 = data2->getRoutingEvent();
      if ( !event2 ) continue;

      _S.addAction ( perpandiculars[i], SegmentAction::SelfRipupAndAxisHint, axisHint );
    }

    _event->setMinimized ();

    return true;
  }


  bool  Manipulator::relax ( size_t i )
  {
    ltrace(200) << "Manipulator::relax() " << _segment << endl; 

    if ( /*  _segment->base()->isGlobal()
        ||*/ _segment->isFixed() 
        || ( _segment->getDogLegLevel() > 0 )
        || ( _segment->getLength() < DbU::lambda(10.0) )
        ||  !_segment->canDogLegAt(_segment->getGCell()) )
      return false;

    TrackElement* perpandicular
      = _segment->makeDogLeg ( Session::lookup(_segment->base()->getAutoSource()->getGCell()) );

    if ( perpandicular ) {
      RoutingEvent* event = perpandicular->getDataNegociate()->getRoutingEvent();
      if ( event ) {
        TrackCost& cost    = _S.getCost(i);
        Interval   optimal = cost.getInterval();
        event->setOptimalAxis ( optimal.inflate( - Session::getExtensionCap() ) );
      } else {
        cerr << Bug("New dog leg without active RoutingEvent.\n      "
                    "%s",getString(perpandicular).c_str()) << endl;
      }
    }
    
    return true;
  }


  void  Manipulator::reprocessPerpandiculars ()
  {
    if ( _event->getAxisHistory() == _event->getAxisHint() ) return;

    bool moveLeft = (_event->getAxisHistory() > _event->getAxisHint());

    const vector<TrackElement*>& perpandiculars = _event->getPerpandiculars();
    for ( size_t iperpand=0 ; iperpand<perpandiculars.size() ; iperpand++ ) {
      TrackElement*  perpandicular = perpandiculars[iperpand];
      DataNegociate* data          = perpandicular->getDataNegociate();

      if ( perpandicular->isFixed() ) continue;
      if ( not data or (data->getGCellOrder() != Session::getOrder()) ) continue;
      if ( not perpandicular->getTrack() ) continue;
      if ( not Manipulator(perpandicular,_S).canRipup()
         or (data->getState() >= DataNegociate::MaximumSlack) ) continue;

    // Ugly: ExtensionCap usage.
      if ( moveLeft ) {
        if ( perpandicular->getTargetU()-Session::getExtensionCap() == _event->getAxisHistory() )
          _S.addAction ( perpandicular, SegmentAction::OtherPacking );
      } else {
        if ( perpandicular->getSourceU()+Session::getExtensionCap() == _event->getAxisHistory() )
          _S.addAction ( perpandicular, SegmentAction::OtherPacking );
      }
    }
  }
  

// -------------------------------------------------------------------
// Local Functions.


} // End of anonymous namespace.


namespace Kite {


  using std::cerr;
  using std::endl;
  using std::setw;
  using std::min;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::DebugSession;
  using Hurricane::Bug;
  using Hurricane::ForEachIterator;
  using Hurricane::Net;
  using Hurricane::Layer;


// -------------------------------------------------------------------
// Class  :  "RoutingEvent::Compare".


  bool  RoutingEvent::Compare::operator() ( const RoutingEvent* lhs, const RoutingEvent* rhs ) const
  {
  //if ( !lhs->isValid() ) const_cast<RoutingEvent*>(lhs)->revalidate ();
  //if ( !rhs->isValid() ) const_cast<RoutingEvent*>(rhs)->revalidate ();

  //unsigned int lhsDepth = Session::getRoutingGauge()->getLayerDepth(lhs->getSegment()->getLayer());
  //unsigned int rhsDepth = Session::getRoutingGauge()->getLayerDepth(rhs->getSegment()->getLayer());

  //if ( lhsDepth > rhsDepth ) return true;
  //if ( lhsDepth < rhsDepth ) return false;

    if ( lhs == rhs ) return false;

    return RoutingEvent::Key::Compare() ( lhs->getKey(), rhs->getKey() );

  //   if ( lhs->getEventLevel() < rhs->getEventLevel() ) return true;
  //   if ( lhs->getEventLevel() > rhs->getEventLevel() ) return false;

  // // Uses static ordering.
  //   if ( lhs->getTracksNb() < rhs->getTracksNb() ) return false;
  //   if ( lhs->getTracksNb() > rhs->getTracksNb() ) return true;

  //   if ( lhs->getPriority() < rhs->getPriority() ) return false;
  //   if ( lhs->getPriority() > rhs->getPriority() ) return true;

  // //return false;
  // // For debugging purpose only: ensure a reproductible sort. Could be removed.
  // //bool segmentCompare = TrackElement::CompareByPosition() ( lhs->getSegment(), rhs->getSegment() );

  // //return (segmentCompare or ((void*)lhs < (void*)rhs));
  //   return ((void*)lhs < (void*)rhs);
  }


// -------------------------------------------------------------------
// Class  :  "RoutingEvent::Key".


  bool  RoutingEvent::Key::Compare::operator() ( const RoutingEvent::Key& lhs, const RoutingEvent::Key& rhs ) const
  {
  //if ( lhs._ring xor rhs._ring ) return lhs._ring;

    if ( lhs._eventLevel < rhs._eventLevel ) return true;
    if ( lhs._eventLevel > rhs._eventLevel ) return false;

    if ( lhs._canRipple    xor rhs._canRipple    ) return rhs._canRipple;
  //if ( lhs._slackenStrap xor rhs._slackenStrap ) return lhs._slackenStrap;

  // Uses static ordering.
  //if ( lhs._tracksNb < rhs._tracksNb ) return false;
  //if ( lhs._tracksNb > rhs._tracksNb ) return true;

    if ( lhs._priority < rhs._priority ) return false;
    if ( lhs._priority > rhs._priority ) return true;

    if ( lhs._length < rhs._length ) return true;
    if ( lhs._length > rhs._length ) return false;

    if ( lhs._isHorizontal xor rhs._isHorizontal ) return rhs._isHorizontal;

    if ( lhs._axis > rhs._axis ) return true;
    if ( lhs._axis < rhs._axis ) return false;

    if ( lhs._sourceU > rhs._sourceU ) return true;
    if ( lhs._sourceU < rhs._sourceU ) return false;

    if ( lhs._net->getName() != rhs._net->getName() )
      return lhs._net->getName() < rhs._net->getName();

    return lhs._id < rhs._id;
  }


  RoutingEvent::Key::Key ( const RoutingEvent* event )
    : _slackenStrap(event->getSegment()->isSlackenStrap())
    , _isHorizontal(event->getSegment()->isHorizontal())
    , _canRipple   (event->getSegment()->canRipple())
    , _tracksNb    (event->getTracksNb())
    , _priority    (event->getPriority())
    , _eventLevel  (event->getEventLevel())
    , _length      (event->getSegment()->getLength())
    , _axis        (event->getSegment()->getAxis())
    , _sourceU     (event->getSegment()->getSourceU())
    , _net         (event->getSegment()->getNet())
    , _id          (event->getSegment()->getId())
  { }


  void  RoutingEvent::Key::update ( const RoutingEvent* event )
  {
    if ( !event ) return;

    _slackenStrap = event->getSegment()->isSlackenStrap();
    _canRipple    = event->getSegment()->canRipple();
    _tracksNb     = event->getTracksNb();
    _priority     = event->getPriority();
    _eventLevel   = event->getEventLevel();
    _length       = event->getSegment()->getLength();
    _axis         = event->getSegment()->getAxis();
    _sourceU      = event->getSegment()->getSourceU();
    _id           = event->getSegment()->getId();
  }


// -------------------------------------------------------------------
// Class  :  "RoutingEvent".


  size_t  RoutingEvent::_allocateds = 0;
  size_t  RoutingEvent::_processeds = 0;
  size_t  RoutingEvent::_cloneds    = 0;


  size_t  RoutingEvent::getAllocateds   () { return _allocateds; }
  size_t  RoutingEvent::getProcesseds   () { return _processeds; }
  size_t  RoutingEvent::getCloneds      () { return _cloneds; }
  void    RoutingEvent::resetProcesseds () { _processeds = 0; }


  RoutingEvent::RoutingEvent ( TrackElement* segment, unsigned int mode )
    : _cloned              (false)
    , _processed           (false)
    , _disabled            (false)
    , _valid               (false)
    , _validConstraints    (false)
    , _validPerpandiculars (false)
    , _canHandleConstraints(false)
    , _minimized           (false)
    , _forceToHint         (false)
    , _segment             (segment)
    , _axisHistory         (segment->getAxis())
    , _axisHint            (segment->getAxis())
    , _optimalAxis         (false)
    , _constraints         ()
    , _optimal             ()
    , _perpandicular       (false)
    , _shearGCell          (NULL)
    , _tracksNb            (0)
    , _tracksFree          (0)
    , _insertState         (0)
    , _mode                (mode)
    , _rippleState         (0)
    , _eventLevel          (0)
    , _priority            (0.0)
    , _perpandiculars      ()
    , _key                 (this)
  {
    DataNegociate* data = _segment->getDataNegociate();
    if ( data )
      data->setRoutingEvent ( this );

    // Interval optimal;
    // _segment->base()->getOptimal ( optimal );
    // _axisHint = optimal.getCenter();

    invalidate ( true );

    ltrace(180) << "create: " << (void*)this << ":" << this << endl;
    ltrace(200) << "Initial setAxisHint @" << DbU::getValueString(_axisHint) << endl;

    if ( _segment->getTrack() ) {
      cerr << Bug("RoutingEvent::create() - TrackElement is already inserted in a Track."
                  "\n      %s."
                 ,getString(_segment).c_str()
                 ) << endl;
    }
  }


  RoutingEvent* RoutingEvent::create ( TrackElement* segment, unsigned int mode )
  {
    RoutingEvent* event = new RoutingEvent ( segment, mode );
    ++_allocateds;

    return event;
  }


  RoutingEvent* RoutingEvent::clone () const
  {
    _cloned = true;

    RoutingEvent* clone = new RoutingEvent ( *this );
    ++_allocateds;
    ++_cloneds;

    clone->_cloned     = false;
    clone->_disabled   = false;
    clone->_eventLevel = 0;

    ltrace(200) << "RoutingEvent::clone() " << (void*)clone << ":" << clone
                << " (from: " << (void*)this << ")" <<  endl;

    return clone;
  }


  RoutingEvent::~RoutingEvent ()
  {
    ltrace(180) << "~RoutingEvent() " << (void*)this << endl;

    DataNegociate* data = _segment->getDataNegociate();
    if ( data and ( data->getRoutingEvent() == this ) )
      data->setRoutingEvent ( NULL );
  }


  void  RoutingEvent::destroy ()
  {
    ltrace(180) << "RoutingEvent::destroy() " << (void*)this << ":" << this << endl;
    if ( _allocateds > 0 ) --_allocateds;

    delete this;
  }


  bool  RoutingEvent::isUnimplemented () const
  { return getState() == DataNegociate::Unimplemented; }


  unsigned int  RoutingEvent::getState () const
  {
    DataNegociate* data = _segment->getDataNegociate();
    if ( not data ) return 0;

    return data->getState();
  }


  void  RoutingEvent::setState ( unsigned int state )
  {
    DataNegociate* data = _segment->getDataNegociate();
    if ( not data ) return;

    data->setState ( state );
  }


  void  RoutingEvent::setEventLevel ( unsigned int level )
  { _eventLevel = level; }


  void  RoutingEvent::setAxisHint ( DbU::Unit axis )
  {
    ltrace(200) << "setAxisHint @" << DbU::getValueString(axis) << " " << _segment << endl;
    _axisHint = axis;
  }


  long  RoutingEvent::getAxisWeight ( DbU::Unit axis ) const
  { return abs(axis - _axisHint); }


  RoutingEvent* RoutingEvent::reschedule ( RoutingEventQueue& queue, unsigned int eventLevel )
  {
    RoutingEvent* active = _segment->getDataNegociate()->getRoutingEvent();
    if ( active != this )
      return active->reschedule ( queue, eventLevel );

    RoutingEvent* fork = NULL;

    if ( isUnimplemented() ) {
      ltrace(200) << "Reschedule: cancelled (Unimplemented) "
                  << (void*)this << " -> " << (void*)fork << ":" << fork << endl;
      return NULL;
    }

    if ( not isProcessed() ) {
      fork = this;
      ltrace(200) << "Reschedule/Self: "
                  << (void*)this << " -> "
                  << (void*)fork << ":" << eventLevel << ":" << fork << endl;
    } else {
      fork = clone();
      fork->_processed  = false;

      _segment->getDataNegociate()->setRoutingEvent ( fork );

      ltrace(200) << "Reschedule/Fork: "
                  << (void*)this << " -> " << (void*)fork << ":" << fork << endl;
    }
    if ( fork->_eventLevel < eventLevel )
      fork->_eventLevel = eventLevel;
    queue.repush ( fork );

    return fork;
  }


  void  RoutingEvent::setSegment ( TrackElement* segment )
  {
    if ( _segment )
      _segment->getDataNegociate()->setRoutingEvent ( NULL );
    _segment = segment;
    _segment->getDataNegociate()->setRoutingEvent ( this );

    invalidate ( true, true );
  }


  void  RoutingEvent::process ( RoutingEventQueue& queue, RoutingEventHistory& history )
  {
    DebugSession::open ( _segment->getNet(), 200 );

#if defined(CHECK_DETERMINISM)
    cerr << "Order: "
         <<         getProcesseds()
         << ","  << getEventLevel()
         << ","  << setw(6) << getPriority()
         << " "  << setw(6) << DbU::getValueString(_segment->getLength())
         << " "             << _segment->isHorizontal()
         << " "  << setw(6) << DbU::getValueString(_segment->getAxis())
         << " "  << setw(6) << DbU::getValueString(_segment->getSourceU())
         << ": " << _segment << endl;
#endif
    _processeds++;

    ltrace(210) << "Event:" << _processeds << " " << this << endl;
    ltracein(200);
    ltrace(200) << "State: *before* "
                << DataNegociate::getStateString(_segment->getDataNegociate())
                << " ripup:" << _segment->getDataNegociate()->getRipupCount()
                << endl;
    ltrace(149) << "Level: " << getEventLevel()
                << ", area: " << _segment->getArea() << endl;

    _preCheck(_segment);
    _eventLevel = 0;

    if ( _mode < PostPack ) history.push ( this );

    if ( isProcessed() || isDisabled() ) {
      ltrace(200) << "Already processed or disabled." << endl;
    } else {
      setProcessed ();

      switch ( _mode ) {
        case Negociate: _processNegociate ( queue, history ); break;
        case Pack:
        case PostPack:  _processPacking   ( queue, history ); break;
        default:
          cerr << Bug("RoutingEvent::process() - Unknown mode value:%d.",_mode) << endl;
          break;
      }
    }

#if defined(CHECK_DATABASE)
    Session::getKiteEngine()->_check ( _segment->getNet() );
#endif
    ltraceout(200);

    queue.repushInvalidateds ();
    Session::revalidate ();
    queue.commit ();

    _postCheck(_segment);
    DebugSession::close ();

    if ( Session::getKiteEngine()->getPostEventCb() != NULL )
      Session::getKiteEngine()->getPostEventCb() ();
  }


  void  RoutingEvent::_processNegociate ( RoutingEventQueue& queue, RoutingEventHistory& history )
  {
    ltrace(200) << "* Mode:Negociation." << endl;

    State S ( this, queue, history );

    if ( S.getHistory().looping() ) {
      cerr << Error("RoutingEvent::process() - Simple Loop detection triggered.") << endl;

      setState ( DataNegociate::Unimplemented );
      S.getHistory().dump ( cerr, 40 );
      return;
    }

    if ( S.getState() == State::MissingData ) {
      cerr << Error("RoutingEvent::process() - Missing datas.") << endl;
      return;
    }

    ltracein(200);

    S.getData()->incRipupCount ();

    size_t itrack = 0;
    for ( itrack = 0 ; itrack < S.getCosts().size() ; itrack++ )
      ltrace(200) << "| " << S.getCost(itrack) << endl;

    itrack = 0;
    if ( isForcedToHint() ) {
    // Try to honor the forced axis hint.
      bool hintFound = false;
      for ( ; itrack < S.getCosts().size() ; itrack++ ) {
        if ( S.getCost(itrack).getTrack()->getAxis() == _axisHint ) {
          hintFound = true;
          break;
        }
      }
      if ( !hintFound ) itrack = 0;
      ltrace(200) << "Forcing to hint Track: " << itrack << endl;
    }

    if ( Manipulator(_segment,S).canRipup() ) {
      if ( S.getCosts().size() and S.getCost(itrack).isFree() ) {
      // Track is free: insertion.
        ltrace(200) << "Insert in free space " << this << endl;
        resetInsertState ();

        _axisHistory = _segment->getAxis();
        _eventLevel  = 0;
        Session::addInsertEvent ( _segment, S.getCost(itrack).getTrack() );
        Manipulator(_segment,S).reprocessPerpandiculars ();
        S.setState ( State::SelfInserted );
      } else {
      // Do ripup.
        if ( S.getState() == State::EmptyTrackList ) {
          Manipulator(_segment,S).ripupPerpandiculars ();
        } else {
          for ( itrack=0 ; itrack<S.getCosts().size() ; itrack++ ) {
            if ( S.getCost(itrack).isInfinite() ) break;
            if ( S.insertInTrack(itrack) ) break;
            resetInsertState ();
          }

        //if ( S.getCosts().size() and not S.getCost(itrack).isInfinite() )
        //  S.insertInTrack ( itrack );
          
          if ( S.getState() != State::OtherRipup ) {
            S.slackenTopology ( _segment );
          }
        }
      }
    } else {
    // Ripup limit has been reached.
      if ( not S.slackenTopology ( _segment ) ) {
        S.setState ( State::SelfMaximumRipup );
      }
    }

    S.doActions ();

    if ( itrack < S.getCosts().size() ) {
      ltrace(200) << "Placed: @" << DbU::getValueString(S.getCost(itrack).getTrack()->getAxis())
                  << " " << (void*)this << ":" << this << endl;
    }

    ltraceout(200);
  }


  void  RoutingEvent::_processPacking ( RoutingEventQueue& queue, RoutingEventHistory& history )
  {
    ltrace(200) << "* Mode:Packing." << endl;

    if ( _segment->getTrack() != NULL ) {
      ltrace(200) << "* Cancel: already in Track." << endl;
      return;
    }
    // if ( !_canHandleConstraints ) {
    //   ltrace(200) << "* Cancel: cannot handle constraints." << endl;
    //   return;
    // }

    State S ( this, queue, history );
    if ( S.getState() == State::MissingData    ) return;
    if ( S.getState() == State::EmptyTrackList ) return;

    ltracein(200);
    for ( size_t i = 0 ; i < S.getCosts().size() ; i++ )
      ltrace(200) << "| " << S.getCost(i) << endl;
    ltraceout(200);

    if ( S.getCosts().size() and S.getCost(0).isFree() ) {
      ltrace(200) << "Insert in free space." << endl;
      Session::addInsertEvent ( _segment, S.getCost(0).getTrack() );
      S.setState ( State::SelfInserted );
    } else {
      ltrace(200) << "Packing failed." << endl;
      if ( _mode == Pack ) {
        _mode = Negociate;
        S.addAction ( _segment, SegmentAction::SelfInsert );
        S.doActions ();
      }
    }
  }


  void  RoutingEvent::revalidate ( bool force )
  {
    if ( _valid and not force ) return;

    DebugSession::open ( _segment->getNet(), 148 );

    ltrace(200) << "RoutingEvent::revalidate() - " << (void*)this << ":" << this << endl;
    ltracein(200);
    ltrace(200) << "axisHint:" << DbU::getValueString(_axisHint) << endl;

    _canHandleConstraints = true;
    if ( /*!_validConstraints*/ true ) {
      _segment->base()->getConstraints ( _constraints );
      _segment->base()->getOptimal     ( _optimal );

      ltrace(200) << "| Raw Track Constraint: " << _constraints << endl;

      _validConstraints = true;
    }

    if ( !_validPerpandiculars ) {
      TrackElement* perpandicular;
      Interval      canonical;

      _perpandiculars.clear ();
      forEach ( AutoSegment*, isegment, _segment->base()->getCollapsedPerpandiculars() ) {
        ltrace(200) << "| perpandicular " << *isegment << endl;
        perpandicular = Session::lookup ( isegment->getCanonical(canonical)->base() );
        if ( !perpandicular ) continue;

        _perpandiculars.push_back ( perpandicular );
      }
      _validPerpandiculars = true;

      if ( not _segment->isTerminal() and (_perpandiculars.size() < 2) )
        cerr << Bug("Less than two perpandiculars on %s.",getString(_segment).c_str()) << endl;
    }

    ltrace(200) << "| Track Constraint: " << _constraints << endl;

    _perpandicular = _constraints;
    DataNegociate* dataPerpandicular;

    for ( size_t i=0 ; i<_perpandiculars.size() ; i++ ) {
      dataPerpandicular = _perpandiculars[i]->getDataNegociate();
      if ( !dataPerpandicular ) {
        ltrace(200) << "| No data " << _perpandiculars[i] << endl;
        continue;
      }

      if ( _perpandiculars[i]->getTrack() ) {
        Interval  trackFree = _perpandiculars[i]->getFreeInterval( false );
        ltrace(200) << "| From " << _perpandiculars[i] << endl;
        ltrace(200) << "| Track Perpandicular Free: " << trackFree << endl;

        _perpandicular.intersection ( trackFree );
        if ( dataPerpandicular->getGCellOrder() < Session::getOrder() )
          _constraints.intersection ( trackFree );
      } else {
        ltrace(200) << "| Not in any track " << _perpandiculars[i] << endl;
      }
    }
    _tracksNb = 0;

    ltrace(200) << "| Perpandicular Free: " << _perpandicular << endl;
    if ( !_perpandicular.isEmpty() ) {
      RoutingPlane* plane   = Session::getKiteEngine()->getRoutingPlaneByLayer(_segment->getLayer());
      Track*        track   = plane->getTrackByPosition(_perpandicular.getVMin());

      if ( track && (track->getAxis() < _perpandicular.getVMin()) ) track = track->getNext();
      for ( ; track && (track->getAxis() <= _perpandicular.getVMax())
            ; track = track->getNext(), _tracksNb++ );
    }
    if ( not _tracksNb ) {
      ltrace(200) << "| Reverting to pure constraints." << endl;
      RoutingPlane* plane   = Session::getKiteEngine()->getRoutingPlaneByLayer(_segment->getLayer());
      Track*        track   = plane->getTrackByPosition(_constraints.getVMin());

      if ( track && (track->getAxis() < _constraints.getVMin()) ) track = track->getNext();
      for ( ; track && (track->getAxis() <= _constraints.getVMax())
            ; track = track->getNext(), _tracksNb++ );

      _canHandleConstraints = false;
    }
    if ( not _tracksNb ) {
      ltrace(200) << "| Pure constraints are too tight." << endl;
    //_segment->base()->getConstraints ( _constraints );
      _canHandleConstraints = false;
    }
      
    _priority
      = (DbU::getLambda(_segment->getLength())        + 1.0)
      * (DbU::getLambda(_segment->base()->getSlack()) + 1.0);
  //_priority = (DbU::getLambda(_segment->getLength()) + 1.0) * (float)(_tracksNb+1);
  //_priority = (float)_segment->getArea();
    _valid    = true;

    ltrace(200) << _segment << " has " << _tracksNb << " choices " << _perpandicular << endl;
    ltraceout(200);

    DebugSession::close();
  }


  string  RoutingEvent::_getTypeName () const
  { return "RoutingEvent"; }


  string  RoutingEvent::_getString () const
  {
    string s = "<" + _getTypeName();

    s += " "  + getString(_eventLevel);
    s += ","  + getString((long)getPriority());
    s += " @" + DbU::getValueString(getAxisHistory());
    s += ":"  + getString(_segment);
    s += " "  + getString(_segment->getDataNegociate()->getRipupCount());
    s += " ";
    s +=        (isCloned       ()?"C":"-");
    s +=        (isDisabled     ()?"d":"-");
    s +=        (isUnimplemented()?"u":"-");
    s += ">";

    return s;
  }


  Record* RoutingEvent::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot ( "_segment"   , _segment    ) );
    record->add ( getSlot ( "_processed" , _processed  ) );
                                     
    return record;
  }


} // End of Kite namespace.
