
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2025-2025.
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/TrackMarkerBase.h"                    |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/DbU.h"
namespace Hurricane {
  class Net;
}


namespace Katana {

  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Interval;
  using Hurricane::Net;

  class Track;

  
// -------------------------------------------------------------------
// Class  :  "TrackMarkerBase".

  class TrackMarkerBase {

    public:
              void             destroy        ();
    public:                                   
      virtual Net*             getNet         () const = 0;
      inline  uint32_t         getRefCount    () const;
      inline  DbU::Unit        getSourceU     () const;
      inline  DbU::Unit        getTargetU     () const;
      inline  Interval         getSpan        () const;
      inline  Track*           getTrack       () const;
      inline  void             setTrack       ( Track* );
      inline  void             setSourceU     ( DbU::Unit );
      inline  void             setTargetU     ( DbU::Unit );
      inline  void             incRefcount    ();
      inline  void             decRefcount    ();
      virtual Record*          _getRecord     () const;
      virtual std::string      _getString     () const;
      virtual std::string      _getTypeName   () const;
    public:
      class Compare {
        public:
          inline bool operator() ( const TrackMarkerBase* lhs , const TrackMarkerBase* rhs  ) const;
          inline bool operator() (       DbU::Unit        lhsU, const TrackMarkerBase* rhs  ) const;
          inline bool operator() ( const TrackMarkerBase* lhs ,       DbU::Unit        rhsU ) const;
        private:
          inline bool markerLess ( DbU::Unit lhsU, DbU::Unit rhsU ) const; 
      };
    private:
      DbU::Unit   _sourcePosition;
      DbU::Unit   _targetPosition;
      Track*      _track;
      uint32_t    _refCount;
    protected:
    // Constructors & destructors.
                       TrackMarkerBase ();
      virtual         ~TrackMarkerBase () {};
    private:
                       TrackMarkerBase ( const TrackMarkerBase& );
      TrackMarkerBase& operator=       ( const TrackMarkerBase& );
  };


// Inline Functions.
  inline  uint32_t      TrackMarkerBase::getRefCount () const { return _refCount; }
  inline  DbU::Unit     TrackMarkerBase::getSourceU  () const { return _sourcePosition; }
  inline  DbU::Unit     TrackMarkerBase::getTargetU  () const { return _targetPosition; }
  inline  Interval      TrackMarkerBase::getSpan     () const { return Interval(_sourcePosition,_targetPosition); }
  inline  Track*        TrackMarkerBase::getTrack    () const { return _track; }
  inline  void          TrackMarkerBase::setTrack    ( Track* track ) { _track = track; }
  inline  void          TrackMarkerBase::setSourceU  ( DbU::Unit u ) { _sourcePosition=u; }
  inline  void          TrackMarkerBase::setTargetU  ( DbU::Unit u ) { _targetPosition=u; }
  inline  void          TrackMarkerBase::incRefcount () { _refCount++; }
  inline  void          TrackMarkerBase::decRefcount () { if (_refCount) _refCount--; }

  inline bool TrackMarkerBase::Compare::operator() ( const TrackMarkerBase* lhs, const TrackMarkerBase* rhs ) const
  { return markerLess ( lhs->getSourceU(), rhs->getSourceU() ); }

  inline bool TrackMarkerBase::Compare::operator() ( DbU::Unit lhsU, const TrackMarkerBase* rhs ) const
  { return markerLess ( lhsU, rhs->getSourceU() ); }

  inline bool TrackMarkerBase::Compare::operator() ( const TrackMarkerBase* lhs, DbU::Unit rhsU ) const
  { return markerLess ( lhs->getSourceU(), rhsU ); }

  inline bool TrackMarkerBase::Compare::markerLess ( DbU::Unit lhsU, DbU::Unit rhsU ) const
  { return ( lhsU < rhsU ); }


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackMarkerBase);
