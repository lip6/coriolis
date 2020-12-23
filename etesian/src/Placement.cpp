// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Placement.cpp"                          |
// +-----------------------------------------------------------------+


#include <map>
#include <list>
#include <tuple>
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Plug.h"
#include "hurricane/Path.h"
#include "hurricane/Instance.h"
#include "hurricane/Library.h"
#include "hurricane/viewer/CellWidget.h"
#include "hurricane/viewer/CellViewer.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/ToolBox.h"
#include "etesian/EtesianEngine.h"


namespace {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::Warning;
  using Hurricane::Error;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Interval;
  using Hurricane::DBo;
  using Hurricane::Occurrence;
  using Hurricane::Instance;
  using Hurricane::Path;
  using Hurricane::Transformation;
  using Hurricane::DataBase;
  using Hurricane::Cell;
  using Hurricane::Library;
  using CRL::AllianceFramework;
  using CRL::CatalogExtension;
  using CRL::getTransformation;
  using Etesian::EtesianEngine;


// -------------------------------------------------------------------
// Class  :  "::TieDatas".

  class TieDatas {
    public:
      inline            TieDatas        ( Cell* cell=NULL, DbU::Unit leftDistance=0, DbU::Unit rightDistance=0 );
      inline Cell*      getCell         () const;
      inline DbU::Unit  getLeftDistance () const;
      inline DbU::Unit  getRightDistance() const;
    private:
      Cell*      _cell;
      DbU::Unit  _leftDistance;
      DbU::Unit  _rightDistance;
  };


  inline TieDatas::TieDatas ( Cell* cell, DbU::Unit leftDistance, DbU::Unit rightDistance )
    : _cell         (cell)
    , _leftDistance (leftDistance)
    , _rightDistance(rightDistance)
  { }

  inline Cell*      TieDatas::getCell         () const { return _cell; }
  inline DbU::Unit  TieDatas::getLeftDistance () const { return _leftDistance; }
  inline DbU::Unit  TieDatas::getRightDistance() const { return _rightDistance; }


// -------------------------------------------------------------------
// Class  :  "::TieLUT".

  class TieLUT {
    public:
      inline                 TieLUT           ();
      inline const TieDatas* getTieDatas      ( Cell* ) const;
      inline DbU::Unit       getLeftDistance  ( Cell* ) const;
      inline DbU::Unit       getRightDistance ( Cell* ) const;
      inline void            addTieDatas      ( Cell*, DbU::Unit leftDistance, DbU::Unit rightDistance ); 
    private:
      map< Cell*, TieDatas, DBo::CompareById >  _tieLut;
  };

  
  inline TieLUT::TieLUT ()
    : _tieLut()
  { }


  inline void  TieLUT::addTieDatas ( Cell* cell, DbU::Unit leftDistance, DbU::Unit rightDistance )
  {
    const TieDatas* datas = getTieDatas( cell );
    if (datas) {
      cerr << Error( "TieLUT::addTieDatas(): Duplicate datas for % (ignoreds)."
                   , getString(cell).c_str()
                   ) << endl;
    }
    _tieLut[ cell ] = TieDatas( cell, leftDistance, rightDistance );
  }

  
  inline const TieDatas* TieLUT::getTieDatas ( Cell* cell ) const
  {
    auto iDatas = _tieLut.find( cell );
    return (iDatas != _tieLut.end()) ? &((*iDatas).second) : NULL;
  }

  
  inline DbU::Unit  TieLUT::getLeftDistance  ( Cell* cell ) const
  {
    const TieDatas* datas = getTieDatas( cell );
    return (datas) ? datas->getLeftDistance() : 0;
  }

  
  inline DbU::Unit  TieLUT::getRightDistance ( Cell* cell ) const
  {
    const TieDatas* datas = getTieDatas( cell );
    return (datas) ? datas->getRightDistance() : 0;
  }


// -------------------------------------------------------------------
// Class  :  "::Tile".

  class Area;
  class Slice;


