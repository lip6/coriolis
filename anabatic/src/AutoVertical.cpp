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
// |  C++ Module  :       "./AutoVertical.cpp"                       |
// +-----------------------------------------------------------------+


#include <algorithm>
#include "hurricane/Bug.h"
#include "hurricane/ViaLayer.h"
#include "hurricane/Vertical.h"
#include "crlcore/RoutingGauge.h"
#include "anabatic/Configuration.h"
#include "anabatic/AutoContactTurn.h"
#include "anabatic/AutoVertical.h"
#include "anabatic/AutoHorizontal.h"


namespace Anabatic {

  using std::min;
  using std::max;
  using Hurricane::Error;
  using Hurricane::Bug;
  using Hurricane::ViaLayer;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoVertical".


  Segment*   AutoVertical::base         ()       { return _vertical; }
  Segment*   AutoVertical::base         () const { return _vertical; }
  Vertical*  AutoVertical::getVertical  ()       { return _vertical; }
  DbU::Unit  AutoVertical::getSourceU   () const { return _vertical->getSourceY(); }
  DbU::Unit  AutoVertical::getTargetU   () const { return _vertical->getTargetY(); }
  DbU::Unit  AutoVertical::getDuSource  () const { return _vertical->getDySource(); }
  DbU::Unit  AutoVertical::getDuTarget  () const { return _vertical->getDyTarget(); }
  Interval   AutoVertical::getSpanU     () const { return Interval(_vertical->getSourceY(),_vertical->getTargetY()); }
  void       AutoVertical::setDuSource  ( DbU::Unit du ) { _vertical->setDySource(du); }
  void       AutoVertical::setDuTarget  ( DbU::Unit du ) { _vertical->setDyTarget(du); }
  string     AutoVertical::_getTypeName () const { return "AutoVertical"; }


  AutoVertical::AutoVertical ( Vertical* vertical )
    : AutoSegment(vertical)
    , _vertical(vertical)
  {
    cdebug_log(145,0) << "CTOR AutoVertical " << this << endl;
    cdebug_log(145,0) << "             over " << vertical << endl;
  }


  void  AutoVertical::_postCreate ()
  {
    AutoSegment::_postCreate ();

    AutoContact* source = getAutoSource();
    if (source->isTerminal()) source->setX( _vertical->getX() );

    AutoContact* target = getAutoTarget();
    if (target->isTerminal()) target->setX( _vertical->getX() );

    _gcell = source->getGCell();

    setOptimalMax( getGCell()->getXMax() );
    resetNativeConstraints( getGCell()->getXMin(), getGCell()->getConstraintXMax() );

    cdebug_log(145,0) << "Source in " << getGCell() << endl;
    cdebug_log(145,0) << "Target in " << target->getGCell() << endl;
    if (getGCell() != target->getGCell()) {
      setFlags( SegGlobal );

      vector<GCell*> gcells;
      getGCells( gcells );
      for ( GCell* gcell : gcells ) {
        if ( (gcell != getGCell()) and (gcell != target->getGCell()) )
          gcell->addVSegment( this );
        mergeNativeMin( gcell->getXMin() );
        mergeNativeMax( gcell->getConstraintXMax() );
      }
    }
  }


  void  AutoVertical::_preDestroy ()
  {
    cdebug_log(149,1) << "AutoVertical::_preDestroy() - <id:" << getId() << "> " << endl;
    cdebug_log(149,0)   << _getString() << endl;

    if ( not Session::doDestroyTool() ) {
      vector<GCell*> gcells;
      getGCells( gcells );
      for ( GCell* gcell : gcells ) gcell->removeVSegment( this );
    }

    AutoSegment::_preDestroy ();
    cdebug_tabw(145,-1);
  }


  AutoVertical::~AutoVertical ()
  {
    if ( Session::doDestroyBaseSegment() and not Session::doDestroyTool() ) {
      cdebug_log(149,0) << "~AutoVertical() - " << endl;
      _vertical->destroy ();
    }
  }


