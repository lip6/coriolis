
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./GCell.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <limits>
#include  <sstream>
#include  <algorithm>

#include  "hurricane/DebugSession.h"
#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "hurricane/Component.h"
#include  "hurricane/RoutingPad.h"
#include  "katabatic/AutoSegment.h"
#include  "katabatic/AutoContact.h"
#include  "kite/DataNegociate.h"
#include  "kite/TrackSegment.h"
#include  "kite/RoutingPlane.h"
#include  "kite/GCell.h"
#include  "kite/GCellGrid.h"
#include  "kite/NegociateWindow.h"


namespace {

  using namespace Hurricane;
  using namespace Kite;


  TrackElement* getCandidate ( RoutingPad* rp, Box bb )
  {
    size_t depth = Session::getConfiguration()->getLayerDepth ( rp->getLayer() );
    if ( depth > 2 ) return NULL;

    DebugSession::open ( rp->getNet() );

  //if ( (depth != 0) and (depth != 2) ) return NULL;

    ltrace(200) << "depth: " << depth << " " << rp->getSourcePosition() << " " << rp << endl;

    if ( depth%2 == 0 ) {
    // Propagate from vertical Terminals.
      forEach ( Contact*, icontact, rp->getSlaveComponents().getSubSet<Contact*>() ) {
        if ( not bb.contains(icontact->getCenter()) ) continue;

        forEach ( Segment*, isegment, icontact->getSlaveComponents().getSubSet<Segment*>() ) {
          TrackElement* autoSegment = Session::lookup(*isegment);
          if ( not autoSegment or autoSegment->isFixed() ) continue;
          
          DebugSession::close ();
          return autoSegment;
        }
      }
    } else {
#if DISABLED
    // Propage from Horizontal Terminals.
      bool         hasM3protect = false;
      AutoContact* toContact    = NULL;

      forEach ( Contact*, icontact, rp->getSlaveComponents().getSubSet<Contact*>() ) {
        if ( not bb.contains(icontact->getCenter()) ) continue;

        forEach ( Segment*, isegment, icontact->getSlaveComponents().getSubSet<Segment*>() ) {
          TrackElement* autoSegment = Session::lookup(*isegment);
          if ( not autoSegment ) continue;
          if ( autoSegment->isFixed() ) {
            if ( Session::getConfiguration()->getLayerDepth(autoSegment->getLayer()) == 2 ) {
              ltrace(200) << "M3 protection found for " << rp << endl;
              hasM3protect = true;
            }
            continue;
          } 
          toContact = autoSegment->base()->getAutoSource();
          if ( toContact->base() == *icontact )
            toContact = autoSegment->base()->getAutoTarget();
        }
      }

    // Find M3 associated to this terminal.
      if ( hasM3protect ) {
        if ( toContact ) {
          ltrace(200) << "toContact: " << toContact << endl;
          forEach ( Segment*, isegment, toContact->base()->getSlaveComponents().getSubSet<Segment*>() ) {
            ltrace(200) << "| slave: " << *isegment << endl;
            if ( Session::getConfiguration()->getLayerDepth(isegment->getLayer()) == 2 ) {
              TrackElement* autoSegment = Session::lookup(*isegment);
              ltrace(200) << "M3 candidate: " << autoSegment << endl;
              DebugSession::close ();
              return autoSegment;
            }
          }
        }
      }
#endif
    }

    DebugSession::close ();
    return NULL;
  }


}


namespace Kite {


  using std::cerr;
  using std::endl;
  using std::swap;
  using std::numeric_limits;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::roundfp;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::ForEachIterator;
  using Hurricane::Component;
  using Hurricane::RoutingPad;

  using Katabatic::AutoContact;


// -------------------------------------------------------------------
// Class  :  "Kite::GCell::CompareByDensity".
//
// lhs < rhs --> true


  bool  GCell::CompareByDensity::operator() ( GCell* lhs, GCell* rhs )
  {
  //int difference = floatCompare ( lhs->base()->getDensity(), rhs->base()->getDensity() );
  //if ( abs(difference) > 1000 ) return difference > 0;

    float difference = roundfp ( lhs->base()->getDensity() - rhs->base()->getDensity() );
    if ( difference != 0.0 ) return (difference > 0.0);

    if ( lhs->getIndex() < rhs->getIndex() ) return true;
    return false;
  }


// -------------------------------------------------------------------
// Class  :  "Kite::GCell::CompareByStiffness".
//
// lhs < rhs --> true


