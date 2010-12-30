
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./BuildPowerRails.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <map>
#include  <list>

#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/BasicLayer.h"
#include  "hurricane/RegularLayer.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/NetExternalComponents.h"
#include  "hurricane/Instance.h"
#include  "hurricane/Plug.h"
#include  "hurricane/Path.h"
#include  "hurricane/Query.h"
#include  "crlcore/AllianceFramework.h"
#include  "kite/RoutingPlane.h"
#include  "kite/TrackFixedSegment.h"
#include  "kite/Track.h"
#include  "kite/KiteEngine.h"


namespace {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ForEachIterator;
  using Hurricane::Warning;
  using Hurricane::Error;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Interval;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::NetExternalComponents;
  using Hurricane::Instance;
  using Hurricane::Plug;
  using Hurricane::Path;
  using Hurricane::Query;
  using Hurricane::Go;
  using Hurricane::Rubber;
  using Hurricane::Layer;
  using Hurricane::BasicLayer;
  using Hurricane::RegularLayer;
  using Hurricane::Transformation;
  using Hurricane::Technology;
  using Hurricane::DataBase;
  using CRL::AllianceFramework;
  using Katabatic::ChipTools;
  using namespace Kite;


// -------------------------------------------------------------------
// Class  :  "::GlobalNetTable".


  class GlobalNetTable {
    public:
                   GlobalNetTable ( Cell* );
             Net*  getRootNet     ( const Net*, Path ) const;
      inline Net*  getVdde        () const;
      inline Net*  getVddi        () const;
      inline Net*  getVsse        () const;
      inline Net*  getVssi        () const;
      inline Net*  getCk          () const;
      inline Net*  getCki         () const;
      inline Net*  getCkc         () const;
      inline Net*  getBlockage    () const;
      inline void  setBlockage    ( Net* );
    private:
      Name  _vddeName;
      Name  _vddiName;
      Name  _vsseName;
      Name  _vssiName;
      Name  _ckName;
      Name  _ckiName;
      Name  _ckcName;
      Net*  _vdde;
      Net*  _vddi;
      Net*  _vsse;
      Net*  _vssi;
      Net*  _ck;    // Clock net on the (external) pad.
      Net*  _cki;   // Clock net in the pad ring.
      Net*  _ckc;   // Clock net of the core (design).
      Net*  _blockage;
  };


  inline Net*  GlobalNetTable::getVdde     () const { return _vdde; }
  inline Net*  GlobalNetTable::getVddi     () const { return _vddi; }
  inline Net*  GlobalNetTable::getVsse     () const { return _vsse; }
  inline Net*  GlobalNetTable::getVssi     () const { return _vssi; }
  inline Net*  GlobalNetTable::getCk       () const { return _ck; }
  inline Net*  GlobalNetTable::getCki      () const { return _cki; }
  inline Net*  GlobalNetTable::getCkc      () const { return _ckc; }
  inline Net*  GlobalNetTable::getBlockage () const { return _blockage; }
  inline void  GlobalNetTable::setBlockage ( Net* net ) { _blockage=net; }

