
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
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
#include "katabatic/Session.h"
#include "katabatic/AutoContact.h"
#include "katabatic/AutoSegment.h"
#include "katabatic/AutoSegment.h"
#include "katabatic/GCellGrid.h"
#include "katabatic/KatabaticEngine.h"


namespace {


  const char* reopenSession =
    "Session::open() :\n\n"
    "    Session already open for %s (internal error).";

  const char* openSessionError =
    "%s :\n\n"
    "    Session has not been opened (internal error).";


} // End of local namespace.




namespace Katabatic {


  using namespace std;
  using Hurricane::tab;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::inltrace;
  using Hurricane::ltracelevel;
  using Hurricane::Error;
  using Hurricane::ForEachIterator;
  using Hurricane::UpdateSession;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::Cell;


// -------------------------------------------------------------------
// Class  :  "Katabatic::Session".

  Session*  Session::_session = NULL;


  Session* Session::get ( const char* message )
  {
    if (not _session and message)
      throw Error( openSessionError, message );
    return _session;
  }


  Session::Session ( KatabaticEngine* ktbt )
    : _katabatic          (ktbt)
    , _technology         (ktbt->getRoutingGauge()->getTechnology())
    , _routingGauge       (ktbt->getRoutingGauge())
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
    if (_katabatic->getState() <= EngineActive) {
      _revalidate ();

      if (_katabatic->getGCellGrid())
        _katabatic->getGCellGrid()->updateDensity();
    }
    UpdateSession::close();
  }


  bool           Session::_doDestroyBaseContact () { return _katabatic->doDestroyBaseContact(); }
  bool           Session::_doDestroyBaseSegment () { return _katabatic->doDestroyBaseSegment(); }
  bool           Session::_doDestroyTool        () { return _katabatic->doDestroyTool(); }
  Configuration* Session::_getConfiguration     () { return _katabatic->getConfiguration(); }


  void  Session::_invalidate ( Net* net )
  {
    ltrace(200) << "Session::invalidate(Net*) - " << net << endl;
    _netInvalidateds.insert(net);
  }


  void  Session::_canonize ()
  {
    ltrace(110) << "Katabatic::Session::_canonize()" << endl;
    ltracein(110);

    if (_segmentInvalidateds.empty()) {
      ltrace(110) << "Invalidated AutoSegment collection <_segmentInvalidateds> is empty." << endl;
      ltraceout(110);
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
        ltrace(110) << "New chunk from: " << seedSegment << endl;
        aligneds.push_back( seedSegment );

        bool isWeakGlobal = seedSegment->isGlobal();
        if (not seedSegment->isNotAligned()) {
          forEach ( AutoSegment*, aligned, seedSegment->getAligneds() ) {
            ltrace(110) << "Aligned: " << *aligned << endl;
            aligneds.push_back ( *aligned );
            exploredSegments.insert ( aligned->base() );

            isWeakGlobal = isWeakGlobal or aligned->isGlobal();
            if (AutoSegment::CompareId()( *aligned, canonical ))
              canonical = *aligned;
          }
        }

        ltracein(110);

        canonical->setFlags( SegCanonical );
        ltrace(110) << "Canonical: " << canonical << endl;

        for ( size_t j=0 ; j<aligneds.size() ; j++ ) {
          if (isWeakGlobal) aligneds[j]->setFlags  ( SegWeakGlobal );
          else              aligneds[j]->unsetFlags( SegWeakGlobal );

          if (aligneds[j] == canonical) continue;
          if (aligneds[j]->isCanonical()) {
            cerr << Error("Session::_canonize(): On %s\n"
                          "        Segment is no longer the canonical one, this must not happens."
                         ,getString(aligneds[j]).c_str()) << endl;
          }
          aligneds[j]->unsetFlags( SegCanonical );
          ltrace(110) << "Secondary: " << aligneds[j] << endl;
        }
        if (aligneds.empty()) canonical->setFlags( SegNotAligned );

        ltrace(159) << "Align @" << DbU::getLambda(canonical->getAxis())
                    << " on " << canonical << endl;

        canonical->setAxis( canonical->getAxis(), KbRealignate );
        aligneds.clear();
        ltraceout(110);
      }
    }

    ltraceout(110);
  }


  void  Session::_revalidateTopology ()
  {
    ltrace(110) << "Katabatic::Session::_revalidateTopology()" << endl;
    ltracein(110);

    set<Net*>::iterator inet = _netInvalidateds.begin();

    for ( ; inet != _netInvalidateds.end() ; inet++ ) {
      ltrace(110) << "Katabatic::Session::_revalidateTopology(Net*)" << *inet << endl;
      _katabatic->updateNetTopology    ( *inet );
      _katabatic->computeNetConstraints( *inet );
      _katabatic->_computeNetOptimals  ( *inet );
      _katabatic->_computeNetTerminals ( *inet );
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

    ltraceout(110);
  }


  size_t  Session::_revalidate ()
  {
    ltrace(110) << "Katabatic::Session::revalidate()" << endl;
    ltracein(110);

    ltrace(110) << "_segmentInvalidateds.size(): " << _segmentInvalidateds.size() << endl;
    ltrace(110) << "_autoContacts.size(): " << _autoContacts.size() << endl;

    size_t count = 0;

    if (not _netInvalidateds.empty()) _revalidateTopology();

    ltrace(110) << "AutoContacts Revalidate (after _revalidateTopology())." << endl;
    for ( size_t i=0 ; i < _autoContacts.size() ; i++, count++ )
      _autoContacts[i]->updateGeometry();
    _autoContacts.clear();

    ltrace(110) << "AutoSegments Revalidate (after AutoContact::updateGeometry())." << endl;
    ltrace(110) << "_segmentInvalidateds.size(): " << _segmentInvalidateds.size() << endl;

    _segmentRevalidateds.clear();
    for ( size_t i=0 ; i < _segmentInvalidateds.size() ; ++i, ++count ) {
      _segmentInvalidateds[i]->revalidate();
      if ( not _destroyedSegments.empty()
         and (_destroyedSegments.find(_segmentInvalidateds[i]) != _destroyedSegments.end()) )
        continue;

      _segmentRevalidateds.push_back( _segmentInvalidateds[i] );
    }
    _segmentInvalidateds.clear();

    ltrace(110) << "AutoSegments/AutoContacts queued deletion." << endl;
    unsigned int flags = _katabatic->getFlags( EngineDestroyMask );
    _katabatic->setFlags( EngineDestroyMask );
    set<AutoSegment*>::iterator  isegment = _destroyedSegments.begin();
    for ( ; isegment != _destroyedSegments.end() ; isegment++ ) {
      AutoContact* source = (*isegment)->getAutoSource();
      AutoContact* target = (*isegment)->getAutoTarget();
      (*isegment)->destroy();
      if (source and source->canDestroy(true)) source->destroy();
      if (target and target->canDestroy(true)) target->destroy();
    }
    _katabatic->setFlags( flags );
    set<AutoSegment*>().swap( _destroyedSegments );

    ltraceout(110);

    return count;
  }


  Session*  Session::open ( KatabaticEngine* ktbt )
  {
    ltrace(110) << "Session::open()" << endl;

    if (_session) {
      if (_session->_katabatic != ktbt)
        throw Error( reopenSession, getString(_session->getKatabatic()).c_str() );

      return _session;
    }

    Session* session = new Session ( ktbt );
    session->_postCreate();

    return session;
  }


  void  Session::close ()
  {
    ltrace(110) << "Session::close()" << endl;
    ltracein(110);

    if (not _session) throw Error( openSessionError, "Session::Close()" );

    _session->_preDestroy();

    delete _session;
    _session = NULL;

    ltraceout(110);
  }


  unsigned int  Session::getLayerDirection ( const Layer* layer )
  {
    RoutingGauge* rg = get("getDirection()")->_routingGauge;
    switch ( rg->getLayerDirection(layer) ) {
      case Constant::Horizontal: return KbHorizontal;
      case Constant::Vertical:   return KbVertical;
    }
    return 0;
  }


  bool  Session::isInDemoMode ()
  { return get("isInDemoMode()")->_katabatic->isInDemoMode(); }


  float  Session::getSaturateRatio ()
  { return get("getSaturateRatio()")->_katabatic->getSaturateRatio(); }


  size_t  Session::getSaturateRp ()
  { return get("getSaturateRp()")->_katabatic->getSaturateRp(); }


  bool  Session::doWarnGCellOverload ()
  { return get("doWarnGCellOverload()")->_katabatic->doWarnOnGCellOverload(); }


  void  Session::setKatabaticFlags ( unsigned int flags )
  { get("setKabaticFlags()")->_katabatic->setFlags(flags); }


  DbU::Unit  Session::getExtensionCap ()
  { return getConfiguration()->getExtensionCap(); }


  const Layer* Session::getRoutingLayer ( size_t depth )
  { return getConfiguration()->getRoutingLayer(depth); }


  const Layer* Session::getContactLayer ( size_t depth )
  { return getConfiguration()->getContactLayer(depth); }


  void  Session::link ( AutoContact* autoContact )
  { return get("link(AutoContact*)")->_katabatic->_link( autoContact ); }


  void  Session::link ( AutoSegment* autoSegment )
  { return get("link(AutoSegment*)")->_katabatic->_link( autoSegment ); }


  void  Session::unlink ( AutoContact* autoContact )
  { return get("unlink(AutoContact*)")->_katabatic->_unlink( autoContact ); }


  void  Session::unlink ( AutoSegment* autoSegment )
  { return get("unlink(AutoSegment*)")->_katabatic->_unlink( autoSegment ); }


  AutoContact* Session::lookup ( Contact* contact )
  { return get("lookup(Contact*)")->_katabatic->_lookup( contact ); }


  AutoSegment* Session::lookup ( Segment* segment )
  { return get("lookup(Segment*)")->_katabatic->_lookup( segment ); }


  string  Session::_getString () const
  {
    return "<" + _getTypeName() + " "
               + getString(_katabatic->getCell()->getName())
               + ">";
  }


  Record* Session::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot ( "_katabatic"   , _katabatic     ) );
    record->add ( getSlot ( "_autoContacts", &_autoContacts ) );
  //record->add ( getSlot ( "_autoSegments", &_autoSegments ) );
    return record;
  }


} // End of Katabatic namespace.
