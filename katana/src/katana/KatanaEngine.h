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
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/KatanaEngine.h"                       |
// +-----------------------------------------------------------------+


#ifndef  KATANA_KATANA_ENGINE_H
#define  KATANA_KATANA_ENGINE_H

#include <iostream>

#include "hurricane/Name.h"
namespace Hurricane {
  class Layer;
  class Net;
  class Cell;
  class CellViewer;
}

#include "crlcore/RoutingGauge.h"
#include "anabatic/AnabaticEngine.h"
#include "katana/Constants.h"
#include "katana/TrackElement.h"
#include "katana/Configuration.h"
#include "katana/DataSymmetric.h"


namespace Katana {

  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::CellViewer;
  using CRL::RoutingGauge;
  using Anabatic::AnabaticEngine;
  using Anabatic::NetData;

  class Block;
  class Track;
  class RoutingPlane;
  class NegociateWindow;


// -------------------------------------------------------------------
// Class  :  "Katana::KatanaEngine".

  class KatanaEngine : public AnabaticEngine {
    public:
      static const uint32_t  DigitalMode            = (1 <<  0);
      static const uint32_t  AnalogMode             = (1 <<  1);
      static const uint32_t  MixedMode              = (1 <<  2);
      static const uint32_t  ChannelMode            = (1 <<  3);
      static const uint32_t  GlobalRoutingSuccess   = (1 <<  0);
      static const uint32_t  DetailedRoutingSuccess = (1 <<  1);
    public:
      typedef  AnabaticEngine  Super;
    public:
      static  const Name&              staticGetName              ();
      static  KatanaEngine*            create                     ( Cell* );
      static  KatanaEngine*            get                        ( const Cell* );
    public:                                                      
      inline  bool                     isDigitalMode              () const;
      inline  bool                     isAnalogMode               () const;
      inline  bool                     isMixedMode                () const;
      inline  bool                     isChannelMode              () const;
      inline  bool                     isGlobalRoutingSuccess     () const;
      inline  bool                     isDetailedRoutingSuccess   () const;
      inline  bool                     useClockTree               () const;
      inline  bool                     useGlobalEstimate          () const;
      inline  bool                     useStaticBloatProfile      () const;
      inline  CellViewer*              getViewer                  () const;
      inline  AnabaticEngine*          base                       ();
      inline  Configuration*           getKatanaConfiguration     ();
      virtual Configuration*           getConfiguration           ();
      inline  uint32_t                 getSuccessState            () const;
      inline  uint32_t                 getStage                   () const;
      inline  uint64_t                 getEventsLimit             () const;
      inline  uint32_t                 getRipupLimit              ( uint32_t type ) const;
              uint32_t                 getRipupLimit              ( const TrackElement* ) const;
      inline  uint32_t                 getRipupCost               () const;
      inline  uint32_t                 getSearchHalo              () const;
      inline  uint32_t                 getBloatOverloadAdd        () const;
      inline  uint32_t                 getHTracksReservedLocal    () const;
      inline  uint32_t                 getVTracksReservedLocal    () const;
      inline  uint32_t                 getTermSatReservedLocal    () const;
      inline  uint32_t                 getTermSatThreshold        () const;
      inline  uint32_t                 getTrackFill               () const;
      inline  bool                     profileEventCosts          () const;
      virtual const Name&              getName                    () const;
      inline  Configuration::PostEventCb_t&
                                       getPostEventCb             ();
      inline  NegociateWindow*         getNegociateWindow         ();
      inline  size_t                   getRoutingPlanesSize       () const;
              RoutingPlane*            getRoutingPlaneByIndex     ( size_t index ) const;
              RoutingPlane*            getRoutingPlaneByLayer     ( const Layer* ) const;
              Track*                   getTrackByPosition         ( const Layer*, DbU::Unit axis, uint32_t mode=Constant::Nearest ) const;
              DataSymmetric*           getDataSymmetric           ( Net* );
      inline  const DataSymmetricMap&  getSymmetrics              () const;
      inline  Block*                   getBlock                   ( size_t i ) const;
      inline  void                     printConfiguration         () const;
              void                     printCompletion            () const;
              void                     dumpMeasures               ( std::ostream& ) const;
              void                     dumpMeasures               () const;
      inline  void                     setGlobalRoutingSuccess    ( bool ) const;
      inline  void                     setDetailedRoutingSuccess  ( bool ) const;
      virtual void                     openSession                ();
      inline  void                     setStage                   ( uint32_t );
      inline  void                     setViewer                  ( CellViewer* );
      inline  void                     setPostEventCb             ( Configuration::PostEventCb_t );
      inline  void                     setEventLimit              ( uint64_t );
      inline  void                     setMinimumWL               ( double );
      inline  void                     setRipupLimit              ( uint32_t type, uint32_t );
      inline  void                     setRipupCost               ( uint32_t );
      inline  void                     setBloatOverloadAdd        ( uint32_t );
      inline  void                     setHTracksReservedLocal    ( uint32_t );
      inline  void                     setVTracksReservedLocal    ( uint32_t );
      inline  void                     addBlock                   ( Block* );
              DataSymmetric*           addDataSymmetric           ( Net* );
              void                     setupChannelMode           ();
              void                     setupPowerRails            ();
              void                     protectRoutingPads         ();
              void                     preProcess                 ();
              void                     setInterrupt               ( bool );
              void                     createChannels             ();
              void                     setupRoutingPlanes         ();
              void                     setupGlobalGraph           ( uint32_t mode );
              void                     annotateGlobalGraph        ();
              void                     setFixedPreRouted          ();
              void                     digitalInit                ();
              void                     analogInit                 ();
              void                     pairSymmetrics             ();
              void                     updateEstimateDensity      ( NetData*, double weight );
              void                     runNegociate               ( Flags flags=Flags::NoFlags );
              void                     runGlobalRouter            ( Flags flags=Flags::NoFlags );
              void                     computeGlobalWireLength    ( long& wireLength, long& viaCount );
              void                     runTest                    ();
              void                     resetRouting               ();
      virtual void                     finalizeLayout             ();
              void                     _runKatanaInit             ();
              void                     _gutKatana                 ();
              void                     _buildBloatProfile         ();
              void                     _computeCagedConstraints   ();
              TrackElement*            _lookup                    ( Segment* ) const;
      inline  TrackElement*            _lookup                    ( AutoSegment* ) const;
      inline  void                     _addShortDogleg            ( TrackElement*, TrackElement* );
      inline  TrackElement*            _getDoglegPaired           ( TrackElement* ) const;
              bool                     _check                     ( uint32_t& overlap, const char* message=NULL ) const;
              void                     _check                     ( Net* ) const;
      virtual Record*                  _getRecord                 () const;
      virtual string                   _getString                 () const;
      virtual string                   _getTypeName               () const;
    private:                          
    // Attributes.                    
      static  Name                     _toolName;
    protected:                         
              CellViewer*              _viewer;
              Configuration*           _configuration;
              vector<Block*>           _blocks;
              vector<RoutingPlane*>    _routingPlanes;
              NegociateWindow*         _negociateWindow;
              double                   _minimumWL;
              TrackElementPairing      _shortDoglegs;
              DataSymmetricMap         _symmetrics;
              uint32_t                 _mode;
              uint32_t                 _stage;
      mutable uint32_t                 _successState;
    protected:
    // Constructors & Destructors.
                            KatanaEngine  ( Cell* );
      virtual              ~KatanaEngine  ();
      virtual void          _postCreate   ();
      virtual void          _preDestroy   ();
    private:
                            KatanaEngine  ( const KatanaEngine& );
              KatanaEngine& operator=     ( const KatanaEngine& );
  };


// Inline Functions.
  inline  bool                          KatanaEngine::isDigitalMode           () const { return (_mode & DigitalMode); };
  inline  bool                          KatanaEngine::isAnalogMode            () const { return (_mode & AnalogMode); };
  inline  bool                          KatanaEngine::isMixedMode             () const { return (_mode & MixedMode); };
  inline  bool                          KatanaEngine::isChannelMode           () const { return (_mode & ChannelMode); };
  inline  bool                          KatanaEngine::isGlobalRoutingSuccess  () const { return (_successState & GlobalRoutingSuccess); }
  inline  bool                          KatanaEngine::isDetailedRoutingSuccess() const { return (_successState & DetailedRoutingSuccess); }
  inline  bool                          KatanaEngine::useClockTree            () const { return _configuration->useClockTree(); }
  inline  bool                          KatanaEngine::useGlobalEstimate       () const { return _configuration->useGlobalEstimate(); }
  inline  bool                          KatanaEngine::useStaticBloatProfile   () const { return _configuration->useStaticBloatProfile(); }
  inline  CellViewer*                   KatanaEngine::getViewer               () const { return _viewer; }
  inline  AnabaticEngine*               KatanaEngine::base                    () { return static_cast<AnabaticEngine*>(this); }
  inline  Configuration*                KatanaEngine::getKatanaConfiguration  () { return _configuration; }
  inline  uint32_t                      KatanaEngine::getStage                () const { return _stage; }
  inline  Configuration::PostEventCb_t& KatanaEngine::getPostEventCb          () { return _configuration->getPostEventCb(); }
  inline  uint32_t                      KatanaEngine::getSuccessState         () const { return _successState; }
  inline  uint64_t                      KatanaEngine::getEventsLimit          () const { return _configuration->getEventsLimit(); }
  inline  uint32_t                      KatanaEngine::getRipupCost            () const { return _configuration->getRipupCost(); }
  inline  uint32_t                      KatanaEngine::getSearchHalo           () const { return _configuration->getSearchHalo(); }
  inline  uint32_t                      KatanaEngine::getBloatOverloadAdd     () const { return _configuration->getBloatOverloadAdd(); }
  inline  uint32_t                      KatanaEngine::getHTracksReservedLocal () const { return _configuration->getHTracksReservedLocal(); }
  inline  uint32_t                      KatanaEngine::getVTracksReservedLocal () const { return _configuration->getVTracksReservedLocal(); }
  inline  uint32_t                      KatanaEngine::getTermSatReservedLocal () const { return _configuration->getTermSatReservedLocal(); }
  inline  uint32_t                      KatanaEngine::getTermSatThreshold     () const { return _configuration->getTermSatThreshold(); }
  inline  uint32_t                      KatanaEngine::getRipupLimit           ( uint32_t type ) const { return _configuration->getRipupLimit(type); }
  inline  uint32_t                      KatanaEngine::getTrackFill            () const { return _configuration->getTrackFill(); }
  inline  bool                          KatanaEngine::profileEventCosts       () const { return _configuration->profileEventCosts(); }
  inline  const DataSymmetricMap&       KatanaEngine::getSymmetrics           () const { return _symmetrics; }
  inline  Block*                        KatanaEngine::getBlock                ( size_t i ) const { return (i < _blocks.size()) ? _blocks[i] : NULL; }
  inline  NegociateWindow*              KatanaEngine::getNegociateWindow      () { return _negociateWindow; }
  inline  size_t                        KatanaEngine::getRoutingPlanesSize    () const { return _routingPlanes.size(); }
  inline  void                          KatanaEngine::setViewer               ( CellViewer* viewer ) { _viewer=viewer; }
  inline  void                          KatanaEngine::setStage                ( uint32_t stage ) { _stage=stage; }
  inline  void                          KatanaEngine::setEventLimit           ( uint64_t limit ) { _configuration->setEventsLimit(limit); }
  inline  void                          KatanaEngine::setRipupLimit           ( uint32_t type, uint32_t limit ) { _configuration->setRipupLimit(limit,type); }
  inline  void                          KatanaEngine::setRipupCost            ( uint32_t cost ) { _configuration->setRipupCost(cost); }
  inline  void                          KatanaEngine::setBloatOverloadAdd     ( uint32_t add ) { _configuration->setBloatOverloadAdd(add); }
  inline  void                          KatanaEngine::setHTracksReservedLocal ( uint32_t reserved ) { _configuration->setHTracksReservedLocal(reserved); }
  inline  void                          KatanaEngine::setVTracksReservedLocal ( uint32_t reserved ) { _configuration->setVTracksReservedLocal(reserved); }
  inline  void                          KatanaEngine::addBlock                ( Block* block ) { _blocks.push_back(block); }
  inline  void                          KatanaEngine::setMinimumWL            ( double minimum ) { _minimumWL = minimum; }
  inline  void                          KatanaEngine::setPostEventCb          ( Configuration::PostEventCb_t cb ) { _configuration->setPostEventCb(cb); }
  inline  void                          KatanaEngine::printConfiguration      () const { _configuration->print(getCell()); }
  inline  TrackElement*                 KatanaEngine::_lookup                 ( AutoSegment* segment ) const { return segment->getObserver<TrackElement>(AutoSegment::Observable::TrackSegment); }

  inline  void  KatanaEngine::_addShortDogleg ( TrackElement* segmentA, TrackElement* segmentB )
  {
    _shortDoglegs.insert( std::make_pair(segmentA,segmentB) );
    _shortDoglegs.insert( std::make_pair(segmentB,segmentA) );
  }

  inline  TrackElement* KatanaEngine::_getDoglegPaired ( TrackElement* segment ) const
  {
    auto ipaired = _shortDoglegs.find( segment );
    if (ipaired != _shortDoglegs.end()) return ipaired->second;
    return NULL;
  }

  inline  void  KatanaEngine::setGlobalRoutingSuccess  ( bool state ) const
  {
    if (state) _successState |=  GlobalRoutingSuccess;
    else       _successState &= ~GlobalRoutingSuccess;
  }

  inline  void  KatanaEngine::setDetailedRoutingSuccess( bool state ) const
  {
    if (state) _successState |=  DetailedRoutingSuccess;
    else       _successState &= ~DetailedRoutingSuccess;
  }

// Variables.
  extern const char* missingRW;


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::KatanaEngine);


#endif  // KATANA_KATANA_ENGINE_H
