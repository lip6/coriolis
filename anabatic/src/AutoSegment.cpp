// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AutoSegment.cpp"                        |
// +-----------------------------------------------------------------+


#include <cmath>
#include "hurricane/DebugSession.h"
#include "hurricane/Warning.h"
#include "hurricane/Bug.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/ViaLayer.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "crlcore/RoutingGauge.h"
#include "anabatic/Session.h"
#include "anabatic/AutoContact.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/AutoHorizontal.h"
#include "anabatic/AutoVertical.h"
#include "anabatic/GCell.h"
#include "anabatic/AnabaticEngine.h"


namespace {

  using namespace std;
  using namespace CRL;
  using namespace Hurricane;
  using namespace Anabatic;


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
    "Anabatic::AutoSegment::create () :\n\n"
    "    Segment between %s and %s\n"
    "    is neither horizontal nor vertical .\n";

  const char* badSegmentSource =
    "Anabatic::AutoSegment::create () :\n\n"
    "    Source anchor of segment %s is not a Contact\n"
    "    (%s)\n";

  const char* badSegmentTarget =
    "Anabatic::AutoSegment::create () :\n\n"
    "    Source anchor of segment %s is not a Contact\n"
    "    (%s)\n";

  const char* mismatchSegmentSource =
    "Anabatic::AutoSegment::create () :\n\n"
    "    Source anchor of segment %s is already an AutoContact\n"
    "    (%s)\n";

  const char* mismatchSegmentTarget =
    "Anabatic::AutoSegment::create () :\n\n"
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

      cdebug_log(145,0) << "Examining " << autoSegment << " " << fromContact << endl;

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
        cdebug_log(145,0) << "Terminal is " << terminalContact << endl;

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

    cdebug_log(145,0) << "add Attractor @" << DbU::getValueString(position)
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


  // ---------------------------------------------------------------
  // Class  :  "SideStack".

  class SideStack {
    public:
                              SideStack      ( Flags direction, DbU::Unit pitch );
      inline       bool       isHoled        () const;
             const Interval&  getSideAt      ( DbU::Unit ) const;
      inline const Interval&  getGSide       () const;
      inline       DbU::Unit  getGSideMin    () const;
      inline       DbU::Unit  getGSideMax    () const;
      inline       DbU::Unit  getGSideCenter () const;
                   void       addGCell       ( const GCell* );
      inline       bool       intersect      ( const Interval& ) const;
      inline       void       restrictGSide  ( const Interval& );
                   void       show           () const;
    private:
      Flags                    _direction;
      bool                     _holed;
      DbU::Unit                _pitch;
      Interval                 _full;
      Interval                 _gside;
      map<DbU::Unit,Interval>  _sides;
  };


  SideStack::SideStack ( Flags direction, DbU::Unit pitch )
    : _direction( (direction & Flags::Horizontal) ? Flags::Vertical : Flags::Horizontal )
    , _holed    (false)
    , _pitch    (pitch)
    , _full     (false)
    , _gside    (false)
    , _sides    ()
  { }


  inline       bool       SideStack::isHoled        () const { return _holed; }
  inline const Interval&  SideStack::getGSide       () const { return _gside; }
  inline       DbU::Unit  SideStack::getGSideMin    () const { return _gside.getVMin(); }
  inline       DbU::Unit  SideStack::getGSideMax    () const { return _gside.getVMax(); }
  inline       DbU::Unit  SideStack::getGSideCenter () const { return _gside.getVMax(); }
  inline       bool       SideStack::intersect      ( const Interval& side ) const { return _gside.intersect(side); }
  inline       void       SideStack::restrictGSide  ( const Interval& restrict ) { if (not _holed) _gside.intersection(restrict); }


  const Interval& SideStack::getSideAt ( DbU::Unit position ) const
  {
    if (_sides.empty()) return _full;
    if (_sides.size() == 1) return _sides.begin()->second;

    if (_sides.begin()->first > position) return _sides.begin()->second;
    for ( auto iside = ++_sides.begin() ; iside != _sides.end() ; ++iside ) {
      if (iside->first >= position) return (--iside)->second;
    }
    return _sides.rbegin()->second;
  }


  void  SideStack::addGCell ( const GCell* gcell )
  {
    Interval  side     = gcell->getSide( _direction, _pitch );
    DbU::Unit position = (_direction & Flags::Vertical) ? gcell->getBoundingBox().getXMin()
                                                        : gcell->getBoundingBox().getYMin();

    if (not _holed and intersect(side)) _gside.intersection( side );
    else {
      if (not _holed) {
        _holed = true;
        if (side.getVMin() > _gside.getVMax()) _gside = Interval( _gside.getVMax(),   side.getVMin() );
        else                                   _gside = Interval(   side.getVMax(), _gside.getVMin() );
      } else {
        if (not intersect(side)) {
          if (side.getVMax() < _gside.getVMin()) _gside.merge( side.getVMax() );
          else                                   _gside.merge( side.getVMin() );
        }
      }
    }

    _sides.insert( make_pair(position,side) );
  }


  void  SideStack::show () const
  {
    cdebug_log(145,0) << "SideStack::show()" << endl;
    for ( auto pside : _sides ) {
      cdebug_log(145,0) << "@ " << DbU::getValueString(pside.first)
                        << " " << pside.second << endl;
    }
  }


} // End of local namespace.


namespace Anabatic {


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoSegment::CompareByDepthLength".


