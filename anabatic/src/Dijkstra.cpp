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
// Class  :  "Anabatic::Symmetry".


  Symmetry::Symmetry( unsigned int type, DbU::Unit value )
  {
    if      (type == sHorizontal) setAsH();
    else if (type == sVertical  ) setAsV();

    _value = value;
  }


  Symmetry::~Symmetry() {}


  Symmetry* Symmetry::create( unsigned int type, DbU::Unit value )
  {
    return new Symmetry( type, value );
  }


  unsigned int Symmetry::getType() const 
  {
    if      (_flags & sHorizontal) return sHorizontal;
    else if (_flags & sVertical  ) return sVertical;
    else    return 0;
  }



// -------------------------------------------------------------------
// Class  :  "Anabatic::IntervalC".


  IntervalC::IntervalC()
  {
    _min  = Vertex::unreached;
    _max  = Vertex::unreached;
    _axis = Vertex::unreached;
  }


  IntervalC::~IntervalC() {}


  IntervalC* IntervalC::create()
  {
    return new IntervalC();
  }


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
    cerr << "[IntervalC]: min: " << DbU::getValueString(_min) << ", max:" << DbU::getValueString(_min) << ", axis:" << DbU::getValueString(_axis) << endl;
  }


  void IntervalC::reset()
  {
    _min  = Vertex::unreached;
    _max  = Vertex::unreached;
    _axis = Vertex::unreached;
    _flags &= ~iSet;
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


  bool Vertex::isRestricted ( const Vertex* v1, const Vertex* v2 )
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
    
    if   (restricted) return true;
    else {
    // Check from GCell 2 
      if        ( c2->isNorth(c1) ) {
        if ( v2->isNRestricted() ) return true;
        else                       return false;
      } else if ( c2->isSouth(c1) ) {
        if ( v2->isSRestricted() ) return true;
        else                       return false;
      } else if ( c2->isEast (c1) ) {
        if ( v2->isERestricted() ) return true;
        else                       return false;
      } else if ( c2->isWest (c1) ) {
        if ( v2->isWRestricted() ) return true;
        else                       return false;
      } else {
        cerr << Error( "GCells are not side by side." ) << endl;
        return true;
      }
    }
  }


  Point Vertex::getNextPathPoint2( const Vertex* vcurr, const Vertex* vnext ) 
  {
  //cdebug_log(112,0) << "Point Dijkstra::getNextPathPoint( const Vertex* current, const Vertex* next )" << endl;
    if ((vcurr == NULL) || (vnext == NULL)){
    //cdebug_log(112,0) << "Error(Point Dijkstra::_getNextPathPoint( const Vertex*, const Vertex* )): Unvalid NULL argument."<< endl;
      return Point(0,0);
    }

    if (vnext->getGCell()->isMatrix()) {
      return Point(vnext->getGCell()->getXCenter(), vnext->getGCell()->getYCenter());
    }
    
    Point  pcurr = vcurr->getPathPoint(vnext);
    GCell* gnext = vnext->getGCell();
    GCell* gcurr = vcurr->getGCell();
    DbU::Unit x = 0;
    DbU::Unit y = 0;


    if (vnext->isV()){
    //cdebug_log(112,0) << "Case next: Vertical: " << vnext->isiSet() << endl; //", d:" << vnext->getDistance() << endl;
      if ((vnext->isiSet())&&(vnext->hasValidStamp())){
      //cdebug_log(112,0) << "Case set" << endl;
        x = vnext->getIAxis();
        if (vcurr->isNorth(vnext))      y = vnext->getIMin();
        else if (vcurr->isSouth(vnext)) y = vnext->getIMax();
        else if ((vcurr->isWest(vnext))||(vcurr->isEast(vnext))) {
          if      ( pcurr.getY() > vnext->getIMax() ) y = vnext->getIMax();
          else if ( pcurr.getY() < vnext->getIMin() ) y = vnext->getIMin();
          else                                        y = pcurr.getY();
        } else  cdebug_log(112,0) << "Error(Point Vertex::getNextPathPoint2(...) const: Something is wrong.1" << endl;
      } else {
      //cdebug_log(112,0) << "Case not set" << endl;
        if (vcurr->isNorth(vnext)){
          y = gcurr->getYMax();
          if      (pcurr.getX() < gnext->getXMin()) x = gnext->getXMin();
          else if (pcurr.getX() > gnext->getXMax()) x = gnext->getXMax();
          else                                      x = pcurr.getX();
        } else if (vcurr->isSouth(vnext)){
          y = gcurr->getYMin();
          if      (pcurr.getX() < gnext->getXMin()) x = gnext->getXMin();
          else if (pcurr.getX() > gnext->getXMax()) x = gnext->getXMax();
          else                                      x = pcurr.getX();
        } else if (vcurr->isWest(vnext)){
          x = gcurr->getXMin();
          if      (pcurr.getY() < gnext->getYMin()) y = gnext->getYMin();
          else if (pcurr.getY() > gnext->getYMax()) y = gnext->getYMax();
          else                                      y = pcurr.getY();
          
        } else if (vcurr->isEast(vnext)){
          x = gcurr->getXMax();
          if      (pcurr.getY() < gnext->getYMin()) y = gnext->getYMin();
          else if (pcurr.getY() > gnext->getYMax()) y = gnext->getYMax();
          else                                      y = pcurr.getY();
        } else  cdebug_log(112,0) << "Error(Point Vertex::getNextPathPoint2(...) const: Something is wrong.2" << endl;
      }
      
    } else if (vnext->isH()) {
    //cdebug_log(112,0) << "Case next: Horizontal: " << vnext->isiSet() << endl; //", d:" << vnext->getDistance() << endl;
      
      if ((vnext->isiSet())&&(vnext->hasValidStamp())){
      //cdebug_log(112,0) << "Case set" << endl;
        y = vnext->getIAxis();
        if      (vcurr->isEast (vnext)) x = vnext->getIMin();
        else if (vcurr->isWest (vnext)) x = vnext->getIMax();
        else if ((vcurr->isNorth(vnext))||(vcurr->isSouth(vnext))) {
          if      ( pcurr.getX() > vnext->getIMax() ) x = vnext->getIMax();
          else if ( pcurr.getX() < vnext->getIMin() ) x = vnext->getIMin();
          else                                        x = pcurr.getX();
        } else  cdebug_log(112,0) << "Error(Point Vertex::getNextPathPoint2(...) const: Something is wrong.3" << endl;

      } else {
      //cdebug_log(112,0) << "Case not set" << endl;
        if (vcurr->isNorth(vnext)){
          y = gcurr->getYMax();
          if      (pcurr.getX() < gnext->getXMin()) x = gnext->getXMin();
          else if (pcurr.getX() > gnext->getXMax()) x = gnext->getXMax();
          else                                      x = pcurr.getX();
        } else if (vcurr->isSouth(vnext)){
          y = gcurr->getYMin();
          if      (pcurr.getX() < gnext->getXMin()) x = gnext->getXMin();
          else if (pcurr.getX() > gnext->getXMax()) x = gnext->getXMax();
          else                                      x = pcurr.getX();
        } else if (vcurr->isWest(vnext)){
          x = gcurr->getXMin();
          if      (pcurr.getY() < gnext->getYMin()) y = gnext->getYMin();
          else if (pcurr.getY() > gnext->getYMax()) y = gnext->getYMax();
          else                                      y = pcurr.getY();
          
        } else if (vcurr->isEast(vnext)){
          x = gcurr->getXMax();
          if      (pcurr.getY() < gnext->getYMin()) y = gnext->getYMin();
          else if (pcurr.getY() > gnext->getYMax()) y = gnext->getYMax();
          else                                      y = pcurr.getY();
        } else  cdebug_log(112,0) << "Error(Point Vertex::getNextPathPoint2(...) const: Something is wrong.4" << endl;
      }
    } else {
      cdebug_log(112,0) << "Error(Point Vertex::getNextPathPoint2(...) const: Something is wrong.5" << endl;
    }
    return Point(x,y);
  }


  Point Vertex::getPathPoint( const Vertex* next ) const
  {
  //cdebug_log(112,0) << "Point Vertex::getPathPoint( const Vertex* next ) const:" << this << endl;
    
    GCell* gcurr = getGCell();
    GCell* gnext = next->getGCell();
    DbU::Unit x = 0;
    DbU::Unit y = 0;
    

    if (gcurr->isDevice  ()){
    //cdebug_log(112,0) << "Case device" << endl;
      if      (isH()){
      //cdebug_log(112,0) << "hinterval: " <<  DbU::getValueString(_interv->getAxis()) << endl;
        y = _interv->getAxis();
        if      ((gnext->getXMax() < _interv->getMin())||(isWest (next))) x = _interv->getMin();
        else if ((gnext->getXMin() > _interv->getMax())||(isEast (next))) x = _interv->getMax();
        else    x = (max(gnext->getXMin(), _interv->getMin())+min(gnext->getXMax(), _interv->getMax()))/2;

      } else if (isV()){
      //cdebug_log(112,0) << "vinterval" << endl;
        x = _interv->getAxis();
        if      ((gnext->getYMax() < _interv->getMin())||(isSouth(next))) y = _interv->getMin();
        else if ((gnext->getYMin() > _interv->getMax())||(isNorth(next))) y = _interv->getMax();
        else y = (max(gnext->getYMin(), _interv->getMin())+min(gnext->getYMax(), _interv->getMax()))/2 ;
      } else {
        cdebug_log(112,0) << "Error(Point Vertex::getPathPoint( Vertex * vertex ) const: Something is wrong." << endl;
        return Point(0,0);
      }
    } else if (isH()) {
    //cdebug_log(112,0) << "Case horizontal: " << isiSet() << endl;
      GCell*  gprev = getFrom()->getOpposite(gcurr);
      Vertex* prev  = gprev->getObserver<Vertex>(GCell::Observable::Vertex);
      if (isiSet()){
      //cdebug_log(112,0) << "isiSet" << endl;
        y = _interv->getAxis();
        if      ((gnext->getXMax() < _interv->getMin())||(isWest (next))) x = _interv->getMin();
        else if ((gnext->getXMin() > _interv->getMax())||(isEast (next))) x = _interv->getMax();
        else    x = (max(gnext->getXMin(), _interv->getMin())+min(gnext->getXMax(), _interv->getMax()))/2;
      } else {
        if        (prev->isH()){
        //cdebug_log(112,0) << "prev is H" << endl;
          if        (isNorth(prev)){
            x = (max(_intervfrom->getMin(), gcurr->getXMin())+min(_intervfrom->getMax(), gcurr->getXMax()))/2 ;
            y = gcurr->getYMax();
          } else if (isSouth(prev)){
            x = (max(_intervfrom->getMin(), gcurr->getXMin())+min(_intervfrom->getMax(), gcurr->getXMax()))/2 ;
            y = gcurr->getYMin();
          } else if (isWest (prev)){
            x = gcurr->getXMin();
            y = _intervfrom->getAxis();
          } else if (isEast (prev)){
            x = gcurr->getXMax();
            y = _intervfrom->getAxis();
          } else {
            cdebug_log(112,0) << "Error(Point Vertex::getPathPoint( Vertex * vertex ) const: Something is wrong." << endl;
            return Point(0,0);
          }
        } else if (prev->isV()){
          cdebug_log(112,0) << "prev is V" << endl;
          if        (isNorth(prev)){
          //cdebug_log(112,0) << "1" << endl;
            x = _intervfrom->getAxis();
            y = gcurr->getYMax();
          } else if (isSouth(prev)){
          //cdebug_log(112,0) << "2" << endl;
            x = _intervfrom->getAxis();
            y = gcurr->getYMin();
          } else if (isWest (prev)){
          //cdebug_log(112,0) << "3" << endl;
            x = gcurr->getXMin();
            if        ( _intervfrom->getMin() > gcurr->getYMax() ){
              y = gcurr->getYMax();
            } else if ( _intervfrom->getMax() < gcurr->getYMin() ){
              y = gcurr->getYMin();
            } else {
              y = (max(_intervfrom->getMin(), gcurr->getYMin())+min(_intervfrom->getMax(), gcurr->getYMax()))/2 ;
            }
          } else if (isEast (prev)){
          //cdebug_log(112,0) << "4" << endl;
            x = gcurr->getXMax();
            if        ( _intervfrom->getMin() > gcurr->getYMax() ){
              y = gcurr->getYMax();
            } else if ( _intervfrom->getMax() < gcurr->getYMin() ){
              y = gcurr->getYMin();
            } else {
              y = (max(_intervfrom->getMin(), gcurr->getYMin())+min(_intervfrom->getMax(), gcurr->getYMax()))/2 ;
            }
          } else {
            cdebug_log(112,0) << "Error(Point Vertex::getPathPoint( Vertex * vertex ) const: Something is wrong." << endl;
            return Point(0,0);
          }
          cdebug_log(112,0) << "x: " << DbU::getValueString(x) << ", y:" << DbU::getValueString(y) << endl;
        } else {
          cdebug_log(112,0) << "Error(Point Vertex::getPathPoint( Vertex * vertex ) const: Something is wrong." << endl;
          return Point(0,0);
        }
      }
    } else if (isV()) {
    //cdebug_log(112,0) << "Case V: " << isiSet() << endl;
      GCell*  gprev = getFrom()->getOpposite(gcurr);
      Vertex* prev  = gprev->getObserver<Vertex>(GCell::Observable::Vertex);
      if (isiSet()){
      //cdebug_log(112,0) << "isiSet" << endl;
        x = _interv->getAxis();
        if      ((gnext->getYMax() < _interv->getMin())||(isSouth(next))) y = _interv->getMin();
        else if ((gnext->getYMin() > _interv->getMax())||(isNorth(next))) y = _interv->getMax();
        else y = (max(gnext->getYMin(), _interv->getMin())+min(gnext->getYMax(), _interv->getMax()))/2 ;
      } else {
        if        (prev->isH()){
        //cdebug_log(112,0) << "prev is H" << endl;
          if        (isNorth(prev)){
            y = gcurr->getYMax();
            if        ( _intervfrom->getMin() > gcurr->getXMax() ){
              x = gcurr->getXMax();
            } else if ( _intervfrom->getMax() < gcurr->getXMin() ){
              x = gcurr->getXMin();
            } else {
              x = (max(_intervfrom->getMin(), gcurr->getXMin())+min(_intervfrom->getMax(), gcurr->getXMax()))/2 ;
            }
          } else if (isSouth(prev)){
            y = gcurr->getYMin();
            if        ( _intervfrom->getMin() > gcurr->getXMax() ){
              x = gcurr->getXMax();
            } else if ( _intervfrom->getMax() < gcurr->getXMin() ){
              x = gcurr->getXMin();
            } else {
              x = (max(_intervfrom->getMin(), gcurr->getXMin())+min(_intervfrom->getMax(), gcurr->getXMax()))/2 ;
            }
          } else if (isWest (prev)){
            x = gcurr->getXMin();
            y = _intervfrom->getAxis();
          } else if (isEast (prev)){
            x = gcurr->getXMax();
            y = _intervfrom->getAxis();
          } else {
            cdebug_log(112,0) << "Error(Point Vertex::getPathPoint( Vertex * vertex ) const: Something is wrong." << endl;
            return Point(0,0);
          }
        } else if (prev->isV()){
        //cdebug_log(112,0) << "prev is V" << endl;
          if        (isNorth(prev)){
            x = _intervfrom->getAxis();
            y = gcurr->getYMax();
          } else if (isSouth(prev)){
            x = _intervfrom->getAxis();
            y = gcurr->getYMin();
          } else if (isWest (prev)){
            x = gcurr->getXMin();
            y = (max(_intervfrom->getMin(), gcurr->getYMin())+min(_intervfrom->getMax(), gcurr->getYMax()))/2 ;
          } else if (isEast (prev)){
            x = gcurr->getXMax();
            y = (max(_intervfrom->getMin(), gcurr->getYMin())+min(_intervfrom->getMax(), gcurr->getYMax()))/2 ;
          } else {
            cdebug_log(112,0) << "Error(Point Vertex::getPathPoint( Vertex * vertex ) const: Something is wrong." << endl;
            return Point(0,0);
          }
        } else {
          cdebug_log(112,0) << "Error(Point Vertex::getPathPoint( Vertex * vertex ) const: Something is wrong." << endl;
          return Point(0,0);
        }
      }
    } else {
        cdebug_log(112,0) << "Error(Point Vertex::getPathPoint( Vertex * vertex ) const: Something is wrong." << endl;
        return Point(0,0);
    }
    return Point(x,y);
  }


  bool Vertex::isH() const
  {
    GCell* gcell = getGCell();
    if      (gcell->isDevice())   return _interv->isH();
    else if (gcell->isHChannel()) return true;
    else if (gcell->isStrut())    return gcell->getWidth() > gcell->getHeight();
    else                          return false;
  }


  bool Vertex::isV() const
  {
    GCell* gcell = getGCell();
    if      (gcell->isDevice())   return _interv->isV();
    else if (gcell->isVChannel()) return true;
    else if (gcell->isStrut())    return gcell->getWidth() < gcell->getHeight();
    else                          return false;
  }

  
  void Vertex::setIntervals ( Vertex* vcurr )
  {
    Point pcurr = vcurr->getPathPoint(this);
    Point pnext = Vertex::getNextPathPoint2( vcurr, this );
  //cdebug_log(112,0) << "void Vertex::setIntervals ( Vertex* vcurr )" << endl;
    cdebug_log(112,0) << "Pcurrent  : X:" << DbU::getValueString(pcurr.getX())   << ", Y:" << DbU::getValueString(pcurr.getY())   << endl;
  //cdebug_log(112,0) << "Pneighbour: X:" << DbU::getValueString(pnext.getX()) << ", Y:" << DbU::getValueString(pnext.getY()) << endl;
    DbU::Unit min, max, axis;

    if        (vcurr->isH()){
    //cdebug_log(112,0) << "case vcurr: Horizontal" << endl;
      if ((vcurr->isiSet())&&(vcurr->hasValidStamp())){
      //cdebug_log(112,0) << "case set" << endl;
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
      //cdebug_log(112,0) << "case not set" << endl;
        axis = pcurr.getY();
        if (pcurr.getX() < pnext.getX()){
          min = pcurr.getX();
          max = pnext.getX();
        } else {
          max = pcurr.getX();
          min = pnext.getX();
        }
      }

    } else if (vcurr->isV()){
    //cdebug_log(112,0) << "case vcurr: Vertical" << endl;
      
      if ((vcurr->isiSet())&&(vcurr->hasValidStamp())){
      //cdebug_log(112,0) << "case set" << endl;
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
      //cdebug_log(112,0) << "case not set" << endl;
        axis = pcurr.getX();
        if (pcurr.getY() < pnext.getY()){
          min = pcurr.getY();
          max = pnext.getY();
        } else {
          max = pcurr.getY();
          min = pnext.getY();
        }
      }

    } else {
      cdebug_log(112,0) << "Error(void Vertex::setIntervals(...)): Something is wrong." << endl;
      return;
    }
    cdebug_log(112,0) << "IntervFrom => min: " << DbU::getValueString(min) << ", max: " << DbU::getValueString(max) << ", axis:" << DbU::getValueString(axis) << endl; 
    
    _intervfrom->set( min, max, axis );
  }


  void Vertex::resetIntervals()
  {
    _interv->reset();
    _intervfrom->reset();
  }


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
    /*+ " rps:" +  getString(_rpCount)
             + " deg:" +  getString(_degree)
             + " connexId:" + ((_connexId >= 0) ? getString(_connexId) : "None")*/
             + " d:" + ((_distance == unreached) ? "unreached"
                                                 : ((_distance == unreachable) ? "unreachable"
                                                                               : DbU::getValueString(_distance)) )
    /*+   "+" + getString(_branchId)
      + " stamp:" + (hasValidStamp() ? "valid" : "outdated")*/
             + " from:" + ((_from) ? "set" : "NULL")
             + " restricted:" + (isNRestricted() ? "N" : "-")
             + (isSRestricted() ? "S" : "-")
             + (isERestricted() ? "E" : "-")
             + (isWRestricted() ? "W" : "-")
             + " isiSet:" +(isiSet() ? "1" : "0")
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
    if (Vertex::isRestricted(current, vneighbour)) return Vertex::unreachable;
    else                                           return current->getDistance() + e->getDistance();
  }


  DbU::Unit Dijkstra::_getDistancetoRP( Point p )
  {

    cdebug_log(112,0) << "DbU::Unit Dijkstra::_getDistancetoRP( Point p ): " << p << endl;
    RoutingPad* rp   = NULL;
    DbU::Unit   dist = 0;
    Cell*       cell = _anabatic->getCell();
    int         cpt  = 0;

  /*for ( Vertex* vertex : _sources ) {
      if (vertex->hasRP()){
        for ( Component* component : cell->getComponentsUnder(vertex->getGCell()->getBoundingBox()) ){   
          rp = dynamic_cast<RoutingPad*>( component );
          if (rp) break;
        }
        if (rp) {
          cdebug_log(112,0) << "Vertex: " << vertex << endl;
          dist +=  _estimatePtoRP(p, rp, vertex );
        }
      }
      }*/
    for ( Vertex* vertex : _targets ) {
      if (vertex->hasRP(_net)){
        for ( Component* component : cell->getComponentsUnder(vertex->getGCell()->getBoundingBox()) ){   
          rp = dynamic_cast<RoutingPad*>( component );
          if (rp) {
            if (rp->getNet() == _net) break;
          }
        }
        if (rp) {
          DbU::Unit distance = _estimatePtoRP(p, rp, vertex );
          cdebug_log(112,0) << "Dist to Vertex: " << vertex  << " IS = " << DbU::getValueString(distance) << endl;
          dist += distance;
          cpt++;
        }
      }
    }
    if (cpt != 0) dist = dist/cpt;
    else          dist = Vertex::unreachable;
    return dist;
  }


  DbU::Unit calcDistance( Point p1, Point p2 )
  {
    return abs(p1.getX()-p2.getX()) + abs(p1.getY()-p2.getY());
  }


  DbU::Unit Dijkstra::_estimatePtoRP( Point p, RoutingPad* rp, Vertex* vertex )
  {
    Horizontal* h = dynamic_cast<Horizontal*>(rp->_getEntityAsSegment());
    Vertical*   v = dynamic_cast<Vertical*  >(rp->_getEntityAsSegment());
    DbU::Unit   d = Vertex::unreachable;  
    GCell*      gvertex = vertex->getGCell(); 

    if (h) {
      cdebug_log(112,0) << "Case: Horizontal: " << endl; 
      if        (p.getX() < rp->getSourceX()){
        cdebug_log(112,0) << "Case: p.getX() < rp->getSourceX()" <<  endl;
        if        (p.getY() < gvertex->getYMin()){
          cdebug_log(112,0) << "Case: West South" <<  endl;
          if ( (!vertex->isSRestricted()) 
             || (!vertex->isWRestricted()) ) d = calcDistance (p, Point(rp->getSourceX(), rp->getSourceY()));
          else if (!vertex->isNRestricted()) d = calcDistance (p, Point(rp->getSourceX(), rp->getSourceY())) + (gvertex->getYMax() - rp->getY())*2;
          else if (!vertex->isERestricted()) d = calcDistance (p, Point(rp->getTargetX(), rp->getTargetY()));
          else                               d = Vertex::unreachable;
        } else if (p.getY() > gvertex->getYMax()){
          cdebug_log(112,0) << "Case: West North" <<  endl;
          if ( (!vertex->isNRestricted()) 
             || (!vertex->isWRestricted()) ) d = calcDistance (p, Point(rp->getSourceX(), rp->getSourceY()));
          else if (!vertex->isSRestricted()) d = calcDistance (p, Point(rp->getSourceX(), rp->getSourceY())) + (rp->getY() - gvertex->getYMin())*2;
          else if (!vertex->isERestricted()) d = calcDistance (p, Point(rp->getTargetX(), rp->getTargetY()));
          else                               d = Vertex::unreachable;
        } else {
          cdebug_log(112,0) << "Case: West" <<  endl;
          if (!vertex->isWRestricted()) d = min(d, calcDistance (p, Point(rp->getSourceX(), rp->getSourceY())));
          if (!vertex->isNRestricted()) d = min(d, calcDistance (p, Point(rp->getSourceX(), rp->getSourceY()))+ (gvertex->getYMax() - rp->getY())*2);
          if (!vertex->isSRestricted()) d = min(d, calcDistance (p, Point(rp->getSourceX(), rp->getSourceY()))+ (rp->getY() - gvertex->getYMin())*2);
          if (!vertex->isERestricted()) d = min(d, calcDistance (p, Point(rp->getTargetX(), rp->getTargetY())));
        }
      } else if (p.getX() > rp->getTargetX()){
        cdebug_log(112,0) << "Case: p.getX() > rp->getTargetX()" <<  endl;
        if        (p.getY() < gvertex->getYMin()){
          cdebug_log(112,0) << "Case: East South" <<  endl;
          if ( (!vertex->isSRestricted()) 
             || (!vertex->isERestricted()) ) d = calcDistance (p, Point(rp->getTargetX(), rp->getTargetY()));
          else if (!vertex->isNRestricted()) d = calcDistance (p, Point(rp->getTargetX(), rp->getTargetY())) + (gvertex->getYMax() - rp->getY())*2;
          else if (!vertex->isWRestricted()) d = calcDistance (p, Point(rp->getSourceX(), rp->getSourceY()));
          else                               d = Vertex::unreachable;
        } else if (p.getY() > gvertex->getYMax()){
          cdebug_log(112,0) << "Case: East North" <<  endl;
          if ( (!vertex->isNRestricted()) 
             || (!vertex->isERestricted()) ) d = calcDistance (p, Point(rp->getTargetX(), rp->getTargetY()));
          else if (!vertex->isSRestricted()) d = calcDistance (p, Point(rp->getTargetX(), rp->getTargetY())) + (rp->getY() - gvertex->getYMin())*2;
          else if (!vertex->isWRestricted()) d = calcDistance (p, Point(rp->getSourceX(), rp->getSourceY()));
          else                               d = Vertex::unreachable;
        } else {
          cdebug_log(112,0) << "Case: East" <<  endl;
          if (!vertex->isERestricted()) d = min(d, calcDistance (p, Point(rp->getTargetX(), rp->getTargetY())));
          if (!vertex->isNRestricted()) d = min(d, calcDistance (p, Point(rp->getTargetX(), rp->getTargetY()))+ (gvertex->getYMax() - rp->getY())*2);
          if (!vertex->isSRestricted()) d = min(d, calcDistance (p, Point(rp->getTargetX(), rp->getTargetY()))+ (rp->getY() - gvertex->getYMin())*2);
          if (!vertex->isWRestricted()) d = min(d, calcDistance (p, Point(rp->getSourceX(), rp->getSourceY())));
        }
      } else if (p.getY() < gvertex->getYMin()){
        cdebug_log(112,0) << "Case: South" <<  endl;
        if (!vertex->isSRestricted()) d = min(d, rp->getY()-p.getY());
        if (!vertex->isERestricted()) d = min(d, rp->getY()-p.getY()+ (gvertex->getXMax()-p.getX())*2);
        if (!vertex->isWRestricted()) d = min(d, rp->getY()-p.getY()+ (p.getX()-gvertex->getXMin())*2);
        if (!vertex->isNRestricted()) d = min(d, min ( gvertex->getXMax()-p.getX() + gvertex->getYMax()-p.getY() + gvertex->getYMax()-rp->getY()
                                                     , p.getX()-gvertex->getXMin() + gvertex->getYMax()-p.getY() + gvertex->getYMax()-rp->getY()
                                                     )
                                             );
      } else if (p.getY() > gvertex->getYMax()){
        cdebug_log(112,0) << "Case: North" <<  endl;
        if (!vertex->isNRestricted()) d = min(d, p.getY()-rp->getY());
        if (!vertex->isERestricted()) d = min(d, p.getY()-rp->getY()+ (gvertex->getXMax()-p.getX())*2);
        if (!vertex->isWRestricted()) d = min(d, p.getY()-rp->getY()+ (p.getX()-gvertex->getXMin())*2);
        if (!vertex->isSRestricted()) d = min(d, min ( gvertex->getXMax()-p.getX() + p.getY()-gvertex->getYMin() + rp->getY()-gvertex->getYMin()
                                                     , p.getX()-gvertex->getXMin() + p.getY()-gvertex->getYMin() + rp->getY()-gvertex->getYMin()
                                                     )
                                             );
      }
    }
  
    if (v) { 
      cdebug_log(112,0) << "Case: Vertical" <<  endl;
      if        (p.getY() < rp->getSourceY()){
        cdebug_log(112,0) << "Case: p.getY() < rp->getSourceY()" <<  endl;
        if        (p.getX() < gvertex->getXMin()){
          cdebug_log(112,0) << "Case: South West" <<  endl;
          if ( (!vertex->isSRestricted()) 
             || (!vertex->isWRestricted()) ) d = calcDistance (p, Point(rp->getSourceX(), rp->getSourceY()));
          else if (!vertex->isERestricted()) d = calcDistance (p, Point(rp->getSourceX(), rp->getSourceY())) + (gvertex->getXMax() - rp->getX())*2;
          else if (!vertex->isNRestricted()) d = calcDistance (p, Point(rp->getTargetX(), rp->getTargetY()));
          else                               d = Vertex::unreachable;
        } else if (p.getX() > gvertex->getXMax()){
          cdebug_log(112,0) << "Case: South East" <<  endl;
          if ( (!vertex->isSRestricted()) 
             || (!vertex->isERestricted()) ) d = calcDistance (p, Point(rp->getSourceX(), rp->getSourceY()));
          else if (!vertex->isWRestricted()) d = calcDistance (p, Point(rp->getSourceX(), rp->getSourceY())) + (rp->getX() - gvertex->getXMin())*2;
          else if (!vertex->isNRestricted()) d = calcDistance (p, Point(rp->getTargetX(), rp->getTargetY()));
          else                               d = Vertex::unreachable;
        } else {
          cdebug_log(112,0) << "Case: South " <<  endl;
          if (!vertex->isSRestricted()) d = min(d, calcDistance (p, Point(rp->getSourceX(), rp->getSourceY())));
          if (!vertex->isWRestricted()) d = min(d, calcDistance (p, Point(rp->getSourceX(), rp->getSourceY()))+ (gvertex->getXMax() - rp->getX())*2);
          if (!vertex->isERestricted()) d = min(d, calcDistance (p, Point(rp->getSourceX(), rp->getSourceY()))+ (rp->getX() - gvertex->getXMin())*2);
          if (!vertex->isNRestricted()) d = min(d, calcDistance (p, Point(rp->getTargetX(), rp->getTargetY())));
        }
      } else if (p.getY() > rp->getTargetY()){
        cdebug_log(112,0) << "Case: p.getY() > rp->getTargetY()" <<  endl;
        if        (p.getX() < gvertex->getXMin()){
          cdebug_log(112,0) << "Case: North West" <<  endl;
          if ( (!vertex->isNRestricted()) 
             || (!vertex->isWRestricted()) ) d = calcDistance (p, Point(rp->getTargetX(), rp->getTargetY()));
          else if (!vertex->isERestricted()) d = calcDistance (p, Point(rp->getTargetX(), rp->getTargetY())) + (gvertex->getXMax() - rp->getX())*2;
          else if (!vertex->isSRestricted()) d = calcDistance (p, Point(rp->getSourceX(), rp->getSourceY()));
          else                               d = Vertex::unreachable;
        } else if (p.getX() > gvertex->getXMax()){
          cdebug_log(112,0) << "Case: North East" <<  endl;
          if ( (!vertex->isNRestricted()) 
             || (!vertex->isERestricted()) ) d = calcDistance (p, Point(rp->getTargetX(), rp->getTargetY()));
          else if (!vertex->isWRestricted()) d = calcDistance (p, Point(rp->getTargetX(), rp->getTargetY())) + (rp->getX() - gvertex->getXMin())*2;
          else if (!vertex->isSRestricted()) d = calcDistance (p, Point(rp->getSourceX(), rp->getSourceY()));
          else                               d = Vertex::unreachable;
        } else {
          cdebug_log(112,0) << "Case: North" << endl;
          if (!vertex->isNRestricted()) d = min(d, calcDistance (p, Point(rp->getTargetX(), rp->getTargetY())));
          if (!vertex->isWRestricted()) d = min(d, calcDistance (p, Point(rp->getTargetX(), rp->getTargetY()))+ (gvertex->getXMax() - rp->getX())*2);
          if (!vertex->isERestricted()) d = min(d, calcDistance (p, Point(rp->getTargetX(), rp->getTargetY()))+ (rp->getX() - gvertex->getXMin())*2);
          if (!vertex->isSRestricted()) d = min(d, calcDistance (p, Point(rp->getSourceX(), rp->getSourceY())));
        }
      } else if (p.getX() < gvertex->getXMin()){
        cdebug_log(112,0) << "Case: West" <<  endl;
        if (!vertex->isWRestricted()) d = min(d, rp->getX()-p.getX());
        if (!vertex->isNRestricted()) d = min(d, rp->getX()-p.getX()+ (gvertex->getYMax()-p.getY())*2);
        if (!vertex->isSRestricted()) d = min(d, rp->getX()-p.getX()+ (p.getY()-gvertex->getYMin())*2);
        if (!vertex->isERestricted()) d = min(d, min ( gvertex->getYMax()-p.getY() + gvertex->getXMax()-p.getX() + gvertex->getXMax()-rp->getX()
                                                     , p.getY()-gvertex->getYMin() + gvertex->getXMax()-p.getX() + gvertex->getXMax()-rp->getX()
                                                     )
                                             );
      } else if (p.getX() > gvertex->getXMax()){
        cdebug_log(112,0) << "Case: East" << endl;
        if (!vertex->isERestricted()) d = min(d, p.getX()-rp->getX());
        if (!vertex->isNRestricted()) d = min(d, p.getX()-rp->getX()+ (gvertex->getYMax()-p.getY())*2);
        if (!vertex->isSRestricted()) d = min(d, p.getX()-rp->getX()+ (p.getY()-gvertex->getYMin())*2);
        if (!vertex->isWRestricted()) d = min(d, min ( gvertex->getYMax()-p.getY() + p.getX()-gvertex->getXMin() + rp->getX()-gvertex->getXMin()
                                                     , p.getY()-gvertex->getYMin() + p.getX()-gvertex->getXMin() + rp->getX()-gvertex->getXMin()
                                                     )
                                             );
      }
    }
    return d;
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
              continue;
            }
          }
        }
        rps.push_back( rp ); 
        cdebug_log(112,0) << "| " << rp << endl;
        continue; 
      }
    }

    for ( auto rp : rps ) {
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
        }
      } else if (gcell->isDevice()){
        _searchArea.merge( _net->getCell()->getAbutmentBox() );
      }

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

      cdebug_log(112,0) << "Current Search area: " << _searchArea << ", gcell: " << gcell << endl;
      _searchArea.merge( gcell->getBoundingBox() );
      cdebug_log(112,0) << "New Search area: " << _searchArea << endl;

      Vertex*     seed = gcell->getObserver<Vertex>(GCell::Observable::Vertex);
      Horizontal* h    = dynamic_cast<Horizontal*>(rp->_getEntityAsSegment());
      Vertical*   v    = dynamic_cast<Vertical*>  (rp->_getEntityAsSegment());
      if (h) {
        seed->setIAsH();
        seed->setInterv(rp->getBoundingBox().getXMin(), rp->getBoundingBox().getXMax(), rp->getBoundingBox().getYCenter());
      }
      if (v) {
        seed->setIAsV();
        seed->setInterv(rp->getBoundingBox().getYMin(), rp->getBoundingBox().getYMax(), rp->getBoundingBox().getXCenter());
      }
      
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
            current->setAxisTarget();
          //cdebug_log(112,0) << "AxisTarget: " << current << endl;
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
    cdebug_log(112,1) << "Dijkstra::_propagate() " << _net << endl;

    while ( not _queue.empty() ) {
      _queue.dump();

      Vertex* current = _queue.top();
      cdebug_log(111,0) << endl;
      cdebug_log(111,0) << "[Current Vertex]: " << current << ", current->getConnexId() == _connectedsId):" << (current->getConnexId() == _connectedsId)<< ", (current->getConnexId() < 0): "  << current->getConnexId()  << endl;
      _queue.pop();

      if (   current->isAxisTarget() 
         and needAxisTarget()
         ){ 
        unsetFlags(Mode::AxisTarget);
      } else if ((current->getConnexId() == _connectedsId) or (current->getConnexId() < 0)) {

        for ( Edge* edge : current->getGCell()->getEdges() ) {
          if (edge == current->getFrom()) {
            cdebug_log(111,0) << "edge == current->getFrom()" << endl;
            continue;
          }
          GCell*  gneighbor = edge->getOpposite(current->getGCell());
          GCell*  gcurrent  = current->getGCell();
          Vertex* vneighbor = gneighbor->getObserver<Vertex>(GCell::Observable::Vertex);

          if (vneighbor->getConnexId() == _connectedsId) {
            cdebug_log(111,0) << "ConnectedsId" << endl;
            continue;
          }

          if (not _searchArea.intersect(gneighbor->getBoundingBox())) {
            cdebug_log(111,0) << "not in _searchArea: " << _searchArea << ", gneighbor area: "  << gneighbor->getBoundingBox() << endl;
            continue;
          }
          cdebug_log(111,0) << endl  << "===================================================================================" << endl << endl;
          cdebug_log(111,0) << "| Curr: " << current;
          if (current->getFrom()){
            cdebug_log(111,0) << "| From: " << current->getFrom()->getOpposite(gcurrent) << endl;
          } else {
            cdebug_log(111,0) << endl;
          }
          cdebug_log(111,0) << "| Edge " << edge << endl;
          cdebug_log(111,0) << "+ Neighbor: " << vneighbor;
          if (vneighbor->getFrom() != NULL) {cdebug_log(111,0) << "| Neighbor getfrom:" << vneighbor->getFrom()->getOpposite( gneighbor ) << endl;}
          else                              {cdebug_log(111,0) << endl;}


          DbU::Unit distance = _distanceCb( current, vneighbor, edge );
          cdebug_log(111,0) << "Distance curr: " << DbU::getValueString(distance) << endl;
          cdebug_log(111,0) << "Distance prev: " << DbU::getValueString(vneighbor->getDistance()) << endl;

          if  (   (distance == vneighbor->getDistance())
              and ( (!gcurrent->isMatrix())||(!gneighbor->isMatrix()) )
              ){
            cdebug_log(111,0) << "Distance EQUAL" << endl;
            cdebug_log(111,0) << "Previous getfrom:" << vneighbor->getFrom()->getOpposite( gneighbor ) << endl;
            
            GCell*  gnext = vneighbor->getGCell();
            GCell*  gprev = vneighbor->getFrom()->getOpposite(gnext);
            Vertex* vprev = gprev->getObserver<Vertex>(GCell::Observable::Vertex);


            Point     pnext    = Vertex::getNextPathPoint2( current, vneighbor );
            Point     pprev    = Vertex::getNextPathPoint2( vprev, vneighbor );
            Point     ppond    = _getPonderedPoint();
            DbU::Unit distcurr =  this->_getDistancetoRP(pnext);
            DbU::Unit distprev =  this->_getDistancetoRP(pprev);

            cdebug_log(111,0) << "Pcurrent: X:" << DbU::getValueString(pnext.getX()) << ", Y:" << DbU::getValueString(pnext.getY()) << endl;
            cdebug_log(111,0) << "Pprev   : X:" << DbU::getValueString(pprev.getX()) << ", Y:" << DbU::getValueString(pprev.getY()) << endl;

            cdebug_log(111,0) << "distToTargets curr: " << DbU::getValueString(distcurr) << endl;
            cdebug_log(111,0) << "distToTargets prev: " << DbU::getValueString(distprev) << endl;

            if (  ( distcurr < distprev )
               || (  ( distcurr == distprev )
                  && ( calcDistance(ppond, pnext) < calcDistance(ppond, pprev) )
                  )
               ){
              cdebug_log(111,0) << "BETTER GetFROM" << endl;
              vneighbor->setBranchId( current->getBranchId() );
              vneighbor->setDistance( distance );
            //Point pathPoint = Vertex::getNextPathPoint( current, vneighbor );
            //vneighbor->setPathPoint( pathPoint.getX(), pathPoint.getY() );
              vneighbor->setFrom     ( edge );
              _queue.push( vneighbor );

              vneighbor->setIntervals( current );
              cdebug_log(111,0) << "Push: (size:" << _queue.size() << ") " << vneighbor << endl;
            }
          } else if (  ( (distance  < vneighbor->getDistance()) and (distance  != Vertex::unreachable) ) 
                    or ( (distance == vneighbor->getDistance()) and (current->getBranchId() > vneighbor->getBranchId()) ) 
                    ) {
            if (vneighbor->getDistance() != Vertex::unreached) {
              _queue.erase( vneighbor );
            } else {
              if (not vneighbor->hasValidStamp()) {
                cdebug_log(111,0) << "Distance FIRST" << endl;
                vneighbor->setConnexId( -1 );
                vneighbor->setStamp   ( _stamp );
                vneighbor->setDegree  ( 1 );
                vneighbor->setRpCount ( 0 );
                vneighbor->unsetFlags(Vertex::AxisTarget);
              }
            }

            cdebug_log(111,0) << "Distance INF" << endl;
            vneighbor->setBranchId( current->getBranchId() );
            vneighbor->setDistance( distance );
            vneighbor->setFrom     ( edge );
            _queue.push( vneighbor );

            vneighbor->setIntervals( current );
            cdebug_log(111,0) << "Push: (size:" << _queue.size() << ") " << vneighbor << endl;
          }
        }

        continue;
      }

    // We did reach another target (different <connexId>).
    // Tag back the path, with a higher <branchId>.
      _traceback( current );
      cdebug_tabw(112,-1);
      return true;
    }

    cerr << Error( "Dijkstra::propagate(): %s has unreachable targets."
                 , getString(_net).c_str()
                 ) << endl;
    
    for ( Vertex* v : _targets ) {
      cdebug_tabw(112, 0) << v << endl;
      
    }

    cdebug_tabw(112,-1);
    return false;
  }


  void  Dijkstra::_traceback ( Vertex* current )
  {
    cdebug_log(112,1) << "Dijkstra::_traceback() " << _net << " branchId:" << _sources.size() << endl;

    int   branchId      = _sources.size();
    _toSources( current, _connectedsId );


    bool    isfirst  = true;
    if (!current->getGCell()->isMatrix()){
      GCell*  gcurr  = current->getGCell();
      GCell*  gprev  = current->getFrom()->getOpposite(gcurr);
      Vertex* vprev  = gprev->getObserver<Vertex>(GCell::Observable::Vertex);
      Point   pentry = Vertex::getNextPathPoint2( vprev, current );

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
      cdebug_log(112,0) << "isiSet: " << current->isiSet() << ", " << current << endl;
    } else {
      current = current->getPredecessor();
      isfirst = false;
    }

    while ( current ) {
      cdebug_log(112,0) << "| " << current << " | " << endl;

      if (!isfirst){
        current->incDegree();
        if (current->getConnexId() == _connectedsId) break;

        Edge* from = current->getFrom();
        if (not from) break;

        current->setDistance( 0.0 );
        current->setConnexId( _connectedsId );
        current->setBranchId( branchId );
        _sources.insert( current );
        _queue.push( current );
      } else {
        isfirst = false;
      }

      if (!current->getGCell()->isMatrix()){
        if (current->getPredecessor() != NULL){
          cdebug_log(112,0) << "[Interval update]: min : " << DbU::getValueString(current->getPIMin());
          cdebug_log(112,0) <<                  ", max : " << DbU::getValueString(current->getPIMax());
          cdebug_log(112,0) <<                  ", axis: " << DbU::getValueString(current->getPIAxis()) << endl;
          current->getPredecessor()->setInterv(current->getPIMin(), current->getPIMax(), current->getPIAxis());
          cdebug_log(112,0) << "isiSet: " << current->getPredecessor()->isiSet() << ", " << current->getPredecessor() << endl;
        } 
      } 
      
      current = current->getPredecessor();
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
      ////////////////////////////////////////////
        source->resetIntervals();
      ////////////////////////////////////////////

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

          segment = Horizontal::create( sourceContact
                                      , targetContact
                                      , _anabatic->getConfiguration()->getGHorizontalLayer()
                                      , constraint.getCenter()
                                      , DbU::fromLambda(2.0)
                                      );
          for ( Edge* through : aligneds ) through->add( segment );
          if (state){
            if (state->isSymmetric()) _createSelfSymSeg ( segment );
          }
        } else {
          if (sourceContact->getY() > targetContact->getY())
            std::swap( sourceContact, targetContact );

          segment = Vertical::create( sourceContact
                                    , targetContact
                                    , _anabatic->getConfiguration()->getGVerticalLayer()
                                    , constraint.getCenter()
                                    , DbU::fromLambda(2.0)
                                    );
          for ( Edge* through : aligneds ) through->add( segment );
          if (state){
            if (state->isSymmetric()) _createSelfSymSeg ( segment );
          }
        }

        cdebug_log(112,0) << "|| " << segment << endl;
      //cdebug_log(112,0) << "| " << "break (turn, branch or terminal)." << endl;
        source = (target->getFrom()) ? target : NULL;
      }
    }

    cdebug_tabw(112,-1);
  }


  void  Dijkstra::run ( Dijkstra::Mode mode )
  {
    DebugSession::open( _net, 112, 120 );

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
  //cdebug_log(112,1) << "Dijkstra::_toSources() " << endl;
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
                                              , DbU::fromLambda(2.0)
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
                                            , DbU::fromLambda(2.0)
                                            );
        cdebug_log(112,0) << "|| " << segment2 << endl;
      }
    }
  }



}  // Anabatic namespace.
