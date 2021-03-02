// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./FlattenPower.cpp"                       |
// +-----------------------------------------------------------------+


#include <map>
#include <list>
#include "vlsisapd/configuration/Configuration.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Pad.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Rectilinear.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/NetRoutingProperty.h"
#include "hurricane/Instance.h"
#include "hurricane/Plug.h"
#include "hurricane/Path.h"
#include "hurricane/Query.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/RoutingGauge.h"
#include "etesian/EtesianEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using CRL::AllianceFramework;
  using CRL::RoutingGauge;
  using CRL::RoutingLayerGauge;


// -------------------------------------------------------------------
// Local Functions.

  void  destroyRing ( Net* net )
  {
    for( RoutingPad* rp : net->getRoutingPads() ) {
      bool          allMasters = true;
      vector<Hook*> ring;
      for( Hook* hook : rp->getBodyHook()->getHooks() ) {
        if (not hook->isMaster()) { allMasters = false; break; }
        ring.push_back( hook );
      }
      if (allMasters) {
        for ( auto hook : ring ) {
          hook->_setNextHook( hook );
        }
      }
    }
  }


// -------------------------------------------------------------------
// Class  :  "::GlobalNetTable".

  class GlobalNetTable {
    public:
                   GlobalNetTable       ( Cell* );
      inline Cell* getTopCell           () const;
             Net*  getRootNet           ( const Net*, Path ) const;
      inline Net*  getVdd               () const;
      inline Net*  getVss               () const;
      inline Net*  getBlockage          () const;
      inline void  setBlockage          ( Net* );
    private:                            
             bool  guessGlobalNet       ( const Name&, Net* );
    private:
      uint32_t  _flags;
      Name      _vddCoreName;
      Name      _vssCoreName;
      Net*      _vdd;
      Net*      _vss;
      Net*      _blockage;
      Cell*     _topCell;
  };


  inline Cell* GlobalNetTable::getTopCell  () const { return _topCell; }
  inline Net*  GlobalNetTable::getVdd      () const { return _vdd; }
  inline Net*  GlobalNetTable::getVss      () const { return _vss; }
  inline Net*  GlobalNetTable::getBlockage () const { return _blockage; }
  inline void  GlobalNetTable::setBlockage ( Net* net ) { _blockage=net; }

  GlobalNetTable::GlobalNetTable ( Cell* topCell )
    : _flags      (0)
    , _vddCoreName()
    , _vssCoreName()
    , _vdd        (NULL)
    , _vss        (NULL)
    , _blockage   (NULL)
    , _topCell    (topCell)
  {
    if (_topCell == NULL) return;

    cmess1 << "  o  Looking for powers/grounds & clocks." << endl;

    for( Net* net : _topCell->getNets() ) {
      Net::Type netType = net->getType();

      if (NetRoutingExtension::isManualDetailRoute(net)) continue;
      if (NetRoutingExtension::isManualGlobalRoute(net)) continue;

      if (netType == Net::Type::POWER) {
        if (_vddCoreName.isEmpty()) {
          _vddCoreName = net->getName();
          _vdd         = net;
        } else {
          cerr << Error("Second power supply net <%s> net at top block level will be ignored.\n"
                        "        (will consider only <%s>)"
                       , getString(net ->getName()).c_str()
                       , getString(_vdd->getName()).c_str()
                       ) << endl;
        }
      }

      if (netType == Net::Type::GROUND) {
        if (_vssCoreName.isEmpty()) {
          _vssCoreName = net->getName();
          _vss         = net;
        } else {
          cerr << Error("Second power ground net <%s> net at top block level will be ignored.\n"
                        "        (will consider only <%s>)"
                       , getString(net ->getName()).c_str()
                       , getString(_vss->getName()).c_str()
                       ) << endl;
        }
      }
    }
    
    if (_vdd == NULL) cerr << Error("Missing POWER net at top block level." ) << endl;
    else destroyRing( _vdd );
    if (_vss == NULL) cerr << Error("Missing GROUND net at top block level." ) << endl;
    else destroyRing( _vss );
  }


  bool  GlobalNetTable::guessGlobalNet ( const Name& name, Net* net )
  {
    if (name == _vddCoreName) {
      cmess1 << "        - Using <" << net->getName() << "> as core (internal:vdd) power net." << endl;
      _vdd = net;
      return true;
    }

    if (name == _vssCoreName) {
      cmess1 << "        - Using <" << net->getName() << "> as core (internal:vss) ground net." << endl;
      _vss = net;
      return true;
    }

    return false;
  }


  Net* GlobalNetTable::getRootNet ( const Net* net, Path path ) const
  {
    cdebug_log(159,0) << "    getRootNet:" << path << ":" << net << endl;

    if (net == _blockage) return _blockage;
    if (net->getType() == Net::Type::POWER ) return _vdd;
    if (net->getType() == Net::Type::GROUND) return _vss;
    return NULL;
  }

  
  class Rails;
  class Plane;


