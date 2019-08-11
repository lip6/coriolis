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
// |  C++ Module  :       "./AutoHorizontal.cpp"                     |
// +-----------------------------------------------------------------+


#include  <algorithm>
#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "hurricane/DebugSession.h"
#include  "hurricane/ViaLayer.h"
#include  "hurricane/RoutingPad.h"
#include  "crlcore/RoutingGauge.h"
#include  "anabatic/Configuration.h"
#include  "anabatic/AutoContactTerminal.h"
#include  "anabatic/AutoContactTurn.h"
#include  "anabatic/AutoHorizontal.h"
#include  "anabatic/AutoVertical.h"


namespace Anabatic {


  using std::min;
  using std::max;
  using Hurricane::Error;
  using Hurricane::Bug;
  using Hurricane::DebugSession;
  using Hurricane::ViaLayer;
  using Hurricane::RoutingPad;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoHorizontal".


  Segment*    AutoHorizontal::base          ()       { return _horizontal; }
  Segment*    AutoHorizontal::base          () const { return _horizontal; }
  Horizontal* AutoHorizontal::getHorizontal ()       { return _horizontal; }
  DbU::Unit   AutoHorizontal::getSourceU    () const { return _horizontal->getSourceX(); }
  DbU::Unit   AutoHorizontal::getTargetU    () const { return _horizontal->getTargetX(); }
  DbU::Unit   AutoHorizontal::getDuSource   () const { return _horizontal->getDxSource(); }
  DbU::Unit   AutoHorizontal::getDuTarget   () const { return _horizontal->getDxTarget(); }
  Interval    AutoHorizontal::getSpanU      () const { return Interval(_horizontal->getSourceX(),_horizontal->getTargetX()); }
  void        AutoHorizontal::setDuSource   ( DbU::Unit du ) { _horizontal->setDxSource(du); }
  void        AutoHorizontal::setDuTarget   ( DbU::Unit du ) { _horizontal->setDxTarget(du); }
  string      AutoHorizontal::_getTypeName  () const { return "AutoHorizontal"; }


  AutoHorizontal::AutoHorizontal ( Horizontal* horizontal )
    : AutoSegment(horizontal)
    , _horizontal(horizontal)
  {
    cdebug_log(145,0) << "CTOR AutoHorizontal " << this << endl;
    cdebug_log(145,0) << "               over " << horizontal << endl;
  }


  void  AutoHorizontal::_postCreate ()
  {
    AutoSegment::_postCreate ();

    AutoContact* source = getAutoSource();
    if (source->isTerminal()) source->setY( _horizontal->getY() );

    AutoContact* target = getAutoTarget();
    if (target->isTerminal()) target->setY( _horizontal->getY() );

    _gcell = source->getGCell();

    setOptimalMax( getGCell()->getYMax() );
    resetNativeConstraints( getGCell()->getYMin(), getGCell()->getConstraintYMax() );

    if (getGCell() != target->getGCell()) {
      setFlags( SegGlobal );

      vector<GCell*> gcells;
      getGCells( gcells );
      for ( GCell* gcell : gcells ) {
        if ( (gcell != getGCell()) and (gcell != target->getGCell()) )
          gcell->addHSegment( this );
        mergeNativeMin( gcell->getYMin() );
        mergeNativeMax( gcell->getConstraintYMax() );
      }
    }
  }


  void  AutoHorizontal::_preDestroy ()
  {
    cdebug_log(149,0) << "AutoHorizontal::_preDestroy() - <id:" << getId() << "> " << endl;
    cdebug_log(149,0) << "  " << _getString() << endl;
    cdebug_tabw(145,1);

    if (not Session::doDestroyTool()) {
      vector<GCell*> gcells;
      getGCells( gcells );
      for ( GCell* gcell : gcells ) gcell->removeHSegment( this );
    }

    AutoSegment::_preDestroy ();
    cdebug_tabw(145,-1);
  }


  AutoHorizontal::~AutoHorizontal ()
  {
    if ( Session::doDestroyBaseSegment() and not Session::doDestroyTool() ) {
      cdebug_log(149,0) << "~AutoHorizontal() - " << endl;
      _horizontal->destroy ();
    }
  }


  Interval  AutoHorizontal::getSourceConstraints ( Flags flags ) const
  {
    if (flags & Flags::NativeConstraints) {
      Box nativeBox ( getAutoSource()->getNativeConstraintBox() );
      return Interval ( nativeBox.getYMin(), nativeBox.getYMax() );
    }
    return Interval ( getAutoSource()->getCBYMin(), getAutoSource()->getCBYMax() );
  }


