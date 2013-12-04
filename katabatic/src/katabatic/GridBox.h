
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/GridBox.h"                         |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_GRID_BOX_H
#define  KATABATIC_GRID_BOX_H

#ifndef  KATABATIC_GRID_H
#error  "GridBox.h must be included only through Grid.h."
#endif

#include  <algorithm>

#include  "hurricane/Error.h"
#include  "crlcore/RoutingLayerGauge.h"


namespace Katabatic {


  using Hurricane::Error;
  using std::min;
  using std::max;


// -------------------------------------------------------------------
// Template Class  :  "Katabatic::GridBox".
 

  template<typename GCellT>
  class GridBox {

    public:
      static GridBox*                    create        ( Grid<GCellT>*  grid
                                                       , unsigned int   columnMin
                                                       , unsigned int   rowMin
                                                       , unsigned int   columnMax
                                                       , unsigned int   rowMax
                                                       );
      inline Grid<GCellT>*               getGrid       () const;
      inline unsigned int                getColumnMin  () const;
      inline unsigned int                getColumnMax  () const;
      inline unsigned int                getRowMin     () const;
      inline unsigned int                getRowMax     () const;
      inline unsigned int                getRows       () const;
      inline unsigned int                getColumns    () const;
      inline Box                         getArea       () const;
      inline Point                       getCenter     () const;
      inline DbU::Unit                   getXMin       () const;
      inline DbU::Unit                   getYMin       () const;
      inline DbU::Unit                   getXMax       () const;
      inline DbU::Unit                   getYMax       () const;
      inline void                        getChannelBox ( unsigned int direction
                                                       , DbU::Unit&   channelMin
                                                       , DbU::Unit&   channelMax
                                                       , DbU::Unit&   channelLeft
                                                       , DbU::Unit&   channelRight
                                                       , bool         inclusive=false
                                                       ) const;
      inline GenericCollection<GCellT*>  getSlice      ( unsigned int direction, unsigned int slice );
      inline GenericCollection<GCellT*>  getGCells     ();
      inline string                      _getTypeName  () const;
      inline string                      _getString    () const;
      inline Record*                     _getRecord    () const;

    protected:
    // Attributes.
      Grid<GCellT>* _grid;
      Box           _area;
      unsigned int  _columnMin;
      unsigned int  _columnMax;
      unsigned int  _rowMin;
      unsigned int  _rowMax;

    protected:
    // Constructors & Destructors.
                GridBox ( Grid<GCellT>* grid
                         , unsigned int  columnMin
                         , unsigned int  rowMin
                         , unsigned int  columnMax
                         , unsigned int  rowMax
                         );
    private:
      GridBox            ( const GridBox& );
      GridBox& operator= ( const GridBox& );

  };


  template<typename GCellT> inline Grid<GCellT>* GridBox<GCellT>::getGrid      () const { return _grid; }
  template<typename GCellT> inline unsigned int  GridBox<GCellT>::getColumnMin () const { return _columnMin; }
  template<typename GCellT> inline unsigned int  GridBox<GCellT>::getColumnMax () const { return _columnMax; }
  template<typename GCellT> inline unsigned int  GridBox<GCellT>::getRowMin    () const { return _rowMin; }
  template<typename GCellT> inline unsigned int  GridBox<GCellT>::getRowMax    () const { return _rowMax; }
  template<typename GCellT> inline unsigned int  GridBox<GCellT>::getRows      () const { return _rowMax - _rowMin; }
  template<typename GCellT> inline unsigned int  GridBox<GCellT>::getColumns   () const { return _columnMax - _columnMin; }
  template<typename GCellT> inline Box           GridBox<GCellT>::getArea      () const { return _area; }
  template<typename GCellT> inline Point         GridBox<GCellT>::getCenter    () const { return _area.getCenter(); }
  template<typename GCellT> inline DbU::Unit     GridBox<GCellT>::getXMin      () const { return _area.getXMin(); }
  template<typename GCellT> inline DbU::Unit     GridBox<GCellT>::getYMin      () const { return _area.getYMin(); }
  template<typename GCellT> inline DbU::Unit     GridBox<GCellT>::getXMax      () const { return _area.getXMax(); }
  template<typename GCellT> inline DbU::Unit     GridBox<GCellT>::getYMax      () const { return _area.getYMax(); }
  template<typename GCellT> inline GenericCollection<GCellT*> 
                                                 GridBox<GCellT>::getGCells    () { return new Grid_Area<GCellT>(_grid,_columnMin,_rowMin,_columnMax,_rowMax); }
  template<typename GCellT> inline string        GridBox<GCellT>::_getTypeName () const { return "Katabatic::GridBox"; }





