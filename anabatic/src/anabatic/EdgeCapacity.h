// -*- mode: C++; explicit-buffer-name: "EdgeCapacity.h<anabatic>" -*-
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
// |  C++ Header  :  "./anabatic/EdgeCapacity.h"                     |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_EDGE_CAPACITY_H
#define  ANABATIC_EDGE_CAPACITY_H

#include <string>
#include <set>
#include "hurricane/Interval.h"
#include "anabatic/Constants.h"

namespace Anabatic {

  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Interval;
  class AnabaticEngine;


// -------------------------------------------------------------------
// Class  :  "EdgeCapacity".

  class EdgeCapacity {
    public:
      class Compare {
        public:
          inline bool  operator() ( const EdgeCapacity*, const EdgeCapacity* ) const; 
      };
    public:
                                          EdgeCapacity  ( AnabaticEngine*, Flags, Interval, size_t depth );
             inline       size_t          incref        ();
             inline       size_t          decref        ();
             inline       size_t          getref        () const;
             inline       AnabaticEngine* getAnabatic   () const;
             inline const Interval&       getSpan       () const;
             inline       int             getCapacity   () const;
             inline       int             getCapacity   ( size_t depth ) const;
                          void            forceCapacity ( unsigned int );
                          std::string     _getString    () const;
                          Record*         _getRecord    () const;
    private:                              
    private:
      AnabaticEngine*   _anabatic;
      size_t            _refCount;
      Flags             _flags;
      size_t            _depth;
      Interval          _span;
      std::vector<int>  _capacities;
  };


  inline       size_t          EdgeCapacity::incref      () { return ++_refCount; }
  inline       size_t          EdgeCapacity::decref      () { if (_refCount < 2) { delete this; return 0; } return --_refCount; }
  inline       size_t          EdgeCapacity::getref      () const { return _refCount; }
  inline       AnabaticEngine* EdgeCapacity::getAnabatic () const { return _anabatic; }
  inline const Interval&       EdgeCapacity::getSpan     () const { return _span; }
  inline       int             EdgeCapacity::getCapacity ( size_t depth ) const { return (depth<_depth) ? _capacities[depth] : 0; }

  inline int  EdgeCapacity::getCapacity () const
  {
    int full = 0;
    for ( size_t depth=0; depth<_depth ; ++depth ) full += _capacities[depth];
    return full;
  }

  inline bool  EdgeCapacity::Compare::operator() ( const EdgeCapacity* lhs, const EdgeCapacity* rhs ) const
  {
    if (    (lhs->_flags & Flags::Horizontal) and not (rhs->_flags & Flags::Horizontal)) return true;
    if (not (lhs->_flags & Flags::Horizontal) and     (rhs->_flags & Flags::Horizontal)) return false;

    DbU::Unit dmin = lhs->getSpan().getVMin() - rhs->getSpan().getVMin();
    if (dmin < 0) return true;
    if (dmin > 0) return false;
    return lhs->getSpan().getVMax() < rhs->getSpan().getVMax();
  }


  typedef  std::set<EdgeCapacity*,EdgeCapacity::Compare>  EdgeCapacityLut;


}  // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::EdgeCapacity);

#endif  // ANABATIC_EDGE_CAPACITY_H
