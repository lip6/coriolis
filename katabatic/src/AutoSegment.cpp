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
// |  C++ Module  :       "./AutoSegment.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/Warning.h"
#include "hurricane/Bug.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "crlcore/RoutingGauge.h"
#include "katabatic/Session.h"
#include "katabatic/AutoContact.h"
#include "katabatic/AutoSegment.h"
#include "katabatic/AutoHorizontal.h"
#include "katabatic/AutoVertical.h"
#include "katabatic/GCell.h"
#include "katabatic/KatabaticEngine.h"


namespace {


  using namespace std;
  using namespace CRL;
  using namespace Hurricane;
  using namespace Katabatic;


  // ---------------------------------------------------------------
  // Local Variables.


  const char* badAutoSegmentAnchor =
    "AutoSegment::create() :\n\n"
    "    Source and/or target anchor is NOT an <AutoContact> (internal error).\n"
    "    Source: %s, Target: %s";

  const char* dupAutoSegmentAnchor =
    "AutoSegment::create() :\n\n"
    "    Source and Target anchor are the same : %s (internal error).";

  const char* badSegment =
    "Katabatic::AutoSegment::create () :\n\n"
    "    Segment between %s and %s\n"
    "    is neither horizontal nor vertical .\n";

  const char* badSegmentSource =
    "Katabatic::AutoSegment::create () :\n\n"
    "    Source anchor of segment %s is not a Contact\n"
    "    (%s)\n";

  const char* badSegmentTarget =
    "Katabatic::AutoSegment::create () :\n\n"
    "    Source anchor of segment %s is not a Contact\n"
    "    (%s)\n";

  const char* mismatchSegmentSource =
    "Katabatic::AutoSegment::create () :\n\n"
    "    Source anchor of segment %s is already an AutoContact\n"
    "    (%s)\n";

  const char* mismatchSegmentTarget =
    "Katabatic::AutoSegment::create () :\n\n"
    "    Target anchor of segment %s is already an AutoContact\n"
    "    (%s)\n";


  // ---------------------------------------------------------------
  // Local Functions.


  bool  getTerminalInterval ( AutoSegment* autoSegment
                            , AutoContact* fromContact
                            , bool         isHorizontal
                            , DbU::Unit&   min
                            , DbU::Unit&   max
                            )
  {
    AutoContact* terminalContact = NULL;

    if ( !fromContact ) {
      bool found = getTerminalInterval ( autoSegment
                                       , autoSegment->getAutoSource()
                                       , autoSegment->isHorizontal()
                                       , min
                                       , max );
      if ( !found )
        found = getTerminalInterval ( autoSegment
                                    , autoSegment->getAutoTarget()
                                    , autoSegment->isHorizontal()
                                    , min
                                    , max );

      //if ( !found )
      //  cerr << "[ERROR] Cannot find terminal of " << autoSegment << "." << endl;

      return found;
    } else {
      if ( autoSegment->isGlobal() ) return false;

      ltrace(88) << "Examining " << autoSegment << " " << fromContact << endl;

      if ( autoSegment->getSource() == autoSegment->getTarget() ) {
        cerr << Error("Source & Target are the same :\n"
                      "        %s\n        %s"
                     ,getString(autoSegment).c_str()
                     ,getString(autoSegment->getSource()).c_str()) << endl;
      }

      terminalContact = autoSegment->getAutoSource();
      if ( terminalContact == fromContact ) {
        terminalContact = autoSegment->getAutoTarget();
      }

      if ( !terminalContact->isTerminal() ) {
        AutoSegment* segment      = NULL;
        size_t       segmentCount = 0;
        forEach ( Component*, icomponent, terminalContact->getSlaveComponents() ) {
          if ( *icomponent == autoSegment->base() ) continue;

          Segment* connex = dynamic_cast<Segment*>(*icomponent);
          if ( !connex ) continue;

          segment = Session::lookup ( connex );
          if ( not segment or not segment->isWeakTerminal() ) continue;

          segmentCount++;
        }
          
        if ( segmentCount == 1 ) {
          return getTerminalInterval ( segment, terminalContact, isHorizontal, min, max );

          return false;
        }
      } else {
        ltrace(88) << "Terminal is " << terminalContact << endl;

        Box constraintBox = terminalContact->getConstraintBox();
        if ( isHorizontal ) {
          min = constraintBox.getXMin ();
          max = constraintBox.getXMax ();
        } else {
          min = constraintBox.getYMin ();
          max = constraintBox.getYMax ();
        }
        return true;
      }
    }

    return false;
  }


  // ---------------------------------------------------------------
  // Class  :  "AttractorsMap".


  class AttractorsMap {
    // Constructor.
    public:
      inline            AttractorsMap      ();
      inline size_t     getAttractorsCount () const;
             DbU::Unit  getLowerMedian     () const;
             DbU::Unit  getUpperMedian     () const;
             void       addAttractor       ( DbU::Unit position );
    protected:
      map<DbU::Unit,size_t>  _attractors;
      size_t                 _attractorsCount;
  };


  inline AttractorsMap::AttractorsMap ()
    : _attractors(), _attractorsCount(0)
  { }


  inline size_t  AttractorsMap::getAttractorsCount () const
  {
    return _attractorsCount;
  }


  void  AttractorsMap::addAttractor ( DbU::Unit position )
  {
    _attractors[position]++;
    _attractorsCount++;

    ltrace(88) << "add Attractor @" << DbU::toLambda(position)
               << " [" << _attractors[position] << "]" << endl;
  }


  DbU::Unit  AttractorsMap::getLowerMedian () const
  {
    size_t  median = (_attractorsCount/2) + (_attractorsCount%2);
    size_t  lower  = 0;

    map<DbU::Unit,size_t>::const_iterator  it = _attractors.begin ();
    for ( ; it != _attractors.end() ; it++ ) {
      lower += it->second;
      if ( lower >= median ) break;
    }

    return it->first;
  }


  DbU::Unit  AttractorsMap::getUpperMedian () const
  {
    size_t  median = _attractorsCount / 2;
    size_t  upper  = 0;

    map<DbU::Unit,size_t>::const_iterator  it = _attractors.begin ();
    for ( ; it != _attractors.end() ; it++ ) {
      upper += it->second;
      if ( upper > median ) break;
    }

    return it->first;
  }


} // End of local namespace.


namespace Katabatic {


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoSegment::CompareByDepthLength".


  bool  AutoSegment::CompareByDepthLength::operator() ( AutoSegment* lhs, AutoSegment* rhs ) const
  {
    int deltaDepth = (int)(Session::getRoutingGauge()->getLayerDepth(lhs->getLayer()))
                   - (int)(Session::getRoutingGauge()->getLayerDepth(rhs->getLayer()));
    if ( deltaDepth < 0 ) return true;  // Lowest layer first.
    if ( deltaDepth > 0 ) return false;

    DbU::Unit deltaUnit = lhs->getSourceU() - rhs->getSourceU();
    if ( deltaUnit < 0 ) return true;  // Smallest source first.
    if ( deltaUnit > 0 ) return false;

    deltaUnit = lhs->getLength() - rhs->getLength();
    if ( deltaUnit > 0 ) return true;  // Longest first.
    if ( deltaUnit < 0 ) return true;

    deltaUnit = lhs->getAxis() - rhs->getAxis();
    if ( deltaUnit < 0 ) return true;  // Smallest axis first.
    if ( deltaUnit > 0 ) return false;

    // if ( lhs->isCanonical   () xor rhs->isCanonical   () ) return lhs->isCanonical();
    // if ( lhs->isCollapsed   () xor rhs->isCollapsed   () ) return rhs->isCollapsed();
    // if ( lhs->isSlackenStrap() xor rhs->isSlackenStrap() ) return lhs->isSlackenStrap();

    // if ( lhs->isGlobal    () xor rhs->isGlobal    () ) return lhs->isGlobal();
    // if ( lhs->isTerminal  () xor rhs->isTerminal  () ) return rhs->isTerminal();
    // if ( lhs->isHorizontal() xor rhs->isHorizontal() ) return lhs->isHorizontal();

    // if ( lhs->isFixed() xor rhs->isFixed() ) return lhs->isFixed();

    return lhs->getId() < rhs->getId();
  }


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoSegment::CompareByDepthAxis".


  bool  AutoSegment::CompareByDepthAxis::operator() ( AutoSegment* lhs, AutoSegment* rhs ) const
  {
    int deltaDepth = (int)(Session::getRoutingGauge()->getLayerDepth(lhs->getLayer()))
                   - (int)(Session::getRoutingGauge()->getLayerDepth(rhs->getLayer()));
    if ( deltaDepth < 0 ) return true;  // Lowest layer first.
    if ( deltaDepth > 0 ) return false;

    DbU::Unit deltaUnit = lhs->getAxis() - rhs->getAxis();
    if ( deltaUnit < 0 ) return true;  // Smallest axis first.
    if ( deltaUnit > 0 ) return false;

    deltaUnit = lhs->getSourceU() - rhs->getSourceU();
    if ( deltaUnit < 0 ) return true;  // Smallest source first.
    if ( deltaUnit > 0 ) return false;

    return lhs->getId() < rhs->getId(); // Smallest Id first.
  }


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoSegment".


  size_t         AutoSegment::_allocateds   = 0;
  size_t         AutoSegment::_globalsCount = 0;
  unsigned long  AutoSegment::_maxId        = 0;


