// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2012-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AutoContactTerminal.cpp"                |
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
#include "katabatic/AutoContactTerminal.h"
#include "katabatic/AutoContactTurn.h"
#include "katabatic/AutoVertical.h"
#include "katabatic/AutoHorizontal.h"
#include "katabatic/Session.h"


namespace Katabatic {

  using std::ostringstream;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::DebugSession;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoContactTerminal".


  AutoContactTerminal* AutoContactTerminal::create ( GCell*       gcell
                                                   , Component*   anchor
                                                   , const Layer* layer
                                                   , Point        point
                                                   , DbU::Unit    width
                                                   , DbU::Unit    height
                                                   )
  {
    ltrace(90) << "AutoContactTerminal::create(... Point ...)" << endl;
    ltracein(90);
    ltrace(90) << "@" << point << endl; 

    anchor->getBodyHook()->detach();

    AutoContactTerminal* autoContact = AutoContactTerminal::create( gcell
                                                                  , anchor
                                                                  , layer
                                                                  , point.getX(), point.getY()
                                                                  , width, height
                                                                  );
    ltraceout(90);
    return autoContact;
  }


  AutoContactTerminal* AutoContactTerminal::create (       GCell*     gcell
                                                   ,       Component* anchor
                                                   , const Layer*     layer
                                                   , const DbU::Unit  x
                                                   , const DbU::Unit  y
                                                   , const DbU::Unit  width
                                                   , const DbU::Unit  height
                                                   )
  {
    ltrace(90) << "AutoContactTerminal::create(... x, y ...)" << endl;
    ltrace(90) << "@ x:" << DbU::getValueString(x) << " y:" << DbU::getValueString(y) << endl; 

    Point anchorPosition = anchor->getPosition();

    Contact* contact = Contact::create( anchor
                                      , layer
                                      , x - anchorPosition.getX()
                                      , y - anchorPosition.getY()
                                      , width
                                      , height
                                      );
    AutoContactTerminal* autoContact = new AutoContactTerminal( gcell, contact );
    
    autoContact->_postCreate();
    autoContact->unsetFlags( CntInCreationStage );

    ltrace(90) << "create(Component*) " << autoContact << endl;
    return autoContact;
  }


  AutoContactTerminal::AutoContactTerminal ( GCell* gcell, Contact* contact )
    : AutoContact(gcell,contact)
    , _segment   (NULL)
  {
    setFlags( CntTerminal );
  }


  AutoContactTerminal::~AutoContactTerminal ()
  { }


  AutoSegment* AutoContactTerminal::getOpposite ( const AutoSegment* ) const
  { return NULL; }


  AutoSegment* AutoContactTerminal::getSegment ( unsigned int index ) const
  {
    if (_segment) {
      switch ( index ) {
        case 0: return (_segment->isHorizontal()) ? _segment : NULL;
        case 2: return (_segment->isVertical  ()) ? _segment : NULL;
      }
    }
    return NULL;
  }


  Box  AutoContactTerminal::getNativeConstraintBox () const
  {
    Component* component = getAnchor();
    if (component == NULL) {
      cerr << Error( "%s is not anchored.", getString(this).c_str() ) << endl;
      return _gcell->getBoundingBox ();
    }
    
    DbU::Unit   xMin;
    DbU::Unit   xMax;
    DbU::Unit   yMin;
    DbU::Unit   yMax;
    Vertical*   vertical;
    Horizontal* horizontal;
    RoutingPad* routingPad;

    if ( (horizontal = dynamic_cast<Horizontal*>(component)) ) {
      xMin = horizontal->getSourcePosition().getX();
      xMax = horizontal->getTargetPosition().getX();
      yMin = yMax
           = horizontal->getTargetPosition().getY();
    } else if ( (vertical = dynamic_cast<Vertical*>(component)) ) {
      yMin = vertical->getSourcePosition().getY();
      yMax = vertical->getTargetPosition().getY();
      xMin = xMax
           = vertical->getTargetPosition().getX();
    } else if ( (routingPad = dynamic_cast<RoutingPad*>(component)) ) {
      Entity* entity = routingPad->getOccurrence().getEntity();

    // Assumes there is no rotation in the Transformation.
      if ( dynamic_cast<Horizontal*>(entity) ) {
        xMin = routingPad->getSourcePosition().getX();
        xMax = routingPad->getTargetPosition().getX();
        yMin = yMax
             = routingPad->getTargetPosition().getY();
      } else if ( dynamic_cast<Vertical*>(entity) ) {
        yMin = routingPad->getSourcePosition().getY();
        yMax = routingPad->getTargetPosition().getY();
        xMin = xMax
             = routingPad->getTargetPosition().getX();
      } else {
        xMin = xMax = routingPad->getPosition().getX();
        yMin = yMax = routingPad->getPosition().getY();
      }
    } else {
      xMin = xMax = component->getPosition().getX();
      yMin = yMax = component->getPosition().getY();
    }

    order( xMin, xMax );
    order( yMin, yMax );

    return Box( xMin, yMin, xMax, yMax );
  }


  void  AutoContactTerminal::_invalidate ( unsigned int flags )
  {
    if (_segment) _segment->invalidate();
  }


  void  AutoContactTerminal::cacheDetach ( AutoSegment* segment )
  {
    if (_segment == segment) {
      _segment = NULL;
      setFlags( CntInvalidatedCache );
    }
  }