  Interval  AutoHorizontal::getTargetConstraints ( Flags flags ) const
  {
    if (flags & Flags::NativeConstraints) {
      Box nativeBox ( getAutoTarget()->getNativeConstraintBox() );
      return Interval ( nativeBox.getYMin(), nativeBox.getYMax() );
    }
    return Interval ( getAutoTarget()->getCBYMin(), getAutoTarget()->getCBYMax() );
  }


  bool  AutoHorizontal::getConstraints ( DbU::Unit& constraintMin, DbU::Unit& constraintMax ) const
  {
    cdebug_log(155,1) << "getConstraints() " << this << endl;

    constraintMin = getNativeMin();
    constraintMax = getNativeMax();

    cdebug_log(155,0) << "Native constraints: ["
                      << DbU::getValueString(constraintMin) << ":"
                      << DbU::getValueString(constraintMax) << "]"
                      << endl;

    constraintMin = std::max ( constraintMin, getAutoSource()->getCBYMin() );
    constraintMax = std::min ( constraintMax, getAutoSource()->getCBYMax() );
    cdebug_log(155,0) << "Merge with source constraints: ["
                      << DbU::getValueString(getAutoSource()->getCBYMin()) << ":"
                      << DbU::getValueString(getAutoSource()->getCBYMax()) << "]"
                      << endl;

    constraintMin = std::max ( constraintMin, getAutoTarget()->getCBYMin() );
    constraintMax = std::min ( constraintMax, getAutoTarget()->getCBYMax() );
    cdebug_log(155,0) << "Merge with target constraints: ["
                      << DbU::getValueString(getAutoTarget()->getCBYMin()) << ":"
                      << DbU::getValueString(getAutoTarget()->getCBYMax()) << "]"
                      << endl;

    Interval userConstraints = getUserConstraints();
    if (not userConstraints.isEmpty()) {
      constraintMin = std::max ( constraintMin, userConstraints.getVMin() );
      constraintMax = std::min ( constraintMax, userConstraints.getVMax() );

      cdebug_log(155,0) << "Merge with user constraints: ["
                        << DbU::getValueString(userConstraints.getVMin()) << ":"
                        << DbU::getValueString(userConstraints.getVMax()) << "]"
                        << endl;
    } else
      cdebug_log(155,0) << "Empty user constraints" << endl;

    cdebug_log(155,0) << "Resulting constraints: ["
                      << DbU::getValueString(constraintMin) << ":"
                      << DbU::getValueString(constraintMax) << "]"
                      << endl;

    if (constraintMin > constraintMax)
      cerr << Error( "AutoHorizontal::getConstraints(): Invalid interval [%s : %s] -> [%d : %d]\n"
                     "        on %s"
                   , DbU::getValueString(constraintMin).c_str()
                   , DbU::getValueString(constraintMax).c_str()
                   , constraintMin
                   , constraintMax
                   , getString(this).c_str()
                   ) << endl;
    
    cdebug_tabw(155,-1);
    return true;
  }


  Flags  AutoHorizontal::getDirection () const
  { return Flags::Horizontal; }


  bool  AutoHorizontal::getGCells ( vector<GCell*>& gcells ) const
  {
    vector<GCell*>().swap( gcells );

    bool      success = true;
    DbU::Unit yprobe  = getY();
    GCell*    gcell   = getAutoSource()->getGCell();
    GCell*    end     = getAutoTarget()->getGCell();

    cdebug_log(144,0) << "yprobe: " << DbU::getValueString(yprobe) << endl;

    if (gcell->getXMin() > end->getXMin()) std::swap( gcell, end );
    if (yprobe == gcell->getConstraintYMax()) yprobe--;

    gcells.push_back( gcell );

    while ( gcell != end ) {
      gcell = gcell->getEast( yprobe );
      if (not gcell) {
        success = false;
        cerr << Error( "AutoHorizontal::getGCells() : NULL GCell under %s\n"
                       "        begin:%s\n"
                       "        end:  %s"
                     , getString(this).c_str()
                     , getString(getAutoSource()->getGCell()).c_str()
                     , getString(getAutoTarget()->getGCell()).c_str()
                     ) << endl;
        break;
      }

      gcells.push_back( gcell );
    }

    return success;
  }


