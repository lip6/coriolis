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
  { }


  SweepLine::~SweepLine ()
  { }


  void  SweepLine::run ()
  {
  //cerr << "SweepLine::run()" << endl;
  //DebugSession::open( 0, 2 );
    loadTiles();
    for ( Element& element : _tiles ) {
      Tile*     tile     = element.getTile();
      TileIntv  tileIntv ( tile, tile->getYMin(), tile->getYMax() );
    //cerr << right << setw(10) << DbU::getValueString(element.getX()) << " > " << tile << endl;
      auto  intvTree = _intervalTrees.find( element.getMask() );
      if (intvTree == _intervalTrees.end()) {
        cerr << Error( "SweepLine::run(): Missing interval tree for layer(mask) %s."
                       "        (for tile: %s)"
                     , getString(element.getMask()).c_str()
                     , getString(element.getTile()).c_str()
                     ) << endl;
        continue;
      }
      if (element.isLeftEdge()) {
        for ( const TileIntv& overlap : intvTree->second.getOverlaps(
                                           Interval(tile->getYMin(), tile->getYMax() ))) {
        //cerr << "           | intersect " << overlap.getData() << endl;
          tile->merge( overlap.getData() );
        }
      //cerr << "           | insert tile" << endl;
        intvTree->second.insert( tileIntv );
      } else {
      //cerr << "           | remove tile" << endl;
        intvTree->second.remove( tileIntv );
        // if (tile->getId() == 46055) {
        //   intvTree->second.write( "we_at_remove.gv" );
        //   for ( auto tile : intvTree->second.getElements() ) {
        //     cerr << "| in tree:" << tile << endl;
        //   }
        // }
      }
    }
  //DebugSession::close();
    mergeEquipotentials();
  }


  void  SweepLine::loadTiles ()
  {
  //cerr << "SweepLine::loadTiles()" << endl;
    vector<const BasicLayer*>  extracteds;
    for ( const BasicLayer* bl : DataBase::getDB()->getTechnology()->getBasicLayers() ) {
      if (bl->getMaterial() == BasicLayer::Material::metal)
        extracteds.push_back( bl );
    }
    
    // extracteds.push_back( DataBase::getDB()->getTechnology()->getBasicLayer( "metal5" ));
    // extracteds.push_back( DataBase::getDB()->getTechnology()->getBasicLayer( "metal4" ));
    // extracteds.push_back( DataBase::getDB()->getTechnology()->getBasicLayer( "metal3" ));
    // extracteds.push_back( DataBase::getDB()->getTechnology()->getBasicLayer( "metal2" ));
    // extracteds.push_back( DataBase::getDB()->getTechnology()->getBasicLayer( "metal1" ));
    // extracteds.push_back( DataBase::getDB()->getTechnology()->getBasicLayer( "poly"   ));

    for ( const BasicLayer* layer : extracteds ) {
      _intervalTrees.insert( make_pair( layer->getMask(), TileIntvTree() ));
    }

    for ( Occurrence occurrence : getCell()->getOccurrencesUnder( getCell()->getBoundingBox() ) ) {
      vector<Tile*> tiles;
      Component* component = dynamic_cast<Component*>( occurrence.getEntity() );
      if (not component) continue;
      for ( const BasicLayer* layer : extracteds ) {
        if (not component->getLayer()->getMask().intersect(layer->getMask())) continue;
        tiles.push_back( Tile::create( occurrence, layer ));
        _tiles.push_back( Element( tiles.back(), Tile::LeftEdge ) );
        _tiles.push_back( Element( tiles.back(), Tile::RightEdge ) );
        if (tiles.size() > 1)
          tiles.back()->setParent( tiles[0] );
      }
      tiles.clear();
    }
    sort( _tiles.begin(), _tiles.end() );
  }


  void  SweepLine::mergeEquipotentials ()
  {
  //cerr << "SweepLine::mergeEquipotentials()" << endl;
    Tile::timeTick();
    for ( Tile* tile : Tile::getAllTiles() ) {
      tile->getRoot( Tile::MergeEqui );
    }
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