  void  AutoContactTerminal::cacheAttach ( AutoSegment* segment )
  {
    if (_segment) {
      cerr << Bug( "%s::cacheAttach() On %s,\n"
                   "      cache has not been cleared first, cancelled."
                 , _getTypeName().c_str(), getString(this).c_str()
                 ) << endl;
      return;
    }
    _segment = segment;
    unsetFlags( CntInvalidatedCache  );
  }


  void  AutoContactTerminal::updateCache ()
  {
    DebugSession::open( getNet(), 80 );

    ltrace(110) << _getTypeName() << "::updateCache() " << this << endl;
    ltracein(110);

    Component*   anchor;
    Horizontal** horizontals = new Horizontal* [2];
    Vertical**   verticals   = new Vertical*   [2];

    _getTopology( anchor, horizontals, verticals, 2 );

    if (anchor == NULL)
      showTopologyError( "Terminal is missing an anchor (RoutingPad or Component)." );

    size_t count = 0;
    if (horizontals[0] != NULL) ++count;
    if (horizontals[1] != NULL) ++count;
    if (verticals  [0] != NULL) ++count;
    if (verticals  [1] != NULL) ++count;
    if (count > 1) {
      showTopologyError( "Terminal has more than one segment." );
    }
    if (horizontals[0] != NULL ) {
      _segment = Session::lookup( horizontals[0] );
    } else {
      _segment = Session::lookup( verticals[0] );
    }
    if (_segment == NULL) {
      ostringstream os;
      os << this << ", AutoSegment lookup failed for:"
         << "\n        h1: " << horizontals[0] 
         << "\n        v1: " << verticals[0];

      delete [] horizontals;
      delete [] verticals;

      showTopologyError( os.str() );
      throw Error( os.str() );
    }
    unsetFlags( CntInvalidatedCache );
    ltrace(110) << "seg:" << _segment << endl;

    delete [] horizontals;
    delete [] verticals;

    ltraceout(110);
    DebugSession::close();
  }


  void  AutoContactTerminal::updateGeometry ()
  {
    DebugSession::open( getNet(), 80 );

    ltrace(110) << _getTypeName() << "::updateGeometry() " << this << endl;
    ltracein(110);

    if (isInvalidatedCache()) updateCache();
    if (isInvalidatedCache()) {
      cerr << Error( "%s::updateGeometry() %s: Unable to restore cache."
                   , _getTypeName().c_str(), getString(this).c_str() ) << endl;
      ltraceout(110);
      return;
    }

    base()->invalidate( false );
    unsetFlags( CntInvalidated );

    ostringstream message;
    if (not hasBadTopology()) {
      if (_segment->isHorizontal()) {
        if (not getUConstraints(KbVertical).contains(_segment->getY())) {
          ltrace(110) << "Cached: " << _segment << endl;
          message << "Terminal horizontal segment Y " << DbU::getValueString(_segment->getY())
                  << " axis is outside RoutingPad " << getUConstraints(KbVertical) << ".";
          showTopologyError( message.str() );
        } else
          setY( _segment->getY() );
      } else {
        if (not getUConstraints(KbHorizontal).contains(_segment->getX())) {
          ltrace(110) << "Cached: " << _segment << endl;
          message << "Terminal vertical segment X" << DbU::getValueString(_segment->getX())
                  << " axis is outside RoutingPad " << getUConstraints(KbHorizontal) << ".";
          showTopologyError( message.str() );
        } else
          setX( _segment->getX() );
      }
    }

    ltraceout(110);
    DebugSession::close();
  }


  void  AutoContactTerminal::updateTopology ()
  {
    DebugSession::open( getNet(), 80 );

    ltrace(110) << _getTypeName() << "::updateTopology() " << this << endl;
    ltracein(110);

    if (isInvalidatedCache()) updateCache();
    if (isInvalidatedCache()) {
      cerr << Error( "%s::updateGeometry() %s: Unable to restore cache."
                   , _getTypeName().c_str(), getString(this).c_str() ) << endl;
      ltraceout(110);
      return;
    }

    RoutingGauge* rg           = Session::getRoutingGauge();
    size_t        anchorDepth  = rg->getLayerDepth( (_flags & CntIgnoreAnchor) ? getLayer()
                                                                               : getAnchor()->getLayer() );
    size_t        segmentDepth = rg->getLayerDepth( _segment->getLayer() );
    size_t        delta        = abssub( anchorDepth, segmentDepth );

    if (delta > 3) {
      showTopologyError( "Sheared Terminal, layer delta exceed 3." );
      setFlags( CntBadTopology );
    } else {
      if (delta > 1) {
      //_segment = _segment->makeDogleg( this );
        _segment->makeDogleg( this );
        ltrace(110) << "Update seg: " << _segment << endl;
        delta = abssub( anchorDepth, rg->getLayerDepth( _segment->getLayer() ) );
      }
      else if (delta == 0) setLayer( rg->getRoutingLayer(anchorDepth) );
      else if (delta == 1) setLayer( rg->getContactLayer(std::min(anchorDepth,segmentDepth)) );
    }
    _segment->invalidate( this );

    ltraceout(110);
    DebugSession::close();
  }


  string  AutoContactTerminal::_getTypeName () const
  { return "ContactTerminal"; }


} // Katabatic namespace.
