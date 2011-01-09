
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
  class RoutingPad;
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
  using Hurricane::RoutingPad;

  class GCellGrid;
  class AutoContact;
  class AutoSegment;


// -------------------------------------------------------------------
// Class  :  "Katabatic::GCell".

 
  class GCell : public ExtensionGo {

    public:
      class CompareId : public binary_function<const GCell*,const GCell*,bool> {
        public:
          bool operator() ( const GCell* lhs, const GCell* rhs );
      };
      class CompareByDensity : public binary_function<GCell*,GCell*,bool> {
        public:
               CompareByDensity ( unsigned int depth );
          bool operator()       ( GCell* lhs, GCell* rhs );
        private:
          unsigned int  _depth;
      };
      class CompareByKey {
        public:
          bool  operator() ( const GCell* lhs, const GCell* rhs );
      };
      class Key {
        private:
          float         _density;
          unsigned int  _index;
        public:
          inline               Key        ( float, unsigned int );
          inline float         getDensity () const;
          inline unsigned int  getIndex   () const;
          inline void          update     ( GCell*, unsigned int );
          friend bool          operator<  ( const Key&, const Key& );
      };
    public:
      typedef set<GCell*,CompareId>  SetId;

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
              bool                  hasFreeTrack        ( size_t depth, float reserve ) const;
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
      inline  DbU::Unit             getBlockage         ( unsigned int depth ) const;
      inline  float                 getFragmentation    ( unsigned int depth ) const;
      inline  float                 getGlobalsCount     ( unsigned int depth ) const;
              float                 getStiffness        () const;
      inline  vector<AutoSegment*>* getVSegments        ();
      inline  vector<AutoSegment*>* getHSegments        ();
      inline  vector<AutoContact*>* getContacts         ();
              AutoSegments          getHStartSegments   ();
              AutoSegments          getVStartSegments   ();
              AutoSegments          getHStopSegments    ();
              AutoSegments          getVStopSegments    ();
      inline  AutoSegments          getStartSegments    ( unsigned int direction );
      inline  AutoSegments          getStopSegments     ( unsigned int direction );
              size_t                getRoutingPads      ( set<RoutingPad*>& );
      inline  const Key&            getKey              () const;
              size_t                checkDensity        () const;
              bool                  checkEdgeSaturation ( float threshold ) const;
    // Modifiers.
              void                  addBlockage         ( unsigned int depth, DbU::Unit );
    //        void                  addBlockedAxis      ( unsigned int depth, DbU::Unit );
      inline  void                  addVSegment         ( AutoSegment* );
      inline  void                  addHSegment         ( AutoSegment* );
      inline  void                  addContact          ( AutoContact* );
              void                  removeVSegment      ( AutoSegment* );
              void                  removeHSegment      ( AutoSegment* );
              void                  removeContact       ( AutoContact* );
              void                  updateContacts      ();
              size_t                updateDensity       ();
      inline  void                  updateKey           ( unsigned int depth );
              void                  desaturate          ( unsigned int depth, set<Net*>& );
              bool                  stepDesaturate      ( unsigned int depth, set<Net*>&, AutoSegment*& moved, bool force=false );
              bool                  stepNetDesaturate   ( unsigned int depth, set<Net*>&, SetId& invalidateds );
              void                  rpDesaturate        ( set<Net*>& );
      inline  void                  invalidate          ();
    // Inspector Management.                            
              Record*               _getRecord          () const;
              string                _getString          () const;
      inline  string                _getTypeName        () const;
              void                  _xmlWrite           ( ostream& o ) const;

    private:
      class BlockedAxis {
        public:
                                BlockedAxis ( GCell* );
          const set<DbU::Unit>& getAxisSet  ( size_t depth ) const;
          void                  addAxis     ( size_t depth, DbU::Unit );
        private:
          GCell*           _gcell;
          set<DbU::Unit>** _axisSets;
      };

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
              DbU::Unit*            _blockages;
              float                 _cDensity;
              float*                _densities;
              float*                _feedthroughs;
              float*                _fragmentations;
              float*                _globalsCount;
            //BlockedAxis           _blockedAxis; 
            //float*                _saturateDensities;
              bool                  _saturated;
              bool                  _invalid;
              Key                   _key;

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


// GCell Inline Functions.
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
  inline  string                GCell::_getTypeName      () const { return _TName("GCell"); }
  inline  void                  GCell::invalidate        () { _invalid = true; }
  inline  const GCell::Key&     GCell::getKey            () const { return _key; }
  inline  void                  GCell::updateKey         ( unsigned int depth ) { _key.update(this,depth); }

  inline  AutoSegments  GCell::getStartSegments ( unsigned int direction )
  { return (direction&Constant::Horizontal) ? getHStartSegments() : getVStartSegments(); }

  inline  AutoSegments  GCell::getStopSegments ( unsigned int direction )
  { return (direction&Constant::Horizontal) ? getHStopSegments() : getVStopSegments(); }

  inline  float  GCell::getCDensity ( bool update ) const
  { if (_invalid and update) const_cast<GCell*>(this)->updateDensity(); return _cDensity; }

  inline  float  GCell::getDensity ( unsigned int depth, bool update  ) const
  { if (_invalid and update) const_cast<GCell*>(this)->updateDensity(); return _densities[depth]; }

  inline  float  GCell::getFragmentation ( unsigned int depth ) const
  { if (_invalid) const_cast<GCell*>(this)->updateDensity(); return _fragmentations[depth]; }

  inline  float  GCell::getGlobalsCount ( unsigned int depth ) const
  { if (_invalid) const_cast<GCell*>(this)->updateDensity(); return _globalsCount[depth]; }

  inline  DbU::Unit  GCell::getBlockage ( unsigned int depth ) const
  { return (depth<_depth) ? _blockages[depth] : 0; }

  inline  void  GCell::addVSegment ( AutoSegment* segment )
  { invalidate(); _vsegments.push_back(segment); }

  inline  void  GCell::addHSegment ( AutoSegment* segment )
  { invalidate(); _hsegments.push_back(segment); }

  inline  void  GCell::addContact ( AutoContact* contact )
  { invalidate(); _contacts.push_back(contact); }


// GCell::CompareId Inline Functions.
  inline bool  GCell::CompareId::operator() ( const GCell* lhs, const GCell* rhs )
  { return ( lhs->getIndex() < rhs->getIndex() ); }


// GCell::Key Inline Functions.
  inline               GCell::Key::Key        ( float density, unsigned int index ) : _density(density), _index(index) {}
  inline float         GCell::Key::getDensity () const { return _density; }
  inline unsigned int  GCell::Key::getIndex   () const { return _index; }
  inline void          GCell::Key::update     ( GCell* gcell, unsigned int depth )
                       { _density=gcell->getDensity(depth); _index=gcell->getIndex(); }

  inline bool  operator< ( const GCell::Key& lhs, const GCell::Key& rhs )
  {
    float difference = Hurricane::roundfp ( lhs._density - rhs._density );
    if ( difference != 0.0 ) return (difference > 0.0);

    return ( lhs._index < rhs._index );
  }


// -------------------------------------------------------------------
// Class  :  "DyKeyQueue".
 

  class DyKeyQueue {
    public:
                                                  DyKeyQueue ( unsigned int depth );
                                                  DyKeyQueue ( unsigned int depth, const std::vector<GCell*>& );
                                                 ~DyKeyQueue ();
      const std::set<GCell*,GCell::CompareByKey>& getGCells  () const;
      GCell*                                      pop        ();
      void                                        push       ( GCell* );
      void                                        invalidate ( GCell* );
      void                                        revalidate ();
    private:
      unsigned int                          _depth;
      std::set<GCell*,GCell::CompareByKey>  _map;
      GCell::SetId                          _requests;
  };


// -------------------------------------------------------------------
// Utilities.


  string  getVectorString ( float*, size_t );


  typedef std::vector<GCell*>  GCellVector;


} // End of Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::GCell);


#endif  // __KATABATIC_GCELL__