// -------------------------------------------------------------------
// Class  :  "::Rail".

  class Rail {
    public:
                                Rail          ( Rails*, DbU::Unit axis, DbU::Unit width );
      inline DbU::Unit          getAxis       () const;
      inline DbU::Unit          getWidth      () const;
      inline Rails*             getRails      () const;
      inline uint32_t           getDirection  () const;
      inline RoutingLayerGauge* getLayerGauge () const;
      inline Net*               getNet        () const;
      inline const Layer*       getLayer      () const;
             void               merge         ( DbU::Unit source, DbU::Unit target );
             void               doLayout      ( const Layer* );
             string             _getString    () const;
    private:
      Rails*         _rails;
      DbU::Unit      _axis;
      DbU::Unit      _width;
      list<Interval> _chunks;
  };


  void  Rail::merge ( DbU::Unit source, DbU::Unit target )
  {
    Interval chunkToMerge ( source, target );
    cdebug_log(159,0) << "    Rail::merge() "
                << ((getDirection()==Constant::Horizontal) ? "Horizontal" : "Vertical")
                << " " << chunkToMerge << endl;
    cdebug_log(159,0) << "    | " << _getString() << endl;

    list<Interval>::iterator imerge = _chunks.end();
    list<Interval>::iterator ichunk = _chunks.begin();

    while ( ichunk != _chunks.end() ) {
      if (imerge == _chunks.end()) {
        if (chunkToMerge.getVMax() < (*ichunk).getVMin()) {
          cdebug_log(159,0) << "    | Insert before " << *ichunk << endl;
          imerge = _chunks.insert( ichunk, chunkToMerge );
          break;
        }

        if (chunkToMerge.intersect(*ichunk)) {
          cdebug_log(159,0) << "    | Merge with " << *ichunk << endl;
          imerge = ichunk;
          (*imerge).merge( chunkToMerge );
        }
      } else {
        if (chunkToMerge.getVMax() >= (*ichunk).getVMin()) {
          (*imerge).merge( *ichunk );
          cdebug_log(159,0) << "    | Absorb (erase) " << *ichunk << endl;
          ichunk = _chunks.erase( ichunk );
          continue;
        } else
          break;
      }
      ++ichunk;
    }

    if (imerge == _chunks.end()) {
      _chunks.insert( ichunk, chunkToMerge );
      cdebug_log(159,0) << "    | Insert at end " << DbU::getValueString(_axis) << " " << chunkToMerge << endl;
      cdebug_log(159,0) << "    | " << _getString() << endl;
    }
  }


  void  Rail::doLayout ( const Layer* layer )
  {
    cdebug_log(159,0) << "Doing layout of rail: "
                << " " << layer->getName()
                << " " << ((getDirection()==Constant::Horizontal) ? "Horizontal" : "Vertical")
                << " @" << DbU::getValueString(_axis) << endl;
    cdebug_log(159,0) << _getString() << endl;

    Net*               net       = getNet();
    RoutingLayerGauge* rg        = getLayerGauge();
    Segment*           segment   = NULL;
    DbU::Unit          extension = layer->getExtentionCap();

    cdebug_log(159,0) << " (pitch:" << DbU::getValueString(rg->getPitch())
                      << " , ww/2:" << DbU::getValueString(rg->getHalfWireWidth())
                      << ")" << endl;

    // if ( type == Constant::PinOnly ) {
    //   cdebug_log(159,0) << "  Layer is PinOnly." << endl;
    //   return;
    // }

    if ( getDirection() == Constant::Horizontal ) {
      list<Interval>::iterator ichunk     = _chunks.begin();
      list<Interval>::iterator ichunknext = ichunk;
      ++ichunknext;

      for ( ; ichunk != _chunks.end() ; ++ichunk, ++ichunknext ) {

        if (ichunknext != _chunks.end()) {
          if ((*ichunk).intersect(*ichunknext))
            cerr << Error( "Overlaping consecutive chunks in %s %s Rail @%s:\n"
                           "  %s"
                         , getString(layer->getName()).c_str()
                         , ((getDirection()==Constant::Horizontal) ? "Horizontal" : "Vertical")
                         , DbU::getValueString(_axis).c_str()
                         , _getString().c_str()
                         ) << endl;
        }
        
        cdebug_log(159,0) << "  chunk: [" << DbU::getValueString((*ichunk).getVMin())
                          << ":" << DbU::getValueString((*ichunk).getVMax()) << "]" << endl;

        segment = Horizontal::create ( net
                                     , layer
                                     , _axis
                                     , _width
                                     , (*ichunk).getVMin()+extension
                                     , (*ichunk).getVMax()-extension
                                     );
        if ( segment and net->isExternal() )
          NetExternalComponents::setExternal ( segment );
      }
    } else {
      list<Interval>::iterator ichunk = _chunks.begin();
      for ( ; ichunk != _chunks.end() ; ichunk++ ) {
        cdebug_log(159,0) << "  chunk: [" << DbU::getValueString((*ichunk).getVMin())
                    << ":" << DbU::getValueString((*ichunk).getVMax()) << "]" << endl;

        segment = Vertical::create ( net
                                   , layer
                                   , _axis
                                   , _width
                                   , (*ichunk).getVMin()+extension
                                   , (*ichunk).getVMax()-extension
                                   );
        if ( segment and net->isExternal() )
          NetExternalComponents::setExternal ( segment );
      }
    }
  }


  string   Rail::_getString () const
  {
    ostringstream os;

    os << "<Rail " << ((getDirection()==Constant::Horizontal) ? "Horizontal" : "Vertical")
       << " @"  << DbU::getValueString(_axis)  << " "
       << " w:" << DbU::getValueString(_width) << " ";
    list<Interval>::const_iterator ichunk = _chunks.begin();
    for ( ; ichunk != _chunks.end() ; ++ichunk ) {
      if (ichunk != _chunks.begin()) os << " ";
      os << "[" << DbU::getValueString((*ichunk).getVMin())
         << " " << DbU::getValueString((*ichunk).getVMax()) << "]";
    }
    os << ">";
    return os.str();
  }


