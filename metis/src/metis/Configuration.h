// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      M e t i s  -  h M e t i s   W r a p p e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./metis/Configuration.h"                  |
// +-----------------------------------------------------------------+


#ifndef  METIS_CONFIGURATION_H
#define  METIS_CONFIGURATION_H

#include  <string>
#include  <functional>

namespace Hurricane {
  class Record;
  class Cell;
}


namespace Metis {

  using Hurricane::Record;
  using Hurricane::Cell;


// -------------------------------------------------------------------
// Class  :  "Metis::Configuration".


  class Configuration {
    public:
      typedef std::function< void(void) >  RefreshCb_t;
    public:
      enum MetisOption { CustomOptions    =0
                       , HMetisNRuns      =1
                       , HMetisCType      =2
                       , HMetisRType      =3
                       , HMetisVCycle     =4
                       , HMetisReconst    =5
                       , HMetisPreAssign  =6
                       , HMetisRandom     =7
                       , HMetisDebugLevel =8
                       , HMetisOptionsSize=9
                       };
      enum HMetisCTypeValues   { CTypeHFC            =1  // Hybrid First Choice Scheme.
                               , CTypeFC             =2  // First Choice Scheme.
                               , CTypeGFC            =3  // Greedy First Choice Scheme.
                               , CTypeHyperEdge      =4  // Hyper-Edge Scheme.
                               , CTypeEdge           =5  // Edge Scheme.
                               };
      enum HMetisRTypeValues   { RTypeFM             =1  // Uncoarsening Fiduccia-Mattheyses.
                               , RTypeOneWayFM       =2  // Uncoarsening One Way Fiduccia-Mattheyses.
                               , RTypeEarlyExitFM    =3  // Uncoarsening Early Exit Fiduccia-Mattheyses.
                               };
      enum HMetisVCycleValues  { VCycleDisable       =0  // Do not perform any V-Cycle Reffinment.
                               , VCycleFinal         =1  // Perform V-Cycle on each final bisection step.
                               , VCycleBestIntermed  =2  // Perform V-Cycle on each intermediate solution.
                               , VCycleAllIntermed   =3  // Perform V-Cycle on all intermediate solution.
                               };
      enum HMetisReconstValues { ReconstRemoveCutHE  =0  // Remove cut hyperedges.
                               , ReconstKeepCutHE    =1  // Keep each part of the cuts hyperedges.
                               };
      enum HMetisDebugValues   { DebugDisable        =0  // Disable debugging.
                               , DebugCoarseningStage=1  // Debug coarsening stage.
                               , DebugInitialPart    =2  // Debug initial partition stage.
                               , DebugRefinement     =4  // Debug refinement stage.
                               , DebugMultRuns       =8  // Debug multiple runs stage.
                               , DebugMoreMultRuns   =16 // More debug of the multiple runs stage.
                               };
    public:                                                    
    // Constructor & Destructor.                               
                             Configuration                     ();
                            ~Configuration                     ();
    // Methods.                                                
              void           print                             ( Cell* ) const;
      inline  RefreshCb_t&   getRefreshCb                      ();
      inline  bool           getPartOrKWayHMetis               () const;
      inline  unsigned int   getNumberOfInstancesStopCriterion () const;
      inline  int            getGlobalConnectionsWeightRatio   () const;
      inline  int            getUbFactor                       () const;
      inline  int*           getHMetisOptions                  ();
      inline  int            getHMetisOption                   ( MetisOption );
      inline  void           setRefreshCb                      ( Configuration::RefreshCb_t );
      inline  void           setPartOrKWayHMetis               ( bool usePart );
      inline  void           setNumberOfInstancesStopCriterion ( unsigned int );
      inline  void           setGlobalConnectionsWeightRatio   ( int );
      inline  void           setUbFactor                       ( int );
      inline  void           setHMetisOption                   ( MetisOption, int value );
              Record*        _getRecord                        () const;
              std::string    _getString                        () const;
              std::string    _getTypeName                      () const;
    private:
    // Attributes.
             RefreshCb_t    _refreshCb;
             bool           _partOrKWayHMetis;               // True for Recursive 2-part, false for KWay.
             unsigned int   _numberOfInstancesStopCriterion; // Minimal number of instances to partition.
             int            _globalConnectionsWeightRatio;
             int            _ubFactor;
             int            _hmetisOptions[HMetisOptionsSize];
    private:
                             Configuration ( const Configuration& );
      Configuration& operator=             ( const Configuration& );
  };


// Inline Methods.
  inline  Configuration::RefreshCb_t&
                        Configuration::getRefreshCb                      () { return _refreshCb; }
  inline  bool          Configuration::getPartOrKWayHMetis               () const { return _partOrKWayHMetis; }
  inline  unsigned int  Configuration::getNumberOfInstancesStopCriterion () const { return _numberOfInstancesStopCriterion; }
  inline  int           Configuration::getGlobalConnectionsWeightRatio   () const { return _globalConnectionsWeightRatio; }
  inline  int           Configuration::getUbFactor                       () const { return _ubFactor; }
  inline  int*          Configuration::getHMetisOptions                  () { return _hmetisOptions; }
  inline  int           Configuration::getHMetisOption                   ( MetisOption option ) { return (option<HMetisOptionsSize) ? _hmetisOptions[option] : 0; }
  inline  void          Configuration::setRefreshCb                      ( Configuration::RefreshCb_t cb ) { _refreshCb=cb; }
  inline  void          Configuration::setPartOrKWayHMetis               ( bool usePart ) { _partOrKWayHMetis=usePart; }
  inline  void          Configuration::setNumberOfInstancesStopCriterion ( unsigned int value ) { _numberOfInstancesStopCriterion=value; }
  inline  void          Configuration::setGlobalConnectionsWeightRatio   ( int value ) { _globalConnectionsWeightRatio=value; }
  inline  void          Configuration::setUbFactor                       ( int value ) { _ubFactor=value; }
  inline  void          Configuration::setHMetisOption                   ( MetisOption option, int value )
                        { if (option<HMetisOptionsSize) _hmetisOptions[option] = value; }


}  // Metis namespace.

INSPECTOR_P_SUPPORT(Metis::Configuration);

#endif  // METIS_CONFIGURATION_H