  bool  AutoHorizontal::_canSlacken () const
  {
    cdebug_tabw(149,1);

    Interval sourceSide        = getAutoSource()->getGCell()->getSide( Flags::Vertical );
    Interval targetSide        = getAutoTarget()->getGCell()->getSide( Flags::Vertical );
    Interval sourceConstraints = Interval(getAutoSource()->getCBYMin(),getAutoSource()->getCBYMax());
    Interval targetConstraints = Interval(getAutoTarget()->getCBYMin(),getAutoTarget()->getCBYMax());
    bool     sourceGoStraight  = getAutoSource()->getGCell()->isGoStraight();
    bool     targetGoStraight  = getAutoTarget()->getGCell()->isGoStraight();

  // Expand by a tiny amount for the "contains" to work for sure.
    sourceConstraints.inflate( 1 );
    targetConstraints.inflate( 1 );

    cdebug_log(149,0) << "source " << getAutoSource() << endl;
    cdebug_log(149,0) << "source constraints: " << sourceConstraints
                << " " << DbU::getValueString(sourceConstraints.getSize()) << endl;
    cdebug_log(149,0) << "target " << getAutoTarget() << endl;
    cdebug_log(149,0) << "target constraints: " << targetConstraints
                << " " << DbU::getValueString(targetConstraints.getSize()) << endl;

    if (not sourceGoStraight and not sourceConstraints.contains(sourceSide)) { cdebug_tabw(149,-1); return true; }
    if (not targetGoStraight and not targetConstraints.contains(targetSide)) { cdebug_tabw(149,-1); return true; }

    cdebug_tabw(149,-1);
    return false;
  }


  bool  AutoHorizontal::_slacken ( Flags flags )
  {
    cdebug_log(149,0) << "AutoHorizontal::_slacken() " << this << endl;

    if (not isStrongTerminal()) return false;

    const Configuration* configuration = Session::getConfiguration();
    const Layer*         metal2        = configuration->getRoutingLayer( 1 );

    bool         success        = false;
    bool         isMetal2Source = false;
    bool         isMetal2Target = false;
    DbU::Unit    height         = 0;
    AutoContact* source         = getAutoSource();
    AutoContact* target         = getAutoTarget();
    if (source->isTerminal()) {
      height = (static_cast<RoutingPad*>(source->getAnchor()))->getBoundingBox().getHeight();
      isMetal2Source = (source->getLayer() == metal2);
    }
    if (target->isTerminal()) {
      height = std::min( height, (static_cast<RoutingPad*>(target->getAnchor()))->getBoundingBox().getHeight() );
      isMetal2Target = (target->getLayer() == metal2);
    }

    if (height >= 4*getPitch()) {
      if (not (_flags & (SegGlobal|SegWeakGlobal)) and (getLength() < 5*getPitch()))
        return false;
    }

    cdebug_tabw(149,1);
    cdebug_log(149,0) << "_flags:" << (_flags & (SegGlobal|SegWeakGlobal)) << endl;
    cdebug_log(149,0) << "test:" << (getLength() < 5*getPitch()) << endl;
    cdebug_log(149,0) << "length:" << DbU::getValueString(getLength()) << endl;

    int          lowSlack        = (flags & Flags::HalfSlacken) ? 3 : 10;
    bool         sourceSlackened = false;
    bool         targetSlackened = false;
    bool         halfSlackened   = false;
    DbU::Unit    targetPosition  = getTargetPosition();
    AutoSegment* parallel        = this;

    if (source->isTerminal()) {
      Interval  perpandConstraints = getAutoTarget()->getUConstraints(Flags::Horizontal);
      Interval  constraints        = source->getUConstraints      (Flags::Vertical|Flags::NoGCellShrink);
      Interval  nativeConstraints  = source->getNativeUConstraints(Flags::Vertical|Flags::NoGCellShrink);
      int       slack              = constraints.getSize()       / getPitch();
      int       nativeSlack        = nativeConstraints.getSize() / getPitch();

      cdebug_log(149,0) << "Source constraint: " << constraints
                  << " slack:"        << slack
                  << " native slack:" << nativeSlack << endl;
      cdebug_log(149,0) << "Perpand constraints on target: " << perpandConstraints << endl; 
    // Ugly: GCell's track number is hardwired.
      if ((nativeSlack < lowSlack) or (nativeSlack - slack < 3)) {
        cdebug_log(149,0) << "Slackening from Source: " << source << endl;
        _makeDogleg( source->getGCell(), Flags::NoFlags );
        sourceSlackened = true;
      } else if (slack < 10) {
        halfSlackened = true;
      }

      const vector<AutoSegment*>& doglegs = Session::getDoglegs();
      if (sourceSlackened and (doglegs.size() >= 2)) {
        cdebug_log(149,0) << "Slackened from source @" << DbU::getValueString(getSourcePosition()) << endl;
        doglegs[doglegs.size()-2]->setAxis( getSourcePosition() );
        success = true;

        if (isMetal2Source) {
          cdebug_log(149,0) << "Fixing on source terminal contact."
                      << doglegs[doglegs.size()-2]->getAutoSource() << endl;
        //doglegs[doglegs.size()-2]->getAutoSource()->setFlags( CntFixed );
          doglegs[doglegs.size()-2]->getAutoSource()->setConstraintBox( source->getConstraintBox() );
          doglegs[doglegs.size()-2]->getAutoSource()->setFlags( CntUserNativeConstraints );
        }

        parallel = doglegs[ doglegs.size()-1 ];
      }
    }

    if (parallel) target = parallel->getAutoTarget();

    if (target->isTerminal()) {
      Interval  constraints       = target->getUConstraints      (Flags::Vertical|Flags::NoGCellShrink);
      Interval  nativeConstraints = target->getNativeUConstraints(Flags::Vertical|Flags::NoGCellShrink);
      int       slack             = constraints.getSize()       / getPitch();
      int       nativeSlack       = nativeConstraints.getSize() / getPitch();

    // Ugly: GCell's track number is hardwired.
      cdebug_log(149,0) << "Target constraint: " << constraints
                  << " slack:"        << slack
                  << " native slack:" << nativeSlack << endl;
      if ((nativeSlack < lowSlack) or (nativeSlack - slack < 3)) {
        cdebug_log(149,0) << "Slackening from Target: " << target << endl;
        parallel->_makeDogleg( target->getGCell(), Flags::NoFlags );
        targetSlackened = true;
      } else if (slack < 10) {
        halfSlackened = true;
      }

      const vector<AutoSegment*>& doglegs = Session::getDoglegs();
      if (targetSlackened and (doglegs.size() >= 2)) {
        cdebug_log(149,0) << "AutoHorizontal::_slacken(): Target @" << DbU::getValueString(targetPosition) << endl;
        doglegs[doglegs.size()-2]->setAxis( targetPosition );
        success = true;

        if (isMetal2Target) {
          cdebug_log(149,0) << "Fixing on target terminal contact: "
                      << doglegs[doglegs.size()-2]->getAutoTarget() << endl;
        //doglegs[doglegs.size()-2]->getAutoTarget()->setFlags( CntFixed );
          doglegs[doglegs.size()-2]->getAutoTarget()->setConstraintBox( target->getConstraintBox() );
          doglegs[doglegs.size()-2]->getAutoTarget()->setFlags( CntUserNativeConstraints );
        }
      }
    }

    if (sourceSlackened and targetSlackened) {
      setFlags  ( SegSlackened );
      unsetFlags( SegHalfSlackened );
    } else {
      if (sourceSlackened or targetSlackened) {
        if (halfSlackened) {
          setFlags( SegHalfSlackened );
        } else {
          setFlags  ( SegSlackened );
          unsetFlags( SegHalfSlackened );
        }
      }
    }

    cdebug_tabw(149,-1);

    return success;
  }