// -------------------------------------------------------------------
// Class  :  "::RailCompare" & "::RailMatch".

  class RailCompare {
    public:
      bool operator() ( const Rail* lhs, const Rail* rhs );
  };


  inline bool  RailCompare::operator() ( const Rail* lhs, const Rail* rhs )
  {
    if ( lhs->getAxis () < rhs->getAxis () ) return true;
    if ( lhs->getWidth() < rhs->getWidth() ) return true;
    return false;
  }


  class RailMatch : public unary_function<Rail*,bool> {
    public:
      inline      RailMatch  ( DbU::Unit axis, DbU::Unit width );
      inline bool operator() ( const Rail* );
    private:
      DbU::Unit  _axis;
      DbU::Unit  _width;
  };


  inline  RailMatch::RailMatch ( DbU::Unit axis, DbU::Unit width )
    : _axis(axis)
    , _width(width)
  { }


  inline bool  RailMatch::operator() ( const Rail* rail )
  { return (rail->getAxis() == _axis) and (rail->getWidth() == _width); }


// -------------------------------------------------------------------
// Class  :  "::Rails".

  class Rails {
    public:
                                Rails          ( Plane*, uint32_t direction, Net* );
                               ~Rails          ();
      inline Plane*             getPlane       ();
      inline uint32_t           getDirection   () const;
      inline RoutingLayerGauge* getLayerGauge  () const;
      inline Net*               getNet         () const;
             void               merge          ( const Box& );
             void               doLayout       ( const Layer* );
    private:
      Plane*         _plane;
      uint32_t       _direction;
      Net*           _net;
      vector<Rail*>  _rails;
  };


