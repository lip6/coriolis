// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2012-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        A n a b a t i c  -  Routing Toolbox                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AutoContactTurn.cpp"                    |
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
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/DebugSession.h"
#include "crlcore/RoutingGauge.h"
#include "anabatic/AutoContactTurn.h"
#include "anabatic/AutoVertical.h"
#include "anabatic/AutoHorizontal.h"
#include "anabatic/Session.h"


namespace Anabatic {

  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::DebugSession;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoContactTurn".


  AutoContactTurn* AutoContactTurn::create ( GCell* gcell, Net* net, const Layer* layer )
  {
    _preCreate( gcell, net, layer );

    DbU::Unit viaSide = Session::getViaWidth( layer );
    Contact*  contact = Contact::create ( net
                                        , layer
                                        , gcell->getCenter().getX()
                                        , gcell->getCenter().getY()
                                        , viaSide
                                        , viaSide
                                        );
    AutoContactTurn* autoContact = new AutoContactTurn ( gcell, contact );

    autoContact->_postCreate();
    autoContact->unsetFlags( CntInCreationStage );

    cdebug_log(145,0) << "create(net*) " << autoContact << endl;
    return autoContact;
  }


  AutoContactTurn::AutoContactTurn ( GCell* gcell, Contact* contact )
    : AutoContact (gcell,contact)
    , _horizontal1(NULL)
    , _vertical1  (NULL)
  {
    setFlags( CntTurn );
  }


  AutoContactTurn::~AutoContactTurn ()
  { }
    

  AutoSegment* AutoContactTurn::getOpposite ( const AutoSegment* ) const
  { return NULL; }


  AutoSegment* AutoContactTurn::getPerpandicular ( const AutoSegment* reference ) const
  {
    cdebug_log(149,0) << _getTypeName() << "::getPerpandicular() " << this
                      << " to:" << reference << endl;
    cdebug_log(149,0) << "| _horizontal1:" << _horizontal1 << endl;
    cdebug_log(149,0) << "| _vertical1  :" << _vertical1   << endl;

    if (reference == _horizontal1) return _vertical1;
    if (reference == _vertical1  ) return _horizontal1;

    cdebug_log(149,0) << _getTypeName() << "::getPerpandicular() " << this
                      << " to:" << reference << " failed." << endl;

    return NULL;
  }


  AutoSegment* AutoContactTurn::getSegment ( unsigned int index ) const
  {
    switch ( index ) {
      case 0: return _horizontal1; 
      case 2: return _vertical1;
    }
    return NULL;
  }


  AutoHorizontal*  AutoContactTurn::getHorizontal1 () const { return _horizontal1; };
  AutoVertical*    AutoContactTurn::getVertical1   () const { return _vertical1; };


  void  AutoContactTurn::_invalidate ( Flags flags )
  {
    if (_horizontal1) _horizontal1->invalidate( flags|Flags::Propagate );
    if (_vertical1  ) _vertical1  ->invalidate( flags|Flags::Propagate );
  }


  void  AutoContactTurn::cacheDetach ( AutoSegment* segment )
  {
    cdebug_log(149,0) << _getTypeName() << "::cacheDetach() " << this
                      << " from:" << segment << endl;

    if      (segment == _horizontal1) _horizontal1 = NULL;
    else if (segment == _vertical1)   _vertical1   = NULL;
    else return;

    setFlags( CntInvalidatedCache );
  }


  void  AutoContactTurn::cacheAttach ( AutoSegment* segment )
  {
    cdebug_log(149,0) << _getTypeName() << "::cacheAttach() " << this
                      << " to:" << segment << endl;

    if (segment->getDirection() == Flags::Horizontal) {
      if (_horizontal1) {
        cerr << Bug( "%s::cacheAttach() On %s,\n"
                     "      h1 cache has not been cleared first, cancelled."
                   , _getTypeName().c_str(), getString(this).c_str()
                   ) << endl;
        return;
      }
      _horizontal1 = static_cast<AutoHorizontal*>(segment);
    } else if (segment->getDirection() == Flags::Vertical) {
      if (_vertical1) {
        cerr << Bug( "%s::cacheAttach() On %s,\n"
                     "      v1 cache has not been cleared first, cancelled."
                   , _getTypeName().c_str(), getString(this).c_str()
                   ) << endl;
        return;
      }
      _vertical1 = static_cast<AutoVertical*>(segment);
    }

    if (_horizontal1 and _vertical1) unsetFlags( CntInvalidatedCache  );
    setFlags( CntInvalidatedWidth );
  }