  AutoSegment::AutoSegment ( Segment* segment )
    : _id             (segment->getId())
    , _flags          (SegCreated)
    , _depth          (Session::getLayerDepth(segment->getLayer()))
    , _optimalMin     (0)
    , _sourcePosition (0)
    , _targetPosition (0)
    , _userConstraints(false)
    , _parent         (NULL)
    , _observers      ()
  {
    AutoContact* source = Session::lookup(dynamic_cast<Contact*>(segment->getSource()));
    AutoContact* target = Session::lookup(dynamic_cast<Contact*>(segment->getTarget()));

    _allocateds++;

    if (dynamic_cast<Horizontal*>(segment)) setFlags( SegHorizontal );
    if (source->isTerminal())               setFlags( SegSourceTerminal );
    if (target->isTerminal())               setFlags( SegTargetTerminal );

    _gcell = source->getGCell();
    setOptimalMax ( isHorizontal() ? _gcell->getBoundingBox().getYMax()
                                   : _gcell->getBoundingBox().getXMax() );

    _globalsCount += isGlobal() ? 1 : 0;

    source->invalidate( KbTopology );
  }


  void  AutoSegment::_preCreate ( AutoContact* source, AutoContact* target )
  {
    if ( (source == NULL) or (target == NULL) )
      throw Error( badAutoSegmentAnchor
                 , ((source)?getString(source).c_str():"NULL")
                 , ((target)?getString(target).c_str():"NULL")
                 );

    if (source == target)
      throw Error( dupAutoSegmentAnchor, getString(source).c_str() );
  }


  void  AutoSegment::_postCreate ()
  {
    Session::invalidate( getNet() );
    Session::link( this );
    updateOrient();
    updatePositions();
    invalidate( KbTopology );

    _observers.notify( Create );
  }


  void  AutoSegment::_preDestroy ()
  {
    ltrace(200) << "AutoSegment::_preDestroy() - " << (void*)this << endl;
    ltracein(90);

    _observers.notify( Destroy );

    Session::unlink( this );
    ltraceout(90);
  }


  AutoSegment::~AutoSegment ()
  {
    _allocateds--;
    if ( isGlobal() and (_globalsCount > 0) ) _globalsCount--;
  }


  DbU::Unit  AutoSegment::getX () const
  { return base()->getX(); }


  DbU::Unit  AutoSegment::getY () const
  { return base()->getY(); }


  AutoContact* AutoSegment::getOppositeAnchor ( AutoContact* anchor ) const
  { return Session::lookup(static_cast<Contact*>(getOppositeAnchor(anchor->base()))); }


  Interval& AutoSegment::getOptimal ( Interval& i ) const
  {
    i.getVMin() = getOptimalMin();
    i.getVMax() = getOptimalMax();
    return i;
  }


  bool  AutoSegment::checkNotInvalidated () const
  {
    if (isInvalidated())
      cerr << Error("%s is invalidated.",getString(this).c_str()) << endl;

    return not isInvalidated();
  }


  void  AutoSegment::invalidate ( unsigned int flags )
  {
    if (Session::doDestroyTool()) return;
    if (flags & KbSource) setFlags( SegInvalidatedSource );
    if (flags & KbTarget) setFlags( SegInvalidatedTarget );
    if (isInvalidated()) return;

    ltrace(200) << "AutoSegment::invalidate() " << flags << " " << this << endl;
    ltracein(200);

    _invalidate();

    if ((flags & KbPropagate) and not isNotAligned()) {
      forEach( AutoSegment*, isegment, getAligneds() ) {
        isegment->_invalidate();
      }
    }
    ltraceout(200);
  }


  void  AutoSegment::_invalidate ()
  {
    if (isInvalidated()) return;
    ltrace(110) << "AutoSegment::_invalidate() " << this << endl;

    setFlags( SegInvalidated );
    Session::invalidate( this );

    _observers.notify( Invalidate );
  }


  void  AutoSegment::invalidate ( AutoContact* contact )
  {
    if (Session::doDestroyTool()) return;
    if (contact == getAutoSource()) setFlags( SegInvalidatedSource );
    if (contact == getAutoTarget()) setFlags( SegInvalidatedTarget );
  }


  void  AutoSegment::revalidate ()
  {
    ltrace(200) << "AutoSegment::revalidate() " << this << endl;
    if (not isInvalidated()) return;

    ltracein(200);

    updateOrient   ();
    updatePositions();

    unsigned int oldSpinFlags = _flags & SegDepthSpin;

    if (_flags & (SegInvalidatedSource|SegCreated)) {
      const Layer*  contactLayer = getAutoSource()->getLayer();
      const Layer*  segmentLayer = getLayer();
      ltrace(200) << "Changed source: " << getAutoSource() << endl;

      unsetFlags( SegSourceTop|SegSourceBottom );
      if (contactLayer != segmentLayer) {
        setFlags( (segmentLayer == contactLayer->getTop()) ? SegSourceBottom : SegSourceTop ); 
      }
    }

    if (_flags & (SegInvalidatedTarget|SegCreated)) {
      const Layer*  contactLayer = getAutoTarget()->getLayer();
      const Layer*  segmentLayer = getLayer();
      ltrace(200) << "Changed target: " << getAutoTarget() << endl;

      unsetFlags( SegTargetTop|SegTargetBottom );
      if (contactLayer != segmentLayer) {
        setFlags( (segmentLayer == contactLayer->getTop()) ? SegTargetBottom : SegTargetTop ); 
      }
    }

    unsigned int observerFlags = Revalidate;
    if ( (_flags & SegCreated) or (oldSpinFlags != (_flags & SegDepthSpin)) )
      observerFlags |= RevalidatePPitch;

    unsetFlags( SegInvalidated
              | SegInvalidatedSource
              | SegInvalidatedTarget
              | SegInvalidatedLayer
              | SegCreated
              );

    _observers.notify( observerFlags );

    ltrace(200) << "Updated: " << this << endl;
    ltraceout(200);
  }


  bool  AutoSegment::isStrongTerminal ( unsigned int flags ) const
  {
    if (_flags & SegStrongTerminal) return true;

    if ((flags & KbPropagate) and not isNotAligned()) {
      forEach( AutoSegment*, isegment, const_cast<AutoSegment*>(this)->getAligneds() ) {
        if (isegment->_flags & SegStrongTerminal) return true;
      }
    }
    return false;
  }


  bool  AutoSegment::isSameLayerDogleg () const
  {
    if (not isSpinTopOrBottom()) return false;

    unsigned int perpandicularDepth = getDepth() + (isSpinTop() ? 1 : -1);
    if (perpandicularDepth >= Session::getDepth()) {
      cerr << this << " isSpinTop too high." << endl;
    }
    perpandicularDepth = Session::getDepth() - 1;

    return (getLength() > (Session::getPitch(perpandicularDepth)))
      and  (getLength() < (Session::getPitch(perpandicularDepth) * 3));
  }


  DbU::Unit  AutoSegment::getPPitch () const
  {
    unsigned int depth        = getDepth();
    DbU::Unit    topPPitch    = Session::getPitch( depth + ( ((_flags & SegSpinTop) and (depth+1 < Session::getDepth())) ? 1 : 0) );
    DbU::Unit    bottomPPitch = Session::getPitch( depth - ( ((_flags & SegSpinBottom) and (depth > 0))? 1 : 0) );

    return std::max( topPPitch, bottomPPitch );
  }


  DbU::Unit  AutoSegment::getSlack () const
  {
    DbU::Unit  constraintMin;
    DbU::Unit  constraintMax;

    getConstraints( constraintMin, constraintMax );

    return constraintMax - constraintMin;
  }


  DbU::Unit  AutoSegment::getCost ( DbU::Unit axis ) const
  {
    DbU::Unit  optimal = getOptimalMin();
    if (axis < optimal) return optimal - axis;

    optimal = getOptimalMax();
    if (axis > optimal) return axis - optimal;

    return 0;
  }


  AutoSegment* AutoSegment::getCanonical ( DbU::Unit& min, DbU::Unit& max )
  {
    ltrace(89) << "AutoSegment::getCanonical() - " << this << endl;

    min = getSourcePosition ();
    max = getTargetPosition ();

    if (max < min) swap( min, max );

  //ltrace(89) << "[" << DbU::getValueString(min) << " " << DbU::getValueString(max) << "]" << endl;

    AutoSegment* canonical    = this;
    size_t       canonicals   = isCanonical();
    size_t       aligneds     = 1;
    DbU::Unit    collapsedMin;
    DbU::Unit    collapsedMax;

    if (not isNotAligned()) {
      forEach( AutoSegment*, isegment, getAligneds() ) {
        if (isegment->isCanonical()) {
          canonical = *isegment;
          canonicals++;
        }

        collapsedMin = isegment->getSourcePosition();
        collapsedMax = isegment->getTargetPosition();
        if (collapsedMax < collapsedMin) swap( collapsedMin, collapsedMax );
        if (collapsedMin < min) min = collapsedMin;
        if (collapsedMax > max) max = collapsedMax;
        
        aligneds++;
      }
    //ltrace(89) << "[" << DbU::getValueString(min) << " " << DbU::getValueString(max) << "]" << endl;
      ltrace(89) << "Canonical: " << canonical << endl;

      if ( (canonicals > 1) or ( not canonicals and (aligneds > 2) ) ) {
        cerr << Bug("AutoSegment::getCanonical(): %p:%s"
                    "\n      Bad canonization: %d canonicals out of %d collapseds."
                   , base(), _getString().c_str(), canonicals, aligneds ) << endl;

        int count = 0;
        cerr << "      " << count++ << ": " << this << endl;
        forEach( AutoSegment*, isegment, getAligneds() )
          cerr << "      " << count++ << ": " << *isegment << endl;
      }
    }

    return canonical;
  }


  AutoSegments  AutoSegment::getOnSourceContact ( unsigned int direction )
  {
    return AutoSegments_OnContact
      ( this, getSource() ).getSubSet( AutoSegments_InDirection(direction) );
  }


