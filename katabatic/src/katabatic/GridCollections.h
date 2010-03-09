
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GridCollections.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __KATABATIC_GRID_COLLECTIONS__
#define  __KATABATIC_GRID_COLLECTIONS__

#ifndef  __KATABATIC_GRID__
#error  "GridCollections.h must be included only through Grid.h."
#endif

#include  "hurricane/Collection.h"


namespace Katabatic {


  using  Hurricane::_TName;
  using  Hurricane::Locator;
  using  Hurricane::Collection;
  using  Hurricane::GenericLocator;
  using  Hurricane::GenericCollection;
  using  Hurricane::GenericFilter;


// -------------------------------------------------------------------
// Template Class  :  "Katabatic::Grid_Column".


  template<typename GCellT>
  class Grid_Column : public Collection<GCellT*> {

    public:
    // Sub-Class: Locator.
      class Locator : public Hurricane::Locator<GCellT*> {
        public:
                                               Locator      ( const Grid<GCellT>* grid
                                                            ,       unsigned int  start
                                                            ,       unsigned int  stop );
                                               Locator      ( const Locator& );
          virtual Hurricane::Locator<GCellT*>* getClone     () const;
          virtual GCellT*                      getElement   () const;
                  Grid<GCellT>*                getGrid      () const;
          virtual bool                         isValid      () const;
          virtual void                         progress     ();
          virtual string                       _getString   () const;

        protected:
          const Grid<GCellT>* _grid;
          unsigned int        _index;
          unsigned int        _stop;
      };

    public:
    // Grid_Column Methods.
                                           Grid_Column ( const Grid<GCellT>* grid
                                                       ,       unsigned int  column
                                                       ,       unsigned int  rowStart
                                                       ,       unsigned int  rowStop
                                                       );
                                           Grid_Column ( const Grid_Column& );
      virtual Collection<GCellT*>*         getClone    () const;
	  virtual Hurricane::Locator<GCellT*>* getLocator  () const;
      virtual string                       _getString  () const;

    protected:
    // Grid_Column Attributes.
      const Grid<GCellT>* _grid;
            unsigned int  _start;
            unsigned int  _stop;

  };


// Grid_Column Locator Part.
  template<typename GCellT>
  Grid_Column<GCellT>::Locator::Locator ( const Grid<GCellT>* grid
                                        ,       unsigned int  start
                                        ,       unsigned int  stop )
    : Hurricane::Locator<GCellT*>()
    , _grid(grid)
    , _index(start)
    , _stop(stop)
  { }


  template<typename GCellT>
  Grid_Column<GCellT>::Locator::Locator ( const Locator& locator )
    : Hurricane::Locator<GCellT*>()
    , _grid(locator._grid)
    , _index(locator._index)
    , _stop(locator._stop)
  { };


  template<typename GCellT>
  GCellT* Grid_Column<GCellT>::Locator::getElement () const
  {
    return _grid->getGCell ( _index );
  }


  template<typename GCellT>
  void  Grid_Column<GCellT>::Locator::progress ()
  {
    _index += _grid->getColumns ();
  }


  template<typename GCellT>
  Hurricane::Locator<GCellT*>* Grid_Column<GCellT>::Locator::getClone () const
  {
    return new Locator(*this);
  }


  template<typename GCellT>
  bool  Grid_Column<GCellT>::Locator::isValid () const
  {
    return _index < _stop;
  }


  template<typename GCellT>
  Grid<GCellT>* Grid_Column<GCellT>::Locator::getGrid () const
  {
    return _grid;
  }


  template<typename GCellT>
  string  Grid_Column<GCellT>::Locator::_getString () const
  {
    string s = "<" + _TName("Grid_Column<GCellT>::Locator")
                   + getString(_grid)
                   + ">";
    return s;
  }


// Grid_Column Collection Part.
  template<typename GCellT>
  Grid_Column<GCellT>::Grid_Column ( const Grid<GCellT>* grid
                                   ,       unsigned int  column
                                   ,       unsigned int  rowStart
                                   ,       unsigned int  rowStop
                                   )
    : Collection<GCellT*>()
    , _grid(grid)
    , _start(grid->getIndex(column,rowStart))
    , _stop (grid->getIndex(column,rowStop))
  {
    if (    ( column   >= _grid->getColumns() )
         || ( rowStart >= _grid->getRows()    )
         || ( rowStart >= rowStop ) ) {
      _start = 1;
      _stop  = 0;
    } else {
      _start = grid->getIndex ( column, rowStart );
      _stop  = grid->getIndex ( column, min(rowStop,grid->getRows()) );
    }
  }