  Interval  AutoVertical::getSourceConstraints ( Flags flags ) const
  {
    if (flags & Flags::NativeConstraints) {
      Box nativeBox ( getAutoSource()->getNativeConstraintBox() );
      return Interval ( nativeBox.getXMin(), nativeBox.getXMax() );
    }
    return Interval ( getAutoSource()->getCBXMin(), getAutoSource()->getCBXMax() );
  }


  Interval  AutoVertical::getTargetConstraints ( Flags flags ) const
  {
    if (flags & Flags::NativeConstraints) {
      Box nativeBox ( getAutoTarget()->getNativeConstraintBox() );
      return Interval ( nativeBox.getXMin(), nativeBox.getXMax() );
    }
    return Interval ( getAutoTarget()->getCBXMin(), getAutoTarget()->getCBXMax() );
  }


  bool  AutoVertical::getConstraints ( DbU::Unit& constraintMin, DbU::Unit& constraintMax ) const
  {
    cdebug_log(149,1) << "getConstraints() " << this << endl;

    constraintMin = getNativeMin();
    constraintMax = getNativeMax();

    cdebug_log(149,0) << "Native constraints: ["
                      << DbU::getValueString(constraintMin) << ":"
                      << DbU::getValueString(constraintMax) << "]"
                      << endl;

    constraintMin = std::max ( constraintMin, getAutoSource()->getCBXMin() );
    constraintMax = std::min ( constraintMax, getAutoSource()->getCBXMax() );
    cdebug_log(149,0) << "Merge with source constraints: ["
                      << DbU::getValueString(getAutoSource()->getCBXMin()) << ":"
                      << DbU::getValueString(getAutoSource()->getCBXMax()) << "]"
                      << endl;

    constraintMin = std::max ( constraintMin, getAutoTarget()->getCBXMin() );
    constraintMax = std::min ( constraintMax, getAutoTarget()->getCBXMax() );
    cdebug_log(149,0) << "Merge with target constraints: ["
                      << DbU::getValueString(getAutoTarget()->getCBXMin()) << ":"
                      << DbU::getValueString(getAutoTarget()->getCBXMax()) << "]"
                      << endl;

    Interval userConstraints = getUserConstraints();
    if (not userConstraints.isEmpty()) {
      constraintMin = max ( constraintMin, userConstraints.getVMin() );
      constraintMax = min ( constraintMax, userConstraints.getVMax() );

      cdebug_log(149,0) << "Merge with user constraints: ["
                        << DbU::getValueString(userConstraints.getVMin()) << ":"
                        << DbU::getValueString(userConstraints.getVMax()) << "]"
                        << endl;
    } else
      cdebug_log(155,0) << "Empty user constraints" << endl;

    cdebug_log(149,0) << "Resulting constraints: ["
                      << DbU::getValueString(constraintMin) << ":"
                      << DbU::getValueString(constraintMax) << "]"
                      << endl;

    if (constraintMin > constraintMax)
      cerr << Error( "AutoVertical::getConstraints(): Invalid interval [%s : %s]\n"
                     "        on %s"
                   , DbU::getValueString(constraintMin).c_str()
                   , DbU::getValueString(constraintMax).c_str()
                   , getString(this).c_str()
                   ) << endl;
    
    cdebug_tabw(149,-1);
    return true;
  }


  Flags  AutoVertical::getDirection () const
  { return Flags::Vertical; }


