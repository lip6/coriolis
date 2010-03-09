
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GCell.h"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KATABATIC_GCELL__
#define  __KATABATIC_GCELL__

#include  <set>
#include  <vector>
#include  <iostream>
#include  <functional>

#include  "hurricane/DbU.h"
#include  "hurricane/Point.h"
#include  "hurricane/Box.h"
#include  "hurricane/Interval.h"
#include  "hurricane/ExtensionGo.h"
namespace Hurricane {
  class Name;
}

#include  "crlcore/RoutingLayerGauge.h"
#include  "katabatic/AutoSegments.h"


namespace Katabatic {


  using std::set;
  using std::vector;
  using std::ostream;
  using std::binary_function;
  using Hurricane::_TName;
  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Box;
  using Hurricane::Interval;
  using Hurricane::ExtensionGo;

  class GCellGrid;
  class AutoContact;
  class AutoSegment;


// -------------------------------------------------------------------
// Class  :  "Katabatic::GCell".

 
  class GCell : public ExtensionGo {

    public:
      class CompareByDensity : public binary_function<GCell*,GCell*,bool> {
        public:
               CompareByDensity ( unsigned int depth );
          bool operator()       ( GCell* lhs, GCell* rhs );
        private:
          unsigned int  _depth;
      };

    public:
    // Static Utilities.
      static  bool                  areDensityConnex    ( GCell* a, GCell* b );
    // Static Accessors.                                
      static  size_t                getAllocateds       ();
      static  DbU::Unit             getTopRightShrink   ();
      static  const Name&           getStaticName       ();
      virtual const Name&           getName             () const;
    // Accessors.                                       
      inline  bool                  isSaturated         () const;
              bool                  isSaturated         ( unsigned int depth ) const;
      inline  bool                  isValid             () const;
              bool                  isAboveDensity      ( float threshold ) const;
              bool                  hasFreeTrack        ( size_t depth ) const;
      inline  GCellGrid*            getGCellGrid        () const;
      inline  unsigned int          getDepth            () const;
      inline  unsigned int          getIndex            () const;
              unsigned int          getRow              () const;
              unsigned int          getColumn           () const;
              void                  getDensities        ( float* ) const;
      virtual void                  translate           ( const DbU::Unit&, const DbU::Unit& );
      virtual Box                   getBoundingBox      () const;
      inline  Point                 getCenter           () const;
      inline  DbU::Unit             getX                () const;
      inline  DbU::Unit             getY                () const;
      inline  DbU::Unit             getXMax             () const;
      inline  DbU::Unit             getYMax             () const;
              Interval              getUSide            ( unsigned int ) const;
              GCell*                getLeft             () const;
              GCell*                getRight            () const;
              GCell*                getUp               () const;
              GCell*                getDown             () const;
              float                 getHCapacity        () const;
              float                 getVCapacity        () const;
      inline  float                 getCDensity         ( bool update=true ) const;
      inline  float                 getDensity          ( unsigned int depth, bool update=true  ) const;
              float                 getDensity          ( bool update=true ) const;
              float                 getMaxHVDensity     ( bool update=true ) const;
              float                 getStiffness        () const;
      inline  unsigned int          getSegmentCount     () const;
      inline  unsigned int          getRoutedCount      () const;
      inline  vector<AutoSegment*>* getVSegments        ();
      inline  vector<AutoSegment*>* getHSegments        ();
      inline  vector<AutoContact*>* getContacts         ();
              AutoSegments          getHStartSegments   ();
              AutoSegments          getVStartSegments   ();
              AutoSegments          getHStopSegments    ();
              AutoSegments          getVStopSegments    ();
      inline  AutoSegments          getStartSegments    ( unsigned int direction );
      inline  AutoSegments          getStopSegments     ( unsigned int direction );
              size_t                checkDensity        () const;
              bool                  checkEdgeSaturation ( float threshold ) const;
    // Modifiers.
              void                  incSegmentCount     ( int count );
              void                  incRoutedCount      ( int count );
              void                  addBlockage         ( unsigned int depth, float );
      inline  void                  addVSegment         ( AutoSegment* );
      inline  void                  addHSegment         ( AutoSegment* );
      inline  void                  addContact          ( AutoContact* );
              void                  removeVSegment      ( AutoSegment* );
              void                  removeHSegment      ( AutoSegment* );
              void                  removeContact       ( AutoContact* );
              void                  updateContacts      ();
              size_t                updateDensity       ();
              void                  desaturate          ( unsigned int depth, set<Net*>& );
              bool                  stepDesaturate      ( unsigned int depth, set<Net*>& );
      inline  void                  invalidate          ();
    // Inspector Management.                            
              Record*               _getRecord          () const;
              string                _getString          () const;
      inline  string                _getTypeName        () const;
              void                  _xmlWrite           ( ostream& o ) const;