  AutoSegments  AutoSegment::getOnTargetContact ( unsigned int direction )
  {
    return AutoSegments_OnContact
      ( this, getTarget() ).getSubSet( AutoSegments_InDirection(direction) );
  }


  AutoSegments  AutoSegment::getCachedOnSourceContact ( unsigned int direction )
  { return AutoSegments_CachedOnContact( getAutoSource(), direction ); }


  AutoSegments  AutoSegment::getCachedOnTargetContact ( unsigned int direction )
  { return AutoSegments_CachedOnContact( getAutoTarget(), direction ); }


  AutoSegments  AutoSegment::getAligneds ( unsigned int flags )
  {
    ltrace(89) << "AutoSegment::getAligneds() - flags:" << flags << endl;
    return AutoSegments_Aligneds( this, flags );
  }


  AutoSegments  AutoSegment::getPerpandiculars ()
  { return AutoSegments_Perpandiculars( this ); }


  bool  AutoSegment::checkDepthSpin () const
  {
    bool         valid       = true;
    const Layer* sourceLayer = getAutoSource()->getLayer();
    const Layer* targetLayer = getAutoTarget()->getLayer();

    if (  (_flags & SegSourceTop) and (sourceLayer->getBottom() != getLayer()) ) {
      cerr << Error("%s\n"
                    "        Source is not going above, connected to *top* of %s."
                   , getString(this).c_str()
                   , getString(getAutoSource()).c_str()
                   ) << endl;
      valid = false;
    }
    if (  (_flags & SegSourceBottom) and (sourceLayer->getTop() != getLayer()) ) {
      cerr << Error("%s\n"
                    "        Source is not going below, connected to *bottom* of %s."
                   , getString(this).c_str()
                   , getString(getAutoSource()).c_str()
                   ) << endl;
      valid = false;
    }
    if (  (_flags & SegTargetTop) and (targetLayer->getBottom() != getLayer()) ) {
      cerr << Error("%s\n"
                    "        Target is not going above connected to *top* of %s."
                   , getString(this).c_str()
                   , getString(getAutoTarget()).c_str()
                   ) << endl;
      valid = false;
    }
    if (  (_flags & SegTargetBottom) and (targetLayer->getTop() != getLayer()) ) {
      cerr << Error("%s\n"
                    "        Target is not going below, connected to *bottom* of %s."
                   , getString(this).c_str()
                   , getString(getAutoTarget()).c_str()
                   ) << endl;
      valid = false;
    }

    return valid;
  }


  void  AutoSegment::setFlagsOnAligneds ( unsigned int flags )
  {
    setFlags( flags );
    if (not isNotAligned()) {
      forEach( AutoSegment*, isegment, getAligneds() )
        isegment->setFlags( flags );
    }
  }


  void  AutoSegment::sourceDetach ()
  {
    AutoContact* source = getAutoSource();
    if (source) {
      base()->getSourceHook()->detach();
      source->cacheDetach( this );
      unsetFlags( SegNotSourceAligned );
      setFlags( SegInvalidatedSource );
    }
  }


  void  AutoSegment::targetDetach ()
  {
    AutoContact* target = getAutoTarget();
    if (target) {
      base()->getTargetHook()->detach();
      target->cacheDetach( this );
      unsetFlags( SegNotTargetAligned );
      setFlags( SegInvalidatedTarget );
    }
  }


  void  AutoSegment::sourceAttach ( AutoContact* source )
  {
    if (source) {
      if (not base()->getSourceHook()->isAttached())
        base()->getSourceHook()->attach( source->base()->getBodyHook() );
      source->cacheAttach( this );

      // if      (source->isHTee() and isHorizontal()) return;
      // else if (source->isVTee() and isVertical  ()) return;
      // setFlags( SegNotSourceAligned );
    }
  }


  void  AutoSegment::targetAttach ( AutoContact* target )
  {
    if (target) {
      if (not base()->getTargetHook()->isAttached())
        base()->getTargetHook()->attach( target->base()->getBodyHook() );
      target->cacheAttach( this );

      // if      (target->isHTee() and isHorizontal()) return;
      // else if (target->isVTee() and isVertical  ()) return;
      // setFlags( SegNotTargetAligned );
    }
  }


  void  AutoSegment::mergeUserConstraints ( const Interval& constraints )
  {
    ltrace(200) << "mergeUserConstraints() " << this << endl;
    ltrace(200) << "| " << constraints << " merged with " << _userConstraints << endl;
    _userConstraints.intersection(constraints);
  }


  bool  AutoSegment::toConstraintAxis ( unsigned int flags )
  {
    ltrace(200) << "toConstraintAxis() " << this << endl;
    ltracein(200);

    if (not isCanonical()) { ltraceout(200); return false; }

    DbU::Unit constraintMin;
    DbU::Unit constraintMax;

    getConstraints( constraintMin, constraintMax );

  // Empty constraint interval: ignore.
    if (constraintMin > constraintMax) { ltraceout(200); return false; }

    if (isDogleg()) {
      DbU::Unit halfSideLength = getAutoSource()->getGCell()->getSide
                                   ( isHorizontal() ? KbVertical : KbHorizontal ).getHalfSize();
      constraintMin -= halfSideLength;
      constraintMax += halfSideLength;
    }

    if (getAxis() < constraintMin) {
      setAxis( constraintMin, flags );
      ltraceout(200);
      return true;
    }

    if (getAxis() > constraintMax) {
      setAxis( constraintMax, flags );
      ltraceout(200);
      return true;
    }

    ltraceout(200);
    return false;
  }


  bool  AutoSegment::toOptimalAxis ( unsigned int flags )
  {
    ltrace(200) << "toOptimalAxis() " << this << endl;
    ltracein(200);

    if (not isCanonical()) { ltraceout(200); return false; }

    DbU::Unit constraintMin;
    DbU::Unit constraintMax;

    getConstraints( constraintMin, constraintMax );

    DbU::Unit optimalMin = max( min(getOptimalMin(),constraintMax), constraintMin );
    DbU::Unit optimalMax = min( max(getOptimalMax(),constraintMin), constraintMax );

    if (getAxis() < optimalMin) {
      setAxis( optimalMin, flags );
      ltraceout(200);
      return true;
    }

    if (getAxis() > optimalMax) {
      setAxis( optimalMax, flags );
      ltraceout(200);
      return true;
    }

    if (flags & KbRealignate) setAxis( getAxis(), flags );

    ltraceout(200);
    return false;
  }


  void  AutoSegment::setAxis ( DbU::Unit axis, unsigned int flags )
  {
    if (not isCanonical()) return;

    if ( (axis == getAxis()) and not (flags & KbRealignate) ) return;

    ltrace(200) << "setAxis() @"
                << ((isHorizontal())?"Y ":"X ") << DbU::toLambda(getAxis())
                << " to " << DbU::toLambda(axis) << " on " << this << endl;
    ltracein(80);

    _setAxis( axis );

    if (not isNotAligned()) {
      forEach( AutoSegment*, isegment, getAligneds() ) {
        isegment->_setAxis( getAxis() );
      }
    } else {
      ltrace(200) << "No need to process parallels." << endl;
    }

    ltraceout(80);
  }


  void  AutoSegment::computeTerminal ()
  {
    AutoContact* source = getAutoSource();
    AutoContact* target = getAutoTarget();

    ltrace(99) << "computeTerminal() S:" << source->isTerminal()
               << " T:" << target->isTerminal()
               << " " << this << endl;

    if (source->isTerminal()) {
      unsetFlags( SegWeakTerminal );
      setFlags  ( SegSourceTerminal );
    } else if (target->isTerminal()) {
      unsetFlags( SegWeakTerminal );
      setFlags  ( SegTargetTerminal );
    } else {
      unsigned int terminalFlag = 0;
      switch ( _getFlags() & SegWeakTerminal ) {
        case 0: break;
        case SegSourceTerminal|SegTargetTerminal:
        case SegSourceTerminal:
        case SegTargetTerminal: terminalFlag = SegWeakTerminal1; break;
        case SegWeakTerminal1:  terminalFlag = SegWeakTerminal1; break;
        case SegWeakTerminal2:  terminalFlag = SegWeakTerminal2; break;
        default:
          cerr << Warning("%s has multiple terminal flag sets: %x."
                         ,getString(this).c_str()
                         ,_flags
                         ) << endl;
          terminalFlag = SegWeakTerminal2; break;
      }
      unsetFlags( SegWeakTerminal );
      setFlags  ( terminalFlag );
    }
  }


