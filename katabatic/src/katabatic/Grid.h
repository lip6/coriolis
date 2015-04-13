// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/Grid.h"                            |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_GRID_H
#define  KATABATIC_GRID_H

#include  <string>
#include  <vector>
#include  "hurricane/Point.h"
#include  "hurricane/Box.h"
#include  "hurricane/Collection.h"


namespace Katabatic {

  using std::string;
  using std::vector;
  using Hurricane::_TName;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Box;
  using Hurricane::Collection;
  using Hurricane::GenericCollection;
  using Hurricane::getCollection;


// -------------------------------------------------------------------
// Class  :  "Katabatic::BaseGrid".

  class BaseGrid {

    public:
      class  Axis;
    public:
      inline  void          destroy         ();
    // Accessors.
      inline  bool          isOnTopBorder   ( unsigned int ) const;
      inline  bool          isOnRightBorder ( unsigned int ) const;
      inline  const Box&    getBoundingBox  () const;
      inline  unsigned int  getColumns      () const;
      inline  unsigned int  getRows         () const;
      inline  unsigned int  getRawSize      () const;
      inline  unsigned int  getIndex        ( unsigned int c, unsigned int r ) const;
      inline  unsigned int  getRow          ( unsigned int ) const;
      inline  unsigned int  getColumn       ( unsigned int ) const;
      inline  const Axis&   getXGrads       () const;
      inline  const Axis&   getYGrads       () const;
    // Inspector Managment.                 
      virtual Record*       _getRecord      () const;
      virtual string        _getString      () const = 0;

    public:
    // Sub-Class Grid::Axis. 
      class Axis {
        public:
        // Modifiers.
          inline void             addGraduation       ( DbU::Unit );
                 void             sort                ();
        // Accessors.
          inline unsigned int     getSize             () const;
                 unsigned int     getGraduationNumber ( DbU::Unit pos, bool& onGraduation ) const;
        // Operators.
          inline const DbU::Unit& operator[]          ( unsigned int i ) const;
        // Inspector Management.
                 Record*          _getRecord          () const;
                 string           _getString          () const;
          inline string           _getTypeName        () const;
                 string           _print              () const;
        protected:
        // Attributes.
          vector<DbU::Unit>  _graduations;
      };

    protected:
    // Attributes.
      Box           _boundingBox;
      Axis          _xGraduations;
      Axis          _yGraduations;
      unsigned int  _rows;
      unsigned int  _columns;
      unsigned int  _rawSize;

    // Constructors & Destructors.
    protected:
                        BaseGrid    ( const Box& );
      virtual          ~BaseGrid    ();
      virtual void      _postCreate ();
      virtual void      _preDestroy ();
    private:
                        BaseGrid    ( const BaseGrid& );
              BaseGrid& operator=   ( const BaseGrid& );
  };


// Inline Functions.
  inline void             BaseGrid::Axis::addGraduation ( DbU::Unit graduation ) { _graduations.push_back(graduation); }
  inline unsigned int     BaseGrid::Axis::getSize       () const { return _graduations.size(); }
  inline const DbU::Unit& BaseGrid::Axis::operator[]    ( unsigned int i ) const { return _graduations[i]; }
  inline string           BaseGrid::Axis::_getTypeName  () const { return _TName("BaseGrid::Axis"); }

  inline void         BaseGrid::destroy         () { _preDestroy(); delete this; }
  inline const Box&   BaseGrid::getBoundingBox  () const { return _boundingBox; };
  inline unsigned int BaseGrid::getColumns      () const { return _columns; };
  inline unsigned int BaseGrid::getRows         () const { return _rows; };
  inline unsigned int BaseGrid::getRawSize      () const { return getColumns() * getRows(); }
  inline unsigned int BaseGrid::getIndex        ( unsigned int c, unsigned int r ) const { return c+(r*getColumns()); }
  inline unsigned int BaseGrid::getRow          ( unsigned int i ) const { return i / getColumns(); }
  inline unsigned int BaseGrid::getColumn       ( unsigned int i ) const { return i % getColumns(); }
  inline bool         BaseGrid::isOnTopBorder   ( unsigned int i ) const { return getRow   (i)+1 == getRows(); }
  inline bool         BaseGrid::isOnRightBorder ( unsigned int i ) const { return getColumn(i)+1 == getColumns(); }

  inline const BaseGrid::Axis& BaseGrid::getXGrads () const { return _xGraduations; }
  inline const BaseGrid::Axis& BaseGrid::getYGrads () const { return _yGraduations; }



// -------------------------------------------------------------------
// Template Class  :  "Katabatic::Grid".
 

  template<typename GCellT>
  class Grid : public BaseGrid {

    public:
    // Accessors.
      inline  GCellT*                    getGCell          ( unsigned int index ) const;
      inline  GCellT*                    getGCell          ( const Point p ) const;
      inline  GCellT*                    getGCell          ( const Point p1, const Point p2 ) const;
      inline  GCellT*                    getGCellLeft      ( const GCellT* gcell ) const;
      inline  GCellT*                    getGCellRight     ( const GCellT* gcell ) const;
      inline  GCellT*                    getGCellUp        ( const GCellT* gcell ) const;
      inline  GCellT*                    getGCellDown      ( const GCellT* gcell ) const;
      inline  vector<GCellT*>*           getGCellVector    ();
    // Collections & Filters.
      inline  GenericCollection<GCellT*> getGCells         ();
      inline  GenericCollection<GCellT*> getGCellsColumn   ( unsigned int column
                                                           , unsigned int rowStart
                                                           , unsigned int rowStop );
      inline  GenericCollection<GCellT*> getGCellsRow      ( unsigned int row
                                                           , unsigned int columnStart
                                                           , unsigned int columnStop );
    // Inspector Managment.
      virtual Record*                    _getRecord        () const;

