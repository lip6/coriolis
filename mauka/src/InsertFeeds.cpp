// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./InsertFeeds.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/UpdateSession.h"
#include "crlcore/ToolBox.h"
#include "mauka/MaukaEngine.h"


namespace {

  using namespace std;
  using namespace Mauka;
  using namespace Hurricane;
  using namespace CRL;


// -------------------------------------------------------------------
// Class  :  "::RowFeed"

  class SurfaceFeeds;


  class RowFeed {
    public:
                           RowFeed           ( SurfaceFeeds*, const Box& bb );
      inline const Box&    getBox            () const;
      inline SurfaceFeeds* getSurfaceFeeds   ();
      inline MaukaEngine*  getMauka          ();
      inline void          setOrientation    ( Transformation::Orientation );
             void          mergeInstanceSpan ( DbU::Unit source, DbU::Unit target );
             void          insertTieChunk    ( DbU::Unit  xmin
                                             , DbU::Unit  xmax
                                             , DbU::Unit  y
                                             , const Transformation::Orientation& );
             void          insertFeeds       ();
    private:
      SurfaceFeeds*                _surface;
      Box                          _boundingBox;
      Transformation::Orientation  _orientation;
      list<Interval>               _instanceSpans;
  };


// -------------------------------------------------------------------
// Class  :  "::SurfaceFeeds"


  class SurfaceFeeds {
    public:
                           SurfaceFeeds          ( MaukaEngine* );
                          ~SurfaceFeeds          ();
      inline MaukaEngine*  getMauka              ();
             void          addInstanceOccurrence ( Occurrence );
             void          insertFeeds           ();
    private:
             void          _orientRows           ( Transformation::Orientation );
    private:
      MaukaEngine*      _mauka;
      Box               _area;
      vector<RowFeed*>  _rowFeeds;
      bool              _oriented;
  };


// -------------------------------------------------------------------
// Class Implantation  :  "::RowFeed"


  RowFeed::RowFeed ( SurfaceFeeds* surface, const Box& bb )
    : _surface      (surface)
    , _boundingBox  (bb)
    , _orientation  (Transformation::Orientation::ID)
    , _instanceSpans()
  { }


  inline const Box&   RowFeed::getBox   () const { return _boundingBox; }
  inline MaukaEngine* RowFeed::getMauka () { return _surface->getMauka(); }

  inline void  RowFeed::setOrientation ( Transformation::Orientation orientation )
  { _orientation = orientation; }


  void  RowFeed::mergeInstanceSpan ( DbU::Unit source, DbU::Unit target )
  {
    Interval spanMerge ( source, target );

    list<Interval>::iterator imerge = _instanceSpans.end();
    list<Interval>::iterator ispan  = _instanceSpans.begin();

    while ( ispan != _instanceSpans.end() ) {
      if ( spanMerge.getVMax() < (*ispan).getVMin() ) break;

      if ( spanMerge.intersect(*ispan) ) {
        if ( imerge == _instanceSpans.end() ) {
          imerge = ispan;
          (*imerge).merge ( spanMerge );
        } else {
          (*imerge).merge ( *ispan );
          ispan = _instanceSpans.erase ( ispan );
          continue;
        }
      }
      ispan++;
    }

    if ( imerge == _instanceSpans.end() ) {
      _instanceSpans.insert ( ispan, spanMerge );
    }
  }


  void  RowFeed::insertTieChunk ( DbU::Unit xmin
                                , DbU::Unit xmax
                                , DbU::Unit y
                                , const Transformation::Orientation& orientation )
  {
    Cell* feed = getMauka()->getFeedCells().getBiggestFeed();
    if ( feed == NULL ) {
      cerr << Error("No feed has been registered, ignoring.") << endl;
    }

    DbU::Unit feedWidth = feed->getAbutmentBox().getWidth();
    DbU::Unit xtie      = xmin;

    while ( true ) {
      if ( xtie           >= xmax ) break;
      if ( xtie+feedWidth >  xmax ) {
      // Feed is too big, try to find a smaller one.
        int pitch = (int)((xmax-xtie) / getMauka()->getPitch());
        for ( ; pitch > 0 ; --pitch ) {
          feed      = getMauka()->getFeedCells().getFeed ( pitch );
          feedWidth = feed->getAbutmentBox().getWidth();
          if ( feed != NULL ) break;
        }
        if ( feed == NULL ) break;
      }

      Instance::create ( getMauka()->getCell()
                       , getMauka()->getFeedCells().getNewFeedName().c_str()
                       , feed
                       , getTransformation(feed->getAbutmentBox(),xtie,y,orientation)
                       , Instance::PlacementStatus::PLACED
                       );

      xtie += feedWidth;
    }
  }