  template<typename GCellT>
  Grid_Column<GCellT>::Grid_Column ( const Grid_Column<GCellT>& gridColumn )
    : Collection<GCellT*>()
    , _grid(gridColumn._grid)
    , _start(gridColumn._start)
    , _stop(gridColumn._stop)
  { }


  template<typename GCellT>
  Collection<GCellT*>* Grid_Column<GCellT>::getClone () const
  {
    return new Grid_Column(*this);
  }


  template<typename GCellT>
  Hurricane::Locator<GCellT*>* Grid_Column<GCellT>::getLocator () const
  {
    return new Locator(_grid,_start,_stop);
  }


  template<typename GCellT>
  string  Grid_Column<GCellT>::_getString () const
  {
    string s = "<" + _TName("Grid_Column") + " "
                   + getString(_grid) + " "
                   + getString(_grid->getColumn(_start)) + " ["
                   + getString(_grid->getRow(_start)) + ":"
                   + getString(_grid->getRow(_stop)) + "]"
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Remplate Class  :  "Grid_Row".


  template<typename GCellT>
  class Grid_Row : public Collection<GCellT*> {

    public:
    // Sub-Class : Locator.
      class Locator : public Hurricane::Locator<GCellT*> {
        public:
                                               Locator    ( const Grid<GCellT>* grid
                                                          ,       unsigned int  start
                                                          ,       unsigned int  stop );
                                               Locator    ( const Locator& );
          virtual GCellT*                      getElement () const;
          virtual Hurricane::Locator<GCellT*>* getClone   () const;
          virtual bool                         isValid    () const;
          virtual void                         progress   ();
                  Grid<GCellT>*                getGrid    () const;
          virtual string                       _getString () const;
        protected:
          const Grid<GCellT>* _grid;
          unsigned int        _index;
          unsigned int        _stop;
      };

    public:
    // Grid_Row Methods.
                                           Grid_Row   ( const Grid<GCellT>* grid
                                                      ,       unsigned int  row
                                                      ,       unsigned int  columnStart
                                                      ,       unsigned int  columnStop
                                                      );
                                           Grid_Row   ( const Grid_Row& );
      virtual Collection<GCellT*>*         getClone   () const;
	  virtual Hurricane::Locator<GCellT*>* getLocator () const;
      virtual string                       _getString () const;

    protected:
    // Grid_Row Attributes.
      const Grid<GCellT>* _grid;
            unsigned int  _start;
            unsigned int  _stop;
  };


// Grid_Column Locator Part.
  template<typename GCellT>
  Grid_Row<GCellT>::Locator::Locator ( const Grid<GCellT>* grid
                                     ,       unsigned int  start
                                     ,       unsigned int  stop )
    : Hurricane::Locator<GCellT*>()
    , _grid(grid)
    , _index(start)
    , _stop(stop)
  { }


  template<typename GCellT>
  Grid_Row<GCellT>::Locator::Locator ( const Locator& locator )
    : Hurricane::Locator<GCellT*>()
    , _grid(locator._grid)
    , _index(locator._index)
    , _stop(locator._stop)
  { }


  template<typename GCellT>
  GCellT* Grid_Row<GCellT>::Locator::getElement () const
  {
    return _grid->getGCell ( _index );
  }


  template<typename GCellT>
  Hurricane::Locator<GCellT*>* Grid_Row<GCellT>::Locator::getClone () const
  {
    return new Locator(*this);
  }


  template<typename GCellT>
  bool  Grid_Row<GCellT>::Locator::isValid () const
  {
    return _index < _stop;
  }


  template<typename GCellT>
  void  Grid_Row<GCellT>::Locator::progress ()
  {
    _index++;
  }


  template<typename GCellT>
  Grid<GCellT>* Grid_Row<GCellT>::Locator::getGrid () const
  {
    return _grid;
  }


  template<typename GCellT>
  string  Grid_Row<GCellT>::Locator::_getString () const
  {
    string s = "<" + _TName("Grid_Row<GCellT>::Locator")
                   + getString(_grid)
                   + ">";
    
    return s;
  }


// Grid_Column Collection Part.
  template<typename GCellT>
  Grid_Row<GCellT>::Grid_Row ( const Grid<GCellT>* grid
                             , unsigned int        row
                             , unsigned int        columnStart
                             , unsigned int        columnStop
                             )
    : Collection<GCellT*>()
    , _grid(grid)
  {
    if (    ( row         >= _grid->getRows()    )
         || ( columnStart >= _grid->getColumns() )
         || ( columnStart >= columnStop ) ) {
      _start = 1;
      _stop  = 0;
    } else {
      _start = grid->getIndex ( columnStart, row );
      _stop  = grid->getIndex ( min(columnStop,grid->getColumns()), row );
    }
  }


  template<typename GCellT>
  Grid_Row<GCellT>::Grid_Row ( const Grid_Row<GCellT>& gridRow )
    : Collection<GCellT*>()
    , _grid(gridRow._grid)
    , _start(gridRow._start)
    , _stop(gridRow._stop)
  { }


  template<typename GCellT>
  Collection<GCellT*>* Grid_Row<GCellT>::getClone () const
  {
    return new Grid_Row(*this);
  }


  template<typename GCellT>
  Hurricane::Locator<GCellT*>* Grid_Row<GCellT>::getLocator () const
  {
    return new Locator(_grid,_start,_stop);
  }


  template<typename GCellT>
  string  Grid_Row<GCellT>::_getString () const
  {
    string s = "<" + _TName("Grid_Row<GCellT>") + " "
                   + getString(_grid) + " "
                   + getString(_grid->getRow(_start)) + " ["
                   + getString(_grid->getColumn(_start)) + ":"
                   + getString(_grid->getColumn(_stop)) + "]"
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Template Class  :  "Grid_Area".


  template<typename GCellT>
  class Grid_Area : public Collection<GCellT*> {

    public:
    // Sub-Class : Locator.
      class Locator : public Hurricane::Locator<GCellT*> {
        public:
                                               Locator    ( const Grid<GCellT>* grid
                                                          ,       unsigned int  columnStart
                                                          ,       unsigned int  rowStart
                                                          ,       unsigned int  colummStop
                                                          ,       unsigned int  rowStop );
                                               Locator    ( const Locator& );
                                              ~Locator    ();
          virtual GCellT*                      getElement () const;
          virtual Hurricane::Locator<GCellT*>* getClone   () const;
          virtual bool                         isValid    () const;
          virtual void                         progress   ();
          virtual string                       _getString () const;
        protected:
          const Grid<GCellT>*                 _grid;
                unsigned int                  _rowStart;
                unsigned int                  _rowStop;
                unsigned int                  _column;
                unsigned int                  _columnStop;
                Hurricane::Locator<GCellT*>*  _columnLocator;
      };

    public:
    // Grid_Area Methods.
                                           Grid_Area  ( const Grid<GCellT>* grid
                                                      ,       unsigned int  columnStart
                                                      ,       unsigned int  rowStart
                                                      ,       unsigned int  columnStop
                                                      ,       unsigned int  rowStop
                                                      );
                                           Grid_Area  ( const Grid_Area& );
      virtual Collection<GCellT*>*         getClone   () const;
	  virtual Hurricane::Locator<GCellT*>* getLocator () const;
      virtual string                       _getString () const;

    public:
    // Grid_Area Attributes.
      const Grid<GCellT>* _grid;
            unsigned int  _columnStart;
            unsigned int  _columnStop;
            unsigned int  _rowStart;
            unsigned int  _rowStop;
  };


// Grid_Area Locator Part.
  template<typename GCellT>
  Grid_Area<GCellT>::Locator::Locator ( const Grid<GCellT>* grid
                                      ,       unsigned int  columnStart
                                      ,       unsigned int  rowStart
                                      ,       unsigned int  columnStop
                                      ,       unsigned int  rowStop
                                      )
    : _grid(grid)
    , _rowStart(rowStart)
    , _rowStop(rowStop)
    , _column(columnStart)
    , _columnStop(columnStop)
  {
    _columnLocator = new typename Grid_Column<GCellT>::Locator ( _grid
                                                               , _grid->getIndex(_column,rowStart)
                                                               , _grid->getIndex(_column,rowStop ) );
    if ( !_columnLocator->isValid() ) _column = _columnStop;
  }


  template<typename GCellT>
  Grid_Area<GCellT>::Locator::Locator ( const Locator& locator )
    : Hurricane::Locator<GCellT*>()
    , _grid(locator._grid)
    , _rowStart(locator._rowStart)
    , _rowStop(locator._rowStop)
    , _column(locator._column)
    , _columnStop(locator._columnStop)
    , _columnLocator(_columnLocator->getClone())
  { }


  template<typename GCellT>
  Grid_Area<GCellT>::Locator::~Locator ()
  {
    if (_columnLocator) delete _columnLocator;
  }


  template<typename GCellT>
  Hurricane::Locator<GCellT*>* Grid_Area<GCellT>::Locator::getClone () const
  {
    return new Locator(*this);
  }


  template<typename GCellT>
  GCellT* Grid_Area<GCellT>::Locator::getElement () const
  {
    return _columnLocator->getElement();
  }


  template<typename GCellT>
  void  Grid_Area<GCellT>::Locator::progress ()
  {
    if ( !isValid() ) return;

    _columnLocator->progress();
    if ( !_columnLocator->isValid() ) {
      delete _columnLocator;
      _columnLocator = NULL;


      _column += 1;
      if ( !isValid() ) return;

      _columnLocator = new typename Grid_Column<GCellT>::Locator ( _grid
                                                                 , _grid->getIndex(_column,_rowStart)
                                                                 , _grid->getIndex(_column,_rowStop ) );
    }
  }


  template<typename GCellT>
  bool  Grid_Area<GCellT>::Locator::isValid () const
  {
    return _column < _columnStop;
  }


  template<typename GCellT>
  string  Grid_Area<GCellT>::Locator::_getString () const
  {
    string s = "<" + _TName("Grid_Area<GCellT>::Locator")
                   + getString(_grid)
                   + ">";
    return s;
  }


// Grid_Area Collection Part.
  template<typename GCellT>
  Grid_Area<GCellT>::Grid_Area ( const Grid<GCellT>* grid
                               ,       unsigned int  columnStart
                               ,       unsigned int  rowStart
                               ,       unsigned int  columnStop
                               ,       unsigned int  rowStop
                               )
    : Collection<GCellT*>()
    , _grid(grid)
    , _columnStart(columnStart)
    , _columnStop(columnStop)
    , _rowStart(rowStart)
    , _rowStop(rowStop)
  { }


  template<typename GCellT>
  Grid_Area<GCellT>::Grid_Area ( const Grid_Area<GCellT>& fcells )
    : Collection<GCellT*>()
    , _grid(fcells._grid)
    , _columnStart(fcells._columnStart)
    , _columnStop(fcells._columnStop)
    , _rowStart(fcells._rowStart)
    , _rowStop(fcells._rowStop)
  { }


  template<typename GCellT>
  Collection<GCellT*>* Grid_Area<GCellT>::getClone () const
  {
    return new Grid_Area(*this);
  }


  template<typename GCellT>
  Hurricane::Locator<GCellT*>* Grid_Area<GCellT>::getLocator () const
  {
    return new Locator(_grid,_columnStart,_rowStart,_columnStop,_rowStop);
  }


  template<typename GCellT>
  string  Grid_Area<GCellT>::_getString () const
  {
    string s = "<" + _TName("Grid_Area<GCellT>") + " "
                   + getString(_grid) + " <Area "
                   + getString(_columnStart) + " "
                   + getString(_rowStart) + " "
                   + getString(_columnStop) + " "
                   + getString(_rowStop) + ">"
                   + ">";
    return s;
  }


} // End of Katabatic namespace.


#endif  // __KATABATIC_GRID_COLLECTIONS__
