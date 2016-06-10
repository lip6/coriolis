// -*- mode: C++; explicit-buffer-name: "Dijkstra.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./anabatic/Dijkstra.cpp"                       |
// +-----------------------------------------------------------------+


#include <limits>
#include "hurricane/Error.h"
#include "hurricane/Net.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/DebugSession.h"
#include "anabatic/AnabaticEngine.h"
#include "anabatic/Dijkstra.h"


namespace Anabatic {

  using std::cerr;
  using std::endl;
  using std::numeric_limits;
  using Hurricane::ForEachIterator;
  using Hurricane::Error;
  using Hurricane::Component;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Hurricane::UpdateSession;
  using Hurricane::DebugSession;


// -------------------------------------------------------------------
// Class  :  "Anabatic::Vertex".


  float  Vertex::unreached = numeric_limits<float>::max();


  bool  Vertex::hasValidStamp () const
  { return _stamp == getAnabatic()->getStamp(); }


  string  Vertex::_getString () const
  {
    if (not _gcell) {
      string s = "<Vertex [key] " + getString(_id) + ">";
      return s;
    }

    string s = "<Vertex " + getString(_id)
             + " @(" + DbU::getValueString(_gcell->getXMin())
             +  "," + DbU::getValueString(_gcell->getYMin()) + ")"
             + " connexId:" + getString(_connexId)
             + " d:" + ((_distance == unreached) ? "unreached" : getString(_distance) )
             + " stamp:" + (hasValidStamp() ? "valid" : "outdated")
             + " from:" + ((_from) ? "set" : "NULL")
             + ">";
    return s;
  }


