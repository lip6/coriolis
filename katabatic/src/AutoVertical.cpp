
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
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
// |  C++ Module  :       "./AutoVertical.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <algorithm>
#include  "hurricane/Bug.h"
#include  "hurricane/Vertical.h"
#include  "crlcore/RoutingGauge.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoVertical.h"
#include  "katabatic/AutoHorizontal.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace Katabatic;


  void  slacken ( AutoVertical* segment, bool fromSource )
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

    forEach ( Horizontal*, ihorizontal, contact->getSlaveComponents().getSubSet<Horizontal*>() ) {
      AutoSegment* autoHorizontal = Session::lookup ( *ihorizontal );
      if ( autoHorizontal ) {
        autoHorizontal->invalidate ();
        if ( autoHorizontal->isGlobal() ) return;
      }
    }

    size_t       depth        = Session::getRoutingGauge()->getLayerDepth   ( segment->getLayer() );
    Layer*       contactLayer = Session::getRoutingGauge()->getContactLayer ( depth - 1 );
    const Layer* slackLayer   = Session::getRoutingGauge()->getRoutingLayer ( depth - 1 );

    if ( fromSource ) segment->getSourceHook()->detach ();
    else              segment->getTargetHook()->detach ();

    AutoContact* contact1 = AutoContact::create ( contact->getGCell(), segment->getNet(), contactLayer );
    AutoContact* contact2 = AutoContact::create ( contact->getGCell(), segment->getNet(), contactLayer );

    AutoSegment* vsegment = AutoVertical::create ( contact
                                                 , contact1
                                                 , segment->getLayer()
                                                 , slackPoint.getX()
                                                 , DbU::lambda(2.0)
                                                 , AutoSegment::Local
                                                 , true  // terminal
                                                 , false // collapsed
                                                 );

    AutoSegment* hsegment = AutoHorizontal::create ( contact1
                                                   , contact2
                                                   , slackLayer
                                                   , slackPoint.getY()
                                                   , DbU::lambda(2.0)
                                                   , AutoSegment::Local
                                                   , false
                                                   , false
                                                   );

    if ( fromSource ) 
      segment->getSourceHook()->attach ( contact2->getContact()->getBodyHook() );
    else
      segment->getTargetHook()->attach ( contact2->getContact()->getBodyHook() );
    contact->restoreHConnexity ( slackPoint.getY(), true );

  //contact->setHAlignate ( true );

    hsegment->setSlackenStrap ( true );
    hsegment->setSlackened    ( true );
    vsegment->setSlackened    ( true );

    ltrace(200) << "Session::slacken() new paral: " << vsegment << endl;
    ltrace(200) << "Session::slacken() perpand:   " << hsegment << endl;
    ltrace(200) << "Session::slacken() original:  " << segment << endl;
  }


} // End of local namespace.


namespace Katabatic {


  using std::min;
  using std::max;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::Error;
  using Hurricane::Bug;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoVertical".


  AutoVertical::AutoVertical ( Vertical* vertical
                             , int       type
                             , bool      terminal
                             , bool      collapsed
                             )
    : AutoSegment(vertical,false,type,terminal,collapsed)
    , _vertical(vertical)
  { }


