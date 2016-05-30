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
namespace Hurricane {
  class Net;
}
#include "anabatic/GCell.h"


namespace Anabatic {

  using std::set;
  using Hurricane::Net;
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
      static        float           unreached;
    public:                        
             inline                 Vertex         ( GCell* );
             inline                 Vertex         ( size_t id );
             inline                ~Vertex         ();
             inline unsigned int    getId          () const;
             inline GCell*          getGCell       () const;
             inline AnabaticEngine* getAnabatic    () const;
             inline Contact*        getGContact    ( Net* );
                    bool            hasValidStamp  () const;
             inline Point           getCenter      () const;
             inline float           getDistance    () const;
             inline int             getStamp       () const;
             inline int             getConnexId    () const;
             inline Edge*           getFrom        () const;
             inline Vertex*         getPredecessor () const;
             inline void            setDistance    ( float );
             inline void            setStamp       ( int );
             inline void            setConnexId    ( int );
             inline void            setFrom        ( Edge* );
    private:                        
                                    Vertex         ( const Vertex& );
             Vertex&                operator=      ( const Vertex& );
    private:
      size_t  _id;
      GCell*  _gcell;
      int     _connexId;
      int     _stamp;
      float   _distance;
      Edge*   _from;
  };


  inline Vertex::Vertex ( GCell* gcell )
    : _id      (gcell->getId())
    , _gcell   (gcell)
    , _connexId(-1)
    , _stamp   (-1)
    , _distance(unreached)
    , _from    (NULL)
  {
    gcell->setLookup<Vertex>( this );
  }

  inline Vertex::Vertex ( size_t id )
    : _id      (id)
    , _gcell   (NULL)
    , _connexId(-1)
    , _stamp   (-1)
    , _distance(unreached)
    , _from    (NULL)
  { }

  inline                  Vertex::~Vertex        () { }
  inline unsigned int     Vertex::getId          () const { return _id; }
  inline GCell*           Vertex::getGCell       () const { return _gcell; }
  inline AnabaticEngine*  Vertex::getAnabatic    () const { return _gcell->getAnabatic(); }
  inline Contact*         Vertex::getGContact    ( Net* net ) { return _gcell->getGContact(net); }
  inline Point            Vertex::getCenter      () const { return _gcell->getBoundingBox().getCenter(); }
  inline float            Vertex::getDistance    () const { return hasValidStamp() ? _distance : unreached; }
  inline int              Vertex::getStamp       () const { return _stamp; }
  inline int              Vertex::getConnexId    () const { return hasValidStamp() ? _connexId : -1; }
  inline Edge*            Vertex::getFrom        () const { return _from; }
  inline void             Vertex::setDistance    ( float distance ) { _distance=distance; }
  inline void             Vertex::setFrom        ( Edge* from ) { _from=from; }
  inline void             Vertex::setStamp       ( int stamp ) { _stamp=stamp; }
  inline void             Vertex::setConnexId    ( int id ) { _connexId=id; }

  inline Vertex* Vertex::getPredecessor () const
  { return (hasValidStamp() and _from) ? _from->getOpposite(_gcell)->lookup<Vertex>() : NULL; }

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
    private:
      class CompareByDistance {
        public:
          inline bool operator() ( const Vertex* lhs, const Vertex* rhs );
      };
    private:
      set<Vertex*,CompareByDistance>  _queue;
  };


  inline bool PriorityQueue::CompareByDistance::operator() ( const Vertex* lhs, const Vertex* rhs )
  { return lhs->getDistance() < rhs->getDistance(); }


  inline          PriorityQueue::PriorityQueue  () : _queue() { }
  inline          PriorityQueue::~PriorityQueue () { }
  inline  bool    PriorityQueue::empty          () const { return _queue.empty(); }
  inline  size_t  PriorityQueue::size           () const { return _queue.size(); }
  inline  void    PriorityQueue::push           ( Vertex* v ) { _queue.insert(v); }
  inline  void    PriorityQueue::erase          ( Vertex* v ) { _queue.erase(v); }
  inline  Vertex* PriorityQueue::top            () { return _queue.empty() ? NULL : *_queue.begin(); }
  inline  void    PriorityQueue::pop            () { _queue.erase(_queue.begin()); }
  inline  void    PriorityQueue::clear          () { _queue.clear(); }


// -------------------------------------------------------------------
// Class  :  "Anabatic::Dijkstra".

  class Dijkstra {
    public:
                Dijkstra          ( AnabaticEngine* );
               ~Dijkstra          ();
    public:
      void      load              ( Net* );
      void      run               ();
      void      propagate         ();
      void      selectFirstSource ();
      void      toWires           ();
    private:
                Dijkstra          ( const Dijkstra& );
      Dijkstra& operator=         ( const Dijkstra& );
    private:
      AnabaticEngine*  _anabatic;
      vector<Vertex*>  _vertexes;
      Net*             _net;
      int              _stamp;
      VertexSet        _sources;
      VertexSet        _targets;
      Box              _searchArea;
      int              _connectedsId;
      PriorityQueue    _queue;
  };


}  // Anabatic namespace.

#endif  // ANABATIC_DIJKSTRA_H
