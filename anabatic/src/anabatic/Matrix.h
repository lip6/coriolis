// -*- mode: C++; explicit-buffer-name: "Matrix.h<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/Matrix.h"                           |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_MATRIX_H
#define  ANABATIC_MATRIX_H

#include <string>
#include <vector>
#include <iostream>
#include "hurricane/Box.h"
namespace Hurricane {
  class Cell;
}


namespace Anabatic {

  using std::string;
  using std::vector;
  using std::ostream;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Box;
  using Hurricane::Cell;

  class GCell;


  class Matrix {
    public:
      class Index {
        public:
          static inline Matrix::Index  asMin ( Matrix*, DbU::Unit x, DbU::Unit y );
          static inline Matrix::Index  asMax ( Matrix*, DbU::Unit x, DbU::Unit y );
          static inline Matrix::Index  asMin ( Matrix*, Point position );
          static inline Matrix::Index  asMax ( Matrix*, Point position );
        public:
          inline               Index      ( Matrix*, int index );
          inline               Index      ( Matrix*, int i, int j );
          inline       Matrix* matrix     () const;
          inline const int&    index      () const;
          inline       int&    index      ();
          inline       bool    valid      () const;
          inline       int     i          () const;
          inline       int     j          () const;
          inline       Index&  west       ();
          inline       Index&  east       ();
          inline       Index&  south      ();
          inline       Index&  north      ();
          inline       string  _getString () const;
        private:
          Matrix* _matrix;
          int     _index;
      };
    public:
                         Matrix       ();
                         Matrix       ( Box area, DbU::Unit side );
                        ~Matrix       ();
      inline  Box        getArea      () const;
      inline  DbU::Unit  getSide      () const;
      inline  Point      getGridPoint ( int i, int j ) const;
      inline  Point      getGridPoint ( int index ) const;
      inline  Point      getGridPoint ( const Index& ) const;
      inline  int        getIMax      () const;
      inline  int        getJMax      () const;
      inline  int        index2i      (       int    ) const;
      inline  int        index2j      (       int    ) const;
      inline  int        index2i      ( const Index& ) const;
      inline  int        index2j      ( const Index& ) const;
      inline  int        ij2index     ( int i, int j ) const;
      inline  int        xy2minIndex  ( DbU::Unit x, DbU::Unit y ) const;
      inline  int        xy2minIndex  ( Point ) const;
      inline  int        xy2maxIndex  ( DbU::Unit x, DbU::Unit y ) const;
      inline  int        xy2maxIndex  ( Point ) const;
      inline  Index&     west         ( Index& ) const;
      inline  Index&     east         ( Index& ) const;
      inline  Index&     south        ( Index& ) const;
      inline  Index&     north        ( Index& ) const;
              GCell*     getUnder     ( DbU::Unit x, DbU::Unit y ) const;
      inline  GCell*     getUnder     ( Point ) const;
              void       setCell      ( Cell*, DbU::Unit side );
              void       updateLookup ( GCell* );
              void       resize       ( Cell*, const vector<GCell*>& );
              void       show         () const;
    // Inspector support.                            
      virtual Record*    _getRecord   () const;
      virtual string     _getString   () const;
      virtual string     _getTypeName () const;
    private:
                         Matrix       ( const Matrix& );
              Matrix&    operator=    ( const Matrix& );
    private:
      Box             _area;
      DbU::Unit       _side;
      vector<GCell*>  _gcells;
      int             _imax;
      int             _jmax;
  };


// Matrix inline functions.
  inline Box            Matrix::getArea () const { return _area; }
  inline DbU::Unit      Matrix::getSide () const { return _side; }
  inline int            Matrix::getIMax () const { return _imax; }
  inline int            Matrix::getJMax () const { return _jmax; }

  inline int  Matrix::index2i ( int index ) const
  { return ((index >= 0) and (index < _imax*_jmax+1)) ? index % _imax : -1; }

  inline int  Matrix::index2i ( const Index& index ) const
  { return (index.valid()) ? index.index() % _imax : -1; }

  inline int  Matrix::index2j ( int index ) const
  { return ((index >= 0) and (index < _imax*_jmax+1)) ? index / _imax : -1; }

  inline int  Matrix::index2j ( const Index& index ) const
  { return (index.valid()) ? index.index() / _imax : -1; }

  inline int  Matrix::ij2index ( int i, int j ) const
  {
    if ((i < 0) or (i >= _imax)) return -1;
    if ((j < 0) or (j >= _jmax)) return -1;
    return j*_imax + i;
  }

