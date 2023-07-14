// -*- mode: C++; explicit-buffer-name: "Dijkstra.h<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
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


#pragma  once
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
// Class  :  "Anabatic::IntervalC".

  class IntervalC
  {
    public:
      enum iFlag { None         = 0
                 , iFHorizontal = (1<<0)
                 , iFVertical   = (1<<1)
                 , iSet         = (1<<2)
      };

    public:
                          IntervalC();
                          IntervalC(const IntervalC&);
                          IntervalC(IntervalC&);
                          IntervalC( DbU::Unit, DbU::Unit, DbU::Unit );
                         ~IntervalC();
             void         set        ( DbU::Unit, DbU::Unit, DbU::Unit );
             void         setRange   ( DbU::Unit, DbU::Unit );
             void         extendMin  ( DbU::Unit );
             void         extendMax  ( DbU::Unit );
             void         print      () const;
      inline bool         isH        () const;
      inline bool         isV        () const;
      inline void         setAsH     ();
      inline void         setAsV     ();
      inline DbU::Unit    getMin     () const;
      inline DbU::Unit    getMax     () const;
      inline DbU::Unit    getCenter  () const;
             DbU::Unit    getAxis    () const;
      inline void         setAxis    ( DbU::Unit );
      inline void         setiSet    ();
      inline bool         isiSet     () const;
             void         reset      ();

             Flags        getFlags   () const;
             void         setFlags   (Flags);

    private:
      Flags        _flags;
      DbU::Unit    _min;
      DbU::Unit    _max;
      DbU::Unit    _axis;
  };

  inline void         IntervalC::setAsH   () { _flags = ((_flags & ~(0x3)) | iFHorizontal); }
  inline void         IntervalC::setAsV   () { _flags = ((_flags & ~(0x3)) | iFVertical  ); }
  inline void         IntervalC::setAxis  ( DbU::Unit axis ) { _axis = axis; }
  inline DbU::Unit    IntervalC::getAxis  () const           { return _axis; }
  inline DbU::Unit    IntervalC::getCenter() const           { return getMin()+getMax(); }
  inline DbU::Unit    IntervalC::getMin   () const           { return _min; }
  inline DbU::Unit    IntervalC::getMax   () const           { return _max; }
  inline void         IntervalC::setiSet  ()  { _flags |= iSet; }
  inline bool         IntervalC::isiSet   () const { return _flags & iSet; }
  inline bool         IntervalC::isH      () const { return _flags & iFHorizontal; }
  inline bool         IntervalC::isV      () const { return _flags & iFVertical  ; }
  inline void         IntervalC::setFlags ( Flags flags ) { _flags = flags; }
  inline Flags        IntervalC::getFlags () const    { return _flags; }

