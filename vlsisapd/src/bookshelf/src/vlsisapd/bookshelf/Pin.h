// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./vlsisapd/bookshelf/Pin.h"               |
// +-----------------------------------------------------------------+


#ifndef  VLSISAPD_BOOKSHELF_PIN_H
#define  VLSISAPD_BOOKSHELF_PIN_H

#include  <string>
#include  <iostream>
#include  <iomanip>


namespace Bookshelf {

  class Node;
  class Net;
  class Circuit;


  class Direction {
    public:
      enum Codes { Disabled=0x0, Input=0x1, Output=0x2, Bidirectional=Input|Output };
  };


  class Pin {
    public:
                                    Pin           ( Circuit*
                                                  , Node*
                                                  , Net*
                                                  , double       x        =0.0
                                                  , double       y        =0.0
                                                  , unsigned int direction=Direction::Disabled
                                                  );
      inline size_t                 getId         () const;
      inline Node*                  getNode       () const;
      inline Net*                   getNet        () const;
      inline double                 getX          () const;
      inline double                 getY          () const;
      inline unsigned int           getDirection  () const;
             void                   writeToStream ( std::ostream& );
    private:
             size_t        _id;
             double        _x;
             double        _y;
             unsigned int  _direction;
             Node*         _node;
             Net*          _net;
  };


  inline size_t        Pin::getId        () const { return _id; }
  inline Net*          Pin::getNet       () const { return _net; }
  inline double        Pin::getX         () const { return _x; }
  inline double        Pin::getY         () const { return _y; }
  inline unsigned int  Pin::getDirection () const { return _direction; }
  inline Node*         Pin::getNode      () const { return _node; }


} // Bookshelf namespace.

#endif  // VLSISAPD_BOOKSHELF_PIN_H