  GlobalNetTable::GlobalNetTable ( Cell* topCell )
    : _vddeName("vdde")
    , _vddiName("vddi")
    , _vsseName("vsse")
    , _vssiName("vssi")
    , _ckName  ("ck")
    , _ckiName ("cki")
    , _ckcName ("ckc")
    , _vdde    (NULL)
    , _vddi    (NULL)
    , _vsse    (NULL)
    , _vssi    (NULL)
    , _ck      (NULL)
    , _cki     (NULL)
    , _ckc     (NULL)
    , _blockage(NULL)
  {
    if ( topCell == NULL ) return;

    AllianceFramework* af = AllianceFramework::get ();

    bool hasPad = false;
    forEach ( Instance*, iinstance, topCell->getInstances() ) {
      if ( af->isPad(iinstance->getMasterCell()) ) {
        cmess1 << "  o  Design has pads, assuming complete chip top structure." << endl;
        hasPad = true;
        break;
      }
    }

    forEach ( Net*, inet, topCell->getNets() ) {
      Net::Type netType = inet->getType();
      if (   (netType != Net::Type::POWER )
         and (netType != Net::Type::GROUND)
         and (netType != Net::Type::CLOCK ) ) continue;
      if ( not inet->isGlobal() ) {
        cerr << Warning("Non global supply/clock net <%s>.",getString(inet->getName()).c_str()) << endl;
      }

      if      ( hasPad and (inet->getName() == _vddeName) ) { _vdde = *inet; continue; }
      else if ( hasPad and (inet->getName() == _vsseName) ) { _vsse = *inet; continue; }
      else if ( hasPad and (inet->getName() == _ckName  ) ) { _ck   = *inet; continue; }
      else if ( hasPad and (inet->getName() == _ckiName ) ) { _cki  = *inet; continue; }

      if ( inet->isPower() ) {
        if ( _vddi == NULL ) {
          cmess1 << "     - Using <" << inet->getName() << "> as core power net." << endl;
          _vddi = *inet;
        } else {
          cerr << Error("More than one power net in designs is not supported yet.\n"
                        "        (<%s> and <%s>)"
                       ,getString(_vddi->getName()).c_str()
                       ,getString(inet->getName()).c_str()
                       ) << endl;
        }
      }

      if ( inet->isGround() ) {
        if ( _vssi == NULL ) {
          cmess1 << "     - Using <" << inet->getName() << "> as core ground net." << endl;
          _vssi = *inet;
        } else {
          cerr << Error("More than one ground net in designs is not supported yet.\n"
                        "        (<%s> and <%s>)"
                       ,getString(_vssi->getName()).c_str()
                       ,getString(inet->getName()).c_str()
                       ) << endl;
        }
      }

      if ( inet->isClock() ) {
        if ( _ckc == NULL ) {
          cmess1 << "     - Using <" << inet->getName() << "> as core clock net." << endl;
          _ckc = *inet;
        } else {
          cerr << Error("More than one clock net in designs is not supported yet.\n"
                        "        (<%s> and <%s>)"
                       ,getString(_ckc->getName()).c_str()
                       ,getString(inet->getName()).c_str()
                       ) << endl;
        }
      }
    }

    if ( hasPad ) {
      if ( _vdde == NULL ) cerr << Error("Missing (pad) <vdde> net at chip level." ) << endl;
      if ( _vsse == NULL ) cerr << Error("Missing (pad) <vsse> net at chip level." ) << endl;
      if ( _ck   == NULL ) cerr << Warning("No (pad) <ck> net at chip level." ) << endl;
      if ( _cki  == NULL ) cerr << Warning("No (pad) <cki> net at chip level." ) << endl;
    }

    if ( _vddi == NULL ) cerr << Error("Missing <vddi>/<vdd> net at top level." ) << endl;
    if ( _vssi == NULL ) cerr << Error("Missing <vssi>/<vss> net at top level." ) << endl;
    if ( _ckc  == NULL ) cerr << Warning("No <ck> net at top level." ) << endl;
  }


  Net* GlobalNetTable::getRootNet ( const Net* net, Path path ) const
  {
  //ltrace(300) << "getRootNet:" << path << ":" << net << endl;
    if ( net == _blockage ) return _blockage;

    if ( net->getName() == _vddeName ) return _vdde;
    if ( net->getName() == _vsseName ) return _vsse;

    if ( net->getType() == Net::Type::POWER  ) return _vddi;
    if ( net->getType() == Net::Type::GROUND ) return _vssi;
    if ( net->getType() != Net::Type::CLOCK  ) return NULL;

    const Net* upNet = net;

    if ( not path.isEmpty() ) {
      Path       upPath   = path;
      Instance*  instance = NULL;
      Plug*      plug     = NULL;

      while ( true ) {
      //cerr << path << "+" << upNet << endl;

        if ( (upNet == NULL) or not upNet->isExternal() ) return NULL;
        if ( path.isEmpty() ) break;

        instance = path.getTailInstance();
        plug     = instance->getPlug(net);
        if ( plug == NULL ) return NULL;

        upNet = plug->getNet();
        path  = path.getHeadPath();
      }
    }

    if ( upNet->getName() == _ckName   ) return _ck;
    if ( upNet->getName() == _ckiName  ) return _cki;
    if ( upNet->getName() == _ckcName  ) return _ckc;

    return NULL;
  }


// -------------------------------------------------------------------
// Class  :  "::PowerRailsPlanes".


