// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/AnalogDistance.h"                       |
// +-----------------------------------------------------------------+


#ifndef BORA_ANALOG_DISTANCE_H
#define BORA_ANALOG_DISTANCE_H

#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Cell.h"
#include "anabatic/Dijkstra.h"
#include "anabatic/GCell.h"


namespace Bora {

  using Hurricane::DbU;
  using Hurricane::Net;
  using Hurricane::Point;
  using Hurricane::Component;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Anabatic::Edge;
  using Anabatic::Vertex;
  using Anabatic::GCell;
  using Anabatic::Dijkstra;
  using Anabatic::IntervalC;


  class AnalogDistance {
    public:
                       AnalogDistance       ( Cell*, DbU::Unit, DbU::Unit );
      inline Net*      getNet               () const;
      inline void      setNet               ( Net* );
             DbU::Unit operator()           ( const Vertex*, const Vertex*, const Edge* ) const;
             DbU::Unit addCost              ( const Vertex*, const Vertex*, DbU::Unit   ) const;
             DbU::Unit getPenality          ( const DbU::Unit ) const;
      inline double    getReferenceDistance () const;
             bool      isXUnion             ( const GCell*, const GCell* ) const;
             bool      isYUnion             ( const GCell*, const GCell* ) const;
             bool      isXUnion             ( DbU::Unit, DbU::Unit, const GCell* ) const;
             bool      isYUnion             ( DbU::Unit, DbU::Unit, const GCell* ) const;
             DbU::Unit calcDistance         ( const Point p1, const Point p2 ) const;
    private:
             Cell*      _cell;
             DbU::Unit  _hpitch;
             DbU::Unit  _vpitch;
      static Net*       _net;
  };


  Net* AnalogDistance::_net = NULL;

  
  AnalogDistance::AnalogDistance ( Cell* cell, DbU::Unit h, DbU::Unit v ) 
    : _cell  (cell)
    , _hpitch(h)
    , _vpitch(v)
  { }


  inline void    AnalogDistance::setNet               ( Net* net ) { _net = net;  }
  inline Net*    AnalogDistance::getNet               () const     { return _net; }
  inline double  AnalogDistance::getReferenceDistance () const     { return std::max(_cell->getAbutmentBox().getHeight(),_cell->getAbutmentBox().getWidth() ); }
      

  DbU::Unit  AnalogDistance::operator() ( const Vertex* vcurr, const Vertex* vnext, const Edge* e ) const
  {
    cdebug_log(112,1) << "Calcul _distance: "<< _net << std::endl;

    DbU::Unit distance      = vcurr->getDistance();
    DbU::Unit addedDistance = 0;

    if (Vertex::isRestricted(vcurr, vnext, e, _hpitch, _vpitch, _net)) {
      cdebug_log(112,1) << "Unreachable, restricted "<< e << std::endl;
      distance = Vertex::unreachable;
    } else {
      if ( (vcurr->getGCell()->isMatrix()) and (vnext->getGCell()->isMatrix()) ) {
        cdebug_tabw(112,-1);
        return distance + e->getDistance();
      } else {
        Point pcurrent   = vcurr->getStartPathPoint( vnext );
        Point pneighbour = vcurr->getNextPathPoint ( pcurrent, vnext );
        
        addedDistance = std::abs( pneighbour.getX() - pcurrent.getX()) + std::abs(pneighbour.getY() - pcurrent.getY() );
        cdebug_log(112,0) << "Pcurrent  : X:" << DbU::getValueString(pcurrent.getX())   << ", Y:" << DbU::getValueString(pcurrent.getY())   << std::endl;
        cdebug_log(112,0) << "Pneighbour: X:" << DbU::getValueString(pneighbour.getX()) << ", Y:" << DbU::getValueString(pneighbour.getY()) << std::endl;
        addedDistance += addCost(vcurr, vnext, addedDistance); 
        distance += addedDistance;
      }
    }

    cdebug_tabw(112,-1);
    return distance;
  }