  void  AutoSegment::computeOptimal ( set<AutoSegment*>& processeds )
  {
    ltrace(89) << "computeOptimal() - " << this << endl;
    ltracein(89);

    DbU::Unit  optimalMin;
    DbU::Unit  optimalMax;
    DbU::Unit  constraintMin;
    DbU::Unit  constraintMax;
  
    getConstraints( constraintMin, constraintMax );

    if (isUserDefined()) {
      optimalMin = optimalMax = getAxis();
    } else {
      DbU::Unit      minGCell    = getOrigin();
      DbU::Unit      maxGCell    = getExtremity();
      DbU::Unit      terminalMin;
      DbU::Unit      terminalMax;
      AttractorsMap  attractors;
  
      AutoContact* anchor = getAutoSource();
      if (anchor->isTerminal()) {
        Box constraintBox = anchor->getConstraintBox();
        if ( isHorizontal() ) {
          terminalMin = constraintBox.getYMin();
          terminalMax = constraintBox.getYMax();
        } else {
          terminalMin = constraintBox.getXMin();
          terminalMax = constraintBox.getXMax();
        }
  
        attractors.addAttractor( terminalMin );
        if (terminalMin != terminalMax)
          attractors.addAttractor( terminalMax );
      }
  
      anchor = getAutoTarget();
      if (anchor->isTerminal()) {
        Box constraintBox = anchor->getConstraintBox();
        if (isHorizontal()) {
          terminalMin = constraintBox.getYMin();
          terminalMax = constraintBox.getYMax();
        } else {
          terminalMin = constraintBox.getXMin();
          terminalMax = constraintBox.getXMax();
        }
  
        attractors.addAttractor( terminalMin );
        if (terminalMin != terminalMax)
          attractors.addAttractor( terminalMax );
      }
  
      forEach( AutoSegment*, autoSegment, getPerpandiculars() ) {
        ltrace(89) << "Perpandicular " << *autoSegment << endl;
        ltracein(89);
        if (autoSegment->isLocal()) {
          if (not autoSegment->isStrongTerminal()) { ltraceout(89); continue; }
  
          DbU::Unit  terminalMin;
          DbU::Unit  terminalMax;
  
          if (getTerminalInterval( *autoSegment
                                 , NULL
                                 , isHorizontal()
                                 , terminalMin
                                 , terminalMax )) {
            attractors.addAttractor( terminalMin );
            if (terminalMin != terminalMax)
              attractors.addAttractor( terminalMax );
          }
        } else {
          bool isMin = true;
          if (    isHorizontal()
             and (autoSegment->getAutoSource()->getGCell()->getRow() == _gcell->getRow()) )
            isMin = false;
          if (    isVertical()
             and (autoSegment->getAutoSource()->getGCell()->getColumn() == _gcell->getColumn()) )
            isMin = false;
          attractors.addAttractor( (isMin) ? minGCell : maxGCell );
        }
        ltraceout(89);
      }
  
      if (attractors.getAttractorsCount()) {
        ltrace(89) << "Lower Median " << DbU::toLambda(attractors.getLowerMedian()) << endl;
        ltrace(89) << "Upper Median " << DbU::toLambda(attractors.getUpperMedian()) << endl;
  
        optimalMin = attractors.getLowerMedian();
        optimalMax = attractors.getUpperMedian();
      } else {
        optimalMin = 0;
        optimalMax = (isHorizontal()) ? _gcell->getBoundingBox().getYMax()
                                      : _gcell->getBoundingBox().getXMax();
      }
    }

    setInBound( constraintMin, constraintMax, optimalMin );
    setInBound( constraintMin, constraintMax, optimalMax );

    ltrace(89) << "Applying constraint on: " << this << endl;
    setOptimalMin( optimalMin );
    setOptimalMax( optimalMax );
    processeds.insert( this );
    if (not isNotAligned()) {
      forEach ( AutoSegment*, autoSegment, getAligneds() ) {
        ltrace(89) << "Applying constraint on: " << *autoSegment << endl;
        autoSegment->setOptimalMin( optimalMin );
        autoSegment->setOptimalMax( optimalMax );
        processeds.insert( (*autoSegment) );
      }
    }

    ltraceout(89);
  }


  AutoSegment* AutoSegment::canonize ( unsigned int flags )
  {
    ltrace(159) << "canonize() - " << this << endl;

    // if (isCanonical() and isGlobal()) {
    //   ltrace(159) << "* " << this << " canonical" << endl;
    //   return this;
    // }

    vector<AutoSegment*>  segments;
    AutoSegment*          canonical    = this;
    bool                  hasCanonical = isCanonical();
    bool                  hasGlobal    = isGlobal();

    if (not isNotAligned()) {
      forEach( AutoSegment*, isegment, getAligneds(flags) ) {
        if (isegment->isFixed()) continue;

        hasGlobal = hasGlobal or isegment->isGlobal();
        segments.push_back( *isegment );

        if (not hasCanonical) {
          if (isegment->isCanonical()) {
            ltrace(159) << "* " << *isegment << " canonical already set" << endl;
            canonical    = *isegment;
            hasCanonical = true;
          }

          if (CompareId()(*isegment,canonical)) canonical = *isegment;
        }
      }

      canonical->setFlags( SegCanonical );
      if (hasGlobal) {
        for ( size_t i=0 ; i<segments.size() ; ++i ) {
          if (not segments[i]->isGlobal())
            segments[i]->setFlags( SegWeakGlobal );
        }
      } else {
        for ( size_t i=0 ; i<segments.size() ; ++i )
          segments[i]->unsetFlags( SegWeakGlobal );
      }
      if (segments.empty()) setFlags( SegNotAligned );

      if (isCanonical()) { ltrace(159) << "* " << this << " canonical" << endl; }
      else {
        ltrace(159) << "* " << this << " not canonical" << endl;
        ltrace(159) << "* " << canonical << " *is* the canonical" << endl;
      }
    } else {
      setFlags  ( SegCanonical  );
      unsetFlags( SegWeakGlobal );
    }

    return canonical;
  }


  size_t  AutoSegment::getAlignedContacts ( map<AutoContact*,int>& innerContacts ) const
  {
    map<AutoContact*,int>::iterator icontact;

    innerContacts.clear();
    innerContacts.insert( make_pair(getAutoSource(),0x1) );
    innerContacts.insert( make_pair(getAutoTarget(),0x4) );

    if (not isNotAligned()) {
      forEach ( AutoSegment*, isegment, const_cast<AutoSegment*>(this)->getAligneds() ) {
        if ( (icontact = innerContacts.find(isegment->getAutoSource())) != innerContacts.end() ) {
          if (icontact->second & 0x1) icontact->second |= 0x2;
          else                        icontact->second |= 0x1;
        } else
          innerContacts.insert( make_pair(getAutoSource(),0x1) );
        
        if ( (icontact = innerContacts.find(isegment->getAutoTarget())) != innerContacts.end() ) {
          if (icontact->second & 0x4) icontact->second |= 0x8;
          else                        icontact->second |= 0x4;
        } else
          innerContacts.insert( make_pair(getAutoTarget(),0x4) );
      }
    }

    return innerContacts.size();
  }


  Interval  AutoSegment::getMinSpanU () const
  {
    map<AutoContact*,int>           contacts;
    map<AutoContact*,int>::iterator icontact;

    getAlignedContacts( contacts );

    DbU::Unit    spanMin   = DbU::Min;
    DbU::Unit    spanMax   = DbU::Max;
    Interval     constraints;
    unsigned int direction = getDirection();

    for ( icontact=contacts.begin() ; icontact != contacts.end() ; icontact++ ) {
      constraints = icontact->first->getUConstraints( direction );
      if (icontact->second == 0x1) {
        spanMin = max( spanMin, constraints.getVMax() );
      }
      if (icontact->second == 0x4) {
        spanMax = min( spanMax, constraints.getVMin() );
      }
    }

    return Interval(spanMin,spanMax);
  }


  size_t  AutoSegment::getPerpandicularsBound ( set<AutoSegment*>& bounds )
  {
    map<AutoContact*,int>           contacts;
    map<AutoContact*,int>::iterator icontact;

    getAlignedContacts( contacts );

    for ( icontact=contacts.begin() ; icontact != contacts.end() ; icontact++ ) {
      if ( (icontact->second == 0x1) or (icontact->second == 0x4) ) {
        forEach ( Segment*, isegment, icontact->first->getSlaveComponents().getSubSet<Segment*>() ) {
          AutoSegment* autoSegment = Session::lookup ( *isegment );
          if (not autoSegment) continue;
          if (autoSegment->getDirection() == getDirection()) continue;

          bounds.insert( autoSegment );
        }
      }
    }

    return bounds.size();
  }


  void  AutoSegment::changeDepth ( unsigned int depth, unsigned int flags )
  {
    ltrace(200) << "changeDepth() " << depth << " - " << this << endl;
    ltracein(200);
    Session::invalidate( getNet() );

    _changeDepth( depth, flags & ~KbPropagate );

    if ((flags & KbPropagate) and not isNotAligned()) {
      forEach ( AutoSegment*, isegment, getAligneds(KbNoCheckLayer) ) {
        (*isegment)->_changeDepth( depth, flags & ~KbPropagate );
      }
    }

    ltraceout(200);
  }


  void  AutoSegment::_changeDepth ( unsigned int depth, unsigned int flags )
  {
    ltrace(200) << "_changeDepth() - " << this << endl;
    ltracein(200);

    invalidate( KbNoFlags );
    setFlags( SegInvalidatedLayer|SegInvalidatedSource|SegInvalidatedTarget );

    const Layer* newLayer = Session::getRoutingGauge()->getRoutingLayer(depth);
    if (getLayer() != newLayer) {
      setLayer( newLayer );

      getAutoSource()->invalidate( KbTopology|KbNoCheckLayer );
      getAutoTarget()->invalidate( KbTopology|KbNoCheckLayer );
    }

    if (not (flags & KbWithNeighbors)) {
      ltraceout(200);
      return;
    }

    forEach ( AutoSegment*, isegment, getCachedOnSourceContact(KbDirectionMask) ) {
      if ((*isegment) == this) continue;
      if ((*isegment)->isGlobal  ()) continue;
      if ((*isegment)->isTerminal()) continue;

      if (not ((*isegment)->isHorizontal() xor isHorizontal()))
        (*isegment)->_changeDepth( depth  , KbNoFlags );
      else
        (*isegment)->_changeDepth( depth-1, KbNoFlags );
    }

    forEach ( AutoSegment*, isegment, getCachedOnTargetContact(KbDirectionMask) ) {
      if ((*isegment) == this) continue;
      if ((*isegment)->isGlobal  ()) continue;
      if ((*isegment)->isTerminal()) continue;

      if (not ((*isegment)->isHorizontal() xor isHorizontal()))
        (*isegment)->_changeDepth( depth  , KbNoFlags );
      else
        (*isegment)->_changeDepth( depth-1, KbNoFlags );
    }

    vector<GCell*> gcells;
    getGCells( gcells );
    for ( size_t i=0 ; i<gcells.size() ; ++i )
      gcells[i]->invalidate();

    ltraceout(200);
  }


