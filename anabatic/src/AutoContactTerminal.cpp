// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2012-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
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
#include "anabatic/AutoContactTerminal.h"
#include "anabatic/AutoContactTurn.h"
#include "anabatic/AutoVertical.h"
#include "anabatic/AutoHorizontal.h"
#include "anabatic/Session.h"


namespace Anabatic {

  using std::ostringstream;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::DebugSession;
  using Hurricane::Transformation;
  using Hurricane::Entity;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoContactTerminal".


  AutoContactTerminal* AutoContactTerminal::create ( GCell*       gcell
                                                   , Component*   anchor
                                                   , const Layer* layer
                                                   , Point        point
                                                   , DbU::Unit    width
                                                   , DbU::Unit    height
                                                   )
  {
    cdebug_log(145,1) << "AutoContactTerminal::create(... Point, ...)" << endl;
    cdebug_log(145,0)   << "@" << point << endl; 

    anchor->getBodyHook()->detach();

    AutoContactTerminal* autoContact = AutoContactTerminal::create( gcell
                                                                  , anchor
                                                                  , layer
                                                                  , point.getX(), point.getY()
                                                                  , width, height
                                                                  );
    cdebug_tabw(145,-1);
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
    cdebug_log(145,0) << "AutoContactTerminal::create(... x, y, ...)" << endl;
    cdebug_log(145,0) << "@ x:" << DbU::getValueString(x) << " y:" << DbU::getValueString(y) << endl; 

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

    cdebug_log(145,0) << "create(Component*) " << autoContact << endl;
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


  AutoSegment* AutoContactTerminal::getPerpandicular ( const AutoSegment* ) const
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
    cdebug_log(145,1) << "AutoContactTerminal::getNativeConstraintBox()" << endl;

    Component* component = getAnchor();
    if (component == NULL) {
      cerr << Error( "%s is not anchored.", getString(this).c_str() ) << endl;
      cdebug_tabw(145,-1);
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
      cdebug_log(145,0) << "Anchor: " << horizontal << "@" << horizontal->getSourcePosition() << endl;
      xMin = horizontal->getSourcePosition().getX();
      xMax = horizontal->getTargetPosition().getX();
      yMin = yMax
           = horizontal->getTargetPosition().getY();
    } else if ( (vertical = dynamic_cast<Vertical*>(component)) ) {
      cdebug_log(145,0) << "Anchor: " << vertical << "@" << vertical->getSourcePosition() << endl;
      yMin = vertical->getSourcePosition().getY();
      yMax = vertical->getTargetPosition().getY();
      xMin = xMax
           = vertical->getTargetPosition().getX();
    } else if ( (routingPad = dynamic_cast<RoutingPad*>(component)) ) {
      Entity*         entity = routingPad->getOccurrence().getEntity();
      Transformation  transf = routingPad->getOccurrence().getPath().getTransformation();
      cdebug_log(145,0) << "Anchor: " << routingPad << endl;

      int rpOrient = 1;
      switch ( transf.getOrientation() ) {
        case Transformation::Orientation::R1:
        case Transformation::Orientation::R3:
        case Transformation::Orientation::XR:
        case Transformation::Orientation::YR:
          rpOrient = 2;
          break;
        default:
          break;
      }

      if (dynamic_cast<Horizontal*>(entity)) {
      // rpOrient *is* the rotation.
      } else if ( dynamic_cast<Vertical*>(entity) ) {
      // rpOrient is the inverse rotation.
        rpOrient = (rpOrient == 1) ? 2 : 1;
      } else {
        rpOrient = 0;
      }
      
      switch ( rpOrient ) {
        case 1:
          xMin = routingPad->getSourcePosition().getX();
          xMax = routingPad->getTargetPosition().getX();
          yMin = yMax
               = routingPad->getTargetPosition().getY();
          break;
        case 2:
          yMin = routingPad->getSourcePosition().getY();
          yMax = routingPad->getTargetPosition().getY();
          xMin = xMax
               = routingPad->getTargetPosition().getX();
          break;
        default:
          xMin = xMax = routingPad->getPosition().getX();
          yMin = yMax = routingPad->getPosition().getY();
          break;
      }
    } else {
      xMin = xMax = component->getPosition().getX();
      yMin = yMax = component->getPosition().getY();
    }

    order( xMin, xMax );
    order( yMin, yMax );

    cdebug_log(145,0) << "| Using (y): " << DbU::getValueString(yMin) << " "
                                   << DbU::getValueString(yMax) << endl;

    cdebug_tabw(145,-1);
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
    DebugSession::open( getNet(), 140, 150 );

    cdebug_log(145,1) << _getTypeName() << "::updateCache() " << this << endl;

    Component*   anchor;
    Horizontal** horizontals = new Horizontal* [2];
    Vertical**   verticals   = new Vertical*   [2];

    _getTopology( base(), anchor, horizontals, verticals, 2 );

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
    cdebug_log(145,0) << "seg:" << _segment << endl;

    delete [] horizontals;
    delete [] verticals;

    cdebug_tabw(145,-1);
    DebugSession::close();
  }


