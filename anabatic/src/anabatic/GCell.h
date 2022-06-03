// -*- mode: C++; explicit-buffer-name: "GCell.h<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/GCell.h"                            |
// +-----------------------------------------------------------------+


#pragma  once
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <functional>
#include "hurricane/Name.h"
#include "hurricane/Box.h"
#include "hurricane/Cell.h"
#include "hurricane/Component.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/ExtensionGo.h"
namespace Hurricane {
  class Contact;
  class RoutingPad;
  class Component;
  class Horizontal;
  class Vertical;
}
#include "anabatic/Edge.h"
#include "anabatic/AutoSegments.h"


namespace Anabatic {

  using std::string;
  using std::vector;
  using std::set;
  using std::binary_function;
  using Hurricane::StaticObservable;
  using Hurricane::BaseObserver;
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Interval;
  using Hurricane::Box;
  using Hurricane::Net;
  using Hurricane::Entity;
  using Hurricane::Contact;
  using Hurricane::RoutingPad;
  using Hurricane::Cell;
  using Hurricane::Component;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;

  class AnabaticEngine;
  class GCell;
  



// -------------------------------------------------------------------
// Class  :  "GCell".

  class GCell : public ExtensionGo {
    public:
      typedef ExtensionGo  Super;
      typedef std::set< GCell*, Entity::CompareById >  Set;
      typedef std::vector<GCell*>                      Vector;
    public:
      enum DisplayMode { Boundary         = 1
                       , Density          = 2
                       };
      enum DensityMode { AverageHVDensity = 1  // Average between all densities.
                       , AverageHDensity  = 2  // Average between all H densities.
                       , AverageVDensity  = 3  // Average between all V densities.
                       , MaxHVDensity     = 4  // Maximum between average H and average V.
                       , MaxVDensity      = 5  // Maximum of V densities.
                       , MaxHDensity      = 6  // Maximum of H densities.
                       , MaxDensity       = 7  // Maximum of H & V densities.
                       };