  void  AutoContactTurn::updateCache ()
  {
    DebugSession::open( getNet(), 145, 150 );

    cdebug_log(145,1) << _getTypeName() << "::updateCache() " << this << endl;

    Component*   anchor;
    Horizontal** horizontals = new Horizontal* [2];
    Vertical**   verticals   = new Vertical*   [2];

    _getTopology ( base(), anchor, horizontals, verticals, 2 );

    _horizontal1 = static_cast<AutoHorizontal*>( Session::lookup(horizontals[0]) );
    _vertical1   = static_cast<AutoVertical  *>( Session::lookup(verticals  [0]) );

    string message;
    if      (horizontals[0] == NULL) message = "Turn is missing mandatory horizontal segment.";
    else if (horizontals[1] != NULL) message = "Turn has more than one horizontal segment.";
    else if (verticals  [0] == NULL) message = "Turn is missing mandatory vertical segment.";
    else if (verticals  [1] != NULL) message = "Turn has more than one vertical segment.";
    else if (_horizontal1   == NULL) message = "AutoSegment lookup failed on horizontal segment.";
    else if (_vertical1     == NULL) message = "AutoSegment lookup failed on vertical segment.";
    if (not message.empty()) {
      showTopologyError( message );
      setFlags( CntBadTopology );
    } else {
      unsetFlags( CntInvalidatedCache );
      setFlags  ( CntInvalidatedWidth );
    }

    cdebug_log(145,0) << "h1:" << _horizontal1 << endl;
    cdebug_log(145,0) << "v1:" << _vertical1 << endl;

    delete [] horizontals;
    delete [] verticals;

    cdebug_tabw(145,-1);
    DebugSession::close();
  }


  void  AutoContactTurn::updateGeometry ()
  {
    DebugSession::open( getNet(), 145, 150 );

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

    if (not hasBadTopology()) {
      setX( getVertical1  ()->getX() );
      setY( getHorizontal1()->getY() );
      updateSize();
    }

    cdebug_tabw(145,-1);
    DebugSession::close();
  }


  void  AutoContactTurn::updateTopology ()
  {
    DebugSession::open ( getNet(), 145, 150 );

    cdebug_log(145,1) << _getTypeName() << "::updateTopology() " << this << endl;

    if (isInvalidatedCache()) updateCache();
    if (isInvalidatedCache()) {
      cerr << Error( "%s::updateGeometry() %s: Unable to restore cache."
                   , _getTypeName().c_str(), getString(this).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    if (not hasBadTopology()) {
      RoutingGauge* rg           = Session::getRoutingGauge();
      size_t        depthH1      = rg->getLayerDepth( getHorizontal1()->getLayer() );
      size_t        depthV1      = rg->getLayerDepth( getVertical1  ()->getLayer() );
      size_t        depthContact = (depthH1 <= depthV1) ? depthH1 : depthH1-1;
      size_t        delta        = abssub ( depthH1, depthV1 );

      unsetFlags( CntWeakTerminal );
      if (delta > 3) {
        showTopologyError( "Sheared Turn, layer delta exceed 3." );
        setFlags( CntBadTopology );
      } else {
        if (delta > 1) {
          setLayerAndWidth( delta, std::min( depthH1, depthV1 ));

          bool updateH1 = (depthH1 > depthV1);
          if (_vertical1->isNonPref() xor _horizontal1->isNonPref())
            updateH1 = _vertical1->isNonPref();
          
          if (updateH1) {
          //_horizontal1 = static_cast<AutoHorizontal*>( _horizontal1->makeDogleg(this) );
            _horizontal1->makeDogleg(this);
            cdebug_log(145,0) << "Update h1: " << _horizontal1 << endl;
          } else /*if (_vertical1->isInvalidatedLayer())*/ {
          //_vertical1 = static_cast<AutoVertical*>( _vertical1->makeDogleg(this) );
            _vertical1->makeDogleg(this);
            cdebug_log(145,0) << "Update v1: " << _vertical1 << endl;
          }
          depthH1      = rg->getLayerDepth( _horizontal1->getLayer() );
          depthV1      = rg->getLayerDepth( _vertical1->getLayer() );
          depthContact = (depthH1 <= depthV1) ? depthH1 : depthH1-1;
          delta        = abssub ( depthH1, depthV1 );
        }

        setLayerAndWidth( delta, depthContact );
      }

      _horizontal1->invalidate( this );
      _vertical1  ->invalidate( this );
    }

    unsetFlags( CntInvalidatedTopology );
    cdebug_tabw(145,-1);
    DebugSession::close ();
  }


  string  AutoContactTurn::_getTypeName () const
  { return "ContactTurn"; }


} // Anabatic namespace.