  void  AutoContactTerminal::updateGeometry ()
  {
    DebugSession::open( getNet(), 140, 150 );

    cdebug_log(145,1) << _getTypeName() << "::updateGeometry() " << this << endl;

    if (isInvalidatedCache()) updateCache();
    if (isInvalidatedCache()) {
      cerr << Error( "%s::updateGeometry() %s: Unable to restore cache."
                   , _getTypeName().c_str(), getString(this).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    base()->invalidate( false );
    unsetFlags( CntInvalidated );

    ostringstream message;
    if (not hasBadTopology()) {
      if (_segment->isHorizontal()) {
        if (not getUConstraints(Flags::Vertical).contains(_segment->getY())) {
          cdebug_log(145,0) << "Cached: " << _segment << endl;
          message << "Terminal horizontal segment Y " << DbU::getValueString(_segment->getY())
                  << " axis is outside RoutingPad " << getUConstraints(Flags::Vertical) << ".";

          Interval intv;
          _segment->getConstraints( intv );
          message << "\n        Segment constraints: " << intv << endl;

          unsigned int flags = 0;
          if (_segment->isCreated()) flags |= Flags::CParanoid;
          showTopologyError( message.str(), flags );
        } else
          setY( _segment->getY() );
      } else {
        if (not getUConstraints(Flags::Horizontal).contains(_segment->getX())) {
          cdebug_log(145,0) << "Cached: " << _segment << endl;
          message << "Terminal vertical segment X" << DbU::getValueString(_segment->getX())
                  << " axis is outside RoutingPad " << getUConstraints(Flags::Horizontal) << ".";

          unsigned int flags = 0;
          if (_segment->isCreated()) flags |= Flags::CParanoid;
          showTopologyError( message.str(), flags );
        } else
          setX( _segment->getX() );
      }
    }

    cdebug_tabw(145,-1);
    DebugSession::close();
  }


  void  AutoContactTerminal::updateTopology ()
  {
    DebugSession::open( getNet(), 140, 150 );

    cdebug_log(145,1) << _getTypeName() << "::updateTopology() " << this << endl;

    if (isInvalidatedCache()) updateCache();
    if (isInvalidatedCache()) {
      cerr << Error( "%s::updateGeometry() %s: Unable to restore cache."
                   , _getTypeName().c_str(), getString(this).c_str() ) << endl;
      cdebug_tabw(145,-1);
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
        cdebug_log(145,0) << "Update seg: " << _segment << endl;
        delta = abssub( anchorDepth, rg->getLayerDepth( _segment->getLayer() ) );
      }
      else if (delta == 0) setLayer( rg->getRoutingLayer(anchorDepth) );
      else if (delta == 1) setLayer( rg->getContactLayer(std::min(anchorDepth,segmentDepth)) );
    }
    _segment->invalidate( this );

    cdebug_tabw(145,-1);
    DebugSession::close();
  }


  string  AutoContactTerminal::_getTypeName () const
  { return "ContactTerminal"; }


} // Anabatic namespace.
