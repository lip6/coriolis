// -*- mode: C++; explicit-buffer-name: "GCell.h<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
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


#ifndef  ANABATIC_GCELL_H
#define  ANABATIC_GCELL_H

#include <vector>
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
          inline bool operator()       ( GCell* lhs, GCell* rhs );
        private:
          size_t  _depth;
      };
      class CompareByKey : public binary_function<const GCell*,const GCell*,bool> {
        public:
          inline bool  operator() ( const GCell* lhs, const GCell* rhs );
      };
    public:
      class Key {
        public:
          inline        Key        ( GCell*, size_t depth );
          inline float  getDensity () const;
          inline GCell* getGCell   () const;
          inline void   update     ( size_t depth );
          friend bool   operator<  ( const Key&, const Key& );
        private:
          GCell* _gcell;
          float  _density;
      };
    public:
      static        Box                   getBorder           ( const GCell*, const GCell* );
    public:                           
      static        GCell*                create              ( AnabaticEngine* );
    public:                           
      inline        bool                  isSaturated         () const;
                    bool                  isSaturated         ( size_t depth ) const;
      inline        bool                  isInvalidated       () const;
      inline        bool                  isHFlat             () const;
      inline        bool                  isVFlat             () const;
      inline        bool                  isFlat              () const;
      inline        bool                  isDevice            () const;
      inline        bool                  isHChannel          () const;
      inline        bool                  isVChannel          () const;
      inline        bool                  isHStrut            () const;
      inline        bool                  isVStrut            () const;
      inline        bool                  isMatrix            () const;
      inline        bool                  isIoPad             () const;
                    bool                  isWest              ( GCell* ) const;
                    bool                  isEast              ( GCell* ) const;
                    bool                  isNorth             ( GCell* ) const;
                    bool                  isSouth             ( GCell* ) const;
                    Contact*              hasGContact         ( const Contact* ) const;
                    Contact*              hasGContact         ( const Net* ) const;
      inline        AnabaticEngine*       getAnabatic         () const;
      inline        Flags                 getType             () const;
      inline        DbU::Unit             getXMin             () const;
      inline        DbU::Unit             getYMin             () const;
      inline        DbU::Unit             getXMax             ( int shrink=0 ) const;
      inline        DbU::Unit             getYMax             ( int shrink=0 ) const;
      inline        Interval              getSide             ( Flags direction ) const;
      inline        Point                 getCenter           () const;
      inline const  vector<Edge*>&        getWestEdges        () const;
      inline const  vector<Edge*>&        getEastEdges        () const;
      inline const  vector<Edge*>&        getNorthEdges       () const;
      inline const  vector<Edge*>&        getSouthEdges       () const;
                    Edge*                 getEdgeTo           ( GCell*, Flags sideHint=Flags::AllSides ) const;
                    Edge*                 getEdgeAt           ( Flags sideHint, DbU::Unit u ) const;
      inline        Edges                 getEdges            ( Flags sides=Flags::AllSides ) const;
      inline        GCell*                getWest             () const;
      inline        GCell*                getEast             () const;
      inline        GCell*                getSouth            () const;
      inline        GCell*                getNorth            () const;
                    GCell*                getWest             ( DbU::Unit y ) const;
                    GCell*                getEast             ( DbU::Unit y ) const;
                    GCell*                getSouth            ( DbU::Unit x ) const;
                    GCell*                getNorth            ( DbU::Unit x ) const;
                    GCell*                getNeighborAt       ( Flags side, DbU::Unit axis ) const;
                    GCell*                getUnder            ( DbU::Unit x, DbU::Unit y ) const;
      inline        GCell*                getUnder            ( Point p ) const;
                    GCell*                hcut                ( DbU::Unit y );
                    GCell*                vcut                ( DbU::Unit x );
                    bool                  doGrid              ();
                    Contact*              getGContact         ( Net* );
      inline const  vector<Contact*>&     getGContacts        () const;
                    Contact*              breakGoThrough      ( Net* net );
                    bool                  unrefContact        ( Contact* );
                    void                  setXY               ( DbU::Unit x, DbU::Unit y );
                    void                  updateContactsPosition ();
                    void                  cleanupGlobal       ();
    // Detailed routing functions.
                    bool                  hasFreeTrack        ( size_t depth, float reserve ) const;
      inline        size_t                getDepth            () const;
                    Interval              getSide             ( unsigned int ) const;
                    float                 getHCapacity        () const;
                    float                 getVCapacity        () const;
                    float                 getDensity          ( unsigned int flags=Flags::NoFlags ) const;
                    float                 getAverageHVDensity () const;
                    float                 getMaxHVDensity     () const;
      inline        float                 getCDensity         ( unsigned int flags=Flags::NoFlags ) const;
      inline        float                 getWDensity         ( size_t depth, unsigned int flags=Flags::NoFlags  ) const;
      inline        DbU::Unit             getBlockage         ( size_t depth ) const;
      inline        float                 getFragmentation    ( size_t depth ) const;
      inline        float                 getFeedthroughs     ( size_t depth ) const;
      inline        float                 getGlobalsCount     ( size_t depth ) const;
      inline  const vector<AutoSegment*>& getHSegments        () const;
      inline  const vector<AutoSegment*>& getVSegments        () const;
      inline  const vector<AutoContact*>& getContacts         () const;
                    AutoSegments          getHStartSegments   ();
                    AutoSegments          getVStartSegments   ();
                    AutoSegments          getHStopSegments    ();
                    AutoSegments          getVStopSegments    ();
      inline        AutoSegments          getStartSegments    ( unsigned int direction );
      inline        AutoSegments          getStopSegments     ( unsigned int direction );
                    size_t                getRoutingPads      ( set<RoutingPad*>& );
      inline  const Key&                  getKey              () const;
                    size_t                checkDensity        () const;
                    bool                  checkEdgeSaturation ( size_t hreserved, size_t vreserved) const;
                    void                  setType             ( Flags );
                    void                  addBlockage         ( size_t depth, DbU::Unit );
      inline        void                  addHSegment         ( AutoSegment* );
      inline        void                  addVSegment         ( AutoSegment* );
      inline        void                  addContact          ( AutoContact* );
                    void                  removeVSegment      ( AutoSegment* );
                    void                  removeHSegment      ( AutoSegment* );
                    void                  removeContact       ( AutoContact* );
                    void                  updateContacts      ();
                    size_t                updateDensity       ();
      inline        void                  updateKey           ( size_t depth );
                    void                  truncDensities      ();
                    bool                  stepBalance         ( size_t depth, Set& invalidateds );
                    void                  rpDesaturate        ( set<Net*>& );
                    bool                  stepDesaturate      ( size_t                    depth
                                                              , set<Net*>&, AutoSegment*& moved
                                                              , unsigned int              flags=Flags::NoFlags );
                    bool                  stepNetDesaturate   ( size_t     depth
                                                              , set<Net*>& globalNets
                                                              , Set&       invalidateds );
    // Misc. functions.
      inline const  Flags&                flags               () const;
      inline        Flags&                flags               ();
                    void                  _add                ( Edge* edge, Flags side );
                    void                  _remove             ( Edge* edge, Flags side=Flags::AllSides );
                    void                  _destroyEdges       ();
                    void                  _revalidate         ();
    private:                              
                    void                  _moveEdges          ( GCell* dest, size_t ibegin, Flags flags );
    public:                                      
    // Observers.                         
      template<typename OwnerT>           
      inline        OwnerT*               getObserver         ( size_t slot );
      inline        void                  setObserver         ( size_t slot, BaseObserver* );
      inline        void                  notify              ( unsigned int flags );
    // ExtensionGo support.                      
      inline  const Name&                 staticGetName       (); 
      virtual const Name&                 getName             () const;
      virtual       void                  translate           ( const DbU::Unit&, const DbU::Unit& );
      virtual       Box                   getBoundingBox      () const;
    public:                                                
    // Inspector support.                                  
      virtual       string                _getTypeName        () const;
      virtual       string                _getString          () const;
      virtual       Record*               _getRecord          () const;
    protected:                                                
                                          GCell               ( AnabaticEngine*, DbU::Unit xmin, DbU::Unit ymin );
      virtual                            ~GCell               ();
                    GCell*                _create             ( DbU::Unit xmin, DbU::Unit ymin );
      virtual       void                  _postCreate         ();
      virtual       void                  _preDestroy         ();
    private:                        
                                          GCell               ( const GCell& );
                    GCell&                operator=           ( const GCell& );
    private:
      static  Name                  _extensionName;
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
              DbU::Unit*            _blockages;
              float                 _cDensity;
              float*                _densities;
              float*                _feedthroughs;
              float*                _fragmentations;
              float*                _globalsCount;
              Key                   _key;
  };


  inline       bool                  GCell::isHFlat       () const { return getYMin() == getYMax(); }
  inline       bool                  GCell::isVFlat       () const { return getXMin() == getXMax(); }
  inline       bool                  GCell::isFlat        () const { return isHFlat() or isVFlat(); }
  inline       bool                  GCell::isDevice      () const { return _flags & Flags::DeviceGCell; }
  inline       bool                  GCell::isHChannel    () const { return _flags & Flags::HChannelGCell; }
  inline       bool                  GCell::isVChannel    () const { return _flags & Flags::VChannelGCell; }
  inline       bool                  GCell::isHStrut      () const { return _flags & Flags::HStrutGCell; }
  inline       bool                  GCell::isVStrut      () const { return _flags & Flags::VStrutGCell; }
  inline       bool                  GCell::isMatrix      () const { return _flags & Flags::MatrixGCell; }
  inline       bool                  GCell::isIoPad       () const { return _flags & Flags::IoPadGCell; }
  inline       bool                  GCell::isSaturated   () const { return _flags & Flags::Saturated; }
  inline       bool                  GCell::isInvalidated () const { return _flags & Flags::Invalidated; }
  inline       Flags                 GCell::getType       () const { return _flags & Flags::GCellTypeMask; }
  inline       AnabaticEngine*       GCell::getAnabatic   () const { return _anabatic; }
  inline       DbU::Unit             GCell::getXMin       () const { return _xmin; }
  inline       DbU::Unit             GCell::getYMin       () const { return _ymin; }
  inline       Edges                 GCell::getEdges      ( Flags sides ) const { return new GCell_Edges(this,sides); }
  inline const vector<Edge*>&        GCell::getWestEdges  () const { return _westEdges; }
  inline const vector<Edge*>&        GCell::getEastEdges  () const { return _eastEdges; }
  inline const vector<Edge*>&        GCell::getNorthEdges () const { return _northEdges; }
  inline const vector<Edge*>&        GCell::getSouthEdges () const { return _southEdges; }
  inline       GCell*                GCell::getWest       () const { return  _westEdges.empty() ? NULL :  _westEdges[0]->getOpposite(this); }
  inline       GCell*                GCell::getEast       () const { return  _eastEdges.empty() ? NULL :  _eastEdges[0]->getOpposite(this); }
  inline       GCell*                GCell::getSouth      () const { return _southEdges.empty() ? NULL : _southEdges[0]->getOpposite(this); }
  inline       GCell*                GCell::getNorth      () const { return _northEdges.empty() ? NULL : _northEdges[0]->getOpposite(this); }
  inline       GCell*                GCell::getUnder      ( Point p ) const { return getUnder(p.getX(),p.getY()); }
  inline const vector<Contact*>&     GCell::getGContacts  () const { return _gcontacts; }
  inline        size_t               GCell::getDepth      () const { return _depth; }
  inline const vector<AutoSegment*>& GCell::getVSegments  () const { return _vsegments; }
  inline const vector<AutoSegment*>& GCell::getHSegments  () const { return _hsegments; }
  inline const vector<AutoContact*>& GCell::getContacts   () const { return _contacts; }
  inline const GCell::Key&           GCell::getKey        () const { return _key; }
  inline       void                  GCell::setType       ( Flags type ) { _flags.reset(Flags::GCellTypeMask); _flags |= (type&Flags::GCellTypeMask); };
  inline       void                  GCell::updateKey     ( size_t depth ) { _key.update(depth); }
  inline const Flags&                GCell::flags         () const { return _flags; }
  inline       Flags&                GCell::flags         () { return _flags; }

  inline DbU::Unit  GCell::getXMax ( int shrink ) const
  { return _eastEdges.empty() ?       getCell()->getAbutmentBox().getXMax() - shrink
                              : _eastEdges[0]->getOpposite(this)->getXMin() - shrink; }

  inline DbU::Unit  GCell::getYMax ( int shrink ) const
  { return _northEdges.empty() ?        getCell()->getAbutmentBox().getYMax() - shrink 
                               : _northEdges[0]->getOpposite(this)->getYMin() - shrink; }

  inline Point  GCell::getCenter () const
  { return Point( (getXMin()+getXMax())/2, (getYMin()+getYMax())/2); }

  inline Interval  GCell::getSide ( Flags direction ) const
  {
    if (direction.isset(Flags::Vertical)) return Interval( getYMin(), getYMax() );
    return Interval( getXMin(), getXMax() );
  }

  inline void  GCell::setObserver ( size_t slot, BaseObserver* observer )
  { _observable.setObserver( slot, observer ); }

  template<typename OwnerT>
  inline OwnerT* GCell::getObserver ( size_t slot )
  { return _observable.getObserver<OwnerT>(slot); }

  inline void  GCell::notify ( unsigned int flags )
  { _observable.notify( flags ); }

  inline GCell::Observable::Observable () : StaticObservable<1>() { }

  inline AutoSegments  GCell::getStartSegments ( unsigned int direction )
  { return (direction&Flags::Horizontal) ? getHStartSegments() : getVStartSegments(); }

  inline  AutoSegments  GCell::getStopSegments ( unsigned int direction )
  { return (direction&Flags::Horizontal) ? getHStopSegments() : getVStopSegments(); }

  inline  float  GCell::getCDensity ( unsigned int flags ) const
  { if (isInvalidated() and not(flags & Flags::NoUpdate)) const_cast<GCell*>(this)->updateDensity(); return _cDensity; }

  inline  float  GCell::getWDensity ( size_t depth, unsigned int flags  ) const
  { if (isInvalidated() and not(flags & Flags::NoUpdate)) const_cast<GCell*>(this)->updateDensity(); return _densities[depth]; }

  inline  float  GCell::getFragmentation ( size_t depth ) const
  { if (isInvalidated()) const_cast<GCell*>(this)->updateDensity(); return _fragmentations[depth]; }

  inline  float  GCell::getFeedthroughs ( size_t depth ) const
  { if (isInvalidated()) const_cast<GCell*>(this)->updateDensity(); return _feedthroughs[depth]; }

  inline  float  GCell::getGlobalsCount ( size_t depth ) const
  { if (isInvalidated()) const_cast<GCell*>(this)->updateDensity(); return _globalsCount[depth]; }

  inline  DbU::Unit  GCell::getBlockage ( size_t depth ) const
  { return (depth<_depth) ? _blockages[depth] : 0; }

  inline  void  GCell::addVSegment ( AutoSegment* segment )
  { _flags |= Flags::Invalidated; _vsegments.push_back(segment); }

  inline  void  GCell::addHSegment ( AutoSegment* segment )
  { _flags |= Flags::Invalidated; _hsegments.push_back(segment); }

  inline  void  GCell::addContact ( AutoContact* contact )
  { _flags |= Flags::Invalidated; _contacts.push_back(contact); }


  inline bool  operator< ( const GCell& lhs, const GCell& rhs )
  {
    if (lhs.getYMin() != rhs.getYMin()) return lhs.getYMin() < rhs.getYMin();
    if (lhs.getXMin() != rhs.getXMin()) return lhs.getXMin() < rhs.getXMin();
    return lhs.getId() < rhs.getId();
  }


// GCell::CompareByKey Inline Functions.
  inline bool  GCell::CompareByKey::operator() ( const GCell* lhs, const GCell* rhs )
  { return lhs->getKey() < rhs->getKey(); }


// GCell::Key Inline Functions.
  inline        GCell::Key::Key        ( GCell* owner, size_t depth ) : _gcell(owner), _density(owner->getWDensity(depth,Flags::NoUpdate)) {}
  inline float  GCell::Key::getDensity () const { return _density; }
  inline GCell* GCell::Key::getGCell   () const { return _gcell; }
  inline void   GCell::Key::update     ( size_t depth ) { _density=_gcell->getWDensity(depth); }

  inline bool  operator< ( const GCell::Key& lhs, const GCell::Key& rhs )
  {
    float difference = lhs._density - rhs._density;
    if (difference != 0.0) return (difference > 0.0);

    return lhs._gcell->getId() < rhs._gcell->getId();
  }


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

#endif  // ANABATIC_GCELL_H
