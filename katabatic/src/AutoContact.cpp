// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AutoContact.cpp"                        |
// +-----------------------------------------------------------------+


#include  <cstdlib>
#include  <climits>
#include  <sstream>
#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Layer.h"
#include  "hurricane/ViaLayer.h"
#include  "hurricane/BasicLayer.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Net.h"
#include  "hurricane/Plug.h"
#include  "hurricane/RoutingPad.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/DebugSession.h"
#include  "crlcore/RoutingGauge.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoVertical.h"
#include  "katabatic/AutoHorizontal.h"
#include  "katabatic/Session.h"


namespace Katabatic {

  using std::ostringstream;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DebugSession;
  using Hurricane::ForEachIterator;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoContact".


  size_t      AutoContact::_maxId       = 0;
  size_t      AutoContact::_allocateds  = 0;
  const Name  AutoContact::_goName      = "Katabatic::AutoContact";


  AutoContact::AutoContact ( GCell* gcell, Contact* contact )
    : ExtensionGo(contact->getCell())
  //, _id        (_maxId++)
    , _id        (contact->getId())
    , _contact   (contact)
    , _gcell     (gcell)
    , _flags     (CntInvalidatedCache|CntInCreationStage)
    , _dxMin     (0)
    , _dxMax     ((int)DbU::toLambda( _gcell->getXMax()-_gcell->getX() ))
    , _dyMin     (0)
    , _dyMax     ((int)DbU::toLambda( _gcell->getYMax()-_gcell->getY() ))
  {
    _allocateds++;
    _gcell->addContact ( this );
  }


  void  AutoContact::_preCreate ( GCell* gcell, Net* net, const Layer* layer )
  {
    if (not gcell) throw Error("AutoContact::_preCreate(): GCell* parameter must not be NULL.");
    if (not net  ) throw Error("AutoContact::_preCreate(): Net* parameter must not be NULL.");
    if (not layer) throw Error("AutoContact::_preCreate(): const Layer* parameter must not be NULL.");
  }


  void  AutoContact::_postCreate ()
  {
    ExtensionGo::_postCreate();

    restoreNativeConstraintBox();

    ltrace(90) << "Native CBox: " << this
               << " <" << DbU::toLambda(getCBXMin())
               << " "  << DbU::toLambda(getCBYMin())
               << " "  << DbU::toLambda(getCBXMax())
               << " "  << DbU::toLambda(getCBYMax()) << ">" << endl;
    
    Session::link( this );
    invalidate( KbTopology );

    ltrace(90) << "AutoContact::_postCreate() - " << this << " in " << _gcell << endl;
  }


  void  AutoContact::_preDestroy ()
  {
    DebugSession::open( _contact->getNet() );

    ltrace(90) << "AutoContact::_preDestroy() - <AutoContact id:" << _id << ">" << endl;

#if 0
    bool canDestroyBase = true;

    if (not _contact->getSlaveComponents().isEmpty()) {
      ostringstream message;
      message << "Base contact still have slaves components, cancelled.\n"
              << "        on: " << this;

      forEach ( Component*, icomponent, _contact->getSlaveComponents() ) {
        message << "\n          | " << (*icomponent);
      }
      cerr << Error( message.str() ) << endl;

      canDestroyBase = false;
    }
#endif

    if (not Session::doDestroyTool()) {
      _gcell->removeContact( this );
      Session::unlink( this );
    }
        
    ExtensionGo::_preDestroy();
#if 0
    if (Session::doDestroyBaseContact() and canDestroyBase)
      _contact->destroy();
#endif

    DebugSession::close();
  }


  AutoContact::~AutoContact ()
  { _allocateds--; }


  size_t  AutoContact::getAllocateds ()
  { return _allocateds; }


  const Name& AutoContact::getStaticName ()
  { return _goName; }


  bool  AutoContact::canDestroy ( unsigned int flags ) const
  {
    if (not _contact->getSlaveComponents().isEmpty()) {
      if (flags & KbWarnOnError) {
        cerr << Error("Base contact still have slaves components, cancelled.\n"
                     "        (%s)"
                     ,_getString().c_str()) << endl;
      }
      return false;
    }
    return true;
  }


  const Name& AutoContact::getName () const
  { return _goName; }


  AutoSegments  AutoContact::getAutoSegments ()
  { return new AutoSegments_CachedOnContact(this); }


