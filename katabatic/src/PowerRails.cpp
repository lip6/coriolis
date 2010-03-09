
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PowerRails.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <algorithm>

#include  "hurricane/Warning.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Net.h"
#include  "hurricane/NetExternalComponents.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Instance.h"
#include  "crlcore/RoutingLayerGauge.h"
#include  "crlcore/RoutingGauge.h"
#include  "katabatic/KatabaticEngine.h"


namespace {


  using namespace std;
  using namespace Hurricane;


// -------------------------------------------------------------------
// Class  :  "::RailSegment".


  class RailSegment {
    public:
                             RailSegment ( DbU::Unit axis, DbU::Unit width );
      inline DbU::Unit       getAxis     () const;
      inline DbU::Unit       getMin      () const;
      inline DbU::Unit       getMax      () const;
      inline const Interval& getSpan     () const;
      inline DbU::Unit       getWidth    () const;
             void            merge       ( DbU::Unit );
             void            doLayout    ( Cell*, Net*, const Layer*, Constant::Direction ) const;
    private:
      DbU::Unit  _axis;
      DbU::Unit  _width;
      Interval   _span;
  };


  RailSegment::RailSegment ( DbU::Unit axis, DbU::Unit width )
    : _axis (axis)
    , _width(width)
    , _span ()
  { }


  inline DbU::Unit       RailSegment::getAxis  () const { return _axis; }
  inline DbU::Unit       RailSegment::getMin   () const { return _span.getVMin(); }
  inline DbU::Unit       RailSegment::getMax   () const { return _span.getVMax(); }
  inline const Interval& RailSegment::getSpan  () const { return _span; }
  inline DbU::Unit       RailSegment::getWidth () const { return _width; }
  inline void            RailSegment::merge    ( DbU::Unit bound ) { _span.merge(bound); }


  void  RailSegment::doLayout ( Cell* cell, Net* net, const Layer* layer, Constant::Direction direction ) const
  {
    Segment* segment = NULL;

    switch ( direction ) {
      case Constant::Horizontal:
        segment = Horizontal::create ( net
                                     , layer
                                     , _axis
                                     , _width
                                     , _span.getVMin()
                                     , _span.getVMax() );
        break;
      case Constant::Vertical:
        segment = Vertical::create ( net
                                   , layer
                                   , _axis
                                   , _width
                                   , _span.getVMin()
                                   , _span.getVMax() );
        break;
    }

    if ( segment )
      NetExternalComponents::setExternal ( segment );
  }


// -------------------------------------------------------------------
// Class  :  "::PowerRail".


  class PowerRail {
    public:
      struct CompareByAxis : binary_function<PowerRail*,PowerRail*,bool> {
          bool  operator() ( const PowerRail* lhs, const PowerRail* rhs );
      };
    public:
                          PowerRail ( Net::Type, const Layer*, Constant::Direction, DbU::Unit axis );
                         ~PowerRail ();
      inline Net::Type    getType   () const;
      inline const Layer* getLayer  () const;
      inline DbU::Unit    getAxis   () const;
             void         merge     ( DbU::Unit width, DbU::Unit min, DbU::Unit max );
             void         doLayout  ( Cell*, Net* powerNet, Net* groundNet ) const;
    private:
      Net::Type            _type;
      const Layer*         _layer;
      Constant::Direction  _direction;
      DbU::Unit            _axis;
      list<RailSegment*>   _segments;
  };


  PowerRail::PowerRail ( Net::Type type, const Layer* layer, Constant::Direction direction, DbU::Unit axis )
    : _type     (type)
    , _layer    (layer)
    , _direction(direction)
    , _axis     (axis)
    , _segments ()
  { }


  PowerRail::~PowerRail ()
  {
    while ( !_segments.empty() ) {
      delete _segments.front ();
      _segments.pop_front ();
    }
  }


  inline Net::Type    PowerRail::getType  () const { return _type; }
  inline const Layer* PowerRail::getLayer () const { return _layer; }
  inline DbU::Unit    PowerRail::getAxis  () const { return _axis; }


  void  PowerRail::merge ( DbU::Unit width, DbU::Unit min, DbU::Unit max )
  {
    RailSegment* inserted = NULL;

    list<RailSegment*>::iterator isegment = _segments.begin();
    for ( ; (isegment != _segments.end()) && !inserted ; isegment++ ) {
      if ( (*isegment)->getWidth() != width ) continue;

      if ( (*isegment)->getMin() > max ) {
        inserted = new RailSegment ( _axis, width );
        inserted->merge ( min );
        inserted->merge ( max );
        _segments.insert ( isegment, inserted );

        break;
      }
      if ( (*isegment)->getMax() < min ) {
        continue;
      }

      inserted = *isegment;
      (*isegment)->merge ( min );
      (*isegment)->merge ( max );

      list<RailSegment*>::iterator imerge = isegment;
      if ( imerge != _segments.end() ) imerge++;

      while ( imerge != _segments.end() ) {
        if ( (*imerge)->getMin() > (*isegment)->getMax() ) break;

        (*isegment)->merge ( (*imerge)->getMax() );
       
        delete *imerge;
        _segments.erase ( imerge );
        imerge = isegment;
        imerge++;
      }

      break;
    }

    if ( !inserted ) {
      inserted = new RailSegment ( _axis, width );
      inserted->merge ( min );
      inserted->merge ( max );
      _segments.insert ( _segments.end(), inserted );
    }
  }


