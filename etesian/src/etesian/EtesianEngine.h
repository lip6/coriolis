// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2014-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./etesian/EtesianEngine.h"                     |
// +-----------------------------------------------------------------+


#pragma once
#include <tuple>
#include <iostream>
#include <unordered_map>
#include "coloquinte/coloquinte.hpp"

#include "hurricane/Timer.h"
#include "hurricane/Name.h"
namespace Hurricane {
  class Layer;
  class Net;
  class RoutingPad;
  class Cell;
  class CellWidget;
  class CellViewer;
  class Instance;
}

#include "crlcore/ToolEngine.h"
#include "etesian/Configuration.h"
#include "etesian/FeedCells.h"
#include "etesian/BufferCells.h"
#include "etesian/BloatCells.h"
#include "etesian/Placement.h"


namespace Etesian {

  using Hurricane::Timer;
  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::DBo;
  using Hurricane::Net;
  using Hurricane::RoutingPad;
  using Hurricane::Cell;
  using Hurricane::Record;
  using Hurricane::Instance;
  using Hurricane::Point;
  using Hurricane::Path;
  using Hurricane::Transformation;


// -------------------------------------------------------------------
// Class  :  "Etesian::EtesianEngine".

  class EtesianEngine : public CRL::ToolEngine {
    public:
      static  const uint32_t  NeedsDiode = (1<<0);
      static  const uint32_t  FinalStage = (1<<1);
      static  const uint32_t  RightSide  = (1<<2);
      static  const uint32_t  LeftSide   = (1<<3);
    public:
      typedef ToolEngine  Super;
      typedef std::tuple<Net*,int32_t,uint32_t>                NetInfos;
      typedef std::tuple<Instance*, std::vector<RoutingPad*> > InstanceInfos;
      typedef std::map<Instance*,size_t,DBo::CompareById>      InstancesToIds;
      typedef std::set<std::string>                            NetNameSet;
    public:
      static  const Name&             staticGetName             ();
      static  EtesianEngine*          create                    ( Cell* );
      static  EtesianEngine*          get                       ( const Cell* );
    public:                                                     
      inline  bool                    isExcluded                ( std::string ) const;
      virtual Configuration*          getConfiguration          ();
      virtual const Configuration*    getConfiguration          () const;
      virtual const Name&             getName                   () const;
      inline  RoutingGauge*           getGauge                  () const;
      inline  CellGauge*              getCellGauge              () const;
      inline  DbU::Unit               getHorizontalPitch        () const;
      inline  DbU::Unit               getVerticalPitch          () const;
      inline  DbU::Unit               getSliceHeight            () const;
      inline  DbU::Unit               getSliceStep              () const;
      inline  DbU::Unit               getFixedAbHeight          () const;
      inline  DbU::Unit               getFixedAbWidth           () const;
      inline  Effort                  getPlaceEffort            () const;
      inline  GraphicUpdate           getUpdateConf             () const;
      inline  double                  getSpaceMargin            () const;
      inline  double                  getDensityVariation       () const;
      inline  double                  getAspectRatio            () const;
      inline  DbU::Unit               getAntennaGateMaxWL       () const;
      inline  DbU::Unit               getAntennaDiodeMaxWL      () const;
      inline  DbU::Unit               getLatchUpDistance        () const;
      inline  const FeedCells&        getFeedCells              () const;
      inline  const BufferCells&      getBufferCells            () const;
      inline  Cell*                   getDiodeCell              () const;
              std::string             getUniqueDiodeName        ();
      inline  const Box&              getPlaceArea              () const;
      inline  Area*                   getArea                   () const;
      inline  Hurricane::CellViewer*  getViewer                 () const;
      inline  void                    setViewer                 ( Hurricane::CellViewer* );
      inline  Cell*                   getBlockCell              () const;
      inline  Instance*               getBlockInstance          () const;
      inline  const NetNameSet&       getExcludedNets           () const;
      inline  const std::vector<Box>& getTrackAvoids            () const;
      inline  void                    setBlock                  ( Instance* );
      inline  void                    setFixedAbHeight          ( DbU::Unit );
      inline  void                    setFixedAbWidth           ( DbU::Unit );
      inline  void                    setSpaceMargin            ( double );
      inline  void                    setDensityVariation       ( double );
      inline  void                    setAspectRatio            ( double );
              void                    setDefaultAb              ();
              void                    adjustSliceHeight         ();
              void                    resetPlacement            ();
              void                    clearColoquinte           ();
              void                    loadLeafCellLayouts       ();
      inline  DbU::Unit               toDbU                     ( int64_t ) const;
      inline  Occurrence              toCell                    ( Occurrence ) const;
      inline  Point                   toCell                    ( const Point& ) const;
      inline  Box                     toCell                    ( const Box& ) const;
      inline  Transformation          toCell                    ( Transformation ) const;
      inline  Path                    toBlock                   ( Path ) const;
      inline  Occurrence              toBlock                   ( Occurrence ) const;
      inline  Point                   toBlock                   ( const Point& ) const;
      inline  Transformation          toBlock                   ( const Transformation& ) const;
              void                    setPlaceArea              ( const Box& );
              size_t                  toColoquinte              ();
              void                    globalPlace               ();
              void                    detailedPlace             ();
              void                    place                     ();
              uint32_t                doHFNS                    ();
      inline  void                    useFeed                   ( Cell* );
              size_t                  findYSpin                 ();
      inline  void                    exclude                   ( string netName );
              void                    addFeeds                  ();
              void                    toHurricane               ();
              void                    flattenPower              ();
      inline  void                    selectBloat               ( std::string );
      inline  void                    addTrackAvoid             ( const Box& );
      virtual Record*                 _getRecord                () const;
      virtual std::string             _getString                () const;
      virtual std::string             _getTypeName              () const;
    private:
    // Attributes.
      static Name                                 _toolName;
    protected:
             Configuration*                       _configuration;
             Instance*                            _block;
             bool                                 _placed;
             bool                                 _ySpinSet;
             bool                                 _flatDesign;
             Box                                  _placeArea;
             std::vector<Box>                     _trackAvoids;
             coloquinte::Rectangle*               _surface;
             coloquinte::Circuit*                 _circuit;
             coloquinte::PlacementSolution*       _placementLB;
             coloquinte::PlacementSolution*        _placementUB;
             InstancesToIds                       _instsToIds;
             std::vector<InstanceInfos>           _idsToInsts;
             Hurricane::CellViewer*               _viewer;
             Cell*                                _diodeCell;
             FeedCells                            _feedCells;
             BufferCells                          _bufferCells;
             BloatCells                           _bloatCells;
             Area*                                _area;
             size_t                               _yspinSlice0;
             DbU::Unit                            _sliceHeight;
             DbU::Unit                            _fixedAbHeight;
             DbU::Unit                            _fixedAbWidth;
             uint32_t                             _diodeCount;
             uint32_t                             _bufferCount;
             NetNameSet                           _excludedNets;

