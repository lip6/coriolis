
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
// |  C++ Module  :       "./Session.cpp"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <cstdlib>
#include  <sstream>

#include  "hurricane/Error.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Cell.h"
#include  "hurricane/UpdateSession.h"
#include  "crlcore/RoutingGauge.h"
#include  "katabatic/Session.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoSegment.h"
#include  "katabatic/AutoSegment.h"
#include  "katabatic/GCellGrid.h"
#include  "katabatic/KatabaticEngine.h"


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
    if ( not _session and message )
      throw Error ( openSessionError, message );
    return _session;
  }


  Session::Session ( KatabaticEngine* ktbt )
    : _katabatic       (ktbt)
    , _technology      (ktbt->getRoutingGauge()->getTechnology())
    , _routingGauge    (ktbt->getRoutingGauge())
    , _autoContacts    ()
    , _autoSegments    ()
    , _revalidateds    ()
    , _dogLegs         ()
    , _netInvalidateds ()
    , _netRevalidateds ()
    , _invalidateMask  (0)
  { }


  void  Session::_postCreate ()
  {
    UpdateSession::open ();
    _session = this;
  }


  Session::~Session ()
  { }


  size_t  Session::_preDestroy ()
  {
    size_t count = 0;
    if ( _katabatic->getState() <= StateActive ) {
      _revalidate ();

      if ( _katabatic->getGCellGrid() )
        _katabatic->getGCellGrid()->updateDensity ();
    }
    UpdateSession::close();

    return count;
  }


  bool           Session::_doDestroyBaseContact () { return _katabatic->doDestroyBaseContact(); }
  bool           Session::_doDestroyBaseSegment () { return _katabatic->doDestroyBaseSegment(); }
  bool           Session::_doDestroyTool        () { return _katabatic->doDestroyTool(); }
  Configuration* Session::_getConfiguration     () { return _katabatic->getConfiguration(); }


  void  Session::_splitContacts ()
  {
    ltrace(110) << "Katabatic::Session::_splitContacts()" << endl;
    ltracein(110);

    for ( size_t i=0; i<_autoContacts.size() ; i++ )
      _autoContacts[i]->split ();

    ltraceout(110);
  }


  void  Session::_restoreVCon ()
  {
    ltrace(110) << "Katabatic::Session::_restoreVCon()" << endl;
    ltracein(110);

    for ( size_t i=0; i<_autoContacts.size() ; i++ ) {
      DbU::Unit y = DbU::Max;
      forEach ( Horizontal*, isegment, _autoContacts[i]->getSlaveComponents().getSubSet<Horizontal*>() ) {
        y = isegment->getY();
      }
      _autoContacts[i]->restoreVConnexity ( y, true );
    }

    ltraceout(110);
  }


  void  Session::_restoreHCon ()
  {
    ltrace(110) << "Katabatic::Session::_restoreHCon()" << endl;
    ltracein(110);

    for ( size_t i=0; i<_autoContacts.size() ; i++ ) {
      DbU::Unit x = DbU::Max;
      forEach ( Vertical*, isegment, _autoContacts[i]->getSlaveComponents().getSubSet<Vertical*>() ) {
        x = isegment->getX();
      }
      _autoContacts[i]->restoreHConnexity ( x, true );
    }

    ltraceout(110);
  }


  void  Session::_canonize ()
  {
    ltrace(110) << "Katabatic::Session::_canonize()" << endl;
    ltracein(110);

    set<Segment*>        exploredSegments;
    vector<AutoSegment*> aligneds;

    sort ( _autoSegments.begin(), _autoSegments.end(), AutoSegment::CompareCanonical() );

    for ( size_t i=0 ; i<_autoSegments.size() ; i++ ) {
      AutoSegment* seedSegment = _autoSegments[i];

      if ( exploredSegments.find(seedSegment->getSegment()) == exploredSegments.end() ) {
        ltrace(110) << "New chunk from: " << seedSegment << endl;
        aligneds.push_back ( seedSegment );

        bool isCanonicalLocal = seedSegment->isLocal();
        forEach ( AutoSegment*, collapsed, seedSegment->getCollapseds() ) {
          ltrace(110) << "Aligned: " << *collapsed << endl;
          aligneds.push_back ( *collapsed );
          exploredSegments.insert ( collapsed->getSegment() );

          if ( collapsed->isGlobal() ) isCanonicalLocal = false;
        }

        ltracein(110);
        sort ( aligneds.begin(), aligneds.end(), AutoSegment::CompareCanonical() );

        if ( aligneds.size() > 1 ) {
          if ( not AutoSegment::CompareCanonical() ( aligneds[0], aligneds[1] ) ) {
            cerr << "Ambiguous canonization: " << aligneds[0]->base() << endl;
            cerr << "Ambiguous canonization: " << aligneds[1]->base() << endl;
          }
        }

        aligneds[0]->setCanonical      ( true );
        aligneds[0]->setCanonicalLocal ( isCanonicalLocal );
        ltrace(110) << "Canonical: " << aligneds[0] << endl;

        for ( size_t j=1 ; j<aligneds.size() ; j++ ) {
          aligneds[j]->setCanonical ( false );
          ltrace(110) << "Secondary: " << aligneds[j] << endl;
        }

        ltrace(159) << "Align @" << DbU::getLambda(aligneds[0]->getAxis())
                    << " on " << aligneds[0] << endl;

        aligneds[0]->setAxis ( aligneds[0]->getAxis(), Realignate );
        aligneds.clear ();
        ltraceout(110);
      }
    }

    ltraceout(110);
  }


  void  Session::_revalidateTopology ()
  {
    ltrace(110) << "Katabatic::Session::_revalidateTopology()" << endl;
    ltracein(110);

    set<AutoSegment*> faileds;

    if ( not _netInvalidateds.empty() ) {
      set<Net*>::iterator inet = _netInvalidateds.begin();

      if ( _invalidateMask & NetSplitContacts ) {
        _splitContacts ();
        _invalidateMask &= ~NetSplitContacts;
      }

      if ( _invalidateMask & RestoreVCon ) {
        _restoreVCon ();
        _invalidateMask &= ~RestoreVCon;
      }

      if ( _invalidateMask & RestoreHCon ) {
        _restoreVCon ();
        _invalidateMask &= ~RestoreHCon;
      }

      if ( _invalidateMask & NetCanonize ) {
        for ( ; inet != _netInvalidateds.end() ; inet++ ) {
          ltrace(110) << "Katabatic::Session::_revalidateTopology(Net*)" << *inet << endl;
          _katabatic->_computeNetConstraints ( *inet, faileds );
          _katabatic->_computeNetOptimals    ( *inet );
          _katabatic->_computeNetTerminals   ( *inet );
        }
        _canonize ();

        for ( size_t i=0 ; i<_autoSegments.size() ; ++i ) {
          if (   _autoSegments[i]->isUnsetAxis()
             and _autoSegments[i]->isCanonical() ) {
            _autoSegments[i]->toOptimalAxis ();
          }
        }
        _invalidateMask &= ~NetCanonize;
      }
    }

    ltraceout(110);
  }


  size_t  Session::_revalidate ()
  {
    ltrace(110) << "Katabatic::Session::revalidate()" << endl;
    ltracein(110);

    size_t count = 0;

    _revalidateTopology ();
    _netRevalidateds.clear ();
    _netRevalidateds.swap ( _netInvalidateds );

    ltrace(110) << "AutoContacts Revalidate (after canonize)." << endl;
    for ( size_t i=0 ; i < _autoContacts.size() ; i++, count++ )
      _autoContacts[i]->revalidate ();
    _autoContacts.clear ();

    ltrace(110) << "AutoSegments Revalidate (after canonize)." << endl;

    _dogLegs.clear ();
    _revalidateds.clear ();
    for ( size_t i=0 ; i < _autoSegments.size() ; i++, count++ ) {
      _autoSegments[i]->revalidate ();
      if ( not _destroyedSegments.empty()
         and (_destroyedSegments.find(_autoSegments[i]) != _destroyedSegments.end()) )
        continue;

      _revalidateds.push_back ( _autoSegments[i] );
    }
    _autoSegments.clear ();

    ltrace(110) << "AutoSegments/AutoContacts queued deletion." << endl;
    bool destroySegment = _katabatic->setDestroyBaseSegment ( true );
    bool destroyContact = _katabatic->setDestroyBaseContact ( true );
    set<AutoSegment*>::iterator  isegment = _destroyedSegments.begin();
    for ( ; isegment != _destroyedSegments.end() ; isegment++ ) {
      AutoContact* source = (*isegment)->getAutoSource();
      AutoContact* target = (*isegment)->getAutoTarget();
      (*isegment)->destroy ();
      if (source and source->canDestroy(true)) source->destroy ();
      if (target and target->canDestroy(true)) target->destroy ();
    }
    _katabatic->setDestroyBaseSegment ( destroySegment );
    _katabatic->setDestroyBaseContact ( destroyContact );
    set<AutoSegment*>().swap ( _destroyedSegments );

    ltraceout(110);

    return count;
  }


  Session*  Session::open ( KatabaticEngine* ktbt )
  {
    ltrace(110) << "Session::open()" << endl;

    if ( _session ) {
      if ( _session->_katabatic != ktbt )
        throw Error ( reopenSession, getString(_session->getKatabatic()).c_str() );

      return _session;
    }

    Session* session = new Session ( ktbt );
    session->_postCreate ();

    return session;
  }


  size_t  Session::close ()
  {
    ltrace(110) << "Session::close()" << endl;
    ltracein(110);

    if ( !_session ) throw Error ( openSessionError, "Session::Close()" );

    size_t count = _session->_preDestroy ();

    delete _session;
    _session = NULL;

    ltraceout(110);

    return count;
  }


  bool  Session::getDemoMode ()
  { return get("getDemoMode()")->_katabatic->getDemoMode(); }


  float  Session::getSaturateRatio ()
  { return get("getSaturateRatio()")->_katabatic->getSaturateRatio(); }


  size_t  Session::getSaturateRp ()
  { return get("getSaturateRp()")->_katabatic->getSaturateRp(); }


  bool  Session::getWarnGCellOverload ()
  { return get("getWarnGCellOverload()")->_katabatic->getWarnGCellOverload(); }


  void  Session::setWarnGCellOverload ( bool state )
  { get("getWarnGCellOverload()")->_katabatic->setWarnGCellOverload(state); }


  DbU::Unit  Session::getExtensionCap ()
  { return getConfiguration()->getExtensionCap(); }


  const Layer* Session::getRoutingLayer ( size_t depth )
  { return getConfiguration()->getRoutingLayer(depth); }


  const Layer* Session::getContactLayer ( size_t depth )
  { return getConfiguration()->getContactLayer(depth); }


  void  Session::link ( AutoContact* autoContact )
  { return get("link(AutoContact*)")->_katabatic->_link ( autoContact ); }


  void  Session::link ( AutoSegment* autoSegment )
  { return get("link(AutoSegment*)")->_katabatic->_link ( autoSegment ); }


  void  Session::unlink ( AutoContact* autoContact )
  { return get("unlink(AutoContact*)")->_katabatic->_unlink ( autoContact ); }


  void  Session::unlink ( AutoSegment* autoSegment )
  { return get("unlink(AutoSegment*)")->_katabatic->_unlink ( autoSegment ); }


  AutoContact* Session::lookup ( Contact* contact )
  { return get("lookup(Contact*)")->_katabatic->_lookup ( contact ); }


  AutoSegment* Session::lookup ( Segment* segment )
  { return get("lookup(Segment*)")->_katabatic->_lookup ( segment ); }


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
    record->add ( getSlot ( "_autoSegments", &_autoSegments ) );
    return record;
  }


} // End of Katabatic namespace.
