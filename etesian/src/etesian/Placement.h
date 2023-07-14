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
// |  C++ Header  :       "./Placement.h"                            |
// +-----------------------------------------------------------------+


#pragma  once
#include <map>
#include <list>
#include "hurricane/Box.h"
#include "hurricane/Interval.h"
#include "hurricane/Instance.h"
#include "hurricane/Occurrence.h"
namespace Hurricane {
    class RoutingPad;
}


namespace Etesian {

  using Hurricane::Error;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Interval;
  using Hurricane::DBo;
  using Hurricane::Cell;
  using Hurricane::RoutingPad;
  using Hurricane::Instance;
  using Hurricane::Transformation;
  using Hurricane::Occurrence;
  class EtesianEngine;


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
      std::map< Cell*, TieDatas, DBo::CompareById >  _tieLut;
  };

  
  inline TieLUT::TieLUT ()
    : _tieLut()
  { }


  inline void  TieLUT::addTieDatas ( Cell* cell, DbU::Unit leftDistance, DbU::Unit rightDistance )
  {
    const TieDatas* datas = getTieDatas( cell );
    if (datas) {
      std::cerr << Error( "TieLUT::addTieDatas(): Duplicate datas for % (ignoreds)."
                        , getString(cell).c_str()
                        ) << std::endl;
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
      inline                   Tile          ( const Tile& );
      inline                  ~Tile          ();
      inline       bool        isFixed       () const;
      inline       DbU::Unit   getXMin       () const;
      inline       DbU::Unit   getXMax       () const;
      inline       DbU::Unit   getWidth      () const;
      inline       Cell*       getMasterCell () const;
      inline       Instance*   getInstance   () const;
      inline const Occurrence& getOccurrence () const;
      inline       void        translate     ( DbU::Unit );
      inline       Tile&       operator=     ( const Tile& );
      inline       std::string _getString    () const;
                   Record*     _getRecord    () const;
    private:
      DbU::Unit   _xMin;
      DbU::Unit   _width;
      Occurrence  _occurrence;
  };

  inline Tile::Tile ( DbU::Unit xMin, DbU::Unit width, const Occurrence& occurrence )
    : _xMin(xMin)
    , _width(width)
    , _occurrence(occurrence)
  { }

  inline Tile::Tile ( const Tile& other )
    : _xMin(other._xMin)
    , _width(other._width)
    , _occurrence(other._occurrence)
  { }

  inline Tile& Tile::operator= ( const Tile& other )
  {
    _xMin       = other._xMin;
    _width      = other._width;
    _occurrence = other._occurrence;
    return *this;
  }

  inline Tile::~Tile ()
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
    cdebug_log(121,0) << "  Tile::translate(), dx:" << DbU::getValueString(dx) << ", " << _occurrence << std::endl;
    Instance*      instance  = getInstance();
    Transformation reference = instance->getTransformation();
    Transformation transf    = Transformation( reference.getTx() + dx
                                             , reference.getTy()
                                             , reference.getOrientation() );
    instance->setTransformation( transf );
    _xMin += dx;
  }

  
  inline std::string  Tile::_getString () const
  {
    string s = "<Tile @" + DbU::getValueString(_xMin)
             + " w="     + DbU::getValueString(_width)
             + " \""     + getString(getInstance()->getName())
             + "\">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "::SubSlice".

  class SubSlice {
    public:
                                              SubSlice        ( Slice*, const std::list<Tile>::iterator& beginTile );
      inline const std::list<Tile>::iterator  getBeginTile    () const;
      inline const std::list<Tile>::iterator  getEndTile      () const;
      inline       DbU::Unit                  getYBottom      () const;
      inline       DbU::Unit                  getXMin         () const;
      inline       DbU::Unit                  getXMax         () const;
                   size_t                     getUsedVTracks  ( const Tile& , std::set<DbU::Unit>& vtracks );
                   DbU::Unit                  getAverageChunk ( size_t& ) const;
                   void                       trackAvoid      ( DbU::Unit xTrack );
                   void                       insertTies      ( DbU::Unit latchUpMax );
    private:
      Slice*                    _slice;
      std::list<Tile>::iterator _beginTile;
      std::list<Tile>::iterator _endTile;
  };

  inline const std::list<Tile>::iterator  SubSlice::getBeginTile () const { return _beginTile; }
  inline const std::list<Tile>::iterator  SubSlice::getEndTile   () const { return _endTile; }


// -------------------------------------------------------------------
// Class  :  "::Slice".

  class Slice {
    public:
                              Slice            ( Area*, DbU::Unit ybottom );
      inline DbU::Unit        getYBottom       () const;
      inline DbU::Unit        getXMin          () const;
      inline DbU::Unit        getXMax          () const;
      inline Interval         getXSpan         () const;
      inline Area*            getArea          () const;
      inline EtesianEngine*   getEtesian       () const;
      inline size_t           getSpinSlice0    () const;
      inline DbU::Unit        getLeftDistance  ( Cell* cell ) const;
      inline DbU::Unit        getRightDistance ( Cell* cell ) const;
             bool             validate         ( DbU::Unit latchUpMax ) const;
      inline std::list<Tile>& getTiles         ();
             void             merge            ( const Occurrence&, const Box& );
             void             addFeeds         ( size_t islice );
             void             fillHole         ( std::list<Tile>::iterator before
                                               , DbU::Unit xmin
                                               , DbU::Unit xmax
                                               , DbU::Unit ybottom
                                               , size_t yspin );
             void             buildSubSlices   ();
             void             showSubSlices    ();
             void             trackAvoid       ( DbU::Unit xTrack );
             void             insertTies       ( DbU::Unit latchUpMax );
             Instance*        createDiodeUnder ( RoutingPad*, const Box&, DbU::Unit xHint );
      inline std::string      _getString       () const;
             Record*          _getRecord       () const;
    private:
      Area*                  _area;
      DbU::Unit              _ybottom;
      std::list<Tile>        _tiles;
      std::vector<SubSlice>  _subSlices;
  };

  
  inline std::string  Slice::_getString () const
  {
    string s = "<Slice @" + DbU::getValueString(_ybottom) + ">";
    return s;
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
      inline const Box&     getPlaceArea     () const;
      inline DbU::Unit      getLeftDistance  ( Cell* cell ) const;
      inline DbU::Unit      getRightDistance ( Cell* cell ) const;
             bool           validate         ( DbU::Unit latchUpMax ) const;
             void           merge            ( const Occurrence&, const Box& );
             void           addFeeds         ();
             void           buildSubSlices   ();
             void           showSubSlices    ();
             void           trackAvoid       ( const Box& trackAvoid );
             void           insertTies       ( DbU::Unit latchUpMax );
             Instance*      createDiodeUnder ( RoutingPad*, const Box&, DbU::Unit xHint );
      inline std::string    _getString       () const;
             Record*        _getRecord       () const;
    private:
      EtesianEngine*       _etesian;
      TieLUT               _tieLut;
      Box                  _placeArea;
      DbU::Unit            _sliceHeight;
      std::vector<Slice*>  _slices;
      size_t               _spinSlice0;
  };


  inline EtesianEngine* Area::getEtesian       () const { return _etesian; }
  inline size_t         Area::getSpinSlice0    () const { return _spinSlice0; }
  inline void           Area::setSpinSlice0    ( size_t spinSlice0 ) { _spinSlice0 = spinSlice0; }
  inline DbU::Unit      Area::getXMin          () const { return _placeArea.getXMin(); }
  inline DbU::Unit      Area::getXMax          () const { return _placeArea.getXMax(); }
  inline const Box&     Area::getPlaceArea     () const { return _placeArea; }
  inline DbU::Unit      Area::getLeftDistance  ( Cell* cell ) const { return _tieLut.getLeftDistance(cell); }
  inline DbU::Unit      Area::getRightDistance ( Cell* cell ) const { return _tieLut.getRightDistance(cell); }
  inline string         Area::_getString       () const { return "<Etesian::Area>"; }


  inline DbU::Unit  SubSlice::getYBottom () const { return _slice->getYBottom(); }

  inline DbU::Unit  SubSlice::getXMin () const
  { return (_beginTile == _slice->getTiles().begin()) ? _slice->getXMin() : (*_beginTile).getXMin(); }

  inline DbU::Unit  SubSlice::getXMax () const
  { return (_endTile   == _slice->getTiles().end  ()) ? _slice->getXMax() : (*_endTile).getXMax(); }


  inline DbU::Unit        Slice::getYBottom       () const { return _ybottom; }
  inline DbU::Unit        Slice::getXMin          () const { return _area->getXMin(); }
  inline DbU::Unit        Slice::getXMax          () const { return _area->getXMax(); }
  inline DbU::Unit        Slice::getLeftDistance  ( Cell* cell ) const { return _area->getLeftDistance(cell); }
  inline DbU::Unit        Slice::getRightDistance ( Cell* cell ) const { return _area->getRightDistance(cell); }
  inline Interval         Slice::getXSpan         () const { return Interval( getXMin(), getXMax() ); }
  inline Area*            Slice::getArea          () const { return _area; }
  inline EtesianEngine*   Slice::getEtesian       () const { return getArea()->getEtesian(); }
  inline size_t           Slice::getSpinSlice0    () const { return getArea()->getSpinSlice0(); }
  inline std::list<Tile>& Slice::getTiles         () { return _tiles; }


}  // Etesian namespace.


INSPECTOR_PR_SUPPORT(Etesian::Tile);
INSPECTOR_P_SUPPORT(Etesian::Slice);
INSPECTOR_P_SUPPORT(Etesian::Area);
