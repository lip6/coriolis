// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AntennaProtect.cpp"                     |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include <tuple>
#include "hurricane/Bug.h"
#include "hurricane/Warning.h"
#include "hurricane/DebugSession.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/NetRoutingProperty.h"
#include "hurricane/Layer.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Cell.h"
#include "crlcore/RoutingGauge.h"
#include "etesian/EtesianEngine.h"
#include "anabatic/AutoContactTerminal.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/AnabaticEngine.h"


namespace {


  using namespace std;
  using namespace CRL;
  using namespace Hurricane;
  using namespace Anabatic;
  using Etesian::EtesianEngine;

  class CompareByLength {
    public:
      inline bool operator() ( const Segment* lhs, const Segment* rhs ) const
      {
        DbU::Unit delta = lhs->getLength() - rhs->getLength();
        if (delta < 0) return false;
        if (delta > 0) return true;
        return lhs->getId() < rhs->getId();
      }
  };

  class CompareBySegmentBox {
    public:
      inline bool operator() ( const Box& lhs, const Box& rhs ) const
      {
        bool lhsH = (lhs.getWidth() >= lhs.getHeight());
        bool rhsH = (rhs.getWidth() >= rhs.getHeight());
        if (lhsH xor rhsH) return lhsH;

        DbU::Unit lhsLength = (lhsH) ? lhs.getWidth() : lhs.getHeight();
        DbU::Unit rhsLength = (rhsH) ? rhs.getWidth() : rhs.getHeight();
        return lhsLength > rhsLength;
      }
  };


  typedef tuple<RoutingPad*,uint32_t>  RPInfosItem;

  class CompareRPInfos {
    public:
      inline bool operator () ( const RPInfosItem& lhs, const RPInfosItem& rhs ) const;
  };

  
  inline bool CompareRPInfos::operator () ( const RPInfosItem& lhs, const RPInfosItem& rhs ) const
  { return std::get<0>(lhs)->getId() < std::get<0>(rhs)->getId(); }

  
  typedef tuple<GCell*,uint32_t,GCell*>  GCellInfosItem;

  inline const uint32_t& elemFlags ( const GCellInfosItem& item ) { return std::get<1>(item); }
  inline       uint32_t& elemFlags (       GCellInfosItem& item ) { return std::get<1>(item); }
  inline       GCell*    elemGCell ( const GCellInfosItem& item ) { return std::get<0>(item); }

  class CompareGCellInfos {
    public:
      inline bool operator () ( const GCellInfosItem& lhs, const GCellInfosItem& rhs ) const;
  };
  
  inline bool CompareGCellInfos::operator () ( const GCellInfosItem& lhs, const GCellInfosItem& rhs ) const
  {
    if (std::get<1>(lhs) != std::get<1>(rhs)) return std::get<1>(lhs) < std::get<1>(rhs);
    return std::get<0>(lhs)->getId() < std::get<0>(rhs)->getId();
  }

  
  typedef set<RPInfosItem,CompareRPInfos>        RoutingPadInfos;
  typedef set<GCellInfosItem,CompareGCellInfos>  GCellArea;


  inline DbU::Unit getBoxLength ( const Box& bb )
  { return (bb.getWidth() > bb.getHeight()) ? bb.getWidth() : bb.getHeight(); }


// -----------------------------------------------------------------
// Class : "::DiodeCluster".

  class DiodeCluster {
    public:
      static const uint32_t  IsDriver;
      static const uint32_t  IsSink;
      static const uint32_t  HasDiode;
      static const uint32_t  IsSegSource;
      static const uint32_t  InCluster;
      static       string    toStr ( uint32_t );
    public:
      template<typename DerivedT>
      inline const DerivedT* as () const { return dynamic_cast<const DerivedT>( this ); }
      template<typename DerivedT>
      inline DerivedT* as () { return dynamic_cast<DerivedT>( this ); }
    public:
                                       DiodeCluster         ( AnabaticEngine*, RoutingPad* );
      virtual                         ~DiodeCluster         ();
                    DbU::Unit          getAntennaGateMaxWL  () const;
                    DbU::Unit          getAntennaDiodeMaxWL () const;
      inline        bool               hasRp                ( RoutingPad* ) const;
                    bool               hasGCell             ( GCell* ) const;
      inline        Net*               getTopNet            () const;
      inline        RoutingPad*        getRefRp             () const;
      inline        vector<GCellArea>& _getAreas            ();
      inline        AnabaticEngine*    _getAnabatic         () const;
      inline  const RoutingPadInfos&   getRoutingPads       () const;
      inline        RoutingPadInfos&   _getRoutingPads      () ;
      inline  const set<size_t>&       getNeighbors         () const;
      inline  const vector<Instance*>& getDiodes            () const;
      inline        vector<Instance*>& _getDiodes           ();
      inline        uint32_t           getForcedDiodes      () const;
      inline        DbU::Unit          getWL                () const;
      inline        DbU::Unit&         _getWL               ();
                    void               showArea             () const;
      virtual       bool               needsDiode           () const = 0;
                    Box                getBoundingBox       () const;
      inline        void               addNeighbor          ( size_t );
      inline        void               addForcedDiodes      ( uint32_t );
                    void               merge                ( GCell*, uint32_t distance, GCell* back=NULL );
      virtual       void               merge                ( RoutingPad* );
      virtual       void               merge                ( Segment* ) = 0;
                    void               mergeForcedHalo      ( size_t iarea, GCell*, uint32_t distance );
      virtual       void               mergeHalo            ( Segment*, uint32_t flags );
      virtual       void               inflateArea          (); 
                    Instance*          _createDiode         ( Etesian::Area*, const Box&, DbU::Unit uHint );
                    Instance*          _createDiode         ( Etesian::Area*, GCell*, GCell* );
      virtual const vector<Instance*>& createDiodes         ( Etesian::Area* );
                    bool               connectDiodes        ();
    private:
      AnabaticEngine*    _anabatic;
      DbU::Unit          _WL;
      RoutingPadInfos    _routingPads;
      vector<GCellArea>  _areas;
      vector<Instance*>  _diodes;
      set<size_t>        _neighbors;
      uint32_t           _forcedDiodes;
  };

  
  const uint32_t  DiodeCluster::IsDriver    = (1 << 0);
  const uint32_t  DiodeCluster::IsSink      = (1 << 1);
  const uint32_t  DiodeCluster::HasDiode    = (1 << 2);
  const uint32_t  DiodeCluster::IsSegSource = (1 << 3);
  const uint32_t  DiodeCluster::InCluster   = (1 << 4);