  bool  AutoSegment::canSlacken ( unsigned int flags ) const
  {
    ltrace(200) << "AutoSegment::canSlacken()" << endl;

    if (not isGlobal() and not (flags & KbPropagate)) return false;

    if (_canSlacken()) return true;
    if ((flags & KbPropagate) and not isNotAligned()) {
      forEach ( AutoSegment*, isegment, const_cast<AutoSegment*>(this)->getAligneds() ) {
        if (isegment->_canSlacken()) return true;
      }
    }

    return false;
  }


  bool  AutoSegment::slacken ( unsigned int flags )
  {
    bool success = false;

    success = success or _slacken( flags );

    if ((flags & KbPropagate) and not isNotAligned()) {
      forEach ( AutoSegment*, isegment, getAligneds() ) {
        success = success or (*isegment)->_slacken( flags );
      }
    }

    return success;
  }


  float  AutoSegment::getMaxUnderDensity ( unsigned int flags )
  {
    ltrace(200) << "AutoSegment::getMaxUnderDensity() " << endl;

    GCell* begin = NULL;
    GCell* end   = NULL;

    size_t depth = Session::getRoutingGauge()->getLayerDepth(getLayer());

    vector<GCell*> gcells;
    getGCells( gcells );
    begin = *gcells.begin ();
    end   = *gcells.rbegin();

    float maxDensity = 0.0;

    for ( size_t i=0 ; i<gcells.size() ; ++i ) {
      maxDensity = std::max( maxDensity, gcells[i]->getFeedthroughs(depth) );
    }

    if ((flags & KbPropagate) and not isNotAligned()) {
      forEach ( AutoSegment*, isegment, getAligneds() ) {
        isegment->getGCells( gcells );
        if ((*gcells.begin ())->getIndex() < begin->getIndex()) begin = *gcells.begin (); 
        if ((*gcells.rbegin())->getIndex() > end  ->getIndex()) end   = *gcells.rbegin(); 

        for ( size_t i=0 ; i<gcells.size() ; ++i ) {
          maxDensity = std::max( maxDensity, gcells[i]->getFeedthroughs(depth) );
        }
      }
    }

    return maxDensity;
  }


  bool  AutoSegment::canPivotUp ( float reserve, unsigned int flags ) const
  {
    ltrace(200) << "AutoSegment::canPivotUp() - " << flags
                << " (reserve:" << reserve << ")" << endl;

    if ( isLayerChange()    or isFixed() ) return false;
    if ( isStrongTerminal() or isLocal() ) return false;

    size_t depth = Session::getRoutingGauge()->getLayerDepth( getLayer() );
    if (depth+2 >= Session::getRoutingGauge()->getDepth()) return false;

    vector<GCell*> gcells;
    getGCells( gcells );
    for ( size_t i=0 ; i<gcells.size() ; i++ ) {
      if (not gcells[i]->hasFreeTrack(depth+2,reserve)) return false;
    }

    if ( not (flags&KbIgnoreContacts) ) {
      ltrace(200) << getAutoSource() << endl;
      ltrace(200) << getAutoTarget() << endl;
      ltrace(200) << "min depths, Segment:" << depth
                  <<            " S:" << getAutoSource()->getMinDepth()
                  <<            " T:" << getAutoTarget()->getMinDepth() << endl;

      if (getAutoSource()->getMinDepth() < depth) return false;
      if (getAutoTarget()->getMinDepth() < depth) return false;
    }

    if ((flags & KbPropagate) and not isNotAligned()) {
      forEach ( AutoSegment*, isegment, const_cast<AutoSegment*>(this)->getAligneds(flags) ) {
        isegment->getGCells( gcells );
        for ( size_t i=0 ; i<gcells.size() ; i++ ) {
          if (not gcells[i]->hasFreeTrack(depth+2,reserve)) return false;
        }
        if (isegment->getAutoSource()->getMinDepth() < depth) return false;
        if (isegment->getAutoTarget()->getMinDepth() < depth) return false;
      }
    } else {
      ltrace(200) << "AutoSegment::canPivotUp() - true [no propagate]" << endl;
      return true;
    }

    ltrace(200) << "AutoSegment::canPivotUp() - true [propagate]" << endl;

    return true;
  }


  bool  AutoSegment::canPivotDown ( float reserve, unsigned int flags ) const
  {
    ltrace(200) << "AutoSegment::canPivotDown()"
                << " (reserve:" << reserve << ")" << endl;

    if ( isLayerChange()    or isFixed() ) return false;
    if ( isStrongTerminal() or isLocal() ) return false;

    size_t depth = Session::getRoutingGauge()->getLayerDepth( getLayer() );
    if (depth < 3) return false;

    vector<GCell*> gcells;
    getGCells( gcells );
    for ( size_t i=0 ; i<gcells.size() ; i++ ) {
      if (not gcells[i]->hasFreeTrack(depth-2,reserve)) return false;
    }

    ltrace(200) << getAutoSource() << endl;
    ltrace(200) << getAutoTarget() << endl;
    ltrace(200) << "max depths, Segment:" << depth
                <<            " S:" << getAutoSource()->getMaxDepth()
                <<            " T:" << getAutoTarget()->getMaxDepth() << endl;

    if (getAutoSource()->getMaxDepth() > depth) return false;
    if (getAutoTarget()->getMaxDepth() > depth) return false;
    if (not (flags & KbPropagate)) {
      ltrace(200) << "AutoSegment::canPivotDown() - true [no propagate]" << endl;
      return true;
    }

    if ((flags & KbPropagate) and not isNotAligned()) {
      forEach ( AutoSegment*, isegment, const_cast<AutoSegment*>(this)->getAligneds() ) {
        isegment->getGCells( gcells );
        for ( size_t i=0 ; i<gcells.size() ; i++ ) {
          if (not gcells[i]->hasFreeTrack(depth-2,reserve)) return false;
        }
        if (isegment->getAutoSource()->getMaxDepth() < depth) return false;
        if (isegment->getAutoTarget()->getMaxDepth() < depth) return false;
      }
    }

    ltrace(200) << "AutoSegment::canPivotDown() - true [propagate]" << endl;

    return true;
  }


  bool  AutoSegment::canMoveUp ( float reserve, unsigned int flags ) const
  {
    ltrace(200) << "AutoSegment::canMoveUp() " << flags
                << " (reserve:" << reserve << ")" << endl;

    GCell* begin = NULL;
    GCell* end   = NULL;

    if ( isLayerChange() or isFixed() ) return false;
    if ( isStrongTerminal() and (not (flags & KbAllowTerminal)) ) return false;
    if ( isLocal()          and (not (flags & KbAllowLocal   )) ) return false;

    size_t depth = Session::getRoutingGauge()->getLayerDepth(getLayer()) + 2;
    if (depth > Session::getConfiguration()->getAllowedDepth()) return false;

    vector<GCell*> gcells;
    getGCells( gcells );
    begin = *gcells.begin ();
    end   = *gcells.rbegin();

    for ( size_t i=0 ; i<gcells.size() ; i++ ) {
      if (not gcells[i]->hasFreeTrack(depth,reserve)) {
        ltrace(200) << "Not enough free track in " << gcells[i] << endl;
        return false;
      }
    }

    ltrace(200) << "Enough free track under canonical segment." << endl;

    if ( isLocal() and not (flags & KbPropagate) ) {
      if (not getAutoSource()->canMoveUp(this)) return false;
      if (not getAutoTarget()->canMoveUp(this)) return false;
      return true;
    }

  //bool hasGlobalSegment = false;
    if ((flags & KbPropagate) and not isNotAligned()) {
      forEach ( AutoSegment*, isegment, const_cast<AutoSegment*>(this)->getAligneds(flags) ) {
        if (isegment->isFixed ()) return false;
      //if (isegment->isGlobal()) hasGlobalSegment = true;

        isegment->getGCells( gcells );
        if ( (*gcells.begin ())->getIndex() < begin->getIndex() ) begin = *gcells.begin (); 
        if ( (*gcells.rbegin())->getIndex() > end  ->getIndex() ) end   = *gcells.rbegin(); 

        for ( size_t i=0 ; i<gcells.size() ; i++ ) {
          if (not gcells[i]->hasFreeTrack(depth,reserve)) {
            ltrace(200) << "Not enough free track in " << gcells[i] << endl;
            return false;
          }
        }
      }
    }

    if ( (depth >= 4) and (flags & KbWithPerpands) ) {
      float fragmentation = begin->getFragmentation( depth-1 );
      ltrace(200) << "Check begin GCell perpandicular fragmentation: " << fragmentation << endl;

      if (fragmentation < 0.5) {
        ltrace(200) << "Not enough free track for perpandicular in begin GCell "
                    << "(frag:" << fragmentation << ")."
                    << endl;
        return false;
      }

      fragmentation = end->getFragmentation( depth-1 );
      ltrace(200) << "Check end GCell perpandicular fragmentation: " << fragmentation << endl;

      if (fragmentation < 0.5) {
        ltrace(200) << "Not enough free track for perpandicular in end GCell "
                    << "(frag:" << fragmentation << ")."
                    << endl;
        return false;
      }
    }

    return true;
  }


