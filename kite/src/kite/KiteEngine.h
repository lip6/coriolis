// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./kite/KiteEngine.h"                           |
// +-----------------------------------------------------------------+


#ifndef  KITE_KITE_ENGINE_H
#define  KITE_KITE_ENGINE_H

#include <iostream>

#include "hurricane/Name.h"
namespace Hurricane {
  class Layer;
  class Net;
  class Cell;
  class CellViewer;
}

#include "crlcore/RoutingGauge.h"
#include "katabatic/KatabaticEngine.h"

namespace Knik {
  class KnikEngine;
}

#include "kite/Constants.h"
#include "kite/TrackElement.h"
#include "kite/Configuration.h"


namespace Kite {

  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::CellViewer;
  using CRL::RoutingGauge;
  using Katabatic::KatabaticEngine;

  class Track;
  class RoutingPlane;
  class NegociateWindow;


// -------------------------------------------------------------------
// Class  :  "Kite::KiteEngine".

  class KiteEngine : public KatabaticEngine {

    public:
      static  const Name&             staticGetName              ();
      static  KiteEngine*             create                     ( Cell* );
      static  KiteEngine*             get                        ( const Cell* );
      static  void                    wipeOutRouting             ( Cell* );
    public:                                                      
      inline  bool                    useClockTree               () const;
      inline  CellViewer*             getViewer                  () const;
      inline  KatabaticEngine*        base                       ();
      inline  Configuration*          getKiteConfiguration       ();
      virtual Configuration*          getConfiguration           ();
      inline  Net*                    getBlockageNet             ();
      inline  bool                    getToolSuccess             () const;
      inline  unsigned long           getEventsLimit             () const;
      inline  unsigned int            getRipupLimit              ( unsigned int type ) const;
              unsigned int            getRipupLimit              ( const TrackElement* ) const;
      inline  unsigned int            getRipupCost               () const;
      inline  size_t                  getHTracksReservedLocal    () const;
      inline  size_t                  getVTracksReservedLocal    () const;
      virtual const Name&             getName                    () const;
      inline  Configuration::PostEventCb_t&
                                      getPostEventCb             ();
      inline  NegociateWindow*        getNegociateWindow         ();
      inline  size_t                  getRoutingPlanesSize       () const;
              RoutingPlane*           getRoutingPlaneByIndex     ( size_t index ) const;
              RoutingPlane*           getRoutingPlaneByLayer     ( const Layer* ) const;
              Track*                  getTrackByPosition         ( const Layer*, DbU::Unit axis, unsigned int mode=Constant::Nearest ) const;
      inline  void                    printConfiguration         () const;
              void                    printCompletion            () const;
              void                    dumpMeasures               ( std::ostream& ) const;
              void                    dumpMeasures               () const;
      inline  void                    setViewer                  ( CellViewer* );
      inline  void                    setPostEventCb             ( Configuration::PostEventCb_t );
      inline  void                    setEventLimit              ( unsigned long );
      inline  void                    setMinimumWL               ( double );
      inline  void                    setRipupLimit              ( unsigned int type, unsigned int );
      inline  void                    setRipupCost               ( unsigned int );
      inline  void                    setHTracksReservedLocal    ( size_t );
      inline  void                    setVTracksReservedLocal    ( size_t );
              void                    buildPowerRails            ();
              void                    buildPreRouteds            ();
              void                    protectRoutingPads         ();
              void                    preProcess                 ();
              void                    setInterrupt               ( bool );
              void                    createGlobalGraph          ( unsigned int mode );
      virtual void                    createDetailedGrid         ();
              void                    saveGlobalSolution         ();
              void                    annotateGlobalGraph        ();
              void                    setFixedPreRouted          ();
              void                    runNegociate               ( unsigned int flags=KtNoFlags );
              void                    runGlobalRouter            ( unsigned int mode );
      virtual void                    loadGlobalRouting          ( unsigned int method );
      virtual void                    finalizeLayout             ();
              void                    _runKiteInit               ();
              void                    _gutKite                   ();
              void                    _computeCagedConstraints   ();
              TrackElement*           _lookup                    ( Segment* ) const;
      inline  TrackElement*           _lookup                    ( AutoSegment* ) const;
              bool                    _check                     ( unsigned int& overlap, const char* message=NULL ) const;
              void                    _check                     ( Net* ) const;
      virtual Record*                 _getRecord                 () const;
      virtual string                  _getString                 () const;
      virtual string                  _getTypeName               () const;
                                      