  string DiodeCluster::toStr ( uint32_t flags )
  {
    string s;
    s += (flags & IsDriver   ) ? 'd' : '-';
    s += (flags & IsSink     ) ? 's' : '-';
    s += (flags & HasDiode   ) ? 'D' : '-';
    s += (flags & IsSegSource) ? 'S' : '-';
    s += (flags & InCluster  ) ? 'C' : '-';
    return s;
  }


  DiodeCluster::DiodeCluster ( AnabaticEngine* anabatic, RoutingPad* rp )
    : _anabatic(anabatic)
    , _WL(0)
    , _routingPads()
    , _areas(1)
    , _diodes()
    , _neighbors()
    , _forcedDiodes(0)
  {
    merge( rp );
  }


  DiodeCluster::~DiodeCluster ()
  { }


  inline       AnabaticEngine*     DiodeCluster::_getAnabatic    () const { return _anabatic; }
  inline       vector<GCellArea>&  DiodeCluster::_getAreas       () { return _areas; }
  inline       Net*                DiodeCluster::getTopNet       () const { return getRefRp()->getNet(); }
  inline       DbU::Unit           DiodeCluster::getWL           () const { return _WL; }
  inline       DbU::Unit&          DiodeCluster::_getWL          () { return _WL; }
  inline const RoutingPadInfos&    DiodeCluster::getRoutingPads  () const { return _routingPads; }
  inline       RoutingPadInfos&    DiodeCluster::_getRoutingPads () { return _routingPads; }
  inline const vector<Instance*>&  DiodeCluster::getDiodes       () const { return _diodes; }
  inline       vector<Instance*>&  DiodeCluster::_getDiodes      () { return _diodes; }
  inline const set<size_t>&        DiodeCluster::getNeighbors    () const { return _neighbors; }
  inline       void                DiodeCluster::addNeighbor     ( size_t neighbor ) { _neighbors.insert(neighbor); }
  inline       uint32_t            DiodeCluster::getForcedDiodes () const { return _forcedDiodes; }
  inline       void                DiodeCluster::addForcedDiodes ( uint32_t count ) { _forcedDiodes += count; }


  DbU::Unit  DiodeCluster::getAntennaGateMaxWL () const
  {
    EtesianEngine* etesian = static_cast<EtesianEngine*>
      ( ToolEngine::get( _getAnabatic()->getCell(), EtesianEngine::staticGetName() ));
    return etesian->getAntennaGateMaxWL();
  }


  DbU::Unit  DiodeCluster::getAntennaDiodeMaxWL () const
  {
    EtesianEngine* etesian = static_cast<EtesianEngine*>
      ( ToolEngine::get( _getAnabatic()->getCell(), EtesianEngine::staticGetName() ));
    return etesian->getAntennaDiodeMaxWL();
  }

  
  inline bool  DiodeCluster::hasRp ( RoutingPad* rp ) const
  { return (_routingPads.find(make_tuple(rp,0)) != _routingPads.end()); }


  inline bool  DiodeCluster::hasGCell ( GCell* gcell ) const
  {
    if (not gcell) return false;
    for ( auto& item : _areas[0] ) {
      if (elemGCell(item) == gcell) return true;
    }
    return false;
  }


  inline RoutingPad* DiodeCluster::getRefRp () const
  {
    if (not _routingPads.empty()) return std::get<0>( *_routingPads.begin() );
    return NULL;
  }


  void  DiodeCluster::showArea () const
  {
    cdebug_log(147,1) << "GCell diode areas:" << endl;
    for ( size_t i=0 ; i<_areas.size() ; ++i ) {
      cdebug_log(147,0) << "GCell diode area [" << i << "]" << endl;
      for ( auto& item : _areas[i] ) {
        cdebug_log(147,0) << "  | d=" << std::get<1>(item)
                          << " "      << std::get<0>(item)
                          << " back=" << std::get<2>(item) << endl;
      }
    }
    cdebug_tabw(147,-1);
  }

  
  void  DiodeCluster::merge ( RoutingPad* rp )
  {
    Plug* rpPlug = dynamic_cast<Plug*>( rp->getPlugOccurrence().getEntity() ); 
    if (rpPlug) {
      GCell* rpGCell = nullptr;
      for ( Hook* hook : rp->getBodyHook()->getHooks() ) {
        Contact* gcontact = dynamic_cast<Contact*>( hook->getComponent() );
        if (gcontact) {
          rpGCell = _getAnabatic()->getGCellUnder( gcontact->getPosition() );
          break;
        }
      }
      if (not rpGCell) {
        cdebug_log(147,0) << "| Unable to locate GCell under, using fallback " << rp << endl;
        rpGCell = _getAnabatic()->getGCellUnder( rp->getPosition() );
      }
      DiodeCluster::merge( rpGCell, 0 );
      if (rpPlug->getMasterNet()->getDirection() & Net::Direction::DirIn) {
        cdebug_log(147,0) << "| Sink " << rp << endl;
        _getRoutingPads().insert( make_tuple(rp,IsSink) );
      } else {
        _getRoutingPads().insert( make_tuple(rp,IsDriver) );
        cdebug_log(147,0) << "| Driver " << rp << endl;
      }
    } else {
      Pin* rpPin = dynamic_cast<Pin*>( rp->getPlugOccurrence().getEntity() ); 
      if (rpPin) {
        _getRoutingPads().insert( make_tuple(rp,IsDriver) );
        cdebug_log(147,0) << "| Pin (considered driver) " << rp << endl;
      }
    }
  }