// -------------------------------------------------------------------
// Class  :  "Anabatic::GRAData".

  class GRAData 
  {
    private:
                      GRAData();
                     ~GRAData();
    public:
      static GRAData* create();  
    //////////////////////////////////////// GRDATA
      inline bool       isiSet            () const;
      inline IntervalC  getInterv         () const;
      inline IntervalC  getIntervFrom     () const;
      inline DbU::Unit  getIAxis          () const;
      inline DbU::Unit  getIMax           () const;
      inline DbU::Unit  getIMin           () const;
      inline DbU::Unit  getPIAxis         () const;
      inline DbU::Unit  getPIMax          () const;
      inline DbU::Unit  getPIMin          () const;

      inline IntervalC  getIntervFrom2    () const;
      inline DbU::Unit  getPIMax2         () const;
      inline DbU::Unit  getPIMin2         () const;
      inline DbU::Unit  getPIAxis2        () const;

      inline void       setInterv         ( DbU::Unit, DbU::Unit, DbU::Unit );
      inline void       setIntervfrom     ( DbU::Unit, DbU::Unit, DbU::Unit );
      inline void       setIntervfrom2    ( DbU::Unit, DbU::Unit, DbU::Unit );
             void       resetIntervals    ();
             void       clearFrom2        ();
      inline Edge*      getFrom2          () const;
      inline void       setFrom2          ( Edge* );
      inline void       printInterv       () const;
      inline void       printIntervfrom   () const;

    private: 
      IntervalC  _intervfrom;
      IntervalC  _interv;
      Edge*      _from2;
      IntervalC  _intervfrom2;
  };

  inline bool         GRAData::isiSet       () const { return _interv.isiSet()     ; }
  inline IntervalC    GRAData::getInterv    () const { return _interv; }
  inline IntervalC    GRAData::getIntervFrom() const { return _intervfrom; }
  inline DbU::Unit    GRAData::getIAxis     () const { return _interv.getAxis()    ; }
  inline DbU::Unit    GRAData::getIMax      () const { return _interv.getMax()     ; }
  inline DbU::Unit    GRAData::getIMin      () const { return _interv.getMin()     ; }
  inline DbU::Unit    GRAData::getPIAxis    () const { return _intervfrom.getAxis(); }
  inline DbU::Unit    GRAData::getPIMax     () const { return _intervfrom.getMax() ; }
  inline DbU::Unit    GRAData::getPIMin     () const { return _intervfrom.getMin() ; }

  inline DbU::Unit    GRAData::getPIMax2        () const { return _intervfrom2.getMax() ; }
  inline DbU::Unit    GRAData::getPIMin2        () const { return _intervfrom2.getMin() ; }
  inline DbU::Unit    GRAData::getPIAxis2       () const { return _intervfrom2.getAxis(); }
  inline IntervalC    GRAData::getIntervFrom2   () const { return _intervfrom2; }

  inline void         GRAData::setInterv     ( DbU::Unit min, DbU::Unit max, DbU::Unit axis ) { _interv.set(min, max, axis); }
  inline void         GRAData::setIntervfrom ( DbU::Unit min, DbU::Unit max, DbU::Unit axis ) { _intervfrom.set(min, max, axis); }
  inline void         GRAData::setIntervfrom2( DbU::Unit min, DbU::Unit max, DbU::Unit axis ) { _intervfrom2.set(min, max, axis); }

  inline Edge*        GRAData::getFrom2        () const       { return _from2; }
  inline void         GRAData::setFrom2        ( Edge* from ) { _from2 = from; }
  inline void         GRAData::printInterv     () const { _interv.print()    ; }
  inline void         GRAData::printIntervfrom () const { _intervfrom.print(); }