  bool  AutoSegment::moveUp ( unsigned int flags )
  {
  //if ( not canMoveUp(0.0,flags) ) return false;
    changeDepth( Session::getRoutingGauge()->getLayerDepth(getLayer()) + 2, flags&KbPropagate );

    return true;
  }


  bool  AutoSegment::moveDown ( unsigned int flags )
  {
  //if ( not canPivotDown(0.0,flags) ) return false;
    changeDepth( Session::getRoutingGauge()->getLayerDepth(getLayer()) - 2, flags&KbPropagate );

    return true;
  }


#if THIS_IS_DISABLED


  bool  AutoSegment::shearUp ( GCell* upGCell, AutoSegment*& movedUp, float reserve, unsigned int flags )
  {
    ltrace(200) << "AutoSegment::shearUp() " << this << endl;

    movedUp = NULL;

    if ( isLayerChange() or isFixed() /*or isTerminal()*/ or isLocal() ) return false;

    size_t upDepth = Session::getRoutingGauge()->getLayerDepth(getLayer()) + 2;
    if ( upDepth > Session::getConfiguration()->getAllowedDepth() ) return false;

    vector<GCell*> gcells;
    getGCells ( gcells );

    size_t iupGCell = 0;
    for ( ; iupGCell<gcells.size() ; ++iupGCell ) {
      if ( gcells[iupGCell] == upGCell ) break;
    }
    if ( iupGCell == gcells.size() ) {
      cerr << Warning("Shear start %s not under %s."
                     ,getString(upGCell).c_str()
                     ,getString(this).c_str()
                     ) << endl;
      return false;
    }

    GCell* rightShear = NULL;
    for ( size_t i=iupGCell ; i<gcells.size() ; i++ ) {
      if ( not gcells[i]->hasFreeTrack(upDepth,reserve) ) {
        ltrace(200) << "Right shearing @ " << gcells[i] << endl;
        rightShear = gcells[i];
      }
    }

    GCell* leftShear = NULL;
    if ( iupGCell > 0 ) {
      size_t i = iupGCell;
      do {
        --i;
        if ( not gcells[i]->hasFreeTrack(upDepth,reserve) ) {
          ltrace(200) << "Left shearing @ " << gcells[i] << endl;
          leftShear = gcells[i];
        }
      } while (i > 0);
    }

    AutoSegment* before = this;
    const vector<AutoSegment*>& doglegs = Session::getDoglegs();

    if ( leftShear  ) {
      makeDogleg ( leftShear, true );
      movedUp = doglegs[2];
    } else {
      before  = NULL;
      movedUp = this;
    }

    if ( rightShear ) makeDogleg(rightShear,true);

    if ( movedUp->moveUp(flags) ) {
      if ( rightShear or leftShear )
        cinfo << "Shearing Up " << this << "." << endl;
      return true;
    }

    movedUp = NULL;
    return false;
  }
#endif


  unsigned int  AutoSegment::canDogleg ( Interval interval )
  {
    ltrace(200) << "AutoSegment::canDogleg(Interval) " << interval << endl;

    size_t  leftDogleg  = 0;
    size_t  rightDogleg = 0;
    if (getSpanU().contains(interval.getVMin())) leftDogleg++;
    if (getSpanU().contains(interval.getVMax())) rightDogleg++;

    if (not isNotAligned()) {
      forEach ( AutoSegment*, isegment, getAligneds() ) {
        if (isegment->getSpanU().contains(interval.getVMin())) {
          if (isegment->isFixed()) return false;
          leftDogleg++;
        }
        if (isegment->getSpanU().contains(interval.getVMax())) {
          if (isegment->isFixed()) return 0;
          rightDogleg++;
        }
      }
    }

    if ( (leftDogleg == 1) and (rightDogleg <= 1) ) return KbDoglegOnLeft;
    if ( (leftDogleg <= 1) and (rightDogleg == 1) ) return KbDoglegOnRight;

    ltrace(200) << "leftCount:" << leftDogleg << " rightCount:" << rightDogleg << endl;

    return 0;
  }


  AutoSegment* AutoSegment::makeDogleg ( AutoContact* from )
  {
    ltrace(200) << "AutoSegment::makeDogleg(AutoContact*) " << from << endl;
    ltracein(200);
    ltrace(200) << this << endl;

    RoutingGauge*               rg           = Session::getRoutingGauge();
    size_t                      segmentDepth = rg->getLayerDepth( getLayer() );
    const vector<AutoSegment*>& doglegs      = Session::getDoglegs();
    size_t                      index        = doglegs.size();
    bool                        isSource     = (getAutoSource() == from);

    ltrace(200) << "isSource:" << isSource << endl;

    makeDogleg( from->getGCell(), KbNoCheckLayer );
    if (doglegs.size() == index) {
      ltraceout(200);
      return NULL;
    }
    doglegs[ index+1 ]->setAxis( isHorizontal() ? from->getX() : from->getY() );

    if (not from->getLayer()->contains(getLayer())) {
      ltrace(200) << "Contact layer do not contains Segment layer, adjust layers" << endl;

      if (getLayer()->above(from->getLayer())) {
        ltrace(200) << "Go Down from depth " << segmentDepth << endl;

        doglegs[ index + 1 ]->setLayer( rg->getRoutingLayer(segmentDepth-1) );
        ltrace(200) << "doglegs[i+1]: " << doglegs[index+1] << endl;

        if (isSource) {
          doglegs[ index + 0 ]->setLayer( rg->getRoutingLayer(segmentDepth-2) );
          doglegs[ index + 1 ]->getAutoSource()->setLayer( rg->getContactLayer(segmentDepth-2) );
          doglegs[ index + 1 ]->getAutoTarget()->setLayer( rg->getContactLayer(segmentDepth-1) );
          ltrace(200) << "doglegs[i+0]: " << doglegs[index+0] << endl;
          ltrace(200) << "doglegs[i+1]: " << doglegs[index+1]->getAutoSource() << endl;
          ltrace(200) << "doglegs[i+1]: " << doglegs[index+1]->getAutoTarget() << endl;
        } else {
          doglegs[ index + 2 ]->setLayer( rg->getRoutingLayer(segmentDepth-2) );
          doglegs[ index + 1 ]->getAutoTarget()->setLayer( rg->getContactLayer(segmentDepth-2) );
          doglegs[ index + 1 ]->getAutoSource()->setLayer( rg->getContactLayer(segmentDepth-1) );
          ltrace(200) << "doglegs[i+2]: " << doglegs[index+2] << endl;
          ltrace(200) << "doglegs[i+1]: " << doglegs[index+1]->getAutoTarget() << endl;
          ltrace(200) << "doglegs[i+1]: " << doglegs[index+1]->getAutoSource() << endl;
        }
      } else {
        ltrace(200) << "Go Up from depth " << segmentDepth << endl;

        doglegs[ index + 1 ]->setLayer( rg->getRoutingLayer(segmentDepth+1) );
        ltrace(200) << "doglegs[i+1]: " << doglegs[index+1] << endl;

        if (isSource) {
          doglegs[ index + 0 ]->setLayer( rg->getRoutingLayer(segmentDepth+2) );
          doglegs[ index + 1 ]->getAutoSource()->setLayer( rg->getContactLayer(segmentDepth+1) );
          doglegs[ index + 1 ]->getAutoTarget()->setLayer( rg->getContactLayer(segmentDepth  ) );
          ltrace(200) << "doglegs[i+0]: " << doglegs[index+0] << endl;
          ltrace(200) << "doglegs[i+1]: " << doglegs[index+1]->getAutoSource() << endl;
          ltrace(200) << "doglegs[i+1]: " << doglegs[index+1]->getAutoTarget() << endl;
        } else {
          doglegs[ index + 2 ]->setLayer( rg->getRoutingLayer(segmentDepth+2) );
          doglegs[ index + 1 ]->getAutoTarget()->setLayer( rg->getContactLayer(segmentDepth+1) );
          doglegs[ index + 1 ]->getAutoSource()->setLayer( rg->getContactLayer(segmentDepth  ) );
          ltrace(200) << "doglegs[i+2]: " << doglegs[index+2] << endl;
          ltrace(200) << "doglegs[i+1]: " << doglegs[index+1]->getAutoTarget() << endl;
          ltrace(200) << "doglegs[i+1]: " << doglegs[index+1]->getAutoSource() << endl;
        }
      }
    }

    ltraceout(200);
    return doglegs[ index + (isSource?0:2) ];
  }