  DbU::Unit  AnalogDistance::addCost ( const Vertex* vcurr, const Vertex* vnext, DbU::Unit aDistance ) const
  {
    cdebug_log(112,1) << "AnalogDistance::addCost(const Vertex* vcurr,const Vertex* vnext): aDistance: "
                      << DbU::getValueString(aDistance) << std::endl;

    double    addedDistance = aDistance;
    DbU::Unit supDist       = 0;
    Vertex*   vprev         = NULL;
    GCell*    gcurr         = vcurr->getGCell();
    GCell*    gnext         = vnext->getGCell();
    GCell*    gprev         = vcurr->getGPrev(Vertex::From2Mode);
    if (gprev) vprev = gprev->getObserver<Vertex>(GCell::Observable::Vertex);
    
  // Case: Device, only simple turn case possible
    if (vcurr->getRpCount()) {
      RoutingPad* rp   = NULL;
      for ( Component* component : _cell->getComponentsUnder(gcurr->getBoundingBox()) ) {
        rp = dynamic_cast<RoutingPad*>( component );
        if (rp) break;
      }
      if (rp) {
        Horizontal* h = dynamic_cast<Horizontal*>( rp->_getEntityAs<Segment>() );
        Vertical*   v = dynamic_cast<Vertical*  >( rp->_getEntityAs<Segment>() );
        cdebug_log(112,0) << "Current: isDevice, ";
        if (h) {
          cdebug_log(112,0) << " rp: Horizontal and ";
          if ( (gcurr->isNorth(gnext)) or (gcurr->isSouth(gnext)) ) {
            cdebug_log(112,0) << "COST." << std::endl;
            supDist += getPenality(addedDistance)*1;
          } else
            cdebug_log(112,0) << "FREE." << std::endl;
        } else if (v) {
          cdebug_log(112,0) << " rp: Vertical and ";
          if ( (gcurr->isWest(gnext)) or (gcurr->isEast(gnext)) ) {
            cdebug_log(112,0) << "COST." << std::endl;
            supDist += getPenality(addedDistance)*1;
          } else
            cdebug_log(112,0) << "FREE." << std::endl;
        } else
          cdebug_log(112,0) << "IS UNKNOWN.1" << std::endl;
      }
    } else {
      IntervalC intervfrom = vcurr->getIntervFrom( Vertex::From2Mode );

      cdebug_log(112,0) << "Case: vcurr is NOT a source nor a device " << std::endl;
      cdebug_log(112,0) << "(gprev->isEast (gcurr):" << gprev->isWest (gcurr) << std::endl;
      cdebug_log(112,0) << "(gcurr->isEast (gnext):" << gcurr->isWest (gnext) << std::endl;
      cdebug_log(112,0) << "isXUnion(gprev, gnext):" << isXUnion(gprev, gnext) << std::endl;
      cdebug_log(112,0) << "isYUnion(gprev, gnext):" << isYUnion(gprev, gnext) << std::endl;
         
      if (vcurr->areSameSide(vprev,vnext)) {
        cdebug_log(112,0) << "Case: Turn around" << std::endl;
        supDist += getPenality(addedDistance)*4;
      } else if (  (   (gcurr->isNorth(gprev) or gcurr->isSouth(gprev))
                   and (gcurr->isEast (gnext) or gcurr->isWest (gnext)) )
                or (   (gcurr->isEast (gprev) or gcurr->isWest (gprev))
                   and (gcurr->isNorth(gnext) or gcurr->isSouth(gnext)) ) ) {
        cdebug_log(112,0) << "Case: Simple turn" << std::endl;
        supDist += getPenality( addedDistance );
      } else if (vprev->isH()) {
        cdebug_log(112,0) << "Case: vprev is horizontal" << std::endl;
        if ( ( (  (gcurr->isSouth(gprev) and gcurr->isNorth(gnext))
               or (gcurr->isNorth(gprev) and gcurr->isSouth(gnext)) )
             and (not isXUnion(intervfrom.getMin(), intervfrom.getMax(), gnext)) )
           or (  (  (gcurr->isWest (gprev) and gcurr->isEast (gnext))
                 or (gcurr->isEast (gprev) and gcurr->isWest (gnext)) )
              and (  (intervfrom.getAxis() < gnext->getYMin()) 
                  or (intervfrom.getAxis() > gnext->getYMax()) ) ) ) {
          cdebug_log(112,0) << "Case: Double turn1" << std::endl;
          cdebug_log(112,0) << "((gcurr->isSouth(gprev)) && (gcurr->isNorth(gnext)))        : " << ((gcurr->isSouth(gprev)) && (gcurr->isNorth(gnext))) << std::endl;
          cdebug_log(112,0) << "((gcurr->isNorth(gprev)) && (gcurr->isSouth(gnext)))        : " << ((gcurr->isNorth(gprev)) && (gcurr->isSouth(gnext))) << std::endl;
          cdebug_log(112,0) << "(!isXUnion(intervfrom.getMin(), intervfrom.getMax(), gnext)): " << (!isXUnion(intervfrom.getMin(), intervfrom.getMax(), gnext)) << std::endl;
          supDist += getPenality( addedDistance )*1;
        } else
          cdebug_log(112,0) << "Case: No cost" << std::endl;
      } else if (vprev->isV()) {
        cdebug_log(112,0) << "Case: vprev is vertical" << std::endl;
        if (  ( (  (gcurr->isWest(gprev) and gcurr->isEast (gnext))
                or (gcurr->isEast(gprev) and gcurr->isWest (gnext)) )
              and (not isYUnion(intervfrom.getMin(), intervfrom.getMax(), gnext)) ) 
           or ( (  (gcurr->isSouth(gprev) and gcurr->isNorth(gnext))
                or (gcurr->isNorth(gprev) and gcurr->isSouth(gnext)) )
              and (  (intervfrom.getAxis() < gnext->getXMin()) 
                  or (intervfrom.getAxis() > gnext->getXMax()) ) ) ) {
          cdebug_log(112,0) << "Case: Double turn2" << std::endl;
          cdebug_log(112,0) << "((gcurr->isWest(gprev)) && (gcurr->isEast(gnext)))          :" << ((gcurr->isWest(gprev)) && (gcurr->isEast(gnext))) << std::endl;
          cdebug_log(112,0) << "((gcurr->isEast(gprev)) && (gcurr->isWest(gnext)))          :" << ((gcurr->isEast(gprev)) && (gcurr->isWest(gnext))) <<  std::endl;
          cdebug_log(112,0) << "(!isYUnion(intervfrom.getMin(), intervfrom.getMax(), gnext)): " <<  (!isYUnion(intervfrom.getMin(), intervfrom.getMax(), gnext)) << std::endl;
          supDist += getPenality( addedDistance )*1;
        } else
          cdebug_log(112,0) << "Case: No cost" << std::endl;
      } else cdebug_log(112,0) << "Case: No cost" << std::endl;
    }
    
  // Case: Next is Device, add cost to arrive to RP 
    if (vcurr->getRpCount()) {
      RoutingPad* rp = NULL;
      for ( Component* component : _cell->getComponentsUnder(gnext->getBoundingBox()) ) {
        rp = dynamic_cast<RoutingPad*>( component );
        if (rp) break;
      }
      if (rp) {
        cdebug_log(112,0) << "Case: Next is a Device and ";
        Horizontal* h = dynamic_cast<Horizontal*>( rp->_getEntityAs<Segment>() );
        Vertical*   v = dynamic_cast<Vertical*  >( rp->_getEntityAs<Segment>() );
        if (h) {
          if (gcurr->isNorth(gnext) or gcurr->isSouth(gnext)) {
            cdebug_log(112,0) << "COST." << std::endl;
            supDist += getPenality( addedDistance )*1;
          } else
            cdebug_log(112,0) << "FREE." << std::endl;
        } else if (v) {
          if (gcurr->isWest(gnext) or gcurr->isEast(gnext)) {
            cdebug_log(112,0) << "COST." << std::endl;
            supDist += getPenality( addedDistance )*1;
          } else
            cdebug_log(112,0) << "FREE." << std::endl;
        }
      }
    }

    cdebug_log(112,0) << "supDist = " << DbU::getValueString(supDist) << std::endl;
    cdebug_tabw(112,-1);

    return supDist;
  }


