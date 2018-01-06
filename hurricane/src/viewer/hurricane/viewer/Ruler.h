// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Ruler.h"                                |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_RULER_H
#define  HURRICANE_RULER_H

#include  <memory>
#include  <set>

#include  "hurricane/Point.h"
#include  "hurricane/Box.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::Ruler".


  class Ruler {
    public:
                          Ruler          ( const Point& origin );
                          Ruler          ( const Point& origin, const Point& extremity );
      inline const Point& getOrigin      () const;
      inline const Point& getExtremity   () const;
             Point        getAngle       () const;
      inline Box          getBoundingBox () const;
      inline void         setExtremity   ( const Point& );
      inline bool         intersect      ( const Box& ) const;
    private:
      Point  _origin;
      Point  _extremity;
  };


// Inline Functions.
  inline const Point& Ruler::getOrigin      () const { return _origin; }
  inline const Point& Ruler::getExtremity   () const { return _extremity; }
  inline Box          Ruler::getBoundingBox () const { return Box(_origin,_extremity); }
  inline void         Ruler::setExtremity   ( const Point& extremity ) { _extremity = extremity; }
  inline bool         Ruler::intersect      ( const Box& area ) const { return area.intersect(Box(_origin,getAngle())); }

  typedef  std::set< std::shared_ptr<Ruler> >  RulerSet;

} // End of Hurricane namespace.


#endif  // __HURRICANE_RULER__
