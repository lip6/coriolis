
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingEventLoop.cpp"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iostream>
#include  <algorithm>
#include  "kite/RoutingEvent.h"
#include  "kite/RoutingEventLoop.h"


namespace Kite {

  using std::cerr;
  using std::endl;
  using std::vector;
  using std::stable_sort;


  RoutingEventLoop::RoutingEventLoop ( size_t depth, int countLimit )
    : _elements  ()
    , _depth     (depth)
    , _maxCount  (0)
    , _countLimit(countLimit)
    , _isLooping (false)
  { }


  void  RoutingEventLoop::update ( size_t id )
  {
    vector<Element>::iterator ielement = _elements.begin();
    for ( ; ielement != _elements.end() ; ++ielement ) {
      if ( (*ielement)._id == id ) {
      // Increment an already present element.
        (*ielement)._count    += 1;
        (*ielement)._timestamp = RoutingEvent::getProcesseds();
        _maxCount = std::max ( _maxCount, (*ielement)._count );

        if ( _maxCount > _countLimit ) _isLooping = true;
        break;
      }
    }

  // The Event was not found.
    if ( ielement == _elements.end() ) {
      if ( _elements.size() >= _depth ) _elements.pop_back ();
      _elements.push_back ( Element(id,RoutingEvent::getProcesseds()) );
    }

    stable_sort ( _elements.begin(), _elements.end(), CompareByTimestamp() );
  }


  void  RoutingEventLoop::erase ( size_t id )
  {
    vector<Element>::iterator ielement = _elements.begin();
    for ( ; ielement != _elements.end() ; ++ielement ) {
      if ( (*ielement)._id == id ) {
        _elements.erase ( ielement );
        break;
      }
    }

    _maxCount = 0;
    ielement = _elements.begin();
    for ( ; ielement != _elements.end() ; ++ielement ) {
      _maxCount = std::max ( _maxCount, (*ielement)._count );
    }

    _isLooping = (_maxCount > _countLimit);
  }


}  // End of Kite namespace.
