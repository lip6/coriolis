// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AutoContact.cpp"                        |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <climits>
#include <sstream>
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Layer.h"
#include "hurricane/ViaLayer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/Net.h"
#include "hurricane/Plug.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/DebugSession.h"
#include "crlcore/RoutingGauge.h"
#include "anabatic/AutoContact.h"
#include "anabatic/AutoContactTerminal.h"
#include "anabatic/AutoContactTurn.h"
#include "anabatic/AutoContactHTee.h"
#include "anabatic/AutoContactVTee.h"
#include "anabatic/AutoVertical.h"
#include "anabatic/AutoHorizontal.h"
#include "anabatic/AnabaticEngine.h"
#include "anabatic/Session.h"


namespace Anabatic {

  using std::ostringstream;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DebugSession;
  using Hurricane::ForEachIterator;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoContact".


  size_t      AutoContact::_maxId       = 0;
  size_t      AutoContact::_allocateds  = 0;
  const Name  AutoContact::_goName      = "Anabatic::AutoContact";


  AutoContact::AutoContact ( GCell* gcell, Contact* contact )
    : _id        (contact->getId())
    , _contact   (contact)
    , _gcell     (gcell)
    , _flags     (CntInvalidatedCache|CntInCreationStage)
    , _xMin      (_gcell->getXMin())
    , _xMax      (_gcell->getConstraintXMax())
    , _yMin      (_gcell->getYMin())
    , _yMax      (_gcell->getConstraintYMax())
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
    restoreNativeConstraintBox();

    cdebug_log(145,0) << "Native CBox: " << this
                    << " <" << DbU::getValueString(getCBXMin())
                    << " "  << DbU::getValueString(getCBYMin())
                    << " "  << DbU::getValueString(getCBXMax())
                    << " "  << DbU::getValueString(getCBYMax()) << ">" << endl;
    
    Session::link( this );
    invalidate( Flags::Topology );

