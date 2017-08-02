// -*- mode: C++; explicit-buffer-name: "Dijkstra.cpp<anabatic>" -*-
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
// |  C++ Module  :  "./anabatic/Dijkstra.cpp"                       |
// +-----------------------------------------------------------------+


#include <limits>
#include <algorithm>
#include "hurricane/Error.h"
#include "hurricane/Net.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/DebugSession.h"
#include "crlcore/Utilities.h"
#include "anabatic/AnabaticEngine.h"
#include "anabatic/Dijkstra.h"
#include "hurricane/DataBase.h"
#include "hurricane/viewer/CellViewer.h"
#include "hurricane/Technology.h"
#include "hurricane/NetRoutingProperty.h"


namespace Anabatic {

  using std::cerr;
  using std::endl;
  using std::numeric_limits;
  using Hurricane::ForEachIterator;
  using Hurricane::Error;
  using Hurricane::Component;
  using Hurricane::Segment;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Hurricane::UpdateSession;
  using Hurricane::DebugSession;
  using Hurricane::NetRoutingExtension;


// -------------------------------------------------------------------
// Class  :  "Anabatic::IntervalC".


  IntervalC::IntervalC()
  {
    _min  = Vertex::unreached;
    _max  = Vertex::unreached;
    _axis = Vertex::unreached;
    _flags = 0;
  }


  IntervalC::IntervalC(IntervalC& i)
  {
    _min  = i.getMin();
    _max  = i.getMax();
    _axis = i.getAxis();
    setFlags(i.getFlags());
  }


  IntervalC::IntervalC(const IntervalC& i)
  {
    _min  = i.getMin();
    _max  = i.getMax();
    _axis = i.getAxis();
    setFlags(i.getFlags());
  }


  IntervalC::~IntervalC() {}


  void IntervalC::set ( DbU::Unit min, DbU::Unit max, DbU::Unit axis )
  {
    _min  = min;
    _max  = max;
    _axis = axis;
    setiSet();
  }


  void IntervalC::setRange( DbU::Unit vmin, DbU::Unit vmax )
  {
    if (vmin < vmax){
      _min = vmin;
      _max = vmax;
      setiSet();
    } else {
      _min = vmax;
      _max = vmin;
      setiSet();
    }
  }


  void  IntervalC::extendMin( DbU::Unit vmin )
  {
    if (_min > vmin) _min = vmin;
  }


  void  IntervalC::extendMax( DbU::Unit vmax )
  {
    if (_max < vmax) _max = vmax;
  }


  void IntervalC::print() const
  {
    cdebug_log(112,0) << "[IntervalC]: min: " << DbU::getValueString(_min) << ", max:" << DbU::getValueString(_max) << ", axis:" << DbU::getValueString(_axis) << endl;
  }


  void IntervalC::reset()
  {
    _min  = Vertex::unreached;
    _max  = Vertex::unreached;
    _axis = Vertex::unreached;
    _flags &= ~iSet;
  }

// -------------------------------------------------------------------
// Class  :  "Anabatic::GRAData".
  GRAData::GRAData ()
    : _intervfrom  (IntervalC())
    , _interv      (IntervalC())
    , _from2       (NULL)
    , _intervfrom2 (IntervalC())
  {}


  GRAData::~GRAData() {}


  GRAData* GRAData::create() 
  {
    return new GRAData();
  }


  void GRAData::resetIntervals()
  {
    _interv.reset();
    _intervfrom.reset();
  }


  void GRAData::clearFrom2 ()
  {
    _from2 = NULL;
  }

// -------------------------------------------------------------------
// Class  :  "Anabatic::Vertex".


  DbU::Unit  Vertex::unreached    = std::numeric_limits<long>::max();
  DbU::Unit  Vertex::unreachable  = std::numeric_limits<long>::max()-1;


  bool  Vertex::hasValidStamp () const
  { return _stamp == getAnabatic()->getStamp(); }


  bool Vertex::hasRP( Net* net ) const 
  { 
    if (getGCell() != NULL ){
      Cell* cell   = getGCell()->getAnabatic()->getCell();
      RoutingPad* rp   = NULL;
      for ( Component* component : cell->getComponentsUnder(getGCell()->getBoundingBox().inflate(-1)) ){
        rp = dynamic_cast<RoutingPad*>( component );
        if (rp) {
          if (rp->getNet() == net) return true;
        }
      }
    } 
    return false; 
  }


  bool Vertex::hasVRP( Net* net ) const 
  {
    if (getGCell() != NULL){
      Cell* cell   = getGCell()->getAnabatic()->getCell();
      RoutingPad* rp   = NULL;
      for ( Component* component : cell->getComponentsUnder(getGCell()->getBoundingBox().inflate(-1)) ){
        rp = dynamic_cast<RoutingPad*>( component );
        if (rp) {
          if  (rp->getNet() == net) break;
        }
      }
      if (rp) {
        Vertical* v = dynamic_cast<Vertical*>(rp->_getEntityAsSegment());
        if (v) { return true; }
      }
    } 
    return false; 
  }


  bool Vertex::hasHRP( Net* net ) const 
  {
    if (getGCell() != NULL){
      Cell* cell   = getGCell()->getAnabatic()->getCell();
      RoutingPad* rp   = NULL;
      for ( Component* component : cell->getComponentsUnder(getGCell()->getBoundingBox().inflate(-1)) ){
        rp = dynamic_cast<RoutingPad*>( component );
        if (rp) {
          if  (rp->getNet() == net) break;
        }
      }
      if (rp) {
        Horizontal* h = dynamic_cast<Horizontal*>(rp->_getEntityAsSegment());
        if (h) { return true; }
      }
    } 
    return false; 
  }


  bool Vertex::isRestricted ( const Vertex* v1, const Vertex* v2, const Edge* e, DbU::Unit hpitch, DbU::Unit vpitch, Net* net )
  {
    bool   restricted = true;
    GCell* c1         = v1->getGCell();
    GCell* c2         = v2->getGCell();
    
  // Check from GCell 1 
    if        ( c1->isNorth(c2) ) {
      if ( !v1->isNRestricted() ) restricted = false;
    } else if ( c1->isSouth(c2) ) {
      if ( !v1->isSRestricted() ) restricted = false;
    } else if ( c1->isEast (c2) ) {
      if ( !v1->isERestricted() ) restricted = false;
    } else if ( c1->isWest (c2) ) {
      if ( !v1->isWRestricted() ) restricted = false;
    } else {
      cerr << Error( "GCells are not side by side." ) << endl;
      return true;
    }

    if   ( (c1->getWidth()  < hpitch)
         ||(c1->getHeight() < vpitch)
         ||(restricted)
         ) return true;
    else {
      restricted = true;
    // Check from GCell 2 
      if        ( c2->isNorth(c1) ) {
        if ( !v2->isNRestricted() ) restricted = false;
      } else if ( c2->isSouth(c1) ) {
        if ( !v2->isSRestricted() ) restricted = false;
      } else if ( c2->isEast (c1) ) {
        if ( !v2->isERestricted() ) restricted = false;
      } else if ( c2->isWest (c1) ) {
        if ( !v2->isWRestricted() ) restricted = false;
      } else {
        cerr << Error( "GCells are not side by side." ) << endl;
        return true;
      }
      if   ( (c2->getWidth()  < hpitch)
           ||(c2->getHeight() < vpitch)
           ||(restricted)
           ) return true;
      else {
        if ((v2->getGCell()->isStrut())){
          if (e->isMaxCapacity(net)) return true;
          else return false;
        } else return false;
      }
    }
  }


  Point Vertex::getNextPathPoint( Point pcurr, const Vertex* vnext ) const
  {
    cdebug_log(112,1) << "Point Dijkstra::getNextPathPoint( Point pcurr, const Vertex* vnext )" << endl;
    if (vnext == NULL){
      cdebug_tabw(112,-1);
      return Point(0,0);
    }

    if (vnext->getGCell()->isMatrix()) {
      cdebug_tabw(112,-1);
      return Point(vnext->getGCell()->getXCenter(), vnext->getGCell()->getYCenter());
    }
    
    GCell* gnext = vnext->getGCell();
    GCell* gcurr = getGCell();
    DbU::Unit x = 0;
    DbU::Unit y = 0;


    if (vnext->isV()){
      cdebug_log(112,0) << "Case next: Vertical: " << vnext->isiSet() << endl; //", d:" << vnext->getDistance() << endl;
      if ((vnext->isiSet())&&(vnext->hasValidStamp())){
        cdebug_log(112,0) << "Case set" << endl;
        x = vnext->getIAxis();
        if (isNorth(vnext))      y = vnext->getIMin();
        else if (isSouth(vnext)) y = vnext->getIMax();
        else if ((isWest(vnext))||(isEast(vnext))) {
          if      ( pcurr.getY() > vnext->getIMax() ) y = vnext->getIMax();
          else if ( pcurr.getY() < vnext->getIMin() ) y = vnext->getIMin();
          else                                        y = pcurr.getY();
        } else  cdebug_log(112,0) << "[ERROR](Point Vertex::getNextPathPoint2(...) const: Something is wrong.1" << endl;
      } else {
        cdebug_log(112,0) << "Case not set" << endl;
        if (isNorth(vnext)){
          y = gcurr->getYMax();
          if      (pcurr.getX() < gnext->getXMin()) x = gnext->getXMin();
          else if (pcurr.getX() > gnext->getXMax()) x = gnext->getXMax();
          else                                      x = pcurr.getX();
        } else if (isSouth(vnext)){
          y = gcurr->getYMin();
          if      (pcurr.getX() < gnext->getXMin()) x = gnext->getXMin();
          else if (pcurr.getX() > gnext->getXMax()) x = gnext->getXMax();
          else                                      x = pcurr.getX();
        } else if (isWest(vnext)){
          x = gcurr->getXMin();
          if      (pcurr.getY() < gnext->getYMin()) y = gnext->getYMin();
          else if (pcurr.getY() > gnext->getYMax()) y = gnext->getYMax();
          else                                      y = pcurr.getY();
          
        } else if (isEast(vnext)){
          x = gcurr->getXMax();
          if      (pcurr.getY() < gnext->getYMin()) y = gnext->getYMin();
          else if (pcurr.getY() > gnext->getYMax()) y = gnext->getYMax();
          else                                      y = pcurr.getY();
        } else  cdebug_log(112,0) << "[ERROR](Point Vertex::getNextPathPoint2(...) const: Something is wrong.2" << endl;
      }
      
    } else if (vnext->isH()) {
      cdebug_log(112,0) << "Case next: Horizontal: " << vnext->isiSet() << endl; //", d:" << vnext->getDistance() << endl;
      
      if ((vnext->isiSet())&&(vnext->hasValidStamp())){
        cdebug_log(112,0) << "Case set" << endl;
        y = vnext->getIAxis();
        if      (isEast (vnext)) x = vnext->getIMin();
        else if (isWest (vnext)) x = vnext->getIMax();
        else if ((isNorth(vnext))||(isSouth(vnext))) {
          if      ( pcurr.getX() > vnext->getIMax() ) x = vnext->getIMax();
          else if ( pcurr.getX() < vnext->getIMin() ) x = vnext->getIMin();
          else                                        x = pcurr.getX();
        } else  cdebug_log(112,0) << "[ERROR](Point Vertex::getNextPathPoint2(...) const: Something is wrong.3" << endl;

      } else {
        cdebug_log(112,0) << "Case not set" << endl;
        if (isNorth(vnext)){
          y = gcurr->getYMax();
          if      (pcurr.getX() < gnext->getXMin()) x = gnext->getXMin();
          else if (pcurr.getX() > gnext->getXMax()) x = gnext->getXMax();
          else                                      x = pcurr.getX();
        } else if (isSouth(vnext)){
          y = gcurr->getYMin();
          if      (pcurr.getX() < gnext->getXMin()) x = gnext->getXMin();
          else if (pcurr.getX() > gnext->getXMax()) x = gnext->getXMax();
          else                                      x = pcurr.getX();
        } else if (isWest(vnext)){
          x = gcurr->getXMin();
          if      (pcurr.getY() < gnext->getYMin()) y = gnext->getYMin();
          else if (pcurr.getY() > gnext->getYMax()) y = gnext->getYMax();
          else                                      y = pcurr.getY();
          
        } else if (isEast(vnext)){
          x = gcurr->getXMax();
          if      (pcurr.getY() < gnext->getYMin()) y = gnext->getYMin();
          else if (pcurr.getY() > gnext->getYMax()) y = gnext->getYMax();
          else                                      y = pcurr.getY();
        } else  cdebug_log(112,0) << "[ERROR](Point Vertex::getNextPathPoint2(...) const: Something is wrong.4" << endl;
      }
    } else {
      cdebug_log(112,0) << "[ERROR](Point Vertex::getNextPathPoint2(...) const: Something is wrong.5" << endl;
    }
    cdebug_tabw(112,-1);
    return Point(x,y);
  }


