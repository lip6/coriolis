// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./katana/DataNegociate.h"                       |
// +-----------------------------------------------------------------+


#ifndef  KATANA_DATA_NEGOCIATE_H
#define  KATANA_DATA_NEGOCIATE_H

#include <string>
#include <iostream>

namespace Hurricane {
  class Record;
}

#include  "katana/TrackElement.h"
namespace Anabatic {
  class AutoSegment;
}


namespace Katana {

  using std::string;
  using std::cerr;
  using std::endl;
  using Hurricane::Record;
  using Anabatic::AutoSegment;

  class Track;
  class TrackElement;
  class RoutingEvent;


// -------------------------------------------------------------------
// Class  :  "DataNegociate".
 

  class DataNegociate {
    public:
      enum SlackState { RipupPerpandiculars    = 1
                      , Minimize               = 2
                      , Dogleg                 = 3
                      , Slacken                = 4
                      , ConflictSolveByHistory = 5
                      , ConflictSolveByPlaceds = 6
                      , LocalVsGlobal          = 7
                      , MoveUp                 = 8
                      , MaximumSlack           = 9
                      , Unimplemented          =10
                      , Repair                 =11
                      };
    public:
                                          DataNegociate         ( TrackElement* );
                                         ~DataNegociate         ();
      inline bool                         hasRoutingEvent       () const;
      inline RoutingEvent*                getRoutingEvent       () const;
      inline TrackElement*                getTrackSegment       () const;
      inline TrackElement*                getChildSegment       () const;
      inline Track*                       getTrack              () const;
      inline DbU::Unit                    getLeftMinExtend      () const;
      inline DbU::Unit                    getRightMinExtend     () const;
      inline unsigned int                 getTerminals          () const;
      inline Net*                         getNet                () const;
      inline unsigned int                 getState              () const;
      inline unsigned int                 getStateCount         () const;
      inline unsigned int                 getRipupCount         () const;
      inline unsigned int                 getStateAndRipupCount () const;
             DbU::Unit                    getWiringDelta        ( DbU::Unit axis ) const;
      inline const vector<TrackElement*>& getPerpandiculars     () const;
      inline const Interval&              getPerpandicularFree  () const;
      inline void                         setState              ( unsigned int, unsigned int flags=0 );
      inline void                         setRoutingEvent       ( RoutingEvent* );
      inline void                         setChildSegment       ( TrackElement* );
      inline void                         setRipupCount         ( unsigned int );
      inline void                         incRipupCount         ();
      inline void                         decRipupCount         ();
      inline void                         resetRipupCount       ();
      inline void                         resetStateCount       ();
             void                         update                ();
      static string                       getStateString        ( DataNegociate* );
             Record*                      _getRecord            () const;
             string                       _getString            () const;
      inline string                       _getTypeName          () const;
    protected:
    // Attributes.
      TrackElement*         _trackSegment;
      TrackElement*         _childSegment;
      RoutingEvent*         _routingEvent;
      Net*                  _net;
      unsigned int          _state      :  5;
      unsigned int          _stateCount :  5;
      unsigned int          _terminals  :  5;
      unsigned int          _ripupCount : 16;
      DbU::Unit             _leftMinExtend;
      DbU::Unit             _rightMinExtend;
      vector<DbU::Unit>     _attractors;
      vector<TrackElement*> _perpandiculars;
      Interval              _perpandicularFree;
      Interval              _reduceRanges[2];
    private:
                             DataNegociate     ( const DataNegociate& );
              DataNegociate& operator=         ( const DataNegociate& );
  };


// Inline Functions.
  inline bool                         DataNegociate::hasRoutingEvent      () const { return _routingEvent != NULL; }
  inline RoutingEvent*                DataNegociate::getRoutingEvent      () const { return _routingEvent; }
  inline TrackElement*                DataNegociate::getTrackSegment      () const { return _trackSegment; }
  inline TrackElement*                DataNegociate::getChildSegment      () const { return _childSegment; }
  inline Track*                       DataNegociate::getTrack             () const { return _trackSegment->getTrack(); }
  inline unsigned int                 DataNegociate::getState             () const { return _state; }
  inline unsigned int                 DataNegociate::getTerminals         () const { return _terminals; }
  inline unsigned int                 DataNegociate::getRipupCount        () const { return _ripupCount; }
  inline DbU::Unit                    DataNegociate::getLeftMinExtend     () const { return _leftMinExtend; }
  inline DbU::Unit                    DataNegociate::getRightMinExtend    () const { return _rightMinExtend; }
  inline Net*                         DataNegociate::getNet               () const { return _net; }
  inline const vector<TrackElement*>& DataNegociate::getPerpandiculars    () const { return _perpandiculars; }
  inline const Interval&              DataNegociate::getPerpandicularFree () const { return _perpandicularFree; }
  inline unsigned int                 DataNegociate::getStateCount        () const { return _stateCount; }
  inline void                         DataNegociate::resetStateCount      () { _stateCount=0; }
  inline void                         DataNegociate::setRoutingEvent      ( RoutingEvent* event ) { _routingEvent = event; }
  inline void                         DataNegociate::setChildSegment      ( TrackElement* child ) { _childSegment = child; }
  inline void                         DataNegociate::setRipupCount        ( unsigned int count ) { _ripupCount = count; }
  inline void                         DataNegociate::incRipupCount        () { _ripupCount++; }
  inline void                         DataNegociate::decRipupCount        () { if (_ripupCount) _ripupCount--; }
  inline void                         DataNegociate::resetRipupCount      () { _ripupCount = 0; }
  inline string                       DataNegociate::_getTypeName         () const { return "DataNegociate"; }

  inline void  DataNegociate::setState ( unsigned int state, unsigned int flags )
  {
    if ( (_state != state) or (flags & Flags::ResetCount) ) {
    //std::cerr << "Changing state to:" << state << std::endl;
      _state      = state;
      _stateCount = 1;
    } else
      _stateCount++;
  }

  inline unsigned int  DataNegociate::getStateAndRipupCount () const
  { return (_state << 4) + _ripupCount; }


}  // Katana namespace.

#endif  // KATANA_DATA_NEGOCIATE_H
