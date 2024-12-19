// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                   Christian MASSON              |
// |  First impl.  :                           Yifei WU              |
// |  Second impl. :                   Jean-Paul CHAPUT              |
// |  E-mail       :           Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module   :  "./SweepLine.cpp"                              |
// +-----------------------------------------------------------------+


#include <iomanip>
#include "hurricane/utilities/Path.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Timer.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Layer.h"
#include "hurricane/ViaLayer.h"
#include "hurricane/Net.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/RoutingPad.h"
#include "crlcore/Utilities.h"
#include "crlcore/ToolBox.h"
#include "tramontana/SweepLine.h"
#include "tramontana/QueryTiles.h"


namespace Tramontana {

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::dec;
  using std::setw;
  using std::setfill;
  using std::left;
  using std::right;
  using std::string;
  using std::ostream;
  using std::ofstream;
  using std::ostringstream;
  using std::setprecision;
  using std::vector;
  using std::make_pair;
  using Hurricane::dbo_ptr;
  using Hurricane::UpdateSession;
  using Hurricane::DebugSession;
  using Hurricane::tab;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Interval;
  using Hurricane::Box;
  using Hurricane::DataBase;
  using Hurricane::Technology;
  using Hurricane::Layer;
  using Hurricane::ViaLayer;
  using Hurricane::Entity;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Hurricane::Cell;
  using Hurricane::Instance;


// -------------------------------------------------------------------
// Class  :  "Tramontana::SweepLine".


  SweepLine::SweepLine ( TramontanaEngine* tramontana  )
    : _tramontana   (tramontana) 
    , _tiles        ()
    , _intervalTrees()
    , _slidingWindow()
    , _lastLeftEdge (nullptr)
    , _splitCount   (0)
    , _flags        (0)
  {
    for ( const BasicLayer* layer : getExtracteds() ) {
      _intervalTrees.insert( make_pair( layer->getMask(), TileIntvTree() ));
    }
    size_t instancesCount = CRL::getInstancesCount( getCell() );
    _splitCount = instancesCount / tramontana->getInstancesPerWindows();
  }


  SweepLine::~SweepLine ()
  { }
  

  void  SweepLine::run ( bool isTopLevel )
  {
    UpdateSession::open();
    // if (getCell()->getName() == "arlet6502_cts_r")
    //   DebugSession::open( 160, 169 );
    cdebug_log(160,1) << "SweepLine::run()" << endl;
    Box       ab         = getCell()->getBoundingBox();
    Interval  sweepSpan  = Interval( ab.getXMin(), ab.getXMax() );
    size_t    processeds = 0;
    DbU::Unit xSweepLine = sweepSpan.getVMin();
    
    loadNextWindow();
    do {
      Tile::timeTick();
      for ( ; processeds<_tiles.size() ; ++processeds ) {
        Tile*     tile     = _tiles[processeds].getTile();
        TileIntv  tileIntv ( tile, tile->getYMin(), tile->getYMax() );
      
        if (_tiles[processeds].isLeftEdge()) {
          if (tile == _lastLeftEdge) {
            cdebug_log(160,0) << "Window limit tile marker: " << tile << endl;
            break;
          }
          
          if (tile->getLeftEdge() != xSweepLine) {
            xSweepLine = tile->getLeftEdge();
            if (tty::enabled()) {
              DbU::Unit progress = ((xSweepLine - sweepSpan.getVMin()) * 100) / sweepSpan.getSize();
              cmess2 << "        <SweepLine @" << tty::bold
                     << right << setw(12) << DbU::getValueString(xSweepLine,DbU::Physical) << " | "
                     << right << setw( 3) << progress << "%"
                     << tty::reset
                     << setfill(' ') << tty::reset << ">" << tty::cr;
              cmess2.flush ();
            }
          }
        }
        
        cdebug_log(160,1) << "X@ + " << DbU::getValueString(_tiles[processeds].getX()) << " " << tile << endl;
        auto  intvTree = _intervalTrees.find( _tiles[processeds].getMask() );
        if (intvTree == _intervalTrees.end()) {
          cerr << Error( "SweepLine::run(): Missing interval tree for layer(mask) %s."
                         "        (for tile: %s)"
                       , getString(_tiles[processeds].getMask()).c_str()
                       , getString(_tiles[processeds].getTile()).c_str()
                       ) << endl;
          cdebug_tabw(160,-1);
          continue;
        }
        if (_tiles[processeds].isLeftEdge()) {
          // if (tile->getId() == 3) {
          //   cdebug_log(160,0) << " Dumping tree:" << endl;
          //   for ( const TileIntv& treeTile : intvTree->second ) {
          //     cdebug_log(160,0) << " | " << treeTile << endl;
          //   }
          //   cdebug_log(160,0) << " Writing tree:" << endl;
          //   intvTree->second.write( "debug-tree.dot" );
          //   cdebug_log(160,0) << " Done" << endl;
          //   DebugSession::open( 0, 169 );
          // }
          for ( const TileIntv& overlap : intvTree->second.getOverlaps(
                                             Interval(tile->getYMin(), tile->getYMax() ))) {
            cdebug_log(160,0) << " | intersect " << overlap.getData() << endl;
            tile->merge( overlap.getData() );
          }
          // if (tile->getId() == 3) {
          //   DebugSession::close();
          // }
          cdebug_log(160,0) << " | insert tile" << endl;
          intvTree->second.insert( tileIntv );
        } else {
          cdebug_log(160,0) << " | remove tile" << endl;
          intvTree->second.remove( tileIntv );
        }
        tile->decRefCount();
        cdebug_tabw(160,-1);
      }

      cdebug_log(160,0) << "Flushing window, reached @" << DbU::getValueString(xSweepLine) << endl;
      Tile::timeTick();
      for ( size_t i=0 ; i<processeds ; ++i ) {
        Tile* tile = _tiles[i].getTile();
        if (tile->isFreed()) continue;
        tile->getRoot( Tile::Compress|Tile::MergeEqui );
      }
      _tiles.erase( _tiles.begin(), _tiles.begin() + processeds );
      Tile::destroyQueued();
      cdebug_log(160,0) << "  -> Freeds " << Tile::getFreedIds().size() << endl;
      processeds = 0;
      loadNextWindow();
    } while ( processeds < _tiles.size() );
    
    if (tty::enabled()) cmess2 << endl;
    cdebug_tabw(160,-1);
    mergeEquipotentials( Tile::MakeLeafEqui );
    if (isTopLevel) printSummary();
    Tile::deleteAllTiles();
    // if (getCell()->getName() == "arlet6502_cts_r")
    //   DebugSession::close();
    UpdateSession::close();
  }


