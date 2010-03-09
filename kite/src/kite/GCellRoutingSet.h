
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
// |  C++ Header  :       "./GCellRoutingSet.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KITE_GCELL_ROUTING_SET__
#define  __KITE_GCELL_ROUTING_SET__

#include  <vector>


namespace Kite {

  using std::vector;
  class TrackElement;
  class GCell;
  class GCellGrid;


// -------------------------------------------------------------------
// Class  :  "Statistics".


  class Statistics {
    public:
      inline             Statistics       ();
      inline size_t      getGCellsCount   () const;
      inline size_t      getSegmentsCount () const;
      inline size_t      getEventsCount   () const;
      inline void        setGCellsCount   ( size_t );
      inline void        setSegmentsCount ( size_t );
      inline void        setEventsCount   ( size_t );
      inline void        incGCellCount    ( size_t );
      inline void        incSegmentsCount ( size_t );
      inline void        incEventsCount   ( size_t );
      inline Statistics& operator+=       ( const Statistics& );
    private:
      size_t  _gcellsCount;
      size_t  _segmentsCount;
      size_t  _eventsCount;

  };


// -------------------------------------------------------------------
// Class  :  "GCellRoutingSet".
 

  class GCellRoutingSet {

    public:
      static  GCellRoutingSet*       create           ( GCell*, float densityExpandRatio );
      virtual void                   destroy          ();
      inline  unsigned int           getOrder         () const;
      inline  const vector<GCell*>&  getGCells        () const;
              vector<TrackElement*>& getOwnedSegments ( vector<TrackElement*>& ) const;
              unsigned int&          loadRouting      ( unsigned int& order );
              void                   loadBorder       ( GCellGrid* );
              void                   freeBorder       ();
              void                   expand           ( GCellGrid* );
              void                   setRouted        ( bool );
      inline  Statistics&            getStatistics    ();
      virtual Record*                _getRecord       () const;
      virtual string                 _getString       () const;
      virtual string                 _getTypeName     () const;

    private:
      class BorderSegment {
        public:
          inline  BorderSegment ( TrackElement*, unsigned int );
        public:
          TrackElement* _segment;
          unsigned int  _order;
      };

    private:
    // Attributes.
      unsigned int          _order;
      float                 _minDensity;
      float*                _minDensities;
      vector<GCell*>        _gcells;
      vector<BorderSegment> _borderSegments;
      Statistics            _statistics;

    protected:
    // Constructors & Destructors.
                               GCellRoutingSet ( GCell*, float expandRatio, float minDensity );
      virtual                 ~GCellRoutingSet ();
      virtual void             _postCreate     ();
      virtual void             _preDestroy     ();
    private:
              GCellRoutingSet                  ( const GCellRoutingSet& );
              GCellRoutingSet& operator=       ( const GCellRoutingSet& );
  };


// Inline Functions.
  inline  unsigned int          GCellRoutingSet::getOrder      () const { return _order; }
  inline  const vector<GCell*>& GCellRoutingSet::getGCells     () const { return _gcells; }
  inline  Statistics&           GCellRoutingSet::getStatistics () { return _statistics; }

  inline  GCellRoutingSet::BorderSegment::BorderSegment ( TrackElement* segment, unsigned int order )
    : _segment(segment)
    , _order  (order)
  { }

  inline   Statistics::Statistics ()
    : _gcellsCount   (0)
    , _segmentsCount (0)
    , _eventsCount   (0)
  { }

  inline size_t  Statistics::getGCellsCount   () const { return _gcellsCount; }
  inline size_t  Statistics::getSegmentsCount () const { return _segmentsCount; }
  inline size_t  Statistics::getEventsCount   () const { return _eventsCount; }
  inline void    Statistics::setGCellsCount   ( size_t count ) { _gcellsCount = count; }
  inline void    Statistics::setSegmentsCount ( size_t count ) { _segmentsCount = count; }
  inline void    Statistics::setEventsCount   ( size_t count ) { _eventsCount = count; }
  inline void    Statistics::incGCellCount    ( size_t count ) { _gcellsCount += count; }
  inline void    Statistics::incSegmentsCount ( size_t count ) { _segmentsCount += count; }
  inline void    Statistics::incEventsCount   ( size_t count ) { _eventsCount += count; }

  inline Statistics& Statistics::operator+= ( const Statistics& other )
  {
    _gcellsCount   += other._gcellsCount;
    _segmentsCount += other._segmentsCount;
    _eventsCount   += other._eventsCount;
    return *this;
  }


} // End of Kite namespace.


INSPECTOR_P_SUPPORT(Kite::GCellRoutingSet);


#endif  // __KITE_GCELL_ROUTING_SET__