// Delayed "::Rail" functions.

  Rail::Rail ( Rails* rails, DbU::Unit axis, DbU::Unit width )
    : _rails (rails)
    , _axis  (axis)
    , _width (width)
    , _chunks()
  {
    cdebug_log(159,0) << "    new Rail "
                << " @" << DbU::getValueString(axis)
                      << " " << getLayer()->getName()
                << " " << getRails()->getNet()
                << " " << ((getDirection()==Constant::Horizontal) ? "Horizontal" : "Vertical")<< endl;
  }

  inline DbU::Unit          Rail::getAxis       () const { return _axis; }
  inline DbU::Unit          Rail::getWidth      () const { return _width; }
  inline Rails*             Rail::getRails      () const { return _rails; }
  inline uint32_t           Rail::getDirection  () const { return _rails->getDirection(); }
  inline RoutingLayerGauge* Rail::getLayerGauge () const { return _rails->getLayerGauge(); }
  inline Net*               Rail::getNet        () const { return _rails->getNet(); }

// Delayed "::Rail" functions.
 

  Rails::Rails ( Plane* plane, uint32_t direction , Net* net )
    : _plane         (plane)
    , _direction     (direction)
    , _net           (net)
    , _rails         ()
  {
    cdebug_log(159,0) << "  new Rails @"
                << " " << getLayerGauge()->getLayer()->getName()
                << " " << net
                << " " << ((getDirection()==Constant::Horizontal) ? "Horizontal": "Vertical") << endl;
  }


  Rails::~Rails ()
  {
    while ( not _rails.empty() ) {
      delete (*_rails.begin());
      _rails.erase ( _rails.begin() );
    }
  }


  inline Plane*    Rails::getPlane       () { return _plane; }
  inline uint32_t  Rails::getDirection   () const { return _direction; }
  inline Net*      Rails::getNet         () const { return _net; }


  void  Rails::merge ( const Box& bb )
  {
    DbU::Unit  axis;
    DbU::Unit  width;
    DbU::Unit  sourceU;
    DbU::Unit  targetU;

    if (getDirection() == Constant::Horizontal) {
      axis    = bb.getYCenter();
      width   = bb.getHeight();
      sourceU = bb.getXMin();
      targetU = bb.getXMax();
    } else {
      axis    = bb.getXCenter();
      width   = bb.getWidth();
      sourceU = bb.getYMin();
      targetU = bb.getYMax();
    }

    vector<Rail*>::iterator irail = find_if ( _rails.begin(), _rails.end(), RailMatch(axis,width) );

    Rail* rail = NULL;
    if ( irail == _rails.end() ) {
      rail = new Rail(this,axis,width);
      _rails.push_back ( rail );
      stable_sort ( _rails.begin(), _rails.end(), RailCompare() );
    } else {
      rail = *irail;
    }

    rail->merge ( sourceU, targetU );
  }


  void  Rails::doLayout ( const Layer* layer )
  {
    cdebug_log(159,0) << "Doing layout of rails: " << layer->getName()
                << " " << ((_direction==Constant::Horizontal) ? "Horizontal" : "Vertical")
                << " " << _net->getName() << endl;

    for ( size_t irail=0 ; irail<_rails.size() ; irail++ )
      _rails[irail]->doLayout ( layer );
  }


