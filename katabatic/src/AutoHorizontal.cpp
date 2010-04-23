
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
// |  C++ Module  :       "./AutoHorizontal.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <algorithm>
#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "hurricane/DebugSession.h"
#include  "crlcore/RoutingGauge.h"
#include  "katabatic/Configuration.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoHorizontal.h"
#include  "katabatic/AutoVertical.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace Katabatic;


  bool  slacken ( AutoContact* contact )
  {
    if ( contact->getLayer() != Session::getConfiguration()->getContactLayer(0) )
      return false;

    ltrace(200) << "Session::slacken(): " << contact << endl;

    vector<Hook*> hooks;
    forEach ( Hook*, ihook, contact->getBodyHook()->getSlaveHooks() ) hooks.push_back ( *ihook );

    contact->getBodyHook()->_setNextHook ( contact->getBodyHook() );
    for ( size_t i=0 ; i<hooks.size() ; i++ ) hooks[i]->_setNextHook ( hooks[i] );

    const Layer* paralLayer   = Session::getConfiguration()->getRoutingLayer(1);
    const Layer* perpandLayer = Session::getConfiguration()->getRoutingLayer(2);
    const Layer* contactLayer = Session::getConfiguration()->getContactLayer(1);
    AutoContact* contact1     = AutoContact::create ( contact->getGCell(), contact->getNet(), contactLayer );
    AutoContact* contact2     = AutoContact::create ( contact->getGCell(), contact->getNet(), contactLayer );

    AutoSegment* hsegment = AutoHorizontal::create ( contact
                                                   , contact1
                                                   , paralLayer
                                                   , contact->getY()
                                                   , DbU::lambda(2.0)
                                                   , AutoSegment::Local
                                                   , true  // terminal
                                                   , false // collapsed
                                                   );

    AutoSegment* vsegment = AutoVertical::create ( contact1
                                                 , contact2
                                                 , perpandLayer
                                                 , contact->getX()
                                                 , DbU::lambda(2.0)
                                                 , AutoSegment::Local
                                                 , false
                                                 , false
                                                 );

    for ( size_t i=0 ; i<hooks.size() ; i++ ) hooks[i]->attach ( contact2->getBodyHook() );

    contact2->setVAlignate      ( true );
    contact2->restoreHConnexity ( contact->getX(), true );

    hsegment->setSlackened    ( true );
    vsegment->setSlackened    ( true );
    vsegment->setSlackenStrap ( true );

    ltrace(200) << "Session::slacken() new paral: " << hsegment << endl;
    ltrace(200) << "Session::slacken() perpand:   " << vsegment << endl;

    return true;
  }


  void  slacken ( AutoHorizontal* segment, bool fromSource )
  {
    AutoContact* contact = NULL;
    Point        slackPoint;
    if (fromSource) {
      contact    = segment->getAutoSource();
      slackPoint = Point ( segment->getSourceX(), segment->getSourceY() );
    } else {
      contact    = segment->getAutoTarget();
      slackPoint = Point ( segment->getTargetX(), segment->getTargetY() );
    }
    if ( !contact ) return;

    forEach ( Vertical*, ivertical, contact->getSlaveComponents().getSubSet<Vertical*>() ) {
      AutoSegment* autoVertical = Session::lookup ( *ivertical );
      if ( autoVertical ) {
        autoVertical->invalidate ();
        if ( autoVertical->isGlobal() and not contact->getAnchor() ) return;
      }
    }

    if ( slacken(contact) ) return;

    size_t       depth        = Session::getRoutingGauge()->getLayerDepth   ( segment->getLayer() );
    Layer*       contactLayer = Session::getRoutingGauge()->getContactLayer ( depth );
    const Layer* slackLayer   = Session::getRoutingGauge()->getRoutingLayer ( depth + 1 );

    if ( fromSource ) segment->getSourceHook()->detach ();
    else              segment->getTargetHook()->detach ();

    AutoContact* contact1 = AutoContact::create ( contact->getGCell(), segment->getNet(), contactLayer );
    AutoContact* contact2 = AutoContact::create ( contact->getGCell(), segment->getNet(), contactLayer );

    AutoSegment* hsegment = AutoHorizontal::create ( contact
                                                   , contact1
                                                   , segment->getLayer()
                                                   , slackPoint.getY()
                                                   , DbU::lambda(2.0)
                                                   , AutoSegment::Local
                                                   , false
                                                   , false
                                                   );

    AutoSegment* vsegment = AutoVertical::create ( contact1
                                                 , contact2
                                                 , slackLayer
                                                 , slackPoint.getX()
                                                 , DbU::lambda(2.0)
                                                 , AutoSegment::Local
                                                 , false
                                                 , false
                                                 );

    if ( fromSource )
      segment->getSourceHook()->attach ( contact2->getContact()->getBodyHook() );
    else
      segment->getTargetHook()->attach ( contact2->getContact()->getBodyHook() );
    contact->restoreVConnexity ( slackPoint.getY(), true );

    hsegment->setSlackened    ( true );
    vsegment->setSlackened    ( true );
    vsegment->setSlackenStrap ( true );

  //contact->setVAlignate ( true );

    ltrace(200) << "Session::slacken() new paral: " << hsegment << endl;
    ltrace(200) << "Session::slacken() perpand:   " << vsegment << endl;
    ltrace(200) << "Session::slacken() original:  " << segment  << endl;
  }


} // End of local namespace.


