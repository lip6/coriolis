// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/Matrix.h"                   |
// +-----------------------------------------------------------------+


#ifndef ANALOG_MATRIX_H
#define ANALOG_MATRIX_H

#include <cstddef>


namespace Analog {

  using std::size_t;


  class Matrix {
    public:
      inline           Matrix    ( size_t rows=0, size_t columns=0 );
      inline           Matrix    ( const Matrix& );
      inline          ~Matrix    ();
      inline  bool     empty     () const;
      inline  Matrix&  operator= ( const Matrix& );
      inline  size_t   rows      () const;
      inline  size_t   columns   () const;
      inline  size_t   at        ( size_t row , size_t column  ) const;
      inline  size_t&  at        ( size_t row , size_t column  );
      inline  size_t   index     ( size_t row , size_t column  ) const;
      inline  void     resize    ( size_t rows, size_t columns );
      inline  Matrix&  makeEmpty ();
    private:
      size_t  _rows;
      size_t  _columns;
      size_t* _table;
  };

  
  inline Matrix::Matrix ( size_t rows, size_t columns )
    : _rows(rows), _columns(columns), _table(NULL)
  {
    if (empty()) {
      _rows    = 0;
      _columns = 0;
    } else
      _table = new size_t [ _rows * _columns ];
  }

  
  inline Matrix::Matrix ( const Matrix& other )
    : _rows(other._rows), _columns(other._columns), _table(NULL)
  {
    size_t length = _rows * _columns;
    _table = new size_t [ length ];
    for ( size_t i=0 ; i<length ; ++i ) _table[i] = other._table[i];
  }


  inline Matrix::~Matrix ()
  { delete [] _table; }


  inline Matrix& Matrix::operator= ( const Matrix& other )
  {
    delete [] _table;
    _rows    = other._rows;
    _columns = other._columns;
    
    size_t length = _rows * _columns;
    _table = new size_t [ length ];
    for ( size_t i=0 ; i<length ; ++i ) _table[i] = other._table[i];

    return *this;
  }


  inline void  Matrix::resize ( size_t rows, size_t columns )
  {
    if (rows * columns > _rows * _columns) {
      delete [] _table;
      _table = new size_t [ rows * columns ];
    } 

    _rows    = rows;
    _columns = columns;
  }


  inline Matrix& Matrix::makeEmpty ()
  {
    delete [] _table;

    _rows    = 0;
    _columns = 0;

    return *this;
  }

  
  inline  bool    Matrix::empty     () const { return not _rows or not _columns; }
  inline  size_t  Matrix::rows      () const { return _rows; }
  inline  size_t  Matrix::columns   () const { return _columns; }
  inline  size_t  Matrix::at        ( size_t row, size_t column ) const { return _table[ index(row,column) ]; }
  inline  size_t& Matrix::at        ( size_t row, size_t column ) { return _table[ index(row,column) ]; }
  inline  size_t  Matrix::index     ( size_t row, size_t column ) const { return row*_columns + column; }


}

#endif