  bool  GCell::CompareByStiffness::operator() ( GCell* lhs, GCell* rhs )
  {
    if ( lhs->isRouted() xor rhs->isRouted() ) return rhs->isRouted();

    float difference = roundfp ( lhs->getStiffness() - rhs->getStiffness() );
    if ( difference != 0.0 ) return (difference > 0.0);

    return lhs->getIndex() < rhs->getIndex();
  }


// -------------------------------------------------------------------
// Class  :  "Kite::GCell".


  GCell::GCell ( GCellGrid* grid, Katabatic::GCell* gcell )
    : _gcellGrid       (grid)
    , _base            (gcell)
    , _segments        ()
    , _order           (numeric_limits<unsigned int>::max())
    , _isInRoutingSet  (false)
    , _isRouted        (false)
  {
    if ( !gcell )
      cerr << Bug("GCell:GCell() - NULL base.") << endl;

    _leftSegments [0] = NULL;
    _leftSegments [1] = NULL;
    _rightSegments[0] = NULL;
    _rightSegments[1] = NULL;
  }


  GCell* GCell::create ( GCellGrid* grid, Katabatic::GCell* gcell )
  {
    GCell* kiteGCell = new GCell ( grid, gcell );

    ltrace(90) << "Kite::GCell::create() - " << (void*)kiteGCell << " " << kiteGCell << endl;

    return kiteGCell;
  }


  GCell::~GCell ()
  { }


  void  GCell::destroy ()
  {
    ltrace(90) << "Kite::GCell::destroy() - " << (void*)this << " " << this << endl;
    ltracein(90);

    for ( size_t i=0 ; i < _segments.size() ; i++ ) {
      if ( !_segments[i]->getTrack() )
        _segments[i]->destroy ();
      else {
        cerr << Error("%s still bound to a track!\n"
                      "        (not deleted: let's get the memory leak)"
                     ,getString(_segments[i]).c_str()
                     ) << endl;
      }
    }
    delete this;

    ltraceout(90);
  }


  GCell* GCell::getLeft () const
  {
    return _gcellGrid->getGCellLeft(this);
  }


  GCell* GCell::getRight () const
  {
    return _gcellGrid->getGCellRight(this);
  }


  GCell* GCell::getUp () const
  {
    return _gcellGrid->getGCellUp(this);
  }


  GCell* GCell::getDown () const
  {
    return _gcellGrid->getGCellDown(this);
  }


  bool  GCell::areDensityConnex ( GCell* a, GCell* b )
  { return Katabatic::GCell::areDensityConnex ( a->base(), b->base() ); }


  GCell* GCell::getLowestOrder ( TrackElement* trackSegment )
  {
    vector<GCell*>  gcells;
    trackSegment->getGCells ( gcells );

    if ( gcells.empty() ) return NULL;

    size_t ilower = 0;
    for ( size_t i=0 ; i<gcells.size() ; i++ )
      if ( gcells[i]->getOrder() < gcells[ilower]->getOrder() )
        ilower = i;

    return gcells[ilower];
  }


  void  GCell::loadRouting ( unsigned int order )
  {
    _segments.clear ();
    _order = order;

    ltrace(200) << "GCell::loadRouting() - " << this << " Session:" << Session::getOrder() << endl;
    ltracein(200);

    Segment*     segment;
    AutoSegment* autoSegment;

    ltrace(149) << "AutoSegments from AutoContacts" << endl;
    vector<AutoContact*>* contacts = getContacts();
    for ( size_t i=0 ; i<contacts->size() ; i++ ) {
      forEach ( Component*, component, (*contacts)[i]->getSlaveComponents() ) {
        segment      = dynamic_cast<Segment*>(*component);
        autoSegment  = Session::base()->lookup ( segment );
        ltrace(149) << autoSegment << endl;
        if ( autoSegment ) {
          addTrackSegment ( this, autoSegment, true );
        }
      }
    }

  // // Effective deletion of rejecteds.
  //   set<AutoSegment*>::iterator ireject = rejecteds.begin();
  //   for ( ; ireject != rejecteds.end() ; ireject++ ) {
  //   }

    ltrace(149) << "Horizontal overcell AutoSegments" << endl;
    vector<AutoSegment*>* segments = getHSegments();
    for ( size_t i=0 ; i<segments->size() ; i++ ) {
      addTrackSegment ( this, (*segments)[i], true );
    }

    ltrace(149) << "Vertical overcell AutoSegments" << endl;
    segments = getVSegments();
    for ( size_t i=0 ; i<segments->size() ; i++ ) {
      addTrackSegment ( this, (*segments)[i], true );
    }

    ltrace(149) << "_segments.size():" << _segments.size() << endl;
    ltraceout(200);
  }


