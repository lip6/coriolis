// -*- mode: C++; explicit-buffer-name: "GCell.cpp<anabatic>" -*-
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
// |  C++ Module  :  "./GCell.cpp"                                   |
// +-----------------------------------------------------------------+


#include <iostream>
#include "hurricane/Bug.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Contact.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/UpdateSession.h"
#include "anabatic/GCell.h"
#include "anabatic/AnabaticEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace Anabatic;


// -------------------------------------------------------------------
// Class  :  "::UsedFragments".

  class UsedFragments {
    private:
      class Axiss;

      class Axis {
        public:
                                Axis             ( UsedFragments*, DbU::Unit axis );
          inline DbU::Unit      getAxis          () const;
          inline UsedFragments* getUsedFragments () const;
                 void           merge            ( const Interval& mergeChunk );
                 Interval       getMaxFree       () const;
        private:
          UsedFragments* _ufragments;
          DbU::Unit      _axis;
          list<Interval> _chunks;
      };

    private:
      class AxisCompare {
        public:
          bool operator() ( const Axis* lhs, const Axis* rhs ) const;
      };

      class AxisMatch : public unary_function<Axis*,bool> {
        public:
          inline      AxisMatch  ( DbU::Unit axis );
          inline bool operator() ( const Axis* );
        private:
          DbU::Unit  _axis;
      };

    public:
                        UsedFragments ();
                       ~UsedFragments ();
      inline DbU::Unit  getPitch      () const;
      inline DbU::Unit  getMin        () const;
      inline DbU::Unit  getMax        () const;
             Interval   getMaxFree    () const;
      inline void       setSpan       ( DbU::Unit min, DbU::Unit max );
      inline void       setCapacity   ( size_t );
      inline void       incGlobals    ( size_t count=1 );
      inline void       setPitch      ( DbU::Unit );
             void       merge         ( DbU::Unit axis, const Interval& );
    private:
      DbU::Unit      _pitch;
      vector<Axis*>  _axiss;
      Interval       _span;
      size_t         _capacity;
      size_t         _globals;
  };


  UsedFragments::Axis::Axis ( UsedFragments* ufragments, DbU::Unit axis )
    : _ufragments(ufragments)
    , _axis      (axis)
    , _chunks    ()
  {
    merge( Interval( ufragments->getMin()-ufragments->getPitch(), ufragments->getMin() ) );
    merge( Interval( ufragments->getMax(), ufragments->getMax()+ufragments->getPitch() ) );
  }

  inline DbU::Unit      UsedFragments::Axis::getAxis          () const { return _axis; }
  inline UsedFragments* UsedFragments::Axis::getUsedFragments () const { return _ufragments; }


  void  UsedFragments::Axis::merge ( const Interval& chunkMerge )
  {
    // cerr << "    Merge @" << DbU::getValueString(_axis)
    //      << " " << chunkMerge << endl;

    list<Interval>::iterator imerge = _chunks.end();
    list<Interval>::iterator ichunk = _chunks.begin();

    while ( ichunk != _chunks.end() ) {
      if (chunkMerge.getVMax() < (*ichunk).getVMin()) break;

      if (chunkMerge.intersect(*ichunk)) {
        if (imerge == _chunks.end()) {
          imerge = ichunk;
          (*imerge).merge( chunkMerge );
        } else {
          (*imerge).merge( *ichunk );
          ichunk = _chunks.erase( ichunk );
          continue;
        }
      }
      ichunk++;
    }

    if (imerge == _chunks.end()) {
      _chunks.insert( ichunk, chunkMerge );
    }
  }


  Interval  UsedFragments::Axis::getMaxFree () const
  {
    list<Interval>::const_iterator ichunk =   _chunks.begin();
    list<Interval>::const_iterator iend   = --_chunks.end();

    Interval maxFree;
    for ( ; ichunk != iend ; ++ichunk ) {
      list<Interval>::const_iterator inext = ichunk;
      ++inext;

      if (inext == iend) break;

      Interval currentFree ( (*ichunk).getVMax(), (*inext).getVMin() );
      if (currentFree.getSize() > maxFree.getSize())
        maxFree = currentFree;

      // cerr << "    @" << DbU::getValueString(_axis)
      //      << " before:" << *ichunk << " after:" << *inext
      //      << " size:" << DbU::getValueString(currentFree.getSize()) << endl;
    }

    return maxFree;
  }


  inline bool  UsedFragments::AxisCompare::operator() ( const Axis* lhs, const Axis* rhs ) const
  {
    if (lhs->getAxis () < rhs->getAxis ()) return true;
    return false;
  }


  inline  UsedFragments::AxisMatch::AxisMatch ( DbU::Unit axis )
    : _axis(axis)
  { }


  inline bool  UsedFragments::AxisMatch::operator() ( const Axis* axis )
  { return (axis->getAxis() == _axis); }


  UsedFragments::UsedFragments ()
    : _pitch   (0)
    , _axiss   ()
    , _span    (false)
    , _capacity(0)
    , _globals (0)
  { }


  UsedFragments::~UsedFragments ()
  {
    while ( not _axiss.empty() ) {
      delete (*_axiss.begin());
      _axiss.erase( _axiss.begin() );
    }
  }


  inline DbU::Unit  UsedFragments::getPitch    () const { return _pitch; }
  inline DbU::Unit  UsedFragments::getMin      () const { return _span.getVMin(); }
  inline DbU::Unit  UsedFragments::getMax      () const { return _span.getVMax(); }
  inline void       UsedFragments::setPitch    ( DbU::Unit pitch ) { _pitch=pitch; }
  inline void       UsedFragments::setSpan     ( DbU::Unit min, DbU::Unit max ) { _span=Interval(min,max); }
  inline void       UsedFragments::setCapacity ( size_t capacity ) { _capacity=capacity; }
  inline void       UsedFragments::incGlobals  ( size_t count ) { _globals+=count; }


  void  UsedFragments::merge ( DbU::Unit axis, const Interval& chunkMerge )
  {
    Interval restrict = chunkMerge.getIntersection(_span);
    if (restrict.isEmpty()) return;

    vector<Axis*>::iterator iaxis = find_if( _axiss.begin(), _axiss.end(), AxisMatch(axis) );

    Axis* paxis = NULL;
    if (iaxis == _axiss.end()) {
      paxis = new Axis(this,axis);
      _axiss.push_back ( paxis );
      stable_sort( _axiss.begin(), _axiss.end(), AxisCompare() );
    } else {
      paxis = *iaxis;
    }

    paxis->merge( restrict );
  }


  Interval  UsedFragments::getMaxFree () const
  {
  //cerr << "capacity:" << _capacity << " _globals:" << _globals << " _axiss:" << _axiss.size() << endl;
    if (_capacity > _globals + _axiss.size() + 1) return _span;

    Interval maxFree;
    vector<Axis*>::const_iterator iaxis = _axiss.begin();
    for ( ; iaxis != _axiss.end() ; ++iaxis ) {
      Interval axisMaxFree = (*iaxis)->getMaxFree();
      if (axisMaxFree.getSize() > maxFree.getSize())
        maxFree = axisMaxFree;
    }

    return maxFree;
  }


} // End of anonymous namespace.


namespace Anabatic {

  using std::cerr;
  using std::endl;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::UpdateSession;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;


// -------------------------------------------------------------------
// Class  :  "Katabatic::GCell::CompareByDensity".


  GCell::CompareByDensity::CompareByDensity ( size_t depth )
    : _depth(depth)
  { }