  bool  AutoVertical::getGCells ( vector<GCell*>& gcells ) const
  {
    vector<GCell*>().swap( gcells );

    bool      success = true;
    DbU::Unit xprobe  = getX();
    GCell*    gcell   = getAutoSource()->getGCell();
    GCell*    end     = getAutoTarget()->getGCell();

    cdebug_log(144,0) << "xprobe: " << DbU::getValueString(xprobe) << endl;

    if (gcell->getYMin() > end->getYMin()) std::swap( gcell, end );
    if (xprobe == gcell->getConstraintXMax()) xprobe--;

    gcells.push_back( gcell );

    while ( gcell != end ) {
      gcell = gcell->getNorth( xprobe );

      if (not gcell) {
        success = false;
        cerr << Error( "AutoVertical::getGCells() : NULL GCell under %s\n"
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


  bool  AutoVertical::_canSlacken () const
  {
    cdebug_tabw(149,-1);

    Interval sourceSide        = getAutoSource()->getGCell()->getSide( Flags::Horizontal );
    Interval targetSide        = getAutoTarget()->getGCell()->getSide( Flags::Horizontal );
    Interval sourceConstraints = Interval(getAutoSource()->getCBXMin(),getAutoSource()->getCBXMax());
    Interval targetConstraints = Interval(getAutoTarget()->getCBXMin(),getAutoTarget()->getCBXMax());
    bool     sourceGoStraight  = getAutoSource()->getGCell()->isGoStraight();
    bool     targetGoStraight  = getAutoTarget()->getGCell()->isGoStraight();

  // Expand by a tiny amount for the "contains" to work for sure.
    sourceConstraints.inflate( 1 );
    targetConstraints.inflate( 1 );

    if (not sourceGoStraight and not sourceConstraints.contains(sourceSide)) { cdebug_tabw(149,-1); return true; }
    if (not targetGoStraight and not targetConstraints.contains(targetSide)) { cdebug_tabw(149,-1); return true; }

    cdebug_tabw(149,-1);
    return false;
  }


  bool  AutoVertical::_slacken ( Flags flags )
  {
    cdebug_log(149,1) << "AutoVertical::_slacken() " << this << endl;

    if (not isDrag()) {
      if (   not isStrongTerminal()
         or (not (_flags & (SegGlobal|SegWeakGlobal)) and (getLength() < getPitch()*5)) )
        { cdebug_tabw(149,-1); return false; }
    }

    cdebug_log(149,0) << "_flags:" << (_flags & (SegGlobal|SegWeakGlobal)) << endl;
    cdebug_log(149,0) << "test:" << (getLength() < getPitch()*5) << endl;
    cdebug_log(149,0) << "length:" << DbU::getValueString(getLength()) << endl;

    bool         success         = false;
    bool         sourceSlackened = false;
    bool         targetSlackened = false;
    bool         halfSlackened   = false;
    int          lowSlack        = (flags & Flags::HalfSlacken) ? 3 : 10;
    AutoContact* source          = getAutoSource();
    AutoSegment* parallel        = this;

    if (source->isTerminal()) {
      Interval  constraints       = source->getUConstraints      (Flags::Horizontal|Flags::NoGCellShrink);
      Interval  nativeConstraints = source->getNativeUConstraints(Flags::Horizontal|Flags::NoGCellShrink);
      int       slack             = constraints.getSize()       / getPitch();
      int       nativeSlack       = nativeConstraints.getSize() / getPitch();

    // Ugly: GCell's track number is hardwired.
      if ((slack < lowSlack) or (nativeSlack - slack < 3)) {
        _makeDogleg( source->getGCell(), Flags::NoFlags );
        sourceSlackened = true;
      } else if (slack < 10) {
        halfSlackened = true;
      }

      const vector<AutoSegment*>& doglegs = Session::getDoglegs();
      if (sourceSlackened and (doglegs.size() >= 2)) {
        cdebug_log(149,0) << "AutoVertical::_slacken(): Source @" << DbU::getValueString(getSourcePosition()) << endl;
        doglegs[doglegs.size()-2]->_setAxis( getSourcePosition() );
        success = true;

        parallel = doglegs[ doglegs.size()-1 ];
      }
    }

    AutoContact* target = NULL;
    if (parallel) {
      target = parallel->getAutoTarget();
    } else {
      target = getAutoTarget();
    }

    if (target->isTerminal()) {
      Interval  constraints       = target->getUConstraints      (Flags::Horizontal|Flags::NoGCellShrink);
      Interval  nativeConstraints = target->getNativeUConstraints(Flags::Horizontal|Flags::NoGCellShrink);
      int       slack             = constraints.getSize()       / getPitch();
      int       nativeSlack       = nativeConstraints.getSize() / getPitch();

    // Ugly: GCell's track number is hardwired.
      if ((slack < lowSlack) or (nativeSlack - slack < 3)) {
        _makeDogleg( target->getGCell(), Flags::NoFlags );
        targetSlackened = true;
      } else if (slack < 10) {
        halfSlackened = true;
      }

      const vector<AutoSegment*>& doglegs = Session::getDoglegs();
      if (targetSlackened and (doglegs.size() >= 2)) {
        cdebug_log(149,0) << "AutoVertical::_slacken(): Source @" << DbU::getValueString(getTargetPosition()) << endl;
        doglegs[doglegs.size()-2]->_setAxis( getTargetPosition() );
        success = true;
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


  void  AutoVertical::_setAxis ( DbU::Unit axis )
  {
    setFlags( SegAxisSet );

    if (_vertical->getX() == axis) return;

    cdebug_log(144,0) << "_setAxis() @X " << DbU::getValueString(axis) << " " << this << endl;

    _vertical->setX( axis );
    invalidate();

    AutoContact* anchor = getAutoSource();
    anchor->invalidate();
    if (anchor->isTerminal()) anchor->setX( axis );

    anchor = getAutoTarget();
    anchor->invalidate();
    if (anchor->isTerminal()) anchor->setX( axis );
  }


  void  AutoVertical::updateOrient ()
  {
    if (_vertical->getTargetY() < _vertical->getSourceY()) {
      cdebug_log(145,0) << "updateOrient() " << this << " (before S/T swap)" << endl;
      _vertical->invert();

      unsigned int spinFlags = _flags & SegDepthSpin;
      unsetFlags( SegDepthSpin );
      if (spinFlags & SegSourceTop   ) setFlags( SegTargetTop );
      if (spinFlags & SegSourceBottom) setFlags( SegTargetBottom );
      if (spinFlags & SegTargetTop   ) setFlags( SegSourceTop );
      if (spinFlags & SegTargetBottom) setFlags( SegSourceBottom );

      unsigned int invalidatedFlags = _flags & (SegInvalidatedSource|SegInvalidatedTarget);
      unsetFlags( SegInvalidatedSource|SegInvalidatedTarget );
      if (invalidatedFlags & SegInvalidatedSource) setFlags( SegInvalidatedTarget );
      if (invalidatedFlags & SegInvalidatedTarget) setFlags( SegInvalidatedSource );

      unsigned int terminalFlags = _flags & SegStrongTerminal;
      unsetFlags( SegStrongTerminal );
      if (terminalFlags & SegSourceTerminal) setFlags( SegTargetTerminal );
      if (terminalFlags & SegTargetTerminal) setFlags( SegSourceTerminal );
    }
  }


  void  AutoVertical::updatePositions ()
  {
    _sourcePosition = _vertical->getSourceY() - getExtensionCap(Flags::Source);
    _targetPosition = _vertical->getTargetY() + getExtensionCap(Flags::Target);
  }


  void  AutoVertical::updateNativeConstraints ()
  {
    vector<GCell*> gcells;
    getGCells( gcells );

    resetNativeConstraints( gcells[0]->getXMin(), gcells[0]->getConstraintXMax() );
    for ( GCell* gcell : gcells ) {
      mergeNativeMin( gcell->getXMin() );
      mergeNativeMax( gcell->getConstraintXMax() );
    }
  }


  bool  AutoVertical::checkPositions () const
  {
    bool      coherency      = true;
    DbU::Unit sourcePosition = _vertical->getSourceY() - getExtensionCap(Flags::Source);
    DbU::Unit targetPosition = _vertical->getTargetY() + getExtensionCap(Flags::Target);

    if ( _sourcePosition != sourcePosition ) {
      cerr << Error ( "%s\n        Source position incoherency: "
                      "Shadow: %s, real: %s."
                    , _getString().c_str() 
                    , DbU::getValueString(_sourcePosition).c_str()
                    , DbU::getValueString( sourcePosition).c_str()
                    ) << endl;
      coherency = false;
    }

    if ( _targetPosition != targetPosition ) {
      cerr << Error ( "%s\n        Target position incoherency: "
                      "Shadow: %s, real: %s."
                    , _getString().c_str() 
                    , DbU::getValueString(_targetPosition).c_str()
                    , DbU::getValueString( targetPosition).c_str()
                    ) << endl;
      coherency = false;
    }

    return coherency;
  }


  bool  AutoVertical::checkConstraints () const
  {
    Interval sourceConstraints = Interval(getAutoSource()->getCBXMin(),getAutoSource()->getCBXMax());
    Interval targetConstraints = Interval(getAutoTarget()->getCBXMin(),getAutoTarget()->getCBXMax());

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


  bool  AutoVertical::canMoveULeft ( float reserve ) const
  {
#if THIS_IS_DISABLED
    if (not isGlobal()) return false;
    if (not getAutoSource()->isTurn() or not getAutoTarget()->isTurn()) return false;
    if (not getAutoSource()->getGCell()->getLeft()) return false;

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

    for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getUp() ) {
      if (currMaxDensity < gcell->getWDensity(depth)) currMaxDensity = gcell->getWDensity( depth );
    }
            
    begin = begin->getLeft();
    end   = end  ->getLeft();

    for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getUp() ) {
      if (leftMaxDensity < gcell->getWDensity(depth)) leftMaxDensity = gcell->getWDensity( depth );
    }

    return (leftMaxDensity + reserve < currMaxDensity);
#endif
    return false;
  }


  bool  AutoVertical::canMoveURight ( float reserve ) const
  {
#if THIS_IS_DISABLED
    if (not isGlobal()) return false;
    if (not getAutoSource()->isTurn() or not getAutoTarget()->isTurn()) return false;
    if (not getAutoSource()->getGCell()->getRight()) return false;

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

    for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getUp() ) {
      if (currMaxDensity < gcell->getWDensity(depth)) currMaxDensity = gcell->getWDensity( depth );
    }
            
    begin = begin->getRight();
    end   = end  ->getRight();

    for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getUp() ) {
      if (leftMaxDensity < gcell->getWDensity(depth)) leftMaxDensity = gcell->getWDensity( depth );
    }

    return (leftMaxDensity + reserve < currMaxDensity);
#endif
    return false;
  }


  bool  AutoVertical::moveULeft ()
  {
#if THIS_IS_DISABLED
    if (not getAutoSource()->isTurn() or not getAutoTarget()->isTurn()) return false;
    if (not getAutoSource()->getGCell()->getLeft()) return false;

    AutoContact* autoSource        = getAutoSource();
    AutoContact* autoTarget        = getAutoTarget();
    GCell*       begin             = autoSource->getGCell();
    GCell*       end               = autoTarget->getGCell();
    AutoSegment* perpandicular     = autoSource->getSegment(0);

    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Anabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoSource() == autoSource) {
        begin->addHSegment( perpandicular );
      } else {
        if (begin->getLeft() == perpandicular->getAutoSource()->getGCell()) {
          perpandicular->unsetFlags( Anabatic::SegGlobal );
        } else
          begin->getLeft()->removeHSegment( perpandicular );
      }
    }

    perpandicular = autoTarget->getSegment(0);
    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Anabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoSource() == autoTarget) {
        end->addHSegment( perpandicular );
      } else {
        if (end->getLeft() == perpandicular->getAutoSource()->getGCell()) {
          perpandicular->unsetFlags( Anabatic::SegGlobal );
        } else
          end->getLeft()->removeHSegment( perpandicular );
      }
    }

    if (begin != end) {
      for ( GCell* gcell=begin->getUp() ; gcell and gcell!=end ; gcell=gcell->getUp() )
        gcell->removeVSegment( this );
    }

    begin = begin->getLeft();
    end   = end  ->getLeft();

    autoSource->setGCell( begin );
    autoTarget->setGCell( end   );
    if (begin != end) {
      for ( GCell* gcell=begin->getUp() ; gcell and gcell!=end ; gcell=gcell->getUp() )
        gcell->addVSegment( this );
    }

    DbU::Unit x = begin->getSide( Flags::Horizontal ).getVMax();
    setAxis( x );

    return true;
#endif
    return false;
  }