    protected:
    // Attributes.
      vector<GCellT*>  _gcells;

    // Constructors & Destructors.
    protected:
      inline        Grid        ( const Box& );
      virtual      ~Grid        ();
    private:
                    Grid        ( const Grid& );
              Grid& operator=   ( const Grid& );
  };


} // End of Katabatic namespace.


#include  "katabatic/GridCollections.h"
#include  "katabatic/GridBox.h"


namespace Katabatic {


// Inline Functions.

  template<typename GCellT>
  Grid<GCellT>::Grid ( const Box& bb )
    : BaseGrid(bb)
    , _gcells ()
  { }


  template<typename GCellT>
  Grid<GCellT>::~Grid ()
  { }


  template<typename GCellT>
  GCellT* Grid<GCellT>::getGCell ( unsigned int index ) const
  {
    if ( ( index < 0 ) || ( index >= _rawSize ) ) return NULL;

    return _gcells [ index ];
  }


  template<typename GCellT>
  GCellT* Grid<GCellT>::getGCell ( const Point p ) const
  {
    if (not getBoundingBox().contains(p)) return NULL;

    bool  onColumn;
    bool  onRow;

    unsigned int column = _xGraduations.getGraduationNumber ( p.getX(), onColumn );
    unsigned int row    = _yGraduations.getGraduationNumber ( p.getY(), onRow );

    return getGCell ( getIndex(column,row) );
  }


  template<typename GCellT>
  GCellT* Grid<GCellT>::getGCell ( const Point p1, const Point p2 ) const
  {
    if (not getBoundingBox().contains(p1)) return NULL;
    if (not getBoundingBox().contains(p2)) return NULL;

    bool  onColumn1;
    bool  onColumn2;
    bool  onRow1;
    bool  onRow2;

    unsigned int column1 = _xGraduations.getGraduationNumber ( p1.getX(), onColumn1 );
    unsigned int column2 = _xGraduations.getGraduationNumber ( p2.getX(), onColumn2 );
    unsigned int row1    = _yGraduations.getGraduationNumber ( p1.getY(), onRow1 );
    unsigned int row2    = _yGraduations.getGraduationNumber ( p2.getY(), onRow2 );

    if ( row1 != row2 ) {
      if ( onRow1 ) row1 = row2;
    }

    if ( column1 != column2 ) {
      if ( onColumn1 ) column1 = column2;
    }

    return getGCell ( getIndex(column1,row1) );
  }


  template<typename GCellT>
  GCellT* Grid<GCellT>::getGCellLeft ( const GCellT* gcell) const
  {
    if ( !gcell ) return NULL;

    unsigned int index = gcell->getIndex();
    if ( !getColumn(index) ) return NULL;

    return getGCell ( index - 1 ); 
  }


  template<typename GCellT>
  GCellT* Grid<GCellT>::getGCellRight ( const GCellT* gcell) const
  {
    if ( !gcell ) return NULL;

    unsigned int index = gcell->getIndex();
    if ( getColumn(index) >= getColumns()-1 ) return NULL;

    return getGCell ( index + 1 ); 
  }


  template<typename GCellT>
  GCellT* Grid<GCellT>::getGCellUp ( const GCellT* gcell) const
  {
    if ( !gcell ) return NULL;

    unsigned int index = gcell->getIndex();
    if ( getRow(index) >= getRows()-1 ) return NULL;

    return getGCell ( index + getColumns() ); 
  }


  template<typename GCellT>
  GCellT* Grid<GCellT>::getGCellDown ( const GCellT* gcell) const
  {
    if ( !gcell ) return NULL;

    unsigned int index = gcell->getIndex();
    if ( !getRow(index) ) return NULL;

    return getGCell ( index - getColumns() ); 
  }


  template<typename GCellT>
  inline vector<GCellT*>* Grid<GCellT>::getGCellVector ()
  {
    return &_gcells;
  }


  template<typename GCellT>
  inline GenericCollection<GCellT*>  Grid<GCellT>::getGCells ()
  {
    return getCollection(_gcells);
  }

  template<typename GCellT>
  inline  GenericCollection<GCellT*>  Grid<GCellT>::getGCellsColumn ( unsigned int column
                                                                    , unsigned int rowStart
                                                                    , unsigned int rowStop )
  {
    return Grid_Column<GCellT>(this,column,rowStart,rowStop);
  }

  template<typename GCellT>
  inline  GenericCollection<GCellT*>  Grid<GCellT>::getGCellsRow ( unsigned int row
                                                                 , unsigned int columnStart
                                                                 , unsigned int columnStop )
  {
    return Grid_Row<GCellT>(this,row,columnStart,columnStop);
  }


  template<typename GCellT>
  Record* Grid<GCellT>::_getRecord () const
  {
    Record* record = BaseGrid::_getRecord ();
    record->add ( getSlot ( "_gcells", &_gcells ) );
    return record;
  }


} // End of Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::BaseGrid::Axis);


#endif  // KATABATIC_GRID_H