  void  AutoHorizontal::_setAxis ( DbU::Unit axis )
  {
    setFlags( SegAxisSet );

    if ((axis != getAxis()) and isFixed()) {
      cerr << Error( "AutoHorizontal::setAxis(): Cannot move fixed segment to %s.\n"
                     "        (on: %s)"
                   , DbU::getValueString(axis).c_str()
                   , _getString().c_str()
                   ) << endl;
    }

    if (_horizontal->getY() == axis) return;

    cdebug_log(144,0) << "_setAxis() @Y " << DbU::getValueString(axis) << " " << this << endl;

    _horizontal->setY( axis );
    invalidate();

    AutoContact* anchor = getAutoSource();
    anchor->invalidate();
    if (anchor->isTerminal()) anchor->setY( axis );

    anchor = getAutoTarget();
    anchor->invalidate();
    if (anchor->isTerminal()) anchor->setY( axis );
  }


  void  AutoHorizontal::updateOrient ()
  {
    if (_horizontal->getTargetX() < _horizontal->getSourceX()) {
      cdebug_log(145,0) << "updateOrient() " << this << " (before S/T swap)" << endl;
      _horizontal->invert();

      uint64_t spinFlags = _flags & SegDepthSpin;
      unsetFlags( SegDepthSpin );
      if (spinFlags & SegSourceTop   ) setFlags( SegTargetTop );
      if (spinFlags & SegSourceBottom) setFlags( SegTargetBottom );
      if (spinFlags & SegTargetTop   ) setFlags( SegSourceTop );
      if (spinFlags & SegTargetBottom) setFlags( SegSourceBottom );

      uint64_t invalidatedFlags = _flags & (SegInvalidatedSource|SegInvalidatedTarget);
      unsetFlags( SegInvalidatedSource|SegInvalidatedTarget );
      if (invalidatedFlags & SegInvalidatedSource) setFlags( SegInvalidatedTarget );
      if (invalidatedFlags & SegInvalidatedTarget) setFlags( SegInvalidatedSource );

      uint64_t terminalFlags = _flags & SegStrongTerminal;
      unsetFlags( SegStrongTerminal );
      if (terminalFlags & SegSourceTerminal) setFlags( SegTargetTerminal );
      if (terminalFlags & SegTargetTerminal) setFlags( SegSourceTerminal );
    }
  }


