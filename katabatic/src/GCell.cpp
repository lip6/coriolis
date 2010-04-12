
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
// |  C++ Module  :       "./GCell.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


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


  GCell::CompareByDensity::CompareByDensity ( size_t depth )
    : _depth(depth)
  { }


  bool  GCell::CompareByDensity::operator() ( GCell* lhs, GCell* rhs )
  {
    float difference = roundfp ( lhs->getDensity(_depth) - rhs->getDensity(_depth) );
    if ( difference != 0.0 ) return (difference > 0.0);

  //int difference = floatCompare ( lhs->getDensity(_depth), rhs->getDensity(_depth) );
  //if ( abs(difference) > 1000 ) return difference > 0;
    
    if ( lhs->getIndex() < rhs->getIndex() ) return true;
    return false;
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
    , _blockages         (new float [_depth])
    , _cDensity          (0.0)
    , _densities         (new float [_depth])
    , _saturateDensities (new float [_depth])
    , _segmentCount      (0)
    , _routedSegmentCount(0)
    , _saturated         (false)
    , _invalid           (true)
    , _key               (0.0,_index)
  {
    for ( size_t i=0 ; i<_depth ; i++ ) {
      _blockages        [i] = 0.0;
      _densities        [i] = 0.0;
      _saturateDensities[i] = 0.0;

      if ( Session::getRoutingGauge()->getLayerGauge(i)->getType() == Constant::PinOnly )
        ++_pinDepth;
    }

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
    if ( !_topRightShrink )
      _topRightShrink = 1 /*DbU::lambda(0.0)*/;

    GCell* gcell = new GCell ( gcellGrid, index, box.inflate(0
                                                            ,0
                                                            ,-_topRightShrink
                                                            ,-_topRightShrink) );

    gcell->_postCreate ();

    return gcell;
  }


  GCell::~GCell ()
  {
    ltrace(90) << "GCell::~GCell()" << endl;

    delete [] _densities;
    delete [] _saturateDensities;

    _allocateds--;
  }


  void  GCell::_preDestroy ()
  {
    ltrace(90) << "GCell::_preDestroy() - " << (void*)this << " " << this << endl;
    ExtensionGo::_preDestroy ();
  }


  bool  GCell::isSaturated ( size_t depth ) const
  { return getDensity(depth) > Session::getSaturateRatio(); }


  bool  GCell::isAboveDensity ( float threshold ) const
  {
    if (_invalid) const_cast<GCell*>(this)->updateDensity();
    float difference = roundfp ( getDensity() - threshold );
    
  //int difference = floatDifference(getDensity(),threshold,10000);
    ltrace(190) << "GCell:isAboveDensity() " << threshold << " diff:" << difference << endl;
    return (difference > 0.0);
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
    if (_invalid) const_cast<GCell*>(this)->updateDensity();

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


  Interval  GCell::getUSide ( unsigned int direction ) const
  {
    Interval side;
    switch ( direction ) {
      default:
      case Constant::Horizontal: side = Interval(_box.getXMin(),_box.getXMax()); break;
      case Constant::Vertical:   side = Interval(_box.getYMin(),_box.getYMax()); break;
    }
    return side;
  }


  AutoSegments  GCell::getHStartSegments ()
  {
    return new AutoSegments_AnchorOnGCell (this,true,Constant::Horizontal);
  }


  AutoSegments  GCell::getVStartSegments ()
  {
    return new AutoSegments_AnchorOnGCell (this,true,Constant::Vertical);
  }


  AutoSegments  GCell::getHStopSegments ()
  {
    return new AutoSegments_AnchorOnGCell (this,false,Constant::Horizontal);
  }


  AutoSegments  GCell::getVStopSegments ()
  {
    return new AutoSegments_AnchorOnGCell (this,false,Constant::Vertical);
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
    return (float)( _box.getHeight() / DbU::lambda(5.0) + 1 );
  }


  float  GCell::getVCapacity () const
  {
    return (float)( _box.getWidth () / DbU::lambda(5.0) + 1 );
  }


  float  GCell::getDensity ( bool update ) const
  {
    if (_invalid and update) const_cast<GCell*>(this)->updateDensity();

    float density = 0.0;
    for ( size_t i=0 ; i<_depth ; i++ )
      density += _densities[i];

#if defined(CHECK_DETERMINISM)
    cerr << "Order: Sum density " << setprecision(9) << density << endl;

    density /= (float)(_depth-_pinDepth);
    cerr << "Order: density " << setprecision(9) << density << endl;

    int intdensity  = (int)roundfp(density);
    cerr << "Order: rounded density *100 " << setprecision(9) << intdensity << endl;

    density  = ((float)(intdensity)) / 100.0;
    cerr << "Order: rounded density /100 " << setprecision(9) << density << endl;

    return density;
#else
    return roundfp ( density/((float)(_depth-_pinDepth)) );
#endif
  }


  float  GCell::getMaxHVDensity ( bool update ) const
  {
    if (_invalid and update) const_cast<GCell*>(this)->updateDensity();

    size_t hplanes  = 0;
    size_t vplanes  = 0;
    float  hdensity = 0.0;
    float  vdensity = 0.0;

    for ( size_t i=0 ; i<_depth ; i++ ) {
      if ( i%2 ) { hdensity += _densities[i]; ++hplanes; }
      else       { vdensity += _densities[i]; ++vplanes; }
    }

    if (hplanes) hdensity /= hplanes;
    if (vplanes) vdensity /= vplanes;

    return (hdensity > vdensity) ? hdensity : vdensity;
  }


  float  GCell::getStiffness () const
  {
    if ( _segmentCount == 0 ) return 0.0;
    return roundfp ( (float)_routedSegmentCount / (float)_segmentCount );
  }


  void  GCell::incSegmentCount ( int count )
  {
    if ( (count < 0) and ((unsigned int)abs(count) > _segmentCount) )
      _segmentCount = 0;
    else
      _segmentCount += count;
  }


  void  GCell::incRoutedCount ( int count )
  {
    if ( (count < 0) and ((unsigned int)abs(count) > _routedSegmentCount) )
      _routedSegmentCount = 0;
    else
      _routedSegmentCount += count;
  }


  void  GCell::addBlockage ( unsigned int depth, float length )
  {
    if ( depth >= _depth ) return;

    _blockages[depth] += length;
    _invalid = true;

  //cerr << "GCell:addBlockage() <id:" << getIndex() << "> "
  //     << depth << ":" << _blockages[depth] << endl;
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
      ltrace(200) << "remove " << (void*)ac->base() << ":" << ac
                  << " from " << this << endl;
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
      cerr << Bug("%p %s do not go through %s."
                 ,(void*)segment
                 ,getString(segment).c_str()
                 ,_getString().c_str()) << endl;
      return;
    }

    if ( _hsegments.size() - end > 1 )
      cerr << Bug("%p %s has multiple occurrences of %s."
                 ,(void*)segment
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
      cerr << Bug("%p %s do not go through %s."
                 ,(void*)segment
                 ,getString(segment).c_str()
                 ,_getString().c_str()) << endl;
      return;
    }

    if ( _vsegments.size() - end > 1 )
      cerr << Bug("%p %s has multiple occurrences of %s."
                 ,(void*)segment
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
    if ( !_invalid ) return (_saturated) ? 1 : 0;

    _saturated = false;

    for ( size_t i=0 ; i<_vsegments.size() ; i++ ) {
      if ( _vsegments[i] == NULL )
        cerr << "NULL Autosegment at index " << i << endl;
    }

    sort ( _hsegments.begin(), _hsegments.end(), AutoSegment::CompareByDepthLength() );
    sort ( _vsegments.begin(), _vsegments.end(), AutoSegment::CompareByDepthLength() );

#if defined(CHECK_DETERMINISM)
    cerr << "Order: Update density " << this << endl;
    for ( size_t i=0 ; i<_hsegments.size() ; i++ ) cerr << "Order: " << _hsegments[i] << endl;
    for ( size_t i=0 ; i<_vsegments.size() ; i++ ) cerr << "Order: " << _vsegments[i] << endl;
#endif

    float     hcapacity    = getHCapacity ();
    float     vcapacity    = getVCapacity ();
    float     ccapacity    = hcapacity * vcapacity * 4; 
    DbU::Unit hpenalty     = 0 /*_box.getWidth () / 3*/;
    DbU::Unit vpenalty     = 0 /*_box.getHeight() / 3*/;
    DbU::Unit uLengths1 [ _depth ];
    float     fLengths1 [ _depth ];
    float     fLengths2 [ _depth ];

    for ( size_t i=0 ; i<_depth ; i++ ) {
      fLengths1[i] = 0.0;
      fLengths2[i] = 0.0;
    }

  // Compute wirelength associated to contacts (in DbU::Unit converted to float).
    set<AutoSegment*> processeds;
    vector<AutoContact*>::iterator  it = _contacts.begin();
    vector<AutoContact*>::iterator end = _contacts.end  ();
    for ( ; it != end ; it++ ) {
      for ( size_t i=0 ; i<_depth ; i++ ) uLengths1[i] = 0;
      (*it)->getLengths ( uLengths1, processeds );
      for ( size_t i=0 ; i<_depth ; i++ ) {
        fLengths1[i] += (float)uLengths1[i];
        switch ( Session::getRoutingGauge()->getLayerDirection(i) ) {
          case Constant::Horizontal: fLengths2[i] += (float)(uLengths1[i]+hpenalty); break;
          case Constant::Vertical:   fLengths2[i] += (float)(uLengths1[i]+vpenalty); break;
        }
      }
    }

  // Transform the wirelength in density (divide by track length, one
  // occupied track count for "one").
    for ( size_t i=0 ; i<_depth ; i++ ) {
      switch ( Session::getRoutingGauge()->getLayerDirection(i) ) {
        case Constant::Horizontal:
          fLengths1[i] /= (float)_box.getWidth ();
          fLengths2[i] /= (float)_box.getWidth ();
          break;
        case Constant::Vertical:
          fLengths1[i] /= (float)_box.getHeight();
          fLengths2[i] /= (float)_box.getHeight();
          break;
      }
    }

  // Add the "pass through" horizontal segments.
    if ( _hsegments.size() ) {
      const Layer* layer = _hsegments[0]->getLayer();
      size_t       depth = Session::getRoutingGauge()->getLayerDepth(layer);
      size_t       count = 0;
      for ( size_t i=0 ; i<_hsegments.size() ; i++ ) {
        if ( layer != _hsegments[i]->getLayer() ) {
          fLengths1[depth] += (float)count;
          fLengths2[depth] += (float)count;

          count = 0;
          layer = _hsegments[i]->getLayer();
          depth = Session::getRoutingGauge()->getLayerDepth(layer);
        }
        count++;
      }
      if ( count ) {
        fLengths1[depth] += (float)count;
        fLengths2[depth] += (float)count;
      }
    }

  // Add the "pass through" vertical segments.
    if ( _vsegments.size() ) {
      const Layer* layer = _vsegments[0]->getLayer();
      size_t       depth = Session::getRoutingGauge()->getLayerDepth(layer);
      size_t       count = 0;
      for ( size_t i=0 ; i<_vsegments.size() ; i++ ) {
        if ( layer != _vsegments[i]->getLayer() ) {
          fLengths1[depth] += (float)count;
          fLengths2[depth] += (float)count;

          count = 0;
          layer = _vsegments[i]->getLayer();
          depth = Session::getRoutingGauge()->getLayerDepth(layer);
        }
        count++;
      }
      if ( count ) {
        fLengths1[depth] += (float)count;
        fLengths2[depth] += (float)count;
      }
    }

  // Add the blockages.
    for ( size_t i=0 ; i<_depth ; i++ ) {
      fLengths1[i] += _blockages[i];
      fLengths2[i] += _blockages[i];
    }

  // Normalize: 0 < d < 1.0 (divide by H/V capacity).
    for ( size_t i=0 ; i<_depth ; i++ ) {
      switch ( Session::getRoutingGauge()->getLayerDirection(i) ) {
        case Constant::Horizontal:
          fLengths1[i] /= hcapacity;
          fLengths2[i] /= hcapacity;
          break;
        case Constant::Vertical:
          fLengths1[i] /= vcapacity;
          fLengths2[i] /= vcapacity;
          break;
      }
      if ( fLengths1[i] > 1.0 ) {
        _saturated = true;
      //fLengths[i] = 1.0;
      }
      _densities        [i] = fLengths1[i];
      _saturateDensities[i] = fLengths2[i];
    }

    _cDensity = ( (float)_contacts.size() ) / ccapacity;
    _invalid  = false;

    for ( size_t i=0 ; i<_depth ; i++ ) {
      _densities        [i] = roundfp ( _densities        [i] );
      _saturateDensities[i] = roundfp ( _saturateDensities[i] );
    }
    _cDensity = roundfp (_cDensity );

    checkDensity ();

#if defined(CHECK_DETERMINISM)
    float gdensity = getDensity();
    cerr << "Order: [" << getIndex() << "] "
         << getVectorString(_densities,_depth)
         << " " << setprecision(9) << gdensity << endl;
#endif

  //_segmentCount = _hsegments.size() + _vsegments.size() + processeds.size();

    return ( _saturated ) ? 1 : 0 ;
  }


  size_t  GCell::checkDensity () const
  {
    if ( _invalid ) const_cast<GCell*>(this)->updateDensity ();

    if ( !Session::getDemoMode() && Session::getWarnGCellOverload() ) {
      for ( size_t i=0 ; i<_depth ; i++ ) {
        if ( _densities[i] > 1.0 ) {
          cerr << Warning("%s @%dx%d overloaded in %s (M2:%.2f M3:%.2f M4:%.2f M5:%.2f)"
                         ,_getString().c_str()
                         ,getColumn()
                         ,getRow()
                         ,getString(Session::getRoutingGauge()->getRoutingLayer(i)->getName()).c_str()
                         ,_densities[1]
                         ,_densities[2]
                         ,_densities[3]
                         ,_densities[4]
                         )
             << endl;
        }
      }
    }

    return ( _saturated ) ? 1 : 0 ;
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

    if ( rpNets.size() < 8 ) return;

    cerr << "[WARNING] " << this << " has " << rps.size() << " terminals h:"
         << _hsegments.size() << endl;

    irp = rps.begin();
    for ( ; irp != rps.end() ; ++irp )
      cerr << "  " << *irp << endl;

    AutoSegment* segment;
    while ( stepDesaturate ( 1, globalNets, segment, true ) ) {
    //cerr << "Moved up: " << segment << endl;
    }

#if 0
    set<RoutingPad*>::iterator ipad = rps.begin();
    for ( ; ipad != rps.end() ; ++ipad ) {
      forEach ( Segment*, isegment, (*ipad)->getSlaveComponents().getSubSet<Segment*>() ) {
      }
    }
#endif
  }


  bool  GCell::hasFreeTrack ( size_t depth ) const
  {
    if (_invalid) const_cast<GCell*>(this)->updateDensity();

#if DEPRECATED
    float  capacity;
    vector<AutoSegment*>::const_iterator isegment;
    vector<AutoSegment*>::const_iterator iend;

    switch ( Session::getRoutingGauge()->getLayerDirection(depth) ) {
      case Constant::Horizontal:
        iend     = _hsegments.end   ();
        isegment = _hsegments.begin ();
        capacity = getHCapacity ();
        break;
      case Constant::Vertical:
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
    switch ( Session::getRoutingGauge()->getLayerDirection(depth) ) {
      case Constant::Horizontal: capacity = getHCapacity(); break;
      case Constant::Vertical:   capacity = getVCapacity(); break;
    }

    ltrace(200) << "| hasFreeTrack [" << getIndex() << "] depth:" << depth << " "
                << Session::getRoutingGauge()->getRoutingLayer(depth)->getName()
                << " " << (_saturateDensities[depth]*capacity) << " vs. " << capacity
                << endl;

    return (_saturateDensities[depth]*capacity + 1.0 <= capacity);
  }


  bool  GCell::stepDesaturate ( unsigned int depth, set<Net*>& globalNets, AutoSegment*& moved, bool force )
  {
#if defined(CHECK_DETERMINISM)
    cerr << "Order: stepDesaturate [" << getIndex() << "] depth:" << depth << endl;
#endif

    updateDensity ();

  //float density   = _densities[depth];
  //float densityUp = _densities[depth+2];

    if ( not force and not isSaturated(depth) ) return false;

    float capacity;
    vector<AutoSegment*>::iterator isegment;
    vector<AutoSegment*>::iterator iend;

    switch ( Session::getRoutingGauge()->getLayerDirection(depth) ) {
      case Constant::Horizontal:
        iend     = _hsegments.end   ();
        isegment = _hsegments.begin ();
        capacity = getHCapacity ();
        break;
      case Constant::Vertical:
        iend     = _vsegments.end   ();
        isegment = _vsegments.begin ();
        capacity = getVCapacity ();
        break;
    }

    for ( ; (isegment != iend) ; isegment++ ) {
      unsigned int segmentDepth = Session::getRoutingGauge()->getLayerDepth((*isegment)->getLayer());

      if ( segmentDepth < depth ) continue;
      if ( segmentDepth > depth ) break;

      globalNets.insert ( (*isegment)->getNet() );
#if defined(CHECK_DETERMINISM)
      cerr << "Order: Move up " << (*isegment) << endl;
#endif
    //cerr << "Move up " << (*isegment) << endl;
      (*isegment)->changeDepth ( depth+2, false, false );
      moved = (*isegment);

      updateDensity ();

      return true;
    }

    return false;
  }


  void  GCell::desaturate ( unsigned int depth, set<Net*>& globalNets )
  {
    updateDensity ();

    float density   = _densities[depth];
    float densityUp = _densities[depth+2];

    if ( density < Session::getSaturateRatio() ) return;

    float capacity;
    vector<AutoSegment*>::iterator isegment;
    vector<AutoSegment*>::iterator iend;

    switch ( Session::getRoutingGauge()->getLayerDirection(depth) ) {
      case Constant::Horizontal:
        iend     = _hsegments.end   ();
        isegment = _hsegments.begin ();
        capacity = getHCapacity ();
        break;
      default:
      case Constant::Vertical:
        iend     = _vsegments.end   ();
        isegment = _vsegments.begin ();
        capacity = getHCapacity ();
        break;
    }

    unsigned int  overload    = (unsigned int)( ( density - Session::getSaturateRatio() ) * capacity );
    unsigned int  displaced   = 0;

    for ( ; (isegment != iend) && (displaced < overload) ; isegment++ ) {
      unsigned int segmentDepth = Session::getRoutingGauge()->getLayerDepth((*isegment)->getLayer());

      if ( segmentDepth < depth ) continue;
      if ( segmentDepth > depth ) break;

      globalNets.insert ( (*isegment)->getNet() );
      (*isegment)->changeDepth ( depth+2, false, false );

      displaced++;
    }

    float desaturated = density - ((float)(displaced)) / capacity;

    cmess2 << "     - GCell [" << getIndex() << "] @" << getColumn() << "x" << getRow()
           << ":" << setprecision(4) << density
           << " -> " << desaturated
           << " [displaced:" << displaced
           << " cap:" << capacity
           << " M+2: " << densityUp
           << " -> " << (densityUp + ((float)(displaced)) / capacity) << "]."
           << endl;
  }


  bool  GCell::checkEdgeSaturation ( float threshold ) const
  {
    unsigned int edgeUpUsage         = 0;
    unsigned int edgeRightUsage      = 0;
    float        edgeUpSaturation    = 0.0;
    float        edgeRightSaturation = 0.0;

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
      edgeUpSaturation = (float)edgeUpUsage/((float)getVCapacity()*2.0);
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
      edgeRightSaturation = (float)edgeRightUsage/((float)getHCapacity()*2.0);
    }

    bool overload = false;
    if ( (edgeUpSaturation > threshold) or (edgeRightSaturation > threshold) ) {
      overload = true;

      cerr << Warning("In %s, (over %.2f) ", _getString().c_str(), threshold);

      ostringstream message;
      message << setprecision(3);
      if ( edgeUpSaturation > threshold )
        message << "up edge: " << edgeUpUsage << "/" << (getVCapacity()*2.0)
                << " " << edgeUpSaturation;
      if ( edgeRightSaturation > threshold ) {
        if ( message.str().size() ) message << " & ";
        message << "right edge: " << edgeRightUsage << "/" << (getVCapacity()*2.0)
                << " " << edgeRightSaturation;
      }

      cerr << message.str() << "." << endl;
    }

    return overload;
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
      << setprecision(9)
#if not defined(CHECK_DETERMINISM)
      << getDensity(false)
#endif
      << " S:" << _routedSegmentCount << "/" << _segmentCount
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
    record->add ( getSlot ( "_saturated"         ,  _saturated          ) );
    record->add ( getSlot ( "_segmentCount"      ,  _segmentCount       ) );
    record->add ( getSlot ( "_routedSegmentCount",  _routedSegmentCount ) );
    record->add ( getSlot ( "_invalid"           ,  _invalid            ) );

    for ( size_t depth=0 ; depth<_depth ; ++depth ) {
      record->add ( getSlot ( "_blockages[]",  &_blockages[depth] ) );
    }

    for ( size_t depth=0 ; depth<_depth ; ++depth ) {
      record->add ( getSlot ( "_densities[]",  &_densities[depth] ) );
    }

    for ( size_t depth=0 ; depth<_depth ; ++depth ) {
      record->add ( getSlot ( "_saturateDensities[]",  &_saturateDensities[depth] ) );
    }
                                     
    return record;
  }


  void  GCell::_xmlWrite ( ostream& o ) const
  {
    char line[1024];

    snprintf ( line, 1024
             , "<GCell index=\"%04i %04i\" corner=\"%06.1f %06.1f\" density=\"%3.2f,%3.2f\">"
             , getColumn()
             , getRow()
             , DbU::getLambda(getX())
             , DbU::getLambda(getY())
             , getDensity()
             , _cDensity
             );

    o << line;
  }


// -------------------------------------------------------------------
// Class  :  "Kite::DyKeyQueue".


  DyKeyQueue::DyKeyQueue ( unsigned int depth )
    : _depth   (depth)
    , _map     ()
    , _requests()
  { }


  DyKeyQueue::DyKeyQueue ( unsigned int depth, const vector<GCell*>& gcells )
    : _depth   (depth)
    , _map     ()
    , _requests()
  {
    for ( size_t i=0 ; i<gcells.size() ; i++ )
      _requests.insert ( gcells[i] );
  }


  DyKeyQueue::~DyKeyQueue ()
  {
    if ( not _requests.empty() ) {
      cerr << Warning("~DyKeyQueue(): Still contains %d requests (and %d elements)."
                     ,_requests.size(),_map.size()) << endl;
    }
  }


  const std::set<GCell*,GCell::CompareByKey>& DyKeyQueue::getGCells () const
  { return _map; }


  void  DyKeyQueue::invalidate ( GCell* gcell )
  { push ( gcell ); }


  void  DyKeyQueue::push ( GCell* gcell )
  { _requests.insert ( gcell ); }


  void  DyKeyQueue::revalidate ()
  {
    std::set<GCell*,GCell::CompareByKey>::iterator iinserted;
    std::set<GCell*>::iterator                     igcell    = _requests.begin();
    for ( ; igcell != _requests.end() ; ++igcell ) {
      iinserted = _map.find(*igcell);
      if ( iinserted != _map.end() ) {
        _map.erase ( iinserted );
      }
      (*igcell)->updateKey (_depth);
      _map.insert ( *igcell );
    }
    _requests.clear ();
  }


  GCell*  DyKeyQueue::pop ()
  {
    if ( _map.empty() ) return NULL;
    std::set<GCell*,GCell::CompareByKey>::iterator igcell = _map.begin();
    GCell* gcell = *igcell;
    _map.erase ( igcell );
    return gcell;
  }


// -------------------------------------------------------------------
// Utilities.


  string  getVectorString ( float* v, size_t size )
  {
    ostringstream s;

    s << setprecision(9);
    for ( size_t i=0 ; i<size ; i++ ) {
      if ( !i ) s << "[";
      else      s << " ";
      s << v[i];
    }
    s << "]";

    return s.str();
  }


} // End of Katabatic namespace.