    protected:
    // Constructors & Destructors.
                             EtesianEngine    ( Cell* );
      virtual               ~EtesianEngine    ();
      virtual void           _postCreate      ();
      virtual void           _preDestroy      ();
    private:                                  
                             EtesianEngine    ( const EtesianEngine& );
              EtesianEngine& operator=        ( const EtesianEngine& );
    private:
      inline  uint32_t       _getNewDiodeId   ();
              Instance*      _createDiode     ( Cell* );
              void           _updatePlacement ( const coloquinte::PlacementSolution* );
              void           _coloquinteCallback(coloquinte::PlacementStep step);
              void           _checkNotAFeed   ( Occurrence occurrence ) const;
  };


// Inline Functions.
  inline  void                   EtesianEngine::setViewer                 ( Hurricane::CellViewer* viewer ) { _viewer = viewer; }
  inline  Hurricane::CellViewer* EtesianEngine::getViewer                 () const { return _viewer; }
  inline  RoutingGauge*          EtesianEngine::getGauge                  () const { return getConfiguration()->getGauge(); }
  inline  CellGauge*             EtesianEngine::getCellGauge              () const { return getConfiguration()->getCellGauge(); }
  inline  DbU::Unit              EtesianEngine::getHorizontalPitch        () const { return getGauge()->getHorizontalPitch(); }
  inline  DbU::Unit              EtesianEngine::getVerticalPitch          () const { return getGauge()->getVerticalPitch(); }
  inline  DbU::Unit              EtesianEngine::getSliceHeight            () const { return _sliceHeight; }
  inline  DbU::Unit              EtesianEngine::getSliceStep              () const { return getCellGauge()->getSliceStep(); }
  inline  DbU::Unit              EtesianEngine::getFixedAbHeight          () const { return _fixedAbHeight; }
  inline  DbU::Unit              EtesianEngine::getFixedAbWidth           () const { return _fixedAbWidth; }
  inline  Effort                 EtesianEngine::getPlaceEffort            () const { return getConfiguration()->getPlaceEffort(); }
  inline  GraphicUpdate          EtesianEngine::getUpdateConf             () const { return getConfiguration()->getUpdateConf(); }
  inline  double                 EtesianEngine::getSpaceMargin            () const { return getConfiguration()->getSpaceMargin(); }
  inline  double                 EtesianEngine::getDensityVariation       () const { return getConfiguration()->getDensityVariation(); }
  inline  double                 EtesianEngine::getAspectRatio            () const { return getConfiguration()->getAspectRatio(); }
  inline  DbU::Unit              EtesianEngine::getAntennaGateMaxWL       () const { return getConfiguration()->getAntennaGateMaxWL(); }
  inline  DbU::Unit              EtesianEngine::getAntennaDiodeMaxWL      () const { return getConfiguration()->getAntennaDiodeMaxWL(); }
  inline  DbU::Unit              EtesianEngine::getLatchUpDistance        () const { return getConfiguration()->getLatchUpDistance(); }
  inline  void                   EtesianEngine::useFeed                   ( Cell* cell ) { _feedCells.useFeed(cell); }
  inline  const FeedCells&       EtesianEngine::getFeedCells              () const { return _feedCells; }
  inline  const BufferCells&     EtesianEngine::getBufferCells            () const { return _bufferCells; }
  inline  Cell*                  EtesianEngine::getDiodeCell              () const { return _diodeCell; }
  inline  void                   EtesianEngine::selectBloat               ( std::string profile ) { _bloatCells.select(profile); }
                                                                          
