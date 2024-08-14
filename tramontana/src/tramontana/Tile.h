// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                    Christian MASSON             |
// |  First impl.  :                            Yifei WU             |
// |  Second impl. :                    Jean-Paul CHAPUT             |
// |  E-mail       :            Jean-Paul.Chaput@lip6.fr             |
// | =============================================================== |
// |  C++ Header   :  "./tramontana/Tile.h"                          |
// +-----------------------------------------------------------------+


#pragma  once
#include <iostream>
#include <vector>
#include "hurricane/Box.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Component.h"
#include "hurricane/Occurrence.h"
#include "hurricane/IntervalTree.h"
namespace Hurricane {
  class Net;
}


namespace Tramontana {

  using Hurricane::Record;
  using Hurricane::Box;
  using Hurricane::DbU;
  using Hurricane::Layer;
  using Hurricane::BasicLayer;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Component;
  using Hurricane::Occurrence;
  using Hurricane::RbTree;
  using Hurricane::IntervalData;
  using Hurricane::IntervalTree;
  class Equipotential;
  class SweepLine;


// -------------------------------------------------------------------
// Class  :  "Tramontana::Tile".

  class Tile;


  class TileCompare {
    public:
      inline bool  operator() ( const Tile* lhs, const Tile* rhs ) const;
  };


  class Tile {
    public:
      static const uint32_t  NoFlags      =  0;
      static const uint32_t  LeftEdge     = (1<<0);
      static const uint32_t  RightEdge    = (1<<1);
      static const uint32_t  Compress     = (1<<2);
      static const uint32_t  MergeEqui    = (1<<3);
      static const uint32_t  MakeLeafEqui = (1<<4);
      static const uint32_t  ForceLayer   = (1<<5);
      static const uint32_t  OccMerged    = (1<<6);
      static const uint32_t  TopLevel     = (1<<7);
      static const uint32_t  Freed        = (1<<8);
    public:
      typedef std::set<Tile*,TileCompare>  TileSet;
    public:
      static inline const std::vector<Tile*>&  getAllTiles          ();
      static inline const std::vector<size_t>& getFreedIds          ();
      static              void                 showStats            ();
      static              void                 deleteAllTiles       ();
      static              void                 destroyQueued        ();
      static inline       void                 timeTick             ();
      static              Tile*                create               ( Occurrence
                                                                    , const BasicLayer*
                                                                    , Tile* rootTile
                                                                    , SweepLine*
                                                                    , uint32_t flags=NoFlags );
                          void                 check                ( std::string tag ) const;
                          void                 queuedDestroy        ();
                          void                 destroyQueue         ();
             inline       void                 toDeleteQueue        ();
             inline       bool                 isUpToDate           () const;
             inline       bool                 isOccMerged          () const;
             inline       bool                 isTopLevel           () const;
             inline       bool                 isFreed              () const;
             inline       uint32_t             getId                () const;
             inline       uint32_t             getRefCount          () const;
             inline       uint32_t             getRank              () const;
             inline       Tile*                getParent            () const;
                          Tile*                getRoot              ( uint32_t flags=Compress );
             inline       Occurrence           getOccurrence        () const;
             inline       Occurrence           getDeepOccurrence    () const;
             inline       Layer::Mask          getMask              () const;
             inline const BasicLayer*          getLayer             () const;
             inline const Box&                 getBoundingBox       () const;
             inline       Equipotential*       getEquipotential     () const;
             inline       DbU::Unit            getLeftEdge          () const;
             inline       DbU::Unit            getRightEdge         () const;
             inline       DbU::Unit            getYMin              () const;
             inline       DbU::Unit            getYMax              () const;
             inline       uint32_t             getFlags             () const;
             inline       void                 incRank              ();
             inline       void                 incRefCount          ( uint32_t delta=1 );
             inline       void                 decRefCount          ();
             inline       void                 syncTime             ();
             inline       void                 setParent            ( Tile* );
                          Tile*                merge                ( Tile* );
                          bool                 _mergeEqui           ( Tile* );
             inline       void                 setOccMerged         ( bool state );
             inline       void                 setEquipotential     ( Equipotential* );
                          Equipotential*       newEquipotential     ();
                          void                 destroyEquipotential ();
                          Record*              _getRecord           () const;
                          std::string          _getString           () const;
                          std::string          _getTypeName         () const;
    private:
                      Tile      ( Occurrence occ, Occurrence deepOcc, const BasicLayer*, const Box&, Tile* parent );
                     ~Tile      ();
    private:
                      Tile      ( const Tile& ) = delete;
              Tile&   operator= ( const Tile& ) = delete;
    private:
      static       uint32_t             _idCounter;
      static       uint32_t             _time;
      static       std::vector<Tile*>   _allocateds;
      static       std::vector<Tile*>   _destroyQueue;
      static       std::vector<size_t>  _freeds;
                   uint32_t             _id;
                   uint32_t             _refCount;
                   Occurrence           _occurrence;
                   Occurrence           _deepOccurrence;
             const BasicLayer*          _layer;
                   Box                  _boundingBox;
                   Equipotential*       _equipotential;
                   uint32_t             _flags;
                   Tile*                _parent;
                   uint32_t             _rank;
                   uint32_t             _timeStamp;
  };

