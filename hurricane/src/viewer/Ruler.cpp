
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2016, All Rights Reserved
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


  Point  Ruler::getAngle () const
  {
    if ( abs(_extremity.getX() - _origin.getX()) >=  abs(_extremity.getY() - _origin.getY()) )
      return Point(_extremity.getX(),_origin.getY());
    return Point(_origin.getX(),_extremity.getY());
  }


}  // End of Hurricane namespace. 
