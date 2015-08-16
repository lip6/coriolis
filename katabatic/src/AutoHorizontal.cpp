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
// |  C++ Module  :       "./AutoHorizontal.cpp"                     |
// +-----------------------------------------------------------------+


#include  <algorithm>
#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "hurricane/DebugSession.h"
#include  "hurricane/RoutingPad.h"
#include  "crlcore/RoutingGauge.h"
#include  "katabatic/Configuration.h"
#include  "katabatic/AutoContactTerminal.h"
#include  "katabatic/AutoContactTurn.h"
#include  "katabatic/AutoHorizontal.h"
#include  "katabatic/AutoVertical.h"


namespace Katabatic {


  using std::min;
  using std::max;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::Error;
  using Hurricane::Bug;
  using Hurricane::DebugSession;
  using Hurricane::RoutingPad;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoHorizontal".


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
    ltrace(99) << "CTOR AutoHorizontal " << this << endl;
    ltrace(99) << "               over " << horizontal << endl;
  }


  void  AutoHorizontal::_postCreate ()
  {
    AutoSegment::_postCreate ();

    AutoContact* source = getAutoSource();
    if (source->isTerminal()) source->setY( _horizontal->getY() );

    AutoContact* target = getAutoTarget();
    if (target->isTerminal()) target->setY( _horizontal->getY() );

    if (source->getGCell() != target->getGCell()) {
      setFlags( SegGlobal );

      GCell* gcell;
      GCell* end;

      if ( source->getGCell()->getX() < target->getGCell()->getX() ) {
        gcell = source->getGCell()->getRight();
        end   = target->getGCell();
      } else {
        gcell = target->getGCell()->getRight();
        end   = source->getGCell();
      }

      for ( ; gcell != end ; gcell = gcell->getRight() ) {
        if ( !gcell ) {
          cerr << Error("AutoHorizontal::create() : NULL GCell.") << endl;
          break;
        }
        gcell->addHSegment ( this );
      }
    }
  }


  void  AutoHorizontal::_preDestroy ()
  {
    ltrace(200) << "AutoHorizontal::_preDestroy() - <id:" << getId() << "> " << endl;
    ltrace(200) << "  " << _getString() << endl;
    ltracein(90);

    if ( not Session::doDestroyTool() ) {
      AutoContact* source = getAutoSource();
      AutoContact* target = getAutoTarget();

      if ( source and target and (source->getGCell() != target->getGCell()) ) {
        GCell* gcell;
        GCell* end;

        if ( source->getGCell()->getX() < target->getGCell()->getX() ) {
          gcell = source->getGCell()->getRight();
          end   = target->getGCell();
        } else {
          gcell = target->getGCell()->getRight();
          end   = source->getGCell();
        }

        for ( ; gcell != end ; gcell = gcell->getRight() ) {
          if ( !gcell ) {
            cerr << Error("AutoHorizontal::_preDestroy() : NULL GCell.") << endl;
            break;
          }
          gcell->removeHSegment ( this );
        }
      }
    }

    AutoSegment::_preDestroy ();
    ltraceout(90);
  }


  AutoHorizontal::~AutoHorizontal ()
  {
    if ( Session::doDestroyBaseSegment() and not Session::doDestroyTool() ) {
      ltrace(200) << "~AutoHorizontal() - " << endl;
      _horizontal->destroy ();
    }
  }


  Interval  AutoHorizontal::getSourceConstraints ( unsigned int flags ) const
  {
    if (flags & KbNativeConstraints) {
      Box nativeBox ( getAutoSource()->getNativeConstraintBox() );
      return Interval ( nativeBox.getYMin(), nativeBox.getYMax() );
    }
    return Interval ( getAutoSource()->getCBYMin(), getAutoSource()->getCBYMax() );
  }


  Interval  AutoHorizontal::getTargetConstraints ( unsigned int flags ) const
  {
    if (flags & KbNativeConstraints) {
      Box nativeBox ( getAutoTarget()->getNativeConstraintBox() );
      return Interval ( nativeBox.getYMin(), nativeBox.getYMax() );
    }
    return Interval ( getAutoTarget()->getCBYMin(), getAutoTarget()->getCBYMax() );
  }


  bool  AutoHorizontal::getConstraints ( DbU::Unit& constraintMin, DbU::Unit& constraintMax ) const
  {
    AutoContact* contact = getAutoSource();
    constraintMin = contact->getCBYMin();
    constraintMax = contact->getCBYMax();

    ltrace(148) << "Source constraints: " << contact << " ["
                << DbU::getValueString(constraintMin) << ":"
                << DbU::getValueString(constraintMax) << "]"
                << endl;

    contact = getAutoTarget();
    constraintMin = max ( constraintMin, contact->getCBYMin() );
    constraintMax = min ( constraintMax, contact->getCBYMax() );

    ltrace(148) << "Merge with target constraints: " << contact << " ["
                << DbU::getValueString(constraintMin) << ":"
                << DbU::getValueString(constraintMax) << "]"
                << endl;

    constraintMin = max ( constraintMin, getUserConstraints().getVMin() );
    constraintMax = min ( constraintMax, getUserConstraints().getVMax() );

    ltrace(148) << "Merge with user constraints: " << getUserConstraints() << " ["
                << DbU::getValueString(constraintMin) << ":"
                << DbU::getValueString(constraintMax) << "]"
                << endl;
    
    return true;
  }


  unsigned int  AutoHorizontal::getDirection () const
  { return KbHorizontal; }


  size_t  AutoHorizontal::getGCells ( vector<GCell*>& gcells ) const
  {
    vector<GCell*>().swap ( gcells );
    gcells.push_back ( getAutoSource()->getGCell() );

    GCell* gcell = gcells.front();
    GCell* end   = getAutoTarget()->getGCell();

    while ( gcell != end ) {
      gcell = gcell->getRight ();
      if ( !gcell ) break;

      gcells.push_back ( gcell );
    }

    return gcells.size();
  }


  bool  AutoHorizontal::_canSlacken () const
  {
    ltracein(200);

    Interval sourceSide        = getAutoSource()->getGCell()->getSide( KbVertical );
    Interval targetSide        = getAutoTarget()->getGCell()->getSide( KbVertical );
    Interval sourceConstraints = Interval(getAutoSource()->getCBYMin(),getAutoSource()->getCBYMax());
    Interval targetConstraints = Interval(getAutoTarget()->getCBYMin(),getAutoTarget()->getCBYMax());

  // Expand by a tiny amount for the "contains" to work for sure.
    sourceConstraints.inflate( 1 );
    targetConstraints.inflate( 1 );

    ltrace(200) << "source " << getAutoSource() << endl;
    ltrace(200) << "source constraints: " << sourceConstraints
                << " " << DbU::getValueString(sourceConstraints.getSize()) << endl;
    ltrace(200) << "target " << getAutoTarget() << endl;
    ltrace(200) << "target constraints: " << targetConstraints
                << " " << DbU::getValueString(targetConstraints.getSize()) << endl;

    if (not sourceConstraints.contains(sourceSide)) { ltraceout(200); return true; }
    if (not targetConstraints.contains(targetSide)) { ltraceout(200); return true; }

    ltraceout(200);
    return false;
  }


  bool  AutoHorizontal::_slacken ( unsigned int flags )
  {
    ltrace(200) << "AutoHorizontal::_slacken() " << this << endl;

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

    ltracein(200);
    ltrace(200) << "_flags:" << (_flags & (SegGlobal|SegWeakGlobal)) << endl;
    ltrace(200) << "test:" << (getLength() < 5*getPitch()) << endl;
    ltrace(200) << "length:" << DbU::getValueString(getLength()) << endl;

    int          lowSlack       = (flags & KbHalfSlacken) ? 3 : 10;
    bool         slackened      = false;
    bool         halfSlackened  = false;
    DbU::Unit    targetPosition = getTargetPosition();
    AutoSegment* parallel       = this;

    if (source->isTerminal()) {
      Interval  perpandConstraints = getAutoTarget()->getUConstraints(KbHorizontal);
      Interval  constraints        = source->getUConstraints      (KbVertical|KbNoGCellShrink);
      Interval  nativeConstraints  = source->getNativeUConstraints(KbVertical|KbNoGCellShrink);
      int       slack              = constraints.getSize()       / getPitch();
      int       nativeSlack        = nativeConstraints.getSize() / getPitch();

      ltrace(200) << "Source constraint: " << constraints
                  << " slack:"        << slack
                  << " native slack:" << nativeSlack << endl;
      ltrace(200) << "Perpand constraints on target: " << perpandConstraints << endl; 
    // Ugly: GCell's track number is hardwired.
      if ((nativeSlack < lowSlack) or (nativeSlack - slack < 3)) {
        ltrace(200) << "Slackening from Source: " << source << endl;
        _makeDogleg( source->getGCell(), KbNoFlags );
        slackened = true;
      } else if (slack < 10) {
        halfSlackened = true;
      }

      const vector<AutoSegment*>& doglegs = Session::getDoglegs();
      if (doglegs.size() >= 2) {
        ltrace(200) << "AutoSegment::_slaken(): Source @" << DbU::getValueString(getSourcePosition()) << endl;
        doglegs[doglegs.size()-2]->setAxis( getSourcePosition() );
        success = true;

        if (isMetal2Source) {
          ltrace(200) << "Fixing on source terminal contact." << endl;
          doglegs[doglegs.size()-2]->getAutoSource()->setFlags( CntFixed );
        }

        parallel = doglegs[ doglegs.size()-1 ];
      }
    }

    if (parallel) target = parallel->getAutoTarget();

    if (target->isTerminal()) {
      Interval  constraints       = target->getUConstraints      (KbVertical|KbNoGCellShrink);
      Interval  nativeConstraints = target->getNativeUConstraints(KbVertical|KbNoGCellShrink);
      int       slack             = constraints.getSize()       / getPitch();
      int       nativeSlack       = nativeConstraints.getSize() / getPitch();

    // Ugly: GCell's track number is hardwired.
      ltrace(200) << "Target constraint: " << constraints
                  << " slack:"        << slack
                  << " native slack:" << nativeSlack << endl;
      if ((nativeSlack < lowSlack) or (nativeSlack - slack < 3)) {
        ltrace(200) << "Slackening from Target: " << target << endl;
        parallel->_makeDogleg( target->getGCell(), KbNoFlags );
        slackened = true;
      } else if (slack < 10) {
        halfSlackened = true;
      }

      if (halfSlackened) {
        setFlags( SegHalfSlackened );
      } else if (slackened) {
        setFlags  ( SegSlackened );
        unsetFlags( SegHalfSlackened );
      }

      const vector<AutoSegment*>& doglegs = Session::getDoglegs();
      if (doglegs.size() >= 2) {
        ltrace(200) << "AutoSegment::_slaken(): Target @" << DbU::getValueString(targetPosition) << endl;
        doglegs[doglegs.size()-2]->setAxis( targetPosition );
        success = true;

        if (isMetal2Target) {
          ltrace(200) << "Fixing on target terminal contact." << endl;
          doglegs[doglegs.size()-2]->getAutoTarget()->setFlags( CntFixed );
        }
      }
    }
    ltraceout(200);

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

    ltrace(80) << "_setAxis() @Y " << DbU::toLambda(axis) << " " << this << endl;

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
      ltrace(80) << "updateOrient() " << this << " (before S/T swap)" << endl;
      _horizontal->invert();

      unsigned int spinFlags = _flags & SegDepthSpin;
      unsetFlags( SegDepthSpin );
      if (spinFlags & SegSourceTop   ) setFlags( SegTargetTop );
      if (spinFlags & SegSourceBottom) setFlags( SegTargetBottom );
      if (spinFlags & SegTargetTop   ) setFlags( SegSourceTop );
      if (spinFlags & SegTargetBottom) setFlags( SegSourceBottom );
    }
  }


  void  AutoHorizontal::updatePositions ()
  {
    _sourcePosition = _horizontal->getSourceX() - Session::getExtensionCap(getLayer());
    _targetPosition = _horizontal->getTargetX() + Session::getExtensionCap(getLayer());
  }


  bool  AutoHorizontal::checkPositions () const
  {
    bool      coherency      = true;
    DbU::Unit sourcePosition = _horizontal->getSourceX() - Session::getExtensionCap(getLayer());
    DbU::Unit targetPosition = _horizontal->getTargetX() + Session::getExtensionCap(getLayer());

    if ( _sourcePosition != sourcePosition ) {
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
  }


  bool  AutoHorizontal::canMoveURight ( float reserve ) const
  {
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
  }


  bool  AutoHorizontal::moveULeft ()
  {
    if (not getAutoSource()->isTurn() or not getAutoTarget()->isTurn()) return false;
    if (not getAutoSource()->getGCell()->getDown()) return false;

    AutoContact* autoSource        = getAutoSource();
    AutoContact* autoTarget        = getAutoTarget();
    GCell*       begin             = autoSource->getGCell();
    GCell*       end               = autoTarget->getGCell();
    AutoSegment* perpandicular     = autoSource->getSegment(2);

    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Katabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoSource() == autoSource) {
        begin->addVSegment( perpandicular );
      } else {
        if (begin->getDown() == perpandicular->getAutoSource()->getGCell()) {
          perpandicular->unsetFlags( Katabatic::SegGlobal );
        } else
          begin->getDown()->removeVSegment( perpandicular );
      }
    }

    perpandicular = autoTarget->getSegment(2);
    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Katabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoSource() == autoTarget) {
        end->addVSegment( perpandicular );
      } else {
        if (end->getDown() == perpandicular->getAutoSource()->getGCell()) {
          perpandicular->unsetFlags( Katabatic::SegGlobal );
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

    DbU::Unit y = begin->getSide(KbVertical).getVMax();
    setAxis( y );

    return true;
  }


  bool  AutoHorizontal::moveURight ()
  {
  //cerr << "moveURight() " << this << endl;

    if (not getAutoSource()->isTurn() or not getAutoTarget()->isTurn()) return false;
    if (not getAutoSource()->getGCell()->getUp()) return false;

    AutoContact* autoSource        = getAutoSource();
    AutoContact* autoTarget        = getAutoTarget();
    GCell*       begin             = autoSource->getGCell();
    GCell*       end               = autoTarget->getGCell();
    AutoSegment* perpandicular     = autoSource->getSegment(2);

    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Katabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoTarget() == autoSource) {
        begin->addVSegment( perpandicular );
      } else {
        if (begin->getUp() == perpandicular->getAutoTarget()->getGCell()) {
          perpandicular->unsetFlags( Katabatic::SegGlobal );
        } else
          begin->getUp()->removeVSegment( perpandicular );
      }
    }

    perpandicular = autoTarget->getSegment(2);
    if (perpandicular->isLocal()) {
      perpandicular->setFlags( Katabatic::SegGlobal );
    } else {
      if (perpandicular->getAutoTarget() == autoTarget) {
        end->addVSegment( perpandicular );
      } else {
        if (end->getUp() == perpandicular->getAutoTarget()->getGCell()) {
          perpandicular->unsetFlags( Katabatic::SegGlobal );
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

    DbU::Unit y = begin->getSide( KbVertical ).getVMin();
    setAxis( y );

    return true;
  }


  unsigned int  AutoHorizontal::_makeDogleg ( GCell* doglegGCell, unsigned int flags )
  {
    DebugSession::open( getNet(), 80 );
    ltrace(200) << "AutoHorizontal::_makeDogleg(GCell*)" << endl;
    ltracein(159);

  //Session::doglegReset();

    AutoContact*  autoTarget      = getAutoTarget();
    AutoContact*  autoSource      = getAutoSource();
    GCell*        begin           = autoSource->getGCell();
    GCell*        end             = autoTarget->getGCell();

    DbU::Unit doglegAxis = (doglegGCell->getXMax() + doglegGCell->getX()) / 2;
    if (isLocal())
      doglegAxis = (getSourceX() + getTargetX()) / 2;

    ltrace(159) << "Detaching from Target AutoContact " << autoTarget << "." << endl;

    if (doglegGCell == begin) unsetFlags( SegGlobal );
    if (doglegGCell != end) {
      GCell* gcell = doglegGCell;
      do {
        if (gcell != begin)
          gcell->removeHSegment( this );
        gcell = gcell->getRight();
      } while ( gcell and (gcell != end) );
    }

    size_t       depth        = Session::getRoutingGauge()->getLayerDepth( _horizontal->getLayer() );
    bool         upLayer      = (depth+1 <= Session::getConfiguration()->getAllowedDepth());
    Layer*       contactLayer = Session::getRoutingGauge()->getContactLayer( depth + ((upLayer)?0:-1) );
    const Layer* doglegLayer  = Session::getRoutingGauge()->getRoutingLayer( depth + ((upLayer)?1:-1) );

    Session::dogleg( this );
    targetDetach();
    invalidate( KbTopology );
    autoTarget->invalidate( KbTopology );
    AutoContact* dlContact1 = AutoContactTurn::create( doglegGCell, _horizontal->getNet(), contactLayer );
    AutoContact* dlContact2 = AutoContactTurn::create( doglegGCell, _horizontal->getNet(), contactLayer );
    AutoSegment* segment1   = AutoSegment::create( dlContact1 , dlContact2, KbVertical );
    segment1->setLayer( doglegLayer );
    segment1->_setAxis( doglegAxis );
    segment1->setFlags( SegDogleg|SegSlackened|SegCanonical|SegNotAligned );

    ltrace(200) << "New " << dlContact1 << endl;
    ltrace(200) << "New " << dlContact2 << endl;
    Session::dogleg( segment1 );

    targetAttach( dlContact1 );
    AutoSegment* segment2 = AutoSegment::create( dlContact2 , autoTarget, KbHorizontal );
    autoTarget->cacheAttach( segment2 );
    segment2->setLayer( getLayer() );
    segment2->_setAxis( getY() );
    segment2->setFlags( (isSlackened()?SegSlackened:0) );
    Session::dogleg( segment2 );

    if (isSourceTerminal()) {
      segment1->setFlags( SegWeakTerminal1 );
      segment2->setFlags( SegWeakTerminal1 );
    } else if (isTargetTerminal()) {
      unsetFlags( SegTargetTerminal );
      setFlags( SegWeakTerminal1 );
      segment1->setFlags( SegWeakTerminal1 );
      segment2->setFlags( SegTargetTerminal );
    } else if (isWeakTerminal()) {
      segment1->setFlags( SegWeakTerminal1 );
      segment2->setFlags( SegWeakTerminal1 );
    }

    ltrace(200) << "Session::dogleg[x+1] perpand:   " << segment1 << endl;
    ltrace(200) << "Session::dogleg[x+2] new paral: " << segment2 << endl;
    ltrace(200) << "Session::dogleg[x+0] original:  " << this << endl;

    dlContact1->updateCache();
    dlContact2->updateCache();
  //autoTarget->updateCache();

    segment2->canonize( flags );
    if (not isCanonical()) canonize( flags );

    ltraceout(159);
    DebugSession::close();

    return (upLayer) ? KbUseAboveLayer : KbUseBelowLayer;
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


} // End of Katabatic namespace.