  void  GCell::computeBorder ()
  {
    ltrace(200) << "GCell::computeBorder() " << this << endl;
    ltracein(200);

    vector<AutoContact*>* contacts = _base->getContacts();
    TrackElement*         candidate;
    DataNegociate*        data;
    Box                   bb       = getBoundingBox();
    size_t                iLeft    = 0;
    size_t                iRight   = 0;

    for ( size_t icontact=0 ; icontact < contacts->size() ; icontact++ ) {
      Component*  anchor = (*contacts)[icontact]->getAnchor();
      RoutingPad* rp     = dynamic_cast<RoutingPad*>(anchor);

      if ( not rp ) continue;

      // size_t depth = Session::getConfiguration()->getLayerDepth ( rp->getLayer() );
      // if ( (depth != 0) and (depth != 2) ) continue;

      // ltrace(200) << "depth: " << depth << " " << rp << endl;

      candidate = getCandidate ( rp, bb );
      if ( not candidate ) continue;

      data = candidate->getDataNegociate();
      if ( not data ) continue;

    // Ugly: hardwired pitch usage.
      if ( (rp->getX() - getX() < DbU::lambda(11.0)) and (iLeft < 2) ) {
      //data->setLeftBorder(true);
        ltrace(200) << "Left Segments[" << iLeft << "]: " << candidate << endl;
        _leftSegments[iLeft++] = candidate;
        continue;
      }

      if ( (getXMax() - rp->getX() < DbU::lambda(11.0)) and (iRight < 2) ) {
      //data->setRightBorder(true);
        ltrace(200) << "Right Segment[" << iRight << "]: " << candidate << endl;
        _rightSegments[iRight++] = candidate;
      }
    }

    ltraceout(200);
  }


  TrackElement* GCell::addTrackSegment ( GCell* gcell, AutoSegment* autoSegment, bool loading )
  {
    ltrace(200) << "GCell::addTrackSegment() - " << autoSegment << endl;
    ltracein(159);

  // Special case: fixed AutoSegments must not interfere with blockages.
  // Ugly: uses of getExtensionCap().
    if ( autoSegment->isFixed() ) {
      RoutingPlane* plane = Session::getKiteEngine()->getRoutingPlaneByLayer(autoSegment->getLayer());
      Track*        track = plane->getTrackByPosition ( autoSegment->getAxis() );
      size_t        begin;
      size_t        end;
      Interval      fixedSpan;
      Interval      blockageSpan;

      autoSegment->getCanonical ( fixedSpan );
      fixedSpan.inflate ( Session::getExtensionCap() );

      track->getOverlapBounds ( fixedSpan, begin, end );
      for ( ; (begin < end) ; begin++ ) {

        TrackElement* other = track->getSegment(begin);
        ltrace(200) << "| overlap: " << other << endl;

        if ( not other->isBlockage() ) continue;

        other->getCanonical ( blockageSpan );
        blockageSpan.inflate(Session::getExtensionCap());

        ltrace(200) << "  fixed:" << fixedSpan << " vs. blockage:" << blockageSpan << endl;

        if ( not fixedSpan.intersect(blockageSpan) ) continue;

      // Overlap between fixed & blockage.
        ltrace(200) << "* Blockage overlap: " << autoSegment << endl;
        Session::destroyRequest ( autoSegment );
        return NULL;
      }
    }

    Interval span;
    autoSegment = autoSegment->getCanonical ( span );

    bool           created;
    TrackElement*  trackSegment  = TrackSegment::create ( autoSegment, NULL, created );
    DataNegociate* data          = trackSegment->getDataNegociate ();
    GCell*         previousGCell = trackSegment->getGCell();

    if ( not loading )
      ltrace(159) << "* lookup: " << autoSegment << endl;

    if ( created )
      ltrace(159) << "* " << trackSegment << endl;

    if ( not created and not loading ) {
      ltrace(200) << "TrackSegment already exists (and not in loading stage)." << endl;
      ltrace(200) << "Previous owning GCell: " << previousGCell << endl;
      if ( previousGCell != gcell ) {
        previousGCell->removeTrackSegment ( trackSegment );
        trackSegment->setGCell ( NULL );
      }
    }

    vector<GCell*> gcells;
    trackSegment->getGCells ( gcells );
    GCell* lowest = gcells[0];
    bool validPrevious = false;

    for ( size_t igcell=0 ; igcell<gcells.size() ; igcell++ ) {
      ltrace(200) << "| " << igcell << ":" << gcells[igcell] << endl;
      if ( gcells[igcell]->getOrder() < lowest->getOrder()) {
        lowest = gcells[igcell];
      }
      if ( gcells[igcell] == previousGCell ) validPrevious = true;
    }
    if ( not validPrevious ) previousGCell = NULL;

    if ( not gcell ) {
      gcell = lowest;
      if ( previousGCell and (gcell->getOrder() == previousGCell->getOrder()) )
        gcell = previousGCell;
      ltrace(200) << "New owner: " << gcell << endl;
    } else if ( created ) {
      if ( (lowest != gcell) && (lowest->getOrder() != gcell->getOrder() ) ) {
        cerr << Bug("Not the right lowest: %s",getString(lowest).c_str()) << endl;
      }
    }

    if ( (gcell->getOrder() > Session::getOrder()) and not (data->isRing() or data->isBorder())  ) {
      cinfo << "[INFO] GCell::addTrackSegment() - Owning GCell is not in active set ("
            << gcell->getOrder() << " > Session:" << Session::getOrder() << ")."
            << "\n       " << trackSegment
            << endl;
    }

    if ( not trackSegment->getGCell() ) {
      trackSegment->setGCell ( gcell );
      gcell->addTrackSegment ( trackSegment );

      if ( loading ) {
        RoutingPlane* plane = Session::getKiteEngine()->getRoutingPlaneByLayer(autoSegment->getLayer());
        Track*        track = plane->getTrackByPosition ( autoSegment->getAxis() );
        Interval      uside = gcell->getUSide ( Constant::perpandicular(autoSegment->getDirection()), false );

        if ( track->getAxis() > uside.getVMax() ) track = track->getPrevious();
        if ( track->getAxis() < uside.getVMin() ) track = track->getNext();

        trackSegment->setAxis ( track->getAxis(), Katabatic::Realignate|Katabatic::AxisSet );
        trackSegment->invalidate ();

        if ( created and trackSegment->isFixed() ) {
          Session::addInsertEvent ( trackSegment, track );
        }
      }
    }

    ltraceout(159);

    return trackSegment;
  }