  bool  AutoSegment::CompareByDepthLength::operator() ( AutoSegment* lhs, AutoSegment* rhs ) const
  {
    int deltaDepth = (int)(Session::getRoutingGauge()->getLayerDepth(lhs->getLayer()))
                   - (int)(Session::getRoutingGauge()->getLayerDepth(rhs->getLayer()));
    if ( deltaDepth < 0 ) return true;  // Lowest layer first.
    if ( deltaDepth > 0 ) return false;

    DbU::Unit deltaUnit = lhs->getSourceU() - rhs->getSourceU();
    if ( deltaUnit < 0 ) return true;  // Smallest source first.
    if ( deltaUnit > 0 ) return false;

    deltaUnit = lhs->getAnchoredLength() - rhs->getAnchoredLength();
    if ( deltaUnit > 0 ) return true;  // Longest first.
    if ( deltaUnit < 0 ) return false;

    deltaUnit = lhs->getAxis() - rhs->getAxis();
    if ( deltaUnit < 0 ) return true;  // Smallest axis first.
    if ( deltaUnit > 0 ) return false;

#if THIS_IS_DISABLED
    if ( lhs->isCanonical   () xor rhs->isCanonical   () ) return lhs->isCanonical();
    if ( lhs->isCollapsed   () xor rhs->isCollapsed   () ) return rhs->isCollapsed();
    if ( lhs->isSlackenStrap() xor rhs->isSlackenStrap() ) return lhs->isSlackenStrap();

    if ( lhs->isGlobal    () xor rhs->isGlobal    () ) return lhs->isGlobal();
    if ( lhs->isTerminal  () xor rhs->isTerminal  () ) return rhs->isTerminal();
    if ( lhs->isHorizontal() xor rhs->isHorizontal() ) return lhs->isHorizontal();

    if ( lhs->isFixed() xor rhs->isFixed() ) return lhs->isFixed();
#endif

    return lhs->getId() < rhs->getId();
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoSegment::CompareByDepthAxis".


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
// Class  :  "Anabatic::AutoSegment::CompareBySourceU".


  bool  AutoSegment::CompareBySourceU::operator() ( AutoSegment* lhs, AutoSegment* rhs ) const
  {
    DbU::Unit deltaUnit = lhs->getSourceU() - rhs->getSourceU();
    if (deltaUnit < 0) return true;  // Smallest source first.
    if (deltaUnit > 0) return false;

    return lhs->getId() < rhs->getId(); // Smallest Id first.
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoSegment::CompareByRevalidate".


  bool  AutoSegment::CompareByRevalidate::operator() ( AutoSegment* lhs, AutoSegment* rhs ) const
  {
    if (lhs->isNonPref() xor rhs->isNonPref() ) return lhs->isNonPref();

    DbU::Unit deltaLength = lhs->getAnchoredLength() - rhs->getAnchoredLength();
    if (deltaLength > 0) return true;  // Longest length first.
    if (deltaLength < 0) return false;

    return lhs->getId() < rhs->getId(); // Smallest Id first.
  }


  bool  AutoSegment::CompareByReduceds::operator() ( AutoSegment* lhs, AutoSegment* rhs ) const
  {
    uint32_t deltaReduceds = lhs->getReduceds() - rhs->getReduceds();
    if (deltaReduceds < 0) return true;  // Smallest source first.
    if (deltaReduceds > 0) return false;

    return lhs->getId() < rhs->getId(); // Smallest Id first.
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoSegment".


  size_t                         AutoSegment::_allocateds    = 0;
  size_t                         AutoSegment::_globalsCount  = 0;
  bool                           AutoSegment::_analogMode    = false;
  bool                           AutoSegment::_shortNetMode  = false;
  bool                           AutoSegment::_initialized   = false;
  vector< array<DbU::Unit*,4> >  AutoSegment::_extensionCaps;


  void  AutoSegment::setAnalogMode   ( bool state ) { _analogMode = state; }
  bool  AutoSegment::getAnalogMode   () { return _analogMode; }
  void  AutoSegment::setShortNetMode ( bool state ) { _shortNetMode = state; }


  void  AutoSegment::initialize ()
  {
    // cerr << "AutoSegment::initialize()" << endl;

    _initialized = true;
    DbU::Unit twoGrid = DbU::fromGrid( 2 );
    for ( size_t depth=0 ; depth<Session::getDepth() ; ++depth ) {
      DbU::Unit* viaToTopCap    = new DbU::Unit ( 0 );
      DbU::Unit* viaToBottomCap = new DbU::Unit ( 0 );
      DbU::Unit* viaToSameCap   = new DbU::Unit ( 0 );
      DbU::Unit* minimalLength  = new DbU::Unit ( 0 );
      bool       isVertical     = (depth == 0) or (Session::getLayerGauge(depth)->isVertical());
      uint32_t   flags          = (isVertical) ? Layer::EnclosureV : Layer::EnclosureH ;

      // cerr << depth << ":"   << Session::getLayerGauge(depth)->getLayer()->getName()
      //      << " isVertical:" << Session::getLayerGauge(depth)->isVertical() << endl;
      // cerr << "  minimalSpacing: "
      //      << DbU::getValueString( Session::getLayerGauge(depth)->getLayer()->getMinimalSpacing() ) << endl;

      *viaToSameCap = Session::getPWireWidth(depth)/2;

    // Bottom metal of the VIA going *up*.
      const Layer* viaLayer = dynamic_cast<const ViaLayer*>( Session::getContactLayer(depth) );
      if (viaLayer)
        *viaToTopCap = Session::getViaWidth(depth)/2 + viaLayer->getBottomEnclosure( flags );

    // Top metal of the VIA going *down*.
      if (depth > 0) {
        viaLayer = dynamic_cast<const ViaLayer*>( Session::getContactLayer(depth-1) );
        if (viaLayer)
          *viaToBottomCap = Session::getViaWidth(depth-1)/2 + viaLayer->getTopEnclosure( flags );
      }

      const Layer* routingLayer = Session::getRoutingLayer( depth );
      double minimalArea = routingLayer->getMinimalArea();
      if (minimalArea != 0.0) {
        *minimalLength = DbU::fromMicrons( minimalArea / DbU::toMicrons( Session::getWireWidth(depth) ) );
        DbU::Unit modulo = *minimalLength % twoGrid;
        if (modulo)
          *minimalLength += twoGrid - modulo;
      }

      // cerr << "  viaToTop width:   " << DbU::getValueString( Session::getViaWidth(depth) ) << endl;
      // cerr << "  viaToTopCap:      " << DbU::getValueString(*viaToTopCap   ) << endl;
      // if (depth > 0)                                                                          
      //   cerr << "  viaToBottom width:" << DbU::getValueString( Session::getViaWidth(depth-1)/2 ) << endl;
      // cerr << "  viaToBottomCap:   " << DbU::getValueString(*viaToBottomCap) << endl;
      // cerr << "  viaToSameCap:     " << DbU::getValueString(*viaToSameCap  ) << endl;
 
      _extensionCaps.push_back( std::array<DbU::Unit*,4>( {{ viaToTopCap
                                                           , viaToBottomCap
                                                           , viaToSameCap
                                                           , minimalLength }} ) );
    }
  }


  AutoSegment::AutoSegment ( Segment* segment )
    : _id               (segment->getId())
    , _gcell            (NULL)
    , _flags            (SegCreated)
    , _optimalMin       (0)
    , _optimalMax       (0)
    , _depth            (Session::getLayerDepth(segment->getLayer()))
    , _reduceds         (0)
    , _rpDistance       (15)
    , _breakLevel       (0)
    , _sourcePosition   (0)
    , _targetPosition   (0)
    , _userConstraints  (false)
    , _nativeConstraints(false)
    , _parent           (NULL)
    , _observers        ()
  {
    if (not _initialized) initialize();
    
    _allocateds++;

    if (dynamic_cast<Horizontal*>(segment)) setFlags( SegHorizontal );


    AutoContact* source = Session::lookup(dynamic_cast<Contact*>(segment->getSource()));
    AutoContact* target = Session::lookup(dynamic_cast<Contact*>(segment->getTarget()));
    _globalsCount += (source->getGCell() != target->getGCell()) ? 1 : 0;

    if (source->isTerminal()) setFlags( SegSourceTerminal );
    if (target->isTerminal()) setFlags( SegTargetTerminal );
    if (_analogMode)          setFlags( SegAnalog );
    if (_shortNetMode)        setFlags( SegShortNet );

    source->invalidate( Flags::Topology );
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
    invalidate( Flags::Topology );

    _observers.notify( Create );
  }


  void  AutoSegment::_preDestroy ()
  {
    cdebug_log(149,0) << "AutoSegment::_preDestroy() - " << (void*)this << endl;
    cdebug_tabw(145,1);

    _observers.notify( Destroy );

    AutoContact* contact = getAutoSource();
    if (contact) contact->cacheDetach( this );
    contact = getAutoTarget();
    if (contact) contact->cacheDetach( this );

    Session::unlink( this );
    cdebug_tabw(145,-1);
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


  void  AutoSegment::invalidate ( Flags flags )
  {
    if (Session::doDestroyTool()) return;

    cdebug_log(149,0) << "AutoSegment::invalidate() " << flags.asString(FlagsFunction)
                      << " " << this << endl;
    cdebug_tabw(149,1);

    if (flags & Flags::Source) setFlags( SegInvalidatedSource );
    if (flags & Flags::Target) setFlags( SegInvalidatedTarget );

    if ( (getFlags() & SegSourceTerminal)
       and getAutoSource()
       and getAutoSource()->canDrag()
       and not getAutoSource()->isInvalidated() )
      getAutoSource()->invalidate( flags );

    if ( (getFlags() & SegTargetTerminal)
       and getAutoTarget()
       and getAutoTarget()->canDrag()
       and not getAutoTarget()->isInvalidated() )
      getAutoTarget()->invalidate( flags );

    if (isInvalidated()) { cdebug_tabw(149,-1); return; }

    _invalidate();

    if ((flags & Flags::Propagate) and not isNotAligned()) {
      for ( AutoSegment* segment : getAligneds(flags & Flags::NoCheckLayer) ) {
        if (not segment->isInvalidated()) segment->_invalidate();
      }
    }
    cdebug_tabw(149,-1);
  }


  void  AutoSegment::_invalidate ()
  {
    if (isInvalidated()) return;
    cdebug_log(145,0) << "AutoSegment::_invalidate() " << this << endl;

    setFlags( SegInvalidated );
    Session::invalidate( this );

    _observers.notify( Invalidate );
  }


  void  AutoSegment::invalidate ( AutoContact* contact )
  {
    cdebug_log(149,0) << "AutoSegment::invalidate() " << this << endl;
    cdebug_log(149,0) << " -> " << contact << endl;
    if (Session::doDestroyTool()) return;
    if (contact == getAutoSource()) setFlags( SegInvalidatedSource );
    if (contact == getAutoTarget()) setFlags( SegInvalidatedTarget );
  }


  void  AutoSegment::revalidate ()
  {
    DebugSession::open( getNet(), 159, 160 );
    cdebug_log(149,0) << "AutoSegment::revalidate() " << this << endl;
    if (not isInvalidated()) {
      DebugSession::close();
      return;
    }

    cdebug_tabw(149,1);

    updateOrient();

    uint64_t oldSpinFlags = _flags & SegDepthSpin;

    if (_flags & (SegInvalidatedSource|SegCreated)) {
      AutoContact*  source       = getAutoSource();
      const Layer*  contactLayer = source->getLayer();
      const Layer*  segmentLayer = getLayer();
      cdebug_log(149,0) << "Changed source: " << source << endl;

      unsetFlags( SegSourceTop|SegSourceBottom );
      if (contactLayer->getMask() != segmentLayer->getMask())
        setFlags( (segmentLayer->getMask() == contactLayer->getTop()->getMask()) ? SegSourceBottom : SegSourceTop ); 
      if (source->isTurn() and source->getPerpandicular(this)->isReduced())
        incReduceds();
    }

    if (_flags & (SegInvalidatedTarget|SegCreated)) {
      AutoContact*  target       = getAutoTarget();
      const Layer*  contactLayer = target->getLayer();
      const Layer*  segmentLayer = getLayer();
      cdebug_log(149,0) << "Changed target: " << target << endl;

      unsetFlags( SegTargetTop|SegTargetBottom );
      if (contactLayer->getMask() != segmentLayer->getMask())
        setFlags( (segmentLayer->getMask() == contactLayer->getTop()->getMask()) ? SegTargetBottom : SegTargetTop ); 
      if (target->isTurn() and target->getPerpandicular(this)->isReduced())
        incReduceds();
    }

    Interval oldSpan = Interval( _sourcePosition, _targetPosition );
    if (_flags & SegCreated) oldSpan.makeEmpty();
    expandToMinLength( oldSpan );
    if (_flags & SegAtMinArea) unexpandToMinLength();
    updatePositions();

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

    cdebug_log(149,0) << "Updated: " << this << endl;
    cdebug_tabw(149,-1);
    DebugSession::close();
  }


  bool  AutoSegment::isStrongTerminal ( Flags flags ) const
  {
    if (isTerminal()) return true;

    if ((flags & Flags::Propagate) and not isNotAligned()) {
      for ( AutoSegment* segment : const_cast<AutoSegment*>(this)->getAligneds() ) {
        if (segment->isTerminal()) return true;
      }
    }
    return false;
  }


  DbU::Unit  AutoSegment::getPPitch () const
  {
    unsigned int depth        = getDepth();
    DbU::Unit    topPPitch    = Session::getPitch( depth + ( ((_flags & SegSpinTop   ) and (depth+1 < Session::getDepth())) ? 1 : 0) );
    DbU::Unit    bottomPPitch = Session::getPitch( depth - ( ((_flags & SegSpinBottom) and (depth > 0))? 1 : 0) );

    return std::max( topPPitch, bottomPPitch );
  }


  DbU::Unit  AutoSegment::getExtensionCap ( Flags flags ) const
  {
    size_t     depth = Session::getLayerDepth( getLayer() );
    DbU::Unit  cap   = 0;

    if (flags & Flags::Source) {
      if      (getFlags() & SegSourceTop   ) cap = getViaToTopCap   (depth);
      else if (getFlags() & SegSourceBottom) cap = getViaToBottomCap(depth);
      else                                   cap = getViaToSameCap  (depth);
      // if (getId() == 473829) {
      //   cdebug_log(150,0) << "getExtensionCap(): depth=" << depth
      //                     << " (source) flags:" << getFlags()
      //                     << " VIA cap:" << DbU::getValueString(cap)
      //                     << " t:" << (getFlags() & SegSourceBottom)
      //                     << " b:" << (getFlags() & SegSourceTop)
      //                     << endl;
      // }
      if (not (flags & Flags::NoSegExt)) {
        // cdebug_log(150,0) << "duSource=" << DbU::getValueString(getDuSource()) << endl;
        if (-getDuSource() > cap) {
          cap = -getDuSource();
          // cdebug_log(150,0) << "-> Custom cap (-duSource):" << DbU::getValueString(cap) << endl;
        }
      }
    } else {
      if (flags & Flags::Target) {
        if      (getFlags() & SegTargetTop   ) cap = getViaToTopCap   (depth);
        else if (getFlags() & SegTargetBottom) cap = getViaToBottomCap(depth);
        else                                   cap = getViaToSameCap  (depth);
        // if (getId() == 473829) {
        //   cdebug_log(150,0) << "getExtensionCap(): depth=" << depth
        //                     << " (target) flags:" << getFlags()
        //                     << " VIA cap:" << DbU::getValueString(cap)
        //                     << " t:" << (getFlags() & SegTargetBottom)
        //                     << " b:" << (getFlags() & SegTargetTop)
        //                     << endl;
        // }
        if (not (flags & Flags::NoSegExt)) {
          // cdebug_log(150,0) << "duTarget=" << DbU::getValueString(getDuTarget()) << endl;
          if (getDuTarget() > cap) {
            cap = getDuTarget();
            // cdebug_log(150,0) << "-> Custom cap (+duTarget):" << DbU::getValueString(cap) << endl;
          }
        }
      } else {
      //cap = getViaToBottomCap( depth );
        cap = getViaToSameCap( depth );
        // cdebug_log(150,0) << "getExtensionCap(): (force bottom) flags:" << getFlags()
        //                   << " VIA cap:" << DbU::getValueString(cap)
        //                   << endl;
      }
    }

    // if (   not isCreated()
    //    and not (flags & Flags::NoMinLength)
    //    and     (flags & Flags::Target)
    //    and (getMinimalLength(depth) != 0.0)
    //    and isNearMinArea() ) {
    //   DbU::Unit realLength = getExtensionCap( Flags::Source|Flags::LayerCapOnly|Flags::NoMinLength )
    //                        + getAnchoredLength();
    //   if (realLength + cap < getMinimalLength(depth)) {
    //     cap = getMinimalLength(depth) - realLength;
    //   }
    // }

    if (getLayer()->isSymbolic() and (cap < getWidth()/2)) cap  = getWidth()/2;
    if (not (flags & Flags::LayerCapOnly))                 cap += getLayer()->getMinimalSpacing()/2;
    // cdebug_log(150,0) << "getExtensionCap(): cap=" << DbU::getValueString(cap) << endl;
    return cap;
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
    cdebug_log(145,0) << "AutoSegment::getCanonical() - " << this << endl;

    min = getSourcePosition();
    max = getTargetPosition();

    if (max < min) swap( min, max );

  //cdebug_log(145,0) << "[" << DbU::getValueString(min) << " " << DbU::getValueString(max) << "]" << endl;

    AutoSegment* canonical    = this;
    size_t       canonicals   = isCanonical();
    size_t       aligneds     = 1;
    DbU::Unit    collapsedMin;
    DbU::Unit    collapsedMax;

    if (not isNotAligned()) {
      for ( AutoSegment* segment : getAligneds() ) {
        if (segment->isCanonical()) {
          canonical = segment;
          canonicals++;
        }

        collapsedMin = segment->getSourcePosition();
        collapsedMax = segment->getTargetPosition();
        if (collapsedMax < collapsedMin) swap( collapsedMin, collapsedMax );
        if (collapsedMin < min) min = collapsedMin;
        if (collapsedMax > max) max = collapsedMax;
        
        aligneds++;
      }
    //cdebug_log(145,0) << "[" << DbU::getValueString(min) << " " << DbU::getValueString(max) << "]" << endl;
      cdebug_log(145,0) << "Canonical: " << canonical << endl;

      if ( (canonicals > 1) or ( not canonicals and (aligneds > 2) ) ) {
        cerr << Bug("AutoSegment::getCanonical(): %p:%s"
                    "\n      Bad canonization: %d canonicals out of %d collapseds."
                   , base(), _getString().c_str(), canonicals, aligneds ) << endl;

        int count = 0;
        cerr << "      " << count++ << ": " << this << endl;
        for ( AutoSegment* segment : getAligneds() )
          cerr << "      " << count++ << ": " << segment << endl;
      }
    }

    return canonical;
  }


  void  AutoSegment::getEndAxes ( DbU::Unit& sourceAxis, DbU::Unit& targetAxis ) const
  {
    cdebug_log(145,0) << "AutoSegment::getEndAxes() - " << this << endl;

    sourceAxis = getSourceU();
    targetAxis = getTargetU();

  //if (not isNotAligned()) {
      for( AutoSegment* aligned : const_cast<AutoSegment*>(this)->getAligneds() ) {
        sourceAxis = std::min( sourceAxis, aligned->getSourceU() );
        targetAxis = std::max( targetAxis, aligned->getTargetU() );
      }
  //}
  }


  AutoSegments  AutoSegment::getOnSourceContact ( Flags direction )
  {
    return AutoSegments_OnContact
      ( this, getSource() ).getSubSet( AutoSegments_InDirection(direction) );
  }


  AutoSegments  AutoSegment::getOnTargetContact ( Flags direction )
  {
    return AutoSegments_OnContact
      ( this, getTarget() ).getSubSet( AutoSegments_InDirection(direction) );
  }


  AutoSegments  AutoSegment::getCachedOnSourceContact ( Flags direction )
  { return AutoSegments_CachedOnContact( getAutoSource(), direction ); }


  AutoSegments  AutoSegment::getCachedOnTargetContact ( Flags direction )
  { return AutoSegments_CachedOnContact( getAutoTarget(), direction ); }


  AutoSegments  AutoSegment::getAligneds ( Flags flags )
  { return AutoSegments_Aligneds( this, flags ); }


  AutoSegments  AutoSegment::getConnecteds ( Flags flags )
  { return AutoSegments_Connecteds( this, flags ); }


  AutoSegments  AutoSegment::getPerpandiculars ( Flags flags )
  { return AutoSegments_Perpandiculars( this, flags ); }


  bool  AutoSegment::checkDepthSpin () const
  {
    bool         valid       = true;
    const Layer* sourceLayer = getAutoSource()->getLayer();
    const Layer* targetLayer = getAutoTarget()->getLayer();

    if (   (_flags & SegSourceTop)
       and (sourceLayer->getBottom()->getMask() != getLayer()->getMask()) ) {
      cerr << Error( "%s\n"
                     "        Source is not going above, connected to *top* of %s.\n"
                     "        bottom:%s mask:%s\n" 
                     "        layer:%s mask:%s\n" 
                   , getString(this).c_str()
                   , getString(getAutoSource()).c_str()
                   , getString(sourceLayer->getBottom()).c_str()
                   , getString(sourceLayer->getBottom()->getMask()).c_str()
                   , getString(getLayer()).c_str()
                   , getString(getLayer()->getMask()).c_str()
                   ) << endl;
      valid = false;
    }
    if (   (_flags & SegSourceBottom)
       and (sourceLayer->getTop()->getMask() != getLayer()->getMask()) ) {
      cerr << Error("%s\n"
                    "        Source is not going below, connected to *bottom* of %s."
                   , getString(this).c_str()
                   , getString(getAutoSource()).c_str()
                   ) << endl;
      valid = false;
    }
    if (   (_flags & SegTargetTop)
       and (targetLayer->getBottom()->getMask() != getLayer()->getMask()) ) {
      cerr << Error("%s\n"
                    "        Target is not going above connected to *top* of %s."
                   , getString(this).c_str()
                   , getString(getAutoTarget()).c_str()
                   ) << endl;
      valid = false;
    }
    if (   (_flags & SegTargetBottom)
       and (targetLayer->getTop()->getMask() != getLayer()->getMask()) ) {
      cerr << Error("%s\n"
                    "        Target is not going below, connected to *bottom* of %s."
                   , getString(this).c_str()
                   , getString(getAutoTarget()).c_str()
                   ) << endl;
      valid = false;
    }

    return valid;
  }


  void  AutoSegment::setFlagsOnAligneds ( uint64_t flags )
  {
    setFlags( flags );
    if (not isNotAligned()) {
      for( AutoSegment* segment : getAligneds() )
        segment->setFlags( flags );
    }
  }


  void  AutoSegment::sourceDetach ()
  {
    cdebug_log(149,0) << "AutoSegment::sourceDetach() " << this << endl;
    AutoContact* source = getAutoSource();
    if (source) {
      if (source->isTurn()) {
        AutoSegment* perpandicular = source->getPerpandicular(this);
        if (perpandicular and perpandicular->isReduced())
          decReduceds();
      }
      base()->getSourceHook()->detach();
      source->cacheDetach( this );
      unsetFlags( SegNotSourceAligned );
      setFlags( SegInvalidatedSource );
    }
  }


  void  AutoSegment::targetDetach ()
  {
    cdebug_log(149,0) << "AutoSegment::targetDetach() " << this << endl;
    AutoContact* target = getAutoTarget();
    if (target) {
      if (target->isTurn()) {
        AutoSegment* perpandicular = target->getPerpandicular(this);
        if (perpandicular and perpandicular->isReduced())
          decReduceds();
      }
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
    DebugSession::open( getNet(), 149, 160 );
    cdebug_log(149,0) << "mergeUserConstraints() " << this << endl;
    cdebug_log(149,0) << "| " << constraints << " merged with " << _userConstraints << endl;
    _userConstraints.intersection(constraints);
    DebugSession::close();
  }


  bool  AutoSegment::toConstraintAxis ( Flags flags )
  {
    cdebug_log(149,1) << "toConstraintAxis() " << this << endl;

    if (not isCanonical()) { cdebug_tabw(149,-1); return false; }

    DbU::Unit constraintMin;
    DbU::Unit constraintMax;

    getConstraints( constraintMin, constraintMax );

  // Empty constraint interval: ignore.
    if (constraintMin > constraintMax) { cdebug_tabw(149,-1); return false; }

    if (isDogleg()) {
      DbU::Unit halfSideLength = getAutoSource()->getGCell()->getSide
                                 ( isHorizontal() ? Flags::Vertical : Flags::Horizontal ).getHalfSize();
      constraintMin -= halfSideLength;
      constraintMax += halfSideLength;
    }

    if (getAxis() < constraintMin) {
      setAxis( constraintMin, flags );
      cdebug_tabw(149,-1);
      return true;
    }

    if (getAxis() > constraintMax) {
      setAxis( constraintMax, flags );
      cdebug_tabw(149,-1);
      return true;
    }

    cdebug_tabw(149,-1);
    return false;
  }


  bool  AutoSegment::toOptimalAxis ( Flags flags )
  {
    cdebug_log(149,1) << "toOptimalAxis() " << this << endl;

    if (not isCanonical()) { cdebug_tabw(149,-1); return false; }
    if (not isUnsetAxis()) {
      cdebug_tabw(149,-1);
      return toConstraintAxis( flags );
    }

    DbU::Unit constraintMin;
    DbU::Unit constraintMax;

    getConstraints( constraintMin, constraintMax );

    DbU::Unit optimalMin = max( min(getOptimalMin(),constraintMax), constraintMin );
    DbU::Unit optimalMax = min( max(getOptimalMax(),constraintMin), constraintMax );

    cdebug_log(149,0) << "optimal:[" << DbU::getValueString(optimalMin)
                      << " "         << DbU::getValueString(optimalMax) << "]" << endl;

    if (getAxis() < optimalMin) {
      setAxis( optimalMin, flags );
      cdebug_tabw(149,-1);
      return true;
    }
    
    if (getAxis() > optimalMax) {
      setAxis( optimalMax, flags );
      cdebug_tabw(149,-1);
      return true;
    }

    if (flags & Flags::Realignate) setAxis( getAxis(), flags );

  //setAxis( optimalMin, flags );

    cdebug_tabw(149,-1);
    return false;
  }


  void  AutoSegment::setAxis ( DbU::Unit axis, Flags flags )
  {
    if (not isCanonical() and not (flags & Flags::Force)) return;

    if ( (axis == getAxis()) and not (flags & Flags::Realignate) ) return;

    cdebug_log(159,0) << "setAxis() @"
                << ((isHorizontal())?"Y ":"X ") << DbU::getValueString(getAxis())
                << " to " << DbU::getValueString(axis) << " on " << this << endl;
    cdebug_tabw(145,1);

    _setAxis( axis );

    if (not isNotAligned()) {
      for ( AutoSegment* segment : getAligneds() ) {
        segment->_setAxis( getAxis() );
      }
    } else {
      cdebug_log(149,0) << "No need to process parallels." << endl;
    }

    cdebug_tabw(145,-1);
  }


  void  AutoSegment::computeTerminal ()
  {
    AutoContact* source   = getAutoSource();
    AutoContact* target   = getAutoTarget();

    if (source->isTerminal()) {
      unsetFlags( SegWeakTerminal );
      setFlags  ( SegSourceTerminal );

      if (not target->isTerminal())
        target->setFlags( CntWeakTerminal );
    } else if (target->isTerminal()) {
      unsetFlags( SegWeakTerminal );
      setFlags  ( SegTargetTerminal );

      if (not source->isTerminal())
        source->setFlags( CntWeakTerminal );
    } else {
      uint64_t terminalFlag = 0;
      switch ( _getFlags() & SegWeakTerminal ) {
        case 0: break;
        case SegSourceTerminal|SegTargetTerminal:
        case SegSourceTerminal:
        case SegTargetTerminal: terminalFlag = SegWeakTerminal1; break;
        case SegWeakTerminal1:  terminalFlag = SegWeakTerminal1; break;
        case SegWeakTerminal2:  terminalFlag = SegWeakTerminal2; break;
        default:
          cerr << Warning("%s has multiple terminal flag sets:%s (%x)."
                         ,getString(this).c_str()
                         ,_getStringFlags().c_str()
                         ,_flags
                         ) << endl;
          terminalFlag = SegWeakTerminal2; break;
      }
      unsetFlags( SegWeakTerminal );
      setFlags  ( terminalFlag );
    }

    cdebug_log(145,0) << "computeTerminal() S:" << source->isTerminal()
                      << " T:" << target->isTerminal()
                      << " " << this << endl;
  }


  void  AutoSegment::computeOptimal ( set<AutoSegment*>& processeds )
  {
    cdebug_log(145,1) << "computeOptimal() - " << this << endl;

    DbU::Unit            optimalMin;
    DbU::Unit            optimalMax;
    DbU::Unit            constraintMin;
    DbU::Unit            constraintMax;
    vector<AutoSegment*> aligneds;
    SideStack            sideStack ( (isHorizontal() ? Flags::Horizontal : Flags::Vertical), getPitch() );
  
    getConstraints( constraintMin, constraintMax );
    cdebug_log(145,0) << "Constraints: [" << DbU::getValueString(constraintMin)
                      << " "              << DbU::getValueString(constraintMax) << "]" << endl;

    AutoContact* source = getAutoSource();
    AutoContact* target = getAutoTarget();
    
    if (isLocal() and source->isTurn() and target->isTurn() and not isUserDefined()) {
      AutoSegment* sourcePerpand = source->getPerpandicular(this);
      AutoSegment* targetPerpand = target->getPerpandicular(this);

      sourcePerpand->updateOrient();
      targetPerpand->updateOrient();

      if (not (   (sourcePerpand->getAutoSource() == source)
              xor (targetPerpand->getAutoSource() == target) ) ) {
      // This is a U-Turn.
        cdebug_log(145,0) << "U-Turn special case." << endl;
        DbU::Unit optimal = 0;

        if (sourcePerpand->getAutoSource() == source) {
          optimal = std::min( sourcePerpand->getTargetU(), targetPerpand->getTargetU() );
          optimal = std::min( optimal, constraintMax );
        } else {
          optimal = std::max( sourcePerpand->getSourceU(), targetPerpand->getSourceU() );
          optimal = std::max( optimal, constraintMin );
        }

        cdebug_log(145,0) << "| Source perpandicular: " << sourcePerpand << endl;
        cdebug_log(145,0) << "| Target perpandicular: " << targetPerpand << endl;
        cdebug_log(145,0) << "Applying constraint (U-Turn) on: " << this << endl;
        cdebug_log(145,0) << "optimal: " << DbU::getValueString(optimal) << endl;
        
        setOptimalMin( optimal );
        setOptimalMax( optimal );
        processeds.insert( this );

        cdebug_tabw(145,-1);
        return;
      }
    }

    if (isLocal() and (  (source->isTerminal() and target->isTurn())
                      or (target->isTerminal() and source->isTurn())) ) {
      cdebug_log(145,0) << "Terminal with local turns special case." << endl;
      AutoContact* fromContact   = (source->isTerminal()) ? target : source;
      AutoSegment* perpandicular = fromContact->getPerpandicular( this );
      while ( perpandicular and not perpandicular->isGlobal()) {
        fromContact = perpandicular->getOppositeAnchor( fromContact );
        if (not fromContact->isTurn()) break;
        perpandicular = fromContact->getPerpandicular( perpandicular );
      }
      if (perpandicular and perpandicular->isGlobal() and (perpandicular->getDirection() != getDirection())) {
        cdebug_log(145,0) << "Reached global perpandicular " << perpandicular << endl;
        sideStack.addGCell( getGCell() );
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
        const Interval& side = sideStack.getSideAt( getAxis() );
        if (perpandicular->getSourceU() < side.getVMin()) attractors.addAttractor( sideStack.getGSideMin() );
        if (perpandicular->getTargetU() > side.getVMax()) attractors.addAttractor( sideStack.getGSideMax() );
        optimalMin = attractors.getLowerMedian();
        optimalMax = attractors.getUpperMedian();
        setInBound( constraintMin, constraintMax, optimalMin );
        setInBound( constraintMin, constraintMax, optimalMax );
        setOptimalMin( optimalMin );
        setOptimalMax( optimalMax );
        processeds.insert( this );

        cdebug_log(145,0) << "optimalMin: " << DbU::getValueString(optimalMin) << endl;
        cdebug_log(145,0) << "optimalMax: " << DbU::getValueString(optimalMax) << endl;
        cdebug_tabw(145,-1);
        return;
      }
    }

    if (isUserDefined()) {
      optimalMin = optimalMax = getAxis();
      aligneds.push_back( this );
    } else {
      DbU::Unit      terminalMin;
      DbU::Unit      terminalMax;
      AttractorsMap  attractors;

      Flags flags = (isAnalog() ? Flags::WithDoglegs : Flags::NoFlags);

      getAligneds( Flags::WithSelf|flags ).fill( aligneds );

      if (getGCell()->isMatrix()) {
        sideStack.addGCell( getGCell() );
      } else {
        vector<GCell*> gcells;

        cdebug_log(145,0) << "Using pitch for L/T shrink:" << DbU::getValueString(getPitch()) << endl;
        for ( AutoSegment* aligned : aligneds ) {
          cdebug_log(145,0) << "@ " << aligned << endl;

          aligned->getGCells( gcells );
          for ( GCell* gcell : gcells ) {
            sideStack.addGCell( gcell );
            cdebug_log(145,0) << "| gcellSide:" << sideStack.getGSide() << " (from " << gcell << ")" << endl;
          }
          if (aligned->isStrongTerminal() and not sideStack.isHoled()) {
            cdebug_log(145,0) << "> Is strong terminal, restrict." << aligned << endl;

            Interval terminalConstraints;
            aligned->getConstraints( terminalConstraints );
            sideStack.restrictGSide( terminalConstraints );
            cdebug_log(145,0) << "| " << terminalConstraints.intersection(sideStack.getGSide()) << endl;
            cdebug_log(145,0) << "| gcellSide:" << sideStack.getGSide() << " (from " << aligned << ")" << endl;
          }
        }
      }
      sideStack.show();

      cdebug_log(145,0) << "GCell interval " << sideStack.getGSide() << endl;
  
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
  
      for ( AutoSegment* autoSegment : getPerpandiculars(flags) ) {
        cdebug_log(145,1) << "| Perpandicular " << autoSegment << endl;
        if (autoSegment->isGlobal()) {
          cdebug_log(145,0) << "Used as global." << endl;

          const Interval& side = sideStack.getSideAt( autoSegment->getAxis() );
          cdebug_log(145,0) << "Side @" << DbU::getValueString(autoSegment->getAxis())
                            << " " << side << endl;

          if (autoSegment->getSourceU() < side.getVMin()) attractors.addAttractor( sideStack.getGSideMin() );
          if (autoSegment->getTargetU() > side.getVMax()) attractors.addAttractor( sideStack.getGSideMax() );

        // // Sloppy implentation.
        //   DbU::Unit perpandMin = autoSegment->getSourceU();
        //   DbU::Unit perpandMax = autoSegment->getTargetU();

        //   if (perpandMin < minGCell) attractors.addAttractor( minGCell );
        //   if (perpandMax > maxGCell) attractors.addAttractor( maxGCell );
        } else if (autoSegment->isLocal()) {
          if (autoSegment->isStrongTerminal()) {
            cdebug_log(145,0) << "Used as strong terminal." << endl;

            DbU::Unit  terminalMin;
            DbU::Unit  terminalMax;
  
            if (getTerminalInterval( autoSegment
                                   , NULL
                                   , isHorizontal()
                                   , terminalMin
                                   , terminalMax )) {
              attractors.addAttractor( terminalMin );
              if (terminalMin != terminalMax)
                attractors.addAttractor( terminalMax );
            }
          } else if (autoSegment->isLongLocal() or (autoSegment->getAnchoredLength() > getPPitch()*20)) {
            cdebug_log(145,0) << "Used as long global attractor." << endl;

            DbU::Unit perpandMin = autoSegment->getSourceU();
            DbU::Unit perpandMax = autoSegment->getTargetU();

            if (perpandMin != perpandMax) {
              if (perpandMin == getAxis()) attractors.addAttractor( perpandMax ); 
              if (perpandMax == getAxis()) attractors.addAttractor( perpandMin ); 
            }
          }
        }
        cdebug_tabw(145,-1);
      }
  
      if (sideStack.isHoled()) {
        optimalMin = optimalMax = sideStack.getGSideCenter();
      } else {
        if (attractors.getAttractorsCount()) {
          optimalMin = attractors.getLowerMedian();
          optimalMax = attractors.getUpperMedian();
        } else {
          cdebug_log(145,0) << "No attractors, reverting to GCell bounding box" << endl;
          
          optimalMin = 0;
          optimalMax = (isHorizontal()) ? _gcell->getBoundingBox().getYMax()
                                        : _gcell->getBoundingBox().getXMax();
        }

        setInBound( sideStack.getGSideMin(), sideStack.getGSideMax(), optimalMin );
        setInBound( sideStack.getGSideMin(), sideStack.getGSideMax(), optimalMax );
      }

      cdebug_log(145,0) << "optimalMin: " << DbU::getValueString(optimalMin) << endl;
      cdebug_log(145,0) << "optimalMax: " << DbU::getValueString(optimalMax) << endl;
    }

    setInBound( constraintMin, constraintMax, optimalMin );
    setInBound( constraintMin, constraintMax, optimalMax );

    for ( AutoSegment* aligned : aligneds ) {
      cdebug_log(145,0) << "Applying constraint on: " << aligned << endl;
      aligned->setOptimalMin( optimalMin );
      aligned->setOptimalMax( optimalMax );
      processeds.insert( aligned );
    }

    // cdebug_log(145,0) << "Applying constraint on: " << this << endl;
    // setOptimalMin( optimalMin );
    // setOptimalMax( optimalMax );
    // processeds.insert( this );
    // if (not isNotAligned()) {
    //   for ( AutoSegment* autoSegment : getAligneds() ) {
    //     cdebug_log(145,0) << "Applying constraint on: " << autoSegment << endl;
    //     autoSegment->setOptimalMin( optimalMin );
    //     autoSegment->setOptimalMax( optimalMax );
    //     processeds.insert( autoSegment );
    //   }
    // }

    cdebug_tabw(145,-1);
  }


  AutoSegment* AutoSegment::canonize ( Flags flags )
  {
    cdebug_log(149,0) << "canonize() - " << this << endl;
    if (Session::getAnabatic()->isCanonizeDisabled()) return this;

    // if (isCanonical() and isGlobal()) {
    //   cdebug_log(149,0) << "* " << this << " canonical" << endl;
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
            cdebug_log(149,0) << "* " << *isegment << " canonical already set" << endl;
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

      if (isCanonical()) { cdebug_log(149,0) << "* " << this << " canonical" << endl; }
      else {
        cdebug_log(149,0) << "* " << this << " not canonical" << endl;
        cdebug_log(149,0) << "* " << canonical << " *is* the canonical" << endl;
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

    DbU::Unit spanMin   = DbU::Min;
    DbU::Unit spanMax   = DbU::Max;
    Interval  constraints;
    Flags     direction = getDirection();

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


  bool  AutoSegment::isUTurn () const
  {
    if (isGlobal()) return false;

    AutoContact* source = getAutoSource();
    AutoContact* target = getAutoTarget();

    cerr << "AutoSegment::isUTurn():" << endl;

    if (not source->isTurn() or not target->isTurn()) return false;

    cerr << "  Turn connected" << endl;

    AutoSegment* perpandicular = source->getPerpandicular( this );
    bool onPSourceSource = (perpandicular->getAutoSource() == source);

    perpandicular = target->getPerpandicular( this );
    bool onPTargetSource = (perpandicular->getAutoSource() == target);

    cerr << "  PSource:" << onPSourceSource << " PTarget:" << onPTargetSource << endl;

    return not (onPSourceSource xor onPTargetSource);
  }


  bool  AutoSegment::isNearMinArea () const
  {
    cdebug_log(149,0) << "AutoSegment::isNearMinArea() - " << this << endl;
    if (isNonPref()) return false;
    if (isGlobal()) {
      if (getLength() > getPPitch()) return false;
      cdebug_log(149,0) << "| Considering this global anyway because it is too short. " << endl;
    }

    DbU::Unit sourceAxis = 0;
    DbU::Unit targetAxis = 0;
    getEndAxes( sourceAxis, targetAxis );
    if ((targetAxis - sourceAxis) >= getPPitch()) {
      cdebug_log(149,0) << "| Canonical axis length superior to P-Pitch " << this << endl;
      return false;
    }
    cdebug_log(149,0) << "  Length below P-Pitch." << endl;
    return true;
  }


  void  AutoSegment::expandToMinLength ( Interval span )
  {
    if (not isNearMinArea()) return;
    DebugSession::open( getNet(), 149, 160 );
    cdebug_log(149,1) << "AutoSegment::expandToMinLength() " << this << endl;
    cdebug_log(149,0) << "In span=" << span << endl;
    cdebug_log(149,0) << "Before: [" << DbU::getValueString(getSourceU() - getExtensionCap( Flags::Source|Flags::LayerCapOnly ))
                      << " "         << DbU::getValueString(getTargetU() + getExtensionCap( Flags::Target|Flags::LayerCapOnly ))
                      << "]" << endl;

    DbU::Unit halfMinSpacing = getLayer()->getMinimalSpacing() / 2;
    DbU::Unit sourceCap      = getExtensionCap( Flags::Source|Flags::LayerCapOnly );
    DbU::Unit targetCap      = getExtensionCap( Flags::Target|Flags::LayerCapOnly );
    DbU::Unit segMinLength   = getAnchoredLength() + sourceCap + targetCap;
    DbU::Unit techMinLength  = getMinimalLength( Session::getLayerDepth( getLayer() ));
    cdebug_log(149,0) << "Minimal length "      << DbU::getValueString(techMinLength)
                      << " vs. current length " << DbU::getValueString(segMinLength) << endl;
    if (segMinLength >= techMinLength) {
      if (segMinLength == techMinLength)
        setFlags( SegAtMinArea );
      cdebug_tabw(149,-1);
      DebugSession::close();
      return;
    }
    sourceCap    = getExtensionCap( Flags::Source|Flags::NoSegExt|Flags::LayerCapOnly );
    targetCap    = getExtensionCap( Flags::Target|Flags::NoSegExt|Flags::LayerCapOnly );
    segMinLength = getAnchoredLength() + sourceCap + targetCap;
    
    DbU::Unit oneGrid      = DbU::fromGrid( 1 );
    DbU::Unit targetExpand =   (techMinLength - segMinLength) / 2 + targetCap;
    DbU::Unit sourceExpand = - (techMinLength - segMinLength) / 2 - sourceCap;
    if (targetExpand % oneGrid)
      targetExpand += oneGrid - targetExpand % oneGrid;
    if (sourceExpand % oneGrid)
      sourceExpand -= oneGrid + sourceExpand % oneGrid;
    if (not span.isEmpty()) {
      DbU::Unit shiftLeft = span.getVMax() - (getTargetU() + targetExpand + halfMinSpacing);
      if (shiftLeft < 0) {
        if (targetExpand + shiftLeft < targetCap)
          shiftLeft = targetCap - targetExpand;
        targetExpand += shiftLeft;
        sourceExpand += shiftLeft;
      }
      DbU::Unit shiftRight = span.getVMin() - (getSourceU() + sourceExpand - halfMinSpacing);
      if (shiftRight > 0) {
        if (sourceExpand + shiftRight < sourceCap)
          shiftRight = - sourceExpand - sourceCap;
        targetExpand += shiftRight;
        sourceExpand += shiftRight;
      }
    }
    setDuSource( sourceExpand );
    setDuTarget( targetExpand );
    cdebug_log(149,0) <<  "sourceExpand=" << DbU::getValueString(sourceExpand)
                      << " targetExpand=" << DbU::getValueString(targetExpand) << endl;
    cdebug_log(149,0) << "After: [" << DbU::getValueString(getSourceU() - getExtensionCap( Flags::Source|Flags::LayerCapOnly ))
                      << " "        << DbU::getValueString(getTargetU() + getExtensionCap( Flags::Target|Flags::LayerCapOnly ))
                      << "] expand:" << DbU::getValueString(techMinLength - segMinLength)<< endl;
    setFlags( SegAtMinArea );
    cdebug_tabw(149,-1);
    DebugSession::close();
  }


  void  AutoSegment::unexpandToMinLength ()
  {
    cdebug_log(149,0) << "AutoSegment::unexpandToMinLength() " << this << endl;
  // Note: sourceU is a negative number.
  //       targetU is a positive number.
  //       But *both* "cap" are positives.
    DbU::Unit duSource      = getDuSource();
    DbU::Unit duTarget      = getDuTarget();
    DbU::Unit sourceCap     = getExtensionCap( Flags::Source|Flags::NoSegExt|Flags::LayerCapOnly );
    DbU::Unit targetCap     = getExtensionCap( Flags::Target|Flags::NoSegExt|Flags::LayerCapOnly );
    DbU::Unit segLength     = getTargetU() - getSourceU();
    DbU::Unit segMinLength  = getAnchoredLength() + sourceCap + targetCap;
    DbU::Unit techMinLength = getMinimalLength( Session::getLayerDepth( getLayer() ));

    cdebug_log(149,0) << "* Anchored length " << DbU::getValueString(getAnchoredLength()) << endl;
    cdebug_log(149,0) << "* Source cap " << DbU::getValueString(sourceCap) << endl;
    cdebug_log(149,0) << "* Target cap " << DbU::getValueString(targetCap) << endl;
    cdebug_log(149,0) << "* duSource " << DbU::getValueString(duSource) << endl;
    cdebug_log(149,0) << "* duTarget " << DbU::getValueString(duTarget) << endl;

    if ((duSource == 0) and (duTarget == 0)) {
      cdebug_log(149,0) << "Already reset!" << endl;
      return;
    }

    if (segLength <= techMinLength) {
      cdebug_log(149,0) << "Still at min area, do nothing." << endl;
      return;
    }

    if (segMinLength > techMinLength) {
      cdebug_log(149,0) << "Complete reset." << endl;
      setDuSource( 0 );
      setDuTarget( 0 );
      unsetFlags( SegAtMinArea );
      return;
    }

    DbU::Unit shrink = (getAnchoredLength() + duTarget - duSource) - techMinLength;
    if (shrink < 0) {
      cerr << Warning( "AutoSegment::unexpandToMinLength(): Negative shrink %s, but forbidden to expand.\n"
                       "          On %s"
                     , DbU::getValueString(shrink).c_str()
                     , getString(this).c_str()
                     ) << endl;
      return;
    }
    
    DbU::Unit margin = -duSource - sourceCap;
    if (shrink <= margin) {
      cdebug_log(149,0) << "Shrink source of " << DbU::getValueString(shrink) << endl;
      duSource += shrink;
      setDuSource( duSource );
      return;
    }

    setDuSource( 0 );
    cdebug_log(149,0) << "Shrink target of " << DbU::getValueString(shrink) << endl;
    margin = duTarget - targetCap;
    if (margin > shrink)
      setDuTarget( duTarget - shrink );
    else {
      cdebug_log(149,0) << "Target reset" << endl;
      setDuTarget( 0 );
    }
  }


  bool  AutoSegment::isReduceCandidate () const
  {
    if (isGlobal()) return false;
    if (not isSpinTopOrBottom()) return false;
    if (_reduceds) return false;

    AutoContact* source = getAutoSource();
    AutoContact* target = getAutoTarget();

    if (not source->isTurn() or not target->isTurn()) return false;

    return true;
  }


  bool  AutoSegment::canReduce ( Flags flags ) const
  {
    cdebug_log(159,0) << "AutoSegment::canReduce():" << this << endl;
    cdebug_log(159,0) << "  _reduceds:" << _reduceds << endl;

    DbU::Unit length = getAnchoredLength();
    if (length > getPPitch()) {
      if (isGlobal() or isFixed()) return false;
    }

    if (isDrag()) return false;
    if (not isSpinTopOrBottom()) return false;

    AutoContact* source = getAutoSource();
    AutoContact* target = getAutoTarget();

    if (isFixed()) {
      cdebug_log(159,0) << "isSpinTopOrBottom():" << isSpinTopOrBottom() << endl;
      if (length < getPPitch()) {
        if (Session::getAnabatic()->getState() >= Anabatic::EngineDriving) {
          cdebug_log(159,0) << "In EngineDriving mode" << endl;
          return isSpinTopOrBottom();
        }
      }
      // if (isSpinTopOrBottom()
      //    and source->isTurn()
      //    and target->isTurn())
      //   return true;
      return false;
    }

    if (not Session::getAnabatic()->isChannelStyle()
       and (getDepth() == 1) and isSpinBottom()) return false;
    if ((flags & Flags::WithPerpands) and _reduceds) return false;

    cdebug_log(159,0) << "  source:" << source->isHTee() << "+" << source->isVTee() << endl;
    cdebug_log(159,0) << "  target:" << target->isHTee() << "+" << target->isVTee() << endl;

    if (  ((source->isHTee() or target->isHTee()) and isHorizontal())
       or ((source->isVTee() or target->isVTee()) and isVertical  ()) ) return false;
    
    // if (  source->isHTee() or source->isVTee()
    //    or target->isHTee() or target->isVTee() ) return false;

    cdebug_log(159,0) << "  length:" << DbU::getValueString(length) << endl;
    if (flags & Flags::NullLength) return (length == 0); 

    unsigned int perpandicularDepth = getDepth();
    if (isSpinBottom()) {
      if (perpandicularDepth > 0) --perpandicularDepth;
    } else if (isSpinTop()) {
      ++perpandicularDepth;
      if (perpandicularDepth >= Session::getDepth()) return false;
    } else
      return false;
    
    if (getAnchoredLength() >= Session::getPitch(perpandicularDepth) * 2) return false;

    return true;
  }


  bool  AutoSegment::reduce ( Flags flags )
  {
    if (isReduced()) return false;
    if (not canReduce(flags)) return false;
    cdebug_log(159,0) << "AutoSegment::reduce():" << this << endl;

    AutoContact* source = getAutoSource();
    AutoContact* target = getAutoTarget();

    _flags |= SegIsReduced;
    for ( AutoSegment* perpandicular : source->getAutoSegments() ) {
      if (perpandicular == this) continue;
      perpandicular->incReduceds();
    }
    for ( AutoSegment* perpandicular : target->getAutoSegments() ) {
      if (perpandicular == this) continue;
      perpandicular->incReduceds();
    }
    
    return true;
  }


  uint32_t  AutoSegment::getNonReduceds ( Flags flags ) const
  {
    if (not canReduce(flags)) return false;
    cdebug_log(159,0) << "AutoSegment::getNonReduceds():" << this << endl;

    AutoContact* source = getAutoSource();
    AutoContact* target = getAutoTarget();
    uint32_t     nonReduceds = 0;

    for ( AutoSegment* perpandicular : source->getAutoSegments() ) {
      if (perpandicular == this) continue;
      if (perpandicular->getAnchoredLength()) ++nonReduceds;
    }
    for ( AutoSegment* perpandicular : target->getAutoSegments() ) {
      if (perpandicular == this) continue;
      if (perpandicular->getAnchoredLength()) ++nonReduceds;
    }
    
    return nonReduceds;
  }


  bool  AutoSegment::mustRaise () const
  {
    if (not (_flags & SegIsReduced)) return false;

    unsigned int perpandicularDepth = getDepth();
    if      (isSpinBottom()) --perpandicularDepth;
    else if (isSpinTop   ()) ++perpandicularDepth;
    else return true;

    return (getAnchoredLength() >= (Session::getPitch(perpandicularDepth) * 2));
  }


  bool  AutoSegment::raise ()
  {
    if (not (_flags & SegIsReduced)) return false;
    cdebug_log(159,0) << "AutoSegment::raise():" << this << endl;

    AutoContact* source = getAutoSource();
    AutoContact* target = getAutoTarget();

    _flags &= ~SegIsReduced;
    for ( AutoSegment* perpandicular : source->getAutoSegments() ) {
      if (perpandicular == this) continue;
      cdebug_log(159,0) << "dec PP:" << perpandicular << endl;
      perpandicular->decReduceds();
    }
    for ( AutoSegment* perpandicular : target->getAutoSegments() ) {
      if (perpandicular == this) continue;
      cdebug_log(159,0) << "dec PP:" << perpandicular << endl;
      perpandicular->decReduceds();
    }

    return true;
  }


  void  AutoSegment::changeDepth ( unsigned int depth, Flags flags )
  {
    DebugSession::open( getNet(), 145, 150 );

    cdebug_log(149,1) << "changeDepth() " << depth << " - " << this << endl;
    Session::invalidate( getNet() );

    _changeDepth( depth, flags & ~Flags::Propagate );

    if ((flags & Flags::Propagate) and not isNotAligned()) {
      cdebug_log(149,0) << "Propagate to aligneds." << endl;
      for ( AutoSegment* segment : getAligneds(Flags::NoCheckLayer) ) {
        segment->_changeDepth( depth, flags & ~Flags::Propagate );
      }
    }

    cdebug_tabw(149,-1);

    DebugSession::close();
  }


  void  AutoSegment::_changeDepth ( unsigned int depth, Flags flags )
  {
    cdebug_log(149,1) << "_changeDepth() - " << this << endl;

    if (isNonPref()) {
      if (depth != getDepth()+1)
        throw Error ( "AutoSegment::_changeDepth(): Non-Pref segment can only be moved up one layer\n"
                      "        on %s"
                    , getString(this).c_str() );
      unsetFlags( SegNonPref );
    }

    invalidate( Flags::Topology|Flags::NoCheckLayer );
    setFlags( SegInvalidatedLayer|SegInvalidatedSource|SegInvalidatedTarget );

    const Layer* newLayer = Session::getRoutingGauge()->getRoutingLayer(depth);
    if (getLayer() != newLayer) {
      cdebug_log(149,0) << "Effective layer change to " << depth << "/" << newLayer << endl;
      setLayer( depth );
      getAutoSource()->invalidate( Flags::Topology|Flags::NoCheckLayer );
      getAutoTarget()->invalidate( Flags::Topology|Flags::NoCheckLayer );
    }

    vector<GCell*> gcells;
    getGCells( gcells );
    for ( size_t i=0 ; i<gcells.size() ; ++i ) {
      gcells[i]->flags() |= Flags::Invalidated;
      cdebug_log(149,0) << "changeDepth() " << gcells[i] << this << " " << endl;
    }

    if (not (flags & Flags::WithNeighbors)) {
      cdebug_tabw(149,-1);
      return;
    }

    for ( AutoSegment* segment : getCachedOnSourceContact(Flags::DirectionMask) ) {
      if (segment == this) continue;
      if (segment->isGlobal  ()) continue;
      if (segment->isTerminal()) continue;

      if (not (segment->isHorizontal() xor isHorizontal()))
        segment->_changeDepth( depth  , Flags::NoFlags );
      else
        segment->_changeDepth( depth-1, Flags::NoFlags );
    }

    for ( AutoSegment* segment : getCachedOnTargetContact(Flags::DirectionMask) ) {
      if (segment == this) continue;
      if (segment->isGlobal  ()) continue;
      if (segment->isTerminal()) continue;

      if (not (segment->isHorizontal() xor isHorizontal()))
        segment->_changeDepth( depth  , Flags::NoFlags );
      else
        segment->_changeDepth( depth-1, Flags::NoFlags );
    }

    cdebug_tabw(149,-1);
  }


  bool  AutoSegment::canSlacken ( Flags flags ) const
  {
    cdebug_log(149,0) << "AutoSegment::canSlacken()" << endl;

    if (not isGlobal() and not (flags & Flags::Propagate)) return false;

    if (_canSlacken()) return true;
    if ((flags & Flags::Propagate) and not isNotAligned()) {
      for ( AutoSegment* segment : const_cast<AutoSegment*>(this)->getAligneds() ) {
        if (segment->_canSlacken()) return true;
      }
    }

    return false;
  }


  bool  AutoSegment::slacken ( Flags flags )
  {
    bool success = false;

    success = success or _slacken( flags );

    if ((flags & Flags::Propagate) and not isNotAligned()) {
      for ( AutoSegment* segment : getAligneds() ) {
        success = success or segment->_slacken( flags );
      }
    }

    return success;
  }


  float  AutoSegment::getMaxUnderDensity ( Flags flags )
  {
    cdebug_log(149,0) << "AutoSegment::getMaxUnderDensity() " << endl;

    size_t depth = Session::getRoutingGauge()->getLayerDepth(getLayer());

    vector<GCell*> gcells;
    getGCells( gcells );

    float maxDensity = 0.0;

    for ( size_t i=0 ; i<gcells.size() ; ++i ) {
      maxDensity = std::max( maxDensity, gcells[i]->getFeedthroughs(depth) );
    }

    if ((flags & Flags::Propagate) and not isNotAligned()) {
      forEach ( AutoSegment*, isegment, getAligneds() ) {
        isegment->getGCells( gcells );
        for ( size_t i=0 ; i<gcells.size() ; ++i ) {
          maxDensity = std::max( maxDensity, gcells[i]->getFeedthroughs(depth) );
        }
      }
    }

    return maxDensity;
  }


  bool  AutoSegment::canPivotUp ( float reserve, Flags flags ) const
  {
    cdebug_log(149,0) << "AutoSegment::canPivotUp() - " << flags
                      << " (reserve:" << reserve << ")" << endl;

    if ( isLayerChange()    or isFixed() or isUnbreakable() or isNoMoveUp() ) return false;
    if ( isStrongTerminal() and (not (flags & Flags::AllowTerminal)) ) return false;
    if ( isLocal()          and (not (flags & Flags::AllowLocal   )) ) return false;

    size_t depth = Session::getRoutingGauge()->getLayerDepth( getLayer() );
    if (depth+2 >= Session::getRoutingGauge()->getDepth()) return false;

    vector<GCell*> gcells;
    getGCells( gcells );
    for ( size_t i=0 ; i<gcells.size() ; i++ ) {
      if (not gcells[i]->hasFreeTrack(depth+2,reserve,flags)) return false;
    }

    if (not (flags&Flags::IgnoreContacts)) {
      cdebug_log(149,0) << getAutoSource() << endl;
      cdebug_log(149,0) << getAutoTarget() << endl;
      cdebug_log(149,0) << "min depths, Segment:" << depth
                        <<            " S:" << getAutoSource()->getMinDepth()
                        <<            " T:" << getAutoTarget()->getMinDepth() << endl;

      if (getAutoSource()->getMinDepth() < depth) return false;
      if (getAutoTarget()->getMinDepth() < depth) return false;
    }

    if ((flags & Flags::Propagate) and not isNotAligned()) {
      for ( AutoSegment* segment : const_cast<AutoSegment*>(this)->getAligneds(flags) ) {
        segment->getGCells( gcells );
        for ( size_t i=0 ; i<gcells.size() ; i++ ) {
          if (not gcells[i]->hasFreeTrack(depth+2,reserve,flags)) return false;
        }
        if (not (flags&Flags::IgnoreContacts)) {
          if (segment->getAutoSource()->getMinDepth() < depth) return false;
          if (segment->getAutoTarget()->getMinDepth() < depth) return false;
        }
      }
    } else {
      cdebug_log(149,0) << "AutoSegment::canPivotUp() - true [no propagate]" << endl;
      return true;
    }

    cdebug_log(149,0) << "AutoSegment::canPivotUp() - true [propagate]" << endl;

    return true;
  }


  bool  AutoSegment::canPivotDown ( float reserve, Flags flags ) const
  {
    cdebug_log(149,0) << "AutoSegment::canPivotDown()"
                << " (reserve:" << reserve << ")" << endl;

    if ( isLayerChange()    or isFixed() or isUnbreakable() ) return false;
    if ( isStrongTerminal() or isLocal() ) return false;

    size_t depth = Session::getRoutingGauge()->getLayerDepth( getLayer() );
    if (depth < 3) return false;

    vector<GCell*> gcells;
    getGCells( gcells );
    for ( size_t i=0 ; i<gcells.size() ; i++ ) {
      if (not gcells[i]->hasFreeTrack(depth-2,reserve)) return false;
    }

    cdebug_log(149,0) << getAutoSource() << endl;
    cdebug_log(149,0) << getAutoTarget() << endl;
    cdebug_log(149,0) << "max depths, Segment:" << depth
                <<            " S:" << getAutoSource()->getMaxDepth()
                <<            " T:" << getAutoTarget()->getMaxDepth() << endl;

    if (getAutoSource()->getMaxDepth() > depth) return false;
    if (getAutoTarget()->getMaxDepth() > depth) return false;
    if (not (flags & Flags::Propagate)) {
      cdebug_log(149,0) << "AutoSegment::canPivotDown() - true [no propagate]" << endl;
      return true;
    }

    if ((flags & Flags::Propagate) and not isNotAligned()) {
      forEach ( AutoSegment*, isegment, const_cast<AutoSegment*>(this)->getAligneds() ) {
        isegment->getGCells( gcells );
        for ( size_t i=0 ; i<gcells.size() ; i++ ) {
          if (not gcells[i]->hasFreeTrack(depth-2,reserve)) return false;
        }
        if (isegment->getAutoSource()->getMaxDepth() < depth) return false;
        if (isegment->getAutoTarget()->getMaxDepth() < depth) return false;
      }
    }

    cdebug_log(149,0) << "AutoSegment::canPivotDown() - true [propagate]" << endl;

    return true;
  }


  bool  AutoSegment::canMoveUp ( float reserve, Flags flags ) const
  {
    cdebug_log(159,0) << "AutoSegment::canMoveUp() " << flags
                      << " (reserve:" << reserve << ") " << this << endl;
    if (Session::getRoutingGauge()->getName() == "FlexLib")
      reserve += 2.0;

  // ls180 hard-coded hack.
  //if (getId() == 10023986) return false;
  //if (getId() == 6378409) return false;

    bool   nLowDensity   = true;
    bool   nLowUpDensity = true;

    if ( isLayerChange() or isFixed() or isUnbreakable() or isNoMoveUp() ) return false;
    if ( isStrongTerminal() and (not (flags & Flags::AllowTerminal)) ) return false;
    if ( isLocal()          and (not (flags & Flags::AllowLocal   )) ) return false;

    size_t depth = Session::getRoutingGauge()->getLayerDepth(getLayer()) + 2;
    if (depth > Session::getConfiguration()->getAllowedDepth()) return false;

    vector<GCell*> gcells;
    getGCells( gcells );

    for ( size_t i=0 ; i<gcells.size() ; i++ ) {
      if ( nLowDensity   and (gcells[i]->getWDensity(depth-2) > 0.5) ) nLowDensity   = false;
      if ( nLowUpDensity and (gcells[i]->getWDensity(depth)   > 0.2) ) nLowUpDensity = false;
      if (not gcells[i]->hasFreeTrack(depth,reserve,Flags::AllAbove)) {
        cdebug_log(159,0) << "Not enough free track in " << gcells[i] << endl;
        return false;
      }
    }

    cdebug_log(159,0) << "Enough free track under canonical segment." << endl;

    if (not (flags & Flags::IgnoreContacts)) {
      if (getAutoSource()->getMinDepth() < depth-2) return false;
      if (getAutoTarget()->getMinDepth() < depth-2) return false;
    }

    if ( isLocal() and not (flags & Flags::Propagate) ) {
      if (not getAutoSource()->canMoveUp(this)) return false;
      if (not getAutoTarget()->canMoveUp(this)) return false;
      return true;
    }

    // if (getAutoSource()->isTurn() and (getAutoSource()->getPerpandicular(this)->getLayer() == getLayer())) return false;
    // if (getAutoTarget()->isTurn() and (getAutoTarget()->getPerpandicular(this)->getLayer() == getLayer())) return false;
       
    cdebug_log(159,0) << "Both source & target Contacts can move up." << endl;

  //bool hasGlobalSegment = false;
    if ((flags & Flags::Propagate) and not isNotAligned()) {
      for ( AutoSegment* segment : const_cast<AutoSegment*>(this)->getAligneds(flags) ) {
        if (segment->isFixed ()) return false;
      //if (segment->isGlobal()) hasGlobalSegment = true;

        if (not (flags & Flags::IgnoreContacts)) {
          if (segment->getAutoSource()->getMinDepth() < depth-2) return false;
          if (segment->getAutoTarget()->getMinDepth() < depth-2) return false;
        }

        segment->getGCells( gcells );

        for ( size_t i=0 ; i<gcells.size() ; i++ ) {
          if ( nLowDensity   and (gcells[i]->getWDensity(depth-2) > 0.6) )
            nLowDensity = false;
          if ( nLowUpDensity and (gcells[i]->getWDensity(depth,Flags::AllAbove) > 0.2) ) {
            cdebug_log(159,0) << "lowUpDensity false in " << gcells[i]
                              << "d:" << gcells[i]->getWDensity(depth) << endl;
            nLowUpDensity = false;
          }
          if (not gcells[i]->hasFreeTrack(depth,reserve)) {
            cdebug_log(159,0) << "Not enough free track in " << gcells[i] << endl;
            return false;
          }
        }
      }
    }

    if (    nLowDensity   and (flags & Flags::CheckLowDensity  )) return false;
    if (not nLowUpDensity and (flags & Flags::CheckLowUpDensity)) return false;

    if ( (depth >= 4) and (flags & Flags::WithPerpands) ) {
      float fragmentation = (*gcells.begin())->getFragmentation( depth-1 );
      cdebug_log(159,0) << "Check begin GCell perpandicular fragmentation: " << fragmentation << endl;

      if (fragmentation < 0.5) {
        cdebug_log(159,0) << "Not enough free track for perpandicular in begin GCell "
                          << "(frag:" << fragmentation << ")."
                          << endl;
        return false;
      }

      fragmentation = (*gcells.rbegin())->getFragmentation( depth-1 );
      cdebug_log(159,0) << "Check end GCell perpandicular fragmentation: " << fragmentation << endl;

      if (fragmentation < 0.5) {
        cdebug_log(159,0) << "Not enough free track for perpandicular in end GCell "
                          << "(frag:" << fragmentation << ")."
                          << endl;
        return false;
      }
    }

    return true;
  }


  bool  AutoSegment::moveUp ( Flags flags )
  {
  //if ( not canMoveUp(0.0,flags) ) return false;
    changeDepth( Session::getRoutingGauge()->getLayerDepth(getLayer()) + (isNonPref() ? 1 : 2)
               , flags );

    return true;
  }


  bool  AutoSegment::moveUpToPref ( Flags flags )
  {
    size_t depth = Session::getRoutingGauge()->getLayerDepth(getLayer());
    if (not isNonPref() or (depth >= Session::getAllowedDepth())) return false;
    changeDepth( depth + 1, flags|Flags::Propagate );
    return true;
  }


  bool  AutoSegment::moveDown ( Flags flags )
  {
  //if ( not canPivotDown(0.0,flags) ) return false;
    changeDepth( Session::getRoutingGauge()->getLayerDepth(getLayer()) - 2, flags&Flags::Propagate );

    return true;
  }


  bool  AutoSegment::reduceDoglegLayer ()
  {
    if (not isReduced()) return false;

    DebugSession::open( getNet(), 149, 160 );
    cdebug_log(159,1) << "AutoSegment::reduceDoglegLayer(): " << this << endl;

    bool         success = false;
    AutoContact* source  = getAutoSource();
    AutoContact* target  = getAutoTarget();

    unsigned int minSourceDepth = Session::getAllowedDepth();
    unsigned int maxSourceDepth = 0;
    unsigned int minTargetDepth = Session::getAllowedDepth();
    unsigned int maxTargetDepth = 0;

    if (source->isTerminal()) {
      unsigned int anchorDepth = Session::getLayerDepth( source->base()->getAnchor()->getLayer() );
      minSourceDepth = std::min( minSourceDepth, anchorDepth );
      maxSourceDepth = std::max( maxSourceDepth, anchorDepth );
      cdebug_log(151,0) << "  source:" << source << endl;
    } else {
      for ( AutoSegment* perpandicular : source->getAutoSegments() ) {
        if (perpandicular == this) continue;
        cdebug_log(151,0) << "  connected:" << perpandicular << endl;
        minSourceDepth = std::min( minSourceDepth, perpandicular->getDepth() );
        maxSourceDepth = std::max( maxSourceDepth, perpandicular->getDepth() );
      }
    }
    if (target->isTerminal()) {
      unsigned int anchorDepth = Session::getLayerDepth( target->base()->getAnchor()->getLayer() );
      minTargetDepth = std::min( minTargetDepth, anchorDepth );
      maxTargetDepth = std::max( maxTargetDepth, anchorDepth );
      cdebug_log(151,0) << "  target:" << target << endl;
    } else {
      for ( AutoSegment* perpandicular : target->getAutoSegments() ) {
        if (perpandicular == this) continue;
        cdebug_log(151,0) << "  connected:" << perpandicular << endl;
        minTargetDepth = std::min( minTargetDepth, perpandicular->getDepth() );
        maxTargetDepth = std::max( maxTargetDepth, perpandicular->getDepth() );
      }
    }

    cdebug_log(159,0) << "Source span: [" << minSourceDepth << " " << maxSourceDepth << "]" << endl;
    cdebug_log(159,0) << "Target span: [" << minTargetDepth << " " << maxTargetDepth << "]" << endl;

    if (   (minSourceDepth == maxSourceDepth)
       and (minTargetDepth == maxTargetDepth)
       and (minSourceDepth == minTargetDepth) ) {
      const Layer* layer = Session::getRoutingLayer(minSourceDepth);
      DbU::Unit    vside = Session::getWireWidth   (minSourceDepth);
      DbU::Unit    hside = Session::getPWireWidth  (minSourceDepth);
      if (Session::getDirection(minSourceDepth) & Flags::Vertical)
        std::swap( hside, vside );

      cdebug_log(159,0) << "Reducing to " << minSourceDepth << " " << layer << endl;
    
      source->setLayer( layer );
      target->setLayer( layer );
      setLayer( layer );
      setWidth( hside );
      source->setSizes( hside, vside );
      target->setSizes( hside, vside );
      setDuSource( 0 );
      setDuTarget( 0 );

      success = true;
    }

    cdebug_tabw(159,-1);
    DebugSession::close();
    return success;

    
    // if (not source->isTurn() or not target->isTurn()) return true;

    // unsigned int perpandicularDepth = getDepth();
    // if (isSpinBottom()) --perpandicularDepth;
    // if (isSpinTop   ()) ++perpandicularDepth;

    // if (perpandicularDepth == getDepth()) {
    //   cerr << Bug( "AutoSegment::reduceDoglegLayer(): Reduced segment spin is neither top (TT) nor bottom (BB).\n"
    //                "      %s"
    //              , getString(this).c_str() ) << endl;
    //   return false;
    // }

    // const Layer* layer = Session::getRoutingLayer(perpandicularDepth);
    // DbU::Unit    side  = Session::getWireWidth   (perpandicularDepth);
    
    // source->setLayer( layer );
    // target->setLayer( layer );
    // setLayer( layer );
    // source->setSizes( side, side );
    // target->setSizes( side, side );

    // return true;
  }


  bool  AutoSegment::bloatStackedStrap ()
  {
    DebugSession::open( getNet(), 145, 150 );
    cdebug_log(149,1) << "AutoSegment::bloatStackedStrap() " << this << endl;
    double minArea = getLayer()->getMinimalArea();
    if (minArea == 0.0) {
      cdebug_log(149,-1) << "False, NO minimal area." << endl;
      DebugSession::close();
      return false;
    }
    
    DbU::Unit minLength
      = DbU::fromPhysical( minArea / DbU::toPhysical( getWidth(), DbU::UnitPower::Micro )
                         , DbU::UnitPower::Micro );
    cdebug_log(149,0) << "Min length: " << DbU::getValueString(minLength) << " ." << endl;

    if ((getSpanLength() >= minLength) or isReduced()) {
      cdebug_log(149,-1) << "False, has length or is reduced." << endl;
      DebugSession::close();
      return false;
    }
    if (isDrag()) {
      for ( AutoSegment* perpandicular : getPerpandiculars() ) {
        if (perpandicular->getSpanLength() > minLength) {
          cdebug_log(149,-1) << "False (drag), has length or PP has length." << endl;
          DebugSession::close();
          return false;
        }
      }
    } else {
      if (   ((_flags & (SegSourceBottom|SegTargetTop)) != (SegSourceBottom|SegTargetTop))
         and ((_flags & (SegTargetBottom|SegSourceTop)) != (SegTargetBottom|SegSourceTop)) ) {
        cdebug_log(149,-1) << "False, not part of a stacked VIA." << endl;
        DebugSession::close();
        return false;
      }
    }

    DbU::Unit side = DbU::fromPhysical( std::sqrt(minArea) , DbU::UnitPower::Micro );
    setWidth( side );
    setDuSource( -side/2 );
    setDuTarget(  side/2 );

    cdebug_log(149,-1) << "True, add area." << endl;
    DebugSession::close();
    return true;
  }


#if THIS_IS_DISABLED
  bool  AutoSegment::shearUp ( GCell* upGCell, AutoSegment*& movedUp, float reserve, Flags flags )
  {
    cdebug_log(149,0) << "AutoSegment::shearUp() " << this << endl;

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
        cdebug_log(149,0) << "Right shearing @ " << gcells[i] << endl;
        rightShear = gcells[i];
      }
    }

    GCell* leftShear = NULL;
    if ( iupGCell > 0 ) {
      size_t i = iupGCell;
      do {
        --i;
        if ( not gcells[i]->hasFreeTrack(upDepth,reserve) ) {
          cdebug_log(149,0) << "Left shearing @ " << gcells[i] << endl;
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


  Flags  AutoSegment::canDogleg ( Interval interval )
  {
    cdebug_log(149,0) << "AutoSegment::canDogleg(Interval) " << interval << endl;

    size_t  leftDogleg  = 0;
    size_t  rightDogleg = 0;
    if (getSpanU().contains(interval.getVMin())) leftDogleg++;
    if (getSpanU().contains(interval.getVMax())) rightDogleg++;

    if (not isNotAligned()) {
      for ( AutoSegment* segment : getAligneds() ) {
        if (segment->getSpanU().contains(interval.getVMin())) {
          if (segment->isFixed()) return Flags::NoFlags;
          leftDogleg++;
        }
        if (segment->getSpanU().contains(interval.getVMax())) {
          if (segment->isFixed()) return Flags::NoFlags;
          rightDogleg++;
        }
      }
    }

    if ( (leftDogleg == 1) and (rightDogleg <= 1) ) return Flags::DoglegOnLeft;
    if ( (leftDogleg <= 1) and (rightDogleg == 1) ) return Flags::DoglegOnRight;

    cdebug_log(149,0) << "leftCount:" << leftDogleg << " rightCount:" << rightDogleg << endl;

    return Flags::NoFlags;
  }


  AutoSegment* AutoSegment::makeDogleg ( AutoContact* from )
  {
    cdebug_log(149,1) << "AutoSegment::makeDogleg(AutoContact*) " << from << endl;
    cdebug_log(149,0)   << this << endl;

    RoutingGauge*               rg           = Session::getRoutingGauge();
    size_t                      segmentDepth = rg->getLayerDepth( getLayer() );
    const vector<AutoSegment*>& doglegs      = Session::getDoglegs();
    size_t                      index        = doglegs.size();
    bool                        isSource     = (getAutoSource() == from);

    cdebug_log(149,0) << "isSource:" << isSource << endl;

    makeDogleg( from->getGCell(), Flags::NoCheckLayer|Flags::NoCheckGCell );
    if (doglegs.size() == index) {
      cdebug_tabw(149,-1);
      return NULL;
    }
    doglegs[ index+1 ]->setAxis( isHorizontal() ? from->getX() : from->getY() );

    if (not from->getLayer()->contains(getLayer())) {
      cdebug_log(149,0) << "Contact layer do not contains Segment layer, adjust layers" << endl;

      if (getLayer()->above(from->getLayer())) {
        cdebug_log(149,0) << "Go Down from depth " << segmentDepth << endl;

        doglegs[ index + 1 ]->setLayer( segmentDepth-1 );
        cdebug_log(149,0) << "doglegs[i+1]: " << doglegs[index+1] << endl;

        if (isSource) {
          doglegs[ index + 0 ]->setLayer( std::max((size_t)1,segmentDepth-2) );
          cdebug_log(149,0) << "doglegs[i+0]: " << doglegs[index+0] << endl;
          cdebug_log(149,0) << "doglegs[i+1]: " << doglegs[index+1]->getAutoSource() << endl;
          cdebug_log(149,0) << "doglegs[i+1]: " << doglegs[index+1]->getAutoTarget() << endl;
        } else {
          doglegs[ index + 2 ]->setLayer( std::max((size_t)1,segmentDepth-2) );
          cdebug_log(149,0) << "doglegs[i+2]: " << doglegs[index+2] << endl;
          cdebug_log(149,0) << "doglegs[i+1]: " << doglegs[index+1]->getAutoTarget() << endl;
          cdebug_log(149,0) << "doglegs[i+1]: " << doglegs[index+1]->getAutoSource() << endl;
        }
      } else {
        cdebug_log(149,0) << "Go Up from depth " << segmentDepth << endl;

        doglegs[ index + 1 ]->setLayer( segmentDepth+1 );
        cdebug_log(149,0) << "doglegs[i+1]: " << doglegs[index+1] << endl;

        if (isSource) {
          doglegs[ index + 0 ]->setLayer( segmentDepth+2 );
          cdebug_log(149,0) << "doglegs[i+0]: " << doglegs[index+0] << endl;
          cdebug_log(149,0) << "doglegs[i+1]: " << doglegs[index+1]->getAutoSource() << endl;
          cdebug_log(149,0) << "doglegs[i+1]: " << doglegs[index+1]->getAutoTarget() << endl;
        } else {
          doglegs[ index + 2 ]->setLayer( segmentDepth+2 );
          cdebug_log(149,0) << "doglegs[i+2]: " << doglegs[index+2] << endl;
          cdebug_log(149,0) << "doglegs[i+1]: " << doglegs[index+1]->getAutoTarget() << endl;
          cdebug_log(149,0) << "doglegs[i+1]: " << doglegs[index+1]->getAutoSource() << endl;
        }
      }
      doglegs[ index + 1 ]->getAutoSource()->updateLayer();
      doglegs[ index + 1 ]->getAutoTarget()->updateLayer();
    }

    cdebug_tabw(149,-1);
    return doglegs[ index + (isSource?0:2) ];
  }


  Flags  AutoSegment::makeDogleg ( Interval interval, Flags flags )
  {
    cdebug_log(149,1) << "AutoSegment::makeDogleg(Interval) - " << interval << endl;

    bool         leftDogleg       = true;
    Flags        rflags           = Flags::NoFlags;
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

    if ( (leftDoglegCount != 1) and (rightDoglegCount != 1) )  { cdebug_tabw(149,-1); return 0; }
    if (not leftDoglegCount) {
      leftDogleg     = false;
      leftCandidate  = rightCandidate;
      rightCandidate = NULL;
    }
    
    if (leftCandidate and rightCandidate) {
      cdebug_log(149,0) << "Left  Constraint: " << leftCandidate->getSourceConstraints(Flags::NativeConstraints) << endl;
      cdebug_log(149,0) << "Right Constraint: " << rightCandidate->getTargetConstraints(Flags::NativeConstraints) << endl;

      if ( leftCandidate ->getTargetConstraints(Flags::NativeConstraints).getSize()
         < rightCandidate->getSourceConstraints(Flags::NativeConstraints).getSize() ) {
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

      cdebug_log(149,0) << "Break @" << DbU::getValueString(axis) << " " << leftCandidate << endl;

      Flags  direction = getDirection();
      GCell* gcell     = leftCandidate->getAutoSource()->getGCell();
      GCell* end       = leftCandidate->getAutoTarget()->getGCell();
      while ( gcell != end ) {
        if (gcell->getSide(direction).contains(axis)) break; 
        gcell = (direction == Flags::Horizontal) ? gcell->getEast (getNativeMin())
                                                 : gcell->getNorth(getNativeMin());
      }

      cdebug_log(149,0) << "In " << gcell << endl;
      rflags = leftCandidate->_makeDogleg( gcell, flags );

      const vector<AutoSegment*>& doglegs = Session::getDoglegs();
      if (doglegs.size() >= 2) {
        cdebug_log(149,0) << "AutoSegment::makeDogleg(): @" << DbU::getValueString(axis) << endl;
        doglegs[1]->setAxis( axis );
      }
    }

    cdebug_tabw(149,-1);
    return rflags | (leftDogleg ? Flags::DoglegOnLeft : Flags::DoglegOnRight);
  }


  Flags  AutoSegment::makeDogleg ( GCell* doglegGCell, Flags flags )
  {
    cdebug_tabw(149,1);

    Flags  rflags = Flags::NoFlags;

    if (    doglegGCell->isIoPad()
       and (Session::getAnabatic()->getState() != EngineGlobalLoaded) ) {
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

    if ((flags & Flags::NoCheckGCell) or doglegGCell->getSide(getDirection()).intersect(getSpanU())) {
      cdebug_log(149,0) << "Dogleg in " << this << " spanU:" << getSpanU() << endl;
      rflags = _makeDogleg( doglegGCell, flags );
    } else {
      cdebug_log(149,0) << "Looking in aligneds." << endl;
      if (not isNotAligned()) {
        for ( AutoSegment* aligned : getAligneds(flags) ) {
          cdebug_log(149,0) << "| Try in " << aligned << endl;
          if (doglegGCell->getSide(getDirection()).intersect(aligned->getSpanU())) {
            cdebug_log(149,0) << "Dogleg in " << aligned << endl;
            rflags = aligned->_makeDogleg( doglegGCell, flags );
            cdebug_tabw(149,-1);
            return 0;
          }
        }
      }
      cerr << Bug("Cannot make a dogleg in %s at %s"
                 ,_getString().c_str(), getString(doglegGCell).c_str()) << endl;
    }
    cdebug_tabw(149,-1);

    return rflags;
  }


  bool  AutoSegment::_check () const
  {
    bool coherency = true;

    coherency = checkNotInvalidated() and coherency;
    coherency = checkPositions()      and coherency;
    coherency = checkConstraints()    and coherency;
    coherency = checkDepthSpin()      and coherency;

    if (isDrag() xor (getAutoSource()->canDrag() or getAutoTarget()->canDrag())) {
      cerr << Error( "%s\n"
                     "        Discrepency between segment \"drag\" state and it's contacts.\n"
                     "        source:%s\n" 
                     "        target:%s" 
                   , getString(this).c_str()
                   , getString(getAutoSource()).c_str()
                   , getString(getAutoTarget()).c_str()
                   ) << endl;
      coherency = false;
    }
    return coherency;
  }


  string  AutoSegment::_getStringFlags () const
  {
    string state;
    state += isFixed            () ?" F":" -";
    state += isFixedAxis        () ? "X": "-";
    state += isUnsetAxis        () ? "u": "-";
    state += isAtMinArea        () ? "a": "-";
    state += isStrap            () ? "S": "-";
    state += isUnbreakable      () ? "U": "-";
    state += isCanonical        () ? "C": "-";
    state += isGlobal           () ? "G": "-";
    state += isWeakGlobal       () ? "g": "-";
    state += isLongLocal        () ? "L": "-";
    state += isStrongTerminal   () ? "T": "-";
    state += isDrag             () ? "D": "-";
    state += isWeakTerminal1    () ? "W": "-";
    state += isWeakTerminal2    () ? "w": "-";
    state += isNotAligned       () ? "A": "-";
    state += isSlackened        () ? "S": "-";
    state += isReduced          () ? "r": "-";
    state += isInvalidated      () ? "i": "-";
    state += isInvalidatedLayer () ? "l": "-";
    state += isBelowPitch       () ? "Z": "-";
    state += hasBecomeBelowPitch() ? "z": "-";
    state += isNonPref          () ? "P": "-";

    if      (_flags & SegSourceTop)    state += 't';
    else if (_flags & SegSourceBottom) state += 'b';
    else state += '-';
    if      (_flags & SegTargetTop)    state += 't';
    else if (_flags & SegTargetBottom) state += 'b';
    else state += '-';

    state += isShortNet        () ? "s": "-";
    state += isNoMoveUp        () ? "M": "-";

    return state;
  }


  string  AutoSegment::_getString () const
  {
    string sdistance = " rpD:" + getString(_rpDistance);
    string sblevel   = " bl:"  + getString(_breakLevel);
    string s         = base()->_getString();
    s.insert ( s.size()-1, sdistance );
    s.insert ( s.size()-1, sblevel );
    s.insert ( s.size()-1, _getStringFlags() );
    return s;
  }


  Record* AutoSegment::_getRecord () const
  {
    Record* record = base()->_getRecord ();
    record->add ( getSlot ( "_extensionCaps" , &_extensionCaps   ) );
    record->add ( getSlot ( "_gcell"         ,  _gcell           ) );
    record->add ( getSlot ( "_id"            , &_id              ) );
    record->add ( getSlot ( "_flags"         , &_flags           ) );
    record->add ( getSlot ( "_rpDistance"    ,  _rpDistance      ) );
    record->add ( getSlot ( "_breakLevel"    ,  _breakLevel      ) );
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
    Horizontal*  horizontal = dynamic_cast<Horizontal*>( hurricaneSegment );
    Vertical*    vertical   = dynamic_cast<Vertical*  >( hurricaneSegment );

    const Layer* horizontalLayer = Session::getDHorizontalLayer();
    DbU::Unit    horizontalWidth = Session::getDHorizontalWidth();
    const Layer* verticalLayer   = Session::getDVerticalLayer();
    DbU::Unit    verticalWidth   = Session::getDVerticalWidth();
    if (not Session::getAnabatic()->getConfiguration()->isGMetal(hurricaneSegment->getLayer())) {
      size_t depth = Session::getAnabatic()->getConfiguration()->getLayerDepth( hurricaneSegment->getLayer() );
      if ((depth == 0) or (depth > 2)) {
        horizontalLayer = verticalLayer = hurricaneSegment->getLayer();
        horizontalWidth = Session::getAnabatic()->getConfiguration()->getWireWidth( depth );
        verticalWidth   = Session::getAnabatic()->getConfiguration()->getPWireWidth( depth );
        if (vertical)
          std::swap( horizontalWidth, verticalWidth );
      }
    }

    uint32_t wPitch = NetRoutingExtension::getWPitch( source->getNet() );
    if (wPitch > 1) {
      horizontalWidth += (wPitch-1) * Session::getDHorizontalPitch();
      verticalWidth   += (wPitch-1) * Session::getDVerticalPitch  ();
    }
    cdebug_log(149,0) << "wPitch:" << wPitch << " hW:" << DbU::getValueString(horizontalWidth) << endl;

    if (wPitch > 2) {
      throw Error( "wPitch %d for \"%s\"", wPitch, getString(source->getNet()->getName()).c_str() );
    }

    bool         reattachSource = false;
    bool         reattachTarget = false;
    AutoSegment* segment   = NULL;
    AutoContact* reference = NULL;

    cdebug_log(149,0) << "Source:" << source << endl;
    cdebug_log(149,0) << "Target:" << target << endl;

    if (source->isFixed() and target->isFixed()) {
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
    }

    if (target->isFixed() or target->isTerminal()) reference = target;
    if (source->isFixed() or source->isTerminal()) reference = source;

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
        if (Session::getAnabatic()->getConfiguration()->isGMetal(horizontal->getLayer())) {
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

      if (reference) horizontal->setY( reference->getY() );
      segment = new AutoHorizontal ( horizontal );
      segment->_postCreate();
    } else if (vertical) {
      if (vertical->getLayer() != verticalLayer) {
        if (Session::getAnabatic()->getConfiguration()->isGMetal(vertical->getLayer()) ) {
          vertical->setLayer( verticalLayer );
          vertical->setWidth( verticalWidth );
        }
      } else {
        if (vertical->getWidth() != verticalWidth) {
            cerr << Warning("Segment %s has non-default width %s."
                           ,getString(vertical).c_str()
                           ,DbU::getValueString(vertical->getWidth()).c_str()) << endl;
        }
      }

      if (reference) vertical->setX( reference->getX() );
      segment = new AutoVertical ( vertical );
      segment->_postCreate();
    } else {
      throw Error( badSegment, getString(source).c_str(), getString(target).c_str() );
    }

    if (wPitch > 1) segment->setFlags( SegWide );
    if (source->canDrag() or target->canDrag()) segment->setFlags( SegDrag );

    return segment;
  }


  AutoSegment* AutoSegment::create ( AutoContact* source
                                   , AutoContact* target
                                   , Flags        dir
                                   , size_t       depth
                                   )
  {
  // Hardcoded: make the assumption that,
  //    depth=0 is terminal reserved  |  METAL1
  //    depth=1 is horizontal         |  METAL2
  //    depth=2 is vertical           |  METAL3
  // Should be based on gauge informations.
    const Layer* hLayer = Session::getDHorizontalLayer();
    DbU::Unit    hWidth = Session::getDHorizontalWidth();
    const Layer* vLayer = Session::getDVerticalLayer();
    DbU::Unit    vWidth = Session::getDVerticalWidth();

    if (dir & Flags::UseNonPref) {
      if (dir & Flags::Vertical) {
        vLayer = hLayer;
        vWidth = Session::getDPHorizontalWidth();
        cdebug_log(149,0) << "Make vertical in non-preferred direction (ppW:"
                          << DbU::getValueString(vWidth).c_str() << ")." << endl;
      }
      if (dir & Flags::Horizontal) {
        hLayer = vLayer;
        hWidth = Session::getDPVerticalWidth();
        cdebug_log(149,0) << "Make horizontal in non-preferred direction (ppW:"
                          << DbU::getValueString(hWidth).c_str() << ")." << endl;
      }
    }

    const Layer* horizontalLayer = hLayer;
    DbU::Unit    horizontalWidth = hWidth;
    const Layer* verticalLayer   = vLayer;
    DbU::Unit    verticalWidth   = vWidth;
    cdebug_log(149,0) << "verticalWidth:" << DbU::getValueString(verticalWidth).c_str() << endl;

    uint32_t wPitch = NetRoutingExtension::getWPitch( source->getNet() );
    if (wPitch > 1) {
      horizontalWidth = (wPitch-1) * Session::getDHorizontalPitch() + hWidth;
      verticalWidth   = (wPitch-1) * Session::getDVerticalPitch  () + vWidth;
    }
    cdebug_log(149,0) << "verticalWidth:" << DbU::getValueString(verticalWidth).c_str() << endl;

    if (depth != RoutingGauge::nlayerdepth) {
      horizontalLayer = verticalLayer = Session::getRoutingLayer( depth );

      if (wPitch > 1) {
        horizontalWidth = verticalWidth = (wPitch-1) * Session::getPitch    (depth)
                                                     + Session::getWireWidth(depth);
      } else {
        if (dir & Flags::Horizontal) {
          horizontalWidth = Session::getWireWidth ( depth );
          verticalWidth   = Session::getPWireWidth( depth );
        } else {
          verticalWidth   = Session::getWireWidth ( depth );
          horizontalWidth = Session::getPWireWidth( depth );
        }
        cdebug_log(149,0) << "hW:" << DbU::getValueString(horizontalWidth).c_str()
                          << "vW:" << DbU::getValueString(  verticalWidth).c_str()
                          << endl;
        if (dir & Flags::UseNonPref) {
          cdebug_log(149,0) << "swap H/W width." << endl;
          std::swap( horizontalWidth, verticalWidth );
        }
      }
    }
    cdebug_log(149,0) << "verticalWidth:" << DbU::getValueString(verticalWidth).c_str() << endl;

    AutoSegment* segment;
    AutoContact* reference = source;

    cdebug_log(149,0) << "Source:" << source << endl;
    cdebug_log(149,0) << "Target:" << target << endl;

    if (target->isFixed()) {
      if (source->isFixed()) {
        if ( (dir == Flags::Horizontal) and (source->getY() != target->getY()))
          cerr << Warning( "Straight AutoHorizontal connecting misaligned contacts:\n"
                           "          %s\n"
                           "          %s"
                         , getString(source).c_str()
                         , getString(target).c_str()
                         ) << endl;
        if ( (dir == Flags::Vertical) and (source->getX() != target->getX()))
          cerr << Warning( "Straight AutoVertical connecting misaligned contacts:\n"
                           "          %s\n"
                           "          %s"
                         , getString(source).c_str()
                         , getString(target).c_str()
                         ) << endl;
      } else
        reference = target;
    }

    if (dir & Flags::Horizontal) {
      segment = create( source
                      , target
                      , Horizontal::create( source->base()
                                          , target->base()
                                          , horizontalLayer
                                          , reference->getY()
                                          , horizontalWidth ) );
    } else if (dir & Flags::Vertical) {
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

    if (wPitch > 1)                                      segment->setFlags( SegWide );
    if (source->canDrag() or target->canDrag())          segment->setFlags( SegDrag );
    if (dir & Flags::UseNonPref)                         segment->setFlags( SegNonPref );
    if (dir & Flags::Unbreakable)                        segment->setFlags( SegUnbreakable );
    if (dir.contains(Flags::UseNonPref|Flags::OnVSmall)) segment->setFlags( SegOnVSmall );

    return segment;
  }


  void  AutoSegment::destroy ()
  {
    _preDestroy ();
    delete this;
  }


  AutoSegment* AutoSegment::getGlobalThroughDogleg ( AutoSegment* dogleg, AutoContact* from )
  {
    AutoContact* source = dogleg->getAutoSource();
    AutoContact* target = dogleg->getAutoTarget();
    if (not source->isTurn() or not target->isTurn()) return NULL;

    AutoSegment* fromSegment = (source == from) ? source->getPerpandicular(dogleg) : target->getPerpandicular(dogleg);
    AutoSegment* toSegment   = (source != from) ? source->getPerpandicular(dogleg) : target->getPerpandicular(dogleg);

    if (not toSegment->isGlobal() or (toSegment->getLayer() != fromSegment->getLayer())) return NULL;

    Interval fromConstraints;
    Interval toConstraints;
    fromSegment->getConstraints( fromConstraints );
    toSegment  ->getConstraints( toConstraints );
    if (not fromConstraints.intersect(toConstraints)) return NULL;

    return toSegment;
  }


  bool  AutoSegment::isTopologicalBound ( AutoSegment* seed, Flags flags )
  {
    cdebug_log(145,1) << "isTopologicalBound() - " << seed << endl;

    set<AutoContact*>     exploreds;
    vector<AutoContact*>  stack;
    DbU::Unit             axis;

    if (flags & Flags::Superior) axis = seed->getTargetU();
    else                    axis = seed->getSourceU();

    cdebug_log(145,0) << "check for bound " << DbU::getValueString(axis) << endl;

    exploreds.insert( seed->getAutoSource() );
    exploreds.insert( seed->getAutoTarget() );

    if (seed->getAnchoredLength()) {
      if (flags & Flags::Superior) stack.push_back( seed->getAutoTarget() );
      else                    stack.push_back( seed->getAutoSource() );
    } else {
      stack.push_back( seed->getAutoTarget() );
      stack.push_back( seed->getAutoSource() );
    }

    while ( not stack.empty() ) {
      AutoContact* currentContact = stack.back();
      stack.pop_back();

      cdebug_log(145,0) << "Exploring: " << (void*)currentContact << " " << currentContact << endl;

      exploreds.insert( currentContact );

      if (currentContact->getAnchor()) { cdebug_tabw(145,-1); return true; }

      forEach ( Component*, component, currentContact->getSlaveComponents() ) {
        Segment* segment = dynamic_cast<Segment*>( *component );
        if (not segment) continue;

        AutoSegment* autoSegment = Session::lookup( segment );
        if (not autoSegment) continue;

        if (not autoSegment->getAnchoredLength()) {
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

        if (autoSegment->isHorizontal() xor (bool)(flags & Flags::Horizontal)) continue;

        cdebug_log(145,0) << "| " << autoSegment << endl;

        if (flags & Flags::Superior) {
          if (autoSegment->getTargetU() > axis) { cdebug_tabw(145,-1); return true; }
        } else {
          if (autoSegment->getSourceU() < axis) { cdebug_tabw(145,-1); return true; }
        }
      }
    }

    cdebug_tabw(145,-1);
    return false;
  }


#if THIS_IS_DISABLED
  Flags  AutoSegment::getPerpandicularState ( AutoContact* contact
                                            , AutoSegment* source
                                            , AutoSegment* current
                                            , bool         isHorizontalMaster
                                            , const Layer* masterLayer )
  {
    Flags state = Flags::NoFlags;

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
                                         , vector< tuple<AutoSegment*,Flags> >&
                                                                 perpandiculars
                                         , DbU::Unit&            leftBound
                                         , DbU::Unit&            rightBound
                                         )
  {
    cdebug_log(145,1) << "getTopologicalInfos() - " << seed << endl;

    bool  isSourceBoundToChannel = false;
    bool  isTargetBoundToChannel = false;

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

      cdebug_log(149,0) << "Segments of: " << sourceContact << endl;
      LocatorHelper helper (sourceContact, Flags::Horizontal|Flags::WithPerpands);
      for ( ; helper.isValid() ; helper.progress() ) {
        AutoSegment* currentSegment = helper.getSegment();
        cdebug_log(149,0) << "Looking for: " << currentSegment << endl;
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
          cdebug_log(149,0) << "Target: " << targetContact << endl;
          if (targetContact) {
            if (  (seed->isHorizontal() and sourceContact->isHTee())
               or (seed->isVertical  () and sourceContact->isVTee()) ) {
              cdebug_log(149,0) << "Stacking target. " << endl;
              stack.push( targetContact, currentSegment );
            }
          }
        } else {
          cdebug_log(149,0) << "| perpandicular " << currentSegment << endl; 
          Flags perpandFlags = (currentSegment->getAutoSource() == sourceContact)
            ? Flags::Source : Flags::Target;
          perpandiculars.push_back( make_tuple( currentSegment, perpandFlags ));
          if (Session::getAnabatic()->isChannelStyle()) {
            if (currentSegment->isNonPref() and currentSegment->isFixed()) {
              if (perpandFlags & Flags::Source) isSourceBoundToChannel = true;
              else                              isTargetBoundToChannel = true;
            }
          }
        }
      }
    }

    if (isSourceBoundToChannel) leftBound  += (rightBound - leftBound)/2;
    if (isTargetBoundToChannel) rightBound -= (rightBound - leftBound)/2;

    cdebug_tabw(145,-1);
  }


  int  AutoSegment::getTerminalCount ( AutoSegment* seed, vector<AutoSegment*>& collapseds )
  {
    cdebug_log(145,0) << "getTerminalCount() - " << seed << " (+collapseds)" << endl;

    int  count = 0;
    for ( size_t i=0 ; i < collapseds.size() ; i++ ) {
      if (collapseds[i]->isStrongTerminal())
        count++;
    }
    if (seed->getAutoSource()->isTerminal()) count++;
    if (seed->getAutoTarget()->isTerminal()) count++;

    return count;
  }


} // End of Anabatic namespace.
