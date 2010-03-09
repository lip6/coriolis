
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
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
// |  C++ Module  :       "./GCellRoutingSet.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <limits>
#include  <queue>
#include  <iostream>
#include  <iomanip>
#include  <sstream>
#include  <boost/bind.hpp>

#include  "hurricane/Error.h"
#include  "hurricane/Contact.h"
#include  "hurricane/Segment.h"
#include  "hurricane/RoutingPad.h"
#include  "katabatic/AutoSegment.h"
#include  "kite/GCell.h"
#include  "kite/GCellGrid.h"
#include  "kite/GCellRoutingSet.h"
#include  "kite/DataNegociate.h"


namespace Kite {


  using std::numeric_limits;
  using std::queue;
  using std::cerr;
  using std::endl;
  using std::ostringstream;
  using std::setprecision;
  using Hurricane::roundfp;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::Error;
  using Katabatic::getVectorString;


  const char* usedGCellSeed =
    "GCellRoutingSet::create() : %s is already in a GCellRoutingSet.\n";


// -------------------------------------------------------------------
// Class  :  "GCellRoutingSet".


  GCellRoutingSet::GCellRoutingSet ( GCell* gcell, float expandStep, float minDensity )
    : _order         (numeric_limits<unsigned int>::max())
    , _minDensity    (minDensity)
    , _minDensities  (new float [gcell->getDepth()])
    , _gcells        (1,gcell)
    , _borderSegments()
    , _statistics    ()
  {
    gcell->setInRoutingSet ( true );

    if ( _minDensity > expandStep ) _minDensity -= expandStep;
    else                            _minDensity  = 0.0;
    _minDensity = roundfp ( _minDensity );
    for ( unsigned int i=0 ; i < gcell->getDepth() ; i++ ) {
      _minDensities[i] = gcell->getDensity(i) * expandStep;
      _minDensities[i] = roundfp ( _minDensities[i] );
    }
    ltrace(200) << getVectorString(_minDensities,gcell->getDepth()) << endl;
  }


  void  GCellRoutingSet::_postCreate ()
  {
    ltrace(90) << "Kite::GCellRoutingSet::_postCreate() - " << (void*)this << " " << this << endl;
    ltracein(90);

    ltraceout(90);
  }


  GCellRoutingSet::~GCellRoutingSet ()
  {
    delete [] _minDensities;
  }


  void  GCellRoutingSet::_preDestroy ()
  { }


  void  GCellRoutingSet::destroy ()
  {
    _preDestroy ();
    delete this;
  }


  GCellRoutingSet* GCellRoutingSet::create ( GCell* gcell, float expandStep )
  {
    if ( gcell->isInRoutingSet() )
      throw Error ( usedGCellSeed, getString(gcell).c_str() );

    GCellRoutingSet* rs = new GCellRoutingSet ( gcell
                                              , expandStep
                                              , gcell->getDensity()
                                              );
    rs->_postCreate ();

    return rs;
  }


  void  GCellRoutingSet::setRouted ( bool state )
  {
    for ( size_t igcell=0 ; igcell<_gcells.size() ; ++igcell )
      _gcells[igcell]->setRouted ( true );
  }


  vector<TrackElement*>&  GCellRoutingSet::getOwnedSegments ( vector<TrackElement*>& segments ) const
  {
    for ( size_t i=0 ; i<_gcells.size() ; i++ ) {
      if ( _gcells[i]->isRouted() ) continue;

      const vector<TrackElement*>& gcellSegments = _gcells[i]->getOwnedSegments();
      for ( size_t j=0 ; j<gcellSegments.size() ; j++ ) {
        if ( !gcellSegments[j]->isFixed() )
          segments.push_back ( gcellSegments[j] );
      }
//       segments.insert ( segments.end()
//                       , _gcells[i]->getOwnedSegments().begin()
//                       , _gcells[i]->getOwnedSegments().end()
//                       );
    }

    for ( size_t i=0 ; i<_borderSegments.size() ; i++ ) {
      TrackElement*  segment = _borderSegments[i]._segment;
      DataNegociate* data    = segment->getDataNegociate();
      data->setGCellOrder ( _order );
      segments.push_back  ( segment );
    }

    return segments;
  }


