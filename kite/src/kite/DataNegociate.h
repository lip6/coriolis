
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
// |  C++ Header  :       "./DataNegociate.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __KITE_DATA_NEGOCIATE__
#define  __KITE_DATA_NEGOCIATE__

#include  <string>
#include  <iostream>

namespace Hurricane {
  class Record;
}

#include  "kite/TrackSegmentCost.h"
#include  "kite/TrackElement.h"
#include  "kite/RoutingEvent.h"
namespace Katabatic {
  class AutoSegment;
}


namespace Kite {

  using std::string;
  using std::cerr;
  using std::endl;
  using Hurricane::Record;
  using Katabatic::AutoSegment;

  class Track;
  class TrackElement;
  class RoutingEvent;


// -------------------------------------------------------------------
// Class  :  "DataNegociate".
 

  class DataNegociate {

    public:
      enum SlackState { RipupPerpandiculars= 1
                      , Minimize           = 2
                      , DogLeg             = 3
                      , Desalignate        = 4
                      , Slacken            = 5
                      , ConflictSolve1     = 6
                      , ConflictSolve2     = 7
                      , LocalVsGlobal      = 8
                      , MoveUp             = 9
                      , MaximumSlack       =10
                      , Unimplemented      =11
                      };

    public:
                               DataNegociate         ( TrackElement* );
                              ~DataNegociate         ();
      inline bool              hasRoutingEvent       () const;
      inline RoutingEvent*     getRoutingEvent       () const;
      inline TrackElement*     getTrackSegment       () const;
      inline Track*            getTrack              () const;
      inline TrackSegmentCost& getCost               ();
    //inline unsigned int      getZ                  () const;
      inline unsigned int      getState              () const;
      inline unsigned int      getStateCount         () const;
      inline unsigned int      getRipupCount         () const;
      inline unsigned int      getStateAndRipupCount () const;
      inline void              setState              ( unsigned int, bool reset=false );
      inline void              setRoutingEvent       ( RoutingEvent* );
      inline void              setRipupCount         ( unsigned int );
      inline void              incRipupCount         ();
      inline void              decRipupCount         ();
      inline void              resetRipupCount       ();
      inline void              resetStateCount       ();
      inline void              invalidate            ( bool withPerpandiculars=false, bool withConstraints=false );
             void              update                ();
      static string            getStateString        ( DataNegociate* );
             Record*           _getRecord            () const;
             string            _getString            () const;
      inline string            _getTypeName          () const;

    protected:
    // Attributes.
      RoutingEvent*     _routingEvent;
      TrackElement*     _trackSegment;
      TrackSegmentCost  _cost;
      unsigned int      _state     :5;
      unsigned int      _stateCount:5;
    //unsigned int      _z : 5;

    private:
                             DataNegociate     ( const DataNegociate& );
              DataNegociate& operator=         ( const DataNegociate& );
  };


// Inline Functions.
  inline bool              DataNegociate::hasRoutingEvent   () const { return _routingEvent != NULL; }
  inline RoutingEvent*     DataNegociate::getRoutingEvent   () const { return _routingEvent; }
  inline TrackElement*     DataNegociate::getTrackSegment   () const { return _trackSegment; }
  inline Track*            DataNegociate::getTrack          () const { return _trackSegment->getTrack(); }
  inline TrackSegmentCost& DataNegociate::getCost           ()       { return _cost; }
  inline unsigned int      DataNegociate::getState          () const { return _state; }
  inline unsigned int      DataNegociate::getStateCount     () const { return _stateCount; }
//inline unsigned int      DataNegociate::getZ              () const { return _z; }
  inline unsigned int      DataNegociate::getRipupCount     () const { return _cost.getRipupCount(); }
  inline void              DataNegociate::setRoutingEvent   ( RoutingEvent* event ) { _routingEvent = event; }
  inline void              DataNegociate::setRipupCount     ( unsigned int count ) { _cost.setRipupCount(count); }
  inline void              DataNegociate::incRipupCount     () { _cost.incRipupCount(); }
  inline void              DataNegociate::decRipupCount     () { _cost.decRipupCount(); }
  inline void              DataNegociate::resetRipupCount   () { _cost.resetRipupCount(); }
  inline void              DataNegociate::resetStateCount   () { _stateCount=0; }
  inline string            DataNegociate::_getTypeName      () const { return "DataNegociate"; }

  inline void  DataNegociate::invalidate ( bool withPerpandiculars, bool withConstraints )
  { if (_routingEvent) _routingEvent->invalidate(withPerpandiculars,withConstraints); }

  inline void  DataNegociate::setState ( unsigned int state, bool reset )
  {
    if ( (_state != state) or reset ) {
    //std::cerr << "Changing state to:" << state << std::endl;
      _state      = state;
      _stateCount = 1;
    } else
      _stateCount++;
  }

  inline unsigned int  DataNegociate::getStateAndRipupCount () const
  { return (_state << 4) + getRipupCount(); }


} // End of Kite namespace.


#endif  // __KITE_DATA_NEGOCIATE__