// -------------------------------------------------------------------
// Class  :  "Anabatic::Vertex".

  class Vertex {
    public:
      static inline std::string  getValueString ( DbU::Unit );
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
                     , From2Mode     = (1<<5)
                     , UseFromFrom2  = (1<<6)
                     , iHorizontal   = (1<<7)
                     , iVertical     = (1<<8)
                     , iSet          = (1<<9)
                     , Driver        = (1<<10)
                     };
    public:
      static         DbU::Unit       unreached;
      static         DbU::Unit       unreachable;
    public:                         
      static         void            notify            ( Vertex*, unsigned flags );
      static inline  Vertex*         lookup            ( GCell* );
    public:                                            
             inline                  Vertex            ( GCell* );
           //inline                  Vertex            ( size_t id );
             inline                 ~Vertex            ();
             inline  bool            isDriver          () const;
             inline  bool            isAnalog          () const;
             inline  bool            hasDoneAllRps     () const;
             inline  Contact*        hasGContact       ( Net* ) const;
             inline  unsigned int    getId             () const;
             inline  GCell*          getGCell          () const;
             inline  Box             getBoundingBox    () const;
             inline  Edges           getEdges          ( Flags sides=Flags::AllSides ) const;
             inline  AnabaticEngine* getAnabatic       () const;
             inline  Contact*        getGContact       ( Net* );
                     bool            hasValidStamp     () const;
             inline  Point           getCenter         () const;
             inline  DbU::Unit       getDistance       () const;
             inline  int             getStamp          () const;
             inline  int             getBranchId       () const;
             inline  int             getConnexId       () const;
             inline  int             getDegree         () const;
             inline  int             getRpCount        () const;
                     Edge*           getFrom           () const;
             inline  Vertex*         getPredecessor    () const;
             inline  Vertex*         getNeighbor       ( Edge* ) const;
             inline  void            setDriver         ( bool state );
             inline  void            setDistance       ( DbU::Unit );
             inline  void            setStamp          ( int );
             inline  void            setConnexId       ( int );
             inline  void            setBranchId       ( int );
             inline  void            setDegree         ( int );
             inline  void            incDegree         ( int delta=1 );
             inline  void            setRpCount        ( int );
             inline  void            incRpCount        ( int delta=1 );
             inline  void            setFrom           ( Edge* );
             inline  void            add               ( RoutingPad* );
             inline  void            clearRps          ();
             inline  Contact*        breakGoThrough    ( Net* );
    // Analog related methods.
             inline  bool            isNorth           ( const Vertex* ) const;
             inline  bool            isSouth           ( const Vertex* ) const;
             inline  bool            isEast            ( const Vertex* ) const;
             inline  bool            isWest            ( const Vertex* ) const;
             inline  bool            isNRestricted     () const;
             inline  bool            isSRestricted     () const;
             inline  bool            isERestricted     () const;
             inline  bool            isWRestricted     () const;
             inline  bool            hasRestrictions   () const;
                     void            setRestricted     ();
                     void            clearRestriction  ();
             inline  void            setNRestricted    ();
             inline  void            setSRestricted    ();
             inline  void            setERestricted    ();
             inline  void            setWRestricted    ();
                     bool            hasRP             ( Net* ) const;
                     bool            hasVRP            ( Net* ) const;
                     bool            hasHRP            ( Net* ) const;
      static         bool            isRestricted      ( const Vertex* v1, const Vertex* v2, const Edge* e, DbU::Unit hpitch = 0, DbU::Unit vpitch = 0, Net* net = NULL);
                     bool            areSameSide       ( const Vertex*, const Vertex* ) const;
              inline bool            isFromFrom2       () const;
              inline bool            isFrom2Mode       () const;
              inline bool            isAxisTarget      () const;
              inline bool            isiHorizontal     () const;
              inline bool            isiVertical       () const;
              inline void            setFlags          ( uint32_t );
              inline void            unsetFlags        ( uint32_t );
                     bool            isH               () const;
                     bool            isV               () const;
              inline void            createAData       ();
                     Point           getStartPathPoint ( const Vertex* next ) const;
                     Point           getNextPathPoint  ( Point, const Vertex* ) const;
                     void            setIntervals      ( Vertex* );
              inline bool            hasAData          () const;
                     bool            isiSet            () const;
                     DbU::Unit       getIAxis          () const;
                     DbU::Unit       getIMax           () const;
                     DbU::Unit       getIMin           () const;
                     DbU::Unit       getPIAxis         () const;
                     DbU::Unit       getPIMax          () const;
                     DbU::Unit       getPIMin          () const;
                     void            setInterv         ( DbU::Unit, DbU::Unit, DbU::Unit );
                     void            setIntervfrom     ( DbU::Unit, DbU::Unit, DbU::Unit );
                     void            setIntervfrom2    ( DbU::Unit, DbU::Unit, DbU::Unit );
                     void            resetIntervals    ();
                     void            clearFrom2        ();
                     Edge*           getFrom2          () const;
                     void            setFrom2          ( Edge* );
                     DbU::Unit       getPIMax2         () const;
                     DbU::Unit       getPIMin2         () const;
                     DbU::Unit       getPIAxis2        () const;
                     IntervalC       getIntervFrom2    () const;
                     IntervalC       getIntervFrom     ( uint32_t criteria=0 ) const;
                     IntervalC       getInterv         () const;
                     void            printInterv       () const;
                     void            printIntervfrom   () const;
                     GCell*          getGPrev          ( uint32_t criteria=0 ) const;
 
    // Inspector support. 
                     string          _getString        () const;
    private:                        
                                     Vertex            ( const Vertex& );
                     Vertex&         operator=         ( const Vertex& );
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
      uint32_t             _flags;
      GRAData*             _adata;
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
    , _adata   (NULL)
  {
    gcell->setObserver( GCell::Observable::Vertex, &_observer );
  }


  inline Vertex*         Vertex::lookup         ( GCell* gcell ) { return gcell->getObserver<Vertex>(GCell::Observable::Vertex); }
  inline                 Vertex::~Vertex        () { _gcell->setObserver( GCell::Observable::Vertex, NULL ); }
  inline bool            Vertex::isDriver       () const { return _flags & Driver; }
  inline bool            Vertex::isAnalog       () const { return _gcell->isAnalog(); }
  inline Box             Vertex::getBoundingBox () const { return _gcell->getBoundingBox(); }
  inline Edges           Vertex::getEdges       ( Flags sides ) const { return _gcell->getEdges(sides); }
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
//inline Edge*           Vertex::getFrom        () const { return _from; }
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

  inline Vertex* Vertex::getNeighbor ( Edge* edge ) const
  {
    GCell* gcell = edge->getOpposite( getGCell() );
    return (gcell) ? gcell->getObserver<Vertex>(GCell::Observable::Vertex) : NULL;
  }

  inline void  Vertex::setDriver ( bool state )
  {
    if (state) _flags |=  Driver;
    else       _flags &= ~Driver;
  }

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

  inline bool         Vertex::hasAData         () const { return (_adata !=NULL)? true : false; }
