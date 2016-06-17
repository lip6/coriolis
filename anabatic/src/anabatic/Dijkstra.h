// -*- mode: C++; explicit-buffer-name: "Dijkstra.h<anabatic>" -*-
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
// |  C++ Header  :  "./anabatic/Dijkstra.h"                         |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_DIJKSTRA_H
#define  ANABATIC_DIJKSTRA_H

#include <set>
#include <iomanip>
#include "hurricane/Observer.h"
namespace Hurricane {
  class Net;
  class RoutingPad;
}
#include "anabatic/GCell.h"


namespace Anabatic {

  using std::set;
  using std::multiset;
  using Hurricane::Observer;
  using Hurricane::Net;
  using Hurricane::RoutingPad;
  class AnabaticEngine;


// -------------------------------------------------------------------
// Class  :  "Anabatic::Vertex".

  class Vertex {
    public:
      class CompareById {
        public:
          inline bool  operator() ( const Vertex* lhs, const Vertex* rhs );
      };
    public:
      static         DbU::Unit       unreached;
    public:                         
      static         void            notify         ( Vertex*, unsigned flags );
    public:                         
             inline                  Vertex         ( GCell* );
           //inline                  Vertex         ( size_t id );
             inline                 ~Vertex         ();
             inline  bool            hasDoneAllRps  () const;
             inline  unsigned int    getId          () const;
             inline  GCell*          getGCell       () const;
             inline  AnabaticEngine* getAnabatic    () const;
             inline  Contact*        getGContact    ( Net* );
                     bool            hasValidStamp  () const;
             inline  Point           getCenter      () const;
             inline  DbU::Unit       getDistance    () const;
             inline  int             getStamp       () const;
             inline  int             getBranchId    () const;
             inline  int             getConnexId    () const;
             inline  Edge*           getFrom        () const;
             inline  Vertex*         getPredecessor () const;
             inline  void            setDistance    ( DbU::Unit );
             inline  void            setStamp       ( int );
             inline  void            setConnexId    ( int );
             inline  void            setBranchId    ( int );
             inline  void            setFrom        ( Edge* );
             inline  void            add            ( RoutingPad* );
             inline  void            clearRps       ();
    // Inspector support. 
                     string          _getString     () const;
    private:                        
                                     Vertex         ( const Vertex& );
                     Vertex&         operator=      ( const Vertex& );
    private:
      size_t               _id;
      GCell*               _gcell;
      Observer<Vertex>     _observer;
      int                  _connexId;
      int                  _branchId;
      int                  _stamp;
      DbU::Unit            _distance;
      Edge*                _from;
  };


  inline Vertex::Vertex ( GCell* gcell )
    : _id         (gcell->getId())
    , _gcell      (gcell)
    , _observer   (this)
    , _connexId   (-1)
    , _branchId   ( 0)
    , _stamp      (-1)
    , _distance   (unreached)
    , _from       (NULL)
  {
    gcell->setObserver( GCell::Observable::Vertex, &_observer );
  }


  inline                 Vertex::~Vertex       () { }
  inline unsigned int    Vertex::getId         () const { return _id; }
  inline GCell*          Vertex::getGCell      () const { return _gcell; }
  inline AnabaticEngine* Vertex::getAnabatic   () const { return _gcell->getAnabatic(); }
  inline Contact*        Vertex::getGContact   ( Net* net ) { return _gcell->getGContact(net); }
  inline Point           Vertex::getCenter     () const { return _gcell->getBoundingBox().getCenter(); }
  inline DbU::Unit       Vertex::getDistance   () const { return hasValidStamp() ? _distance : unreached; }
  inline int             Vertex::getStamp      () const { return _stamp; }
  inline int             Vertex::getConnexId   () const { return hasValidStamp() ? _connexId : -1; }
  inline int             Vertex::getBranchId   () const { return hasValidStamp() ? _branchId :  0; }
  inline Edge*           Vertex::getFrom       () const { return _from; }
  inline void            Vertex::setDistance   ( DbU::Unit distance ) { _distance=distance; }
  inline void            Vertex::setFrom       ( Edge* from ) { _from=from; }
  inline void            Vertex::setStamp      ( int stamp ) { _stamp=stamp; }
  inline void            Vertex::setConnexId   ( int id ) { _connexId=id; }
  inline void            Vertex::setBranchId   ( int id ) { _branchId=id; }

  inline Vertex* Vertex::getPredecessor () const
  { return (hasValidStamp() and _from) ? _from->getOpposite(_gcell)->getObserver<Vertex>(GCell::Observable::Vertex) : NULL; }

  inline bool  Vertex::CompareById::operator() ( const Vertex* lhs, const Vertex* rhs )
  { return lhs->getId() < rhs->getId(); }


  typedef  set<Vertex*,Vertex::CompareById>  VertexSet;


// -------------------------------------------------------------------
// Class  :  "Anabatic::PriorityQueue".

