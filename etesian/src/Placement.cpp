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


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/DataBase.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/DeepNet.h"
#include "hurricane/Plug.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/Path.h"
#include "hurricane/Library.h"
#include "hurricane/viewer/CellWidget.h"
#include "hurricane/viewer/CellViewer.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/ToolBox.h"
#include "etesian/EtesianEngine.h"


namespace Etesian {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Path;
  using Hurricane::Transformation;
  using Hurricane::DataBase;
  using Hurricane::Library;
  using Hurricane::Component;
  using Hurricane::DeepNet;
  using Hurricane::Plug;
  using Hurricane::RoutingPad;
  using Hurricane::Vertical;
  using Hurricane::Horizontal;
  using Hurricane::NetExternalComponents;
  using Hurricane::DebugSession;
  using Hurricane::UpdateSession;
  using CRL::AllianceFramework;
  using CRL::CatalogExtension;
  using CRL::getTransformation;
  using CRL::RoutingLayerGauge;
  using Constant::LayerGaugeType;
  using Constant::Direction;
  using Etesian::EtesianEngine;


// -------------------------------------------------------------------
// Class  :  "::Tile".


  Record* Tile::_getRecord () const
  {
    Record* record = new Record(getString(this));
    record->add( getSlot( "_occurrence",  _occurrence ) );
    record->add( DbU::getValueSlot( "_xMin" , &_xMin ) );
    record->add( DbU::getValueSlot( "_width", &_width ) );
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "::Slice".


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

  //list<Tile>::iterator imerge = _tiles.end();
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


  void  Slice::trackAvoid ( DbU::Unit xTrack )
  {
    cdebug_log(121,1) << "Slice::trackAvoid() " << this << " @" << DbU::getValueString(xTrack) << endl;
    for ( SubSlice& subSlice : _subSlices ) {
      if (subSlice.getXMax() == xTrack) break;
      if (subSlice.getXMax()  > xTrack) {
        subSlice.trackAvoid( xTrack );
        break;
      }
    }
    cdebug_tabw(121,-1);
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
      fillHole( _tiles.end(), getXMin(), getXMax(), getYBottom(), islice%2 );
      return;
    }

    list<Tile>::iterator itile     = _tiles.begin();
    list<Tile>::iterator itilenext = itile;
    ++itilenext;

  // Hole before the first chunk.
    if ((*itile).getXMin() > getXMin()) {
      fillHole( itile, getXMin(), (*itile).getXMin(), getYBottom(), (islice+getSpinSlice0())%2 );
    }

    while ( itilenext != _tiles.end() ) {
      fillHole( itilenext, (*itile).getXMax(), (*itilenext).getXMin(), getYBottom(), (islice+getSpinSlice0())%2 );
      itile = itilenext;
      ++itilenext;
    }

  // Hole after the last chunk.
    if ((*itile).getXMax() < getXMax()) {
      fillHole( _tiles.end(), (*itile).getXMax(), getXMax(), getYBottom(), (islice+getSpinSlice0())%2 );
    }
  }


