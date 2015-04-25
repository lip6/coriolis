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
// |  C++ Module  :       "./GCell.cpp"                              |
// +-----------------------------------------------------------------+


#include  <cmath>
#include  <cstdlib>
#include  <iomanip>
#include  <sstream>
#include  <functional>

#include  "hurricane/Bug.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Layer.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/RoutingPad.h"
#include  "crlcore/RoutingGauge.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoSegment.h"
#include  "katabatic/GCell.h"
#include  "katabatic/GCellGrid.h"
#include  "katabatic/Session.h"
#include  "katabatic/KatabaticEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace Katabatic;


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
          bool operator() ( const Axis* lhs, const Axis* rhs );
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
    merge ( Interval ( ufragments->getMin()-ufragments->getPitch(), ufragments->getMin() ) );
    merge ( Interval ( ufragments->getMax(), ufragments->getMax()+ufragments->getPitch() ) );
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
      if ( chunkMerge.getVMax() < (*ichunk).getVMin() ) break;

      if ( chunkMerge.intersect(*ichunk) ) {
        if ( imerge == _chunks.end() ) {
          imerge = ichunk;
          (*imerge).merge ( chunkMerge );
        } else {
          (*imerge).merge ( *ichunk );
          ichunk = _chunks.erase ( ichunk );
          continue;
        }
      }
      ichunk++;
    }

    if ( imerge == _chunks.end() ) {
      _chunks.insert ( ichunk, chunkMerge );
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

      if ( inext == iend ) break;

      Interval currentFree ( (*ichunk).getVMax(), (*inext).getVMin() );
      if ( currentFree.getSize() > maxFree.getSize() )
        maxFree = currentFree;

      // cerr << "    @" << DbU::getValueString(_axis)
      //      << " before:" << *ichunk << " after:" << *inext
      //      << " size:" << DbU::getValueString(currentFree.getSize()) << endl;
    }

    return maxFree;
  }


  inline bool  UsedFragments::AxisCompare::operator() ( const Axis* lhs, const Axis* rhs )
  {
    if ( lhs->getAxis () < rhs->getAxis () ) return true;
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
      _axiss.erase ( _axiss.begin() );
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
    if ( restrict.isEmpty() ) return;

    vector<Axis*>::iterator iaxis = find_if ( _axiss.begin(), _axiss.end(), AxisMatch(axis) );

    Axis* paxis = NULL;
    if ( iaxis == _axiss.end() ) {
      paxis = new Axis(this,axis);
      _axiss.push_back ( paxis );
      stable_sort ( _axiss.begin(), _axiss.end(), AxisCompare() );
    } else {
      paxis = *iaxis;
    }

    paxis->merge ( restrict );
  }


  Interval  UsedFragments::getMaxFree () const
  {
  //cerr << "capacity:" << _capacity << " _globals:" << _globals << " _axiss:" << _axiss.size() << endl;
    if ( _capacity > _globals + _axiss.size() + 1 )
      return _span;

    Interval maxFree;
    vector<Axis*>::const_iterator iaxis = _axiss.begin();
    for ( ; iaxis != _axiss.end() ; ++iaxis ) {
      Interval axisMaxFree = (*iaxis)->getMaxFree();
      if ( axisMaxFree.getSize() > maxFree.getSize() )
        maxFree = axisMaxFree;
    }

    return maxFree;
  }


} // End of anonymous namespace.


namespace Katabatic {


  using namespace std;
  using Hurricane::roundfp;
  using Hurricane::tab;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::inltrace;
  using Hurricane::ForEachIterator;
  using Hurricane::Warning;
  using Hurricane::Bug;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  


// -------------------------------------------------------------------
// Class  :  "Katabatic::GCell::CompareByDensity".


  GCell::CompareByDensity::CompareByDensity ( unsigned int depth )
    : _depth(depth)
  { }


  bool  GCell::CompareByDensity::operator() ( GCell* lhs, GCell* rhs )
  {
  //float difference = roundfp ( lhs->getDensity(_depth) - rhs->getDensity(_depth) );
    float difference = lhs->getDensity(_depth) - rhs->getDensity(_depth);
    if (difference != 0.0) return (difference > 0.0);

  //int difference = floatCompare ( lhs->getDensity(_depth), rhs->getDensity(_depth) );
  //if ( abs(difference) > 1000 ) return difference > 0;
    
    return lhs->getIndex() < rhs->getIndex();
  }


// -------------------------------------------------------------------
// Class  :  "Kite::GCell::CompareByKey".
//
// lhs < rhs --> true


  bool  GCell::CompareByKey::operator() ( const GCell* lhs, const GCell* rhs )
  {
    return lhs->getKey() < rhs->getKey();
  }


// -------------------------------------------------------------------
// Class  :  "Katabatic::GCell".


  const Name  GCell::_goName         = "Katabatic::GCell";
  DbU::Unit   GCell::_topRightShrink = 0;
  size_t      GCell::_allocateds     = 0;