  bool  SweepLine::loadNextWindow ()
  {
    cdebug_log(160,1) << "SweepLine::loadNextWindow()" << endl;

    size_t    tilesCount = Tile::activeTilesCount();
    Box       bb         = getCell()->getBoundingBox();
    DbU::Unit sliceWidth = bb.getWidth() / (_splitCount + 1);
    _lastLeftEdge = nullptr;
    if (_slidingWindow.isEmpty()) {
      _flags |= IsLeftMostWindow;
      _slidingWindow = Box( bb.getXMin(), bb.getYMin(), bb.getXMin()+sliceWidth, bb.getYMax() );
    } else {
      _flags &= ~IsLeftMostWindow;
      _slidingWindow.translate( sliceWidth, 0 );
    }
    if (_slidingWindow.getXMax() > bb.getXMax()) {
      _flags |= IsRightMostWindow;
    }

    cdebug_log(160,0) << "Area window: " << _slidingWindow << endl;
    QueryTiles::doAreaQuery( this, _slidingWindow );
    sort( _tiles.begin(), _tiles.end() );
    if (_tiles.size() > 1) {
      size_t itile = _tiles.size();
      while ( itile > 1 ) {
        if (_tiles[--itile].isLeftEdge()) {
          _lastLeftEdge = _tiles[itile].getTile();
          cdebug_log(160,0) << "_lastLeftEdge: " << _lastLeftEdge << endl;
          break;
        }
      }
    }

    cdebug_tabw(160,-1);
    return true;
  }


  void  SweepLine::mergeEquipotentials ( uint32_t flags )
  {
    cout.flush();
    cerr.flush();
  //DebugSession::open( 160, 169 );
    cdebug_log(160,1) << "SweepLine::mergeEquipotentials()" << endl;
    Tile::timeTick();
    const vector<Tile*>& tiles = Tile::getAllTiles();
    for ( size_t i=0 ; i<tiles.size() ; ++i ) {
      if (not tiles[i]) continue;
      tiles[i]->getRoot( Tile::Compress|Tile::MergeEqui|flags );
      Tile::destroyQueued();
    }
    cdebug_tabw(160,-1);
  //Tile::showStats();
  //DebugSession::close();
  }


  void  SweepLine::printSummary () const
  {
    cmess2 << Dots::asUInt("        - Windows"    , _splitCount+1          ) << endl;
    cmess2 << Dots::asUInt("        - Peak tiles" , Tile::peakTilesCount ()) << endl;
    cmess2 << Dots::asUInt("        - Total tiles", Tile::totalTilesCount()) << endl;
  }


  string  SweepLine::_getTypeName () const
  { return "Tramontana::SweepLine"; }


  string  SweepLine::_getString () const
  {
    ostringstream  os;
    os << "<SweepLine \"" << _tramontana->getCell()->getName() << "\">";
    return os.str();
  }


  Record* SweepLine::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if (record) {
      record->add( getSlot( "_tramontana" , &_tramontana ) );
      record->add( getSlot( "_tiles"      , &_tiles      ) );
    }
    return record;
  }


} // Tramontana namespace.