  void  RowFeed::insertFeeds ()
  {
    list<Interval>::iterator ispan  = _instanceSpans.begin();
    
    DbU::Unit minFeed = _boundingBox.getXMin();
    DbU::Unit maxFeed;
    if ( ispan == _instanceSpans.end() ) {
      maxFeed = _boundingBox.getXMax();
      insertTieChunk ( minFeed, maxFeed, _boundingBox.getYMin(), _orientation );
      return;
    }

    maxFeed = (*ispan).getVMin();
    insertTieChunk ( minFeed, maxFeed, _boundingBox.getYMin(), _orientation );

    while ( ispan != _instanceSpans.end() ) {
      minFeed = (*ispan).getVMax();

      if ( ++ispan != _instanceSpans.end() )
        maxFeed = (*ispan).getVMin();
      else
        maxFeed = _boundingBox.getXMax();

      insertTieChunk ( minFeed, maxFeed, _boundingBox.getYMin(), _orientation );
    }
  }


// -------------------------------------------------------------------
// Class Implantation  :  "::SurfaceFeeds"


  SurfaceFeeds::SurfaceFeeds ( MaukaEngine* mauka )
    : _mauka   (mauka)
    , _area    (mauka->getCell()->getAbutmentBox())
    , _rowFeeds()
    , _oriented(false)
  {
    DbU::Unit sliceHeight = _mauka->getSliceHeight();
    size_t    rowsNb      = _area.getHeight() / sliceHeight;

    _rowFeeds.reserve ( rowsNb );
    for ( size_t irow=0 ; irow<rowsNb ; ++irow )
      _rowFeeds.push_back ( new RowFeed(this,Box(_area.getXMin()
                                                , irow    * sliceHeight
                                                ,_area.getXMax()
                                                ,(irow+1) * sliceHeight )) );
  }


  SurfaceFeeds::~SurfaceFeeds ()
  {
    for ( size_t irow=0 ; irow<_rowFeeds.size() ; ++irow )
      delete _rowFeeds[irow];
  }


  inline MaukaEngine* SurfaceFeeds::getMauka () { return _mauka; }


  void  SurfaceFeeds::addInstanceOccurrence ( Occurrence occurrence )
  {
    Instance* instance = dynamic_cast<Instance*>(occurrence.getEntity());
    if ( instance == NULL ) {
      cerr << Error("Entity of occurrence %s is not an instance."
                   ,getString(occurrence).c_str()) << endl;
      return;
    }

    DbU::Unit      sliceHeight    = _mauka->getSliceHeight();
    Box            masterABox     = instance->getAbutmentBox();
    Transformation transformation = occurrence.getPath().getTransformation();

    transformation.applyOn(masterABox);
    Transformation::Orientation orientation = occurrence.getPath().getTransformation().getOrientation();
    int rowStart = (masterABox.getYMin() - _area.getYMin()) / sliceHeight;
    int rowStop  = (masterABox.getYMax() - _area.getYMin()) / sliceHeight;

    if ( (rowStart < 0) or (rowStop > (int)_rowFeeds.size()) ) {
      cerr << Error("Instance %s (AB:%s) is outside the placement area."
                   ,getString(occurrence).c_str()
                   ,getString(masterABox).c_str()
                   ) << endl;
      return;
    }

    if ( not _oriented ) {
      size_t instanceRowMY = 0;
      switch ( orientation ) {
        case Transformation::Orientation::ID: instanceRowMY = 0; break;
        default:
        case Transformation::Orientation::MY: instanceRowMY = 1; break;
      }
      
      Transformation::Orientation rowZeroOrientation = Transformation::Orientation::ID;
      switch ( (rowStart+instanceRowMY) % 2 ) {
        case 0: orientation = Transformation::Orientation::ID; break;
        case 1: orientation = Transformation::Orientation::MY; break;
      }

      _orientRows ( rowZeroOrientation );
    }

    for ( size_t irow = rowStart ; (int)irow<rowStop ; ++irow ) {
      _rowFeeds[irow]->mergeInstanceSpan ( masterABox.getXMin(), masterABox.getXMax() );
    }
  }


  void  SurfaceFeeds::insertFeeds ()
  {
    for ( size_t irow=0 ; irow<_rowFeeds.size() ; ++irow )
      _rowFeeds[irow]->insertFeeds ();
  }


  void  SurfaceFeeds::_orientRows ( Transformation::Orientation rowZeroOrientation )
  {
    if ( _oriented ) return;
    _oriented = true;

    size_t rowZeroMY = 0;
    switch ( rowZeroOrientation ) {
      case Transformation::Orientation::ID: rowZeroMY = 0; break;
      default:
      case Transformation::Orientation::MY: rowZeroMY = 1; break;
    }

    for ( size_t irow=0 ; irow<_rowFeeds.size() ; ++irow ) {
      Transformation::Orientation orientation = Transformation::Orientation::ID;
      switch ( (irow+rowZeroMY) % 2 ) {
        case 0: orientation = Transformation::Orientation::ID; break;
        case 1: orientation = Transformation::Orientation::MY; break;
      }

      _rowFeeds[irow]->setOrientation(orientation);
    }
  }


} // End of anonymous namespace.


namespace Mauka {

  using Hurricane::UpdateSession;


  void  MaukaEngine::insertFeeds ()
  {
    UpdateSession::open ();

    SurfaceFeeds surfaceFeeds ( this );

    forEach ( Occurrence, ioccurrence, getCell()->getLeafInstanceOccurrences() )
    {
      surfaceFeeds.addInstanceOccurrence ( *ioccurrence );
    }
    surfaceFeeds.insertFeeds ();

    UpdateSession::close ();
  }


}  // End of Mauka namespace.
