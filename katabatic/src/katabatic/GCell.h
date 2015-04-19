// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/GCell.h"                           |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_GCELL_H
#define  KATABATIC_GCELL_H

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
#include  "katabatic/Constants.h"
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


  enum GCellFunctionFlags { NoUpdate = 0x00000001 };


// -------------------------------------------------------------------
// Class  :  "Katabatic::GCell".

  enum GCellFlag { GCellInvalidated = 0x00000001
                 , GCellSaturated   = 0x00000002
                 , GCellUnderIoPad  = 0x00000004
                 };

 
  class GCell : public ExtensionGo {

    public:
      class CompareByIndex : public binary_function<const GCell*,const GCell*,bool> {
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
      class CompareByKey : public binary_function<const GCell*,const GCell*,bool> {
        public:
          bool  operator() ( const GCell* lhs, const GCell* rhs );
      };

      class Key {
        private:
          GCell* _gcell;
          float  _density;
        public:
          inline        Key        ( GCell*, unsigned int depth );
          inline float  getDensity () const;
          inline GCell* getGCell   () const;
          inline void   update     ( unsigned int depth );
          friend bool   operator<  ( const Key&, const Key& );
      };
    public:
      typedef set<GCell*,CompareByIndex>  SetIndex;

    public:
    // Static Utilities.
      static  bool                        areDensityConnex    ( GCell* a, GCell* b );
    // Static Accessors.                                      
      static  size_t                      getAllocateds       ();
      static  DbU::Unit                   getTopRightShrink   ();
      static  const Name&                 getStaticName       ();
      virtual const Name&                 getName             () const;
    // Accessors.                                             
      inline  bool                        isSaturated         () const;
              bool                        isSaturated         ( unsigned int depth ) const;
      inline  bool                        isValid             () const;
      inline  bool                        isUnderIoPad        () const;
              bool                        isAboveDensity      ( float threshold ) const;
              bool                        hasFreeTrack        ( size_t depth, float reserve ) const;
      inline  GCellGrid*                  getGCellGrid        () const;
      inline  unsigned int                getDepth            () const;
      inline  unsigned int                getIndex            () const;
              unsigned int                getRow              () const;
              unsigned int                getColumn           () const;
              GCell*                      getLeft             () const;
              GCell*                      getRight            () const;
              GCell*                      getUp               () const;
              GCell*                      getDown             () const;
              void                        getDensities        ( float* ) const;
      virtual void                        translate           ( const DbU::Unit&, const DbU::Unit& );
      virtual Box                         getBoundingBox      () const;
      inline  Point                       getCenter           () const;
      inline  DbU::Unit                   getX                () const;
      inline  DbU::Unit                   getY                () const;
      inline  DbU::Unit                   getXMax             () const;
      inline  DbU::Unit                   getYMax             () const;
              Interval                    getSide             ( unsigned int ) const;
              float                       getHCapacity        () const;
              float                       getVCapacity        () const;
              float                       getDensity          ( unsigned int flags=0 ) const;
              float                       getAverageHVDensity () const;
              float                       getMaxHVDensity     () const;
      inline  float                       getCDensity         ( unsigned int flags=0 ) const;
      inline  float                       getWDensity         ( unsigned int depth, unsigned int flags=0  ) const;
      inline  DbU::Unit                   getBlockage         ( unsigned int depth ) const;
      inline  float                       getFragmentation    ( unsigned int depth ) const;
      inline  float                       getFeedthroughs     ( unsigned int depth ) const;
      inline  float                       getGlobalsCount     ( unsigned int depth ) const;
      inline  const vector<AutoSegment*>& getHSegments        () const;
      inline  const vector<AutoSegment*>& getVSegments        () const;
      inline  const vector<AutoContact*>& getContacts         () const;
              AutoSegments                getHStartSegments   ();
              AutoSegments                getVStartSegments   ();
              AutoSegments                getHStopSegments    ();
              AutoSegments                getVStopSegments    ();
      inline  AutoSegments                getStartSegments    ( unsigned int direction );
      inline  AutoSegments                getStopSegments     ( unsigned int direction );
              size_t                      getRoutingPads      ( set<RoutingPad*>& );
      inline  const Key&                  getKey              () const;
              size_t                      checkDensity        () const;
              bool                        checkEdgeSaturation ( size_t hreserved, size_t vreserved) const;
    // Modifiers.                         
              void                        addBlockage         ( unsigned int depth, DbU::Unit );
      inline  void                        addHSegment         ( AutoSegment* );
      inline  void                        addVSegment         ( AutoSegment* );
      inline  void                        addContact          ( AutoContact* );
              void                        removeVSegment      ( AutoSegment* );
              void                        removeHSegment      ( AutoSegment* );
              void                        removeContact       ( AutoContact* );
              void                        updateContacts      ();
              size_t                      updateDensity       ();
      inline  void                        updateKey           ( unsigned int depth );
              bool                        stepBalance         ( unsigned int depth, SetIndex& invalidateds );
              void                        rpDesaturate        ( set<Net*>& );
              bool                        stepDesaturate      ( unsigned int              depth
                                                              , set<Net*>&, AutoSegment*& moved
                                                              , unsigned int              flags=0 );
              bool                        stepNetDesaturate   ( unsigned int depth
                                                              , set<Net*>&   globalNets
                                                              , SetIndex&    invalidateds );
      inline  void                        invalidateCt        ();
      inline  void                        setUnderIoPad       ();
    // Inspector Management.                                  
              Record*                     _getRecord          () const;
              string                      _getString          () const;
      inline  string                      _getTypeName        () const;
              void                        _xmlWrite           ( ostream& o ) const;

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
              unsigned int          _flags;
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
  inline  bool                        GCell::isSaturated  () const { return _flags&GCellSaturated; }
  inline  bool                        GCell::isValid      () const { return not (_flags&GCellInvalidated); }
  inline  bool                        GCell::isUnderIoPad () const { return _flags&GCellUnderIoPad; }
  inline  GCellGrid*                  GCell::getGCellGrid () const { return _gcellGrid; }
  inline  unsigned int                GCell::getDepth     () const { return _depth; }
  inline  unsigned int                GCell::getIndex     () const { return _index; }
  inline  Point                       GCell::getCenter    () const { return _box.getCenter(); }
  inline  DbU::Unit                   GCell::getX         () const { return _box.getXMin(); }
  inline  DbU::Unit                   GCell::getY         () const { return _box.getYMin(); }
  inline  DbU::Unit                   GCell::getXMax      () const { return _box.getXMax(); }
  inline  DbU::Unit                   GCell::getYMax      () const { return _box.getYMax(); }
  inline  const vector<AutoSegment*>& GCell::getVSegments () const { return _vsegments; }
  inline  const vector<AutoSegment*>& GCell::getHSegments () const { return _hsegments; }
  inline  const vector<AutoContact*>& GCell::getContacts  () const { return _contacts; }
  inline  string                      GCell::_getTypeName () const { return _TName("GCell"); }
  inline  void                        GCell::invalidateCt () { _flags |= GCellInvalidated; }
  inline  void                        GCell::setUnderIoPad() { _flags |= GCellUnderIoPad; }
  inline  const GCell::Key&           GCell::getKey       () const { return _key; }
  inline  void                        GCell::updateKey    ( unsigned int depth ) { _key.update(depth); }

  inline  AutoSegments  GCell::getStartSegments ( unsigned int direction )
  { return (direction&KbHorizontal) ? getHStartSegments() : getVStartSegments(); }

  inline  AutoSegments  GCell::getStopSegments ( unsigned int direction )
  { return (direction&KbHorizontal) ? getHStopSegments() : getVStopSegments(); }

  inline  float  GCell::getCDensity ( unsigned int flags ) const
  { if (not isValid() and not(flags & NoUpdate)) const_cast<GCell*>(this)->updateDensity(); return _cDensity; }

  inline  float  GCell::getWDensity ( unsigned int depth, unsigned int flags  ) const
  { if (not isValid() and not(flags & NoUpdate)) const_cast<GCell*>(this)->updateDensity(); return _densities[depth]; }

  inline  float  GCell::getFragmentation ( unsigned int depth ) const
  { if (not isValid()) const_cast<GCell*>(this)->updateDensity(); return _fragmentations[depth]; }

  inline  float  GCell::getFeedthroughs ( unsigned int depth ) const
  { if (not isValid()) const_cast<GCell*>(this)->updateDensity(); return _feedthroughs[depth]; }

  inline  float  GCell::getGlobalsCount ( unsigned int depth ) const
  { if (not isValid()) const_cast<GCell*>(this)->updateDensity(); return _globalsCount[depth]; }

  inline  DbU::Unit  GCell::getBlockage ( unsigned int depth ) const
  { return (depth<_depth) ? _blockages[depth] : 0; }

  inline  void  GCell::addVSegment ( AutoSegment* segment )
  { invalidateCt(); _vsegments.push_back(segment); }

  inline  void  GCell::addHSegment ( AutoSegment* segment )
  { invalidateCt(); _hsegments.push_back(segment); }

  inline  void  GCell::addContact ( AutoContact* contact )
  { invalidateCt(); _contacts.push_back(contact); }


// GCell::CompareByIndex Inline Functions.
  inline bool  GCell::CompareByIndex::operator() ( const GCell* lhs, const GCell* rhs )
  { return ( lhs->getIndex() < rhs->getIndex() ); }


// GCell::Key Inline Functions.
  inline        GCell::Key::Key        ( GCell* owner, unsigned int depth ) : _gcell(owner), _density(owner->getWDensity(depth,NoUpdate)) {}
  inline float  GCell::Key::getDensity () const { return _density; }
  inline GCell* GCell::Key::getGCell   () const { return _gcell; }
  inline void   GCell::Key::update     ( unsigned int depth ) { _density=_gcell->getWDensity(depth); }

  inline bool  operator< ( const GCell::Key& lhs, const GCell::Key& rhs )
  {
  //float difference = Hurricane::roundfp ( lhs._density - rhs._density );
    float difference = lhs._density - rhs._density;
    if (difference != 0.0) return (difference > 0.0);

    return lhs._gcell->getIndex() < rhs._gcell->getIndex();
  }


// -------------------------------------------------------------------
// Class  :  "GCellDensitySet".
 
  class GCellDensitySet {
    public:
                     GCellDensitySet ( unsigned int depth );
                     GCellDensitySet ( unsigned int depth, const std::vector<GCell*>& );
                    ~GCellDensitySet ();
      inline bool    empty           () const;
      inline size_t  size            () const;
      inline const std::set<GCell*,GCell::CompareByKey>&
                     getGCells       () const;
      inline void    insert          ( GCell* );
      inline void    erase           ( GCell* );
      inline void    unqueue         ( GCell* );
             void    requeue         ();
    private:
      unsigned int                          _depth;
      std::set<GCell*,GCell::CompareByKey>  _set;
      GCell::SetIndex                       _requests;
  };


  inline bool    GCellDensitySet::empty   () const { return _set.empty(); }
  inline size_t  GCellDensitySet::size    () const { return _set.size(); }
  inline void    GCellDensitySet::insert  ( GCell* gcell ) { _requests.insert( gcell ); }
  inline void    GCellDensitySet::erase   ( GCell* gcell ) { if (not _set.empty()) _set.erase( gcell ); }
  inline void    GCellDensitySet::unqueue ( GCell* gcell ) { insert( gcell ); }
  inline const std::set<GCell*,GCell::CompareByKey>& GCellDensitySet::getGCells () const { return _set; }


// -------------------------------------------------------------------
// Utilities.


  string  getVectorString ( float*, size_t );


  typedef std::vector<GCell*>  GCellVector;


} // End of Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::GCell);


#endif  // KATABATIC_GCELL_H
