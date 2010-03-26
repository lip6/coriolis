
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
// |  C++ Header  :       "./GCell.h"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KITE_GCELL__
#define  __KITE_GCELL__

#include  <vector>
#include  <set>
#include  <iostream>
#include  <string>

#include  "hurricane/Collection.h"
namespace Hurricane {
  class Name;
}

#include  "katabatic/GCell.h"


namespace Kite {


  using std::vector;
  using std::string;
  using Hurricane::_TName;
  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Box;
  using Hurricane::Interval;
  using Hurricane::ExtensionGo;
  using Hurricane::GenericCollection;
  using Hurricane::GenericLocator;
  using Hurricane::GenericFilter;
  using Katabatic::AutoSegment;
  using Katabatic::AutoContact;

  class TrackElement;
  class GCellGrid;


// -------------------------------------------------------------------
// Class  :  "GCell".
 

  class GCell {

    public:
    // Sub-Class: "CompareByDensity()".
      class CompareByDensity {
        public:
          bool  operator() ( GCell* lhs, GCell* rhs );
      };
    // Sub-Class: "CompareByStiffness()".
      class CompareByStiffness {
        public:
          bool  operator() ( GCell* lhs, GCell* rhs );
      };
      friend class Compare;

    public:
      static  bool                         areDensityConnex   ( GCell* a, GCell* b );
      static  GCell*                       getLowestOrder     ( TrackElement* );
      static  TrackElement*                addTrackSegment    ( GCell*, AutoSegment*, bool loading=false );
      static  GCell*                       create             ( GCellGrid*, Katabatic::GCell* );
    public:                                                   
              void                         destroy            ();
      inline  Katabatic::GCell*            base               ();
      inline  bool                         isInRoutingSet     () const;
      inline  unsigned int                 getOrder           () const;
      inline  const vector<TrackElement*>& getOwnedSegments   () const;
      inline  void                         setInRoutingSet    ( bool state );
      inline  void                         setRouted          ( bool state );
      inline  TrackElement*                getLeftRp          ( size_t ) const;
      inline  TrackElement*                getRightRp         ( size_t ) const;
              void                         addTrackSegment    ( TrackElement* );
              void                         removeTrackSegment ( TrackElement* );
              void                         loadRouting        ( unsigned int order );
              void                         computeBorder      ();
              double                       getOwnedWireLength () const;
    public:                                                   
      inline  bool                         isAboveDensity     ( float threshold ) const;
      inline  bool                         isRouted           () const;
      inline  unsigned int                 getDepth           () const;
      inline  unsigned int                 getIndex           () const;
      inline  unsigned int                 getColumn          () const;
      inline  unsigned int                 getRow             () const;
      inline  Box                          getBoundingBox     () const;
      inline  DbU::Unit                    getX               () const;
      inline  DbU::Unit                    getY               () const;
      inline  DbU::Unit                    getXMax            () const;
      inline  DbU::Unit                    getYMax            () const;
      inline  Interval                     getUSide           ( unsigned int, bool noShrink ) const;
      inline  float                        getDensity         ( size_t depth ) const;
      inline  float                        getCDensity        () const;
      inline  float                        getDensity         () const;
      inline  float                        getStiffness       () const;
              GCell*                       getLeft            () const;
              GCell*                       getRight           () const;
              GCell*                       getUp              () const;
              GCell*                       getDown            () const;
      inline  vector<AutoSegment*>*        getVSegments       ();
      inline  vector<AutoSegment*>*        getHSegments       ();
      inline  vector<AutoContact*>*        getContacts        ();
      inline  unsigned int                 getSegmentCount    () const;
      inline  unsigned int                 getRoutedCount     () const;
      inline  void                         incSegmentCount    ( int count );
      inline  void                         incRoutedCount     ( int count );
      inline  void                         addBlockage        ( unsigned int depth, float );
              void                         anticipateRouting  ( unsigned int );
      inline  size_t                       checkDensity       () const;
      inline  size_t                       updateDensity      ();
      virtual Record*                      _getRecord         () const;
      virtual string                       _getString         () const;
      virtual string                       _getTypeName       () const;

    protected:
    // Attributes.
      GCellGrid*             _gcellGrid;
      Katabatic::GCell*      _base;
      vector<TrackElement*>  _segments;
      unsigned int           _order;
      bool                   _isInRoutingSet;
      bool                   _isRouted;
      TrackElement*          _leftSegments [2];
      TrackElement*          _rightSegments[2];