  inline const std::vector<Tile*>&  Tile::getAllTiles       () { return _allocateds; }
  inline const std::vector<size_t>& Tile::getFreedIds       () { return _freeds; }
  inline       void                 Tile::timeTick          () { _time++; }
  inline       bool                 Tile::isUpToDate        () const { return _timeStamp >= _time; }
  inline       bool                 Tile::isOccMerged       () const { return _flags & OccMerged; }
  inline       bool                 Tile::isTopLevel        () const { return _flags & TopLevel; }
  inline       bool                 Tile::isFreed           () const { return _flags & Freed; }
  inline       uint32_t             Tile::getId             () const { return _id; }
  inline       uint32_t             Tile::getRefCount       () const { return _refCount; }
  inline       Occurrence           Tile::getOccurrence     () const { return _occurrence; }
  inline       Occurrence           Tile::getDeepOccurrence () const { return _deepOccurrence; }
  inline       Layer::Mask          Tile::getMask           () const { return _layer->getMask(); }
  inline const BasicLayer*          Tile::getLayer          () const { return _layer; }
  inline const Box&                 Tile::getBoundingBox    () const { return _boundingBox; }
  inline       Equipotential*       Tile::getEquipotential  () const { return _equipotential; }
  inline       DbU::Unit            Tile::getLeftEdge       () const { return _boundingBox.getXMin(); }
  inline       DbU::Unit            Tile::getRightEdge      () const { return _boundingBox.getXMax(); }
  inline       DbU::Unit            Tile::getYMin           () const { return _boundingBox.getYMin(); }
  inline       DbU::Unit            Tile::getYMax           () const { return _boundingBox.getYMax(); }
  inline       uint32_t             Tile::getFlags          () const { return _flags; }
  inline       uint32_t             Tile::getRank           () const { return _rank; }
  inline       Tile*                Tile::getParent         () const { return _parent; }
  inline       void                 Tile::incRank           () { _rank++; }
  inline       void                 Tile::incRefCount       ( uint32_t count ) { _refCount += count; }
  inline       void                 Tile::syncTime          () { _timeStamp=_time; }
  inline       void                 Tile::setEquipotential  ( Equipotential* equi ) { _equipotential=equi; }

  inline void  Tile::decRefCount ()
  {
    if (_refCount) _refCount--;
    if (not _refCount and isOccMerged()) {
      if (not _parent) {
        if (not getEquipotential()) newEquipotential();
      }
    //check( "Tile::decRefCount()" );
      queuedDestroy();
    }
  }

  inline void  Tile::setParent ( Tile* parent )
  {
    if (isFreed()) return;
    if (parent == _parent) return;
    if (_parent) _parent->decRefCount();
    _parent = parent;
    _parent->incRefCount();
  }

  inline void  Tile::setOccMerged ( bool state )
  {
    if (state) _flags |=  OccMerged;
    else       _flags &= ~OccMerged;
    if (not _refCount) queuedDestroy();
  }


  inline bool  TileCompare::operator() ( const Tile* lhs, const Tile* rhs ) const
  {
    cdebug_log(0,0) << "TileCompare::operator()" << std::endl;
    return lhs->getId() < rhs->getId();
  }


// -------------------------------------------------------------------
// Class  :  "Tramontana::TileIntvTree".


  typedef  IntervalData<Tile*>              TileIntv;
  typedef  IntervalTree<Tile*,TileCompare>  TileIntvTree;


}  // Tramontana namespace.


INSPECTOR_P_SUPPORT(Tramontana::Tile);
INSPECTOR_PR_SUPPORT(Tramontana::TileIntv);
INSPECTOR_PR_SUPPORT(Tramontana::TileIntvTree);
