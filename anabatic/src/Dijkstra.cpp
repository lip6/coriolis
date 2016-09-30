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
#include "crlcore/Utilities.h"
#include "anabatic/AnabaticEngine.h"
#include "anabatic/Dijkstra.h"


namespace Anabatic {

  using std::cerr;
  using std::endl;
  using std::numeric_limits;
  using Hurricane::ForEachIterator;
  using Hurricane::Error;
  using Hurricane::Component;
  using Hurricane::Segment;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Hurricane::UpdateSession;
  using Hurricane::DebugSession;


// -------------------------------------------------------------------
// Class  :  "Anabatic::Vertex".


  DbU::Unit  Vertex::unreached    = std::numeric_limits<long>::max();
  DbU::Unit  Vertex::unreachable  = std::numeric_limits<long>::max()-1;


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
             +  "-" + DbU::getValueString(_gcell->getYMin()) 
             +  "-" + DbU::getValueString(_gcell->getXMax())
             +  "-" + DbU::getValueString(_gcell->getYMax()) + ")"
             + " rps:" +  getString(_rpCount)
             + " deg:" +  getString(_degree)
             + " connexId:" + ((_connexId >= 0) ? getString(_connexId) : "None")
             + " d:" + ((_distance == unreached) ? "unreached"
                                                 : ((_distance == unreachable) ? "unreachable"
                                                                               : DbU::getValueString(_distance)) )
             +   "+" + getString(_branchId)
             + " stamp:" + (hasValidStamp() ? "valid" : "outdated")
             + " from:" + ((_from) ? "set" : "NULL")
             + " restricted:" + (isNRestricted() ? "N" : "-")
             + (isSRestricted() ? "S" : "-")
             + (isERestricted() ? "E" : "-")
             + (isWRestricted() ? "W" : "-")
             + ">";
    return s;
  }


  void  Vertex::notify ( Vertex* vertex, unsigned int flags )
  {
    cdebug_log(111,0) << "Vertex::notify() " << vertex << endl;
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


  DbU::Unit  Dijkstra::_distance ( const Vertex* a, const Vertex* b, const Edge* e )
  {
    cdebug_log(112,0) << "Calcul _distance "<< endl;
    cdebug_log(112,0) << "a: "<< b->hasRestrictions() << ", " << a <<  endl;
    cdebug_log(112,0) << "b: "<< b->hasRestrictions() << ", " << b <<  endl;
    DbU::Unit distance = a->getDistance() + e->getDistance();

    if ( (a->hasRestrictions()) || (b->hasRestrictions()) ) { 
      if (isRestricted(a, b)) {
        distance = Vertex::unreachable;
      }
    }
    // Edge* aFrom = a->getFrom();
    // if (aFrom) {
    //   distance += (aFrom->isHorizontal() xor e->isHorizontal()) ? 3.0 : 0.0;
    // }
    return distance;
  }


  bool Dijkstra::isRestricted ( const Vertex* v1, const Vertex* v2 )
  {
    bool   restricted = true;
    GCell* c1         = v1->getGCell();
    GCell* c2         = v2->getGCell();
    
  // Check from GCell 1 
    if        ( c1->isNorth(c2) ) {
      if ( !v1->isNRestricted() ) restricted = false;
    } else if ( c1->isSouth(c2) ) {
      if ( !v1->isSRestricted() ) restricted = false;
    } else if ( c1->isEast (c2) ) {
      if ( !v1->isERestricted() ) restricted = false;
    } else if ( c1->isWest (c2) ) {
      if ( !v1->isWRestricted() ) restricted = false;
    } else {
      cerr << Error( "GCells are not side by side." ) << endl;
      return true;
    }
    
    if   (restricted) return true;
    else {
    // Check from GCell 2 
      if        ( c2->isNorth(c1) ) {
        if ( v2->isNRestricted() ) return true;
        else                       return false;
      } else if ( c2->isSouth(c1) ) {
        if ( v2->isSRestricted() ) return true;
        else                       return false;
      } else if ( c2->isEast (c1) ) {
        if ( v2->isERestricted() ) return true;
        else                       return false;
      } else if ( c2->isWest (c1) ) {
        if ( v2->isWRestricted() ) return true;
        else                       return false;
      } else {
        cerr << Error( "GCells are not side by side." ) << endl;
        return true;
      }
    }
  }


  Dijkstra::Dijkstra ( AnabaticEngine* anabatic )
    : _anabatic      (anabatic)
    , _vertexes      ()
    , _distanceCb    (_distance)
    , _mode          (Mode::Standart)
    , _net           (NULL)
    , _stamp         (-1)
    , _sources       ()
    , _targets       ()
    , _searchArea    ()
    , _searchAreaHalo(0)
    , _connectedsId  (-1)
    , _queue         ()
  {
    const vector<GCell*>& gcells = _anabatic->getGCells();
    for ( GCell* gcell : gcells ) {
      _vertexes.push_back( new Vertex (gcell) );
    }
    _anabatic->getMatrix()->show();
  }


  Dijkstra::~Dijkstra ()
  {
    for ( Vertex* vertex : _vertexes ) delete vertex;
  }


  void  Dijkstra::load ( Net* net )
  {
    _cleanup();

    _net   = net;
    _stamp = _anabatic->incStamp();

    DebugSession::open( _net, 112, 120 );
    cdebug_log(112,1) << "Dijkstra::load() " << _net << endl;

    vector<RoutingPad*> rps;
    for ( Component* component : _net->getComponents() ) {
      RoutingPad* rp = dynamic_cast<RoutingPad*>( component );
      if (rp) { 
        rps.push_back( rp ); 
        cerr << "rp to route: " << rp << endl;
        continue; 
      }
    }

    for ( auto rp : rps ) {
      Point  center = rp->getBoundingBox().getCenter();
      GCell* gcell  = _anabatic->getGCellUnder( center );
      cerr << "rp   : " << rp << endl;
      cerr << "gcell: " << gcell << endl;
      
      if (gcell->isDevice()){
        _searchArea.merge( _net->getCell()->getAbutmentBox() );
      }

      cdebug_log(112,0) << "| " << rp << endl;
        
      if (not gcell) {
        cerr << Error( "Dijkstra::load(): %s\n"
                       "        @%s of %s is not under any GCell.\n"
                       "        It will be ignored so the routing may be incomplete."
                     , getString(rp).c_str()
                     , getString(center).c_str()
                     , getString(_net).c_str()
                     ) << endl;
        continue;
      }

      cdebug_log(112,0) << "Current Search area: " << _searchArea << ", gcell: " << gcell << endl;
      _searchArea.merge( gcell->getBoundingBox() );
      cdebug_log(112,0) << "New Search area: " << _searchArea << endl;

      Vertex* seed = gcell->getObserver<Vertex>(GCell::Observable::Vertex);
      if (seed->getConnexId() < 0) {
        VertexSet  connecteds;
        _getConnecteds( seed, connecteds );

        ++_connectedsId;
        for ( Vertex* vertex : connecteds ) {
          vertex->setDistance     ( Vertex::unreached );
          vertex->setStamp        ( _stamp );
          vertex->setConnexId     ( _connectedsId );
          vertex->setBranchId     ( 0 );
          vertex->setDegree       ( 1 );
          vertex->setRpCount      ( 0 );
          vertex->setFrom         ( NULL );
          vertex->clearRestriction();
          _targets.insert( vertex );
          cdebug_log(112,0) << "| Add: " << vertex << endl;
        }
      }

      seed->incRpCount();
      Contact* vcontact = seed->getGContact( _net );
      rp->getBodyHook()->detach();
      rp->getBodyHook()->attach( vcontact->getBodyHook() );
    }

    _searchArea.inflate( _searchAreaHalo );
    cdebug_log(112,0) << "Search area: " << _searchArea << endl;
    cdebug_tabw(112,-1);
    DebugSession::close();
  }


  void  Dijkstra::_selectFirstSource ()
  {
    if (_targets.empty()) {
#if 0
      cparanoid << Error( "Dijkstra::_selectFirstSource(): %s has no vertexes to route, ignored."
                        , getString(_net).c_str()
                        ) << endl;
#endif
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

    for ( auto ivertex = _targets.begin() ; ivertex != _targets.end() ; ) {
      auto inext = ivertex; inext++;

      if ((*ivertex)->getConnexId() == firstSource->getConnexId()) {
        _sources.insert( *ivertex );
        _targets.erase (  ivertex );
      }
 
      ivertex = inext;
    }

    cdebug_log(112,0) << "Dijkstra::_selectFirstSource() " << *_sources.begin() << endl;
  }


  void  Dijkstra::_cleanup ()
  {
  //_checkEdges();
    _sources.clear();
    _targets.clear();
    _searchArea.makeEmpty();
    _connectedsId = 0;
  }


  bool  Dijkstra::_propagate ( Flags enabledSides )
  {
    cdebug_log(112,1) << "Dijkstra::_propagate() " << _net << endl;

    while ( not _queue.empty() ) {
      _queue.dump();

      Vertex* current = _queue.top();
      cdebug_log(111,0) << "[Current Vertex]: " << current << endl;
      _queue.pop();

      if ((current->getConnexId() == _connectedsId) or (current->getConnexId() < 0)) {
        for ( Edge* edge : current->getGCell()->getEdges() ) {

          if (edge == current->getFrom()) {
            cdebug_log(111,0) << "edge == current->getFrom()" << endl;
            continue;
          }
          
          GCell*  gneighbor = edge->getOpposite(current->getGCell());
          Vertex* vneighbor = gneighbor->getObserver<Vertex>(GCell::Observable::Vertex);

        //if (not _searchArea.contains(vneighbor->getCenter())) {
          if (not _searchArea.intersect(gneighbor->getBoundingBox())) {
            cdebug_log(111,0) << "not in _searchArea: " << _searchArea << ", gneighbor area: "  << gneighbor->getBoundingBox() << endl;
            continue;
          }

          cdebug_log(111,0) << "| Edge " << edge << endl;
          cdebug_log(111,0) << "+ Neighbor: " << vneighbor << endl;

          DbU::Unit distance = _distanceCb( current, vneighbor, edge );
          cdebug_log(111,0) << "Distance: " << distance << ", unreachable: " << Vertex::unreachable << endl;


          if (vneighbor->getConnexId() == _connectedsId) {
            cdebug_log(111,0) << "ConnectedsId" << endl;
            continue;
          }

          if ( (      (distance  < vneighbor->getDistance())
               or (   (distance == vneighbor->getDistance())
                  and (current->getBranchId() > vneighbor->getBranchId())) 
               ) 
             and (distance != Vertex::unreachable)
             ){
            cdebug_log(111,0) << "1" << endl;
            if (vneighbor->getDistance() != Vertex::unreached) {
              cdebug_log(111,0) << "2" << endl;
              _queue.erase( vneighbor );
            } else {
              cdebug_log(111,0) << "3" << endl;
              if (not vneighbor->hasValidStamp()) {
                vneighbor->setConnexId( -1 );
                vneighbor->setStamp   ( _stamp );
                vneighbor->setDegree  ( 1 );
                vneighbor->setRpCount ( 0 );
              }
            }

            vneighbor->setBranchId( current->getBranchId() );
            vneighbor->setDistance( distance );
            vneighbor->setFrom    ( edge );
            _queue.push( vneighbor );

            cdebug_log(111,0) << "Push: (size:" << _queue.size() << ") " << vneighbor << endl;
          }
        }

        continue;
      }

    // We did reach another target (different <connexId>).
    // Tag back the path, with a higher <branchId>.
      _traceback( current );
      cdebug_tabw(112,-1);
      return true;
    }

    cerr << Error( "Dijkstra::propagate(): %s has unreachable targets."
                 , getString(_net).c_str()
                 ) << endl;

    cdebug_tabw(112,-1);
    return false;
  }


  void  Dijkstra::_traceback ( Vertex* current )
  {
    cdebug_log(112,1) << "Dijkstra::_traceback() " << _net << " branchId:" << _sources.size() << endl;

    int branchId = _sources.size();
    _toSources( current, _connectedsId );

    current = current->getPredecessor();
    while ( current ) {
      cdebug_log(112,0) << "| " << current << endl;

      current->incDegree();
      if (current->getConnexId() == _connectedsId) break;

      Edge* from = current->getFrom();
      if (not from) break;

      current->setDistance( 0.0 );
      current->setConnexId( _connectedsId );
      current->setBranchId( branchId );
      _sources.insert( current );
      _queue.push( current );
      
      current = current->getPredecessor();
    }

    cdebug_tabw(112,-1);
  }


  void  Dijkstra::_materialize ()
  {
    cdebug_log(112,1) << "Dijkstra::_materialize() " << _net << " _sources:" << _sources.size() << endl;

    if (_sources.size() < 2) { cdebug_tabw(112,-1); return; }

    for ( Vertex* startVertex : _sources ) {
      cdebug_log(112,0) << "? " << startVertex << endl;

      if (not startVertex->getFrom()) continue;
      if (   not startVertex->hasGContact(_net)
         and not startVertex->getRpCount()
         and (startVertex->getDegree() < 3)) continue;

      Vertex* source = startVertex;
      while ( source ) {
        cdebug_log(112,0) << "* " << source << endl;

        Edge* from = source->getFrom();
        vector<Edge*> aligneds;
        aligneds.push_back( from );

        Vertex*  target       = source->getPredecessor();
        Interval constraint   = from->getSide();
        source->setFrom( NULL );

        cdebug_log(112,0) << "+ " << target << endl;

        if (target->getConnexId() < 0) {
          cdebug_log(112,0) << "| " << "break (abort: false start)." << endl;
          break;
        }

        while ( true ) {
          from = target->getFrom();
          if (  not from
             or not (target->getGCell()->isMatrix())
             or (target->hasGContact(_net))
             or (target->getRpCount())
             or (target->getDegree() > 2)
             or (aligneds.back()->isHorizontal() xor from->isHorizontal())
             or not constraint.intersect(from->getSide())) break;

          aligneds.push_back( from );
          constraint.merge( from->getSide() );

          Vertex* nextTarget = target->getPredecessor();
          target->setFrom( NULL );
          target = nextTarget;

          cdebug_log(112,0) << "| " << target << endl;
        }

        Contact* sourceContact = source->getGContact( _net );
        Contact* targetContact = target->hasGContact( _net );
        Segment* segment       = NULL;

        if (not targetContact) {
          if (target->getFrom()) targetContact = target->getGContact( _net );
          else                   targetContact = target->breakGoThrough( _net );
        }

        if (aligneds.front()->isHorizontal()) {
          if (sourceContact->getX() > targetContact->getX())
            std::swap( sourceContact, targetContact );
          if (sourceContact->getX() == targetContact->getX()){
            cerr << "source Vertex: " << source << endl;
            cerr << "target Vertex: " << source << endl;
          }

          segment = Horizontal::create( sourceContact
                                      , targetContact
                                      , _anabatic->getConfiguration()->getGHorizontalLayer()
                                      , constraint.getCenter()
                                      , DbU::fromLambda(2.0)
                                      );
          for ( Edge* through : aligneds ) through->add( segment );
        } else {
          if (sourceContact->getY() > targetContact->getY())
            std::swap( sourceContact, targetContact );
          if (sourceContact->getY() == targetContact->getY()){
            cerr << "source Vertex: " << source << endl;
            cerr << "target Vertex: " << source << endl;
          }

          segment = Vertical::create( sourceContact
                                    , targetContact
                                    , _anabatic->getConfiguration()->getGVerticalLayer()
                                    , constraint.getCenter()
                                    , DbU::fromLambda(2.0)
                                    );
          for ( Edge* through : aligneds ) through->add( segment );
        }

        cdebug_log(112,0) << "| " << segment << endl;
        cdebug_log(112,0) << "| " << "break (turn, branch or terminal)." << endl;
        source = (target->getFrom()) ? target : NULL;
      }
    }

    cdebug_tabw(112,-1);
  }


  void  Dijkstra::run ( Dijkstra::Mode mode )
  {
    DebugSession::open( _net, 112, 120 );

    cdebug_log(112,1) << "Dijkstra::run() on " << _net << " mode:" << mode << endl;
    _mode = mode;

    _selectFirstSource();
    if (_sources.empty()) {
      cdebug_log(112,0) << "No source to start, not routed." << endl;
      cdebug_tabw(112,-1);
      return;
    }

    Flags enabledEdges = Flags::AllSides;
    if (_mode & Mode::Monotonic) {
      if ((*_sources.begin())->getCenter().getY() <= (*_targets.begin())->getCenter().getY())
        enabledEdges = Flags::EastSide | Flags::NorthSide;
      else
        enabledEdges = Flags::EastSide | Flags::SouthSide;
    }

    _queue.clear();
    _connectedsId = (*_sources.begin())->getConnexId();
    for ( Vertex* source : _sources ) {
      _queue.push( source );
      source->setDistance( 0.0 );
      cdebug_log(112,0) << "Push source: (size:" << _queue.size() << ") "
                        << source
                        << " _connectedsId:" << _connectedsId << endl;
    }


    while ( not _targets.empty() and _propagate(enabledEdges) );

    _queue.clear();
    _materialize();

    _anabatic->getNetData( _net )->setGlobalRouted( true );

    cdebug_tabw(112,-1);
    DebugSession::close();
  }


  void  Dijkstra::_toSources ( Vertex* source, int connexId )
  {
    cdebug_log(112,1) << "Dijkstra::_toSources() " << endl;
    cdebug_log(112,0) << "* from:  " << source << endl;

    source->setConnexId( connexId );
    source->setDistance( 0.0 );
    _targets.erase ( source );
    _sources.insert( source );
    _queue.push( source );

    VertexSet stack;
    stack.insert( source );

    while ( not stack.empty() ) {
      source = *stack.begin();
      stack.erase( source );

      cdebug_log(112,0) << "| source:" << source << " stack.size():" << stack.size() << endl;

      for ( Edge* edge : source->getGCell()->getEdges() ) {
        if (not edge->hasNet(_net)) {
          cdebug_log(112,0) << "  Not connected:" << edge
                            << " to:" << edge->getOpposite(source->getGCell()) << endl; 
          continue;
        }

        GCell*  gneighbor = edge->getOpposite(source->getGCell());
        Vertex* vneighbor = gneighbor->getObserver<Vertex>(GCell::Observable::Vertex);

        if (not vneighbor->hasValidStamp()) continue; 
        if (vneighbor->getConnexId() == connexId) continue;

        vneighbor->setConnexId( connexId );
        vneighbor->setDistance( 0.0 );
      //vneighbor->setFrom    ( edge );
        _targets.erase ( vneighbor );
        _sources.insert( vneighbor );
        _queue.push( vneighbor );
        stack.insert( vneighbor );
      }
    }

    cdebug_tabw(112,-1);
  }


  void  Dijkstra::_getConnecteds ( Vertex* source, VertexSet& connecteds )
  {
    cdebug_log(112,1) << "Dijkstra::_getConnecteds()" << endl;

    connecteds.clear();
    connecteds.insert( source );

    VertexSet stack;
    stack.insert( source );

    while ( not stack.empty() ) {
      source = *stack.begin();
      stack.erase( source );

      cdebug_log(112,0) << "| source:" << source << " stack.size():" << stack.size() << endl;

      for ( Edge* edge : source->getGCell()->getEdges() ) {
        if (not edge->hasNet(_net)) {
          cdebug_log(112,0) << "  Not connected:" << edge << endl; 
          continue;
        }

        GCell*  gneighbor = edge->getOpposite(source->getGCell());
        Vertex* vneighbor = gneighbor->getObserver<Vertex>(GCell::Observable::Vertex);

        if (connecteds.find(vneighbor) != connecteds.end()) continue;

        stack.insert( vneighbor );
        connecteds.insert( vneighbor );
      }
    }

    cdebug_tabw(112,-1);
  }


  void  Dijkstra::_checkEdges () const
  {
    cdebug_log(112,1) << "Dijkstra::_checkEdges()" << endl;

    // for ( Vertex* vertex : _vertexes ) {
    //   for ( Edge* edge : vertex->getGCell()->getEdges(Flags::EastSide|Flags::NorthSide) ) {
    //   }
    // }

    cdebug_tabw(112,-1);
  }


}  // Anabatic namespace.