    protected:
    // Constructors & Destructors.
                     GCell            ( GCellGrid*, Katabatic::GCell* );
      virtual       ~GCell            ();
    private:
                     GCell            ( const GCell& );
                     GCell& operator= ( const GCell& );
  };


// GCell Inline Functions.
  inline  Katabatic::GCell*            GCell::base               () { return _base; }
  inline  bool                         GCell::isInRoutingSet     () const { return _isInRoutingSet; }
  inline  bool                         GCell::isRouted           () const { return _isRouted; }
  inline  bool                         GCell::isAboveDensity     ( float threshold ) const { return _base->isAboveDensity(threshold); }
  inline  unsigned int                 GCell::getColumn          () const { return _base->getColumn(); }
  inline  unsigned int                 GCell::getRow             () const { return _base->getRow(); }
  inline  unsigned int                 GCell::getDepth           () const { return _base->getDepth(); }
  inline  unsigned int                 GCell::getOrder           () const { return _order; }
  inline  const vector<TrackElement*>& GCell::getOwnedSegments   () const { return _segments; }
  inline  TrackElement*                GCell::getLeftRp          ( size_t i ) const { return _leftSegments[i]; }
  inline  TrackElement*                GCell::getRightRp         ( size_t i ) const { return _rightSegments[i]; }
  inline  void                         GCell::setInRoutingSet    ( bool state ) { _isInRoutingSet = state; }
  inline  void                         GCell::setRouted          ( bool state ) { _isRouted = state; }
  inline  unsigned int                 GCell::getIndex           () const { return _base->getIndex(); }
  inline  Box                          GCell::getBoundingBox     () const { return _base->getBoundingBox(); }
  inline  DbU::Unit                    GCell::getX               () const { return _base->getX(); }
  inline  DbU::Unit                    GCell::getY               () const { return _base->getY(); }
  inline  DbU::Unit                    GCell::getXMax            () const { return _base->getXMax(); }
  inline  DbU::Unit                    GCell::getYMax            () const { return _base->getYMax(); }
  inline  float                        GCell::getDensity         ( size_t depth ) const { return _base->getDensity(depth); }
  inline  float                        GCell::getCDensity        () const { return _base->getCDensity(); }
  inline  float                        GCell::getDensity         () const { return _base->getDensity(); }
  inline  float                        GCell::getStiffness       () const { return _base->getStiffness(); }
  inline  vector<AutoSegment*>*        GCell::getVSegments       () { return _base->getVSegments(); }
  inline  vector<AutoSegment*>*        GCell::getHSegments       () { return _base->getHSegments(); }
  inline  vector<AutoContact*>*        GCell::getContacts        () { return _base->getContacts(); }
  inline  unsigned int                 GCell::getSegmentCount    () const { return _base->getSegmentCount(); }
  inline  unsigned int                 GCell::getRoutedCount     () const { return _base->getRoutedCount(); }
  inline  void                         GCell::incSegmentCount    ( int count ) { _base->incSegmentCount(count); }
  inline  void                         GCell::incRoutedCount     ( int count ) { _base->incRoutedCount(count); }
  inline  void                         GCell::addBlockage        ( unsigned int depth, float length ) { _base->addBlockage(depth,length); }
  inline  size_t                       GCell::checkDensity       () const { return _base->checkDensity(); }
  inline  size_t                       GCell::updateDensity      () { return _base->updateDensity(); }

  inline  Interval  GCell::getUSide ( unsigned int dir, bool noShrink ) const
  { return _base->getUSide(dir).inflate((noShrink)?Katabatic::GCell::getTopRightShrink():0,0); }

  inline bool operator< ( const GCell& lhs, const GCell& rhs ) { return lhs.getIndex() < rhs.getIndex(); }
  inline bool operator> ( const GCell& lhs, const GCell& rhs ) { return lhs.getIndex() > rhs.getIndex(); }


// -------------------------------------------------------------------
// Collections.


  typedef GenericCollection<GCell*> GCells;
  typedef GenericLocator<GCell*>    GCellLocator;
  typedef GenericFilter<GCell*>     GCellFilter;


} // End of Kite namespace.


INSPECTOR_P_SUPPORT(Kite::GCell);


#endif  // __KITE_GCELL__
