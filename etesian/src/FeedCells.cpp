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
// |  C++ Module  :       "./FeedCells.cpp"                          |
// +-----------------------------------------------------------------+


#include <sstream>
#include "hurricane/Warning.h"
#include "etesian/FeedCells.h"
#include "etesian/EtesianEngine.h"


namespace Etesian {

  using std::cerr;
  using std::endl;
  using std::map;
  using std::string;
  using std::ostringstream;
  using std::make_pair;
  using Hurricane::Warning;
  using Hurricane::DbU;


  void  FeedCells::useTie ( Cell* cell )
  {
    if (cell == NULL) return;

    DbU::Unit pitch = _etesian->getSliceStep();

    if (cell->getAbutmentBox().getWidth() % pitch != 0)
      cerr << Warning( "FeedCells::useTie(): \"%s\" has not a width (%s) multiple of pitch (%s)."
                     , getString(cell->getName()).c_str()
                     , DbU::getValueString(cell->getAbutmentBox().getWidth()).c_str()
                     , DbU::getValueString(pitch).c_str()
                     ) << endl;

    int pitchNb = (int)( cell->getAbutmentBox().getWidth() / pitch );

    _tieCell = cell;
  }


  void  FeedCells::useFeed ( Cell* cell )
  {
    if ( cell == NULL ) return;

    DbU::Unit pitch = _etesian->getSliceStep();

    if (cell->getAbutmentBox().getWidth() % pitch != 0)
      cerr << Warning( "FeedCells::useFeed(): \"%s\" has not a width (%s) multiple of pitch (%s)."
                     , getString(cell->getName()).c_str()
                     , DbU::getValueString(cell->getAbutmentBox().getWidth()).c_str()
                     , DbU::getValueString(pitch).c_str()
                     ) << endl;

    int pitchNb = (int)( cell->getAbutmentBox().getWidth() / pitch );
    
    if (getFeed(pitchNb) != NULL) {
      cerr << Warning( "FeedCells::addFeed(): \"%s\" duplicate feed for width %d."
                     , getString(cell->getName()).c_str()
                     , pitchNb
                     ) << endl;
      return;
    }

    _feedCells.insert( make_pair(pitchNb,cell) );
  }


  Cell* FeedCells::getBiggestFeed () const
  {
    if (_feedCells.empty()) return NULL;
    return (*(--_feedCells.end())).second;
  }


  Cell* FeedCells::getSmallestFeed () const
  {
    if (_feedCells.empty()) return NULL;
    return (*(_feedCells.begin())).second;
  }


  Cell* FeedCells::getFeed ( int pitches ) const
  {
    map<int,Cell*>::const_iterator ifeed = _feedCells.find( pitches );
    if (ifeed == _feedCells.end()) return NULL;

    return (*ifeed).second;
  }


  string  FeedCells::getUniqueInstanceName () const
  {
    ostringstream name;
    name << "feed_" << _feedCount++;

    return name.str();
  }


}  // Etesian namespace.