  class PowerRailsPlanes {
    private:
      class Rails;
      class Plane;

      class Rail {
        public:
                                      Rail            ( Rails*, DbU::Unit axis, DbU::Unit width );
          inline DbU::Unit            getAxis         () const;
          inline DbU::Unit            getWidth        () const;
          inline Rails*               getRails        () const;
          inline RoutingPlane*        getRoutingPlane () const;
          inline Constant::Direction  getDirection    () const;
          inline Net*                 getNet          () const;
                 void                 merge           ( DbU::Unit source, DbU::Unit target );
                 void                 doLayout        ( const Layer* );
        private:
          Rails*         _rails;
          DbU::Unit      _axis;
          DbU::Unit      _width;
          list<Interval> _chunks;
          Net*           _net;
      };

    private:
      class RailCompare {
        public:
          bool operator() ( const Rail* lhs, const Rail* rhs );
      };

      class RailMatch : public unary_function<Rail*,bool> {
        public:
          inline      RailMatch  ( DbU::Unit axis, DbU::Unit width );
          inline bool operator() ( const Rail* );
        private:
          DbU::Unit  _axis;
          DbU::Unit  _width;
      };

    private:
      class Rails {
        public:
                                      Rails           ( Plane*, Constant::Direction, Net* );
                                     ~Rails           ();
          inline Plane*               getPlane        ();
          inline RoutingPlane*        getRoutingPlane ();
          inline Constant::Direction  getDirection    () const;
          inline Net*                 getNet          () const;
                 void                 merge           ( const Box& );
                 void                 doLayout        ( const Layer* );
        private:
          Plane*               _plane;
          Constant::Direction  _direction;
          Net*                 _net;
          vector<Rail*>        _rails;
      };

    private:
      class Plane {
        public:
                                      Plane           ( const Layer*, RoutingPlane* );
                                     ~Plane           ();
          inline const Layer*         getLayer        () const;
          inline RoutingPlane*        getRoutingPlane ();
          inline Constant::Direction  getDirection    () const;
                 void                 merge           ( const Box&, Net* );
                 void                 doLayout        ();
        private:
          const Layer*         _layer;
          RoutingPlane*        _routingPlane;
          map<Net*,Rails*>     _horizontalRails;
          map<Net*,Rails*>     _verticalRails;
      };

    public:
                    PowerRailsPlanes    ( KiteEngine* );
                   ~PowerRailsPlanes    ();
      inline Net*   getRootNet          ( Net*, Path );
             bool   hasPlane            ( const BasicLayer* );
             bool   setActivePlane      ( const BasicLayer* );
      inline Plane* getActivePlane      () const;
             void   merge               ( const Box&, Net* );
             void   doLayout            ();
    private:
      KiteEngine*                    _kite;
      GlobalNetTable                 _globalNets;
      map<const BasicLayer*,Plane*>  _planes;
      Plane*                         _activePlane;
  };


  PowerRailsPlanes::Rail::Rail ( Rails* rails, DbU::Unit axis, DbU::Unit width )
    : _rails (rails)
    , _axis  (axis)
    , _width (width)
    , _chunks()
  {
    ltrace(300) << "    new Rail " << (void*)this
                << " @" << DbU::getValueString(axis)
                << " " << getRoutingPlane()->getLayer()->getName()
                << " " << getRails()->getNet()
                << " " << getString(getDirection()) << endl;
  }

