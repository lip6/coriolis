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
    : _tramontana    (tramontana) 
    , _extracteds    ()
    , _extractedsMask()
    , _connexityMap  ()
    , _tiles         ()
    , _intervalTrees ()
  {
    for ( const BasicLayer* bl : DataBase::getDB()->getTechnology()->getBasicLayers() ) {
    // HARDCODED. Should read the gauge.
      if (getString(bl->getName()).substr(0,6) == "gmetal") continue;
      if (  (bl->getMaterial() == BasicLayer::Material::metal)
         or (bl->getMaterial() == BasicLayer::Material::poly)
         or (bl->getMaterial() == BasicLayer::Material::cut)) {
        _extracteds.push_back( bl );
        _extractedsMask |= bl->getMask();
      }
    }
    _buildCutConnexMap();
    
    // _extracteds.push_back( DataBase::getDB()->getTechnology()->getBasicLayer( "metal5" ));
    // _extracteds.push_back( DataBase::getDB()->getTechnology()->getBasicLayer( "metal4" ));
    // _extracteds.push_back( DataBase::getDB()->getTechnology()->getBasicLayer( "metal3" ));
    // _extracteds.push_back( DataBase::getDB()->getTechnology()->getBasicLayer( "metal2" ));
    // _extracteds.push_back( DataBase::getDB()->getTechnology()->getBasicLayer( "metal1" ));
    // _extracteds.push_back( DataBase::getDB()->getTechnology()->getBasicLayer( "poly"   ));
  }


  SweepLine::~SweepLine ()
  { }


  void  SweepLine::_buildCutConnexMap ()
  {
    for ( const ViaLayer* viaLayer : DataBase::getDB()->getTechnology()->getViaLayers() ) {
      const BasicLayer* cutLayer = nullptr;
      for ( const BasicLayer* layer : viaLayer->getBasicLayers() ) {
        if (layer->getMaterial() == BasicLayer::Material::cut) {
          cutLayer = layer;
          break;
        }
      }
      if (not cutLayer) {
        cerr << Error( "SweepLine::_buildConnexityMap(): ViaLayer \"%s\" does not contains any *cut* (ignored)."
                     , getString(viaLayer->getName()).c_str()
                     ) << endl;
        continue;
      }
      auto iCutMap = _connexityMap.find( cutLayer );
      if (iCutMap == _connexityMap.end()) {
        _connexityMap.insert( make_pair( cutLayer, LayerSet() ));
        iCutMap = _connexityMap.find( cutLayer );
      }
      for ( const BasicLayer* layer : viaLayer->getBasicLayers() ) {
        if (   (layer->getMaterial() != BasicLayer::Material::cut)
           and (_extractedsMask.intersect(layer->getMask())) ) {
          iCutMap->second.insert( layer );
        }
      }
    }
    // for ( auto item : _connexityMap ) {
    //   cerr << "BasicLayers connex to cut: " << item.first << endl;
    //   for ( const BasicLayer* bl : item.second ) {
    //     cerr << "| " << bl << endl;
    //   }
    // }
  }

  
  const SweepLine::LayerSet&  SweepLine::getCutConnexLayers ( const BasicLayer* cutLayer ) const
  {
    static LayerSet emptySet;
    auto iCutMap = _connexityMap.find( cutLayer );
    if (iCutMap == _connexityMap.end())
      return emptySet;
    return iCutMap->second;
  }
  

  void  SweepLine::run ()
  {
    UpdateSession::open();
  //DebugSession::open( 160, 169 );
    cdebug_log(160,1) << "SweepLine::run()" << endl;
    loadTiles();
  //bool debugOn = false;
  //bool written = false;
    size_t processedTiles = 0;
    for ( Element& element : _tiles ) {
      processedTiles++;
      Tile*     tile     = element.getTile();
      TileIntv  tileIntv ( tile, tile->getYMin(), tile->getYMax() );

      if (tty::enabled()) {
        cmess2 << "        <tile:" << tty::bold << right << setw(10) << setfill('0')
               << processedTiles << tty::reset
               << " remains:" << right << setw(10) << setfill('0')
               << (_tiles.size() - processedTiles)
               << setfill(' ') << tty::reset << ">" << tty::cr;
        cmess2.flush ();
      } else {
        cmess2 << "        <tile:" << right << setw(10) << setfill('0')
               << processedTiles << tty::reset
               << " remains:" << right << setw(10) << setfill('0')
               << (_tiles.size() - processedTiles)
               << setfill(' ') << "> " << tile << endl;
        cmess2.flush ();
      }

      // if (tile->getOccurrence().getEntity()->getId() == 3348) {
      //   DebugSession::open( 160, 169 );
      // }
      // if (getString(tile->getNet()->getName()) == "a(13)") {
      //   cerr << tile << endl;
      // }
      // if (not debugOn and (element.getX() == DbU::fromLambda(1724.0))) {
      //   debugOn = true;
      //   DebugSession::open( 0, 169 );
      // }
      // if (debugOn and (element.getX() > DbU::fromLambda(1726.0))) {
      //   debugOn = false;
      //   DebugSession::close();
      // }
      cdebug_log(160,1) << "X@ + " << DbU::getValueString(element.getX()) << " " << tile << endl;
      auto  intvTree = _intervalTrees.find( element.getMask() );
      if (intvTree == _intervalTrees.end()) {
        cerr << Error( "SweepLine::run(): Missing interval tree for layer(mask) %s."
                       "        (for tile: %s)"
                     , getString(element.getMask()).c_str()
                     , getString(element.getTile()).c_str()
                     ) << endl;
        cdebug_tabw(160,-1);
        continue;
      }
      if (element.isLeftEdge()) {
        // if (tile->getOccurrence().getEntity()->getId() == 3348) {
        // //if (not written) intvTree->second.write( "tree-before.gv" );
        //   cdebug_log(160,0) << " Interval tree *before* insertion." << endl;
        //   for ( auto elt : intvTree->second.getElements() ) {
        //     cdebug_log(160,0) << " | in tree:" << elt << endl;
        //     if (elt.getData()->getBoundingBox().getXMax() < tile->getLeftEdge())
        //       cdebug_log(160,0) << " * Should have been removed !" << endl;
        //   }
        // }
        for ( const TileIntv& overlap : intvTree->second.getOverlaps(
                                           Interval(tile->getYMin(), tile->getYMax() ))) {
          cdebug_log(160,0) << " | intersect " << overlap.getData() << endl;
          tile->merge( overlap.getData() );
        }
        cdebug_log(160,0) << " | insert tile" << endl;
        // if (tile->getId() == 60117) {
        //   cerr << " | insert in " << element.getMask() << endl;
        //   cerr << " | " << tile << endl;
        // }
        // if (tile->getId() == 46373) {
        //   cerr << "   | insert " << tile << endl;
        // }
        intvTree->second.insert( tileIntv );
        if (tile->getOccurrence().getEntity()->getId() == 3348) {
        //if (not written) intvTree->second.write( "tree-after.gv" );
        //written = true;
        }
      } else {
        // if (tile->getId() == 289) {
        //   DebugSession::open( 0, 169 );
        // }
        // cdebug_log(160,0) << "   | remove tile from " << element.getMask() << endl;
        // cdebug_log(160,0) << "   | " << tile << endl;
        // if ((tile->getId() == 289) and not written) {
        //   cerr << "(before) written is " << written << endl; 
        //   DebugSession::open( 0, 169 );
        //   intvTree->second.write( "tree-before.gv" );
        // //DebugSession::close();
        //   for ( auto elt : intvTree->second.getElements() ) {
        //     cerr << "   | in tree:" << elt << endl;
        //   }
        // }
        cdebug_log(160,0) << " | remove tile" << endl;
        intvTree->second.remove( tileIntv );
        // DebugSession::open( 0, 169 );
        // intvTree->second.checkVMax();
        // DebugSession::close();
        // if ((tile->getId() == 289) and not written) {
        // //DebugSession::open( 0, 169 );
        //   written = true;
        //   cerr << "(after) written is " << written << endl; 
        //   intvTree->second.write( "tree-after.gv" );
        //   DebugSession::close();
        // }
        // if (intvTree->second.find( tileIntv ) != intvTree->second.end()) {
        //   cerr << "NOT Removed " << tileIntv << endl;
        // }
        // if (tile->getId() == 289) {
        //   cerr << "   | removed " << tile << endl;
        //   intvTree->second.write( "tree.gv" );
        //   for ( auto elt : intvTree->second.getElements() ) {
        //     cerr << "   | in tree:" << elt << endl;
        //   }
        //   DebugSession::close();
        // }
        // if (tile->getId() == 46055) {
        //   intvTree->second.write( "we_at_remove.gv" );
        //   for ( auto tile : intvTree->second.getElements() ) {
        //     cerr << "| in tree:" << tile << endl;
        //   }
        // }
      }
    //intvTree->second.checkVMax();
      // cdebug_tabw(160,-1);
      // if (tile->getOccurrence().getEntity()->getId() == 3348) {
      //   DebugSession::close();
      // }
      cdebug_tabw(160,-1);
    }
  //if (debugOn) DebugSession::close();
    cdebug_tabw(160,-1);
  //DebugSession::close();
    mergeEquipotentials();
    deleteTiles();
    UpdateSession::close();
  }


  void  SweepLine::loadTiles ()
  {
  //cerr << "SweepLine::loadTiles()" << endl;

    for ( const BasicLayer* layer : _extracteds ) {
      _intervalTrees.insert( make_pair( layer->getMask(), TileIntvTree() ));
    }

    QueryTiles query ( this );
    for ( const BasicLayer* layer : _extracteds ) {
      query.setBasicLayer( layer );
      query.doQuery();
    }
    cmess2 << "     - Loaded " << _tiles.size() << " tiles (from "
           << query.getGoMatchCount() << " gos)." << endl;

    // for ( Occurrence occurrence : getCell()->getOccurrencesUnder( getCell()->getBoundingBox() ) ) {
    //   vector<Tile*> tiles;
    //   Component* component = dynamic_cast<Component*>( occurrence.getEntity() );
    //   if (occurrence.getPath().getInstances().getSize() > 0) {
    //     cerr << occurrence << endl;
    //   }
    //   if (not component) continue;
    //   for ( const BasicLayer* layer : extracteds ) {
    //     if (not component->getLayer()->getMask().intersect(layer->getMask())) continue;
    //     tiles.push_back( Tile::create( occurrence, layer ));
    //     _tiles.push_back( Element( tiles.back(), Tile::LeftEdge ) );
    //     _tiles.push_back( Element( tiles.back(), Tile::RightEdge ) );
    //     if (tiles.size() > 1)
    //       tiles.back()->setParent( tiles[0] );
    //   }
    //   tiles.clear();
    // }
    sort( _tiles.begin(), _tiles.end() );
  }


  void  SweepLine::deleteTiles ()
  {
    Tile::deleteAllTiles();
  }


  void  SweepLine::mergeEquipotentials ()
  {
  //DebugSession::open( 160, 169 );
    cdebug_log(160,1) << "SweepLine::mergeEquipotentials()" << endl;
  //cerr << "SweepLine::mergeEquipotentials()" << endl;
    Tile::timeTick();
    for ( Tile* tile : Tile::getAllTiles() ) {
      tile->getRoot( Tile::MergeEqui|Tile::MakeLeafEqui );
    }
    cdebug_tabw(160,-1);
  //DebugSession::close();
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
