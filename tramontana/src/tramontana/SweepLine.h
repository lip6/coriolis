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
// |  C++ Header   :  "./tramontana/SweepLine.h"                     |
// +-----------------------------------------------------------------+


#pragma  once
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "hurricane/BasicLayer.h"
namespace Hurricane {
  class Net;
}
#include "tramontana/Tile.h"
#include "tramontana/TramontanaEngine.h"


namespace Tramontana {

  using Hurricane::Record;
  using Hurricane::Box;
  using Hurricane::DbU;
  using Hurricane::DBo;
  using Hurricane::Cell;
  using Hurricane::Layer;
  using Hurricane::BasicLayer;


// -------------------------------------------------------------------
// Class  :  "Tramontana::SweepLine".

  class SweepLine {
    private:
      typedef  std::map<Layer::Mask, TileIntvTree>  IntervalTrees;
    private:
      class Element {
        public:
          inline              Element    ( Tile*, uint32_t flags );
          inline bool         operator<  ( const Element& ) const;
          inline bool         operator== ( const Element& ) const;
          inline bool         isLeftEdge () const;
          inline Tile*        getTile    () const;
          inline DbU::Unit    getX       () const;
          inline DbU::Unit    getY       () const;
          inline DbU::Unit    getId      () const;
          inline Layer::Mask  getMask    () const;
        private:
          Tile*    _tile;
          uint32_t _flags;
      };
    public:
                                SweepLine           ( TramontanaEngine* );
                               ~SweepLine           ();
      inline  Cell*             getCell             ();
      inline  const std::vector<const BasicLayer*>&
                                getExtracteds       () const;
      inline  Layer::Mask       getExtractedMask    () const;
      inline  const TramontanaEngine::LayerSet&
                                getCutConnexLayers  ( const BasicLayer* ) const;
              void              run                 ();
              void              loadTiles           ();
              void              deleteTiles         ();
      inline  void              add                 ( Tile* );
              void              mergeEquipotentials ();
              Record*           _getRecord          () const;
              std::string       _getString          () const;
              std::string       _getTypeName        () const;
    private:                                        
                                SweepLine           ( const SweepLine& ) = delete;
              SweepLine&        operator=           ( const SweepLine& ) = delete;
    private:
      TramontanaEngine*               _tramontana;
      std::vector<Element>            _tiles;
      IntervalTrees                   _intervalTrees;
  };


// SweepLine::Element.  
  inline              SweepLine::Element::Element    ( Tile* tile, uint32_t flags ) : _tile(tile), _flags(flags) { }
  inline bool         SweepLine::Element::isLeftEdge () const { return _flags & Tile::LeftEdge; }
  inline Tile*        SweepLine::Element::getTile    () const { return _tile; }
  inline DbU::Unit    SweepLine::Element::getX       () const { return isLeftEdge() ? _tile->getLeftEdge() : _tile->getRightEdge(); } 
  inline DbU::Unit    SweepLine::Element::getY       () const { return _tile->getBoundingBox().getYMin(); }
  inline DbU::Unit    SweepLine::Element::getId      () const { return _tile->getId(); }
  inline Layer::Mask  SweepLine::Element::getMask    () const { return _tile->getMask(); }

  inline bool  SweepLine::Element::operator< ( const Element& rhs ) const
  {
    if (getX() != rhs.getX()) return (getX() < rhs.getX());
    if (isLeftEdge() xor rhs.isLeftEdge()) return isLeftEdge();
    if (getY() != rhs.getY()) return (getY() < rhs.getY());
    if (getMask() != rhs.getMask()) return (getMask() < rhs.getMask());
    return TileCompare() ( getTile(), rhs.getTile() );
  }


  inline bool  SweepLine::Element::operator== ( const Element& rhs ) const
  { return getTile() == rhs.getTile(); }

  
// SweepLine.  
  inline        Cell*                           SweepLine::getCell            () { return _tramontana->getCell(); }
  inline        Layer::Mask                     SweepLine::getExtractedMask   () const { return _tramontana->getExtractedMask(); }
  inline  const std::vector<const BasicLayer*>& SweepLine::getExtracteds      () const { return _tramontana->getExtracteds(); }

  inline  const TramontanaEngine::LayerSet& SweepLine::getCutConnexLayers ( const BasicLayer* cutLayer ) const
  { return _tramontana->getCutConnexLayers( cutLayer ); }

  inline  void  SweepLine::add ( Tile* tile )
  {
    tile->incRefCount( 2 );
    _tiles.push_back( Element( tile, Tile::LeftEdge ) );
    _tiles.push_back( Element( tile, Tile::RightEdge ) );
  }


  
}  // Tramontana namespace.


INSPECTOR_P_SUPPORT(Tramontana::SweepLine);