  void  PowerRail::doLayout ( Cell* cell, Net* powerNet, Net* groundNet ) const
  {
  //const Layer* layer = DataBase::getDB()->getTechnology()->getLayer("METAL1");

    Net* railNet = (_type == Net::Type::POWER) ? powerNet : groundNet;

    list<RailSegment*>::const_iterator isegment = _segments.begin();
    for ( ; isegment != _segments.end() ; isegment++ )
      (*isegment)->doLayout ( cell, railNet, _layer, _direction );
  }


  bool  PowerRail::CompareByAxis::operator() ( const PowerRail* lhs, const PowerRail* rhs )
  { return lhs->getAxis() < rhs->getAxis(); }


// -------------------------------------------------------------------
// Class  :  "::PowerPlane".


  class PowerPlane {
    public:
                          PowerPlane ( const Layer* );
                         ~PowerPlane ();
      inline const Layer* getLayer   () const;
             size_t       find       ( DbU::Unit axis, Constant::Direction ) const;
             void         merge      ( Net::Type, Constant::Direction, DbU::Unit axis, DbU::Unit width, DbU::Unit min, DbU::Unit max );
             void         doLayout   ( Cell* cell, Net* powerNet, Net* groundNet ) const;
    private:
      const Layer*        _layer;
      vector<PowerRail*>  _hrails;
      vector<PowerRail*>  _vrails;
  };


  PowerPlane::PowerPlane ( const Layer* layer )
    : _layer (layer)
    , _hrails()
    , _vrails()
  { }


  PowerPlane::~PowerPlane ()
  {
    while ( !_hrails.empty() ) {
      delete _hrails.back();
      _hrails.pop_back ();
    }
    while ( !_vrails.empty() ) {
      delete _vrails.back();
      _vrails.pop_back ();
    }
  }


  size_t  PowerPlane::find ( DbU::Unit axis, Constant::Direction direction ) const
  {
    PowerRail  bound(Net::Type::GROUND,NULL,Constant::Horizontal,axis);

    if ( direction == Constant::Horizontal ) {
      vector<PowerRail*>::const_iterator it
        = lower_bound(_hrails.begin(),_hrails.end(),&bound,PowerRail::CompareByAxis());
      return it - _hrails.begin();
    }

    vector<PowerRail*>::const_iterator it
      = lower_bound(_vrails.begin(),_vrails.end(),&bound,PowerRail::CompareByAxis());
    return it - _vrails.begin();
  }


  void  PowerPlane::merge ( Net::Type           type
                          , Constant::Direction direction
                          , DbU::Unit           axis
                          , DbU::Unit           width
                          , DbU::Unit           rmin
                          , DbU::Unit           rmax
                          )
  {
    vector<PowerRail*>* rails;

    switch ( direction ) {
      case Constant::Vertical  : rails = &_vrails; break;
      case Constant::Horizontal:
      default:
        rails = &_hrails; break;
    }

    size_t i = find ( axis, direction );
    if ( ( i == rails->size() ) || ( (*rails)[i]->getAxis() != axis ) ) {
      PowerRail* rail = new PowerRail (type,_layer,direction,axis);
      rail->merge ( width, rmin, rmax );
      rails->push_back ( rail );
      sort ( rails->begin(), rails->end(), PowerRail::CompareByAxis() );
    } else {
      if ( (*rails)[i]->getType() != type ) {
        cerr << Error("Short between power rails at %d.",DbU::getValueString(axis).c_str()) << endl;
        return;
      }
      (*rails)[i]->merge ( width, rmin, rmax );
    }
  }


  void  PowerPlane::doLayout ( Cell* cell, Net* powerNet, Net* groundNet ) const
  {
    for ( size_t i=0 ; i<_hrails.size() ; i++ )
      _hrails[i]->doLayout ( cell, powerNet, groundNet );

    for ( size_t i=0 ; i<_vrails.size() ; i++ )
      _vrails[i]->doLayout ( cell, powerNet, groundNet );
  }


// -------------------------------------------------------------------
// Class  :  "::PowerRail".


  class PowerGrid {
    public:
                  PowerGrid ( Cell* );
                 ~PowerGrid ();
      PowerPlane* getPlane  ( const Layer* );
      void        merge     ( const Transformation&, Horizontal* );
      void        merge     ( const Transformation&, Vertical* );
      void        doLayout  () const;
    private:
      Cell*                          _cell;
      Net*                           _powerNet;
      Net*                           _groundNet;
      map<const Layer*,PowerPlane*>  _planes;
  };


