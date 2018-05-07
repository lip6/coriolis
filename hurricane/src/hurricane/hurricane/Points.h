// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Points.h"                          |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_POINTS_H
#define  HURRICANE_POINTS_H

#include "hurricane/Collection.h"
#include "hurricane/Point.h"


namespace Hurricane {

  typedef GenericCollection<Point> Points;
  typedef GenericLocator   <Point> PointLocator;
  typedef GenericFilter    <Point> PointFilter;
  typedef Filter           <Point> PointHF;
  typedef Locator          <Point> PointHL;
  typedef Collection       <Point> PointHC;


#define for_each_point(point, points)          \
{                                              \
  PointLocator _locator = points.getLocator(); \
  while (_locator.isValid()) {                 \
    Point point = _locator.getElement();       \
    _locator.progress();


} // Hurricane namespace.

#endif // HURRICANE_POINTS_H
