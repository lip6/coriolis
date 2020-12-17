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
#include "hurricane/Instance.h"
#include "hurricane/Plug.h"
#include "hurricane/Path.h"
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
  using Hurricane::Occurrence;
  using Hurricane::Instance;
  using Hurricane::Path;
  using Hurricane::Transformation;
  using Hurricane::DataBase;
  using Hurricane::Cell;
  using CRL::AllianceFramework;
  using CRL::CatalogExtension;
  using CRL::getTransformation;
  using Etesian::EtesianEngine;


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

  inline bool  Tile::isFixed () const
  {
    return static_cast<Instance*>( _occurrence.getEntity() )->getPlacementStatus()
             == Instance::PlacementStatus::FIXED;
  }

  inline void  Tile::translate ( DbU::Unit dx )
  {
    cerr << "      Tile::translate(), dx:" << DbU::getValueString(dx) << ", " << _occurrence << endl;
    Instance*      instance  = static_cast<Instance*>( _occurrence.getEntity() );
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
                            Slice          ( Area*, DbU::Unit ybottom );
      inline DbU::Unit      getYBottom     () const;
      inline DbU::Unit      getXMin        () const;
      inline DbU::Unit      getXMax        () const;
      inline Interval       getXSpan       () const;
      inline Area*          getArea        () const;
      inline EtesianEngine* getEtesian     () const;
      inline size_t         getSpinSlice0  () const;
             bool           validate       () const;
      inline list<Tile>&    getTiles       ();
             void           merge          ( const Occurrence&, const Box& );
             void           addFeeds       ( size_t islice );
             void           fillHole       ( DbU::Unit xmin, DbU::Unit xmax, DbU::Unit ybottom, size_t yspin );
             void           buildSubSlices ();
             void           showSubSlices  ();
             void           insertTies     ( DbU::Unit latchUpMax );
             string         _getString     () const;
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


  bool  Slice::validate () const
  {
    if (_tiles.empty()) return true;

    bool validated = true;
    auto iTile     = _tiles.begin();
    auto iTileNext = iTile;
    ++iTileNext;
    while ( iTileNext != _tiles.end() ) {
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

    cerr << "+ Slice @" << DbU::getValueString(_ybottom) << endl;
    for ( const SubSlice& subSlice : _subSlices ) {
      size_t    count = 0;
      DbU::Unit avg   = subSlice.getAverageChunk( count );
      cerr << "| [" << DbU::getValueString(subSlice.getXMin())
           << " " << DbU::getValueString(subSlice.getXMax())
           << "] tiles:" << count
           << " avg:" << DbU::getValueString(avg)
           << " " << endl;
    }
  }


  void  Slice::insertTies ( DbU::Unit latchUpMax )
  {
    cerr << "Slice::insertTies() @" << DbU::getValueString(_ybottom) << endl;
    for ( SubSlice& subSlice : _subSlices ) subSlice.insertTies( latchUpMax );
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
                            Area           ( EtesianEngine* );
                           ~Area           ();
      inline EtesianEngine* getEtesian     () const;
      inline size_t         getSpinSlice0  () const;
      inline void           setSpinSlice0  ( size_t );
      inline DbU::Unit      getXMin        () const;
      inline DbU::Unit      getXMax        () const;
             bool           validate       () const;
             void           merge          ( const Occurrence&, const Box& );
             void           addFeeds       ();
             void           buildSubSlices ();
             void           showSubSlices  ();
             void           insertTies     ( DbU::Unit latchUpMax );
    private:
      EtesianEngine*  _etesian;
      Box             _cellAb;
      DbU::Unit       _sliceHeight;
      vector<Slice*>  _slices;
      size_t          _spinSlice0;
  };


  Area::Area ( EtesianEngine* etesian )
    : _etesian    (etesian)
    , _cellAb     (etesian->getBlockCell()->getAbutmentBox())
    , _sliceHeight(_etesian->getSliceHeight())
    , _slices     ()
  {
    size_t slicesNb = _cellAb.getHeight() / _sliceHeight;
    for ( size_t islice=0 ; islice<slicesNb ; ++islice )
      _slices.push_back( new Slice( this, _cellAb.getYMin()+islice*_sliceHeight ) );
  }


  Area::~Area ()
  {
    for ( size_t islice=0 ; islice<_slices.size() ; ++islice )
      delete _slices[islice];
  }


  inline EtesianEngine* Area::getEtesian    () const { return _etesian; }
  inline size_t         Area::getSpinSlice0 () const { return _spinSlice0; }
  inline void           Area::setSpinSlice0 ( size_t spinSlice0 ) { _spinSlice0 = spinSlice0; }
  inline DbU::Unit      Area::getXMin       () const { return _cellAb.getXMin(); }
  inline DbU::Unit      Area::getXMax       () const { return _cellAb.getXMax(); }


  bool  Area::validate () const
  {
    bool validated = true;
    for ( const Slice* slice : _slices )
      validated = validated and slice->validate();
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
    validate();
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
    Cell* feed = _slice->getEtesian()->getFeedCells().getBiggestFeed();
    if (feed == NULL) {
      cerr << Error( "SubSlice::insertTies(): No feed has been registered, ignoring." ) << endl;
      return;
    }
    DbU::Unit  feedWidth  = feed->getAbutmentBox().getWidth();

    cerr << "SubSlice::insterTies(): LatchUpMax:" << DbU::getValueString( latchUpMax ) << endl;
    cerr << "  Direct subSlice walkthrough." << endl;

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
      DbU::Unit dxLeft = (*iTile).getXMin() - leftPosition;
      if (dxLeft >= feedWidth) {
        cerr << "    Enough space, reset rigth shift." << endl;
        leftPosition = (*iTile).getXMin();
        tileLength   = 0;
      }
      
      if (tileLength + (*iTile).getWidth() > latchUpMax) {
        cerr << "    Length above threshold, insert tie space." << endl;
        leftPosition += feedWidth;
        tileLength    = 0;
      }

      (*iTile).translate( leftPosition - (*iTile).getXMin() );
      tileLength   += (*iTile).getWidth();
      leftPosition += (*iTile).getWidth();

      if (iTile == endTile) break;
      ++iTile;
    }

    cerr << "  Reverse subSlice walkthrough (feedWidth:" << DbU::getValueString(feedWidth) << ")" << endl;
    leftPosition = xMax;
    iTile        = endTile;
    if (iTile == _slice->getTiles().end()) --iTile;
    if ((*iTile).getXMax() > leftPosition) {
      tileLength = 0;
      DbU::Unit prevDxRight = (*iTile).getXMax() - leftPosition;
      while ( true ) {
        DbU::Unit dxRight = (*iTile).getXMax() - leftPosition;
        if (dxRight <= 0) {
          cerr << "    Negative dxRight, all shift has been compensated." << endl;
          break;
        }
        DbU::Unit leftHole = prevDxRight - dxRight;
        if (leftHole > feedWidth) {
          cerr << "    Absorbing shift (wide hole)" << endl;
          leftHole     -= feedWidth;
          leftPosition += leftHole;
          prevDxRight   = dxRight - leftHole;
          tileLength    = 0;
        } else if (leftHole != 0) {
          if (tileLength+(*iTile).getWidth() < latchUpMax) {
            cerr << "    Absorbing shift (full, below latchUp) delta leftPos:"
                 << DbU::getValueString(leftHole) << endl;
            leftHole = 0;
          } else {
            cerr << "    Keeping spacing." << endl;
            leftPosition -= leftHole;
            tileLength    = 0;
          }
          prevDxRight = dxRight - leftHole;
        } else {
          cerr << "    No change in shift." << endl;
        }
        
        cerr << "    Left position (+width) " << DbU::getValueString(leftPosition)
             << " width:" << DbU::getValueString((*iTile).getWidth()) << endl;
        (*iTile).translate( leftPosition - (*iTile).getXMax() );
        leftPosition -= (*iTile).getWidth();
        tileLength   += (*iTile).getWidth();

        if (iTile == _beginTile) break;
        --iTile;
      } 
    }
  }


  inline DbU::Unit       Slice::getYBottom    () const { return _ybottom; }
  inline DbU::Unit       Slice::getXMin       () const { return _area->getXMin(); }
  inline DbU::Unit       Slice::getXMax       () const { return _area->getXMax(); }
  inline Interval        Slice::getXSpan      () const { return Interval( getXMin(), getXMax() ); }
  inline Area*           Slice::getArea       () const { return _area; }
  inline EtesianEngine*  Slice::getEtesian    () const { return getArea()->getEtesian(); }
  inline size_t          Slice::getSpinSlice0 () const { return getArea()->getSpinSlice0(); }
  inline list<Tile>&     Slice::getTiles      () { return _tiles; }


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
      DbU::Unit tieSpacing = getConfiguration()->getLatchUpDistance()*2;
      Cell*     feed       = getFeedCells().getBiggestFeed();
      if (feed != NULL)
        tieSpacing -= feed->getAbutmentBox().getWidth();
      area.insertTies( tieSpacing );
    }
    area.addFeeds();

    UpdateSession::close();

    if (_viewer) _viewer->getCellWidget()->refresh();
  }


} // Etesian namespace.