  void  AutoHorizontal::updatePositions ()
  {
    _sourcePosition = _horizontal->getSourceX() - getExtensionCap(Flags::Source);
    _targetPosition = _horizontal->getTargetX() + getExtensionCap(Flags::Target);
  }


  void  AutoHorizontal::updateNativeConstraints ()
  {
    vector<GCell*> gcells;
    getGCells( gcells );

    resetNativeConstraints( gcells[0]->getYMin(), gcells[0]->getConstraintYMax() );
    for ( GCell* gcell : gcells ) {
      mergeNativeMin( gcell->getYMin() );
      mergeNativeMax( gcell->getConstraintYMax() );
    }
  }


  bool  AutoHorizontal::checkPositions () const
  {
    bool      coherency      = true;
    DbU::Unit sourcePosition = _horizontal->getSourceX() - getExtensionCap(Flags::Source);
    DbU::Unit targetPosition = _horizontal->getTargetX() + getExtensionCap(Flags::Target);

    if ( _sourcePosition != sourcePosition ) {
      cerr << "extensionCap: " << DbU::getValueString(getExtensionCap(Flags::Source)) << endl;
      cerr << "ppitch:       " << DbU::getValueString(getPPitch()) << endl;
      cerr << "via width:    " << DbU::getValueString(Session::getViaWidth(getLayer())) << endl;
      cerr << Error ( "%s\n        Source position incoherency: "
                      "shadow: %s, real: %s."
                    , _getString().c_str() 
                    , DbU::getValueString(_sourcePosition).c_str()
                    , DbU::getValueString( sourcePosition).c_str()
                    ) << endl;
      coherency = false;
    }

    if ( _targetPosition != targetPosition ) {
      cerr << Error ( "%s\n        Target position incoherency: "
                      "shadow: %s, real: %s."
                    , _getString().c_str() 
                    , DbU::getValueString(_targetPosition).c_str()
                    , DbU::getValueString( targetPosition).c_str()
                    ) << endl;
      coherency = false;
    }

    return coherency;
  }


  bool  AutoHorizontal::checkConstraints () const
  {
    Interval sourceConstraints = Interval(getAutoSource()->getCBYMin(),getAutoSource()->getCBYMax());
    Interval targetConstraints = Interval(getAutoTarget()->getCBYMin(),getAutoTarget()->getCBYMax());

    if (not sourceConstraints.intersect(targetConstraints)) {
      cerr << Error ( "%s\n        Constraints incoherency:\n"
                      "          S:%s %s\n"
                      "          T:%s %s"
                    , _getString().c_str() 
                    , getString(sourceConstraints).c_str()
                    , getString(getAutoSource()).c_str()
                    , getString(targetConstraints).c_str()
                    , getString(getAutoTarget()).c_str()
                    ) << endl;
      return false;
    }

    return true;
  }


  bool  AutoHorizontal::canMoveULeft ( float reserve ) const
  {
#if THIS_IS_DISABLED
  //cerr << "canMoveULeft() " << this << endl;

    if (not isGlobal()) return false;
    if (not getAutoSource()->isTurn() or not getAutoTarget()->isTurn()) return false;
    if (not getAutoSource()->getGCell()->getDown()) return false;

    AutoContact* autoSource        = getAutoSource();
    AutoContact* autoTarget        = getAutoTarget();
    AutoSegment* perpandiculars[2] = { autoSource->getSegment(0), autoTarget->getSegment(0) };

    if (   ( (not perpandiculars[0]->isGlobal()) or (perpandiculars[0]->getAutoSource() == autoSource) )
       and ( (not perpandiculars[1]->isGlobal()) or (perpandiculars[1]->getAutoSource() == autoTarget) ) )
      return false;

    GCell*        begin             = autoSource->getGCell();
    GCell*        end               = autoTarget->getGCell();
    unsigned int  depth             = Session::getRoutingGauge()->getLayerDepth( getLayer() );
    float         currMaxDensity    = 0.0;
    float         leftMaxDensity    = 0.0;

  //cerr << "| begin:" << begin << endl;
  //cerr << "| end:  " << end << endl;
    for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getRight() ) {
    //cerr << "| gcell:" << gcell << endl;
      if (currMaxDensity < gcell->getWDensity(depth)) currMaxDensity = gcell->getWDensity( depth );
    }
            
    begin = begin->getDown();
    end   = end  ->getDown();

    for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getRight() ) {
      if (leftMaxDensity < gcell->getWDensity(depth)) leftMaxDensity = gcell->getWDensity( depth );
    }

    return (leftMaxDensity + reserve < currMaxDensity);