  inline int  Matrix::xy2minIndex ( DbU::Unit x, DbU::Unit y ) const
  {
    DbU::Unit dx = x - _area.getXMin();
    DbU::Unit dy = y - _area.getYMin();
    return ij2index( dx / _side + ((dx%_side) ? 1 : 0)
                   , dy / _side + ((dy%_side) ? 1 : 0) );
  }

  inline int  Matrix::xy2minIndex ( Point p ) const
  { return xy2minIndex( p.getX(), p.getY() ); }

  inline int  Matrix::xy2maxIndex ( DbU::Unit x, DbU::Unit y ) const
  {
    DbU::Unit dx = x - _area.getXMin();
    DbU::Unit dy = y - _area.getYMin();
    return ij2index( dx / _side, dy / _side );
  }

  inline int  Matrix::xy2maxIndex ( Point p ) const
  { return xy2maxIndex( p.getX(), p.getY() ); }

  inline Matrix::Index& Matrix::west ( Matrix::Index& index ) const
  {
    if (index.valid()) {
      if (index.index() % _imax) index.index() -=  1;
      else                       index.index()  = -1;
    }
    return index;
  }

  inline Matrix::Index& Matrix::east ( Matrix::Index& index ) const
  {
    if (index.valid()) {
      if ((index.index() % _imax) < _imax-1) index.index() +=  1;
      else                                   index.index()  = -1;
    }
    return index;
  }

  inline Matrix::Index& Matrix::south ( Matrix::Index& index ) const
  {
    if (index.valid()) {
      if (index.index() >= _imax) index.index() -= _imax;
      else                        index.index()  = -1;
    }
    return index;
  }

  inline Matrix::Index& Matrix::north ( Matrix::Index& index ) const
  {
    if (index.valid()) {
      if (index.index() >= _imax*(_jmax-1)) index.index() += _imax;
      else                                  index.index()  = -1;
    }
    return index;
  }

  inline GCell*  Matrix::getUnder ( Point p ) const
  { return getUnder( p.getX(), p.getY() ); }

  inline Point  Matrix::getGridPoint ( int i, int j ) const
  { return Point( _area.getXMin() + _side*i, _area.getYMin() + _side*j ); }

  inline Point  Matrix::getGridPoint ( int index ) const
  { return getGridPoint( index2i(index), index2j(index) ); }

  inline Point  Matrix::getGridPoint ( const Index& index ) const
  { return getGridPoint( index.i(), index.j() ); }

  inline       Matrix::Index   Matrix::Index::asMin  ( Matrix* m, DbU::Unit x, DbU::Unit y ) { return Index( m, m->xy2minIndex(x,y) ); }
  inline       Matrix::Index   Matrix::Index::asMin  ( Matrix* m, Point position )           { return Index( m, m->xy2minIndex(position) ); }
  inline       Matrix::Index   Matrix::Index::asMax  ( Matrix* m, DbU::Unit x, DbU::Unit y ) { return Index( m, m->xy2maxIndex(x,y) ); }
  inline       Matrix::Index   Matrix::Index::asMax  ( Matrix* m, Point position )           { return Index( m, m->xy2maxIndex(position) ); }
  inline       Matrix*         Matrix::Index::matrix () const { return _matrix; }
  inline const int&            Matrix::Index::index  () const { return _index; }
  inline       int&            Matrix::Index::index  ()       { return _index; }
  inline       bool            Matrix::Index::valid  () const { return _index >= 0; }
  inline       int             Matrix::Index::i      () const { return _matrix->index2i(*this); }
  inline       int             Matrix::Index::j      () const { return _matrix->index2j(*this); }
  inline       Matrix::Index&  Matrix::Index::west   ()       { return _matrix->west   (*this); }
  inline       Matrix::Index&  Matrix::Index::east   ()       { return _matrix->east   (*this); }
  inline       Matrix::Index&  Matrix::Index::south  ()       { return _matrix->south  (*this); }
  inline       Matrix::Index&  Matrix::Index::north  ()       { return _matrix->north  (*this); }


// Matrix::Index inline functions.
  inline  Matrix::Index::Index ( Matrix* m, int index )    : _matrix(m), _index(index) { }
  inline  Matrix::Index::Index ( Matrix* m, int i, int j ) : _matrix(m), _index(m->ij2index(i,j)) { }

  inline string  Matrix::Index::_getString () const
  { return "(" + getString(index()) + "|" + getString(i()) + "," + getString(j()) + ")"; }


}  // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::Matrix);
GETSTRING_POINTER_SUPPORT(Anabatic::Matrix::Index);
GETSTRING_VALUE_SUPPORT(Anabatic::Matrix::Index);
IOSTREAM_POINTER_SUPPORT(Anabatic::Matrix::Index);
IOSTREAM_VALUE_SUPPORT(Anabatic::Matrix::Index);

#endif  // ANABATIC_MATRIX_H
