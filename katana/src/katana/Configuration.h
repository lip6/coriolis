// -*- mode: C++; explicit-buffer-name: "Configuration.h<katana>" -*-
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
                       , RipupLimitsTableSize = 4
                       };                     
      enum Constants   { MaxMetalDepth        = 20 };
      enum Flag        { UseClockTree         = (1 << 0) };
    public:
    // Constructor & Destructor.
      virtual       Configuration*             clone                   () const;
                                               Configuration           ();
                                              ~Configuration           ();
    // Decorateds.                                               
      inline        bool                       useClockTree            () const;
      inline        bool                       profileEventCosts       () const;
    // Methods.                                                  
      inline        Anabatic::Configuration*   base                    ();
      inline  const Anabatic::Configuration*   base                    () const;
      inline        PostEventCb_t&             getPostEventCb          ();
      inline        unsigned long              getEventsLimit          () const;
      inline        unsigned int               getRipupCost            () const;
                    unsigned int               getRipupLimit           ( unsigned int type ) const;
      inline        size_t                     getHTracksReservedLocal () const;
      inline        size_t                     getVTracksReservedLocal () const;
      inline        void                       setEventsLimit          ( unsigned long );
      inline        void                       setRipupCost            ( unsigned int );
                    void                       setRipupLimit           ( unsigned int limit, unsigned int type );
      inline        void                       setPostEventCb          ( PostEventCb_t );
                    void                       setHTracksReservedLocal ( size_t );
                    void                       setVTracksReservedLocal ( size_t );
      inline        void                       setFlags                ( unsigned int );
      inline        void                       unsetFlags              ( unsigned int );
      inline        void                       setProfileEventCosts    ( bool );
      virtual       void                       print                   ( Cell* ) const;
      virtual       Record*                    _getRecord              () const;
      virtual       string                     _getString              () const;
      virtual       string                     _getTypeName            () const;
    private:
    // Attributes.
             PostEventCb_t               _postEventCb;
             size_t                      _hTracksReservedLocal;
             size_t                      _vTracksReservedLocal;
             unsigned int                _ripupLimits         [RipupLimitsTableSize];
             unsigned int                _ripupCost;
             unsigned long               _eventsLimit;
             unsigned int                _flags;
             bool                        _profileEventCosts;
    private:
                     Configuration ( const Configuration& other );
      Configuration& operator=     ( const Configuration& );
  };


// Inline Functions.
  inline const Anabatic::Configuration*      Configuration::base                    () const { return dynamic_cast<const Anabatic::Configuration*>(this); }
  inline       Anabatic::Configuration*      Configuration::base                    () { return dynamic_cast<Anabatic::Configuration*>(this); }
  inline       Configuration::PostEventCb_t& Configuration::getPostEventCb          () { return _postEventCb; }
  inline       unsigned long                 Configuration::getEventsLimit          () const { return _eventsLimit; }
  inline       unsigned int                  Configuration::getRipupCost            () const { return _ripupCost; }
  inline       size_t                        Configuration::getHTracksReservedLocal () const { return _hTracksReservedLocal; }
  inline       size_t                        Configuration::getVTracksReservedLocal () const { return _vTracksReservedLocal; }
  inline       void                          Configuration::setRipupCost            ( unsigned int cost ) { _ripupCost = cost; }
  inline       void                          Configuration::setPostEventCb          ( PostEventCb_t cb ) { _postEventCb = cb; }
  inline       void                          Configuration::setEventsLimit          ( unsigned long limit ) { _eventsLimit = limit; }
  inline       bool                          Configuration::useClockTree            () const { return _flags & UseClockTree; }
  inline       bool                          Configuration::profileEventCosts       () const { return _profileEventCosts; }
  inline       void                          Configuration::setFlags                ( unsigned int flags ) { _flags |=  flags; }
  inline       void                          Configuration::unsetFlags              ( unsigned int flags ) { _flags &= ~flags; }
  inline       void                          Configuration::setProfileEventCosts    ( bool state ) { _profileEventCosts = state; }



}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::Configuration);


#endif  // KATANA_CONFIGURATION_H
