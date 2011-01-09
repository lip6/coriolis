
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
// |  C++ Module  :       "./AutoSegment.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/Warning.h"
#include  "hurricane/Bug.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "crlcore/RoutingGauge.h"

#include  "katabatic/Session.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoSegment.h"
#include  "katabatic/AutoHorizontal.h"
#include  "katabatic/AutoVertical.h"
#include  "katabatic/GCell.h"
#include  "katabatic/KatabaticEngine.h"


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
          if ( *icomponent == autoSegment->getSegment() ) continue;

          Segment* connex = dynamic_cast<Segment*>(*icomponent);
          if ( !connex ) continue;

          segment = Session::lookup ( connex );
          if ( !segment || !segment->isTerminal() ) continue;

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

    ltrace(88) << "add Attractor @" << DbU::getLambda(position)
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


  DbU::Unit  AutoSegment::getX () const
  {
    return getSegment()->getX();
  }


  DbU::Unit  AutoSegment::getY () const
  {
    return getSegment()->getY();
  }


  AutoContact* AutoSegment::getOppositeAnchor ( AutoContact* anchor ) const
  {
    return Session::lookup(static_cast<Contact*>(getOppositeAnchor(anchor->getContact())));
  }


  Interval& AutoSegment::getOptimal ( Interval& i ) const
  {
    i.getVMin() = getOptimalMin();
    i.getVMax() = getOptimalMax();
    return i;
  }


  bool  AutoSegment::checkInvalidated () const
  {
    if ( isInvalidated() )
      cerr << Error("%s is invalidated.",getString(this).c_str()) << endl;

    return !isInvalidated();
  }


  void  AutoSegment::invalidate ()
  {
    if ( Session::doDestroyTool() ) return;

    _invalidate ();
    forEach ( AutoSegment*, isegment, getCollapseds() )
      isegment->_invalidate ();
  }


  void  AutoSegment::_invalidate ()
  {
    if ( !isInvalidated() ) {
      ltrace(110) << "AutoSegment::_invalidate() " << this << endl;

      setInvalidated ( true );
      Session::invalidate ( this );
    }
  }


  void  AutoSegment::revalidate ()
  {
    ltrace(110) << "AutoSegment::revalidate() " << this << endl;
    ltracein(110);

    setPositions ();
    setInvalidated ( false );

    ltraceout(110);
  }


  DbU::Unit  AutoSegment::getSlack () const
  {
    DbU::Unit  constraintMin;
    DbU::Unit  constraintMax;

    getConstraints ( constraintMin, constraintMax );

    return constraintMax - constraintMin;
  }


  DbU::Unit  AutoSegment::getCost ( DbU::Unit axis ) const
  {
    DbU::Unit  optimal = getOptimalMin();
    if ( axis < optimal )
      return optimal - axis;

    optimal = getOptimalMax();
    if ( axis > optimal )
      return axis - optimal;

    return 0;
  }


  AutoSegment* AutoSegment::getCanonical ( DbU::Unit& min, DbU::Unit& max )
  {
    min = getSourcePosition ();
    max = getTargetPosition ();

    if ( max < min ) swap ( min, max );

    AutoSegment* canonical    = this;
    size_t       canonicals   = isCanonical();
    size_t       aligneds     = 1;
    DbU::Unit    collapsedMin;
    DbU::Unit    collapsedMax;

    forEach ( AutoSegment*, isegment, getCollapseds() ) {
      if ( isegment->isCanonical() ) {
        canonical = *isegment;
        canonicals++;
      }

      collapsedMin = isegment->getSourcePosition();
      collapsedMax = isegment->getTargetPosition();
      if ( collapsedMax < collapsedMin ) swap ( collapsedMin, collapsedMax );
      if ( collapsedMin < min ) min = collapsedMin;
      if ( collapsedMax > max ) max = collapsedMax;

      aligneds++;
    }

    if ( (canonicals > 1) || ( !canonicals && (aligneds > 2) ) ) {
      cerr << Bug("AutoSegment::getCanonical(): %p:%s"
                  "\n      Bad canonization: %d canonicals out of %d collapseds."
                 , base(), _getString().c_str(), canonicals, aligneds ) << endl;

      int count = 0;
      cerr << "      " << count++ << ": " << this << endl;
      forEach ( AutoSegment*, isegment, getCollapseds() )
        cerr << "      " << count++ << ": " << *isegment << endl;
    }

    return canonical;
  }


  AutoSegments  AutoSegment::getOnSourceContact ( unsigned int direction )
  {
    return AutoSegments_OnContact
      ( this, getSource() ).getSubSet ( AutoSegments_InDirection(direction) );
  }


  AutoSegments  AutoSegment::getOnTargetContact ( unsigned int direction )
  {
    return AutoSegments_OnContact
      ( this, getTarget() ).getSubSet ( AutoSegments_InDirection(direction) );
  }


  AutoSegments  AutoSegment::getCollapseds ( bool withPerpand )
  {
    return AutoSegments_Collapsed ( this, withPerpand );
  }


  AutoSegments  AutoSegment::getCollapsedPerpandiculars ()
  {
    return AutoSegments_CollapsedPerpandicular ( this );
  }


  bool  AutoSegment::isCanonicalStrap () const
  {
    if ( not isStrap() ) return false;
    forEach ( AutoSegment*, isegment, const_cast<AutoSegment*>(this)->getCollapseds() ) {
      if ( not isegment->isStrap() ) return false;
    }
    return true;
  }


  bool  AutoSegment::collapse ()
  {
    if ( _isGlobal ) {
      cerr << Error("Global %s cannot be collapsed.",getString(this).c_str()) << endl;
      return false;
    }
    if ( _isCollapsed ) return true;

    _isCollapsed = true;

    unsigned int direction = (_isHorizontal) ? Constant::Vertical : Constant::Horizontal;
    forEach ( AutoSegment*, isegment, AutoSegments_AnchoredBySource(getAutoSource(),direction) ) {
      isegment->setCanonical ( false );
    }
    forEach ( AutoSegment*, isegment, AutoSegments_AnchoredBySource(getAutoTarget(),direction) ) {
      isegment->setCanonical ( false );
    }

    return true;
  }


  bool  AutoSegment::expand ()
  {
    if ( _isGlobal ) {
      cerr << Warning("Global %s already uncollapsed.",getString(this).c_str()) << endl;
      return false;
    }
    if ( !_isCollapsed ) {
      cerr << Warning("Local %s already uncollapsed.",getString(this).c_str()) << endl;
      return true;
    }

    _isCollapsed = false;

    canonize ();

    unsigned int direction = (_isHorizontal) ? Constant::Vertical : Constant::Horizontal;
    forEach ( AutoSegment*, segment, getOnSourceContact(direction) ) {
      segment->canonize ();
    }
    forEach ( AutoSegment*, segment, getOnTargetContact(direction) ) {
      segment->canonize ();
    }

    return true;
  }


  bool  AutoSegment::toConstraintAxis ( set<AutoSegment*>* processeds )
  {
    if ( processeds && (processeds->find(this) != processeds->end()) ) return false;

    DbU::Unit constraintMin;
    DbU::Unit constraintMax;

    getConstraints ( constraintMin, constraintMax );

  // Empty constraint interval: ignore.
    if ( constraintMin > constraintMax ) return false;

    if ( allowOutsideGCell() ) {
    // Ugly: hard-wired value of the track spacing.
      constraintMin -= DbU::lambda(5.0) * 8;
      constraintMax += DbU::lambda(5.0) * 8;
    }

    if ( getAxis() < constraintMin ) {
      setAxis ( constraintMin, Realignate, processeds );
      return true;
    }

    if ( getAxis() > constraintMax ) {
      setAxis ( constraintMax, Realignate, processeds );
      return true;
    }

    return false;
  }


  bool  AutoSegment::toOptimalAxis ( set<AutoSegment*>* processeds )
  {
    if ( processeds && (processeds->find(this) != processeds->end()) ) return false;

    DbU::Unit constraintMin;
    DbU::Unit constraintMax;

    getConstraints ( constraintMin, constraintMax );

    DbU::Unit optimalMin = max ( min(getOptimalMin(),constraintMax), constraintMin );
    DbU::Unit optimalMax = min ( max(getOptimalMax(),constraintMin), constraintMax );

    if ( getAxis() < optimalMin ) {
#if defined(CHECK_DETERMINISM)
      cerr << "Order: toOptimalMin "
           << DbU::getValueString(optimalMin) << " ["
           << DbU::getValueString(optimalMin) << ":"
           << DbU::getValueString(optimalMax) << "] ["
           << DbU::getValueString(constraintMin) << ":"
           << DbU::getValueString(constraintMax) << "] "
           << this << endl;
#endif
      setAxis ( optimalMin, Realignate|AxisSet, processeds );
      return true;
    }

    if ( getAxis() > optimalMax ) {
#if defined(CHECK_DETERMINISM)
      cerr << "Order: toOptimalMax "
           << DbU::getValueString(optimalMin) << " ["
           << DbU::getValueString(optimalMin) << ":"
           << DbU::getValueString(optimalMax) << "] ["
           << DbU::getValueString(constraintMin) << ":"
           << DbU::getValueString(constraintMax) << "] "
           << this << endl;
#endif
      setAxis ( optimalMax, Realignate|AxisSet, processeds );
      return true;
    }

#if defined(CHECK_DETERMINISM)
    cerr << "Order: in optimal position  "
         << DbU::getValueString(optimalMin) << " ["
         << DbU::getValueString(optimalMin) << ":"
         << DbU::getValueString(optimalMax) << "] ["
         << DbU::getValueString(constraintMin) << ":"
         << DbU::getValueString(constraintMax) << "] "
         << this << endl;
#endif

    return false;
  }


  void  AutoSegment::setAxis ( DbU::Unit axis, unsigned int flags, set<AutoSegment*>* processeds )
  {
    if ( processeds and (processeds->find(this) != processeds->end()) ) return;

    if ( ( axis != getAxis() ) and isFixed() ) {
      cerr << Error("AutoSegment::setAxis(): Cannot move fixed segment to %s.\n"
                    "        (on: %s)"
                   ,DbU::getValueString(axis).c_str()
                   ,_getString().c_str()
                   ) << endl;
    }

    if ( _isUnsetAxis and (flags & AxisSet) ) {
      ltrace(200) << "setAxis() - AxisSet flag raised " << this << endl;
      _isUnsetAxis = false;
    }
    if ( ( axis == getAxis() ) and not (flags & Realignate) ) return;

    ltrace(200) << "setAxis() @"
                << ((_isHorizontal)?"Y ":"X ") << DbU::getLambda(getAxis())
                << " to " << DbU::getLambda(axis) << " on " << this << endl;
    ltracein(80);

    alignate ( axis );
    if ( processeds ) processeds->insert ( this );

    forEach ( AutoSegment*, isegment, getCollapseds() ) {
      isegment->alignate ( getAxis() );
      if ( flags & AxisSet ) isegment->_isUnsetAxis = false;
      if ( processeds ) processeds->insert ( *isegment );
    }

    ltraceout(80);
  }


  void  AutoSegment::computeOptimal ( set<AutoSegment*>* processeds )
  {
    ltrace(89) << "computeOptimal() - " << this << endl;
    ltracein(89);

    if ( processeds && (processeds->find(this) != processeds->end()) ) { ltraceout(89); return; }

    if ( _isCollapsed ) {
      _optimalMin = 0;
      setOptimalMax ( (_isHorizontal) ? _gcell->getBoundingBox().getYMax()
                    : _gcell->getBoundingBox().getXMax() );
      ltraceout(89);
      return;
    }

    DbU::Unit      minGCell    = getOrigin();
    DbU::Unit      maxGCell    = getExtremity();
    DbU::Unit      terminalMin;
    DbU::Unit      terminalMax;
    AttractorsMap  attractors;

    AutoContact* anchor = getAutoSource();
    if ( anchor->isTerminal() ) {
      Box constraintBox = anchor->getConstraintBox();
      if ( isHorizontal() ) {
        terminalMin = constraintBox.getYMin ();
        terminalMax = constraintBox.getYMax ();
      } else {
        terminalMin = constraintBox.getXMin ();
        terminalMax = constraintBox.getXMax ();
      }

      attractors.addAttractor ( terminalMin );
      if ( terminalMin != terminalMax )
        attractors.addAttractor ( terminalMax );
    }

    anchor = getAutoTarget();
    if ( anchor->isTerminal() ) {
      Box constraintBox = anchor->getConstraintBox();
      if ( isHorizontal() ) {
        terminalMin = constraintBox.getYMin ();
        terminalMax = constraintBox.getYMax ();
      } else {
        terminalMin = constraintBox.getXMin ();
        terminalMax = constraintBox.getXMax ();
      }

      attractors.addAttractor ( terminalMin );
      if ( terminalMin != terminalMax )
        attractors.addAttractor ( terminalMax );
    }

    forEach ( AutoSegment*, autoSegment, getCollapsedPerpandiculars() ) {
      ltrace(89) << "Perpandicular " << *autoSegment << endl;
      ltracein(89);
      if ( autoSegment->isLocal() ) {
        if ( !autoSegment->isTerminal() ) { ltraceout(89); continue; }

        DbU::Unit  terminalMin;
        DbU::Unit  terminalMax;

        if ( getTerminalInterval ( *autoSegment
                                 , NULL
                                 , isHorizontal()
                                 , terminalMin
                                 , terminalMax ) ) {
          attractors.addAttractor ( terminalMin );
          if ( terminalMin != terminalMax )
            attractors.addAttractor ( terminalMax );
        }
      } else {
        bool isMin = true;
        if (      isHorizontal()
             && ( autoSegment->getAutoSource()->getGCell()->getRow() == _gcell->getRow() ) )
          isMin = false;
        if (      isVertical()
             && ( autoSegment->getAutoSource()->getGCell()->getColumn() == _gcell->getColumn() ) )
          isMin = false;
        attractors.addAttractor ( (isMin) ? minGCell : maxGCell );
      }
      ltraceout(89);
    }

    DbU::Unit  optimalMin;
    DbU::Unit  optimalMax;
    DbU::Unit  constraintMin;
    DbU::Unit  constraintMax;
    getConstraints ( constraintMin, constraintMax );

    if ( attractors.getAttractorsCount() ) {
      ltrace(89) << "Lower Median " << DbU::getLambda(attractors.getLowerMedian()) << endl;
      ltrace(89) << "Upper Median " << DbU::getLambda(attractors.getUpperMedian()) << endl;

      optimalMin = attractors.getLowerMedian();
      optimalMax = attractors.getUpperMedian();
    } else {
      optimalMin = 0;
      optimalMax = (_isHorizontal) ? _gcell->getBoundingBox().getYMax()
                                   : _gcell->getBoundingBox().getXMax();
    }

    setInBound ( constraintMin, constraintMax, optimalMin );
    setInBound ( constraintMin, constraintMax, optimalMax );

    if ( processeds ) processeds->insert ( this );
    setOptimalMin ( optimalMin );
    setOptimalMax ( optimalMax );
    forEach ( AutoSegment*, autoSegment, getCollapseds() ) {
      if ( processeds ) processeds->insert ( *autoSegment );
      autoSegment->setOptimalMin ( optimalMin );
      autoSegment->setOptimalMax ( optimalMax );
    }

    ltraceout(89);
  }


  AutoSegment* AutoSegment::canonize ()
  {
    ltrace(159) << "canonize() - " << this << endl;

    if ( isCanonical() ) {
      ltrace(159) << "* " << this << " canonical" << endl;
      return this;
    }

    AutoSegment* canonical        = this;
    bool         hasCanonical     = false;
    bool         isCanonicalLocal = true;

    forEach ( AutoSegment*, isegment, getCollapseds() ) {
      if ( isegment->isGlobal() ) isCanonicalLocal = false;

      if ( isegment->isCanonical() ) {
        ltrace(159) << "* " << *isegment << " canonical" << endl;
      //return *isegment;
        canonical    = *isegment;
        hasCanonical = true;
        break;
      }

      if ( not hasCanonical ) {
        if ( CompareId()(*isegment,canonical) )
          canonical = *isegment;
      }
    }

    canonical->setCanonical      ( true );
    canonical->setCanonicalLocal ( isCanonicalLocal );

  // ltrace: 159
    if ( isCanonical() ) cerr << "* " << this << " canonical" << endl;
    else                 cerr << "* " << this << endl;

    forEach ( AutoSegment*, isegment, getCollapseds() ) {
      if ( isegment->isCanonical() ) cerr << "| " << *isegment << " canonical" << endl;
      else                           cerr << "| " << *isegment << endl;
    }

    return canonical;
  }


  AutoSegment::AutoSegment ( Segment* segment
                           , bool     isHorizontal
                           , int      type
                           , bool     terminal
                           , bool     collapsed
                           )
    : _isUnsetAxis      (true)
    , _invalidated      (false)
    , _isHorizontal     (isHorizontal)
    , _isTerminal       (terminal)
    , _isCollapsed      (collapsed)
    , _isCanonical      (false)
    , _isFixed          (false)
    , _strap            (false)
    , _layerChange      (false)
    , _slackened        (false)
    , _slackenStrap     (false)
    , _allowOutsideGCell(false)
    , _id               (_maxId++)
    , _optimalMin       (0)
    , _userConstraints  (false)
    , _parent           (NULL)
  {
  //cerr << "AutoSegment::AutoSegment() - <id:" << _id << ">" << endl;
#if defined(CHECK_DETERMINISM)
    cerr << "Order: AutoSegment::AutoSegment() - <id:" << _id << ">" << endl;
#endif
    AutoContact* source = Session::lookup(dynamic_cast<Contact*>(segment->getSource()));
    AutoContact* target = Session::lookup(dynamic_cast<Contact*>(segment->getTarget()));

    _allocateds++;

    _gcell = source->getGCell();
    setOptimalMax ( (_isHorizontal) ? _gcell->getBoundingBox().getYMax()
                  : _gcell->getBoundingBox().getXMax() );

    switch ( type ) {
      case AutoSegment::Global: _isGlobal = true;  break;
      case AutoSegment::Local : _isGlobal = false; break;
      case AutoSegment::Guess :
        _isGlobal = ( source->getGCell() != target->getGCell() );
        break;
    }
    _globalsCount   += (_isGlobal) ? 1 : 0;
    _isCanonicalLocal = not _isGlobal;

    _computeTerminal ( segment );
  //if ( source->isTerminal() or target->isTerminal() ) _isTerminal = true;

  //if (   source->isTerminal()
  //   and target->isTerminal()
  //   /*and (segment->getLength() < DbU::lambda(25.0))*/
  //   and (source->getGCell() == target->getGCell()) ) {
  //  _strap = true;
  //}

    source->setInvalidatedTopology ( true );
  }


  void  AutoSegment::_preCreate ( Component* source, Component* target )
  {
    AutoContact* acSource = Session::lookup(dynamic_cast<Contact*>(source));
    AutoContact* acTarget = Session::lookup(dynamic_cast<Contact*>(target));

    _preCreate ( acSource, acTarget );
  }


  void  AutoSegment::_preCreate ( AutoContact* source, AutoContact* target )
  {
    if ( !source || !target )
      throw Error ( badAutoSegmentAnchor
                  , ((source)?getString(source).c_str():"NULL")
                  , ((target)?getString(target).c_str():"NULL")
                  );

    if ( source == target )
      throw Error ( dupAutoSegmentAnchor, getString(source).c_str() );
  }


  void  AutoSegment::_postCreate ()
  {
    Session::invalidate ( getNet() );
    Session::link ( this );
    invalidate ();
  }


  void  AutoSegment::_preDestroy ()
  {
    ltrace(200) << "AutoSegment::_preDestroy() - " << (void*)this << endl;
    ltracein(90);

    Session::unlink ( this );
    ltraceout(90);
  }


  AutoSegment::~AutoSegment ()
  {
    _allocateds--;
    if ( _isGlobal and (_globalsCount > 0) ) _globalsCount--;
  }


  void  AutoSegment::_computeTerminal ( Segment* segment )
  {
    AutoContact* source = Session::lookup(dynamic_cast<Contact*>(segment->getSource()));
    AutoContact* target = Session::lookup(dynamic_cast<Contact*>(segment->getTarget()));

    _isTerminal = source->isTerminal() or target->isTerminal();
  }


  size_t  AutoSegment::getAlignedContacts ( map<AutoContact*,int>& innerContacts )
  {
    map<AutoContact*,int>::iterator icontact;

    innerContacts.clear  ();
    innerContacts.insert ( make_pair(getAutoSource(),0x1) );
    innerContacts.insert ( make_pair(getAutoTarget(),0x4) );

    forEach ( AutoSegment*, isegment, getCollapseds() ) {
      if ( (icontact = innerContacts.find(isegment->getAutoSource())) != innerContacts.end() ) {
        if ( icontact->second & 0x1 ) icontact->second |= 0x2;
        else                          icontact->second |= 0x1;
      } else
        innerContacts.insert ( make_pair(getAutoSource(),0x1) );

      if ( (icontact = innerContacts.find(isegment->getAutoTarget())) != innerContacts.end() ) {
        if ( icontact->second & 0x4 ) icontact->second |= 0x8;
        else                          icontact->second |= 0x4;
      } else
        innerContacts.insert ( make_pair(getAutoTarget(),0x4) );
    }

    return innerContacts.size();
  }


  size_t  AutoSegment::getPerpandicularsBound ( set<AutoSegment*>& bounds )
  {
    map<AutoContact*,int>           contacts;
    map<AutoContact*,int>::iterator icontact;

    getAlignedContacts ( contacts );

    for ( icontact=contacts.begin() ; icontact != contacts.end() ; icontact++ ) {
      if ( (icontact->second == 0x1) or (icontact->second == 0x4) ) {
        forEach ( Segment*, isegment, icontact->first->getSlaveComponents().getSubSet<Segment*>() ) {
          AutoSegment* autoSegment = Session::lookup ( *isegment );
          if ( !autoSegment ) continue;
          if ( autoSegment->getDirection() == getDirection() ) continue;

          bounds.insert ( autoSegment );
        }
      }
    }

    return bounds.size();
  }


  Interval  AutoSegment::getMinSpanU ()
  {
    map<AutoContact*,int>           contacts;
    map<AutoContact*,int>::iterator icontact;

    getAlignedContacts ( contacts );

    DbU::Unit    spanMin   = DbU::Min;
    DbU::Unit    spanMax   = DbU::Max;
    Interval     constraints;
    unsigned int direction = Constant::perpandicular(getDirection());

    for ( icontact=contacts.begin() ; icontact != contacts.end() ; icontact++ ) {
      constraints = icontact->first->getUConstraints ( direction );
      if ( icontact->second == 0x1 ) {
        spanMin = max ( spanMin, constraints.getVMax() );
      }
      if ( icontact->second == 0x4 ) {
        spanMax = min ( spanMax, constraints.getVMin() );
      }
    }

    return Interval(spanMin,spanMax);
  }


  void  AutoSegment::setAllowOutsideGCell ( bool state, bool propagate )
  {
    if ( allowOutsideGCell() ) return;

    _setAllowOutsideGCell ( state );

    if ( propagate ) {
      forEach ( AutoSegment*, isegment, getCollapseds() ) {
        isegment->_setAllowOutsideGCell ( state );
      }
    }
  }


  void  AutoSegment::_setAllowOutsideGCell ( bool state )
  {
    ltrace(200) << "_setAllowOutsideGCell() - " << this << endl;
    _allowOutsideGCell = state;
  }


  bool  AutoSegment::canGoOutsideGCell () const
  {
    ltracein(200);

    bool goOutsideGCell = getAutoSource()->canGoOutsideGCell(this);
    goOutsideGCell = goOutsideGCell and getAutoTarget()->canGoOutsideGCell(this);

    if ( !goOutsideGCell ) {
      ltraceout(200);
      return false;
    }

    GCell*    sourceGCell = getAutoSource()->getGCell();
    GCell*    leftGCell   = NULL;
    GCell*    rightGCell  = NULL;
    Interval  uside;
    bool      goLeft      = false;
    bool      goRight     = false;

    if ( isHorizontal() ) {
      uside      = sourceGCell->getUSide ( Constant::Vertical );
      leftGCell  = sourceGCell->getDown();
      rightGCell = sourceGCell->getUp  ();
    } else {
      uside      = sourceGCell->getUSide ( Constant::Horizontal );
      leftGCell  = sourceGCell->getLeft ();
      rightGCell = sourceGCell->getRight();
    }

    DbU::Unit constraintMin;
    DbU::Unit constraintMax;
    getConstraints ( constraintMin, constraintMax );

    if ( leftGCell && (uside.getVMin() >= constraintMin) ) {
      ltrace(200) << "Can go Left." << endl;
      goLeft = true;
    }
  // Ugly: Must use the right compensator for VMax.
    if ( rightGCell && (uside.getVMax() <= constraintMax)+DbU::lambda(1.0) ) {
      ltrace(200) << "Can go Right." << endl;
      goRight = true;
    }

    goOutsideGCell = goOutsideGCell and (goRight or goLeft);

  // Override.
  //goOutsideGCell = !isGlobal() && !isTerminal();

    ltrace(200) << "AutoSegment::canGoOutsideGCell() - " << goOutsideGCell << endl;
    ltraceout(200);

    return goOutsideGCell;
  }


  bool  AutoSegment::canDesalignate ()
  {
    ltrace(200) << "AutoSegment::canDesalignate()" << endl;

    map<AutoContact*,int>           innerContacts;
    map<AutoContact*,int>::iterator icontact;

    getAlignedContacts ( innerContacts );

    for ( icontact=innerContacts.begin() ; icontact != innerContacts.end() ; icontact++ ) {
      ltrace(200) << "| " << "flags:" << icontact->second
                  << " " << (void*)icontact->first->base() << ":" << icontact->first << endl;
      if ( (icontact->second & 0x5 ) && canDesalignate(icontact->first) ) return true;
    //if ( (icontact->second & 0x3 ) && canDesalignate(icontact->first) ) return true;
    //if ( (icontact->second & 0x12) && canDesalignate(icontact->first) ) return true;
    }
    ltrace(200) << "No AutoContact suitable for desalignment." << endl;

    return false;
  }


  void  AutoSegment::desalignate ()
  {
    map<AutoContact*,int>           innerContacts;
    map<AutoContact*,int>::iterator icontact;
    vector<AutoSegment*>            segments;

  // Ugly. Must fusion with the inner contact loop.
    forEach ( AutoSegment*, isegment, getCollapseds() ) {
      segments.push_back ( *isegment );
    }

    invalidate ();
    getAlignedContacts ( innerContacts );

    for ( icontact=innerContacts.begin() ; icontact != innerContacts.end() ; icontact++ ) {
      desalignate ( icontact->first );
    //if ( icontact->second & 0x3 ) desalignate ( icontact->first );
    //if ( icontact->second & 0x3 ) desalignate ( icontact->first );
    //if ( icontact->second & 0x12) desalignate ( icontact->first );
    }

    // for ( size_t i=0 ; i<segments.size() ; ++i ) {
    //   segments[i]->_computeTerminal ( segments[i]->base() );
    //   ltrace(99) << "_computeTerminals() - " << segments[i] << endl;
    // }

    Session::invalidate         ( getNet() );
    Session::revalidateTopology ();
  }


  void  AutoSegment::changeDepth ( unsigned int depth, bool propagate, bool standAlone )
  {
    invalidate ();
    Session::invalidate        ( getNet() );
    Session::setInvalidateMask ( Session::NetSplitContacts );

    _changeDepth ( depth, true );

    if ( propagate ) {
      forEach ( AutoSegment*, isegment, getCollapseds() ) {
        isegment->_changeDepth ( depth, true );
      }
    }

    if ( standAlone ) Session::revalidateTopology();
  }


  void  AutoSegment::_changeDepth ( unsigned int depth, bool withNeighbors )
  {
    ltrace(200) << "_changeDepth() - " << this << endl;
    ltracein(200);

    const Layer* layer0 = Session::getRoutingGauge()->getRoutingLayer(depth);
    if ( getLayer() != layer0 ) {
      setLayer ( layer0 );

      getAutoSource()->invalidate();
      getAutoTarget()->invalidate();
    }

    if ( !withNeighbors ) {
      ltraceout(200);
      return;
    }

    forEach ( Component*, icomponent, getAutoSource()->getSlaveComponents() ) {
      if ( *icomponent == base() ) continue;

      Segment* segment = dynamic_cast<Segment*>(*icomponent);
      if ( !segment ) continue;

      AutoSegment* autoSegment = Session::lookup ( segment );
      if ( !autoSegment ) continue;
      if ( autoSegment->isGlobal  () ) continue;
      if ( autoSegment->isTerminal() ) continue;

      if ( !( autoSegment->isHorizontal() xor isHorizontal() ) ) {
        autoSegment->_changeDepth ( depth, false );
      } else {
        autoSegment->_changeDepth ( depth-1, false );
      }
    }

    forEach ( Component*, icomponent, getAutoTarget()->getSlaveComponents() ) {
      if ( *icomponent == base() ) continue;

      Segment* segment = dynamic_cast<Segment*>(*icomponent);
      if ( !segment ) continue;

      AutoSegment* autoSegment = Session::lookup ( segment );
      if ( !autoSegment ) continue;
      if ( autoSegment->isGlobal  () ) continue;
      if ( autoSegment->isTerminal() ) continue;

      if ( !( autoSegment->isHorizontal() xor isHorizontal() ) ) {
        autoSegment->_changeDepth ( depth, false );
      } else {
        autoSegment->_changeDepth ( depth-1, false );
      }
    }

    vector<GCell*> gcells;
    getGCells ( gcells );
    for ( size_t i=0 ; i<gcells.size() ; i++ ) {
      gcells[i]->invalidate ();
    }

    ltraceout(200);
  }


  bool  AutoSegment::canSlacken ( bool propagate )
  {
    ltrace(200) << "AutoSegment::canSlacken()" << endl;

    if ( !isGlobal() && !propagate ) return false;

    if ( _canSlacken() ) return true;
    if ( propagate ) {
      forEach ( AutoSegment*, isegment, getCollapseds() ) {
        if ( isegment->_canSlacken() ) return true;
      }
    }

    return false;
  }


  void  AutoSegment::slacken ( bool propagate )
  {
    invalidate ();

    set<AutoSegment*,AutoSegment::CompareId> collapseds;
    collapseds.insert ( this );

    if ( propagate ) {
      forEach ( AutoSegment*, isegment, getCollapseds() )
        collapseds.insert ( *isegment );
    }

    set<AutoSegment*>::iterator isegment = collapseds.begin();
    for ( ; isegment != collapseds.end() ; isegment++ )
      (*isegment)->_slacken ();

    Session::invalidate         ( getNet() );
    Session::revalidateTopology ();
  }


  bool  AutoSegment::canPivotUp ( float reserve, unsigned int flags )
  {
    ltrace(200) << "AutoSegment::canPivotUp() - " << flags
                << " (reserve:" << reserve << ")" << endl;

    if ( isLayerChange() or isFixed() ) return false;
    if ( isTerminal   () or isLocal() ) return false;

  //if ( isTerminal() ) return false;

    size_t depth = Session::getRoutingGauge()->getLayerDepth(getLayer());
    if ( depth+2 >= Session::getRoutingGauge()->getDepth() ) return false;

    vector<GCell*> gcells;
    getGCells ( gcells );
    for ( size_t i=0 ; i<gcells.size() ; i++ ) {
      if ( not gcells[i]->hasFreeTrack(depth+2,reserve) ) return false;
    }

    if ( not (flags&IgnoreContact) ) {
      ltrace(200) << getAutoSource() << endl;
      ltrace(200) << getAutoTarget() << endl;
      ltrace(200) << "min depths, Segment:" << depth
                  <<            " S:" << getAutoSource()->getMinDepth()
                  <<            " T:" << getAutoTarget()->getMinDepth() << endl;

      if ( getAutoSource()->getMinDepth() < depth ) return false;
      if ( getAutoTarget()->getMinDepth() < depth ) return false;
    }

    if ( flags & Propagate ) {
      forEach ( AutoSegment*, isegment, getCollapseds() ) {
        isegment->getGCells ( gcells );
        for ( size_t i=0 ; i<gcells.size() ; i++ ) {
          if ( not gcells[i]->hasFreeTrack(depth+2,reserve) ) return false;
        }
        if ( isegment->getAutoSource()->getMinDepth() < depth ) return false;
        if ( isegment->getAutoTarget()->getMinDepth() < depth ) return false;
      }
    } else {
      ltrace(200) << "AutoSegment::canPivotUp() - true [no propagate]" << endl;
      return true;
    }

    ltrace(200) << "AutoSegment::canPivotUp() - true [propagate]" << endl;

    return true;
  }


  bool  AutoSegment::canPivotDown ( bool propagate, float reserve )
  {
    ltrace(200) << "AutoSegment::canPivotDown()"
                << " (reserve:" << reserve << ")" << endl;

    if ( isLayerChange() or isFixed() ) return false;
    if ( isTerminal   () or isLocal() ) return false;
  //if ( isTerminal   () ) return false;

    size_t depth = Session::getRoutingGauge()->getLayerDepth(getLayer());
    if ( depth < 3 ) return false;

    vector<GCell*> gcells;
    getGCells ( gcells );
    for ( size_t i=0 ; i<gcells.size() ; i++ ) {
      if ( not gcells[i]->hasFreeTrack(depth-2,reserve) ) return false;
    }

    ltrace(200) << getAutoSource() << endl;
    ltrace(200) << getAutoTarget() << endl;
    ltrace(200) << "max depths, Segment:" << depth
                <<            " S:" << getAutoSource()->getMaxDepth()
                <<            " T:" << getAutoTarget()->getMaxDepth() << endl;

    if ( getAutoSource()->getMaxDepth() > depth ) return false;
    if ( getAutoTarget()->getMaxDepth() > depth ) return false;
    if ( not propagate ) {
      ltrace(200) << "AutoSegment::canPivotDown() - true [no propagate]" << endl;
      return true;
    }

    if ( propagate ) {
      forEach ( AutoSegment*, isegment, getCollapseds() ) {
        isegment->getGCells ( gcells );
        for ( size_t i=0 ; i<gcells.size() ; i++ ) {
          if ( not gcells[i]->hasFreeTrack(depth-2,reserve) ) return false;
        }
        if ( isegment->getAutoSource()->getMaxDepth() < depth ) return false;
        if ( isegment->getAutoTarget()->getMaxDepth() < depth ) return false;
      }
    }

    ltrace(200) << "AutoSegment::canPivotDown() - true [propagate]" << endl;

    return true;
  }


  bool  AutoSegment::canMoveUp ( float reserve, unsigned int flags )
  {
    ltrace(200) << "AutoSegment::canMoveUp() " << flags
                << " (reserve:" << reserve << ")" << endl;

    GCell* begin = NULL;
    GCell* end   = NULL;

    if ( isLayerChange() or isFixed() ) return false;
    if ( isTerminal() and (not (flags & AllowTerminal)) ) return false;
    if ( isLocal()    and (not (flags & AllowLocal   )) ) return false;

    size_t depth = Session::getRoutingGauge()->getLayerDepth(getLayer()) + 2;
    if ( depth >= Session::getConfiguration()->getAllowedDepth() ) return false;

    vector<GCell*> gcells;
    getGCells ( gcells );
    begin = *gcells.begin ();
    end   = *gcells.rbegin();

    for ( size_t i=0 ; i<gcells.size() ; i++ ) {
      if ( not gcells[i]->hasFreeTrack(depth,reserve) ) {
        ltrace(200) << "Not enough free track in " << gcells[i] << endl;
        return false;
      }
    }

    ltrace(200) << "Enough free track under canonical segment." << endl;

    if ( isLocal() and not (flags & Propagate) ) {
      if ( not getAutoSource()->canMoveUp(this) ) return false;
      if ( not getAutoTarget()->canMoveUp(this) ) return false;
      return true;
    }

    bool hasGlobalSegment = false;
    if ( flags & Propagate ) {
      forEach ( AutoSegment*, isegment, getCollapseds() ) {
        if ( isegment->isFixed () ) return false;
        if ( isegment->isGlobal() ) hasGlobalSegment = true;

        isegment->getGCells ( gcells );
        if ( (*gcells.begin ())->getIndex() < begin->getIndex() ) begin = *gcells.begin (); 
        if ( (*gcells.rbegin())->getIndex() > end  ->getIndex() ) end   = *gcells.rbegin(); 

        for ( size_t i=0 ; i<gcells.size() ; i++ ) {
          if ( not gcells[i]->hasFreeTrack(depth,reserve) ) {
            ltrace(200) << "Not enough free track in " << gcells[i] << endl;
            return false;
          }
        }
      }
    }

    if ( (depth >= 4) and (flags & PerpandicularFrag) ) {
      float fragmentation = begin->getFragmentation(depth-1);
      ltrace(200) << "Check begin GCell perpandicular fragmentation: " << fragmentation << endl;

      if ( fragmentation < 0.5 ) {
        ltrace(200) << "Not enough free track for perpandicular in begin GCell "
                    << "(frag:" << fragmentation << ")."
                    << endl;
        return false;
      }

      fragmentation = end->getFragmentation(depth-1);
      ltrace(200) << "Check end GCell perpandicular fragmentation: " << fragmentation << endl;

      if ( fragmentation < 0.5 ) {
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
    if ( not canMoveUp(0.0,flags) ) return false;
    changeDepth ( Session::getRoutingGauge()->getLayerDepth(getLayer()) + 2, flags&Propagate );

    return true;
  }


  bool  AutoSegment::moveDown ( unsigned int flags )
  {
    if ( not canPivotDown(0.0,flags) ) return false;
    changeDepth ( Session::getRoutingGauge()->getLayerDepth(getLayer()) - 2, flags&Propagate );

    return true;
  }


  bool  AutoSegment::shearUp ( GCell* upGCell, AutoSegment*& movedUp, float reserve, unsigned int flags )
  {
    ltrace(200) << "AutoSegment::shearUp() " << this << endl;

    movedUp = NULL;

    if ( isLayerChange() or isFixed() /*or isTerminal()*/ or isLocal() ) return false;

    size_t upDepth = Session::getRoutingGauge()->getLayerDepth(getLayer()) + 2;
    if ( upDepth >= Session::getConfiguration()->getAllowedDepth() ) return false;

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
    const vector<AutoSegment*>& doglegs = Session::getDogLegs();

    if ( leftShear  ) {
      makeDogLeg ( leftShear, true );
      movedUp = doglegs[2];
    } else {
      before  = NULL;
      movedUp = this;
    }

    if ( rightShear ) makeDogLeg(rightShear,true);

    if ( movedUp->moveUp(flags) ) {
      if ( rightShear or leftShear )
        cinfo << "Shearing Up " << this << "." << endl;
      return true;
    }

    movedUp = NULL;
    return false;
  }


  bool  AutoSegment::canDogLeg ( Interval interval )
  {
    ltrace(200) << "AutoSegment::canDogLeg(Interval) " << interval << endl;

    size_t  leftDogleg  = 0;
    size_t  rightDogleg = 0;
    if ( getSpanU().contains(interval.getVMin()) ) leftDogleg++;
    if ( getSpanU().contains(interval.getVMax()) ) rightDogleg++;

    forEach ( AutoSegment*, isegment, getCollapseds() ) {
      if ( isegment->getSpanU().contains(interval.getVMin()) ) {
        if ( isegment->isFixed() ) return false;
        leftDogleg++;
      }
      if ( isegment->getSpanU().contains(interval.getVMax()) ) {
        if ( isegment->isFixed() ) return false;
        rightDogleg++;
      }
    }

    if ( (leftDogleg == 1) and (rightDogleg <= 1) ) return true;
    if ( (leftDogleg <= 1) and (rightDogleg == 1) ) return true;

    ltrace(200) << "leftCount:" << leftDogleg << " rightCount:" << rightDogleg << endl;

    return false;
  }


  void  AutoSegment::makeDogLeg ( Interval interval, bool upLayer, bool& leftDogleg )
  {
    ltrace(200) << "AutoSegment::makeDogLeg(Interval)" << endl;

    size_t       leftDoglegCount  = 0;
    size_t       rightDoglegCount = 0;
    AutoSegment* leftCandidate    = NULL;
    AutoSegment* rightCandidate   = NULL;

    if ( getSpanU().contains(interval.getVMin()) ) { leftCandidate  = this; leftDoglegCount++; }
    if ( getSpanU().contains(interval.getVMax()) ) { rightCandidate = this; rightDoglegCount++; }

    forEach ( AutoSegment*, isegment, getCollapseds() ) {
      if ( isegment->getSpanU().contains(interval.getVMin()) ) { leftCandidate  = *isegment; leftDoglegCount++; }
      if ( isegment->getSpanU().contains(interval.getVMax()) ) { rightCandidate = *isegment; rightDoglegCount++; }
    }

    leftDogleg = true;
    if ( (leftDoglegCount != 1) and (rightDoglegCount != 1) )  return;
    if ( !leftDoglegCount ) {
      leftDogleg     = false;
      leftCandidate  = rightCandidate;
      rightCandidate = NULL;
    }
    
    if ( leftCandidate && rightCandidate ) {
      ltrace(200) << "Left  Constraint: " << leftCandidate->getSourceConstraints(true) << endl;
      ltrace(200) << "Right Constraint: " << rightCandidate->getTargetConstraints(true) << endl;

      if ( leftCandidate ->getTargetConstraints(true).getSize()
         < rightCandidate->getSourceConstraints(true).getSize() ) {
        leftCandidate = rightCandidate;
        leftDogleg    = false;
      }
    } else {
      if ( !leftCandidate ) {
        leftCandidate = rightCandidate;
        leftDogleg    = false;
      }
    }

    if ( leftCandidate ) {
      leftCandidate->_makeDogLeg ( getAutoSource()->getGCell(), upLayer );

      const vector<AutoSegment*>& dogLegs = Session::getDogLegs();
      if ( dogLegs.size() >= 2 ) {
        DbU::Unit axis;
        if ( leftDogleg )
          axis = interval.getVMin() - DbU::lambda(5.0); // Ugly: Hard-wired track spacing.
        else
          axis = interval.getVMax() + DbU::lambda(5.0); // Ugly: Hard-wired track spacing.

        ltrace(200) << "AutoSegment::makeDogLeg(): @" << DbU::getValueString(axis) << endl;
        dogLegs[1]->setAxis ( axis );
      }
    }
  }


  void  AutoSegment::makeDogLeg ( GCell* dogLegGCell, bool upLayer )
  {
    ltrace(160) << "AutoSegment::makeDogLeg(GCell*)" << endl;
    ltracein(160);

    if ( isFixed() ) {
      cerr << Error("AutoSegment::makeDogLeg(): Cannot make a dog leg on a fixed segment.\n"
                   "        (on: %s)",_getString().c_str()) << endl;
      return;
    }

    invalidate ();

    if ( dogLegGCell->getUSide(getDirection()).intersect(getSpanU()) ) {
      ltrace(159) << "Dogleg in " << this << endl;
      _makeDogLeg ( dogLegGCell, upLayer );
    //Session::revalidate ( getNet() );
    } else {
      ltrace(159) << "Looking in aligneds." << endl;
      forEach ( AutoSegment*, aligned, getCollapseds() ) {
        ltrace(159) << "| Try in " << *aligned << endl;
        if ( dogLegGCell->getUSide(getDirection()).intersect(aligned->getSpanU()) ) {
          ltrace(159) << "Dogleg in " << *aligned << endl;
          aligned->_makeDogLeg ( dogLegGCell, upLayer );
        //Session::revalidate ( getNet() );
          ltraceout(160);
          return;
        }
      }
      cerr << Bug("Cannot make a dogleg in %s at %s"
                 ,_getString().c_str(), getString(dogLegGCell).c_str()) << endl;
    }
    ltraceout(160);
  }


  bool  AutoSegment::_check () const
  {
    bool coherency = true;

    coherency = coherency && checkInvalidated();
    coherency = coherency && checkPositions();
    coherency = coherency && checkConstraints();

    return coherency;
  }


  string  AutoSegment::_getString () const
  {
    string  s = getSegment()->_getString();
    s.insert ( 1, "id: " );
    s.insert ( 4, getString(_id) );
    s.insert ( s.size()-1, (_isFixed    )?" F":" -" );
    s.insert ( s.size()-1, (_strap      )? "S": "-" );
    s.insert ( s.size()-1, (_isCanonical)? "C": "-" );
    s.insert ( s.size()-1, (_isCollapsed)? "c": "-" );
    s.insert ( s.size()-1, (_isGlobal)   ? "g": "-" );
    s.insert ( s.size()-1, (_isTerminal) ? "t": "-" );
    s.insert ( s.size()-1, (_slackened)  ? "S": "-" );
    s.insert ( s.size()-1, (_invalidated)? "i": "-" );
    return s;
  }


  Record* AutoSegment::_getRecord () const
  {
    Record* record = getSegment()->_getRecord ();
    record->add ( getSlot ( "_gcell"         ,  _gcell          ) );
    record->add ( getSlot ( "_isHorizontal"  , &_isHorizontal   ) );
    record->add ( getSlot ( "_isFixed"       , &_isFixed        ) );
    record->add ( getSlot ( "_strap"         , &_strap          ) );
    record->add ( getSlot ( "_layerChange"   , &_layerChange    ) );
    record->add ( getSlot ( "_isCanonical"   , &_isCanonical    ) );
    record->add ( getSlot ( "_isCollapsed"   , &_isCollapsed    ) );
    record->add ( getSlot ( "_isGlobal"      , &_isGlobal       ) );
    record->add ( getSlot ( "_isTerminal"    , &_isTerminal     ) );
    record->add ( getSlot ( "_slackened"     , &_slackened      ) );
    record->add ( getSlot ( "_invalidated"   , &_invalidated    ) );
    record->add ( getSlot ( "_sourcePosition", &_sourcePosition ) );
    record->add ( getSlot ( "_targetPosition", &_targetPosition ) );
    return record;
  }


  AutoSegment* AutoSegment::create ( AutoContact*  source
                                   , AutoContact*  target
                                   , Segment*      hurricaneSegment
                                   )
  {
    static Layer* verticalLayer    = DataBase::getDB()->getTechnology()->getLayer ( "METAL3"  );
    static Layer* horizontalLayer  = DataBase::getDB()->getTechnology()->getLayer ( "METAL2"  );

    AutoSegment* segment;
    Horizontal*  horizontal;
    Vertical*    vertical;

    Contact*     contact     = dynamic_cast<Contact*>(hurricaneSegment->getSource());
    AutoContact* autoContact = Session::lookup(contact);
    if ( contact == NULL ) {
      throw Error ( badSegmentSource, getString(hurricaneSegment).c_str() );
      if ( autoContact and ( autoContact != source ) )
        throw Error ( mismatchSegmentSource
                    , getString(hurricaneSegment).c_str()
                    , getString(contact).c_str() );
    }

    contact     = dynamic_cast<Contact*>(hurricaneSegment->getTarget());
    autoContact = Session::lookup(contact);
    if ( contact == NULL ) {
      throw Error ( badSegmentTarget, getString(hurricaneSegment).c_str() );
      if ( autoContact and ( autoContact != target ) )
        throw Error ( mismatchSegmentTarget
                    , getString(hurricaneSegment).c_str()
                    , getString(contact).c_str() );
    }

    Hook* hook = hurricaneSegment->getSourceHook();
    hook->detach ();
    hook->attach ( source->getBodyHook() );

    hook = hurricaneSegment->getTargetHook();
    hook->detach ();
    hook->attach ( target->getBodyHook() );

    if ( (horizontal = dynamic_cast<Horizontal*>(hurricaneSegment) ) ) {
      if ( horizontal->getLayer() != horizontalLayer ) {
        if ( !Session::getKatabatic()->isGMetal(horizontal->getLayer()) )
          cerr << Warning("Segment %s forced to %s."
                         ,getString(horizontal).c_str()
                         ,getString(horizontalLayer).c_str()) << endl;
        horizontal->setLayer ( horizontalLayer );
      }

      segment = AutoHorizontal::create ( horizontal
                                       , AutoSegment::Global
                                       , false
                                       , false
                                       );
    } else if ( (vertical = dynamic_cast<Vertical*>(hurricaneSegment)) ) {
      if ( vertical->getLayer() != verticalLayer ) {
        if ( !Session::getKatabatic()->isGMetal(vertical->getLayer()) )
          cerr << Warning("Segment %s forced to %s."
                         ,getString(vertical).c_str()
                         ,getString(verticalLayer).c_str()) << endl;
        vertical->setLayer ( verticalLayer );
      }

      segment = AutoVertical::create ( vertical
                                     , AutoSegment::Global
                                     , false
                                     , false
                                     );
    } else {
      throw Error ( badSegment, getString(source).c_str(), getString(target).c_str() );
    }

    ltrace(99) << "Creating " << segment << endl;

    return segment;
  }


  AutoSegment* AutoSegment::create ( AutoContact*  source
                                   , AutoContact*  target
                                   , unsigned int  dir
                                   , int           type
                                   , bool          terminal
                                   , bool          collapsed
                                   )
  {
  //static Layer* verticalLayer   = DataBase::getDB()->getTechnology()->getLayer ( "METAL3" );
  //static Layer* horizontalLayer = DataBase::getDB()->getTechnology()->getLayer ( "METAL2" );

    static const Layer* horizontalLayer = Session::getRoutingLayer ( 1 );
    static const Layer* verticalLayer   = Session::getRoutingLayer ( 2 );

    GCell*       gcell;
    GCell*       end;
    AutoSegment* segment;

    if ( dir & Constant::Horizontal ) {
      segment = AutoHorizontal::create ( source
                                       , target
                                       , horizontalLayer
                                       , source->getY()
                                       , DbU::lambda(2.0)
                                       , type
                                       , terminal
                                       , collapsed
                                       );
      if ( type == AutoSegment::Global ) {
        if ( source->getGCell()->getX() < target->getGCell()->getX() ) {
          gcell = source->getGCell()->getRight();
          end   = target->getGCell();
        } else {
          gcell = target->getGCell()->getRight();
          end   = source->getGCell();
        }
        for ( ; gcell != end ; gcell = gcell->getRight() ) {
          if ( !gcell ) {
            cerr << Error("AutoSegment::create() : NULL GCell.") << endl;
            break;
          }
          gcell->addHSegment ( segment );
        }
      }
    } else if ( dir & Constant::Vertical ) {
      segment = AutoVertical::create ( source
                                     , target
                                     , verticalLayer
                                     , source->getX()
                                     , DbU::lambda(2.0)
                                     , type
                                     , terminal
                                     , collapsed
                                     );
      if ( type == AutoSegment::Global ) {
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
          gcell->addVSegment ( segment );
        }
      }
    } else
      throw Error ( badSegment, getString(source).c_str(), getString(target).c_str() );

    ltrace(99) << "create() " << segment << endl;

    return segment;
  }


  void  AutoSegment::destroy ()
  {
    _preDestroy ();
    delete this;
  }


  bool  AutoSegment::isTopologicalBound ( AutoSegment*  seed
                                        , bool          superior
                                        , bool          isHorizontal )
  {
    ltrace(80) << "isTopologicalBound() - " << seed << endl;
    ltracein(80);

    set<AutoContact*>     exploreds;
    vector<AutoContact*>  stack;
    DbU::Unit             axis;

    if ( superior ) axis = seed->getTargetU();
    else            axis = seed->getSourceU();

    ltrace(80) << "check for bound " << DbU::getValueString(axis) << endl;

    exploreds.insert ( seed->getAutoSource() );
    exploreds.insert ( seed->getAutoTarget() );

    if ( seed->getLength() ) {
      if ( superior ) stack.push_back ( seed->getAutoTarget() );
      else            stack.push_back ( seed->getAutoSource() );
    } else {
      stack.push_back ( seed->getAutoTarget() );
      stack.push_back ( seed->getAutoSource() );
    }

    while ( !stack.empty() ) {
      AutoContact* currentContact = stack.back();
      stack.pop_back ();

      ltrace(80) << "Exploring: " << (void*)currentContact
                 << " " << currentContact << endl;

      exploreds.insert ( currentContact );

      if ( currentContact->getAnchor() ) { ltraceout(80); return true; }

      forEach ( Component*, component, currentContact->getSlaveComponents() ) {
        Segment* segment = dynamic_cast<Segment*>(*component);
        if ( !segment ) continue;

        AutoSegment* autoSegment = Session::lookup ( segment );
        if ( !autoSegment ) continue;

        if ( !autoSegment->getLength() ) {
          AutoContact* contact = autoSegment->getAutoSource();
          if ( contact && ( contact != currentContact ) ) {
            if ( exploreds.find(contact) == exploreds.end() )
              stack.push_back ( contact );
          }

          contact = autoSegment->getAutoTarget();
          if ( contact && ( contact != currentContact ) ) {
            if ( exploreds.find(contact) == exploreds.end() )
              stack.push_back ( contact );
          }

          continue;
        }

        if ( autoSegment->isHorizontal() != isHorizontal ) continue;

        ltrace(80) << "| " << autoSegment << endl;

        if ( superior ) {
          if ( autoSegment->getTargetU() > axis ) { ltraceout(80); return true; }
        } else {
          if ( autoSegment->getSourceU() < axis ) { ltraceout(80); return true; }
        }
      }
    }

    ltraceout(80);
    return false;
  }


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


  void  AutoSegment::getTopologicalInfos ( AutoSegment*          seed
                                         , vector<AutoSegment*>& collapseds
                                         , vector<AutoSegment*>& perpandiculars
                                         , DbU::Unit&            leftBound
                                         , DbU::Unit&            rightBound
                                         )
  {
    ltrace(80) << "getTopologicalInfos() - " << seed << endl;

    leftBound  = DbU::Max;
    rightBound = DbU::Min;

    AutoSegmentStack stack;

    stack.push ( seed->getAutoSource(), seed );
    stack.push ( seed->getAutoTarget(), seed );

    while ( not stack.isEmpty() ) {
      AutoContact* sourceContact = stack.getAutoContact ();
      AutoSegment* sourceSegment = stack.getAutoSegment ();

      stack.pop ();

      DbU::Unit  constraint;

      if ( seed->isHorizontal() ) constraint = sourceContact->getCBXMax();
      else                        constraint = sourceContact->getCBYMax();
      if ( constraint < leftBound ) leftBound = constraint;

      if ( seed->isHorizontal() ) constraint = sourceContact->getCBXMin();
      else                        constraint = sourceContact->getCBYMin();
      if ( constraint > rightBound ) rightBound = constraint;

      forEach ( Component*, component, sourceContact->getSlaveComponents() ) {
        Segment* segment = dynamic_cast<Segment*>(*component);
        if ( ( not segment ) || ( segment == sourceSegment->getSegment() ) ) continue;

        AutoSegment* currentSegment = Session::lookup ( segment );
        if ( not currentSegment ) {
          cerr << Error("Can't lookup <AutoSegment> for %s.",getString(segment).c_str()) << endl;
          continue;
        }

        unsigned int  state = getPerpandicularState ( sourceContact
                                                    , sourceSegment
                                                    , currentSegment
                                                    , seed           );
        if ( state & PerpandicularAny ) {
          ltrace(79) << "Perpandicular: " << currentSegment << endl;
          perpandiculars.push_back ( currentSegment );
        }
        if ( state & (PerpandicularIndirect
                     |ParallelOrExpanded
                     |ParallelAndLayerChange ) ) {
          ltrace(79) << "Reject: " << currentSegment << endl;
          continue;
        }

        if ( not areAligneds(currentSegment,seed) ) {
          collapseds.push_back ( currentSegment );
          ltrace(79) << "collapsed: " << currentSegment << endl;
        }

        Component*   opposite      = segment->getOppositeAnchor ( sourceContact->getContact() );
        AutoContact* targetContact = Session::lookup(static_cast<Contact*>(opposite));
        if ( targetContact ) stack.push ( targetContact, currentSegment );
      }
    }
  }


  int  AutoSegment::getTerminalCount ( AutoSegment* seed, vector<AutoSegment*>& collapseds )
  {
    ltrace(80) << "getTerminalCount() - " << seed << " (+collapseds)" << endl;

    int  count = 0;
    for ( size_t i=0 ; i < collapseds.size() ; i++ ) {
      if ( collapseds[i]->isTerminal() )
        count++;
    }
    if ( seed->getAutoSource()->isTerminal() ) count++;
    if ( seed->getAutoTarget()->isTerminal() ) count++;

    return count;
  }


} // End of Katabatic namespace.