  GCell::GCell ( GCellGrid* gcellGrid, unsigned int index, const Box& box )
    : ExtensionGo        (gcellGrid->getCell())
    , _gcellGrid         (gcellGrid)
    , _index             (index)
    , _vsegments         ()
    , _hsegments         ()
    , _contacts          ()
    , _box               (box)
    , _depth             (Session::getRoutingGauge()->getDepth())
    , _pinDepth          (0)
    , _blockages         (new DbU::Unit [_depth])
    , _cDensity          (0.0)
    , _densities         (new float [_depth])
    , _feedthroughs      (new float [_depth])
    , _fragmentations    (new float [_depth])
    , _globalsCount      (new float [_depth])
  //, _blockedAxis       (this)
  //, _saturateDensities (new float [_depth])
    , _flags             (GCellInvalidated)
    , _key               (this,1)
  {
    for ( size_t i=0 ; i<_depth ; i++ ) {
      _blockages        [i] = 0;
      _densities        [i] = 0.0;
      _feedthroughs     [i] = 0.0;
      _fragmentations   [i] = 0.0;
      _globalsCount     [i] = 0.0;
    //_saturateDensities[i] = 0.0;

      if ( Session::getRoutingGauge()->getLayerGauge(i)->getType() == Constant::PinOnly )
        ++_pinDepth;
    }

    updateKey(1);
    _allocateds++;
  }


  void  GCell::_postCreate ()
  {
    ltrace(90) << "GCell::_postCreate() - " << (void*)this << " " << this << endl;
    ltracein(90);

    ExtensionGo::_postCreate ();

    ltraceout(90);
  }


  GCell* GCell::create ( GCellGrid* gcellGrid, unsigned int index, Box box )
  {
    if (_topRightShrink == 0) _topRightShrink = 1;

    DbU::Unit  trShrink = (  gcellGrid->isOnTopBorder  (index)
                          or gcellGrid->isOnRightBorder(index)) ? 0 : _topRightShrink;

    ltrace(90) << "Gcell::create()" << endl;
    GCell* gcell = new GCell ( gcellGrid, index, box.inflate( 0 ,0 ,-trShrink ,-trShrink ) );

    gcell->_postCreate ();

    return gcell;
  }


  GCell::~GCell ()
  {
    ltrace(90) << "GCell::~GCell()" << endl;

    delete [] _blockages;
    delete [] _densities;
    delete [] _feedthroughs;
    delete [] _fragmentations;
    delete [] _globalsCount;
  //delete [] _saturateDensities;

    _allocateds--;
  }


  void  GCell::_preDestroy ()
  {
    ltrace(90) << "GCell::_preDestroy() - " << (void*)this << " " << this << endl;
    ExtensionGo::_preDestroy ();
  }


  bool  GCell::isSaturated ( unsigned int depth ) const
  { return getDensity(depth) > Session::getSaturateRatio(); }


  bool  GCell::isAboveDensity ( float threshold ) const
  {
    if (not isValid()) const_cast<GCell*>(this)->updateDensity();
    float difference = roundfp ( getDensity() - threshold );
    
  //int difference = floatDifference(getDensity(),threshold,10000);
    ltrace(190) << "GCell:isAboveDensity() " << threshold << " diff:" << difference << endl;
    return (difference >= 0.0);
  }


  bool  GCell::areDensityConnex ( GCell* a, GCell* b )
  {
    ltrace(190) << "GCell:areDensityConnex()" << endl;

    for ( unsigned int i=1 ; i<a->getDepth() ; i++ ) { // Ugly: hard-coded skip METAL1.
    //int highDiffa = floatDifference(a->_densities[i],0.6,10000);
    //int highDiffb = floatDifference(b->_densities[i],0.6,10000);

      float highDiffa = roundfp ( a->_densities[i] - 0.6 );
      float highDiffb = roundfp ( b->_densities[i] - 0.6 );
      ltrace(190) << "Compare depth " << i
                  << " "   << a->_densities[i] << "," << highDiffa
                  << " & " << b->_densities[i] << "," << highDiffb << endl;

      if ( (highDiffa > 0) and (highDiffb > 0) ) {
        ltrace(190) << "GCell::areDensityConnex() Neighboring high saturated GCell (depth:" << i
                    << " " << a->_densities[i] << " & " << b->_densities[i] << ")" << endl;
        return true;
      }
    }
    return false;
  }


  void  GCell::getDensities ( float* densities ) const
  {
    if (not isValid()) const_cast<GCell*>(this)->updateDensity();

    for ( unsigned int i=0 ; i<_depth ; i++ ) {
      densities[i] = _densities[i];
    }
  }


  const Name& GCell::getStaticName ()
  { return _goName; }


  size_t  GCell::getAllocateds ()
  { return _allocateds; }


  DbU::Unit  GCell::getTopRightShrink ()
  { return _topRightShrink; }


  const Name& GCell::getName () const
  { return _goName; }


  unsigned int  GCell::getRow () const
  { return _gcellGrid->getRow ( _index ); }