  void  DiodeCluster::merge ( GCell* gcell, uint32_t distance, GCell* back )
  {
    if (not gcell) return;
    if (hasGCell(gcell)) return;
    if (back) distance += 20;
    _areas[0].insert( make_tuple(gcell,distance,back) );
  }


  void  DiodeCluster::mergeForcedHalo ( size_t iarea, GCell* gcell, uint32_t distance )
  {
    if (not gcell) return;
    while ( iarea >= _areas.size() ) _areas.push_back( GCellArea() );
    _areas[iarea].insert( make_tuple(gcell,distance,(GCell*)NULL) );
  }


  Box  DiodeCluster::getBoundingBox () const
  {
    Box bb;
    for ( auto& item : _areas[0] ) bb.merge( elemGCell(item)->getBoundingBox() );
    return bb;
  }


  void  DiodeCluster::mergeHalo ( Segment* segment, uint32_t flags )
  {
    cerr << Error( "DiodeCluster::mergeHalo(Segment*): Unimplemented (%s). "
                 , getString(segment).c_str() ) << endl;
  }


  void  DiodeCluster::inflateArea ()
  {
    cerr << Error( "DiodeCluster::inflateArea(): Unimplemented. " ) << endl;
  }
  
  
  
  Instance* DiodeCluster::_createDiode ( Etesian::Area* area, const Box& bb, DbU::Unit uHint )
  {
    cdebug_log(147,1) << "DiodeCluster::_createDiode(): under=" << bb
                      << " uHint=" << DbU::getValueString(uHint) << endl;

    Instance* diode = area->createDiodeUnder( getRefRp(), bb, uHint );
    if (diode) {
      cdebug_log(147,0) << "| New diode " << diode << endl;
      _diodes.push_back( diode );
    }
    
    cdebug_tabw(147,-1);
    return diode;
  }

  
  Instance* DiodeCluster::_createDiode ( Etesian::Area* area, GCell* gcell, GCell* backGCell )
  {
    cdebug_log(147,1) << "DiodeCluster::_createDiode(): under=" << gcell << endl;

    Box bb = gcell->getBoundingBox(); 
    cdebug_log(147,0) << "> GCell area: " << bb << endl;

    Instance* diode = area->createDiodeUnder( getRefRp(), bb, 0 );
    if (diode) {
      cdebug_log(147,0) << "| New diode " << diode << endl;
      _diodes.push_back( diode );
      Transformation trans  = getRefRp()->getPlugOccurrence().getPath().getTransformation();
      Point          center = diode->getAbutmentBox().getCenter();
      trans.applyOn( center );
      bool     newContact = false;
      Contact* contact    = gcell->hasGContact( getTopNet() );
      if (not contact) {
        newContact = true;
        contact    = gcell->breakGoThrough( getTopNet() );
      }
      cdebug_log(147,0) << "| breakGoThrough(), contact= " << contact << endl;

      if (backGCell and newContact) {
        Contact* backContact = backGCell->breakGoThrough( getTopNet() );
        if (backContact->getY() == contact->getY()) {
          if (contact->getX() > backContact->getX())
              std::swap( backContact, contact );

          Horizontal::create( contact
                            , backContact
                            , _anabatic->getConfiguration()->getGHorizontalLayer()
                            , contact->getY()
                            , _anabatic->getConfiguration()->getGHorizontalPitch()
                            );
        } else {
          if (backContact->getX() == contact->getX()) {
            if (contact->getY() > backContact->getY())
              std::swap( backContact, contact );

            Vertical::create( contact
                            , backContact
                            , _anabatic->getConfiguration()->getGHorizontalLayer()
                            , contact->getX()
                            , _anabatic->getConfiguration()->getGHorizontalPitch()
                            );
          } else {
            cerr << Error( "DiodeCluster::_createDiode(): Back GCell not aligned with diode GCell.\n"
                           "        * %s\n"
                           "        * %s"
                         , getString(gcell).c_str()
                         , getString(backGCell).c_str()
                         ) << endl;
          }
        }
      }
    }
    cdebug_tabw(147,-1);
    return diode;
  }

  
  const vector<Instance*>& DiodeCluster::createDiodes ( Etesian::Area* area )
  {
    if (not needsDiode()) return _diodes;

    DbU::Unit antennaDiodeMaxWL = getAntennaDiodeMaxWL();
    size_t    diodeCount = getWL() / antennaDiodeMaxWL;
    if (not diodeCount) diodeCount = 1;

    showArea();

    cdebug_log(147,1) << "DiodeCluster::createDiodes() count=" << diodeCount
                      << ", forcedHalo=" << (_areas.size()-1) << endl;
    Instance* diode = NULL;
  //for ( size_t i=(_areas.size() == 1)?0:1 ; i<_areas.size() ; ++i ) {
    for ( size_t i=0 ; i<_areas.size() ; ++i ) {
      if (i) diodeCount = 1;
      cdebug_log(147,0) << "Diode for area [" << i << "]" << endl;
      for ( auto& item : _areas[i] ) {
        GCell* gcell     = std::get<0>( item );
        GCell* backGCell = std::get<2>( item );
        cdebug_log(147,0) << "| d=" << std::get<1>(item) << " " << gcell << endl;
        diode = _createDiode( area, gcell, backGCell );
        if (diode) {
          if (_diodes.size() < diodeCount) {
            diode = NULL;
            continue;
          }
          break;
        }
      }
    }
    cdebug_tabw(147,-1);

    return _diodes;
  }


