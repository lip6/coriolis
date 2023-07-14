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
// |  C++ Header  :  "./katana/RoutingPlane.h"                       |
// +-----------------------------------------------------------------+


#pragma  once
#include "crlcore/RoutingLayerGauge.h"
#include "katana/Track.h"


namespace Katana {

  using CRL::RoutingLayerGauge;
  class KatanaEngine;


// -------------------------------------------------------------------
// Class  :  "RoutingPlane".
 
  class RoutingPlane {

    public:
      static RoutingPlane*        create             ( KatanaEngine*, size_t depth );
             void                 destroy            ();
      inline bool                 isHorizontal       () const;
      inline bool                 isVertical         () const;
      inline KatanaEngine*        getKatanaEngine    () const;
      inline RoutingLayerGauge*   getLayerGauge      () const;
      inline Flags                getDirection       () const;
      inline size_t               getDepth           () const;
      inline DbU::Unit            getAxisMin         () const;
      inline DbU::Unit            getAxisMax         () const;
      inline DbU::Unit            getTrackMin        () const;
      inline DbU::Unit            getTrackMax        () const;
             RoutingPlane*        getTop             () const;
             RoutingPlane*        getBottom          () const;
      inline const Layer*         getLayer           () const;
      inline const Layer*         getBlockageLayer   () const;
      inline const vector<Track*> getTracks          () const;
      inline size_t               getTracksSize      () const;
      inline size_t               computeTracksSize  () const;
      inline DbU::Unit            getTrackPosition   ( size_t index ) const;
             Track*               getTrackByIndex    ( size_t index ) const;
             Track*               getTrackByPosition ( DbU::Unit axis, uint32_t mode=Constant::Nearest ) const;
             bool                 _check             ( uint32_t& overlaps ) const;
             Record*              _getRecord         () const;
             string               _getString         () const;
      inline string               _getTypeName       () const;

    protected:
    // Sub-Class: TrackCompare.
      struct TrackCompare {
          inline bool operator() ( Track* track1, Track* track2 );
      };

    protected:
    // Attributes.
      KatanaEngine*      _katana;
      RoutingLayerGauge* _layerGauge;
      size_t             _depth;
      Flags              _flags;
      DbU::Unit          _axisMin;
      DbU::Unit          _axisMax;
      DbU::Unit          _trackMin;
      DbU::Unit          _trackMax;
      vector<Track*>     _tracks;

    protected:
    // Constructors & Destructors.
                    RoutingPlane ( KatanaEngine*, size_t depth );
                   ~RoutingPlane ();
    private:
      RoutingPlane               ( const RoutingPlane& );
      RoutingPlane& operator=    ( const RoutingPlane& );
  };


// Inline Functions.
  inline bool RoutingPlane::TrackCompare::operator() ( Track* lhs, Track* rhs )
  { return lhs->getAxis() > rhs->getAxis(); };

  inline KatanaEngine*         RoutingPlane::getKatanaEngine  () const { return _katana; }
  inline RoutingLayerGauge*    RoutingPlane::getLayerGauge    () const { return _layerGauge; }
  inline Flags                 RoutingPlane::getDirection     () const { return _flags & Flags::DirectionMask; }
  inline size_t                RoutingPlane::getDepth         () const { return _depth; }
  inline DbU::Unit             RoutingPlane::getAxisMin       () const { return _axisMin; }
  inline DbU::Unit             RoutingPlane::getAxisMax       () const { return _axisMax; }
  inline DbU::Unit             RoutingPlane::getTrackMin      () const { return _trackMin; }
  inline DbU::Unit             RoutingPlane::getTrackMax      () const { return _trackMax; }
  inline const Layer*          RoutingPlane::getLayer         () const { return getLayerGauge()->getLayer(); }
  inline const Layer*          RoutingPlane::getBlockageLayer () const { return getLayerGauge()->getBlockageLayer(); }
  inline const vector<Track*>  RoutingPlane::getTracks        () const { return _tracks; }
  inline size_t                RoutingPlane::getTracksSize    () const { return _tracks.size(); }
  inline string                RoutingPlane::_getTypeName     () const { return "RoutingPlane"; }

  inline size_t  RoutingPlane::computeTracksSize () const
  { return _layerGauge->getTrackNumber(_axisMin,_axisMax); }

  inline DbU::Unit  RoutingPlane::getTrackPosition ( size_t index ) const
  { return _layerGauge->getTrackPosition(_axisMin,index); }

  inline bool  RoutingPlane::isHorizontal () const
  { return (getDirection() & Flags::Horizontal); }

  inline bool  RoutingPlane::isVertical () const
  { return (getDirection() & Flags::Vertical); }


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::RoutingPlane);