  bool  GCell::CompareByDensity::operator() ( GCell* lhs, GCell* rhs ) const
  {
    float difference = lhs->getDensity(_depth) - rhs->getDensity(_depth);
    if (difference != 0.0) return (difference > 0.0);
    
    return lhs->getId() < rhs->getId();
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::GCell".

  Name      GCell::_extensionName = "Anabatic::GCell";
  uint32_t  GCell::_displayMode   = GCell::Boundary;
  DbU::Unit GCell::_matrixHSide   = 0;
  DbU::Unit GCell::_matrixVSide   = 0;


  uint32_t  GCell::getDisplayMode () { return _displayMode; }
  void      GCell::setDisplayMode ( uint32_t mode ) { _displayMode = mode; }


  GCell::GCell ( AnabaticEngine* anabatic, DbU::Unit xmin, DbU::Unit ymin )
    : Super(anabatic->getCell())
    , _observable    ()
    , _anabatic      (anabatic)
    , _flags         (Flags::HChannelGCell|Flags::Invalidated)
    , _westEdges     ()
    , _eastEdges     ()
    , _southEdges    ()
    , _northEdges    ()
    , _xmin          (xmin)
    , _ymin          (ymin)
    , _gcontacts     ()
    , _vsegments     ()
    , _hsegments     ()
    , _contacts      ()
    , _depth         (Session::getRoutingGauge()->getDepth())
    , _pinDepth      (0)
    , _satProcessed  (0)
    , _rpCount       (0)
    , _blockages     (new DbU::Unit [_depth])
    , _cDensity      (0.0)
    , _densities     (new float [_depth])
    , _feedthroughs  (new float [_depth])
    , _fragmentations(new float [_depth])
    , _globalsCount  (new float [_depth])
    , _key           (this,1)
    , _lastClonedKey (NULL)
  {
    if (not _matrixHSide) {
      _matrixVSide = Session::getSliceHeight();
      _matrixHSide = Session::getSliceHeight();

      if (_matrixHSide % Session::getSliceStep())
        _matrixHSide += Session::getSliceStep() - _matrixHSide % Session::getSliceStep();
    }

    for ( size_t i=0 ; i<_depth ; i++ ) {
      _blockages     [i] = 0;
      _densities     [i] = 0.0;
      _feedthroughs  [i] = 0.0;
      _fragmentations[i] = 0.0;
      _globalsCount  [i] = 0.0;

      if (Session::getRoutingGauge()->getLayerGauge(i)->getType() == Constant::PinOnly)
        ++_pinDepth;
    }

    updateKey( 1 );
  }


  void  GCell::_postCreate ()
  {
    Super::_postCreate();
    _anabatic->_add( this );
  }


  GCell* GCell::create ( AnabaticEngine* anabatic )
  {
    if (not anabatic)            throw Error( "GCell::create(): NULL anabatic argument." );
    if (not anabatic->getCell()) throw Error( "GCell::create(): AnabaticEngine has no Cell loaded." );

    bool reUseSession = Session::isOpen();
    if (not reUseSession) anabatic->openSession();
    GCell* gcell = new GCell ( anabatic
                             , anabatic->getCell()->getAbutmentBox().getXMin()
                             , anabatic->getCell()->getAbutmentBox().getYMin() );
    gcell->_postCreate();
    if (not reUseSession) Session::close();

    return gcell;
  }


  GCell* GCell::_create ( DbU::Unit xmin, DbU::Unit ymin )
  {
    GCell* gcell = new GCell ( getAnabatic(), xmin, ymin );
    gcell->_postCreate();
    return gcell;
  }


  GCell::~GCell ()
  {
  //cdebug_log(145,0) << "GCell::~GCell()" << endl;

    delete [] _blockages;
    delete [] _densities;
    delete [] _feedthroughs;
    delete [] _fragmentations;
    delete [] _globalsCount;
  }


  void  GCell::_destroyEdges ()
  {
    while (not  _westEdges.empty()) (* _westEdges.rbegin())->destroy();
    while (not  _eastEdges.empty()) (* _eastEdges.rbegin())->destroy();
    while (not _southEdges.empty()) (*_southEdges.rbegin())->destroy();
    while (not _northEdges.empty()) (*_northEdges.rbegin())->destroy();
  }


  void  GCell::_preDestroy ()
  {
    _destroyEdges();
    _anabatic->_remove( this );
    Super::_preDestroy();
  }


  void  GCell::_remove ( Edge* edge, Flags side )
  {
    if (side.contains(Flags::WestSide )) erase_element(  _westEdges, edge );
    if (side.contains(Flags::EastSide )) erase_element(  _eastEdges, edge );
    if (side.contains(Flags::SouthSide)) erase_element( _southEdges, edge );
    if (side.contains(Flags::NorthSide)) erase_element( _northEdges, edge );
  }


  void  GCell::_add ( Edge* edge, Flags side )
  {
    cdebug_log(110,1) << "GCell::_add(side): side:" << side << " " << edge << endl;
    if (side.contains(Flags::WestSide)) {
      cdebug_log(110,0) << "Adding to West side of " << this << endl;
      for ( auto iedge=_westEdges.begin() ; iedge != _westEdges.end() ; ++iedge )
        if ((*iedge)->getAxisMin() > edge->getAxisMin()) {
          _westEdges.insert( iedge, edge );
          cdebug_tabw(110,-1);
          return;
        }
      _westEdges.push_back( edge );
    }

    if (side.contains(Flags::EastSide)) {
      cdebug_log(110,0) << "Adding to East side of " << this << endl;
      for ( auto iedge=_eastEdges.begin() ; iedge != _eastEdges.end() ; ++iedge )
        if ((*iedge)->getAxisMin() > edge->getAxisMin()) {
          _eastEdges.insert( iedge, edge );
          cdebug_tabw(110,-1);
          return;
        }
      _eastEdges.push_back( edge );
    }

    if (side.contains(Flags::SouthSide)) {
      cdebug_log(110,0) << "Adding to South side of " << this << endl;
      for ( auto iedge=_southEdges.begin() ; iedge != _southEdges.end() ; ++iedge )
        cdebug_log(110,0) << "| @" << DbU::getValueString((*iedge)->getAxisMin()) << " " << *iedge << endl;

      for ( auto iedge=_southEdges.begin() ; iedge != _southEdges.end() ; ++iedge )
        if ((*iedge)->getAxisMin() > edge->getAxisMin()) {
          cdebug_log(110,0) << "Insert *before* " << *iedge << endl;

          _southEdges.insert( iedge, edge );
          for ( auto iedge2=_southEdges.begin() ; iedge2 != _southEdges.end() ; ++iedge2 )
            cdebug_log(110,0) << "| @" <<  DbU::getValueString((*iedge2)->getAxisMin()) << " " << *iedge2 << endl;
          cdebug_tabw(110,-1);
          return;
        }
      _southEdges.push_back( edge );
    }

    if (side.contains(Flags::NorthSide)) {
      cdebug_log(110,0) << "Adding to North side of " << this << endl;
      for ( auto iedge=_northEdges.begin() ; iedge != _northEdges.end() ; ++iedge )
        if ((*iedge)->getAxisMin() > edge->getAxisMin()) {
          _northEdges.insert( iedge, edge );
          cdebug_tabw(110,-1);
          return;
        }
      _northEdges.push_back( edge );
    }

    cdebug_tabw(110,-1);
  }


  bool  GCell::isHorizontalPlane ( size_t depth ) const
  { return _anabatic->getConfiguration()->getLayerGauge(depth)->isHorizontal(); }


  bool  GCell::isVerticalPlane ( size_t depth ) const
  { return _anabatic->getConfiguration()->getLayerGauge(depth)->isVertical(); }
  

  Contact* GCell::hasGContact ( const Net* net ) const
  {
    for ( Contact* contact : _gcontacts ) {
      if (contact->getNet() == net) return contact;
    }
    return NULL;
  }


  bool  GCell::hasNet ( const Net* net ) const
  {
    if (hasGContact(net)) return true;

    for ( Edge* edge : _eastEdges ) {
      for ( Segment* segment : edge->getSegments() ) {
        if (segment->getNet() == net) return true;
      }
    }

    for ( Edge* edge : _northEdges ) {
      for ( Segment* segment : edge->getSegments() ) {
        if (segment->getNet() == net) return true;
      }
    }

    return false;
  }


  Contact* GCell::hasGContact ( const Contact* owned ) const
  {
    for ( Contact* contact : _gcontacts ) {
      if (contact == owned) return contact;
    }
    return NULL;
  }


  Contact* GCell::breakGoThrough ( Net* net )
  {
    Contact* gcontact = hasGContact( net );
    if (gcontact) return gcontact;

    for ( Edge* edge : _eastEdges ) {
      for ( Segment* segment : edge->getSegments() ) {
        if (segment->getNet() == net) 
          return getAnabatic()->breakAt( segment, this );
      }
    }

    for ( Edge* edge : _northEdges ) {
      for ( Segment* segment : edge->getSegments() ) {
        if (segment->getNet() == net) 
          return getAnabatic()->breakAt( segment, this );
      }
    }

    return getGContact( net );
  }


  Segment* GCell::hasGoThrough ( Net* net ) const
  {
    for ( Edge* edge : _eastEdges ) {
      for ( Segment* segment : edge->getSegments() ) {
        if (segment->getNet() == net) return segment;
      }
    }
    for ( Edge* edge : _northEdges ) {
      for ( Segment* segment : edge->getSegments() ) {
        if (segment->getNet() == net) return segment;
      }
    }
    return NULL;
  }


  Edge* GCell::getEdgeTo ( GCell* neighbor, Flags sideHint ) const
  {
    for ( Edge* edge : getEdges(sideHint) ) {
      if (edge->getOpposite(this) == neighbor) return edge;
    }
    return NULL;
  }


  Edge* GCell::getEdgeAt ( Flags sideHint, DbU::Unit u ) const
  {
    for ( Edge* edge : getEdges(sideHint) ) {
      GCell* side = edge->getOpposite(this);
      if (   (sideHint.contains(Flags::WestSide) or sideHint.contains(Flags::EastSide ))
         and (u < side->getYMax()) ) {
        cdebug_log(112,0) << "H Opposite @" << DbU::getValueString(u) << " is: " << side << endl;
        return edge;
      }
      if (   (sideHint.contains(Flags::SouthSide) or sideHint.contains(Flags::NorthSide))
         and (u < side->getXMax()) ) {
        cdebug_log(112,0) << "V Opposite @" << DbU::getValueString(u) << " is: " << side << endl;
        return edge;
      }
    }
    return NULL;
  }


  GCell* GCell::getEastNMatrix() const
  {
    if (!this->getEast()->isMatrix()) return this->getEast();
    else {
      GCell* gcell = this->getEast();
      while(gcell->getEast()){
        if   (!gcell->getEast()->isMatrix()) break;
        else                                 gcell = gcell->getEast();
      }
      return gcell->getEast();
    }
  }
  

  GCell* GCell::getNorthNMatrix() const
  {
    if (!this->getNorth()->isMatrix()) return this->getNorth();
    else {
      GCell* gcell = this->getNorth();
      while(gcell->getNorth()){
        if   (!gcell->getNorth()->isMatrix()) break;
        else                                 gcell = gcell->getNorth();
      }
      return gcell->getNorth();
    }
  }


  GCell* GCell::getWest  ( DbU::Unit y ) const
  {
    for ( Edge* edge : _westEdges ) {
      GCell* side = edge->getOpposite(this);
      if (y < side->getYMax()) return side;
    }
    return NULL;
  }


  GCell* GCell::getEast  ( DbU::Unit y ) const
  {
    for ( Edge* edge : _eastEdges ) {
      GCell* side = edge->getOpposite(this);
      if (y < side->getYMax()) return side;
    }
    return NULL;
  }


  GCell* GCell::getSouth ( DbU::Unit x ) const
  {
    for ( Edge* edge : _southEdges ) {
      GCell* side = edge->getOpposite(this);
      if (x < side->getXMax()) return side;
    }
    return NULL;
  }


  GCell* GCell::getNorth ( DbU::Unit x ) const
  {
    for ( Edge* edge : _northEdges ) {
      GCell* side = edge->getOpposite(this);
      if (x < side->getXMax()) return side;
    }
    return NULL;
  }


  GCell* GCell::getNeighborAt ( Flags side, DbU::Unit axis ) const
  {
    if (side & Flags::EastSide ) return getEast (axis);
    if (side & Flags::WestSide ) return getWest (axis);
    if (side & Flags::NorthSide) return getNorth(axis);
    if (side & Flags::SouthSide) return getSouth(axis);
    return NULL;
  }


  GCell* GCell::getUnder ( DbU::Unit x, DbU::Unit y ) const
  {
    const GCell* current = this;

    while ( current ) {
      if (not current->isFlat() and current->getBoundingBox().contains(x,y)) break;

      if (x >= current->getXMax()) { current = current->getEast (); continue; }
      if (y >= current->getYMax()) { current = current->getNorth(); continue; }

      cerr << Error( "GCell::getUnder(): No GCell under (%s,%s), this must *never* happen."
                   , DbU::getValueString(x).c_str()
                   , DbU::getValueString(y).c_str()
                   ) << endl;
      current = NULL; break;
    }
    
    return const_cast<GCell*>( current );
  }


  Box  GCell::getBorder ( const GCell* s, const GCell* t )
  {
    Flags flags = Flags::NoFlags;
    flags |= (s->getXMax() == t->getXMin()) ? Flags::EastSide  : Flags::NoFlags;
    flags |= (t->getXMax() == s->getXMin()) ? Flags::WestSide  : Flags::NoFlags;
    flags |= (s->getYMax() == t->getYMin()) ? Flags::NorthSide : Flags::NoFlags;
    flags |= (t->getYMax() == s->getYMin()) ? Flags::SouthSide : Flags::NoFlags;

    if (flags & Flags::Vertical) {
      if (flags & Flags::Horizontal) return Box();
      if (flags & Flags::WestSide)
        return Box( s->getXMin(), std::max( s->getYMin(), t->getYMin() )
                  , s->getXMin(), std::min( s->getYMax(), t->getYMax() ) );
      else
        return Box( t->getXMin(), std::max( s->getYMin(), t->getYMin() )
                  , t->getXMin(), std::min( s->getYMax(), t->getYMax() ) );
    }

    if (flags & Flags::Horizontal) {
      if (flags & Flags::Vertical) return Box();
      if (flags & Flags::NorthSide)
        return Box( std::max( s->getXMin(), t->getXMin() ), t->getYMin()
                  , std::min( s->getXMax(), t->getXMax() ), t->getYMin() );
      else
        return Box( std::max( s->getXMin(), t->getXMin() ), s->getYMin()
                  , std::min( s->getXMax(), t->getXMax() ), s->getYMin() );
    }

    return Box();
  }


  GCell*  GCell::vcut ( DbU::Unit x )
  {
    cdebug_log(110,1) << "GCell::vcut() @x:" << DbU::getValueString(x) << " " << this << endl;

    if ( (x < getXMin()) or (x > getXMax()) )
      throw Error( "GCell::vcut(): Vertical cut axis at %s is outside GCell box,\n"
                   "        in %s."
                 , DbU::getValueString(x).c_str()
                 , getString(this).c_str()
                 );

    GCell* chunk = _create( x, getYMin() );
    cdebug_log(110,0) << "New chunk:" << chunk << endl;

    _moveEdges( chunk, 0, Flags::EastSide );
    Edge::create( this, chunk, Flags::Horizontal );

    if (not _southEdges.empty()) {
      cdebug_log(110,0) << "Split/create south edges." << endl;

      size_t iedge = 0;
      for ( ; (iedge < _southEdges.size()) ; ++iedge ) {
        cdebug_log(110,0) << "[" << iedge << "] xmax of:"
                          << _southEdges[iedge]->getOpposite(this)
                          << " " << _southEdges[iedge] << endl;
        if (x <= _southEdges[iedge]->getOpposite(this)->getXMax()) break;
      }

      if (      (x <  _southEdges[iedge]->getOpposite(this)->getXMax())
         or (   (x == _southEdges[iedge]->getOpposite(this)->getXMax())
            and (chunk->getXMax() == getXMax())) ) {
        Edge::create( _southEdges[iedge]->getOpposite(this), chunk, Flags::Vertical );
        _southEdges[iedge]->invalidate( false );
      }

      _moveEdges( chunk, iedge+1, Flags::SouthSide );
    }

    if (not _northEdges.empty()) {
      cdebug_log(110,0) << "Split/create north edges." << endl;

      size_t iedge = 0;
      for ( ; (iedge < _northEdges.size()) ; ++iedge )
        if (x <= _northEdges[iedge]->getOpposite(this)->getXMax()) break;

      if (      (x <  _northEdges[iedge]->getOpposite(this)->getXMax())
         or (   (x == _northEdges[iedge]->getOpposite(this)->getXMax())
            and (chunk->getXMax() == getXMax())) ) {
        Edge::create( chunk, _northEdges[iedge]->getOpposite(this), Flags::Vertical );
        _northEdges[iedge]->invalidate( false );
      }

      _moveEdges( chunk, iedge+1, Flags::NorthSide );
    }

    cdebug_tabw(110,-1);

    return chunk;
  }


  GCell* GCell::hcut ( DbU::Unit y )
  {
    cdebug_log(110,1) << "GCell::hcut() @y:" << DbU::getValueString(y) << " " << this << endl;

    if ( (y < getYMin()) or (y > getYMax()) )
      throw Error( "GCell::hcut(): Horizontal cut axis at %s is outside GCell box,\n"
                   "        in %s."
                 , DbU::getValueString(y).c_str()
                 , getString(this).c_str()
                 );

    GCell* chunk = _create( getXMin(), y );
    cdebug_log(110,0) << "New chunk:" << chunk << endl;

    _moveEdges( chunk, 0, Flags::NorthSide );
    Edge::create( this, chunk, Flags::Vertical );

    if (not _westEdges.empty()) {
      size_t iedge = 0;
      for ( ; (iedge < _westEdges.size()) ; ++iedge )
        if (y <= _westEdges[iedge]->getOpposite(this)->getYMax()) break;

      if (      (y <  _westEdges[iedge]->getOpposite(this)->getYMax())
         or (   (y == _westEdges[iedge]->getOpposite(this)->getYMax())
            and (chunk->getYMax() == getYMax())) ) {
        Edge::create( _westEdges[iedge]->getOpposite(this), chunk, Flags::Horizontal );
        _westEdges[iedge]->invalidate( false );
      }

      _moveEdges( chunk, iedge+1, Flags::WestSide );
    }

    if (not _eastEdges.empty()) {
      size_t iedge = 0;
      for ( ; (iedge < _eastEdges.size()) ; ++iedge )
        if (y <= _eastEdges[iedge]->getOpposite(this)->getYMax()) break;

      if (      (y <  _eastEdges[iedge]->getOpposite(this)->getYMax())
         or (   (y == _eastEdges[iedge]->getOpposite(this)->getYMax())
            and (chunk->getYMax() == getYMax())) ) {
        Edge::create( chunk, _eastEdges[iedge]->getOpposite(this), Flags::Horizontal );
        _eastEdges[iedge]->invalidate( false );
      }

      _moveEdges( chunk, iedge+1, Flags::EastSide );
    }

    cdebug_tabw(110,-1);

    return chunk;
  }


  bool  GCell::doGrid ()
  {
    bool openSession = Session::isOpen();
    if (not openSession) getAnabatic()->openSession();

    DbU::Unit vside = getMatrixVSide();
    DbU::Unit hside = getMatrixHSide();
    Interval  hspan = getSide( Flags::Horizontal );
    Interval  vspan = getSide( Flags::Vertical );

    // if (hspan.getSize() < 2*hside) {
    //   cerr << Error( "GCell::doGrid(): GCell is too narrow (dx:%s) to build a grid.\n"
    //                  "        (%s)"
    //                , DbU::getValueString(hspan.getSize()).c_str()
    //                , getString(this).c_str()
    //                ) << endl;
    //   Session::close();
    //   return false;
    // }

    // if (vspan.getSize() < 2*vside) {
    //   cerr << Error( "GCell::doGrid(): GCell is too narrow (dy:%s) to build a grid.\n"
    //                  "        (%s)"
    //                , DbU::getValueString(vspan.getSize()).c_str()
    //                , getString(this).c_str()
    //                ) << endl;
    //   return false;
    // }

    GCell*    row    = this;
    GCell*    column = NULL;
    DbU::Unit ycut   = vspan.getVMin()+vside;
    for ( ; ycut < vspan.getVMax() ; ycut += vside ) {
      column = row;
      row    = row->hcut( ycut );
      row->setType( Flags::MatrixGCell );

      for ( DbU::Unit xcut = hspan.getVMin()+hside ; xcut < hspan.getVMax() ; xcut += hside ) {
        column = column->vcut( xcut );
        column->setType( Flags::MatrixGCell );
      }
    }

    column = row;
    for ( DbU::Unit xcut = hspan.getVMin()+hside ; xcut < hspan.getVMax() ; xcut += hside ) {
      column = column->vcut( xcut );
      column->setType( Flags::MatrixGCell );
    }

    setType( Flags::MatrixGCell );

  //size_t hLocal = - getAnabatic()->getConfiguration()->getHEdgeLocal();
  //size_t vLocal = - getAnabatic()->getConfiguration()->getVEdgeLocal();
  //for ( ; ibegin < gcells.size() ; ++ibegin ) {
  //  gcells[ibegin]->setType( Flags::MatrixGCell );

  //  for ( Edge* edge : gcells[ibegin]->getEdges(Flags::NorthSide|Flags::EastSide) ) {
  //    if (edge->isHorizontal()) edge->incCapacity( hLocal );
  //    else                      edge->incCapacity( vLocal );
  //  }
  //}

    if (not openSession) Session::close();
    return true;
  }


  void  GCell::invalidate ( bool propagateFlag )
  {
    cdebug_log(110,1) << "GCell::invalidate() " << this << endl;
    Super::invalidate( propagateFlag );
    _flags |= Flags::Invalidated;

    cdebug_log(110,1) << "West side."  << endl; for ( Edge* edge : _westEdges  ) edge->invalidate(); cdebug_tabw(110,-1);
    cdebug_log(110,1) << "East side."  << endl; for ( Edge* edge : _eastEdges  ) edge->invalidate(); cdebug_tabw(110,-1);
    cdebug_log(110,1) << "South side." << endl; for ( Edge* edge : _southEdges ) edge->invalidate(); cdebug_tabw(110,-1);
    cdebug_log(110,1) << "North side." << endl; for ( Edge* edge : _northEdges ) edge->invalidate(); cdebug_tabw(110,-1);

    cdebug_tabw(110,-1);
  }


  void  GCell::materialize ()
  {
    cdebug_log(110,1) << "GCell::materialize() " << this << endl;

    if (_xmin > getXMax()+1)
      cerr << Error( "GCell::materialize(): %s, X Min is greater than Max.", getString(this).c_str() );
    if (_ymin > getYMax()+1)
      cerr << Error( "GCell::materialize(): %s, Y Min is greater than Max.", getString(this).c_str() );

    _anabatic->_updateLookup( this );
  //_anabatic->getMatrix()->show();
    Super::materialize();

    cdebug_tabw(110,-1);
  }


  void  GCell::_moveEdges ( GCell* dest, size_t ibegin, Flags flags )
  {
    cdebug_log(110,1) << "GCell::_moveEdges() " << this << endl;
    cdebug_log(110,0)   << "           toward " << dest << endl;
    cdebug_log(110,0)   << "           ibegin: " << ibegin << " flags:" << flags << endl;

    size_t iclear = ibegin;

    if (flags.contains(Flags::SouthSide) and not _southEdges.empty()) {
      cdebug_log(110,0) << "South side." << endl;

      if (iclear < _southEdges.size()) {
        for ( size_t iedge=ibegin ; (iedge < _southEdges.size()) ; ++iedge ) {
          _southEdges[iedge]->_setTarget( dest );
          dest->_southEdges.push_back( _southEdges[iedge] );
        }
        _southEdges.resize( iclear );
      } else {
        if (iclear > _southEdges.size())
          cerr << Error("GCell::_moveEdges(): On south side, iclear=%u is greater than size()-1=%u\n"
                        "        (%s)"
                       , iclear
                       , _southEdges.size()
                       , getString(this).c_str()
                       ) << endl;
      }
    }

    if (flags.contains(Flags::NorthSide) and not _northEdges.empty()) {
      cdebug_log(110,0) << "North side." << endl;

      if (iclear < _northEdges.size()) {
        for ( size_t iedge=ibegin ; (iedge < _northEdges.size()) ; ++iedge ) {
          _northEdges[iedge]->_setSource( dest );
          dest->_northEdges.push_back( _northEdges[iedge] );
        }
        _northEdges.resize( iclear );
      } else {
        if (iclear > _northEdges.size())
          cerr << Error("GCell::_moveEdges(): On north side, iclear=%u is greater than size()-1=%u\n"
                        "        (%s)"
                       , iclear
                       , _northEdges.size()
                       , getString(this).c_str()
                       ) << endl;
      }
    }

    if (flags.contains(Flags::WestSide) and not _westEdges.empty()) {
      cdebug_log(110,0) << "West side." << endl;

      if (iclear < _westEdges.size()) {
        for ( size_t iedge=ibegin ; (iedge < _westEdges.size()) ; ++iedge ) {
          _westEdges[iedge]->_setTarget( dest );
          dest->_westEdges.push_back( _westEdges[iedge] );
        }
        _westEdges.resize( iclear );
      } else {
        if (iclear > _westEdges.size())
          cerr << Error("GCell::_moveEdges(): On west side, iclear=%u is greater than size()-1=%u\n"
                        "        (%s)"
                       , iclear
                       , _westEdges.size()
                       , getString(this).c_str()
                       ) << endl;
      }
    }

    if (flags.contains(Flags::EastSide) and not _eastEdges.empty()) {
      cdebug_log(110,0) << "East side." << endl;

      if (iclear < _eastEdges.size()) {
        for ( size_t iedge=ibegin ; (iedge < _eastEdges.size()) ; ++iedge ) {
          _eastEdges[iedge]->_setSource( dest );
          dest->_eastEdges.push_back( _eastEdges[iedge] );
        }
        _eastEdges.resize( iclear );
      } else {
        if (iclear > _eastEdges.size())
           cerr << Error("GCell::_moveEdges(): On east side, iclear=%u is greater than size()-1=%u\n"
                        "        (%s)"
                        , iclear
                        , _eastEdges.size()
                        , getString(this).c_str()
                        ) << endl;
      }
    }

    cdebug_tabw(110,-1);
  }


  void GCell::setSouthWestCorner ( DbU::Unit x, DbU::Unit y )
  {
  //DbU::Unit dx = x - _xmin;
  //DbU::Unit dy = y - _ymin;

  /*for ( Contact* contact : _gcontacts ) {
    Point position = contact->getPosition().translate( dx, dy );

      for ( Component* component : contact->getSlaveComponents() ) {
        Horizontal* horizontal = dynamic_cast<Horizontal*>( component );
        if (horizontal) {
        horizontal->setY( position.getY() );
        } else {
          Vertical* vertical = dynamic_cast<Vertical*>( component );
        vertical->setX( position.getX() );
        }
      }
      
      if (not contact->getAnchor()) contact->setPosition( Point(x,y) );
      }*/

    _xmin = x;
    _ymin = y;

    invalidate( false );
  }


  void GCell::updateGContacts ( Flags flags )
  {
    Point center ( _xmin+getWidth()/2, _ymin+getHeight()/2 );

    for ( Contact* contact : _gcontacts ) {
      for ( Component* component : contact->getSlaveComponents() ) {
        Horizontal* horizontal = dynamic_cast<Horizontal*>( component );
        if (horizontal and (flags & Flags::Vertical)) {
          horizontal->setY( center.getY() );
        } else {
          Vertical* vertical = dynamic_cast<Vertical*>( component );
          if (vertical and (flags & Flags::Horizontal)) {
            vertical->setX( center.getX() );
          }
        }
      }
      if (not contact->getAnchor()) contact->setPosition( center );
    }
  }


  Contact* GCell::getGContact ( Net* net )
  {
    for ( Contact* contact : _gcontacts ) {
      if (contact->getNet() == net) {
        cdebug_log(111,0) << "GCell::getGContact(): " << contact << endl;
        return contact;
      }
    }

    Point    center  = getBoundingBox().getCenter();
    Contact* contact = Contact::create( net
                                      , _anabatic->getConfiguration()->getGContactLayer()
                                      , center.getX()
                                      , center.getY()
                                      , DbU::fromLambda(2.0)
                                      , DbU::fromLambda(2.0)
                                      );
    _gcontacts.push_back( contact );
    cdebug_log(111,0) << "GCell::getGContact(): " << contact << endl;
    return contact;
  }


  bool  GCell::unrefContact ( Contact* unref )
  {
    if (_gcontacts.empty()) return false;

    cdebug_log(112,0) << "GCell::unrefContact(): " << unref << endl;

    for ( size_t i=0 ; i< _gcontacts.size() ; ++i ) {
      if (_gcontacts[i] == unref) {
        if (_gcontacts[i]->getSlaveComponents().getLocator()->isValid()) return false;
        
        cdebug_log(112,0) << "  Effective destroy " << (void*)unref << endl;
        std::swap( _gcontacts[i], _gcontacts[_gcontacts.size()-1] );
        _gcontacts[ _gcontacts.size()-1 ]->destroy();
        _gcontacts.pop_back();

        return true;
      }
    }

    return false;
  }


  void  GCell::cleanupGlobal ()
  {
    for ( size_t i=0 ; i<_gcontacts.size() ; ) {
      if (not _gcontacts[i]->getSlaveComponents().getLocator()->isValid()) {
        std::swap( _gcontacts[i], _gcontacts[_gcontacts.size()-1] );
        _gcontacts[ _gcontacts.size()-1 ]->destroy();
        _gcontacts.pop_back();
      } else
        ++i;
    }
  }


  const Name& GCell::getName () const
  { return _extensionName; }


  Box  GCell::getBoundingBox () const
  {
    return Box( getXMin(), getYMin(), getXMax(1), getYMax(1) );
  }


  void  GCell::translate ( const DbU::Unit&, const DbU::Unit& )
  {
    cerr << Error( "GCell::translate(): On %s,\n"
                   "        Must never be called on a GCell object (ignored)."
                 , getString(this).c_str()
                 ) << endl;
  }


  bool  GCell::isNorth ( GCell* c ) const
  {
    bool found = false;
    for (vector<Edge*>::const_iterator it = _northEdges.begin(); it != _northEdges.end(); it++){
      if ( (*it)->getOpposite(this)->getId() == c->getId() ) {
        found = true;
        break;
      }
    }
    return found;
  }


  bool  GCell::isSouth ( GCell* c ) const
  {
    bool found = false;
    for (vector<Edge*>::const_iterator it = _southEdges.begin(); it != _southEdges.end(); it++){
      if ( (*it)->getOpposite(this)->getId() == c->getId() ) {
        found = true;
        break;
      }
    }
    return found;
  }


  bool  GCell::isEast ( GCell* c ) const
  {
    bool found = false;
    for (vector<Edge*>::const_iterator it = _eastEdges.begin(); it != _eastEdges.end(); it++){
      if ( (*it)->getOpposite(this)->getId() == c->getId() ) {
        found = true;
        break;
      }
    }
    return found;
  }

  
  bool  GCell::isWest ( GCell* c ) const
  {
    bool found = false;
    for (vector<Edge*>::const_iterator it = _westEdges.begin(); it != _westEdges.end(); it++){
      if ( (*it)->getOpposite(this)->getId() == c->getId() ) {
        found = true;
        break;
      }
    }
    return found;
  }


  bool  GCell::isSaturated ( size_t depth ) const
  { return getDensity(depth) > Session::getSaturateRatio(); }


  // Interval  GCell::getSide ( Flags direction ) const
  // {
  //   if (direction & Flags::Vertical) return Interval( getYMin(), getYMax() );
  //   return Interval( getXMin(), getXMax() );
  // }


  AutoSegments  GCell::getHStartSegments ()
  { return new AutoSegments_AnchorOnGCell (this,Flags::EastSide); }


  AutoSegments  GCell::getVStartSegments ()
  { return new AutoSegments_AnchorOnGCell (this,Flags::NorthSide); }


  AutoSegments  GCell::getHStopSegments ()
  { return new AutoSegments_AnchorOnGCell (this,Flags::WestSide); }


  AutoSegments  GCell::getVStopSegments ()
  { return new AutoSegments_AnchorOnGCell (this,Flags::SouthSide); }


  size_t  GCell::getRoutingPads ( set<RoutingPad*>& rps )
  {
    for ( size_t i=0 ; i<_contacts.size() ; ++i ) {
      RoutingPad* rp = dynamic_cast<RoutingPad*>(_contacts[i]->getAnchor());
      if ( rp ) {
        rps.insert ( rp );
      }
    }
    return rps.size();
  }


  int  GCell::getHCapacity () const
  {
    int capacity = 0;
    if (not _eastEdges.empty()) {
      for ( Edge* edge : _eastEdges ) capacity += edge->getCapacity(); 
    } else {
      for ( Edge* edge : _westEdges ) capacity += edge->getCapacity(); 
    }
    return capacity;
  }


  int  GCell::getVCapacity () const
  {
    int capacity = 0;
    if (not _northEdges.empty()) {
      for ( Edge* edge : _northEdges ) capacity += edge->getCapacity(); 
    } else {
      for ( Edge* edge : _southEdges ) capacity += edge->getCapacity(); 
    }
    return capacity;
  }


  int  GCell::getCapacity ( size_t depth ) const
  {
    const vector<Edge*>* edges = NULL;
    if (isHorizontalPlane(depth)) edges = (_eastEdges .empty()) ? &_westEdges  : &_eastEdges;
    else                          edges = (_northEdges.empty()) ? &_southEdges : &_northEdges;

    int capacity = 0;
    for ( Edge* edge : *edges ) capacity += edge->getCapacity(depth); 

    return capacity;
  }


  float  GCell::getAverageHVDensity () const
  {
  // Average density of all layers mixeds together.
    float density = 0.0;
    for ( size_t i=0 ; i<_depth ; i++ )
      density += _densities[i];
    return density / ((float)(_depth-_pinDepth));
  }


  float  GCell::getMaxHVDensity () const
  {
  // Maximum density between all horizontal vs. all vertical layers.
    size_t hplanes  = 0;
    size_t vplanes  = 0;
    float  hdensity = 0.0;
    float  vdensity = 0.0;

    for ( size_t i=_pinDepth ; i<_depth ; i++ ) {
      if (isHorizontalPlane(i)) { hdensity += _densities[i]; ++hplanes; }
      else                      { vdensity += _densities[i]; ++vplanes; }
    }

    if (hplanes) hdensity /= hplanes;
    if (vplanes) vdensity /= vplanes;

    return std::max(hdensity, vdensity);
  }


  float  GCell::getDensity ( Flags flags ) const
  {
    if (isInvalidated() and not(flags & Flags::NoUpdate))  const_cast<GCell*>(this)->updateDensity();

    float density = 0.0;

    if (getAnabatic()->getDensityMode() == AverageHVDensity) {
      density = getAverageHVDensity();
    } else if (getAnabatic()->getDensityMode() == MaxHVDensity) {
      density = getMaxHVDensity();
    } else if (getAnabatic()->getDensityMode() == AverageHDensity) {
      size_t hplanes  = 0;
      float  hdensity = 0.0;

      for ( size_t i=_pinDepth ; i<_depth ; i++ ) {
        if (isHorizontalPlane(i)) { hdensity += _densities[i]; ++hplanes; }
      }
      if (hplanes) hdensity /= hplanes;

      density = hdensity;
    } else if (getAnabatic()->getDensityMode() == AverageVDensity) {
      size_t vplanes  = 0;
      float  vdensity = 0.0;

      for ( size_t i=_pinDepth ; i<_depth ; i++ ) {
        if (isVerticalPlane(i)) { vdensity += _densities[i]; ++vplanes; }
      }

      if (vplanes) vdensity /= vplanes;

      density = vdensity;
    } else if (getAnabatic()->getDensityMode() == MaxDensity) {
      for ( size_t i=_pinDepth ; i<_depth ; i++ ) {
        if (_densities[i] > density) density = _densities[i];
      }
    } else if (getAnabatic()->getDensityMode() == MaxHDensity) {
      for ( size_t i=_pinDepth ; i<_depth ; i++ ) {
        if (isHorizontalPlane(i) and (_densities[i] > density)) density = _densities[i];
      }
    } else if (getAnabatic()->getDensityMode() == MaxVDensity) {
      for ( size_t i=_pinDepth ; i<_depth ; i++ ) {
        if (isVerticalPlane(i) and (_densities[i] > density)) density = _densities[i];
      }
    }

    return density;
  }


  void  GCell::postGlobalAnnotate ()
  {
    if (isInvalidated()) updateDensity();

    for ( size_t depth=0 ; depth<_depth ; ++depth ) {
      RoutingLayerGauge* rlg = Session::getLayerGauge( depth );
      if (rlg->getType() & Constant::PinOnly) continue;
      if (_densities[depth] >= 0.9) {
        if (depth+2 < _depth) {
          Edge* edge = (rlg->getDirection() == Constant::Vertical) ? getNorthEdge()
                                                                   : getEastEdge();
          if (edge) {
            edge->reserveCapacity( 2 );
          }
        }
      }
    }
  }


  void  GCell::addBlockage ( size_t depth, DbU::Unit length )
  {
    if (depth >= _depth) return;

    _blockages[depth] += length;
    _flags |= Flags::Invalidated;

    cdebug_log(149,0) << "GCell::addBlockage() " << this << " "
                << depth << ":" << DbU::getValueString(_blockages[depth]) << endl;
  }


  void  GCell::removeContact ( AutoContact* ac )
  {
    size_t begin = 0;
    size_t end   = _contacts.size();
    bool   found = false;

    for ( ; not found and (begin < end) ; begin++ ) {
      if ( _contacts[begin] == ac ) {
        _contacts[begin] = _contacts[end-1];
        found = true;
      }
    }

    if (found) {
      cdebug_log(149,0) << "remove " << ac << " from " << this << endl;
      _contacts.pop_back();
      _flags |= Flags::Invalidated;
    } else {
      cerr << Bug("%p:%s do not belong to %s."
                 ,ac->base(),getString(ac).c_str(),_getString().c_str()) << endl;
    }
  }


  void  GCell::removeHSegment ( AutoSegment* segment )
  {
    size_t end   = _hsegments.size();
    size_t begin = 0;

    for ( ; begin < end ; begin++ ) {
      if (not _hsegments[begin])
        cerr << Bug( "GCell::removeHSegment(): In %s, NULL segment at [%u/%u]."
                   , _getString().c_str(), begin, _hsegments.size() ) << endl;
      
      if (_hsegments[begin] == segment) std::swap( _hsegments[begin], _hsegments[--end] );
      cdebug_log(9000,0) << "GCell::removeHSegment() " << this << endl;
      cdebug_log(9000,0) << "  " << segment << endl;
    }

    if (_hsegments.size() == end) {
      cerr << Bug( "GCell::removeHSegment(): %s do not go through %s."
                 , getString(segment).c_str(), _getString().c_str() ) << endl;
      return;
    }

    if (_hsegments.size() - end > 1)
      cerr << Bug( "%s has multiple occurrences of %s."
                 , _getString().c_str(), getString(segment).c_str() ) << endl;

    _hsegments.erase( _hsegments.begin() + end, _hsegments.end() );
    _flags |= Flags::Invalidated;
  }


  void  GCell::removeVSegment ( AutoSegment* segment )
  {
    size_t end   = _vsegments.size();
    size_t begin = 0;

    for ( ; begin < end ; begin++ ) {
      if (_vsegments[begin] == segment) std::swap( _vsegments[begin], _vsegments[--end] );
    }

    if (_vsegments.size() == end) {
      cerr << Bug( "%s do not go through %s."
                 , getString(segment).c_str()
                 , _getString().c_str() ) << endl;
      return;
    }

    if (_vsegments.size() - end > 1)
      cerr << Bug( "%s has multiple occurrences of %s."
                 , _getString().c_str()
                 , getString(segment).c_str() ) << endl;

    _vsegments.erase( _vsegments.begin() + end, _vsegments.end() );
    _flags |= Flags::Invalidated;
  }


  void  GCell::updateContacts ()
  { for ( AutoContact* contact : _contacts ) contact->updateGeometry(); }


  size_t  GCell::updateDensity ()
  {
    if (not isInvalidated()) return (isSaturated()) ? 1 : 0;

    _flags.reset( Flags::Saturated );

    sort( _hsegments.begin(), _hsegments.end(), AutoSegment::CompareByDepthLength() );
    sort( _vsegments.begin(), _vsegments.end(), AutoSegment::CompareByDepthLength() );

    float                 ccapacity    = getHCapacity() * getVCapacity() * (_depth-_pinDepth); 
    DbU::Unit             width        = getXMax() - getXMin();
    DbU::Unit             height       = getYMax() - getYMin();
    DbU::Unit             hpenalty     = 0 /*_box.getWidth () / 3*/;
    DbU::Unit             vpenalty     = 0 /*_box.getHeight() / 3*/;
    DbU::Unit             uLengths1    [ _depth ];
    DbU::Unit             uLengths2    [ _depth ];
    float                 localCounts  [ _depth ];
    vector<UsedFragments> ufragments   ( _depth );

    for ( size_t i=0 ; i<_depth ; i++ ) {
      ufragments[i].setPitch ( Session::getPitch(i) );
      _feedthroughs[i] = 0.0;
      uLengths1    [i] = 0;
      uLengths2    [i] = 0;
      localCounts  [i] = 0.0;
      _globalsCount[i] = 0.0;

      ufragments[i].setCapacity( (size_t)getCapacity(i) );
      if (isHorizontalPlane(i)) ufragments[i].setSpan( getXMin(), getXMax() );
      else                      ufragments[i].setSpan( getYMin(), getYMax() );
    }

  // Compute wirelength associated to contacts (in DbU::Unit converted to float).
    AutoSegment::DepthLengthSet  processeds;
    for ( AutoContact* contact : _contacts ) {
      for ( size_t i=0 ; i<_depth ; i++ ) uLengths1[i] = 0;
      contact->getLengths( uLengths1, processeds );
      for ( size_t i=0 ; i<_depth ; i++ ) {
        if (isHorizontalPlane(i)) uLengths2[i] += uLengths1[i]+hpenalty;
        else                      uLengths2[i] += uLengths1[i]+vpenalty;
      }
    }

  // Add the "pass through" horizontal segments.
    if (not _hsegments.empty()) {
      const Layer* layer = _hsegments[0]->getLayer();
      size_t       depth = Session::getRoutingGauge()->getLayerDepth(layer);
      size_t       count = 0;
      for ( size_t i=0 ; i<_hsegments.size() ; i++ ) {
        _globalsCount[depth] += 1.0;
        ufragments[depth].incGlobals();

        if ( layer != _hsegments[i]->getLayer() ) {
          uLengths2[depth] += count * width;

          count = 0;
          layer = _hsegments[i]->getLayer();
          depth = Session::getRoutingGauge()->getLayerDepth(layer);
        }
        count++;
        _feedthroughs[depth] += 1.0;
      }
      if ( count ) {
        uLengths2[depth] += count * width;
      }
    }

  // Add the "pass through" vertical segments.
    if (not _vsegments.empty()) {
      const Layer* layer = _vsegments[0]->getLayer();
      size_t       depth = Session::getRoutingGauge()->getLayerDepth(layer);
      size_t       count = 0;
      for ( size_t i=0 ; i<_vsegments.size() ; i++ ) {
        _globalsCount[depth] += 1.0;
        ufragments[depth].incGlobals();

        if (layer != _vsegments[i]->getLayer()) {
          uLengths2[depth] += count * height;

          count = 0;
          layer = _vsegments[i]->getLayer();
          depth = Session::getRoutingGauge()->getLayerDepth(layer);
        }
        count++;
        _feedthroughs[depth] += 1.0;
      }
      if (count) {
        uLengths2[depth] += count * height;
      }
    }

  // Add the blockages.
    int contiguousNonSaturated = 0;
    for ( size_t i=0 ; i<_depth ; i++ ) {
      uLengths2[i] += _blockages[i];
      if (not i) continue;
      if (Session::getLayerGauge(i)->getType() & Constant::PowerSupply)
        continue;
      if ((float)(_blockages[i] * Session::getPitch(i)) > 0.60*(float)(width*height))
        contiguousNonSaturated = 0;
      else
        contiguousNonSaturated++;
    }
    if (contiguousNonSaturated < 2) {
      flags() |= Flags::GoStraight;
    //cerr << "| Set GoStraight on " << this << endl;
    }

  // Compute the number of non pass-through tracks.
    if (not processeds.empty()) {
      AutoSegment::DepthLengthSet::iterator isegment = processeds.begin();
      const Layer* layer = (*isegment)->getLayer();
      DbU::Unit    axis  = (*isegment)->getAxis();
      size_t       depth = Session::getRoutingGauge()->getLayerDepth(layer);
      size_t       count = 0;
      for ( ; isegment != processeds.end(); ++isegment ) {
      //_feedthroughs[depth] += ((*isegment)->isGlobal()) ? 0.50 : 0.33;
        _feedthroughs[depth] += 0.50;
        localCounts  [depth] += 1.0;
        if ( (*isegment)->isGlobal() ) _globalsCount[depth] += 1.0;

        ufragments[depth].merge( (*isegment)->getAxis(), (*isegment)->getSpanU() );
        if ( (axis != (*isegment)->getAxis()) or (layer != (*isegment)->getLayer()) ) {
          count = 0;
          axis  = (*isegment)->getAxis();
          layer = (*isegment)->getLayer();
          depth = Session::getRoutingGauge()->getLayerDepth(layer);
        }
        ++count;
      }
    }

  // Normalize: 0 < d < 1.0 (divide by H/V capacity).
    for ( size_t i=0 ; i<_depth ; i++ ) {
      int capacity = getCapacity(i);
      
      if (Session::getDirection(i) & Flags::Horizontal) {
        if (width and capacity) {
          _densities     [i]  = ((float)uLengths2[i]) / (float)( capacity * width );
          _feedthroughs  [i] += (float)(_blockages[i] / width);
          _fragmentations[i]  = (float)ufragments[i].getMaxFree().getSize() / (float)width;
        } else {
          _densities     [i]  = 0;
          _feedthroughs  [i]  = 0;
          _fragmentations[i]  = 0;
        }
      } else {
        if (height and capacity) {
          _densities     [i]  = ((float)uLengths2[i]) / (float)( capacity * height );
          _feedthroughs  [i] += (float)(_blockages[i] / height);
          _fragmentations[i]  = (float)ufragments[i].getMaxFree().getSize() / (float)height;
        } else {
          _densities     [i]  = 0;
          _feedthroughs  [i]  = 0;
          _fragmentations[i]  = 0;
        }
      }

      if (_densities[i] >= 1.0) _flags |= Flags::Saturated;
    }

    if (ccapacity) _cDensity = ( (float)_contacts.size() ) / ccapacity;
    else           _cDensity = 0;
    _flags.reset( Flags::Invalidated );

    checkDensity();

    return isSaturated() ? 1 : 0 ;
  }


  void  GCell::truncDensities ()
  {
    Box  bBox = getBoundingBox();

    for ( size_t i=0 ; i<_depth ; i++ ) {
      int capacity = getCapacity(i);
      if (isHorizontalPlane(i)) {
        if (_blockages[i] > capacity * bBox.getWidth())
          _blockages[i]  = capacity * bBox.getWidth();
      } else {
        if (_blockages[i] > capacity * bBox.getHeight())
          _blockages[i]  = capacity * bBox.getHeight();
      }
    }
    _flags &= ~Flags::Saturated;
  }


  size_t  GCell::checkDensity () const
  {
    if (isInvalidated()) const_cast<GCell*>(this)->updateDensity();

    if ( not Session::isInDemoMode() and Session::doWarnGCellOverload() ) {
      for ( size_t i=0 ; i<_depth ; i++ ) {
        if (_densities[i] > 1.0) {
          cparanoid << Warning( "%s overloaded in %s (M2:%.2f M3:%.2f M4:%.2f M5:%.2f)"
                              , _getString().c_str()
                              , getString(Session::getRoutingGauge()->getRoutingLayer(i)->getName()).c_str()
                              , _densities[1]  // M2
                              , _densities[2]  // M3
                            //, _blockages[2]  // M4
                              , _densities[3]  // M5
                              , _densities[4]  // M6
                              )
             << endl;
        }
      }
    }

    return isSaturated() ? 1 : 0 ;
  }


  bool  GCell::hasFreeTrack ( size_t depth, float reserve ) const
  {
    if (isInvalidated()) const_cast<GCell*>(this)->updateDensity();

    float capacity = getCapacity(depth);

    cdebug_log(149,0) << "  | hasFreeTrack [" << getId() << "] depth:" << depth << " "
                      << Session::getRoutingGauge()->getRoutingLayer(depth)->getName()
                    //<< " " << (_densities[depth]*capacity) << " vs. " << capacity
                      << " " << _feedthroughs[depth] << " vs. " << capacity
                      << " " << this << endl;
    
    return (_feedthroughs[depth] + 0.99 + reserve <= capacity);
  }


  size_t  GCell::getNetCount () const
  {
    set<Net*> nets;

    for ( Edge* edge : _westEdges  ) for ( Segment* segment : edge->getSegments() ) nets.insert( segment->getNet() );
    for ( Edge* edge : _eastEdges  ) for ( Segment* segment : edge->getSegments() ) nets.insert( segment->getNet() );
    for ( Edge* edge : _northEdges ) for ( Segment* segment : edge->getSegments() ) nets.insert( segment->getNet() );
    for ( Edge* edge : _southEdges ) for ( Segment* segment : edge->getSegments() ) nets.insert( segment->getNet() );

    return nets.size();
  }


  void  GCell::rpDesaturate ( set<Net*>& globalNets )
  {
    set<RoutingPad*> rps;
    getRoutingPads( rps );

    set<Net*> rpNets;
    for ( RoutingPad* rp : rps ) {
      if (rp->getLayer() != Session::getRoutingLayer(0)) continue;
      rpNets.insert( rp->getNet() );
    }

    if (rpNets.size() < Session::getSaturateRp()) return;

    cerr << Warning("%s has %zd terminals (h:%zd, v:%zd)"
                   ,getString(this).c_str()
                   ,rps.size()
                   ,_hsegments.size()
                   ,_vsegments.size()
                   ) << endl;

    AutoSegment* segment;
    while ( (_densities[1] > 0.5) and stepDesaturate(1,globalNets,segment,Flags::ForceMove) ) {
      cdebug_log(149,0) << "Moved up: " << segment << endl;
    }
  }


  bool  GCell::stepDesaturate ( size_t        depth
                              , set<Net*>&    globalNets
                              , AutoSegment*& moved
                              , Flags         flags
                              )
  {
    cdebug_log(9000,0) << "Deter| GCell::stepDesaturate() [" << getId() << "] depth:" << depth << endl;

    updateDensity();
    moved = NULL;

    if (not (flags & Flags::ForceMove) and not isSaturated(depth)) return false;

    vector<AutoSegment*>::iterator isegment;
    vector<AutoSegment*>::iterator iend;

    if (Session::getDirection(depth) & Flags::Horizontal) {
      iend     = _hsegments.end  ();
      isegment = _hsegments.begin();
    } else {
      iend     = _vsegments.end  ();
      isegment = _vsegments.begin();
    }

    for ( ; (isegment != iend) ; isegment++ ) {
      unsigned int segmentDepth = Session::getRoutingGauge()->getLayerDepth((*isegment)->getLayer());

      if (segmentDepth < depth) continue;
      if (segmentDepth > depth) break;

      globalNets.insert( (*isegment)->getNet() );
      cdebug_log(9000,0) << "Deter| Move up " << (*isegment) << endl;

      moved = (*isegment);

      if (moved) return true;
    }

    return false;
  }


  bool  GCell::stepBalance ( size_t depth, GCell::Set& invalidateds )
  {
    cdebug_log(149,0) << "stepBalance() - " << this << endl;

    updateDensity();

    vector<AutoSegment*>::iterator isegment;
    vector<AutoSegment*>::iterator iend;
    set<Net*>                      globalNets;

    if (Session::getDirection(depth) & Flags::Horizontal) {
      iend     = _hsegments.end  ();
      isegment = _hsegments.begin();
    } else {
      iend     = _vsegments.end  ();
      isegment = _vsegments.begin();
    }

    for ( ; (isegment != iend) ; isegment++ ) {
      unsigned int segmentDepth = Session::getRoutingGauge()->getLayerDepth((*isegment)->getLayer());

      if (segmentDepth < depth) continue;
      if (segmentDepth > depth) break;

#if THIS_IS_DISABLED
    // Hard-coded: reserve 3 tracks (1/20 * 3).
      if ((*isegment)->canMoveULeft(0.05)) {
        getAnabatic()->moveULeft(*isegment,globalNets,invalidateds);
        return true;
      }
      if ((*isegment)->canMoveURight(0.05)) {
        getAnabatic()->moveURight(*isegment,globalNets,invalidateds);
        return true;
      }
#endif
    }

    return false;
  }


  bool  GCell::stepNetDesaturate ( size_t depth, set<Net*>& globalNets, GCell::Set& invalidateds )
  {
    cdebug_log(149,0) << "GCell::stepNetDesaturate() depth:" << depth << endl;
    cdebug_log(9000,0) << "Deter| " << this << endl;

    updateDensity();

    vector<AutoSegment*>::iterator isegment;
    vector<AutoSegment*>::iterator iend;

    if (Session::getDirection(depth) & Flags::Horizontal) {
      iend     = _hsegments.end   ();
      isegment = _hsegments.begin ();
    } else {
      iend     = _vsegments.end   ();
      isegment = _vsegments.begin ();
    }

    for ( ; (isegment != iend) ; isegment++ ) {
      unsigned int segmentDepth = Session::getRoutingGauge()->getLayerDepth((*isegment)->getLayer());

      if (segmentDepth < depth) continue;
      if (segmentDepth > depth) break;

      cdebug_log(149,0) << "Move up " << (*isegment) << endl;

      if (getAnabatic()->moveUpNetTrunk(*isegment,globalNets,invalidateds))
        return true;
    }

    return false;
  }
  

  void  GCell::forceEdgesCapacities ( unsigned int hcapacity, unsigned int vcapacity )
  {
    if (getEastEdge() ) getEastEdge ()->forceCapacity( hcapacity );
    if (getWestEdge() ) getWestEdge ()->forceCapacity( hcapacity );
    if (getNorthEdge()) getNorthEdge()->forceCapacity( vcapacity );
    if (getSouthEdge()) getSouthEdge()->forceCapacity( vcapacity );
    if (getEastEdge() ) getEastEdge ()->setRealOccupancy(0);
    if (getWestEdge() ) getWestEdge ()->setRealOccupancy(0);
    if (getNorthEdge()) getNorthEdge()->setRealOccupancy(0);
    if (getSouthEdge()) getSouthEdge()->setRealOccupancy(0);
  }


  string  GCell::_getTypeName () const
  { return getString(_extensionName); }


  string  GCell::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.size()-1, " "+getString(getBoundingBox()) );
    s.insert( s.size()-1, " "+getString(_flags) );
    s.insert( s.size()-1, " "+getString(_rpCount) );
  /* string s = "<GCell at(" + DbU::getValueString(getXMin())
       +  "-" + DbU::getValueString(getYMin()) 
       +  "-" + DbU::getValueString(getXMax()) 
       +  "-" + DbU::getValueString(getYMax()) 
       +  "-" + DbU::getValueString(getHeight())
       +  "-" + DbU::getValueString(getWidth()) + ")";*/
    return s;
  }


  Record* GCell::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_flags"      , &_flags     ) );
    record->add( getSlot("_westEdges"  , &_westEdges ) );
    record->add( getSlot("_eastEdges"  , &_eastEdges ) );
    record->add( getSlot("_southEdges" , &_southEdges) );
    record->add( getSlot("_northEdges" , &_northEdges) );
    record->add( DbU::getValueSlot("_xmin", &_xmin) );
    record->add( DbU::getValueSlot("_ymin", &_ymin) );
    record->add( getSlot ( "_gcontacts", &_gcontacts ) );
    record->add( getSlot ( "_vsegments", &_vsegments ) );
    record->add( getSlot ( "_hsegments", &_hsegments ) );
    record->add( getSlot ( "_contacts" , &_contacts  ) );
    record->add( getSlot ( "_depth"    , &_depth     ) );

    RoutingGauge* rg = getAnabatic()->getConfiguration()->getRoutingGauge();

    for ( size_t depth=0 ; depth<_depth ; ++depth ) {
      ostringstream s;
      const Layer* layer = rg->getRoutingLayer(depth)->getBlockageLayer();
      s << "_blockages[" << depth << ":" << ((layer) ? layer->getName() : "None") << "]";
      record->add( DbU::getValueSlot( s.str(), &_blockages[depth] ) );
    }

    for ( size_t depth=0 ; depth<_depth ; ++depth ) {
      ostringstream s;
      const Layer* layer = rg->getRoutingLayer(depth);
      s << "_densities[" << depth << ":" << ((layer) ? layer->getName() : "None") << "]";
      record->add( getSlot ( s.str(),  &_densities[depth] ) );
    }

    for ( size_t depth=0 ; depth<_depth ; ++depth ) {
      ostringstream s;
      const Layer* layer = rg->getRoutingLayer(depth);
      s << "_feedthroughs[" << depth << ":" << ((layer) ? layer->getName() : "None") << "]";
      record->add( getSlot ( s.str(),  &_feedthroughs[depth] ) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::GCellDensitySet".


  GCellDensitySet::GCellDensitySet ( size_t depth )
    : _depth   (depth)
    , _set     ()
    , _requests()
  { }


  GCellDensitySet::GCellDensitySet ( size_t depth, const GCell::Vector& gcells )
    : _depth   (depth)
    , _set     ()
    , _requests()
  {
    for ( size_t i=0 ; i<gcells.size() ; i++ )
      _requests.insert( gcells[i] );
    requeue();
  }


  GCellDensitySet::~GCellDensitySet ()
  {
    if (not _requests.empty()) {
      cerr << Warning("~GCellDensitySet(): Still contains %d requests (and %d elements)."
                     ,_requests.size(),_set.size()) << endl;
    }
  }


  void  GCellDensitySet::requeue ()
  {
    cdebug_log(149,0) << "GCellDensitySet::requeue()" << endl;

    std::set<GCell*,GCell::CompareByKey>::iterator iinserted;
    GCell::Set::iterator                           igcell    = _requests.begin();

  // Remove invalidateds GCell from the queue.
    for ( ; igcell != _requests.end() ; ++igcell ) {
      iinserted = _set.find(*igcell);
      if (iinserted != _set.end()) {
        _set.erase( iinserted );
      }
    }

  // Re-insert invalidateds GCell in the queue *after* updating the key.
    for ( igcell = _requests.begin() ; igcell != _requests.end() ; ++igcell ) {
      (*igcell)->updateKey( _depth );
      _set.insert( *igcell );
    }

    _requests.clear();
  }


// -------------------------------------------------------------------
// Utilities.


  string  getVectorString ( float* v, size_t size )
  {
    ostringstream s;

    s << setprecision(3);
    for ( size_t i=0 ; i<size ; i++ ) {
      if ( !i ) s << "[";
      else      s << " ";
      s << v[i];
    }
    s << "]";

    return s.str();
  }


  bool  isLess ( const GCell* lhs, const GCell* rhs, Flags direction )
  {
    if (direction & Flags::Horizontal) {
      if (lhs->getXMin() != rhs->getXMin()) return lhs->getXMin() < rhs->getXMin();
    } else {
      if (direction & Flags::Vertical) {
        if (lhs->getYMin() != rhs->getYMin()) return lhs->getYMin() < rhs->getYMin();
      }
    }
    return lhs->getId() < rhs->getId();
  }


  bool  isGreater ( const GCell* lhs, const GCell* rhs, Flags direction )
  {
    if (direction & Flags::Horizontal) {
      if (lhs->getXMin() != rhs->getXMin()) return lhs->getXMin() > rhs->getXMin();
    } else {
      if (direction & Flags::Vertical) {
        if (lhs->getYMin() != rhs->getYMin()) return lhs->getYMin() > rhs->getYMin();
      }
    }
    return lhs->getId() > rhs->getId();
  }


}  // Anabatic namespace.