    private:
    // Static Attributes.
      static  const Name            _goName;
      static  size_t                _allocateds;
      static  DbU::Unit             _topRightShrink;
    // Attributes.
              GCellGrid*            _gcellGrid;
              unsigned int          _index;
              vector<AutoSegment*>  _vsegments;
              vector<AutoSegment*>  _hsegments;
              vector<AutoContact*>  _contacts;
              Box                   _box;
              size_t                _depth;
              size_t                _pinDepth;
              float*                _blockages;
              float                 _cDensity;
              float*                _densities;
              float*                _saturateDensities;
              unsigned int          _segmentCount;
              unsigned int          _routedSegmentCount;
              bool                  _saturated;
              bool                  _invalid;

    protected:
    // Constructors & Destructors.
                     GCell       ( GCellGrid*   gcellGrid
                                 , unsigned int index
                                 , const Box&   box
                                 );
      inline         ~GCell      ();
      inline  void   _postCreate ();
      inline  void   _preDestroy ();
      static  GCell* create      ( GCellGrid*   gcellGrid
                                 , unsigned int index
                                 , Box          box
                                 );
    private:
                     GCell       ( const GCell& );
              GCell& operator=   ( const GCell& );

      friend class GCellGrid;
  };


// Inline Functions.
  inline  bool                  GCell::isSaturated       () const { return _saturated; }
  inline  bool                  GCell::isValid           () const { return !_invalid; }
  inline  GCellGrid*            GCell::getGCellGrid      () const { return _gcellGrid; }
  inline  unsigned int          GCell::getDepth          () const { return _depth; }
  inline  unsigned int          GCell::getIndex          () const { return _index; }
  inline  Point                 GCell::getCenter         () const { return _box.getCenter(); }
  inline  DbU::Unit             GCell::getX              () const { return _box.getXMin(); }
  inline  DbU::Unit             GCell::getY              () const { return _box.getYMin(); }
  inline  DbU::Unit             GCell::getXMax           () const { return _box.getXMax(); }
  inline  DbU::Unit             GCell::getYMax           () const { return _box.getYMax(); }
  inline  vector<AutoSegment*>* GCell::getVSegments      ()       { return &_vsegments; }
  inline  vector<AutoSegment*>* GCell::getHSegments      ()       { return &_hsegments; }
  inline  vector<AutoContact*>* GCell::getContacts       ()       { return &_contacts; }
  inline  unsigned int          GCell::getSegmentCount   () const { return _segmentCount; }
  inline  unsigned int          GCell::getRoutedCount    () const { return _routedSegmentCount; }
  inline  string                GCell::_getTypeName      () const { return _TName("GCell"); }
  inline  void                  GCell::invalidate        () { _invalid = true; }

  inline  AutoSegments  GCell::getStartSegments ( unsigned int direction )
  { return (direction&Constant::Horizontal) ? getHStartSegments() : getVStartSegments(); }

  inline  AutoSegments  GCell::getStopSegments ( unsigned int direction )
  { return (direction&Constant::Horizontal) ? getHStopSegments() : getVStopSegments(); }

  inline  float  GCell::getCDensity ( bool update ) const
  { if (_invalid and update) const_cast<GCell*>(this)->updateDensity(); return _cDensity; }

  inline  float  GCell::getDensity ( unsigned int depth, bool update  ) const
  { if (_invalid and update) const_cast<GCell*>(this)->updateDensity(); return _densities[depth]; }

  inline  void  GCell::addVSegment ( AutoSegment* segment )
  { invalidate(); _vsegments.push_back(segment); }

  inline  void  GCell::addHSegment ( AutoSegment* segment )
  { invalidate(); _hsegments.push_back(segment); }

  inline  void  GCell::addContact ( AutoContact* contact )
  { invalidate(); _contacts.push_back(contact); }


// -------------------------------------------------------------------
// Utilities.


  string  getVectorString ( float*, size_t );


} // End of Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::GCell);


#endif  // __KATABATIC_GCELL__