  class Tile {
    public:
      inline                   Tile          ( DbU::Unit xMin, DbU::Unit width, const Occurrence& );
      inline       bool        isFixed       () const;
      inline       DbU::Unit   getXMin       () const;
      inline       DbU::Unit   getXMax       () const;
      inline       DbU::Unit   getWidth      () const;
      inline       Cell*       getMasterCell () const;
      inline       Instance*   getInstance   () const;
      inline const Occurrence& getOccurrence () const;
      inline       void        translate     ( DbU::Unit );
    private:
      DbU::Unit   _xMin;
      DbU::Unit   _width;
      Occurrence  _occurrence;
  };

  inline Tile::Tile ( DbU::Unit xMin, DbU::Unit width, const Occurrence& occurrence )
    : _xMin(xMin), _width(width), _occurrence(occurrence)
  { }

  inline       DbU::Unit   Tile::getXMin       () const { return _xMin; }
  inline       DbU::Unit   Tile::getXMax       () const { return _xMin+_width; }
  inline       DbU::Unit   Tile::getWidth      () const { return _width; }
  inline const Occurrence& Tile::getOccurrence () const { return _occurrence; }
  inline       Instance*   Tile::getInstance   () const { return static_cast<Instance*>( _occurrence.getEntity() ); }
  inline       Cell*       Tile::getMasterCell () const { return getInstance()->getMasterCell(); }

  inline bool  Tile::isFixed () const
  { return getInstance()->getPlacementStatus() == Instance::PlacementStatus::FIXED; }


  inline void  Tile::translate ( DbU::Unit dx )
  {
    cdebug_log(121,0) << "  Tile::translate(), dx:" << DbU::getValueString(dx) << ", " << _occurrence << endl;
    Instance*      instance  = getInstance();
    Transformation reference = instance->getTransformation();
    Transformation transf    = Transformation( reference.getTx() + dx
                                             , reference.getTy()
                                             , reference.getOrientation() );
    instance->setTransformation( transf );
    _xMin += dx;
  }


// -------------------------------------------------------------------
// Class  :  "::Hole".

  class Area;
  class Slice;


  class Hole {
    public:
      inline                   Hole          ( DbU::Unit xMin, DbU::Unit width );
      inline       DbU::Unit   getXMin       () const;
      inline       DbU::Unit   getXMax       () const;
      inline       DbU::Unit   getWidth      () const;
    private:
      DbU::Unit   _xMin;
      DbU::Unit   _width;
  };

  inline Hole::Hole ( DbU::Unit xMin, DbU::Unit width )
    : _xMin(xMin), _width(width)
  { }

  inline       DbU::Unit   Hole::getXMin       () const { return _xMin; }
  inline       DbU::Unit   Hole::getXMax       () const { return _xMin+_width; }
  inline       DbU::Unit   Hole::getWidth      () const { return _width; }


// -------------------------------------------------------------------
// Class  :  "::SubSlice".

  class SubSlice {
    public:
                                         SubSlice        ( Slice*, const list<Tile>::iterator& beginTile );
      inline const list<Tile>::iterator  getBeginTile    () const;
      inline const list<Tile>::iterator  getEndTile      () const;
      inline       DbU::Unit             getYBottom      () const;
      inline       DbU::Unit             getXMin         () const;
      inline       DbU::Unit             getXMax         () const;
                   DbU::Unit             getAverageChunk ( size_t& ) const;
                   void                  insertTies      ( DbU::Unit latchUpMax );
    private:
      Slice*               _slice;
      list<Tile>::iterator _beginTile;
      list<Tile>::iterator _endTile;
  };

  inline const list<Tile>::iterator  SubSlice::getBeginTile () const { return _beginTile; }
  inline const list<Tile>::iterator  SubSlice::getEndTile   () const { return _endTile; }


// -------------------------------------------------------------------
// Class  :  "::Slice".

