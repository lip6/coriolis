
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
// |  C++ Module  :       "./QueryBlockages.cpp"                     |
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
#include  "hurricane/Query.h"
#include  "kite/RoutingPlane.h"
#include  "kite/TrackBlockage.h"
#include  "kite/Track.h"
#include  "kite/KiteEngine.h"


namespace {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Interval;
  using Hurricane::Query;
  using Hurricane::Go;
  using Hurricane::Rubber;
  using Hurricane::Layer;
  using Hurricane::BasicLayer;
  using Hurricane::RegularLayer;
  using Hurricane::Transformation;
  using namespace Kite;


// -------------------------------------------------------------------
// Class  :  "::BlockagesPlanes".


  class BlockagesPlanes {
    private:
      class Track {
        public:
                            Track        ( Kite::Track* );
                 void       add          ( Interval );
          inline DbU::Unit  getAxis      () const;
                 void       dump         ( DbU::Unit width );
        private:
          Kite::Track*    _ktrack;
          list<Interval>  _blockages;
      };
    private:
      class Plane {
        public:
                               Plane            ( RoutingPlane* );
                              ~Plane            ();
                 void          merge            ( Box boundingBox );
          inline unsigned int  getDirection     () const;
          inline const Layer*  getLayer         () const;
          inline DbU::Unit     getHalfWireWidth () const;
          inline DbU::Unit     getHalfPitch     () const;
                 void          dump             ();
        private:
          RoutingPlane*  _routingPlane;
          Interval       _axisSpan;
          vector<Track*> _tracks;
      };
    public:
      static Track* createTrack      ( Kite::Track* );
    public:                          
                    BlockagesPlanes  ( KiteEngine* );
                   ~BlockagesPlanes  ();
             bool   hasPlane         ( const BasicLayer* );
             bool   setActivePlane   ( const BasicLayer* );
      inline Plane* getActivePlane   () const;
             void   add              ( Box boundingBox );
             void   dump             ();
    private:
      KiteEngine*                    _kite;
      map<const BasicLayer*,Plane*>  _planes;
      Plane*                         _activePlane;
  };


  BlockagesPlanes::Track::Track ( Kite::Track* ktrack )
    : _ktrack(ktrack)
    , _blockages()
  { }


  inline DbU::Unit  BlockagesPlanes::Track::getAxis () const { return _ktrack->getAxis(); }


  void  BlockagesPlanes::Track::add ( Interval obstacle )
  {
    if (  (obstacle.getVMax() <= _ktrack->getMin())
       or (obstacle.getVMin() >= _ktrack->getMax()) ) return;

    list<Interval>::iterator imerge    = _blockages.end();
    list<Interval>::iterator iblockage = _blockages.begin();

    while ( iblockage != _blockages.end() ) {
      if ( obstacle.getVMax() < (*iblockage).getVMin() ) break;

      if ( obstacle.intersect(*iblockage) ) {
        if ( imerge == _blockages.end() ) {
          imerge = iblockage;
          (*imerge).merge ( obstacle );
        } else {
          (*imerge).merge ( *iblockage );
          iblockage = _blockages.erase ( iblockage );
          continue;
        }
      }
      iblockage++;
    }

    if ( imerge == _blockages.end() ) {
      _blockages.insert ( iblockage, obstacle );
      ltrace(190) << "| Add on " << DbU::getValueString(_ktrack->getAxis()) << " " << obstacle << endl;
    }
  }


  void  BlockagesPlanes::Track::dump ( DbU::Unit width )
  {
    list<Interval>::iterator iinterval = _blockages.begin();

    if ( _ktrack->getDirection() == Constant::Horizontal ) {
      DbU::Unit ymin = _ktrack->getAxis() - width/2;
      DbU::Unit ymax = _ktrack->getAxis() + width/2;

      for ( ; iinterval != _blockages.end() ; iinterval++ ) {
        Box bb ( (*iinterval).getVMin(), ymin, (*iinterval).getVMax(), ymax );
        TrackBlockage::create ( _ktrack, bb );
      }
    } else {
      DbU::Unit xmin = _ktrack->getAxis() - width/2;
      DbU::Unit xmax = _ktrack->getAxis() + width/2;

      for ( ; iinterval != _blockages.end() ; iinterval++ ) {
        Box bb ( xmin, (*iinterval).getVMin(), xmax, (*iinterval).getVMax() );
        TrackBlockage::create ( _ktrack, bb );
      }
    }
  }


