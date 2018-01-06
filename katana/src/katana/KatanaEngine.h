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

  class Block;
  class Track;
  class RoutingPlane;
  class NegociateWindow;


// -------------------------------------------------------------------
// Class  :  "Katana::KatanaEngine".

  class KatanaEngine : public AnabaticEngine {
    public:
      static const uint32_t  DigitalMode = (1 <<  0);
      static const uint32_t  AnalogMode  = (1 <<  1);
      static const uint32_t  MixedMode   = (1 <<  2);
      static const uint32_t  ChannelMode = (1 <<  3);
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
      inline  bool                     useClockTree               () const;
      inline  CellViewer*              getViewer                  () const;
      inline  AnabaticEngine*          base                       ();
      inline  Configuration*           getKatanaConfiguration     ();
      virtual Configuration*           getConfiguration           ();
      inline  bool                     getToolSuccess             () const;
      inline  uint64_t                 getEventsLimit             () const;
      inline  uint32_t                 getRipupLimit              ( uint32_t type ) const;
              uint32_t                 getRipupLimit              ( const TrackElement* ) const;
      inline  uint32_t                 getRipupCost               () const;
      inline  uint32_t                 getHTracksReservedLocal    () const;
      inline  uint32_t                 getVTracksReservedLocal    () const;
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
      inline  const std::map<Net*,DataSymmetric*>&
                                       getSymmetrics              () const;
      inline  Block*                   getBlock                   ( size_t i ) const;
      inline  void                     printConfiguration         () const;
              void                     printCompletion            () const;
              void                     dumpMeasures               ( std::ostream& ) const;
              void                     dumpMeasures               () const;
      virtual void                     openSession                ();
      inline  void                     setViewer                  ( CellViewer* );
      inline  void                     setPostEventCb             ( Configuration::PostEventCb_t );
      inline  void                     setEventLimit              ( uint64_t );
      inline  void                     setMinimumWL               ( double );
      inline  void                     setRipupLimit              ( uint32_t type, uint32_t );
      inline  void                     setRipupCost               ( uint32_t );
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
              void                     runNegociate               ( Flags flags=Flags::NoFlags );
              void                     runGlobalRouter            ();
              void                     runTest                    ();
      virtual void                     finalizeLayout             ();
              void                     _runKatanaInit             ();
              void                     _gutKatana                 ();
              void                     _computeCagedConstraints   ();
              TrackElement*            _lookup                    ( Segment* ) const;
      inline  TrackElement*            _lookup                    ( AutoSegment* ) const;
              bool                     _check                     ( uint32_t& overlap, const char* message=NULL ) const;
              void                     _check                     ( Net* ) const;
      virtual Record*                  _getRecord                 () const;
      virtual string                   _getString                 () const;
      virtual string                   _getTypeName               () const;
    private:                          
    // Attributes.                    
      static  Name                          _toolName;
    protected:                              
              CellViewer*                   _viewer;
              Configuration*                _configuration;
              vector<Block*>                _blocks;
              vector<RoutingPlane*>         _routingPlanes;
              NegociateWindow*              _negociateWindow;
              double                        _minimumWL;
              std::map<Net*,DataSymmetric*> _symmetrics;
              uint32_t                      _mode;
      mutable bool                          _toolSuccess;
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
  inline  bool                          KatanaEngine::useClockTree            () const { return _configuration->useClockTree(); }
  inline  CellViewer*                   KatanaEngine::getViewer               () const { return _viewer; }
  inline  AnabaticEngine*               KatanaEngine::base                    () { return static_cast<AnabaticEngine*>(this); }
  inline  Configuration*                KatanaEngine::getKatanaConfiguration  () { return _configuration; }
  inline  Configuration::PostEventCb_t& KatanaEngine::getPostEventCb          () { return _configuration->getPostEventCb(); }
  inline  bool                          KatanaEngine::getToolSuccess          () const { return _toolSuccess; }
  inline  uint64_t                      KatanaEngine::getEventsLimit          () const { return _configuration->getEventsLimit(); }
  inline  uint32_t                      KatanaEngine::getRipupCost            () const { return _configuration->getRipupCost(); }
  inline  uint32_t                      KatanaEngine::getHTracksReservedLocal () const { return _configuration->getHTracksReservedLocal(); }
  inline  uint32_t                      KatanaEngine::getVTracksReservedLocal () const { return _configuration->getVTracksReservedLocal(); }
  inline  uint32_t                      KatanaEngine::getRipupLimit           ( uint32_t type ) const { return _configuration->getRipupLimit(type); }
  inline  bool                          KatanaEngine::profileEventCosts       () const { return _configuration->profileEventCosts(); }
  inline  const std::map<Net*,DataSymmetric*>&
                                        KatanaEngine::getSymmetrics           () const { return _symmetrics; }
  inline  Block*                        KatanaEngine::getBlock                ( size_t i ) const { return (i < _blocks.size()) ? _blocks[i] : NULL; }
  inline  NegociateWindow*              KatanaEngine::getNegociateWindow      () { return _negociateWindow; }
  inline  size_t                        KatanaEngine::getRoutingPlanesSize    () const { return _routingPlanes.size(); }
  inline  void                          KatanaEngine::setViewer               ( CellViewer* viewer ) { _viewer=viewer; }
  inline  void                          KatanaEngine::setEventLimit           ( uint64_t limit ) { _configuration->setEventsLimit(limit); }
  inline  void                          KatanaEngine::setRipupLimit           ( uint32_t type, uint32_t limit ) { _configuration->setRipupLimit(limit,type); }
  inline  void                          KatanaEngine::setRipupCost            ( uint32_t cost ) { _configuration->setRipupCost(cost); }
  inline  void                          KatanaEngine::setHTracksReservedLocal ( uint32_t reserved ) { _configuration->setHTracksReservedLocal(reserved); }
  inline  void                          KatanaEngine::setVTracksReservedLocal ( uint32_t reserved ) { _configuration->setVTracksReservedLocal(reserved); }
  inline  void                          KatanaEngine::addBlock                ( Block* block ) { _blocks.push_back(block); }
  inline  void                          KatanaEngine::setMinimumWL            ( double minimum ) { _minimumWL = minimum; }
  inline  void                          KatanaEngine::setPostEventCb          ( Configuration::PostEventCb_t cb ) { _configuration->setPostEventCb(cb); }
  inline  void                          KatanaEngine::printConfiguration      () const { _configuration->print(getCell()); }
  inline  TrackElement*                 KatanaEngine::_lookup                 ( AutoSegment* segment ) const { return segment->getObserver<TrackElement>(AutoSegment::Observable::TrackSegment); }


// Variables.
  extern const char* missingRW;


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::KatanaEngine);


#endif  // KATANA_KATANA_ENGINE_H
