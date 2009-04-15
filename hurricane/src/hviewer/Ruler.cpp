
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
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Ruler.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/viewer/Ruler.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::Ruler".


  Ruler::Ruler ( const Point& origin )
    : _origin   (origin)
    , _extremity(origin)
  { }


  Ruler::Ruler ( const Point& origin, const Point& extremity )
    : _origin   (origin)
    , _extremity(extremity)
  { }


  bool  Ruler::intersect ( const Box& area ) const
  {
    Point angle = getAngle ();
    return area.intersect(Box(_origin,angle)) || area.intersect(Box(angle,_extremity));
  }


}  // End of Hurricane namespace. 