  void  GCellRoutingSet::expand ( GCellGrid* grid )
  {
#if defined(CHECK_DETERMINISM)
    cerr << "Order: Expanding " << this << endl;
#endif

    ltrace(200) << "Expanding: " << this << endl;
    ltracein(200);
    ltrace(200) << getVectorString(_minDensities,_gcells.front()->getDepth()) << endl;

    queue<GCell*>  densityQueue;
    densityQueue.push ( _gcells.front() );

    while ( !densityQueue.empty() ) {
      GCell* current  = densityQueue.front ();
      GCell* neighbor = NULL;
      densityQueue.pop ();

      ltrace(200) << "Popping Density: " << current << endl;

      for ( size_t i=0 ; i<4 ; i++ ) {
        switch ( i ) {
          case 0: neighbor = grid->getGCellLeft (current); break;
          case 1: neighbor = grid->getGCellRight(current); break;
          case 2: neighbor = grid->getGCellUp   (current); break;
          case 3: neighbor = grid->getGCellDown (current); break;
        }

        if ( neighbor ) {
#if defined(CHECK_DETERMINISM)
          cerr << "Order: Looking at neighbor " << neighbor << endl;
#endif
          float densities[32];
          neighbor->base()->getDensities ( densities );
          ltrace(200) << "Neighbor: " << neighbor << " " << getVectorString(densities,5) << endl;
        }

        if ( neighbor and !neighbor->isInRoutingSet() ) {
          if (   not neighbor->isAboveDensity(_minDensity)
             and not GCell::areDensityConnex(current,neighbor) )
            continue;

#if defined(CHECK_DETERMINISM)
          cerr << "Order: Agglomerate " << neighbor << endl;
#endif
          ltrace(200) << "Agglomerating: " << neighbor << endl;

          neighbor->setInRoutingSet ( true );
          _gcells.push_back ( neighbor );
          densityQueue.push ( neighbor );
        }
      }
    }

  // De-notching pass.
    for ( size_t i=0 ; i<_gcells.size() ; i++ ) densityQueue.push ( _gcells[i] );

    while ( not densityQueue.empty() ) {
      GCell* current  = densityQueue.front();
      GCell* neighbor = NULL;

      densityQueue.pop();

      for ( size_t i=0 ; i<4 ; i++ ) {
        switch ( i ) {
          case 0: neighbor = grid->getGCellLeft (current); break;
          case 1: neighbor = grid->getGCellRight(current); break;
          case 2: neighbor = grid->getGCellUp   (current); break;
          case 3: neighbor = grid->getGCellDown (current); break;
        }

        if ( not neighbor ) continue;

#if defined(CHECK_DETERMINISM)
        cerr << "Order: Denotching, looking at neighbor " << neighbor << endl;
#endif
        if ( neighbor->isInRoutingSet() ) continue;

        GCell* left  = grid->getGCellLeft (neighbor);
        GCell* right = grid->getGCellRight(neighbor);
        GCell* up    = grid->getGCellUp   (neighbor);
        GCell* down  = grid->getGCellDown (neighbor);

        bool  hasLeft  = (left)  ? left ->isInRoutingSet() : true;
        bool  hasRight = (right) ? right->isInRoutingSet() : true;
        bool  hasUp    = (up)    ? up   ->isInRoutingSet() : true;
        bool  hasDown  = (down)  ? down ->isInRoutingSet() : true;

        if ( (not (hasLeft and hasRight)) and (not (hasUp and hasDown)) ) continue;

#if defined(CHECK_DETERMINISM)
        cerr << "Order: Denotching, Agglomerate " << neighbor << endl;
#endif
        ltrace(200) << "Denotching, Agglomerating: " << neighbor << endl;

        neighbor->setInRoutingSet ( true );
        _gcells.push_back ( neighbor );
        densityQueue.push ( neighbor );
      }
    }

    ltrace(200) << "GCellRoutingSet (final): " << this << endl;
    for ( size_t i=0 ; i<_gcells.size() ; i++ )
      ltrace(200) << "| " << _gcells[i] << endl;

    _statistics.setGCellsCount   ( _gcells.size() );
    _statistics.setSegmentsCount ( 0 );
    for ( size_t i=0 ; i<_gcells.size() ; i++ ) {
      const vector<TrackElement*>& gcellSegments = _gcells[i]->getOwnedSegments();
      _statistics.incSegmentsCount ( gcellSegments.size() );
    }

    ltraceout(200);
  }