  void  Vertex::notify ( Vertex* vertex, unsigned int flags )
  {
  //Vertex* vertex = getOwner();
    cdebug.log(111) << "Vertex::notify() " << vertex << endl;
  // Take into account the GCell modification here.
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::Dijkstra".


  Dijkstra::Mode::~Mode ()
  { }


  string  Dijkstra::Mode::_getTypeName () const
  { return "Anabatic::Dijkstra::Mode"; }


  string Dijkstra::Mode::_getString () const
  {
    string s = "";
    s += (_flags & Standart ) ? 'S' : '-';
    s += (_flags & Monotonic) ? 'M' : '-';

    return s;
  }


  float  Dijkstra::getDistance ( const Vertex* a, const Vertex* b, const Edge* e )
  {
    float distance = a->getDistance() + e->getDistance();

    Edge* aFrom = a->getFrom();
    if (aFrom) {
      distance += (aFrom->isHorizontal() xor e->isHorizontal()) ? 3.0 : 0.0;
    }
    return distance;
  }


  Dijkstra::Dijkstra ( AnabaticEngine* anabatic )
    : _anabatic    (anabatic)
    , _vertexes    ()
    , _mode        (Mode::Standart)
    , _net         (NULL)
    , _stamp       (-1)
    , _sources     ()
    , _targets     ()
    , _searchArea  ()
    , _connectedsId(-1)
    , _queue       ()
  {
    const vector<GCell*>& gcells = _anabatic->getGCells();
    for ( GCell* gcell : gcells ) {
      _vertexes.push_back( new Vertex (gcell) );
    }
  }


  Dijkstra::~Dijkstra ()
  {
    for ( Vertex* vertex : _vertexes ) delete vertex;
  }


  void  Dijkstra::load ( Net* net )
  {
    _net = net;

    DebugSession::open( _net, 110, 120 );
    cdebug.log(111,1) << "Dijkstra::load() " << _net << endl;

    _sources.clear();
    _targets.clear();
    _searchArea.makeEmpty();
    _stamp = _anabatic->incStamp();

    vector<RoutingPad*> rps;
    for ( RoutingPad* rp : _net->getRoutingPads() ) rps.push_back( rp );
    for ( RoutingPad* rp : rps ) {
      Box    rpBb   = rp->getBoundingBox();
      Point  center = rpBb.getCenter();
      GCell* gcell  = _anabatic->getGCellUnder( center );
        
      if (not gcell) {
        cerr << Error( "Dijkstra::load(): %s of %s is not under any GCell.\n"
                       "        It will be ignored ans the routing will be incomplete."
                     , getString(rp  ).c_str()
                     , getString(_net).c_str()
                     ) << endl;
        continue;
      }

      _searchArea.merge( gcell->getBoundingBox() );

      Vertex* vertex = gcell->getObserver<Vertex>(GCell::Observable::Vertex);
      if (vertex->getConnexId() < 0) {
        vertex->setStamp   ( _stamp );
        vertex->setConnexId( _targets.size() );
        vertex->setFrom    ( NULL );
        _targets.insert( vertex );
        cdebug.log(111) << "Add Vertex: " << vertex << endl;
      }

      Contact* gcontact = vertex->getGContact( _net );
      rp->getBodyHook()->detach();
      rp->getBodyHook()->attach( gcontact->getBodyHook() );
    }

    cdebug.log(111) << "Search area: " << _searchArea << endl;
    cdebug.tabw(111,-1);
    DebugSession::close();
  } 


  void  Dijkstra::_selectFirstSource ()
  {
    if (_targets.empty()) {
      cerr << Error( "Dijkstra::_selectFirstSource(): %s has no vertexes to route, ignored."
                   , getString(_net).c_str()
                   ) << endl;
      return;
    }

    Vertex* firstSource = NULL;

    if (_mode & Mode::Monotonic) {
      if (_targets.size() == 2) {
        auto ivertex = _targets.begin();
        Vertex* v1 = *ivertex;
        Vertex* v2 = *(++ivertex);

        firstSource = (v1->getCenter().getX() <= v2->getCenter().getY()) ? v1 : v2;
      } else {
        cerr << Error( "Dijkstra::_selectFirstSource(): %s cannot be routed in monotonic mode.\n"
                       "        Must have exactly two terminals (%u), revert to Standart."
                     , getString(_net).c_str()
                     , _targets.size()
                     ) << endl;
        _mode = Mode::Standart;
      }
    }

    if (not firstSource) {
    // Standart routing.
      Point      areaCenter  = _searchArea.getCenter();
      auto       ivertex     = _targets.begin();

      firstSource = *ivertex++;
      DbU::Unit  minDistance = areaCenter.manhattanDistance( firstSource->getCenter() );

      for ( ; ivertex != _targets.end() ; ++ivertex ) {
        DbU::Unit distance = areaCenter.manhattanDistance( (*ivertex)->getCenter() );
        if (distance < minDistance) {
          minDistance = distance;
          firstSource = *ivertex;
        }
      }
    }

    _targets.erase ( firstSource );
    _sources.insert( firstSource );

    cdebug.log(111) << "Dijkstra::_selectFirstSource() " << *_sources.begin() << endl;
  }


  bool  Dijkstra::_propagate ( Flags enabledSides )
  {
    cdebug.log(111,1) << "Dijkstra::_propagate() " << _net << endl;

    while ( not _queue.empty() ) {
      _queue.dump();

      Vertex* current = _queue.top();
      _queue.pop();

      if ((current->getConnexId() == _connectedsId) or (current->getConnexId() < 0)) {
        for ( Edge* edge : current->getGCell()->getEdges() ) {

          if (edge == current->getFrom()) continue;
          
          GCell*  gneighbor = edge->getOpposite(current->getGCell());
          Vertex* vneighbor = gneighbor->getObserver<Vertex>(GCell::Observable::Vertex);

          if (not _searchArea.contains(vneighbor->getCenter())) continue;

          cdebug.log(111) << "| Edge " << edge << endl;
          cdebug.log(111) << "+ Neighbor: " << vneighbor << endl;

          float distance = getDistance( current, vneighbor, edge );

          if (vneighbor->getConnexId() == _connectedsId) continue;
          if (vneighbor->getConnexId() >= 0) {
            vneighbor->setFrom    ( edge );
            vneighbor->setDistance( distance );
            cdebug.log(111) << "Push (before): (size:" << _queue.size() << ")" << endl;
            _queue.push( vneighbor );

            cdebug.log(111) << "Push (target): (size:" << _queue.size() << ") " << vneighbor << endl;
            continue;
          }

          if (distance < vneighbor->getDistance()) {
            cdebug.log(111) << "Push (before erase): (size:" << _queue.size() << ") " << vneighbor << endl;
            if (vneighbor->getDistance() != Vertex::unreached) {
              _queue.erase( vneighbor );
              _queue.dump();
            } else {
              vneighbor->setStamp   ( _stamp );
              vneighbor->setConnexId( -1 );
            }
            cdebug.log(111) << "Push (after erase): (size:" << _queue.size() << ")" << endl;

            vneighbor->setDistance( distance );
            vneighbor->setFrom    ( edge );
            _queue.push( vneighbor );

            cdebug.log(111) << "Push: (size:" << _queue.size() << ") " << vneighbor << endl;
          }
        }

        continue;
      }

    // We did reach another target (different <connexId>).
    // Tag back the path.
      cdebug.log(111) << "Trace back" << endl;
      _targets.erase( current );
      while ( current ) {
        cdebug.log(111) << "| " << current << endl;
        if (current->getConnexId() == _connectedsId) break;

        _sources.insert( current );
        current->setDistance( 0.0 );
        current->setConnexId( _connectedsId );
        _queue.push( current );

        current = current->getPredecessor();
      }

      cdebug.tabw(111,-1);
      return true;
    }

    cerr << Error( "Dijkstra::propagate(): %s has unreachable targets."
                 , getString(_net).c_str()
                 ) << endl;

    cdebug.tabw(111,-1);
    return false;
  }


  void  Dijkstra::run ( Dijkstra::Mode mode )
  {
    DebugSession::open( _net, 110, 120 );

    cdebug.log(111,1) << "Dijkstra::run() on " << _net << " mode:" << mode << endl;
    _mode = mode;

    _selectFirstSource();
    if (_sources.empty()) {
      cdebug.log(111) << "No source to start, not routed." << endl;
      cdebug.tabw(111,-1);
      return;
    }

    Flags enabledEdges = Flags::AllSides;
    if (_mode & Mode::Monotonic) {
      if ((*_sources.begin())->getCenter().getY() <= (*_targets.begin())->getCenter().getY())
        enabledEdges = Flags::EastSide | Flags::NorthSide;
      else
        enabledEdges = Flags::EastSide | Flags::SouthSide;
    }

    Vertex* source = *_sources.begin();
    _queue.clear();
    _queue.push( source );
    _connectedsId = source->getConnexId();
    source->setDistance( 0.0 );

    cdebug.log(111) << "Push source: (size:" << _queue.size() << ") "
                    << source
                    << " _connectedsId:" << _connectedsId << endl;

    while ( not _targets.empty() and _propagate(enabledEdges) );

    _toWires();
    _queue.clear();

    cdebug.tabw(111,-1);
    DebugSession::close();
  }


  void  Dijkstra::_toWires ()
  {
    cdebug.log(111,1) << "Dijkstra::toWires() " << _net << endl;

    for ( Vertex* vertex : _sources ) {
      Edge* from = vertex->getFrom();
      if (not from) continue;

      cdebug.log(111) << "| " << vertex << endl;

      from->incRealOccupancy( 1 );

      Vertex* source = vertex;
      Vertex* target = source->getPredecessor();

      if (  (source->getGCell()->getXMin() > target->getGCell()->getXMin())
         or (source->getGCell()->getYMin() > target->getGCell()->getYMin()) )
        std::swap( source, target );

      Contact* sourceContact = source->getGContact( _net );
      Contact* targetContact = target->getGContact( _net );

      if (from->isHorizontal()) {
        Horizontal::create( sourceContact
                          , targetContact
                          , _anabatic->getConfiguration()->getGHorizontalLayer()
                          , from->getAxis()
                          , DbU::fromLambda(2.0)
                          );
      } else {
        Vertical::create( sourceContact
                        , targetContact
                        , _anabatic->getConfiguration()->getGVerticalLayer()
                        , from->getAxis()
                        , DbU::fromLambda(2.0)
                        );
      }
    }

    cdebug.tabw(111,-1);
  }


}  // Anabatic namespace.