  void  Slice::fillHole ( std::list<Tile>::iterator before
                        , DbU::Unit xmin
                        , DbU::Unit xmax
                        , DbU::Unit ybottom
                        , size_t yspin )
  {
    Cell* feed = getEtesian()->getFeedCells().getBiggestFeed();
    if (feed == NULL) {
      cerr << Error("Slice::fillHole(): No feed has been registered, ignoring.") << endl;
      return;
    }

    DbU::Unit xtie   = xmin;
    DbU::Unit modulo = (xmin - getXMin()) % getEtesian()->getSliceStep();
    if (modulo) {
      xtie += getEtesian()->getSliceStep() - modulo;
      // cerr << "Misaligned hole @" << yspin
      //      << " ybottom=" << DbU::getValueString(ybottom)
      //      << " xmin=" <<  DbU::getValueString(xmin)
      //      << " modulo=" <<  DbU::getValueString(modulo)
      //      << " getXMin()=" <<  DbU::getValueString(getXMin())
      //      << endl;
    }
    modulo = (xmax - getXMin()) % getEtesian()->getSliceStep();
    if (modulo) xmax -= modulo;

    DbU::Unit feedWidth = feed->getAbutmentBox().getWidth();
    while ( true ) {
      if (xtie           >= xmax) return;
      if (xtie+feedWidth >  xmax) {
      // Feed is too big, try to find a smaller one.
        feed = NULL;
        int pitch = (int)((xmax-xtie) / getEtesian()->getSliceStep());
        for ( ; pitch > 0 ; --pitch ) {
          feed = getEtesian()->getFeedCells().getFeed( pitch );
          if (feed == NULL) continue;
          
          feedWidth = feed->getAbutmentBox().getWidth();
          if (feed != NULL) break;
        }
        if (feed == NULL) break;
      }

      Point     blockPoint = getEtesian()->toBlock( Point(xtie,_ybottom) );
      Instance* instance   = Instance::create
        ( getEtesian()->getBlockCell()
        , getEtesian()->getFeedCells().getUniqueInstanceName().c_str()
        , feed
        , getTransformation( feed->getAbutmentBox()
                           , blockPoint.getX()
                           , blockPoint.getY()
                           , (yspin) ? Transformation::Orientation::MY
                                     : Transformation::Orientation::ID
                           )
        , Instance::PlacementStatus::PLACED
        );
      _tiles.insert( before
                   , Tile( xtie
                         , feed->getAbutmentBox().getWidth()
                         , getEtesian()->toCell( Occurrence(instance) )));
      xtie += feedWidth;
    }

    Cell* tie = getEtesian()->getFeedCells().getTie();
    feedWidth = 0;
    if (tie) {
      DbU::Unit feedWidth = tie->getAbutmentBox().getWidth();
      if (xtie+feedWidth < xmax) {
        Point     blockPoint = getEtesian()->toBlock( Point(xtie,_ybottom) );
        Instance* instance   = Instance::create
          ( getEtesian()->getBlockCell()
          , getEtesian()->getFeedCells().getUniqueInstanceName().c_str()
          , tie
          , getTransformation( tie->getAbutmentBox()
                             , blockPoint.getX()
                             , blockPoint.getY()
                             , (yspin) ? Transformation::Orientation::MY
                             : Transformation::Orientation::ID
                             )
          , Instance::PlacementStatus::PLACED
          );
        _tiles.insert( before
                     , Tile( xtie
                           , tie->getAbutmentBox().getWidth()
                           , getEtesian()->toCell( Occurrence(instance) )));
        xtie += feedWidth;
      }

      if (xtie+feedWidth < xmax) {
        Point     blockPoint = getEtesian()->toBlock( Point(xmax-feedWidth,_ybottom) );
        Instance* instance   = Instance::create
          ( getEtesian()->getBlockCell()
          , getEtesian()->getFeedCells().getUniqueInstanceName().c_str()
          , tie
          , getTransformation( tie->getAbutmentBox()
                             , blockPoint.getX()
                             , blockPoint.getY()
                             , (yspin) ? Transformation::Orientation::MY
                             : Transformation::Orientation::ID
                             )
          , Instance::PlacementStatus::PLACED
          );
        _tiles.insert( before
                     , Tile( xmax-feedWidth
                           , tie->getAbutmentBox().getWidth()
                           , getEtesian()->toCell( Occurrence(instance) )));
        xmax -= feedWidth;
        before--;
      }
    } else {
      cerr << Error("Slice::fillHole(): No tie has been registered, not inserting.") << endl;
    }
  }

  
  Instance* Slice::createDiodeUnder ( RoutingPad* rp, const Box& diodeArea, DbU::Unit xHint )
  {
    Cell* diode = getEtesian()->getDiodeCell();
    if (diode == NULL) {
      cerr << Error("Slice::createDiodeUnder(): No diode cell has been registered, ignoring.") << endl;
      return NULL;
    }
    Cell* feed  = getEtesian()->getFeedCells().getBiggestFeed();
    if (feed == NULL) {
      cerr << Error("Slice::createDiodeUnder(): No feed has been registered, ignoring.") << endl;
      return NULL;
    }
    Cell* tie = getEtesian()->getFeedCells().getTie();
    if (not feed and not tie) {
      cerr << Error("Slice::createDiodeUnder(): No tie has been registered, ignoring.") << endl;
      return NULL;
    }

    cdebug_log(147,0) << "Slice::createDiodeUnder(): xHint=" << DbU::getValueString(xHint) << endl;
    cdebug_log(147,0) << "  rp=" << rp << endl;
    cdebug_log(147,0) << "  diodeArea=" << diodeArea << endl;

    Instance* blockInst      = getEtesian()->getBlockInstance();
    Instance* diodeInst      = NULL;
    bool      foundCandidate = false;
    auto      iCandidate     = _tiles.begin();
    DbU::Unit dCandidate     = 0;
    for ( auto iTile=_tiles.begin() ; iTile != _tiles.end() ; ++iTile ) {
      if ((*iTile).getXMax() <= diodeArea.getXMin()) continue;
      if ((*iTile).getXMin() <  diodeArea.getXMin()) continue;
      if ((*iTile).getXMin() >= diodeArea.getXMax()) break;
      cdebug_log(147,0) << "| " << (*iTile) << endl;
      if (   ((*iTile).getMasterCell() != feed)
         and ((*iTile).getMasterCell() != tie )) continue;
      if (blockInst) {
        if ((*iTile).getOccurrence().getPath().getHeadInstance() != blockInst) {
          cdebug_log(147,0) << "> Reject, not in block instance" << endl;
          continue;
        }
      }
      DbU::Unit distance = std::abs( (*iTile).getXMin() - xHint );
      if (not foundCandidate or (distance < dCandidate)) {
        cdebug_log(147,0) << "> candidate" << endl;
        foundCandidate = true;
        iCandidate     = iTile;
        dCandidate     = distance;
      }
    }
    
    if (not foundCandidate) return NULL;

    auto before = iCandidate;
    before++;

    DbU::Unit  xmin        = (*iCandidate).getXMin(); 
    DbU::Unit  width       = (*iCandidate).getWidth(); 
    DbU::Unit  fillerWidth = width - diode->getAbutmentBox().getWidth();
    diodeInst = (*iCandidate).getInstance();
    Transformation refTransf = diodeInst->getTransformation();
    _tiles.erase( iCandidate );
    diodeInst->destroy();

    Occurrence      rpOccurrence = rp->getPlugOccurrence();
    Path            instancePath = rpOccurrence.getPath();
    Transformation  transf       = refTransf;
    if (instancePath.isEmpty())
      transf = getEtesian()->toCell( transf );
    // transf = getEtesian()->toBlock( transf );
    // if (instancePath.isEmpty() and blockInst) {
    //   Transformation blockTransf = blockInst->getTransformation();
    //   blockTransf.applyOn( transf );
    // }
    Plug* plug = static_cast<Plug*>( rpOccurrence.getEntity() );
    diodeInst = Instance::create( plug->getCell()
                                , getEtesian()->getUniqueDiodeName()
                                , diode
                                , transf
                                , Instance::PlacementStatus::FIXED );
    _tiles.insert( before, Tile(xmin,width,Occurrence(diodeInst,instancePath)) );
    cdebug_log(147,0) << "  " << diodeInst << " @" << transf << endl;

    if (fillerWidth > 0) {
      Cell* filler = getEtesian()->getFeedCells().getFeedByWidth( fillerWidth );
      if (not filler) {
        cerr << Error("Slice::createDiodeUnder(): No gap filler of width %s."
                     , DbU::getValueString(fillerWidth).c_str() ) << endl;
        return diodeInst;
      }
      transf = refTransf;
      Transformation( fillerWidth, 0 ).applyOn( transf );
      Instance* fillerInst = Instance::create( getEtesian()->getBlockCell()
                                             , getEtesian()->getFeedCells().getUniqueInstanceName().c_str()
                                             , filler
                                             , transf
                                             , Instance::PlacementStatus::FIXED );
      _tiles.insert( before, Tile(xmin+width,fillerWidth,Occurrence(fillerInst,instancePath)) );
      cdebug_log(147,0) << "  " << fillerInst << " @" << transf << endl;
    }

    return diodeInst;
  }