  inline  Cell*                  EtesianEngine::getBlockCell              () const { return (_block) ? _block->getMasterCell() : getCell(); }
  inline  Instance*              EtesianEngine::getBlockInstance          () const { return  _block; }
  inline  void                   EtesianEngine::setFixedAbHeight          ( DbU::Unit abHeight ) { _fixedAbHeight = abHeight; }
  inline  void                   EtesianEngine::setFixedAbWidth           ( DbU::Unit abWidth  ) { _fixedAbWidth  = abWidth; }
  inline  void                   EtesianEngine::setSpaceMargin            ( double margin ) { getConfiguration()->setSpaceMargin(margin); }
  inline  void                   EtesianEngine::setDensityVariation       ( double margin ) { getConfiguration()->setDensityVariation(margin); }
  inline  void                   EtesianEngine::setAspectRatio            ( double ratio  ) { getConfiguration()->setAspectRatio(ratio); }
  inline  DbU::Unit              EtesianEngine::toDbU                     ( int64_t v ) const { return v*getSliceStep(); }
  inline  uint32_t               EtesianEngine::_getNewDiodeId            () { return _diodeCount++; }
  inline  const Box&             EtesianEngine::getPlaceArea              () const { return _placeArea; }
  inline  Area*                  EtesianEngine::getArea                   () const { return _area; }
  inline  const EtesianEngine::NetNameSet&
                                 EtesianEngine::getExcludedNets           () const { return _excludedNets; }
  inline  const std::vector<Box>&
                                 EtesianEngine::getTrackAvoids            () const { return _trackAvoids; }

