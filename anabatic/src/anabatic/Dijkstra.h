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
// Class  :  "Anabatic::Symmetry".

  class Symmetry
  {
    public:
      enum iFlag { None        = 0
                 , sHorizontal = (1<<0)
                 , sVertical   = (1<<1)
                 , sCFound     = (1<<2)
      };
    private:
       Symmetry(unsigned int, DbU::Unit);
      ~Symmetry();
    public:
      static Symmetry* create(unsigned int, DbU::Unit);   

    public:
             unsigned int getType () const;
      inline DbU::Unit    getValue() const;
      inline bool         isCFound() const;
      inline void         setAsH();
      inline void         setAsV();
      inline void         setFound();

    private:
      unsigned int _flags;
      DbU::Unit    _value;
  };

  inline void      Symmetry::setAsH   () { _flags = ((_flags & ~(0x3)) | sHorizontal); }
  inline void      Symmetry::setAsV   () { _flags = ((_flags & ~(0x3)) | sVertical  ); }
  inline void      Symmetry::setFound () { _flags |= sCFound; }
  inline DbU::Unit Symmetry::getValue () const { return _value; }
  inline bool      Symmetry::isCFound () const { return _flags & sCFound; }
  

// -------------------------------------------------------------------
// Class  :  "Anabatic::IntervalC".

  class IntervalC
  {
    public:
      enum iFlag { None        = 0
                 , iHorizontal = (1<<0)
                 , iVertical   = (1<<1)
                 , iSet        = (1<<2)
      };
    private:
                       IntervalC();
                      ~IntervalC();
    public:
      static IntervalC* create();   

    public:
             void       set          ( DbU::Unit, DbU::Unit, DbU::Unit );
             void       setRange     ( DbU::Unit, DbU::Unit );
             void       extendMin    ( DbU::Unit );
             void       extendMax    ( DbU::Unit );
             void       print        () const;
      inline bool       isH          () const;
      inline bool       isV          () const;
      inline void       setAsH       ();
      inline void       setAsV       ();
      inline DbU::Unit  getMin       () const;
      inline DbU::Unit  getMax       () const;
      inline DbU::Unit  getCenter    () const;
             DbU::Unit  getAxis      () const;
      inline void       setAxis      ( DbU::Unit );
      inline void       setiSet      ();
      inline bool       isiSet       () const;
             void       reset        ();

    private:
      unsigned int _flags;
      DbU::Unit    _min;
      DbU::Unit    _max;
      DbU::Unit    _axis;
  };

  inline void      IntervalC::setAsH   () { _flags = ((_flags & ~(0x3)) | iHorizontal); }
  inline void      IntervalC::setAsV   () { _flags = ((_flags & ~(0x3)) | iVertical  ); }
  inline void      IntervalC::setAxis  ( DbU::Unit axis ) { _axis = axis; }
  inline DbU::Unit IntervalC::getAxis  () const           { return _axis; }
  inline DbU::Unit IntervalC::getCenter() const           { return getMin()+getMax(); }
  inline DbU::Unit IntervalC::getMin   () const           { return _min; }
  inline DbU::Unit IntervalC::getMax   () const           { return _max; }
  inline void      IntervalC::setiSet  ()  { _flags |= iSet; }
  inline bool      IntervalC::isiSet   () const { return _flags & iSet; }
  inline bool      IntervalC::isH      () const { return _flags & iHorizontal; }
  inline bool      IntervalC::isV      () const { return _flags & iVertical  ; }

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
                     , AxisTarget    = (1<<4)
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

             inline  bool            isNorth        ( const Vertex* ) const;
             inline  bool            isSouth        ( const Vertex* ) const;
             inline  bool            isEast         ( const Vertex* ) const;
             inline  bool            isWest         ( const Vertex* ) const;
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
             inline  DbU::Unit       getXPath         () const;
             inline  DbU::Unit       getYPath         () const;
                     bool            hasRP            ( Net* ) const;
                     bool            hasVRP           ( Net* ) const;
                     bool            hasHRP           ( Net* ) const;
      static         bool            isRestricted     ( const Vertex* v1, const Vertex* v2 );
      static         Point           getNextPathPoint2( const Vertex*, const Vertex* );
                     Point           getPathPoint     ( const Vertex * ) const;
              inline void            setIAsH          ();
              inline void            setIAsV          ();
              inline DbU::Unit       getIAxis         () const;
              inline void            setIAxis         ( DbU::Unit ) ;
              inline DbU::Unit       getICenter       () const;
              inline DbU::Unit       getIMax          () const;
              inline DbU::Unit       getIMin          () const;
              inline void            setPIAsH          ();
              inline void            setPIAsV          ();
              inline DbU::Unit       getPIAxis         () const;
              inline void            setPIAxis         ( DbU::Unit ) ;
              inline DbU::Unit       getPICenter       () const;
              inline DbU::Unit       getPIMax          () const;
              inline DbU::Unit       getPIMin          () const;
                     bool            isH               () const;
                     bool            isV               () const;
              inline bool            isiSet            () const;
              inline void            setInterv         ( DbU::Unit, DbU::Unit, DbU::Unit );
              inline void            setIRange         ( DbU::Unit, DbU::Unit );
              inline void            setIRangeFrom     ( DbU::Unit, DbU::Unit );
              inline void            printInterval     () const ;
                     void            setIntervals      ( Vertex* );
                     void            resetIntervals    ();
             inline  void            setFlags          ( unsigned int );
             inline  bool            isAxisTarget      () const;
             inline  void            unsetFlags        ( unsigned int );
             inline  void            setAxisTarget     ();
 

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
      IntervalC*           _intervfrom;
      IntervalC*           _interv;
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

    , _intervfrom(IntervalC::create())  
    , _interv    (IntervalC::create()) 
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

  inline  bool Vertex::isNorth ( const Vertex* v ) const { return _gcell->isNorth(v->getGCell()); } 
  inline  bool Vertex::isSouth ( const Vertex* v ) const { return _gcell->isSouth(v->getGCell()); }
  inline  bool Vertex::isEast  ( const Vertex* v ) const { return _gcell->isEast (v->getGCell()); }
  inline  bool Vertex::isWest  ( const Vertex* v ) const { return _gcell->isWest (v->getGCell()); }
  inline  bool Vertex::isNRestricted      () const { return (_flags & NRestricted); }
  inline  bool Vertex::isSRestricted      () const { return (_flags & SRestricted); }
  inline  bool Vertex::isERestricted      () const { return (_flags & ERestricted); }
  inline  bool Vertex::isWRestricted      () const { return (_flags & WRestricted); }
  inline  bool Vertex::hasRestrictions    () const { return ( isNRestricted()||isSRestricted()||isERestricted()||isWRestricted()) ; }

  inline void         Vertex::setRestricted    () { _flags |= 0xF; }
  inline void         Vertex::clearRestriction () { _flags &= ~(0xF); }
  inline void         Vertex::setNRestricted   () { _flags |= NRestricted; }
  inline void         Vertex::setSRestricted   () { _flags |= SRestricted; }
  inline void         Vertex::setERestricted   () { _flags |= ERestricted; }
  inline void         Vertex::setWRestricted   () { _flags |= WRestricted; }
  inline unsigned int Vertex::getFlags         () const { return _flags; }

  inline void         Vertex::setIAxis   ( DbU::Unit axis ) { _interv->setAxis(axis); }
  inline DbU::Unit    Vertex::getIAxis   () const           { return _interv->getAxis(); }
  inline void         Vertex::setIAsH    ()                 { _interv->setAsH(); }
  inline void         Vertex::setIAsV    ()                 { _interv->setAsV(); }
  inline DbU::Unit    Vertex::getICenter () const           { return _interv->getCenter(); }
  inline DbU::Unit    Vertex::getIMax    () const           { return _interv->getMax(); }
  inline DbU::Unit    Vertex::getIMin    () const           { return _interv->getMin(); }
  inline void         Vertex::setPIAxis  ( DbU::Unit axis ) { _intervfrom->setAxis(axis); }
  inline DbU::Unit    Vertex::getPIAxis  () const           { return _intervfrom->getAxis(); }
  inline void         Vertex::setPIAsH   ()                 { _intervfrom->setAsH(); }
  inline void         Vertex::setPIAsV   ()                 { _intervfrom->setAsV(); }
  inline DbU::Unit    Vertex::getPICenter() const           { return _intervfrom->getCenter(); }
  inline DbU::Unit    Vertex::getPIMax   () const           { return _intervfrom->getMax(); }
  inline DbU::Unit    Vertex::getPIMin   () const           { return _intervfrom->getMin(); }
  inline bool         Vertex::isiSet     () const           { return _interv->isiSet(); }

  inline void         Vertex::setInterv    ( DbU::Unit min, DbU::Unit max, DbU::Unit axis ) { _interv->set(min, max, axis); }
  inline void         Vertex::setIRange    ( DbU::Unit min, DbU::Unit max ) { _interv->setRange(min, max); }
  inline void         Vertex::setIRangeFrom( DbU::Unit min, DbU::Unit max ) { _intervfrom->setRange(min, max); }
  inline void         Vertex::printInterval() const { _interv->print(); } 

  inline void         Vertex::setAxisTarget() { _flags |= AxisTarget; }
  inline bool         Vertex::isAxisTarget () const { return (_flags & AxisTarget); }
  inline void         Vertex::unsetFlags   ( unsigned int mask ) { _flags &= ~mask; }
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
          enum Flag { NoMode     = 0
                    , Standart   = (1<<0)
                    , Monotonic  = (1<<1)
                    , AxisTarget = (1<<2)
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
      template<typename DistanceT>
      inline DistanceT* setDistance        ( DistanceT );
      inline void       setSearchAreaHalo  ( DbU::Unit );
             void       load               ( Net* net ); 
             void       run                ( Mode mode=Mode::Standart );
    private:           
                        Dijkstra           ( const Dijkstra& );
             Dijkstra&  operator=          ( const Dijkstra& );
      static DbU::Unit  _distance          ( const Vertex*, const Vertex*, const Edge* );
             DbU::Unit  _getDistancetoRP   ( Point );
             DbU::Unit  _estimatePtoRP     ( Point, RoutingPad*, Vertex* );
             Point      _getPonderedPoint  () const;
             void       _cleanup           ();
             bool       _propagate         ( Flags enabledSides );
             void       _traceback         ( Vertex* );
             void       _materialize       ();
             void       _selectFirstSource ();
             void       _toSources         ( Vertex*, int connexId );
             void       _getConnecteds     ( Vertex*, VertexSet& );
             void       _checkEdges        () const;
             void       _createSelfSymSeg  ( Segment* );

             inline void setAxisTarget     ();
             inline bool needAxisTarget    () const;
             inline void setFlags          ( unsigned int );
             inline void unsetFlags        ( unsigned int );
                    void setAxisTargets    ();
                    void unsetAxisTargets  ();
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
      unsigned int     _flags;
  };


  inline Dijkstra::Mode::Mode ( unsigned int flags ) : BaseFlags(flags) { }
  inline Dijkstra::Mode::Mode ( BaseFlags    base  ) : BaseFlags(base)  { }

  inline bool       Dijkstra::isBipoint         () const { return _net and (_targets.size()+_sources.size() == 2); }
  inline bool       Dijkstra::isSourceVertex    ( Vertex* v ) const { return (_sources.find(v) != _sources.end()); }
  inline bool       Dijkstra::isTargetVertex    ( Vertex* v ) const { return (_targets.find(v) != _targets.end()); }
  inline DbU::Unit  Dijkstra::getSearchAreaHalo () const { return _searchAreaHalo; }
  inline void       Dijkstra::setSearchAreaHalo ( DbU::Unit halo ) { _searchAreaHalo = halo; }

  template<typename DistanceT>
  inline DistanceT* Dijkstra::setDistance       ( DistanceT cb ) { _distanceCb = cb; return _distanceCb.target<DistanceT>(); }

  inline void       Dijkstra::setFlags       ( unsigned int mask ) { _flags |= mask; }
  inline  bool      Dijkstra::needAxisTarget () const { return (_flags & Mode::AxisTarget); }
  inline void       Dijkstra::unsetFlags     ( unsigned int mask ) { _flags &= ~mask; }
}  // Anabatic namespace.


GETSTRING_POINTER_SUPPORT(Anabatic::Vertex);
IOSTREAM_POINTER_SUPPORT(Anabatic::Vertex);
INSPECTOR_PV_SUPPORT(Anabatic::Dijkstra::Mode);

#endif  // ANABATIC_DIJKSTRA_H
