// -*- mode: C++; explicit-buffer-name: "Session.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
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
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Cell.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/RoutingGauge.h"
#include "anabatic/Configuration.h"
#include "anabatic/Session.h"
#include "anabatic/AutoContact.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/AnabaticEngine.h"


namespace {


  const char* reopenSession =
    "Session::open() :\n\n"
    "    Session already open for %s (internal error).";

  const char* openSessionError =
    "%s :\n\n"
    "    Session has not been opened (internal error).";


} // End of local namespace.




namespace Anabatic {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::Error;
  using Hurricane::ForEachIterator;
  using Hurricane::UpdateSession;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::Cell;


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

        canonical->setFlags( SegCanonical );
        cdebug_log(145,0) << "Canonical: " << canonical << endl;

        for ( size_t j=0 ; j<aligneds.size() ; j++ ) {
          if (isWeakGlobal and not aligneds[j]->isGlobal()) aligneds[j]->setFlags  ( SegWeakGlobal );
          else                                              aligneds[j]->unsetFlags( SegWeakGlobal );

          if (aligneds[j] == canonical) continue;
          if (aligneds[j]->isCanonical()) {
            cerr << Error("Session::_canonize(): On %s\n"
                          "        Segment is no longer the canonical one, this must not happens."
                         ,getString(aligneds[j]).c_str()) << endl;
          }
          aligneds[j]->unsetFlags( SegCanonical );
          cdebug_log(145,0) << "Secondary: " << aligneds[j] << endl;
        }
        if (aligneds.empty()) canonical->setFlags( SegNotAligned );

        cdebug_log(149,0) << "Align @" << DbU::toLambda(canonical->getAxis())
                    << " on " << canonical << endl;

      //canonical->setAxis( canonical->getAxis(), Flags::Realignate );
        if (canonical->isUnsetAxis()) canonical->toOptimalAxis( Flags::Realignate|Flags::Propagate );
        else                          canonical->setAxis( canonical->getAxis(), Flags::Realignate|Flags::Propagate );
        aligneds.clear();
        cdebug_tabw(145,-1);
      }
    }

    cdebug_tabw(145,-1);
  }


  void  Session::_revalidateTopology ()
  {
    cdebug_log(145,1) << "Anabatic::Session::_revalidateTopology()" << endl;

    set<Net*>::iterator inet = _netInvalidateds.begin();

    for ( ; inet != _netInvalidateds.end() ; inet++ ) {
      cdebug_log(145,0) << "Anabatic::Session::_revalidateTopology(Net*)" << *inet << endl;
      _anabatic->updateNetTopology    ( *inet );
      _anabatic->computeNetConstraints( *inet );
      _anabatic->_computeNetOptimals  ( *inet );
      _anabatic->_computeNetTerminals ( *inet );
    }
    _canonize ();

    for ( size_t i=0 ; i<_segmentInvalidateds.size() ; ++i ) {
      if (_segmentInvalidateds[i]->isCanonical()) {
        if (_segmentInvalidateds[i]->isUnsetAxis()) _segmentInvalidateds[i]->toOptimalAxis();
        else                                        _segmentInvalidateds[i]->toConstraintAxis();
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
    unsigned int flags = _anabatic->flags() & Flags::DestroyMask;
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


  Session*  Session::open ( AnabaticEngine* anbt )
  {
    cdebug_log(145,0) << "Session::open()" << endl;

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


  unsigned int  Session::getDirection ( size_t depth )
  {
    RoutingGauge* rg = get("getDirection()")->_routingGauge;
    switch ( rg->getLayerDirection(depth) ) {
      case Constant::Horizontal: return Flags::Horizontal;
      case Constant::Vertical:   return Flags::Vertical;
    }
    return 0;
  }


  DbU::Unit  Session::_getPitch ( size_t depth, unsigned int flags ) const
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


  bool  Session::isInDemoMode ()
  { return get("isInDemoMode()")->_anabatic->isInDemoMode(); }


  float  Session::getSaturateRatio ()
  { return get("getSaturateRatio()")->_anabatic->getSaturateRatio(); }


  size_t  Session::getSaturateRp ()
  { return get("getSaturateRp()")->_anabatic->getSaturateRp(); }


  bool  Session::doWarnGCellOverload ()
  { return get("doWarnGCellOverload()")->_anabatic->doWarnOnGCellOverload(); }


  void  Session::setAnabaticFlags ( unsigned int flags )
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