    private:                          
    // Attributes.                    
      static Name                     _toolName;
    protected:                        
             CellViewer*              _viewer;
             Knik::KnikEngine*        _knik;
             Net*                     _blockageNet;
             Configuration*           _configuration;
             vector<RoutingPlane*>    _routingPlanes;
             NegociateWindow*         _negociateWindow;
             double                   _minimumWL;
             mutable bool             _toolSuccess;

    protected:
    // Constructors & Destructors.
                          KiteEngine    ( Cell* );
      virtual            ~KiteEngine    ();
      virtual void        _postCreate   ();
      virtual void        _preDestroy   ();
              void        _initDataBase ();
    private:
                          KiteEngine    ( const KiteEngine& );
              KiteEngine& operator=     ( const KiteEngine& );
  };


// Inline Functions.
  inline  bool                          KiteEngine::useClockTree            () const { return _configuration->useClockTree(); }
  inline  CellViewer*                   KiteEngine::getViewer               () const { return _viewer; }
  inline  KatabaticEngine*              KiteEngine::base                    () { return static_cast<KatabaticEngine*>(this); }
  inline  Configuration*                KiteEngine::getKiteConfiguration    () { return _configuration; }
  inline  Net*                          KiteEngine::getBlockageNet          () { return _blockageNet; }
  inline  Configuration::PostEventCb_t& KiteEngine::getPostEventCb          () { return _configuration->getPostEventCb(); }
  inline  bool                          KiteEngine::getToolSuccess          () const { return _toolSuccess; }
  inline  unsigned long                 KiteEngine::getEventsLimit          () const { return _configuration->getEventsLimit(); }
  inline  unsigned int                  KiteEngine::getRipupCost            () const { return _configuration->getRipupCost(); }
  inline  size_t                        KiteEngine::getHTracksReservedLocal () const { return _configuration->getHTracksReservedLocal(); }
  inline  size_t                        KiteEngine::getVTracksReservedLocal () const { return _configuration->getVTracksReservedLocal(); }
  inline  unsigned int                  KiteEngine::getRipupLimit           ( unsigned int type ) const { return _configuration->getRipupLimit(type); }
  inline  NegociateWindow*              KiteEngine::getNegociateWindow      () { return _negociateWindow; }
  inline  size_t                        KiteEngine::getRoutingPlanesSize    () const { return _routingPlanes.size(); }
  inline  void                          KiteEngine::setViewer               ( CellViewer* viewer ) { _viewer=viewer; }
  inline  void                          KiteEngine::setEventLimit           ( unsigned long limit ) { _configuration->setEventsLimit(limit); }
  inline  void                          KiteEngine::setRipupLimit           ( unsigned int type, unsigned int limit ) { _configuration->setRipupLimit(limit,type); }
  inline  void                          KiteEngine::setRipupCost            ( unsigned int cost ) { _configuration->setRipupCost(cost); }
  inline  void                          KiteEngine::setHTracksReservedLocal ( size_t reserved ) { _configuration->setHTracksReservedLocal(reserved); }
  inline  void                          KiteEngine::setVTracksReservedLocal ( size_t reserved ) { _configuration->setVTracksReservedLocal(reserved); }
  inline  void                          KiteEngine::setMinimumWL            ( double minimum ) { _minimumWL = minimum; }
  inline  void                          KiteEngine::setPostEventCb          ( Configuration::PostEventCb_t cb ) { _configuration->setPostEventCb(cb); }
  inline  void                          KiteEngine::printConfiguration      () const { _configuration->print(getCell()); }
  inline  TrackElement*                 KiteEngine::_lookup                 ( AutoSegment* segment ) const { return segment->getObserver<TrackElement>(); }


// Variables.
  extern const char* missingRW;


}  // Kite namespace.


INSPECTOR_P_SUPPORT(Kite::KiteEngine);


#endif  // KITE_KITE_ENGINE_H