#endif

    return false;
  }


  bool  AutoHorizontal::canMoveURight ( float reserve ) const
  {
#if THIS_IS_DISABLED
  //cerr << "canMoveURight() " << this << endl;

    if (not isGlobal()) return false;
    if (not getAutoSource()->isTurn() or not getAutoTarget()->isTurn()) return false;
    if (not getAutoSource()->getGCell()->getUp()) return false;

    AutoContact* autoSource        = getAutoSource();
    AutoContact* autoTarget        = getAutoTarget();
    AutoSegment* perpandiculars[2] = { autoSource->getSegment(0), autoTarget->getSegment(0) };

    if (   ( (not perpandiculars[0]->isGlobal()) or (perpandiculars[0]->getAutoTarget() == autoSource) )
       and ( (not perpandiculars[1]->isGlobal()) or (perpandiculars[1]->getAutoTarget() == autoTarget) ) )
      return false;

    GCell*        begin             = autoSource->getGCell();
    GCell*        end               = autoTarget->getGCell();
    unsigned int  depth             = Session::getRoutingGauge()->getLayerDepth( getLayer() );
    float         currMaxDensity    = 0.0;
    float         leftMaxDensity    = 0.0;

  //cerr << "| begin:" << begin << endl;
  //cerr << "| end:  " << end << endl;

    for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getRight() ) {
    //cerr << "| gcell: " << gcell << endl;
      if (currMaxDensity < gcell->getWDensity(depth)) currMaxDensity = gcell->getWDensity( depth );
    }
            
    begin = begin->getUp();
    end   = end  ->getUp();

    for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getRight() ) {
      if (leftMaxDensity < gcell->getWDensity(depth)) leftMaxDensity = gcell->getWDensity( depth );
    }

    return (leftMaxDensity + reserve < currMaxDensity);
#endif

    return false;
  }


  bool  AutoHorizontal::moveULeft ()
  {
#if THIS_IS_DISABLED
    if (not getAutoSource()->isTurn() or not getAutoTarget()->isTurn()) return false;
    if (not getAutoSource()->getGCell()->getDown()) return false;

    AutoContact* autoSource        = getAutoSource();
    AutoContact* autoTarget        = getAutoTarget();
    GCell*       begin             = autoSource->getGCell();
    GCell*       end               = autoTarget->getGCell();
    AutoSegment* perpandicular     = autoSource->getSegment(2);

    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Anabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoSource() == autoSource) {
        begin->addVSegment( perpandicular );
      } else {
        if (begin->getDown() == perpandicular->getAutoSource()->getGCell()) {
          perpandicular->unsetFlags( Anabatic::SegGlobal );
        } else
          begin->getDown()->removeVSegment( perpandicular );
      }
    }

    perpandicular = autoTarget->getSegment(2);
    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Anabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoSource() == autoTarget) {
        end->addVSegment( perpandicular );
      } else {
        if (end->getDown() == perpandicular->getAutoSource()->getGCell()) {
          perpandicular->unsetFlags( Anabatic::SegGlobal );
        } else
          end->getDown()->removeVSegment( perpandicular );
      }
    }

    if (begin != end) {
      for ( GCell* gcell=begin->getRight() ; gcell and gcell!=end ; gcell=gcell->getRight() )
        gcell->removeHSegment( this );
    }

    begin = begin->getDown();
    end   = end  ->getDown();

    autoSource->setGCell( begin );
    autoTarget->setGCell( end   );
    if (begin != end) {
      for ( GCell* gcell=begin->getRight() ; gcell and gcell!=end ; gcell=gcell->getRight() )
        gcell->addHSegment( this );
    }

    DbU::Unit y = begin->getSide(Flags::Vertical).getVMax();
    setAxis( y );

    return true;