  class Slice {
    public:
                            Slice            ( Area*, DbU::Unit ybottom );
      inline DbU::Unit      getYBottom       () const;
      inline DbU::Unit      getXMin          () const;
      inline DbU::Unit      getXMax          () const;
      inline Interval       getXSpan         () const;
      inline Area*          getArea          () const;
      inline EtesianEngine* getEtesian       () const;
      inline size_t         getSpinSlice0    () const;
      inline DbU::Unit      getLeftDistance  ( Cell* cell ) const;
      inline DbU::Unit      getRightDistance ( Cell* cell ) const;
             bool           validate         ( DbU::Unit latchUpMax ) const;
      inline list<Tile>&    getTiles         ();
             void           merge            ( const Occurrence&, const Box& );
             void           addFeeds         ( size_t islice );
             void           fillHole         ( DbU::Unit xmin, DbU::Unit xmax, DbU::Unit ybottom, size_t yspin );
             void           buildSubSlices   ();
             void           showSubSlices    ();
             void           insertTies       ( DbU::Unit latchUpMax );
             string         _getString       () const;
    private:
      Area*             _area;
      DbU::Unit         _ybottom;
      list<Tile>        _tiles;
      vector<SubSlice>  _subSlices;
  };


  Slice::Slice ( Area* area, DbU::Unit ybottom )
    : _area     (area)
    , _ybottom  (ybottom)
    , _tiles    ()
    , _subSlices()
  { }


  bool  Slice::validate ( DbU::Unit latchUpMax ) const
  {
    if (_tiles.empty()) return true;

    cdebug_log(121,1) << "Slice::validate() @Y=" << DbU::getValueString(_ybottom) << endl;

    Cell*      feed       = getEtesian()->getFeedCells().getBiggestFeed();
    DbU::Unit  feedWidth  = feed->getAbutmentBox().getWidth();
    DbU::Unit  cellLength = 0;
    bool       validated  = true;
    auto       iTile      = _tiles.begin();
    auto       iTileNext  = iTile;
    auto       iTileFeed  = _tiles.begin();
    ++iTileNext;
    while ( iTileNext != _tiles.end() ) {
      cellLength += (*iTile).getWidth();
      if (  ((*iTileNext).getXMin() - (*iTile).getXMax() >= feedWidth)
         or ((*iTile).getMasterCell() == feed)) {
        if ((*iTile).getMasterCell() == feed)
          cellLength -= feedWidth;
      //if (feed and ((*iTile).getMasterCell() == feed)) {
        cdebug_log(121,0) << "cellLength=" << DbU::getValueString(cellLength) << endl;
        if (cellLength > latchUpMax) {
          while ( (iTileFeed != iTileNext) and (iTileFeed != _tiles.end()) ) {
            cdebug_log(121,0) << "| " << DbU::getValueString((*iTileFeed).getXMin())
                              << " " << (*iTileFeed).getOccurrence() << endl;
            ++iTileFeed;
          }
        } else {
          iTileFeed = iTileNext;
        }
        cellLength = 0;
      }
      if ((*iTile).getXMax() > (*iTileNext).getXMin()) {
        cerr << Error( "Slice::validate(): Overlap in slice @%s between instances,\n"
                       "        %s @%s\n"
                       "        %s @%s"
                     , DbU::getValueString(_ybottom).c_str()
                     , getString((*iTile).getOccurrence()).c_str()
                     , DbU::getValueString((*iTile).getXMin()).c_str()
                     , getString((*iTileNext).getOccurrence()).c_str()
                     , DbU::getValueString((*iTileNext).getXMin()).c_str()
                     ) << endl;
        validated = false;
      }
      ++iTile; ++iTileNext;
    }

    cdebug_log(121,-1);
    return validated;
  }


  void  Slice::merge ( const Occurrence& occurrence, const Box& flatAb )
  {
    if (_tiles.empty() or (_tiles.front().getXMin() > flatAb.getXMin())) {
      _tiles.insert( _tiles.begin(), Tile(flatAb.getXMin(), flatAb.getWidth(), occurrence) );
      return;
    }

    list<Tile>::iterator imerge = _tiles.end();
    for ( auto itile = _tiles.begin() ; itile != _tiles.end() ; ++itile ) {
      if ((*itile).getXMin() > flatAb.getXMin()) {
        _tiles.insert( itile, Tile(flatAb.getXMin(), flatAb.getWidth(), occurrence) );
        return;
      }
    }

    _tiles.insert( _tiles.end(), Tile(flatAb.getXMin(), flatAb.getWidth(), occurrence) );
  }