  inline DbU::Unit                PowerRailsPlanes::Rail::getAxis         () const { return _axis; }
  inline DbU::Unit                PowerRailsPlanes::Rail::getWidth        () const { return _width; }
  inline PowerRailsPlanes::Rails* PowerRailsPlanes::Rail::getRails        () const { return _rails; }
  inline RoutingPlane*            PowerRailsPlanes::Rail::getRoutingPlane () const { return _rails->getRoutingPlane(); }
  inline Constant::Direction      PowerRailsPlanes::Rail::getDirection    () const { return _rails->getDirection(); }
  inline Net*                     PowerRailsPlanes::Rail::getNet          () const { return _rails->getNet(); }


  void  PowerRailsPlanes::Rail::merge ( DbU::Unit source, DbU::Unit target )
  {
    Interval chunkMerge ( source, target );

    list<Interval>::iterator imerge = _chunks.end();
    list<Interval>::iterator ichunk = _chunks.begin();

    while ( ichunk != _chunks.end() ) {
      if ( chunkMerge.getVMax() < (*ichunk).getVMin() ) break;

      if ( chunkMerge.intersect(*ichunk) ) {
        if ( imerge == _chunks.end() ) {
          imerge = ichunk;
          (*imerge).merge ( chunkMerge );
        } else {
          (*imerge).merge ( *ichunk );
          ichunk = _chunks.erase ( ichunk );
          continue;
        }
      }
      ichunk++;
    }

    if ( imerge == _chunks.end() ) {
      _chunks.insert ( ichunk, chunkMerge );
      ltrace(190) << "| Add on " << DbU::getValueString(_axis) << " " << chunkMerge << endl;
    }
  }


  void  PowerRailsPlanes::Rail::doLayout ( const Layer* layer )
  {
    ltrace(300) << "Doing layout of rail: " << (void*)this
                << " " << layer->getName()
                << " " << getString(getDirection()) << " @" << DbU::getValueString(_axis) << endl;

    Net*          net       = getNet();
    RoutingPlane* plane     = getRoutingPlane();
    Segment*      segment   = NULL;
    DbU::Unit     delta     =   plane->getLayerGauge()->getPitch()
                              - plane->getLayerGauge()->getHalfWireWidth()
                              - DbU::lambda(0.1);
    DbU::Unit     extension = layer->getExtentionCap();
  //DbU::Unit     extension = Session::getExtentionCap();
    unsigned int  type      = plane->getLayerGauge()->getType();
    DbU::Unit     axisMin   = 0;
    DbU::Unit     axisMax   = 0;

    if ( getDirection() == Constant::Horizontal ) {
      list<Interval>::iterator ichunk = _chunks.begin();
      for ( ; ichunk != _chunks.end() ; ichunk++ ) {
        if ( type == Constant::PinOnly ) continue;

        ltrace(300) << "  chunk: [" << DbU::getValueString((*ichunk).getVMin())
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

        axisMin = _axis - _width/2 - delta;
        axisMax = _axis + _width/2 + delta;

        Track* track = plane->getTrackByPosition ( axisMin, Constant::Superior );
        for ( ; track and (track->getAxis() <= axisMax) ; track = track->getNext() ) {
          TrackElement* element = TrackFixedSegment::create ( track, segment );
          ltrace(300) << "  Insert in " << track << "+" << element << endl;
        }
      }
    } else {
      list<Interval>::iterator ichunk = _chunks.begin();
      for ( ; ichunk != _chunks.end() ; ichunk++ ) {
        if ( type == Constant::PinOnly ) continue;

        ltrace(300) << "  chunk: [" << DbU::getValueString((*ichunk).getVMin())
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

        axisMin = _axis - _width/2 - delta;
        axisMax = _axis + _width/2 + delta;

        Track* track = plane->getTrackByPosition ( axisMin, Constant::Superior );
        for ( ; track and (track->getAxis() <= axisMax) ; track = track->getNext() ) {
          TrackElement* element = TrackFixedSegment::create ( track, segment );
          ltrace(300) << "  Insert in " << track << "+" << (void*)element << ":" << element << endl;
        }
      }
    }
  }