  bool  DiodeCluster::connectDiodes ()
  {
    EtesianEngine* etesian = static_cast<EtesianEngine*>
      ( ToolEngine::get( _anabatic->getCell(), EtesianEngine::staticGetName() ));

    Cell* diodeCell   = etesian->getDiodeCell();
    Net*  diodeOutput = NULL;
    for ( Net* net : diodeCell->getNets() ) {
      if (net->isSupply() or not net->isExternal()) continue;
      diodeOutput = net;
      break;
    }

    Net*  topNet   = getTopNet();
    Net*  diodeNet = topNet;
    Plug* sinkPlug = dynamic_cast<Plug*>( getRefRp()->getPlugOccurrence().getEntity() );
    Path  path     = Path();

    if (sinkPlug) {
      diodeNet = sinkPlug->getNet();
      path     = getRefRp()->getOccurrence().getPath().getHeadPath();
    }

    for ( Instance* diode : _diodes ) {
      cdebug_log(147,0) << "  Bind diode input:" << endl;
      cdebug_log(147,0) << "    " << diode    << " @" << diode->getTransformation() << endl;
      cdebug_log(147,0) << "    topNet->getCell():" << topNet->getCell() << endl;
      cdebug_log(147,0) << "    " << getRefRp()->getOccurrence().getPath() << endl;
      Plug* diodePlug = diode->getPlug( diodeOutput );
      diodePlug->setNet( diodeNet );
      RoutingPad* diodeRp = RoutingPad::create( topNet, Occurrence(diodePlug,path), RoutingPad::BiggestArea );
      _getAnabatic()->getConfiguration()->selectRpComponent( diodeRp );
      cdebug_log(147,0) << "    " << getRefRp() << endl;

      GCell* gcell = _anabatic->getGCellUnder( diodeRp->getPosition() );
      if (gcell) {
        Contact* contact = gcell->breakGoThrough( topNet );
        contact->getBodyHook()->merge( diodeRp->getBodyHook() );
      }
    }

    return true;
  }


// -----------------------------------------------------------------
// Class : "::DiodeRps".

  class DiodeRps : public DiodeCluster {
    public:
                    DiodeRps    ( AnabaticEngine*, RoutingPad* );
      virtual bool  needsDiode  () const;
      virtual void  merge       ( Segment* );
      virtual void  mergeHalo   ( Segment*, uint32_t flags );
      virtual void  inflateArea (); 
  };


  DiodeRps::DiodeRps ( AnabaticEngine* anabatic, RoutingPad* rp )
    : DiodeCluster(anabatic,rp)
  { }


  bool  DiodeRps::needsDiode () const
  {
    if (getForcedDiodes()) return true;
    for ( auto& infos : getRoutingPads() ) {
      if (std::get<1>(infos) & IsSink) return true;
    }
    return false;
  }


  void  DiodeRps::merge ( Segment* segment )
  {
    _getWL() += segment->getLength();
    GCellsUnder gcells = _getAnabatic()->getGCellsUnder( segment );
    if (not gcells->empty()) {
      for ( size_t i=0 ; i<gcells->size() ; ++i ) {
        DiodeCluster::merge( gcells->gcellAt(i), 0 );
      }
    }
  }


  void  DiodeRps::mergeHalo ( Segment* segment, uint32_t flags )
  {
    cdebug_log(147,0) << "DiodeRps::mergeHalo(): " << segment << endl;
    if (not segment) return;

    // if (   (dynamic_cast<Horizontal*>(segment))
    //    and (getWL() + segment->getLength() > getAntennaGateMaxWL())) {
    if (dynamic_cast<Horizontal*>(segment)) {
      cdebug_log(147,0) << "  Put in forced halo." << segment << endl;
      GCellsUnder gcells = _getAnabatic()->getGCellsUnder( segment );
      if (not gcells->empty()) {
        size_t iarea = _getAreas().size();
        size_t count = std::min( gcells->size(), (size_t)50 );
        for ( size_t i=0 ; i<count ; ++i ) {
          size_t igcell = (flags & IsSegSource) ? i : (gcells->size()-1-i);
          DiodeCluster::mergeForcedHalo( iarea, gcells->gcellAt(igcell), i );
        }
      }
      return;
    }

    GCellsUnder gcells = _getAnabatic()->getGCellsUnder( segment );
    if (not gcells->empty()) {
      size_t count = std::min( gcells->size(), (size_t)10 );
      for ( size_t i=0 ; i<count ; ++i ) {
        size_t igcell = (flags & IsSegSource) ? i : (gcells->size()-1-i);
        DiodeCluster::merge( gcells->gcellAt(igcell), i+1 );
      }
    }
  }


  void  DiodeRps::inflateArea ()
  {
    vector< tuple<GCell*,GCell*> > border;
    for ( auto& item : _getAreas()[0] ) {
      GCell* current = std::get<0>( item );
      if (std::get<2>(item)) continue;

      GCell* neighbor = current->getEast();
      if (neighbor and not neighbor->hasNet(getTopNet()))
        border.push_back( make_tuple(neighbor,current) );
      neighbor = current->getWest();
      if (neighbor and not neighbor->hasNet(getTopNet()))
        border.push_back( make_tuple(neighbor,current) );
    }

    for ( auto& item : _getAreas()[0] ) {
      GCell* current = std::get<0>( item );
      if (std::get<2>(item)) continue;

      GCell* neighbor = current->getNorth();
      if (neighbor and not neighbor->hasNet(getTopNet()))
        border.push_back( make_tuple(neighbor,current) );
      neighbor = current->getSouth();
      if (neighbor and not neighbor->hasNet(getTopNet()))
        border.push_back( make_tuple(neighbor,current) );
    }

    for ( auto& item : border ) {
      DiodeCluster::merge( std::get<0>(item), 1, std::get<1>(item) );
    }
  }


// -----------------------------------------------------------------
// Class : "::DiodeWire".