  void  AutoVertical::_postCreate ()
  {
    AutoSegment::_postCreate ();
    orient       ();
    setPositions ();

    AutoContact* source = getAutoSource();
    if ( source->isTerminal() ) source->setX ( _vertical->getX() );

    AutoContact* target = getAutoTarget();
    if ( target->isTerminal() ) target->setX ( _vertical->getX() );

    if ( source->getGCell() == target->getGCell() ) {
      setGlobal ( false );
    } else {
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
    ltrace(200) << "AutoVertical::_preDestroy() - <id:" << getId() << "> " << (void*)this << endl;
    ltrace(200) << "  " << _getString() << endl;
    ltracein(90);

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
    ltraceout(90);
  }


  AutoVertical::~AutoVertical ()
  {
    if ( Session::doDestroyBaseSegment() and not Session::doDestroyTool() ) {
      ltrace(200) << "~AutoVertical() - " << (void*)_vertical << endl;
      _vertical->destroy ();
    }
  }


  AutoVertical* AutoVertical::create ( Vertical* vertical
                                     , int       type
                                     , bool      terminal
                                     , bool      collapsed
                                     )
  {
    AutoSegment::_preCreate ( vertical->getSource(), vertical->getTarget() );
    AutoVertical* autoVertical = new AutoVertical ( vertical, type, terminal, collapsed );

    autoVertical->_postCreate ();
    return autoVertical;
  }


  AutoVertical* AutoVertical::create ( AutoContact* source
                                     , AutoContact* target
                                     , const Layer* layer
                                     , DbU::Unit    x
                                     , DbU::Unit    width
                                     , int          type
                                     , bool         terminal
                                     , bool         collapsed
                                     )
  {
    AutoSegment::_preCreate ( source, target );
    AutoVertical* autoVertical
      = new AutoVertical ( Vertical::create ( source->getContact()
                                            , target->getContact()
                                            , layer
                                            , x
                                            , width ), type, terminal, collapsed );

    autoVertical->_postCreate ();
    return autoVertical;
  }


  Interval  AutoVertical::getSourceConstraints ( bool native ) const
  {
    if ( native ) {
      Box nativeBox ( getAutoSource()->getNativeConstraintBox() );
      return Interval ( nativeBox.getXMin(), nativeBox.getXMax() );
    }
    return Interval ( getAutoSource()->getCBXMin(), getAutoSource()->getCBXMax() );
  }


  Interval  AutoVertical::getTargetConstraints ( bool native ) const
  {
    if ( native ) {
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

    contact = getAutoTarget();
    constraintMin = max ( constraintMin, contact->getCBXMin() );
    constraintMax = min ( constraintMax, contact->getCBXMax() );

    constraintMin = max ( constraintMin, getUserConstraints().getVMin() );
    constraintMax = min ( constraintMax, getUserConstraints().getVMax() );
    return true;
  }


  unsigned int  AutoVertical::getDirection () const
  { return Constant::Vertical; }


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
    Interval sourceConstraints = Interval(getAutoSource()->getCBXMin(),getAutoSource()->getCBXMax());
    Interval targetConstraints = Interval(getAutoTarget()->getCBXMin(),getAutoTarget()->getCBXMax());

  // Ugly: should uses topRightShrink from GCell.
    sourceConstraints.inflate ( 0, /*Katabatic::GCell::getTopRightShrink()*/ DbU::lambda(1.0) );
    targetConstraints.inflate ( 0, /*Katabatic::GCell::getTopRightShrink()*/ DbU::lambda(1.0) );

  // Ugly: GCell's track number is hardwired.
    if ( sourceConstraints.getSize() / DbU::lambda(5.0) < 10 ) return true;
    if ( targetConstraints.getSize() / DbU::lambda(5.0) < 10 ) return true;

    return false;
  }


  void  AutoVertical::_slacken ()
  {
    AutoContact* contact     = getAutoSource();
    Interval     constraints = Interval(contact->getCBXMin(),contact->getCBXMax());

    constraints.inflate ( 0, /*Katabatic::GCell::getTopRightShrink()*/ DbU::lambda(1.0) );

  // Ugly: GCell's track number is hardwired.
    if ( constraints.getSize() / DbU::lambda(5.0) < 10 ) ::slacken ( this, true );

    contact     = getAutoTarget ();
    constraints = Interval(contact->getCBXMin(),contact->getCBXMax());
    constraints.inflate ( 0, /*Katabatic::GCell::getTopRightShrink()*/ DbU::lambda(1.0) );

    if ( constraints.getSize() / DbU::lambda(5.0) < 10 ) ::slacken ( this, false );

    setSlackened ( true );
  }


  bool  AutoVertical::canDesalignate ( AutoContact* contact ) const
  { return contact->canVDesalignate (); }


  void  AutoVertical::desalignate ( AutoContact* contact )
  { contact->vDesalignate(); }


  void  AutoVertical::alignate ( DbU::Unit axis )
  {
    if ( _vertical->getX() == axis ) return;

    ltrace(159) << "alignate() " << this << " @X " << DbU::getLambda(axis) << endl;

    _vertical->setX ( axis );
    invalidate ();

    AutoContact* anchor = getAutoSource();
    anchor->invalidate ();
    if ( anchor->isTerminal() ) anchor->setX ( axis );

    anchor = getAutoTarget();
    anchor->invalidate ();
    if ( anchor->isTerminal() ) anchor->setX ( axis );
  }


  void  AutoVertical::orient ()
  {
    if ( _vertical->getTargetY() < _vertical->getSourceY() )
      _vertical->invert ();
  }


  void  AutoVertical::setPositions ()
  {
    _sourcePosition = _vertical->getSourceY() - Session::getExtensionCap();
    _targetPosition = _vertical->getTargetY() + Session::getExtensionCap();
  }


  bool  AutoVertical::checkPositions () const
  {
    bool      coherency      = true;
    DbU::Unit sourcePosition = _vertical->getSourceY() - Session::getExtensionCap();
    DbU::Unit targetPosition = _vertical->getTargetY() + Session::getExtensionCap();

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

    if ( !sourceConstraints.intersect(targetConstraints) ) {
      cerr << Error ( "%p:%s\n        Constraints incoherency: S:%p:%s, T:%p:%s"
                    , (void*)base()
                    , _getString().c_str() 
                    , getAutoSource()->getContact()
                    , getString(sourceConstraints).c_str()
                    , getAutoSource()->getContact()
                    , getString(targetConstraints).c_str()
                    ) << endl;
      return false;
    }

    return true;
  }


  void  AutoVertical::_computeTerminal ()
  { _computeTerminal(_vertical); }


  void  AutoVertical::moveULeft ()
  {
    if ( not getAutoSource()->isCorner() or not getAutoTarget()->isCorner() ) return;
    if ( not getAutoSource()->getGCell()->getLeft() ) return;

    Session::setInvalidateMask ( Session::RestoreVCon|Session::NetCanonize );
    Session::invalidate ( getNet() );

    AutoContact* autoSource = getAutoSource();
    AutoContact* autoTarget = getAutoTarget();
    GCell*       begin      = autoSource->getGCell();
    GCell*       end        = autoTarget->getGCell();

    forEach ( Horizontal*, isegment, autoSource->getSlaveComponents().getSubSet<Horizontal*>() ) {
      AutoSegment* segment = Session::lookup ( *isegment );
      if ( segment->isLocal() ) {
        segment->setGlobal ( true );
        continue;
      }

      if ( segment->getAutoSource() == autoSource ) {
        begin->addHSegment ( segment );
      } else {
        if ( begin->getLeft() == segment->getAutoSource()->getGCell() ) {
          segment->setGlobal ( false );
          segment->getAutoSource()->invalidate ();
        } else
          begin->getLeft()->removeHSegment ( segment );
      }
    }

    forEach ( Horizontal*, isegment, autoTarget->getSlaveComponents().getSubSet<Horizontal*>() ) {
      AutoSegment* segment = Session::lookup ( *isegment );
      if ( segment->isLocal() ) {
        segment->setGlobal ( true );
        continue;
      }

      if ( segment->getAutoSource() == autoTarget ) {
        begin->addHSegment ( segment );
      } else {
        if ( end->getLeft() == segment->getAutoSource()->getGCell() ) {
          segment->setGlobal ( false );
          segment->getAutoSource()->invalidate ();
        } else
          end->getLeft()->removeHSegment ( segment );
      }
    }

    if ( begin != end ) {
      for ( GCell* gcell = begin->getUp() ; gcell != end ; gcell = gcell->getUp() )
        gcell->removeVSegment ( this );
    }

    begin = begin->getLeft();
    end   = end  ->getLeft();

    autoSource->setGCell ( begin );
    autoTarget->setGCell ( end   );
    if ( begin != end ) {
      for ( GCell* gcell = begin->getUp() ; gcell != end ; gcell = gcell->getUp() )
        gcell->addVSegment ( this );
    }

    DbU::Unit x = begin->getUSide(Constant::Horizontal).getVMax();
    setAxis ( x, AxisSet);

    Session::revalidateTopology ();
  }


  void  AutoVertical::moveURight ()
  {
    ltrace(200) << "AutoVertical::moveURight()" << endl;

    if ( not getAutoSource()->isCorner() or not getAutoTarget()->isCorner() ) return;
    if ( not getAutoSource()->getGCell()->getRight() ) return;

    Session::setInvalidateMask ( Session::RestoreVCon|Session::NetCanonize );
    Session::invalidate ( getNet() );

    AutoContact* autoSource = getAutoSource();
    AutoContact* autoTarget = getAutoTarget();
    GCell*       begin      = autoSource->getGCell();
    GCell*       end        = autoTarget->getGCell();

    forEach ( Horizontal*, isegment, autoSource->getSlaveComponents().getSubSet<Horizontal*>() ) {
      AutoSegment* segment = Session::lookup ( *isegment );
      if ( segment->isLocal() ) {
        segment->setGlobal ( true );
        continue;
      }

      if ( segment->getAutoTarget() == autoSource ) {
        begin->addHSegment ( segment );
      } else {
        if ( begin->getRight() == segment->getAutoTarget()->getGCell() ) {
          segment->setGlobal ( false );
          segment->getAutoTarget()->invalidate ();
        } else
          begin->getRight()->removeHSegment ( segment );
      }
    }

    forEach ( Horizontal*, isegment, autoTarget->getSlaveComponents().getSubSet<Horizontal*>() ) {
      AutoSegment* segment = Session::lookup ( *isegment );
      if ( segment->isLocal() ) {
        segment->setGlobal ( true );
        continue;
      }

      if ( segment->getAutoTarget() == autoTarget ) {
        begin->addHSegment ( segment );
      } else {
        if ( end->getRight() == segment->getAutoTarget()->getGCell() ) {
          segment->setGlobal ( false );
          segment->getAutoTarget()->invalidate ();
        } else
          end->getRight()->removeHSegment ( segment );
      }
    }

    if ( begin != end ) {
      for ( GCell* gcell = begin->getUp() ; gcell != end ; gcell = gcell->getUp() )
        gcell->removeVSegment ( this );
    }

    begin = begin->getRight();
    end   = end  ->getRight();
    autoSource->setGCell ( begin );
    autoTarget->setGCell ( end   );
    if ( begin != end ) {
      for ( GCell* gcell = begin->getUp() ; gcell != end ; gcell = gcell->getUp() )
        gcell->addVSegment ( this );
    }

    DbU::Unit x = begin->getUSide(Constant::Horizontal).getVMin();
    setAxis ( x, AxisSet );

    ltrace(200) << "Moved to axis: " << DbU::getValueString(x) << endl;

    Session::revalidateTopology ();
  }


  void  AutoVertical::_makeDogLeg ( GCell* dogLegGCell, bool upLayer )
  {
    AutoContact*  autoSource      = getAutoSource();
    AutoContact*  autoTarget      = getAutoTarget();
    GCell*        begin           = autoSource->getGCell();
    GCell*        end             = autoTarget->getGCell();
    unsigned int  fragmentType    = AutoSegment::Global;
    unsigned int  splittedType    = AutoSegment::Global;

    Session::dogLegReset ();

  //DbU::Unit dogLegAxis = (detachSource)?dogLegGCell->getYMax():dogLegGCell->getY();
    DbU::Unit dogLegAxis = (dogLegGCell->getYMax() + dogLegGCell->getY()) / 2;
    if ( isLocal() )
      dogLegAxis = (getSourceY() + getTargetY()) / 2;

    ltrace(159) << "Detaching Target AutoContact." << endl;
    if ( end == dogLegGCell ) {
      fragmentType = AutoSegment::Local;
    }
    if ( begin == dogLegGCell ) {
      splittedType = AutoSegment::Local;
      setGlobal ( false );
    }

    autoTarget->invalidate ();
    autoTarget->setInvalidatedTopology ( true );

    if ( dogLegGCell != end ) {
      GCell* gcell = dogLegGCell;
      do {
        if ( gcell != begin )
          gcell->removeVSegment ( this );
        gcell = gcell->getUp ();
      } while ( gcell && (gcell != end) );
    }

    Session::dogLeg ( this );

    size_t       depth        = Session::getRoutingGauge()->getLayerDepth ( _vertical->getLayer() );
    Layer*       contactLayer = Session::getRoutingGauge()->getContactLayer ( depth + ((upLayer)?0:-1) );
    const Layer* dogLegLayer  = Session::getRoutingGauge()->getRoutingLayer ( depth + ((upLayer)?1:-1) );

    _vertical->getTargetHook()->detach ();
    AutoContact* dlContact1 = AutoContact::create ( dogLegGCell, _vertical->getNet(), contactLayer );
    AutoContact* dlContact2 = AutoContact::create ( dogLegGCell, _vertical->getNet(), contactLayer );
    AutoSegment* segment1   = AutoHorizontal::create ( dlContact1
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
    ltrace(200) << "Session::dogLeg() perpand:   " << segment1 << endl;
    Session::dogLeg ( segment1 );

    _vertical->getTargetHook()->attach ( dlContact1->getContact()->getBodyHook() );
    AutoSegment* segment2 = AutoVertical::create ( dlContact2
                                                 , autoTarget
                                                 , getLayer()
                                                 , getX()
                                                 , DbU::lambda(2.0)
                                                 , fragmentType
                                                 , false
                                                 , false
                                                 );
    segment2->setAxis ( getX(), AxisSet );
    ltrace(200) << "Session::dogLeg() new paral: " << segment2 << endl;
    ltrace(200) << "Session::dogLeg() original:  " << this << endl;
    Session::dogLeg ( segment2 );

    setGlobal ( (splittedType == AutoSegment::Global) );

    if ( (splittedType == AutoSegment::Global) or ( fragmentType == AutoSegment::Global ) ) {
      if ( splittedType == AutoSegment::Local ) autoSource->restoreHConnexity ( getX(), true );
      else                                      autoTarget->restoreHConnexity ( getX(), true );
    }

    setTerminal ( false );
    segment2->setTerminal ( false );
    if ( autoSource->getAnchor() and not isGlobal() ) setTerminal ( true );
    if ( autoTarget->getAnchor() and not segment2->isGlobal() ) segment2->setTerminal ( true );

    segment1->setSlackened ( true );
    segment2->setSlackened ( isSlackened() );

    Session::invalidate         ( getNet() );
    Session::revalidateTopology ();
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
