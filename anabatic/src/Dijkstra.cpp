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
#include "anabatic/AnabaticEngine.h"
#include "anabatic/Dijkstra.h"


namespace Anabatic {

  using std::cerr;
  using std::endl;
  using std::numeric_limits;
  using Hurricane::Error;
  using Hurricane::Component;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Hurricane::UpdateSession;


// -------------------------------------------------------------------
// Class  :  "Anabatic::Vertex".


  float  Vertex::unreached = numeric_limits<float>::max();


  bool  Vertex::hasValidStamp () const
  { return _stamp == getAnabatic()->getStamp(); }


// -------------------------------------------------------------------
// Class  :  "Anabatic::Dijkstra".


  Dijkstra::Dijkstra ( AnabaticEngine* anabatic )
    : _anabatic    (anabatic)
    , _vertexes    ()
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
    UpdateSession::open();

    _searchArea.makeEmpty();
    _stamp = _anabatic->incStamp();

    for ( Component* component : net->getRoutingPads() ) {
      RoutingPad* rp = dynamic_cast<RoutingPad*>( component );
      if (rp) {
        Box    rpBb   = rp->getBoundingBox();
        Point  center = rpBb.getCenter();
        GCell* gcell  = _anabatic->getGCellUnder( center );
        
        if (not gcell) {
          cerr << Error( "Dijkstra::load(): %s of %s is not under any GCell.\n"
                         "        It will be ignored ans the routing will be incomplete."
                       , getString(rp ).c_str()
                       , getString(net).c_str()
                       ) << endl;
          continue;
        }

        _searchArea.merge( rpBb );

        Vertex* vertex = gcell->lookup<Vertex>();
        if (vertex->getConnexId() < 0) {
          vertex->setStamp   ( _stamp );
          vertex->setConnexId( _targets.size() );
          vertex->getGContact( _net );
          _targets.insert( vertex );
        }
      }
    }

    UpdateSession::close();
  } 


  void  Dijkstra::selectFirstSource ()
  {
    if (_targets.empty()) {
      cerr << Error( "Dijkstra::selectFirstSource(): %s has no vertexes to route, ignored.\n"
                   , getString(_net).c_str()
                   ) << endl;
      return;
    }
    Point      areaCenter  = _searchArea.getCenter();
    auto       ivertex     = _targets.begin();
    Vertex*    firstSource = *ivertex++;
    DbU::Unit  minDistance = areaCenter.manhattanDistance( firstSource->getCenter() );

    for ( ; ivertex != _targets.end() ; ++ivertex ) {
      DbU::Unit distance = areaCenter.manhattanDistance( (*ivertex)->getCenter() );
      if (distance < minDistance) {
        minDistance = distance;
        firstSource = *ivertex;
      }
    }

    _targets.erase( firstSource );
  }


  void  Dijkstra::propagate ()
  {
    while ( not _queue.empty() ) {
      Vertex* current = _queue.top();
      _queue.pop();

      if ((current->getConnexId() == _connectedsId) or (current->getConnexId() < 0)) {
        for ( Edge* edge : current->getGCell()->getEdges() ) {
          if (edge == current->getFrom()) continue;
          
          GCell*  gneighbor = edge->getOpposite(current->getGCell());
          Vertex* vneighbor = gneighbor->lookup<Vertex>();

          if (vneighbor->getConnexId() == _connectedsId) continue;

          float distance = current->getDistance() + edge->getDistance();
          if (distance < current->getDistance()) {
            if (vneighbor->getDistance() != Vertex::unreached) _queue.erase( vneighbor );
            else                                               vneighbor->setStamp( _stamp );

            vneighbor->setDistance( distance );
            _queue.push( vneighbor );
          }
        }
      }

    // We did reach another target (different <connexId>).
    // Tag back the path.
      _targets.erase( current );
      while ( current ) {
        _sources.insert( current );
        current->setDistance( 0.0 );
        current->setConnexId( _connectedsId );
        current = current->getPredecessor();
      }

      break;
    }
  }


  void  Dijkstra::run ()
  {
    if (_sources.empty()) return;

    UpdateSession::open();

    _queue.clear();
    _queue.push( *_sources.begin() );
    _connectedsId = (*_sources.begin())->getConnexId();

    while ( not _targets.empty() ) propagate();

    toWires();

    UpdateSession::close();
  }


  void  Dijkstra::toWires ()
  {
    for ( Vertex* vertex : _sources ) {
      Edge* from = vertex->getFrom();
      if (not from) continue;

      Vertex* source = vertex;
      Vertex* target = source->getPredecessor();

      if (  (source->getGCell()->getXMin() > target->getGCell()->getXMin())
         or (source->getGCell()->getYMin() > target->getGCell()->getYMin()) )
        std::swap( source, target );

      Contact* sourceContact = source->getGContact( _net );
      Contact* targetContact = source->getGContact( _net );

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
  }


}  // Anabatic namespace.