  Record* Slice::_getRecord () const
  {
    Record* record = new Record(getString(this));
    record->add( getSlot( "_area"      ,  _area      ) );
    record->add( DbU::getValueSlot( "_ybottom", &_ybottom ) );
    record->add( getSlot( "_tiles"     , &_tiles     ) );
    record->add( getSlot( "_subSlices" , &_subSlices ) );
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "::Area".


  Area::Area ( EtesianEngine* etesian )
    : _etesian    (etesian)
    , _tieLut     ()
    , _placeArea    (etesian->getPlaceArea())
    , _sliceHeight(_etesian->getSliceHeight())
    , _slices     ()
  {
    if (etesian->getBlockInstance()) {
      _placeArea = etesian->toCell( etesian->getPlaceArea() );
    }
       
    size_t slicesNb = _placeArea.getHeight() / _sliceHeight;
    for ( size_t islice=0 ; islice<slicesNb ; ++islice )
      _slices.push_back( new Slice( this, _placeArea.getYMin()+islice*_sliceHeight ) );

    Cell* feed = getEtesian()->getFeedCells().getBiggestFeed();
    if (feed) {
      Library* library = feed->getLibrary();
      if (feed->getLibrary()->getName() == "FlexLib") {
        cmess2 << "  o  Using hard-coded tie/cell datas suited for \"FlexLib\"." << endl;
        Cell* masterCell = library->getCell( "sff1_x4" );
        if (masterCell) {
          _tieLut.addTieDatas( masterCell, DbU::fromMicrons(1.28), DbU::fromMicrons(11.7) );
        }
        masterCell = library->getCell( "dio_x0" );
        if (masterCell) {
          _tieLut.addTieDatas( masterCell, DbU::fromMicrons(6.5), DbU::fromMicrons(6.5) );
        }
      }
    }
  }


  Area::~Area ()
  {
    for ( size_t islice=0 ; islice<_slices.size() ; ++islice )
      delete _slices[islice];
  }


  bool  Area::validate ( DbU::Unit latchUpMax ) const
  {
    bool validated = true;
    for ( const Slice* slice : _slices )
      validated = slice->validate(latchUpMax) and validated;
    return validated;
  }


  void   Area::merge ( const Occurrence& occurrence, const Box& flatAb )
  {
    if (flatAb.getYMin() < _placeArea.getYMin()) {
      cerr << Warning("Area::merge(): Attempt to merge instance outside the placement area.") << endl;
      return;
    }

    DbU::Unit modulo = (flatAb.getXMin() - getXMin()) % getEtesian()->getSliceStep();
    if (modulo) {
      cerr << Warning( "Area::merge(): Misaligned instance %s\n"
                       "           y=%s (%d) x=%d (%d) area.getXMin()=%s sliceStep=%s (%d)"
                     , occurrence.getCompactString().c_str()
                     , DbU::getValueString(flatAb.getYMin()).c_str(), flatAb.getYMin()
                     , DbU::getValueString(flatAb.getXMin()).c_str(), flatAb.getXMin()
                     , DbU::getValueString(getXMin()).c_str()
                     , DbU::getValueString(getEtesian()->getSliceStep()).c_str()
                     , getEtesian()->getSliceStep()
                     ) << endl;
    }

    size_t ibegin = (flatAb.getYMin()-_placeArea.getYMin()) / _sliceHeight;
    size_t iend   = (flatAb.getYMax()-_placeArea.getYMin()) / _sliceHeight;
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


  void  Area::trackAvoid ( const Box& trackAvoid )
  {
    cdebug_log(121,1) << "Area::trackAvoid() over " << trackAvoid << endl;
    Box areaTrackAvoid = _placeArea.getIntersection( trackAvoid );
    if (areaTrackAvoid.isEmpty()) {
      cerr << Warning( "Area::trackAvoid(): Track avoid area %s fully outside placement area %s."
                     , getString(trackAvoid).c_str()
                     , getString(_placeArea).c_str()
                     ) << endl;
      return;
    }

    DbU::Unit xTrack = areaTrackAvoid.getXCenter();
    size_t    ibegin = (areaTrackAvoid.getYMin()-_placeArea.getYMin()) / _sliceHeight;
    size_t    iend   = (areaTrackAvoid.getYMax()-_placeArea.getYMin()) / _sliceHeight + 1;
    if (iend and (iend >= _slices.size())) --iend;
    for ( size_t islice=ibegin ; islice<iend ; ++islice ) {
      _slices[islice]->trackAvoid( xTrack );
    }
    cdebug_tabw(121,-1);
  }


  void  Area::insertTies ( DbU::Unit latchUpMax )
  {
    for ( Slice* slice : _slices ) slice->insertTies( latchUpMax );
    validate( latchUpMax );
  }


  Record* Area::_getRecord () const
  {
    Record* record = new Record(getString(this));
    record->add( getSlot( "_etesian"    ,  _etesian ) );
    record->add( getSlot( "_tieLut"     , &_tieLut  ) );
    record->add( getSlot( "_placeArea"    ,  _placeArea  ) );
    record->add( DbU::getValueSlot( "_sliceHeight", &_sliceHeight ) );
    record->add( getSlot( "_slices"     , &_slices     ) );
    record->add( getSlot( "_spinSlice0" ,  _spinSlice0 ) );
    return record;
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


  size_t  SubSlice::getUsedVTracks ( const Tile& tile, set<DbU::Unit>& vtracks )
  {
    RoutingGauge* rg     = _slice->getArea()->getEtesian()->getGauge();
    DbU::Unit     vpitch = _slice->getArea()->getEtesian()->getSliceStep();
    Cell* cell = tile.getMasterCell();
    for ( Net* net : cell->getNets() ) {
      if (not net->isExternal()) {
        for ( Component* component : net->getComponents() ) {
          if (not component->getLayer()->isBlockage()) continue;
          cdebug_log(121,0) << "Looking at " << component << endl;
          Vertical* v = dynamic_cast<Vertical*>( component );
          if (   not dynamic_cast<Vertical*  >(component)
             and not dynamic_cast<Horizontal*>(component)) continue;

          RoutingLayerGauge* rlg = rg->getLayerGauge( component->getLayer()->getRoutingLayer() );
          if (not rlg) continue;
          if (rlg->getType()      != LayerGaugeType::Default) continue;
          if (rlg->getDirection() != Direction::Vertical) continue;

          Box bb = component->getBoundingBox();
          bb.inflate( -rlg->getWireWidth()/2, 0 );
          Transformation transf = tile.getInstance()->getTransformation();
          tile.getOccurrence().getPath().getTransformation().applyOn( transf );
          transf.applyOn( bb );
          cdebug_log(121,0) << "Obstacle bb " << bb << endl;
          DbU::Unit xtrack = bb.getXMin() - (bb.getXMin() % vpitch);
          for ( ; xtrack <= bb.getXMax() ; xtrack += vpitch ) {
            cdebug_log(121,0) << "| add vtrack @" << DbU::getValueString(xtrack) << endl;
            vtracks.insert( xtrack );
          }
        }
        continue;
      }
      if (net->isPower() or net->isGround()) continue;
      for ( Component* component : NetExternalComponents::get(net) ) {
        Vertical* v = dynamic_cast<Vertical*>( component );
        if (not v) continue;

        Transformation transf = tile.getInstance()->getTransformation();
        tile.getOccurrence().getPath().getTransformation().applyOn( transf );
        Point center = transf.getPoint( v->getBoundingBox().getCenter() );
        if (center.getX() % vpitch) {
          cerr << Error( "Slice::getUsedVTracks(): Misaligned terminal %s.\n"
                         "        (in %s)"
                       , getString(v).c_str()
                       , getString(tile.getOccurrence()).c_str()
                       ) << endl;
        }
        vtracks.insert( center.getX() );
      }
    }
    return vtracks.size();
  }


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


  void  SubSlice::trackAvoid ( DbU::Unit xTrack )
  {
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

    auto iTile = _beginTile;
    while ( true ) {
      Instance* instance = (*iTile).getInstance();

      if ((*iTile).getXMin() >= xTrack) break;
      if ((*iTile).getXMax() == xTrack) break;
      if ((*iTile).getXMax()  > xTrack) {
        cdebug_log(121,0) << instance->getName() << " accross V-Track @" << DbU::getValueString(xTrack) << endl;
        set<DbU::Unit> usedVTracks;
        getUsedVTracks( *iTile, usedVTracks );
        for ( DbU::Unit x : usedVTracks )
          cdebug_log(121,0) << "| V-Track @" << DbU::getValueString(x) << endl;
        if (not usedVTracks.count(xTrack)) break;

        DbU::Unit vpitch     = _slice->getArea()->getEtesian()->getSliceStep();
        DbU::Unit maxDxLeft  = 0;
        DbU::Unit maxDxRight = 0;
        if (iTile != beginTile) {
          auto prevTile = iTile;
          --prevTile;
          maxDxLeft = (*prevTile).getXMax() - (*iTile).getXMin();
        }
        if (iTile != endTile) {
          auto nextTile = iTile;
          ++nextTile;
          maxDxRight = (*nextTile).getXMin() - (*iTile).getXMax();
        }
        cdebug_log(121,0) << "maxDxLeft =" << DbU::getValueString(maxDxLeft ) << endl;
        cdebug_log(121,0) << "maxDxRight=" << DbU::getValueString(maxDxRight) << endl;
        DbU::Unit xShift = 0;
        for ( ; xShift >= maxDxLeft ; xShift -= vpitch ) {
          cdebug_log(121,0) << "| try left shift " << DbU::getValueString(xShift) << endl;
          if (not usedVTracks.count( xTrack - xShift )) break;
        }
        if (xShift < maxDxLeft) {
          xShift = vpitch;
          for ( ; xShift <= maxDxRight ; xShift += vpitch ) {
            cdebug_log(121,0) << "| try right shift " << DbU::getValueString(xShift) << endl;
            if (not usedVTracks.count( xTrack - xShift )) break;
          }
        }
        if (xShift > maxDxRight) {
          cerr << Error( "SubSlice::trackAvoid(): Unable to put out of the way %s."
                       , getString(*iTile).c_str()
                       ) << endl;
          break;
        }
        cdebug_log(121,0) << "Shifting " << (*iTile) << " by " << DbU::getValueString(-xShift) << endl;
        (*iTile).translate( xShift );
        
        break;
      }
    
      if (iTile == endTile) break;
      ++iTile;
    }
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

  
  Instance* Area::createDiodeUnder ( RoutingPad* rp, const Box& diodeArea, DbU::Unit xHint )
  {
  //Transformation toBlockTransf = getEtesian()->getBlockInstance()->getTransformation();
  //toBlockTransf.invert();
    Box blockDiodeArea ( diodeArea );
  //toBlockTransf.applyOn( blockDiodeArea );
  //xHint += toBlockTransf.getTx();
    
    DbU::Unit y = blockDiodeArea.getYCenter();

    if ((y < _placeArea.getYMin()) or  (y >= _placeArea.getYMax())) return NULL;
    if (not blockDiodeArea.intersect(_placeArea)) return NULL;

    size_t islice = (y - _placeArea.getYMin()) / _sliceHeight;
    return _slices[islice]->createDiodeUnder( rp, blockDiodeArea, xHint );
  }


// -------------------------------------------------------------------
// Class  :  "::EtesianEngine".

  void  EtesianEngine::toHurricane ()
  {
    Breakpoint::stop( 101, "EtesianEngine::toHurricane() called." );
    
    if (not getFeedCells().feedNumbers()) {
      cerr << Warning( "EtesianEngine::readSlices(): No feed cells available, skipping." ) << endl;
      return;
    }

  //Breakpoint::stop( 100, "Before adding feeds." );
    cmess2 << "  o  Adding feed cells." << endl;

  //DebugSession::open( 145, 150 );
    UpdateSession::open();

    if (_area) delete _area;
    _area = new Area ( this );
    Box topPlaceArea = _area->getPlaceArea();

    _area->setSpinSlice0( _yspinSlice0 );

    if (getBlockInstance()) {
      for ( Instance* instance : getCell()->getInstances() ) {
        if (instance == getBlockInstance()) continue;
        if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED) {
          Box overlapAb = instance->getAbutmentBox();
          overlapAb = topPlaceArea.getIntersection( overlapAb );
          if (not overlapAb.isEmpty()) {
            _area->merge( Occurrence(instance), overlapAb );
          }
        }
      }
    }

    for ( Occurrence occurrence : getBlockCell()->getTerminalNetlistInstanceOccurrences() )
    {
      Instance* instance     = static_cast<Instance*>( occurrence.getEntity() );
      Cell*     masterCell   = instance->getMasterCell();

      if (CatalogExtension::isFeed(masterCell)) {
        cerr << Warning( "EtesianEngine::readSlices(): Feed instance %s already present."
                       , getString(instance->getName()).c_str() ) << endl;
      }

      Box instanceAb = instance->getAbutmentBox();
      Occurrence cellOccurrence = toCell( occurrence );
      cellOccurrence.getPath().getTransformation().applyOn( instanceAb );

      if (not topPlaceArea.intersect(instanceAb)) {
        if (instance->getPlacementStatus() != Instance::PlacementStatus::FIXED)
          cerr << Warning( "EtesianEngine::readSlices(): Instance %s is not fully enclosed in the top cell.\n"
                           "          * topPlaceArea=%s\n"
                           "          * instanceAb=%s cell=%s"
                         , getString(instance->getName()).c_str()
                         , getString(topPlaceArea).c_str()
                         , getString(instanceAb).c_str()
                         , getString(instance->getCell()).c_str()
                         ) << endl;
        continue;
      }

      if (not topPlaceArea.contains(instanceAb)) {
        if (instance->getPlacementStatus() != Instance::PlacementStatus::FIXED)
          cerr << Warning( "EtesianEngine::readSlices(): Instance %s is not fully enclosed in the top cell.\n"
                           "          * topPlaceArea=%s\n"
                           "          * instanceAb=%s cell=%s"
                         , getString(instance->getName()).c_str()
                         , getString(topPlaceArea).c_str()
                         , getString(instanceAb).c_str()
                         , getString(instance->getCell()).c_str()
                         ) << endl;
        instanceAb = topPlaceArea.getIntersection( instanceAb );
      }

      _area->merge( cellOccurrence, instanceAb );
    }

    _area->buildSubSlices();
    _area->showSubSlices();
    for ( const Box& trackAvoid : _trackAvoids )
      _area->trackAvoid( trackAvoid );
#if DISABLED_TIE_INSERTION
    if (getConfiguration()->getLatchUpDistance()) {
      Cell*     feed       = getFeedCells().getBiggestFeed();
      DbU::Unit tieSpacing = getConfiguration()->getLatchUpDistance()*2 - feed->getAbutmentBox().getWidth();
      if (feed != NULL)
        tieSpacing -= feed->getAbutmentBox().getWidth();
      _area->insertTies( tieSpacing );
    }
#endif
    _area->addFeeds();

    UpdateSession::close();
  //DebugSession::close();

    if (_viewer) _viewer->getCellWidget()->refresh();

    Breakpoint::stop( 101, "EtesianEngine::toHurricane() finished." );
  }


} // Etesian namespace.
