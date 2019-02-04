// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Pareto.cpp"                                  |
// +-----------------------------------------------------------------+


#include <cstdio>
#include <iostream>
#include "bora/Pareto.h"


namespace Bora {

  using namespace std;


  Pareto::Pareto ()
    : _capacity(0)
    , _size    (0)
    , _xs      (NULL)
    , _ys      (NULL)
  { }


  Pareto::~Pareto ()
  { clear(); }


  void  Pareto::_resize ( int capacity )
  {
    if (capacity <= _capacity) return;

    int newcapacity = (_capacity) ? _capacity*2 : 4;
    while ( newcapacity < capacity ) newcapacity <<= 1;

    double* xs = new double [newcapacity];
    double* ys = new double [newcapacity];

    for ( int i=0 ; i<_capacity ; ++i ) {
      xs[i] = _xs[i];
      ys[i] = _ys[i];
    }

    delete [] _xs;
    delete [] _ys;

    _capacity = newcapacity;
    _xs       = xs;
    _ys       = ys;
  }


  void  Pareto::_insert ( int index, double x, double y )
  {
    if (_size == _capacity) _resize( _capacity+1 );

    if (index < _size) {
      for ( int i=_size ; i>index ; --i ) {
        _xs[i] = _xs[i-1];
        _ys[i] = _ys[i-1];
      }
    }

    _xs[index] = x;
    _ys[index] = y;
    _size++;
  }


  void  Pareto::_remove ( int index )
  {
    if (index >= _size) return;

    for ( int i=index ; i<_size-1 ; ++i ) {
      _xs[i] = _xs[i+1];
      _ys[i] = _ys[i+1];
    }
    --_size;
  }


  void  Pareto::_restoreMonotonic ()
  {
    for ( int i=0 ; i<_size-1 ; ) {
      if (_ys[i] >= _ys[i+1]) {
        ++i;
        continue;
      }
      _remove( i+1 );
    }
  }


  void  Pareto::mergePoint ( double x, double y )
  {
  //cerr << "Merge: [" << x << " " << y << "] " << _size << endl;

    if (_size == 0) { _insert( 0, x, y ); return; }
    if (x < _xs[0]) {
      _insert( 0, x, y );
    //cerr << "  insert at 0" << endl;
    } else {
      int i = 1;
      for ( ; i<_size ; ++i ) {
        if ( (x == _xs[i-1]) and (y < _ys[i-1]) ) { _ys[i-1] = y; break; }
        if ( (x == _xs[i  ]) and (y < _ys[i  ]) ) { _ys[i  ] = y; break; }

      //cerr << "  check between x[" << i-1 << "]: " << _xs[i-1]
      //     << " and x[" << i << "]: " << _xs[i] << endl;
        if (x < _xs[i]) {
          // double ypareto = _ys[i-1];
          // ypareto += (_ys[i]-_ys[i-1]) / ((double)_xs[i] - (double)_xs[i-1]) * (x - _xs[i-1]);
          // if (y < ypareto)
          //   _insert( i, x, y );
          _insert( i, x, y );
          break;
        }
      }
      if (i == _size) {
      //cerr << "  insert at " << _size << " (end)" << endl;
        _insert( _size, x, y );
      }
    }

    _restoreMonotonic();
  }


  void  Pareto::dump ()
  {
    for ( int i=0 ; i<_size ; ++i ) {
      cerr << "  point[" << i << "]: [" << _xs[i] << " " << _ys[i] << "]" << endl;
    }
  }


  void  Pareto::clear ()
  {
    if (_xs) delete [] _xs;
    if (_ys) delete [] _ys;
    _capacity = 0;
    _size     = 0;
  }


}  // Bora namespace.