// -------------------------------------------------------------------
// Class  :  "::Plane".

  class Plane {
    public:
      typedef  map<Net*,Rails*,Net::CompareById>  RailsMap;
    public:
                                Plane             ( const Layer*, RoutingLayerGauge* );
                               ~Plane             ();
      inline const Layer*       getLayer          () const;
      inline uint32_t           getDirection      () const;
      inline RoutingLayerGauge* getLayerGauge     () const;
      inline uint32_t           getPowerDirection () const;
             void               merge             ( const Box&, Net* );
             void               doLayout          ();
    private:
      const Layer*       _layer;
      RoutingLayerGauge* _layerGauge;
      RailsMap           _horizontalRails;
      RailsMap           _verticalRails;
      uint32_t           _powerDirection;
  };



// Delayed ::Rails function.

  inline const Layer* Rail::getLayer () const { return _rails->getPlane()->getLayer(); }

  inline RoutingLayerGauge* Rails::getLayerGauge  () const { return _plane->getLayerGauge(); }

// Delayed ::Rails function.


  Plane::Plane ( const Layer* layer, RoutingLayerGauge* layerGauge )
    : _layer          (layer)
    , _layerGauge     (layerGauge)
    , _horizontalRails()
    , _verticalRails  ()
    , _powerDirection (layerGauge->getDirection())
  {
    cdebug_log(159,0) << "New Plane " << _layer->getName() << " " << _layerGauge << endl;

  // Hard-coded SxLib gauge.
    if (_layerGauge->getDepth() == 0) _powerDirection = Constant::Horizontal;
  }


  Plane::~Plane ()
  {
    RailsMap::iterator irail = _horizontalRails.begin();
    for ( ; irail != _horizontalRails.end() ; ++irail ) {
      delete (*irail).second;
    }
    irail = _verticalRails.begin();
    for ( ; irail != _verticalRails.end() ; ++irail ) {
      delete (*irail).second;
    }
  }


  inline const Layer*       Plane::getLayer           () const { return _layer; }
  inline RoutingLayerGauge* Plane::getLayerGauge      () const { return _layerGauge; }
  inline uint32_t           Plane::getDirection       () const { return _layerGauge->getDirection(); }
  inline uint32_t           Plane::getPowerDirection  () const { return _powerDirection; }

  void  Plane::merge ( const Box& bb, Net* net )
  {
    Rails* rails = NULL;

    cdebug_log(159,0) << "    Plane::merge() " << net->getName() << " " << bb << endl;

    uint32_t direction = getDirection();
    if ( (net->getType() == Net::Type::POWER) or (net->getType() == Net::Type::GROUND) )
      direction = getPowerDirection();

    if (direction == Constant::Horizontal) {
      RailsMap::iterator irails = _horizontalRails.find(net);
      if ( irails == _horizontalRails.end() ) {
        rails = new Rails( this, Constant::Horizontal, net );
        _horizontalRails.insert ( make_pair(net,rails) );
      } else
        rails = (*irails).second;

      rails->merge ( bb );
    } else {
      RailsMap::iterator irails = _verticalRails.find(net);
      if ( irails == _verticalRails.end() ) {
        rails = new Rails( this, Constant::Vertical, net );
        _verticalRails.insert ( make_pair(net,rails) );
      } else
        rails = (*irails).second;

      cdebug_log(159,0) << "    Vertical Merging" << endl; 
      rails->merge ( bb );
    }
  }


  void  Plane::doLayout ()
  {
    cdebug_log(159,0) << "Doing layout of plane: " << _layer->getName() << endl;

    RailsMap::iterator irails = _horizontalRails.begin();
    for ( ; irails != _horizontalRails.end() ; ++irails ) {
      (*irails).second->doLayout(_layer);
    }
    irails = _verticalRails.begin();
    for ( ; irails != _verticalRails.end() ; ++irails ) {
      (*irails).second->doLayout(_layer);
    }
  }


