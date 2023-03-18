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
    : _tramontana  (tramontana) 
    , _tiles       ()
    , _intervalTree()
  { }


  SweepLine::~SweepLine ()
  { }


  void  SweepLine::run ()
  {
    BasicLayer* layer = DataBase::getDB()->getTechnology()->getBasicLayer( "metal1" );
    loadTiles( layer );
    for ( Element& element : _tiles ) {
      Tile*     tile     = element.getTile();
      TileIntv  tileIntv ( tile, tile->getYMin(), tile->getYMax() );
      cerr << right << setw(10) << DbU::getValueString(element.getX()) << " > " << tile << endl;
      if (element.isLeftEdge()) {
        for ( const TileIntv& overlap : _intervalTree.getOverlaps(
                                           Interval(tile->getYMin(), tile->getYMax() ))) {
          cerr << "           | intersect " << overlap.getData() << endl;
          tile->merge( overlap.getData() );
        }
        _intervalTree.insert( tileIntv );
      } else {
        _intervalTree.remove( tileIntv );
      }
    }
    mergeEquipotentials();
  }


  void  SweepLine::loadTiles ( const BasicLayer* layer )
  {
    cerr << "SweepLine::run()" << endl;
    for ( Occurrence occurrence : getCell()->getOccurrencesUnder( getCell()->getBoundingBox() ) ) {
      Component* component = dynamic_cast<Component*>( occurrence.getEntity() );
      if (not component) continue;
      if (not component->getLayer()->contains(layer)) continue;
      Tile* tile = Tile::create( occurrence, layer );
      _tiles.push_back( Element( tile, Tile::LeftEdge ) );
      _tiles.push_back( Element( tile, Tile::RightEdge ) );
    }
    sort( _tiles.begin(), _tiles.end() );
  }


  void  SweepLine::mergeEquipotentials ()
  {
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