  class DiodeWire : public DiodeCluster {
    public:
                                                   DiodeWire    ( AnabaticEngine*, RoutingPad* );
      virtual       bool                           needsDiode   () const;
      virtual       void                           merge        ( Segment* );
      virtual const vector<Instance*>&             createDiodes ( Etesian::Area* );
      inline  const set<Segment*,Go::CompareById>& getSegments  () const;
    private:
      set<Box,CompareBySegmentBox>   _boxes;
      set<Contact*,Go::CompareById>  _contacts;
      set<Segment*,Go::CompareById>  _segments;
  };

  
  inline  const set<Segment*,Go::CompareById>& DiodeWire::getSegments  () const { return _segments; }


  DiodeWire::DiodeWire ( AnabaticEngine* anabatic, RoutingPad* rp )
    : DiodeCluster(anabatic,rp)
    , _boxes()
    , _contacts()
    , _segments()
  { }


  bool  DiodeWire::needsDiode () const
  { return getWL() >= getAntennaDiodeMaxWL(); }


  void  DiodeWire::merge ( Segment* segment )
  {
    if (_segments.find(segment) != _segments.end()) return;
    Box bb = segment->getBoundingBox();
    cdebug_log(147,0) << "| merge: " << segment << endl;
    _segments.insert( segment );
    _boxes.insert( bb );
    _getWL() += segment->getLength();
  }

  
  const vector<Instance*>& DiodeWire::createDiodes ( Etesian::Area* area )
  {
    cdebug_log(147,1) << "DiodeWire::createDiodes() on " << _boxes.size() << " boxes." << endl;

    ostringstream m;
    m << "Neighbors: [";
    for ( size_t neighbor : getNeighbors() ) { m << " " << neighbor; }
    m << " ]";
    cdebug_log(147,0) << m.str() << endl;
    
    DbU::Unit antennaDiodeMaxWL = getAntennaDiodeMaxWL();

    size_t diodeCount = getWL() / antennaDiodeMaxWL;
    if (not diodeCount)  return _getDiodes();
    
    for ( const Box& bb : _boxes ) {
      bool       bbH           = (bb.getWidth() >= bb.getHeight());
      DbU::Unit  bbLength      = (bbH) ? bb.getWidth() : bb.getHeight();
      size_t     segDiodeCount = bbLength / antennaDiodeMaxWL;
      if (not segDiodeCount) ++segDiodeCount;
      bool isH = (bb.getWidth() >= bb.getHeight());
      cdebug_log(147,0) << "Processing wire diodes=" << segDiodeCount << " " << bb
                        << " isH=" << isH
                        << " length:" << DbU::getValueString(getBoxLength(bb)) << endl;
    //if (bbLength < antennaDiodeMaxWL/4) continue;

      if (bbH) {
        DbU::Unit uHint = bb.getXMin();
        DbU::Unit uMax  = bb.getXMax();
        while ( uHint < uMax ) {
          _createDiode( area, bb, uHint );
          uHint += antennaDiodeMaxWL;
        }
      } else {
        GCellsUnder gcells = _getAnabatic()->getGCellsUnder( Point(bb.getXCenter(),bb.getYMin())
                                                           , Point(bb.getXCenter(),bb.getYMax()) );
        if (gcells->size()) {
          size_t gcellPeriod = antennaDiodeMaxWL / gcells->gcellAt(0)->getHeight();
          for ( size_t i=0 ; i<gcells->size() ; ++i ) {
            Instance* diode = _createDiode( area, gcells->gcellAt(i), NULL );
            if (diode) {
              i += gcellPeriod - (i%gcellPeriod);
            }
          }
        }
      }

      if (_getDiodes().size() >= diodeCount) {
        cdebug_log(147,0) << "Added enough diodes " << _getDiodes().size() << endl;
        break;
      }
    }

    if (_getDiodes().size() < diodeCount) {
      cerr << Error( "DiodeWire::createDiode(): On cluster of net \"%s\",\n"
                     "        Allocated only %u diodes of %u."
                   , getString(getTopNet()->getName()).c_str()
                   , _getDiodes().size()
                   , diodeCount
                   ) << endl;
    }

    cdebug_tabw(147,-1);
    return _getDiodes();
  }


// -----------------------------------------------------------------
// Local functions.


}  // Anonymous namespace.


namespace Anabatic {