  inline bool  PowerRailsPlanes::RailCompare::operator() ( const Rail* lhs, const Rail* rhs )
  {
    if ( lhs->getAxis () < rhs->getAxis () ) return true;
    if ( lhs->getWidth() < rhs->getWidth() ) return true;
    return false;
  }


  inline  PowerRailsPlanes::RailMatch::RailMatch ( DbU::Unit axis, DbU::Unit width )
    : _axis(axis)
    , _width(width)
  { }


  inline bool  PowerRailsPlanes::RailMatch::operator() ( const Rail* rail )
  { return (rail->getAxis() == _axis) and (rail->getWidth() == _width); }


  PowerRailsPlanes::Rails::Rails ( PowerRailsPlanes::Plane* plane, Constant::Direction direction, Net* net )
    : _plane    (plane)
    , _direction(direction)
    , _net      (net)
    , _rails    ()
  {
    ltrace(300) << "  new Rails @"
                << " " << getRoutingPlane()->getLayer()->getName()
                << " " << net
                << " " << getString(getDirection()) << endl;
  }


  PowerRailsPlanes::Rails::~Rails ()
  {
    while ( not _rails.empty() ) {
      delete (*_rails.begin());
      _rails.erase ( _rails.begin() );
    }
  }


  inline PowerRailsPlanes::Plane* PowerRailsPlanes::Rails::getPlane        () { return _plane; }
  inline RoutingPlane*            PowerRailsPlanes::Rails::getRoutingPlane () { return getPlane()->getRoutingPlane(); }
  inline Constant::Direction      PowerRailsPlanes::Rails::getDirection    () const { return _direction; }
  inline Net*                     PowerRailsPlanes::Rails::getNet          () const { return _net; }


