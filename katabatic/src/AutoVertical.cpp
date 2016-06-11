// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AutoVertical.cpp"                       |
// +-----------------------------------------------------------------+


#include  <algorithm>
#include  "hurricane/Bug.h"
#include  "hurricane/Vertical.h"
#include  "crlcore/RoutingGauge.h"
#include  "katabatic/Configuration.h"
#include  "katabatic/AutoContactTurn.h"
#include  "katabatic/AutoVertical.h"
#include  "katabatic/AutoHorizontal.h"


namespace Katabatic {


  using std::min;
  using std::max;
  using Hurricane::Error;
  using Hurricane::Bug;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoVertical".


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

    if ( source->getGCell() != target->getGCell() ) {
      setFlags( SegGlobal );

      GCell* gcell;
      GCell* end;

      if ( source->getGCell()->getY() < target->getGCell()->getY() ) {
        gcell = source->getGCell()->getUp();
        end   = target->getGCell();
      } else {
        gcell = target->getGCell()->getUp();
        end   = source->getGCell();
      }
      for ( ; gcell != end ; gcell = gcell->getUp() ) {
        if ( !gcell ) {
          cerr << Error("AutoSegment::create() : NULL GCell.") << endl;
          break;
        }
        gcell->addVSegment ( this );
      }
    }
  }


  void  AutoVertical::_preDestroy ()
  {
    cdebug_log(149,1) << "AutoVertical::_preDestroy() - <id:" << getId() << "> " << endl;
    cdebug_log(149,0)   << _getString() << endl;

    if ( not Session::doDestroyTool() ) {
      AutoContact* source = getAutoSource();
      AutoContact* target = getAutoTarget();

      if ( source and target and (source->getGCell() != target->getGCell()) ) {
        GCell* gcell;
        GCell* end;

        if ( source->getGCell()->getY() < target->getGCell()->getY() ) {
          gcell = source->getGCell()->getUp();
          end   = target->getGCell();
        } else {
          gcell = target->getGCell()->getUp();
          end   = source->getGCell();
        }
        for ( ; gcell != end ; gcell = gcell->getUp() ) {
          if ( !gcell ) {
            cerr << Error("AutoSegment::_preDestroy() : NULL GCell.") << endl;
            break;
          }
          gcell->removeVSegment ( this );
        }
      }
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


  Interval  AutoVertical::getSourceConstraints ( unsigned int flags ) const
  {
    if (flags & KbNativeConstraints) {
      Box nativeBox ( getAutoSource()->getNativeConstraintBox() );
      return Interval ( nativeBox.getXMin(), nativeBox.getXMax() );
    }
    return Interval ( getAutoSource()->getCBXMin(), getAutoSource()->getCBXMax() );
  }


  Interval  AutoVertical::getTargetConstraints ( unsigned int flags ) const
  {
    if (flags & KbNativeConstraints) {
      Box nativeBox ( getAutoTarget()->getNativeConstraintBox() );
      return Interval ( nativeBox.getXMin(), nativeBox.getXMax() );
    }
    return Interval ( getAutoTarget()->getCBXMin(), getAutoTarget()->getCBXMax() );
  }


  bool  AutoVertical::getConstraints ( DbU::Unit& constraintMin, DbU::Unit& constraintMax ) const
  {
    AutoContact* contact = getAutoSource();
    constraintMin = contact->getCBXMin();
    constraintMax = contact->getCBXMax();

    cdebug_log(149,0) << "Source constraints: " << contact << " ["
                << DbU::getValueString(constraintMin) << ":"
                << DbU::getValueString(constraintMax) << "]"
                << endl;

    contact = getAutoTarget();
    constraintMin = max ( constraintMin, contact->getCBXMin() );
    constraintMax = min ( constraintMax, contact->getCBXMax() );

    cdebug_log(149,0) << "Merge with target constraints: " << contact << " ["
                << DbU::getValueString(constraintMin) << ":"
                << DbU::getValueString(constraintMax) << "]"
                << endl;

    constraintMin = max ( constraintMin, getUserConstraints().getVMin() );
    constraintMax = min ( constraintMax, getUserConstraints().getVMax() );

    cdebug_log(149,0) << "Merge with user constraints: " << getUserConstraints() << " ["
                << DbU::getValueString(constraintMin) << ":"
                << DbU::getValueString(constraintMax) << "]"
                << endl;

    return true;
  }


  unsigned int  AutoVertical::getDirection () const
  { return KbVertical; }


  size_t  AutoVertical::getGCells ( vector<GCell*>& gcells ) const
  {
    vector<GCell*>().swap ( gcells );
    gcells.push_back ( getAutoSource()->getGCell() );

    GCell* gcell = gcells.front();
    GCell* end   = getAutoTarget()->getGCell();

    while ( gcell != end ) {
      gcell = gcell->getUp ();
      if ( !gcell ) break;

      gcells.push_back ( gcell );
    }

    return gcells.size();
  }


  bool  AutoVertical::_canSlacken () const
  {
    cdebug_tabw(149,-1);

    Interval sourceSide        = getAutoSource()->getGCell()->getSide( KbHorizontal );
    Interval targetSide        = getAutoTarget()->getGCell()->getSide( KbHorizontal );
    Interval sourceConstraints = Interval(getAutoSource()->getCBXMin(),getAutoSource()->getCBXMax());
    Interval targetConstraints = Interval(getAutoTarget()->getCBXMin(),getAutoTarget()->getCBXMax());

  // Expand by a tiny amount for the "contains" to work for sure.
    sourceConstraints.inflate( 1 );
    targetConstraints.inflate( 1 );

    if (not sourceConstraints.contains(sourceSide)) { cdebug_tabw(149,-1); return true; }
    if (not targetConstraints.contains(targetSide)) { cdebug_tabw(149,-1); return true; }

    cdebug_tabw(149,-1);
    return false;
  }


  bool  AutoVertical::_slacken ( unsigned int flags )
  {
    cdebug_log(149,1) << "AutoVertical::_slacken() " << this << endl;

    if (   not isStrongTerminal()
       or (not (_flags & (SegGlobal|SegWeakGlobal)) and (getLength() < getPitch()*5)) )
      { cdebug_tabw(149,-1); return false; }

    cdebug_log(149,0) << "_flags:" << (_flags & (SegGlobal|SegWeakGlobal)) << endl;
    cdebug_log(149,0) << "test:" << (getLength() < getPitch()*5) << endl;
    cdebug_log(149,0) << "length:" << DbU::getValueString(getLength()) << endl;

    bool         success       = false;
    bool         slackened     = false;
    bool         halfSlackened = false;
    int          lowSlack      = (flags & KbHalfSlacken) ? 3 : 10;
    AutoContact* source        = getAutoSource();
    AutoSegment* parallel      = this;

    if (source->isTerminal()) {
      Interval  constraints       = source->getUConstraints      (KbHorizontal|KbNoGCellShrink);
      Interval  nativeConstraints = source->getNativeUConstraints(KbHorizontal|KbNoGCellShrink);
      int       slack             = constraints.getSize()       / getPitch();
      int       nativeSlack       = nativeConstraints.getSize() / getPitch();

    // Ugly: GCell's track number is hardwired.
      if ((slack < lowSlack) or (nativeSlack - slack < 3)) {
        _makeDogleg( source->getGCell(), KbNoFlags );
        slackened = true;
      } else if (slack < 10) {
        halfSlackened = true;
      }

      const vector<AutoSegment*>& doglegs = Session::getDoglegs();
      if (doglegs.size() >= 2) {
        cdebug_log(149,0) << "AutoSegment::_slaken(): Source @" << DbU::getValueString(getSourcePosition()) << endl;
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
      Interval  constraints       = target->getUConstraints      (KbHorizontal|KbNoGCellShrink);
      Interval  nativeConstraints = target->getNativeUConstraints(KbHorizontal|KbNoGCellShrink);
      int       slack             = constraints.getSize()       / getPitch();
      int       nativeSlack       = nativeConstraints.getSize() / getPitch();

    // Ugly: GCell's track number is hardwired.
      if ((slack < lowSlack) or (nativeSlack - slack < 3)) {
        _makeDogleg( target->getGCell(), KbNoFlags );
        slackened = true;
      } else if (slack < 10) {
        halfSlackened = true;
      }

      const vector<AutoSegment*>& doglegs = Session::getDoglegs();
      if (doglegs.size() >= 2) {
        cdebug_log(149,0) << "AutoSegment::_slaken(): Source @" << DbU::getValueString(getTargetPosition()) << endl;
        doglegs[doglegs.size()-2]->_setAxis( getTargetPosition() );
        success = true;
      }
    }

    if (halfSlackened) {
      setFlags( SegHalfSlackened );
    } else if (slackened) {
      setFlags  ( SegSlackened );
      unsetFlags( SegHalfSlackened );
    }
    cdebug_tabw(149,-1);

    return success;
  }


  void  AutoVertical::_setAxis ( DbU::Unit axis )
  {
    setFlags( SegAxisSet );

    if (_vertical->getX() == axis) return;

    cdebug_log(149,0) << "_setAxis() @X " << DbU::toLambda(axis) << " " << this << endl;

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
    }
  }


  void  AutoVertical::updatePositions ()
  {
    _sourcePosition = _vertical->getSourceY() - Session::getExtensionCap(getLayer());
    _targetPosition = _vertical->getTargetY() + Session::getExtensionCap(getLayer());
  }


  bool  AutoVertical::checkPositions () const
  {
    bool      coherency      = true;
    DbU::Unit sourcePosition = _vertical->getSourceY() - Session::getExtensionCap(getLayer());
    DbU::Unit targetPosition = _vertical->getTargetY() + Session::getExtensionCap(getLayer());

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
  }


  bool  AutoVertical::canMoveURight ( float reserve ) const
  {
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
  }


  bool  AutoVertical::moveULeft ()
  {
    if (not getAutoSource()->isTurn() or not getAutoTarget()->isTurn()) return false;
    if (not getAutoSource()->getGCell()->getLeft()) return false;

    AutoContact* autoSource        = getAutoSource();
    AutoContact* autoTarget        = getAutoTarget();
    GCell*       begin             = autoSource->getGCell();
    GCell*       end               = autoTarget->getGCell();
    AutoSegment* perpandicular     = autoSource->getSegment(0);

    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Katabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoSource() == autoSource) {
        begin->addHSegment( perpandicular );
      } else {
        if (begin->getLeft() == perpandicular->getAutoSource()->getGCell()) {
          perpandicular->unsetFlags( Katabatic::SegGlobal );
        } else
          begin->getLeft()->removeHSegment( perpandicular );
      }
    }

    perpandicular = autoTarget->getSegment(0);
    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Katabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoSource() == autoTarget) {
        end->addHSegment( perpandicular );
      } else {
        if (end->getLeft() == perpandicular->getAutoSource()->getGCell()) {
          perpandicular->unsetFlags( Katabatic::SegGlobal );
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

    DbU::Unit x = begin->getSide( KbHorizontal ).getVMax();
    setAxis( x );

    return true;
  }


  bool  AutoVertical::moveURight ()
  {
    cdebug_log(149,0) << "AutoVertical::moveURight()" << endl;

    if (not getAutoSource()->isTurn() or not getAutoTarget()->isTurn()) return true;
    if (not getAutoSource()->getGCell()->getRight()) return true;

    AutoContact* autoSource        = getAutoSource();
    AutoContact* autoTarget        = getAutoTarget();
    GCell*       begin             = autoSource->getGCell();
    GCell*       end               = autoTarget->getGCell();
    AutoSegment* perpandicular     = autoSource->getSegment(0);

    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Katabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoTarget() == autoSource) {
        begin->addHSegment( perpandicular );
      } else {
        if (begin->getRight() == perpandicular->getAutoTarget()->getGCell()) {
          perpandicular->unsetFlags( Katabatic::SegGlobal );
        } else
          begin->getRight()->removeHSegment( perpandicular );
      }
    }

    perpandicular = autoTarget->getSegment(0);
    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Katabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoTarget() == autoTarget) {
        end->addHSegment( perpandicular );
      } else {
        if (end->getRight() == perpandicular->getAutoTarget()->getGCell()) {
          perpandicular->unsetFlags( Katabatic::SegGlobal );
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

    DbU::Unit x = begin->getSide( KbHorizontal ).getVMin();
    setAxis( x );

    cdebug_log(149,0) << "Moved to axis: " << DbU::getValueString(x) << endl;

    return true;
  }


  unsigned int  AutoVertical::_makeDogleg ( GCell* doglegGCell, unsigned int flags )
  {
    cdebug_log(149,0) << "AutoVertical::_makeDogleg(GCell*)" << endl;

    AutoContact*  autoSource = getAutoSource();
    AutoContact*  autoTarget = getAutoTarget();
    GCell*        begin      = autoSource->getGCell();
    GCell*        end        = autoTarget->getGCell();

  //Session::doglegReset();

    DbU::Unit doglegAxis = (doglegGCell->getYMax() + doglegGCell->getY()) / 2;
    if (isLocal())
      doglegAxis = (getSourceY() + getTargetY()) / 2;

    if (doglegGCell == begin) unsetFlags( SegGlobal );
    if (doglegGCell != end) {
      GCell* gcell = doglegGCell;
      do {
        if (gcell != begin)
          gcell->removeVSegment( this );
        gcell = gcell->getUp();
      } while ( gcell and (gcell != end) );
    }

    size_t       depth        = Session::getRoutingGauge()->getLayerDepth ( _vertical->getLayer() );
    bool         upLayer      = (depth+1 <= Session::getConfiguration()->getAllowedDepth());
    Layer*       contactLayer = Session::getRoutingGauge()->getContactLayer ( depth + ((upLayer)?0:-1) );
    const Layer* doglegLayer  = Session::getRoutingGauge()->getRoutingLayer ( depth + ((upLayer)?1:-1) );

    Session::dogleg( this );
    targetDetach();
    invalidate( KbTopology );
    autoTarget->invalidate( KbTopology );
    AutoContact* dlContact1 = AutoContactTurn::create( doglegGCell, _vertical->getNet(), contactLayer );
    cdebug_log(149,0) << dlContact1 << endl;
    AutoContact* dlContact2 = AutoContactTurn::create( doglegGCell, _vertical->getNet(), contactLayer );
    cdebug_log(149,0) << dlContact2 << endl;
    AutoSegment* segment1   = AutoSegment::create( dlContact1 , dlContact2, KbHorizontal );
    cdebug_log(149,0) << segment1 << endl;
    segment1->setLayer( doglegLayer );
    segment1->_setAxis( doglegAxis );
    segment1->setFlags( SegDogleg|SegSlackened|SegCanonical|SegNotAligned );
    cdebug_log(149,0) << "New " << dlContact1->base() << "." << endl;
    cdebug_log(149,0) << "New " << dlContact2->base() << "." << endl;
    Session::dogleg( segment1 );

    targetAttach( dlContact1 );
    AutoSegment* segment2 = AutoVertical::create ( dlContact2, autoTarget, KbVertical );
    autoTarget->cacheAttach( segment2 );
    segment2->setLayer( getLayer() );
    segment2->_setAxis( getX() );
    segment2->setFlags( (isSlackened()?SegSlackened:0) );
    Session::dogleg( segment2 );

    if (isSourceTerminal()) {
      segment1->setFlags( SegWeakTerminal1 );
      segment2->setFlags( SegWeakTerminal1 );
      autoTarget->unsetFlags( CntWeakTerminal );
      dlContact1->setFlags  ( CntWeakTerminal );
      if (autoTarget->getGCell() == doglegGCell)
        dlContact1->migrateConstraintBox( autoTarget );
    } else if (isTargetTerminal()) {
      unsetFlags( SegTargetTerminal );
      setFlags( SegWeakTerminal1 );
      segment1->setFlags( SegWeakTerminal1 );
      segment2->setFlags( SegTargetTerminal );
      autoSource->unsetFlags( CntWeakTerminal );
      dlContact2->setFlags  ( CntWeakTerminal );
      if (autoSource->getGCell() == doglegGCell)
        dlContact2->migrateConstraintBox( autoSource );
    } else if (isWeakTerminal()) {
      segment1->setFlags( SegWeakTerminal1 );
      segment2->setFlags( SegWeakTerminal1 );
    }

    cdebug_log(149,0) << "Session::dogleg[x+1] perpand:   " << segment1 << endl;
    cdebug_log(149,0) << "Session::dogleg[x+2] new paral: " << segment2 << endl;
    cdebug_log(149,0) << "Session::dogleg[x+0] original:  " << this << endl;

    dlContact1->updateCache();
    dlContact2->updateCache();
  //autoTarget->updateCache();

    segment2->canonize( flags );
    if (not isCanonical()) canonize( flags );

    return (upLayer) ? KbUseAboveLayer : KbUseBelowLayer;
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


} // End of Katabatic namespace.