  unsigned int  AutoSegment::makeDogleg ( Interval interval, unsigned int flags )
  {
    ltrace(200) << "AutoSegment::makeDogleg(Interval) - " << interval << endl;
    ltracein(200);

    bool         leftDogleg       = true;
    unsigned int rflags           = 0;
    size_t       leftDoglegCount  = 0;
    size_t       rightDoglegCount = 0;
    AutoSegment* leftCandidate    = NULL;
    AutoSegment* rightCandidate   = NULL;

    if (getSpanU().contains(interval.getVMin())) { leftCandidate  = this; leftDoglegCount++; }
    if (getSpanU().contains(interval.getVMax())) { rightCandidate = this; rightDoglegCount++; }

    if (not isNotAligned()) {
      forEach ( AutoSegment*, isegment, getAligneds(flags) ) {
        if (isegment->getSpanU().contains(interval.getVMin())) { leftCandidate  = *isegment; leftDoglegCount++; }
        if (isegment->getSpanU().contains(interval.getVMax())) { rightCandidate = *isegment; rightDoglegCount++; }
      }
    }

    if ( (leftDoglegCount != 1) and (rightDoglegCount != 1) )  { ltraceout(200); return 0; }
    if (not leftDoglegCount) {
      leftDogleg     = false;
      leftCandidate  = rightCandidate;
      rightCandidate = NULL;
    }
    
    if (leftCandidate and rightCandidate) {
      ltrace(200) << "Left  Constraint: " << leftCandidate->getSourceConstraints(KbNativeConstraints) << endl;
      ltrace(200) << "Right Constraint: " << rightCandidate->getTargetConstraints(KbNativeConstraints) << endl;

      if ( leftCandidate ->getTargetConstraints(KbNativeConstraints).getSize()
         < rightCandidate->getSourceConstraints(KbNativeConstraints).getSize() ) {
        leftCandidate = rightCandidate;
        leftDogleg    = false;
      }
    } else {
      if (not leftCandidate) {
        leftCandidate = rightCandidate;
        leftDogleg    = false;
      }
    }

    if (leftCandidate) {
      DbU::Unit axis;
    // Ugly: Hard-wired track spacing.
      if (leftDogleg) axis = interval.getVMin() - getPitch();
      else            axis = interval.getVMax() + getPitch();

      ltrace(200) << "Break @" << DbU::getValueString(axis) << " " << leftCandidate << endl;

      unsigned int direction = getDirection();
      GCell*       gcell     = leftCandidate->getAutoSource()->getGCell();
      GCell*       end       = leftCandidate->getAutoTarget()->getGCell();
      while ( gcell != end ) {
        if (gcell->getSide(direction).contains(axis)) break; 
        gcell = (direction == KbHorizontal) ? gcell->getRight() : gcell->getUp();
      }

      ltrace(200) << "In " << gcell << endl;
      rflags = leftCandidate->_makeDogleg( gcell, flags );

      const vector<AutoSegment*>& doglegs = Session::getDoglegs();
      if (doglegs.size() >= 2) {
        ltrace(200) << "AutoSegment::makeDogleg(): @" << DbU::getValueString(axis) << endl;
        doglegs[1]->setAxis( axis );
      }
    }

    ltraceout(200);
    return rflags | (leftDogleg ? KbDoglegOnLeft : KbDoglegOnRight);
  }


  unsigned int  AutoSegment::makeDogleg ( GCell* doglegGCell, unsigned int flags )
  {
    ltrace(500) << "Deter| AutoSegment::makeDogleg(GCell*) " << doglegGCell << endl;
    ltrace(500) << "Deter| in " << this << endl;
    ltracein(160);

    unsigned int  rflags = 0;

    if (    doglegGCell->isUnderIoPad()
       and (Session::getKatabatic()->getState() != EngineGlobalLoaded) ) {
      cerr << Bug( "Attempt to make a dogleg in a GCell under a Pad\n"
                   "      %s\n"
                   "      %s"
                 , getString(this).c_str()
                 , getString(doglegGCell).c_str() ) << endl;
    }

    if (isFixed()) {
      cerr << Error( "AutoSegment::makeDogleg(): Cannot make a dog leg on a fixed segment.\n"
                     "        (on: %s)", _getString().c_str() ) << endl;
      return 0;
    }

    if (doglegGCell->getSide(getDirection()).intersect(getSpanU())) {
      ltrace(159) << "Dogleg in " << this << endl;
      rflags = _makeDogleg( doglegGCell, flags );
    } else {
      ltrace(159) << "Looking in aligneds." << endl;
      if (not isNotAligned()) {
        forEach ( AutoSegment*, aligned, getAligneds(flags) ) {
          ltrace(159) << "| Try in " << *aligned << endl;
          if (doglegGCell->getSide(getDirection()).intersect(aligned->getSpanU())) {
            ltrace(159) << "Dogleg in " << *aligned << endl;
            rflags = aligned->_makeDogleg( doglegGCell, flags );
            ltraceout(160);
            return 0;
          }
        }
      }
      cerr << Bug("Cannot make a dogleg in %s at %s"
                 ,_getString().c_str(), getString(doglegGCell).c_str()) << endl;
    }
    ltraceout(160);

    return rflags;
  }


  bool  AutoSegment::_check () const
  {
    bool coherency = true;

    coherency = checkNotInvalidated() and coherency;
    coherency = checkPositions()      and coherency;
    coherency = checkConstraints()    and coherency;
    coherency = checkDepthSpin()      and coherency;

    return coherency;
  }


  string  AutoSegment::_getStringFlags () const
  {
    string state;
    state += isFixed         () ?" F":" -";
    state += isUnsetAxis     () ? "u": "-";
    state += isStrap         () ? "S": "-";
    state += isCanonical     () ? "C": "-";
    state += isGlobal        () ? "G": "-";
    state += isWeakGlobal    () ? "g": "-";
    state += isStrongTerminal() ? "T": "-";
    state += isWeakTerminal1 () ? "W": "-";
    state += isWeakTerminal2 () ? "w": "-";
    state += isNotAligned    () ? "A": "-";
    state += isSlackened     () ? "S": "-";
    state += isInvalidated   () ? "i": "-";

    if      (_flags & SegSourceTop)    state += 'T';
    else if (_flags & SegSourceBottom) state += 'B';
    else state += '-';
    if      (_flags & SegTargetTop)    state += 'T';
    else if (_flags & SegTargetBottom) state += 'B';
    else state += '-';

    return state;
  }


  string  AutoSegment::_getString () const
  {
    string  s = base()->_getString();
  //s.insert ( 1, "id: " );
  //s.insert ( 4, getString(_id) );
    s.insert ( s.size()-1, _getStringFlags() );
    return s;
  }


  Record* AutoSegment::_getRecord () const
  {
    Record* record = base()->_getRecord ();
    record->add ( getSlot ( "_gcell"         ,  _gcell           ) );
    record->add ( getSlot ( "_id"            , &_id              ) );
    record->add ( getSlot ( "_flags"         , &_flags           ) );
    record->add ( getSlot ( "_userContraints", &_userConstraints ) );
    record->add ( getSlot ( "_sourcePosition", &_sourcePosition  ) );
    record->add ( getSlot ( "_targetPosition", &_targetPosition  ) );
    record->add ( getSlot ( "_parent"        ,  _parent          ) );
    return record;
  }


  AutoSegment* AutoSegment::create ( AutoContact*  source
                                   , AutoContact*  target
                                   , Segment*      hurricaneSegment
                                   )
  {
    static const Layer* horizontalLayer = Session::getRoutingLayer( 1 );
    static DbU::Unit    horizontalWidth = Session::getWireWidth   ( 1 );
    static const Layer* verticalLayer   = Session::getRoutingLayer( 2 );
    static DbU::Unit    verticalWidth   = Session::getWireWidth   ( 2 );

    bool         reattachSource = false;
    bool         reattachTarget = false;
    AutoSegment* segment;
    Horizontal*  horizontal = dynamic_cast<Horizontal*>( hurricaneSegment );
    Vertical*    vertical   = dynamic_cast<Vertical*  >( hurricaneSegment );
    AutoContact* reference = source;

    ltrace(159) << "Source:" << source << endl;
    ltrace(159) << "Target:" << target << endl;

    if (target->isFixed()) {
      if (source->isFixed()) {
        if ( (horizontal) and (source->getY() != target->getY()))
          cerr << Warning( "Straight AutoHorizontal connecting misaligned contacts:\n"
                           "          %s\n"
                           "          %s"
                         , getString(source).c_str()
                         , getString(target).c_str()
                         ) << endl;
        if ( (vertical) and (source->getX() != target->getX()))
          cerr << Warning( "Straight AutoVertical connecting misaligned contacts:\n"
                           "          %s\n"
                           "          %s"
                         , getString(source).c_str()
                         , getString(target).c_str()
                         ) << endl;
      } else
        reference = target;
    }

    Contact*     contact     = dynamic_cast<Contact*>( hurricaneSegment->getSource() );
    AutoContact* autoContact = Session::lookup( contact );
    if (contact == NULL) {
      throw Error( badSegmentSource, getString(hurricaneSegment).c_str() );
      if ( autoContact and (autoContact != source) )
        throw Error( mismatchSegmentSource
                   , getString(hurricaneSegment).c_str()
                   , getString(contact).c_str() );
    } else {
      if (autoContact != source) reattachSource = true;
    }

    contact     = dynamic_cast<Contact*>( hurricaneSegment->getTarget() );
    autoContact = Session::lookup( contact );
    if (contact == NULL) {
      throw Error( badSegmentTarget, getString(hurricaneSegment).c_str() );
      if ( autoContact and (autoContact != target) )
        throw Error ( mismatchSegmentTarget
                    , getString(hurricaneSegment).c_str()
                    , getString(contact).c_str() );
    } else {
      if (autoContact != source) reattachTarget = true;
    }

    if (reattachSource) {
      Hook* hook = hurricaneSegment->getSourceHook();
      hook->detach ();
      hook->attach ( source->getBodyHook() );
    }

    if (reattachTarget) {
      Hook* hook = hurricaneSegment->getTargetHook();
      hook->detach ();
      hook->attach ( target->getBodyHook() );
    }

    if (horizontal) {
      if (horizontal->getLayer() != horizontalLayer) {
        if (Session::getKatabatic()->isGMetal(horizontal->getLayer())) {
          horizontal->setLayer( horizontalLayer );
          horizontal->setWidth( horizontalWidth );
        } else {
          if (horizontal->getWidth() != horizontalWidth) {
            cerr << Warning("Segment %s has non-default width %s."
                           ,getString(horizontal).c_str()
                           ,DbU::getValueString(horizontal->getWidth()).c_str()) << endl;
          }
        }
      }

      horizontal->setY( reference->getY() );
      segment = new AutoHorizontal ( horizontal );
      segment->_postCreate();
    } else if (vertical) {
      if (vertical->getLayer() != verticalLayer) {
        if (Session::getKatabatic()->isGMetal(vertical->getLayer()) )
        vertical->setLayer( verticalLayer );
        vertical->setWidth( verticalWidth );
      } else {
        if (vertical->getWidth() != verticalWidth) {
            cerr << Warning("Segment %s has non-default width %s."
                           ,getString(vertical).c_str()
                           ,DbU::getValueString(vertical->getWidth()).c_str()) << endl;
        }
      }

      vertical->setX( reference->getX() );
      segment = new AutoVertical ( vertical );
      segment->_postCreate();
    } else {
      throw Error( badSegment, getString(source).c_str(), getString(target).c_str() );
    }

    return segment;
  }


