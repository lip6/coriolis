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

namespace Knik {
  class KnikEngine;
}

#include "katana/Constants.h"
#include "katana/TrackElement.h"
#include "katana/Configuration.h"


namespace Katana {

  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::CellViewer;
  using CRL::RoutingGauge;
  using Anabatic::AnabaticEngine;

  class Track;
  class RoutingPlane;
  class NegociateWindow;


// -------------------------------------------------------------------
// Class  :  "Katana::KatanaEngine".

  class KatanaEngine : public AnabaticEngine {
    public:
      typedef  AnabaticEngine  Super;
    public:
      static  const Name&              staticGetName              ();
      static  KatanaEngine*            create                     ( Cell* );
      static  KatanaEngine*            get                        ( const Cell* );
    public:                                                      
      inline  bool                     useClockTree               () const;
      inline  CellViewer*              getViewer                  () const;
      inline  AnabaticEngine*          base                       ();
      inline  Configuration*           getKatanaConfiguration     ();
      virtual Configuration*           getConfiguration           ();
      inline  bool                     getToolSuccess             () const;
      inline  unsigned long            getEventsLimit             () const;
      inline  unsigned int             getRipupLimit              ( unsigned int type ) const;
              unsigned int             getRipupLimit              ( const TrackElement* ) const;
      inline  unsigned int             getRipupCost               () const;
      inline  size_t                   getHTracksReservedLocal    () const;
      inline  size_t                   getVTracksReservedLocal    () const;
      virtual const Name&              getName                    () const;
      inline  Configuration::PostEventCb_t&
                                       getPostEventCb             ();
      inline  NegociateWindow*         getNegociateWindow         ();
      inline  size_t                   getRoutingPlanesSize       () const;
              RoutingPlane*            getRoutingPlaneByIndex     ( size_t index ) const;
              RoutingPlane*            getRoutingPlaneByLayer     ( const Layer* ) const;
              Track*                   getTrackByPosition         ( const Layer*, DbU::Unit axis, unsigned int mode=Constant::Nearest ) const;
      inline  void                     printConfiguration         () const;
              void                     printCompletion            () const;
              void                     dumpMeasures               ( std::ostream& ) const;
              void                     dumpMeasures               () const;
      virtual void                     openSession                ();
      inline  void                     setViewer                  ( CellViewer* );
      inline  void                     setPostEventCb             ( Configuration::PostEventCb_t );
      inline  void                     setEventLimit              ( unsigned long );
      inline  void                     setMinimumWL               ( double );
      inline  void                     setRipupLimit              ( unsigned int type, unsigned int );
      inline  void                     setRipupCost               ( unsigned int );
      inline  void                     setHTracksReservedLocal    ( size_t );
      inline  void                     setVTracksReservedLocal    ( size_t );
              void                     setupPowerRails            ();
              void                     protectRoutingPads         ();
              void                     preProcess                 ();
              void                     setInterrupt               ( bool );
              void                     setupRoutingPlanes         ();
              void                     setupGlobalGraph           ( unsigned int mode );
              void                     annotateGlobalGraph        ();
              void                     setFixedPreRouted          ();
              void                     digitalInit                ();
              void                     analogInit                 ();
              void                     runNegociate               ( unsigned int flags=Flags::NoFlags );
              void                     runGlobalRouter            ();
              void                     runTest                    ();
      virtual void                     finalizeLayout             ();
              void                     _runKatanaInit             ();
              void                     _gutKatana                 ();
              void                     _computeCagedConstraints   ();
              TrackElement*            _lookup                    ( Segment* ) const;
      inline  TrackElement*            _lookup                    ( AutoSegment* ) const;
              bool                     _check                     ( unsigned int& overlap, const char* message=NULL ) const;
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
              vector<RoutingPlane*>    _routingPlanes;
              NegociateWindow*         _negociateWindow;
              double                   _minimumWL;
      mutable bool                     _toolSuccess;
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
  inline  bool                          KatanaEngine::useClockTree            () const { return _configuration->useClockTree(); }
  inline  CellViewer*                   KatanaEngine::getViewer               () const { return _viewer; }
  inline  AnabaticEngine*               KatanaEngine::base                    () { return static_cast<AnabaticEngine*>(this); }
  inline  Configuration*                KatanaEngine::getKatanaConfiguration  () { return _configuration; }
  inline  Configuration::PostEventCb_t& KatanaEngine::getPostEventCb          () { return _configuration->getPostEventCb(); }
  inline  bool                          KatanaEngine::getToolSuccess          () const { return _toolSuccess; }
  inline  unsigned long                 KatanaEngine::getEventsLimit          () const { return _configuration->getEventsLimit(); }
  inline  unsigned int                  KatanaEngine::getRipupCost            () const { return _configuration->getRipupCost(); }
  inline  size_t                        KatanaEngine::getHTracksReservedLocal () const { return _configuration->getHTracksReservedLocal(); }
  inline  size_t                        KatanaEngine::getVTracksReservedLocal () const { return _configuration->getVTracksReservedLocal(); }
  inline  unsigned int                  KatanaEngine::getRipupLimit           ( unsigned int type ) const { return _configuration->getRipupLimit(type); }
  inline  NegociateWindow*              KatanaEngine::getNegociateWindow      () { return _negociateWindow; }
  inline  size_t                        KatanaEngine::getRoutingPlanesSize    () const { return _routingPlanes.size(); }
  inline  void                          KatanaEngine::setViewer               ( CellViewer* viewer ) { _viewer=viewer; }
  inline  void                          KatanaEngine::setEventLimit           ( unsigned long limit ) { _configuration->setEventsLimit(limit); }
  inline  void                          KatanaEngine::setRipupLimit           ( unsigned int type, unsigned int limit ) { _configuration->setRipupLimit(limit,type); }
  inline  void                          KatanaEngine::setRipupCost            ( unsigned int cost ) { _configuration->setRipupCost(cost); }
  inline  void                          KatanaEngine::setHTracksReservedLocal ( size_t reserved ) { _configuration->setHTracksReservedLocal(reserved); }
  inline  void                          KatanaEngine::setVTracksReservedLocal ( size_t reserved ) { _configuration->setVTracksReservedLocal(reserved); }
  inline  void                          KatanaEngine::setMinimumWL            ( double minimum ) { _minimumWL = minimum; }
  inline  void                          KatanaEngine::setPostEventCb          ( Configuration::PostEventCb_t cb ) { _configuration->setPostEventCb(cb); }
  inline  void                          KatanaEngine::printConfiguration      () const { _configuration->print(getCell()); }
  inline  TrackElement*                 KatanaEngine::_lookup                 ( AutoSegment* segment ) const { return segment->getObserver<TrackElement>(AutoSegment::Observable::TrackSegment); }


// Variables.
  extern const char* missingRW;


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::KatanaEngine);


#endif  // KATANA_KATANA_ENGINE_H