  using namespace Hurricane;
  using CRL::ToolEngine;
  using Etesian::EtesianEngine;


//! \function  AnabaticEngine::antennaProtect( Net* net, uint32_t& failed, uint32_t& total );
//! \param     net     The net to protect against antenna effects.
//! \param     failed  A reference to the global counter of diodes that we
//!                    where unsucessful to allocate.
//! \param     total   The total number of diode that where requesteds. 
//!                    counting both successful and unsuccessful allocations.
//!
//! \section   antennaSettings  Configuration Variable  for Antenna Effect
//!
//!            <center>
//!              <table class="UserDefined" width="50%">
//!                <tr><td>\c etesian.antennaMaxWL
//!                    <td>The maximum wirelength whitout a diode effect
//!              </table>
//!            </center>
//!
//!
//! \section   antennaAlgo  A Brief Description of the Antenna Protection Algorithm.
//!
//!            The brute force approach would be to put a diode near all sink
//!            points of the net. To reduce that number, we create clusters whose
//!            total wirelength is less than the one triggering an antenna effect.
//!
//!            The antenna protection stage is called after the global routing
//!            and before the detailed routing. The computed wirelength will be
//!            slightly inaccurate but it allow us to directly amend the global
//!            routing so the detailed router needs no modification.
//!
//!            To build the clusters:
//!
//!            <ol> 
//!               <li>Select an unreached (not part of a cluster) RoutingPad.</li>
//!               <li>Perform a depth-first search (DFS) using the segments as edges
//!                   and the Hook rings as nodes. Use a stack to store the search
//!                   state. An element of the stack is a \c tuple of
//!                   \c(Hook*,Segment*,size_t,uint32_t) :
//!
//!                   <ol>
//!                     <li>\c Hook* :    the hook of the Segment we are coming \e from.</li>
//!                     <li>\c Segment* : the segment we are to process.</li>
//!                     <li>\c size_t :   the index, in the stack, of the predecessor
//!                                       segment.</li>
//!                     <li>\c uint32_t : flags. If this segment is \b already part
//!                                       of the cluster.
//!                   </ol>
//!
//!                   All the elements are kept in the stack until the cluster is
//!                   completed. The current top of the stack is given by the \c stackTop
//!                   index.
//!
//!                   When exploring a new node (ring of Hook), all the adjacent segments
//!                   are put on top of the stack. Their suitablility is assesssed only
//!                   when they are popped up.
//!               </li>
//!               <li>When looking at a new stack element (incrementing \c stackTop, not
//!                   really popping up):
//!
//!                   <ol>
//!                     <li>If the segment length is greater than half the maximum antenna
//!                         wirelength, skip it (assume it connects two clusters).</li>
//!                     <li>If the segment length, added to the cluster total length,
//!                         is greater than the antenna length, skip it.
//!                     <li>If the segment is connected to another RoutingPad, agglomerate
//!                         this one the the cluster and merge the segment and all it's
//!                         predecessors to the cluster. Using the back index and setting
//!                         the DiodeCluster::InCluster flags.
//!                   </ol>
//!               </li>
//!               <li>When we reach the end of the stack, close the cluster and build
//!                   it's halo. Go through each elements of the stack again and look
//!                   for segments not part of it, but directly connected to it
//!                   (that is, they have not the InCluster flags set, but their
//!                   immediate predecessor has).
//!               </li>
//!            </ol> 
//!
//!            Structure of a Cluster:
//!
//!            <ol> 
//!               <li>A vector of RoutingPad.</li>
//!               <li>A set of GCells, ordered by priority (distance).
//!                   <ol> 
//!                     <li>A distance of zero means we are under the Segments belonging
//!                         to the cluster itself (directly connecting the RoutingPad).
//!                     </li>
//!                     <li>A distance between 1 to 9 means we are under the halo, that
//!                         is, Segments that <em>connects to</em> the cluster, with the
//!                         increasing distance.
//!                     </li>
//!                     <li>A distance between 10 to 19 means a GCell which is an
//!                         immediate neighbor of the core or halo segments.
//!                     </li>
//!                   </ol> 
//!               </li>
//!            </ol> 
//!
//!            We try to create the cluster's diode in the GCell of the lowest distance
//!            possible.


