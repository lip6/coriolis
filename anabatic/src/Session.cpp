// -*- mode: C++; explicit-buffer-name: "Session.cpp<anabatic>" -*-
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
// |  C++ Module  :       "./Session.cpp"                            |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Cell.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/RoutingLayerGauge.h"
#include "anabatic/Configuration.h"
#include "anabatic/Session.h"
#include "anabatic/AutoContact.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/AnabaticEngine.h"


namespace {


  const char* reopenSession    = "Anabatic::Session::_open(): Session already open for %s (internal error).";
  const char* openSessionError = "%s: Session has not been opened (internal error).";


} // End of local namespace.




namespace Anabatic {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::ForEachIterator;
  using Hurricane::UpdateSession;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::Cell;
  using CRL::RoutingLayerGauge;


// -------------------------------------------------------------------
// Class  :  "Anabatic::Session".

  Session*  Session::_session = NULL;


  Session* Session::get ( const char* message )
  {
    if (not _session and message)
      throw Error( openSessionError, message );
    return _session;
  }


  Session::Session ( AnabaticEngine* anbt )
    : _anabatic           (anbt)
    , _technology         (anbt->getConfiguration()->getRoutingGauge()->getTechnology())
    , _cellGauge          (anbt->getConfiguration()->getCellGauge())
    , _routingGauge       (anbt->getConfiguration()->getRoutingGauge())
    , _autoContacts       ()
    , _doglegs            ()
    , _segmentInvalidateds()
    , _segmentRevalidateds()
    , _netInvalidateds    ()
    , _netRevalidateds    ()
  { }


  void  Session::_postCreate ()
  {
    UpdateSession::open();
    _session = this;
  }


  Session::~Session ()
  { }


  void  Session::_preDestroy ()
  {
    if (_anabatic->getState() <= EngineActive) {
      _revalidate ();
      _anabatic->updateDensity();
    }
    UpdateSession::close();
  }


  bool           Session::_doDestroyBaseContact () { return _anabatic->doDestroyBaseContact(); }
  bool           Session::_doDestroyBaseSegment () { return _anabatic->doDestroyBaseSegment(); }
  bool           Session::_doDestroyTool        () { return _anabatic->doDestroyTool(); }
  Configuration* Session::_getConfiguration     () { return _anabatic->getConfiguration(); }


  void  Session::_invalidate ( Net* net )
  {
    cdebug_log(149,0) << "Session::invalidate(Net*) - " << net << endl;
    _netInvalidateds.insert(net);
  }


  void  Session::_canonize ()
  {
    cdebug_log(145,1) << "Anabatic::Session::_canonize()" << endl;

    if (_segmentInvalidateds.empty()) {
      cdebug_log(145,0) << "Invalidated AutoSegment collection <_segmentInvalidateds> is empty." << endl;
      cdebug_tabw(145,-1);
      return;
    }

    set<Segment*>        exploredSegments;
    vector<AutoSegment*> aligneds;

  // Should no longer be necessary to ensure determinism.
  //sort( _segmentInvalidateds.begin(), _segmentInvalidateds.end(), AutoSegment::CompareId() );

    for ( size_t i=0 ; i<_segmentInvalidateds.size() ; i++ ) {
      AutoSegment* seedSegment = _segmentInvalidateds[i];
      AutoSegment* canonical   = seedSegment;

      if (exploredSegments.find(seedSegment->base()) == exploredSegments.end()) {
        cdebug_log(145,0) << "New chunk from: " << seedSegment << endl;
        aligneds.push_back( seedSegment );

        bool isWeakGlobal = seedSegment->isGlobal();
        if (not seedSegment->isNotAligned()) {
          forEach ( AutoSegment*, aligned, seedSegment->getAligneds() ) {
            cdebug_log(145,0) << "Aligned: " << *aligned << endl;
            aligneds.push_back ( *aligned );
            exploredSegments.insert ( aligned->base() );

            isWeakGlobal = isWeakGlobal or aligned->isGlobal();
            if (AutoSegment::CompareId()( *aligned, canonical ))
              canonical = *aligned;
          }
        }

        cdebug_tabw(145,1);

        canonical->setFlags( AutoSegment::SegCanonical );
        cdebug_log(145,0) << "Canonical: " << canonical << endl;
        Interval userConstraints = canonical->getUserConstraints();

        for ( size_t j=0 ; j<aligneds.size() ; j++ ) {
          if (isWeakGlobal and not aligneds[j]->isGlobal()) aligneds[j]->setFlags  ( AutoSegment::SegWeakGlobal );
          else                                              aligneds[j]->unsetFlags( AutoSegment::SegWeakGlobal );

          if (aligneds[j] == canonical) continue;
          if (aligneds[j]->isCanonical()) {
            cerr << Error("Session::_canonize(): On %s\n"
                          "        Segment is no longer the canonical one, this must not happens."
                         ,getString(aligneds[j]).c_str()) << endl;
          }
          aligneds[j]->unsetFlags( AutoSegment::SegCanonical );
          cdebug_log(145,0) << "Secondary: " << aligneds[j] << endl;

          userConstraints.intersection( aligneds[j]->getUserConstraints() );
        }
        if (aligneds.empty()) canonical->setFlags( AutoSegment::SegNotAligned );

        if (not getRoutingGauge()->isSymbolic()
           and (userConstraints.getSize() < Session::getPitch(1)*2) ) {
            cerr << Warning( "Session::_canonize(): On %s\n"
                             "          Combined user constraints are too tight [%s : %s]."
                           , getString(canonical).c_str()
                           , DbU::getValueString(userConstraints.getVMin()).c_str()
                           , DbU::getValueString(userConstraints.getVMax()).c_str()
                           ) << endl;
        }

        cdebug_log(149,0) << "Align on canonical:" << canonical << endl;
        cdebug_log(145,0) << "Combined user constraints: " << userConstraints << endl;

      //canonical->setAxis( canonical->getAxis(), Flags::Realignate );
        if (canonical->isUnsetAxis() and not canonical->isFixed())
          canonical->toOptimalAxis( Flags::Realignate|Flags::Propagate );
        else
          canonical->setAxis( canonical->getAxis(), Flags::Realignate|Flags::Propagate );
        aligneds.clear();
        cdebug_tabw(145,-1);
      }
    }

    cdebug_tabw(145,-1);
  }