  inline  void  EtesianEngine::addTrackAvoid ( const Box& box )
  {
    cdebug_log(121,0) << "EtesianEngine::trackAvoid() over " << box << std::endl;
    _trackAvoids.push_back( box );
  }

  inline  void  EtesianEngine::setBlock ( Instance* block )
  {
    _block = block;
    _placed = _block->getMasterCell()->isPlaced();
    _placeArea = _block->getMasterCell()->getAbutmentBox();
  }

  inline  Occurrence  EtesianEngine::toCell ( Occurrence blockOccurrence ) const
  {
    if (not _block) return blockOccurrence;
    if (blockOccurrence.getOwnerCell() != getBlockCell()) {
      std::cerr << Error( "EtesianEngine::toCell(Occurrence): %s"
                          "\n        Is *not* rooted to the block %s but to %s."
                        , getString(blockOccurrence).c_str()
                        , getString(getBlockCell()).c_str()
                        , getString(blockOccurrence.getOwnerCell()).c_str()
                        ) << std::endl;
      return blockOccurrence;
    }
    return Occurrence( blockOccurrence.getEntity(), Path(_block,blockOccurrence.getPath()) );
  }


  inline  Point  EtesianEngine::toCell ( const Point& blockPoint ) const
  {
    if (not _block) return blockPoint;
    Point cellPoint = blockPoint;
    _block->getTransformation().applyOn( cellPoint );
    return cellPoint;
  }


  inline  Box  EtesianEngine::toCell ( const Box& blockBox ) const
  {
    if (not _block) return blockBox;
    Box cellBox = blockBox;
    _block->getTransformation().applyOn( cellBox );
    return cellBox;
  }


  inline  Transformation  EtesianEngine::toCell ( Transformation blockTransf ) const
  {
    if (not _block) return blockTransf;
    return _block->getTransformation().getTransformation( blockTransf );
  }


  inline  Path  EtesianEngine::toBlock ( Path cellPath ) const
  {
    if (not _block) return cellPath;
    if (cellPath.getHeadInstance() != getBlockInstance()) {
      std::cerr << Error( "EtesianEngine::toBlock(Path): %s"
                          "\n        Do *not* go through the block %s but through %s."
                        , getString(cellPath).c_str()
                        , getString(getBlockInstance()).c_str()
                        , getString(cellPath.getHeadInstance()).c_str()
                        ) << std::endl;
      return cellPath;
    }
    return cellPath.getTailPath();
  }


  inline  Occurrence  EtesianEngine::toBlock ( Occurrence cellOccurrence ) const
  {
    if (not _block) return cellOccurrence;
    return Occurrence( cellOccurrence.getEntity(), toBlock(cellOccurrence.getPath()) );
  }


  inline  Point  EtesianEngine::toBlock ( const Point& cellPoint ) const
  {
    if (not _block) return cellPoint;
    Point blockPoint = cellPoint;
    Transformation blockTransf = _block->getTransformation();
    blockTransf.invert().applyOn( blockPoint );
    return blockPoint;
  }


  inline  Transformation  EtesianEngine::toBlock ( const Transformation& cellTransf ) const
  {
    if (not _block) return cellTransf;
    Transformation blockTransf = _block->getTransformation();
    return blockTransf.invert().getTransformation( cellTransf );
  }


  inline  bool  EtesianEngine::isExcluded ( string netName ) const
  { return (_excludedNets.find(netName) != _excludedNets.end()); }


  inline  void  EtesianEngine::exclude ( string netName )
  {
    // Net* net = getCell()->getNet( netName );
    // if (not net) {
    //   std::cerr << Error( "EtesianEngine::exclude(Net*): %s has no net named \"%s\"."
    //                     , getString(getCell()).c_str()
    //                     , netName.c_str()
    //                     ) << std::endl;
    //   return;
    // }
    if (isExcluded(netName)) return;
    _excludedNets.insert( netName );
  }

// Variables.
  extern const char* missingEtesian;


}  // Etesian namespace.


INSPECTOR_P_SUPPORT(Etesian::EtesianEngine);
