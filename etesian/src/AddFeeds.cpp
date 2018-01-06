// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AddFeeds.cpp"                           |
// +-----------------------------------------------------------------+


#include <map>
#include <list>
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
  using Hurricane::ForEachIterator;
  using Hurricane::Warning;
  using Hurricane::Error;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Interval;
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
// Class  :  "::SliceHoles".

  class SliceHoles;

  class Slice {
    public:
                             Slice             ( SliceHoles*, DbU::Unit ybottom, Interval xspan );
      inline DbU::Unit       getYBottom        () const;
      inline const Interval& getXSpan          () const;
      inline DbU::Unit       getXMin           () const;
      inline DbU::Unit       getXMax           () const;
      inline SliceHoles*     getSliceHoles     () const;
      inline EtesianEngine*  getEtesian        () const;
      inline size_t          getSpinSlice0     () const;
             void            merge             ( DbU::Unit source, DbU::Unit target );
             void            addFeeds          ( size_t islice );
             void            fillHole          ( DbU::Unit xmin, DbU::Unit xmax, DbU::Unit ybottom, size_t yspin );
             string          _getString        () const;
    private:
      SliceHoles*    _sliceHoles;
      DbU::Unit      _ybottom;
      Interval       _xspan;
      list<Interval> _chunks;
  };


  class SliceHoles {
    public:
                            SliceHoles    ( EtesianEngine* );
                           ~SliceHoles    ();
      inline EtesianEngine* getEtesian    () const;
      inline size_t         getSpinSlice0 () const;
      inline void           setSpinSlice0 ( size_t );
             void           merge         ( const Box& );
             void           addFeeds      ();
    private:
      EtesianEngine*  _etesian;
      Box             _cellAb;
      DbU::Unit       _sliceHeight;
      vector<Slice*>  _slices;
      size_t          _spinSlice0;
  };


  Slice::Slice ( SliceHoles* sliceHoles, DbU::Unit ybottom, Interval xspan )
    : _sliceHoles(sliceHoles)
    , _ybottom   (ybottom)
    , _xspan     (xspan)
    , _chunks    ()
  { }

  inline DbU::Unit       Slice::getYBottom    () const { return _ybottom; }
  inline DbU::Unit       Slice::getXMin       () const { return _xspan.getVMin(); }
  inline DbU::Unit       Slice::getXMax       () const { return _xspan.getVMax(); }
  inline const Interval& Slice::getXSpan      () const { return _xspan; }
  inline SliceHoles*     Slice::getSliceHoles () const { return _sliceHoles; }
  inline EtesianEngine*  Slice::getEtesian    () const { return getSliceHoles()->getEtesian(); }
  inline size_t          Slice::getSpinSlice0 () const { return getSliceHoles()->getSpinSlice0(); }


  void  Slice::merge ( DbU::Unit source, DbU::Unit target )
  {
    Interval chunkToMerge = _xspan.getIntersection( Interval(source,target) );
    cdebug_log(129,0) << "    Slice::merge() " << " " << chunkToMerge << endl;
    cdebug_log(129,0) << "    | " << _getString() << endl;

    if (chunkToMerge.isEmpty()) return;

    list<Interval>::iterator imerge = _chunks.end();
    list<Interval>::iterator ichunk = _chunks.begin();

    while ( ichunk != _chunks.end() ) {
      if (imerge == _chunks.end()) {
        if (chunkToMerge.getVMax() < (*ichunk).getVMin()) {
          cdebug_log(129,0) << "    | Insert before " << *ichunk << endl;
          imerge = _chunks.insert( ichunk, chunkToMerge );
          break;
        }

        if (chunkToMerge.intersect(*ichunk)) {
          cdebug_log(129,0) << "    | Merge with " << *ichunk << endl;
          imerge = ichunk;
          (*imerge).merge( chunkToMerge );
        }
      } else {
        if (chunkToMerge.getVMax() >= (*ichunk).getVMin()) {
          (*imerge).merge( *ichunk );
          cdebug_log(129,0) << "    | Absorb (erase) " << *ichunk << endl;
          ichunk = _chunks.erase( ichunk );
          continue;
        } else
          break;
      }

      ++ichunk;
    }

    if (imerge == _chunks.end()) {
      _chunks.insert( ichunk, chunkToMerge );
      cdebug_log(129,0) << "    | Insert at end " << DbU::getValueString(_ybottom) << " " << chunkToMerge << endl;
      cdebug_log(129,0) << "    | " << _getString() << endl;
    }
  }


  void  Slice::addFeeds ( size_t islice )
  {
    if (_chunks.empty()) {
      fillHole( getXMin(), getXMax(), getYBottom(), islice%2 );
      return;
    }

    list<Interval>::iterator ichunk     = _chunks.begin();
    list<Interval>::iterator ichunknext = ichunk;
    ++ichunknext;

  // Hole before the first chunk.
    if ((*ichunk).getVMin() > getXMin()) {
      fillHole( getXMin(), (*ichunk).getVMin(), getYBottom(), (islice+getSpinSlice0())%2 );
    }

    for ( ; ichunknext != _chunks.end() ; ++ichunk, ++ichunknext ) {
      fillHole( (*ichunk).getVMax(), (*ichunknext).getVMin(), getYBottom(), (islice+getSpinSlice0())%2 );
    }

  // Hole after the last chunk.
    if ((*ichunk).getVMax() < getXMax()) {
      fillHole( (*ichunk).getVMax(), getXMax(), getYBottom(), (islice+getSpinSlice0())%2 );
    }
  }


  void  Slice::fillHole ( DbU::Unit xmin, DbU::Unit xmax, DbU::Unit ybottom, size_t yspin )
  {
    Cell* feed = getEtesian()->getFeedCells().getBiggestFeed();
    if (feed == NULL) {
      cerr << Error("EtesianEngine: No feed has been registered, ignoring.") << endl;
      return;
    }

    DbU::Unit feedWidth = feed->getAbutmentBox().getWidth();
    DbU::Unit xtie      = xmin;

    while ( true ) {
      if (xtie           >= xmax) break;
      if (xtie+feedWidth >  xmax) {
      // Feed is too big, try to find a smaller one.
        int pitch = (int)((xmax-xtie) / getEtesian()->getVerticalPitch());
        for ( ; pitch > 0 ; --pitch ) {
          feed = getEtesian()->getFeedCells().getFeed( pitch );
          if (feed == NULL) continue;
          
          feedWidth = feed->getAbutmentBox().getWidth();
          if (feed != NULL) break;
        }
        if (feed == NULL) break;
      }

      Instance::create ( getEtesian()->getCell()
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


  string   Slice::_getString () const
  {
    ostringstream os;

    os << "<Slice " << " @" << DbU::getValueString(_ybottom) << " ";
    list<Interval>::const_iterator ichunk = _chunks.begin();
    for ( ; ichunk != _chunks.end() ; ++ichunk ) {
      if (ichunk != _chunks.begin()) os << " ";
      os << "[" << DbU::getValueString((*ichunk).getVMin())
         << " " << DbU::getValueString((*ichunk).getVMax()) << "]";
    }
    os << ">";
    return os.str();
  }


  SliceHoles::SliceHoles ( EtesianEngine* etesian )
    : _etesian    (etesian)
    , _cellAb     (etesian->getCell()->getAbutmentBox())
    , _sliceHeight(_etesian->getSliceHeight())
    , _slices     ()
  {
    size_t slicesNb = _cellAb.getHeight() / _sliceHeight;
    for ( size_t islice=0 ; islice<slicesNb ; ++islice )
      _slices.push_back( new Slice( this
                                  , _cellAb.getYMin()+islice*_sliceHeight
                                  , Interval(_cellAb.getXMin(),_cellAb.getXMax()) ) );
  }


  SliceHoles::~SliceHoles ()
  {
    for ( size_t islice=0 ; islice<_slices.size() ; ++islice )
      delete _slices[islice];
  }


  inline EtesianEngine* SliceHoles::getEtesian    () const { return _etesian; }
  inline size_t         SliceHoles::getSpinSlice0 () const { return _spinSlice0; }
  inline void           SliceHoles::setSpinSlice0 ( size_t spinSlice0 ) { _spinSlice0 = spinSlice0; }


  void   SliceHoles::merge ( const Box& bb )
  {
    if (bb.getYMin() < _cellAb.getYMin()) {
      cerr << Warning("Attempt to merge instance outside the Cell abutment box.") << endl;
      return;
    }

    size_t ibegin = (bb.getYMin()-_cellAb.getYMin()) / _sliceHeight;
    size_t iend   = (bb.getYMax()-_cellAb.getYMin()) / _sliceHeight;

    for ( size_t islice=ibegin ; islice<iend ; ++islice ) {
      _slices[islice]->merge( bb.getXMin(), bb.getXMax() );
    }
  }


  void  SliceHoles::addFeeds ()
  {
    for ( size_t islice=0 ; islice<_slices.size() ; islice++ )
      _slices[islice]->addFeeds( islice );
  }


} // End of anonymous namespace.


namespace Etesian {


  using Hurricane::ForEachIterator;
  using Hurricane::DataBase;
  using Hurricane::UpdateSession;
  using Hurricane::Occurrence;


  size_t  EtesianEngine::findYSpin ()
  {
    _ySpinSet     = false;
    _yspinSlice0  = 0;

    Box topCellAb = getCell()->getAbutmentBox();

    if (not topCellAb.isEmpty()) {
      forEach ( Occurrence, ioccurrence, getCell()->getLeafInstanceOccurrences() )
      {
        Instance*      instance       = static_cast<Instance*>((*ioccurrence).getEntity());
        Cell*          masterCell     = instance->getMasterCell();
        Box            instanceAb     = masterCell->getAbutmentBox();
        Transformation instanceTransf = instance->getTransformation();
  
        (*ioccurrence).getPath().getTransformation().applyOn( instanceTransf );
        instanceTransf.applyOn( instanceAb );
  
        if (not topCellAb.contains(instanceAb)) continue;
  
        _ySpinSet = true;
  
        int islice = (instanceAb.getYMin() - getCell()->getAbutmentBox().getYMin()) / getSliceHeight();
  
        switch ( instanceTransf.getOrientation() ) {
          case Transformation::Orientation::ID:
          case Transformation::Orientation::MX:
            _yspinSlice0 = (islice % 2);
            break;
          case Transformation::Orientation::R2:
          case Transformation::Orientation::MY:
            _yspinSlice0 = ((islice+1) % 2);
            break;
          case Transformation::Orientation::R1:
          case Transformation::Orientation::R3:
          case Transformation::Orientation::XR:
          case Transformation::Orientation::YR:
            cerr << Warning( "Instance %s has invalid transformation %s."
                           , getString(instance->getName()).c_str()
                           , getString(instanceTransf.getOrientation()).c_str()
                           ) << endl;
            _ySpinSet = false;
            break;
        }

        if (_ySpinSet) break;
      }
    }
  
    return _yspinSlice0;
  }


  void  EtesianEngine::addFeeds ()
  {
    if (not getFeedCells().feedNumbers()) {
      cerr << Warning( "No feed cells available, skipping." ) << endl;
      return;
    }

    UpdateSession::open();

    SliceHoles sliceHoles ( this );
    Box        topCellAb  = getCell()->getAbutmentBox();

    sliceHoles.setSpinSlice0( _yspinSlice0 );

    forEach ( Occurrence, ioccurrence, getCell()->getLeafInstanceOccurrences() )
    {
      Instance* instance     = static_cast<Instance*>((*ioccurrence).getEntity());
      Cell*     masterCell   = instance->getMasterCell();

      if (CatalogExtension::isFeed(masterCell)) {
        cerr << Warning( "Feed instance %s already present."
                       , getString(instance->getName()).c_str() ) << endl;
      }

      Box instanceAb = masterCell->getAbutmentBox();

      Transformation instanceTransf = instance->getTransformation();
      (*ioccurrence).getPath().getTransformation().applyOn( instanceTransf );
      instanceTransf.applyOn( instanceAb );

      if (not topCellAb.contains(instanceAb)) {
        cerr << Warning( "Instance %s is not fully enclosed in the top cell."
                       , getString(instance->getName()).c_str() ) << endl;
        continue;
      }

      sliceHoles.merge( instanceAb );
    }

    sliceHoles.addFeeds();

    UpdateSession::close();

    if (_viewer) _viewer->getCellWidget()->refresh();
  }


} // Kite namespace.