  void  Slice::buildSubSlices ()
  {
    if (not _tiles.empty() ) {
      _subSlices.push_back( SubSlice( this, _tiles.begin() ) );
      while ( _subSlices.back().getEndTile() != _tiles.end() ) {
        _subSlices.push_back( SubSlice( this, _subSlices.back().getEndTile() ) );
      }
    }
  }


  void  Slice::showSubSlices ()
  {
    if (_tiles.empty()) return;

    cdebug_log(121,0) << "+ Slice @" << DbU::getValueString(_ybottom) << endl;
    for ( const SubSlice& subSlice : _subSlices ) {
      size_t    count = 0;
      DbU::Unit avg   = subSlice.getAverageChunk( count );
      cdebug_log(121,0) << "| [" << DbU::getValueString(subSlice.getXMin())
                        << " " << DbU::getValueString(subSlice.getXMax())
                        << "] tiles:" << count
                        << " avg:" << DbU::getValueString(avg)
                        << " " << endl;
    }
  }


  void  Slice::insertTies ( DbU::Unit latchUpMax )
  {
    cdebug_log(121,1) << "Slice::insertTies() @" << DbU::getValueString(_ybottom) << endl;
    for ( SubSlice& subSlice : _subSlices ) subSlice.insertTies( latchUpMax );
    cdebug_tabw(121,-1);
  }


  void  Slice::addFeeds ( size_t islice )
  {
    if (_tiles.empty()) {
      fillHole( getXMin(), getXMax(), getYBottom(), islice%2 );
      return;
    }

    list<Tile>::iterator itile     = _tiles.begin();
    list<Tile>::iterator itilenext = itile;
    ++itilenext;

  // Hole before the first chunk.
    if ((*itile).getXMin() > getXMin()) {
      fillHole( getXMin(), (*itile).getXMin(), getYBottom(), (islice+getSpinSlice0())%2 );
    }

    for ( ; itilenext != _tiles.end() ; ++itile, ++itilenext ) {
      fillHole( (*itile).getXMax(), (*itilenext).getXMin(), getYBottom(), (islice+getSpinSlice0())%2 );
    }

  // Hole after the last chunk.
    if ((*itile).getXMax() < getXMax()) {
      fillHole( (*itile).getXMax(), getXMax(), getYBottom(), (islice+getSpinSlice0())%2 );
    }
  }


  void  Slice::fillHole ( DbU::Unit xmin, DbU::Unit xmax, DbU::Unit ybottom, size_t yspin )
  {
    Cell* feed = getEtesian()->getFeedCells().getBiggestFeed();
    if (feed == NULL) {
      cerr << Error("Slice::fillHole(): No feed has been registered, ignoring.") << endl;
      return;
    }

    DbU::Unit feedWidth = feed->getAbutmentBox().getWidth();
    DbU::Unit xtie      = xmin;

    while ( true ) {
      if (xtie           >= xmax) break;
      if (xtie+feedWidth >  xmax) {
      // Feed is too big, try to find a smaller one.
        int pitch = (int)((xmax-xtie) / getEtesian()->getSliceStep());
        for ( ; pitch > 0 ; --pitch ) {
          feed = getEtesian()->getFeedCells().getFeed( pitch );
          if (feed == NULL) continue;
          
          feedWidth = feed->getAbutmentBox().getWidth();
          if (feed != NULL) break;
        }
        if (feed == NULL) break;
      }

      Instance::create ( getEtesian()->getBlockCell()
                       , getEtesian()->getFeedCells().getUniqueInstanceName().c_str()
                       , feed
                       , getTransformation( feed->getAbutmentBox()
                                          , xtie
                                          , _ybottom
                                          , (yspin)?Transformation::Orientation::MY
                                                   :Transformation::Orientation::ID
                                          )
                       , Instance::PlacementStatus::PLACED
                       );

      xtie += feedWidth;
    }
  }


// -------------------------------------------------------------------
// Class  :  "::Area".