  Point Vertex::getStartPathPoint( const Vertex* next ) const
  {
    cdebug_log(112,1) << "Point Vertex::getStartPathPoint( const Vertex* next ) const:" << this << endl;
    
    GCell*    gcurr      = getGCell();
    GCell*    gnext      = next->getGCell();
    DbU::Unit x          = 0;
    DbU::Unit y          = 0;
    IntervalC intervfrom = IntervalC();

    if (_adata == NULL){
      cdebug_log(112,0) << "Point Vertex::getStartPathPoint( const Vertex* next ) const: GRAData unset." << endl;
      cdebug_tabw(112,-1);
      return Point(0,0);
    }

    if (gcurr->isDevice  ()){
      cdebug_log(112,0) << "Case device" << endl;
      if      (isH()){
        cdebug_log(112,0) << "hinterval: " <<  DbU::getValueString(getIAxis()) << endl;
        y = getIAxis();
        if      ((gnext->getXMax() < getIMin())||(isWest (next))) x = getIMin();
        else if ((gnext->getXMin() > getIMax())||(isEast (next))) x = getIMax();
        else    x = (max(gnext->getXMin(), getIMin())+min(gnext->getXMax(), getIMax()))/2;

      } else if (isV()){
        cdebug_log(112,0) << "vinterval" << endl;
        x = getIAxis();
        if      ((gnext->getYMax() < getIMin())||(isSouth(next))) y = getIMin();
        else if ((gnext->getYMin() > getIMax())||(isNorth(next))) y = getIMax();
        else y = (max(gnext->getYMin(), getIMin())+min(gnext->getYMax(), getIMax()))/2 ;
      } else {
        cdebug_log(112,0) << "[ERROR](Point Vertex::getStartPathPoint( const Vertex * next ) const: Something is wrong." << endl;
        cdebug_tabw(112,-1);
        return Point(0,0);
      }
    } else if (isH()) {
      cdebug_log(112,0) << "Case horizontal: " << isiSet() << endl; 
      GCell* gprev = getGPrev(Vertex::From2Mode);
      intervfrom   = getIntervFrom(From2Mode);
      Vertex* prev = gprev->getObserver<Vertex>(GCell::Observable::Vertex);
      cdebug_log(112,0) << "PREV: " << prev << " ";  
      intervfrom.print();
     
      if (isiSet()){
        cdebug_log(112,0) << "isiSet: "; 
        printInterv();
        y = getIAxis();
        if      ((gnext->getXMax() < getIMin())||(isWest (next))) x = getIMin();
        else if ((gnext->getXMin() > getIMax())||(isEast (next))) x = getIMax();
        else    x = (max(gnext->getXMin(), getIMin())+min(gnext->getXMax(), getIMax()))/2;
      } else {
        if        (prev->isH()){
          cdebug_log(112,0) << "prev is H" << endl;
          if      (gnext->getXMax() < intervfrom.getMin()) x = intervfrom.getMin();
          else if (gnext->getXMin() > intervfrom.getMax()) x = intervfrom.getMax();
          else    x = (max(gnext->getXMin(), intervfrom.getMin())+min(gnext->getXMax(), intervfrom.getMax()))/2;
          if      (isNorth(prev)) y = gcurr->getYMax();
          else if (isSouth(prev)) y = gcurr->getYMin();
          else                    y = intervfrom.getAxis();
        } else if (prev->isV()){
          cdebug_log(112,0) << "prev is V" << endl;

          if        (isNorth(prev)){
          //cdebug_log(112,0) << "1" << endl;
            x = intervfrom.getAxis();
            y = gcurr->getYMax();
          } else if (isSouth(prev)){
          //cdebug_log(112,0) << "2" << endl;
            x = intervfrom.getAxis();
            y = gcurr->getYMin();
          } else if (isWest (prev)){
          //cdebug_log(112,0) << "3" << endl;
            x = gcurr->getXMin();
            if        (isNorth(next)){
              if   (intervfrom.getMax() > gcurr->getYMax()) y = gcurr->getYMax();
              else                                          y = intervfrom.getMax();
            } else if (isSouth(next)){
              if   (intervfrom.getMin() < gcurr->getYMin()) y = gcurr->getYMin();
              else                                          y = intervfrom.getMin();
            } else { // East side
              if        ( intervfrom.getMin() < gcurr->getYMin() ){ y = gcurr->getYMin();
              } else if ( intervfrom.getMax() > gcurr->getYMax() ){ y = gcurr->getYMax();
              } else {                                              y = (intervfrom.getMin() + intervfrom.getMax())/2 ;
              }
            }
          } else if (isEast (prev)){
          //cdebug_log(112,0) << "4" << endl;
            x = gcurr->getXMax();
            if        (isNorth(next)){
              if   (intervfrom.getMax() > gcurr->getYMax()) y = gcurr->getYMax();
              else                                           y = intervfrom.getMax();
            } else if (isSouth(next)){
              if   (intervfrom.getMin() < gcurr->getYMin()) y = gcurr->getYMin();
              else                                           y = intervfrom.getMin();
            } else { // West side
              if        ( intervfrom.getMin() < gcurr->getYMin() ){ y = gcurr->getYMin();
              } else if ( intervfrom.getMax() > gcurr->getYMax() ){ y = gcurr->getYMax();
              } else {                                               y = (intervfrom.getMin() + intervfrom.getMax())/2 ;
              }
            }
          } else {
            cdebug_log(112,0) << "[ERROR](Point Vertex::getStartPathPoint() const: Something is wrong." << endl;
            cdebug_tabw(112,-1);
            return Point(0,0);
          }
          cdebug_log(112,0) << "x: " << DbU::getValueString(x) << ", y:" << DbU::getValueString(y) << endl;
        } else {
          cdebug_log(112,0) << "[ERROR](Point Vertex::getStartPathPoint() const: Something is wrong." << endl;
          cdebug_tabw(112,-1);
          return Point(0,0);
        }
      }
    } else if (isV()) {
      cdebug_log(112,0) << "Case vertical: " << isiSet() << endl;
    //GCell*     gprev      = NULL;
      GCell* gprev = getGPrev(Vertex::From2Mode);
      intervfrom = getIntervFrom(From2Mode);
      Vertex* prev = gprev->getObserver<Vertex>(GCell::Observable::Vertex);
      cdebug_log(112,0) << "PREV: " << prev << " ";   
      intervfrom.print();  

      if (isiSet()){
        cdebug_log(112,0) << "isiSet: "; 
        printInterv();
        x = getIAxis();
        if      ((gnext->getYMax() <= getIMin())||(isSouth(next))){
          y = getIMin();
        }
        else if ((gnext->getYMin() >= getIMax())||(isNorth(next))){
          y = getIMax();
        }
        else {
          y = (max(gnext->getYMin(), getIMin())+min(gnext->getYMax(), getIMax()))/2 ;
        }
      } else {
        if        (prev->isH()){
          cdebug_log(112,0) << "prev is H" << endl;
          if        (isNorth(prev)){
            y = gcurr->getYMax();
            if        (isNorth(next)){
              if   (intervfrom.getMax() > gcurr->getXMax()) x = gcurr->getXMax();
              else                                          x = intervfrom.getMax();
            } else if (isSouth(next)){
              if   (intervfrom.getMin() < gcurr->getXMin()) x = gcurr->getXMin();
              else                                          x = intervfrom.getMin();
            } else { // West side
              if        ( intervfrom.getMin() < gcurr->getXMin() ){ x = gcurr->getXMin();
              } else if ( intervfrom.getMax() > gcurr->getXMax() ){ x = gcurr->getXMax();
              } else {                                              x = (intervfrom.getMin() + intervfrom.getMax())/2 ;
              }
            }
          } else if (isSouth(prev)){
            y = gcurr->getYMin();
            if        (isEast(next)){
              if   (intervfrom.getMax() > gcurr->getXMax()) x = gcurr->getXMax();
              else                                          x = intervfrom.getMax();
            } else if (isWest(next)){
              if   (intervfrom.getMin() < gcurr->getXMin()) x = gcurr->getXMin();
              else                                          x = intervfrom.getMin();
            } else { // Northside
              if        ( intervfrom.getMin() < gcurr->getXMin() ){ x = gcurr->getXMin();
              } else if ( intervfrom.getMax() > gcurr->getXMax() ){ x = gcurr->getXMax();
              } else {                                              x = (intervfrom.getMin() + intervfrom.getMax())/2 ;
              }
            }
          } else if (isWest (prev)){
            x = gcurr->getXMin();
            y = intervfrom.getAxis();
          } else if (isEast (prev)){
            x = gcurr->getXMax();
            y = intervfrom.getAxis();
          } else {
            cdebug_log(112,0) << "[ERROR](Point Vertex::getStartPathPoint() const: Something is wrong." << endl;
            cdebug_tabw(112,-1);
            return Point(0,0);
          }
        } else if (prev->isV()){
          cdebug_log(112,0) << "prev is V" << endl;
          if      (gnext->getYMax() < intervfrom.getMin())  { y = intervfrom.getMin();
          } else if (gnext->getYMin() > intervfrom.getMax()){ y = intervfrom.getMax();
          } else{ y = (max(gnext->getYMin(), intervfrom.getMin())+min(gnext->getYMax(), intervfrom.getMax()))/2;
          }

          if      (isEast(prev)) x = gcurr->getXMax();
          else if (isWest(prev)) x = gcurr->getXMin();
          else                   x = intervfrom.getAxis();
        } else {
          cdebug_log(112,0) << "[ERROR](Point Vertex::getStartPathPoint() const: Something is wrong." << endl;
          cdebug_tabw(112,-1);
          return Point(0,0);
        }
      }
    } else {
      cdebug_log(112,0) << "[ERROR](Point Vertex::getStartPathPoint() const: Something is wrong." << endl;
      cdebug_tabw(112,-1);
      return Point(0,0);
    }
    cdebug_tabw(112,-1);
    return Point(x,y);
  }