//inline void         Vertex::setRestricted    () { _flags |= 0xF; }
//inline void         Vertex::clearRestriction () { _flags &= ~(0xF); }
  inline void         Vertex::setNRestricted   () { _flags |= NRestricted; }
  inline void         Vertex::setSRestricted   () { _flags |= SRestricted; }
  inline void         Vertex::setERestricted   () { _flags |= ERestricted; }
  inline void         Vertex::setWRestricted   () { _flags |= WRestricted; }

  inline bool         Vertex::isFromFrom2  () const { return (_flags & Vertex::UseFromFrom2); }
  inline bool         Vertex::isFrom2Mode  () const { return (_flags & Vertex::From2Mode   ); }
  inline bool         Vertex::isAxisTarget () const { return (_flags & Vertex::AxisTarget  ); }
  inline bool         Vertex::isiHorizontal() const { return (_flags & Vertex::iHorizontal ); }
  inline bool         Vertex::isiVertical  () const { return (_flags & Vertex::iVertical   ); }
  inline void         Vertex::setFlags     ( uint32_t mask ) { _flags |= mask ; }
  inline void         Vertex::unsetFlags   ( uint32_t mask ) { _flags &= ~mask; }

  inline std::string  Vertex::getValueString ( DbU::Unit distance )
  {
    if (distance == Vertex::unreachable) return "unreachable";
    if (distance == Vertex::unreached  ) return "unreached";
    return DbU::getValueString( distance );
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::PriorityQueue".

  class PriorityQueue {
    public:
      inline                PriorityQueue ();
      inline               ~PriorityQueue ();
      inline        bool    empty         () const;
      inline        size_t  size          () const;
      inline        void    push          ( Vertex* );
      inline        void    erase         ( Vertex* );
      inline        Vertex* top           ();
      inline        void    pop           ();
      inline        void    clear         ();
      inline        void    dump          () const;
      inline        void    setAttractor  ( const Point& );
      inline  const Point&  getAttractor  () const;
      inline        bool    hasAttractor  () const;
    private:
      class CompareByDistance {
        public:
                 inline      CompareByDistance ();
                        bool operator()        ( const Vertex* lhs, const Vertex* rhs ) const;
          static inline void setQueue          ( PriorityQueue* );
        private:
          static PriorityQueue* _pqueue;
      };
    private:
      bool                                 _hasAttractor;
      Point                                _attractor;
      multiset<Vertex*,CompareByDistance>  _queue;
  };


  inline      PriorityQueue::CompareByDistance::CompareByDistance () { }

  inline void PriorityQueue::CompareByDistance::setQueue ( PriorityQueue* pqueue ) { _pqueue = pqueue; }


  inline               PriorityQueue::PriorityQueue  () : _hasAttractor(false), _attractor(), _queue() { PriorityQueue::CompareByDistance::setQueue(this); }
  inline               PriorityQueue::~PriorityQueue () { }
  inline       bool    PriorityQueue::empty          () const { return _queue.empty(); }
  inline       size_t  PriorityQueue::size           () const { return _queue.size(); }
  inline       void    PriorityQueue::push           ( Vertex* v ) { _queue.insert(v); }
  inline       Vertex* PriorityQueue::top            () { return _queue.empty() ? NULL : *_queue.begin(); }
  inline       void    PriorityQueue::clear          () { _queue.clear(); _hasAttractor=false; }
  inline       void    PriorityQueue::setAttractor   ( const Point& p ) { _attractor=p;  _hasAttractor=true; }
  inline       bool    PriorityQueue::hasAttractor   () const { return _hasAttractor; }
  inline const Point&  PriorityQueue::getAttractor   () const { return _attractor; }

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
          inline               Mode         ( Flag flags=NoMode );
          inline               Mode         ( BaseFlags );
          virtual             ~Mode         ();
          virtual std::string  _getTypeName () const;
          virtual std::string  _getString   () const;
      };
    public:
      typedef std::function<DbU::Unit(const Vertex*,const Vertex*,const Edge*)>  distance_t;
    public:
                              Dijkstra                 ( AnabaticEngine* );
                             ~Dijkstra                 ();
    public:                                            
      inline       bool       isBipoint                () const;
      inline       bool       isSourceVertex           ( Vertex* ) const;
      inline       Net*       getNet                   () const;
      inline       bool       isTargetVertex           ( Vertex* ) const;
                   DbU::Unit  getAntennaGateMaxWL      () const;
      inline       DbU::Unit  getSearchAreaHalo        () const;
      template<typename DistanceT>                     
      inline       DistanceT* setDistance              ( DistanceT );
      inline       void       setSearchAreaHalo        ( DbU::Unit );
                   void       load                     ( Net* net ); 
                   void       loadFixedGlobal          ( Net* net ); 
                   void       run                      ( Mode mode=Mode::Standart );
      inline const VertexSet& getSources               () const;
    private:                                           
                               Dijkstra                ( const Dijkstra& );
                   Dijkstra&  operator=                ( const Dijkstra& );
      static       DbU::Unit  _distance                ( const Vertex*, const Vertex*, const Edge* );
                   Point      _getPonderedPoint        () const;
                   void       _cleanup                 ();
                   bool       _propagate               ( Flags enabledSides );
                   void       _traceback               ( Vertex* );
                   void       _materialize             ();
                   void       _selectFirstSource       ();
                   void       _toSources               ( Vertex*, int connexId );
                   void       _getConnecteds           ( Vertex*, VertexSet& );
                   void       _checkEdges              () const;
                   void       _createSelfSymSeg        ( Segment* );
                   
      inline       void       setAxisTarget            ();
      inline       bool       needAxisTarget           () const;
      inline       void       setFlags                 ( Flags );
      inline       void       unsetFlags               ( Flags );
                   void       setAxisTargets           ();
                   void       unsetAxisTargets         ();
                   
                   bool       _attachSymContactsHook   ( RoutingPad* ); 
                   void       _limitSymSearchArea      ( RoutingPad* rp );
                   void       _setSourcesGRAData       ( Vertex*, RoutingPad*);
                   bool       _checkFrom2              ( Edge*, Vertex* );
                   bool       _isDistance2Shorter      ( DbU::Unit&, Vertex*, Vertex*, Edge* );
                   void       _pushEqualDistance       ( DbU::Unit, bool, Vertex*, Vertex*, Edge* );
                   void       _updateGRAData           ( Vertex*, bool, Vertex* );
                   void       _initiateUpdateIntervals ( Vertex* );
                   bool       _updateIntervals         ( bool&, Vertex*, bool&, int&, Edge* );
                   void       _updateRealOccupancy     ( Vertex* );
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
      Flags            _flags;
  };


  inline Dijkstra::Mode::Mode ( Dijkstra::Mode::Flag flags ) : BaseFlags(flags) { }
  inline Dijkstra::Mode::Mode ( BaseFlags            base  ) : BaseFlags(base)  { }

  inline bool       Dijkstra::isBipoint         () const { return _net and (_targets.size()+_sources.size() == 2); }
  inline bool       Dijkstra::isSourceVertex    ( Vertex* v ) const { return (_sources.find(v) != _sources.end()); }
  inline bool       Dijkstra::isTargetVertex    ( Vertex* v ) const { return (_targets.find(v) != _targets.end()); }
  inline Net*       Dijkstra::getNet            () const { return _net; }
  inline DbU::Unit  Dijkstra::getSearchAreaHalo () const { return _searchAreaHalo; }
  inline void       Dijkstra::setSearchAreaHalo ( DbU::Unit halo ) { _searchAreaHalo = halo; }

  template<typename DistanceT>
  inline DistanceT* Dijkstra::setDistance       ( DistanceT cb ) { _distanceCb = cb; return _distanceCb.target<DistanceT>(); }

  inline void       Dijkstra::setFlags       ( Flags mask ) { _flags |= mask; }
  inline bool       Dijkstra::needAxisTarget () const { return (_flags & Mode::AxisTarget); }
  inline void       Dijkstra::unsetFlags     ( Flags mask ) { _flags &= ~mask; }
  inline const VertexSet& Dijkstra::getSources     () const { return _sources; }

}  // Anabatic namespace.


GETSTRING_POINTER_SUPPORT(Anabatic::Vertex);
IOSTREAM_POINTER_SUPPORT(Anabatic::Vertex);
INSPECTOR_PR_SUPPORT(Anabatic::Dijkstra::Mode);
