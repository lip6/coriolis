
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
// |  C++ Module  :  "./FeedCells.cpp"                               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <sstream>
#include  "hurricane/Warning.h"
#include  "mauka/FeedCells.h"
#include  "mauka/MaukaEngine.h"


namespace Mauka {

  using std::cerr;
  using std::endl;
  using std::map;
  using std::string;
  using std::ostringstream;
  using std::make_pair;
  using Hurricane::Warning;
  using Hurricane::DbU;


  void  FeedCells::addFeed ( Cell* cell )
  {
    if ( cell == NULL ) return;

    DbU::Unit pitch = _mauka->getPitch();

    if ( cell->getAbutmentBox().getWidth() % pitch != 0 )
      cerr << Warning("FeedCells::addFeed(): &lt;%s&gt; has not a width (%s) multiple of pitch (%s)."
                     ,getString(cell->getName()).c_str()
                     ,DbU::getValueString(cell->getAbutmentBox().getWidth()).c_str()
                     ,DbU::getValueString(pitch).c_str()
                     ) << endl;

    int pitchNb = (int)(cell->getAbutmentBox().getWidth() / pitch );
    
    if ( getFeed(pitchNb) != NULL ) {
      cerr << Warning("FeedCells::addFeed(): &lt;%s&gt; duplicate feed for width %d."
                     ,getString(cell->getName()).c_str()
                     ,pitchNb
                     ) << endl;
      return;
    }

    _feedCells.insert ( make_pair(pitchNb,cell) );
  }


  Cell* FeedCells::getBiggestFeed () const
  {
    if ( _feedCells.empty() ) return NULL;
    return (*(--_feedCells.end())).second;
  }


  Cell* FeedCells::getSmallestFeed () const
  {
    if ( _feedCells.empty() ) return NULL;
    return (*(_feedCells.begin())).second;
  }


  Cell* FeedCells::getFeed ( int pitches ) const
  {
    map<int,Cell*>::const_iterator ifeed = _feedCells.find ( pitches );
    if ( ifeed == _feedCells.end() ) return NULL;

    return (*ifeed).second;
  }


  string  FeedCells::getNewFeedName () const
  {
    ostringstream name;
    name << "feed_" << _feedCount++;

    return name.str();
  }


}  // End of Mauka namespace.