    public:
      class Observable : public StaticObservable<1> {
        public:
          enum Indexes { Vertex = 0
                       };
        public:
          inline             Observable ();
        private:
                             Observable ( const StaticObservable& );
                 Observable& operator=  ( const StaticObservable& );
      };
    public:
      class CompareByDensity : public binary_function<GCell*,GCell*,bool> {
        public:
                      CompareByDensity ( size_t depth );
          inline bool operator()       ( GCell* lhs, GCell* rhs ) const;
        private:
          size_t  _depth;
      };
      class CompareByKey : public binary_function<const GCell*,const GCell*,bool> {
        public:
          inline bool  operator() ( const GCell* lhs, const GCell* rhs ) const;
      };
    public:
      class Key {
        public:
          inline              Key         ( const GCell*, size_t depth );
          inline             ~Key         ();
          inline       float  getDensity  () const;
          inline const GCell* getGCell    () const;
          inline       bool   isActive    () const;
          inline       bool   isSaturated () const;
          inline       void   update      ( size_t depth );
          friend       bool   operator<   ( const Key&, const Key& );
        public:
          class Compare {
            public:
              inline bool operator() ( const Key*, const Key* ) const;
          };
        private:
          const GCell* _gcell;
                float  _density;
      };
    public:
      static        uint32_t              getDisplayMode       ();
      static        void                  setDisplayMode       ( uint32_t );
      static        Box                   getBorder            ( const GCell*, const GCell* );
      static inline DbU::Unit             getMatrixHSide       ();
      static inline DbU::Unit             getMatrixVSide       ();
    public:                                                    
      static        GCell*                create               ( AnabaticEngine* );
    public:                                                    
      inline        bool                  isSatProcessed       ( size_t depth ) const;
      inline        bool                  isSaturated          () const;
                    bool                  isSaturated          ( size_t depth ) const;
      inline        bool                  isInvalidated        () const;
      inline        bool                  isHFlat              () const;
      inline        bool                  isVFlat              () const;
      inline        bool                  isFlat               () const;
      inline        bool                  isDevice             () const;
      inline        bool                  isHChannel           () const;
      inline        bool                  isVChannel           () const;
      inline        bool                  isStrut              () const;
      inline        bool                  isAnalog             () const;
      inline        bool                  isMatrix             () const;
      inline        bool                  isRow                () const;
      inline        bool                  isIoPad              () const;
      inline        bool                  isGoStraight         () const;
      inline        bool                  isHRail              () const;
      inline        bool                  isVRail              () const;
      inline        bool                  isStdCellRow         () const;
      inline        bool                  isChannelRow         () const;
                    bool                  isWest               ( GCell* ) const;
                    bool                  isEast               ( GCell* ) const;
                    bool                  isNorth              ( GCell* ) const;
                    bool                  isSouth              ( GCell* ) const;
                    bool                  hasNet               ( const Net* ) const;
                    Contact*              hasGContact          ( const Contact* ) const;
                    Contact*              hasGContact          ( const Net* ) const;
                    bool                  isHorizontalPlane    ( size_t depth ) const;
                    bool                  isVerticalPlane      ( size_t depth ) const;
      inline        AnabaticEngine*       getAnabatic          () const;
      inline        Flags                 getType              () const;
      inline        DbU::Unit             getXMin              () const;
      inline        DbU::Unit             getYMin              () const;
      inline        DbU::Unit             getXMax              ( int shrink=0 ) const;
      inline        DbU::Unit             getYMax              ( int shrink=0 ) const;
      inline        DbU::Unit             getXCenter           () const;
      inline        DbU::Unit             getYCenter           () const;
      inline        DbU::Unit             getConstraintXMax    ( int shrink=0 ) const;
      inline        DbU::Unit             getConstraintYMax    ( int shrink=0 ) const;
      inline        Interval              getSide              ( Flags direction, int shrink=0 ) const;
      inline        Interval              getHSide             ( int shrink=0 ) const;
      inline        Interval              getVSide             ( int shrink=0 ) const;
      inline        Point                 getCenter            () const;
      inline        Box                   getConstraintBox     () const;
      inline const  vector<Edge*>&        getWestEdges         () const;
      inline const  vector<Edge*>&        getEastEdges         () const;
      inline const  vector<Edge*>&        getNorthEdges        () const;
      inline const  vector<Edge*>&        getSouthEdges        () const;
                    Edge*                 getEdgeTo            ( GCell*, Flags sideHint=Flags::AllSides ) const;
                    Edge*                 getEdgeAt            ( Flags sideHint, DbU::Unit u ) const;
      inline        Edges                 getEdges             ( Flags sides=Flags::AllSides ) const;
      inline        GCell*                getWest              () const;
      inline        GCell*                getEast              () const;
      inline        GCell*                getSouth             () const;
      inline        GCell*                getNorth             () const;
                                                               
                    GCell*                getEastNMatrix       () const;
                    GCell*                getNorthNMatrix      () const;
                                                               
      inline        Edge*                 getWestEdge          () const;
      inline        Edge*                 getEastEdge          () const;
      inline        Edge*                 getSouthEdge         () const;
      inline        Edge*                 getNorthEdge         () const;
                                                               
