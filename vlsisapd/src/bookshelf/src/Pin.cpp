
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Pin.cpp"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "vlsisapd/bookshelf/Node.h"
#include  "vlsisapd/bookshelf/Pin.h"
#include  "vlsisapd/bookshelf/Net.h"
#include  "vlsisapd/bookshelf/Circuit.h"


namespace Bookshelf {


  Pin::Pin ( Circuit*     circuit
           , Node*        node
           , Net*         net
           , double       x
           , double       y
           , unsigned int direction
           )
    : _id       (circuit->getPinId())
    , _x        (x)
    , _y        (y)
    , _direction(direction)
    , _node     (node)
    , _net      (net)
  {
    _node->addPin ( this );
    _net ->addPin ( this );
  }


  void  Pin::writeToStream ( std::ostream& o )
  {
    o << std::setw(10) << std::right << _node->getName();
    if ( _direction != Direction::Disabled ) {
      if ( _direction == Direction::Bidirectional ) o << " B";
      else {
        if ( _direction & Direction::Input  ) o << " I";
        if ( _direction & Direction::Output ) o << " O";
      }
    }
    if ( (_x != 0.0) or (_y != 0) ) {
      o << " : " << std::setw(5) << std::right << _x << " " << std::setw(5) << std::right << _y;
    }
    o << std::endl;
  }



}  // End of Bookshelf namespace.