  unsigned int  GCell::getColumn () const
  { return _gcellGrid->getColumn ( _index ); }


  GCell* GCell::getLeft () const
  { return getGCellGrid()->getGCellLeft(this); }


  GCell* GCell::getRight () const
  { return getGCellGrid()->getGCellRight(this); }


  GCell* GCell::getUp () const
  { return getGCellGrid()->getGCellUp(this); }


  GCell* GCell::getDown () const
  { return getGCellGrid()->getGCellDown(this); }


  Interval  GCell::getSide ( unsigned int direction ) const
  {
    Interval side;
    switch ( direction ) {
      default:
      case KbHorizontal: side = Interval(_box.getXMin(),_box.getXMax()); break;
      case KbVertical:   side = Interval(_box.getYMin(),_box.getYMax()); break;
    }
    return side;
  }


  AutoSegments  GCell::getHStartSegments ()
  {
    return new AutoSegments_AnchorOnGCell (this,KbHorizontal|KbSource);
  }


  AutoSegments  GCell::getVStartSegments ()
  {
    return new AutoSegments_AnchorOnGCell (this,KbVertical|KbSource);
  }


  AutoSegments  GCell::getHStopSegments ()
  {
    return new AutoSegments_AnchorOnGCell (this,KbHorizontal|KbTarget);
  }


  AutoSegments  GCell::getVStopSegments ()
  {
    return new AutoSegments_AnchorOnGCell (this,KbVertical|KbTarget);
  }


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


  float  GCell::getHCapacity () const
  {
    return (float)( _box.getHeight() / Session::getPitch(1) + 1 );
  }


  float  GCell::getVCapacity () const
  {
    return (float)( _box.getWidth () / Session::getPitch(2) + 1 );
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
      if ( i%2 ) { hdensity += _densities[i]; ++hplanes; }
      else       { vdensity += _densities[i]; ++vplanes; }
    }

    if (hplanes) hdensity /= hplanes;
    if (vplanes) vdensity /= vplanes;

    return std::max(hdensity, vdensity);
  }

  float  GCell::getDensity ( unsigned int flags ) const
  {
    if (not isValid() and not(flags & NoUpdate)) const_cast<GCell*>(this)->updateDensity();

    float density = 0.0;

    if (getGCellGrid()->getDensityMode() == GCellGrid::AverageHVDensity) {
      density = getAverageHVDensity();
    } else if (getGCellGrid()->getDensityMode() == GCellGrid::MaxHVDensity) {
      density = getMaxHVDensity();
    } else if (getGCellGrid()->getDensityMode() == GCellGrid::AverageHDensity) {
    // Average density between all horizontal layers.
      size_t hplanes  = 0;
      float  hdensity = 0.0;

      for ( size_t i=_pinDepth ; i<_depth ; i++ ) {
        if ( i%2 ) { hdensity += _densities[i]; ++hplanes; }
      }
      if (hplanes) hdensity /= hplanes;

    //density = roundfp ( hdensity );
      density = hdensity;
    } else if (getGCellGrid()->getDensityMode() == GCellGrid::AverageVDensity) {
    // Average density between all vertical layers.
      size_t vplanes  = 0;
      float  vdensity = 0.0;

      for ( size_t i=_pinDepth ; i<_depth ; i++ ) {
        if ( i%2 == 0 ) { vdensity += _densities[i]; ++vplanes; }
      }

      if (vplanes) vdensity /= vplanes;

    //density = roundfp ( vdensity );
      density = vdensity;
    } else if ( getGCellGrid()->getDensityMode() == GCellGrid::MaxDensity ) {
    // Density of the most saturated layer.
      for ( size_t i=_pinDepth ; i<_depth ; i++ ) {
        if ( _densities[i] > density ) density = _densities[i];
      }
      density = roundfp(density);
    } else if (getGCellGrid()->getDensityMode() == GCellGrid::MaxHDensity) {
    // Density of the most saturated horizontal layer.
      for ( size_t i=_pinDepth ; i<_depth ; i++ ) {
        if ( (i%2) and (_densities[i] > density) ) density = _densities[i];
      }
    //density = roundfp(density);
    } else if (getGCellGrid()->getDensityMode() == GCellGrid::MaxVDensity) {
    // Density of the most saturated vertical layer.
      for ( size_t i=_pinDepth ; i<_depth ; i++ ) {
        if ( (i%2 == 0) and (_densities[i] > density) ) density = _densities[i];
      }
    //density = roundfp(density);
    }

    return density;
  }


  void  GCell::addBlockage ( unsigned int depth, DbU::Unit length )
  {
    if ( depth >= _depth ) return;

    _blockages[depth] += length;
    _flags |= GCellInvalidated;

    // if ( _blockages[depth] >= 8.0 ) {
    //   cinfo << Warning("%s is under power ring.",getString(this).c_str()) << endl;

    //   unsigned int modulo = depth % 2;

    //   for ( unsigned int parallel=0 ; parallel < _depth ; ++parallel ) {
    //     if ( parallel%2 == modulo ) {
    //       _blockages[parallel] = 10.0;
    //       cinfo << "  Blocking [" << parallel << "]:"
    //             << Session::getRoutingLayer(parallel)->getName() << endl;
    //     }
    //   }
    // }

    ltrace(300) << "GCell:addBlockage() " << this << " "
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
      ltrace(200) << "remove " << ac << " from " << this << endl;
      _contacts.pop_back();
    } else {
      cerr << Bug("%p:%s do not belong to %s."
                 ,ac->base(),getString(ac).c_str(),_getString().c_str()) << endl;
    }