  AutoSegment* AutoSegment::create ( AutoContact*  source
                                   , AutoContact*  target
                                   , unsigned int  dir
                                   )
  {
    static const Layer* horizontalLayer = Session::getRoutingLayer( 1 );
    static DbU::Unit    horizontalWidth = Session::getWireWidth   ( 1 );
    static const Layer* verticalLayer   = Session::getRoutingLayer( 2 );
    static DbU::Unit    verticalWidth   = Session::getWireWidth   ( 2 );

    AutoSegment* segment;
    AutoContact* reference = source;

    ltrace(159) << "Source:" << source << endl;
    ltrace(159) << "Target:" << target << endl;

    if (target->isFixed()) {
      if (source->isFixed()) {
        if ( (dir == KbHorizontal) and (source->getY() != target->getY()))
          cerr << Warning( "Straight AutoHorizontal connecting misaligned contacts:\n"
                           "          %s\n"
                           "          %s"
                         , getString(source).c_str()
                         , getString(target).c_str()
                         ) << endl;
        if ( (dir == KbVertical) and (source->getX() != target->getX()))
          cerr << Warning( "Straight AutoVertical connecting misaligned contacts:\n"
                           "          %s\n"
                           "          %s"
                         , getString(source).c_str()
                         , getString(target).c_str()
                         ) << endl;
      } else
        reference = target;
    }

    if (dir & KbHorizontal) {
      segment = create( source
                      , target
                      , Horizontal::create( source->base()
                                          , target->base()
                                          , horizontalLayer
                                          , reference->getY()
                                          , horizontalWidth ) );
    } else if (dir & KbVertical) {
      segment = create( source
                      , target
                      , Vertical::create( source->base()
                                        , target->base()
                                        , verticalLayer
                                        , reference->getX()
                                        , verticalWidth
                                        ) );
    } else
      throw Error( badSegment, getString(source).c_str(), getString(target).c_str() );

    return segment;
  }


  void  AutoSegment::destroy ()
  {
    _preDestroy ();
    delete this;
  }


  bool  AutoSegment::isTopologicalBound ( AutoSegment* seed, unsigned int flags )
  {
    ltrace(80) << "isTopologicalBound() - " << seed << endl;
    ltracein(80);

    set<AutoContact*>     exploreds;
    vector<AutoContact*>  stack;
    DbU::Unit             axis;

    if (flags & KbSuperior) axis = seed->getTargetU();
    else                    axis = seed->getSourceU();

    ltrace(80) << "check for bound " << DbU::getValueString(axis) << endl;

    exploreds.insert( seed->getAutoSource() );
    exploreds.insert( seed->getAutoTarget() );

    if (seed->getLength()) {
      if (flags & KbSuperior) stack.push_back( seed->getAutoTarget() );
      else                    stack.push_back( seed->getAutoSource() );
    } else {
      stack.push_back( seed->getAutoTarget() );
      stack.push_back( seed->getAutoSource() );
    }

    while ( not stack.empty() ) {
      AutoContact* currentContact = stack.back();
      stack.pop_back();

      ltrace(80) << "Exploring: " << (void*)currentContact << " " << currentContact << endl;

      exploreds.insert( currentContact );

      if (currentContact->getAnchor()) { ltraceout(80); return true; }

      forEach ( Component*, component, currentContact->getSlaveComponents() ) {
        Segment* segment = dynamic_cast<Segment*>( *component );
        if (not segment) continue;

        AutoSegment* autoSegment = Session::lookup( segment );
        if (not autoSegment) continue;

        if (not autoSegment->getLength()) {
          AutoContact* contact = autoSegment->getAutoSource();
          if (contact and (contact != currentContact)) {
            if (exploreds.find(contact) == exploreds.end())
              stack.push_back( contact );
          }

          contact = autoSegment->getAutoTarget();
          if (contact and (contact != currentContact)) {
            if (exploreds.find(contact) == exploreds.end())
              stack.push_back( contact );
          }

          continue;
        }

        if (autoSegment->isHorizontal() xor (flags & KbHorizontal)) continue;

        ltrace(80) << "| " << autoSegment << endl;

        if (flags & KbSuperior) {
          if (autoSegment->getTargetU() > axis) { ltraceout(80); return true; }
        } else {
          if (autoSegment->getSourceU() < axis) { ltraceout(80); return true; }
        }
      }
    }

    ltraceout(80);
    return false;
  }


#if THIS_IS_DISABLED
  unsigned int  AutoSegment::getPerpandicularState ( AutoContact* contact
                                                   , AutoSegment* source
                                                   , AutoSegment* current
                                                   , bool         isHorizontalMaster
                                                   , const Layer* masterLayer )
  {
    unsigned int state = 0;

    bool  sourcePerpandicular  = arePerpandiculars ( isHorizontalMaster, source  );
    bool  currentPerpandicular = arePerpandiculars ( isHorizontalMaster, current );
    bool  contactAlignate
      =  (contact->isHAlignate() and  current->isHorizontal() and  isHorizontalMaster)
      or (contact->isVAlignate() and !current->isHorizontal() and !isHorizontalMaster);

    if ( not currentPerpandicular and masterLayer and (masterLayer != current->getLayer()) )
      state |= ParallelAndLayerChange;

    if ( currentPerpandicular and !current->isCollapsed() )
      state |= PerpandicularAny;

    if ( sourcePerpandicular ) {
    // Source segment is perpandicular to master.
      if ( currentPerpandicular and !current->isCollapsed() )
        state |= PerpandicularIndirect;
    } else {
    // Source segment is parallel to master.
      if ( not (currentPerpandicular and current->isCollapsed()) and not contactAlignate ) {
      // Current segment is parallel OR expanded.
        state |= ParallelOrExpanded;
      }
    }

    return state;
  }
#endif


  void  AutoSegment::getTopologicalInfos ( AutoSegment*          seed
                                         , vector<AutoSegment*>& aligneds
                                         , vector<AutoSegment*>& perpandiculars
                                         , DbU::Unit&            leftBound
                                         , DbU::Unit&            rightBound
                                         )
  {
    ltrace(80) << "getTopologicalInfos() - " << seed << endl;
    ltracein(80);

    leftBound  = DbU::Max;
    rightBound = DbU::Min;

    AutoSegmentStack stack;

    stack.push( seed->getAutoSource(), seed );
    stack.push( seed->getAutoTarget(), seed );

    while ( not stack.isEmpty() ) {
      AutoContact* sourceContact = stack.getAutoContact();
      AutoSegment* sourceSegment = stack.getAutoSegment();

      stack.pop();

      DbU::Unit constraint;

      if (seed->isHorizontal()) constraint = sourceContact->getCBXMax();
      else                      constraint = sourceContact->getCBYMax();
      if (constraint < leftBound) leftBound = constraint;

      if (seed->isHorizontal()) constraint = sourceContact->getCBXMin();
      else                      constraint = sourceContact->getCBYMin();
      if (constraint > rightBound) rightBound = constraint;

      ltrace(200) << "Segments of: " << sourceContact << endl;
      LocatorHelper helper (sourceContact, KbHorizontal|KbWithPerpands);
      for ( ; helper.isValid() ; helper.progress() ) {
        AutoSegment* currentSegment = helper.getSegment();
        ltrace(200) << "Looking for: " << currentSegment << endl;
        if (currentSegment == sourceSegment) continue;

        if (AutoSegment::areAlignedsAndDiffLayer(currentSegment,seed)) {
          cerr << Error("Aligned segments not in same layer\n"
                        "        %s\n"
                        "        %s."
                       ,getString(seed).c_str()
                       ,getString(currentSegment).c_str()) << endl;
          continue;
        }

        if (AutoSegment::areAligneds(currentSegment,seed)) {
          aligneds.push_back( currentSegment );

          AutoContact* targetContact  = currentSegment->getOppositeAnchor( sourceContact );
          ltrace(200) << "Target: " << targetContact << endl;
          if (targetContact) {
            if (  (seed->isHorizontal() and sourceContact->isHTee())
               or (seed->isVertical  () and sourceContact->isVTee()) ) {
              ltrace(200) << "Stacking target. " << endl;
              stack.push( targetContact, currentSegment );
            }
          }
        } else {
          ltrace(200) << "| perpandicular " << currentSegment << endl; 
          perpandiculars.push_back( currentSegment );
        }
      }
    }

    ltraceout(80);
  }


  int  AutoSegment::getTerminalCount ( AutoSegment* seed, vector<AutoSegment*>& collapseds )
  {
    ltrace(80) << "getTerminalCount() - " << seed << " (+collapseds)" << endl;

    int  count = 0;
    for ( size_t i=0 ; i < collapseds.size() ; i++ ) {
      if (collapseds[i]->isStrongTerminal())
        count++;
    }
    if (seed->getAutoSource()->isTerminal()) count++;
    if (seed->getAutoTarget()->isTerminal()) count++;

    return count;
  }


} // End of Katabatic namespace.