  void   PowerRailsPlanes::Rails::merge ( const Box& bb )
  {
    DbU::Unit  axis;
    DbU::Unit  width;
    DbU::Unit  sourceU;
    DbU::Unit  targetU;

    if ( getDirection() == Constant::Horizontal ) {
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


  void  PowerRailsPlanes::Rails::doLayout ( const Layer* layer )
  {
    ltrace(300) << "Doing layout of rails: " << layer->getName()
                << " " << getString(_direction)
                << " " << _net->getName() << endl;

    for ( size_t irail=0 ; irail<_rails.size() ; irail++ )
      _rails[irail]->doLayout ( layer );
  }


  PowerRailsPlanes::Plane::Plane ( const Layer* layer, RoutingPlane* routingPlane )
    : _layer                (layer)
    , _routingPlane         (routingPlane)
    , _horizontalRails      ()
    , _verticalRails        ()
  {
    ltrace(300) << "New Plane " << _layer->getName() << " " << _routingPlane << endl;
  }


  PowerRailsPlanes::Plane::~Plane ()
  {
    map<Net*,Rails*>::iterator irail = _horizontalRails.begin();
    for ( ; irail != _horizontalRails.end() ; ++irail ) {
      delete (*irail).second;
    }
    irail = _verticalRails.begin();
    for ( ; irail != _verticalRails.end() ; ++irail ) {
      delete (*irail).second;
    }
  }


  inline const Layer*         PowerRailsPlanes::Plane::getLayer        () const { return _layer; }
  inline RoutingPlane*        PowerRailsPlanes::Plane::getRoutingPlane () { return _routingPlane; }
  inline Constant::Direction  PowerRailsPlanes::Plane::getDirection    () const { return (Constant::Direction)_routingPlane->getDirection(); }


  void  PowerRailsPlanes::Plane::merge ( const Box& bb, Net* net )
  {
    Rails* rails = NULL;

    ltrace(300) << "    " << net->getName() << " " << (void*)net << endl;

    if ( getDirection() == Constant::Horizontal ) {
      map<Net*,Rails*>::iterator irails = _horizontalRails.find(net);
      if ( irails == _horizontalRails.end() ) {
        rails = new Rails(this,Constant::Horizontal,net);
        _horizontalRails.insert ( make_pair(net,rails) );
      } else
        rails = (*irails).second;

      rails->merge ( bb );
    } else {
      map<Net*,Rails*>::iterator irails = _verticalRails.find(net);
      if ( irails == _verticalRails.end() ) {
        rails = new Rails(this,Constant::Vertical,net);
        _verticalRails.insert ( make_pair(net,rails) );
      } else
        rails = (*irails).second;

      rails->merge ( bb );
    }
  }


  void  PowerRailsPlanes::Plane::doLayout ()
  {
    ltrace(300) << "Doing layout of plane: " << _layer->getName() << endl;

    map<Net*,Rails*>::iterator irails = _horizontalRails.begin();
    for ( ; irails != _horizontalRails.end() ; ++irails ) {
      (*irails).second->doLayout(_layer);
    }
    irails = _verticalRails.begin();
    for ( ; irails != _verticalRails.end() ; ++irails ) {
      (*irails).second->doLayout(_layer);
    }
  }


  PowerRailsPlanes::PowerRailsPlanes ( KiteEngine* kite )
    : _kite       (kite)
    , _globalNets (kite->getCell())
    , _planes     ()
    , _activePlane(NULL)
  {
    _globalNets.setBlockage ( kite->getBlockageNet() );

    Technology*   technology = DataBase::getDB()->getTechnology();
    RoutingGauge* rg         = _kite->getConfiguration()->getRoutingGauge();

    forEach ( Layer*, iLayer, technology->getLayers() ) {
      RegularLayer* regular = dynamic_cast<RegularLayer*>(*iLayer);
      if ( not regular
         or (regular->getBasicLayer()->getMaterial() != BasicLayer::Material::metal) ) continue;

      RoutingLayerGauge* lg = rg->getLayerGauge(regular);
      if ( not lg ) continue;

      ltrace(300) << "Gauge: [" << lg->getDepth() << "] " << lg << endl;

      RoutingPlane* rp = _kite->getRoutingPlaneByIndex(lg->getDepth());
      ltrace(300) << "Plane:"  << rp << endl;

      _planes.insert ( make_pair(regular->getBasicLayer(),new Plane(regular,rp)) );

      const BasicLayer* blockageLayer = regular->getBasicLayer()->getBlockageLayer();
      if ( not blockageLayer ) continue;

      _planes.insert ( make_pair(blockageLayer,new Plane(blockageLayer,rp)) );

      ltrace(300) << "OK" << endl;
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


  bool  PowerRailsPlanes::setActivePlane ( const BasicLayer* layer )
  {
    map<const BasicLayer*,Plane*>::iterator iplane = _planes.find(layer);
    if ( iplane == _planes.end() ) return false;

    _activePlane = iplane->second;
    return true;
  }


  inline PowerRailsPlanes::Plane* PowerRailsPlanes::getActivePlane () const
  { return _activePlane; }


  void  PowerRailsPlanes::merge ( const Box& bb, Net* net )
  {
    if ( not _activePlane ) return;

    Net* topGlobalNet = _globalNets.getRootNet ( net, Path() );
    if ( topGlobalNet == NULL ) return;

    _activePlane->merge ( bb, topGlobalNet );
  }


  void  PowerRailsPlanes::doLayout ()
  {
    map<const BasicLayer*,Plane*>::iterator iplane = _planes.begin();
    for ( ; iplane != _planes.end() ; iplane++ )
      iplane->second->doLayout ();
  }



// -------------------------------------------------------------------
// Class  :  "::QueryPowerRails".


  class QueryPowerRails : public Query {
    public:
                            QueryPowerRails     ( KiteEngine* );
      virtual bool          hasGoCallback       () const;
      virtual void          setBasicLayer       ( const BasicLayer* );
      virtual bool          hasBasicLayer       ( const BasicLayer* );
      virtual void          goCallback          ( Go*     );
      virtual void          rubberCallback      ( Rubber* );
      virtual void          extensionGoCallback ( Go*     );
      virtual void          masterCellCallback  ();
              void          addToPowerRail      ( const Go*              go
                                                , const BasicLayer*      basicLayer
                                                , const Box&             area
                                                , const Transformation&  transformation
                                                );
              void          ringAddToPowerRails ();
      virtual void          doQuery             ();
      inline  void          doLayout            ();
      inline  unsigned int  getGoMatchCount     () const;
    private:
      AllianceFramework*      _framework;
      KiteEngine*             _kite;
      RoutingGauge*           _routingGauge;
      const ChipTools&        _chipTools;
      PowerRailsPlanes        _powerRailsPlanes;
      bool                    _isBlockagePlane;
      vector<const Segment*>  _hRingSegments;
      vector<const Segment*>  _vRingSegments;
      unsigned int            _goMatchCount;
  };


  QueryPowerRails::QueryPowerRails ( KiteEngine* kite )
    : Query            ()
    , _framework       (AllianceFramework::get())
    , _kite            (kite)
    , _routingGauge    (kite->getConfiguration()->getRoutingGauge())
    , _chipTools       (kite->getChipTools())
    , _powerRailsPlanes(kite)
    , _isBlockagePlane (false)
    , _hRingSegments   ()
    , _vRingSegments   ()
    , _goMatchCount    (0)
  {
    setCell       ( kite->getCell() );
    setArea       ( kite->getCell()->getBoundingBox() );
    setBasicLayer ( NULL );
    setFilter     ( Query::DoTerminalCells|Query::DoComponents );
  }


  inline  unsigned int  QueryPowerRails::getGoMatchCount () const
  { return _goMatchCount; }


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
    if ( not _powerRailsPlanes.getActivePlane() ) return;
    Query::doQuery ();

  }


  void  QueryPowerRails::masterCellCallback ()
  { }


  bool  QueryPowerRails::hasGoCallback () const
  { return true; }


  void  QueryPowerRails::goCallback ( Go* go )
  {
    addToPowerRail ( go, getBasicLayer(), getArea(), getTransformation() );
  }


  void  QueryPowerRails::addToPowerRail ( const Go*              go
                                        , const BasicLayer*      basicLayer
                                        , const Box&             area
                                        , const Transformation&  transformation
                                        )
  {
    const Component* component = dynamic_cast<const Component*>(go);
    if ( component ) {
      if (    _framework->isPad(getMasterCell())
         and (_routingGauge->getLayerDepth(component->getLayer()) < 2) )
        return;

      Net* rootNet = NULL;
      if ( not _isBlockagePlane )
        rootNet = _powerRailsPlanes.getRootNet(component->getNet(),getPath());
      else
        rootNet = _kite->getBlockageNet();
      if ( rootNet == NULL ) return;

      const Segment* segment = dynamic_cast<const Segment*>(component);
      if ( segment != NULL ) {
        _goMatchCount++;
        ltrace(300) << "  Merging PowerRail element: " << segment << endl;

        Box bb = segment->getBoundingBox ( basicLayer );

        unsigned int depth = _routingGauge->getLayerDepth ( segment->getLayer() );

        if (    _chipTools.isChip()
            and ((depth == 2) or (depth == 3))
            and (segment->getWidth () == DbU::lambda( 12.0))
            and (segment->getLength() >  DbU::lambda(200.0))
            and (_kite->getChipTools().getCorona().contains(bb)) ) {
          switch ( depth ) {
            case 2: _vRingSegments.push_back ( segment ); break; // M3 V.
            case 3: _hRingSegments.push_back ( segment ); break; // M4 H.
          }
          return;
        }

        transformation.applyOn ( bb );

        _powerRailsPlanes.merge ( bb, rootNet );
      } else {
        const Contact* contact = dynamic_cast<const Contact*>(component);
        if ( contact != NULL ) {
          _goMatchCount++;

          Box bb = contact->getBoundingBox ( basicLayer );
          transformation.applyOn ( bb );

          ltrace(300) << "  Merging PowerRail element: " << contact << " bb:" << bb << endl;
          
          _powerRailsPlanes.merge ( bb, rootNet );
        }
      }
    }
  }


  void  QueryPowerRails::ringAddToPowerRails ()
  {

    if ( not _hRingSegments.empty() ) {
      const RegularLayer* layer = dynamic_cast<const RegularLayer*>(_routingGauge->getRoutingLayer(3));
      setBasicLayer ( layer->getBasicLayer() );

      DbU::Unit   xmin = DbU::Max;
      DbU::Unit   xmax = DbU::Min;
      vector<Box> boxes;

      for ( size_t i=0 ; i<_hRingSegments.size() ; ++i ) {
        boxes.push_back ( _hRingSegments[i]->getBoundingBox() );
        xmin = std::min ( xmin, boxes.back().getXMin() );
        xmax = std::max ( xmax, boxes.back().getXMax() );
      }

      for ( size_t i=0 ; i<_hRingSegments.size() ; ++i ) {
        _powerRailsPlanes.merge ( Box(xmin,boxes[i].getYMin(),xmax,boxes[i].getYMax())
                                , _powerRailsPlanes.getRootNet(_hRingSegments[i]->getNet(),Path()) );
      }
    }

    if ( not _vRingSegments.empty() ) {
      const RegularLayer* layer = dynamic_cast<const RegularLayer*>(_routingGauge->getRoutingLayer(2));
      setBasicLayer ( layer->getBasicLayer() );

      DbU::Unit   ymin = DbU::Max;
      DbU::Unit   ymax = DbU::Min;
      vector<Box> boxes;

      for ( size_t i=0 ; i<_vRingSegments.size() ; ++i ) {
        boxes.push_back ( _vRingSegments[i]->getBoundingBox() );
        ymin = std::min ( ymin, boxes.back().getYMin() );
        ymax = std::max ( ymax, boxes.back().getYMax() );
      }

      for ( size_t i=0 ; i<_vRingSegments.size() ; ++i ) {
        _powerRailsPlanes.merge ( Box(boxes[i].getXMin(),ymin,boxes[i].getXMax(),ymax)
                                , _powerRailsPlanes.getRootNet(_vRingSegments[i]->getNet(),Path()) );
      }
    }
  }


  void  QueryPowerRails::rubberCallback ( Rubber* )
  { }


  void  QueryPowerRails::extensionGoCallback ( Go* )
  { }


} // End of anonymous namespace.


namespace Kite {


  using Hurricane::DataBase;
  using Hurricane::Technology;
  using Hurricane::BasicLayer;
  using Hurricane::ForEachIterator;


  void  KiteEngine::buildPowerRails ()
  {
    cmess1 << "  o  Building power rails." << endl;

    if ( not _blockageNet ) {
      _blockageNet = getCell()->getNet("blockagenet");
      if ( not _blockageNet )
        _blockageNet = Net::create ( getCell(), "blockagenet" );
    }

    QueryPowerRails query ( this );
    Technology*     technology = DataBase::getDB()->getTechnology();

    forEach ( BasicLayer*, iLayer, technology->getBasicLayers() ) {
      if (   (iLayer->getMaterial() != BasicLayer::Material::metal)
         and (iLayer->getMaterial() != BasicLayer::Material::blockage) )
        continue;
      if ( _configuration->isGMetal(*iLayer) ) continue;

      cmess1 << "     - PowerRails in " << iLayer->getName() << " ..." << endl;

      if ( not query.hasBasicLayer(*iLayer) ) continue;

      query.setBasicLayer ( *iLayer );
      query.doQuery       ();
    }
    query.ringAddToPowerRails ();
    query.doLayout ();
    cmess1 << "     - " << query.getGoMatchCount() << " power rails elements found." << endl;

    Session::revalidate ();
  }


} // End of Kite namespace.