                    GCell*                getWest              ( DbU::Unit y ) const;
                    GCell*                getEast              ( DbU::Unit y ) const;
                    GCell*                getSouth             ( DbU::Unit x ) const;
                    GCell*                getNorth             ( DbU::Unit x ) const;
                    GCell*                getNeighborAt        ( Flags side, DbU::Unit axis ) const;
                    GCell*                getUnder             ( DbU::Unit x, DbU::Unit y ) const;
      inline        GCell*                getUnder             ( Point p ) const;
                    GCell*                hcut                 ( DbU::Unit y );
                    GCell*                vcut                 ( DbU::Unit x );
                    bool                  doGrid               ();
                    Contact*              getGContact          ( Net* );
      inline const  vector<Contact*>&     getGContacts         () const;
                    Segment*              hasGoThrough         ( Net* ) const;
                    Contact*              breakGoThrough       ( Net* );
                    bool                  unrefContact         ( Contact* );
                    void                  setSouthWestCorner   ( DbU::Unit x, DbU::Unit y );
                    void                  cleanupGlobal        ();
      inline        DbU::Unit             getWidth             () const;
      inline        DbU::Unit             getHeight            () const;
    // Detailed routing functions.                             
                    bool                  hasFreeTrack         ( size_t depth, float reserve ) const;
      inline        size_t                getDepth             () const;
                    size_t                getNetCount          () const;
      inline        int                   getRpCount           () const;
                    int                   getHCapacity         () const;
                    int                   getVCapacity         () const;
                    int                   getCapacity          ( size_t depth ) const;
                    float                 getDensity           ( Flags flags=Flags::NoFlags ) const;
                    float                 getDensity           ( size_t depth ) const;
                    float                 getAverageHVDensity  () const;
                    float                 getMaxHVDensity      () const;
      inline        float                 getCDensity          ( Flags flags=Flags::NoFlags ) const;
      inline        float                 getWDensity          ( size_t depth, Flags flags=Flags::NoFlags  ) const;
      inline        DbU::Unit             getBlockage          ( size_t depth ) const;
      inline        float                 getFragmentation     ( size_t depth ) const;
      inline        float                 getFeedthroughs      ( size_t depth ) const;
      inline        float                 getGlobalsCount      ( size_t depth ) const;
      inline  const vector<AutoSegment*>& getHSegments         () const;
      inline  const vector<AutoSegment*>& getVSegments         () const;
      inline  const vector<AutoContact*>& getContacts          () const;
                    AutoSegments          getHStartSegments    ();
                    AutoSegments          getVStartSegments    ();
                    AutoSegments          getHStopSegments     ();
                    AutoSegments          getVStopSegments     ();
      inline        AutoSegments          getStartSegments     ( Flags direction );
      inline        AutoSegments          getStopSegments      ( Flags direction );
                    size_t                getRoutingPads       ( set<RoutingPad*>& );
      inline  const Key&                  getKey               () const;
      inline        Key*                  cloneKey             ( size_t depth ) const;
      inline        Key*                  getLastClonedKey     () const;
      inline        void                  clearClonedKey       () const;
                    size_t                checkDensity         () const;
                    bool                  checkEdgeSaturation  ( size_t hreserved, size_t vreserved) const;
                    void                  setType              ( Flags );
      inline        void                  setSatProcessed      ( size_t depth );
                    void                  postGlobalAnnotate   ();
                    void                  addBlockage          ( size_t depth, DbU::Unit );
      inline        void                  addHSegment          ( AutoSegment* );
      inline        void                  addVSegment          ( AutoSegment* );
      inline        void                  addContact           ( AutoContact* );
                    void                  removeVSegment       ( AutoSegment* );
                    void                  removeHSegment       ( AutoSegment* );
                    void                  removeContact        ( AutoContact* );
                    void                  updateGContacts      ( Flags flags );
                    void                  updateContacts       ();
                    size_t                updateDensity        ();
      inline        void                  updateKey            ( size_t depth );
                    void                  truncDensities       ();
                    bool                  stepBalance          ( size_t depth, Set& invalidateds );
                    void                  rpDesaturate         ( set<Net*>& );
                    bool                  stepDesaturate       ( size_t                    depth
                                                               , set<Net*>&, AutoSegment*& moved
                                                               , Flags                     flags=Flags::NoFlags );
                    bool                  stepNetDesaturate    ( size_t     depth
                                                               , set<Net*>& globalNets
                                                               , Set&       invalidateds );
      inline        void                  incRpCount           ( int );
                    void                  forceEdgesCapacities ( unsigned int hcapacities, unsigned int vcapacities );
    // Misc. functions.
      inline const  Flags&                flags                () const;
      inline        Flags&                flags                ();
                    void                  _add                 ( Edge* edge, Flags side );
                    void                  _remove              ( Edge* edge, Flags side=Flags::AllSides );
                    void                  _destroyEdges        ();
    private:                                                   
                    void                  _moveEdges           ( GCell* dest, size_t ibegin, Flags flags );
    public:                                                    
    // Observers.                                              
      template<typename OwnerT>                                
      inline        OwnerT*               getObserver          ( size_t slot );
      inline        void                  setObserver          ( size_t slot, BaseObserver* );
      inline        void                  notify               ( unsigned int flags );
    // ExtensionGo support.                                    
      inline  const Name&                 staticGetName        (); 
      virtual const Name&                 getName              () const;
      virtual       void                  translate            ( const DbU::Unit&, const DbU::Unit& );
      virtual       Box                   getBoundingBox       () const;
      virtual       void                  invalidate           ( bool propagateFlag=true );
      virtual       void                  materialize          ();
    public:                                                    
    // Inspector support.                                      
      virtual       string                _getTypeName         () const;
      virtual       string                _getString           () const;
      virtual       Record*               _getRecord           () const;
    protected:                                                 
                                          GCell                ( AnabaticEngine*, DbU::Unit xmin, DbU::Unit ymin );
      virtual                            ~GCell                ();
                    GCell*                _create              ( DbU::Unit xmin, DbU::Unit ymin );
      virtual       void                  _postCreate          ();
      virtual       void                  _preDestroy          ();
    private:                                                   
                                          GCell                ( const GCell& );
                    GCell&                operator=            ( const GCell& );
    private:
      static  Name                  _extensionName;
      static  uint32_t              _displayMode;
      static  DbU::Unit             _matrixHSide;
      static  DbU::Unit             _matrixVSide;
              Observable            _observable;
              AnabaticEngine*       _anabatic;
              Flags                 _flags;
              vector<Edge*>         _westEdges;
              vector<Edge*>         _eastEdges;
              vector<Edge*>         _southEdges;
              vector<Edge*>         _northEdges;
              DbU::Unit             _xmin;
              DbU::Unit             _ymin;
              vector<Contact*>      _gcontacts;
              vector<AutoSegment*>  _vsegments;
              vector<AutoSegment*>  _hsegments;
              vector<AutoContact*>  _contacts;
              size_t                _depth;
              size_t                _pinDepth;
              uint32_t              _satProcessed;
              int                   _rpCount;
              DbU::Unit*            _blockages;
              float                 _cDensity;
              float*                _densities;
              float*                _feedthroughs;
              float*                _fragmentations;
              float*                _globalsCount;
              Key                   _key;
      mutable Key*                  _lastClonedKey;
  };