  unsigned int  AutoContact::getMinDepth () const
  {
    size_t minDepth = (size_t)-1;
    Component* anchor = getAnchor ();
    if (anchor) {
      minDepth = std::min( minDepth, Session::getRoutingGauge()->getLayerDepth(anchor->getLayer()) );
    //ltrace(200) << "Anchor:" << anchor << endl;
    }

    forEach ( AutoSegment*, isegment, const_cast<AutoContact*>(this)->getAutoSegments() ) {
      minDepth = std::min( minDepth, Session::getRoutingGauge()->getLayerDepth(isegment->getLayer()) );
    //ltrace(200) << "Slave:" << *icomponent << endl;
    }

    return (unsigned int)minDepth;
  }


  unsigned int  AutoContact::getMaxDepth () const
  {
    size_t maxDepth = 0;
    Component* anchor = getAnchor ();
    if ( anchor ) {
      maxDepth = std::max ( maxDepth, Session::getRoutingGauge()->getLayerDepth(anchor->getLayer()) );
    //ltrace(200) << "Anchor:" << anchor << endl;
    }

    forEach ( AutoSegment*, isegment, const_cast<AutoContact*>(this)->getAutoSegments() ) {
      maxDepth = std::max ( maxDepth, Session::getRoutingGauge()->getLayerDepth(isegment->getLayer()) );
    //ltrace(200) << "Slave:" << *icomponent << endl;
    }

    return (unsigned int)maxDepth;
  }


  void  AutoContact::getLengths ( DbU::Unit* lengths, AutoSegment::DepthLengthSet& processeds )
  {
    DbU::Unit hSideLength = getGCell()->getSide( KbHorizontal ).getSize();
    DbU::Unit vSideLength = getGCell()->getSide( KbVertical   ).getSize();

    forEach ( AutoSegment*, isegment, getAutoSegments() ) {
      bool isSourceHook = (isegment->getAutoSource() == this);

      if (processeds.find(*isegment) != processeds.end()) continue;
      processeds.insert( *isegment );

      size_t     depth  = Session::getRoutingGauge()->getLayerDepth(isegment->getLayer());
      DbU::Unit  length;
      if (isegment->isLocal()) {
        length = isegment->getLength();
        lengths[depth] += length;

        DbU::Unit sideLength = (isegment->isHorizontal()) ? hSideLength : vSideLength;
        if ( not isegment->isUnbound() and (abs(length) > sideLength) )
          cerr << Error("Suspicious length:%.2f of %s."
                       ,DbU::toLambda(length),getString(*isegment).c_str()) << endl;
      } else {
        if ( isegment->isHorizontal() ) {
          if ( isSourceHook )
            lengths[depth] += _gcell->getBoundingBox().getXMax() - isegment->getSourceX();
          else
            lengths[depth] += isegment->getTargetX() - _gcell->getBoundingBox().getXMin();
        } else {
          if ( isSourceHook )
            lengths[depth] += _gcell->getBoundingBox().getYMax() - isegment->getSourceY();
          else
            lengths[depth] += isegment->getTargetY() - _gcell->getBoundingBox().getYMin();
        }
      }
    }
  }


  Box  AutoContact::getNativeConstraintBox () const
  { return isFixed() ? Box(_contact->getPosition()) : _gcell->getBoundingBox(); }


  Interval  AutoContact::getNativeUConstraints ( unsigned int direction ) const
  {
    Box       nativeConstraints = getNativeConstraintBox();
    Interval  constraint;
    if (direction & KbHorizontal) {
      constraint = Interval( nativeConstraints.getXMin(), nativeConstraints.getXMax() );
    } else {
      constraint = Interval( nativeConstraints.getYMin(), nativeConstraints.getYMax() );
    }
    if (direction & KbNoGCellShrink) constraint.inflate( 0, GCell::getTopRightShrink() );
    return constraint;
  }


  Interval  AutoContact::getUConstraints ( unsigned int direction ) const
  {
    Interval  constraint;
    if (direction & KbHorizontal) {
      constraint = Interval( getCBXMin(), getCBXMax() );
    } else {
      constraint = Interval( getCBYMin(), getCBYMax() );
    }
    if (direction & KbNoGCellShrink) constraint.inflate( 0, GCell::getTopRightShrink() );
    return constraint;
  }


  void  AutoContact::invalidate ( unsigned int flags )
  {
    if (not isInvalidated()) {
      ltrace(110) << "AutoContact::invalidate() - " << this << endl;
      ltracein(110);
      setFlags( CntInvalidated );
      if (flags & KbTopology ) setFlags( CntInvalidatedCache );
      Session::invalidate( this );

      _invalidate( flags );
    //forEach( AutoSegment*, isegment, getAutoSegments() )
    //  isegment->invalidate();

      getGCell()->invalidate();
      ltraceout(110);
    }
  }