namespace Katabatic {


  using std::min;
  using std::max;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::Error;
  using Hurricane::Bug;
  using Hurricane::DebugSession;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoHorizontal".


  AutoHorizontal::AutoHorizontal ( Horizontal* horizontal
                                 , int         type
                                 , bool        terminal
                                 , bool        collapsed
                                 )
    : AutoSegment(horizontal,true,type,terminal,collapsed)
    , _horizontal(horizontal)
  {
  }


  void  AutoHorizontal::_postCreate ()
  {
    AutoSegment::_postCreate ();
    orient       ();
    setPositions ();

    AutoContact* source = getAutoSource();
    if ( source->isTerminal() ) source->setY ( _horizontal->getY() );

    AutoContact* target = getAutoTarget();
    if ( target->isTerminal() ) target->setY ( _horizontal->getY() );

    if ( source->getGCell() == target->getGCell() ) {
      setGlobal ( false );
    } else {
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


  AutoHorizontal* AutoHorizontal::create ( Horizontal* horizontal
                                         , int         type
                                         , bool        terminal
                                         , bool        collapsed
                                         )
  {
    AutoSegment::_preCreate ( horizontal->getSource(), horizontal->getTarget() );
    AutoHorizontal* autoHorizontal = new AutoHorizontal ( horizontal
                                                        , type
                                                        , terminal
                                                        , collapsed
                                                        );
    autoHorizontal->_postCreate ();
    return autoHorizontal;
  }


  AutoHorizontal* AutoHorizontal::create ( AutoContact* source
                                         , AutoContact* target
                                         , const Layer* layer
                                         , DbU::Unit    y
                                         , DbU::Unit    width
                                         , int          type
                                         , bool         terminal
                                         , bool         collapsed
                                         )
  {
    AutoSegment::_preCreate ( source, target );
    AutoHorizontal* autoHorizontal
      = new AutoHorizontal ( Horizontal::create ( source->getContact()
                                                , target->getContact()
                                                , layer
                                                , y
                                                , width ), type, terminal, collapsed );

    autoHorizontal->_postCreate ();

    return autoHorizontal;
  }


  void  AutoHorizontal::_preDestroy ()
  {
    ltrace(200) << "AutoHorizontal::_preDestroy() - <id:" << getId() << "> " << (void*)this << endl;
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
      ltrace(200) << "~AutoHorizontal() - " << (void*)_horizontal << endl;
      _horizontal->destroy ();
    }
  }


  Interval  AutoHorizontal::getSourceConstraints ( bool native ) const
  {
    if ( native ) {
      Box nativeBox ( getAutoSource()->getNativeConstraintBox() );
      return Interval ( nativeBox.getYMin(), nativeBox.getYMax() );
    }
    return Interval ( getAutoSource()->getCBYMin(), getAutoSource()->getCBYMax() );
  }


  Interval  AutoHorizontal::getTargetConstraints ( bool native ) const
  {
    if ( native ) {
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
  { return Constant::Horizontal; }


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
    Interval sourceConstraints = Interval(getAutoSource()->getCBYMin(),getAutoSource()->getCBYMax());
    Interval targetConstraints = Interval(getAutoTarget()->getCBYMin(),getAutoTarget()->getCBYMax());

  // Ugly: should uses topRightShrink from GCell.
    sourceConstraints.inflate ( 0, /*Katabatic::GCell::getTopRightShrink()*/ DbU::lambda(1.0) );
    targetConstraints.inflate ( 0, /*Katabatic::GCell::getTopRightShrink()*/ DbU::lambda(1.0) );

    ltrace(200) << "source " << (void*)getAutoSource() << ":" << getAutoSource() << endl;
    ltrace(200) << "source constraints: " << sourceConstraints
                << " " << DbU::getValueString(sourceConstraints.getSize()) << endl;
    ltrace(200) << "target " << (void*)getAutoTarget() << ":" << getAutoTarget() << endl;
    ltrace(200) << "target constraints: " << targetConstraints
                << " " << DbU::getValueString(targetConstraints.getSize()) << endl;

  // Ugly: GCell's track number is hardwired.
    if ( sourceConstraints.getSize() / DbU::lambda(5.0) < 10 ) return true;
    if ( targetConstraints.getSize() / DbU::lambda(5.0) < 10 ) return true;

    return false;
  }


  void  AutoHorizontal::_slacken ()
  {
    ltrace(200) << "AutoHorizontal::_slacken() " << this << endl;
    ltracein(200);

    AutoContact* contact     = getAutoSource();
    Interval     constraints = Interval(contact->getCBYMin(),contact->getCBYMax());

    constraints.inflate ( 0, /*Katabatic::GCell::getTopRightShrink()*/ DbU::lambda(1.0) );

  // Ugly: GCell's track number is hardwired.
    if ( constraints.getSize() / DbU::lambda(5.0) < 10 ) ::slacken ( this, true );

    contact     = getAutoTarget ();
    constraints = Interval(contact->getCBYMin(),contact->getCBYMax());
    constraints.inflate ( 0, /*Katabatic::GCell::getTopRightShrink()*/ DbU::lambda(1.0) );

    if ( constraints.getSize() / DbU::lambda(5.0) < 10 ) ::slacken ( this, false );

    setSlackened ( true );

    ltraceout(200);
  }


  bool  AutoHorizontal::canDesalignate ( AutoContact* contact ) const
  { return contact->canHDesalignate(); }


  void  AutoHorizontal::desalignate ( AutoContact* contact )
  { contact->hDesalignate(); }


  void  AutoHorizontal::alignate ( DbU::Unit axis )
  {
    if ( _horizontal->getY() == axis ) return;

    ltrace(80) << "alignate() " << (void*)this << " " << this << " @Y " << DbU::getLambda(axis) << endl;

    _horizontal->setY ( axis );
    invalidate ();

    AutoContact* anchor = getAutoSource();
    anchor->invalidate ();
    if ( anchor->isTerminal() ) anchor->setY ( axis );

    anchor = getAutoTarget();
    anchor->invalidate ();
    if ( anchor->isTerminal() ) anchor->setY ( axis );
  }


  void  AutoHorizontal::orient ()
  {
    if ( _horizontal->getTargetX() < _horizontal->getSourceX() )
      _horizontal->invert ();
  }


  void  AutoHorizontal::setPositions ()
  {
    _sourcePosition = _horizontal->getSourceX() - Session::getExtensionCap();
    _targetPosition = _horizontal->getTargetX() + Session::getExtensionCap();
  }


  bool  AutoHorizontal::checkPositions () const
  {
    bool      coherency      = true;
    DbU::Unit sourcePosition = _horizontal->getSourceX() - Session::getExtensionCap();
    DbU::Unit targetPosition = _horizontal->getTargetX() + Session::getExtensionCap();

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

    if ( !sourceConstraints.intersect(targetConstraints) ) {
      cerr << Error ( "%p:%s\n        Constraints incoherency: S:%p:%s, T:%p:%s"
                    , (void*)base()
                    , _getString().c_str() 
                    , getAutoSource()->getContact()
                    , getString(sourceConstraints).c_str()
                    , getAutoTarget()->getContact()
                    , getString(targetConstraints).c_str()
                    ) << endl;
      return false;
    }

    return true;
  }


  void  AutoHorizontal::_computeTerminal ()
  { _computeTerminal(_horizontal); }


  void  AutoHorizontal::moveULeft ()
  {
    if ( not getAutoSource()->isCorner() or not getAutoTarget()->isCorner() ) return;
    if ( not getAutoSource()->getGCell()->getDown() ) return;

    Session::setInvalidateMask ( Session::RestoreHCon|Session::NetCanonize );
    Session::invalidate ( getNet() );

    AutoContact* autoSource = getAutoSource();
    AutoContact* autoTarget = getAutoTarget();
    GCell*       begin      = autoSource->getGCell();
    GCell*       end        = autoTarget->getGCell();

    forEach ( Vertical*, isegment, autoSource->getSlaveComponents().getSubSet<Vertical*>() ) {
      AutoSegment* segment = Session::lookup ( *isegment );
      if ( segment->isLocal() ) {
        segment->setGlobal ( true );
        continue;
      }

      if ( segment->getAutoSource() == autoSource ) {
        begin->addVSegment ( segment );
      } else {
        if ( begin->getDown() == segment->getAutoSource()->getGCell() ) {
          segment->setGlobal ( false );
          segment->getAutoSource()->invalidate ();
        } else
          begin->getDown()->removeVSegment ( segment );
      }
    }

    forEach ( Vertical*, isegment, autoTarget->getSlaveComponents().getSubSet<Vertical*>() ) {
      AutoSegment* segment = Session::lookup ( *isegment );
      if ( segment->isLocal() ) {
        segment->setGlobal ( true );
        continue;
      }

      if ( segment->getAutoSource() == autoTarget ) {
        begin->addVSegment ( segment );
      } else {
        if ( end->getDown() == segment->getAutoSource()->getGCell() ) {
          segment->setGlobal ( false );
          segment->getAutoSource()->invalidate ();
        } else
          end->getDown()->removeVSegment ( segment );
      }
    }

    if ( begin != end ) {
      for ( GCell* gcell = begin->getLeft() ; gcell != end ; gcell = gcell->getLeft() )
        gcell->removeHSegment ( this );
    }

    begin = begin->getDown();
    end   = end  ->getDown();

    autoSource->setGCell ( begin );
    autoTarget->setGCell ( end   );
    if ( begin != end ) {
      for ( GCell* gcell = begin->getLeft() ; gcell != end ; gcell = gcell->getLeft() )
        gcell->addHSegment ( this );
    }

    DbU::Unit y = begin->getUSide(Constant::Vertical).getVMax();
    setAxis ( y );

    Session::revalidateTopology ();
  }


  void  AutoHorizontal::moveURight ()
  {
    if ( not getAutoSource()->isCorner() or not getAutoTarget()->isCorner() ) return;
    if ( not getAutoSource()->getGCell()->getUp() ) return;

    Session::setInvalidateMask ( Session::RestoreVCon|Session::NetCanonize );
    Session::invalidate ( getNet() );

    AutoContact* autoSource = getAutoSource();
    AutoContact* autoTarget = getAutoTarget();
    GCell*       begin      = autoSource->getGCell();
    GCell*       end        = autoTarget->getGCell();

    forEach ( Vertical*, isegment, autoSource->getSlaveComponents().getSubSet<Vertical*>() ) {
      AutoSegment* segment = Session::lookup ( *isegment );
      if ( segment->isLocal() ) {
        segment->setGlobal ( true );
        continue;
      }

      if ( segment->getAutoTarget() == autoSource ) {
        begin->addVSegment ( segment );
      } else {
        if ( begin->getUp() == segment->getAutoTarget()->getGCell() ) {
          segment->setGlobal ( false );
          segment->getAutoTarget()->invalidate ();
        } else
          begin->getUp()->removeVSegment ( segment );
      }
    }

    forEach ( Vertical*, isegment, autoTarget->getSlaveComponents().getSubSet<Vertical*>() ) {
      AutoSegment* segment = Session::lookup ( *isegment );
      if ( segment->isLocal() ) {
        segment->setGlobal ( true );
        continue;
      }

      if ( segment->getAutoTarget() == autoTarget ) {
        begin->addVSegment ( segment );
      } else {
        if ( end->getUp() == segment->getAutoTarget()->getGCell() ) {
          segment->setGlobal ( false );
          segment->getAutoTarget()->invalidate ();
        } else
          end->getUp()->removeVSegment ( segment );
      }
    }

    if ( begin != end ) {
      for ( GCell* gcell = begin->getLeft() ; gcell != end ; gcell = gcell->getLeft() )
        gcell->removeHSegment ( this );
    }

    begin = begin->getUp();
    end   = end  ->getUp();

    autoSource->setGCell ( begin );
    autoTarget->setGCell ( end   );
    if ( begin != end ) {
      for ( GCell* gcell = begin->getLeft() ; gcell != end ; gcell = gcell->getLeft() )
        gcell->addHSegment ( this );
    }

    DbU::Unit y = begin->getUSide(Constant::Vertical).getVMin();
    setAxis ( y );

    Session::revalidateTopology ();
  }


  void  AutoHorizontal::_makeDogLeg ( GCell* dogLegGCell, bool upLayer )
  {
    DebugSession::open ( getNet(), 110 );
    ltracein(159);

    Session::dogLegReset ();

    AutoContact*  autoTarget      = getAutoTarget();
    AutoContact*  autoSource      = getAutoSource();
    GCell*        begin           = autoSource->getGCell();
    GCell*        end             = autoTarget->getGCell();
    unsigned int  fragmentType    = AutoSegment::Global;
    unsigned int  splittedType    = AutoSegment::Global;

  //DbU::Unit dogLegAxis = (detachSource)?dogLegGCell->getXMax():dogLegGCell->getX();
    DbU::Unit dogLegAxis = (dogLegGCell->getXMax() + dogLegGCell->getX()) / 2;
    if ( isLocal() )
      dogLegAxis = (getSourceX() + getTargetX()) / 2;

    ltrace(159) << "Detaching Target AutoContact "
                << (void*)autoTarget->getContact() << ":"
                << autoTarget->getContact() << "." << endl;
    if ( end == dogLegGCell ) {
      fragmentType = AutoSegment::Local;
    }
    if ( begin == dogLegGCell ) {
      setGlobal ( false );
      splittedType = AutoSegment::Local;
    }

    autoTarget->invalidate ();
    autoTarget->setInvalidatedTopology ( true );

    if ( dogLegGCell != end ) {
      GCell* gcell = dogLegGCell;
      do {
        if ( gcell != begin )
          gcell->removeHSegment ( this );
        gcell = gcell->getRight ();
      } while ( gcell && (gcell != end) );
    }

    Session::dogLeg ( this );

    size_t       depth        = Session::getRoutingGauge()->getLayerDepth ( _horizontal->getLayer() );
    Layer*       contactLayer = Session::getRoutingGauge()->getContactLayer ( depth + ((upLayer)?0:-1) );
    const Layer* dogLegLayer  = Session::getRoutingGauge()->getRoutingLayer ( depth + ((upLayer)?1:-1) );

    _horizontal->getTargetHook()->detach ();

    AutoContact* dlContact1 = AutoContact::create ( dogLegGCell, _horizontal->getNet(), contactLayer );
    AutoContact* dlContact2 = AutoContact::create ( dogLegGCell, _horizontal->getNet(), contactLayer );
    AutoSegment* segment1   = AutoVertical::create ( dlContact1
                                                   , dlContact2
                                                   , dogLegLayer
                                                   , dogLegAxis
                                                   , DbU::lambda(2.0)
                                                   , AutoSegment::Local
                                                   , false
                                                   , false
                                                   );

    ltrace(200) << "New " << (void*)dlContact1->getContact() << ":" << dlContact1->getContact() << "." << endl;
    ltrace(200) << "New " << (void*)dlContact2->getContact() << ":" << dlContact2->getContact() << "." << endl;
    ltrace(200) << "Session::dogLeg[1] perpand:   " << segment1 << endl;
    Session::dogLeg ( segment1 );

    _horizontal->getTargetHook()->attach ( dlContact1->getContact()->getBodyHook() );
    AutoSegment* segment2 = AutoHorizontal::create ( dlContact2
                                                   , autoTarget
                                                   , getLayer()
                                                   , getY()
                                                   , DbU::lambda(2.0)
                                                   , fragmentType
                                                   , false
                                                   , false
                                                   );
    segment2->setAxis ( getY(), AxisSet );
    ltrace(200) << "Session::dogLeg[2] new paral: " << segment2 << endl;
    ltrace(200) << "Session::dogLeg[0] original:  " << this << endl;
    Session::dogLeg ( segment2 );

    setGlobal ( (splittedType == AutoSegment::Global) );

    if ( (splittedType == AutoSegment::Global) or ( fragmentType == AutoSegment::Global ) ) {
      if ( splittedType == AutoSegment::Local ) autoSource->restoreVConnexity ( getY(), true );
      else                                      autoTarget->restoreVConnexity ( getY(), true );
    }

    setTerminal ( false );
    segment2->setTerminal ( false );
    if ( autoSource->getAnchor() and not isGlobal() ) setTerminal ( true );
    if ( autoTarget->getAnchor() and not segment2->isGlobal() ) segment2->setTerminal ( true );

    segment1->setSlackened ( true );
    segment2->setSlackened ( isSlackened() );

    Session::invalidate         ( getNet() );
    Session::revalidateTopology ();

    ltraceout(159);
    DebugSession::close ();
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
