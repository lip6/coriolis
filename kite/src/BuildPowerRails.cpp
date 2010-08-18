
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

#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/BasicLayer.h"
#include  "hurricane/RegularLayer.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/NetExternalComponents.h"
#include  "hurricane/Query.h"
#include  "kite/RoutingPlane.h"
#include  "kite/TrackFixedSegment.h"
#include  "kite/Track.h"
#include  "kite/KiteEngine.h"


namespace {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ForEachIterator;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Interval;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::NetExternalComponents;
  using Hurricane::Query;
  using Hurricane::Go;
  using Hurricane::Rubber;
  using Hurricane::Layer;
  using Hurricane::BasicLayer;
  using Hurricane::RegularLayer;
  using Hurricane::Transformation;
  using Hurricane::Technology;
  using Hurricane::DataBase;
  using namespace Kite;


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
          inline Net::Type            getType         () const;
                 void                 merge           ( DbU::Unit source, DbU::Unit target );
                 void                 doLayout        ( const Layer*, Net* );
        private:
          Rails*         _rails;
          DbU::Unit      _axis;
          DbU::Unit      _width;
          list<Interval> _chunks;
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
                                      Rails           ( Plane*, Constant::Direction, Net::Type );
                                     ~Rails           ();
          inline Plane*               getPlane        ();
          inline RoutingPlane*        getRoutingPlane ();
          inline Constant::Direction  getDirection    () const;
          inline Net::Type            getType         () const;
                 void                 merge           ( Point& source, Point& target, DbU::Unit width );
                 void                 doLayout        ( const Layer*, Net* );
        private:
          Plane*               _plane;
          Constant::Direction  _direction;
          Net::Type            _type;
          vector<Rail*>        _rails;
      };
    private:
      class Plane {
        public:
                               Plane           ( const RegularLayer*, RoutingPlane* );
                              ~Plane           ();
          inline RoutingPlane* getRoutingPlane ();
                 void          merge           ( Point& source, Point& target, DbU::Unit width, Net::Type );
                 void          doLayout        ( Net* powerNet, Net* groundNet );
        private:
          const RegularLayer* _layer;
          RoutingPlane*       _routingPlane;
          Rails               _railsPowerHorizontal;
          Rails               _railsPowerVertical;
          Rails               _railsGroundHorizontal;
          Rails               _railsGroundVertical;
      };
    public:
                    PowerRailsPlanes ( KiteEngine* );
                   ~PowerRailsPlanes ();
             bool   hasPlane         ( const BasicLayer* );
             bool   setActivePlane   ( const BasicLayer* );
      inline Plane* getActivePlane   () const;
             void   merge            ( Point& source, Point& target, DbU::Unit width, Net::Type );
             void   doLayout         ();
    private:
      KiteEngine*                    _kite;
      map<const BasicLayer*,Plane*>  _planes;
      Plane*                         _activePlane;
  };


  PowerRailsPlanes::Rail::Rail ( Rails* rails, DbU::Unit axis, DbU::Unit width )
    : _rails (rails)
    , _axis  (axis)
    , _width (width)
    , _chunks()
  {
    cinfo << "  New rail @" << DbU::getValueString(axis)
          << " " << getRoutingPlane()->getLayer()->getName()
          << " " << getRails()->getType() << endl;
  }

  inline DbU::Unit                PowerRailsPlanes::Rail::getAxis         () const { return _axis; }
  inline DbU::Unit                PowerRailsPlanes::Rail::getWidth        () const { return _width; }
  inline PowerRailsPlanes::Rails* PowerRailsPlanes::Rail::getRails        () const { return _rails; }
  inline RoutingPlane*            PowerRailsPlanes::Rail::getRoutingPlane () const { return _rails->getRoutingPlane(); }
  inline Constant::Direction      PowerRailsPlanes::Rail::getDirection    () const { return _rails->getDirection(); }
  inline Net::Type                PowerRailsPlanes::Rail::getType         () const { return _rails->getType(); }


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


  void  PowerRailsPlanes::Rail::doLayout ( const Layer* layer, Net* net )
  {
    cinfo << "Doing layout of rail: " << layer->getName() << " @" << DbU::getValueString(_axis) << endl;

    RoutingPlane* plane   = getRoutingPlane();
    Segment*      segment = NULL;
    DbU::Unit     delta   = plane->getLayerGauge()->getPitch() - plane->getLayerGauge()->getHalfWireWidth() - 1;
    unsigned int  type    = plane->getLayerGauge()->getType();

    if ( getDirection() == Constant::Horizontal ) {
      list<Interval>::iterator ichunk = _chunks.begin();
      for ( ; ichunk != _chunks.end() ; ichunk++ ) {
        segment = Horizontal::create ( net, layer, _axis, _width, (*ichunk).getVMin(), (*ichunk).getVMax() );
        if ( segment )
          NetExternalComponents::setExternal ( segment );

        if ( type == Constant::PinOnly ) continue;

        DbU::Unit axisMin = _axis - _width/2 - delta;
        DbU::Unit axisMax = _axis + _width/2 + delta;

        Track* track = plane->getTrackByPosition ( axisMin, Constant::Superior );
        for ( ; track and (track->getAxis() <= axisMax) ; track = track->getNext() ) {
          TrackFixedSegment::create ( track, segment );
        }
      }
    } else {
      list<Interval>::iterator ichunk = _chunks.begin();
      for ( ; ichunk != _chunks.end() ; ichunk++ ) {
        segment = Vertical::create ( net, layer, _axis, _width, (*ichunk).getVMin(), (*ichunk).getVMax() );
        if ( segment )
          NetExternalComponents::setExternal ( segment );

        if ( type == Constant::PinOnly ) continue;

        DbU::Unit axisMin = _axis - _width/2 - delta;
        DbU::Unit axisMax = _axis + _width/2 + delta;

        Track* track = plane->getTrackByPosition ( axisMin, Constant::Superior );
        for ( ; track and (track->getAxis() <= axisMax) ; track = track->getNext() ) {
          TrackFixedSegment::create ( track, segment );
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


  PowerRailsPlanes::Rails::Rails ( PowerRailsPlanes::Plane* plane, Constant::Direction direction, Net::Type type )
    : _plane    (plane)
    , _direction(direction)
    , _type     (type)
  {
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
  inline Net::Type                PowerRailsPlanes::Rails::getType         () const { return _type; }


  void   PowerRailsPlanes::Rails::merge ( Point& source, Point& target, DbU::Unit width )
  {
    DbU::Unit axis = (_direction == Constant::Horizontal) ? source.getY() : source.getX();
    vector<Rail*>::iterator irail = find_if ( _rails.begin(), _rails.end(), RailMatch(axis,width) );

    Rail* rail = NULL;
    if ( irail == _rails.end() ) {
      rail = new Rail(this,axis,width);
      _rails.push_back ( rail );
      sort ( _rails.begin(), _rails.end(), RailCompare() );
    } else {
      rail = *irail;
    }

    if ( _direction == Constant::Horizontal )
      rail->merge ( source.getX(), target.getX() );
    else
      rail->merge ( source.getY(), target.getY() );
  }


  void  PowerRailsPlanes::Rails::doLayout ( const Layer* layer, Net* net )
  {
    cinfo << "Doing layout of plane: " << layer->getName() << " " << net->getName() << endl;

    for ( size_t irail=0 ; irail<_rails.size() ; irail++ )
      _rails[irail]->doLayout ( layer, net );
  }


  PowerRailsPlanes::Plane::Plane ( const RegularLayer* layer, RoutingPlane* routingPlane )
    : _layer                (layer)
    , _routingPlane         (routingPlane)
    , _railsPowerHorizontal (this,Constant::Horizontal,Net::Type::POWER)
    , _railsPowerVertical   (this,Constant::Vertical  ,Net::Type::POWER)
    , _railsGroundHorizontal(this,Constant::Horizontal,Net::Type::GROUND)
    , _railsGroundVertical  (this,Constant::Vertical  ,Net::Type::GROUND)
  {
    cinfo << "New Plane " << _layer->getName() << " " << _routingPlane << endl;
  }


  PowerRailsPlanes::Plane::~Plane ()
  {
  }


  inline RoutingPlane* PowerRailsPlanes::Plane::getRoutingPlane () { return _routingPlane; }


  void  PowerRailsPlanes::Plane::merge ( Point& source, Point& target, DbU::Unit width, Net::Type type )
  {
    if ( source.getY() == target.getY() ) {
      switch ( type ) {
        case Net::Type::POWER:  _railsPowerHorizontal .merge(source,target,width); break;
        case Net::Type::GROUND: _railsGroundHorizontal.merge(source,target,width); break;
        default: break;
      }
    } else {
      switch ( type ) {
        case Net::Type::POWER:  _railsPowerVertical .merge(source,target,width); break;
        case Net::Type::GROUND: _railsGroundVertical.merge(source,target,width); break;
        default: break;
      }
    }
  }


  void  PowerRailsPlanes::Plane::doLayout ( Net* powerNet, Net* groundNet )
  {
    _railsPowerHorizontal .doLayout ( _layer, powerNet );
    _railsPowerVertical   .doLayout ( _layer, powerNet );
    _railsGroundHorizontal.doLayout ( _layer, groundNet );
    _railsGroundVertical  .doLayout ( _layer, groundNet );
  }


  PowerRailsPlanes::PowerRailsPlanes ( KiteEngine* kite )
    : _kite       (kite)
    , _planes     ()
    , _activePlane(NULL)
  {
    Technology*   technology = DataBase::getDB()->getTechnology();
    RoutingGauge* rg         = _kite->getConfiguration()->getRoutingGauge();

    forEach ( Layer*, iLayer, technology->getLayers() ) {
      RegularLayer* regular = dynamic_cast<RegularLayer*>(*iLayer);
      if ( not regular
         or (regular->getBasicLayer()->getMaterial() != BasicLayer::Material::metal) ) continue;

      RoutingLayerGauge* lg = rg->getLayerGauge(regular);
      if ( not lg ) continue;

      cinfo << "Gauge: [" << lg->getDepth() << "] " << lg << endl;

      RoutingPlane* rp = _kite->getRoutingPlaneByIndex(lg->getDepth());
      cinfo << "Plane:"  << rp << endl;

      _planes.insert ( make_pair(regular->getBasicLayer(),new Plane(regular,rp)) );
    }
  }


  PowerRailsPlanes::~PowerRailsPlanes ()
  {
    while ( not _planes.empty() ) {
      delete _planes.begin()->second;
      _planes.erase ( _planes.begin() );
    }
  }


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


  void  PowerRailsPlanes::merge ( Point& source, Point& target, DbU::Unit width, Net::Type type )
  {
    if ( not _activePlane ) return;
    _activePlane->merge ( source, target, width, type );
  }


  void  PowerRailsPlanes::doLayout ()
  {
    Net* powerNet  = NULL;
    Net* groundNet = NULL;

    forEach ( Net*, inet, _kite->getCell()->getNets() ) {
      if ( not powerNet  and (inet->getType() == Net::Type::POWER))  powerNet  = *inet;
      if ( not groundNet and (inet->getType() == Net::Type::GROUND)) groundNet = *inet;
      if ( powerNet and groundNet ) break;
    }

    cinfo << "Doing power/ground layout " << powerNet->getName()
          << "/" << groundNet->getName() << endl;

    map<const BasicLayer*,Plane*>::iterator iplane = _planes.begin();
    for ( ; iplane != _planes.end() ; iplane++ )
      iplane->second->doLayout ( powerNet, groundNet );
  }



// -------------------------------------------------------------------
// Class  :  "::QueryPowerRails".


  class QueryPowerRails : public Query {
    public:
                            QueryPowerRails     ( KiteEngine* );
      virtual bool          hasGoCallback       () const;
      virtual void          setBasicLayer       ( const BasicLayer* );
      virtual void          goCallback          ( Go*     );
      virtual void          rubberCallback      ( Rubber* );
      virtual void          extensionGoCallback ( Go*     );
      virtual void          masterCellCallback  ();
              void          addToPowerRail      ( const Go*              go
                                                , const BasicLayer*      basicLayer
                                                , const Box&             area
                                                , const Transformation&  transformation
                                                );
      virtual void          doQuery             ();
      inline  void          doLayout            ();
      inline  unsigned int  getGoMatchCount     () const;
    private:
      KiteEngine*       _kite;
      PowerRailsPlanes  _powerRailsPlanes;
      unsigned int      _goMatchCount;
  };


  QueryPowerRails::QueryPowerRails ( KiteEngine* kite )
    : Query            ()
    , _kite            (kite)
    , _powerRailsPlanes(kite)
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


  void  QueryPowerRails::setBasicLayer ( const BasicLayer* basicLayer )
  {
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
      Net::Type netType = component->getNet()->getType();
      if ( (netType != Net::Type::POWER) and (netType != Net::Type::GROUND) ) return;

      const Segment* segment = dynamic_cast<const Segment*>(component);
      if ( not segment ) return;

      _goMatchCount++;
      cinfo << "  Merging PowerRail element: " << segment << endl;

      Point source = segment->getSourcePosition();
      Point target = segment->getTargetPosition();

      transformation.applyOn ( source );
      transformation.applyOn ( target );

      _powerRailsPlanes.merge ( source, target, segment->getWidth(), netType );
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

    QueryPowerRails query ( this );
    Technology*     technology = DataBase::getDB()->getTechnology();

    forEach ( BasicLayer*, iLayer, technology->getBasicLayers() ) {
      if ( iLayer->getMaterial() != BasicLayer::Material::metal ) continue;
      if ( _configuration->isGMetal(*iLayer) ) continue;

      cmess1 << "     - PowerRails in " << iLayer->getName() << " ..." << endl;

      query.setBasicLayer ( *iLayer );
      query.doQuery       ();
    }
    query.doLayout ();
    cmess1 << "     - " << query.getGoMatchCount() << " power rails elements found." << endl;

    Session::revalidate ();
  }


} // End of Kite namespace.