  void  AutoContact::setGCell ( GCell* gcell )
  {
    invalidate();
    if (_gcell) _gcell->removeContact( this );

    _gcell = gcell;
    if (_gcell) {
      ltrace(110) << "AutoContact::setGCell() " << gcell << endl;
      _gcell->addContact( this );
      _contact->setPosition( _gcell->getCenter() );
      _dxMin = 0;
      _dyMin = 0;
      _dxMax = (int)DbU::toLambda( _gcell->getXMax()-_gcell->getX() );
      _dyMax = (int)DbU::toLambda( _gcell->getYMax()-_gcell->getY() );
      ltrace(110) << "* deltas: [" << _dxMin << " " << _dyMin << " " << _dxMax << " " << _dyMax << "]" << endl;
    } else {
      cerr << Bug( "NULL GCell for %s.", _getString().c_str() ) << endl;
    }
  }


  void  AutoContact::_getTopology ( Component*& anchor, Horizontal**& horizontals, Vertical**& verticals, size_t size )
  {
    size_t hcount = 0;
    size_t vcount = 0;

    for ( size_t i=0 ; i<size ; ++i ) {
      horizontals[i] = NULL;
      verticals  [i] = NULL;
    }

    anchor = getAnchor();

    forEach ( Component*, icomponent, getSlaveComponents() ) {
      Horizontal* h = dynamic_cast<Horizontal*>(*icomponent);
      if (h != NULL) {
        if (hcount < size) horizontals[hcount++] = h;
      } else {
        Vertical* v = dynamic_cast<Vertical*>(*icomponent);
        if ( (v != NULL) and (vcount < size) ) verticals[vcount++] = v;
      }
    }
  }


  void  AutoContact::showTopologyError ( const std::string& message )
  {
    Component*   anchor      = NULL;
    Horizontal** horizontals = new Horizontal* [10];
    Vertical**   verticals   = new Vertical*   [10];

    _getTopology ( anchor, horizontals, verticals, 10 );
 
    cerr << Error("In topology of %s",getString(this).c_str()) << endl;
    if (anchor) cerr << "        A: " << anchor << endl;

    for ( size_t i=0 ; (i<10) and (horizontals[i] != NULL); ++i ) {
      AutoSegment* autoSegment = Session::lookup ( horizontals[i] );
      if (autoSegment != NULL)
        cerr << "        " << (autoSegment->isGlobal()?'G':'L') << ": " << autoSegment << endl; 
      else
        cerr << "        ?: " << horizontals[i] << endl; 
    }

    for ( size_t i=0 ; (i<10) and (verticals[i] != NULL); ++i ) {
      AutoSegment* autoSegment = Session::lookup ( verticals[i] );
      if (autoSegment != NULL)
        cerr << "        " << (autoSegment->isGlobal()?'G':'L') << ": " << autoSegment << endl; 
      else
        cerr << "        ?: " << verticals[i] << endl; 
    }

    cerr << "        " << message  << endl;

    delete [] horizontals;
    delete [] verticals;
  }


  void  AutoContact::checkTopology ()
  {
  //ltrace(110) << "checkTopology() NOT RE-IMPLEMENTED YET " << this << endl;
  }


  bool  AutoContact::isTee ( unsigned int direction ) const
  {
    return (isHTee() and (direction & KbHorizontal))
        or (isVTee() and (direction & KbVertical  ));
  }


  bool  AutoContact::canMoveUp ( const AutoSegment* moved ) const
  {
    ltrace(200) << "AutoContact::canMoveUp() " << this << endl;
    size_t viaDepth = 100;

    RoutingGauge* rg         = Session::getRoutingGauge();
    size_t        movedDepth = rg->getLayerDepth(moved->getLayer());

    Component* anchor = getAnchor();
    if (anchor) {
      viaDepth = rg->getLayerDepth( anchor->getLayer() );
      ltrace(200) << "| Anchor depth: " << viaDepth << endl;
    }

    forEach ( AutoSegment*, isegment, const_cast<AutoContact*>(this)->getAutoSegments() ) {
      if (*isegment == moved) continue;

      size_t depth = rg->getLayerDepth(isegment->getLayer());
      if (viaDepth == 100) viaDepth = depth;
      else
        if (viaDepth != depth) return false;

      ltrace(200) << "| Segment depth: " << depth << endl;
    }

    return (movedDepth+1 == viaDepth);
  }