  void  Session::_revalidateTopology ()
  {
    cdebug_log(145,1) << "Anabatic::Session::_revalidateTopology()" << endl;

    for ( Net* net : _netInvalidateds ) {
      cdebug_log(145,0) << "Anabatic::Session::_revalidateTopology(Net*)" << net << endl;
      _anabatic->updateNetTopology    ( net );
      _anabatic->computeNetConstraints( net );
      _anabatic->_computeNetOptimals  ( net );
      _anabatic->_computeNetTerminals ( net );
    }
    _canonize ();

    for ( AutoSegment* segment : _segmentInvalidateds ) {
      if (segment->isCanonical()) {
        if (segment->isUnsetAxis()) segment->toOptimalAxis();
        else                        segment->toConstraintAxis();
      }
    }
    
    _netRevalidateds.clear();
    _netRevalidateds.swap( _netInvalidateds );

    cdebug_tabw(145,-1);
  }


  size_t  Session::_revalidate ()
  {
    cdebug_log(145,1) << "Anabatic::Session::revalidate()" << endl;
    cdebug_log(145,0)   << "_segmentInvalidateds.size(): " << _segmentInvalidateds.size() << endl;
    cdebug_log(145,0)   << "_autoContacts.size(): " << _autoContacts.size() << endl;

    size_t count = 0;

    if (not _netInvalidateds.empty()) _revalidateTopology();

    cdebug_log(145,0) << "AutoContacts Revalidate (after _revalidateTopology())." << endl;
    for ( size_t i=0 ; i < _autoContacts.size() ; i++, count++ )
      _autoContacts[i]->updateGeometry();
    _autoContacts.clear();

    cdebug_log(145,0) << "AutoSegments Revalidate (after AutoContact::updateGeometry())." << endl;
    cdebug_log(145,0) << "_segmentInvalidateds.size(): " << _segmentInvalidateds.size() << endl;

    _segmentRevalidateds.clear();
    for ( size_t i=0 ; i < _segmentInvalidateds.size() ; ++i, ++count ) {
      _segmentInvalidateds[i]->revalidate();
      if ( not _destroyedSegments.empty()
         and (_destroyedSegments.find(_segmentInvalidateds[i]) != _destroyedSegments.end()) )
        continue;

      _segmentRevalidateds.push_back( _segmentInvalidateds[i] );
    }
    _segmentInvalidateds.clear();

    cdebug_log(145,0) << "AutoSegments/AutoContacts queued deletion." << endl;
    Flags flags = _anabatic->flags() & Flags::DestroyMask;
    _anabatic->flags() = Flags::DestroyMask;
    set<AutoSegment*>::iterator  isegment = _destroyedSegments.begin();
    for ( ; isegment != _destroyedSegments.end() ; isegment++ ) {
      AutoContact* source = (*isegment)->getAutoSource();
      AutoContact* target = (*isegment)->getAutoTarget();
      (*isegment)->destroy();
      if (source and source->canDestroy(true)) source->destroy();
      if (target and target->canDestroy(true)) target->destroy();
    }
    _anabatic->flags() = flags;
    set<AutoSegment*>().swap( _destroyedSegments );

    cdebug_tabw(145,-1);

    return count;
  }