  void  AnabaticEngine::antennaProtect ( Net* net, uint32_t& failed, uint32_t& total )
  {
  // tuple is: Hook (S or T), From segment, back segment index, flags.
    typedef tuple<Hook*,Segment*,size_t,uint32_t> StackItem;

    DebugSession::open( net, 145, 150 );
    cdebug_log(147,1) << "Net \"" << net->getName() << endl;

    EtesianEngine* etesian = static_cast<EtesianEngine*>
      ( ToolEngine::get( getCell(), EtesianEngine::staticGetName() ));

    DbU::Unit antennaGateMaxWL = etesian->getAntennaGateMaxWL();

    vector<DiodeCluster*>                     clusters;
    map<RoutingPad*,size_t,DBo::CompareById>  rpsDone;
    map<Segment*,size_t,DBo::CompareById>     clusterSegments;
    for ( RoutingPad* rp : net->getRoutingPads() ) {
      set<Segment*,DBo::CompareById>  segmentsDone;

      if (rpsDone.find(rp) != rpsDone.end()) continue;
      
      cdebug_log(147,0) << "New Cluster [" << clusters.size() << "] from " << rp << endl;
      DiodeCluster* cluster = new DiodeRps ( this, rp );
      clusters.push_back( cluster );
      rpsDone.insert( make_pair( rp, clusters.size()-1 ) );

      size_t               stackTop = 0;
      vector< StackItem >  hooksStack;
      hooksStack.push_back( make_tuple( rp->getBodyHook()
                                      , (Segment*)NULL
                                      , 0
                                      , DiodeCluster::InCluster ) );
      while ( stackTop < hooksStack.size() ) {
        Hook*       toHook      = std::get<0>( hooksStack[stackTop] );
        Segment*    fromSegment = std::get<1>( hooksStack[stackTop] );
        DbU::Unit   branchWL    = 0;

        cdebug_log(147,0) << "| PROCESS [" << stackTop << "] " << fromSegment << endl;
        if (fromSegment) {
          if (fromSegment->getLength() > antennaGateMaxWL/2) {
            cdebug_log(147,0) << "| Long connecting wire, skipping" << endl;
            ++stackTop;
            continue;
          }

          size_t backIndex = stackTop;
          while ( backIndex ) {
            Segment* segment = std::get<1>( hooksStack[backIndex] );
            branchWL += segment->getLength();
            backIndex = std::get<2>( hooksStack[backIndex] );
            if (not backIndex) break;
            if (std::get<3>( hooksStack[backIndex] ) & DiodeCluster::InCluster) break;
          }
        }
        cdebug_log(147,0) << "| wl=" << DbU::getValueString(cluster->getWL())
                          << " + " << DbU::getValueString(branchWL) << endl;

        if (cluster->getWL() + branchWL > antennaGateMaxWL) {
          cdebug_log(147,0) << "| Cluster above maximul WL, skipping" << endl;
          ++stackTop;
          continue;
        }

        rp = NULL;
        for ( Hook* hook : toHook->getHooks() ) {
          RoutingPad* toRp = dynamic_cast<RoutingPad*>( hook->getComponent() );
          if (toRp) {
            if (rpsDone.find(toRp) == rpsDone.end()) {
              cdebug_log(147,0) << "> Agglomerate " << toRp << endl;
              cluster->merge( toRp );
              rpsDone.insert( make_pair( toRp, clusters.size()-1 ) );
              rp = toRp;
            }
          }
        }

        if (rp) {
          size_t backIndex = stackTop;
          cdebug_log(147,1) << "+ Backtrack" << endl;
          while ( backIndex ) {
            Segment* segment = std::get<1>( hooksStack[backIndex] );
            if (segment->getLength() <= branchWL) branchWL -= segment->getLength();
            else                                  branchWL  = 0;
            cluster->merge( segment );
            std::get<3>( hooksStack[backIndex] ) |= DiodeCluster::InCluster;
            clusterSegments.insert( make_pair( segment, clusters.size()-1 ) );
            cdebug_log(147,0) << "| back=" << backIndex
                              << " -> " << std::get<2>( hooksStack[backIndex] )
                              << " " << DbU::getValueString(segment->getLength())
                              << " " << segment << endl;

            backIndex = std::get<2>( hooksStack[backIndex] );
            if (std::get<3>( hooksStack[backIndex] ) & DiodeCluster::InCluster) {
              cdebug_log(147,0) << "| stop=" << backIndex << " is part of the cluster" << endl;
              break;
            }
          }
          cdebug_tabw(147,-1);
        }
        
        for ( Hook* hook : toHook->getHooks() ) {
          Segment* segment = dynamic_cast<Segment*>( hook->getComponent() );
          if (segment) {
            if (segment == fromSegment) continue;
            cdebug_log(147,0) << "| progress wl=" << DbU::getValueString(cluster->getWL())
                              << " + " << DbU::getValueString(branchWL)
                              << " / " << DbU::getValueString(segment->getLength())
                              << " " << segment << endl;
            if (segmentsDone.find(segment) != segmentsDone.end()) continue;
            segmentsDone.insert( segment );
            uint32_t flags = (segment->getSourceHook() == hook) ? DiodeCluster::IsSegSource : 0;
            if (dynamic_cast<Segment::SourceHook*>(hook)) {
              hooksStack.push_back( make_tuple( segment->getTargetHook(), segment, stackTop, flags ) );
            } else {
              hooksStack.push_back( make_tuple( segment->getSourceHook(), segment, stackTop, flags ) );
            }
          }
        }

        ++stackTop;
      }

      cdebug_log(147,0) << "Cluster halo" << endl;
      for ( size_t i=1 ; i<hooksStack.size() ; ++i ) {
        Segment* segment = std::get<1>( hooksStack[i] );
        uint32_t flags   = std::get<3>( hooksStack[i] );
        cdebug_log(147,0) << "| [" << i << "] flags=" << flags << " "  << segment << endl;
        if (not (flags & DiodeCluster::InCluster)) {
          size_t j = std::get<2>( hooksStack[i] );
          if (std::get<3>(hooksStack[j]) & DiodeCluster::InCluster) {
            cdebug_log(147,0) << "> Put in halo." << endl;
            cluster->mergeHalo( segment, flags );
          }
        }
      }
      cdebug_log(147,0) << "Cluster border" << endl;
      cluster->inflateArea();
    }

    if (clusters.size() > 1) {
      NetData*   netData        = getNetData( net );
      size_t     rpClustersSize = clusters.size();

      if (netData) {
        for ( auto item : clusterSegments ) {
          cdebug_log(147,0) << "No move up: " << item.first << endl;
        //netData->setNoMoveUp( item.first );
        }
      }

      cdebug_log(147,0) << "Cluster wiring, rpClustersSize=" << rpClustersSize << endl;
      for ( Segment* segment : net->getSegments() ) {
        if (clusterSegments.find(segment) != clusterSegments.end()) continue;

        cdebug_log(147,0) << "New Cluster [" << clusters.size()
                          << "] wiring from " << segment << endl;
        DiodeWire* cluster = new DiodeWire( this, clusters[0]->getRefRp() );
        cluster->merge( segment );
        clusters.push_back( cluster );
        clusterSegments.insert( make_pair( segment, clusters.size()-1 ) );

        size_t               stackTop = 0;
        vector< StackItem >  hooksStack;
        hooksStack.push_back( make_tuple( segment->getSourceHook()
                                        , segment
                                        , 0
                                        , DiodeCluster::IsSegSource ) );
        hooksStack.push_back( make_tuple( segment->getTargetHook()
                                        , segment
                                        , 0
                                        , 0 ) );

        while ( stackTop < hooksStack.size() ) {
          Hook*    toHook      = std::get<0>( hooksStack[stackTop] );
          Segment* fromSegment = std::get<1>( hooksStack[stackTop] );
        
          bool hasRp = false;
          for ( Hook* hook : toHook->getHooks() ) {
            RoutingPad* rp = dynamic_cast<RoutingPad*>( hook->getComponent() );
            if (rp) {
              hasRp = true;
              auto irp = rpsDone.find( rp );
              if (irp != rpsDone.end()) {
                size_t neighbor = (*irp).second;
                cdebug_log(147,0) << "| " << rp << " belongs to [" << neighbor << "]" << endl; 
                cluster->addNeighbor( neighbor );
              }
              break;
            }
          }

          if (not hasRp) {
            for ( Hook* hook : toHook->getHooks() ) {
              Segment* segment = dynamic_cast<Segment*>( hook->getComponent() );
              if (segment) {
                if (segment == fromSegment) continue;
                auto iclusterSegment = clusterSegments.find( segment ); 
                if (iclusterSegment != clusterSegments.end()) {
                  size_t neighbor = (*iclusterSegment).second;
                  cdebug_log(147,0) << "| " << segment << " belongs to [" << neighbor << "]" << endl; 
                  if (neighbor < rpClustersSize)
                    cluster->addNeighbor( neighbor );
                  continue;
                }
                clusterSegments.insert( make_pair( segment, clusters.size()-1 ) );
                cluster->merge( segment );
                uint32_t flags = (segment->getSourceHook() == hook) ? DiodeCluster::IsSegSource : 0;
                if (dynamic_cast<Segment::SourceHook*>(hook)) {
                  hooksStack.push_back( make_tuple( segment->getTargetHook(), segment, stackTop, flags ) );
                } else {
                  hooksStack.push_back( make_tuple( segment->getSourceHook(), segment, stackTop, flags ) );
                }
              }
            }
          }

          ++stackTop;
        }
      }

      total += clusters.size();
      cdebug_log(147,1) << "Net \"" << net->getName() << " has " << clusters.size() << " diode clusters." << endl;
      DbU::Unit  clustersWL = 0;
      for ( DiodeCluster* cluster : clusters ) {
        cdebug_log(147,0) << "| Cluster WL=" << DbU::getValueString(cluster->getWL())
                          << " needsDiode=" << cluster->needsDiode() << endl;
        clustersWL += cluster->getWL();
      }
      if (clustersWL < antennaGateMaxWL) {
        cdebug_log(147,0) << "Sum WL " << DbU::getValueString(clustersWL) << " below gate threshold "
                          << DbU::getValueString(antennaGateMaxWL) << ", no need of a diode." << endl;
        cdebug_tabw(147,-2);
        return;
      }
      
      size_t i = clusters.size()-1;
      while ( true ) {
        cdebug_log(147,1) << "Cluster [" << i << "] needsDiode=" << clusters[i]->needsDiode()
                          << " bb=" << clusters[i]->getBoundingBox() << endl;
        cdebug_log(147,0) << "  WL=" << DbU::getValueString(clusters[i]->getWL()) << endl;
        for ( auto& item : clusters[i]->getRoutingPads() ) {
          cdebug_log(147,0) << "| flags=" << DiodeCluster::toStr(std::get<1>(item))
                            << " " << std::get<0>(item) << endl;
        }

        if (clusters[i]->needsDiode()) {
          const vector<Instance*>& diodes = clusters[i]->createDiodes( etesian->getArea() );
          if (not diodes.empty()) {
            clusters[i]->connectDiodes();
          } else {
            cerr << Error( "EtesianEngine::antennaProtect(): For %s (rps:%u, clusters:%u)\n"
                           "        Cannot find a diode nearby %s."
                         , getString(net).c_str()
                         , rpsDone.size()
                         , clusters.size()
                         , getString(clusters[i]->getRefRp()).c_str()
                         ) << endl;
            failed += 1;
            for ( size_t icluster : clusters[i]->getNeighbors() ) {
              clusters[icluster]->addForcedDiodes( 1 );
            }
          }
        }

        cdebug_tabw(147,-1);
        if (i == 0) break;
        --i;
      }

      cdebug_tabw(147,-1);
    }

    if ((rpsDone.size() == 2) and (clusters.size() == 2)) {
      cerr << "Long bipoint " << net << endl;
    }

    for ( DiodeCluster* cluster : clusters ) delete cluster;

    cdebug_tabw(147,-1);
    DebugSession::close();
  }