  void  AutoContact::setConstraintBox ( const Box& box )
  {
    setCBXMin ( box.getXMin() );
    setCBXMax ( box.getXMax() );
    setCBYMin ( box.getYMin() );
    setCBYMax ( box.getYMax() );
    ltrace(110) << "setConstraintBox() - " << this << " " << getConstraintBox() << endl;
    ltrace(110) << "* " << _gcell << endl;
  }


  bool  AutoContact::restrictConstraintBox ( DbU::Unit    constraintMin
                                           , DbU::Unit    constraintMax
                                           , unsigned int flags
                                           )
  {
    if (flags & KbHorizontal) {
      if ( (constraintMin > getCBYMax()) or (constraintMax < getCBYMin()) ) {
        if ( Session::isInDemoMode() or not (flags & KbWarnOnError) ) return false;

        cerr << Error ( "Incompatible DY restriction on %s", _getString().c_str() ) << endl;
        if ( constraintMin > getCBYMax() )
          cerr << Error ( "(constraintMin > CBYMax : %.2lf > %.2lf)"
                        , DbU::toLambda(constraintMin)
                        , DbU::toLambda(getCBYMax()) )
               << endl;
        if ( constraintMax < getCBYMin() )
          cerr << Error ( "(constraintMax < CBYMin : %.2lf < %.2lf)"
                        , DbU::toLambda(constraintMax)
                        , DbU::toLambda(getCBYMin()) )
               << endl;
        return false;
      }
      setCBYMin ( std::max(getCBYMin(),constraintMin) );
      setCBYMax ( std::min(getCBYMax(),constraintMax) );
    } else if (flags & KbVertical) {
      if ( (constraintMin > getCBXMax()) || (constraintMax < getCBXMin()) ) {
        if ( Session::isInDemoMode() or not (flags & KbWarnOnError) ) return false;

        cerr << Error ( "Incompatible DX restriction on %s", _getString().c_str() ) << endl;
        if ( constraintMin > getCBXMax() )
          cerr << Error ( "(constraintMin > CBXMax : %.2lf > %.2lf)"
                        , DbU::toLambda(constraintMin)
                        , DbU::toLambda(getCBXMax()) )
               << endl;
        if ( constraintMax < getCBXMin() )
          cerr << Error ( "(constraintMax < CBXMin : %.2lf < %.2lf)"
                        , DbU::toLambda(constraintMax)
                        , DbU::toLambda(getCBXMin()) )
               << endl;
        return false;
      }
      setCBXMin ( std::max(getCBXMin(),constraintMin) );
      setCBXMax ( std::min(getCBXMax(),constraintMax) );
    }
    ltrace(110) << "restrictConstraintBox() - " << this << " " << getConstraintBox() << endl;
    return true;
  }


  void  AutoContact::restoreNativeConstraintBox ()
  { setConstraintBox ( getNativeConstraintBox() ); }

                 
  Box& AutoContact::intersectConstraintBox ( Box& box ) const
  { return box = box.getIntersection ( getConstraintBox() ); }


  Box  AutoContact::getBoundingBox () const
  { return _gcell->getBoundingBox (); }


  void  AutoContact::translate ( const DbU::Unit& tx, const DbU::Unit& ty )
  {
    cerr << Warning("Calling AutoContact::translate() is likely a bug.") << endl;
    _contact->translate ( tx, ty );
  }


  string  AutoContact::_getTypeName () const
  { return "AutoContact"; }


  string  AutoContact::_getString () const
  {
    string s = _contact->_getString();
    size_t i = s.find(' '); 
    if (i != string::npos) {
      s.erase ( i+1, 7 );
      s.insert( i+1, _getTypeName() );
    }
  //s.insert( 1, "id: " );
  //s.insert( 4, getString(_id) );
    s.insert( s.size()-1, (isFixed           ())?" F":" -" );
    s.insert( s.size()-1, (isTerminal        ())? "T": "-" );
    s.insert( s.size()-1, (isHTee            ())? "h": "-" );
    s.insert( s.size()-1, (isVTee            ())? "v": "-" );
    s.insert( s.size()-1, (isInvalidated     ())? "i": "-" );
    s.insert( s.size()-1, (isInvalidatedCache())? "c": "-" );
    return s;
  }


  Record* AutoContact::_getRecord () const
  {
    Record* record = _contact->_getRecord ();
    record->add ( getSlot ( "_gcell"        , _gcell             ) );
    record->add ( getSlot ( "_constraintBox", getConstraintBox() ) );
    record->add ( getSlot ( "_flags"        , _flags             ) );
    return record;
  }


} // Katabatic namespace.