  bool  AutoVertical::moveURight ()
  {
#if THIS_IS_DISABLED
    cdebug_log(149,0) << "AutoVertical::moveURight()" << endl;

    if (not getAutoSource()->isTurn() or not getAutoTarget()->isTurn()) return true;
    if (not getAutoSource()->getGCell()->getRight()) return true;

    AutoContact* autoSource        = getAutoSource();
    AutoContact* autoTarget        = getAutoTarget();
    GCell*       begin             = autoSource->getGCell();
    GCell*       end               = autoTarget->getGCell();
    AutoSegment* perpandicular     = autoSource->getSegment(0);

    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Anabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoTarget() == autoSource) {
        begin->addHSegment( perpandicular );
      } else {
        if (begin->getRight() == perpandicular->getAutoTarget()->getGCell()) {
          perpandicular->unsetFlags( Anabatic::SegGlobal );
        } else
          begin->getRight()->removeHSegment( perpandicular );
      }
    }

    perpandicular = autoTarget->getSegment(0);
    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Anabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoTarget() == autoTarget) {
        end->addHSegment( perpandicular );
      } else {
        if (end->getRight() == perpandicular->getAutoTarget()->getGCell()) {
          perpandicular->unsetFlags( Anabatic::SegGlobal );
        } else
          end->getRight()->removeHSegment( perpandicular );
      }
    }

    if (begin != end) {
      for ( GCell* gcell=begin->getUp() ; gcell and gcell!=end ; gcell=gcell->getUp() )
        gcell->removeVSegment( this );
    }

    begin = begin->getRight();
    end   = end  ->getRight();
    autoSource->setGCell( begin );
    autoTarget->setGCell( end   );
    if (begin != end) {
      for ( GCell* gcell=begin->getUp() ; gcell and gcell!=end ; gcell=gcell->getUp() )
        gcell->addVSegment( this );
    }

    DbU::Unit x = begin->getSide( Flags::Horizontal ).getVMin();
    setAxis( x );

    cdebug_log(149,0) << "Moved to axis: " << DbU::getValueString(x) << endl;

    return true;
