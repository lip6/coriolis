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


// -------------------------------------------------------------------
// Class  :  "Tramontana::Tile".

  class Tile {
    public:
      static const uint32_t  NoFlags   =  0;
      static const uint32_t  LeftEdge  = (1<<0);
      static const uint32_t  RightEdge = (1<<1);
      static const uint32_t  Compress  = (1<<2);
      static const uint32_t  MergeEqui = (1<<3);
    public:
      static inline const std::vector<Tile*>  getAllTiles      ();
      static inline       void                timeTick         ();
      static              Tile*               create           ( Occurrence, const BasicLayer* );
                          void                destroy          ();
             inline       bool                isUpToDate       () const;
             inline       unsigned int        getId            () const;
             inline       uint32_t            getRank          () const;
             inline       Tile*               getParent        () const;
                          Tile*               getRoot          ( uint32_t flags=Compress );
           //inline       Component*          getComponent     () const;
             inline       Occurrence          getOccurrence    () const;
           //             Net*                getNet           () const;
             inline       Layer::Mask         getMask          () const;
             inline const BasicLayer*         getLayer         () const;
             inline const Box&                getBoundingBox   () const;
             inline       Equipotential*      getEquipotential () const;
             inline       DbU::Unit           getLeftEdge      () const;
             inline       DbU::Unit           getRightEdge     () const;
             inline       DbU::Unit           getYMin          () const;
             inline       DbU::Unit           getYMax          () const;
             inline       uint32_t            getFlags         () const;
             inline       void                incRank          ();
             inline       void                syncTime         ();
             inline       void                setParent        ( Tile* );
                          Tile*               merge            ( Tile* );
             inline       void                setEquipotential ( Equipotential* );
                          Equipotential*      newEquipotential ();
                          Record*             _getRecord       () const;
                          std::string         _getString       () const;
                          std::string         _getTypeName     () const;
    private:
                      Tile      ( Occurrence, const BasicLayer*, const Box& );
                     ~Tile      ();
    private:
                      Tile      ( const Tile& ) = delete;
              Tile&   operator= ( const Tile& ) = delete;
    private:
      static       uint32_t           _idCounter;
      static       uint32_t           _time;
      static       std::vector<Tile*> _allocateds;
                   uint32_t           _id;
                   Occurrence         _occurrence;
             const BasicLayer*        _layer;
                   Box                _boundingBox;
                   Equipotential*     _equipotential;
                   uint32_t           _flags;
                   Tile*              _parent;
                   uint32_t           _rank;
                   uint32_t           _timeStamp;
  };

  inline const std::vector<Tile*>  Tile::getAllTiles      () { return _allocateds; }
  inline       void                Tile::timeTick         () { _time++; }
  inline       bool                Tile::isUpToDate       () const { return _timeStamp >= _time; }
  inline       unsigned int        Tile::getId            () const { return _id; }
//inline       Component*          Tile::getComponent     () const { return dynamic_cast<Component*>( _occurrence.getEntity() ); }
  inline       Occurrence          Tile::getOccurrence    () const { return _occurrence; }
  inline       Layer::Mask         Tile::getMask          () const { return _layer->getMask(); }
  inline const BasicLayer*         Tile::getLayer         () const { return _layer; }
  inline const Box&                Tile::getBoundingBox   () const { return _boundingBox; }
  inline       Equipotential*      Tile::getEquipotential () const { return _equipotential; }
  inline       DbU::Unit           Tile::getLeftEdge      () const { return _boundingBox.getXMin(); }
  inline       DbU::Unit           Tile::getRightEdge     () const { return _boundingBox.getXMax(); }
  inline       DbU::Unit           Tile::getYMin          () const { return _boundingBox.getYMin(); }
  inline       DbU::Unit           Tile::getYMax          () const { return _boundingBox.getYMax(); }
  inline       uint32_t            Tile::getFlags         () const { return _flags; }
  inline       uint32_t            Tile::getRank          () const { return _rank; }
  inline       Tile*               Tile::getParent        () const { return _parent; }
  inline       void                Tile::incRank          () { _rank++; }
  inline       void                Tile::syncTime         () { _timeStamp=_time; }
  inline       void                Tile::setParent        ( Tile* parent ) { _parent=parent; }
  inline       void                Tile::setEquipotential ( Equipotential* equi ) { _equipotential=equi; }


  class TileCompare {
    public:
      inline bool  operator() ( const Tile* lhs, const Tile* rhs ) const
      {
        cdebug_log(0,0) << "TileCompare::operator()" << std::endl;
        return lhs->getId() < rhs->getId();
      }
  };


// -------------------------------------------------------------------
// Class  :  "Tramontana::TileIntvTree".


  typedef  IntervalData<Tile*>              TileIntv;
  typedef  IntervalTree<Tile*,TileCompare>  TileIntvTree;


}  // Tramontana namespace.


INSPECTOR_P_SUPPORT(Tramontana::Tile);
INSPECTOR_PR_SUPPORT(Tramontana::TileIntv);
INSPECTOR_PR_SUPPORT(Tramontana::TileIntvTree);