  class Area {
    public:
                            Area             ( EtesianEngine* );
                           ~Area             ();
      inline EtesianEngine* getEtesian       () const;
      inline size_t         getSpinSlice0    () const;
      inline void           setSpinSlice0    ( size_t );
      inline DbU::Unit      getXMin          () const;
      inline DbU::Unit      getXMax          () const;
      inline DbU::Unit      getLeftDistance  ( Cell* cell ) const;
      inline DbU::Unit      getRightDistance ( Cell* cell ) const;
             bool           validate         ( DbU::Unit latchUpMax ) const;
             void           merge            ( const Occurrence&, const Box& );
             void           addFeeds         ();
             void           buildSubSlices   ();
             void           showSubSlices    ();
             void           insertTies       ( DbU::Unit latchUpMax );
    private:
      EtesianEngine*  _etesian;
      TieLUT          _tieLut;
      Box             _cellAb;
      DbU::Unit       _sliceHeight;
      vector<Slice*>  _slices;
      size_t          _spinSlice0;
  };


  Area::Area ( EtesianEngine* etesian )
    : _etesian    (etesian)
    , _tieLut     ()
    , _cellAb     (etesian->getBlockCell()->getAbutmentBox())
    , _sliceHeight(_etesian->getSliceHeight())
    , _slices     ()
  {
    size_t slicesNb = _cellAb.getHeight() / _sliceHeight;
    for ( size_t islice=0 ; islice<slicesNb ; ++islice )
      _slices.push_back( new Slice( this, _cellAb.getYMin()+islice*_sliceHeight ) );

    Cell* feed = getEtesian()->getFeedCells().getBiggestFeed();
    if (feed) {
      Library* library = feed->getLibrary();
      if (feed->getLibrary()->getName() == "FlexLib") {
        cmess2 << "  o  Using hard-coded tie/cell datas suited for \"FlexLib\"." << endl;
        Cell* masterCell = library->getCell( "sff1_x4" );
        if (masterCell) {
          _tieLut.addTieDatas( masterCell, DbU::fromMicrons(1.28), DbU::fromMicrons(11.7) );
        }
      }
    }
  }


  Area::~Area ()
  {
    for ( size_t islice=0 ; islice<_slices.size() ; ++islice )
      delete _slices[islice];
  }


  inline EtesianEngine* Area::getEtesian       () const { return _etesian; }
  inline size_t         Area::getSpinSlice0    () const { return _spinSlice0; }
  inline void           Area::setSpinSlice0    ( size_t spinSlice0 ) { _spinSlice0 = spinSlice0; }
  inline DbU::Unit      Area::getXMin          () const { return _cellAb.getXMin(); }
  inline DbU::Unit      Area::getXMax          () const { return _cellAb.getXMax(); }
  inline DbU::Unit      Area::getLeftDistance  ( Cell* cell ) const { return _tieLut.getLeftDistance(cell); }
  inline DbU::Unit      Area::getRightDistance ( Cell* cell ) const { return _tieLut.getRightDistance(cell); }


  bool  Area::validate ( DbU::Unit latchUpMax ) const
  {
    bool validated = true;
    for ( const Slice* slice : _slices )
      validated = slice->validate(latchUpMax) and validated;
    return validated;
  }


  void   Area::merge ( const Occurrence& occurrence, const Box& flatAb )
  {
    if (flatAb.getYMin() < _cellAb.getYMin()) {
      cerr << Warning("Area::merge(): Attempt to merge instance outside the Cell abutment box.") << endl;
      return;
    }

    size_t ibegin = (flatAb.getYMin()-_cellAb.getYMin()) / _sliceHeight;
    size_t iend   = (flatAb.getYMax()-_cellAb.getYMin()) / _sliceHeight;

    for ( size_t islice=ibegin ; islice<iend ; ++islice ) {
      _slices[islice]->merge( occurrence, flatAb );
    }
  }


  void  Area::addFeeds ()
  {
    for ( size_t islice=0 ; islice<_slices.size() ; islice++ ) {
      _slices[islice]->addFeeds( islice );
    }
  }


