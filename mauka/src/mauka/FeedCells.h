
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./FeedCells.h"                                 |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __MAUKA_FEEDCELLS_H
#define __MAUKA_FEEDCELLS_H

#include <map>
#include "hurricane/Cell.h"


namespace Mauka {

  using Hurricane::Cell;
  class MaukaEngine;


  class FeedCells {
    public:
      inline               FeedCells       ( MaukaEngine* );
              void         addFeed         ( Cell* );
              Cell*        getBiggestFeed  () const;
              Cell*        getSmallestFeed () const;
              Cell*        getFeed         ( int pitches ) const;
              std::string  getNewFeedName  () const;
    private:
      MaukaEngine*          _mauka;
      std::map<int,Cell*>   _feedCells;
      mutable unsigned int  _feedCount;
  };


// Inline Methods.
  inline  FeedCells::FeedCells ( MaukaEngine* mauka )
    : _mauka    (mauka)
    , _feedCells()
    , _feedCount(0)
  { }


} // End of Mauka namespace.

#endif  // __MAUKA_FEEDCELLS_H
