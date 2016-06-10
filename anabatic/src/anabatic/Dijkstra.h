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
}
#include "anabatic/GCell.h"


namespace Anabatic {

  using std::set;
  using std::multiset;
  using Hurricane::Observer;
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
      static         float           unreached;
    public:                         
      static         void            notify         ( Vertex*, unsigned flags );
    public:                         
             inline                  Vertex         ( GCell* );
           //inline                  Vertex         ( size_t id );
             inline                 ~Vertex         ();
             inline  unsigned int    getId          () const;
             inline  GCell*          getGCell       () const;
             inline  AnabaticEngine* getAnabatic    () const;
             inline  Contact*        getGContact    ( Net* );
                     bool            hasValidStamp  () const;
             inline  Point           getCenter      () const;
             inline  float           getDistance    () const;
             inline  int             getStamp       () const;
             inline  int             getConnexId    () const;
             inline  Edge*           getFrom        () const;
             inline  Vertex*         getPredecessor () const;
             inline  void            setDistance    ( float );
             inline  void            setStamp       ( int );
             inline  void            setConnexId    ( int );
             inline  void            setFrom        ( Edge* );
    // Inspector support. 
                     string          _getString     () const;
    private:                        
                                     Vertex         ( const Vertex& );
                     Vertex&         operator=      ( const Vertex& );
    private:
      size_t            _id;
      GCell*            _gcell;
      Observer<Vertex>  _observer;
      int               _connexId;
      int               _stamp;
      float             _distance;
      Edge*             _from;
  };


  inline Vertex::Vertex ( GCell* gcell )
    : _id      (gcell->getId())
    , _gcell   (gcell)
    , _observer(this)
    , _connexId(-1)
    , _stamp   (-1)
    , _distance(unreached)
    , _from    (NULL)
  {
    gcell->setObserver( GCell::Observable::Vertex, &_observer );
  }

  // inline Vertex::Vertex ( size_t id )
  //   : _id      (id)
  //   , _gcell   (NULL)
  //   , _observer((Vertex*)0x1)  // To trick the NULL detection.
  //   , _connexId(-1)
  //   , _stamp   (-1)
  //   , _distance(unreached)
  //   , _from    (NULL)
  // { }

  inline                 Vertex::~Vertex     () { }
  inline unsigned int    Vertex::getId       () const { return _id; }
  inline GCell*          Vertex::getGCell    () const { return _gcell; }
  inline AnabaticEngine* Vertex::getAnabatic () const { return _gcell->getAnabatic(); }
  inline Contact*        Vertex::getGContact ( Net* net ) { return _gcell->getGContact(net); }
  inline Point           Vertex::getCenter   () const { return _gcell->getBoundingBox().getCenter(); }
  inline float           Vertex::getDistance () const { return hasValidStamp() ? _distance : unreached; }
  inline int             Vertex::getStamp    () const { return _stamp; }
  inline int             Vertex::getConnexId () const { return hasValidStamp() ? _connexId : -1; }
  inline Edge*           Vertex::getFrom     () const { return _from; }
  inline void            Vertex::setDistance ( float distance ) { _distance=distance; }
  inline void            Vertex::setFrom     ( Edge* from ) { _from=from; }
  inline void            Vertex::setStamp    ( int stamp ) { _stamp=stamp; }
  inline void            Vertex::setConnexId ( int id ) { _connexId=id; }

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
  { return lhs->getDistance() < rhs->getDistance(); }


  inline         PriorityQueue::PriorityQueue  () : _queue() { }
  inline         PriorityQueue::~PriorityQueue () { }
  inline bool    PriorityQueue::empty          () const { return _queue.empty(); }
  inline size_t  PriorityQueue::size           () const { return _queue.size(); }
  inline void    PriorityQueue::push           ( Vertex* v ) { _queue.insert(v); }
  inline Vertex* PriorityQueue::top            () { return _queue.empty() ? NULL : *_queue.begin(); }
  inline void    PriorityQueue::clear          () { _queue.clear(); }

  inline void  PriorityQueue::pop ()
  {
    cdebug.log(111) << "Pop: (size:" << _queue.size() << ") " << *_queue.begin() << std::endl;
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
    if (cdebug.enabled(111)) {
      cdebug.log(111,1) << "PriorityQueue::dump() size:" << size() << std::endl;
      size_t order = 0;
      for ( Vertex* v : _queue )
        cdebug.log(111) << "[" << std::setw(3) << order++ << "] " << v << std::endl;
      cdebug.tabw(111,-1);
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
             float     getDistance        ( const Vertex*, const Vertex*, const Edge* );
    public:
                       Dijkstra           ( AnabaticEngine* );
                      ~Dijkstra           ();
    public:                        
      inline bool      isBipoint          () const;
             void      load               ( Net* );
             void      run                ( Mode mode=Mode::Standart );
    private:
                       Dijkstra           ( const Dijkstra& );
             Dijkstra& operator=          ( const Dijkstra& );
             bool      _propagate         ( Flags enabledSides );
             void      _selectFirstSource ();
             void      _toWires           ();
    private:
      AnabaticEngine*  _anabatic;
      vector<Vertex*>  _vertexes;
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

  inline bool  Dijkstra::isBipoint () const { return _net and (_targets.size()+_sources.size() == 2); }


}  // Anabatic namespace.


GETSTRING_POINTER_SUPPORT(Anabatic::Vertex);
IOSTREAM_POINTER_SUPPORT(Anabatic::Vertex);
INSPECTOR_PV_SUPPORT(Anabatic::Dijkstra::Mode);

#endif  // ANABATIC_DIJKSTRA_H