  inline       bool                  GCell::isHFlat          () const { return getYMin() == getYMax(); }
  inline       bool                  GCell::isVFlat          () const { return getXMin() == getXMax(); }
  inline       bool                  GCell::isFlat           () const { return isHFlat() or isVFlat(); }
  inline       bool                  GCell::isDevice         () const { return _flags & Flags::DeviceGCell; }
  inline       bool                  GCell::isHChannel       () const { return _flags & Flags::HChannelGCell; }
  inline       bool                  GCell::isVChannel       () const { return _flags & Flags::VChannelGCell; }
  inline       bool                  GCell::isStrut          () const { return _flags & Flags::StrutGCell; }
  inline       bool                  GCell::isAnalog         () const { return _flags & Flags::AnalogGCellMask; }
  inline       bool                  GCell::isMatrix         () const { return _flags & Flags::MatrixGCell; }
  inline       bool                  GCell::isRow            () const { return _flags & Flags::RowGCellMask; }
  inline       bool                  GCell::isIoPad          () const { return _flags & Flags::IoPadGCell; }
  inline       bool                  GCell::isGoStraight     () const { return _flags & Flags::GoStraight; }
  inline       bool                  GCell::isHRail          () const { return _flags & Flags::HRailGCell; }
  inline       bool                  GCell::isVRail          () const { return _flags & Flags::VRailGCell; }
  inline       bool                  GCell::isStdCellRow     () const { return _flags & Flags::StdCellRow; }
  inline       bool                  GCell::isChannelRow     () const { return _flags & Flags::ChannelRow; }
  inline       bool                  GCell::isSaturated      () const { return _flags & Flags::Saturated; }
  inline       bool                  GCell::isInvalidated    () const { return _flags & Flags::Invalidated; }
  inline       DbU::Unit             GCell::getMatrixHSide   () { return _matrixHSide; }
  inline       DbU::Unit             GCell::getMatrixVSide   () { return _matrixVSide; }
  inline       Flags                 GCell::getType          () const { return _flags & Flags::GCellTypeMask; }
  inline       AnabaticEngine*       GCell::getAnabatic      () const { return _anabatic; }
  inline       DbU::Unit             GCell::getXMin          () const { return _xmin; }
  inline       DbU::Unit             GCell::getYMin          () const { return _ymin; }
  inline       Interval              GCell::getHSide         ( int shrink ) const { return getSide(Flags::Horizontal,shrink); }
  inline       Interval              GCell::getVSide         ( int shrink ) const { return getSide(Flags::Vertical  ,shrink); }
  inline       Edges                 GCell::getEdges         ( Flags sides ) const { return new GCell_Edges(this,sides); }
  inline const vector<Edge*>&        GCell::getWestEdges     () const { return _westEdges; }
  inline const vector<Edge*>&        GCell::getEastEdges     () const { return _eastEdges; }
  inline const vector<Edge*>&        GCell::getNorthEdges    () const { return _northEdges; }
  inline const vector<Edge*>&        GCell::getSouthEdges    () const { return _southEdges; }
  inline       GCell*                GCell::getWest          () const { return  _westEdges.empty() ? NULL :  _westEdges[0]->getOpposite(this); }
  inline       GCell*                GCell::getEast          () const { return  _eastEdges.empty() ? NULL :  _eastEdges[0]->getOpposite(this); }
  inline       GCell*                GCell::getSouth         () const { return _southEdges.empty() ? NULL : _southEdges[0]->getOpposite(this); }
  inline       GCell*                GCell::getNorth         () const { return _northEdges.empty() ? NULL : _northEdges[0]->getOpposite(this); }
                                                             