  void  AnabaticEngine::antennaProtect ()
  {
  //DebugSession::open( 145, 150 );

    if (not ToolEngine::get( getCell(), EtesianEngine::staticGetName() )) {
      cerr << Warning( "AnabaticEngine::antennaProtect(): No EtesianEngine found, skipped." ) << endl;
      return;
    }
    EtesianEngine* etesian = static_cast<EtesianEngine*>
      ( ToolEngine::get( getCell(), EtesianEngine::staticGetName() ));

    if (not etesian->getDiodeCell()) {
      cerr << Warning( "AnabaticEngine::antennaProtect(): No diode cell found, skipped." ) << endl;
      return;
    }
    if (etesian->getAntennaDiodeMaxWL() <= 0) {
      cerr << Warning( "AnabaticEngine::antennaProtect(): Maximum antenna diode WL not set, skipped." ) << endl;
      return;
    }

    DbU::Unit segmentMaxWL = etesian->getAntennaDiodeMaxWL() / 2;

    cmess1 << "  o  Antenna effect protection." << endl;
    startMeasures();
    openSession();

    uint32_t failed = 0;
    uint32_t total  = 0;
    for ( Net* net : getCell()->getNets() ) {
      if (net->isSupply()) continue;
      if (  NetRoutingExtension::isManualDetailRoute(net)
         or NetRoutingExtension::isFixed(net))
        continue;
      antennaProtect( net, failed, total );
    }
    cmess2 << Dots::asString    ( "     - Antenna gate maximum WL"   , DbU::getValueString(etesian->getAntennaGateMaxWL()) ) << endl;
    cmess2 << Dots::asString    ( "     - Antenna diode maximum WL"  , DbU::getValueString(etesian->getAntennaDiodeMaxWL()) ) << endl;
    cmess2 << Dots::asString    ( "     - Antenna segment maximum WL", DbU::getValueString(segmentMaxWL) ) << endl;
    cmess1 << Dots::asInt       ( "     - Total needed diodes", total  ) << endl;
    cmess1 << Dots::asInt       ( "     - Failed to allocate" , failed ) << endl;
    cmess1 << Dots::asPercentage( "     - Success ratio"      , (float)(total-failed)/(float)total ) << endl;

    stopMeasures();
    printMeasures( "antennas" );

    Session::close();
  //DebugSession::close();
    Breakpoint::stop( 100, "After diodes insertions." );
  }


}  // Anabatic namespace.
