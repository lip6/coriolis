
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2024-2024.
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./LandingZone.cpp"                            |
// +-----------------------------------------------------------------+


#include <tuple>
#include "hurricane/Error.h"
#include "hurricane/DebugSession.h"
#include "hurricane/AreaMarker.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Rectilinear.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/Cell.h"
#include "crlcore/RoutingLayerGauge.h"
#include "anabatic/Configuration.h"
#include "anabatic/LandingZone.h"


namespace Anabatic {

  using namespace std;
  using Hurricane::_TName;
  using Hurricane::Error;
  using Hurricane::DebugSession;
  using Hurricane::AreaMarker;
  using Hurricane::Property;
  using Hurricane::Interval;
  using Hurricane::Horizontal;
  using Hurricane::Rectilinear;
  using Hurricane::NetExternalComponents;
  using CRL::RoutingLayerGauge;



// -------------------------------------------------------------------
// Class  :  "Anabatic::CompareLandingCandidate".


  typedef  tuple<Component*,Box,Box>  LandingCandidate;

  class CompareLandingCandidates {
    public:
      inline      CompareLandingCandidates ( DbU::Unit hThreshold, DbU::Unit vThreshold );
      inline bool operator ()              ( const LandingCandidate&, const LandingCandidate& );
    private:
      DbU::Unit  _hThreshold;
      DbU::Unit  _vThreshold;
  };

  
  inline  CompareLandingCandidates::CompareLandingCandidates ( DbU::Unit hThreshold, DbU::Unit vThreshold )
    : _hThreshold(hThreshold)
    , _vThreshold(vThreshold)
  { }

  
  inline bool CompareLandingCandidates::operator () ( const LandingCandidate& lhs, const LandingCandidate& rhs )
  {
    const Box& lhsBox = std::get<2>( lhs );
    const Box& rhsBox = std::get<2>( rhs );

    if (lhsBox.isEmpty() xor rhsBox.isEmpty()) return lhsBox.isEmpty();

    DbU::Unit lhsWidth  = lhsBox.getWidth();
    DbU::Unit lhsHeight = lhsBox.getHeight();
    DbU::Unit rhsWidth  = rhsBox.getWidth();
    DbU::Unit rhsHeight = rhsBox.getHeight();

    bool lhsBelow  = (lhsWidth  < _hThreshold) or (lhsHeight < _vThreshold);
    bool rhsBelow  = (rhsWidth  < _hThreshold) or (rhsHeight < _vThreshold);

    cdebug_log(112,0) << "Compare: " << endl;
    cdebug_log(112,0) << "  " << lhsBox << " < " << rhsBox << endl;
    if (lhsBelow  xor rhsBelow ) {
      cdebug_log(112,0) << "  width " << lhsBelow << " < " << rhsBelow << endl;
      return lhsBelow;
    }
    // if (lhsHeightBelow xor rhsHeightBelow) {
    //   cdebug_log(112,0) << "  height " << lhsHeightBelow << " < " << rhsHeightBelow << endl;
    //   return lhsHeightBelow;
    // }

    if (lhsHeight != rhsHeight) return (lhsHeight < rhsHeight);
    return (lhsWidth < rhsWidth);

  //if (lhsWidth != rhsWidth ) return (lhsWidth < rhsWidth);
  //return (lhsHeight < rhsHeight);

  // Dangerous, may generate non-deterministic behavior.
  //if (lhsHeight != rhsHeight) return (lhsHeight > rhsHeight);
  //return lhs->getComponent()->getId() < rhs->getComponent()->getId();
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::LandingZone"

  Name  LandingZone::_name = "Anabatic::LandingZone";


  LandingZone::LandingZone ( const Configuration* conf, Component* component )
    : PrivateProperty()
    , _area()
  {
    cdebug_log(112,1) << "LandingZone::LandingZone(Configuration*,Component*): " << component << endl;

    _area = expandFromBb( conf
                        , component->getNet()
                        , component->getLayer()
                        , component->getBoundingBox() );
    component->put( this );
    cdebug_tabw(112,-1);
  }


  LandingZone::LandingZone ( const Configuration* conf, Net* net )
    : PrivateProperty()
    , _area()
  {
    if ((net->getCell()->getName() == "gf180mcu_fd_sc_mcu9t5v0_nand2_1") and (net->getName() == "ZN"))
      DebugSession::open( 112, 120 );
    cdebug_log(112,1) << "LandingZone::LandingZone(Configuration*,Net*): " << net << endl;

    Box                ab             = net->getCell()->getAbutmentBox();
    const Layer*       via12          = conf->getContactLayer( 0 );
    const Layer*       metal1         = conf->getLayerGauge( 0 )->getLayer();
    RoutingLayerGauge* gauge          = conf->getLayerGauge( 1 );
    DbU::Unit          hThreshold     = gauge->getPitch() + 2*gauge->getLayer()->getMinimalSpacing();
    DbU::Unit          vThreshold     = conf->getLayerGauge( 0 )->getPitch() + 2*metal1->getMinimalSpacing();
    vector<Box>        candidates;

    vector< LandingCandidate > landingZones;
    for ( Component* component : net->getComponents() ) {
      cdebug_log(112,0) << "@ " << component << endl;
      if (component->getLayer()->getMask() != metal1->getMask()) {
        cdebug_log(112,0) << "  Not in metal1, skip." << endl;
        continue;
      }
      Rectilinear* rectilinear = dynamic_cast<Rectilinear*>( component );
      if (rectilinear) {
        vector<Box> rbbs;
        rectilinear->getAsRectangles( rbbs,  Rectilinear::HSliced );
        for ( Box& bb : rbbs ) {
          cdebug_log(145,0) << "| seedArea (rectilinear):" << bb << endl;
          candidates.clear();
          LandingZone::expandFromBb( conf
                                   , component->getNet()
                                   , component->getLayer()
                                   , bb
                                   , candidates );
          for ( const Box& candidate : candidates )
            landingZones.push_back( make_tuple( nullptr, bb, candidate ));
        }
      } else {
        cdebug_log(145,0) << "| seedArea:" << component->getBoundingBox() << endl;
        candidates.clear();
        LandingZone::expandFromBb( conf
                                 , component->getNet()
                                 , component->getLayer()
                                 , component->getBoundingBox()
                                 , candidates );
        for ( const Box& candidate : candidates )
          landingZones.push_back( make_tuple( component
                                            , component->getBoundingBox(), candidate ));
      }
      
    }
    if (landingZones.empty()) {
      cdebug_tabw(112,-1);
      return;
    }
    stable_sort( landingZones.begin(), landingZones.end(), CompareLandingCandidates(hThreshold,vThreshold) );
    cdebug_log(112,0) << "hThreshold=" << DbU::getValueString(hThreshold) << endl;
    cdebug_log(112,0) << "vThreshold=" << DbU::getValueString(vThreshold) << endl;
    for ( size_t i=0 ; i<landingZones.size() ; ++i ) {
      cdebug_log(112,0) << "[" << i << "] LandingZone" << endl;
      const Box& seed = std::get<1>( landingZones[i] );
      const Box& area = std::get<2>( landingZones[i] );
      cdebug_log(112,0) << "    comp=" << std::get<0>( landingZones[i] ) << endl;
      cdebug_log(112,0) << "    seed=" << seed << " w=" << DbU::getValueString(seed.getWidth()) << " h=" << DbU::getValueString(seed.getHeight()) << endl;
      cdebug_log(112,0) << "    lz  =" << area << " w=" << DbU::getValueString(area.getWidth()) << " h=" << DbU::getValueString(area.getHeight()) << endl;
    }
    Component* component = std::get<0>( landingZones.back() );
    if (not component) {
      Box seedArea = std::get<1>( landingZones.back() );
      component = Horizontal::create( net
                                    , metal1
                                    , seedArea.getYCenter()
                                    , seedArea.getHeight()
                                    , seedArea.getXMin()
                                    , seedArea.getXMax()
                                    );
    }
    component->put( this );
    _area = std::get<2>( landingZones.back() );
    if (_area.getWidth() < hThreshold)
      cerr << Error( "LandingZone(): For terminal \"%s\" in cell \"%s\",\n"
                     "        No suitable landing zone found width %s inferior to %s."
                   , getString(component->getNet()->getName()).c_str()
                   , getString(component->getCell()->getName()).c_str()
                   , DbU::getValueString(_area.getWidth()).c_str()
                   , DbU::getValueString(hThreshold).c_str()
                   ) << endl;
    if (_area.getHeight() < vThreshold)
      cerr << Error( "LandingZone(): For terminal \"%s\" in cell \"%s\",\n"
                     "        No suitable landing zone found height %s inferior to %s."
                   , getString(component->getNet()->getName()).c_str()
                   , getString(component->getCell()->getName()).c_str()
                   , DbU::getValueString(_area.getHeight()).c_str()
                   , DbU::getValueString(vThreshold).c_str()
                   ) << endl;
    cdebug_tabw(112,-1);
    if ((net->getCell()->getName() == "gf180mcu_fd_sc_mcu9t5v0_nand2_1") and (net->getName() == "ZN"))
      DebugSession::close();
  }


  void  LandingZone::expandFromBb ( const Configuration* conf
                                  ,       Net*           net
                                  , const Layer*         layer
                                  ,       Box            seedArea
                                  ,       vector<Box>&   candidates )
  {
    size_t             depth      = conf->getLayerDepth( layer );
    RoutingLayerGauge* gauge      = conf->getLayerGauge( depth );
    DbU::Unit          spacing    = layer->getMinimalSpacing();
    DbU::Unit          pitch      = gauge->getPitch();
    Layer::Mask        filterMask = layer->getMask();
    vector<Box>        areas;
    if (layer->getBlockageLayer())
      filterMask |= layer->getBlockageLayer()->getMask();
    Box startArea = seedArea;
    startArea.inflate( 3*pitch );
    startArea = startArea.getIntersection( net->getCell()->getAbutmentBox() );
    areas.push_back( startArea );

    cdebug_log(112,1) << "In " << net->getCell() << endl;
    cdebug_log(112,0) << "Landing Zone for " << net << endl;
    vector<Box> bbs;
    for ( Component* neighbor : net->getCell()->getComponentsUnder( startArea, filterMask )) {
      bbs.clear();
      if (neighbor->getNet() == net) continue;
      cdebug_log(112,1) << "| neighbor:" << neighbor << endl;

      Rectilinear* rectilinear = dynamic_cast<Rectilinear*>( neighbor );
      if (rectilinear)
        rectilinear->getAsRectangles( bbs, Rectilinear::VSliced );
      else
        bbs.push_back( neighbor->getBoundingBox() );

      for ( const Box& bb : bbs ) {
        vector<Box> areaForks;
        for ( Box& area : areas ) {
          if (not area.intersect(bb)) continue;
          cdebug_log(112,0) << "> prev. area=" << area << endl;
          cdebug_log(112,0) << "| bb=" << bb << endl;

          if (bb.getYMin() >= seedArea.getYMax()) {
            cdebug_log(112,0) << "  North side" << endl; 
            if (not ((bb.getXMin() <= seedArea.getXMin()) and (bb.getXMax() >= seedArea.getXMax()))) {
            // Not fully enclosing the seedArea.
              if (((bb.getXMin() > seedArea.getXMin()) and (bb.getXMax() < seedArea.getXMax()))) {
                cdebug_log(112,0) << "  Fork (2) Full North: fully enclosed" << endl; 
                areaForks.push_back( Box( area.getXMin()
                                        , area.getYMin()
                                        , bb.getXMin()
                                        , area.getYMax() ));
                areaForks.push_back( Box( bb.getXMax()
                                        , area.getYMin()
                                        , area.getXMax()
                                        , area.getYMax() ));
              } else if (bb.getXMin() < seedArea.getXMin()) {
                cdebug_log(112,0) << "  Fork (1) Full North: overlap left" << endl; 
                areaForks.push_back( Box( bb.getXMax()
                                        , area.getYMin()
                                        , area.getXMax()
                                        , area.getYMax() ));
              } else {
                cdebug_log(112,0) << "  Fork (1) Full North: overlap right" << endl; 
                areaForks.push_back( Box( area.getXMin()
                                        , area.getYMin()
                                        , bb.getXMin()
                                        , area.getYMax() ));
              }
            }
            cdebug_log(112,0) << "  Full North" << endl; 
            area.inflate( 0, 0, 0, bb.getYMin() - area.getYMax() );
            continue;
          }

          if (bb.getYMax() <= seedArea.getYMin()) {
            cdebug_log(112,0) << "  South side" << endl; 
            if (not ((bb.getXMin() <= seedArea.getXMin()) and (bb.getXMax() >= seedArea.getXMax()))) {
            // Not fully enclosing the seedArea.
              if (((bb.getXMin() > seedArea.getXMin()) and (bb.getXMax() < seedArea.getXMax()))) {
                cdebug_log(112,0) << "  Fork (2) Full South: fully enclosed" << endl; 
                areaForks.push_back( Box( area.getXMin()
                                        , area.getYMin()
                                        , bb.getXMin()
                                        , area.getYMax() ));
                areaForks.push_back( Box( bb.getXMax()
                                        , area.getYMin()
                                        , area.getXMax()
                                        , area.getYMax() ));
              } else if (bb.getXMin() < seedArea.getXMin()) {
                cdebug_log(112,0) << "  Fork (1) Full South: overlap left" << endl; 
                areaForks.push_back( Box( bb.getXMax()
                                        , area.getYMin()
                                        , area.getXMax()
                                        , area.getYMax() ));
              } else {
                cdebug_log(112,0) << "  Fork (1) Full South: overlap right" << endl; 
                areaForks.push_back( Box( area.getXMin()
                                        , area.getYMin()
                                        , bb.getXMin()
                                        , area.getYMax() ));
              }
            }
            cdebug_log(112,0) << "  Full South" << endl; 
            area.inflate( 0, area.getYMin() - bb.getYMax(), 0, 0 );
            continue;
          }




          
          areas.insert( areas.end(), areaForks.begin(), areaForks.end() );
          


        } // areas.
      } // bb.
      cdebug_tabw(112,-1);
      
    } // neighbor.

    for ( Box& area : areas ) {
      cdebug_log(112,0) << "> final raw area=" << area << endl;
      area.inflate( -spacing );
      cdebug_log(112,0) << "> final lz area=" << area << endl;
    }
    cdebug_tabw(112,-1);
    candidates.insert( candidates.end(), areas.begin(), areas.end() );

#if FIRST_IMPLEMENTATION
    cdebug_log(112,1) << "In " << net->getCell() << endl;
    cdebug_log(112,0) << "Landing Zone for " << net << endl;
    vector<Box> bbs;
    for ( Component* neighbor : net->getCell()->getComponentsUnder( area, filterMask )) {
      bbs.clear();
      if (neighbor->getNet() == net) continue;
      cdebug_log(112,1) << "| neighbor:" << neighbor << endl;

      Rectilinear* rectilinear = dynamic_cast<Rectilinear*>( neighbor );
      if (rectilinear)
        rectilinear->getAsRectangles( bbs, Rectilinear::VSliced );
      else
        bbs.push_back( neighbor->getBoundingBox() );

      for ( const Box& bb : bbs ) {
        if (not area.intersect(bb)) continue;
        cdebug_log(112,0) << "> prev. area=" << area << endl;
        cdebug_log(112,0) << "| bb=" << bb << endl;
        
      // Full north.
        if (bb.getYMin() >= seedArea.getYMax()) {
          cdebug_log(112,0) << "  Full North" << endl; 
          if (bb.getYMin() - seedArea.getYMax() < 2*pitch) {
            if (seedArea.getXMax() - bb.getXMax() > 2*pitch) {
              cdebug_log(112,0) << "  North as West" << endl; 
              area.inflate( area.getXMin() - bb.getXMax(), 0, 0, 0 );
              continue;
            }
            if (bb.getXMin() - seedArea.getXMin() > 2*pitch) {
              cdebug_log(112,0) << "  North as East" << endl; 
              area.inflate( 0, 0, bb.getXMin() - area.getXMax(), 0 );
              continue;
            }
          }
          area.inflate( 0, 0, 0, bb.getYMin() - area.getYMax() );
          continue;
        }
      // Full south.
        if (bb.getYMax() <= seedArea.getYMin()) {
          cdebug_log(112,0) << "  Full South" << endl; 
          if (seedArea.getYMin() - bb.getYMax() < 2*pitch) {
            if (seedArea.getXMax() - bb.getXMax() > 2*pitch) {
              cdebug_log(112,0) << "  South as West" << endl; 
              area.inflate( area.getXMin() - bb.getXMax(), 0, 0, 0 );
              continue;
            }
            if (bb.getXMin() - seedArea.getXMin() > 2*pitch) {
              cdebug_log(112,0) << "  South as East" << endl; 
              area.inflate( 0, 0, bb.getXMin() - area.getXMax(), 0 );
              continue;
            }
          }
          area.inflate( 0, area.getYMin() - bb.getYMax(), 0, 0 );
          continue;
        }
      // Full east/west.
        if ((bb.getYMin() >= seedArea.getYMin()) and (bb.getYMax() <= seedArea.getYMax())) {
        // Full west.
          if (bb.getXMax() < seedArea.getXMin()) {
            cdebug_log(112,0) << "  Full West" << endl; 
            area.inflate( area.getXMin() - bb.getXMax(), 0, 0, 0 );
            continue;
          }
        // Full east.
          if (bb.getXMin() < seedArea.getXMax()) {
            cdebug_log(112,0) << "  Full East" << endl; 
            area.inflate( 0, 0, area.getXMax() - bb.getXMin(), 0 );
            continue;
          }
        }
      // Hybrid cases (corners).
        if ((bb.getYMax() > seedArea.getYMax()) and (bb.getYMin() > seedArea.getYCenter())) {
          Interval vfree = Interval( seedArea.getYMin(), bb.getYMin() );
          if (vfree.getSize() >= 2*pitch) {
            cdebug_log(112,0) << "  As North decrease YMax of "
                              << DbU::getValueString(bb.getYMin() - area.getYMax()) << endl; 
            area.inflate( 0, 0, 0, bb.getYMin() - area.getYMax() );
            continue;
          }
        } else {
          if (bb.getYMax() < seedArea.getYCenter()) {
            Interval vfree = Interval( bb.getYMax(), seedArea.getYMax() );
            if (vfree.getSize() >= 2*pitch) {
              cdebug_log(112,0) << "  As South" << endl; 
              area.inflate( 0, area.getYMin() - bb.getYMax(), 0, 0 );
              continue;
            }
          }
        }

        if (bb.getXMax() < seedArea.getXMin()) {
          cdebug_log(112,0) << "  As West" << endl; 
          area.inflate( area.getXMin() - bb.getXMax(), 0, 0, 0 );
          continue;
        }

        cdebug_log(112,0) << "  As East" << endl; 
        area.inflate( 0, 0, bb.getXMin() - area.getXMax(), 0 );
      }
      cdebug_tabw(112,-1);
    }

    cdebug_log(112,0) << "> final raw area=" << area << endl;
    area.inflate( -spacing );
    cdebug_log(112,-1) << "> final lz area=" << area << endl;
#endif

    return area;
  }


  void  LandingZone::onReleasedBy ( DBo* owner )
  { PrivateProperty::onReleasedBy( owner ); }


  Name  LandingZone::getPropertyName ()
  { return _name; }


  Name  LandingZone::getName () const
  { return getPropertyName(); }


  string  LandingZone::_getTypeName () const
  { return _TName( "LandingZone" ); }


  string  LandingZone::_getString () const
  {
    string s = PrivateProperty::_getString ();
    Component* owner = dynamic_cast<Component*>( getOwner() );
    if (owner) {
      s.insert ( s.length() - 1 , " " + getString(owner->getCell()->getName())
                                + ":" + getString(owner->getNet ()->getName())
                                + " area=" + getString(_area) );
    }
    return s;
  }


  Record* LandingZone::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if ( record ) {
      record->add( getSlot( "_name",  _name  ));
      record->add( getSlot( "_area" , &_area ));
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "LandingZoneExtension"


  const Component* LandingZoneExtension::_owner = nullptr;
  LandingZone*     LandingZoneExtension::_cache = nullptr;


  LandingZone* LandingZoneExtension::get ( const Component* component )
  {
    if (component == _owner) return _cache;
    _owner = component;

    Property* property = _owner->getProperty( LandingZone::getPropertyName() );
    if (property) _cache = static_cast<LandingZone*>( property );
    else          _cache = nullptr;

    return _cache;
  }


  LandingZone* LandingZoneExtension::create ( const Configuration* conf, Component* component )
  {
    get( component );
    if (_cache) return _cache;

    LandingZone* property = new LandingZone( conf, component );
    AreaMarker::create( component->getCell()
                      , "LZ:" + getString(component->getNet()->getName())
                      ,  property->getArea() );

    _cache = property;
    return _cache;
  }


  LandingZone* LandingZoneExtension::create ( const Configuration* conf, Net* net )
  {
    for ( Component* component : NetExternalComponents::get(net) ) {
      get( component );
      if (_cache) return _cache;
    }

    LandingZone* property  = new LandingZone( conf, net );
    Component*   component = property->getComponent();
    AreaMarker::create( component->getCell()
                      , "LZ:" + getString(component->getNet()->getName())
                      ,  property->getArea() );

    _cache = property;
    return _cache;
  }



}  // Anabatic namespace.