#endif
    return false;
  }


  Flags  AutoVertical::_makeDogleg ( GCell* doglegGCell, Flags flags )
  {
    cdebug_log(149,0) << "AutoVertical::_makeDogleg(GCell*)" << endl;

  //Session::doglegReset();

    AutoContact*  autoSource = getAutoSource();
    AutoContact*  autoTarget = getAutoTarget();
    GCell*        begin      = autoSource->getGCell();
    GCell*        end        = autoTarget->getGCell();

    if (not autoSource->canDrag()) unsetFlags( SegDrag );

    DbU::Unit doglegAxis = (doglegGCell->getYMax() + doglegGCell->getYMin()) / 2;
    if (isLocal())
      doglegAxis = (getSourceY() + getTargetY()) / 2;

    if (doglegGCell == begin) unsetFlags( SegGlobal );
    if (doglegGCell != end) {
      GCell* gcell = doglegGCell;
      do {
        if (gcell != begin)
          gcell->removeVSegment( this );
        gcell = gcell->getNorth( getNativeMin() );
      } while ( gcell and (gcell != end) );
    }

    size_t  depth   = Session::getRoutingGauge()->getLayerDepth ( _vertical->getLayer() );
    bool    upLayer = true;

    if (Session::getRoutingGauge()->isTwoMetals()) {
      upLayer = (depth == 0);
    } else if (Session::getRoutingGauge()->isVH()) {
      upLayer = (depth < 2);
    } else {
      upLayer = (depth+1 <= Session::getConfiguration()->getAllowedDepth());
    }

    size_t  doglegDepth  = depth + ((upLayer)?1:-1);
    Layer*  contactLayer = Session::getRoutingGauge()->getContactLayer ( depth + ((upLayer)?0:-1) );

    Session::dogleg( this );
    targetDetach();
    invalidate( Flags::Topology );
    autoTarget->invalidate( Flags::Topology );
    AutoContact* dlContact1 = AutoContactTurn::create( doglegGCell, _vertical->getNet(), contactLayer );
    cdebug_log(149,0) << dlContact1 << endl;
    AutoContact* dlContact2 = AutoContactTurn::create( doglegGCell, _vertical->getNet(), contactLayer );
    cdebug_log(149,0) << dlContact2 << endl;
    AutoSegment* segment1   = AutoSegment::create( dlContact1 , dlContact2, Flags::Horizontal );
    cdebug_log(149,0) << segment1 << endl;
    segment1->setLayer( doglegDepth );
    segment1->_setAxis( doglegAxis );
    segment1->setFlags( SegDogleg|SegSlackened|SegCanonical|SegNotAligned );
    cdebug_log(149,0) << "New " << dlContact1->base() << "." << endl;
    cdebug_log(149,0) << "New " << dlContact2->base() << "." << endl;
    Session::dogleg( segment1 );

    targetAttach( dlContact1 );
    AutoSegment* segment2 = AutoVertical::create ( dlContact2, autoTarget, Flags::Vertical );
    autoTarget->cacheAttach( segment2 );
    segment2->setLayer( depth );
    segment2->_setAxis( getX() );
    segment2->setFlags( (isSlackened()?SegSlackened:0) );
    Session::dogleg( segment2 );

    if (autoSource->isTerminal() and autoTarget->isTerminal()) {
      dlContact1->setFlags  ( CntWeakTerminal );
      dlContact2->setFlags  ( CntWeakTerminal );

      if (autoTarget->getGCell() == doglegGCell) dlContact1->migrateConstraintBox( autoTarget );
      if (autoSource->getGCell() == doglegGCell) dlContact2->migrateConstraintBox( autoSource );
    } else if (autoSource->isTerminal()) {
      autoTarget->unsetFlags( CntWeakTerminal );
      dlContact1->setFlags  ( CntWeakTerminal );
      if (autoTarget->getGCell() == doglegGCell) dlContact1->migrateConstraintBox( autoTarget );
    } else if (autoTarget->isTerminal()) {
      unsetFlags( SegTargetTerminal );
      setFlags( SegWeakTerminal1 );
      autoSource->unsetFlags( CntWeakTerminal );
      dlContact2->setFlags  ( CntWeakTerminal );
      if (autoSource->getGCell() == doglegGCell) dlContact2->migrateConstraintBox( autoSource );
    } else  if (isWeakTerminal()) {
      segment1->setFlags( SegWeakTerminal1 );
      segment2->setFlags( SegWeakTerminal1 );
    }

    // if (isSourceTerminal()) {
    //   segment1->setFlags( SegWeakTerminal1 );
    //   segment2->setFlags( SegWeakTerminal1 );
    //   autoTarget->unsetFlags( CntWeakTerminal );
    //   dlContact1->setFlags  ( CntWeakTerminal );
    //   if (autoTarget->getGCell() == doglegGCell)
    //     dlContact1->migrateConstraintBox( autoTarget );
    // } else if (isTargetTerminal()) {
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

    if (          isLocal()) autoSource->setFlags( AutoContactFlag::CntHDogleg );
    if (segment2->isLocal()) autoTarget->setFlags( AutoContactFlag::CntHDogleg );

    if (autoTarget->canDrag() and not autoSource->canDrag()) {
      if (not autoTarget->getGCell()->isDevice() and (segment1->getGCell() == autoTarget->getGCell())) {
        Interval dragConstraints = autoTarget->getNativeUConstraints(Flags::Vertical);
        segment1->mergeUserConstraints( dragConstraints );

        cdebug_log(149,0) << "Perpandicular has drag constraints: " << dragConstraints << endl;
      }
    }

    return (upLayer) ? Flags::AboveLayer : Flags::BelowLayer;
  }


  string  AutoVertical::_getString () const
  {
    string  s = AutoSegment::_getString();
    return s;
  }


  Record* AutoVertical::_getRecord () const
  {
    Record* record = AutoSegment::_getRecord ();
    record->add ( getSlot ( "_vertical", _vertical ) );
    return record;
  }


} // End of Anabatic namespace.