#endif
    return false;
  }


  bool  AutoHorizontal::moveURight ()
  {
#if THIS_IS_DISABLED
  //cerr << "moveURight() " << this << endl;

    if (not getAutoSource()->isTurn() or not getAutoTarget()->isTurn()) return false;
    if (not getAutoSource()->getGCell()->getUp()) return false;

    AutoContact* autoSource        = getAutoSource();
    AutoContact* autoTarget        = getAutoTarget();
    GCell*       begin             = autoSource->getGCell();
    GCell*       end               = autoTarget->getGCell();
    AutoSegment* perpandicular     = autoSource->getSegment(2);

    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Anabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoTarget() == autoSource) {
        begin->addVSegment( perpandicular );
      } else {
        if (begin->getUp() == perpandicular->getAutoTarget()->getGCell()) {
          perpandicular->unsetFlags( Anabatic::SegGlobal );
        } else
          begin->getUp()->removeVSegment( perpandicular );
      }
    }

    perpandicular = autoTarget->getSegment(2);
    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Anabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoTarget() == autoTarget) {
        end->addVSegment( perpandicular );
      } else {
        if (end->getUp() == perpandicular->getAutoTarget()->getGCell()) {
          perpandicular->unsetFlags( Anabatic::SegGlobal );
        } else
          end->getUp()->removeVSegment( perpandicular );
      }
    }

  //cerr << "| begin:" << begin << endl;
  //cerr << "| end:  " << end << endl;

  //cerr << "* remove" << endl;
    if (begin != end) {
      for ( GCell* gcell=begin->getRight() ; gcell and gcell!=end ; gcell=gcell->getRight() ) {
      //cerr << "| gcell:  " << end << endl;
        gcell->removeHSegment( this );
      }
    }

    begin = begin->getUp();
    end   = end  ->getUp();

    autoSource->setGCell( begin );
    autoTarget->setGCell( end   );
  //cerr << "* add" << endl;
    if (begin != end) {
      for ( GCell* gcell=begin->getRight() ; gcell and gcell!=end ; gcell=gcell->getRight() ) {
      //cerr << "| gcell:  " << end << endl;
        gcell->addHSegment( this );
      }
    }

    DbU::Unit y = begin->getSide( Flags::Vertical ).getVMin();
    setAxis( y );

    return true;
