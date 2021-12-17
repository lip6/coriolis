// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
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


#pragma  once
#include <map>
#include "hurricane/Cell.h"


namespace Etesian {

  using Hurricane::DbU;
  using Hurricane::Cell;
  class EtesianEngine;


  class FeedCells {
    public:
      inline               FeedCells             ( EtesianEngine* );
      inline  size_t       feedNumbers           () const;
              void         useTie                ( Cell* );
              void         useFeed               ( Cell* );
              Cell*        getBiggestFeed        () const;
              Cell*        getSmallestFeed       () const;
      inline  Cell*        getTie                () const;
              Cell*        getFeed               ( int pitches ) const;
              Cell*        getFeedByWidth        ( DbU::Unit width ) const;
              std::string  getUniqueInstanceName () const;
    private:
      EtesianEngine*        _etesian;
      Cell*                 _tieCell;
      std::map<int,Cell*>   _feedCells;
      mutable unsigned int  _feedCount;
  };


// Inline Methods.
  inline  FeedCells::FeedCells ( EtesianEngine* etesian )
    : _etesian  (etesian)
    , _tieCell  (NULL)
    , _feedCells()
    , _feedCount(0)
  { }


  inline size_t  FeedCells::feedNumbers () const { return _feedCells.size(); }
  inline Cell*   FeedCells::getTie      () const { return _tieCell; }


} // Etesian namespace.