  inline       Edge*                 GCell::getWestEdge      () const { return  _westEdges.empty() ? NULL :  _westEdges[0]; }
  inline       Edge*                 GCell::getEastEdge      () const { return  _eastEdges.empty() ? NULL :  _eastEdges[0]; }
  inline       Edge*                 GCell::getSouthEdge     () const { return _southEdges.empty() ? NULL : _southEdges[0]; }
  inline       Edge*                 GCell::getNorthEdge     () const { return _northEdges.empty() ? NULL : _northEdges[0]; }
                                                             
  inline       GCell*                GCell::getUnder         ( Point p ) const { return getUnder(p.getX(),p.getY()); }
  inline const vector<Contact*>&     GCell::getGContacts     () const { return _gcontacts; }
  inline       size_t                GCell::getDepth         () const { return _depth; }
  inline       int                   GCell::getRpCount       () const { return _rpCount; }
         const vector<AutoSegment*>& GCell::getVSegments     () const { return _vsegments; }
  inline const vector<AutoSegment*>& GCell::getHSegments     () const { return _hsegments; }
  inline const vector<AutoContact*>& GCell::getContacts      () const { return _contacts; }
                                                             
  inline       DbU::Unit             GCell::getWidth         () const { return (getXMax()-getXMin()); }
  inline       DbU::Unit             GCell::getHeight        () const { return (getYMax()-getYMin()); }
  inline       float                 GCell::getDensity       ( size_t depth ) const { return (depth<_depth) ? _densities[depth] : 0.0; }
                                                             