// -------------------------------------------------------------------
// Class  :  "::PowerRailsPlanes".

  class PowerRailsPlanes {
    public:
      typedef  map<const BasicLayer*,Plane*,BasicLayer::CompareByMask>  PlanesMap;
    public:
                    PowerRailsPlanes       ( Cell*, RoutingGauge* );
                   ~PowerRailsPlanes       ();
      inline Net*   getRootNet             ( Net*, Path );
             bool   hasPlane               ( const BasicLayer* );
      inline void   setBlockageNet         ( Net* );
             bool   setActivePlane         ( const BasicLayer* );
      inline Plane* getActivePlane         () const;
      inline Plane* getActiveBlockagePlane () const;
             void   merge                  ( const Box&, Net* );
             void   doLayout               ();
    private:
      Cell*           _topCell;
      GlobalNetTable  _globalNets;
      PlanesMap       _planes;
      Plane*          _activePlane;
      Plane*          _activeBlockagePlane;
  };


  PowerRailsPlanes::PowerRailsPlanes ( Cell* topCell, RoutingGauge* rg )
    : _topCell            (topCell)
    , _globalNets         (topCell)
    , _planes             ()
    , _activePlane        (NULL)
    , _activeBlockagePlane(NULL)
  {
    for ( RoutingLayerGauge* lg : rg->getLayerGauges() ) {
      cdebug_log(159,0) << "Gauge: [" << lg->getDepth() << "] " << lg << endl;

      const BasicLayer* basicLayer = dynamic_cast<const BasicLayer*>( lg->getLayer() );
      if (not basicLayer)
        basicLayer = dynamic_cast<const RegularLayer*>( lg->getLayer() )->getBasicLayer();

      _planes.insert( make_pair(basicLayer,new Plane(lg->getLayer(),lg)) );

    //if (lg->getType() == Constant::PinOnly) continue;
      const BasicLayer* blockageLayer = dynamic_cast<const BasicLayer*>( lg->getBlockageLayer() );
      if (not blockageLayer) continue;

      _planes.insert( make_pair(blockageLayer,new Plane(blockageLayer,lg)) );
    }
  }


  PowerRailsPlanes::~PowerRailsPlanes ()
  {
    while ( not _planes.empty() ) {
      delete _planes.begin()->second;
      _planes.erase ( _planes.begin() );
    }
  }

  
  inline Net* PowerRailsPlanes::getRootNet ( Net* net, Path path )
  { return _globalNets.getRootNet(net,path); }


  bool  PowerRailsPlanes::hasPlane ( const BasicLayer* layer )
  { return (_planes.find(layer) != _planes.end()); }


  inline void  PowerRailsPlanes::setBlockageNet ( Net* blockageNet )
  { _globalNets.setBlockage( blockageNet ); }


  bool  PowerRailsPlanes::setActivePlane ( const BasicLayer* layer )
  {
    PlanesMap::iterator iplane = _planes.find(layer);
    if (iplane == _planes.end()) return false;

    _activePlane         = iplane->second;
    _activeBlockagePlane = NULL;
    if (layer->getMaterial() != BasicLayer::Material::blockage) {
      BasicLayer* blockageLayer = layer->getBlockageLayer();
      PlanesMap::iterator ibplane = _planes.find(blockageLayer);
      if (ibplane != _planes.end())
        _activeBlockagePlane = ibplane->second;
    }
    return true;
  }


  inline Plane* PowerRailsPlanes::getActivePlane () const
  { return _activePlane; }


  inline Plane* PowerRailsPlanes::getActiveBlockagePlane () const
  { return _activeBlockagePlane; }


  void  PowerRailsPlanes::merge ( const Box& bb, Net* net )
  {
    if (not _activePlane) return;

    Net* topGlobalNet = _globalNets.getRootNet( net, Path() );
    if (topGlobalNet == NULL) {
      cdebug_log(159,0) << "Not a global net: " << net << endl;
      return;
    }

    if ( (topGlobalNet == _globalNets.getBlockage()) and (_activeBlockagePlane != NULL) )
      _activeBlockagePlane->merge( bb, topGlobalNet );
    else
      _activePlane->merge( bb, topGlobalNet );
  }


  void  PowerRailsPlanes::doLayout ()
  {
    PlanesMap::iterator iplane = _planes.begin();
    for ( ; iplane != _planes.end() ; iplane++ )
      iplane->second->doLayout ();
  }



