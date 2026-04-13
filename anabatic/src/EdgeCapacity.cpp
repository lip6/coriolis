// -*- mode: C++; explicit-buffer-name: "EdgeCapacity.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2018-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./EdgeCapacity.cpp"                            |
// +-----------------------------------------------------------------+


#include <sstream>
#include "anabatic/EdgeCapacity.h"
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using std::ostringstream;

  
  size_t  EdgeCapacity::_allocateds = 0;


  EdgeCapacity::EdgeCapacity ( AnabaticEngine* anabatic, Flags direction, Interval span, size_t depth )
    : _anabatic  (anabatic)
    , _refCount  (0)
    , _flags     (direction)
    , _depth     (depth+1)
    , _span      (span)
    , _capacities()
  {
    _allocateds++;
    int defcap = (_flags & Flags::InfiniteCapacity) ? 100 : 0;

    _capacities.reserve( _depth );
    for ( size_t i=0 ; i<_depth ; ++i ) _capacities.push_back( defcap );

    if (_flags & (Flags::NullCapacity|Flags::InfiniteCapacity)) return;

    Box           ab = _anabatic->getCell()->getAbutmentBox();
    RoutingGauge* rg = _anabatic->getConfiguration()->getRoutingGauge();

    span = _span;
    span.inflate( 0, -1 );
    if (span.isEmpty()) return;

    const vector<RoutingLayerGauge*>& layerGauges = rg->getLayerGauges();
    for ( size_t depth=0 ; depth < _depth ; ++depth ) {
      if (layerGauges[depth]->getType() != Constant::Default) continue;

      if (_flags & Flags::Horizontal) {
        if (layerGauges[depth]->getDirection() != Constant::Horizontal) continue;
        _capacities[depth] = layerGauges[depth]->getTrackNumber( span.getVMin() - ab.getYMin()
                                                               , span.getVMax() - ab.getYMin() );
      //cdebug_log(110,0) << "Horizontal edge capacity:" << _capacities[depth] << endl;
      }

      if (_flags & Flags::Vertical) {
        if (layerGauges[depth]->getDirection() != Constant::Vertical) continue;
        _capacities[depth] = layerGauges[depth]->getTrackNumber( span.getVMin() - ab.getXMin()
                                                               , span.getVMax() - ab.getXMin() );
      //cdebug_log(110,0) << "Vertical edge capacity:" << _capacities[depth] << endl;
      }
    }
    // std::cerr << "EdgeCapacity() "
    //           << " capacity=" << getCapacity()
    //           << " " << (void*)this << std::endl;
  }


  EdgeCapacity::EdgeCapacity ( const EdgeCapacity& other )
    : _anabatic  (other._anabatic)
    , _refCount  (0)
    , _flags     (other._flags | Flags::UniqueCapacity)
    , _depth     (other._depth)
    , _span      (other._span)
    , _capacities()
  {
    _allocateds++;
    _capacities.reserve( _depth );
    for ( size_t i=0 ; i<_depth ; ++i ) _capacities.push_back( other._capacities[i] );
  }

  
  EdgeCapacity::~EdgeCapacity ()
  {
    _allocateds--;
  }


  void  EdgeCapacity::forceCapacity ( unsigned int capacity )
  {
          bool                        forced      = false;
          RoutingGauge*               rg          = _anabatic->getConfiguration()->getRoutingGauge();
    const vector<RoutingLayerGauge*>& layerGauges = rg->getLayerGauges();

    for ( size_t i=0 ; i<_depth ; ++i ) {
      if (layerGauges[i]->getType() != Constant::Default) continue;
      if ( (_flags & Flags::Horizontal) xor layerGauges[i]->isHorizontal() ) continue;
      if (forced) _capacities[i] = 0;
      else {
        _capacities[i] = capacity;
        forced = true;
      }
    }
  }


  int  EdgeCapacity::decreaseCapacity ( int delta, size_t depth )
  {
    if (depth >= _capacities.size()) return 0;
    if (delta > _capacities[depth]) {
      int remains = delta - _capacities[depth];
      _capacities[depth] = 0;
      return remains;
    }
    _capacities[depth] -= delta;
    return 0;
  }


  string  EdgeCapacity::_getString () const
  {
    ostringstream os;
    os << "<EdgeCapacity ";
    if      (_flags & Flags::Horizontal) os << "Horizontal ";
    else if (_flags & Flags::Vertical  ) os << "Vertical ";
    else                                 os << "Unknown ";
    os <<  "["  << DbU::getValueString(_span.getVMin())
       << " : " << DbU::getValueString(_span.getVMax())
       << "] "  << getCapacity()
       << " refs:" << _refCount
       << ">";
    return os.str();
  }


  Record* EdgeCapacity::_getRecord () const
  {
    Record* record = new Record( getString(this) );
    record->add( getSlot( "_anabatic",  _anabatic ) );
    record->add( getSlot( "_refCount",  _refCount ) );
    record->add( getSlot( "_flags"   , &_flags    ) );
    record->add( getSlot( "_span"    , &_span     ) );

    RoutingGauge* rg = getAnabatic()->getConfiguration()->getRoutingGauge();
    for ( size_t depth=0 ; depth<_depth ; ++depth ) {
      ostringstream s;
      const Layer* layer = rg->getRoutingLayer(depth);
      s << "_capacities[" << depth << ":" << ((layer) ? layer->getName() : "None") << "]";
      record->add( getSlot ( s.str(), _capacities[depth] ) );
    }
    return record;
  }


}  // Anabatic namespace.