#endif
    return false;
  }


  Flags  AutoHorizontal::_makeDogleg ( GCell* doglegGCell, Flags flags )
  {
    DebugSession::open( getNet(), 145, 150 );
    cdebug_log(149,0) << "AutoHorizontal::_makeDogleg(GCell*) in " << doglegGCell << endl;
    cdebug_tabw(149,1);

  //Session::doglegReset();

    AutoContact*  autoTarget      = getAutoTarget();
    AutoContact*  autoSource      = getAutoSource();
    GCell*        begin           = autoSource->getGCell();
    GCell*        end             = autoTarget->getGCell();

    if (not autoSource->canDrag()) unsetFlags( SegDrag );

    DbU::Unit doglegAxis = (doglegGCell->getXMax() + doglegGCell->getXMin()) / 2;
    if (isLocal())
      doglegAxis = (getSourceX() + getTargetX()) / 2;

    cdebug_log(149,0) << "Detaching from Target AutoContact " << autoTarget << "." << endl;

    if (doglegGCell == begin) unsetFlags( SegGlobal );
    if (doglegGCell != end) {
      GCell* gcell = doglegGCell;
      do {
        if (gcell != begin) {
          cdebug_log(149,0) << "| Remove from:" << gcell << endl;
          gcell->removeHSegment( this );
        }
        gcell = gcell->getEast( getNativeMin() );
      } while ( gcell and (gcell != end) );
    }

    size_t  depth   = Session::getRoutingGauge()->getLayerDepth( _horizontal->getLayer() );
    bool    upLayer = true;

    if (Session::getRoutingGauge()->isTwoMetals()) {
      upLayer = (depth == 0);
    } else if (Session::getRoutingGauge()->isVH()) {
      upLayer = (depth < 2);
    } else {
      upLayer = (depth+1 <= Session::getConfiguration()->getAllowedDepth());
    }

    size_t       doglegDepth  = depth + ((upLayer)?1:-1);
    Layer*       contactLayer = Session::getRoutingGauge()->getContactLayer( depth + ((upLayer)?0:-1) );
    const Layer* doglegLayer  = Session::getRoutingGauge()->getRoutingLayer( doglegDepth );

    Session::dogleg( this );
    targetDetach();
    invalidate( Flags::Topology );
    autoTarget->invalidate( Flags::Topology );
    AutoContact* dlContact1 = AutoContactTurn::create( doglegGCell, _horizontal->getNet(), contactLayer );
    AutoContact* dlContact2 = AutoContactTurn::create( doglegGCell, _horizontal->getNet(), contactLayer );
    AutoSegment* segment1   = AutoSegment::create( dlContact1 , dlContact2, Flags::Vertical );
    segment1->setLayer( doglegDepth );
    segment1->_setAxis( doglegAxis );
    segment1->setFlags( SegDogleg|SegSlackened|SegCanonical|SegNotAligned );

    cdebug_log(149,0) << "New " << dlContact1 << endl;
    cdebug_log(149,0) << "New " << dlContact2 << endl;
    Session::dogleg( segment1 );

    targetAttach( dlContact1 );
    AutoSegment* segment2 = AutoSegment::create( dlContact2 , autoTarget, Flags::Horizontal );
    autoTarget->cacheAttach( segment2 );
    segment2->setLayer( depth );
    segment2->_setAxis( getY() );
    segment2->setFlags( (isSlackened()?SegSlackened:0) );
    Session::dogleg( segment2 );

    if (autoSource->isTerminal() and autoTarget->isTerminal()) {
      segment1->setRpDistance( 1 );
      segment2->setRpDistance( 0 );
      dlContact1->setFlags  ( CntWeakTerminal );
      dlContact2->setFlags  ( CntWeakTerminal );

      if (autoTarget->getGCell() == doglegGCell) dlContact1->migrateConstraintBox( autoTarget );
      if (autoSource->getGCell() == doglegGCell) dlContact2->migrateConstraintBox( autoSource );
    } else if (autoSource->isTerminal()) {
      segment1->setRpDistance( 1 );
      segment2->setRpDistance( 2 );

      autoTarget->unsetFlags( CntWeakTerminal );
      dlContact1->setFlags  ( CntWeakTerminal );
      if (autoTarget->getGCell() == doglegGCell) dlContact1->migrateConstraintBox( autoTarget );
    } else if (autoTarget->isTerminal()) {
      segment2->setRpDistance( 0 );
      segment1->setRpDistance( 1 );
      setRpDistance( 2 );

      unsetFlags( SegTargetTerminal );
      setFlags( SegWeakTerminal1 );
      autoSource->unsetFlags( CntWeakTerminal );
      dlContact2->setFlags  ( CntWeakTerminal );
      if (autoSource->getGCell() == doglegGCell) dlContact2->migrateConstraintBox( autoSource );
    } else  if (isWeakTerminal()) {
      segment1->setFlags( SegWeakTerminal1 );
      segment2->setFlags( SegWeakTerminal1 );
      segment1->setRpDistance( getRpDistance() );
      segment2->setRpDistance( getRpDistance() );
    } else {
      segment1->setRpDistance( getRpDistance() );
      segment2->setRpDistance( getRpDistance() );
    }

    // if (autoSource->isTerminal()) {
    //   segment1->setFlags( SegWeakTerminal1 );
    //   segment2->setFlags( SegWeakTerminal1 );
    //   autoTarget->unsetFlags( CntWeakTerminal );
    //   dlContact1->setFlags  ( CntWeakTerminal );
    //   if (autoTarget->getGCell() == doglegGCell)
    //     dlContact1->migrateConstraintBox( autoTarget );
    // } else if (autoTarget->isTerminal()) {
    //   unsetFlags( SegTargetTerminal );
    //   setFlags( SegWeakTerminal1 );
    //   segment1->setFlags( SegWeakTerminal1 );
    //   segment2->setFlags( SegTargetTerminal );
    //   autoSource->unsetFlags( CntWeakTerminal );
    //   dlContact2->setFlags  ( CntWeakTerminal );
    //   if (autoSource->getGCell() == doglegGCell)
    //     dlContact2->migrateConstraintBox( autoSource );
    // } else if (isWeakTerminal()) {
    //   segment1->setFlags( SegWeakTerminal1 );
    //   segment2->setFlags( SegWeakTerminal1 );
    // }

    if (isAnalog()) {
      segment1->setFlags( SegAnalog );
      segment2->setFlags( SegAnalog );
    }

    cdebug_log(149,0) << "Session::dogleg[x+1] perpand:   " << segment1 << endl;
    cdebug_log(149,0) << "Session::dogleg[x+2] new paral: " << segment2 << endl;
    cdebug_log(149,0) << "Session::dogleg[x+0] original:  " << this << endl;

    dlContact1->updateCache();
    dlContact2->updateCache();
  //autoTarget->updateCache();

    segment2->canonize( flags );
    if (not isCanonical()) canonize( flags );

    updateNativeConstraints();
    segment2->updateNativeConstraints();

    if (          isLocal()) autoSource->setFlags( AutoContactFlag::CntVDogleg );
    if (segment2->isLocal()) autoTarget->setFlags( AutoContactFlag::CntVDogleg );

    if (autoTarget->canDrag() and not autoSource->canDrag()) {
      if (not autoTarget->getGCell()->isDevice() and (segment1->getGCell() == autoTarget->getGCell())) {
        Interval dragConstraints = autoTarget->getNativeUConstraints(Flags::Horizontal);
        segment1->mergeUserConstraints( dragConstraints );

        cdebug_log(149,0) << "Perpandical has drag constraints: " << dragConstraints << endl;
      }
    }

    cdebug_tabw(149,-1);
    DebugSession::close();

    return (upLayer) ? Flags::AboveLayer : Flags::BelowLayer;
  }


  string  AutoHorizontal::_getString () const
  {
    string  s = AutoSegment::_getString();
    return s;
  }


  Record* AutoHorizontal::_getRecord () const
  {
    Record* record = AutoSegment::_getRecord ();
    record->add ( getSlot ( "_horizontal", _horizontal ) );
                                     
    return record;
  }


} // End of Anabatic namespace.
