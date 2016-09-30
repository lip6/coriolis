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
  using Hurricane::Plug;
  class AnabaticEngine;


// -------------------------------------------------------------------
// Class  :  "Anabatic::Vertex".

  class Vertex {
    public:
      class CompareById {
        public:
          inline bool  operator() ( const Vertex* lhs, const Vertex* rhs ) const;
      };
        public:
          enum FlagR { NoRestriction = 0
                     , NRestricted   = (1<<0)
                     , SRestricted   = (1<<1)
                     , ERestricted   = (1<<2)
                     , WRestricted   = (1<<3)
                     };
    public:
      static         DbU::Unit       unreached;
      static         DbU::Unit       unreachable;
    public:                         
      static         void            notify         ( Vertex*, unsigned flags );
    public:                         
             inline                  Vertex         ( GCell* );
           //inline                  Vertex         ( size_t id );
             inline                 ~Vertex         ();
             inline  bool            hasDoneAllRps  () const;
             inline  Contact*        hasGContact    ( Net* ) const;
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
             inline  int             getDegree      () const;
             inline  int             getRpCount     () const;
             inline  Edge*           getFrom        () const;
             inline  Vertex*         getPredecessor () const;
             inline  void            setDistance    ( DbU::Unit );
             inline  void            setStamp       ( int );
             inline  void            setConnexId    ( int );
             inline  void            setBranchId    ( int );
             inline  void            setDegree      ( int );
             inline  void            incDegree      ( int delta=1 );
             inline  void            setRpCount     ( int );
             inline  void            incRpCount     ( int delta=1 );
             inline  void            setFrom        ( Edge* );
             inline  void            add            ( RoutingPad* );
             inline  void            clearRps       ();
             inline  Contact*        breakGoThrough ( Net* );

             inline  bool            isNorth        ( Vertex* ) const;
             inline  bool            isSouth        ( Vertex* ) const;
             inline  bool            isEast         ( Vertex* ) const;
             inline  bool            isWest         ( Vertex* ) const;
             inline  bool            isNRestricted  () const;
             inline  bool            isSRestricted  () const;
             inline  bool            isERestricted  () const;
             inline  bool            isWRestricted  () const;
             inline  bool            hasRestrictions() const;

             inline  void            setRestricted    ();
             inline  void            clearRestriction ();
             inline  void            setNRestricted   ();
             inline  void            setSRestricted   ();
             inline  void            setERestricted   ();
             inline  void            setWRestricted   ();
             inline  unsigned int    getFlags         () const; 

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
      int                  _degree  : 8;

      int                  _rpCount : 8;
      int                  _stamp;
      DbU::Unit            _distance;
      Edge*                _from;
      unsigned int         _flags;
  };


  inline Vertex::Vertex ( GCell* gcell )
    : _id      (gcell->getId())
    , _gcell   (gcell)
    , _observer(this)
    , _connexId(-1)
    , _branchId( 0)
    , _degree  ( 0)
    , _rpCount ( 0)
    , _stamp   (-1)
    , _distance(unreached)
    , _from    (NULL)
    , _flags   (NoRestriction)
  {
    gcell->setObserver( GCell::Observable::Vertex, &_observer );
  }


  inline                 Vertex::~Vertex        () { _gcell->setObserver( GCell::Observable::Vertex, NULL ); }
  inline Contact*        Vertex::hasGContact    ( Net* net ) const { return _gcell->hasGContact(net); }
  inline unsigned int    Vertex::getId          () const { return _id; }
  inline GCell*          Vertex::getGCell       () const { return _gcell; }
  inline AnabaticEngine* Vertex::getAnabatic    () const { return _gcell->getAnabatic(); }
  inline Contact*        Vertex::getGContact    ( Net* net ) { return _gcell->getGContact(net); }
  inline Point           Vertex::getCenter      () const { return _gcell->getBoundingBox().getCenter(); }
  inline DbU::Unit       Vertex::getDistance    () const { return hasValidStamp() ? _distance : unreached; }
  inline int             Vertex::getStamp       () const { return _stamp; }
  inline int             Vertex::getConnexId    () const { return hasValidStamp() ? _connexId : -1; }
  inline int             Vertex::getBranchId    () const { return hasValidStamp() ? _branchId :  0; }
  inline int             Vertex::getDegree      () const { return hasValidStamp() ? _degree   :  0; }
  inline int             Vertex::getRpCount     () const { return hasValidStamp() ? _rpCount  :  0; }
  inline Edge*           Vertex::getFrom        () const { return _from; }
  inline void            Vertex::setDistance    ( DbU::Unit distance ) { _distance=distance; }
  inline void            Vertex::setFrom        ( Edge* from ) { _from=from; }
  inline void            Vertex::setStamp       ( int stamp ) { _stamp=stamp; }
  inline void            Vertex::setConnexId    ( int id ) { _connexId=id; }
  inline void            Vertex::setBranchId    ( int id ) { _branchId=id; }
  inline void            Vertex::setDegree      ( int degree ) { _degree=degree; }
  inline void            Vertex::incDegree      ( int delta ) { _degree+=delta; }
  inline void            Vertex::setRpCount     ( int count ) { _rpCount=count; }
  inline void            Vertex::incRpCount     ( int delta ) { _rpCount+=delta; }
  inline Contact*        Vertex::breakGoThrough ( Net* net ) { return _gcell->breakGoThrough(net); }

  inline Vertex* Vertex::getPredecessor () const
  { return (hasValidStamp() and _from) ? _from->getOpposite(_gcell)->getObserver<Vertex>(GCell::Observable::Vertex) : NULL; }

  inline bool  Vertex::CompareById::operator() ( const Vertex* lhs, const Vertex* rhs ) const
  { return lhs->getId() < rhs->getId(); }


  typedef  set<Vertex*,Vertex::CompareById>  VertexSet;

  inline  bool Vertex::isNorth ( Vertex* v ) const { return _gcell->isNorth(v->getGCell()); } 
  inline  bool Vertex::isSouth ( Vertex* v ) const { return _gcell->isSouth(v->getGCell()); }
  inline  bool Vertex::isEast  ( Vertex* v ) const { return _gcell->isEast (v->getGCell()); }
  inline  bool Vertex::isWest  ( Vertex* v ) const { return _gcell->isWest (v->getGCell()); }
  inline  bool Vertex::isNRestricted      () const { return (_flags & NRestricted); }
  inline  bool Vertex::isSRestricted      () const { return (_flags & SRestricted); }
  inline  bool Vertex::isERestricted      () const { return (_flags & ERestricted); }
  inline  bool Vertex::isWRestricted      () const { return (_flags & WRestricted); }
  inline  bool Vertex::hasRestrictions    () const { return ( isNRestricted()||isSRestricted()||isERestricted()||isWRestricted()) ; }

  inline void         Vertex::setRestricted    () { _flags |= 0xF; }
  inline void         Vertex::clearRestriction () { _flags &= ~(0xF); }
  inline void         Vertex::setNRestricted   () { _flags |= 0x1; }
  inline void         Vertex::setSRestricted   () { _flags |= 0x2; }
  inline void         Vertex::setERestricted   () { _flags |= 0x4; }
  inline void         Vertex::setWRestricted   () { _flags |= 0x8; }
  inline unsigned int Vertex::getFlags         () const { return _flags; }


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
        cdebug_log(112,0) << "[" << tsetw(3) << order++ << "] " << v << std::endl;
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
      inline bool       isSourceVertex     ( Vertex* ) const;
      inline bool       isTargetVertex     ( Vertex* ) const;
      inline DbU::Unit  getSearchAreaHalo  () const;
      inline void       setDistance        ( distance_t );
      inline void       setSearchAreaHalo  ( DbU::Unit );
             void       load               ( Net* );
             void       run                ( Mode mode=Mode::Standart );
    private:           
                        Dijkstra           ( const Dijkstra& );
             Dijkstra&  operator=          ( const Dijkstra& );
      static DbU::Unit  _distance          ( const Vertex*, const Vertex*, const Edge* );
             void       _cleanup           ();
             bool       _propagate         ( Flags enabledSides );
             void       _traceback         ( Vertex* );
             void       _materialize       ();
             void       _selectFirstSource ();
             void       _toSources         ( Vertex*, int connexId );
             void       _getConnecteds     ( Vertex*, VertexSet& );
             void       _checkEdges        () const;
      static bool       isRestricted       ( const Vertex* v1, const Vertex* v2 );
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
      DbU::Unit        _searchAreaHalo;
      int              _connectedsId;
      PriorityQueue    _queue;
  };


  inline Dijkstra::Mode::Mode ( unsigned int flags ) : BaseFlags(flags) { }
  inline Dijkstra::Mode::Mode ( BaseFlags    base  ) : BaseFlags(base)  { }

  inline bool       Dijkstra::isBipoint         () const { return _net and (_targets.size()+_sources.size() == 2); }
  inline bool       Dijkstra::isSourceVertex    ( Vertex* v ) const { return (_sources.find(v) != _sources.end()); }
  inline bool       Dijkstra::isTargetVertex    ( Vertex* v ) const { return (_targets.find(v) != _targets.end()); }
  inline DbU::Unit  Dijkstra::getSearchAreaHalo () const { return _searchAreaHalo; }
  inline void       Dijkstra::setDistance       ( distance_t cb ) { _distanceCb = cb; }
  inline void       Dijkstra::setSearchAreaHalo ( DbU::Unit halo ) { _searchAreaHalo = halo; }


}  // Anabatic namespace.


GETSTRING_POINTER_SUPPORT(Anabatic::Vertex);
IOSTREAM_POINTER_SUPPORT(Anabatic::Vertex);
INSPECTOR_PV_SUPPORT(Anabatic::Dijkstra::Mode);

#endif  // ANABATIC_DIJKSTRA_H