  BlockagesPlanes::Plane::Plane ( RoutingPlane* plane )
    : _routingPlane(plane)
    , _axisSpan    (plane->getAxisMin(),plane->getAxisMax())
    , _tracks      ()
  {
    Kite::Track* ktrack = plane->getTrackByIndex(0);

    for ( ; ktrack != NULL ; ktrack=ktrack->getNext() ) {
      _tracks.push_back ( BlockagesPlanes::createTrack(ktrack) );
    }
  }


  BlockagesPlanes::Plane::~Plane ()
  {
    for ( size_t i=0 ; i<_tracks.size() ; i++ ) delete _tracks[i];
  }


  inline unsigned int  BlockagesPlanes::Plane::getDirection     () const { return _routingPlane->getDirection(); }
  inline const Layer*  BlockagesPlanes::Plane::getLayer         () const { return _routingPlane->getLayer(); }
  inline DbU::Unit     BlockagesPlanes::Plane::getHalfWireWidth () const { return _routingPlane->getLayerGauge()->getHalfWireWidth(); }
  inline DbU::Unit     BlockagesPlanes::Plane::getHalfPitch     () const { return _routingPlane->getLayerGauge()->getHalfPitch(); }


  void  BlockagesPlanes::Plane::merge ( Box boundingBox )
  {
    ltrace(190) << "| Add on plane " << _routingPlane->getLayer() << " " << boundingBox << endl;

    DbU::Unit delta = getHalfPitch() - getHalfWireWidth() - DbU::lambda(0.1);

    if ( getDirection() == Constant::Horizontal ) {
      boundingBox.inflate ( 0, delta, 0, delta );

      ltrace(190) << "Track range: " << DbU::getValueString(boundingBox.getYMin())
                  << ":"             << DbU::getValueString(boundingBox.getYMax()) << endl;
      for ( size_t i=0 ; (i<_tracks.size()) and (_tracks[i]->getAxis() < boundingBox.getYMax()) ; i++ ) {
        if ( _tracks[i]->getAxis() < boundingBox.getYMin() ) continue;

        _tracks[i]->add ( Interval(boundingBox.getXMin(),boundingBox.getXMax()) );
      } 
    } else {
      boundingBox.inflate ( delta, 0, delta, 0 );
      for ( size_t i=0 ; (i<_tracks.size()) and (_tracks[i]->getAxis() < boundingBox.getXMax()) ; i++ ) {
        if ( _tracks[i]->getAxis() < boundingBox.getXMin() ) continue;

        _tracks[i]->add ( Interval(boundingBox.getYMin(),boundingBox.getYMax()) );
      } 
    }
  }


  void  BlockagesPlanes::Plane::dump ()
  {
    for ( size_t i=0 ; i<_tracks.size() ; i++ ) _tracks[i]->dump(DbU::lambda(2.0));
  }


  BlockagesPlanes::Track* BlockagesPlanes::createTrack ( Kite::Track* ktrack )
  { return new Track ( ktrack ); }


  BlockagesPlanes::BlockagesPlanes ( KiteEngine* kite )
    : _kite       (kite)
    , _planes     ()
    , _activePlane(NULL)
  {
    RoutingGauge* rg         = _kite->getConfiguration()->getRoutingGauge();
    size_t        gaugeDepth = rg->getDepth ();

    for ( size_t depth=0 ; depth<gaugeDepth ; depth++ ) {
      const RegularLayer* routingLayer = dynamic_cast<const RegularLayer*>(rg->getRoutingLayer(depth));
      if ( not routingLayer ) continue;

      const BasicLayer* blockageLayer = routingLayer->getBasicLayer()->getBlockageLayer();
      if ( not blockageLayer ) continue;

      _planes.insert ( make_pair(blockageLayer,new Plane(_kite->getRoutingPlaneByIndex(depth))) );
    }
  }


  BlockagesPlanes::~BlockagesPlanes ()
  {
    while ( not _planes.empty() ) {
      delete _planes.begin()->second;
      _planes.erase ( _planes.begin() );
    }
  }


  bool  BlockagesPlanes::hasPlane ( const BasicLayer* basicLayer )
  { return (_planes.find(basicLayer) != _planes.end()); }


  bool  BlockagesPlanes::setActivePlane ( const BasicLayer* basicLayer )
  {
    map<const BasicLayer*,Plane*>::iterator iplane = _planes.find(basicLayer);
    if ( iplane == _planes.end() ) return false;

    _activePlane = iplane->second;
    return true;
  }


  inline BlockagesPlanes::Plane* BlockagesPlanes::getActivePlane () const
  { return _activePlane; }


  void  BlockagesPlanes::add ( Box boundingBox )
  {
    if ( not _activePlane ) return;
    _activePlane->merge ( boundingBox );
  }