  inline const GCell::Key&           GCell::getKey           () const { return _key; }
  inline       GCell::Key*           GCell::cloneKey         ( size_t depth ) const { _lastClonedKey = new Key(this,depth); return _lastClonedKey; }
  inline       void                  GCell::clearClonedKey   () const { _lastClonedKey=NULL; }
  inline       GCell::Key*           GCell::getLastClonedKey () const { return _lastClonedKey; }
  inline       void                  GCell::setType          ( Flags type ) { _flags.reset(Flags::GCellTypeMask); _flags |= (type&Flags::GCellTypeMask); };
  inline       void                  GCell::updateKey        ( size_t depth ) { _key.update(depth); }
  inline const Flags&                GCell::flags            () const { return _flags; }
  inline       Flags&                GCell::flags            () { return _flags; }

  inline DbU::Unit  GCell::getXMax ( int shrink ) const
  { return _eastEdges.empty() ? getCell()->getAbutmentBox().getXMax() - shrink
                              : _eastEdges[0]->getOpposite(this)->getXMin() - shrink; }

  inline DbU::Unit  GCell::getYMax ( int shrink ) const
  { return _northEdges.empty() ?        getCell()->getAbutmentBox().getYMax() - shrink
                               : _northEdges[0]->getOpposite(this)->getYMin() - shrink; }

  inline DbU::Unit GCell::getXCenter () const { return (getXMin()+getXMax())/2; }
  inline DbU::Unit GCell::getYCenter () const { return (getYMin()+getYMax())/2; }

  inline DbU::Unit  GCell::getConstraintXMax ( int shrink ) const
  { return getXMax( shrink + (_eastEdges.empty() ? 0 : 1) ); }

  inline DbU::Unit  GCell::getConstraintYMax ( int shrink ) const
  { return getYMax( shrink + (_northEdges.empty() ? 0 : 1) ); }

  inline Point  GCell::getCenter () const
  { return Point( (getXMin()+getXMax())/2, (getYMin()+getYMax())/2); }

  inline Box  GCell::getConstraintBox () const
  { return Box( getXMin(), getYMin(), getConstraintXMax(), getConstraintYMax() ); }

  inline Interval  GCell::getSide ( Flags direction, int shrink ) const
  {
    if (direction.isset(Flags::Vertical)) return Interval( getYMin(), getConstraintYMax(shrink) );
    return Interval( getXMin(), getConstraintXMax(shrink) );
  }

  inline void   GCell::incRpCount ( int delta )
  { _rpCount = (_rpCount + delta > 0) ? (_rpCount + delta) : 0; }

  
  inline void  GCell::setObserver ( size_t slot, BaseObserver* observer )
  { _observable.setObserver( slot, observer ); }

  template<typename OwnerT>
  inline OwnerT* GCell::getObserver ( size_t slot )
  { return _observable.getObserver<OwnerT>(slot); }

  inline void  GCell::notify ( unsigned int flags )
  { _observable.notify( flags ); }

  inline GCell::Observable::Observable () : StaticObservable<1>() { }

  inline AutoSegments  GCell::getStartSegments ( Flags direction )
  { return (direction&Flags::Horizontal) ? getHStartSegments() : getVStartSegments(); }

  inline  AutoSegments  GCell::getStopSegments ( Flags direction )
  { return (direction&Flags::Horizontal) ? getHStopSegments() : getVStopSegments(); }

  inline  float  GCell::getCDensity ( Flags flags ) const
  { if (isInvalidated() and not(flags & Flags::NoUpdate)) const_cast<GCell*>(this)->updateDensity(); return _cDensity; }

  inline  float  GCell::getWDensity ( size_t depth, Flags flags  ) const
  { if (isInvalidated() and not(flags & Flags::NoUpdate)) const_cast<GCell*>(this)->updateDensity(); return _densities[depth]; }

  inline  float  GCell::getFragmentation ( size_t depth ) const
  { if (isInvalidated()) const_cast<GCell*>(this)->updateDensity(); return _fragmentations[depth]; }

  inline  float  GCell::getFeedthroughs ( size_t depth ) const
  { if (isInvalidated()) const_cast<GCell*>(this)->updateDensity(); return _feedthroughs[depth]; }

  inline  float  GCell::getGlobalsCount ( size_t depth ) const
  { if (isInvalidated()) const_cast<GCell*>(this)->updateDensity(); return _globalsCount[depth]; }