  template<typename GCellT>
  GridBox<GCellT>::GridBox ( Grid<GCellT>* grid
                           , unsigned int  columnMin
                           , unsigned int  rowMin
                           , unsigned int  columnMax
                           , unsigned int  rowMax
                           )
    : _grid     (grid)
    , _columnMin(columnMin)
    , _columnMax(columnMax)
    , _rowMin   (rowMin)
    , _rowMax   (rowMax)
  {
    GCellT* fcellBL = _grid->getGCell(_grid->getIndex(_columnMin  ,_rowMin  ));
    GCellT* fcellTR = _grid->getGCell(_grid->getIndex(_columnMax-1,_rowMax-1));

    _area = Box ( fcellBL->getX(), fcellBL->getY(), fcellTR->getXMax(), fcellTR->getYMax() );
  }


  template<typename GCellT>
  GridBox<GCellT>* GridBox<GCellT>::create ( Grid<GCellT>* grid
                                           , unsigned int  columnMin
                                           , unsigned int  rowMin
                                           , unsigned int  columnMax
                                           , unsigned int  rowMax
                                           )
  {
    columnMin = min ( columnMin, grid->getColumns() );
    rowMin    = min ( rowMin   , grid->getRows   () );
    columnMax = min ( columnMax, grid->getColumns() );
    rowMax    = min ( rowMax   , grid->getRows   () );

    if ( (columnMin >= columnMax)  || (rowMin >= rowMax) )
      throw Error("GridBox::create(): Empty GridBox requested <%u:%u %u:%u>."
                 ,columnMin,rowMin,columnMax,rowMax);

    return new GridBox ( grid, columnMin, rowMin, columnMax, rowMax );
  }


  template<typename GCellT>
  void  GridBox<GCellT>::getChannelBox ( unsigned int direction
                                       , DbU::Unit&   channelMin
                                       , DbU::Unit&   channelMax
                                       , DbU::Unit&   channelLeft
                                       , DbU::Unit&   channelRight
                                       , bool         inclusive
                                       ) const
  {
    if ( direction & Constant::Horizontal ) {
      channelMin   = getYMin() + DbU::lambda( inclusive?0:0.1 );
      channelMax   = getYMax();
      channelLeft  = getXMin() + DbU::lambda( inclusive?0:0.1 );
      channelRight = getXMax();
    } else {
      channelMin   = getXMin() + DbU::lambda( inclusive?0:0.1 );
      channelMax   = getXMax();
      channelLeft  = getYMin() + DbU::lambda( inclusive?0:0.1 );
      channelRight = getYMax();
    }
  }


  template<typename GCellT>
  GenericCollection<GCellT*>  GridBox<GCellT>::getSlice ( unsigned int direction, unsigned int slice )
  {
    if ( direction & Constant::Horizontal ) {
      unsigned int columnMin = min ( _columnMin+slice  , _columnMax );
      unsigned int columnMax = min ( _columnMin+slice+1, _columnMax );

      return new Grid_Area<GCellT> ( _grid, columnMin, _rowMin, columnMax, _rowMax );
    }

    unsigned int rowMin = min ( _rowMin+slice  , _rowMax );
    unsigned int rowMax = min ( _rowMin+slice+1, _rowMax );

    return new Grid_Area<GCellT> ( _grid, _columnMin, rowMin, _columnMax, rowMax );
  }


  template<typename GCellT>
  string  GridBox<GCellT>::_getString () const
  {
    return "<" + _getTypeName() + " "
               + getString(getColumnMin()) + ":" + getString(getRowMin()) + " "
               + getString(getColumnMax()) + ":" + getString(getRowMax())
               + ">";
  }


  template<typename GCellT>
  Record* GridBox<GCellT>::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot ( "_area"     , &_area      ) );
    record->add ( getSlot ( "_columnMin", &_columnMin ) );
    record->add ( getSlot ( "_columnMax", &_columnMax ) );
    record->add ( getSlot ( "_rowMin"   , &_rowMin    ) );
    record->add ( getSlot ( "_rowMax"   , &_rowMax    ) );
                                     
    return record;
  }


} // Katabatic namespace.

#endif  // KATABATIC_GRID_BOX_H