  bool Vertex::isH() const
  {
    GCell* gcell = getGCell();
    if      (gcell->isDevice())   return isiHorizontal();
    else if (gcell->isHChannel()) return true;
    else if (gcell->isStrut())    return ((gcell->getWidth() > gcell->getHeight())||(gcell->getWidth() == gcell->getHeight()));
    else                          return false;
  }


  bool Vertex::isV() const
  {
    GCell* gcell = getGCell();
    if      (gcell->isDevice())   return isiVertical();
    else if (gcell->isVChannel()) return true;
    else if (gcell->isStrut())    return gcell->getWidth() < gcell->getHeight();
    else                          return false;
  }

  
  void Vertex::setIntervals ( Vertex* vcurr )
  {
    cdebug_log(112,1) << "!SETINTERVALS! ( Vertex* vcurr )" << endl;
    Point pcurr;
    if (isFromFrom2()){
      vcurr->setFlags(Vertex::From2Mode);
      pcurr = vcurr->getStartPathPoint(this);
      vcurr->unsetFlags(Vertex::From2Mode);
    } else {
      pcurr = vcurr->getStartPathPoint(this);
    }
    Point pnext = vcurr->getNextPathPoint( pcurr, this );
    cdebug_log(112,0) << "Pcurrent  : X:" << DbU::getValueString(pcurr.getX())   << ", Y:" << DbU::getValueString(pcurr.getY())   << endl;
    cdebug_log(112,0) << "Pneighbour: X:" << DbU::getValueString(pnext.getX()) << ", Y:" << DbU::getValueString(pnext.getY()) << endl;
    DbU::Unit min, max, axis;

    if        (vcurr->isH()){
      cdebug_log(112,0) << "case vcurr: Horizontal" << endl;
      if ((vcurr->isiSet())&&(vcurr->hasValidStamp())){
        cdebug_log(112,0) << "case set" << endl;
        if      (vcurr->getIMin() > pnext.getX()) {
          min  = pnext.getX();
          max  = vcurr->getIMax();
          axis = vcurr->getIAxis();
        } else if (vcurr->getIMax() < pnext.getX()) {
          min  = vcurr->getIMin();
          max  = pnext.getX();
          axis = vcurr->getIAxis();
        } else {
          min  = vcurr->getIMin();
          max  = vcurr->getIMax();
          axis = vcurr->getIAxis();
        } 
      } else {
        cdebug_log(112,0) << "case not set" << endl;
        axis = pcurr.getY();
        bool hh = false;
        if (vcurr->hasValidStamp() && (vcurr->getFrom() != NULL)){
          GCell*  gprev = vcurr->getGPrev(Vertex::UseFromFrom2);
          Vertex* vprev = gprev->getObserver<Vertex>(GCell::Observable::Vertex);
          if (vprev->isH()) {
            cdebug_log(112,0) << "----------------------------" << endl;
            cdebug_log(112,0) << "HHCASE:" << endl;
            cdebug_log(112,0) << "prev: " << vprev << endl;
            cdebug_log(112,0) << "curr: " << vcurr << endl;
            cdebug_log(112,0) << "next: " << this << endl;
            cdebug_log(112,0) << "----------------------------" << endl;
            hh = true;
          }
        }
        if (hh){
          GCell*     gcurr      = vcurr->getGCell();
          GCell*     gnext      = getGCell();
          IntervalC intervfrom = vcurr->getIntervFrom(UseFromFrom2);
          vcurr->printIntervfrom();

          if        (gnext->getXMin() > intervfrom.getMax()){
          //cdebug_log(112,0) << "1" << endl;
            min = intervfrom.getMax();
            max = gnext->getXMin();
          } else if (gnext->getXMax() < intervfrom.getMin()){
          //cdebug_log(112,0) << "2" << endl;
            min = gnext->getXMax();
            max = intervfrom.getMin();
          } else {
          //cdebug_log(112,0) << "3" << endl;
            min = std::max(gcurr->getXMin(), intervfrom.getMin());
            max = std::min(gcurr->getXMax(), intervfrom.getMax());
          }
        } else {
          if (pcurr.getX() < pnext.getX()){
          //cdebug_log(112,0) << "4" << endl;
            min = pcurr.getX();
            max = pnext.getX();
          } else {
          //cdebug_log(112,0) << "5" << endl;
            max = pcurr.getX();
            min = pnext.getX();
          }
        }
      }

    } else if (vcurr->isV()){
      cdebug_log(112,0) << "case vcurr: Vertical" << endl;
      
      if ((vcurr->isiSet())&&(vcurr->hasValidStamp())){
        cdebug_log(112,0) << "case set" << endl;
        if      (vcurr->getIMin() > pnext.getY()) {
          min  = pnext.getY();
          max  = vcurr->getIMax();
          axis = vcurr->getIAxis();
        } else if (vcurr->getIMax() < pnext.getY()) {
          min  = vcurr->getIMin();
          max  = pnext.getY();
          axis = vcurr->getIAxis();
        } else {
          min  = vcurr->getIMin();
          max  = vcurr->getIMax();
          axis = vcurr->getIAxis();
        }
      } else {
        cdebug_log(112,0) << "case not set" << endl;
        axis = pcurr.getX();
        bool vv = false;
        if (vcurr->hasValidStamp() && (vcurr->getFrom() != NULL)){
          GCell*  gprev = vcurr->getGPrev(Vertex::UseFromFrom2);
          Vertex* vprev = gprev->getObserver<Vertex>(GCell::Observable::Vertex);
          if ((vprev->isV())) {
            cdebug_log(112,0) << "----------------------------" << endl;
            cdebug_log(112,0) << "VVCASE:" << endl;
            cdebug_log(112,0) << "prev: " << vprev << endl;
            cdebug_log(112,0) << "curr: " << vcurr << endl;
            cdebug_log(112,0) << "next: " << this << endl;
            cdebug_log(112,0) << "----------------------------" << endl;
            vv = true;
          }
        }
        if (vv){
          GCell*     gcurr      = vcurr->getGCell();
          GCell*     gnext      = getGCell();
          IntervalC intervfrom = vcurr->getIntervFrom(UseFromFrom2);
          if        (gnext->getYMin() > intervfrom.getMax()){
          //cdebug_log(112,0) << "1" << endl;
            min = intervfrom.getMax();
            max = gnext->getYMin();
          } else if (gnext->getYMax() < intervfrom.getMin()){
          //cdebug_log(112,0) << "2" << endl;
            min = gnext->getYMax();
            max = intervfrom.getMin();
          } else {
          //cdebug_log(112,0) << "3" << endl;
            min = std::max(gcurr->getYMin(), intervfrom.getMin());
            max = std::min(gcurr->getYMax(), intervfrom.getMax());
          }
        } else {
          if (pcurr.getY() < pnext.getY()){
          //cdebug_log(112,0) << "4" << endl;
            min = pcurr.getY();
            max = pnext.getY();
          } else {
          //cdebug_log(112,0) << "5" << endl;
            max = pcurr.getY();
            min = pnext.getY();
          }
        }
      }

    } else {
      cdebug_log(112,0) << "[ERROR](void Vertex::setIntervals(...)): Something is wrong." << endl;
      cdebug_tabw(112,-1);
      return;
    }
    cdebug_log(112,0) << "IntervFrom => min: " << DbU::getValueString(min) << ", max: " << DbU::getValueString(max) << ", axis:" << DbU::getValueString(axis) << endl; 
    
    if (isFrom2Mode()) {
      cdebug_log(112,0) << "SetIntervfrom2" << endl;
      setIntervfrom2(min, max, axis);
    }
    else {
      cdebug_log(112,0) << "SetIntervfrom" << endl;
      setIntervfrom(min, max, axis);
    }
    cdebug_tabw(112,-1);
  }

    
  bool Vertex::areSameSide ( const Vertex* v1, const Vertex* v2 ) const
  {
    if (  (isNorth(v1) and isNorth(v2))
       || (isSouth(v1) and isSouth(v2))
       || (isWest (v1) and isWest (v2))
       || (isEast (v1) and isEast (v2))
       ) return true;
    else return false;
  }


  void Vertex::createAData()       
  {
    if (_adata == NULL) _adata = GRAData::create();
  }


  bool Vertex::isiSet() const
  {
    if (_adata){
      return _adata->isiSet();
    } else {
      cdebug_log(112,0) << "bool Vertex::isiSet() const: Inappropriate usage of GRAData. " <<  endl;
      return false;
    }
  }


  DbU::Unit Vertex::getIAxis() const
  {
    if (_adata){
      return _adata->getIAxis();
    } else {
      cdebug_log(112,0) << "DbU::Unit Vertex::getIAxis() const: Inappropriate usage of GRAData. " <<  endl;
      return 0;
    }
  }


  DbU::Unit Vertex::getIMax() const
  {
    if (_adata){
      return _adata->getIMax();
    } else {
      cdebug_log(112,0) << "DbU::Unit Vertex::getIMax() const: Inappropriate usage of GRAData. " <<  endl;
      return 0;
    }
  }


  DbU::Unit Vertex::getIMin() const
  {
    if (_adata){
      return _adata->getIMin();
    } else {
      cdebug_log(112,0) << "DbU::Unit Vertex::getIMin() const: Inappropriate usage of GRAData. " <<  endl;
      return 0;
    }
  }


  DbU::Unit Vertex::getPIAxis() const
  {
    if (_adata){
      return _adata->getPIAxis();
    } else {
      cdebug_log(112,0) << "DbU::Unit Vertex::getPIAxis() const: Inappropriate usage of GRAData. " <<  endl;
      return 0;
    }
  }


  DbU::Unit Vertex::getPIMax() const 
  {
    if (_adata){
      return _adata->getPIMax();
    } else {
      cdebug_log(112,0) << "DbU::Unit Vertex::getPIMax() const: Inappropriate usage of GRAData. " <<  endl;
      return 0;
    }
  }


  DbU::Unit Vertex::getPIMin() const
  {
    if (_adata){
      return _adata->getPIMin();
    } else {
      cdebug_log(112,0) << "DbU::Unit Vertex::getPIMin() const: Inappropriate usage of GRAData. " <<  endl;
      return 0;
    }
  }


  void Vertex::setInterv( DbU::Unit min, DbU::Unit max, DbU::Unit axis )
  {
    if (_adata){
      _adata->setInterv(min, max, axis);
    } else {
      cdebug_log(112,0) << "void Vertex::setInterv( DbU::Unit min, DbU::Unit max, DbU::Unit axis ): Inappropriate usage of GRAData. " <<  endl;
    }
  }


  void Vertex::setIntervfrom( DbU::Unit min, DbU::Unit max, DbU::Unit axis )
  {
    if (_adata){
      _adata->setIntervfrom(min, max, axis);
    } else {
      cdebug_log(112,0) << "void Vertex::setIntervfrom( DbU::Unit min, DbU::Unit max, DbU::Unit axis ): Inappropriate usage of GRAData. " <<  endl;
    }
  }