  void  GCell::addTrackSegment ( TrackElement* trackSegment )
  {
    _segments.push_back ( trackSegment );
  }


  void  GCell::removeTrackSegment ( TrackElement* trackSegment )
  {
    if ( _segments.empty() ) return;

    size_t i = 0;
    for ( ; i<_segments.size() ; i++ )
      if ( _segments[i] == trackSegment ) {
        swap ( _segments[i], _segments[_segments.size()-1] );
        break;
      }

    if ( i < _segments.size() ) _segments.pop_back ();
  }


  double  GCell::getOwnedWireLength () const
  {
    double ownedWL = 0;

    for ( size_t i=0 ; i<_segments.size() ; i++ ) {
      ownedWL += DbU::getLambda ( _segments[i]->getLength() );
    }

    return ownedWL;
  }


  void  GCell::anticipateRouting ( unsigned int order )
  {
    if ( order < _order ) {
      setRouted ( true );
      loadRouting ( order );
      // _order = order;
      // for ( size_t isegment=0 ; isegment<_segments.size() ; ++isegment ) {
      //   DataNegociate* data = _segments[isegment]->getDataNegociate();
      //   data->setGCellOrder ( order );
      //   data->resetBorder ();
      // }
    }
  }


  string  GCell::_getTypeName() const
  { return "Kite::GCell"; }


  string  GCell::_getString () const
  {
    string s = _base->_getString();
    s.erase ( s.size()-1 );
    s += " o:";
    s += ((_order == numeric_limits<unsigned int>::max()) ? "-" : getString(_order));
    s += " ";
    s += ((_isRouted) ? "R" : "-");
    s += ">";
    return s;
  }


  Record* GCell::_getRecord () const
  { return _base->_getRecord(); }


// -------------------------------------------------------------------
// Class  :  "Kite::DyKeyQueue".


  DyKeyQueue::DyKeyQueue ()
    : _map     ()
    , _requests()
  { }


  DyKeyQueue::~DyKeyQueue ()
  {
    if ( (not _map.empty()) or (not _requests.empty()) ) {
      cerr << Warning("~DyKeyQueue(): Still contains %d elements and %d requests."
                     ,_map.size(),_requests.size()) << endl;
    }
  }
  


} // End of Kite namespace.
