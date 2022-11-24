// -*- mode: C++; explicit-buffer-name: "Configuration.h<kite>" -*-
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
// |  C++ Header  :   "./kite/Configuration.h"                       |
// +-----------------------------------------------------------------+


#ifndef  KITE_CONFIGURATION_H
#define  KITE_CONFIGURATION_H

#include <functional>
#include "katabatic/Configuration.h"


namespace Kite {

  using std::string;
  using Hurricane::Record;
  using Hurricane::Layer;
  using Hurricane::DbU;
  using Hurricane::Cell;
  using CRL::CellGauge;
  using CRL::RoutingGauge;
  using CRL::RoutingLayerGauge;

  class KiteEngine;


// -------------------------------------------------------------------
// Class  :  "Kite::Configuration".

  class Configuration : public Katabatic::Configuration {
    public:
      typedef  std::function< void(void) >  PostEventCb_t;
    public:
      enum RipupTable  { StrapRipupLimit     =0
                       , LocalRipupLimit     =1
                       , GlobalRipupLimit    =2
                       , LongGlobalRipupLimit=3
                       , RipupLimitsTableSize=4
                       };
      enum Constants   { MaxMetalDepth=20 };
      enum Flag        { UseClockTree=0x0001 };
    public:
    // Constructor & Destructor.
      virtual Configuration*             clone                   () const;
      virtual Configuration*             clone                   ( KiteEngine* kite ) const;
                                         Configuration           ( Katabatic::Configuration* );
                                        ~Configuration           ();
    // Decorateds.                                               
      virtual bool                       isGMetal                ( const Layer* ) const;
      virtual bool                       isGContact              ( const Layer* ) const;
      inline  bool                       useClockTree            () const;
      inline  bool                       profileEventCosts       () const;
      virtual size_t                     getDepth                () const;
      virtual size_t                     getAllowedDepth         () const;
      virtual DbU::Unit                  getSliceHeight          () const;
      virtual DbU::Unit                  getSliceStep            () const;
      virtual size_t                     getLayerDepth           ( const Layer* ) const;
      virtual CellGauge*                 getCellGauge            () const;
      virtual RoutingGauge*              getRoutingGauge         () const;
      virtual RoutingLayerGauge*         getLayerGauge           ( size_t depth ) const;
      virtual const Layer*               getRoutingLayer         ( size_t depth ) const;
      virtual Layer*                     getContactLayer         ( size_t depth ) const;
      virtual DbU::Unit                  getPitch                ( size_t depth, unsigned int flags ) const;
      virtual DbU::Unit                  getOffset               ( size_t depth ) const;
      virtual DbU::Unit                  getWireWidth            ( size_t depth ) const;
      virtual DbU::Unit                  getExtensionCap         ( size_t depth ) const;
      virtual unsigned int               getDirection            ( size_t depth ) const;
      virtual DbU::Unit                  getPitch                ( const Layer*, unsigned int flags ) const;
      virtual DbU::Unit                  getOffset               ( const Layer* ) const;
      virtual DbU::Unit                  getWireWidth            ( const Layer* ) const;
      virtual unsigned int               getDirection            ( const Layer* ) const;
      virtual DbU::Unit                  getExtensionCap         ( const Layer* ) const;
      virtual float                      getSaturateRatio        () const;
      virtual size_t                     getSaturateRp           () const;
      virtual DbU::Unit                  getGlobalThreshold      () const;
      virtual size_t                     getHEdgeCapacity        () const;
      virtual size_t                     getVEdgeCapacity        () const;
      virtual void                       setAllowedDepth         ( size_t );
      virtual void                       setSaturateRatio        ( float );
      virtual void                       setSaturateRp           ( size_t );
      virtual void                       setGlobalThreshold      ( DbU::Unit );
      inline  void                       setProfileEventCosts    ( bool );
      virtual void                       print                   ( Cell* ) const;
    // Methods.                                                  
      inline  Katabatic::Configuration*  base                    ();
      inline  PostEventCb_t&             getPostEventCb          ();
      inline  unsigned long              getEventsLimit          () const;
      inline  unsigned int               getRipupCost            () const;
              unsigned int               getRipupLimit           ( unsigned int type ) const;
      inline  size_t                     getHTracksReservedLocal () const;
      inline  size_t                     getVTracksReservedLocal () const;
      inline  void                       setEventsLimit          ( unsigned long );
      inline  void                       setRipupCost            ( unsigned int );
              void                       setRipupLimit           ( unsigned int limit, unsigned int type );
      inline  void                       setPostEventCb          ( PostEventCb_t );
              void                       setHTracksReservedLocal ( size_t );
              void                       setVTracksReservedLocal ( size_t );
      inline  void                       setFlags                ( unsigned int );
      inline  void                       unsetFlags              ( unsigned int );
      virtual Record*                    _getRecord              () const;
      virtual string                     _getString              () const;
      virtual string                     _getTypeName            () const;
    private:
    // Attributes.
             Katabatic::Configuration*   _base;
             PostEventCb_t               _postEventCb;
             size_t                      _hTracksReservedLocal;
             size_t                      _vTracksReservedLocal;
             unsigned int                _ripupLimits         [RipupLimitsTableSize];
             unsigned int                _ripupCost;
             unsigned long               _eventsLimit;
             unsigned int                _flags;
             bool                        _profileEventCosts;
    private:
                     Configuration ( const Configuration& other, Katabatic::Configuration* base=NULL );
      Configuration& operator=     ( const Configuration& );
  };


// Inline Functions.
  inline Katabatic::Configuration*     Configuration::base                    () { return _base; }
  inline Configuration::PostEventCb_t& Configuration::getPostEventCb          () { return _postEventCb; }
  inline unsigned long                 Configuration::getEventsLimit          () const { return _eventsLimit; }
  inline unsigned int                  Configuration::getRipupCost            () const { return _ripupCost; }
  inline size_t                        Configuration::getHTracksReservedLocal () const { return _hTracksReservedLocal; }
  inline size_t                        Configuration::getVTracksReservedLocal () const { return _vTracksReservedLocal; }
  inline void                          Configuration::setRipupCost            ( unsigned int cost ) { _ripupCost = cost; }
  inline void                          Configuration::setPostEventCb          ( PostEventCb_t cb ) { _postEventCb = cb; }
  inline void                          Configuration::setEventsLimit          ( unsigned long limit ) { _eventsLimit = limit; }
  inline bool                          Configuration::useClockTree            () const { return _flags & UseClockTree; }
  inline bool                          Configuration::profileEventCosts       () const { return _profileEventCosts; }
  inline void                          Configuration::setFlags                ( unsigned int flags ) { _flags |=  flags; }
  inline void                          Configuration::unsetFlags              ( unsigned int flags ) { _flags &= ~flags; }
  inline void                          Configuration::setProfileEventCosts    ( bool state ) { _profileEventCosts = state; }



}  // Kite namespace.


INSPECTOR_P_SUPPORT(Kite::Configuration);


#endif  // KITE_CONFIGURATION_H
