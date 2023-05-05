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
// |  C++ Module   :  "./QueryTiles.cpp"                             |
// +-----------------------------------------------------------------+


#include <vector>
#include "tramontana/QueryTiles.h"
#include "tramontana/SweepLine.h"


namespace Tramontana {

  using std::cerr;
  using std::endl;
  using std::vector;


  QueryTiles::QueryTiles ( SweepLine* sweepLine )
    : Query           ()
    , _sweepLine      (sweepLine)
    , _goMatchCount   (0)
    , _processedLayers(0)
  {
    setCell  ( sweepLine->getCell() );
    setArea  ( sweepLine->getCell()->getBoundingBox() );
    setFilter( Query::DoComponents|Query::DoTerminalCells );
  }


  void  QueryTiles::setBasicLayer ( const BasicLayer* basicLayer )
  {
    _processedLayers |= basicLayer->getMask();
    Query::setBasicLayer ( basicLayer );
  }


  bool  QueryTiles::isProcessed ( Component* component ) const
  {
    Layer::Mask fullyProcesseds = _processedLayers & ~getBasicLayer()->getMask();
    return component->getLayer()->getMask().intersect( fullyProcesseds );
  }


  void  QueryTiles::masterCellCallback ()
  { }


  void  QueryTiles::rubberCallback ( Rubber* )
  { }


  void  QueryTiles::extensionGoCallback ( Go* )
  { }


  bool  QueryTiles::hasGoCallback () const
  { return true; }


  void  QueryTiles::goCallback ( Go* go )
  {
    Tile*      rootTile  = nullptr;
    Component* component = dynamic_cast<Component*>( go );
    if (not component) return;
    if (isProcessed(component)) return;
    Occurrence occurrence = Occurrence( go, getPath() );
    for ( const BasicLayer* layer : _sweepLine->getExtracteds() ) {
      if (not component->getLayer()->getMask().intersect(layer->getMask())) continue;
      Tile* tile = Tile::create( occurrence
                               , layer
                               , rootTile
                               , _sweepLine );
      if (not rootTile) rootTile = tile;
    }

    BasicLayer* cutLayer = component->getLayer()->getBasicLayers().getFirst();
    if (cutLayer->getMaterial() == BasicLayer::Material::cut) {
      const SweepLine::LayerSet& connexSet = _sweepLine->getCutConnexLayers( cutLayer );
      for ( const BasicLayer* connexLayer : connexSet ) {
        Tile::create( occurrence
                    , connexLayer
                    , rootTile
                    , _sweepLine
                    , Tile::ForceLayer );
      }
    }
    
    _goMatchCount++;
  }


}  // Tramontana namespace.