  inline  DbU::Unit  GCell::getBlockage ( size_t depth ) const
  { return (depth<_depth) ? _blockages[depth] : 0; }

  inline  void  GCell::addHSegment ( AutoSegment* segment )
  { _flags |= Flags::Invalidated; _hsegments.push_back(segment); }

  inline void  GCell::addVSegment ( AutoSegment* segment )
  { _flags |= Flags::Invalidated; _vsegments.push_back(segment); }

  inline  void  GCell::addContact ( AutoContact* contact )
  { _flags |= Flags::Invalidated; _contacts.push_back(contact); }

  inline bool GCell::isSatProcessed ( size_t depth ) const
  { return (_satProcessed & (1 << depth)); }

  inline void GCell::setSatProcessed ( size_t depth )
  { _satProcessed |= (1 << depth); }


  inline bool  operator< ( const GCell& lhs, const GCell& rhs )
  {
    if (lhs.getYMin() != rhs.getYMin()) return lhs.getYMin() < rhs.getYMin();
    if (lhs.getXMin() != rhs.getXMin()) return lhs.getXMin() < rhs.getXMin();
    return lhs.getId() < rhs.getId();
  }

// GCell::CompareByKey Inline Functions.
  inline bool  GCell::CompareByKey::operator() ( const GCell* lhs, const GCell* rhs ) const
  { return lhs->getKey() < rhs->getKey(); }


// GCell::Key Inline Functions.
  inline  GCell::Key::Key ( const GCell* owner, size_t depth )
    : _gcell(owner)
    , _density(owner->getWDensity(depth,Flags::NoUpdate))
  { }

  inline  GCell::Key::~Key ()
  { if (isActive()) _gcell->clearClonedKey(); }

  inline       float  GCell::Key::getDensity  () const { return _density; }
  inline const GCell* GCell::Key::getGCell    () const { return _gcell; }
  inline       void   GCell::Key::update      ( size_t depth ) { _density=_gcell->getWDensity(depth); }
  inline       bool   GCell::Key::isActive    () const { return (this == _gcell->getLastClonedKey()); }
  inline       bool   GCell::Key::isSaturated () const { return _gcell->isSaturated(); }

  inline bool  operator< ( const GCell::Key& lhs, const GCell::Key& rhs )
  {
    float difference = lhs._density - rhs._density;
    if (difference != 0.0) return (difference > 0.0);

    return lhs._gcell->getId() < rhs._gcell->getId();
  }

  inline bool  GCell::Key::Compare::operator() ( const GCell::Key* lhs, const GCell::Key* rhs ) const
  {
  //if (lhs->isSaturated() xor rhs->isSaturated()) return lhs->isSaturated(); 
      
    float difference = lhs->_density - rhs->_density;
    if (difference != 0.0) return (difference < 0.0);

    return lhs->_gcell->getId() < rhs->_gcell->getId();
  }


// -------------------------------------------------------------------
// Class  :  "GCellKeyQueue".

  typedef std::priority_queue< GCell::Key*, std::vector<GCell::Key*>, GCell::Key::Compare >  GCellKeyQueue;


// -------------------------------------------------------------------
// Class  :  "GCellDensitySet".
 
  class GCellDensitySet {
    public:
                           GCellDensitySet ( size_t depth );
                           GCellDensitySet ( size_t depth, const std::vector<GCell*>& );
                          ~GCellDensitySet ();
      inline       bool    empty           () const;
      inline       size_t  size            () const;
      inline const std::set<GCell*,GCell::CompareByKey>&
                           getGCells       () const;
      inline       void    insert          ( GCell* );
      inline       void    erase           ( GCell* );
      inline       void    unqueue         ( GCell* );
                   void    requeue         ();
    private:
      size_t                                _depth;
      std::set<GCell*,GCell::CompareByKey>  _set;
      GCell::Set                            _requests;
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
  bool    isLess          ( const GCell* lhs, const GCell* rhs, Flags direction );
  bool    isGreater       ( const GCell* lhs, const GCell* rhs, Flags direction );


}  // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::GCell);