  void Vertex::setIntervfrom2( DbU::Unit min, DbU::Unit max, DbU::Unit axis )
  {
    if (_adata){
      _adata->setIntervfrom2(min, max, axis);
    } else {
      cdebug_log(112,0) << "void Vertex::setIntervfrom2( DbU::Unit min, DbU::Unit max, DbU::Unit axis ): Inappropriate usage of GRAData. " <<  endl;
    }
  }


  void Vertex::resetIntervals() 
  {
    if (_adata){
      _adata->resetIntervals();
    } else {
      cdebug_log(112,0) << "void Vertex::resetIntervals(): Inappropriate usage of GRAData. " <<  endl;
    }
  }


  void Vertex::clearFrom2() 
  {
    if (_adata){
      _adata->clearFrom2();
    } else {
      cdebug_log(112,0) << "void Vertex::clearfrom2(): Inappropriate usage of GRAData. " <<  endl;
    }
  }


  Edge* Vertex::getFrom2() const
  {
    if (_adata){
      return _adata->getFrom2();
    } else {
      cdebug_log(112,0) << "Edge* Vertex::getFrom2() const: Inappropriate usage of GRAData. " <<  endl;
      return NULL;
    }
  }


  void Vertex::setFrom2( Edge* from )
  {
    if (_adata){
      _adata->setFrom2(from);
    } else {
      cdebug_log(112,0) << "void Vertex::setFrom2( Edge* from ): Inappropriate usage of GRAData. " <<  endl;
    }
  }


  void Vertex::createIntervFrom2()
  {
    if (_adata){
    //_adata->createIntervFrom2();
    } else {
      cdebug_log(112,0) << "void Vertex::createIntervFrom2(): Inappropriate usage of GRAData. " <<  endl;
    }
  }


  DbU::Unit Vertex::getPIMax2() const
  {
    if (_adata){
      return _adata->getPIMax2();
    } else {
      cdebug_log(112,0) << "DbU::Unit Vertex::getPIMax2() const: Inappropriate usage of GRAData. " <<  endl;
      return 0;
    }
  }


  DbU::Unit Vertex::getPIMin2() const
  {
    if (_adata){
      return _adata->getPIMin2();
    } else {
      cdebug_log(112,0) << "DbU::Unit Vertex::getPIMin2() const: Inappropriate usage of GRAData. " <<  endl;
      return 0;
    }
  }


  DbU::Unit Vertex::getPIAxis2() const
  {
    if (_adata){
      return _adata->getPIAxis2();
    } else {
      cdebug_log(112,0) << "DbU::Unit Vertex::getPIAxis2() const: Inappropriate usage of GRAData. " <<  endl;
      return 0;
    }
  }


  IntervalC Vertex::getIntervFrom2() const
  {
    if (_adata){
      return _adata->getIntervFrom2();
    } else {
      cdebug_log(112,0) << "DbU::Unit Vertex::getIntervFrom2() const: Inappropriate usage of GRAData. " <<  endl;
      return IntervalC();
    }
  }


  IntervalC Vertex::getIntervFrom( uint32_t criteria ) const
  {
    if (_adata){
      switch (criteria){
        case Vertex::From2Mode:
          if ((isFrom2Mode())&&(getFrom2() != NULL)){
            cdebug_log(112,0) << "getIntervFrom:From2Mode:UseFrom2. " <<  endl;
            return _adata->getIntervFrom2();
          } else {
            cdebug_log(112,0) << "getIntervFrom:From2Mode:UseFrom1. " <<  endl;
            return _adata->getIntervFrom();
          }
        case Vertex::UseFromFrom2:
          if ((isFromFrom2())&&(getFrom2() != NULL)){
            cdebug_log(112,0) << "getIntervFrom:UseFromFrom2:UseFrom2. " <<  endl;
            return _adata->getIntervFrom2();
          } else {
            cdebug_log(112,0) << "getIntervFrom:UseFromFrom2:UseFrom1. " <<  endl;
            return _adata->getIntervFrom();
          }
        case 0:
          cdebug_log(112,0) << "getIntervFrom:Default:UseFrom1. " <<  endl;
          return _adata->getIntervFrom();
        default:
          cdebug_log(112,0) << "getIntervFrom:Default:UseFrom1. " <<  endl;
          return _adata->getIntervFrom();
      }
    } else {
      cdebug_log(112,0) << "DbU::Unit Vertex::getIntervFrom(Flags criteria) const: Inappropriate usage of GRAData. " <<  endl;
      return IntervalC();
    }
  }


  GCell* Vertex::getGPrev( uint32_t criteria ) const
  {
    if (_adata){
      switch (criteria){
        case Vertex::From2Mode:
          if ((isFrom2Mode())&&(getFrom2() != NULL)){
            cdebug_log(112,0) << "getGPrev:From2Mode:UseFrom2. " <<  endl;
            return _adata->getFrom2()->getOpposite(getGCell());
          } else {
            cdebug_log(112,0) << "getGPrev:From2Mode:UseFrom1. " <<  endl;
            if (_from) return getFrom()->getOpposite(getGCell());
            else       return NULL;
          }
        case Vertex::UseFromFrom2:
          if ((isFromFrom2())&&(getFrom2() != NULL)){
            cdebug_log(112,0) << "getGPrev:UseFromFrom2:UseFrom2. " <<  endl;
            return _adata->getFrom2()->getOpposite(getGCell());
          } else {
            cdebug_log(112,0) << "getGPrev:UseFromFrom2:UseFrom1. " <<  endl;
            if (_from) return getFrom()->getOpposite(getGCell());
            else       return NULL;
          }
        case 0:
          cdebug_log(112,0) << "getGPrev:Default:UseFrom1. " <<  endl;
          if (_from) return getFrom()->getOpposite(getGCell());
          else       return NULL;
        default:
          cdebug_log(112,0) << "getGPrev:Default:UseFrom1. " <<  endl;
          if (_from) return getFrom()->getOpposite(getGCell());
          else       return NULL;
      }
    } else {
      if (_from) return getFrom()->getOpposite(getGCell());
      else       return NULL;
    }
  }


  IntervalC Vertex::getInterv() const
  {
    if (_adata){
      return _adata->getInterv();
    } else {
      cdebug_log(112,0) << "DbU::Unit Vertex::getInterv() const: Inappropriate usage of GRAData. " <<  endl;
      return IntervalC();
    }
  }


  void Vertex::printInterv() const
  {
    if (_adata){
      _adata->printInterv();
    } else {
      cdebug_log(112,0) << "void Vertex::printInterv() const: Inappropriate usage of GRAData. " <<  endl;
    }
  }


  void Vertex::printIntervfrom() const
  {
    if (_adata){
      _adata->printIntervfrom();
    } else {
      cdebug_log(112,0) << "void Vertex::printIntervfrom() const: Inappropriate usage of GRAData. " <<  endl;
    }
  }

////////////////////////////////////////////////////////////////////



  string  Vertex::_getString () const
  {
    if (not _gcell) {
      string s = "<Vertex [key] " + getString(_id) + ">";
      return s;
    }
    string s = "<Vertex " + getString(_id)
             + " @(" + DbU::getValueString(_gcell->getXMin())
             +  "-" + DbU::getValueString(_gcell->getYMin()) 
             +  "-" + DbU::getValueString(_gcell->getXMax())
             +  "-" + DbU::getValueString(_gcell->getYMax()) + ")"
           //+ " rps:" +  getString(_rpCount)
           //+ " deg:" +  getString(_degree)
             + " connexId:" + ((_connexId >= 0) ? getString(_connexId) : "None")
             + " d:" + ((_distance == unreached) ? "unreached"
                                                 : ((_distance == unreachable) ? "unreachable"
                                                                               : DbU::getValueString(_distance)) )
           //+   "+" + getString(_branchId)
           //+ " stamp:" + (hasValidStamp() ? "valid" : "outdated")
             + " from:" + ((_from) ? "set" : "NULL")
           //+ " from2:" + ((_adata) ? _adata->getFrom2() : "NULL")
             + " restricted:"
             + (isNRestricted() ? "N" : "-")
             + (isSRestricted() ? "S" : "-")
             + (isERestricted() ? "E" : "-")
             + (isWRestricted() ? "W" : "-")
           //+ " isiSet:" +(isiSet() ? "1" : "0")
             + ">";
    return s;
  }