  Session*  Session::_open ( AnabaticEngine* anbt )
  {
    cdebug_log(145,0) << "Anabatic::Session::_open()" << endl;

    if (_session) {
      if (_session->_anabatic != anbt)
        throw Error( reopenSession, getString(_session->getAnabatic()).c_str() );

      return _session;
    }

    Session* session = new Session ( anbt );
    session->_postCreate();

    return session;
  }


  void  Session::close ()
  {
    cdebug_log(145,1) << "Session::close()" << endl;

    if (not _session) throw Error( openSessionError, "Session::Close()" );

    _session->_preDestroy();

    delete _session;
    _session = NULL;

    cdebug_tabw(145,-1);
  }


  Flags  Session::getDirection ( size_t depth )
  {
    RoutingGauge* rg = get("getDirection()")->_routingGauge;
    switch ( rg->getLayerDirection(depth) ) {
      case Constant::Horizontal: return Flags::Horizontal;
      case Constant::Vertical:   return Flags::Vertical;
    }
    return Flags::NoFlags;
  }


  DbU::Unit  Session::_getPitch ( size_t depth, Flags flags ) const
  {
    if (flags == Flags::NoFlags) return _routingGauge->getLayerPitch(depth);

    if (flags & Flags::AboveLayer) {
      if (depth < getAllowedDepth()) 
        return _routingGauge->getLayerPitch( depth + 1 );
      else {
        if ( (depth > 0) and (_routingGauge->getLayerType(depth-1) != Constant::PinOnly) )
          return _routingGauge->getLayerPitch( depth - 1 );
      }
    }

    if (flags & Flags::BelowLayer) {
      if ( (depth > 0) and (_routingGauge->getLayerType(depth-1) != Constant::PinOnly) )
        return _routingGauge->getLayerPitch( depth - 1 );
      else {
        if (depth < getAllowedDepth())
          return _routingGauge->getLayerPitch( depth + 1 );
      }
    }

  // Should issue at least a warning here.
    return _routingGauge->getLayerPitch(depth);
  }


  Point  Session::_getNearestGridPoint ( Point p, Box constraint )
  {
    Box ab = _anabatic->getCell()->getAbutmentBox();

    RoutingLayerGauge* lg = _routingGauge->getVerticalGauge();
    DbU::Unit x = lg->getTrackPosition( ab.getXMin()
                                      , lg->getTrackIndex( ab.getXMin()
                                                         , ab.getXMax()
                                                         , p.getX()
                                                         , Constant::Nearest ) );
    if (x < constraint.getXMin()) x += lg->getPitch();
    if (x > constraint.getXMax()) x -= lg->getPitch();

    lg = _routingGauge->getHorizontalGauge();
    DbU::Unit y = lg->getTrackPosition( ab.getYMin()
                                      , lg->getTrackIndex( ab.getYMin()
                                                         , ab.getYMax()
                                                         , p.getY()
                                                         , Constant::Nearest ) );
    if (y < constraint.getYMin()) y += lg->getPitch();
    if (y > constraint.getYMax()) y -= lg->getPitch();

    return Point(x,y);
  }


  bool  Session::isInDemoMode ()
  { return get("isInDemoMode()")->_anabatic->isInDemoMode(); }


  float  Session::getSaturateRatio ()
  { return get("getSaturateRatio()")->_anabatic->getSaturateRatio(); }


  size_t  Session::getSaturateRp ()
  { return get("getSaturateRp()")->_anabatic->getSaturateRp(); }


  bool  Session::doWarnGCellOverload ()
  { return get("doWarnGCellOverload()")->_anabatic->doWarnOnGCellOverload(); }


  void  Session::setAnabaticFlags ( Flags flags )
  { get("setKabaticFlags()")->_anabatic->flags() = flags; }


  void  Session::link ( AutoContact* autoContact )
  { return get("link(AutoContact*)")->_anabatic->_link( autoContact ); }


  void  Session::link ( AutoSegment* autoSegment )
  { return get("link(AutoSegment*)")->_anabatic->_link( autoSegment ); }


  void  Session::unlink ( AutoContact* autoContact )
  { return get("unlink(AutoContact*)")->_anabatic->_unlink( autoContact ); }


  void  Session::unlink ( AutoSegment* autoSegment )
  { return get("unlink(AutoSegment*)")->_anabatic->_unlink( autoSegment ); }


  AutoContact* Session::lookup ( Contact* contact )
  { return get("lookup(Contact*)")->_anabatic->_lookup( contact ); }


  AutoSegment* Session::lookup ( Segment* segment )
  { return get("lookup(Segment*)")->_anabatic->_lookup( segment ); }


  string  Session::_getString () const
  {
    return "<" + _getTypeName() + " "
               + getString(_anabatic->getCell()->getName())
               + ">";
  }


  Record* Session::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot ( "_anabatic"    , _anabatic      ) );
    record->add ( getSlot ( "_autoContacts", &_autoContacts ) );
  //record->add ( getSlot ( "_autoSegments", &_autoSegments ) );
    return record;
  }


} // End of Anabatic namespace.