    cdebug_log(145,0) << "AutoContact::_postCreate() - " << this << " in " << _gcell << endl;
  }


  void  AutoContact::destroy ()
  {
    _preDestroy ();
    delete this;
  }


  void  AutoContact::_preDestroy ()
  {
    DebugSession::open( _contact->getNet(), 140, 150 );

    cdebug_log(145,0) << "AutoContact::_preDestroy() - <AutoContact id:" << _id << ">" << endl;

#if 0
    bool canDestroyBase = true;

    if (not _contact->getSlaveComponents().isEmpty()) {
      ostringstream message;
      message << "Base contact still have slaves components, cancelled.\n"
              << "        on: " << this;

      for ( Component* component : _contact->getSlaveComponents() ) {
        message << "\n          | " << component;
      }
      cerr << Error( message.str() ) << endl;

      canDestroyBase = false;
    }
#endif

    if (not Session::doDestroyTool()) {
      _gcell->removeContact( this );
      Session::unlink( this );
    }
        
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


  bool  AutoContact::canDestroy ( Flags flags ) const
  {
    if (not _contact->getSlaveComponents().isEmpty()) {
      if (flags & Flags::WarnOnError) {
        cerr << Error("Base contact still have slaves components, cancelled.\n"
                     "        (%s)"
                     ,_getString().c_str()) << endl;
      }
      return false;
    }
    return true;
  }


  const Name&     AutoContact::getName          () const { return _goName; }
  AutoSegments    AutoContact::getAutoSegments  () { return new AutoSegments_CachedOnContact(this); }
  AutoSegment*    AutoContact::getPerpandicular ( const AutoSegment* ) const { return NULL; }
  AutoHorizontal* AutoContact::getHorizontal1   () const { return NULL; }
  AutoHorizontal* AutoContact::getHorizontal2   () const { return NULL; }
  AutoVertical*   AutoContact::getVertical1     () const { return NULL; }
  AutoVertical*   AutoContact::getVertical2     () const { return NULL; }


  void  AutoContact::getDepthSpan ( size_t& minDepth, size_t& maxDepth ) const
  {
    minDepth = (size_t)-1;
    maxDepth = 0;

    Component* anchor = getAnchor ();
    if (anchor) {
      minDepth = std::min( minDepth, Session::getRoutingGauge()->getLayerDepth(anchor->getLayer()) );
      maxDepth = std::max( maxDepth, Session::getRoutingGauge()->getLayerDepth(anchor->getLayer()) );
    }

    for ( AutoSegment* segment : const_cast<AutoContact*>(this)->getAutoSegments() ) {
      minDepth = std::min( minDepth, Session::getRoutingGauge()->getLayerDepth(segment->getLayer()) );
      maxDepth = std::max( maxDepth, Session::getRoutingGauge()->getLayerDepth(segment->getLayer()) );
    }
  }


  void  AutoContact::getLengths ( DbU::Unit* lengths, AutoSegment::DepthLengthSet& processeds )
  {
    DbU::Unit hSideLength = getGCell()->getSide( Flags::Horizontal ).getSize();
    DbU::Unit vSideLength = getGCell()->getSide( Flags::Vertical   ).getSize();

    for ( AutoSegment* segment : getAutoSegments() ) {
      bool isSourceHook = (segment->getAutoSource() == this);

      if (processeds.find(segment) != processeds.end()) continue;
      processeds.insert( segment );

      size_t     depth  = Session::getRoutingGauge()->getLayerDepth(segment->getLayer());
      DbU::Unit  length;
      if (segment->isLocal()) {
        length = segment->getLength();
        lengths[depth] += length;

        DbU::Unit sideLength = (segment->isHorizontal()) ? hSideLength : vSideLength;
        if ( not segment->isUnbound() and (abs(length) > sideLength) )
          cerr << Error("Suspicious length:%.2f of %s."
                       ,DbU::getValueString(length).c_str()
                       ,getString(segment).c_str()) << endl;
      } else {
        if (segment->isHorizontal()) {
          if (isSourceHook)
            lengths[depth] += _gcell->getXMax() - segment->getSourceX();
          else
            lengths[depth] += segment->getTargetX() - _gcell->getXMin();
        } else {
          if (isSourceHook)
            lengths[depth] += _gcell->getYMax() - segment->getSourceY();
          else
            lengths[depth] += segment->getTargetY() - _gcell->getYMin();
        }
      }
    }
  }


  Box  AutoContact::getNativeConstraintBox () const
  {
    if (isUserNativeConstraints()) return getConstraintBox();
    if (isFixed()) return Box(_contact->getPosition());
    return _gcell->getConstraintBox();
  }


  Interval  AutoContact::getNativeUConstraints ( Flags direction ) const
  {
    Box       nativeConstraints = getNativeConstraintBox();
    Interval  constraint;
    if (direction & Flags::Horizontal) {
      constraint = Interval( nativeConstraints.getXMin(), nativeConstraints.getXMax() );
    } else {
      constraint = Interval( nativeConstraints.getYMin(), nativeConstraints.getYMax() );
    }
  //if (direction & Flags::NoGCellShrink) constraint.inflate( 0, GCell::getTopRightShrink() );
    return constraint;
  }


  Interval  AutoContact::getUConstraints ( Flags direction ) const
  {
    Interval  constraint;
    if (direction & Flags::Horizontal) {
      constraint = Interval( getCBXMin(), getCBXMax() );
    } else {
      constraint = Interval( getCBYMin(), getCBYMax() );
    }
  //if (direction & Flags::NoGCellShrink) constraint.inflate( 0, GCell::getTopRightShrink() );
    return constraint;
  }


  void  AutoContact::invalidate ( Flags flags )
  {
    if (not isInvalidated()) {
      cdebug_log(145,1) << "AutoContact::invalidate() - " << this << endl;
      setFlags( CntInvalidated );
      if (flags & Flags::Topology ) setFlags( CntInvalidatedCache );
      Session::invalidate( this );

      _invalidate( flags );

      getGCell()->invalidate();
      cdebug_tabw(145,-1);
    }
  }


  void  AutoContact::setGCell ( GCell* gcell )
  {
    invalidate();
    if (_gcell) _gcell->removeContact( this );

    _gcell = gcell;
    if (_gcell) {
      cdebug_log(145,0) << "AutoContact::setGCell() " << gcell << endl;
      _gcell->addContact( this );
      _contact->setPosition( _gcell->getCenter() );
      _xMin = _gcell->getXMin();
      _yMin = _gcell->getYMin();
      _xMax = _gcell->getConstraintXMax();
      _yMax = _gcell->getConstraintYMax();
      if (cdebug.enabled()) {
        cdebug_log(145,0) << "* deltas: [" << DbU::getValueString(_xMin)
                          << " "           << DbU::getValueString(_yMin)
                          << " "           << DbU::getValueString(_xMax)
                          << " "           << DbU::getValueString(_yMax)
                          << "]" << endl;
      }
    } else {
      cerr << Bug( "NULL GCell for %s.", _getString().c_str() ) << endl;
    }
  }


  void  AutoContact::updateSize ()
  {
    if (isInvalidatedWidth()) {
      size_t minDepth = 0;
      size_t maxDepth = 0;
      getDepthSpan( minDepth, maxDepth );

      if (getVertical1() and getVertical1()->isWide()) {
        size_t    vdepth = (Session::getLayerDepth(getVertical1()->getLayer()) == maxDepth) ? maxDepth : minDepth; 
        DbU::Unit width  = getVertical1()->getWidth();
        width += Session::getViaWidth(vdepth) - Session::getWireWidth(vdepth); 
        setWidth( width );
      }
      
      if (getHorizontal1() and getHorizontal1()->isWide()) {
        size_t    hdepth = (Session::getLayerDepth(getHorizontal1()->getLayer()) == maxDepth) ? maxDepth : minDepth; 
        DbU::Unit width  = getHorizontal1()->getWidth();
        width += Session::getViaWidth(hdepth) - Session::getWireWidth(hdepth); 
        setHeight( width );
      }
      unsetFlags ( CntInvalidatedWidth );
    }
  }


  void  AutoContact::_getTopology ( Contact* support, Component*& anchor, Horizontal**& horizontals, Vertical**& verticals, size_t size )
  {
    size_t hcount = 0;
    size_t vcount = 0;

    for ( size_t i=0 ; i<size ; ++i ) {
      horizontals[i] = NULL;
      verticals  [i] = NULL;
    }

    anchor = support->getAnchor();

    for ( Component* component : support->getSlaveComponents() ) {
      Horizontal* h = dynamic_cast<Horizontal*>(component);
      if (h != NULL) {
        if (hcount < size) horizontals[hcount++] = h;
      } else {
        Vertical* v = dynamic_cast<Vertical*>(component);
        if ( (v != NULL) and (vcount < size) ) verticals[vcount++] = v;
      }
    }
  }


  void  AutoContact::showTopologyError ( const std::string& message, Flags flags )
  {
    Component*    anchor      = NULL;
    Horizontal**  horizontals = new Horizontal* [10];
    Vertical**    verticals   = new Vertical*   [10];

    if (not (flags & Flags::CParanoid)) cparanoid.setStreamMask( mstream::PassThrough );

    _getTopology ( base(), anchor, horizontals, verticals, 10 );
 
    cparanoid << Error("In topology of %s",getString(this).c_str()) << endl;
    if (anchor) cparanoid << "        A: " << anchor << endl;

    for ( size_t i=0 ; (i<10) and (horizontals[i] != NULL); ++i ) {
      AutoSegment* autoSegment = Session::lookup ( horizontals[i] );
      if (autoSegment != NULL)
        cparanoid << "        " << (autoSegment->isGlobal()?'G':'L') << ": " << autoSegment << endl; 
      else
        cparanoid << "        ?: " << horizontals[i] << endl; 
    }

    for ( size_t i=0 ; (i<10) and (verticals[i] != NULL); ++i ) {
      AutoSegment* autoSegment = Session::lookup ( verticals[i] );
      if (autoSegment != NULL)
        cparanoid << "        " << (autoSegment->isGlobal()?'G':'L') << ": " << autoSegment << endl; 
      else
        cparanoid << "        ?: " << verticals[i] << endl; 
    }

    cparanoid << "        " << message  << endl;
    if (not (flags & Flags::CParanoid)) cparanoid.unsetStreamMask( mstream::PassThrough );

    delete [] horizontals;
    delete [] verticals;
  }


  void  AutoContact::checkTopology ()
  {
  //cdebug_log(145,0) << "checkTopology() NOT RE-IMPLEMENTED YET " << this << endl;
  }


  void  AutoContact::forceOnGrid ( Point )
  {
    cerr << Warning( "AutoContact::forcedOnGrid() not implemented for this derived class.\n"
                     "          %s\n"
                   , getString(this).c_str()
                   ) << endl;
  }



  bool  AutoContact::isTee ( Flags direction ) const
  {
    return (isHTee() and (direction & Flags::Horizontal))
        or (isVTee() and (direction & Flags::Vertical  ));
  }


  bool  AutoContact::canMoveUp ( const AutoSegment* moved ) const
  {
    cdebug_log(149,0) << "AutoContact::canMoveUp() " << this << endl;
    size_t viaDepth = 100;

    RoutingGauge* rg         = Session::getRoutingGauge();
    size_t        movedDepth = rg->getLayerDepth(moved->getLayer());

    Component* anchor = getAnchor();
    if (anchor) {
      viaDepth = rg->getLayerDepth( anchor->getLayer() );
      cdebug_log(149,0) << "| Anchor depth: " << viaDepth << endl;
    }

    for ( AutoSegment* segment : const_cast<AutoContact*>(this)->getAutoSegments() ) {
      if (segment == moved) continue;

      size_t depth = rg->getLayerDepth(segment->getLayer());
      if (viaDepth == 100) viaDepth = depth;
      else
        if (viaDepth != depth) return false;

      cdebug_log(149,0) << "| Segment depth: " << depth << endl;
    }

    return (movedDepth+1 == viaDepth);
  }


  void  AutoContact::setConstraintBox ( const Box& box )
  {
    setCBXMin ( box.getXMin() );
    setCBXMax ( box.getXMax() );
    setCBYMin ( box.getYMin() );
    setCBYMax ( box.getYMax() );
    cdebug_log(149,0) << "setConstraintBox() - " << this << " " << getConstraintBox()
                      << " from:" << box << endl;
    cdebug_log(149,0) << "* " << _gcell << endl;
  }


  bool  AutoContact::restrictConstraintBox ( DbU::Unit  constraintMin
                                           , DbU::Unit  constraintMax
                                           , Flags      flags
                                           )
  {
    cdebug_log(149,0) << "restrictConstraintBox() - " << this << " " << getConstraintBox() << endl;
    if (flags & Flags::Horizontal) {
      if ( (constraintMin > getCBYMax()) or (constraintMax < getCBYMin()) ) {
        if ( Session::isInDemoMode() or not (flags & Flags::WarnOnError) ) return false;

        cerr << Error ( "Incompatible DY restriction on %s", _getString().c_str() ) << endl;
        if ( constraintMin > getCBYMax() )
          cerr << Error ( "(constraintMin > CBYMax : %s > %s)"
                        , DbU::getValueString(constraintMin).c_str()
                        , DbU::getValueString(getCBYMax()).c_str() )
               << endl;
        if ( constraintMax < getCBYMin() )
          cerr << Error ( "(constraintMax < CBYMin : %s < %s)"
                        , DbU::getValueString(constraintMax).c_str()
                        , DbU::getValueString(getCBYMin()).c_str() )
               << endl;
        return false;
      }
      setCBYMin ( std::max(getCBYMin(),constraintMin) );
      setCBYMax ( std::min(getCBYMax(),constraintMax) );
    } else if (flags & Flags::Vertical) {
      if ( (constraintMin > getCBXMax()) || (constraintMax < getCBXMin()) ) {
        if ( Session::isInDemoMode() or not (flags & Flags::WarnOnError) ) return false;

        cerr << Error ( "Incompatible DX restriction on %s", _getString().c_str() ) << endl;
        if ( constraintMin > getCBXMax() )
          cerr << Error ( "(constraintMin > CBXMax : %s > %s)"
                        , DbU::getValueString(constraintMin).c_str()
                        , DbU::getValueString(getCBXMax()).c_str() )
               << endl;
        if ( constraintMax < getCBXMin() )
          cerr << Error ( "(constraintMax < CBXMin : %s < %s)"
                        , DbU::getValueString(constraintMax).c_str()
                        , DbU::getValueString(getCBXMin()).c_str() )
               << endl;
        return false;
      }
      setCBXMin ( std::max(getCBXMin(),constraintMin) );
      setCBXMax ( std::min(getCBXMax(),constraintMax) );
    }
    cdebug_log(149,0) << "restrictConstraintBox() - " << this << " " << getConstraintBox() << endl;
    return true;
  }


  void  AutoContact::restoreNativeConstraintBox ()
  { setConstraintBox ( getNativeConstraintBox() ); }

                 
  Box& AutoContact::intersectConstraintBox ( Box& box ) const
  { return box = box.getIntersection ( getConstraintBox() ); }


  void  AutoContact::migrateConstraintBox ( AutoContact* other )
  {
    if (_gcell != other->_gcell) {
      cerr << Error( "AutoContact::migrateConstraintBox(): AutoContacts do not belongs to the same GCell:\n"
                     "        from: %s\n"
                     "        to:   %s"
                   , getString(other).c_str()
                   , getString(this ).c_str()
                   ) << endl;
      return;
    }

    setConstraintBox( other->getConstraintBox() );
    other->restoreNativeConstraintBox();
  }


  Box  AutoContact::getBoundingBox () const
  { return _gcell->getBoundingBox (); }


  void  AutoContact::translate ( const DbU::Unit& tx, const DbU::Unit& ty )
  {
    cerr << Warning("Calling AutoContact::translate() is likely a bug.") << endl;
    _contact->translate ( tx, ty );
  }


  AutoContact* AutoContact::createFrom ( Contact* hurricaneContact )
  {
    AutoContact* autoContact = NULL;
    Component*   anchor;
    size_t       hSize       = 0;
    size_t       vSize       = 0;
    Horizontal** horizontals = new Horizontal* [4];
    Vertical**   verticals   = new Vertical*   [4];
    GCell*       gcell       = Session::getAnabatic()->getGCellUnder( hurricaneContact->getCenter() );

    if (not gcell) {
      throw Error("AutoContact::createFrom( %s ):\n"
                  "        Contact is *not* under a GCell (outside routed area?)"
                 , getString(hurricaneContact).c_str()
                 );
    }

    _getTopology ( hurricaneContact, anchor, horizontals, verticals, 4 );

    for ( size_t i=0 ; i<4 ; ++i ) {
      hSize += (horizontals[i] != NULL) ? 1 : 0;
      vSize += (verticals  [i] != NULL) ? 1 : 0;
    }

    if (anchor) {
      if (hSize+vSize == 1) {
        autoContact = new AutoContactTerminal( gcell, hurricaneContact );
        autoContact->_postCreate();
        autoContact->unsetFlags( CntInCreationStage );
      }
    } else {
      if ((hSize == 1) and (vSize == 1)) {
        autoContact = new AutoContactTurn ( gcell, hurricaneContact );
        autoContact->_postCreate();
        autoContact->unsetFlags( CntInCreationStage );
      } else if ((hSize == 2) and (vSize == 1)) {
        autoContact = new AutoContactHTee ( gcell, hurricaneContact );
        autoContact->_postCreate();
        autoContact->unsetFlags( CntInCreationStage );
      } else if ((hSize == 1) and (vSize == 2)) {
        autoContact = new AutoContactVTee ( gcell, hurricaneContact );
      }
    }

    if (not autoContact) {
      throw Error("AutoContact::createFrom( %s ):\n"
                  "        Contact do not have a manageable topology (a:%u, h:%u, v:%u)"
                 , getString(hurricaneContact).c_str()
                 , ((anchor) ? 1 : 0)
                 , hSize
                 , vSize
                 );
    }

    autoContact->_postCreate();
    autoContact->unsetFlags( CntInCreationStage );

    return autoContact;
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

  //s.insert( s.size()-1, getString(getConstraintBox()));
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


} // Anabatic namespace.
