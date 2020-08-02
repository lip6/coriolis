// -*- mode: C++; explicit-buffer-name: "Configuration.h<katana>" -*-
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
// |  C++ Header  :   "./katana/Configuration.h"                     |
// +-----------------------------------------------------------------+


#ifndef  KATANA_CONFIGURATION_H
#define  KATANA_CONFIGURATION_H

#include <functional>
#include "anabatic/Configuration.h"
#include "katana/Constants.h"


namespace Katana {

  using std::string;
  using Hurricane::Record;
  using Hurricane::Layer;
  using Hurricane::DbU;
  using Hurricane::Cell;
  using CRL::CellGauge;
  using CRL::RoutingGauge;
  using CRL::RoutingLayerGauge;

  class KatanaEngine;


// -------------------------------------------------------------------
// Class  :  "Katana::Configuration".

  class Configuration : public Anabatic::Configuration {
    public:
      typedef  Anabatic::Configuration  Super;
    public:
      typedef  std::function< void(void) >  PostEventCb_t;
    public:
      enum RipupTable  { StrapRipupLimit      = 0
                       , LocalRipupLimit      = 1
                       , GlobalRipupLimit     = 2
                       , LongGlobalRipupLimit = 3
                       , ShortNetRipupLimit   = 4
                       , RipupLimitsTableSize = 5
                       };                     
      enum Constants   { MaxMetalDepth         = 20 };
      enum Flag        { UseClockTree          = (1 << 0)
                       , UseGlobalEstimate     = (1 << 1)
                       , UseStaticBloatProfile = (1 << 2)
                       };
    public:
    // Constructor & Destructor.
      virtual       Configuration*             clone                   () const;
                                               Configuration           ();
                                              ~Configuration           ();
    // Decorateds.                                               
      inline        bool                       useClockTree            () const;
      inline        bool                       useGlobalEstimate       () const;
      inline        bool                       useStaticBloatProfile   () const;
      inline        bool                       profileEventCosts       () const;
    // Methods.                                                  
      inline        Anabatic::Configuration*   base                    ();
      inline  const Anabatic::Configuration*   base                    () const;
      inline        PostEventCb_t&             getPostEventCb          ();
      inline        uint64_t                   getEventsLimit          () const;
      inline        uint32_t                   getRipupCost            () const;
                    uint32_t                   getRipupLimit           ( uint32_t type ) const;
      inline        uint32_t                   getSearchHalo           () const;
      inline        uint32_t                   getBloatOverloadAdd     () const;
      inline        uint32_t                   getHTracksReservedLocal () const;
      inline        uint32_t                   getVTracksReservedLocal () const;
      inline        uint32_t                   getTermSatReservedLocal () const;
      inline        uint32_t                   getHTracksReservedMin   () const;
      inline        uint32_t                   getVTracksReservedMin   () const;
      inline        uint32_t                   getTermSatThreshold     () const;
      inline        void                       setEventsLimit          ( uint64_t );
      inline        void                       setRipupCost            ( uint32_t );
                    void                       setRipupLimit           ( uint32_t limit, uint32_t type );
      inline        void                       setPostEventCb          ( PostEventCb_t );
      inline        void                       setBloatOverloadAdd     ( uint32_t );
                    void                       setHTracksReservedLocal ( uint32_t );
                    void                       setVTracksReservedLocal ( uint32_t );
                    void                       setHTracksReservedMin   ( uint32_t );
                    void                       setVTracksReservedMin   ( uint32_t );
      inline        void                       setFlags                ( unsigned int );
      inline        void                       unsetFlags              ( unsigned int );
      inline        void                       setProfileEventCosts    ( bool );
      virtual       void                       print                   ( Cell* ) const;
      virtual       Record*                    _getRecord              () const;
      virtual       string                     _getString              () const;
      virtual       string                     _getTypeName            () const;
    private:
    // Attributes.
             PostEventCb_t  _postEventCb;
             uint32_t       _searchHalo;
             uint32_t       _hTracksReservedLocal;
             uint32_t       _vTracksReservedLocal;
             uint32_t       _termSatReservedLocal;
             uint32_t       _hTracksReservedMin;
             uint32_t       _vTracksReservedMin;
             uint32_t       _termSatThreshold;
             uint32_t       _ripupLimits         [RipupLimitsTableSize];
             uint32_t       _ripupCost;
             uint64_t       _eventsLimit;
             uint32_t       _bloatOverloadAdd;
             unsigned int   _flags;
             bool           _profileEventCosts;
    private:
                     Configuration ( const Configuration& other );
      Configuration& operator=     ( const Configuration& );
  };


// Inline Functions.
  inline const Anabatic::Configuration*      Configuration::base                    () const { return dynamic_cast<const Anabatic::Configuration*>(this); }
  inline       Anabatic::Configuration*      Configuration::base                    () { return dynamic_cast<Anabatic::Configuration*>(this); }
  inline       Configuration::PostEventCb_t& Configuration::getPostEventCb          () { return _postEventCb; }
  inline       uint64_t                      Configuration::getEventsLimit          () const { return _eventsLimit; }
  inline       uint32_t                      Configuration::getSearchHalo           () const { return _searchHalo; }
  inline       uint32_t                      Configuration::getRipupCost            () const { return _ripupCost; }
  inline       uint32_t                      Configuration::getBloatOverloadAdd     () const { return _bloatOverloadAdd; }
  inline       uint32_t                      Configuration::getHTracksReservedLocal () const { return _hTracksReservedLocal; }
  inline       uint32_t                      Configuration::getVTracksReservedLocal () const { return _vTracksReservedLocal; }
  inline       uint32_t                      Configuration::getTermSatReservedLocal () const { return _termSatReservedLocal; }
  inline       uint32_t                      Configuration::getHTracksReservedMin   () const { return _hTracksReservedMin; }
  inline       uint32_t                      Configuration::getVTracksReservedMin   () const { return _vTracksReservedMin; }
  inline       uint32_t                      Configuration::getTermSatThreshold     () const { return _termSatThreshold; }
  inline       void                          Configuration::setBloatOverloadAdd     ( uint32_t add ) { _bloatOverloadAdd = add; }
  inline       void                          Configuration::setRipupCost            ( uint32_t cost ) { _ripupCost = cost; }
  inline       void                          Configuration::setPostEventCb          ( PostEventCb_t cb ) { _postEventCb = cb; }
  inline       void                          Configuration::setEventsLimit          ( uint64_t limit ) { _eventsLimit = limit; }
  inline       bool                          Configuration::useClockTree            () const { return _flags & UseClockTree; }
  inline       bool                          Configuration::useGlobalEstimate       () const { return _flags & UseGlobalEstimate; }
  inline       bool                          Configuration::useStaticBloatProfile   () const { return _flags & UseStaticBloatProfile; }
  inline       bool                          Configuration::profileEventCosts       () const { return _profileEventCosts; }
  inline       void                          Configuration::setFlags                ( unsigned int flags ) { _flags |=  flags; }
  inline       void                          Configuration::unsetFlags              ( unsigned int flags ) { _flags &= ~flags; }
  inline       void                          Configuration::setProfileEventCosts    ( bool state ) { _profileEventCosts = state; }



}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::Configuration);


#endif  // KATANA_CONFIGURATION_H
