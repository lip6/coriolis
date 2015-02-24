// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./FeedCells.h"                            |
// +-----------------------------------------------------------------+


#ifndef ETESIAN_FEEDCELLS_H
#define ETESIAN_FEEDCELLS_H

#include <map>
#include "hurricane/Cell.h"


namespace Etesian {

  using Hurricane::Cell;
  class EtesianEngine;


  class FeedCells {
    public:
      inline               FeedCells             ( EtesianEngine* );
      inline  size_t       feedNumbers           () const;
              void         useFeed               ( Cell* );
              Cell*        getBiggestFeed        () const;
              Cell*        getSmallestFeed       () const;
              Cell*        getFeed               ( int pitches ) const;
              std::string  getUniqueInstanceName () const;
    private:
      EtesianEngine*        _etesian;
      std::map<int,Cell*>   _feedCells;
      mutable unsigned int  _feedCount;
  };


// Inline Methods.
  inline  FeedCells::FeedCells ( EtesianEngine* etesian )
    : _etesian  (etesian)
    , _feedCells()
    , _feedCount(0)
  { }


  inline size_t  FeedCells::feedNumbers () const { return _feedCells.size(); }


} // Etesian namespace.

#endif  // ETESIAN_FEEDCELLS_H