  PowerGrid::PowerGrid ( Cell* cell )
    : _cell     (cell)
    , _powerNet (NULL)
    , _groundNet(NULL)
    , _planes   ()
  {
    forEach ( Net*, inet, _cell->getNets() ) {
      if ( (inet->getType() == Net::Type::POWER) ) {
        if ( !inet->isExternal() ) {
          cerr << Warning("Ignoring non-external power net %s."
                         ,getString(*inet).c_str()) << endl;
          continue;
        }
        _powerNet = *inet;
        break;
      }
    }
    if ( !_powerNet )
      cerr << Error("Missing POWER net in Cell %s.",getString(_cell->getName()).c_str()) << endl;

    forEach ( Net*, inet, _cell->getNets() ) {
      if ( inet->getType() == Net::Type::GROUND ) {
        if ( !inet->isExternal() ) {
          cerr << Warning("Ignoring non-external ground net %s."
                         ,getString(*inet).c_str()) << endl;
          continue;
        }
        _groundNet = *inet;
        break;
      }
    }
    if ( !_groundNet )
      cerr << Error("Missing GROUND net in Cell %s.",getString(_cell->getName()).c_str()) << endl;
  }


  PowerGrid::~PowerGrid ()
  {
    map<const Layer*,PowerPlane*>::iterator iplane = _planes.begin();
    for ( ; iplane != _planes.end() ; iplane++ )
      delete iplane->second;
  }


  PowerPlane* PowerGrid::getPlane ( const Layer* layer )
  {
    map<const Layer*,PowerPlane*>::iterator iplane = _planes.find(layer);
    if ( iplane != _planes.end() )
      return iplane->second;

    PowerPlane* plane = new PowerPlane ( layer );
    _planes.insert ( make_pair(layer,plane) );

    return plane;
  }


  void  PowerGrid::merge ( const Transformation& transformation, Horizontal* horizontal )
  {
    PowerPlane* plane = getPlane ( horizontal->getLayer() );

    Point source = horizontal->getSourcePosition();
    Point target = horizontal->getTargetPosition();
    transformation.applyOn ( source );
    transformation.applyOn ( target );

    if ( source.getX() > target.getX() ) swap ( source, target );
    plane->merge ( horizontal->getNet()->getType()
                 , Constant::Horizontal
                 , source.getY()
                 , horizontal->getWidth()
                 , source.getX()
                 , target.getX()
                 );
  }


  void  PowerGrid::merge ( const Transformation& transformation, Vertical* vertical )
  {
    PowerPlane* plane = getPlane ( vertical->getLayer() );

    Point source = vertical->getSourcePosition();
    Point target = vertical->getTargetPosition();
    transformation.applyOn ( source );
    transformation.applyOn ( target );

    if ( source.getY() > target.getY() ) swap ( source, target );
    plane->merge ( vertical->getNet()->getType()
                 , Constant::Vertical
                 , source.getX()
                 , vertical->getWidth()
                 , source.getY()
                 , target.getY()
                 );
  }


  void  PowerGrid::doLayout () const
  {
    map<const Layer*,PowerPlane*>::const_iterator iplane = _planes.begin ();
    for ( ; iplane != _planes.end() ; iplane++ )
      iplane->second->doLayout ( _cell, _powerNet, _groundNet );
  }


  void  copyUpPowerRails ( const Transformation& pathTransf
                         , Cell*                 instanceCell
                         , PowerGrid&            powerGrid
                         )
  {
    forEach ( Net*, inet, instanceCell->getNets() ) {
      switch ( inet->getType() ) {
        case Net::Type::POWER:
        case Net::Type::GROUND:
          break;
        default:
          continue;
      }
      
      forEach ( Component*, icomponent, inet->getComponents() ) {
        if ( !NetExternalComponents::isExternal(*icomponent) ) continue;

        Horizontal* horizontal = dynamic_cast<Horizontal*>(*icomponent);
        if ( horizontal )
          powerGrid.merge ( pathTransf, horizontal );
        else {
          Vertical* vertical = dynamic_cast<Vertical*>(*icomponent);
          if ( vertical )
            powerGrid.merge ( pathTransf, vertical );
        }
      }
    }

    forEach ( Instance*, iinstance, instanceCell->getInstances() ) {

      Transformation  instanceTransf = iinstance->getTransformation();
      pathTransf.applyOn ( instanceTransf );

      copyUpPowerRails ( instanceTransf, iinstance->getMasterCell(), powerGrid );
    }
  }


} // End of local namespace.


namespace Katabatic {


  using Hurricane::Point;
  using Hurricane::Horizontal;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;


  void  KatabaticEngine::makePowerRails ()
  {
    PowerGrid  powerGrid ( getCell() );

    Transformation topTransformation; // ID.
    copyUpPowerRails ( topTransformation, getCell(), powerGrid );

    powerGrid.doLayout ();
  }


}  // End of Katabatic namespace.