// -------------------------------------------------------------------
// Class  :  "::QueryPowerRails".

  class QueryPowerRails : public Query {
    public:
                            QueryPowerRails     ( Cell*, RoutingGauge* );
      virtual bool          hasGoCallback       () const;
      virtual void          setBasicLayer       ( const BasicLayer* );
      virtual bool          hasBasicLayer       ( const BasicLayer* );
      inline  Net*          getBlockageNet      () const;
      virtual void          goCallback          ( Go* );
      virtual void          rubberCallback      ( Rubber* );
      virtual void          extensionGoCallback ( Go* );
      virtual void          masterCellCallback  ();
              void          addToPowerRail      ( const Go*              go
                                                , const BasicLayer*      basicLayer
                                                , const Box&             area
                                                , const Transformation&  transformation
                                                );
      virtual void          doQuery             ();
      inline  void          doLayout            ();
      inline  uint32_t      getGoMatchCount     () const;
    private:
      AllianceFramework*      _framework;
      Cell*                   _topCell;
      Net*                    _blockageNet;
      RoutingGauge*           _routingGauge;
      PowerRailsPlanes        _powerRailsPlanes;
      bool                    _isBlockagePlane;
      uint32_t                _goMatchCount;
  };


  QueryPowerRails::QueryPowerRails ( Cell* topCell, RoutingGauge* rg )
    : Query            ()
    , _framework       (AllianceFramework::get())
    , _topCell         (topCell)
    , _blockageNet     (topCell->getNet("blockagenet"))
    , _routingGauge    (rg)
    , _powerRailsPlanes(topCell,rg)
    , _isBlockagePlane (false)
    , _goMatchCount    (0)
  {
    setCell       ( topCell );
    setArea       ( topCell->getAbutmentBox() );
    setBasicLayer ( NULL );
    setFilter     ( Query::DoTerminalCells|Query::DoComponents );

    if (not _blockageNet) {
      _blockageNet = Net::create( topCell, "blockagenet" );
      _blockageNet->setType( Net::Type::BLOCKAGE );
    }
    _powerRailsPlanes.setBlockageNet( _blockageNet );

    cmess1 << "  o  Building power rails." << endl;
  }


  inline  uint32_t  QueryPowerRails::getGoMatchCount () const
  { return _goMatchCount; }


  inline Net* QueryPowerRails::getBlockageNet () const
  { return _blockageNet; }


  inline  void  QueryPowerRails::doLayout ()
  { return _powerRailsPlanes.doLayout(); }


  bool  QueryPowerRails::hasBasicLayer ( const BasicLayer* basicLayer )
  { return _powerRailsPlanes.hasPlane ( basicLayer ); }


  void  QueryPowerRails::setBasicLayer ( const BasicLayer* basicLayer )
  {
    _isBlockagePlane = (basicLayer) and (basicLayer->getMaterial() == BasicLayer::Material::blockage);
    _powerRailsPlanes.setActivePlane ( basicLayer );
    Query::setBasicLayer ( basicLayer );
  }


  void  QueryPowerRails::doQuery ()
  {
    Plane* activePlane = _powerRailsPlanes.getActivePlane();

    if (not activePlane) return;

    cmess1 << "     - PowerRails in " << activePlane->getLayer()->getName() << " ..." << endl;
    Query::doQuery();
  }


  void  QueryPowerRails::masterCellCallback ()
  { }


  bool  QueryPowerRails::hasGoCallback () const
  { return true; }


  void  QueryPowerRails::goCallback ( Go* go )
  {
  //ltrace(80) << "QueryPowerRails::goCallback() " << go->getId() << ":" << go
  //           << " " << getPath().getName() << endl;
    addToPowerRail ( go, getBasicLayer(), getArea(), getTransformation() );
  }


  void  QueryPowerRails::addToPowerRail ( const Go*              go
                                        , const BasicLayer*      basicLayer
                                        , const Box&             area
                                        , const Transformation&  transformation
                                        )
  {
    cdebug_log(159,0) << "addToPowerRail: go=" << go << endl;
    const Component* component = dynamic_cast<const Component*>( go );
    if (component) {
      if (    _framework->isPad(getMasterCell())
         and ( (_routingGauge->getLayerDepth(component->getLayer()) < 2)
             or (component->getLayer()->getBasicLayers().getFirst()->getMaterial()
                != BasicLayer::Material::blockage) ) )
        return;

      Net* rootNet = _blockageNet;
      if (not _isBlockagePlane) {
        rootNet = _powerRailsPlanes.getRootNet(component->getNet(),getPath());
        if (not NetExternalComponents::isExternal(component)) return;
      }
      if (not rootNet) {
        cdebug_log(159,0) << "  rootNet is NULL, not taken into account." << endl;
        return;
      }
      cdebug_log(159,0) << "  rootNet " << rootNet << " "
                        << go->getCell() << " (" << go->getCell()->isTerminal() << ")" << endl;

      const Segment* segment = dynamic_cast<const Segment*>( component );
      if (segment) {
        _goMatchCount++;
        cdebug_log(159,0) << "  Merging PowerRail element: " << segment << endl;

        Box bb = segment->getBoundingBox( basicLayer );
        transformation.applyOn( bb );
        _powerRailsPlanes.merge( bb, rootNet );
      } else {
        const Contact* contact = dynamic_cast<const Contact*>( component );
        if (contact) {
          _goMatchCount++;

          Box bb = contact->getBoundingBox( basicLayer );
          transformation.applyOn( bb );

          cdebug_log(159,0) << "  Merging PowerRail element: " << contact << " bb:" << bb
                      << " " << basicLayer << endl;
          
          _powerRailsPlanes.merge( bb, rootNet );
        } else {
          const Pad* pad = dynamic_cast<const Pad*>( component );
          if (pad) {
            _goMatchCount++;

            Box bb = pad->getBoundingBox( basicLayer );
            transformation.applyOn( bb );
          
            cdebug_log(159,0) << "  Merging PowerRail element: " << pad << " bb:" << bb
                              << " " << basicLayer << endl;
            
            _powerRailsPlanes.merge( bb, rootNet );
          } else {
            const Rectilinear* rectilinear = dynamic_cast<const Rectilinear*>( component );
            if (rectilinear and (rectilinear->getPoints().size() == 5)) {
              _goMatchCount++;

              Box bb = rectilinear->getBoundingBox( basicLayer );
              transformation.applyOn( bb );
          
              cdebug_log(159,0) << "  Merging PowerRail element: " << rectilinear << " bb:" << bb
                                << " " << basicLayer << endl;
            
              _powerRailsPlanes.merge( bb, rootNet );
            }
          }
        }
      }
    }
  }


  void  QueryPowerRails::rubberCallback ( Rubber* )
  { }


  void  QueryPowerRails::extensionGoCallback ( Go* )
  { }


} // anonymous namespace.