  void  Vertex::notify ( Vertex* vertex, unsigned int flags )
  {
    cdebug_log(111,0) << "Vertex::notify() " << vertex << endl;
  // Take into account the GCell modification here.
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::Dijkstra".


  Dijkstra::Mode::~Mode ()
  { }


  string  Dijkstra::Mode::_getTypeName () const
  { return "Anabatic::Dijkstra::Mode"; }


  string Dijkstra::Mode::_getString () const
  {
    string s = "";
    s += (_flags & Standart ) ? 'S' : '-';
    s += (_flags & Monotonic) ? 'M' : '-';

    return s;
  }


  DbU::Unit  Dijkstra::_distance ( const Vertex* current, const Vertex* vneighbour, const Edge* e )
  {
    if (Vertex::isRestricted(current, vneighbour, e)) return Vertex::unreachable;
    else                                              return current->getDistance() + e->getDistance();
  }


  DbU::Unit calcDistance( Point p1, Point p2 )
  {
    return abs(p1.getX()-p2.getX()) + abs(p1.getY()-p2.getY());
  }


  Dijkstra::Dijkstra ( AnabaticEngine* anabatic )
    : _anabatic      (anabatic)
    , _vertexes      ()
    , _distanceCb    (_distance)
    , _mode          (Mode::Standart)
    , _net           (NULL)
    , _stamp         (-1)
    , _sources       ()
    , _targets       ()
    , _searchArea    ()
    , _searchAreaHalo(0)
    , _connectedsId  (-1)
    , _queue         ()
    , _flags         (0)
  {
    const vector<GCell*>& gcells = _anabatic->getGCells();
    for ( GCell* gcell : gcells ) {
      _vertexes.push_back( new Vertex (gcell) );
    }
    _anabatic->getMatrix()->show();
  }


  Dijkstra::~Dijkstra ()
  {
    for ( Vertex* vertex : _vertexes ) delete vertex;
  }


  Point Dijkstra::_getPonderedPoint() const
  {
    vector<RoutingPad*> rps;
    int       cpt = 0;
    DbU::Unit x   = 0;
    DbU::Unit y   = 0;
    
    for ( Component* component : _net->getComponents() ) {
      RoutingPad* rp = dynamic_cast<RoutingPad*>( component );
      if (rp) rps.push_back( rp ); 
    }

    for ( auto rp : rps ) {
      x += rp->getBoundingBox().getCenter().getX();
      y += rp->getBoundingBox().getCenter().getY();
      cpt++;
    }
    return Point(x/cpt, y/cpt);
  }


  void  Dijkstra::load ( Net* net )
  {
    _cleanup();

    _net   = net;
    _stamp = _anabatic->incStamp();

    DebugSession::open( _net, 112, 120 );
    cdebug_log(112,1) << "Dijkstra::load() " << _net << endl;

    vector<RoutingPad*> rps;
    NetRoutingState* state = NetRoutingExtension::get( _net );

    
    if (state){
      if (state->isSelfSym()){
        cdebug_log(112,0) << "Dijkstra::SELF SYMMETRY CASE " << _net << endl;
      }
    }

    for ( Component* component : _net->getComponents() ) {
      RoutingPad* rp = dynamic_cast<RoutingPad*>( component );
      if (rp) { 
        if (_attachSymContactsHook(rp)) continue; // ANALOG

        cdebug_log(112,0) << "| " << rp << endl;
        rps.push_back( rp ); 
        continue; 
      }
    }

    for ( auto rp : rps ) {
      Point  center = rp->getBoundingBox().getCenter();
      GCell* gcell  = _anabatic->getGCellUnder( center );
     
      _limitSymSearchArea(rp); // ANALOG

      cdebug_log(112,0) << "| " << rp << endl;
        
      if (not gcell) {
        cerr << Error( "Dijkstra::load(): %s\n"
                       "        @%s of %s is not under any GCell.\n"
                       "        It will be ignored so the routing may be incomplete."
                     , getString(rp).c_str()
                     , getString(center).c_str()
                     , getString(_net).c_str()
                     ) << endl;
        continue;
      }

      cdebug_log(112,0) << "Merge search area: " << _searchArea << ", gcell: " << gcell << endl;
      _searchArea.merge( gcell->getBoundingBox() );
      cdebug_log(112,0) << "Search area: " << _searchArea << endl;

      Vertex*     seed  = gcell->getObserver<Vertex>(GCell::Observable::Vertex);
      GCell*      gseed = seed->getGCell();

      if (!gseed->isMatrix()) _setSourcesGRAData( seed, rp ); // ANALOG
      
      if (seed->getConnexId() < 0) {
        VertexSet  connecteds;
        _getConnecteds( seed, connecteds );

        ++_connectedsId;
        for ( Vertex* vertex : connecteds ) {
          vertex->setDistance     ( Vertex::unreached );
          vertex->setStamp        ( _stamp );
          vertex->setConnexId     ( _connectedsId );
          vertex->setBranchId     ( 0 );
          vertex->setDegree       ( 1 );
          vertex->setRpCount      ( 0 );
          vertex->setFrom         ( NULL );
          vertex->clearRestriction();
          _targets.insert( vertex );
          cdebug_log(112,0) << "| Add: " << vertex << endl;
        }
      }

      seed->incRpCount();
      Contact* vcontact = seed->getGContact( _net );
      rp->getBodyHook()->detach();
      rp->getBodyHook()->attach( vcontact->getBodyHook() );
    }

    _searchArea.inflate( _searchAreaHalo );
    cdebug_log(112,0) << "Search halo: " << _searchAreaHalo << endl;
    cdebug_log(112,0) << "Search area: " << _searchArea << endl;
    
    setAxisTargets();

    cdebug_tabw(112,-1);
    DebugSession::close();
  }


  void Dijkstra::unsetAxisTargets ()
  {
    NetRoutingState* state = NetRoutingExtension::get( _net );

    if (state){
      if (state->isSelfSym()){
        Cell* cell = _anabatic->getCell();
        _queue.clear();
        GCell* gcell = NULL;
        if (state->isSymVertical()){
          gcell  = _anabatic->getGCellUnder( Point( state->getSymAxis()
                                                  , _anabatic->getCell()->getAbutmentBox().getYMin()
                                                  ) );
        } else if (state->isSymHorizontal()){
          gcell  = _anabatic->getGCellUnder( Point( _anabatic->getCell()->getAbutmentBox().getXMin()
                                                  , state->getSymAxis()
                                                  ) );
        }
        if (gcell) {
          _queue.push(gcell->getObserver<Vertex>(GCell::Observable::Vertex));
        }
        while ( not _queue.empty() ) {
          Vertex* current  = _queue.top();
          _queue.pop();
          if ( (state->isSymVertical()   && (!current->isNRestricted()) && (!current->isSRestricted()))
             ||(state->isSymHorizontal() && (!current->isERestricted()) && (!current->isWRestricted()))
             ){
            current->unsetFlags(Vertex::AxisTarget);
          }

          if (state->isSymVertical()){
          // check North
            for ( Edge* edge : current->getGCell()->getNorthEdges() ) {
              GCell*  gnext = edge->getOpposite(current->getGCell());
              Vertex* vnext = gnext->getObserver<Vertex>(GCell::Observable::Vertex);
              if (  (gnext->getXCenter() == state->getSymAxis()) 
                 && (gnext->getYMin() <= cell->getAbutmentBox().getYMax())
                 ) _queue.push( vnext );
            }
          } else if (state->isSymHorizontal()){
          // check East
            for ( Edge* edge : current->getGCell()->getNorthEdges() ) {
              GCell*  gnext = edge->getOpposite(current->getGCell());
              Vertex* vnext = gnext->getObserver<Vertex>(GCell::Observable::Vertex);
               if (  (gnext->getXCenter() == state->getSymAxis())
                 && (gnext->getXMin() <= cell->getAbutmentBox().getXMax())
                 ) _queue.push( vnext );
            }
          }   
        }
      }
    }
  }


  void Dijkstra::setAxisTargets ()
  {
    NetRoutingState* state = NetRoutingExtension::get( _net );

    if (state){
      if (state->isSelfSym()){
        cdebug_log(112,0) << "void Dijkstra::setAxisTargets (): " << endl;
        
        Cell* cell = _anabatic->getCell();
        _queue.clear();
        GCell* gcell = NULL;
        if (state->isSymVertical()){
          gcell  = _anabatic->getGCellUnder( Point( state->getSymAxis()
                                                  , _anabatic->getCell()->getAbutmentBox().getYMin()
                                                  ) );
        } else if (state->isSymHorizontal()){
          gcell  = _anabatic->getGCellUnder( Point( _anabatic->getCell()->getAbutmentBox().getXMin()
                                                  , state->getSymAxis()
                                                  ) );
        }
        if (gcell) {
          _queue.push(gcell->getObserver<Vertex>(GCell::Observable::Vertex));
          setFlags(Mode::AxisTarget);
          cdebug_log(112,0) << "Find axis targets: " << endl;
        }
        while ( not _queue.empty() ) {
          Vertex* current  = _queue.top();
          _queue.pop();
          if ( (state->isSymVertical()   && (!current->isNRestricted()) && (!current->isSRestricted()))
             ||(state->isSymHorizontal() && (!current->isERestricted()) && (!current->isWRestricted()))
             ){
            current->setDistance ( Vertex::unreached );
            current->setStamp    ( _stamp );
            current->setConnexId( -1 );
            current->setFlags(Vertex::AxisTarget);
          }

          if (state->isSymVertical()){
          // check North
            for ( Edge* edge : current->getGCell()->getNorthEdges() ) {
              GCell*  gnext = edge->getOpposite(current->getGCell());
              Vertex* vnext = gnext->getObserver<Vertex>(GCell::Observable::Vertex);
              if (  (gnext->getXCenter() == state->getSymAxis()) 
                 && (gnext->getYMin() <= cell->getAbutmentBox().getYMax())
                 ) _queue.push( vnext );
            }
          } else if (state->isSymHorizontal()){
          // check East
            for ( Edge* edge : current->getGCell()->getNorthEdges() ) {
              GCell*  gnext = edge->getOpposite(current->getGCell());
              Vertex* vnext = gnext->getObserver<Vertex>(GCell::Observable::Vertex);
               if (  (gnext->getXCenter() == state->getSymAxis())
                 && (gnext->getXMin() <= cell->getAbutmentBox().getXMax())
                 ) _queue.push( vnext );
            }
          }          
          
        }
        
      }
    }
  }


  void  Dijkstra::_selectFirstSource ()
  {
    if (_targets.empty()) {
#if 0
      cparanoid << Error( "Dijkstra::_selectFirstSource(): %s has no vertexes to route, ignored."
                        , getString(_net).c_str()
                        ) << endl;
#endif
      return;
    }

    Vertex* firstSource = NULL;

    if (_mode & Mode::Monotonic) {
      if (_targets.size() == 2) {
        auto ivertex = _targets.begin();
        Vertex* v1 = *ivertex;
        Vertex* v2 = *(++ivertex);

        firstSource = (v1->getCenter().getX() <= v2->getCenter().getY()) ? v1 : v2;
      } else {
        cerr << Error( "Dijkstra::_selectFirstSource(): %s cannot be routed in monotonic mode.\n"
                       "        Must have exactly two terminals (%u), revert to Standart."
                     , getString(_net).c_str()
                     , _targets.size()
                     ) << endl;
        _mode = Mode::Standart;
      }
    }

    if (not firstSource) {
    // Standart routing.
      bool hasDevice = false;
      for (  Vertex* ivertex : _targets ) {
        if (ivertex->getGCell()->isDevice()) hasDevice = true;
      }

      Point areaCenter;
      if (hasDevice) areaCenter = _getPonderedPoint();
      else           areaCenter = _searchArea.getCenter();

      auto ivertex     = _targets.begin();

      firstSource = *ivertex++;
      DbU::Unit  minDistance = areaCenter.manhattanDistance( firstSource->getCenter() );

      for ( ; ivertex != _targets.end() ; ++ivertex ) {
        DbU::Unit distance = areaCenter.manhattanDistance( (*ivertex)->getCenter() );
        if (distance < minDistance) {
          minDistance = distance;
          firstSource = *ivertex;
        }
      }
    }

    for ( auto ivertex = _targets.begin() ; ivertex != _targets.end() ; ) {
      auto inext = ivertex; inext++;

      if ((*ivertex)->getConnexId() == firstSource->getConnexId()) {
        _sources.insert( *ivertex );
        _targets.erase (  ivertex );
      }
 
      ivertex = inext;
    }

    cdebug_log(112,0) << "Dijkstra::_selectFirstSource() " << *_sources.begin() << endl;
  }


  void  Dijkstra::_cleanup ()
  {
  //_checkEdges();
    _sources.clear();
    _targets.clear();
    _searchArea.makeEmpty();
    _connectedsId = 0;
  }


  bool  Dijkstra::_propagate ( Flags enabledSides )
  {
    cdebug_log(112,1) << "Dijkstra::_propagate() " << _net <<  endl;
    while ( not _queue.empty() ) {
      cdebug_log(111,0) << "Number of targets left: " << _targets.size()
                        << " and needaxis? " << needAxisTarget() << endl;

      _queue.dump();
      Vertex* current  = _queue.top();
      GCell*  gcurrent = current->getGCell();

    //cdebug_log(111,1) << "Current:" << current << endl;
      _queue.pop();

      if      ( current->isAxisTarget() and needAxisTarget()) unsetFlags(Mode::AxisTarget);
      else if ((current->getConnexId() == _connectedsId) or (current->getConnexId() < 0)) {
        cdebug_log(111,1) << "Looking for neighbors:" << endl;

        for ( Edge* edge : current->getGCell()->getEdges() ) {
          cdebug_log(111,0) << "[Current]: " << current << endl;
          cdebug_log(111,0) << "@ Edge " << edge << endl;

          if (edge == current->getFrom()) {
            cdebug_log(111,0) << "| Reject: edge == current->getFrom()" << endl;
            continue;
          }
          
          if (_checkFrom2(edge, current)) { // ANALOG
            cdebug_log(111,0) << "| Reject: _checkFrom2()" << endl;
            continue;
          }

          GCell*  gneighbor = edge->getOpposite(current->getGCell());
          Vertex* vneighbor = gneighbor->getObserver<Vertex>( GCell::Observable::Vertex );
          if (not gneighbor->isMatrix()) vneighbor->createAData();

          cdebug_log(111,0) << "+ Neighbor:" << vneighbor << endl;

          if (vneighbor->getConnexId() == _connectedsId) {
            cdebug_log(111,0) << "| Reject: Neighbor already reached (has connectedsId)" << endl;
            continue;
          }
          if (not _searchArea.intersect(gneighbor->getBoundingBox())) {
            cdebug_log(111,0) << "| Reject: not in _searchArea: " << _searchArea << ", gneighbor area: "  << gneighbor->getBoundingBox() << endl;
            continue;
          }

        ////////////////////////////////////// DEBUG ////////////////////////////////////// 
          cdebug_tabw(111,1);
          if (current->getFrom()) { 
            cdebug_log(111,0) << "| From: " << current->getFrom()->getOpposite(gcurrent) << endl;
          //current->getIntervFrom().print();
          } 
          if (current->getFrom2()) { 
            cdebug_log(111,0) << "| From2: " << current->getFrom2()->getOpposite(gcurrent) << endl;
            current->getIntervFrom2().print();
          }
          if ( (vneighbor->getFrom() != NULL) and (vneighbor->hasValidStamp()) ) {
            cdebug_log(111,0) << "| Neighbor GETFROM:" << vneighbor->getFrom()->getOpposite( gneighbor ) << endl;
            cdebug_log(111,0) << "Distance prev : " << DbU::getValueString(vneighbor->getDistance()) << endl;
          }
        /////////////////////////////////////////////////////////////////////////////////// 

          DbU::Unit distance = _distanceCb( current, vneighbor, edge );

          bool isDistance2shorter = _isDistance2Shorter ( distance, current, vneighbor, edge ); // ANALOG

          if  (   (distance == vneighbor->getDistance())
              and (not gcurrent->isMatrix() ) 
              and (not gneighbor->isMatrix()) 
              and (vneighbor->getFrom2() == NULL)
              )  {
            _pushEqualDistance( distance, isDistance2shorter, current, vneighbor, edge ); // ANALOG
          } else
            if ( (distance < vneighbor->getDistance()) and (distance != Vertex::unreachable) ) {
              if (vneighbor->getDistance() != Vertex::unreached) _queue.erase( vneighbor );
              else {
                if (not vneighbor->hasValidStamp()) {
                  cdebug_log(111,0) << "Vertex reached for the first time" << endl;
                  vneighbor->setConnexId( -1 );
                  vneighbor->setStamp   ( _stamp );
                  vneighbor->setDegree  ( 1 );
                  vneighbor->setRpCount ( 0 );
                  vneighbor->unsetFlags(Vertex::AxisTarget);
                }
              }
              cdebug_log(111,0) << "Vertex reached through a shorter path" << endl;
              _updateGRAData( vneighbor, isDistance2shorter, current ); // ANALOG
              
              vneighbor->setBranchId( current->getBranchId() );
              vneighbor->setDistance( distance );
              cdebug_log(111,0) << "setFrom1: " << vneighbor << endl; 
              vneighbor->setFrom ( edge );
              vneighbor->setFrom2( NULL );
              _queue.push( vneighbor );
              cdebug_log(111,0) << "Push: (size:" << _queue.size() << ") " << vneighbor << endl;
            } else {
              cdebug_log(111,0) << "Reject: Vertex reached through a *longer* path" << endl;
            }

          cdebug_tabw(111,-1);
        }

        cdebug_tabw(111,-2);
        continue;
      }

      cdebug_tabw(111,-1);

    // We did reach another target (different <connexId>).
    // Tag back the path, with a higher <branchId>.
      _traceback( current );

      cdebug_tabw(112,-1);
      return true;
    }

    cerr << Error( "Dijkstra::propagate(): %s has unreachable targets."
                 , getString(_net).c_str()
                 ) << endl;
    
    cdebug_log(112, 0) << "Unreached targets:" << endl;
    for ( Vertex* v : _targets )
      cdebug_log(112, 0) << "| " << v << endl;

    cdebug_tabw(112,-1);
    return false;
  }


  void  Dijkstra::_traceback ( Vertex* current )
  {
    cdebug_log(112,1) << "Dijkstra::_traceback() " << _net << " branchId:" << _sources.size() << endl;

    int branchId = _sources.size();
    _toSources( current, _connectedsId );

    bool isfirst  = true;
    bool useFrom2 = false;
    if (!current->getGCell()->isMatrix()){
      _initiateUpdateIntervals ( current ); // ANALOG
    } else {
      current = current->getPredecessor();
      isfirst = false;
      
    }
    cdebug_log(112,0) << "[Start WHILE]" << endl;

    Edge* from = NULL;
    while ( current ) {
      cdebug_log(112,0) << endl;
      cdebug_log(112,0) << "| " << current << " | " << endl;
      if (current->getFrom()) cdebug_log(112,0) << "  | From :" << current->getFrom()->getOpposite(current->getGCell()) << " | " << endl;
      if (current->getFrom2()) cdebug_log(112,0) << "  | From2:" << current->getFrom2()->getOpposite(current->getGCell()) << " | " << endl;

      if (!current->getGCell()->isMatrix()){
      //////////////////////////////////////////////////////////////////////////////////////////// ANALOG
        if (_updateIntervals ( isfirst, current, useFrom2, branchId, from )) break;
        Vertex* next = NULL;
        next = current->getPredecessor();

        if( current->isFromFrom2()) {
          cdebug_log(112,0) << "ISFROMFROM2: " << current << endl;
          useFrom2 = true;
          current->unsetFlags(Vertex::UseFromFrom2);
        } else {
          cdebug_log(112,0) << "ISNOT FROMFROM2" << endl;
          useFrom2 = false;
        }
        cdebug_log(112,0) << "next: " << next << endl;
        current = next;
      //////////////////////////////////////////////////////////////////////////////////////////// 
      } else {
        current->incDegree();
        if (current->getConnexId() == _connectedsId) break;

        from  = current->getFrom();
        if (not from) break;

        current->setDistance( 0.0 );
        current->setConnexId( _connectedsId );
        current->setBranchId( branchId );
        _sources.insert( current );
        _queue.push( current );
        current = current->getPredecessor();
      }
    }
    cdebug_tabw(112,-1);
  }


  void  Dijkstra::_materialize ()
  {
    cdebug_log(112,1) << "Dijkstra::_materialize() " << _net << " _sources:" << _sources.size() << endl;

    if (_sources.size() < 2) { cdebug_tabw(112,-1); return; }

    NetRoutingState* state = NetRoutingExtension::get( _net );
    
    for ( Vertex* startVertex : _sources ) {

      if (not startVertex->getFrom()) continue;
      if (   not startVertex->hasGContact(_net)
         and not startVertex->getRpCount()
         and (startVertex->getDegree() < 3)
         and not startVertex->isAxisTarget() ) continue;

      Vertex* source = startVertex;
      while ( source ) {
        source->resetIntervals(); // ANALOG

        cdebug_log(112,0) << "* " << source << endl;

        Edge* from = source->getFrom();
        vector<Edge*> aligneds;
        aligneds.push_back( from );

        Vertex*  target       = source->getPredecessor();
        Interval constraint   = from->getSide();
        source->setFrom( NULL );

        cdebug_log(112,0) << "+ " << target << endl;

        if (target->getConnexId() < 0) {
          cdebug_log(112,0) << "| " << "break (abort: false start)." << endl;
          break;
        }

        while ( true ) {
          from = target->getFrom();
          if (  not from
             or not (target->getGCell()->isMatrix())
             or (target->hasGContact(_net))
             or (target->getRpCount())
             or (target->getDegree() > 2)
             or (aligneds.back()->isHorizontal() xor from->isHorizontal())
             or not constraint.intersect(from->getSide())) break;

          aligneds.push_back( from );
          constraint.merge( from->getSide() );

          Vertex* nextTarget = target->getPredecessor();
          target->setFrom( NULL );
          target = nextTarget;

          cdebug_log(112,0) << "| " << target << endl;
        }

        Contact* sourceContact = source->getGContact( _net );
        Contact* targetContact = target->hasGContact( _net );
        Segment* segment       = NULL;


        if (not targetContact) {
          if (target->getFrom()) targetContact = target->getGContact( _net );
          else                   targetContact = target->breakGoThrough( _net );
        }
        
        if (aligneds.front()->isHorizontal()) {
          if (sourceContact->getX() > targetContact->getX())
            std::swap( sourceContact, targetContact );

          DbU::Unit width = Session::getPitch(Hurricane::DataBase::getDB()->getTechnology()->getLayer("METAL2"));  //DbU::fromLambda(2.0);
          if (state) width *= state->getWPitch();

          segment = Horizontal::create( sourceContact
                                      , targetContact
                                      , _anabatic->getConfiguration()->getGHorizontalLayer()
                                      , constraint.getCenter()
                                      , width
                                      );
          for ( Edge* through : aligneds ) through->add( segment );
          if (state){
            if (state->isSymmetric()) _createSelfSymSeg ( segment );
          }
        } else {
          if (sourceContact->getY() > targetContact->getY())
            std::swap( sourceContact, targetContact );

          DbU::Unit width = Session::getPitch(Hurricane::DataBase::getDB()->getTechnology()->getLayer("METAL3"));  //DbU::fromLambda(2.0);
          if (state) width *= state->getWPitch();

          segment = Vertical::create( sourceContact
                                    , targetContact
                                    , _anabatic->getConfiguration()->getGVerticalLayer()
                                    , constraint.getCenter()
                                    , width
                                    );
          for ( Edge* through : aligneds ) through->add( segment );
          if (state){
            if (state->isSymmetric()) _createSelfSymSeg ( segment );
          }
        }

        cdebug_log(112,0) << "|| " << segment << endl;
      //cdebug_log(112,0) << "| " << "break (turn, branch or terminal)." << endl;
        Vertex* stc = source;
        source = (target->getFrom()) ? target : NULL;
        stc->clearFrom2();
      }
    }

    cdebug_tabw(112,-1);
  }


  void  Dijkstra::run ( Dijkstra::Mode mode )
  {
    DebugSession::open( _net, 111, 120 );

    cdebug_log(112,1) << "Dijkstra::run() on " << _net << " mode:" << mode << endl;
    _mode = mode;

    _selectFirstSource();
    if (_sources.empty()) {
      cdebug_log(112,0) << "No source to start, not routed." << endl;
      cdebug_tabw(112,-1);
      return;
    }

    Flags enabledEdges = Flags::AllSides;
    if (_mode & Mode::Monotonic) {
      if ((*_sources.begin())->getCenter().getY() <= (*_targets.begin())->getCenter().getY())
        enabledEdges = Flags::EastSide | Flags::NorthSide;
      else
        enabledEdges = Flags::EastSide | Flags::SouthSide;
    }

    _queue.clear();
    _connectedsId = (*_sources.begin())->getConnexId();
    for ( Vertex* source : _sources ) {
      _queue.push( source );
      source->setDistance( 0.0 );
      cdebug_log(112,0) << "Push source: (size:" << _queue.size() << ") "
                        << source
                        << " _connectedsId:" << _connectedsId << endl;
    }
    while ( ((not _targets.empty()) ||  needAxisTarget()) and _propagate(enabledEdges) );

    _queue.clear();
    _materialize();
    unsetAxisTargets();

    _anabatic->getNetData( _net )->setGlobalRouted( true );

    cdebug_tabw(112,-1);
    DebugSession::close();
  }


  void  Dijkstra::_toSources ( Vertex* source, int connexId )
  {
    cdebug_log(112,1) << "Dijkstra::_toSources() " << endl;
  //cdebug_log(112,0) << "* from:  " << source << endl;

    source->setConnexId( connexId );
    source->setDistance( 0.0 );
    _targets.erase ( source );
    _sources.insert( source );
    _queue.push( source );

    VertexSet stack;
    stack.insert( source );

    while ( not stack.empty() ) {
      source = *stack.begin();
      stack.erase( source );

    //cdebug_log(112,0) << "| source:" << source << " stack.size():" << stack.size() << endl;

      for ( Edge* edge : source->getGCell()->getEdges() ) {
        if (not edge->hasNet(_net)) {
        //cdebug_log(112,0) << "  Not connected:" << edge
        //                  << " to:" << edge->getOpposite(source->getGCell()) << endl; 
          continue;
        }

        GCell*  gneighbor = edge->getOpposite(source->getGCell());
        Vertex* vneighbor = gneighbor->getObserver<Vertex>(GCell::Observable::Vertex);

        if (not vneighbor->hasValidStamp()) continue; 
        if (vneighbor->getConnexId() == connexId) continue;

        vneighbor->setConnexId( connexId );
        vneighbor->setDistance( 0.0 );
      //vneighbor->setFrom    ( edge );
        _targets.erase ( vneighbor );
        _sources.insert( vneighbor );
        _queue.push( vneighbor );
        stack.insert( vneighbor );
      }
    }

    cdebug_tabw(112,-1);
  }


  void  Dijkstra::_getConnecteds ( Vertex* source, VertexSet& connecteds )
  {
    cdebug_log(112,1) << "Dijkstra::_getConnecteds()" << endl;

    connecteds.clear();
    connecteds.insert( source );

    VertexSet stack;
    stack.insert( source );

    while ( not stack.empty() ) {
      source = *stack.begin();
      stack.erase( source );

      cdebug_log(112,0) << "| source:" << source << " stack.size():" << stack.size() << endl;

      for ( Edge* edge : source->getGCell()->getEdges() ) {
        if (not edge->hasNet(_net)) {
          cdebug_log(112,0) << "  Not connected:" << edge << endl; 
          continue;
        }

        GCell*  gneighbor = edge->getOpposite(source->getGCell());
        Vertex* vneighbor = gneighbor->getObserver<Vertex>(GCell::Observable::Vertex);

        if (connecteds.find(vneighbor) != connecteds.end()) continue;

        stack.insert( vneighbor );
        connecteds.insert( vneighbor );
      }
    }

    cdebug_tabw(112,-1);
  }


  void  Dijkstra::_checkEdges () const
  {
    cdebug_log(112,1) << "Dijkstra::_checkEdges()" << endl;

    // for ( Vertex* vertex : _vertexes ) {
    //   for ( Edge* edge : vertex->getGCell()->getEdges(Flags::EastSide|Flags::NorthSide) ) {
    //   }
    // }

    cdebug_tabw(112,-1);
  }


  void  Dijkstra::_createSelfSymSeg ( Segment* segment )
  {
    NetRoutingState* state = NetRoutingExtension::get( _net );
    if ((state != NULL)&&(segment!=NULL)){
      Horizontal* h = dynamic_cast<Horizontal*>(segment);
      Vertical*   v = dynamic_cast<Vertical*>(segment);
      Point sp, tp;
      DbU::Unit axis;
      Component* sourceContact = segment->getSource();
      Component* targetContact = segment->getTarget();
      if (h){
        if (state->isSymHorizontal()){
          sp   = Point(sourceContact->getX(), state->getSymValue(sourceContact->getY()) );
          tp   = Point(targetContact->getX(), state->getSymValue(targetContact->getY()) );
          axis = state->getSymValue(segment->getY()); 
        } else if (state->isSymVertical()){
          sp = Point( state->getSymValue(targetContact->getX()), targetContact->getY() );
          tp = Point( state->getSymValue(sourceContact->getX()), sourceContact->getY() );
          axis = segment->getY(); 
        } else {
          cdebug_log(112,0) << "Dijkstra::_materialize(): Something is wrong here. " << endl;
          return;
        }
        GCell*  sgcell  = _anabatic->getGCellUnder( sp );
        GCell*  tgcell  = _anabatic->getGCellUnder( tp );
        Vertex* svertex = sgcell->getObserver<Vertex>(GCell::Observable::Vertex);
        Vertex* tvertex = tgcell->getObserver<Vertex>(GCell::Observable::Vertex);
        Contact* sourceSym = NULL;
        Contact* targetSym = NULL;
        if (state->isSelfSym()){
          cdebug_log(112,0) << "isSelfSym" << endl;
          sourceSym = svertex->getGContact( _net );
          targetSym = tvertex->getGContact( _net );
        } else if (state->isSymMaster()){
          cdebug_log(112,0) << "isSymPair: " << state->getSymNet() << endl;
          sourceSym = svertex->getGContact( state->getSymNet() );
          targetSym = tvertex->getGContact( state->getSymNet() );
        } else {
          cdebug_log(112,0) << "Dijkstra::_materialize(): Something is wrong with the symmetry. " << endl;
          return;
        }
              
        cdebug_log(112,0) << "sourceSym:" << sourceSym << endl;
        cdebug_log(112,0) << "targetSym:" << targetSym << endl;
        Segment* segment2 = Horizontal::create( sourceSym
                                              , targetSym
                                              , _anabatic->getConfiguration()->getGHorizontalLayer()
                                              , axis
                                              , state->getWPitch()*Session::getPitch(Hurricane::DataBase::getDB()->getTechnology()->getLayer("METAL2"))
                                              );
        cdebug_log(112,0) << "|| " << segment2 << endl;
      } else if (v) {
        if (state->isSymVertical()){
          sp = Point(state->getSymValue(sourceContact->getX()), sourceContact->getY() );
          tp = Point(state->getSymValue(targetContact->getX()), targetContact->getY() );
          axis = state->getSymValue(segment->getX()); 
        } else if (state->isSymHorizontal()){
          sp = Point( targetContact->getX(), state->getSymValue(targetContact->getY()) );
          tp = Point( sourceContact->getX(), state->getSymValue(sourceContact->getY()) );
          axis = segment->getX(); 
        } else {
          cdebug_log(112,0) << "Dijkstra::_materialize(): Something is wrong here. " << endl;
          return;
        }
        GCell*  sgcell  = _anabatic->getGCellUnder( sp );
        GCell*  tgcell  = _anabatic->getGCellUnder( tp );
        Vertex* svertex = sgcell->getObserver<Vertex>(GCell::Observable::Vertex);
        Vertex* tvertex = tgcell->getObserver<Vertex>(GCell::Observable::Vertex);
        Contact* sourceSym = NULL;
        Contact* targetSym = NULL;
        if (state->isSelfSym()){
          sourceSym = svertex->getGContact( _net );
          targetSym = tvertex->getGContact( _net );
        } else if (state->isSymMaster()){
          sourceSym = svertex->getGContact( state->getSymNet() );
          targetSym = tvertex->getGContact( state->getSymNet() );
        } else {
          cdebug_log(112,0) << "Dijkstra::_materialize(): Something is wrong with the symmetry. " << endl;
          return;
        }
              
        cdebug_log(112,0) << "sourceSym:" << sourceSym << endl;
        cdebug_log(112,0) << "targetSym:" << targetSym << endl;
        Segment* segment2 = Vertical::create( sourceSym
                                            , targetSym
                                            , _anabatic->getConfiguration()->getGVerticalLayer()
                                            , axis
                                            , state->getWPitch()*Session::getPitch(Hurricane::DataBase::getDB()->getTechnology()->getLayer("METAL3"))
                                            );
        cdebug_log(112,0) << "|| " << segment2 << endl;
      }
    }
  }


  bool Dijkstra::_checkFrom2 ( Edge* edge, Vertex* current )
  {
    if (current->getFrom2()){
      if (edge == current->getFrom2()) {
      //cdebug_log(111,0) << "edge == current->getFrom2()" << endl;
        return true;
      } else {
      //cdebug_log(111,0) << "edge != current->getFrom2(): " << current->getFrom2() << endl;
        return false;
      }
    } else {
    //cdebug_log(111,0) << "current->getFrom2() = NULL" << endl;
      return false;
    }
  }


  bool Dijkstra::_isDistance2Shorter ( DbU::Unit& distance, Vertex* current, Vertex* vneighbor, Edge* edge )
  {
    cdebug_log(111,1) << "Dijkstra::_isDistance2Shorter()" << endl;

    DbU::Unit distance2          = Vertex::unreachable;
    bool      isDistance2shorter = false;
    GCell*    gneighbor          = edge->getOpposite(current->getGCell());

    if (current->getFrom2()) {
      cdebug_log(111,0) << "Has second ::getFrom()" << edge << endl;

      current->setFlags(Vertex::From2Mode);
      distance2 = _distanceCb( current, vneighbor, edge );
      current->unsetFlags(Vertex::From2Mode);

      cdebug_log(111,0) << "Distance 1 from current: " << DbU::getValueString(distance) << endl;
      cdebug_log(111,0) << "Distance 2 from current: " << DbU::getValueString(distance2) << endl;

      if (distance > distance2) {
        cdebug_log(111,0) << "* Distance 2 is shorter" << endl;

        isDistance2shorter = true;
        distance = distance2;
      } else if (distance == distance2) {
        cdebug_log(111,0) << "* Distance 1 equal Distance 2" << endl;

        Point pcurr  = current->getStartPathPoint(vneighbor);
        current->setFlags( Vertex::From2Mode );
        Point pcurr2 = current->getStartPathPoint(vneighbor);
        current->unsetFlags( Vertex::From2Mode );
        Point pnext  = gneighbor->getCenter(); 

        if (calcDistance(pcurr,pnext) > calcDistance(pcurr2,pnext)) {
          cdebug_log(111,0) << "* Distance 2 is shorter" << endl;

          isDistance2shorter = true;
          distance = distance2;
        } else {
          cdebug_log(111,0) << "* Distance 1 is shorter" << endl;
        }
      } else {
        cdebug_log(111,0) << "* Distance 1 is shorter" << endl;
      }
    } else {
      cdebug_log(111,0) << "No second ::getFrom()" << endl;
      cdebug_log(111,0) << "Distance 1 from current: " << DbU::getValueString(distance) << endl;
    }

    cdebug_tabw(111,-1);
    return isDistance2shorter;
  }


  void Dijkstra::_pushEqualDistance ( DbU::Unit distance, bool isDistance2shorter, Vertex* current, Vertex* vneighbor, Edge* edge )
  {
    GCell*  gneighbor = edge->getOpposite(current->getGCell());
    GCell*  gnext     = vneighbor->getGCell();
    GCell*  gprev     = vneighbor->getFrom()->getOpposite(gnext);
    Vertex* vprev     = gprev->getObserver<Vertex>(GCell::Observable::Vertex);

    if ((distance == vneighbor->getDistance()) and vneighbor->areSameSide(vprev, current)){
      cdebug_log(111,0) << "[case: Distance EQUAL + SameSide]" << endl;
      cdebug_log(111,0) << "Previous getfrom:" << vneighbor->getFrom()->getOpposite( gneighbor ) << endl;
      cdebug_log(111,0) << "[case: Other GetFROM]" << endl; 
      cdebug_log(111,0) << "setFrom2: " << vneighbor << endl; 
      vneighbor->setFrom2    ( edge );
      vneighbor->setFlags(Vertex::From2Mode);
    //vneighbor->createIntervFrom2();
      vneighbor->setIntervals( current );
      vneighbor->unsetFlags(Vertex::From2Mode);
      if (isDistance2shorter) {
        vneighbor->setFlags(Vertex::UseFromFrom2);
      //cdebug_log(111,0) << "setFromFrom2: " << vneighbor << endl; 
      }
      cdebug_log(111,0) << "Push BIS : " << vneighbor << endl;
      cdebug_log(111,0) << "From1: " << vneighbor->getFrom()->getOpposite(vneighbor->getGCell()) << endl;
      cdebug_log(111,0) << "From2: " << vneighbor->getFrom2()->getOpposite(vneighbor->getGCell()) << endl;
      vneighbor->getIntervFrom().print();
      vneighbor->getIntervFrom2().print();
    } 
  }


  void Dijkstra::_updateGRAData ( Vertex* vneighbor, bool isDistance2shorter, Vertex* current )
  {
    vneighbor->unsetFlags(Vertex::UseFromFrom2);
  //cdebug_log(111,0) << "unsetFromFrom2: " << vneighbor << endl; 
    vneighbor->clearFrom2();
    if (isDistance2shorter) {
      vneighbor->setFlags(Vertex::UseFromFrom2);
    //cdebug_log(111,0) << "setFromFrom2: " << vneighbor << endl; 
    }// else  cdebug_log(111,0) << "setFrom1: " << vneighbor << endl; 
            
    vneighbor->setIntervals( current );
    vneighbor->getIntervFrom().print();
  }

  void Dijkstra::_initiateUpdateIntervals ( Vertex* current )
  {
    GCell*  gcurr  = current->getGCell();
    GCell*  gprev  = current->getFrom()->getOpposite(gcurr);
    Vertex* vprev  = gprev->getObserver<Vertex>(GCell::Observable::Vertex);
    Point   pcurrent = vprev->getStartPathPoint(current);
    Point   pentry   = vprev->getNextPathPoint( pcurrent, current );
    cdebug_log(112,0) << "current : " << gcurr << endl;
    cdebug_log(112,0) << "previous: " << gprev << endl;
    cdebug_log(112,0) << "pcurr : x: " << DbU::getValueString(pcurrent.getX()) << ", y: " << DbU::getValueString(pcurrent.getY()) << endl;
    cdebug_log(112,0) << "pentry: x: " << DbU::getValueString(pentry.getX())   << ", y: " << DbU::getValueString(pentry.getY())   << endl;
 
    cdebug_log(112,0) << "| " << current << " | " << endl;
    if        (current->isH()){
      if        (pentry.getX() < current->getIMin()){ 
        current->setInterv(pentry.getX(), current->getIMax(), current->getIAxis());
        cdebug_log(112,0) << "[Interval update1]: min : " << DbU::getValueString(pentry.getX());
        cdebug_log(112,0) <<                   ", max : " << DbU::getValueString(current->getIMax());
        cdebug_log(112,0) <<                   ", axis: " << DbU::getValueString(current->getIAxis()) << endl;
      } else if (pentry.getX() > current->getIMax()){
        current->setInterv(current->getIMin(), pentry.getX(), current->getIAxis());
        cdebug_log(112,0) << "[Interval update2]: min : " << DbU::getValueString(current->getIMin());
        cdebug_log(112,0) <<                   ", max : " << DbU::getValueString(pentry.getX());
        cdebug_log(112,0) <<                   ", axis: " << DbU::getValueString(current->getIAxis()) << endl;
      }
    } else if (current->isV()){
      if        (pentry.getY() < current->getIMin()){ 
        current->setInterv(pentry.getY(), current->getIMax(), current->getIAxis());
        cdebug_log(112,0) << "[Interval update3]: min : " << DbU::getValueString(pentry.getY());
        cdebug_log(112,0) <<                   ", max : " << DbU::getValueString(current->getIMax());
        cdebug_log(112,0) <<                   ", axis: " << DbU::getValueString(current->getIAxis()) << endl;
      } else if (pentry.getY() > current->getIMax()){
        current->setInterv(current->getIMin(), pentry.getY(), current->getIAxis());
        cdebug_log(112,0) << "[Interval update4]: min : " << DbU::getValueString(current->getIMin());
        cdebug_log(112,0) <<                   ", max : " << DbU::getValueString(pentry.getY());
        cdebug_log(112,0) <<                   ", axis: " << DbU::getValueString(current->getIAxis()) << endl;
      }
    } 
    cdebug_log(112,0) << "isiSet: " << current->isiSet() << endl;
  }


  bool Dijkstra::_updateIntervals( bool& isfirst, Vertex* current, bool& useFrom2, int& branchId, Edge* from )
  {
    if (!isfirst){
      cdebug_log(112,0) << "Is not first" << endl;
      current->incDegree();
      if (current->getConnexId() == _connectedsId) return true;
      from = NULL;
      if (useFrom2) {
        cdebug_log(112,0) << "USE FROM2: " << current->getFrom2()->getOpposite(current->getGCell()) << endl;
        current->setFrom(current->getFrom2());
        current->setIntervfrom(current->getPIMin2(), current->getPIMax2(), current->getPIAxis2());
        current->clearFrom2();
      }
      from = current->getFrom();
      if (not from) return true;

      current->setDistance( 0.0 );
      current->setConnexId( _connectedsId );
      current->setBranchId( branchId );
      _sources.insert( current );
      _queue.push( current );
    } else isfirst = false;

    if (current->getPredecessor() != NULL){
      cdebug_log(112,0) << "Predecessor()    : " << current->getPredecessor() << endl;
      cdebug_log(112,0) << "[Interval update]: min : " << DbU::getValueString(current->getPIMin());
      cdebug_log(112,0) <<                  ", max : " << DbU::getValueString(current->getPIMax());
      cdebug_log(112,0) <<                  ", axis: " << DbU::getValueString(current->getPIAxis()) << endl;
      current->getPredecessor()->setInterv(current->getPIMin(), current->getPIMax(), current->getPIAxis());
      current->getIntervFrom().print();
    //if (current->getPredecessor()->getGCell()->isStrut()) _updateRealOccupancy( current );
    } 
    return false;
  }


  bool Dijkstra::_attachSymContactsHook( RoutingPad* rp )
  {
    NetRoutingState* state = NetRoutingExtension::get( _net );
    if (state){
      if (state->isSelfSym()){
        if ( ( (state->isSymHorizontal())&&(rp->getBoundingBox().getYMin() > state->getSymAxis()) )
           ||( (state->isSymVertical()  )&&(rp->getBoundingBox().getXMin() > state->getSymAxis()) )
           ){
          Point    center   = rp->getBoundingBox().getCenter();
          GCell*   gcell    = _anabatic->getGCellUnder( center );
          Vertex*  seed     = gcell->getObserver<Vertex>(GCell::Observable::Vertex);
          Contact* vcontact = seed->getGContact( _net );
          rp->getBodyHook()->detach();
          rp->getBodyHook()->attach( vcontact->getBodyHook() );
          return true;
        }
      }
    }
    return false;
  }


  void Dijkstra::_limitSymSearchArea( RoutingPad* rp )
  {
    NetRoutingState* state = NetRoutingExtension::get( _net );
    Point  center = rp->getBoundingBox().getCenter();
    GCell* gcell  = _anabatic->getGCellUnder( center );
    if (state){
      if (state->isSymHorizontal()){
        _searchArea.merge( Box( _net->getCell()->getAbutmentBox().getXMin()
                              , _net->getCell()->getAbutmentBox().getYMin()
                              , _net->getCell()->getAbutmentBox().getXMax()
                              , state->getSymAxis()
                              ) 
                         );
      } else if (state->isSymVertical()){
        _searchArea.merge( Box( _net->getCell()->getAbutmentBox().getXMin()
                              , _net->getCell()->getAbutmentBox().getYMin()
                              , state->getSymAxis()
                              , _net->getCell()->getAbutmentBox().getYMax()
                              ) 
                         );
      } else if (gcell->isDevice()){
        _searchArea.merge( _net->getCell()->getAbutmentBox() );
      }
    } else if (gcell->isDevice()){
      _searchArea.merge( _net->getCell()->getAbutmentBox() );
    }
  }


  void Dijkstra::_setSourcesGRAData( Vertex* seed, RoutingPad* rp )
  {
    Horizontal* h    = dynamic_cast<Horizontal*>(rp->_getEntityAsSegment());
    Vertical*   v    = dynamic_cast<Vertical*>  (rp->_getEntityAsSegment());
    seed->createAData();
    if (h) {
      seed->setFlags(Vertex::iHorizontal);
      seed->setInterv(rp->getBoundingBox().getXMin(), rp->getBoundingBox().getXMax(), rp->getBoundingBox().getYCenter());
    }
    if (v) {
      seed->setFlags(Vertex::iVertical);
      seed->setInterv(rp->getBoundingBox().getYMin(), rp->getBoundingBox().getYMax(), rp->getBoundingBox().getXCenter());
    }
  }


  void Dijkstra::_updateRealOccupancy ( Vertex* current )
  {
    cerr << "void Dijkstra::_updateRealOccupancy ( Vertex* current ): " << current << endl;
    GCell* gcurrent = current->getGCell();
    GCell* gnext    = current->getPredecessor()->getGCell();
    Edge*  e        = gcurrent->getEdgeTo(gnext);

    NetRoutingState* state = NetRoutingExtension::get( _net );
    cerr << "e: " << e << endl;
    e->incRealOccupancy2(state->getWPitch());
    cerr << "e: " << e << endl;
    if (current->getGCell()->getWestEdge()) cerr << "W occupancy: " << current->getGCell()->getWestEdge()->getRealOccupancy()  << "/" << current->getGCell()->getWestEdge()->getCapacity() << endl;
    if (current->getGCell()->getEastEdge()) cerr << "E occupancy: " << current->getGCell()->getEastEdge()->getRealOccupancy()  << "/" << current->getGCell()->getEastEdge()->getCapacity() << endl;
    if (current->getGCell()->getNorthEdge()) cerr << "N occupancy: " << current->getGCell()->getNorthEdge()->getRealOccupancy() << "/" << current->getGCell()->getNorthEdge()->getCapacity() << endl;
    if (current->getGCell()->getSouthEdge()) cerr << "S occupancy: " << current->getGCell()->getSouthEdge()->getRealOccupancy() << "/" << current->getGCell()->getSouthEdge()->getCapacity() << endl;
  }


}  // Anabatic namespace.