  void   Area::buildSubSlices ()
  { for ( Slice* slice : _slices ) slice->buildSubSlices(); }


  void   Area::showSubSlices ()
  { for ( Slice* slice : _slices ) slice->showSubSlices(); }


  void  Area::insertTies ( DbU::Unit latchUpMax )
  {
    for ( Slice* slice : _slices ) slice->insertTies( latchUpMax );
    validate( latchUpMax );
  }

  
  SubSlice::SubSlice ( Slice* slice, const list<Tile>::iterator& beginTile )
    : _slice(slice)
    , _beginTile(beginTile)
    , _endTile  (beginTile)
  {
    for ( ++_endTile; _endTile != slice->getTiles().end() ; ++_endTile ) {
      Instance* instance = dynamic_cast<Instance*>( (*_endTile).getOccurrence().getEntity() ); 
      if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED)
        break;
    }
  }

  inline DbU::Unit  SubSlice::getYBottom () const { return _slice->getYBottom(); }

  inline DbU::Unit  SubSlice::getXMin () const
  { return (_beginTile == _slice->getTiles().begin()) ? _slice->getXMin() : (*_beginTile).getXMin(); }

  inline DbU::Unit  SubSlice::getXMax () const
  { return (_endTile   == _slice->getTiles().end  ()) ? _slice->getXMax() : (*_endTile).getXMax(); }


  DbU::Unit  SubSlice::getAverageChunk ( size_t& count ) const
  {
    count = 0;
    Cell* feed = _slice->getEtesian()->getFeedCells().getBiggestFeed();
    if (feed == NULL) {
      cerr << Error("SubSlice::getAverageChunk(): No feed has been registered, ignoring.") << endl;
      return -1;
    }
    DbU::Unit  feedWidth  = feed->getAbutmentBox().getWidth();
    DbU::Unit  usedLength = 0;
    DbU::Unit  xmin       = getXMin();
    DbU::Unit  xmax       = getXMax();
    list<Tile>::const_iterator iTile = _beginTile;
    for ( ; iTile != _endTile ; ++iTile, ++count )
      usedLength += (*iTile).getWidth();

    if (xmax - xmin - usedLength == 0) return 0;
    return (feedWidth * (xmax - xmin)) / (xmax - xmin - usedLength);
  }


  void  SubSlice::insertTies ( DbU::Unit latchUpMax )
  {
    size_t    count        = 0;
    DbU::Unit averageChunk = getAverageChunk( count );
    if (averageChunk > latchUpMax) {
      cerr << Error( "SubSlice::insertTies(): Not enough free space to insert polarization ties.\n"
                     "        @Y=%s, begin=%s"
                   , getString(DbU::getValueString(getYBottom())).c_str()
                   , getString((*_beginTile).getOccurrence()).c_str()
                   ) << endl;
    }
    
    Cell* feed = _slice->getEtesian()->getFeedCells().getBiggestFeed();
    if (feed == NULL) {
      cerr << Error( "SubSlice::insertTies(): No feed has been registered, ignoring." ) << endl;
      return;
    }
    DbU::Unit  feedWidth  = feed->getAbutmentBox().getWidth();

    cdebug_log(121,1) << "SubSlice::insterTies(): LatchUpMax:" << DbU::getValueString( latchUpMax ) << endl;
    cdebug_log(121,1) << "Direct subSlice walkthrough." << endl;

    DbU::Unit xMin      = getXMin();
    DbU::Unit xMax      = getXMax();
    auto      beginTile = _beginTile;
    auto      endTile   = _endTile;
    if ((*beginTile).isFixed()) {
      xMin += (*beginTile).getWidth();
      ++beginTile;
    }
    if (endTile == _slice->getTiles().end()) {
      --endTile;
    } else if ((*endTile).isFixed()) {
      xMax = (*endTile).getXMin();
      --endTile;
    }

    DbU::Unit leftPosition = (*_beginTile).getXMin();
    DbU::Unit tileLength   = 0;
    auto      iTile        = _beginTile;
    while ( true ) {
      Instance* instance     = (*iTile).getInstance();
      DbU::Unit cellTieLeft  = _slice->getLeftDistance ( instance->getMasterCell() );
      DbU::Unit cellTieRight = _slice->getRightDistance( instance->getMasterCell() );

      DbU::Unit dxLeft = (*iTile).getXMin() - leftPosition;
      if (dxLeft >= feedWidth) {
        cdebug_log(121,0) << "Enough space, reset rigth shift." << endl;
        leftPosition = (*iTile).getXMin();
        tileLength   = 0;
      }
      if (cellTieLeft) {
        cdebug_log(121,0) << instance->getName() << " contains a tie, reset tile length." << endl;
        tileLength = cellTieRight;
      }
      
      if (tileLength + (*iTile).getWidth() > latchUpMax) {
        cdebug_log(121,0) << "Length above threshold, insert tie space." << endl;
        leftPosition += feedWidth;
        tileLength    = 0;
      }

      (*iTile).translate( leftPosition - (*iTile).getXMin() );
      tileLength   += (*iTile).getWidth();
      leftPosition += (*iTile).getWidth();

      if (iTile == endTile) break;
      ++iTile;
    }

    leftPosition = xMax;
    iTile        = endTile;
    if (iTile == _slice->getTiles().end()) --iTile;

    cdebug_tabw(121,-1);
    cdebug_log(121,1) << "Reverse subSlice walkthrough, in excess of "
                      << DbU::getValueString((*iTile).getXMax() - leftPosition)
                      << " (feedWidth:" << DbU::getValueString(feedWidth) << ")" << endl;

    if ((*iTile).getXMax() > leftPosition) {
      tileLength = 0;
      DbU::Unit prevTileShift = leftPosition - (*iTile).getXMax();
      while ( true ) {
        Instance* instance     = (*iTile).getInstance();
        DbU::Unit cellTieLeft  = _slice->getLeftDistance ( instance->getMasterCell() );
        DbU::Unit cellTieRight = _slice->getRightDistance( instance->getMasterCell() );
        if (cellTieRight) {
          cdebug_log(121,0) << instance->getName() << " contains a tie, reset tile length." << endl;
          tileLength = cellTieLeft - (*iTile).getWidth();
        }

        DbU::Unit tileShift = leftPosition - (*iTile).getXMax();
        cdebug_log(121,0) << "tileShift=" << DbU::getValueString(tileShift)
                          << " leftHole=" << DbU::getValueString(prevTileShift-tileShift)
                          << endl;
        DbU::Unit holeLength = tileShift - prevTileShift;
        if (holeLength == 0) {
          cdebug_log(121,0) << "No change in shift (no hole, abuted tiles)." << endl;
        } else {
          if (holeLength < feedWidth) {
            cdebug_log(121,0) << "Hole is less than a feed width ("
                              << DbU::getValueString(holeLength) << " < "
                              << DbU::getValueString(feedWidth)  << "), should not occur." << endl;
          } else {
            if (tileLength+(*iTile).getWidth() < latchUpMax) {
              cdebug_log(121,0) << "Absorbing whole shift (below latch up distance, tile length="
                                << DbU::getValueString(tileLength) << ")" << endl;
            } else {
              cdebug_log(121,0) << "Absorbing shift, keeping one feed spacing." << endl;
              leftPosition -= feedWidth;
              tileLength    = 0;
            }
          }
        }

        cdebug_log(121,0) << "Left position (+width) " << DbU::getValueString(leftPosition)
                          << " width:" << DbU::getValueString((*iTile).getWidth()) << endl;
        if (leftPosition > (*iTile).getXMax()) {
          cdebug_log(121,0) << "Positive tileShift, all excess has been compensated." << endl;
          break;
        }
        (*iTile).translate( leftPosition - (*iTile).getXMax() );
        leftPosition -= (*iTile).getWidth();
        tileLength   += (*iTile).getWidth();

        if (iTile == _beginTile) {
          cerr << Error( "SubSlice::insertTies(): Not enough free space to insert polarization ties.\n"
                         "        @Y=%s, begin=%s"
                       , getString(DbU::getValueString(getYBottom())).c_str()
                       , getString((*iTile).getOccurrence()).c_str()
                       ) << endl;
          break;
        }
        --iTile;
      } 
    }
    cdebug_tabw(121,-2);
  }


  inline DbU::Unit       Slice::getYBottom       () const { return _ybottom; }
  inline DbU::Unit       Slice::getXMin          () const { return _area->getXMin(); }
  inline DbU::Unit       Slice::getXMax          () const { return _area->getXMax(); }
  inline DbU::Unit       Slice::getLeftDistance  ( Cell* cell ) const { return _area->getLeftDistance(cell); }
  inline DbU::Unit       Slice::getRightDistance ( Cell* cell ) const { return _area->getRightDistance(cell); }
  inline Interval        Slice::getXSpan         () const { return Interval( getXMin(), getXMax() ); }
  inline Area*           Slice::getArea          () const { return _area; }
  inline EtesianEngine*  Slice::getEtesian       () const { return getArea()->getEtesian(); }
  inline size_t          Slice::getSpinSlice0    () const { return getArea()->getSpinSlice0(); }
  inline list<Tile>&     Slice::getTiles         () { return _tiles; }


} // Anonymous namespace.