namespace Etesian {


  using Hurricane::UpdateSession;
  using Hurricane::DataBase;
  using Hurricane::Technology;
  using Hurricane::BasicLayer;


  void  EtesianEngine::flattenPower ()
  {
    string gaugeName = Cfg::getParamString("anabatic.routingGauge","sxlib")->asString();
    RoutingGauge* rg = AllianceFramework::get()->getRoutingGauge( gaugeName );
    if (not rg) 
      throw Error( "EtesianEngine::falttenPower(): No routing gauge named \"%s\"", gaugeName.c_str() );

  //DebugSession::open( 150, 160 );
    UpdateSession::open();

    Technology*     technology = DataBase::getDB()->getTechnology();
    QueryPowerRails query ( getCell(), rg );
    query.setStopCellFlags( Cell::Flags::AbstractedSupply );

    for ( BasicLayer* layer : technology->getBasicLayers() ) {
      if (   (layer->getMaterial() != BasicLayer::Material::metal)
         and (layer->getMaterial() != BasicLayer::Material::blockage) )
        continue;
      if (not query.hasBasicLayer(layer)) continue;

      query.setBasicLayer( layer );
      query.doQuery      ();
    }
    query.doLayout();
    cmess1 << "     - " << query.getGoMatchCount() << " power rails elements found." << endl;

    getCell()->setFlags( Cell::Flags::AbstractedSupply );

    UpdateSession::close();
  //DebugSession::close();
  }


} // Etesian namespace.