  class PriorityQueue {
    public:
      inline          PriorityQueue ();
      inline         ~PriorityQueue ();
      inline  bool    empty         () const;
      inline  size_t  size          () const;
      inline  void    push          ( Vertex* );
      inline  void    erase         ( Vertex* );
      inline  Vertex* top           ();
      inline  void    pop           ();
      inline  void    clear         ();
      inline  void    dump          () const;
    private:
      class CompareByDistance {
        public:
          inline bool operator() ( const Vertex* lhs, const Vertex* rhs );
      };
    private:
      multiset<Vertex*,CompareByDistance>  _queue;
  };


  inline bool PriorityQueue::CompareByDistance::operator() ( const Vertex* lhs, const Vertex* rhs )
  {
    if (lhs->getDistance() == rhs->getDistance()) return lhs->getBranchId() > rhs->getBranchId();
    return lhs->getDistance() < rhs->getDistance();
  }


  inline         PriorityQueue::PriorityQueue  () : _queue() { }
  inline         PriorityQueue::~PriorityQueue () { }
  inline bool    PriorityQueue::empty          () const { return _queue.empty(); }
  inline size_t  PriorityQueue::size           () const { return _queue.size(); }
  inline void    PriorityQueue::push           ( Vertex* v ) { _queue.insert(v); }
  inline Vertex* PriorityQueue::top            () { return _queue.empty() ? NULL : *_queue.begin(); }
  inline void    PriorityQueue::clear          () { _queue.clear(); }

  inline void  PriorityQueue::pop ()
  {
    cdebug_log(112,0) << "Pop: (size:" << _queue.size() << ") " << *_queue.begin() << std::endl;
    _queue.erase(_queue.begin());
  }

  inline void  PriorityQueue::erase ( Vertex* v )
  {
    for ( auto ivertex = _queue.begin() ; ivertex != _queue.end() ; ++ivertex ) {
      if (*ivertex == v) { _queue.erase( ivertex ); break; }
    }
  }

  inline void  PriorityQueue::dump () const
  {
    if (cdebug.enabled(112)) {
      cdebug_log(112,1) << "PriorityQueue::dump() size:" << size() << std::endl;
      size_t order = 0;
      for ( Vertex* v : _queue )
        cdebug_log(112,0) << "[" << std::setw(3) << order++ << "] " << v << std::endl;
      cdebug_tabw(112,-1);
    }
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::Dijkstra".

  class Dijkstra {
    public:
    // Mode sub-classe.
      class Mode : public Hurricane::BaseFlags {
        public:
          enum Flag { NoMode    = 0
                    , Standart  = (1<<0)
                    , Monotonic = (1<<1)
                    };
        public:
          inline               Mode         ( unsigned int flags=NoMode );
          inline               Mode         ( BaseFlags );
          virtual             ~Mode         ();
          virtual std::string  _getTypeName () const;
          virtual std::string  _getString   () const;
      };
    public:
      typedef std::function<DbU::Unit(const Vertex*,const Vertex*,const Edge*)>  distance_t;

    public:
                        Dijkstra           ( AnabaticEngine* );
                       ~Dijkstra           ();
    public:                         
      inline bool       isBipoint          () const;
      inline void       setDistance        ( distance_t );
             void       load               ( Net* );
             void       run                ( Mode mode=Mode::Standart );
    private:           
                        Dijkstra           ( const Dijkstra& );
             Dijkstra&  operator=          ( const Dijkstra& );
      static DbU::Unit  _distance          ( const Vertex*, const Vertex*, const Edge* );
             bool       _propagate         ( Flags enabledSides );
             void       _traceback         ( Vertex* );
             void       _selectFirstSource ();
    private:
      AnabaticEngine*  _anabatic;
      vector<Vertex*>  _vertexes;
      distance_t       _distanceCb;
      Mode             _mode;
      Net*             _net;
      int              _stamp;
      VertexSet        _sources;
      VertexSet        _targets;
      Box              _searchArea;
      int              _connectedsId;
      PriorityQueue    _queue;
  };


  inline Dijkstra::Mode::Mode ( unsigned int flags ) : BaseFlags(flags) { }
  inline Dijkstra::Mode::Mode ( BaseFlags    base  ) : BaseFlags(base)  { }

  inline bool  Dijkstra::isBipoint   () const { return _net and (_targets.size()+_sources.size() == 2); }
  inline void  Dijkstra::setDistance ( distance_t cb ) { _distanceCb = cb; }


}  // Anabatic namespace.


GETSTRING_POINTER_SUPPORT(Anabatic::Vertex);
IOSTREAM_POINTER_SUPPORT(Anabatic::Vertex);
INSPECTOR_PV_SUPPORT(Anabatic::Dijkstra::Mode);

#endif  // ANABATIC_DIJKSTRA_H
