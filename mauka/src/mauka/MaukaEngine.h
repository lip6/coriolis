// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./mauka/MaukaEngine.h"                         |
// +-----------------------------------------------------------------+


#ifndef MAUKA_ENGINE_H
#define MAUKA_ENGINE_H

#include "hurricane/Instance.h"
#include "crlcore/ToolEngine.h"
#include "nimbus/GCell.h"

#include "mauka/Configuration.h"
#include "mauka/FeedCells.h"


namespace Mauka {

  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Hurricane::Occurrence;
  using CRL::ToolEngine;
  using Nimbus::GCell;

  class Surface;
  class SimAnnealingPlacer;
  class BBPlacer;
    

// -------------------------------------------------------------------
// Class  :  "Mauka::MaukaEngine".
//
// Mauka, a cool, light, Hawaiian wind descending from the montains. 

  class MaukaEngine: public ToolEngine 
  {
      friend class Surface;
      friend class SimAnnealingPlacer;
      friend class Move;
      friend class Bin;
      friend class SubRow;
      friend class BBPlacer;
    public:
    // Types.
      typedef ToolEngine                         Inherit;
      typedef std::vector<Occurrence>            InstanceOccurrencesVector;
      typedef std::map<Occurrence, unsigned>     InstanceOccurrencesMap;
      typedef std::vector<unsigned>              UVector;
      typedef std::list<unsigned>                UList;
      typedef std::vector<UVector>               UTable;
      typedef std::vector<bool>                  BVector;
      typedef std::vector<DbU::Unit>             UnitVector;
      typedef std::vector<Net*>                  NetVector;
      typedef std::vector<Box>                   BoxVector;
      typedef std::vector<BoxVector>             BBoxes;
      typedef std::vector< std::vector<double> > Costs; 
      typedef std::vector<bool>                  PrePlaceRow;
      typedef std::vector<PrePlaceRow>           PrePlaceTab;
             
    public:
    // Constructor.
      static  void             regroupOverloadedGCells       ( Cell* );
      static  MaukaEngine*     create                        ( Cell*, Box placementbox = Box() );
    // Accessors               
      static  MaukaEngine*     get                           ( Cell* );
      static  const Name&      staticGetName                 ();
      virtual const Name&      getName                       () const;
      inline  Configuration*   getConfiguration              () const;
      inline  void             printConfiguration            () const;
      inline  const FeedCells& getFeedCells                  () const;
      inline  DbU::Unit        getPitch                      () const;
      inline  DbU::Unit        getSliceHeight                () const;
      inline  DbU::Unit        getSliceStep                  () const;
      inline  bool             useStandardSimulatedAnnealing () const;
      inline  bool             doIgnorePins                  () const;
      inline  bool             doPlotBins                    () const;
      inline  bool             doInsertFeeds                 () const;
      inline  Configuration::RefreshCb_t&
                               getRefreshCb                  ();
      inline  double           getSearchRatio                () const;
      inline  double           getAnnealingNetMult           () const;
      inline  double           getAnnealingBinMult           () const;
      inline  double           getAnnealingRowMult           () const;
      inline  void             setRefreshCb                  ( Configuration::RefreshCb_t cb );
      inline  DbU::Unit        getInstanceIdWidth            ( unsigned id ) const;
              unsigned         getRandomInstanceId           () const;
      virtual std::string      _getTypeName                  () const { return "Mauka::MaukaEngine"; }
      virtual Record*          _getRecord                    () const;
    // Mutators.               
      inline  void             addFeed                       ( Cell* );
              bool             Iterate                       ();
              void             Run                           ();
              void             Test                          ();
              void             Save                          () const;
              void             insertFeeds                   ();
              void             PlotBinsStats                 () const;
              void             Plot                          () const;
    
    private:
    // Attributes
      static Name                _toolName;
      Configuration*             _configuration;
      FeedCells                  _feedCells;
      InstanceOccurrencesVector  _instanceOccurrencesVector;
      InstanceOccurrencesMap     _instanceOccurrencesMap;
      UnitVector                 _instanceWidths;
      UTable                     _instanceNets;
      NetVector                  _nets;
      UTable                     _netInstances;
      UVector                    _netInitX;
      UVector                    _netInitY;
      BVector                    _hasInitX;
      BVector                    _hasInitY;
      Surface*                   _surface;
      SimAnnealingPlacer*        _simAnnealingPlacer;
      BBPlacer*                  _bbPlacer;

    private:
    // Internals.
                       MaukaEngine           ( Cell* );
              void     _maukaPostCreate      ( Box& placementbox );
              void     _preDestroy           ();
      inline  Surface* _getSurface           () const { return _surface; }
              Box      PlotFixedPointsLabels ( std::ofstream& ) const;
              void     Construct             ();
  };


// Inline Methods.
  inline  Configuration*   MaukaEngine::getConfiguration              () const { return _configuration; }
  inline  void             MaukaEngine::printConfiguration            () const { _configuration->print(getCell()); }
  inline  const FeedCells& MaukaEngine::getFeedCells                  () const { return _feedCells; }
  inline  DbU::Unit        MaukaEngine::getPitch                      () const { return _configuration->getPitch(); }
  inline  DbU::Unit        MaukaEngine::getSliceHeight                () const { return _configuration->getSliceHeight(); }
  inline  DbU::Unit        MaukaEngine::getSliceStep                  () const { return _configuration->getSliceStep(); }
  inline  bool             MaukaEngine::useStandardSimulatedAnnealing () const { return _configuration->useStandardSimulatedAnnealing(); }
  inline  bool             MaukaEngine::doIgnorePins                  () const { return _configuration->doIgnorePins(); }
  inline  bool             MaukaEngine::doPlotBins                    () const { return _configuration->doPlotBins(); }
  inline  bool             MaukaEngine::doInsertFeeds                 () const { return _configuration->doInsertFeeds(); }
  inline  Configuration::RefreshCb_t&
                           MaukaEngine::getRefreshCb                  () { return _configuration->getRefreshCb(); }
  inline  double           MaukaEngine::getSearchRatio                () const { return _configuration->getSearchRatio(); }
  inline  double           MaukaEngine::getAnnealingNetMult           () const { return _configuration->getAnnealingNetMult(); } 
  inline  double           MaukaEngine::getAnnealingBinMult           () const { return _configuration->getAnnealingBinMult(); }
  inline  double           MaukaEngine::getAnnealingRowMult           () const { return _configuration->getAnnealingRowMult(); }
  inline  void             MaukaEngine::setRefreshCb                  ( Configuration::RefreshCb_t cb ) { _configuration->setRefreshCb(cb); }
  inline  DbU::Unit        MaukaEngine::getInstanceIdWidth            ( unsigned id ) const { return _instanceWidths[id]; }
  inline  void             MaukaEngine::addFeed                       ( Cell* cell ) { _feedCells.addFeed(cell); }
  

  void setPlacementStatusRecursivelyToPlaced(Instance* instance);
  bool TestMaukaConstruction(Cell* cell, GCell* gcell);


}  // Mauka namespace.

#endif  // MAUKA_ENGINE_H