namespace Etesian {


  using Hurricane::DataBase;
  using Hurricane::UpdateSession;
  using Hurricane::Occurrence;


  void  EtesianEngine::readSlices ()
  {
    if (not getFeedCells().feedNumbers()) {
      cerr << Warning( "EtesianEngine::readSlices(): No feed cells available, skipping." ) << endl;
      return;
    }

    UpdateSession::open();

    Area area      ( this );
    Box  topCellAb = getBlockCell()->getAbutmentBox();

    area.setSpinSlice0( _yspinSlice0 );

    if (getBlockInstance()) {
      Transformation toBlockTransf = getBlockInstance()->getTransformation();
      toBlockTransf.invert();
      for ( Instance* instance : getCell()->getInstances() ) {
        if (instance == getBlockInstance()) continue;
        if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED) {
          Box overlapAb = instance->getAbutmentBox();
          toBlockTransf.applyOn( overlapAb );
          overlapAb = topCellAb.getIntersection( overlapAb );
          if (not overlapAb.isEmpty()) {
            area.merge( Occurrence(instance), overlapAb );
          }
        }
      }
    }

    for ( Occurrence occurrence : getBlockCell()->getTerminalNetlistInstanceOccurrences() )
    {
      Instance* instance     = static_cast<Instance*>(occurrence.getEntity());
      Cell*     masterCell   = instance->getMasterCell();

      if (CatalogExtension::isFeed(masterCell)) {
        cerr << Warning( "EtesianEngine::readSlices(): Feed instance %s already present."
                       , getString(instance->getName()).c_str() ) << endl;
      }

      Box instanceAb = masterCell->getAbutmentBox();

      Transformation instanceTransf = instance->getTransformation();
      occurrence.getPath().getTransformation().applyOn( instanceTransf );
      instanceTransf.applyOn( instanceAb );

      if (not topCellAb.contains(instanceAb)) {
        cerr << Warning( "EtesianEngine::readSlices(): Instance %s is not fully enclosed in the top cell."
                       , getString(instance->getName()).c_str() ) << endl;
        continue;
      }

      area.merge( occurrence, instanceAb );
    }

    area.buildSubSlices();
    area.showSubSlices();
    if (getConfiguration()->getLatchUpDistance()) {
      Cell*     feed       = getFeedCells().getBiggestFeed();
      DbU::Unit tieSpacing = getConfiguration()->getLatchUpDistance()*2 - feed->getAbutmentBox().getWidth();
      if (feed != NULL)
        tieSpacing -= feed->getAbutmentBox().getWidth();
      area.insertTies( tieSpacing );
    }
    area.addFeeds();

    UpdateSession::close();

    if (_viewer) _viewer->getCellWidget()->refresh();
  }


} // Etesian namespace.