#if 0
    vector<AutoContact*>::iterator  it  = _contacts.begin ();
    vector<AutoContact*>::iterator  end = _contacts.end ();
    for ( ; it != end ; it++ )
      if ( *it == ac ) *it = *(--end);
    _contacts.pop_back ();
#endif
  }


  void  GCell::removeHSegment ( AutoSegment* segment )
  {
    size_t end   = _hsegments.size();
    size_t begin = 0;

    for ( ; begin < end ; begin++ ) {
      if ( _hsegments[begin] == segment ) {
        swap ( _hsegments[begin], _hsegments[--end] );
      }
    }

    if ( _hsegments.size() == end ) {
      cerr << Bug("%s do not go through %s."
                 ,getString(segment).c_str()
                 ,_getString().c_str()) << endl;
      return;
    }

    if ( _hsegments.size() - end > 1 )
      cerr << Bug("%s has multiple occurrences of %s."
                 ,_getString().c_str()
                 ,getString(segment).c_str()) << endl;

    _hsegments.erase ( _hsegments.begin() + end, _hsegments.end() );
  }


  void  GCell::removeVSegment ( AutoSegment* segment )
  {
    size_t end   = _vsegments.size();
    size_t begin = 0;

    for ( ; begin < end ; begin++ ) {
      if ( _vsegments[begin] == segment ) {
        swap ( _vsegments[begin], _vsegments[--end] );
      }
    }

    if ( _vsegments.size() == end ) {
      cerr << Bug("%s do not go through %s."
                 ,getString(segment).c_str()
                 ,_getString().c_str()) << endl;
      return;
    }

    if ( _vsegments.size() - end > 1 )
      cerr << Bug("%s has multiple occurrences of %s."
                 ,_getString().c_str()
                 ,getString(segment).c_str()) << endl;

    _vsegments.erase ( _vsegments.begin() + end, _vsegments.end() );
  }


  void  GCell::updateContacts ()
  {
    vector<AutoContact*>::iterator  it  = _contacts.begin ();
    vector<AutoContact*>::iterator  end = _contacts.end ();
    for ( ; it != end ; it++ ) (*it)->updateGeometry ();
  }


  size_t  GCell::updateDensity ()
  {
    if (isValid()) return (isSaturated()) ? 1 : 0;

    _flags &= ~GCellSaturated;

    for ( size_t i=0 ; i<_vsegments.size() ; i++ ) {
      if ( _vsegments[i] == NULL )
        cerr << "NULL Autosegment at index " << i << endl;
    }

    sort ( _hsegments.begin(), _hsegments.end(), AutoSegment::CompareByDepthLength() );
    sort ( _vsegments.begin(), _vsegments.end(), AutoSegment::CompareByDepthLength() );

    float                    hcapacity    = getHCapacity ();
    float                    vcapacity    = getVCapacity ();
    float                    ccapacity    = hcapacity * vcapacity * 4; 
    DbU::Unit                hpenalty     = 0 /*_box.getWidth () / 3*/;
    DbU::Unit                vpenalty     = 0 /*_box.getHeight() / 3*/;
    DbU::Unit                uLengths1    [ _depth ];
    DbU::Unit                uLengths2    [ _depth ];
    float                    localCounts  [ _depth ];
    vector<UsedFragments>    ufragments   ( _depth );

    for ( size_t i=0 ; i<_depth ; i++ ) {
      ufragments[i].setPitch ( Session::getPitch(i) );
      _feedthroughs[i] = 0.0;
      uLengths2    [i] = 0;
      localCounts  [i] = 0.0;
      _globalsCount[i] = 0.0;

      switch ( Session::getDirection(i) ) {
        case KbHorizontal:
          ufragments[i].setSpan     ( _box.getXMin(), _box.getXMax() );
          ufragments[i].setCapacity ( (size_t)hcapacity );
          break;
        case KbVertical:
          ufragments[i].setSpan     ( _box.getYMin(), _box.getYMax() );
          ufragments[i].setCapacity ( (size_t)vcapacity );
          break;
      }
    }

  // Compute wirelength associated to contacts (in DbU::Unit converted to float).
    AutoSegment::DepthLengthSet  processeds;
    vector<AutoContact*>::iterator  it = _contacts.begin();
    vector<AutoContact*>::iterator end = _contacts.end  ();
    for ( ; it != end ; it++ ) {
      for ( size_t i=0 ; i<_depth ; i++ ) uLengths1[i] = 0;
      (*it)->getLengths ( uLengths1, processeds );
      for ( size_t i=0 ; i<_depth ; i++ ) {
        switch ( Session::getDirection(i) ) {
          case KbHorizontal: uLengths2[i] += uLengths1[i]+hpenalty; break;
          case KbVertical:   uLengths2[i] += uLengths1[i]+vpenalty; break;
        }
      }
    }

  // Add the "pass through" horizontal segments.
    if ( _hsegments.size() ) {
      const Layer* layer = _hsegments[0]->getLayer();
      size_t       depth = Session::getRoutingGauge()->getLayerDepth(layer);
      size_t       count = 0;
      for ( size_t i=0 ; i<_hsegments.size() ; i++ ) {
        _globalsCount[depth] += 1.0;
        ufragments[depth].incGlobals();

        if ( layer != _hsegments[i]->getLayer() ) {
          uLengths2[depth] += count * _box.getWidth();

          count = 0;
          layer = _hsegments[i]->getLayer();
          depth = Session::getRoutingGauge()->getLayerDepth(layer);
        }
        count++;
        _feedthroughs[depth] += 1.0;
      }
      if ( count ) {
        uLengths2[depth] += count * _box.getWidth();
      }
    }

  // Add the "pass through" vertical segments.
    if ( _vsegments.size() ) {
      const Layer* layer = _vsegments[0]->getLayer();
      size_t       depth = Session::getRoutingGauge()->getLayerDepth(layer);
      size_t       count = 0;
      for ( size_t i=0 ; i<_vsegments.size() ; i++ ) {
        _globalsCount[depth] += 1.0;
        ufragments[depth].incGlobals();

        if ( layer != _vsegments[i]->getLayer() ) {
          uLengths2[depth] += count * _box.getHeight();

          count = 0;
          layer = _vsegments[i]->getLayer();
          depth = Session::getRoutingGauge()->getLayerDepth(layer);
        }
        count++;
        _feedthroughs[depth] += 1.0;
      }
      if ( count ) {
        uLengths2[depth] += count * _box.getHeight();
      }
    }

  // Add the blockages.
    for ( size_t i=0 ; i<_depth ; i++ ) {
      uLengths2[i] += _blockages[i];
    }

  // Compute the number of non pass-through tracks.
    if ( processeds.size() ) {
      AutoSegment::DepthLengthSet::iterator isegment = processeds.begin();
      const Layer* layer = (*isegment)->getLayer();
      DbU::Unit    axis  = (*isegment)->getAxis();
      size_t       depth = Session::getRoutingGauge()->getLayerDepth(layer);
      size_t       count = 0;
      for ( ; isegment != processeds.end(); ++isegment ) {
        _feedthroughs[depth] += ((*isegment)->isGlobal()) ? 0.50 : 0.33;
        localCounts  [depth] += 1.0;
        if ( (*isegment)->isGlobal() ) _globalsCount[depth] += 1.0;

        ufragments[depth].merge ( (*isegment)->getAxis(), (*isegment)->getSpanU() );
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
      switch ( Session::getDirection(i) ) {
        case KbHorizontal:
          _densities     [i]  = ((float)uLengths2[i]) / ( hcapacity * (float)_box.getWidth() );
          _feedthroughs  [i] += (float)(_blockages[i] / _box.getWidth());
          _fragmentations[i]  = (float)ufragments[i].getMaxFree().getSize() / (float)_box.getWidth();
          break;
        case KbVertical:
          _densities     [i]  = ((float)uLengths2[i]) / ( vcapacity * (float)_box.getHeight() );
          _feedthroughs  [i] += (float)(_blockages[i] / _box.getHeight());
          _fragmentations[i]  = (float)ufragments[i].getMaxFree().getSize() / (float)_box.getHeight();
          break;
      }

      if (_densities[i] >= 1.0)
        _flags |= GCellSaturated;
    }

    _cDensity  = ( (float)_contacts.size() ) / ccapacity;
    _flags    &= ~GCellInvalidated;

  //for ( size_t i=0 ; i<_depth ; i++ ) { _densities[i] = roundfp ( _densities[i] ); }
  //_cDensity = roundfp (_cDensity );

  //ltrace(190) << "updateDensity: " << this << endl;

    checkDensity();

    return isSaturated() ? 1 : 0 ;
  }


  size_t  GCell::checkDensity () const
  {
    if (not isValid()) const_cast<GCell*>(this)->updateDensity ();

    if ( not Session::isInDemoMode() and Session::doWarnGCellOverload() ) {
      for ( size_t i=0 ; i<_depth ; i++ ) {
        if ( _densities[i] > 1.0 ) {
          cparanoid << Warning( "%s @%dx%d overloaded in %s (M2:%.2f M3:%.2f M4:%.2f M5:%.2f)"
                              , _getString().c_str()
                              , getColumn()
                              , getRow()
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
      // for ( size_t i=3 ; i<_depth ; i+=2 ) {
      //   if ( (_densities[i] < 0.5) and (_densities[i-2] < 0.5) ) continue;

      //   float balance = _densities[i] / (_densities[i-2]+0.001 );
      //   if ( (balance > 3) or (balance < .33) ) {
      //     cerr << Warning("%s @%dx%d unbalanced in %s (M2:%.2f M3:%.2f M4:%.2f M5:%.2f)"
      //                    ,_getString().c_str()
      //                    ,getColumn()
      //                    ,getRow()
      //                    ,getString(Session::getRoutingGauge()->getRoutingLayer(i)->getName()).c_str()
      //                    ,_densities[1]  // M2
      //                    ,_densities[2]  // M3
      //                  //,_blockages[2]  // M4
      //                    ,_densities[3]  // M5
      //                    ,_densities[4]  // M6
      //                    )
      //        << endl;
      //   }
      // }
    }

    return isSaturated() ? 1 : 0 ;
  }


  bool  GCell::hasFreeTrack ( size_t depth, float reserve ) const
  {
    if (not isValid()) const_cast<GCell*>(this)->updateDensity();

#if DEPRECATED
    float  capacity;
    vector<AutoSegment*>::const_iterator isegment;
    vector<AutoSegment*>::const_iterator iend;

    switch ( Session::getDirection(depth) ) {
      case KbHorizontal:
        iend     = _hsegments.end   ();
        isegment = _hsegments.begin ();
        capacity = getHCapacity ();
        break;
      case KbVertical:
        iend     = _vsegments.end   ();
        isegment = _vsegments.begin ();
        capacity = getVCapacity ();
        break;
    }

    size_t count = 0;
    for ( ; (isegment != iend) ; isegment++ ) {
      unsigned int segmentDepth = Session::getRoutingGauge()->getLayerDepth((*isegment)->getLayer());
      if ( segmentDepth < depth ) continue;
      if ( segmentDepth > depth ) break;
      count++;
    }

    return (capacity - 1.0) >= (float)count;
#endif

    float capacity = 0.0;
    switch ( Session::getDirection(depth) ) {
      case KbHorizontal: capacity = getHCapacity(); break;
      case KbVertical:   capacity = getVCapacity(); break;
    }

    ltrace(200) << "  | hasFreeTrack [" << getIndex() << "] depth:" << depth << " "
                << Session::getRoutingGauge()->getRoutingLayer(depth)->getName()
              //<< " " << (_densities[depth]*capacity) << " vs. " << capacity
                << " " << _feedthroughs[depth] << " vs. " << capacity
                << " " << this << endl;
    
  //return (_densities[depth]*capacity + 1.0 + reserve <= capacity);
    return (_feedthroughs[depth] + 0.99 + reserve <= capacity);
  }


  bool  GCell::checkEdgeSaturation ( size_t hreserved, size_t vreserved) const
  {
    size_t edgeUpUsage       = 0;
    size_t edgeRightUsage    = 0;
    size_t edgeUpCapacity    = getGCellGrid()->getVEdgeCapacity() - vreserved;
    size_t edgeRightCapacity = getGCellGrid()->getHEdgeCapacity() - hreserved;

    if ( getUp() ) {
    // Up Edge Density.
      edgeUpUsage = _vsegments.size();
      for ( size_t icontact=0 ; icontact < _contacts.size() ; icontact++ ) {
        forEach ( Hook*, ihook, _contacts[icontact]->getBodyHook()->getSlaveHooks() ) {
          if ( dynamic_cast<Segment::SourceHook*>(*ihook) == NULL ) continue;

          Segment* segment = dynamic_cast<Vertical*>((*ihook)->getComponent());
          if ( not segment ) continue;

          AutoSegment* autoSegment = Session::lookup ( segment );
          if ( not autoSegment or autoSegment->isLocal() ) continue;

          edgeUpUsage++;
        }
      }
    }

    if ( getRight() ) {
    // Right Edge Density.
      edgeRightUsage = _vsegments.size();
      for ( size_t icontact=0 ; icontact < _contacts.size() ; icontact++ ) {
        forEach ( Hook*, ihook, _contacts[icontact]->getBodyHook()->getSlaveHooks() ) {
          if ( dynamic_cast<Segment::SourceHook*>(*ihook) == NULL ) continue;

          Segment* segment = dynamic_cast<Horizontal*>((*ihook)->getComponent());
          if ( not segment ) continue;

          AutoSegment* autoSegment = Session::lookup ( segment );
          if ( not autoSegment or autoSegment->isLocal() ) continue;

          edgeRightUsage++;
        }
      }
    }

    bool overload = false;
    if ( (edgeUpUsage > edgeUpCapacity) or (edgeRightUsage > edgeRightCapacity) ) {
      overload = true;

      cparanoid << Warning( "In %s, (over h:%d or v:%d)"
                          , _getString().c_str(), edgeRightCapacity, edgeUpCapacity);

      ostringstream message;
      message << setprecision(3);
      if ( edgeUpUsage > edgeUpCapacity )
        message << "up edge: " << edgeUpUsage << " vs. " << getGCellGrid()->getVEdgeCapacity()
                << "-" << vreserved;
      if ( edgeRightUsage > edgeRightCapacity ) {
        if ( message.str().size() ) message << " & ";
        message << "right edge: " << edgeRightUsage << " vs. " << getGCellGrid()->getHEdgeCapacity()
                << "-" << hreserved;
      }

      cparanoid << message.str() << "." << endl;
    }

    return overload;
  }


  void  GCell::rpDesaturate ( set<Net*>& globalNets )
  {
    set<RoutingPad*> rps;
    getRoutingPads ( rps );

    set<Net*> rpNets;
    set<RoutingPad*>::iterator irp = rps.begin();
    for ( ; irp != rps.end() ; ++irp ) {
      if ( (*irp)->getLayer() != Session::getRoutingLayer(0) ) continue;
      rpNets.insert ( (*irp)->getNet() );
    }

    if ( rpNets.size() < Session::getSaturateRp() ) return;

    cerr << Warning("%s has %zd terminals (h:%zd, v:%zd)"
                   ,getString(this).c_str()
                   ,rps.size()
                   ,_hsegments.size()
                   ,_vsegments.size()
                   ) << endl;

    AutoSegment* segment;
    while ( (_densities[1] > 0.5) and stepDesaturate(1,globalNets,segment,KbForceMove) ) {
      ltrace(200) << "Moved up: " << segment << endl;
    }
  }


  bool  GCell::stepDesaturate ( unsigned int  depth
                              , set<Net*>&    globalNets
                              , AutoSegment*& moved
                              , unsigned int  flags
                              )
  {
    ltrace(500) << "Deter| GCell::stepDesaturate() [" << getIndex() << "] depth:" << depth << endl;

    updateDensity ();
    moved = NULL;

    if ( not (flags & KbForceMove) and not isSaturated(depth) ) return false;

  //float capacity;
    vector<AutoSegment*>::iterator isegment;
    vector<AutoSegment*>::iterator iend;

    switch ( Session::getDirection(depth) ) {
      case KbHorizontal:
        iend     = _hsegments.end   ();
        isegment = _hsegments.begin ();
      //capacity = getHCapacity ();
        break;
      case KbVertical:
        iend     = _vsegments.end   ();
        isegment = _vsegments.begin ();
      //capacity = getVCapacity ();
        break;
    }

    for ( ; (isegment != iend) ; isegment++ ) {
      unsigned int segmentDepth = Session::getRoutingGauge()->getLayerDepth((*isegment)->getLayer());

      if ( segmentDepth < depth ) continue;
      if ( segmentDepth > depth ) break;

      globalNets.insert ( (*isegment)->getNet() );
      ltrace(500) << "Deter| Move up " << (*isegment) << endl;

#if THIS_IS_DISABLED
      (*isegment)->changeDepth ( depth+2, false, false );
#endif
      moved = (*isegment);

      if (moved) return true;
    }

    return false;
  }


  bool  GCell::stepBalance ( unsigned int depth, GCell::SetIndex& invalidateds )
  {
    ltrace(200) << "stepBalance() - " << this << endl;

    updateDensity ();

  //float capacity;
    vector<AutoSegment*>::iterator isegment;
    vector<AutoSegment*>::iterator iend;
    set<Net*>                      globalNets;

    switch ( Session::getDirection(depth) ) {
      case KbHorizontal:
        iend     = _hsegments.end   ();
        isegment = _hsegments.begin ();
      //capacity = getHCapacity ();
        break;
      case KbVertical:
        iend     = _vsegments.end   ();
        isegment = _vsegments.begin ();
      //capacity = getVCapacity ();
        break;
    }

    for ( ; (isegment != iend) ; isegment++ ) {
      unsigned int segmentDepth = Session::getRoutingGauge()->getLayerDepth((*isegment)->getLayer());

      if ( segmentDepth < depth ) continue;
      if ( segmentDepth > depth ) break;

    // Hard-coded: reserve 3 tracks (1/20 * 3).
      if ((*isegment)->canMoveULeft(0.05)) {
        getGCellGrid()->getKatabatic()->moveULeft(*isegment,globalNets,invalidateds);
        return true;
      }
      if ((*isegment)->canMoveURight(0.05)) {
        getGCellGrid()->getKatabatic()->moveURight(*isegment,globalNets,invalidateds);
        return true;
      }
    }

    return false;
  }


  bool  GCell::stepNetDesaturate ( unsigned int depth, set<Net*>& globalNets, GCell::SetIndex& invalidateds )
  {
    ltrace(500) << "Deter| GCell::stepNetDesaturate() depth:" << depth << endl;
    ltrace(500) << "Deter| " << this << endl;

    updateDensity ();

  //float capacity;
    vector<AutoSegment*>::iterator isegment;
    vector<AutoSegment*>::iterator iend;

    switch ( Session::getDirection(depth) ) {
      case KbHorizontal:
        iend     = _hsegments.end   ();
        isegment = _hsegments.begin ();
      //capacity = getHCapacity ();
        break;
      case KbVertical:
        iend     = _vsegments.end   ();
        isegment = _vsegments.begin ();
      //capacity = getVCapacity ();
        break;
    }

    for ( ; (isegment != iend) ; isegment++ ) {
      unsigned int segmentDepth = Session::getRoutingGauge()->getLayerDepth((*isegment)->getLayer());

      if ( segmentDepth < depth ) continue;
      if ( segmentDepth > depth ) break;

      ltrace(500) << "Deter| Move up " << (*isegment) << endl;

      if ( getGCellGrid()->getKatabatic()->moveUpNetTrunk(*isegment,globalNets,invalidateds) )
        return true;
    }

    return false;
  }


  Box  GCell::getBoundingBox () const
  {
    return _box;
  }


  void  GCell::translate ( const DbU::Unit&, const DbU::Unit& )
  {
    cerr << Warning("Calling GCell::translate() on %s is likely a bug."
                   ,_getString().c_str()) << endl;
  }


  string  GCell::_getString () const
  {
    Box box = getBoundingBox ();

    ostringstream s;
    s << "<" << _getTypeName() << " [" << _index << "] "
      << DbU::getValueString(box.getXMin()) << ":" << DbU::getValueString(box.getYMin()) << " "
      << DbU::getValueString(box.getXMax()) << ":" << DbU::getValueString(box.getYMax()) << " "
      << setprecision(3)
      << getDensity(NoUpdate) << " "
      << "d:" << _depth << " "
      << getVectorString(_densities   ,_depth) << " "
      << getVectorString(_feedthroughs,_depth)
      << " "
      << (isValid     () ? "-" : "i")
      << (isSaturated () ? "s" : "-")
      << (isUnderIoPad() ? "P" : "-")
      << ">";

    return s.str();
  }


  Record* GCell::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot ( "_gcellGrid"         ,  _gcellGrid          ) );
    record->add ( getSlot ( "_index"             , &_index              ) );
    record->add ( getSlot ( "_vsegments"         , &_vsegments          ) );
    record->add ( getSlot ( "_hsegments"         , &_hsegments          ) );
    record->add ( getSlot ( "_contacts"          , &_contacts           ) );
    record->add ( getSlot ( "_box"               , &_box                ) );
    record->add ( getSlot ( "_depth"             , &_depth              ) );
    record->add ( getSlot ( "_flags"             , &_flags              ) );

    RoutingGauge* rg = getGCellGrid()->getKatabatic()->getRoutingGauge();

    for ( size_t depth=0 ; depth<_depth ; ++depth ) {
      ostringstream s;
      const Layer* layer = rg->getRoutingLayer(depth)->getBlockageLayer();
      s << "_blockages[" << depth << ":" << ((layer) ? layer->getName() : "None") << "]";
      record->add ( getSlot ( s.str(),  &_blockages[depth] ) );
    }

    for ( size_t depth=0 ; depth<_depth ; ++depth ) {
      ostringstream s;
      const Layer* layer = rg->getRoutingLayer(depth);
      s << "_densities[" << depth << ":" << ((layer) ? layer->getName() : "None") << "]";
      record->add ( getSlot ( s.str(),  &_densities[depth] ) );
    }

    // for ( size_t depth=0 ; depth<_depth ; ++depth ) {
    //   ostringstream s;
    //   const Layer* layer = rg->getRoutingLayer(depth);
    //   s << "_saturateDensities[" << depth << ":" << ((layer) ? layer->getName() : "None") << "]";
    //   record->add ( getSlot ( s.str(),  &_saturateDensities[depth] ) );
    // }
                                     
    return record;
  }


  void  GCell::_xmlWrite ( ostream& o ) const
  {
    char line[1024];

    snprintf ( line, 1024
             , "<GCell index=\"%04i %04i\" corner=\"%06.1f %06.1f\" density=\"%3.2f,%3.2f\">"
             , getColumn()
             , getRow()
             , DbU::toLambda(getX())
             , DbU::toLambda(getY())
             , getDensity()
             , _cDensity
             );

    o << line;
  }


// -------------------------------------------------------------------
// Class  :  "Kite::GCellDensitySet".


  GCellDensitySet::GCellDensitySet ( unsigned int depth )
    : _depth   (depth)
    , _set     ()
    , _requests()
  { }


  GCellDensitySet::GCellDensitySet ( unsigned int depth, const std::vector<GCell*>& gcells )
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
    ltrace(190) << "GCellDensitySet::requeue()" << endl;

    std::set<GCell*,GCell::CompareByKey>::iterator iinserted;
    GCell::SetIndex::iterator                      igcell    = _requests.begin();

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


} // End of Katabatic namespace.