  void  BlockagesPlanes::dump ()
  {
    map<const BasicLayer*,Plane*>::iterator iplane = _planes.begin();
    for ( ; iplane != _planes.end() ; iplane++ ) {
      iplane->second->dump();
    }
  }



// -------------------------------------------------------------------
// Class  :  "::QueryBlockages".


  class QueryBlockages : public Query {
    public:
                            QueryBlockages      ( KiteEngine* );
      virtual bool          hasGoCallback       () const;
      virtual bool          hasPlane            ( const BasicLayer* );
      virtual void          setBasicLayer       ( const BasicLayer* );
      virtual void          goCallback          ( Go*     );
      virtual void          rubberCallback      ( Rubber* );
      virtual void          extensionGoCallback ( Go*     );
      virtual void          masterCellCallback  ();
              void          addBlockage         ( const Go*              go
                                                , const BasicLayer*      basicLayer
                                                , const Box&             area
                                                , const Transformation&  transformation
                                                );
      virtual void          doQuery             ();
      inline  void          dump                ();
      inline  unsigned int  getBlockagesCount   () const;
    private:
      KiteEngine*      _kite;
      BlockagesPlanes  _blockagesPlanes;
      unsigned int     _blockagesCount;
  };


  QueryBlockages::QueryBlockages ( KiteEngine* kite )
    : Query           ()
    , _kite           (kite)
    , _blockagesPlanes(kite)
    , _blockagesCount (0)
  {
    setCell       ( kite->getCell() );
    setArea       ( kite->getCell()->getBoundingBox() );
    setBasicLayer ( NULL );
    setFilter     ( Query::DoTerminalCells|Query::DoComponents );
  }


  inline  unsigned int  QueryBlockages::getBlockagesCount () const
  { return _blockagesCount; }


  inline  void  QueryBlockages::dump ()
  { return _blockagesPlanes.dump(); }


  bool  QueryBlockages::hasPlane ( const BasicLayer* basicLayer )
  { return _blockagesPlanes.hasPlane(basicLayer); }


  void  QueryBlockages::setBasicLayer ( const BasicLayer* basicLayer )
  {
    _blockagesPlanes.setActivePlane ( basicLayer );
    Query::setBasicLayer ( basicLayer );
  }


  void  QueryBlockages::doQuery ()
  {
    if ( not _blockagesPlanes.getActivePlane() ) return;
    Query::doQuery ();
  }


  void  QueryBlockages::masterCellCallback ()
  { }


  bool  QueryBlockages::hasGoCallback () const
  { return true; }


  void  QueryBlockages::goCallback ( Go* go )
  {
    addBlockage ( go, getBasicLayer(), getArea(), getTransformation() );
  }


  void  QueryBlockages::addBlockage ( const Go*              go
                                    , const BasicLayer*      basicLayer
                                    , const Box&             area
                                    , const Transformation&  transformation
                                    )
  {
    const Component* component = dynamic_cast<const Component*>(go);
    if ( component ) {
      _blockagesCount++;
      Box bb ( transformation.getBox(component->getBoundingBox(basicLayer)) );
      ltrace(190) << "Blockage: " << bb << " in " << basicLayer->getName() << endl;

      _blockagesPlanes.add ( bb );
    }
  }


  void  QueryBlockages::rubberCallback ( Rubber* )
  { }


  void  QueryBlockages::extensionGoCallback ( Go* )
  { }


} // End of anonymous namespace.


namespace Kite {


  using Hurricane::DataBase;
  using Hurricane::Technology;
  using Hurricane::BasicLayer;
  using Hurricane::ForEachIterator;


  void  KiteEngine::buildBlockages ()
  {
    cmess1 << "  o  Building blockages." << endl;

    if ( not _obstacleNet ) {
      _obstacleNet = getCell()->getNet("obstaclenet");
      if ( not _obstacleNet )
        _obstacleNet = Net::create ( getCell(), "obstaclenet" );
    }

    QueryBlockages query ( this );
    Technology*    technology = DataBase::getDB()->getTechnology();

    forEach ( BasicLayer*, iLayer, technology->getBasicLayers() ) {
      if ( iLayer->getMaterial() != BasicLayer::Material::blockage ) continue;
      if ( not query.hasPlane(*iLayer) ) continue;

      cmess1 << "     - Blockages in " << iLayer->getName() << " ..." << endl;

      query.setBasicLayer ( *iLayer );
      query.doQuery       ();
    }
    query.dump ();
    cmess1 << "     - " << query.getBlockagesCount() << " blockages found." << endl;

    Session::revalidate ();
  }


} // End of Kite namespace.