  inline  DbU::Unit AnalogDistance::getPenality ( const DbU::Unit addedDistance ) const     
  {
    double    refcost = std::max( _hpitch, _vpitch )*1; 
    double    refdist = getReferenceDistance();
    DbU::Unit pen     = refcost;

    if (addedDistance < refdist) pen = ( (refdist - addedDistance) / refdist) * refcost;
    return pen;
  }


  bool  AnalogDistance::isXUnion( const GCell* prev, const GCell* next ) const
  {
    if ( not prev or not next) return false;

    if (  (prev->getXMin() > next->getXMax())
       or (prev->getXMax() < next->getXMin()) ) return false; 

    return true;
  }


  bool  AnalogDistance::isXUnion ( DbU::Unit xmin, DbU::Unit xmax, const GCell* next ) const
  {
    if (not next) return false;

    if (  (xmin > next->getXMax())
       or (xmax < next->getXMin()) ) return false; 

    return true;
  }


  bool  AnalogDistance::isYUnion ( const GCell* prev, const GCell* next ) const
  {
    if (not prev or not next) return false;

    if (  (prev->getYMin() > next->getYMax())
       or (prev->getYMax() < next->getYMin()) ) return false; 

    return true;
  }


  bool AnalogDistance::isYUnion ( DbU::Unit ymin, DbU::Unit ymax, const GCell* next ) const
  {
    if (not next) return false;

    if (  (ymin > next->getYMax())
       or (ymax < next->getYMin()) ) return false; 

    return true;
  }


  DbU::Unit  AnalogDistance::calcDistance ( const Point p1, const Point p2 ) const
  {
    return abs( p1.getX() - p2.getX() ) + abs( p1.getY() - p2.getY() );
  }


}  // Bora namespace.

#endif  // BORA_ANALOG_DISTANCE_H