  void  GCellRoutingSet::loadBorder ( GCellGrid* grid )
  {
    ltrace(200) << "loadBorder() " << this << endl;
    unsigned int   order   = _order;
    TrackElement*  segment = NULL;
    DataNegociate* data;

    for ( size_t igcell=0 ; igcell<_gcells.size() ; igcell++ ) {
      if ( _gcells[igcell]->getOrder() < order ) continue;

      GCell* neighbor = NULL;

      for ( size_t i=0 ; i<2 ; i++ ) {
        switch ( i ) {
          case 0: neighbor = grid->getGCellLeft (_gcells[igcell]); break;
          case 1: neighbor = grid->getGCellRight(_gcells[igcell]); break;
        }
        if ( not neighbor or (neighbor->getOrder() <= order) ) continue;

        neighbor->computeBorder ();

        for ( size_t iRight=0 ; iRight<2 ; iRight++ ) {
          segment = neighbor->getRightRp(iRight);
          if ( segment == NULL ) break;

          data = (segment) ? segment->getDataNegociate() : NULL;
          if ( data ) {
            if ( (i == 0) and (data->getGCellOrder() > order) and not data->isRing() ) {
              ltrace(200) << "| border: [" << data->getGCellOrder()
                          << " > " << order << "] " << segment << endl;
              data->setRightBorder ( true );
              _borderSegments.push_back ( BorderSegment(segment,data->getGCellOrder()) );
            } else
              data->resetBorder();
          }
        }

        for ( size_t iLeft=0 ; iLeft<2 ; iLeft++ ) {
          segment = neighbor->getLeftRp(iLeft);
          if ( segment == NULL ) break;

          data = (segment) ? segment->getDataNegociate() : NULL;
          if ( data ) {
            if ( (i == 1) and (data->getGCellOrder() > order) and not data->isRing() ) {
              ltrace(200) << "| border: [" << data->getGCellOrder()
                          << " > " << order << "] " << segment << endl;
              data->setLeftBorder ( true );
              _borderSegments.push_back ( BorderSegment(segment,data->getGCellOrder()) );
            } else
              data->resetBorder();
          }
        }
      }
    }
  }


  void  GCellRoutingSet::freeBorder ()
  {
    for ( size_t j=0 ; j<_borderSegments.size() ; j++ ) {
      DataNegociate* data = _borderSegments[j]._segment->getDataNegociate();
      if ( not data->isBorder() ) continue;

      data->resetBorder   ();
      data->setGCellOrder ( _borderSegments[j]._order );
      Session::addRemoveEvent ( _borderSegments[j]._segment );
    //_borderSegments[j]._segment->base()->setFixed(true);
    }
    Session::revalidate ();

    vector<BorderSegment>().swap ( _borderSegments );
  }


  unsigned int&  GCellRoutingSet::loadRouting ( unsigned int& order )
  {
    _order = order;
  //cerr << "RoutingSet order:" << order << endl;

    for ( size_t i=0 ; i<_gcells.size() ; i++ )
      _gcells[i]->loadRouting ( order );

    return ++order;
  }


  string  GCellRoutingSet::_getTypeName () const
  { return "GCellRoutingSet"; }


  string  GCellRoutingSet::_getString () const
  {
    ostringstream s;

    s << "<o:" << ((_order!=numeric_limits<unsigned int>::max()) ? getString(_order) : "-")
      << " "   << _gcells.front() << setprecision(9)
      << " ["  << ":" << _minDensity
      << "] "  << _gcells.size()
      << ">";
    return s.str();

  //return "<o:" + ((_order!=numeric_limits<unsigned int>::max()) ? getString(_order) : "-")
  //      + " "  + getString(_gcells.front())
  //      + " [" + /*getString(_minDensity)*/ + ":" + getString(_minDensity)
  //      + "] " + getString(_gcells.size())
  //      + ">";
  }


  Record* GCellRoutingSet::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot ( "_gcells"    , &_gcells     ) );
    record->add ( getSlot ( "_order"     ,  _order      ) );
    record->add ( getSlot ( "_minDensity",  _minDensity ) );

    return record;
  }


} // End of Kite namespace.
